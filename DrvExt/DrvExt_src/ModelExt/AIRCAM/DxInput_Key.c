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
#define VOLDET_KEY_ADC_RANGE         (110)
#define VOLDET_KEY_ADC_LVL0          (250-VOLDET_KEY_ADC_RANGE)

#define VOLDET_KEY_ADC_TH            (512)

#define VOLDET_KEY_LVL_UNKNOWN           0xFFFFFFFF
#define VOLDET_KEY_LVL_0                 0
#define VOLDET_KEY_LVL_1                 1
#define VOLDET_KEY_LVL_2                 2
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
/**
  Get ADC key voltage level

  Get  ADC key  2 voltage level.

  @param void
  @return UINT32 key level, refer to VoltageDet.h -> VOLDET_MS_LVL_XXXX
*/
static UINT32 VolDet_GetKey1Level(void)
{
    UINT32          uiKey1ADC;

    uiKey1ADC = VolDet_GetKey1ADC();
    if (uiKey1ADC < 100)
    {

        // debug_msg("VOLDET_KEY_LVL_MODE \r\n");
        return VOLDET_KEY_LVL_0;
        }
    else if ((uiKey1ADC > (200))
             && (uiKey1ADC < 300))
        {

        //   debug_msg("VOLDET_KEY_LVL_MENU \r\n");
        return VOLDET_KEY_LVL_1;
    }
    else if ((uiKey1ADC > 350)
             && (uiKey1ADC < 450))
    {

        //   debug_msg("VOLDET_KEY_LVL_SOS \r\n");
        return VOLDET_KEY_LVL_2;
    }


    return VOLDET_KEY_LVL_UNKNOWN;
}
static UINT32 VolDet_GetKey2Level(void)
{
    static UINT32   uiRetKey1Lvl;
    UINT32          uiKey1ADC, uiCurKey2Lvl;

    uiKey1ADC = VolDet_GetKey2ADC();
    DBG_IND("uiKey2ADC %d \r\n", uiKey1ADC);
    if (uiKey1ADC < VOLDET_KEY_ADC_TH)
    {
        uiCurKey2Lvl = VOLDET_KEY_LVL_0;
    }
    else
    {
        uiCurKey2Lvl = VOLDET_KEY_LVL_UNKNOWN;
    }

    uiRetKey1Lvl = uiCurKey2Lvl;

    return VOLDET_KEY_LVL_UNKNOWN;
}
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
    UINT32 uiKeyCode = 0,uiKeyGSensor=0;

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

    uiKeyGSensor = DrvKey_DetGsensorEvent();
    uiKeyCode |= uiKeyGSensor;

    DrvKey_DetKeyDelay();
    return uiKeyCode;
}
#elif (_KEY_METHOD_ == _KEY_METHOD_4KEY_)
UINT32 DrvKey_DetNormalKey(void)
{
    UINT32 uiKeyCode = 0,uiKeyGSensor=0;

#if (ADC_KEY == ENABLE)
    UINT32 uiKey1Lvl = VolDet_GetKey1Level();
    UINT32 uiKey2Lvl = VolDet_GetKey2Level();
    switch(uiKey1Lvl)
    {
        case VOLDET_KEY_LVL_UNKNOWN:
        default:
            break;
        case VOLDET_KEY_LVL_0:  //SW2
            uiKeyCode |= FLGKEY_UP;
            break;
        case VOLDET_KEY_LVL_1:  //SW3
            uiKeyCode |= FLGKEY_DOWN;
            break;
        case VOLDET_KEY_LVL_2:  //SW3
            uiKeyCode |= FLGKEY_RIGHT;
            break;
    }
#if 1
    switch(uiKey2Lvl)
    {
    default:
        break;
    }
#endif
#endif

#if 0
    if(!gpio_getPin(GPIO_KEY_SHUTTER2))  //SW1
    {
        uiKeyCode |= FLGKEY_LEFT;
    }
#endif

/*
    //detect if power-on by press playback key
    if (!HwPower_GetPowerKey(POWER_ID_PSW2))
    {
        uiKeyCode |= FLGKEY_PLAYBACK;
    }
*/
    DBG_IND("KEY=%08x\r\n", uiKeyCode);

    uiKeyGSensor = DrvKey_DetGsensorEvent();
    uiKeyCode |= uiKeyGSensor;

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
