/**
    IPH module (type define)

    @file       IPL_Hal.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef IPL_HAL_H_
#define IPL_HAL_H_
#include "IPL_Hal_IFE2.h"
#include "IPL_Hal_IFE.h"
#include "IPL_Hal_SIE_Com.h"
#include "IPL_Hal_DCE.h"
#include "IPL_Hal_IPE.h"
#include "IPL_Hal_IME.h"
#include "IPL_Hal_RDE.h"
#include "IPL_Hal_ISE.h"
#include "IPL_Hal_RHE.h"
#include "IPL_Hal_DRE.h"


/** \addtogroup mILibIPH */
//@{

/**
     engine supported by IPL_Hal
*/
typedef enum _IPL_HAL_ENG
{
    IPL_HAL_UNKNOW  = 0,
    IPL_HAL_SIE1    = 1,    ///< SIE1
    IPL_HAL_SIE2    = 2,    ///< SIE2
    IPL_HAL_SIE3    = 3,    ///< SIE3
    IPL_HAL_SIE4    = 4,    ///< SIE4
    IPL_HAL_RDE     = 5,    ///< RDE
    IPL_HAL_IFE     = 6,    ///< IFE
    IPL_HAL_DCE     = 7,    ///< DCE
    IPL_HAL_IPE     = 8,    ///< IPE
    IPL_HAL_IME     = 9,    ///< IME
    IPL_HAL_IFE2    = 10,   ///< IFE2
    IPL_HAL_ISE     = 11,   ///< ISE
    IPL_HAL_RHE     = 12,   ///< RHE
    IPL_HAL_DRE     = 13,   ///< DRE
    IPL_HAL_MAX     = 14,
    ENUM_DUMMY4WORD(IPL_HAL_ENG)
}IPL_HAL_ENG;

/**
    engine clk
    @note : sie engine clk set to 0 when the protocal of sensor data transition is LVDS
*/
typedef union _IPH_CLK
{
    UINT32      SIEclk;     ///< SIE  clock
    IPH_IPECLK  IPEclk;     ///< IPE  clock
    IPH_DCECLK  DCEclk;     ///< DCE  clock
    IPH_IFECLK  IFEclk;     ///< IFE  clock
    IPH_IFE2CLK IFE2clk;    ///< IFE2 clock
    IPH_IMECLK  IMEclk;     ///< IME  clock
    IPH_RDECLK  RDEclk;     ///< RDE  clock
    UINT32      SIE2clk;    ///< SIE2 clock
    IPH_ISECLK  ISEclk;     ///< ISE  clock
    UINT32      SIE3clk;    ///< SIE3 clock
    UINT32      SIE4clk;    ///< SIE4 clock
    IPH_RHECLK  RHEclk;     ///< RHE  clock
    UINT32      DREclk;     ///< DRE  clock
    UINT32 EngineClk;
}IPH_CLK;

/**
     engine para. update selection
*/
typedef union _IPL_HAL_GROUP
{
    SIE_UPDATE  sieUpdate;  ///< SIE update selection
    IFE_UPDATE  ifeUpdate;  ///< IFE update selection
    IPE_UPDATE  ipeUpdate;  ///< IPE update selection
    DCE_UPDATE  dceUpdate;  ///< DCE update selection
    IFE2_UPDATE ife2Update; ///< IFE2 update selection
    IME_UPDATE  imeUpdate;  ///< IME update selection
    RDE_UPDATE  rdeUpdate;  ///< RDE update selection
    ISE_UPDATE  iseUpdate;  ///< ISE update selection
    RHE_UPDATE  rheUpdate;  ///< RHE update selection
    DRE_UPDATE  dreUpdate;  ///< DRE update selection
    UINT64 data;
}IPL_HAL_GROUP;

/**
     engine sub-function
*/
typedef struct _ENG_SUBFUNC
{
    SIE_SUBFUNC*    pSie_sub;   ///< -
    IFE_SUBFUNC*    pIfe_sub;   ///< -
    DCE_SUBFUNC*    pDce_sub;   ///< -
    IPE_SUBFUNC*    pIpe_sub;   ///< -
    IFE2_SUBFUNC*   pIfe2_sub;  ///< -
    IME_SUBFUNC*    pIme_sub;   ///< -
    RDE_SUBFUNC*    pRde_sub;   ///< -
    ISE_SUBFUNC*    pIse_sub;   ///< -
    RHE_SUBFUNC*    pRhe_sub;   ///< -
    DRE_SUBFUNC*    pDre_sub;   ///< -
}ENG_SUBFUNC;


/**
     Index of parameter set supported by IPL_Hal
*/
typedef enum _IPL_HAL_PARASET
{
    IPL_HAL_PAR0    = 0,        ///< -
    IPL_HAL_PAR1    = 1,        ///< -
    IPL_HAL_PARMAX,             ///< -
    ENUM_DUMMY4WORD(IPL_HAL_PARASET)
}IPL_HAL_PARASET;

//@}
#endif