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
     {  GPIO_SENSOR_STANDBY,    GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      },
     {  GPIO_SENSOR_RESET,      GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      },
     {  GPIO_IRCUT_CTRL0,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      },
     {  GPIO_IRCUT_CTRL1,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_PIN_NOT_EXIST      },
     {  GPIO_DIGITAL_INPUT0,    GPIO_DIR_INPUT,     PAD_PULLDOWN       ,  PAD_PIN_NOT_EXIST      },
     {  GPIO_DIGITAL_OUTPUT0,   GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_PIN_NOT_EXIST      },
     //#NT#2016/03/02#David Tsai -begin
     //#NT#Add RS485 direction control pin init
     {  GPIO_RS485_DIR_CTRL,    GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_PIN_NOT_EXIST      },
     //#NT#2016/03/02#David Tsai -end
#if (_NETWORK_ == _NETWORK_GMAC_ETHERNET_)
     {  GPIO_ETH_GMAC,          GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,  PAD_PIN_NOT_EXIST      },
#endif
#if 0
     {  GPIO_KEY_SHUTTER1,      GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_SHUTTER1    },
     {  GPIO_KEY_SHUTTER2,      GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_SHUTTER2    },
     {  GPIO_LENS_ZOOM_PI,      GPIO_DIR_INPUT,     PAD_PULLDOWN,         PAD_LENS_ZOOM_PI       },
     {  GPIO_LENS_ZOOM_PR,      GPIO_DIR_INPUT,     PAD_PULLDOWN,         PAD_LENS_ZOOM_PR       },
     {  GPIO_LENS_FOCUS_PI,     GPIO_DIR_INPUT,     PAD_PULLDOWN,         PAD_LENS_FOCUS_PI      },

     {  GPIO_LENS_IN1A,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN1B,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN2A,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN2B,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN3A,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN3B,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN4A,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN4B,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN5A,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN5B,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN6A,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      },
     {  GPIO_LENS_IN6B,         GPIO_DIR_OUTPUT,    PAD_NONE,             PAD_PIN_NOT_EXIST      }
     {  GPIO_KEY_ZOOMOUT,       GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_ZOOMOUT       },
     {  GPIO_KEY_ZOOMIN,        GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_ZOOMIN       },

     {  GPIO_KEY_ENTER,         GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_ENTER       },
     {  GPIO_KEY_UP,            GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_UP       },
     {  GPIO_KEY_PLAYBACK,      GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_PLAYBACK       },
     {  GPIO_KEY_DOWN,          GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_DOWN       },
     {  GPIO_KEY_MODE,          GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_MODE       },
     {  GPIO_KEY_MENU,          GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_MENU       },
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
    #endif

    // Enable adc control logic
    adc_setEnable(TRUE);
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
    Delay_DelayMs(10);
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
    g_GyroCfgInfo.DirSelec[1] = 0; //RSC_DIR_POS
    g_GyroCfgInfo.DirSelec[2] = 0; //RSC_DIR_POS
    gyro_cfg(&g_GyroCfgInfo);
}

void Dx_InitIO(void)  // Config IO for external device
{
    IO_InitIntDir();    //initial interrupt destination
    IO_InitPinmux();    //initial PINMUX config
    IO_InitGPIO();      //initial GPIO pin status
    IO_InitADC();       //initial ADC pin status
    IO_InitNet();       //initial Net Device
    IO_InitGyro();      //initial gyro x,y,z direction
}

void Dx_UninitIO(void)  // Config IO for external device
{
}



