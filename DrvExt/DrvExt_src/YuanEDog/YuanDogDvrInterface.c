#include "YuanDogDvrInterface.h"
#include "YuanDogDVRSlave.h"
#include "YuanDogBitmapUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdarg.h>

#ifdef YUANDOG_MTK_8328
#include "config_sdk.h"
#endif

/////////////////////////////////////////////////////////////////
// Here were extern variable

YUANEDOG_CONFIGURATION	YuanCfg={0,0,3,2,2};
YUANEDOG_EDOG			YuanEDog={0,0,0,0};
YUANEDOG_GPS			YuanGPS={0,0,230616,0,0,0,0,0};

YUANEDOG_WEATHER		YuanWeather={0,0,0,0,0};
YUANEDOG_GSMINFO		YuanGSMInfo={0xFF,0};
YUANEDOG_DEVICEINFO		YuanDeviceInfo={0};

char	YuanPostion[16] = "…Ó€⁄";
char	YuanTrafic[512]	= {0};
 
int	YuanGotEDog=0, YuanGotGPS=0, YuanGotRadar=0;
int	YuanGotWeather=0,YuanGotPosition=0,YuanGotTrafic=0;
int YuanGotGSMInfo=0,YuanGotCloudInfo=0;

/////////////////////////////////////////////////////////////////
// Here were reference code of 
// callback function/extern function for difference platform
// SOFTWARE EDOG
#if defined (YUANDOG_SOFT_DOG)

#if defined YUANDOG_NOVATEK_96655
	#include "debug.h"
	
	static unsigned char r_char=0;
	static int  sdog_flag = 0;
	
	//#define YUANDOG_DEBUG_LOG
	#define log_print 	debug_msg
	
	#define YUAN_DOG_ENTER_CRITICAL() 	
	#define YUAN_DOG_EXIT_CRITICAL()	
	
	int sdog_String_RxCallback(const unsigned char *fifo, int size)
	{
		int  i;

		//printc("edog_String_RxCallback(%d):",size);
		for(i=0;i<size;i++)
		{
			r_char = *(fifo+i);
			sdog_flag = 1;
			YuanSlaveIntReciever();
		}
		if(YuanGotEDog || YuanGotGPS || YuanGotRadar || YuanGotWeather
			|| YuanGotPosition || YuanGotGSMInfo || YuanGotCloudInfo)
		{
			// YuanDog got package from UART
		} 
		return size;
	}

#elif defined YUANDOG_MTK_8328	
	#include "YuanDogHalEvent.h"
	#include "YuanDogHalDVRCOM.h"
	#include "simSerialPort.h"
	#include "Customer_Config.h"
	#include "AIT_utility.h"
	#include "mmpf_uart.h"
	#include "AHC_Message.h"
	#include "os_wrap.h"
	#include "GPS_ctl.h"
	#include "AHC_OS.h"

	static unsigned char r_char=0;
	static int  sdog_flag = 0;
	
	AHC_OS_SEMID 	ahc_Yuandog_Ready;

	#define log_print 	printc
	
	#define YUAN_DOG_ENTER_CRITICAL() 	AHC_OS_AcquireSem(ahc_Yuandog_Ready,100)
	#define YUAN_DOG_EXIT_CRITICAL()	AHC_OS_ReleaseSem(ahc_Yuandog_Ready)
	
	int sdog_String_RxCallback(const unsigned char *fifo, int size)
	{
		int  i;

		//printc("edog_String_RxCallback(%d):",size);
		for(i=0;i<size;i++)
		{
			r_char = *(fifo+i);
			sdog_flag = 1;
			YuanSlaveIntReciever();
		}
		if(YuanGotEDog || YuanGotGPS || YuanGotRadar || YuanGotWeather
			|| YuanGotPosition || YuanGotGSMInfo || YuanGotCloudInfo)
		{
			MMPF_OS_SetFlags(UartCtrlFlag, YUANDOG_FLAG_CLOUD, MMPF_OS_FLAG_SET); // YuanDog got package from UART
		} 
		if(YuanGotTrafic)
		{
			MMPF_OS_SetFlags(UartCtrlFlag, YUANDOG_FLAG_TRAFFIC, MMPF_OS_FLAG_SET); // YuanDog got package from UART
		} 
		return size;
	}

#elif defined YUANDOG_JL_5201	
	#include "YuanDogHalEvent.h"
	#include "YuanDogHalDVRCOM.h"
	#include "simSerialPort.h"

	static unsigned char r_char=0;
	static int  sdog_flag = 0;
	
	AHC_OS_SEMID 	ahc_Yuandog_Ready;

	#define log_print 	printc
	
	#define YUAN_DOG_ENTER_CRITICAL() 	
	#define YUAN_DOG_EXIT_CRITICAL()	
	
	int sdog_String_RxCallback(const unsigned char *fifo, int size)
	{
		int  i;

		//printc("edog_String_RxCallback(%d):",size);
		for(i=0;i<size;i++)
		{
			r_char = *(fifo+i);
			sdog_flag = 1;
			YuanSlaveIntReciever();
		}
		if(YuanGotEDog || YuanGotGPS || YuanGotRadar || YuanGotWeather
			|| YuanGotPosition || YuanGotGSMInfo || YuanGotCloudInfo)
		{
			
		} 
		if(YuanGotTrafic)
		{
			
		} 
		return size;
	}
#endif

	int OpenSerialPort()
	{
		simSerialPortSlaveInit(sdog_String_RxCallback);
	}
	int YuanCloudReadByte(unsigned char* Data)
	{
		if(sdog_flag)
		{
			sdog_flag = 0;
			*Data = r_char;
			//YuanPrintLog("%x ", *Data);
			//printc("%x ", *Data);
			return 0;
		}
		return 1;//(!DVRBufferPopRear(Data));
	}
	int YuanCloudReadBytes(unsigned char* Data, int Lenght)
	{
		assert(0);

		return 1;
	}
	int YuanCloudWriteByte(const unsigned char* Data)
	{
		simSerialPortSlaveWrites((unsigned char*)Data,1);

		return 0;
	}
	int YuanCloudWriteBytes(const unsigned char* Data, int Lenght)
	{
		simSerialPortSlaveWrites((unsigned char*)Data,Lenght);

		return 0;
	}
#else //	!YUANDOG_SOFT_DOG
/////////////////////////////////////////////////////////////////
// Here were reference code of 
// callback function/extern function for difference platform
// MTK8328
#if defined YUANDOG_MTK_8328
	#include "Customer_Config.h"
	#include "AIT_utility.h"
	#include "mmpf_uart.h"
	#include "AHC_Message.h"
	#include "os_wrap.h"
	#include "GPS_ctl.h"
	#include "AHC_OS.h"
	
	#define log_print 	printc
	
	#define YUAN_DOG_ENTER_CRITICAL() 	AHC_OS_AcquireSem(ahc_Yuandog_Ready,100)
	#define YUAN_DOG_EXIT_CRITICAL()	AHC_OS_ReleaseSem(ahc_Yuandog_Ready)
	
	#define EDOG_UART    MMP_UART_ID_1
	
	static unsigned char r_char=0;
	static int  edog_flag = 0;
	
	AHC_OS_SEMID 	ahc_Yuandog_Ready;
	
	void edog_String_RxCallback(MMP_UBYTE size, volatile MMP_UBYTE *fifo)
	{
		MMP_LONG  i;

		//YuanPrintLog("edog_String_RxCallback(%d):",size);
		for(i=0;i<size;i++)
		{
			r_char = *fifo;
			edog_flag = 1;
			YuanSlaveIntReciever();
		}
		if(YuanGotEDog || YuanGotGPS || YuanGotRadar || YuanGotWeather	|| 
			YuanGotPosition || YuanGotGSMInfo || YuanGotCloudInfo)
		{
			MMPF_OS_SetFlags(UartCtrlFlag, YUANDOG_FLAG_CLOUD, MMPF_OS_FLAG_SET); // YuanDog got package from UART
    		
			YuanGotEDog = YuanGotGPS = YuanGotRadar =0;
			YuanGotWeather = YuanGotPosition = YuanGotGSMInfo = YuanGotCloudInfo = 0;				
		}
		if (YuanGotTrafic)
		{
			MMPF_OS_SetFlags(UartCtrlFlag, YUANDOG_FLAG_TRAFFIC, MMPF_OS_FLAG_SET); // YuanDog got package from UART
    		
			YuanGotTrafic = 0;
		}
	}
	int OpenSerialPort()
	{
		MMP_UART_ATTR 		uartattribute;
		MMP_LONG 			error =-1;

		uartattribute.bParityEn 	= MMP_FALSE;
		uartattribute.parity 		= MMP_UART_PARITY_ODD;
		uartattribute.bFlowCtlEn 	= MMP_FALSE;
		uartattribute.ubFlowCtlSelect = 1;
		uartattribute.padset 		= 0;
		uartattribute.ulMasterclk 	= (264000 >> 1) * 1000 ;
		uartattribute.ulBaudrate 	= 115200;
	
		//YuanPrintLog("OpenSerialPort()\n");
		error = MMPF_Uart_Open(EDOG_UART, &uartattribute);
		if(error)
		{
			YuanPrintLog("MMPF_Uart_Open UseRxDMA : %d\r\n",error);
		}
		error=MMPF_Uart_EnableRx(EDOG_UART, 1,(UartCallBackFunc *)&edog_String_RxCallback);
		if(error)
		{
			YuanPrintLog("MMPF_Uart_EnableRx UseRxDMA : %d\r\n",error);
			MMPF_Uart_Close(EDOG_UART);
		}
	}
	int YuanCloudReadByte(unsigned char* Data)
	{
		if(edog_flag)
		{
			edog_flag = 0;
			*Data = r_char;
			//YuanPrintLog("%x ", *Data);
			//printc("%x ", *Data);
			return 0;
		}
		return 1;//(!DVRBufferPopRear(Data));
	}
	int YuanCloudReadBytes(unsigned char* Data, int Lenght)
	{
		assert(0);

		return 1;
	}
	int YuanCloudWriteByte(const unsigned char* Data)
	{
		MMPF_Uart_Write(EDOG_UART,(char*)Data,1);

		return 0;
	}
	int YuanCloudWriteBytes(const unsigned char* Data, int Lenght)
	{
		MMPF_Uart_Write(EDOG_UART,(char*)Data,Lenght);

		return 0;
	}

#elif defined WIN32
	#include "YuanSerialPort.h"

	#pragma warning (disable:4996) 

	#define log_print printf

	#define YUAN_DOG_ENTER_CRITICAL()
	#define YUAN_DOG_EXIT_CRITICAL()
//////////////////////////////////////////////////////////////////
// callback function for YuanDogDVRSlave.h
// ALLWINNER V3	
#elif defined YUANDOG_ALLWINNER_V3
	#include "debug.h"
	#include <unistd.h>
	#include <termios.h>
	#include <fcntl.h>
		
	#define log_print db_msg
	
	#define YUAN_DOG_ENTER_CRITICAL()
	#define YUAN_DOG_EXIT_CRITICAL()
	/////////////////////////////////////////////////////////////////////////////////
	// Serial port interface
	int yunFd = 0;
	int openSerialPort(char * path)
	{
		int fd;
	
	    fd = open( path, O_RDWR|O_NOCTTY|O_NDELAY);
	    if(fd <= 0)						{db_error("Can't Open %s Serial Port\n",path);	return -1;}   
	    if(fcntl(fd, F_SETFL, 0) < 0)	{db_error("fcntl failed!\n");}
	    if(isatty(STDIN_FILENO) == 0)	{db_error("standard input(%s) is not a terminal device\n",path);}
	
	    return fd;
	}
	int configureSerialPort(int fd,int nSpeed, int nBits, char nEvent, int nStop)
	{
		struct termios newtio,oldtio;
	
	    if(tcgetattr(fd,&oldtio) != 0)	{db_error("SetupSerial 1");	return -1;}
	
	    bzero(&newtio, sizeof(newtio));
	    newtio.c_cflag  |=  CLOCAL | CREAD; 
	    newtio.c_cflag &= ~CSIZE; 

	    switch( nBits )
	    {
			case 7:
				newtio.c_cflag |= CS7;
				break;
			case 8:
				newtio.c_cflag |= CS8;
				break;
	    }

	    switch( nEvent )
	    {
			case 'O':                     //??D°Í?®¶
				newtio.c_cflag |= PARENB;
				newtio.c_cflag |= PARODD;
				newtio.c_iflag |= (INPCK | ISTRIP);
				break;
			case 'E':                     //??D°Í?®¶
				newtio.c_iflag |= (INPCK | ISTRIP);
				newtio.c_cflag |= PARENB;
				newtio.c_cflag &= ~PARODD;
				break;
			case 'N':                    //?TD°Í?®¶
				newtio.c_cflag &= ~PARENB;
				break;
	    }

		switch( nSpeed )
		{
		    case 2400:
		        cfsetispeed(&newtio, B2400);
		        cfsetospeed(&newtio, B2400);
		        break;
		    case 4800:
		        cfsetispeed(&newtio, B4800);
		        cfsetospeed(&newtio, B4800);
		        break;
		    case 9600:
		        cfsetispeed(&newtio, B9600);
		        cfsetospeed(&newtio, B9600);
		        break;
		    case 115200:
		        cfsetispeed(&newtio, B115200);
		        cfsetospeed(&newtio, B115200);
		        break;
		    default:
		        cfsetispeed(&newtio, B9600);
		        cfsetospeed(&newtio, B9600);
		        break;
	    }
	    if( nStop == 1 )
	    {
	        newtio.c_cflag &=  ~CSTOPB;
	    }
	    else if ( nStop == 2 )
	    {
	        newtio.c_cflag |=  CSTOPB;
	    }
	    newtio.c_cc[VTIME]  = 0;
	    newtio.c_cc[VMIN] = 0;
	    tcflush(fd,TCIFLUSH);
	    if((tcsetattr(fd,TCSANOW,&newtio))!=0)	{db_error("com set error");	return -1;}

	    return 0;
	}

	int readSerialPortByte(unsigned char * Data)
	{
		int ret = 0;
		if( yunFd <= 0 )
		{
			db_msg(" readSerialPortByte, but yunFd is null ....\n");
			yunFd = openSerialPort("/dev/ttyS2");
			if(yunFd <= 0)
			{
				db_error(" open tty file failed ..\n");
				return -1;
			}
			if(configureSerialPort(yunFd,115200,8,'N',1) < 0)
			{
				db_error("set_opt error");
				return -1;
			}
		}
		ret = read(yunFd, Data, 1);

		if (ret)
			db_error("readSerialPortByte0x%X \n",*Data);

		return ret;
	}
		
	int readSerialPortBytes(unsigned char * Data, int length)
	{
		int ret = 0;
		//db_msg(" readSerialPortBytes ....\n");
		if( yunFd <= 0 )
		{
			yunFd = openSerialPort("/dev/ttyS2");
			if(yunFd <= 0)
			{
				db_error(" open tty file failed ..\n");
				return -1;
			}
			if(configureSerialPort(yunFd,115200,8,'N',1) < 0)
			{
				db_error("set_opt error");
				return -1;
			}
		}
		ret = read(yunFd, Data, length);

		db_msg("UART: %x \n",*Data);
		
		return ret;
	}

	int writeSerialPortByte(const unsigned char* Data)
	{
		int ret = 0;
		if( yunFd <= 0 )
		{
			yunFd = openSerialPort("/dev/ttyS2");
			if(yunFd <= 0)
			{
				db_error(" open tty file failed ..\n");
				return -1;
			}
			if(configureSerialPort(yunFd,115200,8,'N',1) < 0)
			{
				db_error("set_opt error");
				return -1;
			}
		}
		ret = write(yunFd,Data,1);
		if(ret <= 0)
		{
			db_msg(" write byte data failed ....");
			return -1;
		}else
		{
			db_msg(" write tty file success ....\n");
		}
		return ret;
	}

	int writeSerialPortBytes(const unsigned char* Data, int length)
	{
		int ret = 0;
		if( yunFd <= 0 )
		{
			yunFd = openSerialPort("/dev/ttyS2");
			if(yunFd <= 0)
			{
				db_error(" open tty file failed ..\n");
				return -1;
			}
			if(configureSerialPort(yunFd,115200,8,'N',1) < 0)
	    	{
				db_error("set_opt error");
				return -1;
	   	 	}
		}
		ret = write(yunFd,Data,length);
		if(ret <= 0)
		{
			db_msg(" write byte data failed ....");
			return -1;
		}
		else
		{
			db_msg(" write tty file success ....\n");
		}
		return ret;
	}
	int YuanCloudReadByte(unsigned char* Data)
	{
		return ( readSerialPortByte(Data) < 1 );
	}
	int YuanCloudReadBytes(unsigned char* Data, int Length)
	{
		//db_msg(" YuanCloudReadBytes ....\n");
	
		return (readSerialPortBytes(Data, Length) < 1);
	}
	int YuanCloudWriteByte(const unsigned char* Data)
	{
		//db_msg(" YuanCloudWriteByte ....\n");

		return (writeSerialPortByte(Data) < 1);
	}
	int YuanCloudWriteBytes(const unsigned char* Data, int Length)
	{
		return (writeSerialPortBytes(Data, Length) < 1);
	}
#endif 
#endif // YUANDOG_SOFT_DOG

#ifdef YUANDOG_DEBUG_LOG
int YuanPrintLog(const char *format, ...)
{
	va_list args;
	char text[512];
	
	va_start(args, format);
	vsprintf(text,format,args);
	log_print("Yuan-:%s",text);

	return 1;
}
#else
#define YuanPrintLog(...)
#endif

/////////////////////////////////////////////////////////////////
// Here were reference code of callback function cloud dog.
//////////////////////////////////////////////////////////////////
// callback function for YuanDogDVRSlave.h
static int YuanCloudAckConfiguration()
{
	//printf("ACK Config\r\n");

	return 0;
}
static int YuanCloudAckReqConfiguration(unsigned short SlienceSpeed,
								unsigned short OverSpeed,unsigned short MeasureMode,
								unsigned char  SenMode	,unsigned char  Volume)
{
	YUAN_DOG_ENTER_CRITICAL();

	YuanCfg.SlienceSpeed	= SlienceSpeed;
	YuanCfg.SenMode			= SenMode;
	YuanCfg.OverSpeed		= OverSpeed;
	YuanCfg.MeasureMode		= MeasureMode;
	YuanCfg.Volume			= Volume;
	
	YUAN_DOG_EXIT_CRITICAL();

	YuanPrintLog("YuanCloudAckReqConfiguration:SliSpd(%d),SenMode(%d),OvSpd(%d),MeaMode(%d),Vol(%d)\r\n",
		SlienceSpeed,SenMode,OverSpeed,MeasureMode,Volume);

	return 0;
}
static int YuanCloudAckReqConfigurationEx(unsigned short SlienceSpeed,
								unsigned short OverSpeed,unsigned short MeasureMode,
								unsigned char  SenMode	,unsigned char  Volume,
								unsigned char VoiceMode, unsigned char TraficEnable,
								unsigned char TestMode)
{
	YUAN_DOG_ENTER_CRITICAL();
	
	YuanCfg.SlienceSpeed	= SlienceSpeed;
	YuanCfg.SenMode			= SenMode;
	YuanCfg.OverSpeed		= OverSpeed;
	YuanCfg.MeasureMode		= MeasureMode;
	YuanCfg.Volume			= Volume;
	YuanCfg.TraficEnable	= TraficEnable;
	YuanCfg.VoiceMode		= VoiceMode;
	YuanCfg.TestMode		= TestMode;
	
	YUAN_DOG_EXIT_CRITICAL();

	YuanPrintLog("YuanCloudAckReqConfigurationEx:SliSpd(%d),SenMode(%d),OvSpd(%d),MeaMode(0x%02x),Vol(%d),TraEn(%d),VoM(%d),TMod(%d)\r\n",
		SlienceSpeed,SenMode,OverSpeed,MeasureMode,Volume,TraficEnable,VoiceMode,TestMode);
	return 0;
}
static int YuanCloudGPSInfo(char Fixed, unsigned long UTCDate, unsigned long UTCTime,
					 float Latitude,  float Longitude, 
					 float Speed, float Bearing, int SatelliteNumber)
{
	struct tm local_time;
	time_t rawtimes;

	YUAN_DOG_ENTER_CRITICAL();

	YuanGPS.Fixed			= Fixed;
	YuanGPS.UTCDate			= UTCDate;
	YuanGPS.UTCTime			= UTCTime;
	YuanGPS.nSatellite		= SatelliteNumber;

	if (YuanGPS.Fixed == 'A')
	{
		YuanGPS.Speed			= Speed;
		YuanGPS.Bearing			= Bearing;
	}
	else
	{
		//YuanGPS.Speed = YuanGPS.Bearing = 0;
	}

	YuanGotGPS = 1;

	if ((UTCDate % 100) > 10 && (UTCDate % 100) < 50)
	{
		local_time.tm_year = UTCDate % 100 + 2000;
		local_time.tm_mon  = (UTCDate / 100) % 100;
		local_time.tm_mday = UTCDate / 10000;

		local_time.tm_hour = UTCTime / 10000;
		local_time.tm_min  = (UTCTime / 100) % 100;
		local_time.tm_sec  = UTCTime % 100;

#ifdef YUANDOG_ALLWINNER_V3		
		{			
    		struct timeval tv; 
	
			local_time.tm_year -= 1900;
			local_time.tm_mon  -= 1;

			rawtimes = mktime(&local_time);
			
		    tv.tv_sec = rawtimes;  
		    tv.tv_usec = 0; 
			//time(&rawtimes);
			if(settimeofday (&tv, (struct timezone *) 0) < 0)  
		    {  
		   		YuanPrintLog("Set system datatime error!/n");  
		    } 
		}
#endif		
	}
	YUAN_DOG_EXIT_CRITICAL();

	if (Fixed == 'A')
	{
		YuanPrintLog("YuanCloudGPSInfo:Fixed(%c),Date(%d/%d/%d-%d:%d:%d),SV(%d),Pos(%f,%f)Speed(%d),Bear(%d)\r\n",
			Fixed,
			local_time.tm_year,local_time.tm_mon,local_time.tm_mday,
			local_time.tm_hour,local_time.tm_min,local_time.tm_sec,
			SatelliteNumber,Latitude,Longitude,(int)Speed,(int)Bearing);
	}
	else
	{
		YuanPrintLog("YuanCloudGPSInfo:Fixed(V)\r\n");
	}

	return 0;
}
static int YuanCloudSatelliteInfo(int SatelliteNumber, 
								  unsigned char* PRN, unsigned char* Inused, unsigned char* SRN)
{
	YUAN_DOG_ENTER_CRITICAL();

	//NOT SUPPORT YET
	YuanPrintLog("YuanCloudSatelliteInfo\r\n");

	YUAN_DOG_EXIT_CRITICAL();

	return 0;
}
static int YuanCloudEDogInfo(unsigned char Type, float Latitude, float Longitude, 
					  float Speed, float Bearing, float Distance, unsigned char LimitedSpeed)
{
	YUAN_DOG_ENTER_CRITICAL();

	YuanEDog.Type				= Type;
		
	if (YUAN_VALID_EDOG(YuanEDog.Type))
	{
		YuanGotEDog = 1;
		YuanGotGPS	= 1;
		
		YuanEDog.Distance		= Distance;
		YuanEDog.LimitedSpeed	= LimitedSpeed;
		
		YuanGPS.Speed			= Speed;
		YuanGPS.Bearing 		= Bearing;

		YuanPrintLog("YuanCloudEDogInfo:Type(%d),Distance(%d),LimitedSpeed(%d)\r\n",Type,(int)Distance,LimitedSpeed);
	}
	else
	{
		YuanGPS.Fixed = 0xFF;
		
		YuanEDog.Distance		= 0;
		YuanEDog.LimitedSpeed	= 0;

		YuanGotEDog = 0;
	}
	
	YUAN_DOG_EXIT_CRITICAL();

	return 0;
}
static int YuanCloudRadarInfo(unsigned char FrequencySeg)
{
	YUAN_DOG_ENTER_CRITICAL();

	YuanEDog.Radar	= FrequencySeg;

	if (YUAN_VALID_RADAR(YuanEDog.Radar))
	{
		YuanGotGPS = 1;
		YuanGotRadar = 1;
		
		YuanPrintLog("YuanCloudRadarInfo:Radar(%x)\r\n",FrequencySeg);
	}
	else
	{
		YuanEDog.Radar = 0xFF;
		
		YuanGotRadar = 0;
	}
	
	YUAN_DOG_EXIT_CRITICAL();

	return 0;
}
static int YuanCloudPostion(char *Provice, char *City, char *Country, char *Road)
{
	YUAN_DOG_ENTER_CRITICAL();

	YuanGotPosition = 1;

	if (strlen(City) < sizeof(YuanPostion))
		strcpy(YuanPostion,City);
	else
		memcpy(YuanPostion,City,sizeof(YuanPostion)-2);

	YUAN_DOG_EXIT_CRITICAL();

	YuanPrintLog("YuanCloudPostion:(%s)\r\n",City);
	
	return 0;
}
static int YuanCloudTfafic(char *Trafic)
{
	YUAN_DOG_ENTER_CRITICAL();

	YuanGotTrafic = 1;
	
	if (strlen(Trafic) < sizeof(YuanTrafic))
		strcpy(YuanTrafic,Trafic);
	else
		memcpy(YuanTrafic,Trafic,sizeof(YuanTrafic)-2);
	
	YUAN_DOG_EXIT_CRITICAL();

	YuanPrintLog("YuanCloudTfafic:(%s)\r\n",Trafic);

	return 0;
}
static int YuanCloudWeather(int Weather, int MinTemp, int MaxTemp, int CurTemp, int AQI, int PM25)
{
	YUAN_DOG_ENTER_CRITICAL();

	YuanWeather.Type		= Weather;
	YuanWeather.MinTemp		= MinTemp;
	YuanWeather.MaxTemp		= MaxTemp;
	YuanWeather.AQI			= AQI;
	YuanWeather.PM25		= PM25;

	YuanGotWeather = 1;
	
	YUAN_DOG_EXIT_CRITICAL();

	YuanPrintLog("YuanCloudWeather:Wea(%d),MinT(%d),MaxT(%d),PM(%d),AQI(%d)\r\n",Weather,MinTemp,MaxTemp,PM25,AQI);

	return 0;
}

static int YuanCloudGSMInfo(int Signal, int Cloud)
{
	YUAN_DOG_ENTER_CRITICAL();

	YuanGotGSMInfo = 1;

	YuanGSMInfo.RSSI 	= Signal;
	YuanGSMInfo.Cloud	= Cloud;
	
	YUAN_DOG_EXIT_CRITICAL();

	YuanPrintLog("YuanCloudGSMInfo:RSSI(%d),Cloud(%d)\r\n",Signal,Cloud);

	return 0;
}
static int YuanCloudDeviceInfo(unsigned short HWVer,
					   unsigned short MTKVer,
					   unsigned short EDogVer,
					   unsigned short ServVer,
					   unsigned char *IMEI,
					   unsigned char *ICCID,
					   unsigned char *IMSI)
{
	YUAN_DOG_ENTER_CRITICAL();
	
	YuanGotCloudInfo = 1;

	YuanDeviceInfo.HWVer	= HWVer;
	YuanDeviceInfo.MTKVer	= MTKVer;
	YuanDeviceInfo.EDogVer	= EDogVer;
	YuanDeviceInfo.ServVer	= ServVer;

	memcpy(YuanDeviceInfo.IMEI	,IMEI ,YUAN_IMEI_LEN+1);
	memcpy(YuanDeviceInfo.ICCID	,ICCID,YUAN_ICCID_LEN+1);
	memcpy(YuanDeviceInfo.IMSI	,IMSI ,YUAN_IMSI_LEN+1);
	
	YUAN_DOG_EXIT_CRITICAL();

	YuanPrintLog("YuanCloudDeviceInfo:HWVer(%d),HWVer(%d),EDogVer(%d),ServVer(%d),IMEI(%s),ICCID(%s)\r\n",
		HWVer,MTKVer,EDogVer,ServVer,IMEI,ICCID);

	return 0;
}
int YuanCloudRequestPhoto(unsigned short FrameNum)
{
	//Please add send photo function here
#ifdef YUANDOG_MTK_8328	

#elif defined YUANDOG_ALLWINNER_V3

#elif defined WIN32
	static int TotalFrame, FileSize;

	if (FrameNum == 0xFFFF)
	{
		YuanSendPhotoByFile("dvr.jpg", FrameNum,&TotalFrame,&FileSize);	
	}
	else
	{
		YuanSendPhotoByFile("dvr.jpg", FrameNum,&TotalFrame,&FileSize);	
	}
#endif

	return 0;
}

int YuanEDogInterfaceInit(int PortNumber,int BandRate)
{	
	YUANCLOUDCALLBACK	YuanCloudCallback={0};
	
#ifdef YUANDOG_MTK_8328	
	OpenSerialPort();
#elif defined YUANDOG_ALLWINNER_V3
	if((yunFd=openSerialPort("/dev/ttyS2")) < 0) 	{db_error("open_port error"); 	return -1;}
    if(configureSerialPort(yunFd,115200,8,'N',1) < 0)	{db_error("set_opt error");		return -1;}
#elif defined WIN32
	YuanCloudOpenSerial(PortNumber,BandRate);
#endif
	
	YuanCloudCallback.write_byte		= YuanCloudWriteByte;
	YuanCloudCallback.write_bytes		= YuanCloudWriteBytes;
	YuanCloudCallback.read_byte			= YuanCloudReadByte;
	YuanCloudCallback.read_bytes		= YuanCloudReadBytes;
	YuanCloudCallback.ack_cfg			= YuanCloudAckConfiguration;
	YuanCloudCallback.ack_request_cfg	= YuanCloudAckReqConfiguration;
	YuanCloudCallback.ack_request_cfgex	= YuanCloudAckReqConfigurationEx;
	YuanCloudCallback.edog_info			= YuanCloudEDogInfo;
	YuanCloudCallback.gps_info			= YuanCloudGPSInfo;
	YuanCloudCallback.radar_info		= YuanCloudRadarInfo;
	YuanCloudCallback.cloud_weather		= YuanCloudWeather;
	YuanCloudCallback.cloud_pos			= YuanCloudPostion;
	YuanCloudCallback.cloud_trafic		= YuanCloudTfafic;
	YuanCloudCallback.cloud_gsminfo		= YuanCloudGSMInfo;
	YuanCloudCallback.cloud_deiveinfo	= YuanCloudDeviceInfo;
	YuanCloudCallback.cloud_requestphoto= YuanCloudRequestPhoto;


#ifdef YUANDOG_DEBUG_LOG	
	YuanCloudCallback.print_log			= YuanPrintLog;
#else
	YuanCloudCallback.print_log= NULL;
#endif

	YuanDogDvrInit(&YuanCloudCallback);


	YuanDvrD2ESendReqeustCloud();
	
	return 0;
}
