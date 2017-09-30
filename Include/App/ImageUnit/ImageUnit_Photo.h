/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_Photo.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_PHOTO_H
#define IMAGEUNIT_PHOTO_H

/*
Support these input port(s): 

Support these output port(s): OUT1

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

#include "PhotoTask.h" //for photo parameters

enum
{
    PHOTO_PARAM_START = 0x80002000,
    PHOTO_PARAM_CACHESIZE = PHOTO_PARAM_START,  ///< Set photo cache size
    PHOTO_PARAM_FUNCINFO,                       ///< Set photo hook function list
    PHOTO_PARAM_SHDR_EN,                        ///< Set Sensor HDR function enable/disable
    PHOTO_PARAM_WDR_EN,                         ///< Set WDR function enable/disable
    PHOTO_PARAM_ADAS_EN,                        ///< Set ADAS function enable/disable
    PHOTO_PARAM_WAITTSK1,                       ///< Wait photo task1 idle
    PHOTO_PARAM_WAITTSK2,                       ///< Wait photo task2 idle
    PHOTO_PARAM_WAITTSK3,                       ///< Wait photo task3 idle
    PHOTO_PARAM_FUNC1,                          ///< Set IPL1 function enable/disable
    PHOTO_PARAM_FUNC2,                          ///< Set IPL2 function enable/disable
    PHOTO_PARAM_FUNC3,                          ///< Set IPL3 function enable/disable
    PHOTO_PARAM_FUNC4,                          ///< Set IPL4 function enable/disable
    PHOTO_PARAM_VR360_1,                        ///< Set IPL1 parameter of VR360
    PHOTO_PARAM_VR360_2,                        ///< Set IPL2 parameter of VR360
};

extern ISF_UNIT ISF_Photo;

/**
    IPL change display size callback function description.

*/
typedef void (*CHGDISPSIZEINFOR_CB_FP)(void);


#endif //IMAGEUNIT_PHOTO_H
