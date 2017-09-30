#ifndef YUANDOGHALINTERFACE_H__
#define YUANDOGHALINTERFACE_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C"{
#endif 

/////////////////////////////////////////////////////////////////////
// static function
#define YUAN_IMEI_LEN	15
#define YUAN_IMSI_LEN	18
#define YUAN_ICCID_LEN	20

/////////////////////////////////////////////////////////////////////
// global function

/////////////////////////////////////////////////////////////////////
// interface file function	
//获取IMEI号
	//参数:				
	//	IMEI		IMEI number；
//返回值:
//	1: 成功；	0: 失败；
int	YuanHalGetIMEI(char* IMEI);

//设置IMEI号
	//参数:						
	//	IMEI		IMEI number；
//返回值:
//	1: 成功；	0: 失败；
int	YuanHalSetIMEI(char* IMEI);

//获取ICCID号
//参数:				
//	ICCID		ICCID number；
//返回值:
//	1: 成功；	0: 失败；
int YuanHalGetICCID(char *ICCID);

//获取IMSI号
//参数:				
//	IMSI		IMSI number；
//返回值:
//	1: 成功；	0: 失败；
int YuanHalGetIMSI(char *IMSI);

//获取版本号
//参数:				
//	HWVer			硬件版本号 - BCD码，etc：v1.01 = 101；
//	MTKVer			软件版本号；
//	EDogVer			电子狗数据版本号；
//返回值:
//	1: 成功；	0: 失败；
int	YuanHalGetVer(uint16 *HWVer, uint16* MTKVer, uint16* EDogVer);


#ifdef __cplusplus
}
#endif

#endif