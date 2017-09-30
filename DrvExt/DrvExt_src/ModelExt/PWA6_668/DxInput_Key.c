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
#include "GSensor.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// ADC related
////////////////////////////////////////////////////////////////////////////////
// ADC related
//#NT#2009/11/16#Lincy -begin
#if (ADC_KEY == ENABLE)
#define VOLDET_KEY_ADC_RANGE        (20)
#define VOLDET_KEY_ADC_LVL0         (0)
#define VOLDET_KEY_ADC_LVL1         (220-VOLDET_KEY_ADC_RANGE)
#define VOLDET_KEY_ADC_LVL2         (402-VOLDET_KEY_ADC_RANGE)
#define VOLDET_KEY_ADC_LVL3         (595-VOLDET_KEY_ADC_RANGE)
#define VOLDET_KEY_ADC_LVL4         (700-VOLDET_KEY_ADC_RANGE)
#define VOLDET_KEY_ADC_TH           (900+VOLDET_KEY_ADC_RANGE)

#define VOLDET_KEY_LVL_UNKNOWN      0xFFFFFFFF
#define VOLDET_KEY_LVL_MENU         10
#define VOLDET_KEY_LVL_REC          1
#define VOLDET_KEY_LVL_UP           2
#define VOLDET_KEY_LVL_DOWN         3
#define VOLDET_KEY_LVL_MODE         4
#define VOLDET_KEY_LVL_SHUTTER2     5

#endif

#if (ADC_KEY == ENABLE)
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
static UINT32 VolDet_GetKey2ADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
    UINT32 uiADCValue;

    uiADCValue = adc_readData(ADC_CH_VOLDET_KEY2);
    // One-Shot Mode, trigger one-shot
    adc_triggerOneShot(ADC_CH_VOLDET_KEY2);

    return uiADCValue;
#else
    return adc_readData(ADC_CH_VOLDET_KEY2);
#endif
}
#endif
/**
  Get ADC key voltage level

  Get  ADC key  2 voltage level.

  @param void
  @return UINT32 key level, refer to VoltageDet.h -> VOLDET_MS_LVL_XXXX
*/

/////////////////////////////////////////////////////////////////////////////
#define KEYSCAN_MENUKEY_INIT_STATE       0
#define KEYSCAN_MENUKEY_RELEASE_STATE    1
#define KEYSCAN_MENUKEY_PRESS_STATE      2

#define KEYSCAN_MENUKEY_UNKNOWN          0xFFFF
#define KEYSCAN_MENUKEY_RELEASED         0
#define KEYSCAN_MENUKEY_PRESSED          1

#define KEYSCAN_MENUKEY_COUNT  20 //20ms is 1

static UINT32 VolDet_GetKey1Level(void)
{
    static UINT32   uiRetKey1Lvl;
    UINT32          uiKey1ADC, uiCurKey2Lvl = 0xFFFF;

    static UINT32 uiMenuKey = KEYSCAN_MENUKEY_UNKNOWN;
    static UINT32 uiMenuKeyState    = KEYSCAN_MENUKEY_INIT_STATE;
    static UINT32 uiMenuKeyPressCount = 0;
    static UINT32 uiMenuKeyCount = 0;
    static UINT32 uiMenuKeyCount2 = 0;

    uiKey1ADC = VolDet_GetKey1ADC();
    //debug_msg("uiKey1ADC %d \r\n", uiKey1ADC);
    if (uiKey1ADC < VOLDET_KEY_ADC_TH)
    {
        if ((uiKey1ADC >= VOLDET_KEY_ADC_LVL3) && (uiKey1ADC <= VOLDET_KEY_ADC_LVL4))
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_DOWN;
        }
        else if ((uiKey1ADC >= VOLDET_KEY_ADC_LVL2) && (uiKey1ADC <= VOLDET_KEY_ADC_LVL3))
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_UP;
        }
        else if ((uiKey1ADC >= VOLDET_KEY_ADC_LVL1) && (uiKey1ADC <= VOLDET_KEY_ADC_LVL2))
        {
           //uiCurKey2Lvl = VOLDET_KEY_LVL_MENU;
        }
        // coverity[unsigned_compare]: Reserved for customed HW setting.
        else if ((uiKey1ADC >= VOLDET_KEY_ADC_LVL0) && (uiKey1ADC <= VOLDET_KEY_ADC_LVL1))
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_SHUTTER2 ;
        }
        else
        {
            uiCurKey2Lvl = VOLDET_KEY_LVL_UNKNOWN;
        }
    }
    else
    {
        uiCurKey2Lvl = VOLDET_KEY_LVL_UNKNOWN;
    }

    #if 1
    {
        if ((uiKey1ADC >= VOLDET_KEY_ADC_LVL1) && (uiKey1ADC <= VOLDET_KEY_ADC_LVL2))
        {
            // Debounce
            if (uiMenuKey == KEYSCAN_MENUKEY_PRESSED)
            {
                if (uiMenuKeyState == KEYSCAN_MENUKEY_RELEASE_STATE)
                {
                    uiMenuKeyPressCount++;
                    //debug_msg(("uiMenuKeyPressCount : %d....%d\r\n",uiMenuKeyPressCount,KEYSCAN_MENUKEY_COUNT));
                    if (uiMenuKeyPressCount > KEYSCAN_MENUKEY_COUNT)
                    {
                        uiMenuKeyState = KEYSCAN_MENUKEY_PRESS_STATE;
                    }
                }

                if(uiMenuKeyState == KEYSCAN_MENUKEY_PRESS_STATE)
                {
                    if(uiMenuKeyCount == 2)
                    {
                        uiMenuKeyPressCount = 0;
                        uiMenuKey = KEYSCAN_MENUKEY_UNKNOWN;
                        uiMenuKeyState = KEYSCAN_MENUKEY_INIT_STATE;
                        uiMenuKeyCount = 0;
                    }
                    uiMenuKeyCount++;

                    uiCurKey2Lvl = VOLDET_KEY_LVL_MODE;
                    //debug_msg("uiCurKey2Lvl = %d VOLDET_KEY_LVL_MODE\r\n",uiCurKey2Lvl);
                }
            }
            else
            {
                uiMenuKey = KEYSCAN_MENUKEY_PRESSED;
            }
        }
        else
        {
            // Debounce
            if (uiMenuKey == KEYSCAN_MENUKEY_RELEASED)
            {
                if (uiMenuKeyState == KEYSCAN_MENUKEY_INIT_STATE)
                {
                    uiMenuKeyState = KEYSCAN_MENUKEY_RELEASE_STATE;
                }

                if (uiMenuKeyPressCount)
                {
                    if(uiMenuKeyCount2 == 2)
                    {
                        uiMenuKeyPressCount = 0;
                        uiMenuKeyCount2 = 0;
                    }
                    uiMenuKeyCount2++;

                    uiCurKey2Lvl = VOLDET_KEY_LVL_MENU;
                    //debug_msg("uiCurKey2Lvl = %d VOLDET_KEY_LVL_MENU\r\n",uiCurKey2Lvl);
                }
            }
            else
            {
                uiMenuKey = KEYSCAN_MENUKEY_RELEASED;
            }
        }
    }
    #endif
    //debug_msg("uiRetKey1Lvl uiCurKey2Lvl = %d %d\r\n",uiRetKey1Lvl,uiCurKey2Lvl);
    uiRetKey1Lvl = uiCurKey2Lvl;

    return uiRetKey1Lvl;
}

#if 0
static UINT32 VolDet_GetKey2Level(void)
{
    static UINT32   uiRetKey1Lvl;
    UINT32          uiKey1ADC, uiCurKey2Lvl;

    uiKey1ADC = VolDet_GetKey2ADC();
    DBG_IND("uiKey2ADC %d \r\n", uiKey1ADC);
    if (uiKey1ADC < VOLDET_KEY_ADC_TH)
    {
        uiCurKey2Lvl = VOLDET_KEY_LVL_UP;
    }
    else
    {
        uiCurKey2Lvl = VOLDET_KEY_LVL_UNKNOWN;
    }

    uiRetKey1Lvl = uiCurKey2Lvl;

    return uiRetKey1Lvl;
}
#endif
/**
  Detect Mode Switch state.

  Detect Mode Switch state.

  @param void
  @return UINT32 Mode Switch state (DSC Mode)
*/
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
}
#if 0
static UINT32 DrvKey_DetGsensorEvent(void)
{
    UINT32 uiKeyCode = 0;
    BOOL bGSensorStatus=FALSE;
    Gsensor_Data GS_Data = {0};

    bGSensorStatus = GSensor_GetStatus(&GS_Data);

    if(bGSensorStatus==TRUE)
    {
        uiKeyCode |= FLGKEY_CUSTOM1;
    }

    return uiKeyCode;
}
#endif
// <Layout>
//------------------------------//
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
/**
  Detect normal key is pressed or not.

  Detect normal key is pressed or not.
  Return key pressed status (refer to KeyDef.h)

  @param void
  @return UINT32
*/
#if (_KEY_METHOD_ == _KEY_METHOD_NONE_)
UINT32 DrvKey_DetNormalKey(void)
{
    return 0;
}
#elif (_KEY_METHOD_ == _KEY_METHOD_2KEY_)
UINT32 DrvKey_DetNormalKey(void)
{
    UINT32 uiKeyCode = 0/*,uiKeyGSensor=0*/;

#if (ADC_KEY == ENABLE)
    UINT32 uiKey1Lvl = VolDet_GetKey1Level();
    UINT32 uiKey2Lvl = VolDet_GetKey2Level();
    switch(uiKey1Lvl)
    {
        case VOLDET_KEY_LVL_UNKNOWN:
        default:
            break;
        case VOLDET_KEY_LVL_0:  //SW2
            uiKeyCode |= FLGKEY_LEFT;
            break;
        case VOLDET_KEY_LVL_1:  //SW3
            uiKeyCode |= FLGKEY_RIGHT;
            break;
    }
    switch(uiKey2Lvl)
    {
        case VOLDET_KEY_LVL_UNKNOWN:
        default:
            break;
        case VOLDET_KEY_LVL_0:  //SW4
            //uiKeyCode |= FLGKEY_LEFT;
            break;
    }
#endif

    if(!gpio_getPin(GPIO_KEY_SHUTTER2))  //SW1
    {
        uiKeyCode |= FLGKEY_SHUTTER2;
    }

/*
    //detect if power-on by press playback key
    if (!HwPower_GetPowerKey(POWER_ID_PSW2))
    {
        uiKeyCode |= FLGKEY_PLAYBACK;
    }
*/
    DBG_IND("KEY=%08x\r\n", uiKeyCode);

    //uiKeyGSensor = DrvKey_DetGsensorEvent();
    //uiKeyCode |= uiKeyGSensor;

    DrvKey_DetKeyDelay();
    return uiKeyCode;
}

#elif (_KEY_METHOD_ == _KEY_METHOD_4KEY_)
UINT32 DrvKey_DetNormalKey(void)
{
    UINT32 uiKeyCode = 0/*,uiKeyGSensor=0*/;

//    static UINT32 uiMenuKey = KEYSCAN_MENUKEY_UNKNOWN;
//    static UINT32 uiMenuKeyState    = KEYSCAN_MENUKEY_INIT_STATE;
//    static UINT32 uiMenuKeyPressCount = 0;


#if (ADC_KEY == ENABLE)
    UINT32 uiKey1Lvl = VolDet_GetKey1Level();
    //UINT32 uiKey2Lvl = VolDet_GetKey2Level();
    //debug_msg("uiKey1Lvl = %d \r\n",uiKey1Lvl);
    switch(uiKey1Lvl)
    {
        case VOLDET_KEY_LVL_UNKNOWN:
        default:
            break;
        case VOLDET_KEY_LVL_UP:
            uiKeyCode |= FLGKEY_UP;
            break;
        case VOLDET_KEY_LVL_DOWN:
            uiKeyCode |= FLGKEY_DOWN;
            break;
        case VOLDET_KEY_LVL_MENU:
            uiKeyCode |= FLGKEY_MENU;
            break;
        case VOLDET_KEY_LVL_MODE:
            uiKeyCode |= FLGKEY_MODE;
            break;
        case VOLDET_KEY_LVL_SHUTTER2:
            uiKeyCode |= FLGKEY_SHUTTER2;
            break;
    }


    #if 0
    {
        if ((VolDet_GetKey1ADC() >= VOLDET_KEY_ADC_LVL1) && (VolDet_GetKey1ADC() <= VOLDET_KEY_ADC_LVL2))
        {
            // Debounce
            if (uiMenuKey == KEYSCAN_MENUKEY_PRESSED)
            {
                if (uiMenuKeyState == KEYSCAN_MENUKEY_RELEASE_STATE)
                {
                    uiMenuKeyPressCount++;
                    if (uiMenuKeyPressCount > KEYSCAN_MENUKEY_COUNT)
                    {
                        uiMenuKeyState = KEYSCAN_MENUKEY_PRESS_STATE;
                    }
                }

                if(uiMenuKeyState == KEYSCAN_MENUKEY_PRESS_STATE)
                {
                    uiMenuKeyPressCount = 0;
                    uiMenuKey == KEYSCAN_MENUKEY_UNKNOWN;
                    uiMenuKeyState = KEYSCAN_MENUKEY_INIT_STATE;

                    uiKeyCode |= FLGKEY_MODE;
                    debug_msg("FLGKEY_MODE\r\n");
                }
            }
            else
            {
                uiMenuKey = KEYSCAN_MENUKEY_PRESSED;
            }
        }
        else
        {
            // Debounce
            if (uiMenuKey == KEYSCAN_MENUKEY_RELEASED)
            {
                if (uiMenuKeyState == KEYSCAN_MENUKEY_INIT_STATE)
                {
                    uiMenuKeyState = KEYSCAN_MENUKEY_RELEASE_STATE;
                }

                if (uiMenuKeyPressCount)
                {
                    uiMenuKeyPressCount = 0;
                    uiKeyCode |= FLGKEY_MENU;
                    debug_msg("FLGKEY_MENU\r\n");
                }
            }
            else
            {
                uiMenuKey = KEYSCAN_MENUKEY_RELEASED;
            }
        }
    }
#endif

    /*
    switch(uiKey2Lvl)
    {
        case VOLDET_KEY_LVL_UNKNOWN:
        default:
            break;
        case VOLDET_KEY_LVL_0:  //SW4
            #if( _UI_STYLE_ == _UI_STYLE_CARDV_)
            uiKeyCode |= FLGKEY_ENTER;
            #else
            uiKeyCode |= FLGKEY_RIGHT;
            #endif
            break;
    }
    */
#endif
/*
    if(!gpio_getPin(GPIO_KEY_SHUTTER2))  //SW1
    {
        uiKeyCode |= FLGKEY_SHUTTER2;

    }
*/
/*
    //detect if power-on by press playback key
    if (!HwPower_GetPowerKey(POWER_ID_PSW2))
    {
        uiKeyCode |= FLGKEY_PLAYBACK;
    }
*/
   // debug_msg("KEY=%08x\r\n", uiKeyCode);

   //uiKeyGSensor = DrvKey_DetGsensorEvent();
   //uiKeyCode |= uiKeyGSensor;

    DrvKey_DetKeyDelay();
    return uiKeyCode;
}
#else
#error "Unknown Key Method"
#endif
/**
  Detect power key is pressed or not.

  Detect power key is pressed or not.
  Return key pressed status (refer to KeyDef.h)

  @param void
  @return UINT32
*/
#if( _UI_STYLE_ == _UI_STYLE_CARDV_)
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
