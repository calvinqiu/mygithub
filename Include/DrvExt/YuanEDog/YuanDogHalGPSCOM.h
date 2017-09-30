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

//初始化 GPS 串口
//参数:				无
//返回值:			无
void YuanHalGPSInit(void);

//GPS 串口打开状态
//参数:				无
//返回值:			
//	1 成功；	0 失败;
int YuanHalGPSIsOpen(void);

//关闭 GPS 串口
//参数:				无
//返回值:			
//	1 成功；	0 失败;
int YuanHalGPSClose(void);

//打开 GPS 串口
//参数:				无
//返回值:			
//	1 成功；	0 失败;
int YuanHalGPSOpen(void);

//从GPS串口获取一个字节数据
//参数:
//	Item			字节指针
//返回值:
//	1: 获取一个字节； 0: 取数据失败；
int YuanHalGPSGetByte(uint8 *Item); 

//从GPS串口获取一行字符串
//参数:
//	Line			字符串数组指针
//	nMaxByte		字符串数组长度
//返回值:
//	返回实际读取GPS串口的字节数；
int YuanHalGPSGets(char *Line, int nMaxByte);

//从GPS串口发送一行字符串
//参数:
//	Line			字符串数组指针
//返回值:
//	返回实际读取GPS串口的字节数；
int YuanHalGPSPuts(char *Line);

//从GPS串口写数据
//参数:
//	pBuf			字符串数组指针
//	nBytes			字符串数组长度
//返回值:
//	返回实际写DVR串口的字节数；
int YuanHalGPSWrite(const uint8 *pBuf, int nBytes);

//从GPS串口读取数据
//参数:
//	pBuf			字符串数组指针
//	nBytes			字符串数组长度
//返回值:
//	返回实际读取的字节数；
int YuanHalGPSRead(uint8* pBuf, int nBytes);

//从DVR串口读取数据
//参数:
//	Item			从DVR串口读取一个字节
//返回值:
//	返回实际读取的字节数；
int YuanHalDVRReadByte(uint8* Item);

#ifdef __cplusplus
}
#endif


#endif