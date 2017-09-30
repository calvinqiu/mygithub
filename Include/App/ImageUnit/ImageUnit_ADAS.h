/*
    Copyright   Novatek Microelectronics Corp. 2005~2016.  All rights reserved.

    @file       ImageUnit_ADAS.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2016/01/14
*/

#ifndef IMAGEUNIT_ADAS_H
#define IMAGEUNIT_ADAS_H

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

#define ADAS_EVENT_SET_CROP             1

enum
{
    ADAS_PARAM_START = 0x00003000,
    ADAS_PARAM_REG_CB = ADAS_PARAM_START,
    ADAS_PARAM_P3_INIT,
    ADAS_PARAM_ENABLE,
};

typedef void (ADASCallbackType)(UINT32 event_id, void *Data);

extern ISF_UNIT ISF_ADAS;

#endif //IMAGEUNIT_ADAS_H

