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


#if ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX206CQC_) &&      \
       (_SENSORLIB2_ == _SENSORLIB2_OFF_) &&            \
       (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
       (_SENSORLIB4_ == _SENSORLIB4_OFF_))
static ER SENSOR_IO(SENSOR_ID Id, UINT32 uiData0)
{
    UINT32 uiData0_bit;
    UINT32 BitCount;

    gpio_setDir(S_GPIO_9, GPIO_DIR_OUTPUT);
    gpio_setDir(S_GPIO_5, GPIO_DIR_OUTPUT);
    gpio_setDir(S_GPIO_6, GPIO_DIR_OUTPUT);

    gpio_setPin(S_GPIO_9);
    gpio_setPin(S_GPIO_5);
    gpio_setPin(S_GPIO_6);

    gpio_clearPin(S_GPIO_9);

    for (BitCount=0 ; BitCount<32 ; BitCount++)
    {
        gpio_clearPin(S_GPIO_5);

        uiData0_bit = ((uiData0 >> BitCount) & (0x1));

        if (uiData0_bit == 0)
            gpio_clearPin(S_GPIO_6);
        else
            gpio_setPin(S_GPIO_6);


        gpio_setPin(S_GPIO_5);
    }

    gpio_setPin(S_GPIO_9);

    return E_OK;
}

static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
    /*
    Sensor pin      LVDS pin
    0   A      ->   0
    1   B      ->   1
    2   C      ->   2
    3   D      ->   3
    */
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_SIF;
    InitObj.CmdInfo.INFO.SIF.busclk = 24000000;
    InitObj.CmdInfo.INFO.SIF.chanel = SIF_CH0;
    InitObj.CmdInfo.INFO.SIF.sen_d_s = 1;
    InitObj.CmdInfo.INFO.SIF.sen_h = 1;
    InitObj.CmdInfo.INFO.SIF.DMA_en = DISABLE;

    InitObj.ChgMclkEn = ENABLE; //notify sensor driver

    InitObj.Sen2LVDSPinMap[0] = 0;
    InitObj.Sen2LVDSPinMap[1] = 1;
    InitObj.Sen2LVDSPinMap[2] = 2;
    InitObj.Sen2LVDSPinMap[3] = 3;
    InitObj.Sen2LVDSPinMap[4] = SEN_IGNORE;
    InitObj.Sen2LVDSPinMap[5] = SEN_IGNORE;
    InitObj.Sen2LVDSPinMap[6] = SEN_IGNORE;
    InitObj.Sen2LVDSPinMap[7] = SEN_IGNORE;
    InitObj.Sen2LVDSPinMap[8] = SEN_IGNORE;
    InitObj.Sen2LVDSPinMap[9] = SEN_IGNORE;
    return InitObj;

}

#if _FPGA_EMULATION_
#include "pll_protected.h"
#endif

static void SenPowerOn(void)
{
    gpio_setDir(P_GPIO_8, GPIO_DIR_OUTPUT);
    gpio_setDir(P_GPIO_9, GPIO_DIR_OUTPUT);
    gpio_setDir(P_GPIO_10, GPIO_DIR_OUTPUT);
    gpio_setDir(P_GPIO_11, GPIO_DIR_OUTPUT);
    gpio_setDir(S_GPIO_9, GPIO_DIR_OUTPUT);
    gpio_setDir(S_GPIO_10, GPIO_DIR_OUTPUT);
    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);

    //gpio_clearPin(P_GPIO_8);
    //gpio_clearPin(P_GPIO_9);
    //gpio_clearPin(P_GPIO_10);
    //gpio_clearPin(P_GPIO_11);
    gpio_clearPin(GPIO_SENSOR_RESET);

    gpio_setPin(P_GPIO_11);
    Delay_DelayMs(5);
    gpio_setPin(P_GPIO_10);
    Delay_DelayMs(5);
    gpio_setPin(P_GPIO_9);
    Delay_DelayMs(5);
    gpio_setPin(P_GPIO_8);
    Delay_DelayMs(5);
    gpio_setPin(GPIO_SENSOR_RESET);

    Delay_DelayMs(2);
    //gpio_setPin(S_GPIO_9);
    gpio_setPin(S_GPIO_10);

    if (pll_getPLLEn(PLL_ID_5) == DISABLE)
    {
        pll_setPLL(PLL_ID_5, 0xC0000);
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 72000000);//72MHz
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);
    
    //gpio_clearPin(S_GPIO_9);    
    gpio_clearPin(S_GPIO_10);    
}

static void SenPowerOff(void)
{
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
    pll_setPLLEn(PLL_ID_5, DISABLE);

    gpio_setDir(P_GPIO_11, GPIO_DIR_OUTPUT);
    gpio_setDir(P_GPIO_10, GPIO_DIR_OUTPUT);
    gpio_setDir(P_GPIO_9, GPIO_DIR_OUTPUT);
    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);

    gpio_clearPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(1);
    gpio_clearPin(P_GPIO_9);
    Delay_DelayMs(1);
    gpio_clearPin(P_GPIO_10);
    Delay_DelayMs(1);
    gpio_clearPin(P_GPIO_11);
}

static void SenPowerSaveOff(void)
{
}

static void SenPowerSaveOn(void)
{
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_IMX206CQC,   NULL,   NULL,   NULL,
};

static SEN_GET_INI_OBJ SenObjTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetObj,   NULL,   NULL,   NULL,
};

static FP SenPowerOnTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerOn,   NULL,   NULL,   NULL,
};

static FP SenPowerOffTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerOff,   NULL,   NULL,   NULL,
};

static FP SenPowerSaveOffTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerSaveOff,   NULL,   NULL,   NULL,
};

static FP SenPowerSaveOnTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerSaveOn,   NULL,   NULL,   NULL,
};

static SEN_PLUG_IN SenDetPlugInOnTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   NULL,   NULL,   NULL,
};

#elif ((_SENSORLIB_ == _SENSORLIB_OFF_) &&  \
       (_SENSORLIB2_ == _SENSORLIB_OFF_) && \
       (_SENSORLIB3_ == _SENSORLIB_OFF_) && \
       (_SENSORLIB4_ == _SENSORLIB_OFF_))
static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   NULL,   NULL,   NULL,
};

static SEN_GET_INI_OBJ SenObjTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   NULL,   NULL,   NULL,
};

static FP SenPowerOnTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   NULL,   NULL,   NULL,
};

static FP SenPowerOffTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   NULL,   NULL,   NULL,
};

static FP SenPowerSaveOffTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   NULL,   NULL,   NULL,
};

static FP SenPowerSaveOnTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   NULL,   NULL,   NULL,
};

static SEN_PLUG_IN SenDetPlugInOnTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   NULL,   NULL,   NULL,
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
