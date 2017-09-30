/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       IOCfg.h
    @ingroup    mIPRJAPCommonIO

    @brief      Header file of IO config
                This file is the header file of IO config

    @note       Nothing.
    @date       2012/09/04
*/

/** \addtogroup mIPRJAPCommonIO */
//@{

#ifndef _IOCFG_H
#define _IOCFG_H

#include "Type.h"
#include "IOInit.h"
#include "pad.h"
#include "top.h"
#include "gpio.h"
#include "adc.h"
#include "pwm.h"

#define USE_VIO ENABLE

#if (USE_VIO == ENABLE)
#define VIO_TV_DET                  1
#define VIO_HDMI_DET                2
#define VIO_LCD2_DET                3
#define VIO_SENSOR2_DET             4
#define VIO_SENSOR1_DET             5
#define VIO_AIN_DET                 6
#define VIO_MAX_ID                  16
extern UINT32 vio_getpin(UINT32 id);
extern void vio_setpin(UINT32 id, UINT32 v);
#endif

//--------------------------------------------------------------------
// GPIO common
//--------------------------------------------------------------------
typedef struct _GPIO_INIT_OBJ{
    UINT32  GpioPin;
    UINT32  GpioDir;
    UINT32  PadDir;
    UINT32  PadPin;
} GPIO_INIT_OBJ,*PGPIO_INIT_OBJ;

#define GPIO_KEY            DISABLE

//--------------------------------------------------------------------
// ADC common
//--------------------------------------------------------------------

#define ADC_KEY             ENABLE

#define VOLDET_ADC_CONT_MODE        ENABLE //!< ENABLE for continuous, DISABLE for one-shot

#if (VOLDET_ADC_CONT_MODE == ENABLE)
#define VOLDET_ADC_MODE             TRUE
#else
#define VOLDET_ADC_MODE             FALSE
#endif

//--------------------------------------------------------------------
// PWM common
//--------------------------------------------------------------------



//--------------------------------------------------------------------
// Display device
//--------------------------------------------------------------------
// LCD communicate
#define LCD_COMM_DUMMY              0
#define LCD_COMM_BY_GPIO            1
#define LCD_COMM_BY_SIF             2
#define LCD_COMM_CTRL               LCD_COMM_BY_GPIO //EVB
#if (LCD_COMM_CTRL == LCD_COMM_BY_GPIO)
#define GPIO_LCD_SIF_SEN            L_GPIO_28//EVB
#define GPIO_LCD_SIF_SCK            L_GPIO_29//EVB
#define GPIO_LCD_SIF_SDA            L_GPIO_30//EVB
#endif
#if (LCD_COMM_CTRL == LCD_COMM_BY_SIF)
#define SIF_LCD_CH                  SIF_CH1
#define PAD_LCD_SIF_SEN             PAD_PIN_LGPIO28
#define PAD_LCD_SIF_SCK             PAD_PIN_LGPIO29
#define PAD_LCD_SIF_SDA             PAD_PIN_LGPIO30
#define DIR_LCD_SIF_SEN             PAD_PULLUP
#define DIR_LCD_SIF_SCK             PAD_PULLUP
#define DIR_LCD_SIF_SDA             PAD_PULLUP
#endif
//#define GPIO_LCD_SLEEP              87 //FPGA not support




// LCD backlight
#define LCD_BACKLIGHT_DUMMY         0
#define LCD_BACKLIGHT_BY_GPIO       1
#define LCD_BACKLIGHT_BY_PWM        2
#define LCD_BACKLIGHT_BY_PWRIC      3
#define LCD_BACKLIGHT_CTRL          LCD_BACKLIGHT_BY_GPIO //EVB
#if (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_PWM)
#define GPIO_PWM_LCD_BLG_PCTL       x
#define PWMID_LCD_BLG_PCTL          PWMID_x
#elif (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_GPIO)
#define GPIO_LCD_BLG_PCTL           P_GPIO_15//P_GPIO_3
#define PAD_LCD_BLG_PCTL            PAD_PIN_PGPIO15//PAD_PIN_PGPIO3
#endif

// LCD2 communicate
#define LCD2_COMM_CTRL              LCD_COMM_BY_GPIO //EVB
#if (LCD2_COMM_CTRL == LCD_COMM_BY_GPIO)
#define GPIO_LCD2_SIF_SEN           P_GPIO_23 //EVB --- NOTE! it will conflict with PIN_SIF_CFG_CH2
#define GPIO_LCD2_SIF_SCK           P_GPIO_25 //EVB --- NOTE! it will conflict with PIN_SIF_CFG_CH2
#define GPIO_LCD2_SIF_SDA           P_GPIO_26 //EVB --- NOTE! it will conflict with PIN_SIF_CFG_CH2
#endif
#if (LCD2_COMM_CTRL == LCD_COMM_BY_SIF)
#define SIF_LCD2_CH                 SIF_CHx
#define PAD_LCD2_SIF_SEN            PAD_PIN_PGPIO23
#define PAD_LCD2_SIF_SCK            PAD_PIN_PGPIO25
#define PAD_LCD2_SIF_SDA            PAD_PIN_PGPIO26
#endif
//#define GPIO_LCD2_SLEEP             87 //FPGA not support
//#define GPIO_LCD2_RESET             88 //FPGA not support

// LCD2 backlight
#define LCD2_BACKLIGHT_CTRL         LCD_BACKLIGHT_DUMMY //EVB
#if (LCD2_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_PWM)
#define GPIO_PWM_LCD2_BLG_PCTL      x
#define PWMID_LCD2_BLG_PCTL         PWMID_x
#elif (LCD2_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_GPIO)
#define GPIO_LCD2_BLG_PCTL          x_GPIO_x
#define PAD_LCD2_BLG_PCTL           PAD_PIN_xGPIOx
#endif

// TV
#define TV_DET_DUMMY                0
#define TV_DET_BY_VIO               1
#define TV_DET_BY_GPIO              2
#define TV_DET_BY_ADC               3
#define TV_DET_CTRL                 TV_DET_BY_GPIO //EVB
#if (TV_DET_CTRL == TV_DET_BY_GPIO)
#define GPIO_TV_PLUG                P_GPIO_17
#define PAD_TV_PLUG                 PAD_PIN_PGPIO17
#endif
#if (TV_DET_CTRL == TV_DET_BY_ADC)
#define ADC_CH_TV_PLUG              ADC_CHANNEL_x
#endif
#if 1
#define GPIO_TV_DET                P_GPIO_17
#define PAD_TV_DET                 PAD_PIN_PGPIO17
#define GPIO_TV_TRIGGER             P_GPIO_10
#define PAD_TV_TRIGGER                 PAD_PIN_PGPIO10
#endif

// HDMI
#define HDMI_DET_DUMMY              0
#define HDMI_DET_BY_VIO             1
#define HDMI_DET_BY_GPIO            2
#define HDMI_DET_BY_ADC             3
#define HDMI_DET_BY_HOTPLUG         4
#define HDMI_DET_CTRL               HDMI_DET_BY_HOTPLUG //EVB

// LCD2
#define LCD2_DET_DUMMY              0
#define LCD2_DET_BY_VIO             1
#define LCD2_DET_BY_GPIO            2
#define LCD2_DET_BY_ADC             3
#define LCD2_DET_CTRL               LCD2_DET_BY_VIO //EVB

// AIN
#define AIN_DET_DUMMY               0
#define AIN_DET_BY_VIO              1
#define AIN_DET_BY_GPIO             2
#define AIN_DET_CTRL                AIN_DET_BY_VIO //EVB
#if (AIN_DET_CTRL == AIN_DET_BY_GPIO)
#define GPIO_AIN_PLUG               C_GPIO_x
#define PAD_AIN_PLUG                PAD_PIN_CGPIOx
#endif

//--------------------------------------------------------------------
// Storage device
//--------------------------------------------------------------------
// CARD
// card power
//#define GPIO_CARD_POWER             x_GPIO_x
//#define PAD_CARD_POWER              PAD_PIN_xGPIOx
// SD card detect
#define GPIO_CARD_DETECT            D_GPIO_0
#define PAD_CARD_DETECT             PAD_PIN_DGPIO0
// SD card write protect
#define GPIO_CARD_WP                D_GPIO_1
#define PAD_CARD_WP                 PAD_PIN_DGPIO1

extern BOOL     SDIOCardUser_CheckCardInserted(void);
extern BOOL     SDIOCardUser_CheckCardWP(void);
#if (_INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
extern BOOL     SDIO2CardUser_CheckCardInserted(void);
extern BOOL     SDIO2CardUser_CheckCardWP(void);
#endif

//--------------------------------------------------------------------
// Audio device
//--------------------------------------------------------------------
//Audio
extern BOOL     GPIOMap_DetAudio(void);


//--------------------------------------------------------------------
// Input device
//--------------------------------------------------------------------

// <Layout>
//------------------------------//
//                              //
//                              //
//                              //
//                              //
//                              //
//                              //
//    SW1   SW2   SW3   SW4     //
//------------------------------//

/*
    SW1        C_GPIO_10
    SW2        ADC ch1 level    0
    SW3        ADC ch1 level ~250
    SW4        ADC ch2 level    0
*/


//#define GPIO_KEY_LEFT               P_GPIO_31           // Key Left (kl)
//#define PAD_KEY_LEFT                PAD_PIN_PGPIO31
//#define GPIO_KEY_RIGHT              P_GPIO_32           // Key Right (kr)
//#define PAD_KEY_RIGHT               PAD_PIN_PGPIO32
#define GPIO_KEY_SHUTTER2           P_GPIO_6//C_GPIO_10           // Key Shutter2 (ks2)
#define PAD_KEY_SHUTTER2            PAD_PIN_PGPIO6//PAD_PIN_CGPIO10

#define ADC_CH_VOLDET_KEY1          ADC_CHANNEL_1
#define ADC_CH_VOLDET_KEY2          ADC_CHANNEL_2

extern UINT32   GPIOMap_DetKey(void);
extern UINT32   GPIOMap_DetModeDial(void);
extern BOOL     GPIOMap_DetShutter1(void);
extern BOOL     GPIOMap_DetMacro(void);
extern BOOL     GPIOMap_DetPoweroff(void);

//--------------------------------------------------------------------
// Output device
//--------------------------------------------------------------------
// LED
#define LED_RED_DUMMY               0
#define LED_RED_BY_GPIO             1
#define LED_RED_CTRL                LED_RED_BY_GPIO
#define GPIO_RED_LED                P_GPIO_0  //FPGA
#define PAD_RED_LED                 PAD_PIN_PGPIO0

#define LED_GREEN_DUMMY             0
#define LED_GREEN_BY_GPIO           1
#define LED_GREEN_CTRL              LED_GREEN_BY_GPIO
#define GPIO_GREEN_LED              P_GPIO_1  //FPGA
#define PAD_GREEN_LED               PAD_PIN_PGPIO1

#define LED_BLUE_DUMMY              0
#define LED_BLUE_BY_GPIO            1
#define LED_BLUE_CTRL               LED_BLUE_BY_GPIO
#define GPIO_BLUE_LED               C_GPIO_9 //FPGA
#define PAD_BLUE_LED                PAD_PIN_CGPIO9


#define LED_FOCUS_DUMMY             0
#define LED_FOCUS_BY_GPIO           1
#define LED_FOCUS_BY_PWM            2 //use PWM to control LED brightness
#define LED_FOCUS_CTRL              LED_RED_DUMMY
#define GPIO_FOCUS_LED              x_GPIO_x  //FPGA
#define PAD_FOCUS_LED               PAD_PIN_xGPIOx
#define PWMID_FOCUS_LED             PWMID_x //FPGA

#define INPUT_UVC_SET_IO			P_GPIO_3
#define PAD_INPUT_UVC_SET_IO   		PAD_PIN_PGPIO3



//--------------------------------------------------------------------
// Power device
//--------------------------------------------------------------------
//Battery
#define ADC_CH_VOLDET_BATTERY       ADC_CHANNEL_0
extern char*    VolDet_GetStatusString(void);



//--------------------------------------------------------------------
// Sensor device
//--------------------------------------------------------------------
// CCD
/*
typedef enum
{
    CCD_PWR_STATUS_OFF = 0,
    CCD_PWR_STATUS_ON,
    CCD_PWR_STATUS_SLEEP,
    CCD_PWR_STATUS_MAXIDX
}CCD_PWR_STATUS;
*/
/*
#define GPIO_CCDTG_VH               L_GPIO_20
#define GPIO_CCDTG_VL               L_GPIO_19
//#define GPIO_CCDTG_VH_2             50
#define GPIO_CCDTG_RST              C_GPIO_22
//#define GPIO_CCDTG_HDRV_EN          88
#define SIF_SENSOR_TG               SIF_CH2
#define SIF_SENSOR_AFE              SIF_CH3
*/

// SENSOR
#define GPIO_SENSOR_STANDBY         S_GPIO_8
#define GPIO_SENSOR_RESET           S_GPIO_7

#define GPIO_SENSOR_PWREN1_2V       P_GPIO_8
#define GPIO_SENSOR_PWREN1_8V       P_GPIO_9
#define GPIO_SENSOR_PWREN2_8V       P_GPIO_19
#define GPIO_SENSOR_PWR             P_GPIO_11
#define GPIO_SENSOR2_PWR            P_GPIO_12

#define SENSOR_DET_DUMMY            0
#define SENSOR_DET_BY_VIO           1
#define SENSOR_DET_BY_GPIO          2
#define SENSOR_DET_BY_VX1           3
#define SENSOR_DET_CTRL             SENSOR_DET_BY_VX1

///4G POWER
#define	GPIO_4G_RESET				L_GPIO_24
#define	GPIO_4G_POWER				P_GPIO_2
#define	GPIO_4G_POWER_EN			L_GPIO_22
#define PAD_4G_POWER_EN   		    PAD_PIN_LGPIO22
#define	GPIO_4G_WAKE_UP_AP			L_GPIO_25
#define	GPIO_4G_MODE0				L_GPIO_26
#define	GPIO_AP_WAKE_UP_4G			L_GPIO_27

#define	GPIO_MCU_RESET				P_GPIO_36

//LCD POWER
#define GPIO_LCD_POWER_EN           P_GPIO_7
#define GPIO_LCD_RESET              L_GPIO_9 //FPGA not support

#define GPIO_WIFI_SHDN P_GPIO_16 //WiFi Shut Down

#define GPIO_WIFI_POWER_EN  P_GPIO_3

#define GPIO_CARD_POWER             P_GPIO_18
#define PAD_CARD_POWER              PAD_PIN_PGPIO18


#if 1
#define GPIO_BACKCAR_DETECT            P_GPIO_37
#define PAD_BACKCAR_DETECT             PAD_PIN_PGPIO37
#endif

#if 1
#define GPIO_SDCARD_SWITCH_DETECT      L_GPIO_29
#define PAD_SDCARD_SWITCH_DETECT       PAD_PIN_LGPIO29
#endif

#if 1
#define GPIO_MCU2NTK_POWERKEY_STATUS             P_GPIO_6
#define PAD_MCU2NTK_POWERKEY_STATUS              PAD_PIN_PGPIO6
#endif

#if 1
#if 1
#define GPIO_NTK2MCU_WATCHDOG                   P_GPIO_40
#define PAD_NTK2MCU_WATCHDOG                    PAD_PIN_PGPIO40

#define GPIO_NTK2MCU_POWER                   P_GPIO_6
#define PAD_NTK2MCU_POWER                    PAD_PIN_PGPIO6
#else
#define GPIO_NTK2MCU_WATCHDOG                   P_GPIO_40
#define PAD_NTK2MCU_WATCHDOG                    PAD_PIN_PGPIO40

#define GPIO_NTK2MCU_DRVIO2                   P_GPIO_5
#define PAD_NTK2MCU_DRVIO2                    PAD_PIN_PGPIO5
#endif
#endif

extern void     GPIOMap_TurnOnCCDPower(void);
extern void     GPIOMap_TurnOffCCDPower(void);
extern UINT8    GPIOMap_IsCCDPowerOn(void);
extern void     GPIOMap_SleepCCDPower(void);
extern void     GPIOMap_WakeUpCCDPower(void);

//--------------------------------------------------------------------
// NET
//--------------------------------------------------------------------
// NETWORK_GMAC_ETHERNET
#define GPIO_ETH_GMAC  P_GPIO_24
// RTL 8192/8189
//#define GPIO_WIFI_SHDN C_GPIO_12 //WiFi Shut Down

//--------------------------------------------------------------------
// Storbe device
//--------------------------------------------------------------------
// STROBE
/*
#define GPIO_FLASH_CHARGE           C_GPIO_21
#define PAD_FL_CHARGE               PAD_PIN_CGPIO21
#define GPIO_FLASH_RDY              P_GPIO_12
#define PAD_FL_RDY                  PAD_PIN_PGPIO12
#define GPIO_FLASH_TRIGGER          D_GPIO_7
#define PAD_FL_TRIGGER              PAD_PIN_DGPIO7
*/

//--------------------------------------------------------------------
// Lens device
//--------------------------------------------------------------------
//#NT#2012/07/20#Isiah Chang -begin
//#NT#Rearrange GPIO map of lens.
//FPGA Lens temporarily
/* Input pins of NT96650. */
#if 0
#define GPIO_LENS_POWER_EN          24
#define GPIO_LENS_ZOOM_PI           P_GPIO_25
#define GPIO_LENS_ZOOM_PI_ITR       GPIO_INT_15

#define GPIO_LENS_ZOOM_PR           P_GPIO_0//P_GPIO_50
#define GPIO_LENS_FOCUS_PI          P_GPIO_1//P_GPIO_49

#define PAD_LENS_ZOOM_PI            PAD_PIN_DGPIO1
#define PAD_LENS_ZOOM_PR            PAD_PIN_PGPIO49
#define PAD_LENS_FOCUS_PI           PAD_PIN_PGPIO50

/* Output pins of NT96650.  */
//Temp solution for fpga 2014/03/03
#define GPIO_LENS_EN                HSI_GPIO_18
#define GPIO_LENS_MD1               HSI_GPIO_19

//#define GPIO_LENS_EN                S_GPIO_18
//#define GPIO_LENS_MD1               S_GPIO_19
#define GPIO_LENS_MD2               L_GPIO_16
#define GPIO_LENS_MD3               L_GPIO_17
#define GPIO_LENS_IN1A              P_GPIO_40 //AF_A+
#define GPIO_LENS_IN1B              P_GPIO_41 //AF_A-
#define GPIO_LENS_IN2A              P_GPIO_42 //AF_B+
#define GPIO_LENS_IN2B              P_GPIO_43 //AF_B-
#define GPIO_LENS_IN3A              P_GPIO_44 //IRIS_A+
#define GPIO_LENS_IN3B              P_GPIO_45 //IRIS_A-
#define GPIO_LENS_IN4A              P_GPIO_46 //IRIS_B+
#define GPIO_LENS_IN4B              P_GPIO_7//P_GPIO_47 //IRIS_B-
#define GPIO_LENS_IN5A              P_GPIO_38 //ZOOM_+
#define GPIO_LENS_IN5B              P_GPIO_39 //ZOOM_-
#define GPIO_LENS_IN6A              P_GPIO_36 //Shutter_+
#define GPIO_LENS_IN6B              P_GPIO_37 //Shutter_-

//#NT#2012/07/20#Isiah Chang -end
#endif

//Lcd switch
#define GPIO_LCD_SWITCH_DETECT            P_GPIO_13
#define PAD_LCD_SWITCH_DETECT             PAD_PIN_PGPIO13
//Back up	//yliu_add
#define GPIO_BACK_UP_DETECT            C_GPIO_23
#define PAD_BACK_UP_DETECT             PAD_PIN_CGPIO23

extern BOOL GPIO_MapDetLcdSwitch(void);

extern void     GPIOMap_SetWatchDogStatus(BOOL HiOrLow);
extern void GPIOMap_SetWatchDogPowerEn(BOOL Flag);
extern BOOL GPIOMap_GetWatchDogPowerEn(void);

extern BOOL GPIO_MapBackUpSwitch(void);

extern void InPut_UVC_Set_IO (void);
extern void OutPut_UVC_Clear_IO (void);

#if 1
BOOL GPIO_GetBackcar_Status(void);
#endif

#if 1
BOOL GPIO_GetSDCard_Switch_Status(void);
#endif

#if 1
void GPIO_Set_AP_Wakeup_BP(BOOL enable);
BOOL GPIO_Get_4G_Wakeup_State(void);
#endif


#if 1
extern BOOL GPIO_Get_Tv_Det_State(void);
extern void Set_Tv_State(BOOL state);
extern BOOL Get_Tv_State(void);
#endif

#if defined(YQCONFIG_KEY_TAKE_PHOTO)
extern BOOL GPIO_Key_Take_Photo_State(void);
#endif

#endif

//@}

