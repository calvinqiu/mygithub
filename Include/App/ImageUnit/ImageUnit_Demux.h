/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_Demux.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_DEMUX_H
#define IMAGEUNIT_DEMUX_H

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

enum
{
    DEMUX_PARAM_START = 0x00008000,
    DEMUX_PARAM_CONNECTTYPE = DEMUX_PARAM_START, //select ISF_CONNECT_PUSH or ISF_CONNECT_PULL, default is ISF_CONNECT_PUSH
    DEMUX_PARAM_CONNECTTYPE2,                    //select ISF_CONNECT_PUSH or ISF_CONNECT_PULL, default is ISF_CONNECT_PUSH
    DEMUX_PARAM_CONNECTTYPE3,                    //select ISF_CONNECT_PUSH or ISF_CONNECT_PULL, default is ISF_CONNECT_PUSH
    DEMUX_PARAM_IN1FROMPORT,
    DEMUX_PARAM_IN2FROMPORT,
    DEMUX_PARAM_IN3FROMPORT,
};

extern ISF_UNIT ISF_Demux;

#endif //IMAGEUNIT_DEMUX_H

