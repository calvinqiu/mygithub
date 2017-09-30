#include <stdio.h>
#include "YuanDogHalGPSCOM.h"
#include "YuanDogHalEvent.h"
#include "YuanDogHalUtils.h"
#include "YuanDogHalLog.h"

/////////////////////////////////////////////////////////////////////
// global variable

int  g_GPSComPort;
int  g_GPSBandRate;

#define GPS_BUFFER_LEN		(1024*16)
/////////////////////////////////////////////////////////////////////
// static variable

/////////////////////////////////////////////////////////////////////
// static function
static int GPSBufferIsEmpty(void);
static int GPSBufferIsFull(void);
static int GPSBufferGetRear(uint8 *Item);
static int GPSBufferPopRear(uint8 *Item);
static int GPSBufferPushFront(uint8 Item);
static int GPSBufferFoundEndLine(void);
static void GPSBufferEmpty(void);
static void GPSBufferRemovedUnusedRearChar(void);

/////////////////////////////////////////////////////////////////
// Please add code here
// callback function/extern function for difference platform
// NOVATEK

	static int gps_critial = 0;

	//HAL_REPLACE: Please add code here
	#define YUAN_GPS_INIT_CRITICAL()
	#define YUAN_GPS_ENTER_CRITICAL()	do {	\
											while(gps_critial){};	\
											gps_critial++;			\
										}while(0);
	#define YUAN_GPS_EXIT_CRITICAL()	do {	\
											if(gps_critial)	gps_critial--;\
										}while(0);
	
	
	static unsigned char r_char=0;
	static int  edog_flag = 0;
	

	void gps_String_RxCallback(int size, unsigned char *fifo)
	{
		int  i;
		YUANHAL_EVENT Event = {YUANHAL_EVENT_GPS_COM,0,0};
		//char log[256] = {0};
		
		for(i=0;i<size;i++)
		{
			r_char = *(fifo+i);
			//sprintf(log+strlen(log),"%02X ",r_char);
			GPSBufferPushFront(r_char);
		}
		if (GPSBufferFoundEndLine())
		{	
			//LOG_HAL_INFO("fonud a return");
			YuanHalSetEvent(&Event);
		}
		else if (GPSBufferIsFull())
		{
			GPSBufferEmpty();
		}
		//LOG_HAL_INFO("gps_String_RxCallback:%s\r\n",log);
	}
	int OpenGPSSerialPort()
	{
		//HAL_REPLACE:Please add code here
		return 1;
	}

/////////////////////////////////////////////////////////////////////
// static function
static uint8 s_GPSBuffer[GPS_BUFFER_LEN];
static int s_GPSBufferFront;
static int s_GPSBufferRear;

static int GPSBufferIsEmpty(void)
{
	int ret;

	YUAN_GPS_ENTER_CRITICAL();
	ret = (s_GPSBufferFront == s_GPSBufferRear);
	YUAN_GPS_EXIT_CRITICAL();
	
	return ret;
}
static int GPSBufferIsFull(void)
{
	int ret;

	YUAN_GPS_ENTER_CRITICAL();
	ret = (((s_GPSBufferFront + 1) % GPS_BUFFER_LEN) == s_GPSBufferRear);
	YUAN_GPS_EXIT_CRITICAL();
	
	return ret;
}
static int GPSBufferGetRear(uint8 *Item)
{
	if (GPSBufferIsEmpty())	return 0;

	YUAN_GPS_ENTER_CRITICAL();
	*Item = s_GPSBuffer[(s_GPSBufferRear + 1) % GPS_BUFFER_LEN];
	YUAN_GPS_EXIT_CRITICAL();
	return 1;
}
static int GPSBufferPopRear(uint8 *Item)
{
	if (GPSBufferIsEmpty())	return 0;

	YUAN_GPS_ENTER_CRITICAL();
	s_GPSBufferRear = (s_GPSBufferRear + 1) % GPS_BUFFER_LEN;
	*Item = s_GPSBuffer[s_GPSBufferRear];
	YUAN_GPS_EXIT_CRITICAL();
	return 1;
}
static int GPSBufferPushFront(uint8 Item)
{
	if (GPSBufferIsFull())	return 0;

	YUAN_GPS_ENTER_CRITICAL();
	s_GPSBufferFront = (s_GPSBufferFront + 1) % GPS_BUFFER_LEN;
	s_GPSBuffer[s_GPSBufferFront] = Item;
	YUAN_GPS_EXIT_CRITICAL();

	return 1;
}
static void GPSBufferEmpty(void)
{
	YUAN_GPS_ENTER_CRITICAL();
	s_GPSBufferRear = s_GPSBufferFront = 0;
	YUAN_GPS_EXIT_CRITICAL();
}
static void GPSBufferRemovedUnusedRearChar(void)
{
	unsigned char Item;

	while (1)
	{
		if (!GPSBufferGetRear(&Item))	break;
		if (Item == '\r' || Item == '\n' || Item == '\0')	GPSBufferPopRear(&Item);
		else												break;
	}
}
static int GPSBufferFoundEndLine(void)
{
	int i = 0;

	YUAN_GPS_ENTER_CRITICAL();
	if (s_GPSBufferFront > s_GPSBufferRear)
	{
		for (i = s_GPSBufferRear+1; i <= s_GPSBufferFront; i++)
		{
			if (s_GPSBuffer[i] == '\r' || s_GPSBuffer[i] == '\n' || s_GPSBuffer[i] == '\0')
			{				
				YUAN_GPS_EXIT_CRITICAL();
				return 1;
			}
		}
		YUAN_GPS_EXIT_CRITICAL();
		return 0;
	}	
	if (s_GPSBufferFront < s_GPSBufferRear)
	{
		for (i = s_GPSBufferRear+1; i < GPS_BUFFER_LEN; i++)
		{
			if (s_GPSBuffer[i] == '\r' || s_GPSBuffer[i] == '\n' || s_GPSBuffer[i] == '\0')
			{				
				YUAN_GPS_EXIT_CRITICAL();
				return 1;
			}
		}
		for (i = 0; i <= s_GPSBufferFront; i++)
		{
			if (s_GPSBuffer[i] == '\r' || s_GPSBuffer[i] == '\n' || s_GPSBuffer[i] == '\0')
			{				
				YUAN_GPS_EXIT_CRITICAL();
				return 1;
			}
		}
		YUAN_GPS_EXIT_CRITICAL();
		return 0;
	}
	YUAN_GPS_EXIT_CRITICAL();
	return 0;
}

////////////////////////////////////////////////////////////
//Global function
void YuanHalGPSInit(void)
{
	s_GPSBufferFront = s_GPSBufferRear = 0;
	
	YUAN_GPS_INIT_CRITICAL();
}
int YuanHalGPSClose(void)
{
	return 1;
}

int YuanHalGPSOpen()
{
	return OpenGPSSerialPort();
}
int YuanHalGPSIsOpen()
{
	return 1;//SerialIsOpen();
}
int YuanHalGPSGetByte(uint8 *Item)
{   
	return 1;
}
int YuanHalGPSGets(char *Line, int nMaxByte)
{
	int i,j=0;
	unsigned char Item;

	if (!GPSBufferFoundEndLine())	return 0;

	for (i=0; i<nMaxByte-1; i++)
	{
		if (0 == GPSBufferPopRear(&Item))
		{
			break;
		}

		if (j == 0)//removed some un-charactor at head.
		{
			if ((Item == '\r') || (Item == '\n') || (Item == '\0'))		continue;
		}
		else
		{
			if ((Item == '\r') || (Item == '\n') || (Item == '\0'))		break;
		}
		Line[j++] = Item;
	}
	Line[j] = '\0';

	GPSBufferRemovedUnusedRearChar();
	//LOG_HAL_INFO("NMEA:%s",Line);
	
	return j;
}

int YuanHalGPSPuts(char *Line)
{
	//not used
	return 1;
}