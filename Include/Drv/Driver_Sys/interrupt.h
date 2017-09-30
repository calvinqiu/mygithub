/**
    Header file for Interrupt module

    This file is the header file that define the API for Interrupt module.

    @file       interrupt.h
    @ingroup    mIDrvSys_Interrupt
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#ifndef _INTERRUPT_H
#define _INTERRUPT_H

#include "Type.h"
#include "cc.h"

// Interrupt number <= 32
//typedef UINT32          INT_PTN;
// Interrupt number > 32
typedef UINT64          INT_PTN;

/**
    @addtogroup mIDrvSys_Interrupt
*/
//@{

/**
    Interrupt configuration identifier

    @note For int_setConfig()
*/
typedef enum
{
    INT_CONFIG_ID_INT_DESTINATION = 0,              ///< Configured Interrupt exception destination to MIPS1/MIPS2/DSP
                                                    ///< @note
                                                    ///< Context is :
                                                    ///< - @b PINT_ID_DST : Point of destination table

    ENUM_DUMMY4WORD(INT_CONFIG_ID)
} INT_CONFIG_ID;

/**
    Interrupt module ID

    Interrupt module ID for int_getIRQId() and int_getDummyId().
*/
typedef enum
{
    INT_ID_TIMER,
    INT_ID_SIE,
    INT_ID_SIE2,
    INT_ID_SIE3,
    INT_ID_SIE4,
    INT_ID_IPE,
    INT_ID_IME,
    INT_ID_DCE,
    INT_ID_IFE,
    INT_ID_IFE2,
    INT_ID_DIS,
    INT_ID_FDE,
    INT_ID_RDE,
    INT_ID_RHE,
    INT_ID_DRE,
    INT_ID_DAI,
    INT_ID_H264,
    INT_ID_JPEG,
    INT_ID_GRAPHIC,
    INT_ID_GRAPHIC2,
    INT_ID_AFFINE,
    INT_ID_ISE,
    INT_ID_TGE,
    INT_ID_TSMUX,
    INT_ID_GPIO,
    INT_ID_REMOTE,
    INT_ID_PWM,
    INT_ID_USB,
    INT_ID_USB2,
    INT_ID_NAND,
    INT_ID_SDIO,
    INT_ID_SDIO2,
    INT_ID_SDIO3,
    INT_ID_DMA,
    INT_ID_ETHERNET,
    INT_ID_SPI,
    INT_ID_SPI2,
    INT_ID_SPI3,
    INT_ID_SPI4,
    INT_ID_SPI5,
    INT_ID_SIF,
    INT_ID_I2C,
    INT_ID_I2C2,
    INT_ID_UART,
    INT_ID_UART2,
    INT_ID_UART3,
    INT_ID_UART4,
    INT_ID_ADC,
    INT_ID_IDE,
    INT_ID_IDE2,
    INT_ID_DSI,
    INT_ID_MI,
    INT_ID_HDMI,
    INT_ID_VX1,
    INT_ID_LVDS,
    INT_ID_LVDS2,
    INT_ID_RTC,
    INT_ID_WDT,
    INT_ID_CG,
    INT_ID_CC,
    INT_ID_I2C3,

    INT_ID_MAX,
    ENUM_DUMMY4WORD(INT_ID)
} INT_ID;

/*
    Interrupt module ID

    Indicate which core will configured as destination

    @note For DMA_WRITEPROT_ATTR
*/


typedef union
{
    INT_PTN Reg;
    struct
    {
        //INT0
        UINT32  bInt_ID_TIMER:1;
        UINT32  bInt_ID_SIE:1;
        UINT32  bInt_ID_SIE2:1;
        UINT32  bInt_ID_SIE3:1;

        UINT32  bInt_ID_SIE4:1;
        UINT32  bInt_ID_IPE:1;
        UINT32  bInt_ID_IME:1;
        UINT32  bInt_ID_DCE:1;

        UINT32  bInt_ID_IFE:1;
        UINT32  bInt_ID_IFE2:1;
        UINT32  bInt_ID_DIS:1;
        UINT32  bInt_ID_FDE:1;

        UINT32  bInt_ID_RDE:1;
        UINT32  bInt_ID_RHE:1;
        UINT32  bInt_ID_DRE:1;
        UINT32  bInt_ID_DAI:1;


        //INT16
        UINT32  bInt_ID_H264:1;
        UINT32  bInt_ID_JPEG:1;
        UINT32  bInt_ID_GRAPHIC:1;
        UINT32  bInt_ID_GRAPHIC2:1;

        UINT32  bInt_ID_AFFINE:1;
        UINT32  bInt_ID_ISE:1;
        UINT32  bInt_ID_TGE:1;
        UINT32  bInt_ID_TSMUX:1;

        UINT32  bInt_ID_GPIO:1;
        UINT32  bInt_ID_REMOTE:1;
        UINT32  bInt_ID_PWM:1;
        UINT32  bInt_ID_USB:1;

        UINT32  bInt_ID_USB2:1;
        UINT32  bInt_ID_NAND:1;
        UINT32  bInt_ID_SDIO:1;
        UINT32  bInt_ID_SDIO2:1;

        //INT32
        UINT32  bInt_ID_SDIO3:1;
        UINT32  bInt_ID_DMA:1;
        UINT32  bInt_ID_ETHERNET:1;
        UINT32  bInt_ID_SPI:1;

        UINT32  bInt_ID_SPI2:1;
        UINT32  bInt_ID_SPI3:1;
        UINT32  bInt_ID_SPI4:1;
        UINT32  bInt_ID_SPI5:1;

        UINT32  bInt_ID_SIF:1;
        UINT32  bInt_ID_I2C:1;
        UINT32  bInt_ID_I2C2:1;
        UINT32  bInt_ID_UART:1;

        UINT32  bInt_ID_UART2:1;
        UINT32  bInt_ID_UART3:1;
        UINT32  bInt_ID_UART4:1;
        UINT32  bInt_ID_ADC:1;

        //INT48
        UINT32  bInt_ID_IDE:1;
        UINT32  bInt_ID_IDE2:1;
        UINT32  bInt_ID_DSI:1;
        UINT32  bInt_ID_MI:1;

        UINT32  bInt_ID_HDMI:1;
        UINT32  bInt_ID_VX1:1;
        UINT32  bInt_ID_LVDS:1;
        UINT32  bInt_ID_LVDS2:1;

        UINT32  bInt_ID_RTC:1;
        UINT32  bInt_ID_WDT:1;
        UINT32  bInt_ID_CG:1;
        UINT32  bInt_ID_CC:1;

        UINT32  bInt_ID_I2C3:1;
        UINT32  bReserved4:3;
     }Bit;
} INT_ID_DST, *PINT_ID_DST;

typedef struct
{
    INT_ID_DST  int_id_dst[CC_CORE_NUM];
} INT_DST, *PINT_DST;


// MIPI CSI share the interrupt ID with LVDS / HiSPi
#define INT_ID_CSI          INT_ID_LVDS
#define INT_ID_CSI2         INT_ID_LVDS2

// Macro to generate bit value from ID
#define INT_ID_TO_BIT(Id)   ((INT_PTN)(1) << (Id))

// check type and ID
STATIC_ASSERT(INT_ID_MAX <= (sizeof(INT_PTN) << 3));

#define INT_ALL_MODULES     ((sizeof(INT_PTN) == 4) ? 0xFFFFFFFFul : 0xFFFFFFFFFFFFFFFFull)

extern INT_PTN  int_getEnable(void);
extern INT_PTN  int_getFlag(void);
extern INT_ID   int_getIRQId(void);
extern INT_ID   int_getDummyId(void);
extern UINT32   int_getLatency(void);
extern void     int_setConfig(INT_CONFIG_ID ConfigID, UINT32 uiConfig);


//@}

#endif
