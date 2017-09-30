#include "YuanDogHalLog.h"
#include "YuanDogHalSound.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>


/////////////////////////////////////////////////////////////////////
// static Variable
static int s_HalLogEnable = 0;
static int s_HalLogPriMask = 0;
static int s_HalLogFacMask = 0;

void YuanHalLogInit()
{

}

void YuanHalLogEnable(uint16 PriMask, uint16 FacMask, int LogEnable)
{
	s_HalLogEnable = LogEnable;
	s_HalLogPriMask = PriMask;
	s_HalLogFacMask = FacMask;
}

void YuanHalPrint(const char *str)
{
	uint32 ticket;

	ticket = YuanHalGetTickCount();

	//HAL_REPLACE:Please used your printf function to replace it
	debug_msg("%08d:%s",ticket,str);
}

void YuanHalPrintHalLog(uint16 PriMask,const char *fmt, ... )
{

	va_list args;
	char buffer[256],buffer1[256];
	uint32 ticket;

	if (1)
	{
		ticket = YuanHalGetTickCount();
		
		va_start(args, fmt);
		vsprintf(buffer,fmt,args);
		sprintf(buffer1,"%08d:%s",ticket,buffer);
		
		//HAL_REPLACE:Please used your printf function to replace it
		debug_msg(buffer1);

		va_end(args);
	}	
}

void YuanHalLogClose()
{
	//fclose(fLog);
}