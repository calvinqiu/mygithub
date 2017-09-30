#ifndef YUANDOGHALGPSCOM_H__
#define YUANDOGHALGPSCOM_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C"{
#endif 
/////////////////////////////////////////////////////////////////////
// global variable
extern int g_GPSComPort;
extern int g_GPSBandRate;

/////////////////////////////////////////////////////////////////////
// global function

//��ʼ�� GPS ����
//����:				��
//����ֵ:			��
void YuanHalGPSInit(void);

//GPS ���ڴ�״̬
//����:				��
//����ֵ:			
//	1 �ɹ���	0 ʧ��;
int YuanHalGPSIsOpen(void);

//�ر� GPS ����
//����:				��
//����ֵ:			
//	1 �ɹ���	0 ʧ��;
int YuanHalGPSClose(void);

//�� GPS ����
//����:				��
//����ֵ:			
//	1 �ɹ���	0 ʧ��;
int YuanHalGPSOpen(void);

//��GPS���ڻ�ȡһ���ֽ�����
//����:
//	Item			�ֽ�ָ��
//����ֵ:
//	1: ��ȡһ���ֽڣ� 0: ȡ����ʧ�ܣ�
int YuanHalGPSGetByte(uint8 *Item); 

//��GPS���ڻ�ȡһ���ַ���
//����:
//	Line			�ַ�������ָ��
//	nMaxByte		�ַ������鳤��
//����ֵ:
//	����ʵ�ʶ�ȡGPS���ڵ��ֽ�����
int YuanHalGPSGets(char *Line, int nMaxByte);

//��GPS���ڷ���һ���ַ���
//����:
//	Line			�ַ�������ָ��
//����ֵ:
//	����ʵ�ʶ�ȡGPS���ڵ��ֽ�����
int YuanHalGPSPuts(char *Line);

//��GPS����д����
//����:
//	pBuf			�ַ�������ָ��
//	nBytes			�ַ������鳤��
//����ֵ:
//	����ʵ��дDVR���ڵ��ֽ�����
int YuanHalGPSWrite(const uint8 *pBuf, int nBytes);

//��GPS���ڶ�ȡ����
//����:
//	pBuf			�ַ�������ָ��
//	nBytes			�ַ������鳤��
//����ֵ:
//	����ʵ�ʶ�ȡ���ֽ�����
int YuanHalGPSRead(uint8* pBuf, int nBytes);

//��DVR���ڶ�ȡ����
//����:
//	Item			��DVR���ڶ�ȡһ���ֽ�
//����ֵ:
//	����ʵ�ʶ�ȡ���ֽ�����
int YuanHalDVRReadByte(uint8* Item);

#ifdef __cplusplus
}
#endif


#endif