/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       DetKey.c
    @ingroup    mIPRJAPKeyIO

    @brief      Scan key, modedial
                Scan key, modedial

    @note       Nothing.

    @date       2009/04/22
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "DxCfg.h"
#include "IOCfg.h"

#include "DxInput.h"
#include "Debug.h"
#include "KeyDef.h"
#include "rtc.h"
#include "HwPower.h"
#include "Delay.h"

#if (GESTURE_KEY == ENABLE)
#include "HwClock.h"
#include "i2c.h"
#include "epl89xx.h"
#include "egt3295_gesture.h"
#endif

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (GESTURE_KEY == ENABLE)
//definition
#define GES_INTEG EPL_GES_INTT_80
//local function
static I2C_STS epl_i2c_init(void);
static I2C_STS epl_i2c_read(UINT8 u8Reg,UINT8 *pu8Value);
static I2C_STS epl_i2c_write(UINT8 u8Reg,UINT8 u8Value);
static I2C_STS epl_i2c_read_length(UINT8 u8Reg,UINT8 *pu8Value,UINT8 u8bytecount);
static void    epl_sleep_change(int is_sleep);
static void    epl_notify_event(int evt);
static void    epl_read_ges_data(void);
//local variable
static I2C_SESSION m_epl_i2c_session = I2C_TOTAL_SESSION;
static int m_epl_event = 0;
static BOOL m_b_epl_init = 0;
static int m_epl_event_last = 0;
static UINT32 m_anti_debounce_time = 500*100; // 500ms
static UINT32 m_anti_debounce_start = 0;
#endif


////////////////////////////////////////////////////////////////////////////////
// ADC related
////////////////////////////////////////////////////////////////////////////////
// ADC related
//#NT#2009/11/16#Lincy -begin
#if (ADC_KEY == ENABLE)
#define VOLDET_KEY_ADC_RANGE         (110)
#define VOLDET_KEY_ADC_LVL0          (212-VOLDET_KEY_ADC_RANGE)
#define VOLDET_KEY_ADC_LVL1          (420-VOLDET_KEY_ADC_RANGE)
#define VOLDET_KEY_ADC_LVL2          (628-VOLDET_KEY_ADC_RANGE)
#define VOLDET_KEY_ADC_LVL3          (837-VOLDET_KEY_ADC_RANGE)
#define VOLDET_KEY_ADC_TH            (1023-VOLDET_KEY_ADC_RANGE)

#define VOLDET_KEY_LVL_UNKNOWN           0xFFFFFFFF
#define VOLDET_KEY_LVL_0                 0
#define VOLDET_KEY_LVL_1                 1
#define VOLDET_KEY_LVL_2                 2
#define VOLDET_KEY_LVL_3                 3
#define VOLDET_KEY_LVL_4                 4
#define VOLDET_KEY_LVL_5                 5
#endif

#define VOLDET_MS_ADC_RANGE         (60)
#define VOLDET_MS_ADC_LVL0          (0)
#define VOLDET_MS_ADC_LVL1          (262-VOLDET_MS_ADC_RANGE)
#define VOLDET_MS_ADC_LVL2          (525-VOLDET_MS_ADC_RANGE)
#define VOLDET_MS_ADC_LVL3          (788-VOLDET_MS_ADC_RANGE)
#define VOLDET_MS_ADC_TH            (788+VOLDET_MS_ADC_RANGE)

#define VOLDET_MS_LVL_UNKNOWN           0xFFFFFFFF
#define VOLDET_MS_LVL_0                 0
#define VOLDET_MS_LVL_1                 1
#define VOLDET_MS_LVL_2                 2
#define VOLDET_MS_LVL_3                 3
#define VOLDET_MS_LVL_4                 4
#define VOLDET_MS_LVL_5                 5

#if (ADC_KEY == ENABLE)
#if 0
static UINT32 VolDet_GetModeSwitch1ADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
    UINT32 uiADCValue;

    uiADCValue = adc_readData(ADC_CH_VOLDET_MS1);
    // One-Shot Mode, trigger one-shot
    adc_triggerOneShot(ADC_CH_VOLDET_MS1);

    return uiADCValue;
#else
    return adc_readData(ADC_CH_VOLDET_MS1);
#endif
}

static UINT32 VolDet_GetModeSwitch2ADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
    UINT32 uiADCValue;

    uiADCValue = adc_readData(ADC_CH_VOLDET_MS2);
    // One-Shot Mode, trigger one-shot
    adc_triggerOneShot(ADC_CH_VOLDET_MS2);

    return uiADCValue;
#else
    return adc_readData(ADC_CH_VOLDET_MS2);
#endif
}
#endif
static UINT32 VolDet_GetKey1ADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
    UINT32 uiADCValue;

    uiADCValue = adc_readData(ADC_CH_VOLDET_KEY1);
    // One-Shot Mode, trigger one-shot
    adc_triggerOneShot(ADC_CH_VOLDET_KEY1);

    return uiADCValue;
#else
    return adc_readData(ADC_CH_VOLDET_KEY1);
#endif
}
#if 0
/**
  Get Mode Switch 1 voltage level

  Get Mode Switch 1 voltage level.

  @param void
  @return UINT32 Mode Switch 1, refer to VoltageDet.h -> VOLDET_MS_LVL_XXXX
*/
static UINT32 VolDet_GetModeSwitch1Level(void)
{
    //static UINT32   uiPreMS1Lvl = VOLDET_MS_LVL_UNKNOWN;
    static UINT32   uiRetMS1Lvl;
    UINT32          uiMS1ADC, uiCurMS1Lvl;

    uiMS1ADC = VolDet_GetModeSwitch1ADC();

    if (uiMS1ADC < VOLDET_MS_ADC_TH) //768+30
    {
        if (uiMS1ADC > VOLDET_MS_ADC_LVL3) //768-30
        {
            uiCurMS1Lvl = VOLDET_MS_LVL_3;
        }
        else if (uiMS1ADC > VOLDET_MS_ADC_LVL2)//512-30
        {
            uiCurMS1Lvl = VOLDET_MS_LVL_2;
        }
        else if (uiMS1ADC > VOLDET_MS_ADC_LVL1)//256-30
        {
            uiCurMS1Lvl = VOLDET_MS_LVL_1;
        }
        else if (uiMS1ADC < VOLDET_MS_ADC_TH)// <30
        {
            uiCurMS1Lvl = VOLDET_MS_LVL_0;
        }
        else
        {
            uiCurMS1Lvl = VOLDET_MS_LVL_UNKNOWN;
        }
    }
    else
    {
        uiCurMS1Lvl = VOLDET_MS_LVL_UNKNOWN;
    }

    uiRetMS1Lvl = uiCurMS1Lvl;

    return uiRetMS1Lvl;
}

/**
  Get Mode Switch 2 voltage level

  Get Mode Switch 2 voltage level.

  @param void
  @return UINT32 Mode Switch 2, refer to VoltageDet.h -> VOLDET_MS_LVL_XXXX
*/
static UINT32 VolDet_GetModeSwitch2Level(void)
{
    static UINT32   uiRetMS2Lvl;
    UINT32          uiMS2ADC, uiCurMS2Lvl;

    uiMS2ADC = VolDet_GetModeSwitch2ADC();

    if (uiMS2ADC < VOLDET_MS_ADC_TH) //768+30
    {
        if (uiMS2ADC > VOLDET_MS_ADC_LVL3) //768-30
        {
            uiCurMS2Lvl = VOLDET_MS_LVL_3;
        }
        else if (uiMS2ADC > VOLDET_MS_ADC_LVL2) //512-30
        {
            uiCurMS2Lvl = VOLDET_MS_LVL_2;
        }
        else if (uiMS2ADC > VOLDET_MS_ADC_LVL1) //256-30
        {
            uiCurMS2Lvl = VOLDET_MS_LVL_1;
        }
        else if (uiMS2ADC < VOLDET_MS_ADC_TH) //<30
        {
            uiCurMS2Lvl = VOLDET_MS_LVL_0;
        }
        else
        {
            uiCurMS2Lvl = VOLDET_MS_LVL_UNKNOWN;
        }
    }
    else
    {
        uiCurMS2Lvl = VOLDET_MS_LVL_UNKNOWN;
    }

    uiRetMS2Lvl = uiCurMS2Lvl;

    return uiRetMS2Lvl;
}
#endif
/**
  Get ADC key voltage level

  Get  ADC key  2 voltage level.

  @param void
  @return UINT32 key level, refer to VoltageDet.h -> VOLDET_MS_LVL_XXXX
*/
static UINT32 VolDet_GetKey1Level(void)
{
    static UINT32   uiRetKey1Lvl;
    UINT32          uiKey1ADC, uiCurKey2Lvl;

    uiKey1ADC = VolDet_GetKey1ADC();
    DBG_IND("uiKey1ADC %d \r\n", uiKey1ADC);
    if (uiKey1ADC < VOLDET_KEY_ADC_TH) //768+30
    {
        if (uiKey1ADC > VOLDET_KEY_ADC_LVL3) //768-30
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_4;
        }
        else if (uiKey1ADC > VOLDET_KEY_ADC_LVL2) //512-30
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_3;
        }
        else if (uiKey1ADC > VOLDET_KEY_ADC_LVL1) //256-30
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_2;
        }
        else if (uiKey1ADC > VOLDET_KEY_ADC_LVL0) //<30
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_1;
        }
        else
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_0;
        }
    }
    else
    {
        uiCurKey2Lvl = VOLDET_KEY_LVL_UNKNOWN;
    }

    uiRetKey1Lvl = uiCurKey2Lvl;

    return uiRetKey1Lvl;
}

/**
  Detect Mode Switch state.

  Detect Mode Switch state.

  @param void
  @return UINT32 Mode Switch state (DSC Mode)
*/
#if 0
static UINT32 VolDet_ModeSwitch(void)
{
    UINT32 uiModeSwitchCode = 0;
    UINT32 uiMS1Level, uiMS2Level;

    uiMS1Level = VolDet_GetModeSwitch1Level();
    uiMS2Level = VolDet_GetModeSwitch2Level();

    DBG_IND("uiMS1Level = %d, uiMS2Level = %d\r\n",uiMS1Level,uiMS2Level);

    if(uiMS1Level != VOLDET_MS_LVL_UNKNOWN)
    {
        if(uiMS1Level == VOLDET_MS_LVL_0)
        {
            uiModeSwitchCode = STATUS_KEY_LVL_7;//VOLDET_MS_MOVIE;
        }
        else if(uiMS1Level == VOLDET_MS_LVL_1)
        {
            uiModeSwitchCode = STATUS_KEY_LVL_8;//VOLDET_MS_ASCENE;
        }
        else if(uiMS1Level == VOLDET_MS_LVL_2)
        {
            uiModeSwitchCode = STATUS_KEY_LVL_1;//VOLDET_MS_PHOTO_AUTO;
        }
        else if(uiMS1Level == VOLDET_MS_LVL_3)
        {
            uiModeSwitchCode =  STATUS_KEY_LVL_2;//VOLDET_MS_PHOTO_MANUAL;
        }
    }
    else if(uiMS2Level != VOLDET_MS_LVL_UNKNOWN)
    {
        if(uiMS2Level == VOLDET_MS_LVL_0)
        {
            uiModeSwitchCode = STATUS_KEY_LVL_3;//VOLDET_MS_PHOTO_PORTRAIT;
        }
        else if(uiMS2Level == VOLDET_MS_LVL_1)
        {
            uiModeSwitchCode = STATUS_KEY_LVL_4;//VOLDET_MS_PHOTO_PANORAMA;
        }
        else if(uiMS2Level == VOLDET_MS_LVL_2)
        {
            uiModeSwitchCode = STATUS_KEY_LVL_5;//VOLDET_MS_PHOTO_SCENE;
        }
        else if(uiMS2Level == VOLDET_MS_LVL_3)
        {
            uiModeSwitchCode = STATUS_KEY_LVL_6;//VOLDET_MS_PHOTO_STAB;
        }

    }
    else
    {
        uiModeSwitchCode = STATUS_KEY_LVL_UNKNOWN;//VOLDET_MS_PHOTO_AUTO;
    }
    DBG_IND("uiModeSwitchCode = %d\r\n",uiModeSwitchCode);

    return uiModeSwitchCode;
}
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
// GPIO related

//static BOOL g_bIsShutter2Pressed = FALSE;



/**
  Delay between toggle GPIO pin of input/output

  Delay between toggle GPIO pin of input/output

  @param void
  @return void
*/
static void DrvKey_DetKeyDelay(void)
{
    gpio_readData(0);
    gpio_readData(0);
    gpio_readData(0);
    gpio_readData(0);
}

void DrvKey_Init(void)
{
#if (GESTURE_KEY == ENABLE)
    epl_i2c_init();
#endif
}
#if( _UI_STYLE_ == _UI_STYLE_CARDV_)
/**
  Detect normal key is pressed or not.

  Detect normal key is pressed or not.
  Return key pressed status (refer to KeyDef.h)

  @param void
  @return UINT32
*/
UINT32 DrvKey_DetNormalKey(void)
{
    UINT32 uiKeyCode = 0;
/*
      UP
S2         OK
     DOWN
*/
#if (ADC_KEY == ENABLE)
    UINT32 uiKey1Lvl = VolDet_GetKey1Level();
    switch(uiKey1Lvl)
    {
        case VOLDET_KEY_LVL_UNKNOWN:
        default:
            break;
        case VOLDET_KEY_LVL_0:
            //uiKeyCode |= FLGKEY_ENTER;
            break;
        case VOLDET_KEY_LVL_1:
            uiKeyCode |= FLGKEY_UP;
            break;
        case VOLDET_KEY_LVL_2:
            uiKeyCode |= FLGKEY_SHUTTER2;
            break;
        case VOLDET_KEY_LVL_3:
            uiKeyCode |= FLGKEY_DOWN;
            break;
        case VOLDET_KEY_LVL_4:
            uiKeyCode |= FLGKEY_ENTER;
            break;
    }
#endif

    if(HwPower_GetPowerKey(POWER_ID_PSW1))
    {
        uiKeyCode |= FLGKEY_MODE;
        // Reset shutdown timer
        HwPower_SetPowerKey(POWER_ID_PSW1, 0xf0);
    }

    DBG_IND("KEY=%08x\r\n", uiKeyCode);

    DrvKey_DetKeyDelay();
    return uiKeyCode;
}
/**
  Detect power key is pressed or not.

  Detect power key is pressed or not.
  Return key pressed status (refer to KeyDef.h)

  @param void
  @return UINT32
*/
#define POWEROFF_COUNT (2000/100) // wait 2000ms
UINT32 DrvKey_DetPowerKey(void)
{
    static UINT32 ShutDownCount = 0;
    UINT32 uiKeyCode = 0;

    if(HwPower_GetPowerKey(POWER_ID_PSW1))
    {
        ShutDownCount++;
        // Reset shutdown timer
        HwPower_SetPowerKey(POWER_ID_PSW1, 0xf0);
        if(ShutDownCount > 20)
        {
            uiKeyCode = FLGKEY_KEY_POWER;
        }
    }
    else
    {
        ShutDownCount = 0;
    }
    return uiKeyCode;
}
#else
/**
  Detect normal key is pressed or not.

  Detect normal key is pressed or not.
  Return key pressed status (refer to KeyDef.h)

  @param void
  @return UINT32
*/
UINT32 DrvKey_DetNormalKey(void)
{
    UINT32 uiKeyCode = 0;
//CHKPNT;
#if (ADC_KEY == ENABLE)
    UINT32 uiKey1Lvl = VolDet_GetKey1Level();
    switch(uiKey1Lvl)
    {
        case VOLDET_KEY_LVL_UNKNOWN:
        default:
            break;
        case VOLDET_KEY_LVL_0:
            uiKeyCode |= FLGKEY_ENTER;
            break;
        case VOLDET_KEY_LVL_1:
            uiKeyCode |= FLGKEY_UP;
            break;
        case VOLDET_KEY_LVL_2:
        #if (_KEY_METHOD_ == _KEY_METHOD_2KEY_)
            uiKeyCode |= FLGKEY_LEFT;
        #else
            uiKeyCode |= FLGKEY_SHUTTER2;
        #endif
            break;
        case VOLDET_KEY_LVL_3:
            uiKeyCode |= FLGKEY_DOWN;
            break;
        case VOLDET_KEY_LVL_4:
            uiKeyCode |= FLGKEY_RIGHT;
            break;
    }
#endif


#if (GPIO_KEY == ENABLE)
    if(!gpio_getPin(GPIO_KEY_MENU))
    {
        //uiKeyCode |= FLGKEY_MENU;
        uiKeyCode |= FLGKEY_SHUTTER2;//for debug
    }
    if(!gpio_getPin(GPIO_KEY_UP))
    {
        uiKeyCode |= FLGKEY_UP;
    }
    if(!gpio_getPin(GPIO_KEY_DOWN))
    {
        uiKeyCode |= FLGKEY_DOWN;
    }

    if(!gpio_getPin(GPIO_KEY_RIGHT))
    {
        uiKeyCode |= FLGKEY_RIGHT;
    }
    if(!gpio_getPin(GPIO_KEY_LEFT))
    {
        uiKeyCode |= FLGKEY_LEFT;
    }
#if 0
    if(!gpio_getPin(GPIO_KEY_SHUTTER1))
    {
        uiKeyCode |= FLGKEY_SHUTTER1;
    }
    if(!gpio_getPin(GPIO_KEY_SHUTTER2))
    {
        uiKeyCode |= FLGKEY_SHUTTER2;
    }
    if(!gpio_getPin(GPIO_KEY_ZOOMIN))
    {
        uiKeyCode |= FLGKEY_ZOOMIN;
    }
    if(!gpio_getPin(GPIO_KEY_ZOOMOUT))
    {
        uiKeyCode |= FLGKEY_ZOOMOUT;
    }
    if(!gpio_getPin(GPIO_KEY_ENTER))
    {
        uiKeyCode |= FLGKEY_ENTER;
    }
    if(!gpio_getPin(GPIO_KEY_MODE))
    {
        uiKeyCode |= FLGKEY_MODE;
    }
    if(!gpio_getPin(GPIO_KEY_PLAYBACK))
    {
        uiKeyCode |= FLGKEY_PLAYBACK;
    }
#endif
#endif
/*
    //detect if power-on by press playback key
    if (!HwPower_GetPowerKey(POWER_ID_PSW2))
    {
        uiKeyCode |= FLGKEY_PLAYBACK;
    }
*/
#if (GESTURE_KEY == ENABLE)

#if (GESTURE_MODE == GESTURE_MODE_POLLING)
    epl_read_ges_data();
#endif

    if(m_epl_event == 0
        && m_epl_event_last // no new key
        && (HwClock_GetCounter() - m_anti_debounce_start) > m_anti_debounce_time)
    {
        m_epl_event_last = 0; //reset key
    }
    else if(m_epl_event !=0)
    {
        m_epl_event_last = m_epl_event;
        m_anti_debounce_start = HwClock_GetCounter();
    }
    switch(m_epl_event_last)
    {
        case 2:
            uiKeyCode |= FLGKEY_UP;
            break;
        case 3:
            uiKeyCode |= FLGKEY_DOWN;
            break;
        case 4:
            uiKeyCode |= FLGKEY_SHUTTER2;
            break;
        case 5:
            uiKeyCode |= FLGKEY_RIGHT;
            break;
    }
    m_epl_event = 0; //clean
#endif
    //DBG_ERR("KEY=%08x\r\n", uiKeyCode);

    DrvKey_DetKeyDelay();
    return uiKeyCode;
}
/**
  Detect power key is pressed or not.

  Detect power key is pressed or not.
  Return key pressed status (refer to KeyDef.h)

  @param void
  @return UINT32
*/
UINT32 DrvKey_DetPowerKey(void)
{
    UINT32 uiKeyCode = 0;

    if(HwPower_GetPowerKey(POWER_ID_PSW1))
    {
        uiKeyCode = FLGKEY_KEY_POWER;
        // Reset shutdown timer
        HwPower_SetPowerKey(POWER_ID_PSW1, 0xf0);
    }
    return uiKeyCode;
}
#endif
UINT32 DrvKey_DetStatusKey(DX_STATUS_KEY_GROUP KeyGroup)
{
    UINT32 uiReturn = STATUS_KEY_LVL_UNKNOWN;
    switch (KeyGroup)
    {
        case DX_STATUS_KEY_GROUP1:
#if 0//(ADC_KEY == ENABLE)
            uiReturn = VolDet_ModeSwitch();
#endif
            break;

        case DX_STATUS_KEY_GROUP2:
            break;

        case DX_STATUS_KEY_GROUP3:
            break;

        case DX_STATUS_KEY_GROUP4:
            break;

        case DX_STATUS_KEY_GROUP5:
            break;

        default:
            DBG_ERR("[StatusKey]no this attribute");
            break;
    }
    return uiReturn;
}


#if (GESTURE_KEY == ENABLE)

#if (GESTURE_MODE == GESTURE_MODE_INTERRUPT)
static void epl_isr(UINT32 uiEvent)
{
    //here need to trigger. don't direct to invoke epl_read_ges_data
    //or cause many losing event.
	//epl_read_ges_data();
}
#endif

static I2C_STS epl_i2c_init(void)
{
    I2C_STS ret;
 	UINT8 u8DataAry[2];

    i2c_open(&m_epl_i2c_session);
    ret = i2c_isOpened(m_epl_i2c_session);
    DBG_IND("is open = 0x%x\r\n",ret);


    i2c_setConfig(m_epl_i2c_session, I2C_CONFIG_ID_MODE, I2C_MODE_MASTER);
    i2c_setConfig(m_epl_i2c_session, I2C_CONFIG_ID_BUSCLOCK, I2C_BUS_CLOCK_400KHZ);
    i2c_setConfig(m_epl_i2c_session, I2C_CONFIG_ID_PINMUX, I2C_PINMUX_1ST); //select to control I2C 1st device

    i2c_lock(m_epl_i2c_session);
    ret = i2c_islocked(m_epl_i2c_session);
    DBG_IND("is lock = 0x%x\r\n",ret);

    epl_i2c_read(0x00,&u8DataAry[0]);
    epl_i2c_read(0x01,&u8DataAry[1]);
    DBG_IND("reg00: 0x0%x, reg01: 0x0%x\r\n", u8DataAry[0], u8DataAry[1]);

    set_gesture_mode(GES_MODE_4D_HOLD);//GES_MODE_DISABLE);

    //disable sensor
    epl_i2c_write(0x00, EPL_MODE_IDLE);
    epl_i2c_write(0x11, EPL_POWER_OFF | EPL_RESETN_RESET);

    //enable sensor
    set_gesture_callback(epl_sleep_change, epl_notify_event);
	set_gesture_mode(GES_MODE_4D_HOLD);

#if (GESTURE_MODE == GESTURE_MODE_POLLING)
    DBG_IND("Polling Mode\r\n");
    epl_i2c_write(0x00, EPL_WAIT_0_MS | EPL_MODE_GES_SS);
#elif (GESTURE_MODE == GESTURE_MODE_INTERRUPT)
     DBG_IND("Interrupt Mode\r\n");
     epl_i2c_write(0x00, EPL_WAIT_50_MS | EPL_MODE_GES_SS);
#endif

#if (GESTURE_CHIP == GESTURE_CHIP_EPL2219)
    DBG_IND("EPL2219\r\n");
    epl_i2c_write(0x24, EPL_GES_ADC_11 | GES_INTEG | EPL_GAIN_LOW);  // 2219
#elif (GESTURE_CHIP == GESTURE_CHIP_EPL8975)
    DBG_IND("EPL8975\r\n");
    epl_i2c_write(0x24, EPL_GES_ADC_11 | EPL_GES_INTT_60 | EPL_GAIN_LOW);  // 8975
#endif

#if (GESTURE_MODE == GESTURE_MODE_POLLING)
    epl_i2c_write(0x25, GES_INT_DISABLE | EPL_INDEX_8 | EPL_IR_MODE_CURRENT | EPL_IR_BOOST_300 | EPL_IR_DRIVE_100);
#elif (GESTURE_MODE == GESTURE_MODE_INTERRUPT)
    epl_i2c_write(0x25, GES_INT_ENABLE | EPL_INDEX_8 | EPL_IR_MODE_CURRENT | EPL_IR_BOOST_300 | EPL_IR_DRIVE_100);
#endif

    epl_i2c_write(0x11, EPL_POWER_ON | EPL_RESETN_RESET);
    epl_i2c_write(0x11, EPL_POWER_ON | EPL_RESETN_RUN);

    i2c_unlock(m_epl_i2c_session);

#if (GESTURE_MODE == GESTURE_MODE_INTERRUPT)
    ret = gpio_isOpened();
    DBG_IND("GPIO is open = 0x%x\r\n",ret);

    gpio_clearIntStatus(GPIO_INT_16);
    ret = gpio_getIntStatus(GPIO_INT_16);
    DBG_IND("GPIO INT  = 0x%x\r\n",ret);
    gpio_setDir(P_GPIO_33,GPIO_DIR_INPUT);

    gpio_setIntTypePol(GPIO_INT_16, GPIO_INTTYPE_EDGE, GPIO_INTPOL_NEGLOW);
    gpio_setIntIsr(GPIO_INT_16, epl_isr);
    gpio_enableInt(GPIO_INT_16);
#endif

    m_b_epl_init = 1;

    return ret;
}

static I2C_STS epl_i2c_read(UINT8 u8Reg,UINT8 *pu8Value)
{

    I2C_DATA i2cData;
    I2C_BYTE I2cByte;
    I2C_STS ret;


    i2cData.VersionInfo= DRV_VER_96660;
    i2cData.pByte = &I2cByte;
    i2cData.ByteCount = I2C_BYTE_CNT_1;


    I2cByte.uiValue = 0x82;
    I2cByte.Param   = I2C_BYTE_PARAM_START;
    ret = i2c_transmit(&i2cData);
    if ( ret != I2C_STS_OK )
    {
        DBG_ERR("i2c ret = %02x!!\r\n", ret);
    }


    I2cByte.uiValue = u8Reg;
    I2cByte.Param   = I2C_BYTE_PARAM_STOP;
    ret = i2c_transmit(&i2cData);
    if ( ret != I2C_STS_OK )
    {
        DBG_ERR("i2c ret = %02x!!\r\n", ret);
    }

    I2cByte.uiValue = 0x83;
    I2cByte.Param   = I2C_BYTE_PARAM_START;
    ret = i2c_transmit(&i2cData);
    if ( ret != I2C_STS_OK )
    {
        DBG_ERR("i2c ret = %02x!!\r\n", ret);
    }

    I2cByte.Param   = I2C_BYTE_PARAM_STOP|I2C_BYTE_PARAM_NACK;
    ret = i2c_receive(&i2cData);
    if ( ret != I2C_STS_OK )
    {
        DBG_ERR("i2c ret = %02x!!\r\n", ret);
    }


    //DBG_IND("read vaule=0x%x\r\n",I2cByte.uiValue);
    *pu8Value = I2cByte.uiValue;
    return ret;


}
static I2C_STS epl_i2c_write(UINT8 u8Reg,UINT8 u8Value)
{

    I2C_DATA i2cData;
    I2C_BYTE I2cByte;
    I2C_STS ret;

    i2cData.VersionInfo= DRV_VER_96660;
    i2cData.pByte = &I2cByte;
    i2cData.ByteCount = I2C_BYTE_CNT_1;

    I2cByte.uiValue = 0x82;
    I2cByte.Param   = I2C_BYTE_PARAM_START;
    ret = i2c_transmit(&i2cData);
    if ( ret != I2C_STS_OK )
    {
        DBG_ERR("i2c ret = %02x!!\r\n", ret);
    }


    I2cByte.uiValue = u8Reg;
    I2cByte.Param   = I2C_BYTE_PARAM_NONE;
    ret = i2c_transmit(&i2cData);
    if ( ret != I2C_STS_OK )
    {
        DBG_ERR("i2c ret = %02x!!\r\n", ret);
    }

    I2cByte.uiValue = u8Value;
    I2cByte.Param   = I2C_BYTE_PARAM_STOP;
    ret = i2c_transmit(&i2cData);
    if ( ret != I2C_STS_OK )
    {
        DBG_ERR("i2c ret = %02x!!\r\n", ret);
    }


    return ret;
}

static I2C_STS epl_i2c_read_length(UINT8 u8Reg,UINT8 *pu8Value,UINT8 u8bytecount)
{

    I2C_DATA i2cData;
    I2C_BYTE I2cByte;
    I2C_STS ret;
    UINT8 cnt=0;


    i2cData.VersionInfo= DRV_VER_96660;
    i2cData.pByte = &I2cByte;
    i2cData.ByteCount = I2C_BYTE_CNT_1;


    if ( u8bytecount == 0 )
    {
        DBG_ERR("u8bytecount==0!!\r\n");
    }

    I2cByte.uiValue = 0x82;
    I2cByte.Param   = I2C_BYTE_PARAM_START;
    ret = i2c_transmit(&i2cData);
    if ( ret != I2C_STS_OK )
    {
        DBG_ERR("i2c ret = %02x!!\r\n", ret);
    }


    I2cByte.uiValue = u8Reg;
    I2cByte.Param   = I2C_BYTE_PARAM_STOP;
    ret = i2c_transmit(&i2cData);
    if ( ret != I2C_STS_OK )
    {
        DBG_ERR("i2c ret = %02x!!\r\n", ret);
    }

    I2cByte.uiValue = 0x83;
    I2cByte.Param   = I2C_BYTE_PARAM_START;
    ret = i2c_transmit(&i2cData);
    if ( ret != I2C_STS_OK )
    {
        DBG_ERR("i2c ret = %02x!!\r\n", ret);
    }

    for(cnt=0; cnt<(u8bytecount-1); cnt++)
    {
      //DBG_IND("cnt = %d\r\n",cnt);
      I2cByte.Param   = I2C_BYTE_PARAM_ACK;
      ret = i2c_receive(&i2cData);
      if ( ret != I2C_STS_OK )
      {
        DBG_ERR("i2c ret = %02x!!\r\n", ret);
      }
      //DBG_IND("read vaule=0x%x\r\n",I2cByte.uiValue);

      *(pu8Value+cnt) = I2cByte.uiValue;
    }

    //DBG_IND("cnt = %d\r\n",cnt);
    I2cByte.Param   = I2C_BYTE_PARAM_STOP|I2C_BYTE_PARAM_NACK;
    ret = i2c_receive(&i2cData);
    if ( ret != I2C_STS_OK )
    {
      DBG_ERR("i2c ret = %02x!!\r\n", ret);
    }
    *(pu8Value+cnt) = I2cByte.uiValue;
    //DBG_IND("read vaule=0x%x\r\n",I2cByte.uiValue);
    //*pu8Value = I2cByte.uiValue;
    return ret;


}


void epl_sleep_change(int is_sleep)
{
    i2c_lock(m_epl_i2c_session);

    if(is_sleep)
    {
        epl_i2c_write(0x00, EPL_WAIT_2_MS);
        epl_i2c_write(0x00, EPL_WAIT_2_MS| EPL_MODE_GES_SS);
    }
    else
    {
        epl_i2c_write(0x00, EPL_WAIT_0_MS);
        epl_i2c_write(0x00, EPL_WAIT_0_MS | EPL_MODE_GES_SS);
    }

    i2c_unlock(m_epl_i2c_session);

    DBG_IND("is_sleep is %d\r\n", is_sleep);
}

void epl_notify_event(int evt)
{
    m_epl_event = evt;
    DBG_DUMP("event is %d\r\n", evt);
}


void epl_read_ges_data(void)
{
    int i, j, k;
    int index = 8, width = 6, shift = 2, f = 2;
    UINT8  buffer[48] = {0};
    UINT16 ges_buffer[8][4] = {0}, ges_data[4] = {0};
    UINT8  u8Reg;
    u8Reg = 0x27;

    if(m_b_epl_init==0)
        return;

    i2c_lock(m_epl_i2c_session);
    epl_i2c_read_length(u8Reg, buffer, 48);
    i2c_unlock(m_epl_i2c_session);
    //DBG_FUNC_END("\r\n");

    for(i = 0 ; i < 8 ; i++)
    {
        j = 6 * i;
        ges_buffer[i][0] = ((buffer[0+j] & 0xff) << 0) | ((buffer[1+j] & 0x0f) << 8);
        ges_buffer[i][1] = ((buffer[2+j] & 0xff) << 4) | ((buffer[1+j] & 0xf0) >> 4);
        ges_buffer[i][2] = ((buffer[3+j] & 0xff) << 0) | ((buffer[4+j] & 0x0f) << 8);
        ges_buffer[i][3] = ((buffer[5+j] & 0xff) << 4) | ((buffer[4+j] & 0xf0) >> 4);
    }


    // check the value is related to object distance
    //DBG_IND("%4d %4d %4d %4d\r\n", ges_buffer[0][0], ges_buffer[0][1], ges_buffer[0][2], ges_buffer[0][3]);
	/*
	if(ges_buffer[0][0]>1000)
		GPIO_CLR_PIN(PA13);
	else
		GPIO_SET_PIN(PA13);
	*/

    for (i = 0; i < index; i += shift)
    {
        memset(ges_data, 0, sizeof(unsigned short) << 2);

        for (k = 0; k < width; k++)
        {
            if (i + k < index)
            {
                for (j = 0; j < 4; j++)
                    ges_data[j] += ges_buffer[i + k][j];
            }
            if (i + k >= index || k == width - 1)
            {
                for (j = 0; j < 4; j++)
                    ges_data[j] = ges_data[j] >> f;

                add_gesture_data(ges_data);
               //detect_gesture_event();
                break;
            }
        }

        if (i + width >= index)
            break;
    }
}
#endif
