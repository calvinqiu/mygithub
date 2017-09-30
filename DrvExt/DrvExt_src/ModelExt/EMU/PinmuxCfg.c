#include "PinmuxCfg.h"
#include "top.h"
#include "../../../../Project/EmuKit/SrcCode/Emulation/emu_auto.h"

const PIN_GROUP_CONFIG vTopConfig[] =
{
    // Module name              pinmux config

    //
    // Storage configuration
    //
    {PIN_FUNC_SDIO,             PIN_SDIO_CFG_4BITS},
    {PIN_FUNC_SDIO2,            PIN_SDIO_CFG_4BITS},
    {PIN_FUNC_SDIO3,            PIN_SDIO_CFG_4BITS|PIN_SDIO_CFG_1ST_PINMUX},

#if (_ALL_IN_ONE_TYPE_ == _EMU_LINUX_)
    {PIN_FUNC_NAND,             PIN_NAND_CFG_NONE},
#else
    {PIN_FUNC_NAND,             PIN_NAND_CFG_1CS},
#endif
    //
    // Sensor configuration
    //
#if (EMU_AUTOTEST == ENABLE)
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_8BITS},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_CCIR8BITS},
    {PIN_FUNC_SENSOR3,          PIN_SENSOR3_CFG_NONE},
    {PIN_FUNC_SENSOR4,          PIN_SENSOR4_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_NONE},
#else
    {PIN_FUNC_SENSOR,           PIN_SENSOR_CFG_MIPI},
    {PIN_FUNC_SENSOR2,          PIN_SENSOR2_CFG_NONE},
    {PIN_FUNC_SENSOR3,          PIN_SENSOR3_CFG_NONE},
    {PIN_FUNC_SENSOR4,          PIN_SENSOR4_CFG_NONE},
    {PIN_FUNC_MIPI_LVDS,        PIN_MIPI_LVDS_CFG_CLK0 | PIN_MIPI_LVDS_CFG_DAT0 | PIN_MIPI_LVDS_CFG_DAT1 | PIN_MIPI_LVDS_CFG_DAT2 | PIN_MIPI_LVDS_CFG_DAT3},
#endif

    //
    // Serial interface configuration
    //
    {PIN_FUNC_I2C,              PIN_I2C_CFG_CH1},
    {PIN_FUNC_SIF,              PIN_SIF_CFG_NONE},


#if (_EMULATION_MULTI_CPU_ == ENABLE)
    {PIN_FUNC_UART,             PIN_UART_CFG_CH1 | PIN_UART_CFG_CH2 | PIN_UART_CFG_CH3 | PIN_UART_CFG_CH3_2ND},
#else

#if (_ALL_IN_ONE_TYPE_ == _EMU_LINUX_)
    {PIN_FUNC_UART,             PIN_UART_CFG_CH1 | PIN_UART_CFG_CH2},
#else
    {PIN_FUNC_UART,             PIN_UART_CFG_CH1},
#endif // all in one type
#endif

#if (_ALL_IN_ONE_TYPE_ == _EMU_LINUX_)
    {PIN_FUNC_SPI,              PIN_SPI_CFG_CH1_4BITS},
#else
    {PIN_FUNC_SPI,              PIN_SPI_CFG_NONE},
#endif
    {PIN_FUNC_REMOTE,           PIN_REMOTE_CFG_NONE},

    //
    // PWM configuration
    //
    {PIN_FUNC_PWM,              PIN_PWM_CFG_PWM1},

    //
    // AUDIO configuration
    //
    {PIN_FUNC_AUDIO,            PIN_AUDIO_CFG_NONE},

    //
    // Display configuration
    //
#if (EMU_AUTOTEST == ENABLE)
    {PIN_FUNC_LCD,              PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_YUV640},
    {PIN_FUNC_LCD2,             PINMUX_DISPMUX_SEL_LCD2 | PINMUX_LCDMODE_YUV640},
#else
    {PIN_FUNC_LCD,              PINMUX_DISPMUX_SEL_LCD | PINMUX_LCDMODE_RGB_SERIAL},
    {PIN_FUNC_LCD2,             PINMUX_DISPMUX_SEL_LCD2 | PINMUX_LCDMODE_RGB_SERIAL},
#endif
    {PIN_FUNC_TV,               PINMUX_TV_HDMI_CFG_NORMAL},
    {PIN_FUNC_HDMI,             PINMUX_TV_HDMI_CFG_NORMAL | PINMUX_HDMIMODE_1280X720P60},

    //
    // USB configuration
    //
    {PIN_FUNC_USB,              PINMUX_USB_CFG_DEVICE},
    {PIN_FUNC_USB2,             PINMUX_USB_CFG_HOST},
    {PIN_FUNC_ETH,              PIN_ETH_CFG_NONE}
};

void IO_InitPinmux(void)
{
    pinmux_init((PIN_GROUP_CONFIG *)vTopConfig);
}

