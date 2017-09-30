/*
    System UI Callback

    System Callback for UI Module.

    @file       SysUICB.c
    @ingroup    mIPRJSYS
    @note       None

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
//#include "AppCommon.h"
//#include "UICommon.h"
////////////////////////////////////////////////////////////////////////////////

#include "CoordTs.h"
#include "UIResource.h"

#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIGraphics.h"
#include "UIDisplay.h"
#include "UIBuffer.h"

//View Plugin
#include "UIView.h"
#include "PhotoView.h"
#include "MovieView.h"
#include "PlaybackView.h"

//---------------------SysUICB Debug Definition -----------------------------
//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysUICB
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//---------------------SysUICB Global Variables -----------------------------
extern VControl* gUxCtrlTypeList[];
extern VControl* gAppCtrlTypeList[];

//---------------------SysUICB Prototype Declaration  -----------------------
//---------------------SysUICB Public API  ----------------------------------
//---------------------SysUICB Private API ----------------------------------

///////////////////////////////////////////////////////////////////////////////

#define MYVIEW_COUNT    13
VIEW_TABLE MyViewList[MYVIEW_COUNT]=
{
//VDO1
{UI_SHOW_PREVIEW, LAYER_VDO1, View_Preview}, //VDO preview
{UI_SHOW_PLAYBACK, LAYER_VDO1, View_Playback}, //VDO playback
//VDO2
{UI_SHOW_QUICKVIEW, LAYER_VDO2, View_Quickview}, //VDO quickview
{UI_SHOW_PANL2RVIEW, LAYER_VDO2, View_Guideview_L},//VDO guideview of pancapture (L2R)
{UI_SHOW_PANR2LVIEW, LAYER_VDO2, View_Guideview_R}, //VDO guideview of pancapture (R2L)
{UI_SHOW_PHOTOFRAME, LAYER_VDO2, View_Photoframe}, //VDO Photo frame preview
{UI_SHOW_PS_VIEW, LAYER_VDO2, View_CS_Quickview}, //VDO PS previw
{UI_SHOW_CS_VIEW, LAYER_VDO2, View_CS_Quickview}, //VDO CS previw
{UI_SHOW_BACKGND, LAYER_VDO2, View_Background}, //VDO backgound
{UI_SHOW_BACKGND_CK, LAYER_VDO2, View_Background_CK}, //VDO backgound with ColorKey (by VDO_TRANSPARENT_COLOR)
//OSD1
{UI_SHOW_WINDOW, LAYER_OSD1, View_Window}, //OSD UI window
//OSD2
{UI_SHOW_INFO, LAYER_OSD2, View_Info}, //OSD face detection frame
{0, 0} //END of table
};


/////////////////////////////////////////////////////////////////////////
//
//  Customize Display Flow
//
/////////////////////////////////////////////////////////////////////////

//font table
FONT* UIFontTable[2]=
{
    (FONT*)gDemoKit_Font,
    NULL
};

_ALIGNED(4) static UINT8 m_TsOsdToVdo[COORDTS_REQ_MEM_SIZE]={0};
COORDTS_HANDLE g_hTsOsdToVdo = NULL;
/*
typedef struct _GFX_INIT
{
    UINT32 uiBufAddr;
    UINT32 uiBufSize;
    //////////////////////////
    UINT32 uiImageCount;
    UINT32 uiPaletteCount;
    UINT32 uiFontCount;
    UINT32 uiStringCount;
    const IMAGE_TABLE*(*pImageTable)(UINT32 id); //id = image style
    const PALETTE_ITEM*(*pPaletteTable)(UINT32 id); //id = color style
    const FONT*(*pFontTable)(UINT32 id); //id = font style
    const STRING_TABLE*(*pStringTable)(UINT32 id); //id = string language
}
GFX_INIT;
*/

void UI_GfxInit(void)
{
    UINT32              uiPoolAddr;

    //Initial all states, all buffers
    DBG_FUNC_BEGIN("\r\n");

    uiPoolAddr = OS_GetMempoolAddr(POOL_ID_GFX_TEMP);

    ////////////////////////////////////////////////////////////////

    GxGfx_Config(CFG_STRING_BUF_SIZE,256);
    //Init Gfx
    GxGfx_Init((UINT32*)uiPoolAddr,POOL_SIZE_GFX_TEMP);       //initial Graphics

    //set default shape, text, image state for GxGfx
    GxGfx_SetAllDefault();
    //set custom image state for GxGfx
    GxGfx_SetImageStroke(ROP_KEY,IMAGEPARAM_DEFAULT);
    GxGfx_SetImageColor(IMAGEPALETTE_DEFAULT,IMAGEPARAM_DEFAULT);
    #if(IPCAM_FUNC==ENABLE)
    GxGfx_SetImageTable(0); //IPCAM no built-in image
    #else
    GxGfx_SetImageTable((const IMAGE_TABLE*)gDemoKit_Image);
    #endif
    //set custom text state for GxGfx
    GxGfx_SetTextColor(TEXTFORECOLOR1_DEFAULT, TEXTFORECOLOR2_DEFAULT, TEXTFORECOLOR3_DEFAULT);
    GxGfx_SetTextStroke((const FONT*)gDemoKit_Font, FONTSTYLE_NORMAL, SCALE_1X);
    #if(IPCAM_FUNC==ENABLE)
    GxGfx_SetStringTable(0); //IPCAM no built-in string
    #else
    GxGfx_SetStringTable((const STRING_TABLE*)Get_LanguageTable());
    #endif
}

extern UINT32 UICTRL_DRW_SEM_ID;

#if (DISPLAY_OSD_FMT == DISP_PXLFMT_RGBA5658_PK)

PALETTE_ITEM gDemoKit_Palette_RGBA5658[256];

#define COLOR_BGRA_GET_R(c)         (CVALUE)(((c) & 0x000000ff) >> 0) ///< get B channel value of RGBA color
#define COLOR_BGRA_GET_G(c)         (CVALUE)(((c) & 0x0000ff00) >> 8) ///< get G channel value of RGBA color
#define COLOR_BGRA_GET_B(c)         (CVALUE)(((c) & 0x00ff0000) >> 16) ///< get R channel value of RGBA color
#define COLOR_RGBA_GET_COLOR(c)     (COLOR_ITEM)(((c) & 0x00ffffff) >> 0) ///< get RGB channel value of RGBA color

UINT32 COLOR_BGRD_2_RGB565(UINT32 bgrd, UINT32 a)
{
    INT32 r,g,b;
    //INT32 y,u,v;
    UINT32 rgb565;
    b = COLOR_BGRA_GET_B(bgrd);
    g = COLOR_BGRA_GET_G(bgrd);
    r = COLOR_BGRA_GET_R(bgrd);
    rgb565 = COLOR_MAKE_RGB565(r,g,b,a);
    return rgb565;
}

void PALETTE_PREPARE_DATA(void)
{
    UINT32 i=0;
    for(i=0;i<256;i++)
        gDemoKit_Palette_RGBA5658[i] = COLOR_BGRD_2_RGB565(gDemoKit_Palette_Palette[i], 0xff);

    gDemoKit_Palette_RGBA5658[0] = COLOR_BGRD_2_RGB565(gDemoKit_Palette_Palette[0], 0x00);

    {
        UINT32 color = COLOR_RGB565_GET_COLOR(gDemoKit_Palette_RGBA5658[0]);
        UINT32 alpha = COLOR_RGB565_GET_A(gDemoKit_Palette_RGBA5658[0]);

        DBG_DUMP("cky rgba5658 = 0x%08x\r\n", gDemoKit_Palette_RGBA5658[0]);
        DBG_DUMP("cky c = 0x%04x\r\n", color);
        DBG_DUMP("cky a = 0x%04x\r\n", alpha);
    }
}

UINT32 ColorTable[2] = {(UINT32)gDemoKit_Palette_RGBA5658, (UINT32)gDemoKit_Palette_RGBA5658};
#endif

void UI_DispInit(void)
{
    COORDTS_CREATE Create={0};
    COORDTS_ORIGIN Origin={0};

    //DeviceSize = GxVideo_GetDeviceSize(DOUT1); //Get LCD size

    //Initial all states, all buffers
    DBG_FUNC_BEGIN("\r\n");

    UI_RegisterBuf();

    ////////////////////////////////////////////////////////////////
    //Init Coordinate Translate

    Create.uiApiVer = COORDTS_API_VERSION;
    Create.pRegBuf = m_TsOsdToVdo;
    Create.uiRegBufSize = sizeof(m_TsOsdToVdo);
    g_hTsOsdToVdo = CoordTs_Create(&Create);

    Origin.hHandle=g_hTsOsdToVdo;
    Origin.SizeOrigin.w = TOOL_LAYOUT_W;
    Origin.SizeOrigin.h = TOOL_LAYOUT_H;
    CoordTs_SetOrigin(&Origin);
    ////////////////////////////////////////////////////////////////
    //Init UI show

    UI_SetLayoutSize(TOOL_LAYOUT_W, TOOL_LAYOUT_H);

    //Init UI show resource
    #if (DISPLAY_OSD_FMT == DISP_PXLFMT_RGBA8888_PK)
    UI_SetDisplayPalette(LAYER_OSD1,0,0,0);
    #elif (DISPLAY_OSD_FMT == DISP_PXLFMT_RGBA5658_PK)
    UI_SetDisplayPalette(LAYER_OSD1,0,0,0);
    #elif (DISPLAY_OSD_FMT == DISP_PXLFMT_INDEX8)
    UI_SetDisplayPalette(LAYER_OSD1,0,256,gDemoKit_Palette_Palette);
    #else //(DISPLAY_OSD_FMT == DISP_PXLFMT_INDEX4)
    UI_SetDisplayPalette(LAYER_OSD1,0,16,gDemoKit_Palette_Palette);
    #endif
    #if (DISPLAY_OSD_FMT == DISP_PXLFMT_RGBA8888_PK)
    UI_SetDisplayPalette(LAYER_OSD2,0,0,0);
    #elif (DISPLAY_OSD_FMT == DISP_PXLFMT_RGBA5658_PK)
    UI_SetDisplayPalette(LAYER_OSD2,0,0,0);
    #elif (DISPLAY_OSD_FMT == DISP_PXLFMT_INDEX8)
    UI_SetDisplayPalette(LAYER_OSD2,0,256,gDemoKit_Palette_Palette);
    #else //(DISPLAY_OSD_FMT == DISP_PXLFMT_INDEX4)
    UI_SetDisplayPalette(LAYER_OSD2,0,16,gDemoKit_Palette_Palette);
    #endif
    //Init UI show view
    UI_RegisterView(MYVIEW_COUNT, (VIEW_TABLE*)MyViewList);

    UI_Show(UI_SHOW_WINDOW, FALSE);
    UI_Show(UI_SHOW_INFO, FALSE);

    //clear screen
    UI_CleanDisplay(FALSE);
}

//Init renderer for UIControl show process
void UI_ControlShowInit(void)
{
    DBG_FUNC_BEGIN("\r\n");

    //set palette rendering-time switch table for Ux_Redraw()
    //Init UI show resource
    #if (DISPLAY_OSD_FMT == DISP_PXLFMT_RGBA8888_PK)
    PALETTE_PREPARE_DATA();
    Ux_SetPaletteTable((MAPPING_ITEM**)ColorTable);
    #elif (DISPLAY_OSD_FMT == DISP_PXLFMT_RGBA5658_PK)
    PALETTE_PREPARE_DATA();
    Ux_SetPaletteTable((MAPPING_ITEM**)ColorTable);
    #elif (DISPLAY_OSD_FMT == DISP_PXLFMT_INDEX8)
    Ux_SetPaletteTable(0);
    #else //(DISPLAY_OSD_FMT == DISP_PXLFMT_INDEX4)
    Ux_SetPaletteTable(0);
    #endif
    #if (DISPLAY_OSD_FMT == DISP_PXLFMT_RGBA8888_PK)
    PALETTE_PREPARE_DATA();
    Ux_SetPaletteTable((MAPPING_ITEM**)ColorTable);
    #elif (DISPLAY_OSD_FMT == DISP_PXLFMT_RGBA5658_PK)
    PALETTE_PREPARE_DATA();
    Ux_SetPaletteTable((MAPPING_ITEM**)ColorTable);
    Ux_SetGlobalColorKey(TRUE,COLOR_RGB565_GET_COLOR(gDemoKit_Palette_RGBA5658[0]));
    //Ux_SetGlobalColorKey(TRUE,0x00005EF0);
    #elif (DISPLAY_OSD_FMT == DISP_PXLFMT_INDEX8)
    Ux_SetPaletteTable(0);
    #else //(DISPLAY_OSD_FMT == DISP_PXLFMT_INDEX4)
    Ux_SetPaletteTable(0);
    #endif
    //set colormap rendering-time switch table for Ux_Redraw()
    Ux_SetColorMapTable(0);
    //set font switch rendering-time table for Ux_Redraw()
    Ux_SetFontTable(UIFontTable);

    //Init UI framework render object
    Ux_SetRender(&demoRender);
#if 0
    UI_SetDrawSemID(SEMID_UIGRAPHIC);
#endif
}

/////////////////////////////////////////////////////////////////////////////


void Load_ResInfo(void)
{
//jeahyen - EVB - begin
/*
//#Multi lang using Pstore
// check FW update Tag
if (*(volatile UINT8 *)(FW_UPDATE_OFFSET)  == FW_UPDATE_TAG)
{
    //#JPG using Pstore
    #if(_LANG_STORE_ == _PSTORE_)
    if(UIRes_ChkWriteLang())
    {
        Ux_SendEvent(&UISetupObjCtrl,NVTEVT_EXE_LANGUAGE,1,Get_LanguageIndex());
    }
    #endif
    #if (_JPG_STORE_ == _PSTORE_)
    if(UIRes_ChkWriteJPGIMG())
    {
    }
    UIRes_InitReadJPGIMG();
    #endif
}
*/
//jeahyen - EVB - end
}

/////////////////////////////////////////////////////////////////////////////

