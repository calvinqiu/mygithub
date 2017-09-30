/*
    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.

    @file       ImageApp_UsbPhoto.h
    @ingroup    mIImageApp

    @note       Nothing.

    @date       2014/11/28
*/

#ifndef USBPHOTO_H
#define USBPHOTO_H

#include "GxLib.h"

#include "ImageStream.h"
#include "ImageUnit_ImagePipe.h"
#include "ImageUnit_UsbUVAC.h"
#include "ImageUnit_Photo.h"
#include "ImageUnit_Cap.h"
#include "ImageUnit_Media.h"
#include "NVTEvent.h"

/**
    @addtogroup mIImageApp
*/
//@{


extern void ImageApp_UsbPhoto_Open(void);
extern void ImageApp_UsbPhoto_Close(void);


//@}
#endif//USBPHOTO_H
