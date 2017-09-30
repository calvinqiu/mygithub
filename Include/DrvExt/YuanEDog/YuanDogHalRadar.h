#ifndef YUANDOGHALRADAR_H__
#define YUANDOGHALRADAR_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////
// Radar function	
//Radar ��ʼ������
//����:				��
//����ֵ:  
//	1: �ɹ���	0: ʧ�ܣ�
int	YuanHalRadarInit(void);

/////////////////////////////////////////////////////////////////////
// Radar function	
//Radar ��ʼ������
//����:				
//	Code		Uart data
//����ֵ:			��
void YuanHalPraseHITRadar(unsigned char Code);


/////////////////////////////////////////////////////////////////////
// Radar function	
//Radar ��ʼ������
//����:				��
//����ֵ:			��
void YuanHalVerifyHITRadar(void);

#ifdef __cplusplus
}
#endif

#endif