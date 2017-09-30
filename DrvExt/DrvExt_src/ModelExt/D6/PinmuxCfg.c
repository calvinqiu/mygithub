#include "PinmuxCfg.h"
#include "IOCfg.h"
#include "top.h"

const PIN_GROUP_CONFIG vTopConfig[] =
{
    // Module name              pinmux config

    //
    // Storage configuration
    //
    {PIN_FUNC_SDIO,             PIN_SDIO_CFG_4BITS},
    {PIN_FUNC_SDIO2,            PIN_SDIO_CFG_4BITS},

#if (_INTERSTORAGE_ == _INTERSTORAGE_NAND_)
    {PIN_FUNC_SDIO3,            PIN_SDIO_CFG_NONE},//PIN_SDIO_CFG_4BITS|PIN_SDIO_CFG_1ST_PINMUX},
    {PIN_FUNC_NAND,             PIN_NAND_CFG_1CS},
#elif (_INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
    {PIN_FUNC_SDIO3,            PIN_SDIO_CFG_4BITS|PIN_SDIO_CFG_2ND_PINMUX},
    {PIN_FUNC_NAND,             PIN_NAND_CFG_NONE},
#else
    {PIN_FUNC_SDIO3,            PIN_SDIO_CFG_NONE},
    {PIN_FUNC_NAND,             PIN_NAND_CFG_NONE},
#endif

    //
    // Sensor configuration
    //
#if ((_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_) && (_SENSORLIB2_ == _SENSORLIB2_CMOS_MN34110_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_))
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_LVDS|PIN_SENSOR_CFG_MCLK},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_LVDS},
    {PIN_FUNC_SENSOR3,          PIN_SENSOR3_CFG_NONE},
    {PIN_FUNC_SENSOR4,          PIN_SENSOR4_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_CLK0 | PIN_MIPI_LVDS_CFG_CLK1 | PIN_MIPI_LVDS_CFG_DAT0 |
                                PIN_MIPI_LVDS_CFG_DAT1 | PIN_MIPI_LVDS_CFG_DAT2 |
                                PIN_MIPI_LVDS_CFG_DAT3 | PIN_MIPI_LVDS_CFG_DAT4 |
                                PIN_MIPI_LVDS_CFG_DAT5 | PIN_MIPI_LVDS_CFG_DAT6 |
                                PIN_MIPI_LVDS_CFG_DAT7 | PIN_MIPI_LVDS_CFG_DAT8 |
                                PIN_MIPI_LVDS_CFG_DAT9},
#elif (((_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_) && (_SENSORLIB2_ == _SENSORLIB2_OFF_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_)) || \
       ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX124LQT_) && (_SENSORLIB2_ == _SENSORLIB2_OFF_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_)) || \
       ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX123LQT_) && (_SENSORLIB2_ == _SENSORLIB2_OFF_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_)))
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_LVDS|PIN_SENSOR_CFG_MCLK},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_NONE},
    {PIN_FUNC_SENSOR3,          PIN_SENSOR3_CFG_NONE},
    {PIN_FUNC_SENSOR4,          PIN_SENSOR4_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_CLK0 | PIN_MIPI_LVDS_CFG_DAT0 |
                                PIN_MIPI_LVDS_CFG_DAT1 | PIN_MIPI_LVDS_CFG_DAT2 |
                                PIN_MIPI_LVDS_CFG_DAT3 | PIN_MIPI_LVDS_CFG_DAT4 |
                                PIN_MIPI_LVDS_CFG_DAT5 | PIN_MIPI_LVDS_CFG_DAT6 |
                                PIN_MIPI_LVDS_CFG_DAT7 | PIN_MIPI_LVDS_CFG_DAT8 |
                                PIN_MIPI_LVDS_CFG_DAT9},
#elif (((_SENSORLIB_ == _SENSORLIB_CMOS_AR0330P_) && (_SENSORLIB2_ == _SENSORLIB2_OFF_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_)) || \
       ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX322LQJ_ || _SENSORLIB_ == _SENSORLIB_CMOS_IMX322LQJVX1_) && (_SENSORLIB2_ == _SENSORLIB2_OFF_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_)))
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_12BITS|PIN_SENSOR_CFG_MCLK},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_NONE},
    {PIN_FUNC_SENSOR3,          PIN_SENSOR3_CFG_NONE},
    {PIN_FUNC_SENSOR4,          PIN_SENSOR4_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_NONE},
#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_OV4689_)  && (_SENSORLIB2_ == _SENSORLIB2_OFF_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_) || \
       (_SENSORLIB_ == _SENSORLIB_CMOS_OV4689_) && (_SENSORLIB2_ == _SENSORLIB2_CMOS_IMX322LQJ_ || _SENSORLIB2_ == _SENSORLIB2_CMOS_IMX322LQJVX1_|| _SENSORLIB2_ == _SENSORLIB2_CMOS_OV9712VX1_|| _SENSORLIB2_ == _SENSORLIB2_CMOS_OV2710VX1_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_))
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_MIPI|PIN_SENSOR_CFG_MCLK},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_NONE},
    {PIN_FUNC_SENSOR3,          PIN_SENSOR3_CFG_NONE},
    {PIN_FUNC_SENSOR4,          PIN_SENSOR4_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_CLK0 | PIN_MIPI_LVDS_CFG_DAT0 | PIN_MIPI_LVDS_CFG_DAT1 | PIN_MIPI_LVDS_CFG_DAT2 | PIN_MIPI_LVDS_CFG_DAT3},
#elif (((_SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_)  && (_SENSORLIB2_ == _SENSORLIB2_OFF_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_)) || \
       ((_SENSORLIB_ == _SENSORLIB_CMOS_AR0230CS_)  && (_SENSORLIB2_ == _SENSORLIB2_OFF_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_)))
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_MIPI|PIN_SENSOR_CFG_MCLK},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_NONE},
    {PIN_FUNC_SENSOR3,          PIN_SENSOR3_CFG_NONE},
    {PIN_FUNC_SENSOR4,          PIN_SENSOR4_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_CLK0 | PIN_MIPI_LVDS_CFG_DAT0 | PIN_MIPI_LVDS_CFG_DAT1 | PIN_MIPI_LVDS_CFG_DAT2 | PIN_MIPI_LVDS_CFG_DAT3},
#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_)  && (_SENSORLIB2_ == _SENSORLIB2_CMOS_AR0330M_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_))
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_MIPI|PIN_SENSOR_CFG_MCLK},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_MIPI|PIN_SENSOR_CFG_MCLK2},
    {PIN_FUNC_SENSOR3,          PIN_SENSOR3_CFG_NONE},
    {PIN_FUNC_SENSOR4,          PIN_SENSOR4_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_CLK0 |PIN_MIPI_LVDS_CFG_CLK1| PIN_MIPI_LVDS_CFG_DAT0 | PIN_MIPI_LVDS_CFG_DAT1 | PIN_MIPI_LVDS_CFG_DAT2 |
    PIN_MIPI_LVDS_CFG_DAT3 | PIN_MIPI_LVDS_CFG_DAT4 | PIN_MIPI_LVDS_CFG_DAT5 | PIN_MIPI_LVDS_CFG_DAT6 | PIN_MIPI_LVDS_CFG_DAT7},
#elif ((_SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_)  && (_SENSORLIB2_ == _SENSORLIB2_CMOS_NT99141_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_))
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_MIPI|PIN_SENSOR_CFG_MCLK},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_CCIR8BITS|PIN_SENSOR_CFG_MCLK2},
    {PIN_FUNC_SENSOR3,          PIN_SENSOR3_CFG_NONE},
    {PIN_FUNC_SENSOR4,          PIN_SENSOR4_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_CLK0 | PIN_MIPI_LVDS_CFG_DAT0 | PIN_MIPI_LVDS_CFG_DAT1 | PIN_MIPI_LVDS_CFG_DAT2 | PIN_MIPI_LVDS_CFG_DAT3},
#else 
    {PIN_FUNC_SENSOR,           PIN_SENSOR3_CFG_NONE},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR3_CFG_NONE},
    {PIN_FUNC_SENSOR3,          PIN_SENSOR3_CFG_NONE},
    {PIN_FUNC_SENSOR4,          PIN_SENSOR4_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_NONE},
#endif

    //
    // Serial interface configuration
    //
    /*
    NOTE:
    PIN_I2C_CFG_CH1             (independent for MOTOR)
    PIN_I2C_CFG_CH1_2ND_PINMUX  (share to /SIF0 /SPI3)
    PIN_I2C_CFG_CH2             (independent for HDMI)
    PIN_I2C_CFG_CH2_2ND_PINMUX  (share to /Parallel_Sensor_VDHD)
    */
    {PIN_FUNC_I2C,              PIN_I2C_CFG_NONE |
#if (1) //for PowerIC
                                PIN_I2C_CFG_CH1 |
#endif

#if (_SENSORLIB2_ == _SENSORLIB2_CMOS_NT99141_)
                                PIN_I2C_CFG_CH1 |
#endif
#if ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX322LQJ_ || _SENSORLIB_ == _SENSORLIB_CMOS_IMX322LQJVX1_) || (_SENSORLIB_ == _SENSORLIB_CMOS_IMX124LQT_) || (_SENSORLIB_ == _SENSORLIB_CMOS_IMX123LQT_))
                                PIN_I2C_CFG_CH1_2ND_PINMUX |
#endif
#if ((_SENSORLIB_ == _SENSORLIB_CMOS_OV4689_) || (_SENSORLIB_ == _SENSORLIB_CMOS_AR0330M_) || (_SENSORLIB_ == _SENSORLIB_CMOS_AR0230CS_))
                                PIN_I2C_CFG_CH1_2ND_PINMUX |
#endif
#if (_HDMITYPE_ == _HDMI_ON_)
                                PIN_I2C_CFG_CH2 |
#endif
                                0},

    {PIN_FUNC_SIF,              PIN_SIF_CFG_NONE |
#if (_SENSORLIB_ == _SENSORLIB_CMOS_MN34110_)
                                PIN_SIF_CFG_CH0 |
#endif
#if ((_LCDTYPE_ == _LCDTYPE_PW27P05_ILI8961_) && (LCD_COMM_CTRL == LCD_COMM_BY_SIF))
                                PIN_SIF_CFG_CH1 |
#endif
#if ((_LCDTYPE_ == _LCDTYPE_PW35P00_HX8238D_) && (LCD_COMM_CTRL == LCD_COMM_BY_SIF))
                                PIN_SIF_CFG_CH1 |
#endif
#if (_LENSMOTORLIB_ == _LENSMOTORLIB_G2007_)
                                PIN_SIF_CFG_CH2 |
#endif
#if ((_SENSORLIB2_ == _SENSORLIB2_CMOS_MN34110_))
                                PIN_SIF_CFG_CH4 |
#endif
                                0},
    {PIN_FUNC_UART,             PIN_UART_CFG_CH1 | PIN_UART_CFG_CH3},
    {PIN_FUNC_SPI,              PIN_SPI_CFG_NONE |
#if (_INTERSTORAGE_ == _INTERSTORAGE_SPI_ || _INTERSTORAGE_ == _INTERSTORAGE_SPINAND_)
                                PIN_SPI_CFG_CH1_4BITS  |
#endif
#if (_NETWORK_ == _NETWORK_SPI_EVB_ETHERNET_)
                                PIN_SPI_CFG_CH2_2BITS  |
#endif
#if (_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
                                PIN_SPI_CFG_CH1 |
#endif
#if (_GYRO_EXT_ != _GYRO_EXT_NONE_)
                                PIN_SPI_CFG_CH4 | PIN_SPI_CFG_CH4_2BITS |
#endif

                                0},
    {PIN_FUNC_REMOTE,           PIN_REMOTE_CFG_NONE},

    {PIN_FUNC_PWM,              PIN_PWM_CFG_NONE |
#if (_LENSDRVLIB_ == _LENSDRVLIB_DCJ36_)
                                /*PIN_PWM_CFG_PWM2 | PIN_PWM_CFG_PWM3 |*/
#endif
                                0},

    //
    // AUDIO configuration
    //
#if( _AUDIOCODEC_EXT_ == _AUDIOCODEC_EXT_WM8978_)//for WM8978
    {PIN_FUNC_AUDIO,            PIN_AUDIO_CFG_I2S|PIN_AUDIO_CFG_MCLK},
#else
    {PIN_FUNC_AUDIO,            PIN_AUDIO_CFG_NONE},
#endif
    //
    // Display configuration
    //
    {PIN_FUNC_LCD,              PINMUX_DISPMUX_SEL_NONE |
#if (_LCDTYPE_ != _LCDTYPE_OFF_)
                                PINMUX_DISPMUX_SEL_LCD |
#elif (_LCDTYPE_ == _LCDTYPE_ALT_025CSLN_J2_OTA5184A_)
                                PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_RGBD320 |
#elif (_LCDTYPE_ == _LCDTYPE_TWJ50IP008_ILI9806G_)
                                PINMUX_DISPMUX_SEL_LCD | PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB888 |                                 
#elif (_LCDTYPE_ == _LCDTYPE_FD050FWV047_JD9161_)
                                PINMUX_DISPMUX_SEL_LCD | PINMUX_DSI_2_LANE_VDO_SYNC_EVENT_RGB888 |                                 

#endif
                                0},
    {PIN_FUNC_LCD2,             PINMUX_DISPMUX_SEL_NONE |
#if (_LCD2TYPE_ != _LCD2TYPE_OFF_)
                                PINMUX_DISPMUX_SEL_LCD2 |
#endif
                                0},
    {PIN_FUNC_TV,               PINMUX_TV_HDMI_CFG_NORMAL},
    {PIN_FUNC_HDMI,             PINMUX_TV_HDMI_CFG_NORMAL |
#if (_HDMITYPE_ == _HDMI_ON_)
                                PINMUX_HDMIMODE_720X480P60 |
#endif
                                0},
    //
    // USB configuration
    //
    {PIN_FUNC_USB,              PINMUX_USB_CFG_DEVICE},
    {PIN_FUNC_USB2,             PINMUX_USB_CFG_HOST},
#if (_NETWORK_ == _NETWORK_GMAC_ETHERNET_)
    {PIN_FUNC_ETH,              PIN_ETH_CFG_RMII},
#else
    {PIN_FUNC_ETH,              PIN_ETH_CFG_NONE},
#endif
};

void IO_InitPinmux(void)
{
    pinmux_init((PIN_GROUP_CONFIG *)vTopConfig);
}

