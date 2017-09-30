/*
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    @file       ImageApp_CamPhoto.h
    @ingroup    mIImageApp

    @note       Nothing.

    @date       2014/11/28
*/

#ifndef CAMPHOTO_H
#define CAMPHOTO_H

#include "GxLib.h"

#include "ImageStream.h"
#include "ImageUnit_ImagePipe.h"
#include "ImageUnit_CamDisp.h"
#include "ImageUnit_CamFile.h"
#include "ImageUnit_Photo.h"
#include "ImageUnit_Cap.h"
#include "NVTEvent.h"

/**
    @addtogroup mIImageApp
*/
//@{


/**
     @name Configuration ID for Config().
*/
//@{
#define PHOTO_CFG_POOL          1           ///<  Pool address & size
//@}


/**
    Config settings for Photo mode.

    Config settings for Photo mode, such as PHOTO_CFG_POOL or others.

    @param[in] config_id PHOTO_CFG_POOL or others.
    @param[in] value configuration value.

    @return void

    Example:
    @code
    {
        // config photo memory pool
        Pool.Addr = OS_GetMempoolAddr(POOL_ID_APP);
        Pool.Size = POOL_SIZE_APP;
        ImageApp_CamPhoto_Config(PHOTO_CFG_POOL, (UINT32)&Pool);
    }
    @endcode
*/
extern void ImageApp_CamPhoto_Config(UINT32 config_id, UINT32 value);

/**
    Open photo mode.

    Open photo mode.

    @param[in] void

    @return void
*/
extern void ImageApp_CamPhoto_Open(void);

/**
    Close photo mode.

    Close photo mode.

    @param[in] void

    @return void
*/
extern void ImageApp_CamPhoto_Close(void);

extern UINT32 ImageApp_CamPhoto_CapStart(void);

extern void ImageApp_CamPhoto_CapStop(void);

extern void ImageApp_CamPhoto_CapWaitFinish(void);


//@}
#endif//CAMPHOTO_H

