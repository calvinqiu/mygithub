/*
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    @file       ImageApp_IPCam.h
    @ingroup    mIImageApp

    @note       Nothing.

    @date       2014/11/28
*/

#ifndef IMAGEAPP_IPCAM_H
#define IMAGEAPP_IPCAM_H

#include "GxLib.h"
#include "MediaWriteLib.h"
#include "AudFilterAPI.h"

#include "ImageStream.h"
#include "ImageUnit_ImagePipe.h"
#include "ImageUnit_CamDisp.h"
#include "ImageUnit_CamFile.h"
#include "ImageUnit_Media.h"
#include "ImageUnit_StreamSender.h"

/**
    @addtogroup mIImageApp
*/
//@{


/**
    @name Media record event ID

    Callback ID of media recorder.
*/
//@{

typedef  enum
{
    IPCAM_CFG_START = 0x00006000,
    IPCAM_CFG_POOL  = IPCAM_CFG_START,
    IPCAM_CFG_PIM_SNAPSHOT,
    IPCAM_CFG_RATIO,
    IPCAM_CFG_IPL_MAXBUF,
    IPCAM_CFG_PLUG_RTSP,
    IPCAM_CFG_CROP,
    IPCAM_CFG_HDMI_OUT,
    IPCAM_CFG_INIT_FLAG,
    IPCAM_CFG_DEWARP_FLAG,
    IPCAM_CFG_SBS_FLAG,
    ENUM_DUMMY4WORD(IPCAM_CFG_ID)
}IPCAM_CFG_ID;



typedef enum
{
    IPCAM_PATH_ID_1 = 0,
    IPCAM_PATH_ID_2 = 1,
    IPCAM_PATH_ID_3 = 2,
    IPCAM_PATH_MAX_ID = 3,
    ENUM_DUMMY4WORD(IPCAM_PATH_ID)
}IPCAM_PATH_ID;


typedef enum
{
    IPCAM_STATE_ENABLE = 0,
    IPCAM_STATE_DISABLE = 1,
    IPCAM_STATE_CHGPARM = 2,
    IPCAM_STATE_MAX_ID = 3,
    ENUM_DUMMY4WORD(IPCAM_STATE)
}IPCAM_STATE;


typedef enum{
    ImageApp_IPcam_IPL1 =0,
	ImageApp_IPcam_IPL2 ,
	ImageApp_IPcam_IPL3,
	ImageApp_IPcam_IPL4,
	ImageApp_IPcam_UVC,
	ImageApp_IPcam_MAX,
	ENUM_DUMMY4WORD(IMAGEAPP_IPCAM_IPL)

}IPCAM_IPL;

//@}


/**
    Config settings for movie recording.

    Config settings for movie recording, such as IPCAM_CFG_POOL or others.

    @param[in] config_id IPCAM_CFG_POOL or others.
    @param[in] value configuration value.

    @return void
*/
extern void ImageApp_IPCam_Config(UINT32 config_id, UINT32 value);

/**
    Get config settings for movie recording.

    Get config settings for movie recording, such as IPCAM_CFG_IMGPATH or others.

    @param[in] config_id IPCAM_CFG_IMGPATH or others.

    @return configuration value
*/

extern UINT32 ImageApp_IPCam_GetConfig(UINT32 config_id);

/**
    Open movie recorder.

    Open movie recorder.

    @param[in] void

    @return void
*/
extern void ImageApp_IPCam_Open(BOOL pathen[3]);

/**
    Close movie recorder.

    Close movie recorder.

    @return void
*/
extern void ImageApp_IPCam_Close(void);

/**
    Start record.

    Start to record a movie file.

    @return void
*/
extern UINT32 ImageApp_IPCam_RecStart(IPCAM_PATH_ID path);

/**
    Stop record.

    Stop recording and save a movie file, and return to preview.

    @return void
*/
extern void ImageApp_IPCam_RecStop(IPCAM_PATH_ID path);

/**
    Start snapshot. (take a picture in movie by extracting recording frame)

    Start snapshot.

    @return void
*/
extern UINT32 ImageApp_IPCam_Snapshot(IPCAM_PATH_ID path);

extern void ImageApp_IPCam_CfgImgSize(IPCAM_PATH_ID Path, UINT32 img_w, UINT32 img_h);

extern void ImageApp_IPCam_CfgImgRatio(IPCAM_PATH_ID Path, UINT32 ratio_w, UINT32 ratio_h);

extern void ImageApp_IPCam_CfgWindow(IPCAM_PATH_ID Path);

extern void ImageApp_IPCam_SetPath(IPCAM_PATH_ID Path, IPCAM_STATE state);

extern void ImageApp_IPCam_ChgRTSPParm(void);

extern IPCAM_IPL * ImageApp_IPCam_Get_IPL_Mapping(void );

void ImageApp_IPCam_Set_IPL_Mapping(IPCAM_IPL IPL1, IPCAM_IPL IPL2,IPCAM_IPL IPL3);

//@}
#endif//IMAGEAPP_IPCAM_H