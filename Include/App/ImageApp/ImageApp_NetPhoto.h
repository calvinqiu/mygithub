/*
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    @file       ImageApp_NetPhoto.h
    @ingroup    mIImageApp

    @note       Nothing.

    @date       2014/11/28
*/

#ifndef NETPHOTO_H
#define NETPHOTO_H

#include "GxLib.h"

#include "ImageStream.h"
#include "ImageUnit_ImagePipe.h"
#include "ImageUnit_CamDisp.h"
#include "ImageUnit_NetRTSP.h"
#include "ImageUnit_NetHTTP.h"
#include "ImageUnit_Photo.h"
#include "ImageUnit_Cap.h"
#include "ImageUnit_Media.h"
#include "NVTEvent.h"

/**
    @addtogroup mIImageApp
*/
//@{

typedef  enum
{
    NETPHOTO_CFG_START = 0x00005000,
    NETPHOTO_CFG_POOL  = NETPHOTO_CFG_START,
    NETPHOTO_CFG_DUALCAM_DISPLAY,
    NETPHOTO_CFG_MODE
}NETPHOTO_CFG_ID;

typedef  enum
{
    NETPHOTO_DUALCAM_FRONT = 0,
    NETPHOTO_DUALCAM_BEHIND,
    NETPHOTO_DUALCAM_BOTH,
}NETPHOTO_DUALCAM_TYPE;

enum
{
    NETPHOTO_TYPE_HTTP,
    NETPHOTO_TYPE_RTSP_MJPG,
};


extern void ImageApp_NetPhoto_Open(void);
extern UINT32 ImageApp_NePhoto_CapStart(void);
extern void ImageApp_NePhoto_CapStop(void);
extern void ImageApp_NetPhoto_Close(void);
extern void ImageApp_NetPhoto_Config(NETPHOTO_CFG_ID config_id, UINT32 value);
extern UINT32 ImageApp_NetPhoto_GetConfig(NETPHOTO_CFG_ID config_id);


//@}
#endif//NETPHOTO_H
