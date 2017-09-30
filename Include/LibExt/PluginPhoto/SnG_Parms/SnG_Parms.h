/**
    ADAS detection library.

    @file       adas_lib.h
    @ingroup    mILibADAS

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _SNGPARMS_LIB_H
#define _SNGPARMS_LIB_H

/**
    @addtogroup mILibADAS
*/

#include "Type.h"
#include "ldws_lib.h"
#include "fcws_lib.h"
#include "ime_lib.h"
#include "IPL_Cmd.h"
#include "SnG_lib.h"
/**
     @name ADASreturn status
*/
//@{
#define SNGPARMS_STA_OK                  (0)                   ///<  the status is ok, for normal case
#define SNGPARMS_STA_ERROR               (-1)                  ///<  some error occurred
//@}
#define SNG_DEBUG                        (0)

extern UINT32 SnGParms_CalcBuffSize(void);
/**
     Process the SnG detection.
*/
extern INT32 SnGParms_Process(void);
/**
     Init ADAS Detect engine.
*/
extern INT32 SnGParms_Init(MEM_RANGE *buf, MEM_RANGE *cachebuf, UINT32 ProcessRate);
/**
    SnG detection function
*/
extern void SnGParms_Detection(void);

/**
    Set save flag
*/
extern void SetSaveDisMvFlg(UINT32 flag);
#endif

