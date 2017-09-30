/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_CamDisp.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_CAMDISP_H
#define IMAGEUNIT_CAMDISP_H

/*
Support these port config API(s):

============================================================
port                        IN1,IN2
------------------------------------------------------------
ImageUnit_CfgImgSize()      [Y]
ImageUnit_CfgImgAspect()    [Y]
ImageUnit_CfgImgWindow()    [Y](*)
ImageUnit_CfgImgDirect()    
============================================================
(*) set IDE output cast window

TODO: if ImagePipe1.out is connected to CamDisp.in
- if assign ImagePipe1's IME crop window, it will also affect CamDisp's IDE cast window?

*/

#include "Type.h"
#include "ImageStream.h"
#include "AppDisp.h"

enum
{
    CAMDISP_PARAM_START = 0x00003000,
    CAMDISP_PARAM_DISPCOUNT = CAMDISP_PARAM_START,
    CAMDISP_PARAM_DISPATTACH,
    CAMDISP_PARAM_DISPDETACH,
    CAMDISP_PARAM_DOUT1,
    CAMDISP_PARAM_DOUT2,
    CAMDISP_PARAM_VIEWSRC,
    CAMDISP_PARAM_DISPMASK,
    CAMDISP_PARAM_FORMAT1,
    CAMDISP_PARAM_FORMAT2,
    CAMDISP_PARAM_VIEWCB,
    CAMDISP_PARAM_DISPDIRECT,
    CAMDISP_PARAM_SBSVIEW_EN,
};

extern ISF_UNIT ISF_CamDisp;

#endif //IMAGEUNIT_CAMDISP_H

