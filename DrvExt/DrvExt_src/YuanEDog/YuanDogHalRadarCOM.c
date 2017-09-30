#include <stdio.h>
#include "YuanDogHalRadarCOM.h"
#include "YuanDogHalRadar.h"
#include "YuanDogHalEvent.h"
#include "YuanDogHalUtils.h"
#include "YuanDogHalLog.h"
#include "debug.h"

/////////////////////////////////////////////////////////////////
// Here were reference code of 
// callback function/extern function for difference platform
// MTK8328

	static int radar_critial = 0;
	
	
	#define YUAN_RADAR_INIT_CRITICAL()
	#define YUAN_RADAR_ENTER_CRITICAL() 	do {	\
											while(radar_critial){};	\
											radar_critial++;			\
										}while(0);
	#define YUAN_RADAR_EXIT_CRITICAL()	do {	\
											if(radar_critial)	radar_critial--;\
										}while(0);
	

	
	void radar_String_RxCallback(int size, unsigned char *fifo)
	{
		int  		i;
		uint8			r_char;

		//printc("radar_String_RxCallback");
		for(i=0;i<size;i++)
		{
			r_char = *fifo;
			
			//LOG_HAL_INFO("%d",r_char);
			
			YuanHalPraseHITRadar(r_char);
		}
	}
	int OpenRadarSerialPort()
	{
			return 1;
	}
	
int YuanHalRadarComInit(void)
{
	return OpenRadarSerialPort();
}