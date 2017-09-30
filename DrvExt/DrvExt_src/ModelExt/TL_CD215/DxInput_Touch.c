/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       Input_Touch.c
    @ingroup    mIPRJAPKeyIO

    @brief      Scan Touch Panel HW level

    @note       Nothing.

    @date       2009/04/22
*/

/** \addtogroup mIPRJAPKeyIO */
//@{
#if (_TOUCH_FUNC_ == _TOUCH_ENABLE_)

#include "kernel.h"
#include "Debug.h"
#include "DxInput.h"
#include "adc.h"
#include "Utility.h"
#include "i2c.h"
#include "IOCfg.h"
#include "mIType.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define TP_PRESSURE_TH      240     // 255 - 10

#define TP_HW_MAX_X         320
#define TP_HW_MAX_Y         240
#define TP_HW_MIN_X         0
#define TP_HW_MIN_Y         0

#define TP_PANEL_X1         0
#define TP_PANEL_Y1         0
#define TP_PANEL_X2         319
#define TP_PANEL_Y2         239

#define TP_PANEL_W          (TP_PANEL_X2-TP_PANEL_X1 + 1)
#define TP_PANEL_H          (TP_PANEL_Y2-TP_PANEL_Y1 + 1)

#define READ_ONE_TP_ONLY    1

//////////////////////////////////////////////////////////////////////
//for touch panel
//Touch panel I2C info structure
typedef struct
{
    I2C_SESSION     I2C_Channel;
    I2C_PINMUX      I2C_PinMux;
    I2C_BUS_CLOCK   I2C_BusClock;
    UINT32          I2C_Slave_WAddr;
    UINT32          I2C_Slave_RAddr;
} TP_INFO, *PTP_INFO;

//Touch Panel recieved data structure // 2 points touch
typedef struct
{
    UINT32 P1_TouchStatus;
    UINT32 P1_TouchEventID;
    UINT32 P1_CoordRecvdX;
    UINT32 P1_CoordRecvdY;
    UINT32 P2_TouchStatus;
    UINT32 P2_TouchEventID;
    UINT32 P2_CoordRecvdX;
    UINT32 P2_CoordRecvdY;
} TP_RECV_DATA, *pPT_RECV_DATA;

//////////////////////////////////////////////////////////////////////

static TP_RECV_DATA     g_TPRecvdData;
static pPT_RECV_DATA    pTPRecvdData = &g_TPRecvdData;
static TP_INFO          g_TpInfo;

/*
    0:  Use AUTO Mode with Polling
    1:  Use AUTO Mode with Interrupt (N/A)
    2:  Use AUTO Mode with FIFO Threshold (N/A)
*/
#define GSTP_USED_MODE      0

static BOOL TP_I2C_Init(TP_INFO TouchInfo)
{
    UINT erReturn;

    g_TpInfo.I2C_PinMux = TouchInfo.I2C_PinMux;
    g_TpInfo.I2C_BusClock = TouchInfo.I2C_BusClock;
    g_TpInfo.I2C_Slave_WAddr = TouchInfo.I2C_Slave_WAddr;
    g_TpInfo.I2C_Slave_RAddr = TouchInfo.I2C_Slave_RAddr;

    if (g_TpInfo.I2C_PinMux > I2C_PINMUX_2ND)
    {
        DBG_ERR("I2C pin mux err!\r\n");
        return FALSE;
    }

    if (g_TpInfo.I2C_BusClock > I2C_BUS_CLOCK_1MHZ)
    {
        DBG_ERR("I2C bus clock err!\r\n");
        return FALSE;
    }

    erReturn = i2c_open(&(g_TpInfo.I2C_Channel));
    if(erReturn != I2C_STS_OK)
    {
        DBG_ERR("Open I2C driver err!\r\n");
        return FALSE;
    }

    i2c_setConfig(g_TpInfo.I2C_Channel, I2C_CONFIG_ID_PINMUX, g_TpInfo.I2C_PinMux); //select to control I2C 1st device
    i2c_setConfig(g_TpInfo.I2C_Channel, I2C_CONFIG_ID_BUSCLOCK, g_TpInfo.I2C_BusClock); //up to 400KHZ

    return TRUE;
}

static UINT32 DetTP_I2C_BurstRead(UINT32 uiAddr, UINT32 *uiValue)
{
    I2C_DATA    I2cData;
    I2C_BYTE    I2cByte[4];
    I2C_STS     ret;
    INT32       i;

    if (i2c_lock(g_TpInfo.I2C_Channel) != E_OK)
    {
        DBG_ERR("I2C lock fail!\r\n");
        return 0;
    }

    I2cData.VersionInfo = DRV_VER_96660;
    I2cData.ByteCount = I2C_BYTE_CNT_3;
    I2cData.pByte = I2cByte;
    I2cByte[0].uiValue = g_TpInfo.I2C_Slave_WAddr;
    I2cByte[0].Param = I2C_BYTE_PARAM_START;
    I2cByte[1].uiValue = (uiAddr & 0x000000ff);
    I2cByte[1].Param = I2C_BYTE_PARAM_NONE;
    I2cByte[2].uiValue = g_TpInfo.I2C_Slave_RAddr;
    I2cByte[2].Param = I2C_BYTE_PARAM_START;
    ret = i2c_transmit(&I2cData);
    if (ret != I2C_STS_OK)
    {
        DBG_ERR("(W)i2c ret = %02x!!\r\n", ret);
    }

    I2cData.VersionInfo = DRV_VER_96660;
    I2cData.ByteCount = I2C_BYTE_CNT_4;
    I2cData.pByte = I2cByte;
    I2cByte[0].Param = I2C_BYTE_PARAM_NONE | I2C_BYTE_PARAM_ACK;
    I2cByte[1].Param = I2C_BYTE_PARAM_NONE | I2C_BYTE_PARAM_ACK;
    I2cByte[2].Param = I2C_BYTE_PARAM_NONE | I2C_BYTE_PARAM_ACK;
    I2cByte[3].Param = I2C_BYTE_PARAM_NONE | I2C_BYTE_PARAM_NACK | I2C_BYTE_PARAM_STOP;
    ret = i2c_receive(&I2cData);
    if ( ret != I2C_STS_OK )
    {
        DBG_ERR("(R)i2c ret = %02x!!\r\n", ret);
    }

    if (i2c_unlock(g_TpInfo.I2C_Channel) != E_OK)
    {
        DBG_ERR("I2C unlock fail!\r\n");
        return  0;
    }

    for(i = 0; i < 4; i ++)
    {
        uiValue[i] = I2cByte[i].uiValue;
    }
    return 1;
}

static void DetTP_Convert2RealXY(UINT32 *pX, UINT32 *pY)
{
    INT32 tempX=0,tempY=0;
    tempX = *pX;
    tempY = *pY;
    if (tempX > TP_HW_MAX_X)
        tempX = TP_HW_MAX_X;
    else if (tempX < TP_HW_MIN_X)
        tempX = TP_HW_MIN_X;

    if (tempY > TP_HW_MAX_Y)
        tempY = TP_HW_MAX_Y;
    else if (tempY < TP_HW_MIN_Y)
        tempY = TP_HW_MIN_Y;

    *pX = tempX;
    *pY = TP_PANEL_H - tempY - 1;
    DBG_IND("Convert:(%d,%d) to (%d,%d)\r\n", tempX, tempY, *pX, *pY);
}

static void DetTP_GetData(pPT_RECV_DATA pData)
{
#if (READ_ONE_TP_ONLY == 1)
    UINT32 recv_data[4] = {0};

    DetTP_I2C_BurstRead (0x00, recv_data);

    pData->P1_TouchStatus   = recv_data[0] & 0x07;
    pData->P1_TouchEventID  = (recv_data[0] & ~0x07) >> 3;
    pData->P1_CoordRecvdY   = recv_data[1] * 16 + recv_data[3] / 16;
    pData->P1_CoordRecvdX   = recv_data[2] * 16 + recv_data[3] % 16;
    DetTP_Convert2RealXY(&(pData->P1_CoordRecvdX), &(pData->P1_CoordRecvdY));

    pData->P2_TouchStatus   = 0;
    pData->P2_TouchEventID  = 0;
    pData->P2_CoordRecvdX   = 0;
    pData->P2_CoordRecvdY   = 0;
#else
    UINT32 recv_data[12];

    DetTP_I2C_BurstRead (0x00, &(recv_data[0]));
    DetTP_I2C_BurstRead (0x06, &(recv_data[6]));

    pData->P1_TouchStatus   = recv_data[0] & 0x07;
    pData->P1_TouchEventID  = (recv_data[0] & ~0x07) >> 3;
    pData->P1_CoordRecvdY   = recv_data[1] * 16 + recv_data[3] / 16;
    pData->P1_CoordRecvdX   = recv_data[2] * 16 + recv_data[3] % 16;
    DetTP_Convert2RealXY(&(pData->P1_CoordRecvdX), &(pData->P1_CoordRecvdY));

    if (recv_data[6] != 0xff)
    {
        pData->P2_TouchStatus   = recv_data[6] & 0x07;
        pData->P2_TouchEventID  = (recv_data[6] & ~0x07) >> 3;
        pData->P2_CoordRecvdY   = recv_data[7] * 16 + recv_data[9] / 16;
        pData->P2_CoordRecvdX   = recv_data[8] * 16 + recv_data[9] % 16;
        DetTP_Convert2RealXY(&(pData->P2_CoordRecvdX), &(pData->P2_CoordRecvdY));
    }
    else
    {
        pData->P2_TouchStatus   = 0;
        pData->P2_TouchEventID  = 0;
        pData->P2_CoordRecvdX   = 0;
        pData->P2_CoordRecvdY   = 0;
    }
#endif
}

void DetTP_GetXY(INT32 *pX, INT32 *pY)
{
    memset(pTPRecvdData, 0, sizeof(TP_RECV_DATA));
    DetTP_GetData(pTPRecvdData);
    if (pTPRecvdData->P1_TouchEventID == 1)
    {
        *pX = pTPRecvdData->P1_CoordRecvdX;
        *pY = pTPRecvdData->P1_CoordRecvdY;
    }
    else if (pTPRecvdData->P2_TouchEventID == 1)
    {
        *pX = pTPRecvdData->P2_CoordRecvdX;
        *pY = pTPRecvdData->P2_CoordRecvdY;
    }
    else
    {
        *pX = -1;
        *pY = -1;
    }
}

static void TP_Reset(void)
{
    gpio_clearPin(GPIO_TP_RST);
    Delay_DelayMs(200);
    gpio_setPin(GPIO_TP_RST);
}

void DetTP_Init(void)
{
    TP_INFO TPInfo;

    TPInfo.I2C_PinMux = I2C_PINMUX_1ST;
    TPInfo.I2C_BusClock = I2C_BUS_CLOCK_400KHZ;
    TPInfo.I2C_Slave_WAddr = 0x02;
    TPInfo.I2C_Slave_RAddr = 0x03;

    TP_Reset();

    if (TP_I2C_Init(TPInfo) == TRUE)
    {
    }
}

BOOL DetTP_IsPenDown(void)
{
    UINT32 uiTPCurLvl;

    uiTPCurLvl = gpio_getPin(GPIO_TP_INT);
    return (uiTPCurLvl == FALSE)?TRUE:FALSE;
}

#endif
