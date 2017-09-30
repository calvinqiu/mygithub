/**
    HW clock utility.

    HW clock utility.

    @file       HwClock.h
    @ingroup    mIUtilHWClock
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _HWCLOCK_H
#define _HWCLOCK_H

#include "Type.h"
#include <time.h>

/**
    @addtogroup mIUtilHWClock
*/
//@{

/**
     Init system timer

     Init system timer of counter and longcounter, this API will be called when 1st task start
*/
extern void HwClock_Init(void);

/**
     Get system counter

     Get system counter value

     @return Counter value, bit 31~0: us, NOTE: it will overflow after 71.58 min!
*/
extern UINT32 HwClock_GetCounter(void);

/**
     Get system counter

     Get system long counter value

     @return LongCounter value, bit 63~32: sec, bit 31~0: us, NOTE: it will overflow after 136.10 year!
*/
extern UINT64 HwClock_GetLongCounter(void);


#define TIME_ID_CURRENT     1 ///< current time
#define TIME_ID_ALARM       2 ///< alarm time => event callback function.
#define TIME_ID_HWRT        3 ///< hw reset time (by power alarm timer)
#define TIME_ID_SWRT        4 ///< sw reset counter (by watch dog timer)


/**
     Get time

     Get time of timeID

     @param[in] timeID      time index.

     @return get time.
*/
struct tm HwClock_GetTime(UINT32 timeID);

/**
     Set time

     Set time of timeID

     @param[in] timeID      time index.
     @param[in] ctv         set time.
     @param[in] param       paramters.

     EX:
     HwClock_SetTime(TIME_ID_CURRENT, ctv, 0);
     HwClock_SetTime(TIME_ID_ALARM, ctv, (UINT32)MyAlaramCB);
     HwClock_SetTime(TIME_ID_HWRT, ctv, 0);
     HwClock_SetTime(TIME_ID_SWRT, ctv, 0);
*/
void HwClock_SetTime(UINT32 timeID, struct tm ctv, UINT32 param);

/**
     Calculate maximum days number of given (year, month).

     Calculate maximum days number of given (year, month).

     @param[in] year      Year.
     @param[in] month     Month

     @return Days number.
*/
extern INT32 TimeUtil_CalcMonthDays(INT32 year, INT32 month);

/**
     Convert date-time value to days number.

     Convert date-time value to days number.

     @param[in] ctv     Date time value.

     @return Days number. [second precison]
*/
extern UINT32 TimeUtil_Value2Days(struct tm ctv);

/**
     Convert days number to date-time value.

     Convert days number to date-time value.

     @param[in] days    Days number.

     @return Date time value. [day precison]
*/
extern struct tm TimeUtil_Days2Value(UINT32 days);

/**
     Calculate sum of date-time value and another differece

     Calculate sum of date-time value and another differece

     @param[in] ctv     Date time value.
     @param[in] diff    Difference (date time value).

     @return Sum (date time value). [second precison]
*/
extern struct tm TimeUtil_ValueAddDiff(struct tm ctv, struct tm diff);

/**
    Install uart command to SxCmd

    Install uart command to SxCmd
*/
extern void HwClock_InstallCmd(void);

/**
    Install uart command to SxCmd (internal API)

    Install uart command to SxCmd (internal API)
*/
extern void HwClk_InstallCmd(void);

//@}

#endif //_HWCLOCK_H
