/**
    Copyright   Novatek Microelectronics Corp. 2007.  All rights reserved.

    @file       BuildDateInfo.c
    @ingroup    mIPRJAPCfg

    @brief      Project source build time information
                Project source build time information

    @note       The project source build time info will be updated every time you build the image.

    @date       2007/11/06
*/

/** \addtogroup mIPRJAPCfg*/
//@{

#include <stdio.h>
#include "ProjectInfo.h"
#include "Type.h"
#include "PrjCfg.h"
#include <string.h>
#include "BinInfo.h"

//-----------------------------------------------------------------------------
// FW info
//-----------------------------------------------------------------------------
//FW updtae FW need access external fw version via versioninfo.s offset 0x000000A8
//#NT#2016/08/17#Niven Cho -begin
//#NT#add _L to indicate linux version
#if (_CPU2_TYPE_ == _CPU2_LINUX_)
char gFWExternalVersion[33]=FW_VERSION_NUM"_L";
#else
char gFWExternalVersion[33]=FW_VERSION_NUM;
#endif
//#NT#2016/08/17#Niven Cho -end

/* modify begin by ZMD, 2017-02-15 new version management*/
#if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
//#define FW_VER_NUM   ".R0001"
#define FW_VER_NUM   ".R0006"

#if defined(YQCONFIG_CUSTOM_VERSION_NAME)
#define PRJ_NAME    YQCONFIG_PLATFORM_NAME  
#define SERVER_NAME  YQCONFIG_CUSTOM_VERSION_NAME
#else
#(error please config your custom version name!!!)
//#define PRJ_NAME     "ERR"
//#define SERVER_NAME  ".youqing"
#endif

#if ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX291M_) && (_SENSORLIB2_ == _SENSORLIB2_CMOS_IMX322LQJVX1_))
	#define SENSOR_TYPE_NUM    "291"
#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_OV4689_) && (_SENSORLIB2_ == _SENSORLIB2_CMOS_IMX322LQJVX1_))
	#define SENSOR_TYPE_NUM    "4689"
#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_AR0238CSP_) && (_SENSORLIB2_ == _SENSORLIB2_CMOS_IMX322LQJVX1_))
       #define SENSOR_TYPE_NUM    "0238"
#else
	#define SENSOR_TYPE_NUM    "ERR"
#endif

//#define PRJ_NAME     "U01"
//#define SERVER_NAME  ".amx"
//#define SENSOR_TYPE_NUM    ".4689"
#define CODE_TYPE_NUM  ".0119"
//#define CODE_TYPE_NUM  ".1013"
//#define FW_VER_NUM   ".T0001"
//#define DATE_NUM  ".161028"




// Retrieve year info
#define OS_YEAR1     ((__DATE__ [9] - '0')* 10 + (__DATE__ [10] - '0'))
// Retrieve month info
#define OS_MONTH1    (__DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 1 : 6) \
                        : __DATE__ [2] == 'b' ? 2 \
                        : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4) \
                        : __DATE__ [2] == 'y' ? 5 \
                        : __DATE__ [2] == 'l' ? 7 \
                        : __DATE__ [2] == 'g' ? 8 \
                        : __DATE__ [2] == 'p' ? 9 \
                        : __DATE__ [2] == 't' ? 10 \
                        : __DATE__ [2] == 'v' ? 11 : 12)
// Retrieve day info
#define OS_DAY1      ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 \
                        + (__DATE__ [5] - '0'))


CHAR FwVersion_string[64]= "";

void Mtk_Prj_GetVersionString(void)
{
       CHAR Str_Temp[64]=""; 
      CHAR String_Array[10]="";
	//strncat(Str_Temp,(CHAR *)PRJ_NAME,strlen((char*)PRJ_NAME));
	strncat(Str_Temp,(CHAR *)SERVER_NAME,strlen((char*)SERVER_NAME));
	strncat(Str_Temp,(CHAR *)SENSOR_TYPE_NUM,strlen((char*)SENSOR_TYPE_NUM));
	strncat(Str_Temp,(CHAR *)CODE_TYPE_NUM,strlen((char*)CODE_TYPE_NUM));
	strncat(Str_Temp,(CHAR *)FW_VER_NUM,strlen((char*)FW_VER_NUM));

	snprintf(String_Array,sizeof(String_Array)-1, ".%02d%02d%02d", OS_YEAR1, OS_MONTH1, OS_DAY1);
	strncat(Str_Temp,(CHAR *)String_Array,strlen((char*)String_Array));
	

       strncpy(FwVersion_string,Str_Temp,strlen(Str_Temp));
}

#endif
/* modify end by ZMD, 2017-02-15 */

char* Prj_GetVersionString()
{
    return gFWExternalVersion;
}

/* modify begin by ZMD, 2017-02-15 new version management*/
#if defined(YQCONFIG_UART_TO_MCU_SUPPORT)

char gMCUVersion[2] = {0};

char*  Prj_GetMCUVersionNum()
{
    return gMCUVersion;
}
BOOL Is_validMCUVersion (void)
{
    return TRUE;
    /*
    if((gMCUVersion[0] != 0x00) || (gMCUVersion[1] != 0x00))
    {
    	return TRUE;
    }
    else
    {
       	return FALSE;
    }
    */
}



#endif
/* modify end by ZMD, 2017-02-15 */



//-----------------------------------------------------------------------------
// User define info
//-----------------------------------------------------------------------------
static char gVersion[9];
static char gModelName[9];
static char gReleaseDate[9];
//extern UINT32 ProjectCode;
//extern UINT32 VersionNumber;
//extern UINT32 ReleaseDate;

//Please modify VersionInfo.s :: ProjectCode
char* Prj_GetModelInfo()
{
    //strncpy(gModelName, (char *)&ProjectCode, 9);
    gModelName[8] = 0;
    return gModelName;
}

//Please modify VersionInfo.s :: VersionNumber
char* Prj_GetVerInfo()
{
    //strncpy(gVersion, (char *)&VersionNumber, 9);
    gVersion[8] = 0;
    return gVersion;
}

//Please modify VersionInfo.s :: ReleaseDate
char* Prj_GetReleaseDate()
{
    //strncpy(gReleaseDate, (char *)&ReleaseDate, 9);
    gReleaseDate[8] = 0;
    return gReleaseDate;
}

//-----------------------------------------------------------------------------
// Compiler-time info
//-----------------------------------------------------------------------------
#include "ProjectInfo.cfg"

// C standard format: Mmm dd yyyy HH:MM:SS (ex. Nov 06 2007 11:15:11)
static CHAR gPrjBuildDateStd[] = __DATE__ ", " __TIME__;

// User defined format: yyyy/mm/dd HH:MM:SS (ex. 2007/11/06 11:15:11)
static CHAR gPrjBuildDateUser[32];

// Retrieve year info
#define OS_YEAR     ((((__DATE__ [7] - '0') * 10 + (__DATE__ [8] - '0')) * 10 \
                        + (__DATE__ [9] - '0')) * 10 + (__DATE__ [10] - '0'))

// Retrieve month info
#define OS_MONTH    (__DATE__ [2] == 'n' ? (__DATE__ [1] == 'a' ? 1 : 6) \
                        : __DATE__ [2] == 'b' ? 2 \
                        : __DATE__ [2] == 'r' ? (__DATE__ [0] == 'M' ? 3 : 4) \
                        : __DATE__ [2] == 'y' ? 5 \
                        : __DATE__ [2] == 'l' ? 7 \
                        : __DATE__ [2] == 'g' ? 8 \
                        : __DATE__ [2] == 'p' ? 9 \
                        : __DATE__ [2] == 't' ? 10 \
                        : __DATE__ [2] == 'v' ? 11 : 12)

// Retrieve day info
#define OS_DAY      ((__DATE__ [4] == ' ' ? 0 : __DATE__ [4] - '0') * 10 \
                        + (__DATE__ [5] - '0'))

// Retrieve hour info
#define OS_HOUR     ((__TIME__ [0] - '0') * 10 + (__TIME__ [1] - '0'))

// Retrieve minute info
#define OS_MINUTE   ((__TIME__ [3] - '0') * 10 + (__TIME__ [4] - '0'))

// Retrieve second info
#define OS_SECOND   ((__TIME__ [6] - '0') * 10 + (__TIME__ [7] - '0'))




//-----------------------------------------------------------------------------
// Project build time info, C standard (US) format
//-----------------------------------------------------------------------------
char* Prj_GetBuildDateStd(void)
{
    return gPrjBuildDateStd;
}

//-----------------------------------------------------------------------------
// Project build time info, user defined format
//-----------------------------------------------------------------------------
char* Prj_GetBuildDateUser(void)
{
    snprintf(gPrjBuildDateUser,sizeof(gPrjBuildDateUser)-1, "%d/%02d/%02d, %s", OS_YEAR, OS_MONTH, OS_DAY, __TIME__);
    return gPrjBuildDateUser;
}

//-----------------------------------------------------------------------------
// Project version string
//-----------------------------------------------------------------------------
extern BININFO gBinInfo;

char* Prj_GetCheckinDate(void)
{
#if (FW_VERSION_RELEASE == ENABLE)
    return gBinInfo.fw.FwInfo_1;
#else
    return gPrjCheckInDate;
#endif
}




//@}
