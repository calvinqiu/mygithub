#ifndef YUANDOGHALTIME_H__
#define YUANDOGHALTIME_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

//��ʼ��ʱ��
//����:			
//	Interval		����YUANHAL_EVENT_TICKET�ļ��ms
//����ֵ:
//����ֵ:		
//	ʱ���Ƿ��ʼ���ɹ�
uint32 YuanHalInitTimer(int32 Interval);

//��ȡʱ�� ticket ��
//����:			��
//����ֵ:
//	����ϵͳticket count��
uint32 YuanHalGetTickCount(void);

//��ȡUTCʱ��
//����:
//	Date		���ڣ�DDMMYY
//	Time		ʱ��: HHMMSS
//����ֵ:		��
void YuanHalGetUTCTime(uint32 *Date, uint32* Time);

//����UTCʱ��
//����:
//	Date		���ڣ�DDMMYY
//	Time		ʱ��: HHMMSS
//����ֵ:		��
void YuanHalSetUTCTime(uint32 *Date, uint32* Time);

//��ȡ����ʱ��
//����:
//	Date		���ڣ�DDMMYY
//	Time		ʱ��: HHMMSS
//����ֵ:		��
uint32 YuanHalGetLocalTime(uint32 *Date, uint32* Time);

//���ñ���ʱ��
//����:
//	Date		���ڣ�DDMMYY
//	Time		ʱ��: HHMMSS
//����ֵ:		��
uint32 YuanHalSetLocalTime(uint32 *Date, uint32* Time);


//����ʱ��EVENT
//����:  ��
//����ֵ:		��
void SetTimerEvent(void);

#ifdef __cplusplus
}
#endif

#endif