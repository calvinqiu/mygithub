/*
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    @file       ImageApp_NetMovie.h
    @ingroup    mIImageApp

    @note       Nothing.

    @date       2014/11/28
*/

#ifndef NETMOVIE_H
#define NETMOVIE_H

#include "GxLib.h"
#include "MediaWriteLib.h"
#include "AudFilterAPI.h"

#include "ImageStream.h"
#include "ImageUnit_ImagePipe.h"
#include "ImageUnit_CamFile.h"
#include "ImageUnit_NetRTSP.h"
#include "ImageUnit_NetHTTP.h"
#include "ImageUnit_Photo.h"
#include "ImageUnit_Cap.h"
#include "ImageUnit_Media.h"
#include "ImageUnit_ADAS.h"
#include "ImageUnit_ALG.h"
#include "ImageUnit_StreamSender.h"

typedef void SET_REC_PARAM(void);


/**
    @addtogroup mIImageApp
*/
//@{

typedef  enum
{
    NETMOVIE_CFG_START = 0x00004000,
    NETMOVIE_CFG_POOL  = NETMOVIE_CFG_START,
    NETMOVIE_CFG_MODE,
    NETMOVIE_CFG_OPEN_REC_IMMEDIATELY,
    NETMOVIE_CFG_REC_PARAM_CB,
    NETMOVIE_CFG_REC_COUNT,
    NETMOVIE_CFG_PIM_SNAPSHOT,
    NETMOVIE_CFG_ISF_OUT4,
    NETMOVIE_CFG_WRITE_COUNT,
    NETMOVIE_CFG_IPL_SETBUFCNT, //count (6 < count < 10)
    NETMOVIE_CFG_DUALCAM_DISPLAY, //for HTTPLIVEVIEW config
    NETMOVIE_CFG_USE_STEAM_1,///using stream 1 to send rtsp or streaming sender
    //#NT#2016/05/16#KCHong -begin
    NETMOVIE_CFG_SFUNC_EN
    //#NT#2016/05/16#KCHong -end
}NETMOVIE_CFG_ID;

enum
{
    NETMOVIE_TYPE_RTSP = 0,
    NETMOVIE_TYPE_HTTP,
    NETMOVIE_TYPE_RTSP_MJPG,
    NETMOVIE_TYPE_IOTP2P,
    NETMOVIE_TYPE_IOTP2P_MJPG,
};

typedef  enum
{
    NETMOVIE_DUALCAM_FRONT = 0,
    NETMOVIE_DUALCAM_BEHIND,
    NETMOVIE_DUALCAM_BOTH,
}NETMOVIE_DUALCAM_TYPE;

//#NT#2016/05/16#KCHong -begin
/**
    NetMovie specific function enable
*/
typedef enum _NETMOVIE_SFUNC_EN
{
    NETMOVIE_SFUNC_NONE             = 0x00000000,    ///< disable specific function related IPL path.
    NETMOVIE_SFUNC_ADAS             = 0x00000001,    ///< Link IPL0 path3 and 4 for AVP and FC/LD.
    NETMOVIE_SFUNC_OT               = 0x00000002,    ///< Link IPL0 path3 for OT.
    NETMOVIE_SFUNC_DDD              = 0x00000004,    ///< Link IPL1 path3 for DDD.
    NETMOVIE_SFUNC_IPL1_PATH3       = 0x00010000,    ///< Link IPL1 path3 to ImageUnit_ALG ISF_IN1
    NETMOVIE_SFUNC_IPL1_PATH4       = 0x00020000,    ///< Link IPL1 path4 to ImageUnit_ALG ISF_IN2
    NETMOVIE_SFUNC_IPL2_PATH3       = 0x00040000,    ///< Link IPL2 path3 to ImageUnit_ALG ISF_IN3
    NETMOVIE_SFUNC_IPL2_PATH4       = 0x00080000,    ///< Link IPL2 path4 to ImageUnit_ALG ISF_IN4
    NETMOVIE_SFUNC_IPL1_PATH3_PVEN  = 0x00100000,    ///< IPL1 path3 output enable when preview
    NETMOVIE_SFUNC_IPL1_PATH4_PVEN  = 0x00200000,    ///< IPL1 path4 output enable when preview
    NETMOVIE_SFUNC_IPL2_PATH3_PVEN  = 0x00400000,    ///< IPL2 path3 output enable when preview
    NETMOVIE_SFUNC_IPL2_PATH4_PVEN  = 0x00800000,    ///< IPL2 path4 output enable when preview
    NETMOVIE_SFUNC_IPL_MASK         = 0x00ff0000,    ///< Mask of IPL1/2 path3/4 setting
    ENUM_DUMMY4WORD(NETMOVIE_SFUNC_EN)
} NETMOVIE_SFUNC_EN;
//#NT#2016/05/16#KCHong -end

extern void ImageApp_NetMovie_Open(void);

extern void ImageApp_NetMovie_Close(void);

extern void ImageApp_NetMovie_RecStart(void);

extern void ImageApp_NetMovie_RecStop(void);

extern void ImageApp_NetMovie_Idle(void);

extern UINT32 ImageApp_NetMovie_Snapshot(void);

extern void ImageApp_NetMovie_Config(NETMOVIE_CFG_ID config_id, UINT32 value);

extern UINT32 ImageApp_NetMovie_GetConfig(NETMOVIE_CFG_ID config_id);

//@}
#endif//NETMOVIE_H

