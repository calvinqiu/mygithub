#include "ImageApp_CamMovie.h"
#include "UIAppMoviePlay.h"
#include "UIMovieInfo.h"
#include "UIPhotoInfo.h" //for IMAGERATIO_XXX
#include "Audio.h"
#include "SysMain.h"
#include "UIMovieMapping.h"
#include "UIFlow.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIMovieMapping
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

UINT32 GetSingleIndex(UINT32 wifiIndex);
UINT32 GetWiFiIndex(UINT32 singleIndex);


typedef struct
{
    UINT32  uiWidth;
    UINT32  uiHeight;
    UINT32  uiVidFrameRate;
    UINT32  uiTargetBitrate;
    UINT32  uiDispAspectRatio;
    UINT32  uiImageRatio;
} MOVIE_SIZE_PARAM;

typedef struct
{
    MOVIE_REC_TYPE Type;
    MOVIE_SIZE_PARAM Front;
    MOVIE_SIZE_PARAM Behind;
} MOVIE_REC_ITEM;

//MOVIE_SIZE_TAG
static MOVIE_REC_ITEM g_MovieRecSizeTable[MOVIE_SIZE_ID_MAX] = {
    [MOVIE_SIZE_FRONT_2880x2160P24] = {
        MOVIE_REC_TYPE_FRONT,
        {2880,  2160,   24,     3000*1024,  VIDENC_DAR_16_9,    IMAGERATIO_16_9},
        {0},
    },

    [MOVIE_SIZE_FRONT_2560x1440P30] = {
        MOVIE_REC_TYPE_FRONT,
        {2560,  1440,   30,     2450*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        {0},
    },

    [MOVIE_SIZE_FRONT_2304x1296P30] = {
        MOVIE_REC_TYPE_FRONT,
        {2304,  1296,   30,     2450*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        {0},
    },

    [MOVIE_SIZE_FRONT_1920x1080P96] = {
        MOVIE_REC_TYPE_FRONT,
        {1920,  1080,   96,     2450*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        {0},
    },

    [MOVIE_SIZE_FRONT_1920x1080P60] = {
        MOVIE_REC_TYPE_FRONT,
        {1920,  1080,   60,     2450*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        {0},
    },

    [MOVIE_SIZE_FRONT_1920x1080P30] = {
        MOVIE_REC_TYPE_FRONT,
        {1920,  1080,   30,     1800*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        {0},
    },

    [MOVIE_SIZE_FRONT_1280x720P120] = {
        MOVIE_REC_TYPE_FRONT,
        {1280,  720,   120,     2450*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        {0},
    },

    [MOVIE_SIZE_FRONT_1280x720P60] = {
        MOVIE_REC_TYPE_FRONT,
        {1280,  720,   60,     1600*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        {0},
    },

    [MOVIE_SIZE_FRONT_1280x720P30] = {
        MOVIE_REC_TYPE_FRONT,
        {1280,  720,    30,     800*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        {0},
    },

    [MOVIE_SIZE_FRONT_848x480P30] = {
        MOVIE_REC_TYPE_FRONT,
        { 848,  480,    30,     500*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        {0},
    },

    [MOVIE_SIZE_FRONT_640x480P240] = {
        MOVIE_REC_TYPE_FRONT,
        { 640,  480,    240,    1600*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_4_3},
        {0},
    },

    [MOVIE_SIZE_FRONT_640x480P30] = {
        MOVIE_REC_TYPE_FRONT,
        { 640,  480,    30,     400*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_4_3},
        {0},
    },

    [MOVIE_SIZE_FRONT_320x240P30] = {
        MOVIE_REC_TYPE_FRONT,
        { 320,  240,    30,     200*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_4_3},
        {0},
    },

    [MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30] = {
        MOVIE_REC_TYPE_DUAL,
        {1920,  1080,   30,     1450*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        {1920,  1080,   30,     1450*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
    },

    [MOVIE_SIZE_CLONE_1920x1080P30_1920x1080P30] = {
        MOVIE_REC_TYPE_CLONE,
        {1920,  1080,   30,     1800*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        {1920,  1080,   30,     1000*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
    },

    [MOVIE_SIZE_CLONE_1920x1080P30_1280x720P30] = {
        MOVIE_REC_TYPE_CLONE,
        {1920,  1080,   30,     1800*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        {1280,  720,    30,     800*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
    },

    [MOVIE_SIZE_CLONE_2560x1440P30_848x480P30] = {
        MOVIE_REC_TYPE_CLONE,
        {2560,  1440,   30,     2450*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        {848,    480,   30,      350*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
    },

    [MOVIE_SIZE_CLONE_2304x1296P30_848x480P30] = {
        MOVIE_REC_TYPE_CLONE,
        {2304,  1296,   30,     2450*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        {848,    480,   30,      350*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
    },

    [MOVIE_SIZE_CLONE_1920x1080P60_848x480P30] = {
        MOVIE_REC_TYPE_CLONE,
        {1920,  1080,   60,     2450*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        { 848,  480,    30,      350*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
    },

    [MOVIE_SIZE_CLONE_1920x1080P60_640x360P30] = {
        MOVIE_REC_TYPE_CLONE,
        {1920,  1080,   60,    2450*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        { 640,   360,   30,     200*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
    },

    [MOVIE_SIZE_CLONE_1920x1080P30_848x480P30] = {
        MOVIE_REC_TYPE_CLONE,
        {1920, 1080,    30,     1800*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        { 848,  480,    30,     350*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
    },

    [MOVIE_SIZE_CLONE_2048x2048P30_480x480P30] = {
        MOVIE_REC_TYPE_CLONE,
        {2048, 2048,    30,     1800*1024,  VIDENC_DAR_DEFAULT, IMAGERATIO_1_1},
        { 480,  480,    30,     200*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_1_1},
    },

    [MOVIE_SIZE_CLONE_1280x720P30_1280x720P30] = {
        MOVIE_REC_TYPE_CLONE,
        {1280,  720,    30,     800*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        {1280,  720,    30,     800*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
    },

    [MOVIE_SIZE_CLONE_848x480P30_848x480P30] = {
        MOVIE_REC_TYPE_CLONE,
        { 848,  480,    30,     500*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
        { 848,  480,    30,     350*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_16_9},
    },

    [MOVIE_SIZE_CLONE_640x480P30_640x480P30] = {
        MOVIE_REC_TYPE_CLONE,
        { 640,  480,    30,     400*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_4_3},
        { 640,  480,    30,     266*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_4_3},
    },

    [MOVIE_SIZE_CLONE_320x240P30_320x240P30] = {
        MOVIE_REC_TYPE_CLONE,
        { 320,  240,    30,     200*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_4_3},
        { 320,  240,    30,     64*1024,   VIDENC_DAR_DEFAULT, IMAGERATIO_4_3},
    },
};

UINT32 GetMovieSizeWidth(UINT32 uiIndex)
{
    DBG_IND("%d\r\n", g_MovieRecSizeTable[uiIndex].Front.uiWidth);
    return g_MovieRecSizeTable[uiIndex].Front.uiWidth;
}
UINT32 GetMovieSizeHeight(UINT32 uiIndex)
{
    DBG_IND("%d\r\n", g_MovieRecSizeTable[uiIndex].Front.uiHeight);
    return g_MovieRecSizeTable[uiIndex].Front.uiHeight;
}
UINT32 GetMovieFrameRate(UINT32 uiIndex)
{
    DBG_IND("%d\r\n", g_MovieRecSizeTable[uiIndex].Front.uiVidFrameRate);
    return g_MovieRecSizeTable[uiIndex].Front.uiVidFrameRate;
}
UINT32 GetMovieTargetBitrate(UINT32 uiIndex, UINT32 uiQuality)
{
    DBG_IND("%d\r\n", g_MovieRecSizeTable[uiIndex].Front.uiTargetBitrate);
    return g_MovieRecSizeTable[uiIndex].Front.uiTargetBitrate;
}
void SetMovieTargetBitrate(UINT32 uiType, UINT32 uiIndex, UINT32 uiBitRate)
{
    DBG_IND("uiBitRate = %d\r\n",uiBitRate);
    if(uiType == 0)
    {
        g_MovieRecSizeTable[uiIndex].Behind.uiTargetBitrate = uiBitRate;
    }
    else
    {
        g_MovieRecSizeTable[uiIndex].Front.uiTargetBitrate = uiBitRate;
    }
}
UINT32 GetMovieDispAspectRatio(UINT32 uiIndex)
{
    DBG_IND("uiDispAspectRatio = %d\r\n",g_MovieRecSizeTable[uiIndex].Front.uiDispAspectRatio);
    return g_MovieRecSizeTable[uiIndex].Front.uiDispAspectRatio;
}

UINT32 GetMovieSizeRatio(UINT32 uiIndex)
{
    DBG_IND("uiImageRatio = %d\r\n",g_MovieRecSizeTable[uiIndex].Front.uiImageRatio);
    return g_MovieRecSizeTable[uiIndex].Front.uiImageRatio;
}

// 2-path image ratio
UINT32 GetMovieSizeRatio_2p(UINT32 uiPath, UINT32 uiIndex)
{
    if(uiPath == 0)
    {
        DBG_IND("uiImageRatio = %d\r\n",g_MovieRecSizeTable[uiIndex].Front.uiImageRatio);
        return g_MovieRecSizeTable[uiIndex].Front.uiImageRatio;
    }
    else
    {
        DBG_IND("uiImageRatio = %d\r\n",g_MovieRecSizeTable[uiIndex].Behind.uiImageRatio);
        return g_MovieRecSizeTable[uiIndex].Behind.uiImageRatio;
    }
}

// 2-path image source
UINT32 GetMovieSizeWidth_2p(UINT32 uiPath, UINT32 uiIndex)
{
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        uiIndex = GetWiFiIndex(uiIndex);
    }

    if(uiPath == 0)
    {
        DBG_IND("uiWidth[0] = %d\r\n",g_MovieRecSizeTable[uiIndex].Front.uiWidth);
        return g_MovieRecSizeTable[uiIndex].Front.uiWidth;
    }
    else
    {
        DBG_IND("uiWidth[1] = %d\r\n",g_MovieRecSizeTable[uiIndex].Behind.uiWidth);
        return g_MovieRecSizeTable[uiIndex].Behind.uiWidth;
    }
}
UINT32 GetMovieSizeHeight_2p(UINT32 uiPath, UINT32 uiIndex)
{
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        uiIndex = GetWiFiIndex(uiIndex);
    }

    if(uiPath == 0)
    {
        DBG_IND("uiHeight[0] = %d\r\n",g_MovieRecSizeTable[uiIndex].Front.uiHeight);
        return g_MovieRecSizeTable[uiIndex].Front.uiHeight;
    }
    else
    {
        DBG_IND("uiHeight[1] = %d\r\n",g_MovieRecSizeTable[uiIndex].Behind.uiHeight);
        return g_MovieRecSizeTable[uiIndex].Behind.uiHeight;
    }
}
UINT32 GetMovieFrameRate_2p(UINT32 uiPath, UINT32 uiIndex)
{
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        uiIndex = GetWiFiIndex(uiIndex);
    }

    if(uiPath == 0)
    {
        DBG_IND("uiVidFrameRate[0] = %d\r\n",g_MovieRecSizeTable[uiIndex].Front.uiVidFrameRate);
        return g_MovieRecSizeTable[uiIndex].Front.uiVidFrameRate;
    }
    else
    {
        DBG_IND("uiVidFrameRate[1] = %d\r\n",g_MovieRecSizeTable[uiIndex].Behind.uiVidFrameRate);
        return g_MovieRecSizeTable[uiIndex].Behind.uiVidFrameRate;
    }
}
UINT32 GetMovieTargetBitrate_2p(UINT32 uiPath, UINT32 uiIndex)
{
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        uiIndex = GetWiFiIndex(uiIndex);
    }

    if(uiPath == 0)
    {
        DBG_IND("uiTargetBitrate[0] = %d\r\n",g_MovieRecSizeTable[uiIndex].Front.uiTargetBitrate);
        return g_MovieRecSizeTable[uiIndex].Front.uiTargetBitrate;
    }
    else
    {
        DBG_IND("uiTargetBitrate[1] = %d\r\n",g_MovieRecSizeTable[uiIndex].Behind.uiTargetBitrate);
        return g_MovieRecSizeTable[uiIndex].Behind.uiTargetBitrate;
    }
}
UINT32 GetMovieDispAspectRatio_2p(UINT32 uiPath, UINT32 uiIndex)
{
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        uiIndex = GetWiFiIndex(uiIndex);
    }

    if(uiPath == 0)
    {
        DBG_IND("uiDispAspectRatio[0] = %d\r\n",g_MovieRecSizeTable[uiIndex].Front.uiDispAspectRatio);
        return g_MovieRecSizeTable[uiIndex].Front.uiDispAspectRatio;
    }
    else
    {
        DBG_IND("uiDispAspectRatio[1] = %d\r\n",g_MovieRecSizeTable[uiIndex].Behind.uiDispAspectRatio);
        return g_MovieRecSizeTable[uiIndex].Behind.uiDispAspectRatio;
    }
}

MOVIE_REC_TYPE GetMovieRecType_2p(UINT32 uiIndex)
{
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        uiIndex = GetWiFiIndex(uiIndex);
    }

    DBG_IND("%d RecType = %d\r\n",uiIndex,g_MovieRecSizeTable[uiIndex].Type);
    return g_MovieRecSizeTable[uiIndex].Type;
}

UINT32 GetMovieAudioVolumeValue(UINT32 uiIndex)
{
    UINT32 audVol = 0;
    //map UI setting to HW setting, refering to DrvSound_SetVol()
    if(uiIndex < MOVIE_AUDIO_VOL_SETTING_MAX)
    {
        audVol = 10*uiIndex;
    }
    return audVol;
}

UINT32 Get_MovieTimeLapseValue(UINT32 uiIndex)
{
   UINT32 uiTimeLapse = 1000;

        switch(SysGetFlag(FL_MOVIE_TIMELAPSE_REC))
        {
          default:
          case MOVIE_TIMELAPSEREC_1SEC:
            uiTimeLapse = 1000;
            break;
          case MOVIE_TIMELAPSEREC_5SEC:
            uiTimeLapse = 5000;
            break;
          case MOVIE_TIMELAPSEREC_10SEC:
            uiTimeLapse = 10000;
            break;
          case MOVIE_TIMELAPSEREC_30SEC:
            uiTimeLapse = 30000;
            break;
          case MOVIE_TIMELAPSEREC_1MIN:
            uiTimeLapse = 60000;
            break;
          case MOVIE_TIMELAPSEREC_5MIN:
            uiTimeLapse = 300000;
            break;
          case MOVIE_TIMELAPSEREC_10MIN:
            uiTimeLapse = 600000;
            break;
          case MOVIE_TIMELAPSEREC_30MIN:
            uiTimeLapse = 1800000;
            break;
          case MOVIE_TIMELAPSEREC_1HOUR:
            uiTimeLapse = 3600000;
            break;
          case MOVIE_TIMELAPSEREC_2HOUR:
            uiTimeLapse = 7200000;
            break;
          case MOVIE_TIMELAPSEREC_3HOUR:
            uiTimeLapse = 10800000;
            break;
          case MOVIE_TIMELAPSEREC_1DAY:
            uiTimeLapse = 86400000;
            break;
        }

     return uiTimeLapse;
}
//#NT#2015/03/18#Lily Kao -begin
//#NT#Add for UVAC to set parameters for MFK
UINT32 GetMovieTBRAspRatio(UINT32 width, UINT32 height, UINT32 fps, UINT32 *pTBR, UINT32 *pAspRatio)
{
    MOVIE_SIZE_PARAM *pMovieSizeParam = 0;
    UINT32 tbr = 1450*1024;
    UINT32 ratio = VIDENC_DAR_DEFAULT;
    UINT32 i = 0;
    DBG_IND(":w=%d,%d,fps=%d,pTBR=0x%x,pAspRatio=0x%x\r\n", width, height, fps, pTBR, pAspRatio);
    for (i = 0; i < MOVIE_SIZE_ID_MAX; i++)
    {
        pMovieSizeParam = &g_MovieRecSizeTable[i].Front;
        if ((pMovieSizeParam->uiWidth == width) && (pMovieSizeParam->uiHeight == height)&& (pMovieSizeParam->uiVidFrameRate == fps))
        {
            tbr = pMovieSizeParam->uiTargetBitrate;
            ratio = pMovieSizeParam->uiDispAspectRatio;
            break;
        }
    }
    if (MOVIE_SIZE_ID_MAX <= i)
    {
        DBG_ERR("No Match Reso(%d, %d, %d) in g_MovieRecSizeTable\r\n", width, height, fps);
    }
    DBG_IND(":w=%d,%d,fps=%d,tbr=%d,ratio=%d\r\n", width, height, fps, tbr, ratio);
    if (pTBR)
    {
        *pTBR = tbr;
    }
    else
    {
        DBG_ERR("NULL Input pTBR\r\n");
    }
    if (pAspRatio)
    {
        *pAspRatio = ratio;
    }
    else
    {
        DBG_ERR("NULL Input pAspRatio\r\n");
    }

    return i;
}
//#NT#2015/03/18#Lily Kao -end

typedef struct
{
    UINT32  SingleIndex;
    UINT32  WiFiIndex;
} MOVIE_SINGLE_WIFI_MAP;

//MOVIE_SIZE_TAG
MOVIE_SINGLE_WIFI_MAP WiFiMapTbl[]=
{
    {MOVIE_SIZE_FRONT_2880x2160P24,MOVIE_SIZE_FRONT_2880x2160P24},  //Not support for WiFi. Only match with the menu option (MenuMovie.c)
    {MOVIE_SIZE_FRONT_2560x1440P30,MOVIE_SIZE_CLONE_2560x1440P30_848x480P30},
    {MOVIE_SIZE_FRONT_2304x1296P30,MOVIE_SIZE_CLONE_2304x1296P30_848x480P30},
    {MOVIE_SIZE_FRONT_1920x1080P96,MOVIE_SIZE_FRONT_1920x1080P96},  //Not support for WiFi. Only match with the menu option (MenuMovie.c)
    {MOVIE_SIZE_FRONT_1920x1080P60,MOVIE_SIZE_CLONE_1920x1080P60_848x480P30},
    {MOVIE_SIZE_FRONT_1920x1080P30,MOVIE_SIZE_CLONE_1920x1080P30_848x480P30}, // Live view size should be 848x480P30.
    {MOVIE_SIZE_FRONT_1280x720P120,MOVIE_SIZE_FRONT_1280x720P120},
    {MOVIE_SIZE_FRONT_1280x720P60,MOVIE_SIZE_FRONT_1280x720P60},
    {MOVIE_SIZE_FRONT_1280x720P30,MOVIE_SIZE_CLONE_1280x720P30_1280x720P30},
    {MOVIE_SIZE_FRONT_848x480P30,MOVIE_SIZE_CLONE_848x480P30_848x480P30},
    {MOVIE_SIZE_FRONT_640x480P240,MOVIE_SIZE_FRONT_640x480P240},
    {MOVIE_SIZE_FRONT_640x480P30,MOVIE_SIZE_CLONE_640x480P30_640x480P30 },
    {MOVIE_SIZE_FRONT_320x240P30,MOVIE_SIZE_CLONE_320x240P30_320x240P30 },
    {MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30,MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30 },
};

UINT32 GetSingleIndex(UINT32 wifiIndex)
{
    UINT32 i =0 ;

    for(i=0;i<sizeof(WiFiMapTbl)/sizeof(MOVIE_SINGLE_WIFI_MAP);i++)
    {
        if(WiFiMapTbl[i].WiFiIndex == wifiIndex)
        {
            DBG_IND("%d to %d\r\n",wifiIndex,WiFiMapTbl[i].SingleIndex);
            return WiFiMapTbl[i].SingleIndex;
        }
    }

    DBG_ERR("index not found %d\r\n",wifiIndex);
    return MOVIE_SIZE_ID_MAX;
}

UINT32 GetWiFiIndex(UINT32 singleIndex)
{
    UINT32 i =0 ;

    for(i=0;i<sizeof(WiFiMapTbl)/sizeof(MOVIE_SINGLE_WIFI_MAP);i++)
    {
        if(WiFiMapTbl[i].SingleIndex == singleIndex)
        {
            DBG_IND("%d to %d\r\n",singleIndex,WiFiMapTbl[i].WiFiIndex);
            return WiFiMapTbl[i].WiFiIndex;
        }
    }

    DBG_ERR("index not found %d\r\n",singleIndex);
    return MOVIE_SIZE_ID_MAX;
}



UINT32 CheckWiFiMapTbl(void)
{
    TM_ITEM*    pItem=0;
    TM_OPTION*  pOption;
    UINT32      i;
    pItem = TM_GetItem(&gMovieMenu,IDM_MOVIE_SIZE);

    if(pItem)
    {
        for(i=0;i<pItem->Count;i++)
        {
            pOption = &pItem->pOptions[i];
            if((pOption->Status & TM_OPTION_STATUS_MASK) == TM_OPTION_ENABLE)
            {
                if(GetWiFiIndex(i)==MOVIE_SIZE_ID_MAX)
                {
                    DBG_ERR("WiFiMapTbl not match\r\n");
                    return E_SYS;
                }
            }
        }

        return E_OK;
    }
    else
    {
        return E_SYS;
    }
}
