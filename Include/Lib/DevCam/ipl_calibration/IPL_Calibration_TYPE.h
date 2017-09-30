#ifndef _IPL_CALIBRATION_TYPE_H_
#define _IPL_CALIBRATION_TYPE_H_
/**
    Calibration Type setting (Normal/Web/USB/...)

    set calibration type.

    @file       IPL_Calibration_TYPE.h
    @ingroup    mILibCal

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/

/**
    calibration type
*/
typedef enum
{
    CAL_TYPE_Normal  = 0,
    CAL_TYPE_WEB     = 1,    ///< use web mode (no SD card & panel)
    CAL_TYPE_USB     = 2,    ///< use usb mode (no panel)
    ENUM_DUMMY4WORD(CAL_TYPE)
}CAL_TYPE;

/**
    calibration type operation structure
    @note plz refer to Cal_TypeEvent()
*/
typedef struct _CAL_TYPE_OBJ
{
    ER          (*fpLockChgType)(void); ///< lock calibration type change
    ER          (*fpUnlockChgType)(void); ///< unlock calibration type change
    ER          (*fpSetType)(CAL_TYPE CalType); ///< set calibration type
    CAL_TYPE    (*fpGetType)(void); ///< get calibration type
}CAL_TYPE_OBJ,*pCAL_TYPE_OBJ;

/**
     calibration type operation


     @return Description of data returned.
         - @b fpLockChgType: lock calibration type change
         - @b fpUnlockChgType: unlock calibration type change
         - @b fpSetType: set calibration type
         - @b fpGetType: get calibration type
*/
extern pCAL_TYPE_OBJ Cal_TypeEvent(void);
//@}
#endif
