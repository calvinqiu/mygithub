#ifndef YUANDOGHALGPIO_H__
#define YUANDOGHALGPIO_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

	
//ȡ���ӹ����ݿ������ļ�����
//����:			��
//����ֵ:		��
void YuanHalGpioInit(void);

//����GPIO
//����:
//	Port		�˿ں�
//	Pin			���ź�
//����ֵ:		��
void YuanHalGpioSet(uint32 Port, uint32 Pin);

//��GPIO
//����:
//	Port		�˿ں�
//	Pin			���ź�
//����ֵ:		��
void YuanHalGpioClear(uint32 Port, uint32 Pin);

#ifdef __cplusplus
}
#endif

#endif