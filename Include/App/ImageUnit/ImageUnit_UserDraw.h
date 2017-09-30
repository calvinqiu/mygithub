/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_UserDraw.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_USERDRAW_H
#define IMAGEUNIT_USERDRAW_H

/*
Support these input port(s): IN1,IN2,IN3,IN4

Support these output port(s): OUT1,OUT2,OUT3,OUT4

Support these port config API(s):

============================================================
port                        IN1,IN2
------------------------------------------------------------
ImageUnit_CfgImgSize()      [Y]
ImageUnit_CfgImgAspect()    [Y]
ImageUnit_CfgImgWindow()    
ImageUnit_CfgImgDirect()    
============================================================
*/

#include "Type.h"
#include "ImageStream.h"
#include "AppDisp.h"

enum
{
    USERDRAW_PARAM_START = 0x00003000,
    USERDRAW_PARAM_ONDRAW_FUNC = USERDRAW_PARAM_START,
    USERDRAW_PARAM_MAX_WIDTH,   //default = 0, for auto gather max size of input-1
    USERDRAW_PARAM_MAX_HEIGHT,  //default = 0, for auto gather max size of input-1
    USERDRAW_PARAM_MAX_FMT,     //default = 0, for auto gather max size of input-1
    USERDRAW_PARAM_MAX_WIDTH2,  //default = 0, for auto gather max size of input-2
    USERDRAW_PARAM_MAX_HEIGHT2, //default = 0, for auto gather max size of input-2
    USERDRAW_PARAM_MAX_FMT2,    //default = 0, for auto gather max size of input-2
    USERDRAW_PARAM_SCALE_WIDTH,   //default = 0x00010001, for 1X (set to 0, for off), for assign scale factor of input-1
    USERDRAW_PARAM_SCALE_HEIGHT,  //default = 0x00010001, for 1X (set to 0, for off), for assign scale factor of input-1
    USERDRAW_PARAM_SCALE_WIDTH2,   //default = 0x00010001, for 1X (set to 0, for off), for assign scale factor of input-2
    USERDRAW_PARAM_SCALE_HEIGHT2,  //default = 0x00010001, for 1X (set to 0, for off), for assign scale factor of input-2
    USERDRAW_PARAM_MAX_WIDTH3,   //default = 0, for auto gather max size of input-3
    USERDRAW_PARAM_MAX_HEIGHT3,  //default = 0, for auto gather max size of input-3
    USERDRAW_PARAM_MAX_FMT3,     //default = 0, for auto gather max size of input-3
    USERDRAW_PARAM_MAX_WIDTH4,  //default = 0, for auto gather max size of input-4
    USERDRAW_PARAM_MAX_HEIGHT4, //default = 0, for auto gather max size of input-4
    USERDRAW_PARAM_MAX_FMT4,    //default = 0, for auto gather max size of input-4
    USERDRAW_PARAM_SCALE_WIDTH3,   //default = 0x00010001, for 1X (set to 0, for off), for assign scale factor of input-3
    USERDRAW_PARAM_SCALE_HEIGHT3,  //default = 0x00010001, for 1X (set to 0, for off), for assign scale factor of input-3
    USERDRAW_PARAM_SCALE_WIDTH4,   //default = 0x00010001, for 1X (set to 0, for off), for assign scale factor of input-4
    USERDRAW_PARAM_SCALE_HEIGHT4,  //default = 0x00010001, for 1X (set to 0, for off), for assign scale factor of input-4
};

//NOTE: this unit's ISF_IN1 will gather max size from three places: (1) parameter (1) ISF_OUT1 unit (2) ISF_OUT2 unit
//NOTE: this unit's ISF_IN2 will gather max size from three places: (1) parameter (1) ISF_OUT2 unit (2) ISF_OUT1 unit
//NOTE: this unit's ISF_IN3 will gather max size from three places: (1) parameter (1) ISF_OUT3 unit (2) ISF_OUT4 unit
//NOTE: this unit's ISF_IN4 will gather max size from three places: (1) parameter (1) ISF_OUT4 unit (2) ISF_OUT3 unit

#define USERDRAW_CROP_X(x, w)    (0x80000000 | ((x)<<16) | ((w)))
#define USERDRAW_CROP_Y(y, h)    (0x80000000 | ((y)<<16) | ((h)))

extern ISF_UNIT ISF_UserDraw;

#endif //IMAGEUNIT_USERDRAW_H

