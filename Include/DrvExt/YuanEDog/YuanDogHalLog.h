#ifndef YUANDOGHALLOG_H__
#define YUANDOGHALLOG_H__

#include <stdarg.h>
#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C" {
#endif


#define LOG_INFO		(0x01<<0)
#define LOG_ERROR		(0x01<<1)
#define LOG_DEBUG		(LOG_ERROR | LOG_INFO)

#define LOG_HAL			(0x01<<0)
#define LOG_CT			(0x01<<1)
#define LOG_SYS			(0x01<<2)
#define LOG_EDOG		(0x01<<3)
#define LOG_CLOUD		(0x01<<4)
#define LOG_DVR			(0x01<<5)
#define LOG_CL			(LOG_EDOG | LOG_CLOUD | LOG_DVR)
#define LOG_DEFAULT		(LOG_HAL | LOG_CT | LOG_SYS | LOG_CL)


#define LOG_HAL_INFO(...)		YuanHalPrintHalLog(LOG_INFO, __VA_ARGS__)
#define LOG_HAL_ERR(...)		YuanHalPrintHalLog(LOG_ERROR, __VA_ARGS__)
//#define LOG_HAL_DEBUG(fmt,...)		YuanSysLogPrint(LOG_PRI_DEBUG,LOG_FAC_CT,fmt, __VA_ARGS__)

//#define YuanHalPrint	printf
/////////////////////////////////////////////////////////////////////
// global function

//��ʼ�� LOG
//����:			��
//����ֵ:		��
void YuanHalLogInit(void);

//��/�ر� LOG
//����:	
//	Enable		��/�ر�LOG
//����ֵ:		��
void YuanHalLogEnable(uint16 PriMask, uint16 FacMask, int LogEnable);
//void YuanHalPrint(const char *fmt, ... );

//��ӡ LOG
//����:
//	str			�ɱ䳤���ַ���
//����ֵ:		��
void YuanHalPrint(const char *str);

//��ӡ HAL LOG
//����:
//	str			�ɱ䳤���ַ���
//����ֵ:		��
void YuanHalPrintHalLog(uint16 PriMask,const char *fmt, ... );

//�ر� LOG
//����:	
//����:			��
//����ֵ:		��
void YuanHalLogClose(void);

#ifdef __cplusplus
}
#endif

#endif