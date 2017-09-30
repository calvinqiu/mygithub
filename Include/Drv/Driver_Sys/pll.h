/**
    PLL Configuration module header

    PLL Configuration module header file.

    @file       pll.h
    @ingroup    mIDrvSys_CG
    @note       Nothing

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _PLL_H
#define _PLL_H

#include "Type.h"

/**
    @addtogroup mIHALSysCG
*/
//@{

/**
    Clock Enable ID

    This is for pll_enableClock() and pll_disableClock().
*/
typedef enum
{
    DMA_CLK = 1,        ///< DMA(SDRAM) clock
    SIE_MCLK,           ///< SIE MCLK
    SIE_MCLK2,          ///< SIE MCLK2
    SIE_CLK,            ///< SIE clock
    SIE2_CLK,           ///< SIE2 Clock
    TGE_CLK,            ///< TGE clock
    IPE_CLK,            ///< IPE clock
    DIS_CLK = 8,        ///< DIS clock
    IME_CLK,            ///< IME clock
    FDE_CLK,            ///< FDE clock
    ISE_CLK,            ///< ISE clock
    SP_CLK,             ///< special clock
    IFE_CLK,            ///< IFE clock
    DCE_CLK,            ///< DCE clock
    IFE2_CLK,           ///< IFE2 clock
    IDE1_CLK = 16,      ///< IDE clock
    IDE2_CLK,           ///< IDE2 clock
    TV_CLK,             ///< TV clock
    MI_CLK = 20,        ///< MI clock
    CEC_CLK,            ///< CEC clock
    HDMI_CLK,           ///< HDMI clock
    CRYPTO_CLK,         ///< Crypto clock
    H264_CLK = 24,      ///< H.264 clock
    AFFINE_CLK,         ///< Affine clock
    JPG_CLK,            ///< JPEG clock
    GRAPH_CLK,          ///< Graphic clock
    GRAPH2_CLK,         ///< Graphic2 clock
    DAI_CLK,            ///< DAI clock
    EAC_A_ADC_CLK,      ///< EAC analog AD clock
    EAC_A_DAC_CLK,      ///< EAC analog DA clock

    NAND_CLK = 32,      ///< NAND clock
    SDIO_CLK = 34,      ///< SDIO clock
    SDIO2_CLK,          ///< SDIO2 clock
    I2C_CLK,            ///< I2C clock
    I2C2_CLK,           ///< I2C2 clock
    SPI_CLK,            ///< SPI clock
    SPI2_CLK,           ///< SPI2 clock
    SPI3_CLK = 40,      ///< SPI3 clock
    SIF_CLK,            ///< SIF clock
    UART_CLK,           ///< UART clock
    UART2_CLK,          ///< UART2 clock
    REMOTE_CLK,         ///< Remote clock
    ADC_CLK,            ///< ADC clock
    SDIO3_CLK,          ///< SDIO3 clock
    SPI4_CLK,           ///< SPI4 clock
    RTC_CLK = 48,       ///< RTC clock
    WDT_CLK ,           ///< WDT clock
    TMR_CLK,            ///< Timer clock
    USB_CLK,            ///< USB clock
    EAC_D_CLKEN,        ///< EAC digital clock
    SPI5_CLK,           ///< SPI5 PX clock
    UART3_CLK,          ///< UART3 clock
    UART4_CLK,          ///< UART4 clock
    EFUSE_CLK = 60,     ///< EFUSE clock
    ETH_CLK,            ///< ETH clock
    SP2_CLK,            ///< SP2 clock
    I2C3_CLK,           ///< I2C3 clock

    LVDS_CLK = 65,      ///< LVDS clock
    MIPI_CLK = 65,      ///< MIPI clock
    MIPI_LVDS_CLK = 65, ///< MIPI/LVDS clock
    MIPI2_CLK = 66,     ///< MIPI2 clock
    MIPI_LVDS2_CLK = 66,///< MIPI/LVDS2 clock
    MIPI_DSI_CLK,       ///< MIPI DSI clock
    SIE_PXCLK,          ///< SIE PX clock
    SIE2_PXCLK,         ///< SIE2 PX clock
    RDE_CLK,            ///< RDE clock
    PWM_CCNT_CLK = 72,  ///< PWM CCNT clock
    PWM_CCNT0_CLK = 72, ///< PWM CCNT0 clock
    PWM_CCNT1_CLK,      ///< PWM CCNT1 clock
    PWM_CCNT2_CLK,      ///< PWM CCNT2 clock
    PWM_CCNT3_CLK,      ///< PWM CCNT3 clock
    DRE_CLK,            ///< DRE clock
    TSMUX_CLK,          ///< TSMUX clock
    Vx1_CLK,            ///< Vx1 clock
    SIE3_PXCLK,         ///< SIE3 PX clock
    SIE4_PXCLK,         ///< SIE4 PX clock
    USB2_CLK,           ///< USB2 clock
    CC_CLK,             ///< CC clock
    RHE_CLK,            ///< RHE clock
    Vx1_SLCLK,          ///< Vx1 sub-link clock


    PWM0_CLK = 96,      ///< PWM0 clock
    PWM1_CLK,           ///< PWM1 clock
    PWM2_CLK,           ///< PWM2 clock
    PWM3_CLK,           ///< PWM3 clock
    PWM4_CLK,           ///< PWM4 clock
    PWM5_CLK,           ///< PWM5 clock
    PWM6_CLK,           ///< PWM6 clock
    PWM7_CLK,           ///< PWM7 clock
    PWM8_CLK,           ///< PWM8 clock
    PWM9_CLK,           ///< PWM9 clock
    PWM10_CLK,          ///< PWM10 clock
    PWM11_CLK,          ///< PWM11 clock
    PWM12_CLK,          ///< PWM12 clock
    PWM13_CLK,          ///< PWM13 clock
    PWM14_CLK,          ///< PWM14 clock
    PWM15_CLK,          ///< PWM15 clock
    PWM16_CLK,          ///< PWM16 clock
    PWM17_CLK,          ///< PWM17 clock
    PWM18_CLK,          ///< PWM18 clock
    PWM19_CLK,          ///< PWM19 clock

    CPU2_CLK = 256,     ///< CPU2 clock
    DSP_CLK,            ///< DSP clock

    ENUM_DUMMY4WORD(CG_EN)
} CG_EN;

#define PRE_CLK 0                ///< Backward compatible

/**
    Clock Gating Select ID

    This is for pll_setClkAutoGating() / pll_clearClkAutoGating() /
    pll_getClkAutoGating() / pll_setPclkAutoGating() /
    pll_clearPclkAutoGating() / pll_getPclkAutoGating().
*/
typedef enum
{
    SIE_GCLK,           ///< Gating SIE clock (Gating PCLK only)
    PRE_GCLK,           ///< Backward compatible
    IPE_GCLK,           ///< Gating IPE clock
    IME_GCLK,           ///< Gating IME clock
    DIS_GCLK,           ///< Gating DIS clock
    FDE_GCLK,           ///< Gating FDE clock
    DCE_GCLK,           ///< Gating DCE clock
    IFE_GCLK,           ///< Gating IFE clock
    GRA_GCLK = 8,       ///< Gating Graphic clock
    GRA2_GCLK,          ///< Gating Graphic2 clock
    IDE_GCLK,           ///< Gating IDE clock  (Gating PCLK only)
    IDE2_GCLK,          ///< Gating IDE2 clock  (Gating PCLK only)
    MI_GCLK,            ///< Gating MI clock  (Gating PCLK only)
    HDMI_GCLK = 13,     ///< Gating HDMI PCLK (Gating PCLK only)
    IFE2_GCLK = 14,     ///< Gating IFE2 clock
    JPG_GCLK,           ///< Gating JPEG clock
    H264_GCLK = 16,     ///< Gating H264 clock
    DAI_GCLK,           ///< Gating DAI clock  (Gating PCLK only)
    EAC_GCLK,           ///< Gating EAC clock  (Gating PCLK only)
    NAND_GCLK,          ///< Gating xD/Nand clock
    SDIO_GCLK,          ///< Gating SDIO clock
    SDIO2_GCLK,         ///< Gating SDIO2 clock
    I2C_GCLK,           ///< Gating I2C clock  (Gating PCLK only)
    I2C2_GCLK,          ///< Gating I2C2 clock  (Gating PCLK only)
    SPI_GCLK = 24,      ///< Gating SPI clock
    SPI2_GCLK,          ///< Gating SPI2 clock
    SPI3_GCLK,          ///< Gating SPI3 clock
    SIF_GCLK,           ///< Gating SIF clock
    UART_GCLK,          ///< Gating UART clock  (Gating PCLK only)
    UART2_GCLK,         ///< Gating UART2 clock  (Gating PCLK only)
    RM_GCLK,            ///< Gating Remote clock  (Gating PCLK only)
    ADC_GCLK,           ///< Gating ADC clock  (Gating PCLK only)

    TMR_GCLK = 32,      ///< Gating TMR clock  (Gating PCLK only)
    WDT_GCLK,           ///< Gating Watchdog clock  (Gating PCLK only)
    LVDS_GCLK = 35,     ///< Gating LVDS clock  (Gating PCLK only)
    MIPI_GCLK = 35,     ///< Gating MIPI clock  (Gating PCLK only)
    MIPI_LVDS_GCLK = 35,///< Gating MIPI/LVDS clock  (Gating PCLK only)
    MIPI2_GCLK = 36,    ///< Gating MIPI2 clock  (Gating PCLK only)
    MIPI_LVDS2_GCLK = 36,///< Gating MIPI/LVDS2 clock  (Gating PCLK only)
    TV_GCLK,            ///< Gating TV clock  (Gating PCLK only)
    MIPI_DSI_GCLK,      ///< Gating MIPI DSI clock  (Gating PCLK only)
    ISE_GCLK = 40,      ///< Gating ISE clock
    SIE2_GCLK,          ///< Gating SIE2 clock  (Gating PCLK only)
    AFFINE_GCLK,        ///< Gating AFFINE clock
    PWM_GCLK,           ///< Gating PWM clock
    PWMCCNT_GCLK,       ///<  Backward compatible
    RDE_GCLK = 45,      ///< Gating RDE clock
    DRE_GCLK,           ///< Gating DRE clock
    SDIO3_GCLK,         ///< Gating SDIO3 clock
    UART3_GCLK,         ///< Gating UART3 clock (Gating PCLK only)
    UART4_GCLK,         ///< Gating UART4 clock (Gating PCLK only)
    SPI4_GCLK,          ///< Gating SPI4 clock
    SPI5_GCLK,          ///< Gating SPI5 clock
    TGE_GCLK,           ///< Gating TGE clock (Gating PCLK only)
    SIE3_GCLK,          ///< Gating SIE3 clock (Gating PCLK only)
    SIE4_GCLK,          ///< Gating SIE4 clock (Gating PCLK only)
    RHE_GCLK,           ///< Gating RHE clock (Gating PCLK only)
    I2C3_GCLK,          ///< Gating I2C3 clock (Gating PCLK only)

    GPIO_GCLK = 64,     ///< Gating GPIO PCLK (Gating PCLK only)
    INTC_GCLK,          ///< Gating INTC PCLK (Gating PCLK only)
    DMA_GCLK = 67,      ///< Gating DMAC PCLK (Gating PCLK only)

    PCLKGAT_MAXNUM,

    ENUM_DUMMY4WORD(GATECLK)
}GATECLK;

/*
    @name Default Gating Clock Select definition

    This is for pll_configClkAutoGating() & pll_configPclkAutoGating().
*/
//@{
#define PLL_CLKSEL_DEFAULT_CLKGATE1     0x00000000
#define PLL_CLKSEL_DEFAULT_CLKGATE2     0x00000000
#define PLL_CLKSEL_DEFAULT_PCLKGATE1    0x00000000
#define PLL_CLKSEL_DEFAULT_PCLKGATE2    0x00000000
#define PLL_CLKSEL_DEFAULT_PCLKGATE3    0x00000000
//@}


/**
    PLL ID
*/
typedef enum
{
    PLL_ID_1,               ///< PLL1 (internal 480 MHz)
    PLL_ID_2,               ///< PLL2 (for video/HDMI)
    PLL_ID_3,               ///< PLL3 (for DMA)
    PLL_ID_4,               ///< PLL4 (for SSPLL)
    PLL_ID_5,               ///< PLL5 (for sensor/MCLK)
    PLL_ID_6,               ///< PLL6 (for IPP)
    PLL_ID_7,               ///< PLL7 (for audio)
    PLL_ID_8,               ///< PLL8 (for CPU)
    PLL_ID_9,               ///< PLL9 (for CPU2)
    PLL_ID_10,              ///< PLL10 (for DSP)
    PLL_ID_11,              ///< PLL11 (for DSI)
    PLL_ID_12,              ///< PLL12 (for ETH)
    PLL_ID_13,              ///< PLL13 (for H264)

    ENUM_DUMMY4WORD(PLL_ID)
} PLL_ID;

/**
    PLL Clock ID

    @note This is for pll_setClkFreq(), pll_getClkFreq(), pll_selectClkSrc(), pll_getClkSrc()
*/
typedef enum
{
    PLL_CLK_SIEMCLK,        ///< SIE MCLK
    PLL_CLK_SIEMCLK2,       ///< SIE MCLK2
    PLL_CLK_SPCLK,          ///< SP CLK
    PLL_CLK_SPCLK2,         ///< SP2 CLK

    ENUM_DUMMY4WORD(PLL_CLK)
} PLL_CLK;

/**
    PLL Clock Source

    @note This is for pll_selectClkSrc(), pll_getClkSrc()
*/
typedef enum
{
    PLL_CLKSRC_240,         ///< CLK source is 240MHz
    PLL_CLKSRC_480,         ///< CLK source is 480MHz
    PLL_CLKSRC_PLL2,        ///< CLK source is PLL2
    PLL_CLKSRC_PLL5,        ///< CLK source is PLL5


    PLL_CLKSRC_UNKNOWN,     ///< unkown clock source

    ENUM_DUMMY4WORD(PLL_CLKSRC)
} PLL_CLKSRC;


extern ER       pll_setPLL(PLL_ID id, UINT32 uiSetting);
extern BOOL     pll_getPLLEn(PLL_ID id);
extern ER       pll_setPLLEn(PLL_ID id, BOOL bEnable);
extern UINT32   pll_getPLLFreq(PLL_ID id);
extern ER       pll_setPLLSpreadSpectrum(PLL_ID id, UINT32 uiLowerFreq, UINT32 uiUpperFreq);
extern ER       pll_getPLLSpreadSpectrum(PLL_ID id, UINT32* puiLowerFreq, UINT32* puiUpperFreq);

extern void     pll_setPWMClockRate(UINT32 uiPWMNumber, UINT32 uiDiv);
extern BOOL     pll_isClockEnabled(CG_EN Num);

extern void     pll_setClkAutoGating(GATECLK ClkSel);
extern void     pll_clearClkAutoGating(GATECLK ClkSel);
extern BOOL     pll_getClkAutoGating(GATECLK ClkSel);

extern void     pll_setPclkAutoGating(GATECLK ClkSel);
extern void     pll_clearPclkAutoGating(GATECLK ClkSel);
extern BOOL     pll_getPclkAutoGating(GATECLK ClkSel);

extern ER       pll_setClkFreq(PLL_CLK clkID, UINT32 uiFreq);
extern ER       pll_getClkFreq(PLL_CLK clkID, UINT32* puiFreq);

extern ER       pll_selectClkSrc(PLL_CLK clkID, PLL_CLKSRC clkSrc);
extern ER       pll_getClkSrc(PLL_CLK clkID, PLL_CLKSRC* pClkSrc);

extern ER       pll_setClkEn(PLL_CLK clkID, BOOL bEnable);
extern ER       pll_getClkEn(PLL_CLK clkID, BOOL* pEnable);
//@}

#endif



