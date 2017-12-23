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
#if defined(YQCONFIG_PLATFORM_NAME_K40)
    {  GPIO_SENSOR_STANDBY,    GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      },
    {  GPIO_SENSOR_RESET,      GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      },	 	
    {  GPIO_SENSOR2_PWR,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,   PAD_PIN_NOT_EXIST      },
    {  GPIO_SENSOR_PWR,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      },
    {  GPIO_4G_POWER,      		GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST     },
    {  GPIO_4G_RESET,      		GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST     },
    {  GPIO_4G_MODE0,      		GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,   PAD_PIN_NOT_EXIST     },
    {  GPIO_MCU_RESET,      	GPIO_DIR_INPUT,    GPIO_SET_OUTPUT_LOW,   PAD_PIN_NOT_EXIST     },
    {  GPIO_LCD_POWER_EN,      	GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST     },
    {  GPIO_LCD_BLG_PCTL,      GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_PIN_NOT_EXIST      },
    {  GPIO_LCD_RESET,         GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,     GPIO_SET_NONE       },  
    {  GPIO_WIFI_POWER_EN,         GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,     GPIO_SET_NONE       },            
    {  GPIO_WIFI_SHDN,     GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,   PAD_PIN_NOT_EXIST      }, //WIFI SHDN, ShutDown

    //LED
    {  GPIO_RED_LED,           GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_HI,     GPIO_SET_NONE       },
    {  GPIO_GREEN_LED,         GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,     GPIO_SET_NONE       },
    {  GPIO_BLUE_LED,          GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,    GPIO_SET_NONE       },
#else
#if defined(YQCONFIG_PLATFORM_NAME_U15)
    {  GPIO_U15RED_LED,          GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,    GPIO_SET_NONE       },
#endif
    {  GPIO_SENSOR_STANDBY,    GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      },
    {  GPIO_SENSOR_RESET,      GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      },
    {  GPIO_KEY_SHUTTER2,      GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_SHUTTER2       },
    {  GPIO_TV_PLUG,           GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_TV_PLUG            },
    {  GPIO_WIFI_SHDN,     GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,   PAD_PIN_NOT_EXIST      }, //WIFI SHDN, ShutDown
    {  GPIO_LCD_BLG_PCTL,      GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_PIN_NOT_EXIST      },
    {  GPIO_LCD_RESET,         GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_HI,     GPIO_SET_NONE       },
#if defined(YQCONFIG_COMB_DET_LCD_SWITCH_BY_GPIO)        
    {  GPIO_LCD_SWITCH_DETECT, GPIO_DIR_INPUT,     PAD_PULLUP,         PAD_LCD_SWITCH_DETECT    },
#endif
#if defined(YQCONFIG_COMB_DET_BACKCAR_BY_GPIO)
    {  GPIO_BACK_UP_DETECT, 	GPIO_DIR_INPUT,     PAD_PULLUP,         PAD_BACK_UP_DETECT   },	//yliu_add_for_back	
#endif     
#if defined(YQCONFIG_GSENSOR_SUPPORT)
    {  GPIO_GSENSOR_POWERON, 	GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW, PAD_GSENSOR_POWERON   },	//yliu_add_for_GSENSOR_POWER
#endif     

    //LED
    {  GPIO_RED_LED,           GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_HI,     GPIO_SET_NONE       },
    {  GPIO_GREEN_LED,         GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,     GPIO_SET_NONE       },
    {  GPIO_BLUE_LED,          GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,    GPIO_SET_NONE       },

    // In put UVC set IO
    {  INPUT_UVC_SET_IO,           GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,     PAD_INPUT_UVC_SET_IO },

#if (_SENSORLIB2_ != _SENSORLIB2_OFF_)
    {  GPIO_SENSOR2_PWR,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      },
#endif

#if (_NETWORK_ == _NETWORK_GMAC_ETHERNET_)
    {  GPIO_ETH_GMAC,          GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,  PAD_PIN_NOT_EXIST       },
#endif

#endif

#if defined(YQCONFIG_BACKCAR_DET_SUPPORT)
    {  GPIO_BACKCAR_DETECT, 	GPIO_DIR_INPUT,     PAD_PULLUP,         PAD_BACKCAR_DETECT   },	//yliu_add_for_back	
#endif
#if defined(YQCONFIG_SDCARD_SWITCH_DETECT_SUPPORT)
    {  GPIO_SDCARD_SWITCH_DETECT, 	GPIO_DIR_INPUT,     PAD_PULLUP,         PAD_SDCARD_SWITCH_DETECT   },	
#endif

#if defined(YQCONFIG_PARKING_MODE_SUPPORT)
    {  GPIO_MCU2NTK_GSENSOR_STATUS, 	GPIO_DIR_INPUT,     PAD_PULLUP,         PAD_MCU2NTK_GSENSOR_STATUS},	
#endif

#if defined(YQCONFIG_PWRKEY_LONGPRESS_PWROFF_SUPPORT)
    {  GPIO_MCU2NTK_POWERKEY_STATUS, 	GPIO_DIR_INPUT,     PAD_PULLUP,         PAD_MCU2NTK_POWERKEY_STATUS   },	
#endif

#if defined(YQCONFIG_NTK2MCU_STATUS_SUPPORT)
    {  GPIO_NTK2MCU_WATCHDOG, 	GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,         PAD_NTK2MCU_WATCHDOG   },	
    {  GPIO_NTK2MCU_DRVIO2, 	GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_HI,         PAD_NTK2MCU_DRVIO2},	
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
#if defined(YQCONFIG_ADC2_FOR_KPD_SUPPORT)
    adc_open(ADC_CH_VOLDET_KEY2);
    adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
    adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
    adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_INTEN, FALSE);
#endif
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
    //#NT#NEW PHI Power Sequence
    Delay_DelayMs(20);
    gpio_setPin(GPIO_ETH_GMAC);
    Delay_DelayMs(1);
    pll_setPLLEn(PLL_ID_12,TRUE);
    pad_setDrivingSink(PAD_DS_LGPIO11, PAD_DRIVINGSINK_10MA);
    Delay_DelayMs(10);
    gpio_clearPin(GPIO_ETH_GMAC);
    Delay_DelayMs(10);
    gpio_setPin(GPIO_ETH_GMAC);
    Delay_DelayMs(150);
    //#NT#2016/08/08#Niven Cho -begin
    //#NT#FIXED, F-PHY cannot work because of clearing LGPIO_21
    //gpio_clearPin(L_GPIO_21);
    //#NT#2016/08/08#Niven Cho -end
    //#NT#2016/06/14#Niven Cho -end
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

void GPIOMap_SetWatchDogStatus(BOOL Flag)
{
    //if(Flag==TRUE)
    //  gpio_setPin(PWMID_WATCHDOG_PCTL);
    //else
    //  gpio_clearPin(PWMID_WATCHDOG_PCTL);
}

void GPIOMap_SetWatchDogPowerEn(BOOL Flag)
{
    //if(Flag==TRUE)
    //  gpio_setPin(GPIO_WATCHDOG_EN);
    //else
    //  gpio_clearPin(GPIO_WATCHDOG_EN);
}

BOOL GPIOMap_GetWatchDogPowerEn(void)
{
    //return (BOOL)gpio_getPin(GPIO_WATCHDOG_EN);
    return TRUE;
}

BOOL GPIO_MapDetLcdSwitch(void)
{
    return (gpio_getPin(GPIO_LCD_SWITCH_DETECT)?FALSE:TRUE);
}

BOOL GPIO_MapBackUpSwitch(void)
{
    return (gpio_getPin(GPIO_BACK_UP_DETECT)?FALSE:TRUE);
}

#if defined(YQCONFIG_GSENSOR_SUPPORT)&&defined(YQCONFIG_GPIO_GSENSOR_POWERON_ANDROID_SUPPORT)
//yliu_add_for_GSENSOR_POWER
BOOL GPIO_GSENSOR_POWERON_ANDROID(void)
{
    gpio_setPin(GPIO_GSENSOR_POWERON);
	Delay_DelayMs(100);
	gpio_clearPin(GPIO_GSENSOR_POWERON);
}
#endif
void InPut_UVC_Set_IO (void)
{
	debug_msg("InPut_UVC_Set_IO---------------1\r\n");
	gpio_setPin(INPUT_UVC_SET_IO);
}

void OutPut_UVC_Clear_IO (void)
{
	debug_msg("OutPut_UVC_Clear_IO---------------2\r\n");
	gpio_clearPin(INPUT_UVC_SET_IO);
}

#if defined(YQCONFIG_BACKCAR_DET_SUPPORT)
BOOL GPIO_GetBackcar_Status(void)
{
    return (gpio_getPin(GPIO_BACKCAR_DETECT)?TRUE:FALSE);
}
#endif

#if defined(YQCONFIG_SDCARD_SWITCH_DETECT_SUPPORT)
BOOL GPIO_GetSDCard_Switch_Status(void)
{
    return (gpio_getPin(GPIO_SDCARD_SWITCH_DETECT)?TRUE:FALSE);
}

#endif

#if defined(YQCONFIG_PWRKEY_LONGPRESS_PWROFF_SUPPORT)
BOOL GPIO_GetPowerKey_Status(void)
{
    return (gpio_getPin(GPIO_MCU2NTK_POWERKEY_STATUS)?TRUE:FALSE);
}
#endif

#if defined(YQCONFIG_PARKING_MODE_SUPPORT)
BOOL GPIO_GetGsensor_Status(void)
{
    return (gpio_getPin(GPIO_MCU2NTK_GSENSOR_STATUS)?TRUE:FALSE);
}
#endif


#if defined(YQCONFIG_PLATFORM_NAME_U15)||defined(YQCONFIG_SET_RECORD_REDLIGHT_TWINKLE)
void GPIO_Set_REDLIGHT_Status(BOOL en)
{
    if (en)
    {
        gpio_setPin(GPIO_U15RED_LED);
	
    }
    else
    {
        gpio_clearPin(GPIO_U15RED_LED);
    }
}
#endif



#if defined(YQCONFIG_NTK2MCU_STATUS_SUPPORT)
void GPIO_SetNTK2MCU_Watchdog_Status(BOOL en)
{
    if (en)
    {
        gpio_setPin(GPIO_NTK2MCU_WATCHDOG);
	
    }
    else
    {
        gpio_clearPin(GPIO_NTK2MCU_WATCHDOG);
    }
}
#endif
