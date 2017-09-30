/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       IOCfg.h
    @ingroup    mIPRJAPCommonIO

    @brief      Header file of IO config
                This file is the header file of IO config

    @note       Nothing.
    @date       2012/09/04
*/

/** \addtogroup mIPRJAPCommonIO */
//@{

#ifndef _IOCFG_H
#define _IOCFG_H
#include "DxCfg.h"
#include "IOCfg.h"
#include "Debug.h"
#include "DxPower.h"
#include "Utility.h"
#include "Type.h"
#include "i2c.h"
#include "top.h"

 #define ABS(x)				((x) >0 ? (x) : -(x) )

 /*初始化开路电压对应百分比表*/

#define OCVREG0				0x00		//3.1328
#define OCVREG1				0x01		//3.2736
#define OCVREG2				0x02		//3.4144
#define OCVREG3				0x05		//3.5552
#define OCVREG4				0x07		//3.6256
#define OCVREG5				0x0D		//3.6608
#define OCVREG6				0x10		//3.6960
#define OCVREG7				0x1A		//3.7312
#define OCVREG8				0x24		//3.7664
#define OCVREG9				0x2E		//3.8016
#define OCVREGA				0x35		//3.8368
#define OCVREGB				0x3D		//3.8720
#define OCVREGC				0x49		//3.9424
#define OCVREGD				0x54		//4.0128
#define OCVREGE				0x5C		//4.0832
#define OCVREGF				0x64		//4.1536

/* 初始化开路电压*/

#define OCVVOL0				3132
#define OCVVOL1				3273
#define OCVVOL2				3414
#define OCVVOL3				3555
#define OCVVOL4				3625
#define OCVVOL5				3660
#define OCVVOL6				3696
#define OCVVOL7				3731
#define OCVVOL8				3766
#define OCVVOL9				3801
#define OCVVOLA				3836
#define OCVVOLB				3872
#define OCVVOLC				3942
#define OCVVOLD				4012
#define OCVVOLE				4083
#define OCVVOLF				4153


typedef enum
{
    PMU_I2C_REGISTER_1BYTE = 1,
    PMU_I2C_REGISTER_2BYTE = 2,
    ENUM_DUMMY4WORD(PMU_I2C_REGISTER_BYTES)
} PMU_I2C_REGISTER_BYTES, *PPMU_I2C_REGISTER_BYTES;

typedef struct
{
	I2C_SESSION I2C_Channel;
    PMU_I2C_REGISTER_BYTES  I2C_RegBytes;
    I2C_PINMUX      I2C_PinMux;
    I2C_BUS_CLOCK   I2C_BusClock;
    UINT32          I2C_Slave_WAddr;
    UINT32          I2C_Slave_RAddr;
} PMU_INFO,*PPMU_INFO;

BOOL Pmu_I2C_Init(PMU_INFO PmuInfo);
void Pmu_I2C_WriteReg(UINT32 uiAddr, UINT32 uiValue);
UINT32 Pmu_I2C_ReadReg(UINT32 uiAddr);
extern BOOL PMU_Init(void);
extern void PMU_Off(void);
extern UINT8 PMU_GetBatteryLevel(void);

#endif

//@}

