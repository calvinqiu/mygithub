#ifndef YUANDOGHALRADAR_H__
#define YUANDOGHALRADAR_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////
// Radar function	
//Radar 初始化函数
//参数:				无
//返回值:  
//	1: 成功；	0: 失败；
int	YuanHalRadarInit(void);

/////////////////////////////////////////////////////////////////////
// Radar function	
//Radar 初始化函数
//参数:				
//	Code		Uart data
//返回值:			无
void YuanHalPraseHITRadar(unsigned char Code);


/////////////////////////////////////////////////////////////////////
// Radar function	
//Radar 初始化函数
//参数:				无
//返回值:			无
void YuanHalVerifyHITRadar(void);

#ifdef __cplusplus
}
#endif

#endif