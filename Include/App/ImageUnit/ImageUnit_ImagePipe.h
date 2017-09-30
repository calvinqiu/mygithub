/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_ImagePipe.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_IMAGEPIPE_H
#define IMAGEUNIT_IMAGEPIPE_H

/*
For ImageUnit_ImagePipe

Support these input port(s): IN1

Support these output port(s): OUT1,OUT2,OUT3,OUT4
*/

/*
For ImageUnit_ImagePipe1, ImageUnit_ImagePipe2, ImageUnit_ImagePipe3, ImageUnit_ImagePipe4

Support these input port(s): IN1

Support these output port(s): OUT1,OUT2,OUT3,OUT4,OUTC

Support these port config API(s):

============================================================
port                        IN1         OUT1,OUT2,OUT3,OUT4
------------------------------------------------------------
ImageUnit_CfgImgSize()                  [Y]
ImageUnit_CfgImgAspect()    [Y]
ImageUnit_CfgImgWindow()    [Y](*)      [Y](**)
ImageUnit_CfgImgDirect()    [Y]
============================================================
(*) set SIE input crop window
(**) set IME output crop window

*/

#include "Type.h"
#include "ImageStream.h"
#include "IPL_CBMsg.h"
#include "IPL_AlgInfor.h" //for imagepipe parameters
#include "IPL_Utility.h" //for imagepipe parameters
#include "IPL_CBMsg.h" //for imagepipe parameters

#include "ImageUnit_Photo.h"

enum
{
    IMAGEPIPE_PARAM_START = 0x80001000,
    IMAGEPIPE_PARAM_SENSORCOUNT = IMAGEPIPE_PARAM_START,
    IMAGEPIPE_PARAM_SENSORATTACH,
    IMAGEPIPE_PARAM_SENSORDETACH,
    IMAGEPIPE_PARAM_IPL_CB = IMAGEPIPE_PARAM_START + 0x10,
    IMAGEPIPE_PARAM_SETMODE,    // allow SetMode by other Unit
    IMAGEPIPE_PARAM_WAITBUFRDY,  // wait buffer ready
    IMAGEPIPE_PARAM_LOCKAE,    // (for ImagePipeN)
    IMAGEPIPE_PARAM_LOCKAWB,    // (for ImagePipeN)
    IMAGEPIPE_PARAM_LOCKAF,    // (for ImagePipeN)
    IMAGEPIPE_PARAM_LOCKCAF,    // (for ImagePipeN)
    IMAGEPIPE_PARAM_WAITAE,    // (for ImagePipeN)
    IMAGEPIPE_PARAM_WAITAWB,    // (for ImagePipeN)
    IMAGEPIPE_PARAM_WAITAF,    // (for ImagePipeN)
    IMAGEPIPE_PARAM_WAITCAF,    // (for ImagePipeN)
    IMAGEPIPE_PARAM_DIRECT2H264CB,    // Register CB function for DIRECT2H264
    IMAGEPIPE_PARAM_DIRECT2H264TG,    // Trigger of DIRECT2H264
    IMAGEPIPE_PARAM_SENSORDISPLAY,
    IMAGEPIPE_PARAM_SETBUFCOUNT,  // set buffer count
    IMAGEPIPE_PARAM_SENSORMASK,  // mask
    IMAGEPIPE_PARAM_PULLBUF_OUT1, // (for ImagePipeN)
    IMAGEPIPE_PARAM_PULLBUF_OUT2, // (for ImagePipeN)
    IMAGEPIPE_PARAM_PULLBUF_OUT3, // (for ImagePipeN)
    IMAGEPIPE_PARAM_PULLBUF_OUT4, // (for ImagePipeN)
    IMAGEPIPE_PARAM_SBSVIEW_EN, // Enable side-by-side view for VR-CAM & 360-CAM
    IMAGEPIPE_PARAM_DIRECT2H264SNAPSHOT,    // Start snapshot for DIRECT2H264
    IMAGEPIPE_PARAM_DIRECT2H264SNAPSHOT_STOP,    // Stop snapshot for DIRECT2H264
    IMAGEPIPE_PARAM_AUTOIPLOFF_EN, // auto off another IPL for reduce DMA B/W (NOTE: FastPipView swicth to "Both" will not work)
    IMAGEPIPE_PARAM_SENSORMAP, // sensor mapping
    IMAGEPIPE_PARAM_SBSVIEW_MODE, // Set Mode of side-by-side view for VR-CAM & 360-CAM (LR or UD)
};

#define IPL_MODE_NOWAIT        0x80000000
#define IPL_MODE_DIRECT2H264   0x40000000   ///< Enable DIRECT2H264

#define IPL_DIR_MIRRORX        0x00000001   ///< Mirror in X direction
#define IPL_DIR_MIRRORY        0x00000002   ///< Mirror in Y direction

#define IPL_SBS_LR             0x00000001   ///< Left/Right side-by-side mode
#define IPL_SBS_UD             0x00000002   ///< Up/Down side-by-side mode

// Sensor mask
#define SENSOR_1                        0x00000001
#define SENSOR_2                        0x00000002
#define SENSOR_3                        0x00000004
#define SENSOR_4                        0x00000008
#define SENSOR_ALL                      0xffffffff

extern ISF_UNIT ISF_ImagePipe;

extern ISF_UNIT ISF_ImagePipe1;
extern ISF_UNIT ISF_ImagePipe2;
extern ISF_UNIT ISF_ImagePipe3;
extern ISF_UNIT ISF_ImagePipe4;

typedef void (*IPL_CB_FP)(IPL_CBMSG uiMsgID, void *Data);

#endif //IMAGEUNIT_IMAGEPIPE_H

