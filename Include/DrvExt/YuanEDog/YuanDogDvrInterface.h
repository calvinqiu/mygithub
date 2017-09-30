/*******************************************************************************/
/** \file YuanDogDvrInterface.h  Basic set of API control by YuanDog moudle.
For more information see \ref Yuanjitong.ltd

* Copyright 2015-2016 Yuanjitong Corporation -- http://www.yuanjitong.com
* This program is the proprietary software of Yuanjitong Corporation and/or
* its licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Yuanjitong (an "Authorized License").
* Except as set forth in an Authorized License, Yuanjitong grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Yuanjitong expressly reserves all rights in and to the
* Software and all intellectual property rights therein. IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY
* WAY, AND SHOULD IMMEDIATELY NOTIFY YUANJITONG AND DISCONTINUE ALL USE OF
* THE SOFTWARE.
*/

#ifndef YUANDOGDVRINTERFACE_H_
#define YUANDOGDVRINTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
//#include "YuanDogBitmapUtils.h"

#define YUAN_VALID_LIMITEDSPEED(s,t)	((s >= 30 && s <= 120) && YUAN_VALID_EDOG(t))	
#define YUAN_ISDAY(x)					(x>=6 && x<=19)	
#define YUAN_VALID_RADAR(x)				(x == 0x40 || x == 0x80  ||  x == 0x58 || x == 0x48 || x == 0x50)

#ifdef YUANDOG_EDOG
#define YUAN_VALID_EDOG(x)		((x > 0 && x <= 0x07) || (x >=0x0A && x <=0x0B) || (x >  0x10 && x <= 0x18) || (x == 0x1D) || (x == 0x27) || (x == 0x2C) || (x == 0x20) || (x == 0x28))
#else
#define YUAN_VALID_EDOG(x)		((x > 0 && x <= 0x07) || (x >=0x0A && x <=0x0B) || (x >  0x10 && x <= 0x2D))
#endif

#define YUAN_IMEI_LEN	15
#define YUAN_IMSI_LEN	18
#define YUAN_ICCID_LEN	20

typedef struct
{
	unsigned short SlienceSpeed;
	unsigned short OverSpeed;
	unsigned short MeasureMode;
	unsigned char  SenMode;
	unsigned char  Volume;
	unsigned char  VoiceMode;
	unsigned char  TraficEnable;
	unsigned short TestMode;
}YUANEDOG_CONFIGURATION;

typedef struct
{
	unsigned char	Type;
	unsigned char	Radar;
	unsigned short	LimitedSpeed;
	float			Distance;
}YUANEDOG_EDOG;

typedef struct
{
	unsigned char Fixed;
	unsigned char nSatellite;
	unsigned long UTCDate;
	unsigned long UTCTime;
	float Latitude;
	float Longitude;
	float Speed;
	float Bearing;
}YUANEDOG_GPS;

typedef struct
{
	unsigned char Prn[32];
	unsigned char InUsed[32];
	unsigned char Snr[32];
}YUANEDOG_SATELLITEINFO;

typedef struct
{
	signed short Type;
	signed short MinTemp;
	signed short MaxTemp;
	signed short AQI;
	signed short PM25;
}YUANEDOG_WEATHER;

typedef struct  
{
	unsigned char	RSSI;
	unsigned char	Cloud;
}YUANEDOG_GSMINFO;

typedef struct  
{
	unsigned short HWVer;
	unsigned short MTKVer;
	unsigned short EDogVer;
	unsigned short ServVer;
	unsigned char  IMEI[YUAN_IMEI_LEN+1];
	unsigned char  ICCID[YUAN_ICCID_LEN+1];
	unsigned char  IMSI[YUAN_IMSI_LEN+1];
}YUANEDOG_DEVICEINFO;

#define YUAN_GPS_MASK			0x01
#define YUAN_EDOG_MASK			0x02
#define YUAN_RADAR_MASK			0x04
#define YUAN_WEATHER_MASK		0x08
#define YUAN_POSITION_MASK		0x10
#define YUAN_TRAFIC_MASK		0x20
#define YUAN_GSM_MASK			0x40
#define YUAN_CLOUDINFO_MASK		0x80

#define YUAN_SHIFT(v,s) ((v?1:0)<<s)
#define YUAN_MERGE_DVR_EVENT(g,e,r,w,p,t,m,c)	(YUAN_SHIFT(g,0) | YUAN_SHIFT(e,1)| YUAN_SHIFT(r,2)| YUAN_SHIFT(w,3)| YUAN_SHIFT(p,4)| YUAN_SHIFT(t,5)| YUAN_SHIFT(m,6)| YUAN_SHIFT(c,7))

extern YUANEDOG_EDOG	YuanEDog;
extern YUANEDOG_GPS		YuanGPS;
extern YUANEDOG_GSMINFO	YuanGSMInfo;
extern YUANEDOG_WEATHER YuanWeather;

extern char	YuanPostion[16];
extern char	YuanTrafic[512];

extern int	YuanGotEDog, YuanGotGPS, YuanGotRadar;
extern int	YuanGotWeather,YuanGotPosition,YuanGotTrafic;
extern int  YuanGotGSMInfo,YuanGotCloudInfo;

#ifdef YUANDOG_MTK_8328
	//#include "AHC_OS.h"

	//extern unsigned long 	ahc_Yuandog_Ready;
#endif

extern int YuanEDogInterfaceInit(int PortNumber,int BandRate);

#ifdef YUANDOG_DEBUG_LOG
	extern int YuanPrintLog(const char* format, ...);
#else
#define YuanPrintLog
#endif

extern int YuanSendPhotoByFile(char *filename,int FrameNum,int *TotalFrame, int *FileSize);
extern int YuanCloudRequestPhoto(unsigned short FrameNum);

#ifdef __cplusplus
}
#endif

//YUANDOGDVRSLAVE_H_
#endif
