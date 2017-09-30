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
#if (_LCDTYPE_ == _LCDTYPE_PW27P05_ILI8961_)
#define LCD_COMM_CTRL               LCD_COMM_BY_SIF //EVB
#elif (_LCDTYPE_ == _LCDTYPE_PW35P00_HX8238D_)
#define LCD_COMM_CTRL               LCD_COMM_BY_SIF //EVB
#else
#define LCD_COMM_CTRL               LCD_COMM_BY_GPIO //EVB
#endif
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
#if (_LCDTYPE_ == _LCDTYPE_PW27P05_ILI8961_)
#define DIR_LCD_SIF_SEN             PAD_PULLUP
#define DIR_LCD_SIF_SCK             PAD_PULLUP
#define DIR_LCD_SIF_SDA             PAD_PULLUP
#elif (_LCDTYPE_ == _LCDTYPE_PW35P00_HX8238D_)
#define DIR_LCD_SIF_SEN             PAD_PULLUP
#define DIR_LCD_SIF_SCK             PAD_PULLUP
#define DIR_LCD_SIF_SDA             PAD_PULLUP
#else
#define DIR_LCD_SIF_SEN             PAD_PULLUP
#define DIR_LCD_SIF_SCK             PAD_PULLDOWN
#define DIR_LCD_SIF_SDA             PAD_PULLDOWN
#endif
#endif
//#define GPIO_LCD_SLEEP              87 //FPGA not support
//#define GPIO_LCD_RESET              88 //FPGA not support
#if (_LCDTYPE_ == _LCDTYPE_ILI9341_IF8B_)
#define GPIO_LCD_RESET              C_GPIO_27 // for ILI9341 + touch panel TEST!!!
#endif

// LCD backlight
#define LCD_BACKLIGHT_DUMMY         0
#define LCD_BACKLIGHT_BY_GPIO       1
#define LCD_BACKLIGHT_BY_PWM        2
#define LCD_BACKLIGHT_BY_PWRIC      3
#define LCD_BACKLIGHT_CTRL          LCD_BACKLIGHT_DUMMY //EVB
#if (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_PWM)
#define GPIO_PWM_LCD_BLG_PCTL       x
#define PWMID_LCD_BLG_PCTL          PWMID_x
#elif (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_GPIO)
#define GPIO_LCD_BLG_PCTL           x_GPIO_x
#define PAD_LCD_BLG_PCTL            PAD_PIN_xGPIOx
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
#define TV_DET_CTRL                 TV_DET_BY_VIO //EVB
#if (TV_DET_CTRL == TV_DET_BY_GPIO)
#define GPIO_TV_PLUG                C_GPIO_x
#define PAD_TV_PLUG                 PAD_PIN_CGPIOx
#endif
#if (TV_DET_CTRL == TV_DET_BY_ADC)
#define ADC_CH_TV_PLUG              ADC_CHANNEL_x
#endif

// HDMI
#define HDMI_DET_DUMMY              0
#define HDMI_DET_BY_VIO             1
#define HDMI_DET_BY_GPIO            2
#define HDMI_DET_BY_ADC             3
#define HDMI_DET_BY_HOTPLUG         4
#define HDMI_DET_CTRL               HDMI_DET_BY_VIO //EVB

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
//------------------------------//  //------------------------------//
//        SW13   SW14   SW16    //  //        SW13   SW14   SW16    //
//                              //  //                              //
// SW8    SW4           SW6     //  // SW8    kl             kr     //
//               SW7            //  //               SW7            //
// SW10                         //  // SW10                         //
//          SW9  SW5  SW12      //  //          SW9  SW5   SW12     //
// SW15                         //  // SW15                         //
//               SW11           //  //               SW11           //
// SW18                         //  // SW18                         //
//------------------------------//  //------------------------------//
/*
    SW4        P_GPIO_31
    SW5        ADC ch2 level ~0
    SW6        P_GPIO_32
    SW7        ADC ch2 level ~208
    SW8        P_GPIO_33
    SW9        ADC ch2 level ~412
    SW10       P_GPIO_34
    SW11       ADC ch2 level ~616
    SW12       ADC ch2 level ~822
    SW13       PWR_SW1
    SW14       PWR_SW3
    SW15       P_GPIO_40
    SW16       PWR_SW4
    (SW17另一版 PCB才有)
    SW18       P_GPIO_20
*/



//#define GPIO_KEY_LEFT               P_GPIO_31           // Key Left (kl)
//#define PAD_KEY_LEFT                PAD_PIN_PGPIO31
//#define GPIO_KEY_RIGHT              P_GPIO_32           // Key Right (kr)
//#define PAD_KEY_RIGHT               PAD_PIN_PGPIO32


/*
#define ADC_CH_VOLDET_MS1           ADC_CHANNEL_1
#define ADC_CH_VOLDET_MS2           ADC_CHANNEL_0
*/
#define ADC_CH_VOLDET_KEY1          ADC_CHANNEL_2


//Touch panel controller

//#define GPIO_TP_BUSY                (2 | GPIO_IS_DGPIO)  //HW TS_INT =DGPIO-02
#define GPIO_TP_PENIRQ              (7 | GPIO_IS_DGPIO) //HW TS_PENIRQ =DGPIO-12
#define GPIO_TP_DOUT                P_GPIO_8 //P_GPIO_58 //HW TP_DI  =SBDAT0/AGPIO-26   (FW output = HW input)
#define GPIO_TP_DIN                 (1 | GPIO_IS_DGPIO) //HW TS_DO  =DGPIO-01  (FW input = HW output)
#define GPIO_TP_DCLK                P_GPIO_7//P_GPIO_57 //HW TS_CLK =SBCK0/AGPIO-25
//#define GPIO_TP_CS                  28 //HW TS_CS  =SBCS1/AGPIO-28  //SIF channel 1
#define GPIO_TP_CS                  P_GPIO_6//P_GPIO_56 //HW TS_CS  =SBCS0/AGPIO-27  //SIF channel 0
#if (_TOUCH_FUNC_ == _TOUCH_ENABLE_)
#define GPIO_TP_INT                 C_GPIO_26 // for ILI9341 + touch panel TEST!!!
#define GPIO_TP_RST                 C_GPIO_25 // for ILI9341 + touch panel TEST!!!
#endif
//#define PAD_TP_BUSY                 PAD_PIN_DGPIO2
#define PAD_TP_PENIRQ               PAD_PIN_DGPIO7
#define PAD_TP_DOUT                 PAD_PIN_PGPIO58
#define PAD_TP_DIN                  PAD_PIN_DGPIO1
#define PAD_TP_DCLK                 PAD_PIN_PGPIO57
//#define PAD_TP_CS                   PAD_PIN_SBCS1  //SIF channel 1
#define PAD_TP_CS                   PAD_PIN_PGPIO56  //SIF channel 0
#if (_TOUCH_FUNC_ == _TOUCH_ENABLE_)
#define PAD_TP_INT                  PAD_PIN_CGPIO26
#endif

//G-Sensor
/*
#define GPIO_GS_INT1                (3 | GPIO_IS_DGPIO)
#define GPIO_GS_INT2                (4 | GPIO_IS_DGPIO)
#define PAD_GS_INT1                 PAD_PIN_DGPIO3
#define PAD_GS_INT2                 PAD_PIN_DGPIO4
*/
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
#define LED_RED_CTRL                LED_RED_DUMMY
#define GPIO_RED_LED                x_GPIO_x  //FPGA
#define PAD_RED_LED                 PAD_PIN_xGPIOx

#define LED_GREEN_DUMMY             0
#define LED_GREEN_BY_GPIO           1
#define LED_GREEN_CTRL              LED_RED_DUMMY
#define GPIO_GREEN_LED              x_GPIO_x  //FPGA
#define PAD_GREEN_LED               PAD_PIN_xGPIOx

#define LED_FOCUS_DUMMY             0
#define LED_FOCUS_BY_GPIO           1
#define LED_FOCUS_BY_PWM            2 //use PWM to control LED brightness
#define LED_FOCUS_CTRL              LED_RED_DUMMY
#define GPIO_FOCUS_LED              x_GPIO_x  //FPGA
#define PAD_FOCUS_LED               PAD_PIN_xGPIOx
#define PWMID_FOCUS_LED             PWMID_x //FPGA


//--------------------------------------------------------------------
// Power device
//--------------------------------------------------------------------
//Battery
#define ADC_CH_VOLDET_BATTERY       ADC_CHANNEL_3
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


#define GPIO_SENSOR_STANDBY         S_GPIO_9    // not used
#define GPIO_SENSOR_RESET           S_GPIO_8    // sensor 1
#define GPIO_SENSOR_RESET2          S_GPIO_7    // sensor 2
// SENSOR
#define SENSOR_DET_DUMMY            0
#define SENSOR_DET_BY_VIO           1
#define SENSOR_DET_BY_GPIO          2
#define SENSOR_DET_BY_VX1           3
#define SENSOR_DET_CTRL             SENSOR_DET_BY_VX1

extern void     GPIOMap_TurnOnCCDPower(void);
extern void     GPIOMap_TurnOffCCDPower(void);
extern UINT8    GPIOMap_IsCCDPowerOn(void);
extern void     GPIOMap_SleepCCDPower(void);
extern void     GPIOMap_WakeUpCCDPower(void);

//--------------------------------------------------------------------
// IR CUT device
//--------------------------------------------------------------------
#define GPIO_IRCUT_CTRL0         P_GPIO_0
#define GPIO_IRCUT_CTRL1         P_GPIO_1

//--------------------------------------------------------------------
// NET
//--------------------------------------------------------------------
// NETWORK_GMAC_ETHERNET
#define GPIO_ETH_GMAC  P_GPIO_24

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
#endif

//@}

