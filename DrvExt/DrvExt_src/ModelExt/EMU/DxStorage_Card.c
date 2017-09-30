/**
    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.

    @file       KeyScanSD.c
    @ingroup    mIPRJAPKeyIO

    @brief      Detect SD card status
                Detect SD card insert/remove, lock/unlock

    @note       Nothing.

    @date       2005/12/15
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "Type.h"
#include "DxCfg.h"
#include "IOCfg.h"

#include "DxStorage.h"
#include "DxCommon.h"
#include "DxApi.h"

#include "sdio.h"
#include "SdmmcDesc.h"
#include "StrgDef.h"
#include "Utility.h"
#include "pll.h"

#ifndef __DBGLVL__
    #define __DBGLVL__  1       // Output all message by default. __DBGLVL__ will be set to 1 via make parameter when release code.
#endif

// Default debug filter
#ifndef __DBGFLT__
    #define __DBGFLT__  "*"     // Display everything when debug level is 2
#endif

#include "DebugModule.h"

void DrvCARD_EnableCardPower(BOOL bEn);
BOOL DrvCARD_DetStrgCard(void);
BOOL DrvCARD_DetStrgCardWP(void);

//public func
UINT32 DrvCARDGetcaps(UINT32 CapID, UINT32 Param1); // Get Capability Flag (Base on interface version)
UINT32 DrvCARDInit(void* pInitParam); // Set Init Parameters
UINT32 DrvCARDOpen(void); // Common Constructor
UINT32 DrvCARDClose(void); // Common Destructor
UINT32 DrvCARDState(UINT32 StateID, UINT32 Value); // General Properties
UINT32 DrvCARDControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2);  // General Methods
UINT32 DrvCARDCommand(CHAR *pcCmdStr); //General Command Console

//dx object
DX_OBJECT gDevCARD1 =
{
    DXFLAG_SIGN,
    DX_CLASS_STORAGE_EXT | DX_TYPE_CARD1,
    STORAGE_VER,
    "Storage_Card1",
    0,0,0,0,
    DrvCARDGetcaps,
    0,
    DrvCARDInit,
    DrvCARDOpen,
    DrvCARDClose,
    DrvCARDState,
    DrvCARDControl,
    DrvCARDCommand,
    0,
};

UINT32 DrvCARDGetcaps(UINT32 CapID, UINT32 Param1) // Get Capability Flag (Base on interface version)
{
    UINT32 v = 0;
    switch(CapID)
    {
    case STORAGE_CAPS_BASE:
        DBG_IND("get card plug caps\r\n");
        v = STORAGE_BF_DETPLUG|STORAGE_BF_DETWP;
        break;
    case STORAGE_CAPS_HANDLE:
        v = (UINT32)sdio_getStorageObject(STRG_OBJ_FAT1);
        break;
    default:
        break;
    }
    return v;
}

UINT32 DrvCARDInit(void* pInitParam) // Set Init Parameters
{
    PSTRG_TAB   pStorageObj = NULL;

    DXSTRG_INIT *g_pInit = (DXSTRG_INIT*)pInitParam; //fat and pstore not complete

#if (_STORAGECARD_ == _STORAGECARD_SDIO_)
    pStorageObj = sdio_getStorageObject(STRG_OBJ_FAT1);
    if (pStorageObj == NULL)
    {
        DBG_ERR("Get SDIO storage object fail!\r\n");
        return DX_NULL_POINTER;
    }

    pStorageObj->SetParam(STRG_SET_MEMORY_REGION, g_pInit->buf.Addr, g_pInit->buf.Size);

    sdio_setCallBack(SDIO_CALLBACK_CARD_DETECT,     (SDIO_CALLBACK_HDL)DrvCARD_DetStrgCard);
    sdio_setCallBack(SDIO_CALLBACK_WRITE_PROTECT,   (SDIO_CALLBACK_HDL)DrvCARD_DetStrgCardWP);

#if (_FPGA_EMULATION_ == ENABLE)
    // Support default speed only
    sdio_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,           SDIO_BUS_SPEED_MODE_DS);
    // Source clock 96 MHz can run faster than default value
    sdio_setConfig(SDIO_CONFIG_ID_SRCLK,                    SDIO_SRC_CLK_96MHZ);
#else
    #if 1
    // Enable SSPLL in EVB environment
    {
        pll_setPLLEn(PLL_ID_4, FALSE);
        pll_setPLLSpreadSpectrum(PLL_ID_4, 46000000, 48000000);
        pll_setPLLEn(PLL_ID_4, TRUE);

        sdio_setConfig(SDIO_CONFIG_ID_SRCLK,                SDIO_SRC_CLK_PLL4);
    }
    sdio_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,           SDIO_BUS_SPEED_MODE_DS | SDIO_BUS_SPEED_MODE_HS);
    #else
    // Support UHSI
    sdio_setConfig(SDIO_CONFIG_ID_UHSI_EN,                  TRUE);
    // Source clock -> 96MHz
    sdio_setConfig(SDIO_CONFIG_ID_SRCLK,                    SDIO_SRC_CLK_96MHZ);
    // SDR50 max. clock -> 96MHz
    sdio_setConfig(SDIO_CONFIG_ID_SDR50_MAX_CLK,            96000000);
    // DDR50 max. clock -> 48MHz
    sdio_setConfig(SDIO_CONFIG_ID_DDR50_MAX_CLK,            48000000);

    // Support default speed + high speed
    sdio_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,           SDIO_BUS_SPEED_MODE_DS | SDIO_BUS_SPEED_MODE_HS | SDIO_BUS_SPEED_MODE_DDR50);
    #endif

    // Default speed driving -> 5.0 mA
    sdio_setConfig(SDIO_CONFIG_ID_DS_DRIVING,               50);
    // High speed driving    -> 10.0 mA
    sdio_setConfig(SDIO_CONFIG_ID_HS_DRIVING,               100);

    // Adjust driving/sink according to operating mode
    sdio_setConfig(SDIO_CONFIG_ID_DRIVING_SINK_EN,          TRUE);
#endif
    sdio_setConfig(SDIO_CONFIG_ID_BUS_WIDTH,                SD_HOST_BUS_8_BITS);

    sdio_setConfig(SDIO_CONFIG_ID_FORMAD_OEMID,             (UINT32)"NOVATEK");
    sdio_setConfig(SDIO_CONFIG_ID_FORMAD_VOL_LABLE,         (UINT32)"SD_VOL");
    sdio_setConfig(SDIO_CONFIG_ID_FORMAD_RAND_VOL_ID_EN,    TRUE);
#endif

#if (_STORAGECARD_ == _STORAGECARD_SDIO2_)
    pStorageObj = sdio2_getStorageObject(STRG_OBJ_FAT1);
    if (pStorageObj == NULL)
    {
        DBG_ERR("Get SDIO2 storage object fail!\r\n");
        return DX_NULL_POINTER;
    }

    pStorageObj->SetParam(STRG_SET_MEMORY_REGION, g_pInit->buf.Addr, g_pInit->buf.Size);

    sdio2_setCallBack(SDIO_CALLBACK_CARD_DETECT,    (SDIO_CALLBACK_HDL)DrvCARD_DetStrgCard);
    sdio2_setCallBack(SDIO_CALLBACK_WRITE_PROTECT,  (SDIO_CALLBACK_HDL)DrvCARD_DetStrgCardWP);

#if (_FPGA_EMULATION_ == ENABLE)
    // Support default speed only
    sdio2_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,          SDIO_BUS_SPEED_MODE_DS);
    // Source clock 96 MHz can run faster than default value
    sdio2_setConfig(SDIO_CONFIG_ID_SRCLK,                   SDIO_SRC_CLK_96MHZ);
#else

    #if 1
    sdio2_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,          SDIO_BUS_SPEED_MODE_DS | SDIO_BUS_SPEED_MODE_HS);
    #else
    // Support UHSI
    sdio2_setConfig(SDIO_CONFIG_ID_UHSI_EN,                 TRUE);
    // Source clock -> 96MHz
    sdio2_setConfig(SDIO_CONFIG_ID_SRCLK,                   SDIO_SRC_CLK_96MHZ);
    // SDR50 max. clock -> 96MHz
    sdio2_setConfig(SDIO_CONFIG_ID_SDR50_MAX_CLK,           96000000);
    // DDR50 max. clock -> 48MHz
    sdio2_setConfig(SDIO_CONFIG_ID_DDR50_MAX_CLK,           48000000);

    // Support default speed + high speed
    sdio2_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,          SDIO_BUS_SPEED_MODE_DS | SDIO_BUS_SPEED_MODE_HS | SDIO_BUS_SPEED_MODE_SDR50);
    #endif

    // Default speed driving -> 5.0 mA
    sdio2_setConfig(SDIO_CONFIG_ID_DS_DRIVING,              50);
    // High speed driving    -> 10.0 mA
    sdio2_setConfig(SDIO_CONFIG_ID_HS_DRIVING,              100);

    // Adjust driving/sink according to operating mode
    sdio2_setConfig(SDIO_CONFIG_ID_DRIVING_SINK_EN,         TRUE);
#endif
    sdio2_setConfig(SDIO_CONFIG_ID_BUS_WIDTH,               SD_HOST_BUS_8_BITS);

    sdio2_setConfig(SDIO_CONFIG_ID_FORMAD_OEMID,            (UINT32)"NOVATEK");
    sdio2_setConfig(SDIO_CONFIG_ID_FORMAD_VOL_LABLE,        (UINT32)"SD_VOL");
    sdio2_setConfig(SDIO_CONFIG_ID_FORMAD_RAND_VOL_ID_EN,   TRUE);
#endif

#if (_STORAGECARD_ == _STORAGECARD_SDIO3_)
    pStorageObj = sdio3_getStorageObject(STRG_OBJ_FAT1);
    if (pStorageObj == NULL)
    {
        DBG_ERR("Get SDIO3 storage object fail!\r\n");
        return DX_NULL_POINTER;
    }

    pStorageObj->SetParam(STRG_SET_MEMORY_REGION, g_pInit->buf.Addr, g_pInit->buf.Size);

    sdio3_setCallBack(SDIO_CALLBACK_CARD_DETECT,    (SDIO_CALLBACK_HDL)DrvCARD_DetStrgCard);
    sdio3_setCallBack(SDIO_CALLBACK_WRITE_PROTECT,  (SDIO_CALLBACK_HDL)DrvCARD_DetStrgCardWP);

#if (_FPGA_EMULATION_ == ENABLE)
    // Support default speed only
    sdio3_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,          SDIO_BUS_SPEED_MODE_DS);
    // Source clock 96 MHz can run faster than default value
    sdio3_setConfig(SDIO_CONFIG_ID_SRCLK,                   SDIO_SRC_CLK_96MHZ);
#else
    // Support default speed + high speed
    sdio3_setConfig(SDIO_CONFIG_ID_BUS_SPEED_MODE,          SDIO_BUS_SPEED_MODE_DS | SDIO_BUS_SPEED_MODE_HS);

    // Default speed driving -> 5.0 mA
    sdio3_setConfig(SDIO_CONFIG_ID_DS_DRIVING,              50);
    // High speed driving    -> 10.0 mA
    sdio3_setConfig(SDIO_CONFIG_ID_HS_DRIVING,              100);

    // Adjust driving/sink according to operating mode
    sdio3_setConfig(SDIO_CONFIG_ID_DRIVING_SINK_EN,         TRUE);
#endif
    sdio3_setConfig(SDIO_CONFIG_ID_BUS_WIDTH,               SD_HOST_BUS_8_BITS);

    sdio3_setConfig(SDIO_CONFIG_ID_FORMAD_OEMID,            (UINT32)"NOVATEK");
    sdio3_setConfig(SDIO_CONFIG_ID_FORMAD_VOL_LABLE,        (UINT32)"SD_VOL");
    sdio3_setConfig(SDIO_CONFIG_ID_FORMAD_RAND_VOL_ID_EN,   TRUE);
#endif

    return DX_OK;
}

UINT32 DrvCARDOpen(void) // Common Constructor
{
    DrvCARD_EnableCardPower(TRUE);

    return DX_OK;
}

UINT32 DrvCARDClose(void) // Common Destructor
{
    DrvCARD_EnableCardPower(FALSE);

    return DX_OK;
}

UINT32 DrvCARDState(UINT32 StateID, UINT32 Value) // General Properties
{

    if (StateID & DXGET)
    {
        UINT32 rvalue =  0;
        StateID &= ~DXGET;
        DBG_IND("get %08x\r\n", StateID);
        switch(StateID)
        {
        case STORAGE_STATE_INSERT:
            rvalue = DrvCARD_DetStrgCard();
            break;

        case STORAGE_STATE_LOCK:
            {
            BOOL bLock = FALSE;
            if(DrvCARD_DetStrgCard())
            {
                bLock = DrvCARD_DetStrgCardWP();
            }
            rvalue = bLock;
            }
            break;

        default:
            DBG_ERR("state=0x%02X not support!\r\n", StateID);
            break;
        }
        return rvalue;
    }
    else if (StateID & DXSET)
    {
        StateID &= ~DXSET;
        DBG_IND("set %08x\r\n", StateID);
        switch(StateID)
        {
        default:
            DBG_ERR("state=0x%02X not support!\r\n", StateID);
            break;
        }
    }
    return DX_OK;
}

UINT32 DrvCARDControl(UINT32 CtrlID, UINT32 Param1, UINT32 Param2)  // General Methods
{
    DBG_IND("ctrl %08x\r\n", CtrlID);

    switch(CtrlID)
    {
    case STORAGE_CTRL_POWER:
        DrvCARD_EnableCardPower(Param1);
        break;

    default:
        DBG_ERR("ctrlid=0x%02X not support!\r\n", CtrlID);
        break;
    }
    return DX_OK;
}

UINT32 DrvCARDCommand(CHAR *pcCmdStr) //General Command Console
{
    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// GPIO related

#define KEYSCAN_CD_GPIO_INT     GPIO_INT_00
#define KEYSCAN_CARDDET_INT     DISABLE
#define KEYSCAN_SDWRPTDET_GPIO  DISABLE

void DrvCARD_EnableCardPower(BOOL bEn)
{
    gpio_setDir(GPIO_CARD_POWER, GPIO_DIR_OUTPUT);
    if(bEn)
    {
        DBG_IND("[SD] - card power enable\r\n");
        gpio_clearPin(GPIO_CARD_POWER);
        Delay_DelayMs(20);
    }
    else
    {

        DBG_IND("[SD] - card power disable\r\n");
        gpio_setPin(GPIO_CARD_POWER);
        Delay_DelayMs(350);
    }
}

#if (KEYSCAN_CARDDET_INT == ENABLE)
static volatile UINT32  uiStrgCardIntCnt  = 0;

/**
  ISR of SD card detection

  ISR of SD card detection

  @param void
  @return void
*/
static void KeyScan_DetStrgCardIsr(UINT32 uiEvent)
{
    uiStrgCardIntCnt++;

    // Debounce
    if (uiStrgCardIntCnt > 1)
    {
        uiStrgCardIntCnt = 0;
    }
}
#endif

void DrvCARD_SetInsert(UINT32 status)
{
    #if (KEYSCAN_CARDDET_INT == ENABLE)
    if(status)
        gpio_setIntTypePol(KEYSCAN_CD_GPIO_INT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    else
        gpio_setIntTypePol(KEYSCAN_CD_GPIO_INT, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_NEGLOW);
    #endif
}

void DrvCARD_DetRestart(void)
{
    #if (KEYSCAN_CARDDET_INT == ENABLE)
    gpio_setIntIsr(KEYSCAN_CD_GPIO_INT, KeyScan_DetStrgCardIsr);
    gpio_enableInt(KEYSCAN_CD_GPIO_INT);
    #endif
}

/**
  Detect Storage card is inserted or not

  Detect Storage card is inserted or not.
  Return TRUE if storage card is inserted, FALSE if storage card is removed.

  @param void
  @return BOOL: TRUE -> Storage card inserted, FALSE -> Storage card removed
*/
BOOL DrvCARD_DetStrgCard(void)
{
#if 1
    return (gpio_getPin(GPIO_CARD_DETECT) == 0 ? TRUE : FALSE);
#else
    return TRUE;
#endif
}

/**
  Detect Storage card is write protected or not

  Detect Storage card is write protected or not.
  Return TRUE if storage card is write protected, FALSE if storage card is not write protected.

  @param void
  @return BOOL: TRUE -> Storage card is write protected, FALSE -> Storage card is not write protected
*/
BOOL DrvCARD_DetStrgCardWP(void)
{
#if 1
    return (gpio_getPin(GPIO_CARD_WP) == 1 ? TRUE : FALSE );
#else
    return FALSE;
#endif
}


//@}
