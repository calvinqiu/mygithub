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

//��ʼ�� EVENT
//����:				��
//����ֵ:			��
void YuanHalEventInit(void);

//���� EVENT
//����:
//	pEvent			EVENT �¼�
//����ֵ:
//	1: �ɹ��� 0: ʧ�ܣ�
int YuanHalSetEvent(YUANHAL_EVENT *pEvent);

//��ȡ EVENT
//����:
//	pEvent			EVENT �¼�
//����ֵ:
//	1: �ɹ��� 0: ʧ�ܣ�
int YuanHalGetEvent(YUANHAL_EVENT *pEvent);

//�ȴ� EVENT
//����:
//	milliseconds			timeout ʱ�䣨���룩
//����ֵ:
//	YUANHAL_EVENT_SET: EVENT �¼��� YUANHAL_EVENT_OTHER: �����¼���
int YuanHalWaitForEvent(uint32 milliseconds);

#ifdef __cplusplus
}
#endif

#endif