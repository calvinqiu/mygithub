/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       IOCfg.c
    @ingroup    mIPRJAPCommonIO

    @brief      IO config module
                This file is the IO config module

    @note       Nothing.

    @date       2005/12/24
*/

/** \addtogroup mIPRJAPCommonIO */
//@{

#include "Type.h"
#include "DrvExt.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "Debug.h"
#include "IOCfg.h"
#include "Utility.h"
#include "pll.h"
#include "IOInit.h"
#include "IntDirCfg.h"
#include "GyroDrv.h"
//#include "timer.h"
#include "RT5072_i2c.h"
#include "PCT2075_i2c.h"
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (USE_VIO == ENABLE)
UINT32 Virtual_IO[VIO_MAX_ID] = {0};
UINT32 vio_getpin(UINT32 id){if(id>=VIO_MAX_ID)return 0; return Virtual_IO[id];}
void vio_setpin(UINT32 id, UINT32 v){if(id>=VIO_MAX_ID)return; Virtual_IO[id] = v;}
#endif

#define GPIO_SET_NONE           0xffffff
#define GPIO_SET_OUTPUT_LOW     0x0
#define GPIO_SET_OUTPUT_HI      0x1

GPIO_INIT_OBJ uiGPIOMapInitTab[] = {
     {  GPIO_SENSOR_STANDBY,    GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,     PAD_PIN_NOT_EXIST   },
     {  GPIO_SENSOR_RESET,      GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,     PAD_PIN_NOT_EXIST   },
     {  GPIO_KEY_SHUTTER2,      GPIO_DIR_INPUT,     PAD_PULLUP,             PAD_KEY_SHUTTER2    },
     {  GPIO_AUDIO_POWER,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,     PAD_PIN_NOT_EXIST   },
#if (TV_DET_CTRL == TV_DET_BY_GPIO)
     {  GPIO_TV_PLUG,           GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_TV_PLUG            },
#endif
     {  GPIO_WIFI_SHDN,         GPIO_DIR_INPUT,     PAD_PULLUP,             PAD_WIFI_SHDN       }, //WiFi Shut Down => useless in C10 Lite, config it as input pull high
     {  GPIO_WIFI_PWREN,        GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,     PAD_PIN_NOT_EXIST   }, //WiFi Power Enable (low enable)
     {  GPIO_WIFI_CS,           GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,    PAD_PIN_NOT_EXIST   }, //WiFi CS Enable (high enable)
#if (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_GPIO)
     {  GPIO_LCD_BLG_PCTL,      GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,    PAD_PIN_NOT_EXIST   },
#endif
     //LED
     {  GPIO_RED_LED,           GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,     GPIO_SET_NONE       },
     {  GPIO_GREEN_LED,         GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,     GPIO_SET_NONE       },
    //IR LED
#if(LED_IR_CTRL == LED_IR_BY_GPIO)
     {  GPIO_IR_LED,            GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,    GPIO_SET_NONE       },
#endif
    //IR CUT
     {  GPIO_IRCUT_CTRL1,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,    GPIO_SET_NONE       },
     {  GPIO_IRCUT_CTRL2,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,    GPIO_SET_NONE       },
#if (_SENSORLIB2_ != _SENSORLIB2_OFF_)
     {  GPIO_SENSOR2_PWR,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,     PAD_PIN_NOT_EXIST   },
#endif

#if (_NETWORK_ == _NETWORK_GMAC_ETHERNET_)
     {  GPIO_ETH_GMAC,          GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,     PAD_PIN_NOT_EXIST   },
#endif
};

UINT32 totalGpioInitCount = sizeof(uiGPIOMapInitTab)/sizeof((uiGPIOMapInitTab)[0]);

/**
  Do GPIO initialization

  Initialize input/output pins, and pin status

  @param void
  @return void
*/
void IO_InitGPIO(void)
{
    UINT32 iSValue;

    DBG_IND("GPIO START\r\n");
    //--------------------------------------------------------------------
    // Open GPIO driver
    //--------------------------------------------------------------------
    gpio_open();
    for(iSValue=0 ; iSValue<totalGpioInitCount ; iSValue++)
    {
        if (uiGPIOMapInitTab[iSValue].GpioDir == GPIO_DIR_INPUT)
        {
            gpio_setDir(uiGPIOMapInitTab[iSValue].GpioPin, GPIO_DIR_INPUT);
            pad_setPullUpDown(uiGPIOMapInitTab[iSValue].PadPin, uiGPIOMapInitTab[iSValue].PadDir);
        }
        else
        {
            gpio_setDir(uiGPIOMapInitTab[iSValue].GpioPin, GPIO_DIR_OUTPUT);
            if (uiGPIOMapInitTab[iSValue].PadDir == GPIO_SET_OUTPUT_HI)
            {
                gpio_setPin(uiGPIOMapInitTab[iSValue].GpioPin);
            }
            else
            {
                gpio_clearPin(uiGPIOMapInitTab[iSValue].GpioPin);
            }
        }
    }

    //--------------------------------------------------------------------
    // Use Non-Used PWM to be Delay Timer
    //--------------------------------------------------------------------
    #if defined(PWMID_TIMER)
    Delay_setPwmChannels(PWMID_TIMER);
    #endif

    DBG_IND("GPIO END\r\n");

}

/**
  Initialize voltage detection

  Initialize voltage detection for battery and flash

  @param void
  @return void
*/

void IO_InitADC(void)
{
#if 1
    if (adc_open(ADC_CH_VOLDET_BATTERY) != E_OK)
    {
        DBG_ERR("Can't open ADC channel for battery voltage detection\r\n");
        return;
    }

    //650 Range is 250K Hz ~ 2M Hz
    adc_setConfig(ADC_CONFIG_ID_OCLK_FREQ, 250000); //250K Hz

    //battery voltage detection
    adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
    adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
    adc_setChConfig(ADC_CH_VOLDET_BATTERY, ADC_CH_CONFIG_ID_INTEN, FALSE);

    #if (ADC_KEY == ENABLE)
    adc_open(ADC_CH_VOLDET_KEY1);
    adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
    adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
    adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_INTEN, FALSE);
    adc_open(ADC_CH_VOLDET_KEY2);
    adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
    adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
    adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_INTEN, FALSE);
    #endif

    // Enable adc control logic
    adc_setEnable(TRUE);
#endif
}

/**
  Initialize Net Device

  @param void
  @return void
*/
void IO_InitNet(void)
{
#if (_NETWORK_ == _NETWORK_GMAC_ETHERNET_)
    //reset
    gpio_clearPin(GPIO_ETH_GMAC);
    Delay_DelayMs(20);
    gpio_setPin(GPIO_ETH_GMAC);
    pll_setPLLEn(PLL_ID_12,TRUE);
    pad_setDrivingSink(PAD_DS_LGPIO11, PAD_DRIVINGSINK_10MA);
#endif
}

static void IO_InitGyro(void)
{
    GYRO_CFGINFO  g_GyroCfgInfo= {0};

    g_GyroCfgInfo.AxisSelec[0] = 0; //RSC_GYRO_AXIS_X;
    g_GyroCfgInfo.AxisSelec[1] = 1; //RSC_GYRO_AXIS_Y;
    g_GyroCfgInfo.AxisSelec[2] = 2; //RSC_GYRO_AXIS_Z;
    g_GyroCfgInfo.DirSelec[0] = 0; //RSC_DIR_POS
    g_GyroCfgInfo.DirSelec[1] = 1; //RSC_DIR_POS
    g_GyroCfgInfo.DirSelec[2] = 0; //RSC_DIR_POS
    gyro_cfg(&g_GyroCfgInfo);
}

void Dx_InitIO(void)  // Config IO for external device
{
    ///pad_setDrivingSink(PAD_DS_CGPIO16, PAD_DRIVINGSINK_15MA);
#if 0
    pad_setDrivingSink(PAD_DS_CGPIO0, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO1, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO2, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO3, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO4, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO5, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO6, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO7, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO8, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO9, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO10, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO11, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO12, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO13, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO14, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO15, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO16, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO17, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO18, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO19, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO20, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO21, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO22, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO23, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO24, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO25, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO26, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO27, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO28, PAD_DRIVINGSINK_2P5MA);
    //pad_setDrivingSink(PAD_DS_CGPIO29, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO30, PAD_DRIVINGSINK_2P5MA);
    //pad_setDrivingSink(PAD_DS_CGPIO31, PAD_DRIVINGSINK_2P5MA);
    //pad_setDrivingSink(PAD_DS_CGPIO32, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_CGPIO33, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO0, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO1, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO2, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO3, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO4, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO5, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO6, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO7, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO8, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO9, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO10, PAD_DRIVINGSINK_2P5MA);
    //pad_setDrivingSink(PAD_DS_LGPIO11, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO12, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO13, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO14, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO15, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO16, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO17, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO18, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO19, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO20, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO21, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO22, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO23, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO24, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO25, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO26, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO27, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO28, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO29, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_LGPIO30, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO0, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO1, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO2, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO3, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO4, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO5, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO6, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO7, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO8, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO9, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO10, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO11, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO12, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO13, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO14, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO15, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO16, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO17, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO18, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO19, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO20, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO21, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO22, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO23, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO24, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO25, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO26, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO27, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO28, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO29, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO30, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO31, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO32, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO33, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO34, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO35, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO36, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO37, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO38, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO39, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO40, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO41, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO42, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO43, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO44, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO45, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_PGPIO46, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_SGPIO0, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_SGPIO1, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_SGPIO2, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_SGPIO3, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_SGPIO4, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_SGPIO5, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_SGPIO6, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_SGPIO7, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_SGPIO8, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_SGPIO9, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_SGPIO10, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_DGPIO1, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_DGPIO2, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_DGPIO3, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_DGPIO4, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_DGPIO5, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_DGPIO6, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_DGPIO7, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_DGPIO8, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_DGPIO9, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_DGPIO10, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_DGPIO11, PAD_DRIVINGSINK_2P5MA);
    pad_setDrivingSink(PAD_DS_DGPIO12, PAD_DRIVINGSINK_2P5MA);

#endif
    IO_InitIntDir();    //initial interrupt destination
    IO_InitPinmux();    //initial PINMUX config
    IO_InitGPIO();      //initial GPIO pin status
    IO_InitADC();       //initial ADC pin status
    IO_InitNet();       //initial Net Device
    IO_InitGyro();      //initial gyro x,y,z direction
#if (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_PWRIC)
    DBG_IND("[PWRIC] - poweric i2c-open\r\n");
    PwrIC_Open();
    DBG_IND("[PWRIC] - poweric init\r\n");
    PwrIC_DoCmd(cmd_PwrIC_init, 0, 0);
    //PwrIC_DoCmd(cmd_LCD_setbl, 0xF, 0);
#endif
    //TempIC_I2C_Init();
}

void Dx_UninitIO(void)  // Config IO for external device
{
}



