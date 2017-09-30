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

//初始化 DVR 串口
//参数:				无
//返回值:			无
void YuanHalDVRInit(void);

//DVR 串口打开状态
//参数:				无
//返回值:
//	串口是否打开
int YuanHalDVRIsOpen(void);

//关闭 DVR 串口
//参数:				无
//返回值:			
//	串口是否关闭成功
int YuanHalDVRClose(void);

//打开 DVR 串口
//参数:				无
//返回值:			
//	串口是否打开成功
int YuanHalDVROpen(void);

//从DVR串口写数据
//参数:
//	pBuf			字符串数组指针
//	nBytes			字符串数组长度
//返回值:
//	返回实际写DVR串口的字节数；
int YuanHalDVRWrite(const uint8 *pBuf, int nBytes);

//从DVR串口读取数据
//参数:
//	pBuf			字符串数组指针
//	nBytes			字符串数组长度
//返回值:
//	返回实际读取的字节数；
int YuanHalDVRRead(uint8* pBuf, int nBytes);

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