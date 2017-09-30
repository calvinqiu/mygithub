/*
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       GPS.c
    @ingroup

    @brief      Excute command via UART interface.
                Excute command and output message via UART interface.

    @note       Nothing.

    @version    V1.00.000
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Debug.h"
#include "SysKer.h"
#include "kernel.h"
#include "uart.h"
#include "YuanDogTsk.h"
#include "../../../Project/DemoKit/SrcCode/NVTUserCommand.h"


//@{
#define THIS_DBGLVL         2       //0=OFF, 1=ERROR, 2=TRACE
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[buf]"
#include "DebugModule.h"
//#include "Perf.h"



typedef unsigned long long  U64;     ///< Unsigned 64 bits data type
typedef signed long long    S64;      ///< Signed 64 bits data type
typedef unsigned long       U32;     ///< Unsigned 32 bits data type
typedef signed long         S32;      ///< Signed 32 bits data type
typedef unsigned short      U16;     ///< Unsigned 16 bits data type
typedef signed short        S16;      ///< Signed 16 bits data type
typedef unsigned char       U8;      ///< Unsigned 8 bits data type
typedef signed char         S8;       ///< Signed 8 bits data type

UINT32  YUAN_DOG_TSK_ID = 0;
UINT32  FLG_ID_YUAN_DOG = 0;

static BOOL bEdogTskOpen = FALSE;

#define PRI_YUAN_DOG            12//10
#define STKSIZE_YUAN_DOG        6144//64KB stack size

void YuanDog_InstallID(void)
{
    OS_CONFIG_TASK(YUAN_DOG_TSK_ID, PRI_YUAN_DOG, STKSIZE_YUAN_DOG, YuanDogTsk);
    OS_CONFIG_FLAG(FLG_ID_YUAN_DOG);
}

ER YuanDogTask_Open()
{
    if (bEdogTskOpen)
        return E_SYS;

    DBG_DUMP("Start YuanDogTask_Open task\r\n");
    sta_tsk(YUAN_DOG_TSK_ID,0);

    loc_cpu();
    bEdogTskOpen = TRUE;
    unl_cpu();

    return E_OK;
}

ER YuanDogTask_Close(void)
{
    if (!bEdogTskOpen)
        return E_SYS;

    DBG_DUMP("Terminate Yuan Edog receive task\r\n");

    ter_tsk(YUAN_DOG_TSK_ID);

    loc_cpu();
    bEdogTskOpen = FALSE;
    unl_cpu();

    return E_OK;
}


void YuanDogTsk(void)
{
	//Delay_DelayMs(1000);
    DBG_DUMP("Start YuanDogTsk task\r\n");
	DogBark();
}


