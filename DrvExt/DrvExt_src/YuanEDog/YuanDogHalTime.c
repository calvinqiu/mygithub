#include "YuanDogHalGpio.h"
#include "YuanDogUtils.h"
#include "YuanDogHalEvent.h"
#include "YuanDogHalTime.h"
#include "YuanDogHalLog.h"

#include "../../../Include/Drv/Driver_Sys/rtc.h"
#include "../../../Include/Drv/Driver_Sys/timer.h"
#include "../../../Include/Common/Kernel/ErrorNo.h"

static int			s_TimeTicketInterval	= 200;

unsigned long guiYuanEdogTimerID;

void YuanEdog_TimerHdl()
{
	SetTimerEvent();
}

uint32 YuanHalInitTimer(int32 Interval)
{
  s_TimeTicketInterval = Interval;
  if (timer_open((unsigned int *)&guiYuanEdogTimerID, YuanEdog_TimerHdl) != E_OK)
  {
	
  }
  timer_cfg(guiYuanEdogTimerID, 200*1000, TIMER_MODE_FREE_RUN | TIMER_MODE_ENABLE_INT, TIMER_STATE_PLAY);  
  return 1;
}

void SetTimerEvent()
{
	YUANHAL_EVENT Event = {YUANHAL_EVENT_TICKET,0,0};
	int count=0;
	
	YuanHalSetEvent(&Event);
}
uint32 YuanHalGetTickCount(void)
{
	//HAL_REPLACE: Please add code here

	return HwClock_GetCounter()/1000; /*GetTickCount();*/
}

void YuanHalGetUTCTime(uint32 *Date, uint32* Time)
{
	//skip
	return;
}
void YuanHalSetUTCTime(uint32 *Date, uint32* Time)
{
	//skip
	return ;
}

uint32 YuanHalGetLocalTime(uint32 *Date, uint32* Time)
{       
	uint32 Year, Month, Day, Hour, Minute, Second;
	RTC_DATE rtcDate;
	RTC_TIME rtcTime;
	
	rtcDate = rtc_getDate();
	rtcTime = rtc_getTime();
	//modify it on Novatek;

	Day 	= rtcDate.s.day;
	Month 	= rtcDate.s.month;
	Year	= rtcDate.s.year;
	
	Hour 	= rtcTime.s.hour;
	Minute 	= rtcTime.s.minute;
	Second	= rtcTime.s.second;

	if (Date)	*Date = Day * 10000 + Month  * 100 + Year % 100;
	if (Time)	*Time = Hour * 10000 + Minute * 100 + Second;

	//LOG_HAL_INFO("YuanHalGetLocalTime:(%d/%d/%d-%d:%d:%d)\r\n",Year,Month,Day,Hour, Minute, Second);

	//HAL_REPLACE: Please add code here
	return 1;
}
uint32 YuanHalSetLocalTime(uint32 *Date, uint32* Time)
{
	uint32 Year, Month, Day, Hour, Minute, Second;

	//GetLocalTime(&st);
	//modify it on Novatek;

	if (Date)	
	{
		Day  = (*Date / 10000) % 100; Month  = (*Date / 100) % 100; Year   = (*Date) % 100 + 2000; 
		rtc_setDate(Year,Month,Day);
	}
	if (Time)	
	{
		Hour = (*Time / 10000) % 100; Minute = (*Time / 100) % 100; Second = (*Time) % 100; 
		rtc_setTime(Hour, Minute, Second);
	}	
	rtc_triggerCSET();		
		
	LOG_HAL_INFO("YuanHalSetLocalTime:(%d/%d/%d-%d:%d:%d)\r\n",Year,Month,Day,Hour, Minute, Second);
	//HAL_REPLACE: Please add code here
	return 0;/*SetLocalTime(&st);*/
}
