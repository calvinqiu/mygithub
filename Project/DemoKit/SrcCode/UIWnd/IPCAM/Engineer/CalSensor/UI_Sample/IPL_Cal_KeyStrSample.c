#include "IPL_Cal_KeyStrSample_Int.h"
#include "UIFramework.h"

#if isDemokit
#include "UIGraphics.h"
#include "UIDisplay.h"
#include "UICommon.h"
#else
#include "GxDC.h"
#include "GxGfx.h"
#include "UIResource.h"

#define GxGfx_OSD 0
#endif
//#NT#2016/07/19#Silvia Wu -begin
//#NT# support usb&web style / move common funtion to Lib
#include "GxInput.h"
#include "KeyDef.h"
#include "Delay.h"
#include "GxVideo.h"
#include "GxDisplay.h"
#include "stdio.h"
#include "IQtoolAPI.h"
#include "IPL_CalApi.h"
#include "IPL_CalUSB.h"

#define __MODULE__ CALUI
//#define __DBGLVL__ 0        //OFF mode, show nothing
//#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

#define MaxLine 20
IQTOOL_SHOWLINE showline={0};
static UINT32 msg_cnt = 0;
CHAR str[32];
UINT32 buf_cnt = 0;
static IPOINT m_StringPos = {0};

static void Cal_WaitKey(UINT32 Key)
{
    #if isDemokit
    Input_SetKeyMask(KEY_PRESS, FLGKEY_KEY_MASK_NULL);

    while(1)
    {
        if(GxKey_GetData(GXKEY_NORMAL_KEY) & Key)
            break;
        else
            Delay_DelayMs(33);
    }
    //incase of ui to detect key pressed when key pressed too long.
    //but key still can't be pressed over 2 sec.
    Delay_DelayMs(2000);
    Input_SetKeyMask(KEY_PRESS, FLGKEY_KEY_MASK_DEFAULT);
    #endif
}

static CalColorMapPrj ColorMapTbl[UICOL_MAX] = {
    {UICOL_NONE,    CLRID_IDX_TRANSPART},
    {UICOL_RED,     CLRID_IDX_RED      },
    {UICOL_GREEN,   CLRID_IDX_GREEN    },
    {UICOL_BLUE,    CLRID_IDX_BLUE     },
    {UICOL_WHITE,   CLRID_IDX_WHITE    },
    {UICOL_BLACK,   CLRID_IDX_BLACK    },
    {UICOL_GRAY,    CLRID_IDX_BLACK50  },
    {UICOL_YELLOW,  CLRID_IDX_YELLOW   },
};

static void Cal_ShowString_Panel(CHAR *pStr, PURECT pRect, UINT32 StrColor, UINT32 BgColor)
{
    UINT32 ColorCnt = 0;
    UINT32 StrColorCnt = 0;
    UINT32 BgColorCnt = 0;

    // map calibration color (UICOL_XX) to project color (CLRID_IDX_XX)
    for (ColorCnt = 0; ColorCnt < UICOL_MAX; ColorCnt++)
    {
        if (ColorMapTbl[ColorCnt].CalColor == StrColor)
            StrColorCnt = ColorCnt;
        if (ColorMapTbl[ColorCnt].CalColor == BgColor)
            BgColorCnt = ColorCnt;
    }

    #if isDemokit
    DC**     pCalDCList;
    UI_SetDisplayDirty(TRUE); //set TRUE to force dirty current begin/end

    pCalDCList = (DC**)UI_BeginScreen();

    //GxGfx_SetTextColor(_OSD_INDEX_TRANSPART, _OSD_INDEX_TRANSPART, 0);
    GxGfx_FillRect(pCalDCList[GxGfx_OSD],pRect->x,pRect->y,pRect->x+pRect->w,pRect->y+pRect->h);
    GxGfx_SetShapeColor(ColorMapTbl[BgColorCnt].PrjIdx, ColorMapTbl[BgColorCnt].PrjIdx, 0);
    GxGfx_SetTextStroke((const FONT*)gDemoKit_Font, FONTSTYLE_NORMAL, SCALE_1X);
    GxGfx_SetTextColor(ColorMapTbl[StrColorCnt].PrjIdx, CLRID_IDX_BLACK50, 0);
    GxGfx_Text(pCalDCList[GxGfx_OSD], pRect->x, pRect->y, pStr);
    UI_EndScreen((UINT32)pCalDCList);
    #endif
}

static void AWB_UI(CalUICB UICB,CHAR* pstr,UINT32 ColorInfo)
{
    static CAL_UI_INIT_OBJ InitObj = {0};
    static BOOL bInit = FALSE;
    IPOINT StringPos = m_StringPos;

    if (!bInit)
    {
        if (CAL_TYPE_WEB == CalApi()->fpTypeEvent()->fpGetType())
        {
            InitObj.StringSize.w = 640;
            InitObj.StringSize.h = 1;
            InitObj.MaxShowLine = MaxLine;
        }
        else if (CAL_TYPE_USB == CalApi()->fpTypeEvent()->fpGetType())
        {
            InitObj.StringSize.h = 50;
        }
        else //if (CAL_TYPE_Normal == CalApi()->fpTypeEvent()->fpGetType())
        {
            ISIZE disp_size;
            disp_size = GxVideo_GetDeviceSize(DOUT1);
            InitObj.ScreenSize.w = disp_size.w;
            InitObj.ScreenSize.h = disp_size.h;
            InitObj.StringSize.w = InitObj.ScreenSize.w;
            InitObj.StringSize.h = 30;
            InitObj.SHOW_CB_FP = (CAL_UI_SHOW_FP)Cal_ShowString_Panel;
            InitObj.WAITKEY_CB_FP = (CAL_UI_WAITKEY_FP)Cal_WaitKey;
        }
        bInit= CalApi()->fpUIEvent()->fpInit(InitObj);
    }

    switch(UICB)
    {
        static UINT32 start_shift_msg_cnt = 0;
        // In the first line print string
        case UICB_START:
            StringPos.x = 0;
            StringPos.y = 0;
            if (pstr)
            {
                CalApi()->fpUIEvent()->fpShowLine(pstr, StringPos, ColorInfo);
                start_shift_msg_cnt = 1;
            }
            break;

        // wait any key
        case UICB_WAITKEY:
            if (pstr!= NULL)
            {
                buf_cnt = snprintf(NULL, 0, "waitkey");
                snprintf(pstr, buf_cnt+1, "waitkey");
            }
            CalApi()->fpUIEvent()->fpWaitkey (pstr, StringPos, ColorInfo, FLGKEY_KEY_MASK);
            break;

        // On the next line print string
        case UICB_PROCESSING:
            if (pstr)
            {
                StringPos.x = 0;
                if (msg_cnt == 0)// not msg or only UICB_START msg
                    StringPos.y = (msg_cnt+start_shift_msg_cnt)*(InitObj.StringSize.h);
                else
                    StringPos.y = (msg_cnt)*(InitObj.StringSize.h);

                CalApi()->fpUIEvent()->fpShowLine(pstr, StringPos, ColorInfo);
                msg_cnt++;
            }
            break;

        // clear all string
        case UICB_CLEAN:
            start_shift_msg_cnt = 0;
            msg_cnt = 0;
            CalApi()->fpUIEvent()->fpCleanScreen(ColorInfo);
            break;

        // In the last line of the print string
        case UICB_END:
            if (pstr)
            {
                StringPos.x = 0;
                StringPos.y = (InitObj.MaxShowLine-1)*(InitObj.StringSize.h);
                CalApi()->fpUIEvent()->fpShowLine(pstr, StringPos, ColorInfo);
            }
            break;

        // clean the last message
        case UICB_DELETE:
            StringPos.x = 0;
            StringPos.y = (msg_cnt+start_shift_msg_cnt)*(InitObj.StringSize.h);
            CalApi()->fpUIEvent()->fpDeleteLine(StringPos, ColorInfo);
            msg_cnt--;
            break;

        case UICB_NONE:
        default:
            break;
    }
}
//#NT#2016/07/19#Silvia Wu -end

static void ISO_UI(CalUICB UICB,CHAR* pstr,UINT32 StrColor)
{
    AWB_UI(UICB,pstr,StrColor);
}

static void ECS_UI(CalUICB UICB,CHAR* pstr,UINT32 StrColor)
{
    AWB_UI(UICB,pstr,StrColor);
}

static void DP_B_UI(CalUICB UICB,CHAR* pstr,UINT32 StrColor)
{
    AWB_UI(UICB,pstr,StrColor);
}

static void DP_W_UI(CalUICB UICB,CHAR* pstr,UINT32 StrColor)
{
    DP_B_UI(UICB,pstr,StrColor);
}

static void Null_UI(CalUICB UICB,CHAR* a,UINT32 StrColor)
{
    return;
}

static void LENSCEN_UI(CalUICB UICB,CHAR* pstr,UINT32 StrColor)
{
    AWB_UI(UICB,pstr,StrColor);
}

CAL_UICB_OBJ Cal_UI = {

    AWB_UI,
    ISO_UI,
    ECS_UI,
    Null_UI,
    DP_W_UI,
    DP_B_UI,
    LENSCEN_UI,
};



pCAL_UICB_OBJ CAL_UICB(void)
{
    return &Cal_UI;

}
