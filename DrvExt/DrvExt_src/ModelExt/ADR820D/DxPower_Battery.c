#include "DxCfg.h"
#include "IOCfg.h"
#include "DxPower.h"
#include "Utility.h"
#include "DxFlash.h"

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxPwr
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


#define TEMPDET_FUNCTION DISABLE
#define TEMPDET_TEST     DISABLE
#define ADC_TEST         DISABLE
#define DUMMY_LOAD                   1
#define BATT_SLIDE_WINDOW_COUNT      8

#define VOLDET_BATTERY_420V
#define VOLDET_BATTERY_415V			910
#define VOLDET_BATTERY_410V
#define VOLDET_BATTERY_405V
#define VOLDET_BATTERY_400V         900
#define VOLDET_BATTERY_390V         880
#define VOLDET_BATTERY_380V         840
#define VOLDET_BATTERY_378V
#define VOLDET_BATTERY_377V
#define VOLDET_BATTERY_376V
#define VOLDET_BATTERY_375V   		825
#define VOLDET_BATTERY_374V
#define VOLDET_BATTERY_373V			820
#define VOLDET_BATTERY_372V
#define VOLDET_BATTERY_371V
#define VOLDET_BATTERY_370V         815
#define VOLDET_BATTERY_369V
#define VOLDET_BATTERY_367V			805
#define VOLDET_BATTERY_365V			802
#define VOLDET_BATTERY_360V         790
#define VOLDET_BATTERY_355V
#define VOLDET_BATTERY_350V
#define VOLDET_BATTERY_340V
#define VOLDET_BATTERY_010V         15
#define CHARGE_ADC_OFFSET           15
#define LENS_ADC_OFFSET             23
#define LENS_ADC_OFFSET2            12



DummyLoadType dummyLoadData[11];


#define OCVREG0				0x00		//3.1328
#define OCVREG1				0x01		//3.2736
#define OCVREG2				0x02		//3.4144
#define OCVREG3				0x05		//3.5552
#define OCVREG4				0x07		//3.6256
#define OCVREG5				0x0D		//3.6608
#define OCVREG6				0x10		//3.6960
#define OCVREG7				0x1A		//3.7312
#define OCVREG8				0x24		//3.7664
#define OCVREG9				0x2E		//3.8016
#define OCVREGA				0x35		//3.8368
#define OCVREGB				0x3D		//3.8720
#define OCVREGC				0x49		//3.9424
#define OCVREGD				0x54		//4.0128
#define OCVREGE				0x5C		//4.0832
#define OCVREGF				0x64		//4.1536


//***********************************************
//*
//*    Battery Rule depend on Model
//*
//***********************************************

//#define VOLDET_BATTERY_ADC_TH       0

#if TEMPDET_TEST
INT32 temperature_value=0;
#endif


static INT32  gTempValue = 25;
#if ADC_TEST
UINT32 gAdcValue = 782;
#endif

//------------------ Battery Status Level -------------------//
#define  BATT_LEVEL_COUNT  4

static UINT32  LiBattAdcLevelValue[BATT_LEVEL_COUNT]={
	OCVREG5,
	OCVREG7,
	OCVREGA,
	OCVREGE,
};



#define  DUMMY_LOAD_OFFSETV          VOLDET_BATTERY_010V
#define  LENS_MOVE_MAX_COUNT         10

static UINT32* pBattAdcLevelValue = &LiBattAdcLevelValue[0];;
static UINT32  uiBattADCSlideWin[BATT_SLIDE_WINDOW_COUNT]={0};
static UINT8   uiBattSlideIdx = 0;
static UINT8   uiCurSlideWinCnt = 0;
static INT32   iBattAdcCalOffset = 0;
#if USB_CHARGE_FUNCTION
static UINT32  u32BattChargeCurrent = BATTERY_CHARGE_CURRENT_LOW;
#endif
static INT32   DrvPower_GetTempCompentation(INT32 tempValue);

static UINT32  bDummyLoadPwrOff = FALSE;

static DX_CALLBACK_PTR   g_fpDxPowerCB = NULL;
/**
  Get battery voltage ADC value

  Get battery voltage ADC value

  @param void
  @return UINT32 ADC value
*/
static UINT32 DrvPower_GetBatteryADC(void)
{
    UINT32 uiADCValue;
    INT32  TempCompentationADC;

    #if ADC_TEST
    uiADCValue = gAdcValue;
    #else
    uiADCValue = (adc_readData(ADC_CH_VOLDET_BATTERY));
	//debug_msg("uiADCValue= %d\r\n",uiADCValue);
    #endif
    {
        TempCompentationADC = DrvPower_GetTempCompentation(gTempValue);
        DBG_MSG("Temp= %d\r\n",gTempValue);
        if (DrvFlash_IsCharge())
        {
            //DBG_MSG("ADC = %d,",uiADCValue);
            uiADCValue+= (CHARGE_ADC_OFFSET+TempCompentationADC);
            DBG_MSG("Charge ADC+= %d\r\n",CHARGE_ADC_OFFSET+TempCompentationADC);
        }
        else
        {
            if (g_fpDxPowerCB)
            {
                if(g_fpDxPowerCB(DRVPWR_CB_IS_LENS_MOVING,0, 0))
                {
                    DBG_MSG("ADC = %d,",uiADCValue);
                    uiADCValue+= (LENS_ADC_OFFSET+TempCompentationADC);
                    DBG_MSG("lens2 ADC+= %d\r\n",LENS_ADC_OFFSET+TempCompentationADC);
                }

            }
        }
        uiADCValue+=(iBattAdcCalOffset/2);
        return uiADCValue;
    }
}
//#NT#2009/09/02#Lincy Lin -begin
//#Add function for check battery overheat
/**
  Get battery

  Get battery Temperature ADC value

  @param void
  @return UINT32 ADC value
*/
static BOOL DrvPower_IsBattOverHeat(void)
{
    return FALSE;
}
//#NT#2009/09/02#Lincy Lin -end




/**
  Get battery voltage level

  Get battery voltage level.
  If battery voltage level is DRVPWR_BATTERY_LVL_EMPTY, it means
  that you have to power off the system.

  @param void
  @return UINT32 Battery Level, refer to VoltageDet.h -> VOLDET_BATTERY_LVL_XXXX
*/

static UINT32 DrvPower_GetBatteryLevel(void)
{
    static UINT32   uiPreBatteryLvl    = DRVPWR_BATTERY_LVL_UNKNOWN;
    static UINT32   uiPreBatteryADC    = 0;
    static UINT32   uiRetBatteryLvl;
    static UINT32   uiEmptycount =0;
    static UINT32   uiCount = 0;
    UINT32          uiCurBatteryADC, uiCurBatteryLvl,i;
    BOOL            isMatch = FALSE;

    uiCurBatteryLvl = 0;
    if(uiPreBatteryLvl== DRVPWR_BATTERY_LVL_UNKNOWN)
    {
        uiCurBatteryADC = DrvPower_GetBatteryADC();
        uiPreBatteryADC = DrvPower_GetBatteryADC()-1;
        for (i = 0;i<BATT_SLIDE_WINDOW_COUNT;i++)
        {
            uiBattADCSlideWin[i] = uiCurBatteryADC;
            DBG_MSG("AVG=%d\r\n",uiCurBatteryADC);
        }
    }
    else
    {

        uiCurSlideWinCnt = BATT_SLIDE_WINDOW_COUNT;
        uiBattADCSlideWin[uiBattSlideIdx++] = DrvPower_GetBatteryADC();
        if (uiBattSlideIdx >= uiCurSlideWinCnt)
        {
            uiBattSlideIdx = 0;
        }
        uiCurBatteryADC = 0;
        for (i = 0;i<uiCurSlideWinCnt;i++)
        {
            uiCurBatteryADC+=uiBattADCSlideWin[i];
            DBG_MSG("A[%d]=%d,",i,uiBattADCSlideWin[i]);
        }
        uiCurBatteryADC/=uiCurSlideWinCnt;
        DBG_MSG("AVG=%d",uiCurBatteryADC);
        DBG_MSG(", V=%1.02f\r\n",((float)uiCurBatteryADC)*4.2/VOLDET_BATTERY_420V);
    }

    //DBG_IND("%d,%d,%d\r\n",VOLDET_BATTERY_ADC_LVL0,VOLDET_BATTERY_ADC_LVL1,VOLDET_BATTERY_ADC_LVL2);
    // Rising
    if (uiCurBatteryADC > uiPreBatteryADC)
    {
        for (i=BATT_LEVEL_COUNT;i>0;i--)
        {
            if (uiCurBatteryADC > pBattAdcLevelValue[i-1])
            {
                uiCurBatteryLvl = i;
                isMatch = TRUE;
                break;
            }
        }
        if (isMatch != TRUE)
        {
            uiCurBatteryLvl = 0;
        }
    }
    // Falling
    else
    {
        for (i=BATT_LEVEL_COUNT;i>0;i--)
        {
            if (uiCurBatteryADC > pBattAdcLevelValue[i-1])
            {
                uiCurBatteryLvl = i;
                isMatch = TRUE;
                break;
            }
        }
        if (isMatch != TRUE)
        {
            uiCurBatteryLvl = 0;
        }
    }
    // Debounce
    if ((uiCurBatteryLvl == uiPreBatteryLvl) ||
        (uiPreBatteryLvl == DRVPWR_BATTERY_LVL_UNKNOWN))
    {
        uiRetBatteryLvl = uiCurBatteryLvl;
    }
    uiPreBatteryLvl = uiCurBatteryLvl;
    uiPreBatteryADC = uiCurBatteryADC;

    if(uiCount % 2 == 0)
    {
        uiRetBatteryLvl = uiPreBatteryLvl;
    }
    uiCount++;
    //
    if(uiEmptycount|| uiRetBatteryLvl == DRVPWR_BATTERY_LVL_0)
    {
        uiEmptycount++;
        if (uiEmptycount == 4)
        {
            return DRVPWR_BATTERY_LVL_EMPTY;
        }
    }

    return DRVPWR_BATTERY_LVL_4;
    return uiRetBatteryLvl;
}


void DrvPower_PowerOnInit(void)
{
    pBattAdcLevelValue = &LiBattAdcLevelValue[0];
    #if USB_CHARGE_FUNCTION
    gTempValue = DrvPower_BattTempGet();
    #endif
}

UINT32 DrvPower_DummyLoad(void)
{
#if DUMMY_LOAD
    UINT32 Adc = 0, i, loopCount = 4;
    for (i=0;i<loopCount;i++)
    {
        Adc += DrvPower_GetBatteryADC();
        Delay_DelayMs(1);
    }
    Adc/=loopCount;
    DBG_MSG("Adc= %d, low batt = %d\r\n",Adc,LiBattAdcLevelValue[0]);
    if (Adc < LiBattAdcLevelValue[0]+DUMMY_LOAD_OFFSETV)
        return FALSE;
    return TRUE;
#else
    return TRUE;
#endif
}

static void DrvPower_BattADC_Calibration(BOOL enable)
{
}

#if USB_CHARGE_FUNCTION
static void DrvPower_EnableChargeIC(BOOL bCharge)
{

}

static void DrvPower_ChargeBattEn(BOOL bCharge)
{

}


static void DrvPower_ChargeCurrentSet(UINT32 Current)
{
    u32BattChargeCurrent = Current;
}

static UINT32 DrvPower_ChargeCurrentGet(void)
{
    return u32BattChargeCurrent;
}

static void DrvPower_ChargeISet(BOOL isHigh)
{

}

static BOOL DrvPower_ChargeIGet(void)
{
    return 0;
}

static void DrvPower_ChargeVSet(BOOL isHigh)
{

}

static BOOL DrvPower_ChargeVGet(void)
{
    return 0;
}


static BOOL DrvPower_IsUnderCharge(void)
{
    return 0;
}

static BOOL DrvPower_IsUSBChargeOK(void)
{
    return 0;
}

static BOOL DrvPower_IsBattIn(void)
{
    return TRUE;

}

static BOOL DrvPower_IsDeadBatt(void)
{
    return FALSE;
}

static BOOL DrvPower_IsNeedRecharge(void)
{
    return FALSE;
}

INT32 DrvPower_BattTempGet(void)
{
    return 25;
}
#endif

static INT32 DrvPower_GetTempCompentation(INT32 tempValue)
{
    return 0;
}


void   DrvPower_SetControl(DRVPWR_CTRL DrvPwrCtrl, UINT32 value)
{
    DBG_IND("DrvPwrCtrl(%d), value(%d)",DrvPwrCtrl,value);
    switch (DrvPwrCtrl)
    {
        case DRVPWR_CTRL_BATTERY_CALIBRATION_EN:
            DrvPower_BattADC_Calibration((BOOL)value);
            break;

        case DRVPWR_CTRL_BATTERY_ADC_CAL_OFFSET:
            iBattAdcCalOffset = (INT32)value;
            break;

        #if USB_CHARGE_FUNCTION
        case DRVPWR_CTRL_BATTERY_CHARGE_EN:
            DrvPower_ChargeBattEn((BOOL)value);
            break;
        case DRVPWR_CTRL_BATTERY_CHARGE_CURRENT:
            DrvPower_ChargeCurrentSet(value);
            break;

        case DRVPWR_CTRL_BATTERY_CHARGE_ISET:
            DrvPower_ChargeISet((BOOL)value);
            break;

        case DRVPWR_CTRL_BATTERY_CHARGE_VSET:
            DrvPower_ChargeVSet((BOOL)value);
            break;

        case DRVPWR_CTRL_ENABLE_CHARGEIC:
            DrvPower_EnableChargeIC((BOOL)value);
            break;

        #endif

        default:
            DBG_ERR("%d \r\n",DrvPwrCtrl);
            break;
    }
}

UINT32  DrvPower_GetControl(DRVPWR_CTRL DrvPwrCtrl)
{
    UINT32 rvalue = 0;
    switch (DrvPwrCtrl)
    {
        case DRVPWR_CTRL_BATTERY_LEVEL:
            rvalue = DrvPower_GetBatteryLevel();
            break;
        case DRVPWR_CTRL_BATTERY_ADC_VALUE:
            rvalue = DrvPower_GetBatteryADC();
            break;
        case DRVPWR_CTRL_BATTERY_ADC_CAL_OFFSET:
            rvalue = iBattAdcCalOffset;
            break;
        case DRVPWR_CTRL_IS_DUMMUYLOAD_POWEROFF:
            rvalue = bDummyLoadPwrOff;
            break;
        case DRVPWR_CTRL_IS_BATT_OVERHEAT:
            rvalue = (UINT32)DrvPower_IsBattOverHeat();
            break;
        #if USB_CHARGE_FUNCTION
        case DRVPWR_CTRL_IS_BATT_INSERT:
            rvalue = (UINT32)DrvPower_IsBattIn();
            break;
        case DRVPWR_CTRL_IS_DEAD_BATT:
            rvalue = (UINT32)DrvPower_IsDeadBatt();
            break;
        case DRVPWR_CTRL_IS_NEED_RECHARGE:
            rvalue = (UINT32)DrvPower_IsNeedRecharge();
            break;
        case DRVPWR_CTRL_BATTERY_CHARGE_EN:
            rvalue = (UINT32)DrvPower_IsUnderCharge();
            break;
        case DRVPWR_CTRL_BATTERY_CHARGE_OK:
            rvalue = (UINT32)DrvPower_IsUSBChargeOK();
            break;
        case DRVPWR_CTRL_BATTERY_CHARGE_CURRENT:
            rvalue = DrvPower_ChargeCurrentGet();
            break;
        case DRVPWR_CTRL_BATTERY_CHARGE_ISET:
            rvalue = (UINT32)DrvPower_ChargeIGet();
            break;
        case DRVPWR_CTRL_BATTERY_CHARGE_VSET:
            rvalue = (UINT32)DrvPower_ChargeVGet();
            break;
        /*case DRVPWR_CTRL_BATTERY_TEMPERATURE:
            rvalue = DrvPower_BattTempGet();
            break;
        */
        #else
        case DRVPWR_CTRL_BATTERY_CHARGE_EN:
            rvalue = FALSE;
            break;
        #endif
        default:
            DBG_ERR("(%d)\r\n",DrvPwrCtrl);
            break;
    }
    return rvalue;
}

void   DrvPower_RegCB(DX_CALLBACK_PTR fpDxPowerCB)
{

    g_fpDxPowerCB = fpDxPowerCB;
}

