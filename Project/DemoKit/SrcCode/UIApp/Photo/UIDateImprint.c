////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "UIInfo.h"
#include "UIPhotoInfo.h"
#include "DateTimeInfo.h"
#include "datastamp.h"
#include "UIDateImprint.h"
#include "UIResource.h"
#include "Utility.h"
#include "FontToYuv.h"
#include "UIMode.h"
#include "GxImage.h"
#include "UIPhotoMapping.h"

#define  USE_DBG_STAMP DISABLE

#define DATE_COLOR_TRANSPARENT  0x00808000
#define DATE_COLOR_WHITE        0x008080FF
#define DATE_COLOR_BLACK        0x00818101
#define DATE_COLOR_ORANGED      0x00D4328A
#define DATE_COLOR_RED          0x00FF554E

typedef struct _DATE_IMPRINT_CTRL{
    IMG_CAP_DATASTAMP_INFO* pInfo;
    UINT32          MemAddr;
    UINT32          MemSize;
    PALETTE_ITEM    FontPalette[3];
    UINT32          PosX;
    UINT32          PosY;
    FONT_TO_YUV_IN  tYuvDesc;
    FONT_TO_YUV_OUT tYuvInfo;
    DS_STAMP_INFOR  StampInfor;
}tDATE_IMPRINT_CTRL;

static tDATE_IMPRINT_CTRL gDateImprintCtrl = {0};
static char gUiDateImprint_StrBuf[64] = {0};
static BOOL   UiDateImprint_GenYuvData(void);
static void   UiDateImprint_CalcPosition(UPOINT* pos);

void UiDateImprint_SetEnable(BOOL bEnable)
{
    if(bEnable)
    {
        DataStamp_Open();
    }
    else
    {
        DataStamp_Close();
    }
}

BOOL UiDateImprint_GenData(IMG_CAP_DATASTAMP_INFO* pInfo)
{
    tDATE_IMPRINT_CTRL* pCtrl = &gDateImprintCtrl;
    BOOL bGenYuv = FALSE;

    pCtrl->pInfo = pInfo;

    switch(pInfo->EventId)
    {
        case CAP_DS_EVENT_THUMB:
        case CAP_DS_EVENT_QV:
        case CAP_DS_EVENT_SCR:
        case CAP_DS_EVENT_PRI:
            bGenYuv = TRUE;
            break;
    }

    if(bGenYuv)
    {
        DS_STAMP_INFOR*  pStampInfor = &pCtrl->StampInfor;
        FONT_TO_YUV_OUT* pYuvInfo = &pCtrl->tYuvInfo;
        UPOINT           pos = {0};

        if(strlen(gUiDateImprint_StrBuf)==0)
        {
            debug_msg("UiDateImprint: strlen=0\r\n");
            return FALSE;
        }

        #if USE_DBG_STAMP
        Perf_Open();
        Perf_Mark();
        #endif

        pCtrl->MemAddr = OS_GetMempoolAddr(POOL_ID_DATEIMPRINT);
        pCtrl->MemSize = POOL_SIZE_DATEIMPRINT;

        //GenYuvData
        UiDateImprint_GenYuvData();

        //Calc Position
        UiDateImprint_CalcPosition(&pos);
        //dump pic just for debug
        #if 0
        GxImg_DumpBuf(&pYuvInfo->GenImg,TRUE);
        #endif
        pStampInfor->PosX = pos.x;
        pStampInfor->PosY = pos.y;
        pStampInfor->Img = pYuvInfo->GenImg;
        pStampInfor->ColorKey = COLOR_MAKE_YUVD(pYuvInfo->ColorKeyY,pYuvInfo->ColorKeyCb,pYuvInfo->ColorKeyCr);
        pStampInfor->StampWeight = 255;
        DataStamp_SetInfor(pStampInfor);

        #if USE_DBG_STAMP
        debug_msg("Time Use = %d ms, MemMax Use = %d bytes, MemMin Use=%d bytes\r\n",Perf_GetDuration()/1000,pYuvInfo->UsedMaxMemSize,pYuvInfo->UsedMemSize);
        Perf_Close();
        #endif
    }

    return TRUE;
}

void UiDateImprint_UpdateDate(void)
{
    DateTime_Load();

    switch(UI_GetData(FL_DATE_STAMP))
    {
        case DATEIMPRINT_DATE:
            strncpy(gUiDateImprint_StrBuf,DateTime_MakeYMD(),sizeof(gUiDateImprint_StrBuf)-1);
        break;

        case DATEIMPRINT_DATE_TIME:
            strncpy(gUiDateImprint_StrBuf,DateTime_MakeYMDHMS(),sizeof(gUiDateImprint_StrBuf)-1);
        break;

        default:
            gUiDateImprint_StrBuf[0]=0; //Empty String
    }
}

static BOOL UiDateImprint_GenYuvData(void)
{
    tDATE_IMPRINT_CTRL* pCtrl = &gDateImprintCtrl;
    FONT_TO_YUV_IN*  pIn = &pCtrl->tYuvDesc;
    FONT_TO_YUV_OUT* pOut = &pCtrl->tYuvInfo;
    UINT32 photo_w = pCtrl->pInfo->ImgInfo.Ch[0].Width;
    UINT32 photo_h = pCtrl->pInfo->ImgInfo.Ch[0].Height;
    //UINT32 font_h;
    //UINT32 stamp_scale;
    //UINT32 stamp_ratio_num; //numerator
    //UINT32 stamp_ratio_denom; //denominator
    //UINT32 ImgSize;

    pIn->MemAddr = pCtrl->MemAddr;
    pIn->MemSize = pCtrl->MemSize;
    pIn->pStr = gUiDateImprint_StrBuf;

    pIn->ciSolid = DATE_COLOR_RED;
    pIn->ciFrame = DATE_COLOR_TRANSPARENT;
    pIn->ciTransparet = DATE_COLOR_TRANSPARENT;

    pIn->Format = GX_IMAGE_PIXEL_FMT_YUV422_PACKED;
    pIn->bEnableSmooth= FALSE;
    //ImgSize = photo_w*photo_h;

    if(photo_w*photo_h <= 160*120)
    {
        //thumbnail Size
        pIn->pFont = (FONT*)gDemo_SmallFont;
        pIn->ScaleFactor = 65536/3; //0.33x
    }
    else if(photo_w*photo_h <= 640*480)
    {
        //VGA Size
        pIn->pFont = (FONT*)gDemo_SmallFont;
        pIn->ScaleFactor = 65536; //1.0x
    }
    else
    {
        UINT32 font_h,stamp_ratio;
        pIn->pFont = (FONT*)gDemo_BigFont;

        font_h = 190;
        stamp_ratio = 0x00000A00; //0.04x
        pIn->ScaleFactor = photo_h * stamp_ratio / font_h;
    }
    if (FontToYuv(pIn,pOut)!=E_OK)
        return FALSE;
    return TRUE;
}

static void UiDateImprint_CalcPosition(UPOINT* pos)
{
    tDATE_IMPRINT_CTRL* pCtrl = &gDateImprintCtrl;
    IMG_CAP_DATASTAMP_INFO* pInfo = pCtrl->pInfo;
    FONT_TO_YUV_OUT* pYuvInfo = &pCtrl->tYuvInfo;

    UINT32 photo_w = pInfo->ImgInfo.Ch[0].Width;
    UINT32 photo_h = pInfo->ImgInfo.Ch[0].Height;

    pos->x = ALIGN_ROUND_4(photo_w - pYuvInfo->GenImg.Width - pYuvInfo->GenImg.Height);
    pos->y = ALIGN_ROUND_4(photo_h - pYuvInfo->GenImg.Height - pYuvInfo->GenImg.Height);
}

void UiDateImprint_InitBuff(void)
{
    DS_STAMP_INFOR  stampInfo ={0};
    UINT32          genMaxw,genMaxh;

    genMaxw = GetPhotoSizeWidth(0)/2;
    genMaxh = GetPhotoSizeHeight(0) * 0.04;
    stampInfo.Img.LineOffset[0] = genMaxw;
    stampInfo.Img.LineOffset[1] = genMaxw;
    stampInfo.Img.Width = genMaxw;
    stampInfo.Img.Height = genMaxh;
    stampInfo.Img.PxlFmt = GX_IMAGE_PIXEL_FMT_YUV422;
    DataStamp_SetInfor(&stampInfo);
}
