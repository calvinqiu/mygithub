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

//// ------------------------------------------------- ////
////        C12 Touch Configuration                    ////
////   Touch IC  : GT915L                              ////
////   I2C path  : I2C2, 2nd pinmux                    ////
////   I2C Speed : 400K                                ////
////   Dev Addr  : 0x28/0x29 (Set by reset sequence)   ////
//// ------------------------------------------------- ////

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
//#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#define __DBGFLT__          "[Touch]" // Touch message
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define TP_HW_MAX_X         856
#define TP_HW_MAX_Y         480
#define TP_HW_MIN_X         0
#define TP_HW_MIN_Y         0

#define TP_PANEL_X1         0
#define TP_PANEL_Y1         0
#define TP_PANEL_X2         (TP_HW_MAX_X - 1)
#define TP_PANEL_Y2         (TP_HW_MAX_Y - 1)

#define TP_PANEL_W          (TP_PANEL_X2-TP_PANEL_X1 + 1)
#define TP_PANEL_H          (TP_PANEL_Y2-TP_PANEL_Y1 + 1)

#define READ_ONE_TP_ONLY    1

#define DEVICE_ADDR_SEL     0xba

//////////////////////////////////////////////////////////////////////
//for touch panel
//Touch panel I2C info structure
typedef struct {
	I2C_SESSION     I2C_Channel;
	I2C_PINMUX      I2C_PinMux;
	I2C_BUS_CLOCK   I2C_BusClock;
	UINT32          I2C_Slave_WAddr;
	UINT32          I2C_Slave_RAddr;
} TP_INFO, *PTP_INFO;

//Touch Panel recieved data structure // 2 points touch
typedef struct {
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

static void DetTP_GetID(void);

static TP_RECV_DATA     g_TPRecvdData;
static pPT_RECV_DATA    pTPRecvdData = &g_TPRecvdData;
static TP_INFO          g_TpInfo;

/*
    0:  Use AUTO Mode with Polling
    1:  Use AUTO Mode with Interrupt (N/A)
    2:  Use AUTO Mode with FIFO Threshold (N/A)
*/
#define GSTP_USED_MODE      0

UINT8 gInitTable[186] = {
	// Table from Liu Jin Hui
	0x00,                       // 0x8047        :
	0xE0, 0x01,                 // 0x8048~0x8049 : Max value of output X (LO_HI)    ==> Set to 480
	0x58, 0x03,                 // 0x804A~0x804B : Max value of output Y (LO_HI)    ==> Set to 856              * Changed from 800 to 856
	0x01,                       // 0x804C        : Max touch count                  ==> Set to 1                * Changed from 5 to 1
	0x36,                       // 0x804D        : Module switch1                   ==> INT(Level LO output)    * Changed from 0x34 to 0x36
	0x00,                       // 0x804E        : Module switch2                   ==> No touch key
	0x01,                       // 0x804F        : Debounce
	0x08,                       // 0x8050        :
	0x28,                       // 0x8051        :
	0x06,                       // 0x8052        :
	0x5A,                       // 0x8053        :                                                              * Changed from 0x64 to 0x5A
	0x3C,                       // 0x8054        :                                                              * Changed from 0x46 to 0x3C
	0x03,                       // 0x8055        : Time to enter low power          ==> Set to 3 sec
	0x05,                       // 0x8056        : Refresh rate (5+N)               ==> Set to 5 (10 ms)        * Changed from 0x07 to 0x05
	0x00,                       // 0x8057        :
	0x00,                       // 0x8058        :
	0xFF,                       // 0x8059        :
	0x7F,                       // 0x805A        :
	0x11, 0x00,                 // 0x805B~0x805C : Border setting
	0x06,                       // 0x805D        :
	0x17, 0x18, 0x1E, 0x14,     // 0x805E~0x8061 : Stretch setting
	0x8B,                       // 0x8062        :
	0x2B,                       // 0x8063        :
	0x0C,                       // 0x8064        :
	0x3C,                       // 0x8065        :
	0x3E,                       // 0x8066        :
	0x0F, 0x0A,                 // 0x8067~0x8068 :
	0x0F, 0x1E,                 // 0x8069~0x806A :
	0x00,                       // 0x806B        :                                                              * Changed from 0x01 to 0x00
	0x03,                       // 0x806C        :
	0x22,                       // 0x806D        :
	0x11,                       // 0x806E        :
	0x03,                       // 0x806F        :
	0x00,                       // 0x8070        :
	0x00,                       // 0x8071        :
	0x00,                       // 0x8072        :
	0x00,                       // 0x8073        :
	0x00,                       // 0x8074        :
	0x00,                       // 0x8075        :
	0x20,                       // 0x8076        :
	0x15,                       // 0x8077        :
	0x00,                       // 0x8078        :
	0x7B,                       // 0x8079        :
	0x1E,                       // 0x807A        :
	0x50,                       // 0x807B        :
	0x94,                       // 0x807C        :
	0xC5,                       // 0x807D        :
	0x02,                       // 0x807E        :
	0x07,                       // 0x807F        :
	0x00, 0x14,                 // 0x8080~0x8081 : Reserved
	0x04,                       // 0x8082        :
	0xC5,                       // 0x8083        :
	0x21,                       // 0x8084        :
	0x00,                       // 0x8085        :
	0xA9,                       // 0x8086        :
	0x28,                       // 0x8087        :
	0x00,                       // 0x8088        :
	0x92,                       // 0x8089        :
	0x31,                       // 0x808A        :
	0x00,                       // 0x808B        :
	0x81,                       // 0x808C        :
	0x3B,                       // 0x808D        :
	0x00,                       // 0x808E        :
	0x73,                       // 0x808F        :
	0x48,                       // 0x8090        :
	0x00,                       // 0x8091        : Reserved
	0x73,                       // 0x8092        :
	0x00,                       // 0x8093        :
	0x00,                       // 0x8094        :
	0x00,                       // 0x8095        :
	0x00,                       // 0x8096        :
	0xF7,                       // 0x8097        :
	0x4A,                       // 0x8098        :
	0x3A,                       // 0x8099        :
	0xEE,                       // 0x809A        :
	0xEE,                       // 0x809B        :
	0x27,                       // 0x809C        :
	0x00, 0x00,                 // 0x809D~0x808E : Reserved
	0x00,                       // 0x809F        :
	0x00,                       // 0x80A0        :
	0x00,                       // 0x80A1        :
	0x00,                       // 0x80A2        :
	0x00,                       // 0x80A3        :
	0x00,                       // 0x80A4        :
	0x00,                       // 0x80A5        :
	0x00,                       // 0x80A6        :
	0x00,                       // 0x80A7        :
	0x00,                       // 0x80A8        :
	0x00,                       // 0x80A9        :
	0x04,                       // 0x80AA        :
	0x10,                       // 0x80AB        :
	0x02,                       // 0x80AC        :
	0xE0,                       // 0x80AD        :
	0x0F,                       // 0x80AE        :
	0x00,                       // 0x80AF        :
	0x00,                       // 0x80B0        :
	0x0A,                       // 0x80B1        :
	0x19,                       // 0x80B2        :
	0x10, 0x00, 0x3C, 0x00,     // 0x80B3~0x80B6 :
	0x02, 0x04, 0x06, 0x08,     // 0x80B7~       : ITO Sensor channel number
	0x0A, 0x0C, 0x0E, 0x10,     //       ~       :
	0x12, 0x14, 0x16, 0x18,     //       ~       :
	0xFF, 0xFF,                 //       ~0x80C4 :
	0x00, 0x00, 0x00, 0x00,     // 0x80C5~       : Reserved
	0x00, 0x00, 0x00, 0x00,     //       ~       :
	0x00, 0x00, 0x00, 0x00,     //       ~       :
	0x00, 0x00, 0x00, 0x00,     //       ~0x80D4 :
	0x26, 0x24, 0x22, 0x21,     // 0x80D5~       : ITO Driver channel number
	0x20, 0x1F, 0x1E, 0x1D,     //       ~       :
	0x1C, 0x18, 0x16, 0x00,     //       ~       :
	0x02, 0x04, 0x06, 0x08,     //       ~       :
	0x0A, 0x0C, 0x0F, 0x10,     //       ~       :
	0x12, 0x13, 0xFF, 0xFF,     //       ~       :
	0xFF, 0xFF,                 //       ~0x80EE :
	0x00, 0x00, 0x00, 0x00,     // 0x80EF~       : Reserved
	0x00, 0x00, 0x00, 0x00,     //       ~       :
	0x00, 0x00, 0x00, 0x00,     //       ~       :
	0x00, 0x00, 0x00, 0x00,     //       ~0x80FE :
	0x00,                       // 0x80FF        : Checksum (will be updated by calling DetTP_CalCheckSum())
	0x01,                       // 0x8100        :
};

static void DetTP_CalCheckSum(void)
{
	UINT32 i = 0;

	gInitTable[184] = 0;
	for (i = 0; i < 184; i ++) {
		gInitTable[184] += gInitTable[i];
	}

	gInitTable[184] = 0 - gInitTable[184];
	DBG_IND("[Touch]Initial table checkSum = %x\r\n", gInitTable[184]);
}

static UINT32 DetTP_LoadInitTable(void)
{
	I2C_DATA    I2cData = {0};
	I2C_BYTE    I2cByte[4] = {0};
	I2C_STS     ret = I2C_STS_OK;
	UINT32      StartSubAddr = 0x8047, CurrSubAddr = 0, Cnt = 0;


	if (i2c2_lock(g_TpInfo.I2C_Channel) != E_OK) {
		DBG_ERR("[Touch]I2C lock fail!\r\n");
		return 0;
	}

	for (Cnt = 0; Cnt < 186; Cnt ++) {
		CurrSubAddr = StartSubAddr + Cnt;
		I2cData.VersionInfo = DRV_VER_96660;
		I2cData.ByteCount = I2C_BYTE_CNT_4;
		I2cData.pByte = I2cByte;
		I2cByte[0].uiValue = g_TpInfo.I2C_Slave_WAddr;
		I2cByte[0].Param = I2C_BYTE_PARAM_START;
		I2cByte[1].uiValue = ((CurrSubAddr & 0x0000ff00) >> 8);
		I2cByte[1].Param = I2C_BYTE_PARAM_NONE;
		I2cByte[2].uiValue = (CurrSubAddr & 0x000000ff);
		I2cByte[2].Param = I2C_BYTE_PARAM_NONE;
		I2cByte[3].uiValue = gInitTable[Cnt];
		I2cByte[3].Param = I2C_BYTE_PARAM_STOP;
		ret = i2c2_transmit(&I2cData);
		if (ret != I2C_STS_OK) {
			//DBG_ERR("[Touch](W)i2c ret = %02x!!\r\n", ret);
			goto unlock_bus;
		}
		DBG_IND("[Touch]W:%04x=%02x\r\n", CurrSubAddr, I2cByte[3].uiValue);
	}

	// Read back to verify
#if 0
	for (Cnt = 0; Cnt < 186; Cnt ++) {
		CurrSubAddr = StartSubAddr + Cnt;
		I2cData.VersionInfo = DRV_VER_96660;
		I2cData.ByteCount = I2C_BYTE_CNT_4;
		I2cData.pByte = I2cByte;
		I2cByte[0].uiValue = g_TpInfo.I2C_Slave_WAddr;
		I2cByte[0].Param = I2C_BYTE_PARAM_START;
		I2cByte[1].uiValue = ((CurrSubAddr & 0x0000ff00) >> 8);
		I2cByte[1].Param = I2C_BYTE_PARAM_NONE;
		I2cByte[2].uiValue = (CurrSubAddr & 0x000000ff);
		I2cByte[2].Param = I2C_BYTE_PARAM_NONE;
		I2cByte[3].uiValue = g_TpInfo.I2C_Slave_RAddr;
		I2cByte[3].Param = I2C_BYTE_PARAM_START;
		ret = i2c2_transmit(&I2cData);
		if (ret != I2C_STS_OK) {
			//DBG_ERR("[Touch](W)i2c ret = %02x!!\r\n", ret);
			goto unlock_bus;
		}

		I2cData.VersionInfo = DRV_VER_96660;
		I2cData.ByteCount = I2C_BYTE_CNT_1;
		I2cData.pByte = I2cByte;
		I2cByte[0].Param = I2C_BYTE_PARAM_NONE | I2C_BYTE_PARAM_NACK | I2C_BYTE_PARAM_STOP;
		ret = i2c2_receive(&I2cData);
		if (ret != I2C_STS_OK) {
			//DBG_ERR("[Touch](R)i2c ret = %02x!!\r\n", ret);
			goto unlock_bus;
		}
		DBG_IND("[Touch]R:%04x=%02x\r\n", CurrSubAddr, I2cByte[0].uiValue);
	}
#endif

unlock_bus:
	if (i2c2_unlock(g_TpInfo.I2C_Channel) != E_OK) {
		DBG_ERR("[Touch]I2C unlock fail!\r\n");
		return  0;
	}

	return 1;
}

static BOOL TP_I2C_Init(TP_INFO TouchInfo)
{
	UINT erReturn;

	g_TpInfo.I2C_PinMux = TouchInfo.I2C_PinMux;
	g_TpInfo.I2C_BusClock = TouchInfo.I2C_BusClock;
	g_TpInfo.I2C_Slave_WAddr = TouchInfo.I2C_Slave_WAddr;
	g_TpInfo.I2C_Slave_RAddr = TouchInfo.I2C_Slave_RAddr;

	if (g_TpInfo.I2C_PinMux > I2C_PINMUX_2ND) {
		DBG_ERR("[Touch]I2C pin mux err!\r\n");
		return FALSE;
	}

	if (g_TpInfo.I2C_BusClock > I2C_BUS_CLOCK_1MHZ) {
		DBG_ERR("[Touch]I2C bus clock err!\r\n");
		return FALSE;
	}

	erReturn = i2c2_open(&(g_TpInfo.I2C_Channel));
	if (erReturn != I2C_STS_OK) {
		DBG_ERR("[Touch]Open I2C driver err!\r\n");
		return FALSE;
	}

	i2c2_setConfig(g_TpInfo.I2C_Channel, I2C_CONFIG_ID_PINMUX, g_TpInfo.I2C_PinMux);
	i2c2_setConfig(g_TpInfo.I2C_Channel, I2C_CONFIG_ID_BUSCLOCK, g_TpInfo.I2C_BusClock);
	i2c2_setConfig(g_TpInfo.I2C_Channel, I2C_CONFIG_ID_SCLTIMEOUT_CHECK, DISABLE);        // Temporary disable this instruction due to i2c2 doesn't have pull-up register at this moment.

	// Get device ID, should be ascii "915L", only for debug check
	DetTP_GetID();

	// Calculate GT915L initial table check sum
	DetTP_CalCheckSum();

	// Download initial table
	DetTP_LoadInitTable();

	return TRUE;
}

static BOOL DetTP_I2C_ClearStatus(void)
{
	I2C_DATA    I2cData;
	I2C_BYTE    I2cByte[4];
	I2C_STS     ret;

	if (i2c2_lock(g_TpInfo.I2C_Channel) != E_OK) {
		DBG_ERR("[Touch]I2C lock fail!\r\n");
		return FALSE;
	}

	I2cData.VersionInfo = DRV_VER_96660;
	I2cData.ByteCount = I2C_BYTE_CNT_4;
	I2cData.pByte = I2cByte;
	I2cByte[0].uiValue = g_TpInfo.I2C_Slave_WAddr;
	I2cByte[0].Param = I2C_BYTE_PARAM_START;
	I2cByte[1].uiValue = 0x81;
	I2cByte[1].Param = I2C_BYTE_PARAM_NONE;
	I2cByte[2].uiValue = 0x4E;
	I2cByte[2].Param = I2C_BYTE_PARAM_NONE;
	I2cByte[3].uiValue = 0x00;
	I2cByte[3].Param = I2C_BYTE_PARAM_STOP;
	ret = i2c2_transmit(&I2cData);
	if (ret != I2C_STS_OK) {
		//DBG_ERR("[Touch](W)i2c ret = %02x!!\r\n", ret);
		goto unlock_bus;
	}

unlock_bus:
	if (i2c2_unlock(g_TpInfo.I2C_Channel) != E_OK) {
		DBG_ERR("[Touch]I2C unlock fail!\r\n");
		return  FALSE;
	}

	return TRUE;
}

static UINT32 DetTP_I2C_BurstRead(UINT32 uiAddr, UINT32 *uiValue, I2C_BYTE_CNT Cnt)
{
	I2C_DATA    I2cData;
	I2C_BYTE    I2cByte[4];
	I2C_STS     ret;
	UINT32      i;
	UINT32      RemainBytes;

	if (Cnt > I2C_BYTE_CNT_8) {
		DBG_ERR("[Touch]Read Cnt should under 8!\r\n");
		return 0;
	}

	if (i2c2_lock(g_TpInfo.I2C_Channel) != E_OK) {
		DBG_ERR("[Touch]I2C lock fail!\r\n");
		return 0;
	}

	I2cData.VersionInfo = DRV_VER_96660;
	I2cData.ByteCount = I2C_BYTE_CNT_4;
	I2cData.pByte = I2cByte;
	I2cByte[0].uiValue = g_TpInfo.I2C_Slave_WAddr;
	I2cByte[0].Param = I2C_BYTE_PARAM_START;
	I2cByte[1].uiValue = (uiAddr & 0x0000ff00) >> 8;
	I2cByte[1].Param = I2C_BYTE_PARAM_NONE;
	I2cByte[2].uiValue = (uiAddr & 0x000000ff);
	I2cByte[2].Param = I2C_BYTE_PARAM_NONE;
	I2cByte[3].uiValue = g_TpInfo.I2C_Slave_RAddr;
	I2cByte[3].Param = I2C_BYTE_PARAM_START;
	ret = i2c2_transmit(&I2cData);
	if (ret != I2C_STS_OK) {
		//DBG_ERR("[Touch](W)i2c ret = %02x!!\r\n", ret);
		goto unlock_bus;
	}

	do {
		RemainBytes = (Cnt > I2C_BYTE_CNT_4) ? (Cnt - I2C_BYTE_CNT_4) : 0;
		Cnt = (Cnt > I2C_BYTE_CNT_4) ? I2C_BYTE_CNT_4 : Cnt;
		I2cData.VersionInfo = DRV_VER_96660;
		I2cData.ByteCount = Cnt;
		I2cData.pByte = I2cByte;
		I2cByte[0].Param = I2C_BYTE_PARAM_NONE | I2C_BYTE_PARAM_ACK;
		I2cByte[1].Param = I2C_BYTE_PARAM_NONE | I2C_BYTE_PARAM_ACK;
		I2cByte[2].Param = I2C_BYTE_PARAM_NONE | I2C_BYTE_PARAM_ACK;
		I2cByte[3].Param = I2C_BYTE_PARAM_NONE | I2C_BYTE_PARAM_ACK;
		if (RemainBytes == 0) {
			I2cByte[Cnt - 1].Param = I2C_BYTE_PARAM_NONE | I2C_BYTE_PARAM_NACK | I2C_BYTE_PARAM_STOP;
		}

		ret = i2c2_receive(&I2cData);
		if (ret != I2C_STS_OK) {
			//DBG_ERR("[Touch](R)i2c ret = %02x!!\r\n", ret);
			goto unlock_bus;
		}
		for (i = 0; i < Cnt; i ++) {
			*uiValue = I2cByte[i].uiValue;
			uiValue++;                              // Move to next data address
		}
		Cnt = RemainBytes;
	} while (RemainBytes != 0);

unlock_bus:
	if (i2c2_unlock(g_TpInfo.I2C_Channel) != E_OK) {
		DBG_ERR("[Touch]I2C unlock fail!\r\n");
		return  0;
	}
	return 1;
}

static void DetTP_Convert2RealXY(UINT32 *pX, UINT32 *pY)
{
	INT32 tempX = 0, tempY = 0;

	// XY should exchanged for this panel!!!
	tempY = *pX;
	tempX = *pY;

	if (tempX > TP_HW_MAX_X) {
		tempX = TP_HW_MAX_X;
	} else if (tempX < TP_HW_MIN_X) {
		tempX = TP_HW_MIN_X;
	}

	if (tempY > TP_HW_MAX_Y) {
		tempY = TP_HW_MAX_Y;
	} else if (tempY < TP_HW_MIN_Y) {
		tempY = TP_HW_MIN_Y;
	}

	// Current X direction is ">", so don't need to change the direction
	*pX = tempX;
	// Current Y direction is "^", so need to change the direction
	*pY = TP_PANEL_H - tempY - 1;

#if 1   // Note: Just for NVT TouchUI test. Remember to remove this code!!!!
	*pX = *pX * 320 / TP_HW_MAX_X;
	*pY = *pY * 240 / TP_HW_MAX_Y;
#endif

	DBG_IND("[Touch]Convert (%d,%d) to (%d,%d)\r\n", tempX, tempY, *pX, *pY);
}

static void DetTP_GetID(void)
{
	UINT32 recv_data[4] = {0};

	DetTP_I2C_BurstRead(0x8140, recv_data, I2C_BYTE_CNT_4);

	DBG_DUMP("[Touch]Get Touch IC ID =%c%c%c%c\r\n", recv_data[0], recv_data[1], recv_data[2], recv_data[3]);
}

static void DetTP_GetData(pPT_RECV_DATA pData)
{
#if (READ_ONE_TP_ONLY == 1)
	UINT32 recv_data[6] = {0};

	DetTP_I2C_BurstRead(0x814E, recv_data, I2C_BYTE_CNT_6);
	pData->P1_TouchStatus   = recv_data[0];
	pData->P1_TouchEventID  = recv_data[1];
	if (pData->P1_TouchStatus & 0x80) {
		DetTP_I2C_ClearStatus();
	}
	pData->P1_CoordRecvdX   = recv_data[2] + recv_data[3] * 256;
	pData->P1_CoordRecvdY   = recv_data[4] + recv_data[5] * 256;
	DetTP_Convert2RealXY(&(pData->P1_CoordRecvdX), &(pData->P1_CoordRecvdY));

	pData->P2_TouchStatus   = 0;
	pData->P2_TouchEventID  = 0;
	pData->P2_CoordRecvdX   = 0;
	pData->P2_CoordRecvdY   = 0;
#else
#if 0
	// Those code are not worked
	UINT32 recv_data[8];

	DetTP_I2C_BurstRead(0x814E, &(recv_data[0]), I2C_BYTE_CNT_4);
	DetTP_I2C_BurstRead(0x8156, &(recv_data[6]), I2C_BYTE_CNT_4);
	DetTP_I2C_ClearStatus();

	pData->P1_TouchStatus   = recv_data[0];
	pData->P1_TouchEventID  = recv_data[1];
	pData->P1_CoordRecvdX   = recv_data[2] + recv_data[3] * 256;
	pData->P1_CoordRecvdY   = recv_data[4] + recv_data[5] * 256;
	DetTP_Convert2RealXY(&(pData->P1_CoordRecvdX), &(pData->P1_CoordRecvdY));

	if (recv_data[7] != 0xff) {     // Not sure whether to recv_data[7] to check TP2
		pData->P2_TouchStatus   = recv_data[0];                 // Use TP1 status
		pData->P2_TouchEventID  = recv_data[7];
		pData->P2_CoordRecvdX   = recv_data[8] + recv_data[9] * 256;
		pData->P2_CoordRecvdY   = recv_data[10] + recv_data[11] * 256;
		DetTP_Convert2RealXY(&(pData->P2_CoordRecvdX), &(pData->P2_CoordRecvdY));
	} else {
		pData->P2_TouchStatus   = 0;
		pData->P2_TouchEventID  = 0;
		pData->P2_CoordRecvdX   = 0;
		pData->P2_CoordRecvdY   = 0;
	}
#endif
#endif
}

void DetTP_GetXY(INT32 *pX, INT32 *pY)
{
	memset(pTPRecvdData, 0, sizeof(TP_RECV_DATA));
	DetTP_GetData(pTPRecvdData);
	//if (pTPRecvdData->P1_TouchStatus & 0x80)      // Not check status, KCHong, 20160912
	{
		*pX = pTPRecvdData->P1_CoordRecvdX;
		*pY = pTPRecvdData->P1_CoordRecvdY;
	}
#if 0       // Not support 2nd touch point now
	else if (pTPRecvdData->P2_TouchEventID == 1) {
		*pX = pTPRecvdData->P2_CoordRecvdX;
		*pY = pTPRecvdData->P2_CoordRecvdY;
	} else {
		*pX = -1;
		*pY = -1;
	}
#endif
}

static void TP_Reset(void)
{
	// If INT = HI when RST release, device addr = 0x28/0x29  (current setting)
	// if INT = LO when RST release, device addr = 0xba/0xbb
	gpio_setDir(GPIO_TP_RST, GPIO_DIR_OUTPUT);
	gpio_setDir(GPIO_TP_INT, GPIO_DIR_OUTPUT);
	gpio_clearPin(GPIO_TP_RST);
	gpio_clearPin(GPIO_TP_INT);
	Delay_DelayMs(50);
#if (DEVICE_ADDR_SEL == 0x28)
	gpio_setPin(GPIO_TP_INT);
#endif
	Delay_DelayMs(10);
	gpio_setPin(GPIO_TP_RST);
	Delay_DelayMs(10);
	gpio_setDir(GPIO_TP_INT, GPIO_DIR_INPUT);
	Delay_DelayMs(100);
}

void DetTP_Init(void)
{
	TP_INFO TPInfo;

	TPInfo.I2C_PinMux = I2C_PINMUX_2ND;
	TPInfo.I2C_BusClock = I2C_BUS_CLOCK_400KHZ;
#if (DEVICE_ADDR_SEL == 0x28)
	TPInfo.I2C_Slave_WAddr = 0x28;
	TPInfo.I2C_Slave_RAddr = 0x29;
#else
	TPInfo.I2C_Slave_WAddr = 0xba;
	TPInfo.I2C_Slave_RAddr = 0xbb;
#endif

	TP_Reset();
	DBG_DUMP("[Touch]Select Touch IC device ID = 0x%x/0x%x, clock = %dHz\r\n", TPInfo.I2C_Slave_WAddr, TPInfo.I2C_Slave_RAddr, TPInfo.I2C_BusClock);

	if (TP_I2C_Init(TPInfo) == FALSE) {
		DBG_ERR("[Touch]Init touch IC fail!\r\n");
	}
}

BOOL DetTP_IsPenDown(void)
{
	UINT32 uiTPCurLvl;

	uiTPCurLvl = gpio_getPin(GPIO_TP_INT);
	return (uiTPCurLvl == FALSE) ? TRUE : FALSE;
}

#endif
