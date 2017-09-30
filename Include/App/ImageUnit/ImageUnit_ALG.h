/*
    Copyright   Novatek Microelectronics Corp. 2005~2016.  All rights reserved.

    @file       ImageUnit_ALG.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2016/06/07
*/

#ifndef IMAGEUNIT_ALG_H
#define IMAGEUNIT_ALG_H

#include "Type.h"
#include "ImageStream.h"

#define ALG_EVENT_SET_CROP             1

enum
{
    ALG_PARAM_START     = 0x00003000,
    ALG_PARAM_SET_IN1   = ALG_PARAM_START,
    ALG_PARAM_SET_IN2,
    ALG_PARAM_SET_IN3,
    ALG_PARAM_SET_IN4,
    ALG_PARAM_GET_IN1,
    ALG_PARAM_GET_IN2,
    ALG_PARAM_GET_IN3,
    ALG_PARAM_GET_IN4,
};

extern ISF_UNIT ISF_ALG;

#endif //IMAGEUNIT_ALG_H

