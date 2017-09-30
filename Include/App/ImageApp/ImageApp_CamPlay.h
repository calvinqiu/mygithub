/*
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    @file       ImageApp_CamPlay.h
    @ingroup    mIImageApp

    @note       Nothing.

    @date       2014/11/28
*/

#ifndef CAMPLAY_H
#define CAMPLAY_H

#include "GxLib.h"

#include "ImageStream.h"
#include "ImageUnit_Play.h"
#include "ImageUnit_CamDisp.h"

/**
    @addtogroup mIImageApp
*/
//@{

#define PLAY_CFG_POOL          1           ///<  Pool address & size

extern void ImageApp_CamPlay_Config(UINT32 config_id, UINT32 value);

extern void ImageApp_CamPlay_Open(void);
extern void ImageApp_CamPlay_Close(void);

//@}
#endif//CAMPLAY_H
