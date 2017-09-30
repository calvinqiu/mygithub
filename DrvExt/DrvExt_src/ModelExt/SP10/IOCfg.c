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
#ifdef GPIO_SENSOR_PWR0
     {  GPIO_SENSOR_PWR0,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      },
#endif
#ifdef GPIO_SENSOR_PWR1
     {  GPIO_SENSOR_PWR1,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      },
#endif
#ifdef GPIO_SENSOR_PWR2
     {  GPIO_SENSOR_PWR2,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      },
#endif
#ifdef GPIO_KEY_SHUTTER2	
     {  GPIO_KEY_SHUTTER2,      GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_KEY_SHUTTER2       },
#endif

#if defined(GPIO_GPS_PWR)
      #ifdef ORDER_DELAY_GPS_PWR_AVOID_UPDATE_RESET
		{  GPIO_GPS_PWR,		GPIO_DIR_OUTPUT,	GPIO_SET_OUTPUT_LOW,  PAD_GPS_PWR	   },
		//{	P_GPIO_36,	  GPIO_DIR_OUTPUT,	  GPIO_SET_OUTPUT_LOW,	PAD_PIN_PGPIO36 	},//yeti 
      #else
		 {  GPIO_GPS_PWR,		 GPIO_DIR_OUTPUT,	 GPIO_SET_OUTPUT_HI,  PAD_GPS_PWR	   },
		//{  P_GPIO_36,	 GPIO_DIR_OUTPUT,	 GPIO_SET_OUTPUT_HI,  PAD_PIN_PGPIO36	   },//yeti 150429
      #endif
#endif     

#if (TV_DET_CTRL == TV_DET_BY_GPIO)
     {  GPIO_TV_PLUG,           GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_TV_PLUG            },
#endif   
#if (LCD_BACKLIGHT_CTRL == LCD_BACKLIGHT_BY_GPIO)
     {  GPIO_LCD_BLG_PCTL,      GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_LOW,  PAD_PIN_NOT_EXIST      },
#endif     

#if (_SENSORLIB2_ != _SENSORLIB2_OFF_)
     {  GPIO_SENSOR2_PWR,       GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,   PAD_PIN_NOT_EXIST      },
#endif

#if (_NETWORK_ == _NETWORK_GMAC_ETHERNET_)
     {  GPIO_ETH_GMAC,          GPIO_DIR_OUTPUT,    GPIO_SET_OUTPUT_HI,  PAD_PIN_NOT_EXIST       },
#endif

#ifdef  GPIO_WIFI_LED
{  GPIO_WIFI_LED,		GPIO_DIR_OUTPUT,	GPIO_SET_OUTPUT_LOW,  PAD_WIFI_LED	   },
#endif
#ifdef  GPIO_MUTE_LED
{  GPIO_MUTE_LED,		GPIO_DIR_OUTPUT,	GPIO_SET_OUTPUT_LOW,  PAD_MUTE_LED	   },
#endif
#ifdef  GPIO_REC_LED
{  GPIO_REC_LED,		GPIO_DIR_OUTPUT,	GPIO_SET_OUTPUT_LOW,  PAD_REC_LED	   },
#endif

#ifdef GPIO_WIFI_SHDN
{  GPIO_WIFI_SHDN,		GPIO_DIR_OUTPUT,	GPIO_SET_OUTPUT_LOW,  PAD_WIFI_SHDN	   },
#endif
#ifdef ORDER_PLAT_IMX291_IMX322_RC500W
     {  GPIO_SENSOR2_FLIP,           GPIO_DIR_INPUT,     PAD_PULLUP,           PAD_SENSOR2_PLUG            },
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
#if defined( CUST_MENU_PARKING_MONITOR)||defined(CUST_MOTION_DETECT_WITH_GSENSOR)
extern UINT16 carMotionGthr;
short carMotionXgPre=0;
short carMotionYgPre=0;
short carMotionZgPre=0;

short carMotionXgCur=0;
short carMotionYgCur=0;
short carMotionZgCur=0;
#if defined(ORDER_GSENSOR_DA380)
BOOL getGsensorUnstableStatus(void) 
{
    unsigned char    tmp_data[6] = {0};
    UINT32   g_thr=0;	
    if (mir3da_register_read_continuously(NSA_REG_ACC_X_LSB, 6, tmp_data) != 0) {
        return FALSE;
    }
    
    carMotionXgCur= ((short)(tmp_data[1] << 8 | tmp_data[0]))>> 4;
    carMotionYgCur= ((short)(tmp_data[3] << 8 | tmp_data[2]))>> 4;
    carMotionZgCur= ((short)(tmp_data[5] << 8 | tmp_data[4]))>> 4;

  	if((abs(carMotionXgCur-carMotionXgPre)>=carMotionGthr)||(abs(carMotionYgCur-carMotionYgPre)>=carMotionGthr)||(abs(carMotionZgCur-carMotionZgPre)>=carMotionGthr))
  	{
   		carMotionXgPre=carMotionXgCur;
  		carMotionYgPre=carMotionYgCur;
   		carMotionZgPre=carMotionZgCur; 
  		 return TRUE;
  	}    
	else
	{
   		carMotionXgPre=carMotionXgCur;
   		carMotionYgPre=carMotionYgCur;
   		carMotionZgPre=carMotionZgCur; 
   		return FALSE;
	}  	 
}

#elif defined(ORDER_GSENSOR_RD07_DA380)
extern UINT16 carCrashGthr;
short carCrashXgPre=0;
short carCrashYgPre=0;
short carCrashZgPre=0;
short carCrashXgCur=0;
short carCrashYgCur=0;
short carCrashZgCur=0;

BOOL getParkingMonitorCarCrash() 
{
	if(gsensorVersion==GSENSOR_ID_RD07)
	{
		carCrashXgCur = (INT8)(GSensor_I2C_ReadReg(0x41)&0xFF);
		carCrashYgCur = (INT8)(GSensor_I2C_ReadReg(0x42)&0xFF);
		carCrashZgCur = (INT8)(GSensor_I2C_ReadReg(0x43)&0xFF);	

		
		if((abs(carCrashXgCur-carCrashXgPre)>=carCrashGthr)||(abs(carCrashYgCur-carCrashYgPre)>=carCrashGthr)||(abs(carCrashZgCur-carCrashZgPre)>=carCrashGthr))
		{
			carCrashXgPre=carCrashXgCur;
			carCrashYgPre=carCrashYgCur;
			carCrashZgPre=carCrashZgCur; 
			return TRUE;
		}	 
		else
		{
			carCrashXgPre=carCrashXgCur;
			carCrashYgPre=carCrashYgCur;
			carCrashZgPre=carCrashZgCur; 
			return FALSE;
		}
	}
	else
	{
		unsigned char	 tmp_data[6] = {0};
		UINT32	 g_thr=0;	
		if (mir3da_register_read_continuously(NSA_REG_ACC_X_LSB, 6, tmp_data) != 0) {
			return FALSE;
		}
		carCrashXgCur= ((short)(tmp_data[1] << 8 | tmp_data[0]))>> 4;
		carCrashYgCur= ((short)(tmp_data[3] << 8 | tmp_data[2]))>> 4;
		carCrashZgCur= ((short)(tmp_data[5] << 8 | tmp_data[4]))>> 4;
		if((abs(carCrashXgCur-carCrashXgPre)>=carCrashGthr)||(abs(carCrashYgCur-carCrashYgPre)>=carCrashGthr)||(abs(carCrashZgCur-carCrashZgPre)>=carCrashGthr))
		{
			carCrashXgPre=carCrashXgCur;
			carCrashYgPre=carCrashYgCur;
			carCrashZgPre=carCrashZgCur; 
			return TRUE;
		}	
		else
		{
			carCrashXgPre=carCrashXgCur;
			carCrashYgPre=carCrashYgCur;
			carCrashZgPre=carCrashZgCur; 
			return FALSE;
		}
	}
}

BOOL getGsensorUnstableStatus(void) 
{
	if(gsensorVersion==GSENSOR_ID_RD07)
	{
		carMotionXgCur = (INT8)(GSensor_I2C_ReadReg(0x41)&0xFF);
		carMotionYgCur = (INT8)(GSensor_I2C_ReadReg(0x42)&0xFF);
		carMotionZgCur = (INT8)(GSensor_I2C_ReadReg(0x43)&0xFF);	

		
  		if((abs(carMotionXgCur-carMotionXgPre)>=carMotionGthr)||(abs(carMotionYgCur-carMotionYgPre)>=carMotionGthr)||(abs(carMotionZgCur-carMotionZgPre)>=carMotionGthr))
  		{
   			carMotionXgPre=carMotionXgCur;
  			carMotionYgPre=carMotionYgCur;
   			carMotionZgPre=carMotionZgCur; 
  		 	return TRUE;
  		}    
		else
		{
   			carMotionXgPre=carMotionXgCur;
   			carMotionYgPre=carMotionYgCur;
   			carMotionZgPre=carMotionZgCur; 
   			return FALSE;
		}  	 
	}	
	else
	{
    		unsigned char    tmp_data[6] = {0};
    		UINT32   g_thr=0;	
    		if (mir3da_register_read_continuously(NSA_REG_ACC_X_LSB, 6, tmp_data) != 0) {
        		return FALSE;
    		}
    
    		carMotionXgCur= ((short)(tmp_data[1] << 8 | tmp_data[0]))>> 4;
    		carMotionYgCur= ((short)(tmp_data[3] << 8 | tmp_data[2]))>> 4;
    		carMotionZgCur= ((short)(tmp_data[5] << 8 | tmp_data[4]))>> 4;

  		if((abs(carMotionXgCur-carMotionXgPre)>=carMotionGthr)||(abs(carMotionYgCur-carMotionYgPre)>=carMotionGthr)||(abs(carMotionZgCur-carMotionZgPre)>=carMotionGthr))
  		{
   			carMotionXgPre=carMotionXgCur;
  			carMotionYgPre=carMotionYgCur;
   			carMotionZgPre=carMotionZgCur; 
  			return TRUE;
  		}    
		else
		{
   			carMotionXgPre=carMotionXgCur;
   			carMotionYgPre=carMotionYgCur;
   			carMotionZgPre=carMotionZgCur; 
   			return FALSE;
		}  	
	}
}

#else

BOOL getGsensorUnstableStatus(void) 
{
	debug_msg(("==getGsensorUnstableStatus==\n\r"));

	return GSensor_ParkingMode();

}

BOOL getParkingMonitorCarCrash() 
{
	
	debug_msg(("==getParkingMonitorCarCrash==\n\r"));
	return GSensor_CrashMode();

}

#endif	

#endif

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

    g_GyroCfgInfo.AxisSelec[0] = 1; //RSC_GYRO_AXIS_X;
    g_GyroCfgInfo.AxisSelec[1] = 0; //RSC_GYRO_AXIS_Y;
    g_GyroCfgInfo.AxisSelec[2] = 2; //RSC_GYRO_AXIS_Z;
    g_GyroCfgInfo.DirSelec[0] = 1; //RSC_DIR_POS
    g_GyroCfgInfo.DirSelec[1] = 0; //RSC_DIR_POS
    g_GyroCfgInfo.DirSelec[2] = 1; //RSC_DIR_POS
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



