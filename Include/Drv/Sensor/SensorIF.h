/**
    @file       SensorIF.h

    @ingroup    mIDrvSensor
    @brief      Define sensor internal function
    @note      Sensor interface APIs

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/
#ifndef _SENSOR_IF_H
#define _SENSOR_IF_H

#include "Sensor.h"


#include "lvds.h"
#include "csi.h"
#include "vx1.h"
#include "pll.h"
#include "tge_lib.h"
#include "sie1_lib.h"
#include "sie2_lib.h"
#include "sie3_lib.h"
#include "sie4_lib.h"

typedef struct _SENSOR_IF_TGE
{
    ER (*open)(TGE_OPENOBJ *OpenObj);
    ER (*close)(void);
    ER (*setMode)(TGE_MODE_PARAM *Param);
    ER (*start)(void);
    ER (*pause)(void);
    ER (*waitEvent)(TGE_WAIT_EVENT_SEL WaitEvent, BOOL bClrFlag);
    ER (*isOpened)(void);
} SENSOR_IF_TGE;

//lvds table
typedef struct _SENSOR_IF_LVDS
{
    ER (*open)(void);
    ER (*close)(void);
    ER (*setEnable)(BOOL bEn);
    void (*setConfig)(LVDS_CONFIG_ID ConfigID, UINT32 uiCfgValue);
    LVDS_INTERRUPT (*waitInterrupt)(LVDS_INTERRUPT WaitedFlag);
    void (*setSyncWord)(UINT32 SyncWordLength, UINT32 *pSyncWord);
    void (*setChConfig)(LVDS_DATLANE_ID ChID, LVDS_CH_CONFIG_ID ConfigID, UINT32 uiCfgValue);
    ER (*enableStreaming)(void);
} SENSOR_IF_LVDS;

//csi table
typedef struct _SENSOR_IF_CSI
{
    ER (*open)(void);
    ER (*close)(void);
    ER (*setEnable)(BOOL bEn);
    CSI_INTERRUPT (*waitInterrupt)(CSI_INTERRUPT WaitedFlag);
    void (*setConfig)(CSI_CONFIG_ID ConfigID, UINT32 uiCfgValue);
} SENSOR_IF_CSI;

typedef struct _SENSOR_IF_VX1
{
    ER (*open)(void);
    ER (*close)(void);
    void (*setEnable)(BOOL bEn);
    VX1_INTERRUPT (*waitInterrupt)(VX1_INTERRUPT WaitedFlag);
    void (*setConfig)(VX1_CONFIG_ID ConfigID, UINT32 uiCfgValue);
    ER (*initTransmitter)(VX1_TXMODULE TxModule, VX1_TXINIT aInit);
} SENSOR_IF_VX1;

//sie table
typedef struct _SENSOR_IF_SIE
{
    ER (*SIE_CLK_openClock)(void);
    ER (*SIE_CLK_closeClock)(void);
    ER (*SIE_CLK_chgClockSource)(SIE_CLKSRC_SEL ClkSrc);
    ER (*SIE_CLK_chgClockRate)(UINT32 uiClkRate);
} SENSOR_IF_SIE;

//pll table
typedef struct _SENSOR_IF_PLL
{
    ER (*setClkEn)(PLL_CLK clkID, BOOL bEnable);
    ER (*setClkFreq)(PLL_CLK clkID, UINT32 uiFreq);
} SENSOR_IF_PLL;

extern SENSOR_IF_LVDS* SensorIF_GetLVDSCtrlObj(SENSOR_ID Id);
extern SENSOR_IF_CSI* SensorIF_GetCsiCtrlObj(SENSOR_ID Id);
extern SENSOR_IF_VX1* SensorIF_GetVx1CtrlObj(SENSOR_ID Id);
extern SENSOR_IF_SIE* SensorIF_GetSIECtrlObj(SENSOR_ID Id);
extern SENSOR_IF_TGE* SensorIF_GetTGECtrlObj(SENSOR_ID Id);
extern SENSOR_IF_PLL* SensorIF_GetPllCtrlObj(SENSOR_ID Id);

#endif

