#ifndef YUANDOGUTILS_H__
#define YUANDOGUTILS_H__

//#include "YuanDogETCConfig.h"
//#include "YuanDogHSGCConfig.h"
//#include "YuanDogMTK8328Config.h"
#include "YuanDogNT9666xConfig.h"

#if defined(__MTK__)
#pragma diag_suppress 870,1
#elif defined(WIN32)
#pragma warning (disable: 4996)
#endif

#if defined(__NOVATEK__) || defined(__MTK8328__) || defined(__JIELI__)
#define __FUNCTION__ __func__
#endif


#define YUANSYS_TICKET_INTERVAL		200

#define YUANDOG_EDOG_VER		109
#define YUANDOG_YUANDOG_VER		250

typedef unsigned long	uint32;
typedef unsigned short	uint16;
typedef unsigned char	uint8;

#if defined(__JIELI__)
#include <stdio.h>
#else
typedef unsigned int	uint;
#endif

typedef long	int32;
typedef short	int16;
typedef char	int8;

#define	YUAN_MIN(a,b)	(((a)>(b)) ? (b) : (a))
#define	YUAN_MAX(a,b)	(((a)>(b)) ? (a) : (b))

#if 0
	#ifndef FALSE
	#define FALSE	0
	#endif
	#ifndef TRUE
	#define TRUE	1
	#endif
#endif
	
#ifndef NULL
#define NULL	(void*)0
#endif

#ifdef __cplusplus
extern "C" 
#endif
void YuanHalPrintHalLog(uint16 PriMask, const char *fmt, ...);

#ifndef ASSERT
#define ASSERT(arg)		do{	\
							if (!(arg))	\
							{	LOG_HAL_ERR("%s:%d\r\n",__FUNCTION__,__LINE__);	}\
							YuanHalAssert((uint32)(arg));		\
						}while(0);
#endif

typedef uint32 (*YUANDOGBARKPROC)(uint32 code, void* wParam,  void* lParam);

#endif
