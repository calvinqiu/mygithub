/*******************************************************************************/
/** \file YuanDogBitmapUtils.h  Basic set of API control by YuanDog moudle.
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

#ifndef YUANDOGBITMAPUTILS_H_
#define YUANDOGBITMAPUTILS_H_

////////////////////////////////////////////////////
//switch for difference platform
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#define YUANDOG_NOVATEK_96655
#define YUANDOG_SOFT_DOG
//#define YUANDOG_SOFT_DOG

#ifdef WIN32
	#pragma warning (disable:4996)
#endif

#if defined (YUANDOG_ALLWINNER_V3) || defined(YUANDOG_MINIGUI) || defined(YUANDOG_SIMMINIGUI)
	#ifndef YUANDOG_MINIGUI
		#define YUANDOG_MINIGUI
	#endif
	#define YUANDOG_BITMAPFILE
	#define YUANDOG_SUPPORT_MAINWINDOW
	#define YUANDOG_SUPPORT_STATUSBAR
	#define YUANDOG_SUPPORT_RECORDERWINDOW
	#define YUAN_FONTFILE
	#define YUANDOG_DEBUG_LOG
#elif defined (YUANDOG_MTK_8328) || defined(YUANDOG_UCGUI)
	#ifndef YUANDOG_UCGUI
		#define YUANDOG_UCGUI
	#endif
	#define YUANDOG_BITMAPROM
	#define YUANDOG_SUPPORT_MAINWINDOW
	//#define YUANDOG_SUPPORT_STATUSBAR
	#define YUANDOG_SUPPORT_RECORDERWINDOW
	#define YUAN_FONT32
	#define YUANDOG_DEBUG_LOG
#elif defined WIN32
	#define YUANDOG_DEBUG_LOG
#endif

#if !defined (WIN32) || !defined (MAX_PATH)
	#define MAX_PATH	50
#endif

#ifdef YUANDOG_MINIGUI
	#include <minigui/common.h>
	#include <minigui/minigui.h>
	#include <minigui/gdi.h>
	#include <minigui/window.h>
	#include <minigui/control.h>

	#undef _USE_OWN_SPRINTF
	#undef sprintf
#elif defined   YUANDOG_UCGUI
	#include "GUI.h"
	#ifdef 	YUANDOG_MTK_8328
		#include "AHC_GUI.h"
	#endif	

	#define HDC			unsigned short
	#define HDC_SCREEN	0	
	
	#ifdef 	YUANDOG_MTK_8328
		#define left 	uiLeft
		#define top 	uiTop
		#define right 	uiWidth
		#define bottom 	uiHeight
	#else
		//	#define	NULL		(void*)0
		#define TRUE		1
		#define FALSE		0

		typedef struct
		{
			int left;
			int top;
			int right;
			int bottom;
		} RECT;
	#endif	

	typedef struct  
	{
		unsigned char*	pData;
		int				Length;
	}BITMAP,*PBITMAP;

#endif

#ifndef YUANDOG_SUPPORT_STATUSBAR
	#define YUANDVR_WINDOW_WIDTH		800
	#define YUANDVR_WINDOW_HEIGHT		480

	#define YUANDOG_RECT_COV(x)			x.left, x.top, x.right,x.bottom
	#define YUANDOG_RECT_TOP(x)			x.top

	#define YUANDOG_RECT2WINRECT(y,w)	do{	\
											w.top		= y.top;					\
											w.bottom	= w.top  + y.bottom;	\
											w.left		= y.left;				\
											w.right		= y.left + y.right;		\
										}while(0)	
#else
	#define YUANDOG_STATUSBAR_HEIGHT	44	
	#define YUANDVR_WINDOW_WIDTH		800
	#define YUANDVR_WINDOW_HEIGHT		480-YUANDOG_STATUSBAR_HEIGHT

	#define YUANDOG_RECT_COV(x)			x.left, x.top-YUANDOG_STATUSBAR_HEIGHT, x.right,x.bottom
	#define YUANDOG_RECT_TOP(x)			(x.top-YUANDOG_STATUSBAR_HEIGHT)
	
	#define YUANDOG_RECT2WINRECT(y,w)	do{	\
											w.top		= y.top	-	YUANDOG_STATUSBAR_HEIGHT;	\
											w.bottom	= w.top  + y.bottom;			\
											w.left		= y.left;						\
											w.right		= y.left + y.right;				\
										}while(0)
#endif

#define YUAN_FONT_ASCII_WIDHT			16
#define YUAN_FONT_GB2312_WIDHT			32
#define YUAN_FONT_HEIGHT				32

enum
{
	YUAN_LANGUAGE_ENGLISH,
	YUAN_LANGUAGE_CHINESE,
	YUAN_LANGUAGE_CHINESE_TRAD
};

#ifdef __cplusplus
}
#endif

//YUANDOGBITMAPUTILS_H_
#endif
