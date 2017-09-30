#ifndef YUANDOGHALDVRCOM_H__
#define YUANDOGHALDVRCOM_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C" {
#endif
/////////////////////////////////////////////////////////////////////
// global variable
extern int g_DVRComPort;
extern int g_DVRBandRate;

/////////////////////////////////////////////////////////////////////
// global function

//��ʼ�� DVR ����
//����:				��
//����ֵ:			��
void YuanHalDVRInit(void);

//DVR ���ڴ�״̬
//����:				��
//����ֵ:
//	�����Ƿ��
int YuanHalDVRIsOpen(void);

//�ر� DVR ����
//����:				��
//����ֵ:			
//	�����Ƿ�رճɹ�
int YuanHalDVRClose(void);

//�� DVR ����
//����:				��
//����ֵ:			
//	�����Ƿ�򿪳ɹ�
int YuanHalDVROpen(void);

//��DVR����д����
//����:
//	pBuf			�ַ�������ָ��
//	nBytes			�ַ������鳤��
//����ֵ:
//	����ʵ��дDVR���ڵ��ֽ�����
int YuanHalDVRWrite(const uint8 *pBuf, int nBytes);

//��DVR���ڶ�ȡ����
//����:
//	pBuf			�ַ�������ָ��
//	nBytes			�ַ������鳤��
//����ֵ:
//	����ʵ�ʶ�ȡ���ֽ�����
int YuanHalDVRRead(uint8* pBuf, int nBytes);

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