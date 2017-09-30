#ifndef YUANDOGHALTIME_H__
#define YUANDOGHALTIME_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

//初始化时间
//参数:			
//	Interval		触发YUANHAL_EVENT_TICKET的间隔ms
//返回值:
//返回值:		
//	时钟是否初始化成功
uint32 YuanHalInitTimer(int32 Interval);

//获取时间 ticket 数
//参数:			无
//返回值:
//	返回系统ticket count；
uint32 YuanHalGetTickCount(void);

//获取UTC时间
//参数:
//	Date		日期：DDMMYY
//	Time		时间: HHMMSS
//返回值:		无
void YuanHalGetUTCTime(uint32 *Date, uint32* Time);

//设置UTC时间
//参数:
//	Date		日期：DDMMYY
//	Time		时间: HHMMSS
//返回值:		无
void YuanHalSetUTCTime(uint32 *Date, uint32* Time);

//获取本地时间
//参数:
//	Date		日期：DDMMYY
//	Time		时间: HHMMSS
//返回值:		无
uint32 YuanHalGetLocalTime(uint32 *Date, uint32* Time);

//设置本地时间
//参数:
//	Date		日期：DDMMYY
//	Time		时间: HHMMSS
//返回值:		无
uint32 YuanHalSetLocalTime(uint32 *Date, uint32* Time);


//设置时间EVENT
//参数:  无
//返回值:		无
void SetTimerEvent(void);

#ifdef __cplusplus
}
#endif

#endif