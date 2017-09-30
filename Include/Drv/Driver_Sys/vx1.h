/**
    Video-by-One(Vx1) HS Controller global header

    Video-by-One(Vx1) HS and Sublink Controller global header

    @file       vx1.h
    @ingroup    mIDrvIO_Vx1
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2015.  All rights reserved.
*/

#ifndef _VX1_H
#define _VX1_H

#include "Type.h"

/**
    @addtogroup mIDrvIO_Vx1
*/
//@{


/**
    Vx1 controller general configuration Selection

    This is used in vx1_setConfig()/vx1_getConfig() to specify which of the function is selected to assign new configuration.
*/
typedef enum
{
    VX1_CONFIG_ID_RXMODE,           ///< Select Vx1 receive mode. Please use VX1_RXMODE as input parameter.
    VX1_CONFIG_ID_PIXSWAP,          ///< If RXMODE is VX1_RXMODE_MUX24_TO_2X12/VX1_RXMODE_MUX32_TO_2X12,
                                    ///< this feature can set the pixel order swap.
    VX1_CONFIG_ID_BITSWAP,          ///< Set Bit-wise swap for each pixel data.
    VX1_CONFIG_ID_HDMUX,            ///< Select HD signal to SIE is from HSYNC or DE from Vx1 PHY.
                                    ///< Please use VX1_HDMUX as input parameter.
    VX1_CONFIG_ID_SUBLINK_AIN,      ///< Select Sublink master's I2C address. Please use VX1_SUBLINK_ADDR as input parameter.
    VX1_CONFIG_ID_PHY_HALFRATE_MODE,///< Enable/Disable Vx1 mainlink phy half-rate mode. Default is Enabled.

    VX1_CONFIG_ID_LOCKSTS_ALN,      ///< This can only be used in vx1_getConfig() to get the LOCK-Status of Vx1 Alignment.
    VX1_CONFIG_ID_LOCKSTS_CDR,      ///< This can only be used in vx1_getConfig() to get the LOCK-Status of Vx1 CDR.
    VX1_CONFIG_ID_LOCKSTS_CALLBACK, ///< IST Callback function when Vx1 Locked or UnLocked. Please use DRV_CB as function prototype.
                                    ///< When Locked, the notify event would be VX1_INTERRUPT_LOCKED. When UnLocked, the notify event would be VX1_INTERRUPT_UNLOCKED.
    VX1_CONFIG_ID_COLOR_MODE,       ///< This can only be used in vx1_getConfig() to get the PHY's 3/4/5byte mode. 0x1 is 3byte mode. 0x2 is 4byte mode.0x3 is 5byte mode.
    VX1_CONFIG_ID_FIFO_EN,          ///< This can only be used in vx1_getConfig() to get the Digital FIFO is enable or disable.
    VX1_CONFIG_ID_SPEED,            ///< This can only be used in vx1_getConfig() to get the PHY's operation speed in Mbps.

    ENUM_DUMMY4WORD(VX1_CONFIG_ID)
}VX1_CONFIG_ID;


/**
    Vx1 interrupt ID

    This is used in vx1_waitInterrupt() to specify which interrupt is waiting for.
*/
typedef enum
{
    VX1_INTERRUPT_LOCKED    = 0x004,    ///< Vx1 ALN Locked.
    VX1_INTERRUPT_UNLOCKED  = 0x008,    ///< Vx1 ALN UnLocked.
    VX1_INTERRUPT_SUBLINK   = 0x020,    ///< Vx1 Sublink.
    VX1_INTERRUPT_VD        = 0x100,    ///< Vx1 Vsync.

    ENUM_DUMMY4WORD(VX1_INTERRUPT)
}VX1_INTERRUPT;


/**
    Vx1 Receive Mode Selection

    This is used in vx1_setConfig(VX1_CONFIG_ID_RXMODE) to assign the Vx1 Rx Mode.
*/
typedef enum
{
    VX1_RXMODE_MUX11_0,         ///< Select Vx1 PHY-D[11:0]  as pixel data to SIE.
    VX1_RXMODE_MUX23_12,        ///< Select Vx1 PHY-D[23:12] as pixel data to SIE.
    VX1_RXMODE_MUX24_TO_2X12,   ///< Select Vx1 PHY-D[11:0] as first pixel and PHY-D[23:12] as second pixel data to SIE.
    VX1_RXMODE_MUX24_TO_16,     ///< Select Vx1 PHY-D[23:16][11:4] as 16bits pixel data to SIE.
    VX1_RXMODE_MUX32_TO_2X12,   ///< Select Vx1 PHY-D[11:0] as first pixel and PHY-D[27:16] as second pixel data to SIE.
    VX1_RXMODE_MUX24_TO_16V2,   ///< Select Vx1 PHY-D[23:16][7:0] as 16bits pixel data to SIE.

    ENUM_DUMMY4WORD(VX1_RXMODE)
}VX1_RXMODE;

/**
    Vx1 HD Mux

    This selects HD(Hsync) signal to SIE is from Vx1-PHY's DE or Hsync signal.
*/
typedef enum
{
    VX1_HDMUX_DE,               ///< HD to SIE is from Vx1 PHY's DE signal.
    VX1_HDMUX_HSYNC,            ///< HD to SIE is from Vx1 PHY's Hsync signal.

    ENUM_DUMMY4WORD(VX1_HDMUX)
}VX1_HDMUX;

/**
    Vx1 Sublink Master's I2C Address select

    This is used to change the sublink i2c device address(7bits).
*/
typedef enum
{
    VX1_SUBLINK_ADDR0X0B,       ///< Set sublink i2c address to 0x0B.
    VX1_SUBLINK_ADDR0X34,       ///< Set sublink i2c address to 0x34.
    VX1_SUBLINK_ADDR0X77,       ///< Set sublink i2c address to 0x77.
    VX1_SUBLINK_ADDR0X65,       ///< Set sublink i2c address to 0x65.

    ENUM_DUMMY4WORD(VX1_SUBLINK_ADDR)
}VX1_SUBLINK_ADDR;

/**
    Vx1 sublink general configuration Selection

    This is used in vx1_setSublinkConfig()/vx1_getSublinkConfig() to specify which of the function is selected to assign new configuration.
*/
typedef enum
{
    VX1SBL_CONFIG_ID_REGADDR_BYTECOUNT, ///< Set i2c device (Sensor) 's register address is 1 to 4 bytes.
    VX1SBL_CONFIG_ID_DATA_BYTECOUNT,    ///< Set i2c device (Sensor) 's register data is 1 to 4 bytes.
    VX1SBL_CONFIG_ID_I2C_SPEED,         ///< Set i2c device (Sensor) 's I2C Bus speed as 50KHz/100KHz/200KHz/400KHz.
    VX1SBL_CONFIG_ID_EMULATION_0,       // novatek internal usage.
    VX1SBL_CONFIG_ID_NACK_CHECK,        ///< Enable/Disable i2c nack check in vx1_writeI2cDevice/vx1_readI2cDevice. Default is enabled.

	VX1SBL_CONFIG_ID_TXMODULE,			///< Set the vx1 transmitter module. Please use VX1_TXMODULE as input parameter.
	VX1SBL_CONFIG_ID_TXMIPI_LANE_NUMBER,///< Set the THCV241 transmitter's mipi input lane number. Only 1/2/4 are valid.


    ENUM_DUMMY4WORD(VX1SBL_CONFIG_ID)
}VX1SBL_CONFIG_ID;

/**
    Vx1 transmitter Module select

    Supported Vx1 transmitter Module select. Currently we support THCV231 and THCV235.
    These select options are used in vx1_initTransmitter()/vx1_cfgTransmitterGPIO() to initialize
    transmitter operation mode and configure the transmitter's GPIO function.
*/
typedef enum
{
    VX1_TXMODULE_THCV235,       ///< Transmitter module is THCV235.
                                ///< The THCV235 only supports the Vx1 4-Bytes Mode.(Inlcude MUX function)
    VX1_TXMODULE_THCV231,       ///< Transmitter module is THCV235.
                                ///< The THCV231 supports the Vx1 4-Bytes and 3-Bytes Mode.(Inlcude MUX function)
	VX1_TXMODULE_THCV241,		///< Transmitter module is THCV241.
								///< The THCV241 supports MIPI sensor and sensor HDR mode.

    ENUM_DUMMY4WORD(VX1_TXMODULE)
}VX1_TXMODULE;

/**
    Vx1 Transmitter operation mode select

    These options are used in vx1_initTransmitter() to initialize the operation mode.
    Different operation mode maps the transmiiter to different operation bus frerquency.
*/
typedef enum
{
    VX1_TXINIT_3BYTEMODE,       ///< 3-Bytes Mode.
                                ///< The Bus frequency would be Sensor (pixel-clock x 30)Hz.
    VX1_TXINIT_3BYTEMODE_MUX,   ///< 3-Bytes Mode with MUX function enabled.
                                ///< The Bus frequency would be Sensor (pixel-clock x 15)Hz.
    VX1_TXINIT_4BYTEMODE,       ///< 4-Bytes Mode.
                                ///< The Bus frequency would be Sensor (pixel-clock x 40)Hz.
    VX1_TXINIT_4BYTEMODE_MUX,   ///< 4-Bytes Mode with MUX function enabled.
                                ///< The Bus frequency would be Sensor (pixel-clock x 20)Hz.

	VX1_TXINIT_THCV241_YUV422_MAP1,		///< THCV241 Format for YUV422_MAP1. Please refer to THCV241 datasheets for details.
	VX1_TXINIT_THCV241_RAW12HF2_MAP2,	///< THCV241 Format for RAW12.3BYTE-MODE with MUX. Please refer to THCV241 datasheets for details.
	VX1_TXINIT_THCV241_RAW10HF2_MAP3,	///< THCV241 Format for RAW10.3BYTE-MODE with MUX. Please refer to THCV241 datasheets for details.

    ENUM_DUMMY4WORD(VX1_TXINIT)
}VX1_TXINIT;

/**
    Vx1 Sublink I2C command transfer status

    The i2c command transfer result through the Vx1 sublink to sensor board on the Vx1 transmitter.
    This is used in vx1_writeI2cDevice()/vx1_readI2cDevice() as return parameter.
*/
typedef enum
{
    VX1_I2CSTS_OK,              ///< I2C command transfer done and success. (Acked)
    VX1_I2CSTS_NACK,            ///< I2C command transfer done but got NACK.
    VX1_I2CSTS_ERROR,           ///< I2C command transfer not complete due to some errors,
                                ///< such as vx1 cable-unplug, driver no open, ...etc.

    ENUM_DUMMY4WORD(VX1_I2CSTS)
}VX1_I2CSTS;

/**
    Vx1 I2C bus speed on transmitter module

    This is used in vx1_setSublinkConfig(VX1SBL_CONFIG_ID_I2C_SPEED) to configure the I2C bus speed
    to sensor borad on the Vx1 transmitter module. Default value is 100KHz.
*/
typedef enum
{
    VX1_I2CSPEED_50KHz,         ///< I2C Bus speed is 50KHz.
    VX1_I2CSPEED_100KHz,        ///< I2C Bus speed is 100KHz. (Default)
    VX1_I2CSPEED_200KHz,        ///< I2C Bus speed is 200KHz.
    VX1_I2CSPEED_400KHz,        ///< I2C Bus speed is 400KHz.

    ENUM_DUMMY4WORD(VX1_I2CSPEED)
}VX1_I2CSPEED;

/**
    GPIO pin select on the Vx1 transmitter module

    The Vx1 transmitter board provides some GPIO pins for input/output usages.
    This is used in vx1_cfgTransmitterGPIO() to assign dedicate GPIO pin for IO.
*/
typedef enum
{
    VX1_TXGPIO_3,               ///< Select Vx1 transmitter's GPIO_3. Valid for THCV241/231/235.
    VX1_TXGPIO_4,               ///< Select Vx1 transmitter's GPIO_4. Valid for THCV231/235.

    VX1_TXGPIO_0,               ///< Select Vx1 transmitter's GPIO_0. Valid for THCV241.
    VX1_TXGPIO_1,               ///< Select Vx1 transmitter's GPIO_1. Valid for THCV241.
    VX1_TXGPIO_2,               ///< Select Vx1 transmitter's GPIO_2. Valid for THCV241.

    ENUM_DUMMY4WORD(VX1_TXGPIO)
}VX1_TXGPIO;

/**
    GPIO Input/Output Direction select on the Vx1 transmitter module

    GPIO Input/Output Direction select on the Vx1 transmitter module
    This is used in vx1_cfgTransmitterGPIO() to assign dedicate GPIO direction.
*/
typedef enum
{
    VX1_GPIODIR_INPUT,          ///< Configure GPIO to input direction.
    VX1_GPIODIR_OUTPUT,         ///< Configure GPIO to output direction.

    ENUM_DUMMY4WORD(VX1_GPIODIR)
}VX1_GPIODIR;



//
//  Extern APIs
//
extern ER               vx1_open(void);
extern ER               vx1_close(void);
extern BOOL             vx1_isOpened(void);

extern void             vx1_setEnable(BOOL bEn);
extern BOOL             vx1_getEnable(void);
extern void             vx1_setConfig(VX1_CONFIG_ID ConfigID, UINT32 uiCfgValue);
extern UINT32           vx1_getConfig(VX1_CONFIG_ID ConfigID);
extern VX1_INTERRUPT    vx1_waitInterrupt(VX1_INTERRUPT WaitedFlag);

extern void             vx1_dumpMainlink(UINT32 uiPage);
extern void             vx1_dumpSublinkMaster(void);
extern void             vx1_dumpSublinkSlave(void);

extern ER               vx1_openSublink(void);
extern ER               vx1_closeSublink(void);
extern BOOL             vx1_isSublinkOpened(void);

extern void             vx1_setSublinkConfig(VX1SBL_CONFIG_ID ConfigID, UINT32 uiCfgValue);
extern UINT32           vx1_getSublinkConfig(VX1SBL_CONFIG_ID ConfigID);

extern VX1_I2CSTS       vx1_writeI2cDevice(UINT32 uiDevAddr,UINT32 uiRegAddr, UINT32 uiValue);
extern VX1_I2CSTS       vx1_readI2cDevice(UINT32 uiDevAddr,UINT32 uiRegAddr, UINT32 *puiData);

extern ER               vx1_initTransmitter(VX1_TXMODULE TxModule, VX1_TXINIT Init);
extern BOOL             vx1_checkSublinkReady(void);
extern ER               vx1_cfgTransmitterGPIO(VX1_TXMODULE TxModule, VX1_TXGPIO TxGpio, VX1_GPIODIR GpioDir, BOOL *pbLevel);
extern void 			vx1_setThcv241PLL(UINT8 *pPLL);
//@}

#endif
