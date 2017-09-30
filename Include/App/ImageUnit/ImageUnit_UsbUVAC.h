/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_UsbUVAC.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_USBUVAC_H
#define IMAGEUNIT_USBUVAC_H

/*
Support these input port(s): IN1,IN2

Support these output port(s):

Support these port config API(s):

============================================================
port
------------------------------------------------------------
ImageUnit_CfgImgSize()
ImageUnit_CfgImgAspect()
ImageUnit_CfgImgWindow()
ImageUnit_CfgImgDirect()
============================================================
*/

#include "Type.h"
#include "ImageStream.h"



extern ISF_UNIT ISF_UsbUVAC;

enum
{
    USBUVAC_PARAM_START = 0x00003000,
};

/**
    Supported video format.

    @note For IMGUNIT_UVAC_CFG_VID_FMT_TYPE.
*/
typedef enum _IMGUNIT_UVAC_VIDEO_FORMAT_TYPE
{
    IMGUNIT_UVAC_VIDEO_FORMAT_H264_MJPEG,          ///< Support H264 and MJPEG.
    IMGUNIT_UVAC_VIDEO_FORMAT_H264_ONLY,           ///< Support H264 only.
    IMGUNIT_UVAC_VIDEO_FORMAT_MJPEG_ONLY,          ///< Support MJPEG only.
    ENUM_DUMMY4WORD(IMGUNIT_UVAC_VIDEO_FORMAT_TYPE)
} IMGUNIT_UVAC_VIDEO_FORMAT_TYPE;


/**
    Parameters for ISF_UsbUVAC.
*/
typedef enum
{
    IMGUNIT_UVAC_CFG_SET_MEM_POOL,        ///< Obsolete
    IMGUNIT_UVAC_CFG_NEED_MEM_SIZE,       ///< Obsolete
    IMGUNIT_UVAC_CFG_UVAC_INFO,           ///< Information needed for UVAC module.
    IMGUNIT_UVAC_CFG_UVAC_VEND_DEV_DESC,  ///< Device descriptor, refering to #_UVAC_VEND_DEV_DESC.
    IMGUNIT_UVAC_CFG_VID_RESO_ARY,        ///< Video resolution array, refering to #_UVAC_VID_RESO_ARY.
    IMGUNIT_UVAC_CFG_AUD_SAMPLERATE_ARY,  ///< Audio sample array, refering to #_UVAC_AUD_SAMPLERATE_ARY_.
    IMGUNIT_UVAC_CFG_UVAC_TBR_MJPG,       ///< Motion JPEG stream target size(Byte/Sec). Default value is 0x300000.
    IMGUNIT_UVAC_CFG_UVAC_TBR_H264,       ///< H.264 stream target size(Byte/Sec). Default value is 0x100000.
    IMGUNIT_UVAC_CFG_BS_START,            ///< Resume  sending stream to USB host.
    IMGUNIT_UVAC_CFG_BS_STOP,             ///< Suspend sending stream to USB host.
    IMGUNIT_UVAC_CFG_UVAC_CAP_M3,         ///< Enable UVC capture method 3.
    IMGUNIT_UVAC_CFG_EU_VENDCMDCB_ID01,   ///> Extension unit callback with CS=1.
    IMGUNIT_UVAC_CFG_EU_VENDCMDCB_ID02,   ///> Extension unit callback with CS=2.
    IMGUNIT_UVAC_CFG_EU_VENDCMDCB_ID03,   ///> Extension unit callback with CS=3.
    IMGUNIT_UVAC_CFG_EU_VENDCMDCB_ID04,   ///> Extension unit callback with CS=4.
    IMGUNIT_UVAC_CFG_EU_VENDCMDCB_ID05,   ///> Extension unit callback with CS=5.
    IMGUNIT_UVAC_CFG_EU_VENDCMDCB_ID06,   ///> Extension unit callback with CS=6.
    IMGUNIT_UVAC_CFG_EU_VENDCMDCB_ID07,   ///> Extension unit callback with CS=7.
    IMGUNIT_UVAC_CFG_EU_VENDCMDCB_ID08,   ///> Extension unit callback with CS=8.
    IMGUNIT_UVAC_CFG_WINUSB_ENABLE,       ///< Enable WINUSB.
    IMGUNIT_UVAC_CFG_WINUSB_CB,           ///< The callback function for WINUSB
    IMGUNIT_UVAC_CFG_BULK_DATA,           ///< Obsolete
    IMGUNIT_UVAC_CFG_VID_FMT_TYPE,        ///< Supported video format by UVC device.
    IMGUNIT_UVAC_CFG_CDC_ENABLE,          ///< Enable CDC ADM.
    IMGUNIT_UVAC_CFG_CDC_PSTN_REQUEST_CB, ///< The callback function for CDC PSTN subclass specific request
    IMGUNIT_UVAC_CFG_AUD_FMT_TYPE,		  ///< Audio format, refering to #_UVAC_AUD_FMT_TYPE
    IMGUNIT_UVAC_CFG_MAX_FRAME_SIZE,	  ///< Set higher frame size in MJPEG format.
    IMGUNIT_UVAC_CFG_MAX,
    ENUM_DUMMY4WORD(IMGUNIT_UVAC_CFG)
}IMGUNIT_UVAC_CFG;

extern ISF_UNIT ISF_UsbUVAC;

#endif //IMAGEUNIT_USBUVAC_H

