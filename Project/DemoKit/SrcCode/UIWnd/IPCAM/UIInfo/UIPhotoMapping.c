#include "Type.h"
#include "ImageApp_CamPhoto.h"
#include "UIPhotoInfo.h"
#include "UIMode.h"
#include "UIFlow.h"

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIPhotoMapping
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass

#include "DebugModule.h"

typedef struct
{
    UINT32  uiWidth;
    UINT32  uiHeight;
    UINT32  uiImageRatio;
    const CHAR *String;
} PHOTO_SIZE_PARAM;

static PHOTO_SIZE_PARAM g_PhotoCapSizeTable[PHOTO_SIZE_ID_MAX+2] =
{
    {4032,  3024,   IMAGERATIO_4_3,     "12M"}, //12M
    {3648,  2736,   IMAGERATIO_4_3,     "10M"}, //10M
    {3264,  2448,   IMAGERATIO_4_3,     "8M"}, // 8M
    {2592,  1944,   IMAGERATIO_4_3,     "5M"}, // 5M
    {2048,  1536,   IMAGERATIO_4_3,     "3M"}, // 3M
    {640,   480,    IMAGERATIO_4_3,     "VGA"},   // VGA
    ///////////////////////////////////////////////////////////////////////////////////
    // +2 mode (only for internel test!!!)
    {1280,  960,    IMAGERATIO_4_3,     "1.3M"},  // 1.3M
    {1920,  1080,   IMAGERATIO_16_9,    "2MHD"}, // 2MHD
};

const CHAR *Get_SizeString(UINT32 uiIndex)
{
    if(uiIndex < PHOTO_SIZE_ID_MAX)
        return g_PhotoCapSizeTable[uiIndex].String;
    else
        return NULL;
}

UINT32 GetPhotoSizeWidth(UINT32 ubIndex)
{
    return g_PhotoCapSizeTable[ubIndex].uiWidth;
}

UINT32 GetPhotoSizeHeight(UINT32 ubIndex)
{
    return g_PhotoCapSizeTable[ubIndex].uiHeight;
}

UINT32 GetPhotoSizeRatio(UINT32 ubIndex)
{
    return g_PhotoCapSizeTable[ubIndex].uiImageRatio;
}

/*************************************************
    Dzoom string handling
**************************************************/
// DZoom string
#define MAX_DZOOM_STR_LEN               16
static CHAR g_cDZoomRatioStr[MAX_DZOOM_STR_LEN] = {0};

CHAR *Get_DZoomRatioString(void)
{
    UINT32 m, n;
    UINT32 DzoomIndx;

    DzoomIndx = DZOOM_IDX_GET();
    m = (DzoomIndx+10)/10;
    n = DzoomIndx%10;
    snprintf(g_cDZoomRatioStr, sizeof(g_cDZoomRatioStr), "x%ld.%ld", m, n);
    return g_cDZoomRatioStr;
}

///////////////////////////////////////////////////////////////////
UINT32 Get_MacroIndexFromValue(UINT32 value)
{
    switch(value)
    {
        case SEL_AFMODE_MACRO:
            return MACRO_ON;
        case SEL_AFMODE_AUTO:
            return MACRO_OFF;
        default:
            DBG_ERR("value=%d\r\n",value);
            return MACRO_OFF;
    }
}

UINT32 Get_MacroValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case MACRO_ON:
            return SEL_AFMODE_MACRO;
        case MACRO_OFF:
            return SEL_AFMODE_AUTO;
        default:
            DBG_ERR("uhIndex=%d\r\n",uhIndex);
            return SEL_AFMODE_AUTO;
    }

}

UINT32 Get_FlashValue(UINT32 uiIndex)
{
    switch (uiIndex)
    {
        case FLASH_AUTO:
            return SEL_FLASHMODE_AUTO;
        case FLASH_RED_EYE:
            return SEL_FLASHMODE_AUTOREDEYE;
        case FLASH_OFF:
            return SEL_FLASHMODE_FORCEOFF;
        case FLASH_ON:
            return SEL_FLASHMODE_FORCEON;
        default:
            DBG_ERR("uiIndex=%d\r\n",uiIndex);
            return 0;
    }
}

UINT32 Get_FlashIndexFromValue(UINT32 value)
{
    switch(value)
    {
        case SEL_FLASHMODE_FORCEON:
            return FLASH_ON;
        case SEL_FLASHMODE_AUTO:
            return FLASH_AUTO;
        case SEL_FLASHMODE_AUTOREDEYE:
            return FLASH_RED_EYE;
        case  SEL_FLASHMODE_FORCEOFF:
            return FLASH_OFF;
        default:
            DBG_ERR("value=%d\r\n",value);
            return FLASH_OFF;
    }
}

UINT32 Get_EVValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case EV_N20:
            return SEL_AEEV_N2;
        case EV_N16:
            return SEL_AEEV_N5_3;
        case EV_N13:
            return SEL_AEEV_N4_3;
        case EV_N10:
            return SEL_AEEV_N1;
        case EV_N06:
            return SEL_AEEV_N2_3;
        case EV_N03:
            return SEL_AEEV_N1_3;
        case EV_00:
            return SEL_AEEV_00;
        case EV_P03:
            return SEL_AEEV_P1_3;
        case EV_P06:
            return SEL_AEEV_P2_3;
        case EV_P10:
            return SEL_AEEV_P1;
        case EV_P13:
            return SEL_AEEV_P4_3;
        case EV_P16:
            return SEL_AEEV_P5_3;
        case EV_P20:
            return SEL_AEEV_P2;
        default:
            DBG_ERR("uhIndex=%d\r\n",uhIndex);
            return SEL_AEEV_00;
    }
}

UINT32 Get_EVIndexFromValue(UINT32 value)
{
    switch(value)
    {
        case SEL_AEEV_N2:
            return EV_N20;
        case SEL_AEEV_N5_3:
            return EV_N16;
        case SEL_AEEV_N4_3:
            return EV_N13;
        case SEL_AEEV_N1:
            return EV_N10;
        case SEL_AEEV_N2_3:
            return EV_N06;
        case SEL_AEEV_N1_3:
            return EV_N03;
        case SEL_AEEV_00:
            return EV_00;
        case SEL_AEEV_P1_3:
            return EV_P03;
        case SEL_AEEV_P2_3:
            return EV_P06;
        case SEL_AEEV_P1:
            return EV_P10;
        case SEL_AEEV_P4_3:
            return EV_P13;
        case SEL_AEEV_P5_3:
            return EV_P16;
        case SEL_AEEV_P2:
            return EV_P20;
        default:
            DBG_ERR("value=%d\r\n",value);
            return EV_00;

    }
}

UINT32 Get_QualityValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case QUALITY_SUPER:
            return 95;
        case QUALITY_FINE:
            return 87;
        case QUALITY_NORMAL:
            return 80;
        default:
            DBG_ERR("uhIndex=%d\r\n",uhIndex);
            return 80;
    }

}


UINT32 Get_WBValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case WB_AUTO:
            return SEL_WBMODE_AUTO;
        case WB_DAYLIGHT:
            return SEL_WBMODE_DAYLIGHT;
        case WB_CLOUDY:
            return SEL_WBMODE_CLOUDY;
        case WB_TUNGSTEN:
            return SEL_WBMODE_TUNGSTEN;
        case WB_FLUORESCENT:
            return SEL_WBMODE_FLUORESCENT1;
        case WB_MANUAL:
            return SEL_WBMODE_MGAIN;
        default:
            DBG_ERR("uhIndex=%d\r\n",uhIndex);
            return SEL_WBMODE_AUTO;
    }
}

UINT32 Get_WBIndexFromValue(UINT32 value)
{
    switch(value)
    {
        case SEL_WBMODE_AUTO:
            return WB_AUTO;
        case SEL_WBMODE_DAYLIGHT:
            return WB_DAYLIGHT;
        case SEL_WBMODE_CLOUDY:
            return WB_CLOUDY;
        case SEL_WBMODE_FLUORESCENT1:
            return WB_FLUORESCENT;
        default:
            DBG_ERR("value=%d\r\n",value);
            return WB_AUTO;
    }
}

UINT32 Get_ColorValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case COLOR_EFFECT_STANDARD:
            return SEL_IMAGEEFFECT_OFF;
        case COLOR_EFFECT_MONOCHROME:
            return SEL_IMAGEEFFECT_BW;
        case COLOR_EFFECT_SEPIA:
            return SEL_IMAGEEFFECT_SEPIA;
        default:
            DBG_ERR("uhIndex=%d\r\n",uhIndex);
            return SEL_IMAGEEFFECT_OFF;
    }
}

UINT32 Get_ISOValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case ISO_AUTO:
            return SEL_ISOINDEX_AUTO;
        case ISO_100:
            return SEL_ISOINDEX_100;
        case ISO_200:
            return SEL_ISOINDEX_200;
        case ISO_400:
            return SEL_ISOINDEX_400;
        case ISO_800:
            return SEL_ISOINDEX_800;
        case ISO_1600:
            return SEL_ISOINDEX_1600;

        default:
            DBG_ERR("uhIndex=%d\r\n",uhIndex);
            return SEL_ISOINDEX_AUTO;
    }
}

UINT32 Get_SceneModeValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case SCENE_LANDSCAPE:
            return SEL_SCENEMODE_LANDSCAPE;

        case SCENE_PORTRAIT:
            return SEL_SCENEMODE_POTRAIT;

        case SCENE_AUTO:
            return SEL_SCENEMODE_AUTO;

        default:
            DBG_ERR("=%d\r\n",uhIndex);
            return SEL_SCENEMODE_AUTO;
    }
}
UINT32 Get_SceneModeIndexFromValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case SEL_SCENEMODE_LANDSCAPE:
            return SCENE_LANDSCAPE;
        case SEL_SCENEMODE_POTRAIT:
            return SCENE_PORTRAIT;
        case SEL_SCENEMODE_AUTO:
            return SCENE_AUTO;
        default:
            DBG_ERR("=%d\r\n",uhIndex);
            return SCENE_AUTO;
    }
}


UINT32 Get_MeteringValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case METERING_SPOT:
            return SEL_AEMODE_SPOT;
        case METERING_CENTER:
            return SEL_AEMODE_CENTERWEIGHTED;
        case METERING_AIAE:
            return SEL_AEMODE_EVALUATIVE;
        default:
            return SEL_AEMODE_EVALUATIVE;

    }
}


UINT32 Get_AFWindowIndexFromValue(UINT32 value)
{
    switch(value)
    {
        case SEL_AFWINDOW_CENTER:
            return AFWINDOW_SINGLE;
        case SEL_AFWINDOW_AUTO:
            return AFWINDOW_MULTI;
        default:
            DBG_ERR("value=%d\r\n",value);
            return AFWINDOW_SINGLE;
    }
}
UINT32 Get_AFWindowValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case AFWINDOW_SINGLE:
            return SEL_AFWINDOW_CENTER;
        case AFWINDOW_MULTI:
            return SEL_AFWINDOW_AUTO;
        default:
            DBG_ERR("uhIndex=%d\r\n",uhIndex);
            return AFWINDOW_SINGLE;
    }
}

UINT32 Get_FDImageRatioValue(UINT32 value)
{
    switch (value)
    {
        case IMAGERATIO_4_3:
             return FD_IMAGERATIO_4_3;
        case IMAGERATIO_3_2:
             return FD_IMAGERATIO_3_2;
        case IMAGERATIO_16_9:
             return FD_IMAGERATIO_16_9;
        default:
            DBG_ERR("value=%d\r\n",value);
            return FD_IMAGERATIO_4_3;
    }
}

UINT32 Get_DatePrintValue(UINT32 value)
{
    switch (value)
    {
        case DATE_STAMP_OFF:
             return SEL_DATASTAMP_OFF;
        case DATE_STAMP_DATE:
        case DATE_STAMP_DATE_TIME:
             return SEL_DATASTAMP_ON;
        default:
            DBG_ERR("value=%d\r\n",value);
            return SEL_DATASTAMP_OFF;
    }
}

UINT32 Get_SharpnessValue(UINT32 value)
{
    switch (value)
    {
        case SHARPNESS_SHARP:
             return SEL_SHARPNESS_P1;
        case SHARPNESS_NORMAL:
             return SEL_SHARPNESS_NORMAL;
        case SHARPNESS_SOFT:
             return SEL_SHARPNESS_N1;

        default:
            DBG_ERR("value=%d\r\n",value);
            return SEL_SHARPNESS_NORMAL;
    }
}

UINT32 Get_SaturationValue(UINT32 value)
{
    switch (value)
    {
        case SATURATION_HIGH:
             return SEL_SATURATION_P1;
        case SATURATION_NORMAL:
             return SEL_SATURATION_NORMAL;
        case SATURATION_LOW:
             return SEL_SATURATION_N1;
        default:
            DBG_ERR("value=%d\r\n",value);
            return SEL_SATURATION_NORMAL;
    }
}

UINT32 Get_ImageEffectValue(UINT32 value)
{

    switch (value)
    {
        case IMAGEEFFECT_SKETCH:
             return SEL_IMAGEEFFECT_SKETCH;

        case IMAGEEFFECT_FISHEYE:
             return SEL_IMAGEEFFECT_FISHEYE;

        case IMAGEEFFECT_COLORPENCIL:
             return SEL_IMAGEEFFECT_COLORPENCIL;

        case IMAGEEFFECT_ANTISHAKE:
             return SEL_IMAGEEFFECT_OFF;

        case IMAGEEFFECT_HDR:
             return SEL_IMAGEEFFECT_OFF;

        default:
             DBG_ERR("value=%d\r\n",value);
             return SEL_IMAGEEFFECT_OFF;
    }
}

UINT32 Get_SHDRValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case SHDR_ON:
            return SEL_SHDR_ON;
        case SHDR_OFF:
            return SEL_SHDR_OFF;
        default:
            DBG_ERR("uhIndex=%d\r\n",uhIndex);
            return SEL_SHDR_ON;
    }
}


UINT32 Get_WDRValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case WDR_ON:
            return SEL_WDR_ON;
        case WDR_OFF:
            return SEL_WDR_OFF;
        default:
            DBG_ERR("uhIndex=%d\r\n",uhIndex);
            return SEL_WDR_ON;
    }
}


UINT32 Get_AntishakeValue(UINT32 uhIndex)
{
    switch(uhIndex)
    {
        case ANTISHAKE_ON:
            return SEL_ANTISHAKE_ON;
        case ANTISHAKE_OFF:
            return SEL_ANTISHAKE_OFF;
        default:
            DBG_ERR("uhIndex=%d\r\n",uhIndex);
            return SEL_ANTISHAKE_OFF;
    }
}


