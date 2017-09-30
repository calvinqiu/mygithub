/**
    DxSensor

    Sensor common interface

    @file       DxSensor.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/


#include "Sensor.h"
#include "DxSensor.h"
#include "sif.h"
#include "pll.h"
#include "IOCfg.h"
#include "Delay.h"
#include "SensorDrv.h"
#include "lvds.h"
#include "vx1.h"
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


typedef SENSOR_DRVTAB* (*SEN_GET_DRV) (void);
typedef SENSOR_INIT_OBJ (*SEN_GET_INI_OBJ)(void);
typedef BOOL (*SEN_PLUG_IN)(void);


#if ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX322LQJ_) &&      \
    (_SENSORLIB2_ == _SENSORLIB2_CMOS_TVP5150_) &&            \
    (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
    (_SENSORLIB4_ == _SENSORLIB4_OFF_))

static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
	SENSOR_INIT_OBJ InitObj = {0};
	InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
	InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_2ND;
	InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
	InitObj.ChgMclkEn = ENABLE;
	InitObj.Option = (SENSOR_OPTION_FLIP | SENSOR_OPTION_MIRROR);
	return InitObj;
}

#if _FPGA_EMULATION_
#include "pll_protected.h"
#endif

UINT32 SenPowerOn2_flag = FALSE;

static void SenPowerOn(void)
{
	if (pll_getPLLEn(PLL_ID_5) == DISABLE)
	{
		pll_setPLL(PLL_ID_5, 0xc0000);//54MHz
		pll_setPLLEn(PLL_ID_5, ENABLE);
	}

	pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
	pll_setClkFreq(PLL_CLK_SIEMCLK, 37125000); //37.125M
	pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

	gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
	gpio_clearPin(GPIO_SENSOR_RESET);
	Delay_DelayMs(5);
	gpio_setPin(GPIO_SENSOR_RESET);
}

static void SenPowerOff(void)
{
	pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
	pll_setPLLEn(PLL_ID_5, DISABLE);
}

static void SenPowerSaveOff(void)
{
}

static void SenPowerSaveOn(void)
{
}


static SENSOR_INIT_OBJ Sensor_GetObj2(void)
{
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_2ND;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
    InitObj.CmdInfo.INFO.I2C.extSlaveRAddr = 0xB9;
    InitObj.CmdInfo.INFO.I2C.extSlaveWAddr = 0xB8;
    InitObj.ChgMclkEn = ENABLE;
	InitObj.Option = (SENSOR_OPTION_FLIP);
    return InitObj;
}

static void SenPowerOn2(void)
{
    if (SenPowerOn2_flag == TRUE)
    {
        return;
    }
    SenPowerOn2_flag = TRUE;

    #if 0
    gpio_setPin(GPIO_SENSOR2_POWER_EN);
    Delay_DelayMs(1);
    gpio_clearPin(GPIO_SENSOR2_PDN);
    Delay_DelayMs(1);
    gpio_setPin(GPIO_SENSOR2_PDN);
    Delay_DelayMs(1);
    gpio_clearPin(GPIO_SENSOR2_RESET);
    Delay_DelayMs(1);
    gpio_setPin(GPIO_SENSOR2_RESET);
    debug_msg("----------------------------------- --PDN=%d\r\n",gpio_getPin(GPIO_SENSOR2_PDN));
    #endif
}

static void SenPowerOff2(void)
{
    if (SenPowerOn2_flag == FALSE)
    {
        return;
    }
    SenPowerOn2_flag = FALSE;
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_IMX322LQJ,   Sensor_GetDrvTab_TVP5150,   NULL,   NULL,
};

static SEN_GET_INI_OBJ SenObjTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetObj,   Sensor_GetObj2,   NULL,   NULL,
};

static FP SenPowerOnTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerOn,   SenPowerOn2,   NULL,   NULL,
};

static FP SenPowerOffTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerOff,   SenPowerOff2,   NULL,   NULL,
};

static FP SenPowerSaveOffTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerSaveOff,   SenPowerSaveOff,   NULL,   NULL,
};

static FP SenPowerSaveOnTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerSaveOn,   SenPowerSaveOn,   NULL,   NULL,
};

static BOOL SenDetPlugIn2(void)
{
	SENSOR_CMD sencmd = {0x88,2,0x20F0,0};
	SENSOR_INIT_OBJ SenObj;
    SENSOR_DRVTAB *SenTab;

    SenPowerOn2();
    SenObj = DrvSensor_GetObj(SENSOR_ID_2);
	SenTab = DrvSensor_GetTab(SENSOR_ID_2);
	Sensor_Open(SENSOR_ID_2, &SenObj, SenTab);

	Sensor_ReadReg(0x01, &sencmd);

	Sensor_Close(SENSOR_ID_2);

	return ((((sencmd.uiData[0]) & 0x0F) != 0)?1:0);
}

static SEN_PLUG_IN SenDetPlugInOnTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   SenDetPlugIn2,   NULL,   NULL,
};
#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX322LQJ_) &&      \
       (_SENSORLIB2_ == _SENSORLIB2_CMOS_GM7150_) &&            \
       (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
       (_SENSORLIB4_ == _SENSORLIB4_OFF_))

static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
	SENSOR_INIT_OBJ InitObj = {0};
	InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
	InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_2ND;
	InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
	InitObj.ChgMclkEn = ENABLE;
	InitObj.Option = (SENSOR_OPTION_FLIP | SENSOR_OPTION_MIRROR);
	return InitObj;
}

#if _FPGA_EMULATION_
#include "pll_protected.h"
#endif

static void SenPowerOn(void)
{
	if (pll_getPLLEn(PLL_ID_5) == DISABLE)
	{
		pll_setPLL(PLL_ID_5, 0xc0000);//54MHz
		pll_setPLLEn(PLL_ID_5, ENABLE);
	}

	pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
	pll_setClkFreq(PLL_CLK_SIEMCLK, 37125000); //37.125M
	pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

	gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
	gpio_clearPin(GPIO_SENSOR_RESET);
	Delay_DelayMs(5);
	gpio_setPin(GPIO_SENSOR_RESET);
}

static void SenPowerOff(void)
{
	pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
	pll_setPLLEn(PLL_ID_5, DISABLE);
}

static void SenPowerSaveOff(void)
{
}

static void SenPowerSaveOn(void)
{
}


static SENSOR_INIT_OBJ Sensor_GetObj2(void)
{
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_2ND;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
    InitObj.CmdInfo.INFO.I2C.extSlaveRAddr = 0xB9;
    InitObj.CmdInfo.INFO.I2C.extSlaveWAddr = 0xB8;
    InitObj.ChgMclkEn = ENABLE;
    return InitObj;
}

static void SenPowerOn2(void)
{
    gpio_setPin(GPIO_SENSOR2_POWER_EN);
    Delay_DelayMs(1);
    gpio_clearPin(GPIO_SENSOR2_PDN);
    Delay_DelayMs(1);
    gpio_setPin(GPIO_SENSOR2_PDN);
    Delay_DelayMs(1);
    gpio_clearPin(GPIO_SENSOR2_RESET);
    Delay_DelayMs(1);
    gpio_setPin(GPIO_SENSOR2_RESET);
    debug_msg("----------------------------------- --PDN=%d\r\n",gpio_getPin(GPIO_SENSOR2_PDN));
}

static void SenPowerOff2(void)
{
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_IMX322LQJ,   Sensor_GetDrvTab_GM7150,   NULL,   NULL,
};

static SEN_GET_INI_OBJ SenObjTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetObj,   Sensor_GetObj2,   NULL,   NULL,
};

static FP SenPowerOnTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerOn,   SenPowerOn2,   NULL,   NULL,
};

static FP SenPowerOffTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerOff,   SenPowerOff2,   NULL,   NULL,
};

static FP SenPowerSaveOffTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerSaveOff,   SenPowerSaveOff,   NULL,   NULL,
};

static FP SenPowerSaveOnTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerSaveOn,   SenPowerSaveOn,   NULL,   NULL,
};

static BOOL SenDetPlugIn2(void)
{
	static UINT32 PlugCnt = 0;
	SENSOR_CMD sencmd = {0x88,2,0x20F0,0};

	if(PlugCnt < 2)
		PlugCnt++;
	if(PlugCnt >= 2)
	{
		Sensor_ReadReg(0x01, &sencmd);
		debug_msg("sencmd.uiData[0]) = 0x%x \r\n",sencmd.uiData[0]);
		return ((((sencmd.uiData[0]) & 0x0F) != 0)?1:0);
	}
	else
    	return FALSE;
}

static SEN_PLUG_IN SenDetPlugInOnTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   SenDetPlugIn2,   NULL,   NULL,
};

#elif ((_SENSORLIB_ == _SENSORLIB_OFF_) &&      \
       (_SENSORLIB2_ == _SENSORLIB2_CMOS_TVP5150_) &&            \
       (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
       (_SENSORLIB4_ == _SENSORLIB4_OFF_))
static SENSOR_INIT_OBJ Sensor_GetObj2(void)
{
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_2ND;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
    InitObj.CmdInfo.INFO.I2C.extSlaveRAddr = 0xB9;
    InitObj.CmdInfo.INFO.I2C.extSlaveWAddr = 0xB8;
    InitObj.ChgMclkEn = ENABLE;
    return InitObj;
}

static void SenPowerOn2(void)
{
    gpio_setPin(GPIO_SENSOR2_POWER_EN);
    Delay_DelayMs(10);
    gpio_clearPin(GPIO_SENSOR2_PDN);
    Delay_DelayMs(10);
    gpio_setPin(GPIO_SENSOR2_PDN);
    Delay_DelayMs(10);
    gpio_clearPin(GPIO_SENSOR2_RESET);
    Delay_DelayMs(10);
    gpio_setPin(GPIO_SENSOR2_RESET);
}

static void SenPowerOff2(void)
{
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   Sensor_GetDrvTab_TVP5150,   NULL,   NULL,
};

static SEN_GET_INI_OBJ SenObjTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   Sensor_GetObj2,   NULL,   NULL,
};

static FP SenPowerOnTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   SenPowerOn2,   NULL,   NULL,
};

static FP SenPowerOffTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   SenPowerOff2,   NULL,   NULL,
};

static FP SenPowerSaveOffTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   NULL,   NULL,   NULL,
};

static FP SenPowerSaveOnTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   NULL,   NULL,   NULL,
};

static BOOL SenDetPlugIn2(void)
{
    return TRUE;
}

static SEN_PLUG_IN SenDetPlugInOnTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   SenDetPlugIn2,   NULL,   NULL,
};

#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX322LQJ_) &&      \
       (_SENSORLIB2_ == _SENSORLIB2_CMOS_IMX322LQJVX1_) &&            \
       (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
       (_SENSORLIB4_ == _SENSORLIB_OFF_))
#if _FPGA_EMULATION_
#include "pll_protected.h"
#endif
static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_2ND;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
    InitObj.ChgMclkEn = ENABLE;
    return InitObj;
}

#if _FPGA_EMULATION_
#include "pll_protected.h"
#endif

static void SenPowerOn(void)
{
    if (pll_getPLLEn(PLL_ID_5) == DISABLE)
    {
        pll_setPLL(PLL_ID_5, 0xc0000);//54MHz
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 37125000); //37.125M
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(5);
    gpio_setPin(GPIO_SENSOR_RESET);
}

static void SenPowerOff(void)
{
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
    pll_setPLLEn(PLL_ID_5, DISABLE);
}

static void SenPowerSaveOff(void)
{
}

static void SenPowerSaveOn(void)
{
}

static SENSOR_INIT_OBJ Sensor_GetObj2(void)
{
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_Vx1;
    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C3_Vx1;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
    InitObj.ChgMclkEn = ENABLE;
    return InitObj;
}

static void SenPowerOn2(void)
{
    pll_selectClkSrc(PLL_CLK_SIEMCLK2, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK2, 37125000); //37.125M
    pll_setClkEn(PLL_CLK_SIEMCLK2, ENABLE);
}

static void SenPowerOff2(void)
{
    pll_setClkEn(PLL_CLK_SIEMCLK2, DISABLE);

    if ((pll_isClockEnabled(SIE_MCLK) == DISABLE) &&
        (pll_isClockEnabled(SIE_MCLK2) == DISABLE))
    {
        pll_setPLLEn(PLL_ID_5, DISABLE);
    }
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_IMX322LQJ,   Sensor_GetDrvTab_IMX322LQJVX1,   NULL,   NULL,
};

static SEN_GET_INI_OBJ SenObjTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetObj,   Sensor_GetObj2,   NULL,   NULL,
};

static FP SenPowerOnTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerOn,   SenPowerOn2,   NULL,   NULL,
};

static FP SenPowerOffTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerOff,   SenPowerOff2,   NULL,   NULL,
};

static FP SenPowerSaveOffTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerSaveOff,   SenPowerSaveOff,   NULL,   NULL,
};

static FP SenPowerSaveOnTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerSaveOn,   SenPowerSaveOn,   NULL,   NULL,
};

static BOOL SenDetPlugIn2(void)
{
#if (SENSOR_DET_CTRL == SENSOR_DET_DUMMY)
    return FALSE;
#endif
#if ((SENSOR_DET_CTRL == SENSOR_DET_BY_VIO) && (USE_VIO == ENABLE))
    return (vio_getpin(VIO_SENSOR2_DET) != 0);
#endif
#if (SENSOR_DET_CTRL == SENSOR_DET_BY_VX1)
    if ( vx1_checkSublinkReady() == FALSE )
        return FALSE;
#endif
    return TRUE;
}

static SEN_PLUG_IN SenDetPlugInOnTab[SENSOR_ID_MAX_NUM] =
{
#if (SENSOR_DET_CTRL != SENSOR_DET_DUMMY)
    NULL,   SenDetPlugIn2,   NULL,   NULL,
#else
    NULL,   NULL,   NULL,   NULL,
#endif
};

#else
#define NO_SENSORLIB_MATCHED
#endif


#if 0
#endif
/************************************************************************/
/*                      common function                                 */
/************************************************************************/
#ifndef NO_SENSORLIB_MATCHED
SENSOR_DRVTAB* DrvSensor_GetTab(SENSOR_ID Id)
{
    if (SenDrvTab[Id] == NULL)
    {
        return NULL;
    }
    return SenDrvTab[Id]();
}

SENSOR_INIT_OBJ DrvSensor_GetObj(SENSOR_ID Id)
{
    if (SenObjTab[Id] == NULL)
    {
        SENSOR_INIT_OBJ InitObj = {0};
        return InitObj;
    }
    return SenObjTab[Id]();
}

void DrvSensor_PowerTurnOn(SENSOR_ID Id)
{
    if (SenPowerOnTab[Id] == NULL)
    {
        return;
    }
    SenPowerOnTab[Id]();
}

void DrvSensor_PowerTurnOff(SENSOR_ID Id)
{
    if (SenPowerOffTab[Id] == NULL)
    {
        return;
    }
    SenPowerOffTab[Id]();
}

void DrvSensor_PowerSaveOff(SENSOR_ID Id)
{
    if (SenPowerSaveOffTab[Id] == NULL)
    {
        return;
    }
    SenPowerSaveOffTab[Id]();
}

void DrvSensor_PowerSaveOn(SENSOR_ID Id)
{
    if (SenPowerSaveOnTab[Id] == NULL)
    {
        return;
    }
    SenPowerSaveOnTab[Id]();
}

BOOL DrvSensor_DetPlugIn(SENSOR_ID Id)
{
    if (SenDetPlugInOnTab[Id] == NULL)
    {
        return 0;
    }

    return SenDetPlugInOnTab[Id]();
}
#endif //#ifndef NO_SENSORLIB_MATCHED
