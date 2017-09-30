/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_NetHTTP.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_NETHTTP_H
#define IMAGEUNIT_NETHTTP_H

/*
Support these input port(s): IN1,IN2

Support these output port(s): OUT1,OUT2

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

enum
{
    NETHTTP_PARAM_START = 0x00003000,
    NETHTTP_PARAM_ENCODER = NETHTTP_PARAM_START,     ///< Set video encoder parameter
    NETHTTP_PARAM_DAEMON,                            ///< Set HTTP live view daemon parameter
    NETHTTP_PARAM_PIP_VIEW,                          ///< Set PIP view parameter
    NETHTTP_PARAM_MAX_IMAGE_WIDTH1,                  ///< Set HTTP input1 image maximum width
    NETHTTP_PARAM_MAX_IMAGE_HEIGHT1,                 ///< Set HTTP input1 image maximum height
    NETHTTP_PARAM_MAX_IMAGE_WIDTH2,                  ///< Set HTTP input2 image maximum width
    NETHTTP_PARAM_MAX_IMAGE_HEIGHT2,                 ///< Set HTTP input2 image maximum height
};


typedef enum {
    NETHTTP_DUALCAM_FRONT = 0,                      ///< FRONT only
    NETHTTP_DUALCAM_BEHIND,                         ///< BEHIND only
    NETHTTP_DUALCAM_BOTH,                           ///< FRONT is major
    NETHTTP_DUALCAM_BOTH2,                          ///< BEHIND is major
    NETHTTP_DUALCAM_SETTING_MAX,
    ENUM_DUMMY4WORD(NETHTTP_DUALCAM_SETTING)
}NETHTTP_DUALCAM_SETTING;


/**
    Init parameters for PIP view.

*/
typedef struct {
    UINT32                  isEnablePIP;            ///<  enable PIP view
    IRECT                   rect2nd;                ///<  second sensor display rectangle
    NETHTTP_DUALCAM_SETTING dualcam;                ///<  dual cam setting
} PIP_VIEW_INFO;

extern ISF_UNIT ISF_NetHTTP;

#endif //IMAGEUNIT_NETHTTP_H

