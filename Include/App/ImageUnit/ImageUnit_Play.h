/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_Play.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_PLAY_H
#define IMAGEUNIT_PLAY_H

/*
Support these input port(s): 

Support these output port(s): OUT1,OUT2

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

enum
{
    PLAY_PARAM_START = 0x00005000,
};

extern ISF_UNIT ISF_Play;

#endif //IMAGEUNIT_PLAY_H

