#include "YuanDogHalDVRCOM.h"
#include "YuanDogHalEvent.h"
#include "simSerialPort.h"
#include "YuanDogHalLog.h"
#include <stdio.h>

/////////////////////////////////////////////////////////////////////
// global variable

int g_DVRComPort;
int g_DVRBandRate;

#define DVR_BUFFER_LEN		(128)

//HAL_REPLACE: Please add code here
#define YUAN_SOUND_INIT_CRITICAL()
#define YUAN_DVR_ENTER_CRITICAL()
#define YUAN_DVR_EXIT_CRITICAL()
/////////////////////////////////////////////////////////////////////
// static variable
static uint8 s_DVRBuffer[DVR_BUFFER_LEN];
static int s_DVRBufferFront = 0;
static int s_DVRBufferRear  = 0;

/////////////////////////////////////////////////////////////////////
// static function
static int DVRBufferIsEmpty(void);
static int DVRBufferIsFull(void);
static int DVRBufferPopRear(uint8 *Item);
static int DVRBufferPushFront(uint8 Item);
static void DVRBufferEmpty(void);

static int cb_DVRReadSerialPort(const uint8* pBuf, int nBytes);
/////////////////////////////////////////////////////////////////////
// static function
static int DVRBufferIsEmpty()
{
	return (s_DVRBufferFront == s_DVRBufferRear);
}
static int DVRBufferIsFull()
{
	return (((s_DVRBufferFront + 1) % DVR_BUFFER_LEN) == s_DVRBufferRear);
}
static int DVRBufferPopRear(uint8 *Item)
{
	if (DVRBufferIsEmpty())	return 0;

	YUAN_DVR_ENTER_CRITICAL();
	s_DVRBufferRear = (s_DVRBufferRear + 1) % DVR_BUFFER_LEN;
	*Item = s_DVRBuffer[s_DVRBufferRear];
	YUAN_DVR_EXIT_CRITICAL();

	return 1;
}
static int DVRBufferPushFront(uint8 Item)
{
	if (DVRBufferIsFull())	return 0;
	
	YUAN_DVR_ENTER_CRITICAL();
	s_DVRBufferFront = (s_DVRBufferFront + 1) % DVR_BUFFER_LEN;
	s_DVRBuffer[s_DVRBufferFront] = Item;
	YUAN_DVR_EXIT_CRITICAL();

	return 1;
}
static void DVRBufferEmpty()
{
	YUAN_DVR_ENTER_CRITICAL();
	s_DVRBufferRear = s_DVRBufferFront = 0;
	YUAN_DVR_EXIT_CRITICAL();
}

static int cb_DVRReadSerialPort(const uint8* pBuf, int nBytes)
{
	int i=0; 
	YUANHAL_EVENT Event = {YUANHAL_EVENT_DVR_COM,0,0};

	LOG_HAL_INFO("cb_DVRReadSerialPort:%d\r\n",nBytes);
	for (i=0; i<nBytes; i++)
	{
		if (!DVRBufferIsFull())DVRBufferPushFront(*(pBuf+i));
		else DVRBufferEmpty();
	}
	YuanHalSetEvent(&Event);

	return nBytes;
}

void YuanHalDVRInit()
{
	s_DVRBufferFront = s_DVRBufferRear = 0;

}
int YuanHalDVRClose()
{
	return 1;
}
int YuanHalDVROpen()
{	
	return simSerialPortHostInit(cb_DVRReadSerialPort);
}
int YuanHalDVRIsOpen()
{
	return 1;
}
int YuanHalDVRWrite(const uint8 *pBuf, int nBytes)
{
	return simSerialPortHostWrites(pBuf,nBytes);
}
int YuanHalDVRRead(uint8* pBuf, int nBytes)
{   
	int i = 0;

	while (YuanHalDVRReadByte(pBuf) && i<nBytes)
	{
		pBuf++;
		i++;
	}
	return i;
}

int YuanHalDVRReadByte(uint8* Item)
{
	int ret;

	ret =  DVRBufferPopRear(Item);

	return ret;
}