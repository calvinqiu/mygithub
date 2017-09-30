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
#include "IOCfg.h"
#include "Utility.h"
#include "pll.h"

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
     {  GPIO_KEY_LEFT,          GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_LEFT       },
     {  GPIO_KEY_ENTER,         GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_ENTER       },
     {  GPIO_KEY_UP,            GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_UP       },
     {  GPIO_KEY_RIGHT,         GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_RIGHT       },
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

    @return void
*/
void IO_InitGPIO(void)
{
    #if 0
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

    DBG_IND("GPIO END\r\n");
    #endif
}

/**
    Initialize voltage detection

    Initialize voltage detection for battery and flash

    @return void
*/

void IO_InitADC(void)
{
#if 0
    if (adc_open(VOLDET_BATTERY_ADC_CH) != E_OK)
    {
        DBG_ERR("Can't open ADC channel for battery voltage detection\r\n");
        return;
    }

    //650 Range is 250K Hz ~ 2M Hz
    adc_setConfig(ADC_CONFIG_ID_OCLK_FREQ, 250000); //250K Hz

    //battery voltage detection
    adc_setChConfig(VOLDET_BATTERY_ADC_CH, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
    adc_setChConfig(VOLDET_BATTERY_ADC_CH, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
    adc_setChConfig(VOLDET_BATTERY_ADC_CH, ADC_CH_CONFIG_ID_INTEN, FALSE);
    // Enable adc control logic
    adc_setEnable(TRUE);
#endif
}

#if 0
static char ADCStr1[32];
static char ADCStr2[32];

char* VolDet_GetStatusString1(void)
{
    sprintf(ADCStr1, "A0=%ld,A1=%ld,A2=%ld,A3=%ld", adc_readData(0), adc_readData(1),adc_readData(2),adc_readData(3));
    return ADCStr1;
}

char* VolDet_GetStatusString2(void)
{
    sprintf(ADCStr2, "A4=%ld,A5=%ld,A6=%ld,A7=%ld", adc_readData(4), adc_readData(5),adc_readData(6),adc_readData(7));
    return ADCStr2;
}
#endif



