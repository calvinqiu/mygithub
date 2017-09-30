#include "ImageApp_CamMovie.h"
#include "UIAppMoviePlay.h"
#include "UIMovieInfo.h"
#include "UIPhotoInfo.h" //for IMAGERATIO_XXX
#include "Audio.h"
#include "SysMain.h"
#include "UIMovieMapping.h"
#include "UIFlow.h"
#include "ImageApp_IPCam.h"
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
    UINT32  uiVidEncFmt;
    UINT32  uiSizeIndex;
    UINT32  uiVidFrameRate;
    UINT32  uiTargetBitrate;
    UINT32  uiDispAspectRatio;
    UINT32  uiGOP;
    UINT32  uiSVCT;
    MOVIE_ENCODE_CROP  Crop;
    MOVIE_BRC_TYPE BrcType;
} MOVIE_CONFIG_PARAM;


typedef struct
{
    UINT32  uiWidth;
    UINT32  uiHeight;
    UINT32  uiImageRatio;

} MOVIE_SIZE_PARAM;

typedef struct {
    union
    {
        SM_H264_CBR_INFO CbrInfo;
        SM_H264_VBR_INFO VbrInfo;
    };
} MOVIE_BRC_INFO;

enum _MOVIE_SIZE_IPCAM
{
    MOVIE_SIZE_IPCAM_2560x1920,
    MOVIE_SIZE_IPCAM_2560x1440,
    MOVIE_SIZE_IPCAM_2048x1536,
    MOVIE_SIZE_IPCAM_1920x1080,
    MOVIE_SIZE_IPCAM_1280x960,
    MOVIE_SIZE_IPCAM_1280x720,
    MOVIE_SIZE_IPCAM_848x480,
    MOVIE_SIZE_IPCAM_720x576,
    MOVIE_SIZE_IPCAM_704x576,
    MOVIE_SIZE_IPCAM_720x480,
    MOVIE_SIZE_IPCAM_704x480,
    MOVIE_SIZE_IPCAM_640x480,
    MOVIE_SIZE_IPCAM_640x360,
    MOVIE_SIZE_IPCAM_352x288,
    MOVIE_SIZE_IPCAM_320x240,
    ////////////////////fisheye type
    MOVIE_SIZE_IPCAM_1440x1440, // 1440x1440
    MOVIE_SIZE_IPCAM_640x640, // 640x640
    MOVIE_SIZE_IPCAM_1920x480,   // 1920x480
    MOVIE_SIZE_IPCAM_640x180,   // 640x180
    MOVIE_SIZE_IPCAM_ID_MAX,
};

#define MOVIE_SIZE_FRONT_1280x720P120

static MOVIE_SIZE_PARAM g_MovieSizeTable[WIFI_MOVIE_APP_PREVIEW_SIZE_ID_MAX]=
{
        {2560, 1920 ,  IMAGERATIO_4_3},
        {2560, 1440 ,  IMAGERATIO_16_9},
        {2048, 1536 ,  IMAGERATIO_4_3},
        {1920, 1080 ,  IMAGERATIO_16_9},
        {1280,  960 ,  IMAGERATIO_4_3},
        {1280,  720 ,  IMAGERATIO_16_9},
        { 848,  480 ,  IMAGERATIO_16_9},
        { 720,  576 ,  IMAGERATIO_4_3},
        { 704,  576 ,  IMAGERATIO_4_3},
        { 720,  480 ,  IMAGERATIO_4_3},
        { 704,  480 ,  IMAGERATIO_4_3},
        { 640,  480 ,  IMAGERATIO_4_3},
        { 640,  360 ,  IMAGERATIO_16_9},
        { 352,  288 ,  IMAGERATIO_4_3},
        { 320,  240 ,  IMAGERATIO_4_3},
		//////fish eye type
        {1440,  1440,  IMAGERATIO_1_1},
		{ 640,   640,  IMAGERATIO_1_1},
		{1920,  480,  IMAGERATIO_4_3},
		{640 ,  180,  IMAGERATIO_4_3}
};

static MOVIE_CONFIG_PARAM g_MovieConfigTable[MOVIE_MAX_CHANNEL] =
{
  // channel 1
  {MEDIAVIDENC_H264, MOVIE_SIZE_IPCAM_1920x1080,  30,     500*1024,   VIDENC_DAR_DEFAULT, 60,  VIDENC_SVC_LAYER3, { FALSE, 0,0}, MOVIE_BRC_TYPE_NONE },   // 1080P, 30fps,  1200KB/sec
  // channel 2
  {MEDIAVIDENC_H264, MOVIE_SIZE_IPCAM_640x480 ,   30,     100*1024,   VIDENC_DAR_DEFAULT, 60,  VIDENC_SVC_LAYER3, { FALSE, 0,0}, MOVIE_BRC_TYPE_NONE },   // VGA, 30fps,  1200KB/sec
  // channel 3
  {MEDIAVIDENC_H264, MOVIE_SIZE_IPCAM_640x480,    30,     100*1024,   VIDENC_DAR_DEFAULT, 60,  VIDENC_SVC_LAYER3, { TRUE,  0,0}, MOVIE_BRC_TYPE_NONE },   // VGA, 30fps,  1200KB/sec
};

//bit rate control info
static MOVIE_BRC_INFO g_MovieBrcInfo[MOVIE_MAX_CHANNEL] = {0};

UINT32 GetMovieSizeWidth(UINT32 uiIndex)
{
    DBG_IND("%d\r\n", g_MovieSizeTable[g_MovieConfigTable[0].uiSizeIndex].uiWidth);
    return g_MovieSizeTable[g_MovieConfigTable[0].uiSizeIndex].uiWidth;
}
UINT32 GetMovieSizeHeight(UINT32 uiIndex)
{
    DBG_IND("%d\r\n", g_MovieSizeTable[g_MovieConfigTable[0].uiSizeIndex].uiHeight);
    return g_MovieSizeTable[g_MovieConfigTable[0].uiSizeIndex].uiHeight;
}
UINT32 GetMovieFrameRate(UINT32 uiIndex)
{
    DBG_IND("%d\r\n", g_MovieConfigTable[0].uiVidFrameRate);
    return g_MovieConfigTable[0].uiVidFrameRate;
}
UINT32 GetMovieTargetBitrate(UINT32 uiIndex, UINT32 uiQuality)
{
    DBG_IND("%d\r\n", g_MovieConfigTable[0].uiTargetBitrate);
    return g_MovieConfigTable[0].uiTargetBitrate;
}
void SetMovieTargetBitrate(UINT32 uiPath, UINT32 uiIndex, UINT32 uiBitRate)
{
    if(uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath=%d\r\n",uiPath);
        return;
    }
    g_MovieConfigTable[uiPath].uiTargetBitrate = uiBitRate;
}

void SetMovieLiveViewFrameRate(UINT32 uiPath, UINT32 uiIndex, UINT32 uiFrameRate)
{
    if(uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath=%d\r\n",uiPath);
        return;
    }
    g_MovieConfigTable[uiPath].uiVidFrameRate = uiFrameRate;
}

UINT32 GetMovieDispAspectRatio(UINT32 uiIndex)
{
    DBG_IND("uiDispAspectRatio = %d\r\n",g_MovieConfigTable[0].uiDispAspectRatio);
    return g_MovieConfigTable[0].uiDispAspectRatio;
}

UINT32 GetMovieSizeRatio(UINT32 uiIndex)
{
    //#NT#2016/04/14#Charlie Chang -begin
    //#NT# using  ImageApp_IPCam_GetConfig to get ratio index
    //DBG_IND("uiImageRatio = %d\r\n",g_MovieConfigTable[0].uiImageRatio);
    //return g_MovieSizeTable[g_MovieConfigTable[0].uiSizeIndex].uiImageRatio;
    return ImageApp_IPCam_GetConfig(IPCAM_CFG_RATIO);
    //#NT#2016/04/14#Charlie Chang -end
}


// 2-path image source
UINT32 GetMovieSizeWidth_2p(UINT32 uiPath, UINT32 uiIndex)
{
    UINT32 ui32Width = 0;

    if(uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath=%d\r\n",uiPath);
        return 0;
    }

    ui32Width = g_MovieSizeTable[g_MovieConfigTable[uiPath].uiSizeIndex].uiWidth;
#if(IPCAM_FUNC == ENABLE)
	#if (FISHEYE_TYPE == ENABLE)
			BOOL dewarp_enable = ImageApp_IPCam_GetConfig(IPCAM_CFG_DEWARP_FLAG);
			if(dewarp_enable == TRUE){
    			if(uiPath == 0){
					ui32Width = g_MovieSizeTable[MOVIE_SIZE_IPCAM_1920x480].uiWidth;
				}
				else if(uiPath==1){
					ui32Width = g_MovieSizeTable[MOVIE_SIZE_IPCAM_640x180].uiWidth;
				}
				else{
        			ui32Width = g_MovieSizeTable[MOVIE_SIZE_IPCAM_640x180].uiWidth;
				}
			}else{
    			if(uiPath == 0){
					ui32Width = g_MovieSizeTable[MOVIE_SIZE_IPCAM_1440x1440].uiWidth;
				}
				else if(uiPath==1){
					ui32Width = g_MovieSizeTable[MOVIE_SIZE_IPCAM_640x640].uiWidth;
				}
				else{
        			ui32Width = g_MovieSizeTable[MOVIE_SIZE_IPCAM_640x640].uiWidth;
				}

			}

	#endif
#endif
    DBG_IND("path %d W=%d\r\n", uiPath, ui32Width);

    return ui32Width;

}
UINT32 GetMovieSizeHeight_2p(UINT32 uiPath, UINT32 uiIndex)
{
    UINT32 ui32Height = 0;

    if(uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath=%d\r\n",uiPath);
        return 0;
    }
    ui32Height = g_MovieSizeTable[g_MovieConfigTable[uiPath].uiSizeIndex].uiHeight;
#if(IPCAM_FUNC == ENABLE)
	#if (FISHEYE_TYPE == ENABLE)
		BOOL dewarp_enable = ImageApp_IPCam_GetConfig(IPCAM_CFG_DEWARP_FLAG);
		if(dewarp_enable == TRUE){
    		if(uiPath == 0){
				ui32Height = g_MovieSizeTable[MOVIE_SIZE_IPCAM_1920x480].uiHeight;
			}
			else if(uiPath==1){
				ui32Height = g_MovieSizeTable[MOVIE_SIZE_IPCAM_640x180].uiHeight;
			}
			else{
        		ui32Height = g_MovieSizeTable[MOVIE_SIZE_IPCAM_640x180].uiHeight;
			}
		}
		else{
    		if(uiPath == 0){
				ui32Height = g_MovieSizeTable[MOVIE_SIZE_IPCAM_1440x1440].uiHeight;
			}
			else if(uiPath==1){
				ui32Height = g_MovieSizeTable[MOVIE_SIZE_IPCAM_640x640].uiHeight;
			}
			else{
        		ui32Height = g_MovieSizeTable[MOVIE_SIZE_IPCAM_640x640].uiHeight;
			}
		}

	#endif

#endif

    DBG_IND("path %d H=%d\r\n", uiPath, ui32Height);
    return ui32Height;

}
UINT32 GetMovieFrameRate_2p(UINT32 uiPath, UINT32 uiIndex)
{
    UINT32 uiFramerate = 0;

    if(uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath=%d\r\n",uiPath);
        return 0;
    }
    uiFramerate = g_MovieConfigTable[uiPath].uiVidFrameRate;
    DBG_IND("path %d Framerate:%d\r\n", uiPath, uiFramerate);
    return uiFramerate;


}
UINT32 GetMovieTargetBitrate_2p(UINT32 uiPath, UINT32 uiIndex)
{
    UINT32 uiBitrate = 0;

    if(uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath=%d\r\n",uiPath);
        return 0;
    }
    uiBitrate = g_MovieConfigTable[uiPath].uiTargetBitrate;
    DBG_IND("path %d Bitrate:%d\r\n", uiPath, uiBitrate);
    return uiBitrate;

}
UINT32 GetMovieDispAspectRatio_2p(UINT32 uiPath, UINT32 uiIndex)
{
    UINT32 uiDispAspecRatio = 0;

    if(uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath=%d\r\n",uiPath);
        return 0;
    }
    uiDispAspecRatio = g_MovieConfigTable[uiPath].uiDispAspectRatio;

    DBG_IND("path %d DispAspecRatio:%d\r\n", uiPath, uiDispAspecRatio);
    return uiDispAspecRatio;

}

UINT32 GetMovieSizeRatio_2p(UINT32 uiPath)
{
    UINT32 uiImageRatio = 0;

    if(uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath=%d\r\n",uiPath);
        return 0;
    }
    uiImageRatio = g_MovieSizeTable[g_MovieConfigTable[uiPath].uiSizeIndex].uiImageRatio;
    DBG_IND("path %d uiImageRatio:%d\r\n", uiPath, uiImageRatio);
    return uiImageRatio;
}

MOVIE_REC_TYPE GetMovieRecType_2p(UINT32 uiIndex)
{
    return MOVIE_REC_TYPE_CLONE;
}


UINT32 GetMovieGOP_2p(UINT32 uiPath)
{
    if (uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath %d\r\n",uiPath);
        return 0;
    }
    DBG_IND("path %d GOP:%d\r\n", uiPath, g_MovieConfigTable[uiPath].uiGOP);
    return g_MovieConfigTable[uiPath].uiGOP;
}

void SetMovieGOP_2p(UINT32 uiPath, UINT32 gop)
{
    if (uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath %d\r\n",uiPath);
        return;
    }
    g_MovieConfigTable[uiPath].uiGOP = gop;
}

UINT32 GetMovieSVCT_2p(UINT32 uiPath)
{
    if (uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath %d\r\n",uiPath);
        return 0;
    }
    DBG_IND("path %d SVC-T:%d\r\n", uiPath, g_MovieConfigTable[uiPath].uiSVCT);
    return g_MovieConfigTable[uiPath].uiSVCT;
}

void SetMovieSVCT_2p(UINT32 uiPath, UINT32 svct)
{
    if (uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath %d\r\n",uiPath);
        return;
    }
    g_MovieConfigTable[uiPath].uiSVCT = svct;
}

void SetMovieVidEncFmt_2p(UINT32 uiPath, UINT32 vidEncFmt)
{
    if (uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath %d\r\n",uiPath);
        return;
    }
    if ((vidEncFmt != MEDIAVIDENC_MJPG) && (vidEncFmt != MEDIAVIDENC_H264))
    {
        DBG_ERR("vidEncFmt %d\r\n",vidEncFmt);
        return;
    }
    g_MovieConfigTable[uiPath].uiVidEncFmt = vidEncFmt;
}

UINT32 GetMovieVidEncFmt_2p(UINT32 uiPath)
{
    if (uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath %d\r\n",uiPath);
        return 0;
    }
    DBG_IND("path %d vidEncFmt %d\r\n", uiPath, g_MovieConfigTable[uiPath].uiVidEncFmt);
    return g_MovieConfigTable[uiPath].uiVidEncFmt;
}

void SetMovieSizeIdx_2p(UINT32 uiPath, UINT32 uiSizeIndex)
{
    if (uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath %d\r\n",uiPath);
        return;
    }
    if (uiSizeIndex >= MOVIE_SIZE_IPCAM_ID_MAX)
    {
        DBG_ERR("uiSizeIndex %d\r\n",uiSizeIndex);
        return;
    }
    g_MovieConfigTable[uiPath].uiSizeIndex = uiSizeIndex;
}

UINT32 GetMovieSizeIdx_2p(UINT32 uiPath)
{
    if (uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath > %d\r\n",MOVIE_MAX_CHANNEL);
        return 0;
    }
    DBG_IND("path %d SizeIndex %d\r\n", uiPath, g_MovieConfigTable[uiPath].uiSizeIndex);
    return g_MovieConfigTable[uiPath].uiSizeIndex;
}

void SetMovieCrop_2p(UINT32 uiPath, MOVIE_ENCODE_CROP *crop)
{
    if (uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath %d\r\n",uiPath);
        return;
    }
    g_MovieConfigTable[uiPath].Crop = *crop;
}

MOVIE_ENCODE_CROP* GetMovieCrop_2p(UINT32 uiPath)
{
    if (uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath %d\r\n",uiPath);
        return NULL;
    }
    return &g_MovieConfigTable[uiPath].Crop;
}

VOID SetMovieBRC_2p(UINT32 uiPath, MOVIE_BRC_TYPE BrcType, VOID *pBrcInfo)
{
    UINT32 BrcInfoSize;

    if (uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath %d\r\n", uiPath);
        return;
    }

    if (BrcType >= MOVIE_BRC_TYPE_MAX)
    {
        DBG_ERR("BrcType %d\r\n", BrcType);
        return;
    }

    DBG_IND("path %d BrcType %d\r\n", uiPath, BrcType);

    g_MovieConfigTable[uiPath].BrcType = BrcType;

    memset(&g_MovieBrcInfo[uiPath], 0, sizeof(g_MovieBrcInfo[0]));

    switch(BrcType) {
    case MOVIE_BRC_TYPE_NONE:
        BrcInfoSize = 0;
        break;
    case MOVIE_BRC_TYPE_CBR:
        BrcInfoSize = sizeof(SM_H264_CBR_INFO);
        break;
    case MOVIE_BRC_TYPE_VBR:
        BrcInfoSize = sizeof(SM_H264_VBR_INFO);
        break;
    default:
        BrcInfoSize = 0;
        break;
    };

    if(BrcInfoSize)
        memcpy(&g_MovieBrcInfo[uiPath], pBrcInfo, BrcInfoSize);
}

MOVIE_BRC_TYPE GetMovieBRC_2p(UINT32 uiPath, UINT32 *pBrcInfoAddr)
{
    if (uiPath >= MOVIE_MAX_CHANNEL)
    {
        DBG_ERR("uiPath %d\r\n",uiPath);
        return MOVIE_BRC_TYPE_NONE;
    }

    *pBrcInfoAddr = (UINT32)(&g_MovieBrcInfo[uiPath]);

    return g_MovieConfigTable[uiPath].BrcType;
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
   *pTBR = 0;
   *pAspRatio = 0;
    return 0;
}
//#NT#2015/03/18#Lily Kao -end


UINT32 GetSingleIndex(UINT32 wifiIndex)
{
    return 0;
}

UINT32 GetWiFiIndex(UINT32 singleIndex)
{
    return 0;
}

UINT32 CheckWiFiMapTbl(void)
{
   return E_OK;
}
