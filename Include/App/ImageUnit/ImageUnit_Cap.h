/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_Cap.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_CAP_H
#define IMAGEUNIT_CAP_H

/*
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

#include "ImgCaptureAPI.h" //for capture parameters
#include "ImgCapInfor.h" //for capture parameters
#include "ImgCapCBMsg.h" //for capture parameters

enum
{
    CAP_PARAM_START = 0x80003000,
    CAP_PARAM_BUFSIZE = CAP_PARAM_START,
    CAP_PARAM_CAP_CB,      // register capture callback
    CAP_PARAM_DETS2_CB,    // register detect s2 callback
    CAP_PARAM_IMGASPECT_W,   // crop image aspect ratio w
    CAP_PARAM_IMGASPECT_H,   // crop image aspect ratio h
    CAP_PARAM_WAITTSK,
};

extern ISF_UNIT ISF_Cap;

#endif //IMAGEUNIT_CAP_H
