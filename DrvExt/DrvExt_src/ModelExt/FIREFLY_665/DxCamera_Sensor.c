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


#if ((_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_) &&   \
     (_SENSORLIB2_ == _SENSORLIB2_OFF_) &&          \
     (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&          \
     (_SENSORLIB4_ == _SENSORLIB4_OFF_))
static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
    /*
    Sensor pin                   LVDS pin
    0   ch 1 port 0  SDOD0 ->    0
    1   ch 1 port 1  SDOD1 ->    1
    2   ch 1 port 2  SDOD3 ->    2
    3   ch 1 port 3  SDOD4 ->    3
    4   ch 2 port 3  SDOD6 ->    4
    5   ch 2 port 2  SDOD7 ->    5
    6   ch 2 port 1  SDOD9 ->    6
    7   ch 2 port 0  SDODA ->    7
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
    InitObj.Sen2LVDSPinMap[4] = 4;
    InitObj.Sen2LVDSPinMap[5] = 5;
    InitObj.Sen2LVDSPinMap[6] = 6;
    InitObj.Sen2LVDSPinMap[7] = 7;
    InitObj.Sen2LVDSPinMap[8] = SEN_IGNORE;
    InitObj.Sen2LVDSPinMap[9] = SEN_IGNORE;
    return InitObj;
}

static void SenPowerOn(void)
{

    gpio_setDir(GPIO_SENSOR_STANDBY, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_SENSOR_STANDBY);
    Delay_DelayMs(20);
    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);
    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);
    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);

    if (pll_getPLLEn(PLL_ID_5) == DISABLE)
    {
        pll_setPLL(PLL_ID_5, 0x90000);//54MHz
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 54000000);// for RealChip emulation
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);
}

static void SenPowerOff(void)
{
    gpio_clearPin(GPIO_SENSOR_RESET);
    gpio_clearPin(GPIO_SENSOR_STANDBY);

    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
    pll_setPLLEn(PLL_ID_5, DISABLE);
}

static void SenPowerSaveOn(void)
{
}

static void SenPowerSaveOff(void)
{
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_MN34110,   NULL,   NULL,   NULL,
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

#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_) &&     \
      (_SENSORLIB2_ == _SENSORLIB2_CMOS_MN34110_) &&    \
      (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&             \
      (_SENSORLIB4_ == _SENSORLIB4_OFF_))

static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
    /*
    Sensor pin                   LVDS pin
    0   ch 1 port 0  SDOD0 ->    0
    1   ch 1 port 1  SDOD1 ->    7
    2   ch 1 port 2  SDOD3 ->    2
    3   ch 1 port 3  SDOD4 ->    3
    4   ch 2 port 3  SDOD6 ->    4
    5   ch 2 port 2  SDOD7 ->    5
    6   ch 2 port 1  SDOD9 ->    6
    7   ch 2 port 0  SDODA ->    1
    */
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_SIF;
    InitObj.CmdInfo.INFO.SIF.busclk = 4800000;//24000000;
    InitObj.CmdInfo.INFO.SIF.chanel = SIF_CH2;
    InitObj.CmdInfo.INFO.SIF.sen_d_s = 1;
    InitObj.CmdInfo.INFO.SIF.sen_h = 1;
    InitObj.CmdInfo.INFO.SIF.DMA_en = DISABLE;

    InitObj.ChgMclkEn = ENABLE;

    InitObj.Sen2LVDSPinMap[0] = 0;
    InitObj.Sen2LVDSPinMap[1] = 7;
    InitObj.Sen2LVDSPinMap[2] = 2;
    InitObj.Sen2LVDSPinMap[3] = 3;
    InitObj.Sen2LVDSPinMap[4] = 4;
    InitObj.Sen2LVDSPinMap[5] = 5;
    InitObj.Sen2LVDSPinMap[6] = 6;
    InitObj.Sen2LVDSPinMap[7] = 1;
    InitObj.Sen2LVDSPinMap[8] = SEN_IGNORE;
    InitObj.Sen2LVDSPinMap[9] = SEN_IGNORE;
    return InitObj;
}

static SENSOR_INIT_OBJ Sensor_GetObj2(void)
{
    /*
    Sensor pin                   LVDS pin
    0   ch 1 port 0  SDOD0 ->    5
    1   ch 1 port 1  SDOD1 ->    1
    2   ch 1 port 2  SDOD3 ->    2
    3   ch 1 port 3  SDOD4 ->    3
    4   ch 2 port 3  SDOD6 ->    4
    5   ch 2 port 2  SDOD7 ->    0
    6   ch 2 port 1  SDOD9 ->    7
    7   ch 2 port 0  SDODA ->    6
    */
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_SIF;
    InitObj.CmdInfo.INFO.SIF.busclk = 4800000;
    InitObj.CmdInfo.INFO.SIF.chanel = SIF_CH4;
    InitObj.CmdInfo.INFO.SIF.sen_d_s = 1;
    InitObj.CmdInfo.INFO.SIF.sen_h = 1;
    InitObj.CmdInfo.INFO.SIF.DMA_en = DISABLE;

    InitObj.ChgMclkEn = ENABLE; //notify sensor driver

    InitObj.Sen2LVDSPinMap[0] = 5;
    InitObj.Sen2LVDSPinMap[1] = 1;
    InitObj.Sen2LVDSPinMap[2] = 2;
    InitObj.Sen2LVDSPinMap[3] = 3;
    InitObj.Sen2LVDSPinMap[4] = 4;
    InitObj.Sen2LVDSPinMap[5] = 0;
    InitObj.Sen2LVDSPinMap[6] = 7;
    InitObj.Sen2LVDSPinMap[7] = 6;
    InitObj.Sen2LVDSPinMap[8] = SEN_IGNORE;
    InitObj.Sen2LVDSPinMap[9] = SEN_IGNORE;

    return InitObj;
}

static void SenPowerOn(void)
{
    if (pll_getPLLEn(PLL_ID_5) == DISABLE)
    {
        pll_setPLL(PLL_ID_5, 0x90000);//54MHz
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 10800000);//10.8MHz
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);
}

static void SenPowerOn2(void)
{
    if (pll_getPLLEn(PLL_ID_5) == DISABLE)
    {
        pll_setPLL(PLL_ID_5, 0x90000);//54MHz
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    pll_selectClkSrc(PLL_CLK_SIEMCLK2, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK2, 10800000);//10.8MHz
    pll_setClkEn(PLL_CLK_SIEMCLK2, ENABLE);
}


static void SenPowerOff(void)
{
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);

    if ((pll_isClockEnabled(SIE_MCLK) == DISABLE) &&
        (pll_isClockEnabled(SIE_MCLK2) == DISABLE))
    {
        pll_setPLLEn(PLL_ID_5, DISABLE);
    }
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

static void SenPowerSaveOn(void)
{

}

static void SenPowerSaveOff(void)
{

}

static void SenPowerSaveOn2(void)
{

}

static void SenPowerSaveOff2(void)
{

}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_MN34110,   Sensor_GetDrvTab_MN34110,   NULL,   NULL,
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
    SenPowerSaveOff,   SenPowerSaveOff2,   NULL,   NULL,
};

static FP SenPowerSaveOnTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerSaveOn,   SenPowerSaveOn2,   NULL,   NULL,
};

static SEN_PLUG_IN SenDetPlugInOnTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   NULL,   NULL,   NULL,
};

#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_AR0330P_) &&      \
       (_SENSORLIB2_ == _SENSORLIB2_OFF_) &&            \
       (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
       (_SENSORLIB4_ == _SENSORLIB_OFF_))

static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_1ST;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = ENABLE;
    InitObj.CmdInfo.INFO.I2C.extSlaveRAddr = 0x21;
    InitObj.CmdInfo.INFO.I2C.extSlaveWAddr = 0x20;

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
        pll_setPLL(PLL_ID_5, 0x90000);//54MHz
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 8000000); //need check ??
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);
}

static void SenPowerOff(void)
{
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);

    if ((pll_isClockEnabled(SIE_MCLK) == DISABLE) &&
        (pll_isClockEnabled(SIE_MCLK2) == DISABLE))
    {
        pll_setPLLEn(PLL_ID_5, DISABLE);
    }
}

static void SenPowerSaveOff(void)
{
}

static void SenPowerSaveOn(void)
{
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_AR0330P,   NULL,   NULL,   NULL,
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

#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_) &&      \
       (_SENSORLIB2_ == _SENSORLIB2_OFF_) &&            \
       (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
       (_SENSORLIB4_ == _SENSORLIB_OFF_))

static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_2ND;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = ENABLE;
    InitObj.CmdInfo.INFO.I2C.extSlaveRAddr = 0x21;
    InitObj.CmdInfo.INFO.I2C.extSlaveWAddr = 0x20;

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
        pll_setPLL(PLL_ID_5, 0x90000);//54MHz
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_480);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 24000000); //need check ??
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);
    gpio_setPin(GPIO_SENSOR_RESET);
}

static void SenPowerOff(void)
{
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);

    if ((pll_isClockEnabled(SIE_MCLK) == DISABLE) &&
        (pll_isClockEnabled(SIE_MCLK2) == DISABLE))
    {
        pll_setPLLEn(PLL_ID_5, DISABLE);
    }
}

static void SenPowerSaveOff(void)
{
}

static void SenPowerSaveOn(void)
{
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_AR0330M,   NULL,   NULL,   NULL,
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

#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_) &&      \
       (_SENSORLIB2_ == _SENSORLIB2_CMOS_AR0330M_) &&            \
       (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
       (_SENSORLIB4_ == _SENSORLIB_OFF_))

static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_2ND;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = ENABLE;
    InitObj.CmdInfo.INFO.I2C.extSlaveRAddr = 0x21;
    InitObj.CmdInfo.INFO.I2C.extSlaveWAddr = 0x20;

    InitObj.ChgMclkEn = ENABLE;
    return InitObj;
}

static SENSOR_INIT_OBJ Sensor_GetObj2(void)
{
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_2ND;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = ENABLE;
    InitObj.CmdInfo.INFO.I2C.extSlaveRAddr = 0x31;
    InitObj.CmdInfo.INFO.I2C.extSlaveWAddr = 0x30;

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
        pll_setPLL(PLL_ID_5, 0x90000);//54MHz
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_480);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 24000000); //need check ??
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);
    gpio_setPin(GPIO_SENSOR_RESET);
}

static void SenPowerOn2(void)
{
    if (pll_getPLLEn(PLL_ID_5) == DISABLE)
    {
        pll_setPLL(PLL_ID_5, 0x90000);//54MHz
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_480);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 24000000); //need check ??
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);
    gpio_setPin(GPIO_SENSOR_RESET);
}

static void SenPowerOff(void)
{
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);

    if ((pll_isClockEnabled(SIE_MCLK) == DISABLE) &&
        (pll_isClockEnabled(SIE_MCLK2) == DISABLE))
    {
        pll_setPLLEn(PLL_ID_5, DISABLE);
    }
}

static void SenPowerOff2(void)
{
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);

    if ((pll_isClockEnabled(SIE_MCLK) == DISABLE) &&
        (pll_isClockEnabled(SIE_MCLK2) == DISABLE))
    {
        pll_setPLLEn(PLL_ID_5, DISABLE);
    }
}

static void SenPowerSaveOff(void)
{
}
static void SenPowerSaveOff2(void)
{
}

static void SenPowerSaveOn(void)
{
}
static void SenPowerSaveOn2(void)
{
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_AR0330M,   Sensor_GetDrvTab_AR0330M,   NULL,   NULL,
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
    SenPowerSaveOff,   SenPowerSaveOff2,   NULL,   NULL,
};

static FP SenPowerSaveOnTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerSaveOn,   SenPowerSaveOn2,   NULL,   NULL,
};

static SEN_PLUG_IN SenDetPlugInOnTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   NULL,   NULL,   NULL,
};

#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_AR0230CS_) &&      \
       (_SENSORLIB2_ == _SENSORLIB2_OFF_) &&            \
       (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
       (_SENSORLIB4_ == _SENSORLIB_OFF_))

static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
    /*
    Sensor pin LVDS pin
    0   ->     0
    1   ->     1
    2   ->     2
    3   ->     3
    */

    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_2ND;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = ENABLE;
    InitObj.CmdInfo.INFO.I2C.extSlaveRAddr = 0x21;
    InitObj.CmdInfo.INFO.I2C.extSlaveWAddr = 0x20;

    InitObj.ChgMclkEn = ENABLE;
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

static void SenPowerOn(void)
{
    if (pll_getPLLEn(PLL_ID_5) == DISABLE)
    {
        pll_setPLL(PLL_ID_5, 0x90000);//54MHz
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 27000000);
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

    gpio_setDir(GPIO_SENSOR_STANDBY, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_SENSOR_STANDBY);

    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);
    gpio_setPin(GPIO_SENSOR_RESET);
}

static void SenPowerOff(void)
{
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);

    if ((pll_isClockEnabled(SIE_MCLK) == DISABLE) &&
        (pll_isClockEnabled(SIE_MCLK2) == DISABLE))
    {
        pll_setPLLEn(PLL_ID_5, DISABLE);
    }
}

static void SenPowerSaveOff(void)
{
}

static void SenPowerSaveOn(void)
{
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_AR0230CS,  NULL,   NULL,   NULL,
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

#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_OV4689_) &&      \
       (_SENSORLIB2_ == _SENSORLIB2_OFF_) &&            \
       (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
       (_SENSORLIB4_ == _SENSORLIB_OFF_))

static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_2ND;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
    InitObj.ChgMclkEn = ENABLE;
    InitObj.Option = SENSOR_OPTION_MIRROR;
    return InitObj;
}

#if _FPGA_EMULATION_
#include "pll_protected.h"
#endif

static void SenPowerOn(void)
{
    gpio_setDir(P_GPIO_36, GPIO_DIR_OUTPUT);
    gpio_setPin(P_GPIO_36);
    gpio_setDir(P_GPIO_37, GPIO_DIR_OUTPUT);
    gpio_setPin(P_GPIO_37);
    Delay_DelayMs(1);
    gpio_setDir(P_GPIO_40, GPIO_DIR_OUTPUT);
    gpio_setPin(P_GPIO_40);

    gpio_setDir(GPIO_SENSOR_STANDBY, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_SENSOR_STANDBY);
    Delay_DelayMs(20);
    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);
    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);
    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);
    if (pll_getPLLEn(PLL_ID_5) == DISABLE)
    {
        pll_setPLL(PLL_ID_5, 0xc0000);//72MHz
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    #if _FPGA_EMULATION_
    pll_setClockRate(PLL_CLKSEL_MIPI_LVDS, PLL_CLKSEL_MIPI_LVDS_120);
    #endif

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 24000000);
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);
}

static void SenPowerOff(void)
{
    gpio_clearPin(GPIO_SENSOR_RESET);
    gpio_clearPin(GPIO_SENSOR_STANDBY);
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);

    if ((pll_isClockEnabled(SIE_MCLK) == DISABLE) &&
        (pll_isClockEnabled(SIE_MCLK2) == DISABLE))
    {
        pll_setPLLEn(PLL_ID_5, DISABLE);
    }
}

static void SenPowerSaveOff(void)
{
}

static void SenPowerSaveOn(void)
{
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_OV4689,   NULL,   NULL,   NULL,
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

#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_) &&      \
       (_SENSORLIB2_ == _SENSORLIB2_CMOS_NT99141_) &&     \
       (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
       (_SENSORLIB4_ == _SENSORLIB4_OFF_))

static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_2ND;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = ENABLE;
    InitObj.CmdInfo.INFO.I2C.extSlaveRAddr = 0x21;
    InitObj.CmdInfo.INFO.I2C.extSlaveWAddr = 0x20;

    InitObj.ChgMclkEn = ENABLE;
    return InitObj;
}

static SENSOR_INIT_OBJ Sensor_GetObj2(void)
{
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;

    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_1ST;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = DISABLE;
    InitObj.CmdInfo.INFO.I2C.extSlaveRAddr = 0x55;
    InitObj.CmdInfo.INFO.I2C.extSlaveWAddr = 0x54;
    InitObj.ChgMclkEn = ENABLE;
    return InitObj;
}

static void SenPowerOn(void)
{

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_480);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 24000000); //need check ??
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);
    gpio_setPin(GPIO_SENSOR_RESET);
}

static void SenPowerOn2(void)
{
    if (pll_getPLLEn(PLL_ID_5) == DISABLE)
    {
        pll_setPLL(PLL_ID_5, 0xc0000);//72MHz
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    pll_selectClkSrc(PLL_CLK_SIEMCLK2, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK2, 24000000);
    pll_setClkEn(PLL_CLK_SIEMCLK2, ENABLE);
}

static void SenPowerOff(void)
{
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);

    if ((pll_isClockEnabled(SIE_MCLK) == DISABLE) &&
        (pll_isClockEnabled(SIE_MCLK2) == DISABLE))
    {
        pll_setPLLEn(PLL_ID_5, DISABLE);
    }
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

static void SenPowerSaveOff(void)
{
}
static void SenPowerSaveOff2(void)
{
}

static void SenPowerSaveOn(void)
{
}
static void SenPowerSaveOn2(void)
{
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_AR0330M,   Sensor_GetDrvTab_NT99141,   NULL,   NULL,
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
    SenPowerSaveOff,   SenPowerSaveOff2,   NULL,   NULL,
};

static FP SenPowerSaveOnTab[SENSOR_ID_MAX_NUM] =
{
    SenPowerSaveOn,   SenPowerSaveOn2,   NULL,   NULL,
};

static SEN_PLUG_IN SenDetPlugInOnTab[SENSOR_ID_MAX_NUM] =
{
    NULL,   NULL,   NULL,   NULL,
};

#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX124LQT_) &&   \
       (_SENSORLIB2_ == _SENSORLIB2_OFF_) &&            \
       (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
       (_SENSORLIB4_ == _SENSORLIB4_OFF_))
static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
    /*
    Sensor pin         LVDS pin
    0   DLOP/M C ->    2
    1   DLOP/M D ->    3
    2   DLOP/M E ->    5
    3   DLOP/M F ->    6
    */
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_2ND;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
    InitObj.ChgMclkEn = ENABLE; //notify sensor driver

    InitObj.Sen2LVDSPinMap[0] = 2;
    InitObj.Sen2LVDSPinMap[1] = 3;
    InitObj.Sen2LVDSPinMap[2] = 5;
    InitObj.Sen2LVDSPinMap[3] = 6;
    InitObj.Sen2LVDSPinMap[4] = SEN_IGNORE;
    InitObj.Sen2LVDSPinMap[5] = SEN_IGNORE;
    InitObj.Sen2LVDSPinMap[6] = SEN_IGNORE;
    InitObj.Sen2LVDSPinMap[7] = SEN_IGNORE;
    InitObj.Sen2LVDSPinMap[8] = SEN_IGNORE;
    InitObj.Sen2LVDSPinMap[9] = SEN_IGNORE;
    return InitObj;
}

static void SenPowerOn(void)
{
    if (pll_getPLLEn(PLL_ID_5) == DISABLE)
    {
        pll_setPLL(PLL_ID_5, 0x90000);//54MHz
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 27000000);//27MHz
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);
}

static void SenPowerOff(void)
{
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
    pll_setPLLEn(PLL_ID_5, DISABLE);
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_IMX124LQT,   NULL,   NULL,   NULL,
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

#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX123LQT_) &&   \
       (_SENSORLIB2_ == _SENSORLIB2_OFF_) &&            \
       (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
       (_SENSORLIB4_ == _SENSORLIB4_OFF_))
static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
    /*
    Sensor pin         LVDS pin
    0   DLOP/M A ->    5
    1   DLOP/M B ->    6
    2   DLOP/M C ->    0
    3   DLOP/M D ->    1
    0   DLOP/M E ->    2
    1   DLOP/M F ->    3
    2   DLOP/M G ->    7
    3   DLOP/M H ->    8
    */
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_2ND;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
    InitObj.ChgMclkEn = ENABLE; //notify sensor driver

    InitObj.Sen2LVDSPinMap[0] = 5;
    InitObj.Sen2LVDSPinMap[1] = 6;
    InitObj.Sen2LVDSPinMap[2] = 0;
    InitObj.Sen2LVDSPinMap[3] = 1;
    InitObj.Sen2LVDSPinMap[4] = 2;
    InitObj.Sen2LVDSPinMap[5] = 3;
    InitObj.Sen2LVDSPinMap[6] = 7;
    InitObj.Sen2LVDSPinMap[7] = 8;
    InitObj.Sen2LVDSPinMap[8] = SEN_IGNORE;
    InitObj.Sen2LVDSPinMap[9] = SEN_IGNORE;
    return InitObj;
}

static void SenPowerOn(void)
{
    if (pll_getPLLEn(PLL_ID_5) == DISABLE)
    {
        pll_setPLL(PLL_ID_5, 0x90000);//54MHz
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 27000000);//27MHz
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);
    gpio_setPin(GPIO_SENSOR_RESET);
}

static void SenPowerOff(void)
{
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
    pll_setPLLEn(PLL_ID_5, DISABLE);
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_IMX123LQT,   NULL,   NULL,   NULL,
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
#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX322LQJ_) &&      \
       (_SENSORLIB2_ == _SENSORLIB2_OFF_) &&            \
       (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
       (_SENSORLIB4_ == _SENSORLIB_OFF_))

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
    Delay_DelayMs(20);
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

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_IMX322LQJ,   NULL,   NULL,   NULL,
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

#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX078CQK_) &&   \
     (_SENSORLIB2_ == _SENSORLIB2_OFF_) &&          \
     (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&          \
     (_SENSORLIB4_ == _SENSORLIB4_OFF_))
static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
    /*
    Sensor pin      LVDS pin
    0   A      ->   0
    1   B      ->   1
    2   C      ->   2
    3   D      ->   3
    4   E      ->   4
    5   F      ->   5
    6   G      ->   6
    7   H      ->   7
    8   I      ->   8
    9   J      ->   9
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
    InitObj.Sen2LVDSPinMap[4] = 4;
    InitObj.Sen2LVDSPinMap[5] = 5;
    InitObj.Sen2LVDSPinMap[6] = 6;
    InitObj.Sen2LVDSPinMap[7] = 7;
    InitObj.Sen2LVDSPinMap[8] = 8;
    InitObj.Sen2LVDSPinMap[9] = 9;
    return InitObj;
}

static void SenPowerOn(void)
{
    gpio_setPin(GPIO_SENSOR_PWM2);  // 1.8 EN   // P_GPIO_37
    gpio_setPin(GPIO_SENSOR_PWM0);  // 1.5 EN   // P_GPIO_40
    gpio_setPin(GPIO_SENSOR_PWM1);  // 2.7 EN   // P_GPIO_36
    gpio_setPin(GPIO_SENSOR_RESET);

    if (pll_getPLLEn(PLL_ID_5) == DISABLE)
    {
        pll_setPLL(PLL_ID_5, 0x300000);//pll_setPLL(PLL_ID_5, 0xC0000);
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 72000000);//72MHz
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);
}

static void SenPowerOff(void)
{
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
    pll_setPLLEn(PLL_ID_5, DISABLE);
}

static void SenPowerSaveOn(void)
{
}

static void SenPowerSaveOff(void)
{
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_IMX078CQK,   NULL,   NULL,   NULL,
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

#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX291M_) &&      \
       (_SENSORLIB2_ == _SENSORLIB2_OFF_) &&            \
       (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
       (_SENSORLIB4_ == _SENSORLIB_OFF_))

static SENSOR_INIT_OBJ Sensor_GetObj(void)
{
    SENSOR_INIT_OBJ InitObj = {0};
    InitObj.CmdInfo.CmdType = SENSOR_CMD_I2C;
    InitObj.CmdInfo.INFO.I2C.I2c = SENSOR_I2C1_2ND;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = FALSE;
    InitObj.CmdInfo.INFO.I2C.extSlaveAddrEnable = ENABLE;
    InitObj.CmdInfo.INFO.I2C.extSlaveRAddr = 0x35;
    InitObj.CmdInfo.INFO.I2C.extSlaveWAddr = 0x34;

    InitObj.ChgMclkEn = ENABLE;
    return InitObj;
}

#if _FPGA_EMULATION_
#include "pll_protected.h"
#endif

static void SenPowerOn(void)
{
    pll_setPLLEn(PLL_ID_5, DISABLE);
    pll_setPLL(PLL_ID_5, 0x63000);//54MHz
    pll_setPLLEn(PLL_ID_5, ENABLE);

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 37125000); //need check ??
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);

    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);
    gpio_setPin(GPIO_SENSOR_RESET);
}

static void SenPowerOff(void)
{
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);

    if ((pll_isClockEnabled(SIE_MCLK) == DISABLE) &&
        (pll_isClockEnabled(SIE_MCLK2) == DISABLE))
    {
        pll_setPLLEn(PLL_ID_5, DISABLE);
    }
}

static void SenPowerSaveOff(void)
{
}

static void SenPowerSaveOn(void)
{
}

static SEN_GET_DRV SenDrvTab[SENSOR_ID_MAX_NUM] =
{
    Sensor_GetDrvTab_IMX291M,   NULL,   NULL,   NULL,
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


#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX206CQC_) &&      \
       (_SENSORLIB2_ == _SENSORLIB2_OFF_) &&            \
       (_SENSORLIB3_ == _SENSORLIB3_OFF_) &&            \
       (_SENSORLIB4_ == _SENSORLIB_OFF_))

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
    gpio_setDir(P_GPIO_40, GPIO_DIR_OUTPUT);
    gpio_setDir(P_GPIO_37, GPIO_DIR_OUTPUT);
    gpio_setDir(P_GPIO_36, GPIO_DIR_OUTPUT);
    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);

    gpio_clearPin(P_GPIO_40);
    gpio_clearPin(P_GPIO_37);
    gpio_clearPin(P_GPIO_36);
    gpio_clearPin(GPIO_SENSOR_RESET);

    gpio_setPin(P_GPIO_40);
    Delay_DelayMs(5);
    gpio_setPin(P_GPIO_37);
    Delay_DelayMs(5);
    gpio_setPin(P_GPIO_36);
    Delay_DelayMs(5);
    gpio_setPin(GPIO_SENSOR_RESET);

    Delay_DelayMs(2);

    if (pll_getPLLEn(PLL_ID_5) == DISABLE)
    {
        pll_setPLL(PLL_ID_5, 0xC0000);
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 72000000);//72MHz
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);
}

static void SenPowerOff(void)
{
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);
    pll_setPLLEn(PLL_ID_5, DISABLE);

    gpio_setDir(P_GPIO_40, GPIO_DIR_OUTPUT);
    gpio_setDir(P_GPIO_37, GPIO_DIR_OUTPUT);
    gpio_setDir(P_GPIO_36, GPIO_DIR_OUTPUT);
    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);

    gpio_clearPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(1);
    gpio_clearPin(P_GPIO_36);
    Delay_DelayMs(1);
    gpio_clearPin(P_GPIO_37);
    Delay_DelayMs(1);
    gpio_clearPin(P_GPIO_40);
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



#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_OV4689_) &&      \
       (_SENSORLIB2_ == _SENSORLIB2_CMOS_IMX322LQJ_) &&            \
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
    InitObj.Option = SENSOR_OPTION_MIRROR;
    return InitObj;
}


static void SenPowerOn(void)
{
    gpio_setDir(GPIO_SENSOR_STANDBY, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_SENSOR_STANDBY);
    Delay_DelayMs(20);
    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);
    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_clearPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);
    gpio_setDir(GPIO_SENSOR_RESET, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_SENSOR_RESET);
    Delay_DelayMs(20);
    if (pll_getPLLEn(PLL_ID_5) == DISABLE)
    {
        pll_setPLL(PLL_ID_5, 0xc0000);//72MHz
        pll_setPLLEn(PLL_ID_5, ENABLE);
    }

    #if _FPGA_EMULATION_
    pll_setClockRate(PLL_CLKSEL_MIPI_LVDS, PLL_CLKSEL_MIPI_LVDS_120);
    #endif

    pll_selectClkSrc(PLL_CLK_SIEMCLK, PLL_CLKSRC_PLL5);
    pll_setClkFreq(PLL_CLK_SIEMCLK, 24000000);
    pll_setClkEn(PLL_CLK_SIEMCLK, ENABLE);
}

static void SenPowerOff(void)
{
    gpio_clearPin(GPIO_SENSOR_RESET);
    gpio_clearPin(GPIO_SENSOR_STANDBY);
    pll_setClkEn(PLL_CLK_SIEMCLK, DISABLE);

    if ((pll_isClockEnabled(SIE_MCLK) == DISABLE) &&
        (pll_isClockEnabled(SIE_MCLK2) == DISABLE))
    {
        pll_setPLLEn(PLL_ID_5, DISABLE);
    }
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
    Sensor_GetDrvTab_OV4689,   Sensor_GetDrvTab_IMX322LQJ,   NULL,   NULL,
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
