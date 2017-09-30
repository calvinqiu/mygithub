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

//初始化 LOG
//参数:			无
//返回值:		无
void YuanHalLogInit(void);

//打开/关闭 LOG
//参数:	
//	Enable		打开/关闭LOG
//返回值:		无
void YuanHalLogEnable(uint16 PriMask, uint16 FacMask, int LogEnable);
//void YuanHalPrint(const char *fmt, ... );

//打印 LOG
//参数:
//	str			可变长度字符串
//返回值:		无
void YuanHalPrint(const char *str);

//打印 HAL LOG
//参数:
//	str			可变长度字符串
//返回值:		无
void YuanHalPrintHalLog(uint16 PriMask,const char *fmt, ... );

//关闭 LOG
//参数:	
//参数:			无
//返回值:		无
void YuanHalLogClose(void);

#ifdef __cplusplus
}
#endif

#endif