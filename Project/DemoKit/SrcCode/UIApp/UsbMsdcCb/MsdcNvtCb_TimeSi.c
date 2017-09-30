//#NT#2016/06/13#Niven Cho -begin
//#NT#TIME_SYNC
#include "SysKer.h"
#include "MsdcNvtApi.h"
#include "MsdcNvtCb_CustomSi.h"
#include "SysCfg.h"
#include "rtc.h"

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
            ((UINT32)(UINT8)(ch0) | ((UINT32)(UINT8)(ch1) << 8) |   \
            ((UINT32)(UINT8)(ch2) << 16) | ((UINT32)(UINT8)(ch3) << 24 ))
#endif //defined(MAKEFOURCC)

#define FOURCC_TIME MAKEFOURCC('T','I','M','E')

typedef struct _MSDCNVT_TIME{
    UINT32 uiFourCC; //Key for 'TIME'
    UINT32 uiYear;
    UINT32 uiMonth;
    UINT32 uiDay;
    UINT32 uiHour;
    UINT32 uiMin;
    UINT32 uiSec;
}MSDCNVT_TIME,*PMSDCNVT_TIME;

//Step 1: Declare custom functions
//Declare Gets Functions
static void GetTime(void);
//Declare Sets Functions
static void SetTime(void);

//Step 2: Create your function mapping table for 'Get' Command
static void (*MSDCCustomGet[])(void)=
{
        GetTime,
};

//Step 3: Create your function mapping table for 'Set' Command
static void (*MSDCCustomSet[])(void)=
{
        SetTime,
};

//Step 4: Provide API for Register Single Direction Functions
BOOL MsdcNvtRegSi_TimeSi(void)
{
    return MsdcNvt_AddCallback_Si(MSDCCustomGet,sizeof(MSDCCustomGet)/sizeof(MSDCCustomGet[0]),MSDCCustomSet,sizeof(MSDCCustomSet)/sizeof(MSDCCustomSet[0]));
}

//Step 5: Start to implement your custom function
static void GetTime(void)
{
    RTC_TIME rtcTime;
    RTC_DATE rtcDate;
    MSDCNVT_TIME *pData    = (MSDCNVT_TIME*)MsdcNvt_GetDataBufferAddress();
    UINT32   uiLength = MsdcNvt_GetTransSize();

    if(uiLength!=sizeof(MSDCNVT_TIME)) //invalid data
        return;

    rtcTime = rtc_getTime();
    rtcDate = rtc_getDate();

    pData->uiFourCC = FOURCC_TIME;
    pData->uiYear = rtcDate.s.year;
    pData->uiMonth = rtcDate.s.month;
    pData->uiDay = rtcDate.s.day;
    pData->uiHour = rtcTime.s.hour;
    pData->uiMin = rtcTime.s.minute;
    pData->uiSec = rtcTime.s.second;
}

static void SetTime(void)
{
    MSDCNVT_TIME* pData    = (MSDCNVT_TIME*)MsdcNvt_GetDataBufferAddress();
    UINT32  uiLength = MsdcNvt_GetTransSize();

    if(uiLength!=sizeof(MSDCNVT_TIME)) //invalid data
        return;

    if(pData->uiFourCC != FOURCC_TIME)
        return;

    rtc_setDate(pData->uiYear,pData->uiMonth,pData->uiDay);
    rtc_setTime(pData->uiHour,pData->uiMin,pData->uiSec);
    rtc_triggerCSET();
    rtc_waitCSETDone();
}
//#NT#2016/06/13#Niven Cho -end