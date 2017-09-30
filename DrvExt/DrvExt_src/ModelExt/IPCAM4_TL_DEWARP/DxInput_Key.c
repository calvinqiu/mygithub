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



/**
  Get ADC key voltage level

  Get  ADC key  2 voltage level.

  @param void
  @return UINT32 key level, refer to VoltageDet.h -> VOLDET_MS_LVL_XXXX
*/

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

void DrvKey_Init(void)
{
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
    return 0;
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
    return 0;
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
    return 0;
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
    return 0;
}
#endif
UINT32 DrvKey_DetStatusKey(DX_STATUS_KEY_GROUP KeyGroup)
{
    UINT32 uiReturn = STATUS_KEY_LVL_UNKNOWN;

    return uiReturn;
}
