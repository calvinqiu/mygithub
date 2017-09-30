#ifndef YUANDOGHALEVENT_H__
#define YUANDOGHALEVENT_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

#define YUANHAL_EVENT_OTHER		0
#define YUANHAL_EVENT_TIMEOUT	1
#define YUANHAL_EVENT_SET		2

#define YUANHAL_EVENT_TTS		4
#define YUANHAL_EVENT_GPS_COM	5
#define YUANHAL_EVENT_DVR_COM	6
#define YUANHAL_EVENT_TICKET	7
#define YUANHAL_EVENT_CLOUD		8
#define YUANHAL_EVENT_KEYPAD	9
#define YUANHAL_EVENT_RADAR		10
#define YUANHAL_EVENT_GSMNOTIFY	11
#define YUANHAL_EVENT_SHUTDOWN	100
#define YUANHAL_EVENT_ADVGPS    12

typedef struct  
{
	uint32	EventCode;
	uint32  wParam;
	uint32	lParam;
}YUANHAL_EVENT;

//初始化 EVENT
//参数:				无
//返回值:			无
void YuanHalEventInit(void);

//设置 EVENT
//参数:
//	pEvent			EVENT 事件
//返回值:
//	1: 成功； 0: 失败；
int YuanHalSetEvent(YUANHAL_EVENT *pEvent);

//获取 EVENT
//参数:
//	pEvent			EVENT 事件
//返回值:
//	1: 成功； 0: 失败；
int YuanHalGetEvent(YUANHAL_EVENT *pEvent);

//等待 EVENT
//参数:
//	milliseconds			timeout 时间（毫秒）
//返回值:
//	YUANHAL_EVENT_SET: EVENT 事件； YUANHAL_EVENT_OTHER: 其他事件；
int YuanHalWaitForEvent(uint32 milliseconds);

#ifdef __cplusplus
}
#endif

#endif