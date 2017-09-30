/*
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    @file       ImageApp_UsbMovie.h
    @ingroup    mIImageApp

    @note       Nothing.

    @date       2014/11/28
*/

#ifndef USBMOVIE_H
#define USBMOVIE_H

#include "GxLib.h"
#include "MediaWriteLib.h"
#include "AudFilterAPI.h"

#include "ImageStream.h"
#include "ImageUnit_ImagePipe.h"
#include "ImageUnit_CamFile.h"
#include "UVAC.h"
#include "ImageUnit_Photo.h"
#include "ImageUnit_Cap.h"
#include "ImageUnit_ALG.h"
#include "ImageUnit_Media.h"

/**
    @addtogroup mIImageApp
*/
//@{

/**
     @name Tagret bitrate for MJPEG. Unit is Byte/Sec.

     @note for NVT_USBMOVIE_CFG_TBR_MJPG
*/
//@{
#define NVT_USBMOVIE_TBR_MJPG_LOW       0x200000  ///< 2M
#define NVT_USBMOVIE_TBR_MJPG_HIGH      0x1000000 ///< 16M
#define NVT_USBMOVIE_TBR_MJPG_DEFAULT   0x800000  ///< 8M
//@}

/**
     @name Tagret bitrate for H264. Unit is Byte/Sec.

     @note for NVT_USBMOVIE_CFG_TBR_H264
*/
//@{
#define NVT_USBMOVIE_TBR_H264_LOW       0x100000  ///< 1M
#define NVT_USBMOVIE_TBR_H264_HIGH      0x800000  ///< 8M
#define NVT_USBMOVIE_TBR_H264_DEFAULT   0x200000  ///< 2M
//@}

/**
     @name Image ratio.

     @note for ImageUnit_CfgAspect()
*/
//@{
#define NVT_USBMOVIE_CROPRATIO_W_16     16
#define NVT_USBMOVIE_CROPRATIO_W_4      4
#define NVT_USBMOVIE_CROPRATIO_H_9      9
#define NVT_USBMOVIE_CROPRATIO_H_3      3
//@}


/**
     @name Usb movie mode.

     @note for NVT_USBMOVIE_CFG_MODE
*/
//@{
#define NVT_USBMOVIE_MODE_CAM             0    ///< like a PC cam
#define NVT_USBMOVIE_MODE_MULTIMEDIA      1    ///< for multimedia system
//@}

/**
     @name Usb media format.

     Uvc payload format.

     @note for NVT_USBMOVIE_CFG_MEDIA_FMT
*/
//@{
#define NVT_USBMOVIE_MEDIA_MJPEG            0    ///< MJPEG
#define NVT_USBMOVIE_MEDIA_H264             1    ///< H264
//@}

/**
    Configuration identifier

    @note for ImageApp_UsbMovie_Config(), ImageApp_UsbMovie_GetConfig()
*/
typedef enum
{
    NVT_USBMOVIE_CFG_POOL_SIZE,             ///< obsolete item
    NVT_USBMOVIE_CFG_POOL,                  ///< memory buffer for usb movie mode
    NVT_USBMOVIE_CFG_PIM_CAPTURE,           ///< enable PIM by capture (picture in movie by capture)
    NVT_USBMOVIE_CFG_SAVE_MOVIE_FILE,       ///< save stream to movie file
    NVT_USBMOVIE_CFG_CHANNEL,               ///< UVC channel number
    NVT_USBMOVIE_CFG_TBR_MJPG,              ///< tagret bitrate for MJPEG. Unit is Byte/Sec.
    NVT_USBMOVIE_CFG_TBR_H264,              ///< tagret bitrate for H264. Unit is Byte/Sec.
    NVT_USBMOVIE_CFG_SET_MOVIE_SIZE_CB,     ///< the callback is invoked while UVC is started by USB host
    NVT_USBMOVIE_CFG_MODE,                  ///< for PC cam mode or multimedia mode
    NVT_USBMOVIE_CFG_MEDIA_FMT,             ///< payload data is MJPEG or H264, for multimedia mode only
    NVT_USBMOVIE_CFG_REC_COUNT,             ///< movie record type, set 1 for MOVIE_REC_TYPE_FRONT and 2 for the others
    NVT_USBMOVIE_CFG_PIM_SNAPSHOT,          ///< enable PIM by snapshot (picture in movie by snapshot)
    NVT_USBMOVIE_CFG_IPL_SETBUFCNT,         ///< count (6 < count < 10)
    NVT_USBMOVIE_CFG_ISF_OUT4,              ///< enable ime path 4
    NVT_USBMOVIE_CFG_WRITE_COUNT,           ///< save 1 or 2 movie files during recording
    NVT_USBMOVIE_CFG_OPEN_REC_IMMEDIATELY,
    NVT_USBMOVIE_CFG_SFUNC_EN,
    NVT_USBMOVIE_CFG_MAX,
}NVT_USBMOVIE_CFG;

/**
    Specific function enable
*/
typedef enum _NVT_USBMOVIE_SFUNC_EN
{
    NVT_USBMOVIE_SFUNC_NONE             = 0x00000000,    ///< disable specific function related IPL path.
    NVT_USBMOVIE_SFUNC_IPL1_PATH3       = 0x00010000,    ///< Link IPL1 path3 to ImageUnit_ALG ISF_IN1
    NVT_USBMOVIE_SFUNC_IPL1_PATH4       = 0x00020000,    ///< Link IPL1 path4 to ImageUnit_ALG ISF_IN2
    NVT_USBMOVIE_SFUNC_IPL2_PATH3       = 0x00040000,    ///< Link IPL2 path3 to ImageUnit_ALG ISF_IN3
    NVT_USBMOVIE_SFUNC_IPL2_PATH4       = 0x00080000,    ///< Link IPL2 path4 to ImageUnit_ALG ISF_IN4
    NVT_USBMOVIE_SFUNC_IPL1_PATH3_PVEN  = 0x00100000,    ///< IPL1 path3 output enable when preview
    NVT_USBMOVIE_SFUNC_IPL1_PATH4_PVEN  = 0x00200000,    ///< IPL1 path4 output enable when preview
    NVT_USBMOVIE_SFUNC_IPL2_PATH3_PVEN  = 0x00400000,    ///< IPL2 path3 output enable when preview
    NVT_USBMOVIE_SFUNC_IPL2_PATH4_PVEN  = 0x00800000,    ///< IPL2 path4 output enable when preview
    NVT_USBMOVIE_SFUNC_IPL_MASK         = 0x00ff0000,    ///< Mask of IPL1/2 path3/4 setting
    ENUM_DUMMY4WORD(NVT_USBMOVIE_SFUNC_EN)
} NVT_USBMOVIE_SFUNC_EN;

/**
    Video resolution.

    @note for the parameter of IMAGAPP_UVAC_SET_VIDRESO_CB
*/
typedef struct _NVT_USBMOVIE_VID_RESO
{
    UINT32 width;
    UINT32 height;
    UINT32 fps;
    UVAC_VIDEO_FORMAT codec;
}NVT_USBMOVIE_VID_RESO, *PNVT_USBMOVIE_VID_RESO;

/**
    @name Callback prototype for NVT_USBMOVIE_CFG_SET_MOVIE_SIZE_CB

    @param[in] pVidReso1 pointer to video resolution of UVC channel 1 , refering to #_NVT_USBMOVIE_VID_RESO
    @param[in] pVidReso2 pointer to video resolution of UVC channel 2 , refering to #_NVT_USBMOVIE_VID_RESO

    @return void
*/
//@{
typedef void (*IMAGAPP_UVAC_SET_VIDRESO_CB)(PNVT_USBMOVIE_VID_RESO pVidReso1, PNVT_USBMOVIE_VID_RESO pVidReso2);
//@}

/**
    Open USB movie mode.

    Open USB movie mode.

    @param[in] void

    @return void
*/
extern void ImageApp_UsbMovie_Open(void);

/**
    Close USB movie mode.

    Close USB movie mode.

    @param[in] void

    @return void
*/
extern void ImageApp_UsbMovie_Close(void);

/**
    Start recording.

    In NVT_USBMOVIE_MODE_CAM, start recording media stream to USB host.
    In NVT_USBMOVIE_MODE_MULTIMEDIA, start recording file.

    @return void
*/
extern void ImageApp_UsbMovie_RecStart(void);

/**
    Stop recording or start live view.

    In NVT_USBMOVIE_MODE_CAM, stop recording and transfering stream to USB host.
    In NVT_USBMOVIE_MODE_MULTIMEDIA, stop recording file in NVT_USBMOVIE_MODE_MULTIMEDIA.

    @return void
*/
extern void ImageApp_UsbMovie_RecStop(void);

/**
    Stop live view.

    Only valid in NVT_USBMOVIE_MODE_MULTIMEDIA, stop transfering stream to USB host.

    @return void
*/
extern void ImageApp_UsbMovie_LviewStop(void);

/**
    Start snapshot. (take a picture in movie by extracting recording frame)

    Start snapshot.

    @return void
*/
extern void ImageApp_UsbMovie_Snapshot(void);

/**
    Config settings for USB movie mode.

    Config settings for USB movie mode.

    @param[in] config_id refer to NVT_USBMOVIE_CFG
    @param[in] value configuration value.

    @return void
*/
extern void ImageApp_UsbMovie_Config(UINT32 config_id, UINT32 value);

/**
    Get config settings for USB movie mode.

    Get config settings for USB movie mode.

    @param[in] config_id refer to NVT_USBMOVIE_CFG

    @return configuration value
*/
extern UINT32 ImageApp_UsbMovie_GetConfig(UINT32 config_id);


//@}
#endif//USBMOVIE_H

