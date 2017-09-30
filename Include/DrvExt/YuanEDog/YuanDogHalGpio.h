#ifndef YUANDOGHALGPIO_H__
#define YUANDOGHALGPIO_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

	
//取电子狗数据库索引文件内容
//参数:			无
//返回值:		无
void YuanHalGpioInit(void);

//设置GPIO
//参数:
//	Port		端口号
//	Pin			引脚号
//返回值:		无
void YuanHalGpioSet(uint32 Port, uint32 Pin);

//清GPIO
//参数:
//	Port		端口号
//	Pin			引脚号
//返回值:		无
void YuanHalGpioClear(uint32 Port, uint32 Pin);

#ifdef __cplusplus
}
#endif

#endif