/**
    AutoVP Parameters library.

    @file       autoVP_Parms.h
    @ingroup    mILibAutoVP

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/
#ifndef _AUTO_VP_PARMS_H
#define _AUTO_VP_PARMS_H


#include "autoVP_alg.h"

/**
     @name AutoVP_PARMS return status
*/
//@{
#define AUTO_VP_SIZE_WIDTH                      (640)
#define AUTO_VP_SIZE_HEIGHT                     (480)
#define AUTO_VP_PARMS_STA_OK                    (0)                   ///<  the status is ok, for normal case
#define AUTO_VP_PARMS_STA_ERROR                 (-1)                  ///<  some error occurred
//@}

/**
     AutoVP detection callback message.
*/
//@{
typedef enum _AUTO_VP_CBMSG
{
    AUTO_VP_CBMSG_WARNNING = 0,       ///< user will receive this notification when the lane departure warning occur
    AUTO_VP_CBMSG_MAX_CNT,            ///< total callback message numbers
    ENUM_DUMMY4WORD(AUTO_VP_CBMSG)
} AUTO_VP_CBMSG;
//@}

/**
     Calculate AutoVP detection required buffer size.

     @return the required buffer size.
*/
extern UINT32 AutoVPParms_CalcBuffSize(void);

extern UINT32 AutoVPParms_CalcCacheBuffSize(void);

/**
     Init AutoVP Detect engine.

     This function is used to initialize smile detection buffer and set the process rate.
*/
extern INT32 AutoVPParms_Init(MEM_RANGE *buf, MEM_RANGE *cacheBuf, UINT32 ProcessRate, BOOL DebugFlag);

/**
     Uninit AutoVP engine.

     This function is used to release AutoVP Detect engine.
*/
extern INT32 AutoVPParms_UnInit(void);

/**
     Process the AutoVP detection.

     This function will find simles in the result of AutoVP detection.
     and then detect if any AutoVP
*/
extern INT32 AutoVPParms_Process(void);

/**
    Get AutoVP Results
*/
extern AUTO_VP_RESULT_INFO AutoVPParms_getAutoVPRst(void);

/**
    Get AutoVP Source
*/
extern AUTO_VP_SRCIMG_INFO AutoVPParms_getAutoVPSrc(void);

/**
    Get AutoVP Debug information
*/
extern AUTO_VP_DEBUG_INFO AutoVPParms_getAutoVPDbg(void);

#endif

