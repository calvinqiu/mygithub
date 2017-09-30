/*
    Copyright   Novatek Microelectronics Corp. 2005~2014.  All rights reserved.

    @file       ImageUnit_StreamSender.h
    @ingroup    mImagePipe

    @note       Nothing.

    @date       2014/10/01
*/

#ifndef IMAGEUNIT_STREAMSENDER_H
#define IMAGEUNIT_STREAMSENDER_H

/*
Support these input port(s): IN1,IN2,IN3

Support these output port(s): 

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

#define STREAMSENDER_CHAN_NUM       3

enum
{
    NETSTREAM_PARAM_START = 0x80008000,                     ///< a non-zero arbitrary value
    NETSTREAM_PARAM_SLOTNUM_AUDIO = NETSTREAM_PARAM_START,  ///< The slot number of audio stream data
    NETSTREAM_PARAM_SLOTNUM_VIDEO,                          ///< The slot number of video stream data
    NETSTREAM_PARAM_SET_AVGSIZE_GOPNUM,                     ///< Set the GOP number of average size
    NETSTREAM_PARAM_GET_AVGSIZE_CH00,                       ///< Get average size of channel 00
    NETSTREAM_PARAM_GET_AVGSIZE_CH01,                       ///< Get average size of channel 01
    NETSTREAM_PARAM_GET_AVGSIZE_CH02,                       ///< Get average size of channel 02
};

extern ISF_UNIT ISF_StreamSender;

#endif //IMAGEUNIT_STREAMSENDER_H

