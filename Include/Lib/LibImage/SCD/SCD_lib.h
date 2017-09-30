/**
    SCD (Scene Change Detection) library functions.

    Detecting scene change of IP camera (malice rotation of camera).

    @file       SCD_lib.h
    @ingroup    SCD
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#ifndef __SCD_LIB_H__
#define __SCD_LIB_H__

#include "Type.h"

/**
    Parameters of SCD processing.
*/
typedef enum
{
    SCD_SET_X_THRESH = 0,   ///< alarm threshold of x motion
    SCD_SET_Y_THRESH,       ///< alarm threshold of y motion
    SCD_SET_FRM_THRESH,     ///< alarm threshold of scene-change frame count

    ENUM_DUMMY4WORD(SCD_SET)
} SCD_SET;

/**
    Parameters of SCD_Process function.
*/
typedef struct
{
    INT32 iX;   ///< x component of motion vector
    INT32 iY;   ///< y component of motion vector
} SCD_PROC_PRMS;

/**
    Set one of the parameters defined by SCD_SET.

    @param[in] iSet: parameter type
    @param[in] uiVal: parameter value
*/
VOID SCD_SetPrms(SCD_SET iSet, UINT32 uiVal);

/**
    Get one of the parameters defined by SCD_SET.

    @param[in] iSet: parameter type

    @return parameter value
*/
UINT32 SCD_GetPrms(SCD_SET iSet);

/**
    Get detection result.

    @return whether SCD alarm is triggered or not
        - @b TRUE: is triggered
        - @b FALSE: is not triggered
*/
BOOL SCD_GetAlarm(VOID);

/**
    Initiate the SCD function.

    @note Parameters are reset to default values.
*/
VOID SCD_Init(VOID);

/**
    Request SCD to process an frame.

    @param[in] pPrms: processing parameters
*/
VOID SCD_Process(SCD_PROC_PRMS *pPrms);

/**
    Reset SCD status.

    Parameters are reset to default values.
    Scene-change frame count is reset to zero.
    Alarm flag is reset to zero.
*/
VOID SCD_Reset(VOID);


#endif  //__SCD_LIB_H__
