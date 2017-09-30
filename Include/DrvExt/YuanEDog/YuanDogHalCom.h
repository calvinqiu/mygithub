#ifndef YUANDOGHALCOM_H__
#define YUANDOGHALCOM_H__

#include "YuanDogUtils.h"

/////////////////////////////////////////////////////////////////////
// global variable
uint16 g_ComPort;
uint32  g_BandRate;

/////////////////////////////////////////////////////////////////////
// global function
void YuanHalComInit();
uint32 YuanHalComClose();
uint32 YuanHalOpen(int nPort, uint32 bandrate);
uint32 YuanHalComWrite(uint8 *pBuf, uint32 nBytes);
uint32 YuanHalComRead(uint8* pBuf, uint32 nBytes,int Index, unsigned tout);

#endif