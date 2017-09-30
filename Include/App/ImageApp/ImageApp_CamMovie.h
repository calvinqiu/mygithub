/*
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    @file       ImageApp_CamMovie.h
    @ingroup    mIImageApp

    @note       Nothing.

    @date       2014/11/28
*/

#ifndef CAMMOVIE_H
#define CAMMOVIE_H

#include "GxLib.h"
#include "MediaWriteLib.h"
#include "AudFilterAPI.h"

#include "ImageStream.h"
#include "ImageUnit_ImagePipe.h"
#include "ImageUnit_CamDisp.h"
#include "ImageUnit_CamFile.h"
#include "ImageUnit_Photo.h"
#include "ImageUnit_Cap.h"
#include "ImageUnit_Media.h"
#include "ImageUnit_ADAS.h"
#include "ImageUnit_ALG.h"
//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#include "ImageUnit_Demux.h"
//#NT#2016/09/20#Bob Huang -end

/**
    @addtogroup mIImageApp
*/
//@{


/**
    @name Media record event ID

    Callback ID of media recorder.
*/
//@{
#define MOVIE_CFG_POOL                  1     //memory address for recording
#define MOVIE_CFG_PIM_CAPTURE           2     //enable PIM by capture (picture in movie by capture)
#define MOVIE_CFG_PIM_SNAPSHOT          3     //enable PIM by snapshot (picture in movie by snapshot)
#define MOVIE_CFG_RATIO                 4     //crop ratio, IPL_SENCROPRATIO_4_3 or others
#define MOVIE_CFG_RECORDCOUNT           5     //record count, current maxmium is 2
#define MOVIE_CFG_ISF_OUT4              6     //ime path 4 is enable or not
#define MOVIE_CFG_IPL_MAXBUF            7     //IPL buffer use maximum count
#define MOVIE_CFG_FRONTVIEW_IPL2_ENABLE 8     //Enable IPL2 on front preview by Dual Sensor case
#define MOVIE_CFG_CAPTURE_UNIT          9     //assign capture unit for PIM
#define MOVIE_CFG_IPL_SETBUFCNT         10    //Set IPL buffer count (6 < count < 10)
//#NT#2016/05/13#KCHong -begin
#define MOVIE_CFG_SFUNC_EN              11    //Set specific function
//#NT#2016/05/13#KCHong -end
//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#define MOVIE_CFG_3DNROUT               12    //enable 3DNR Out
//#NT#2016/09/20#Bob Huang -end
#define MOVIE_CFG_IPLEXCHANGE           13    //exchange IPL 1 & 2
//#define MOVIE_CFG_RSC           5           //RSC on/off, SEL_RSC_OFF or others
//#define MOVIE_CFG_FUNCINFO      9           //PhotoFuncInfo_dummy
//#define MOVIE_CFG_USE_FILEDB      10        //use fileDB or not, 0 or 1
//#define MOVIE_CFG_FILEDB_MEM      11        //memory address for fileDB
//#define MOVIE_CFG_MULTIREC        12        //multi-recording
//#define MOVIE_CFG_MULTIREC_YUVCB  13        //multi-recording, getYUV CB
//#define MOVIE_CFG_IMGPATH         14        //config different image size,refer to
//@}

//#NT#2016/05/13#KCHong -begin
/**
    CamMovie specific function enable
*/
typedef enum _CAMMOVIE_SFUNC_EN
{
    CAMMOVIE_SFUNC_NONE             = 0x00000000,    ///< disable specific function related IPL path.
    CAMMOVIE_SFUNC_ADAS             = 0x00000001,    ///< Link IPL0 path3 and 4 for AVP and FC/LD.
    CAMMOVIE_SFUNC_OT               = 0x00000002,    ///< Link IPL0 path3 for OT.
    CAMMOVIE_SFUNC_DDD              = 0x00000004,    ///< Link IPL1 path3 for DDD.
    CAMMOVIE_SFUNC_IPL1_PATH3       = 0x00010000,    ///< Link IPL1 path3 to ImageUnit_ALG ISF_IN1
    CAMMOVIE_SFUNC_IPL1_PATH4       = 0x00020000,    ///< Link IPL1 path4 to ImageUnit_ALG ISF_IN2
    CAMMOVIE_SFUNC_IPL2_PATH3       = 0x00040000,    ///< Link IPL2 path3 to ImageUnit_ALG ISF_IN3
    CAMMOVIE_SFUNC_IPL2_PATH4       = 0x00080000,    ///< Link IPL2 path4 to ImageUnit_ALG ISF_IN4
    CAMMOVIE_SFUNC_IPL1_PATH3_PVEN  = 0x00100000,    ///< IPL1 path3 output enable during preview
    CAMMOVIE_SFUNC_IPL1_PATH4_PVEN  = 0x00200000,    ///< IPL1 path4 output enable during preview
    CAMMOVIE_SFUNC_IPL2_PATH3_PVEN  = 0x00400000,    ///< IPL2 path3 output enable during preview
    CAMMOVIE_SFUNC_IPL2_PATH4_PVEN  = 0x00800000,    ///< IPL2 path4 output enable during preview
    CAMMOVIE_SFUNC_IPL_MASK         = 0x00ff0000,    ///< Mask of IPL1/2 path3/4 setting
    ENUM_DUMMY4WORD(CAMMOVIE_SFUNC_EN)
} CAMMOVIE_SFUNC_EN;
//#NT#2016/05/13#KCHong -end

/**
    Config settings for movie recording.

    Config settings for movie recording, such as MOVIE_CFG_POOL or others.

    @param[in] config_id MOVIE_CFG_POOL or others.
    @param[in] value configuration value.

    @return void
*/
extern void ImageApp_CamMovie_Config(UINT32 config_id, UINT32 value);

/**
    Get config settings for movie recording.

    Get config settings for movie recording, such as MOVIE_CFG_IMGPATH or others.

    @param[in] config_id MOVIE_CFG_IMGPATH or others.

    @return configuration value
*/

extern UINT32 ImageApp_CamMovie_GetConfig(UINT32 config_id);

/**
    Open movie recorder.

    Open movie recorder.

    @param[in] void

    @return void
*/
extern void ImageApp_CamMovie_Open(void);

/**
    Close movie recorder.

    Close movie recorder.

    @return void
*/
extern void ImageApp_CamMovie_Close(void);

/**
    Start record.

    Start to record a movie file.

    @return void
*/
extern UINT32 ImageApp_CamMovie_RecStart(void);

/**
    Stop record.

    Stop recording and save a movie file, and return to preview.

    @return void
*/
extern void ImageApp_CamMovie_RecStop(void);

/**
    Start capture. (take a picture in movie by capturing full frame)

    Start capture.

    @return void
*/
extern UINT32 ImageApp_CamMovie_CapStart(void);

/**
    Stop capture.

    Stop capture and save a photo file, and return to preview.

    @return void
*/
extern void ImageApp_CamMovie_CapStop(void);

/**
    Start snapshot. (take a picture in movie by extracting recording frame)

    Start snapshot.

    @return void
*/
extern UINT32 ImageApp_CamMovie_Snapshot(void);


//@}
#endif//CAMMOVIE_H
