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
#include "../../../../Include/Lib/_System/GxSystem/BinInfo.h"
extern BININFO gBinInfo;

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
    {  GPIO_SENSOR2_PWR,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      },
    {  GPIO_SENSOR_PWR,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      },
    {  GPIO_4G_POWER,      		GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST     },
    {  GPIO_4G_RESET,      		GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST     },
    {  GPIO_4G_MODE0,      		GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,   PAD_PIN_NOT_EXIST     },
    {  GPIO_MCU_RESET,      	GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,   PAD_PIN_NOT_EXIST     },
    {  GPIO_LCD_POWER_EN,      	GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST     },
    {  GPIO_LCD_BLG_PCTL,      GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_PIN_NOT_EXIST      },
    {  GPIO_LCD_RESET,         GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,     GPIO_SET_NONE       },
    {  GPIO_WIFI_POWER_EN,         GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_HI,     GPIO_SET_NONE       },
    {  GPIO_WIFI_SHDN,     GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      }, //WIFI 
    {  GPIO_TV_DET,    GPIO_DIR_INPUT,     PAD_PULLDOWN,    PAD_TV_DET            },
    {  GPIO_TV_TRIGGER,   GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_HI,   PAD_TV_TRIGGER },

    //LED
    {  GPIO_RED_LED,           GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,     GPIO_SET_NONE       },
    {  GPIO_GREEN_LED,         GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,     GPIO_SET_NONE       },
    {  GPIO_BLUE_LED,          GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,    GPIO_SET_NONE       },


    {  GPIO_BACKCAR_DETECT, 	GPIO_DIR_INPUT,     PAD_NONE,         PAD_BACKCAR_DETECT   },	//yliu_add_for_back


    {  GPIO_SDCARD_SWITCH_DETECT, 	GPIO_DIR_INPUT,     PAD_PULLUP,         PAD_SDCARD_SWITCH_DETECT   },




    {  GPIO_NTK2MCU_WATCHDOG, 	GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,         PAD_NTK2MCU_WATCHDOG   },
    {  GPIO_NTK2MCU_POWER, 	GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,         PAD_NTK2MCU_POWER},





    {  GPIO_4G_WAKE_UP_AP, 	GPIO_DIR_INPUT,     PAD_NONE,         PAD_PIN_NOT_EXIST},
    {  GPIO_AP_WAKE_UP_4G, 	GPIO_DIR_OUTPUT,     GPIO_SET_OUTPUT_LOW,         PAD_PIN_NOT_EXIST},

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

    if (((gBinInfo.ld.LdCtrl2 & LDCF_UPDATE_FW)!=0))
    {
        gpio_setPin(GPIO_NTK2MCU_POWER);
    }
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
    if (adc_open(ADC_CH_VOLDET_KEY1) != E_OK)
    {
        DBG_ERR("Can't open ADC channel for KEY1 voltage detection\r\n");
    }
    else
    {
        adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
        adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
        adc_setChConfig(ADC_CH_VOLDET_KEY1, ADC_CH_CONFIG_ID_INTEN, FALSE);
    }
    
    if (adc_open(ADC_CH_VOLDET_KEY2) != E_OK)
    {
        DBG_ERR("Can't open ADC channel for KEY2 voltage detection\r\n");
    }
    else
    {
        adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_SAMPLE_FREQ, 10000); //10K Hz, sample once about 100 us for CONTINUOUS mode
        adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_SAMPLE_MODE, (VOLDET_ADC_MODE) ? ADC_CH_SAMPLEMODE_CONTINUOUS : ADC_CH_SAMPLEMODE_ONESHOT);
        adc_setChConfig(ADC_CH_VOLDET_KEY2, ADC_CH_CONFIG_ID_INTEN, FALSE);
    }
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


BOOL GPIO_MapDetLcdSwitch(void)
{
    return (gpio_getPin(GPIO_LCD_SWITCH_DETECT)?FALSE:TRUE);
}

BOOL GPIO_MapBackUpSwitch(void)
{
    return (gpio_getPin(GPIO_BACK_UP_DETECT)?FALSE:TRUE);
}

void InPut_UVC_Set_IO (void)
{
    gpio_getPin(INPUT_UVC_SET_IO);
}

void OutPut_UVC_Clear_IO (void)
{
    gpio_clearPin(INPUT_UVC_SET_IO);
}

#if 1
BOOL GPIO_GetBackcar_Status(void)
{
    return (gpio_getPin(GPIO_BACKCAR_DETECT)?TRUE:FALSE);
}
#endif

#if 1
BOOL GPIO_GetSDCard_Switch_Status(void)
{
    return (gpio_getPin(GPIO_SDCARD_SWITCH_DETECT)?TRUE:FALSE);
}

#endif


#if 1
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

void GPIO_SetNTK2MCU_Power_Status(BOOL en)
{
    debug_msg("GPIO_SetNTK2MCU_Power_Status\r\n");
    if (en)
    {
        gpio_setPin(GPIO_NTK2MCU_POWER);

    }
    else
    {
        gpio_clearPin(GPIO_NTK2MCU_POWER);
    }
}
#endif

#if 1
static BOOL gIsPowerOnBy4G = FALSE;
void GPIO_Check_PowerOn_By_4G(void)
{
    if (gpio_getPin(GPIO_4G_WAKE_UP_AP))
    {
        gIsPowerOnBy4G = TRUE;
    }
    debug_msg("GPIO_Check_PowerOn_By_4G gIsPowerOnBy4G:%d\r\n", gIsPowerOnBy4G);
}

BOOL GPIO_GetPowerOn_By_4G(void)
{
    return gIsPowerOnBy4G;
}
void GPIO_Set_AP_Wakeup_BP(BOOL enable)
{
    if (enable)
    {
        gpio_setPin(GPIO_AP_WAKE_UP_4G);
    }
    else
    {
        gpio_clearPin(GPIO_AP_WAKE_UP_4G);
    }

}

BOOL GPIO_Get_4G_Wakeup_State(void)
{
    BOOL gpio_bp_wake_up_ap_state = FALSE;
    BOOL gpio_ap_wake_up_bp_state = FALSE;

    gpio_bp_wake_up_ap_state = gpio_getPin(GPIO_4G_WAKE_UP_AP);
    gpio_ap_wake_up_bp_state = gpio_getPin(GPIO_AP_WAKE_UP_4G);
    //debug_msg("gpio_bp_wake_up_ap_state:%d---gpio_ap_wake_up_bp_state:%d\r\n",gpio_bp_wake_up_ap_state, gpio_ap_wake_up_bp_state);

    return gpio_bp_wake_up_ap_state;

}
#endif


#if 1
void GPIO_Set_4G_POWER(BOOL enable)
{
    if (enable)
    {
        gpio_setPin(GPIO_4G_POWER_EN);
        gpio_setPin(GPIO_4G_POWER);
    }
    else
    {
        gpio_clearPin(GPIO_4G_POWER_EN);
        gpio_clearPin(GPIO_4G_POWER);
    }

}

void GPIO_Set_4G_POWER_IO(BOOL enable)
{
    if (enable)
    {
        gpio_setPin(GPIO_4G_POWER);
    }
    else
    {
        gpio_clearPin(GPIO_4G_POWER);
    }

}

void GPIO_Reset_4G_Module(void)
{
    debug_msg("GPIO_Reset_4G_Module\r\n");
    gpio_setDir(GPIO_4G_RESET, GPIO_DIR_OUTPUT);
    gpio_setPin(GPIO_4G_RESET);
    Delay_DelayMs(20);
    gpio_clearPin(GPIO_4G_RESET);
    Delay_DelayMs(100);
    gpio_setPin(GPIO_4G_RESET);
    Delay_DelayMs(100);
    gpio_setDir(GPIO_4G_RESET, GPIO_DIR_INPUT);

}

#endif

#if 1
static BOOL Gpio_TV_State=FALSE;
BOOL GPIO_Get_Tv_Det_State(void)
{
    BOOL gpio_tv_state = FALSE;    

    gpio_tv_state = gpio_getPin(GPIO_TV_DET);        

    return gpio_tv_state;
}
void Set_Tv_State(BOOL state)
{
    Gpio_TV_State=state;
}

BOOL Get_Tv_State(void)
{         
    return Gpio_TV_State;
}
#endif

#if defined(YQCONFIG_KEY_TAKE_PHOTO)
BOOL GPIO_Key_Take_Photo_State(void)
{
    BOOL gpio_key_photo = FALSE;    
    gpio_key_photo = gpio_getPin(GPIO_SHOT_PHOTO); 
    return gpio_key_photo;
}
#endif
