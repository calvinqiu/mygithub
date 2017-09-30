/**
    IPH module (RDE sub function type define)

    @file       IPL_Hal_RDE_Info.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _IPL_Hal_RDE_Info_H_
#define _IPL_Hal_RDE_Info_H_
#include "Type.h"
#include "IPL_Utility.h"
#include "pll.h"
/** \addtogroup mILibIPH */
//@{

/**
    RDE engine supported clk
*/
typedef enum _IPH_RDECLK
{
    IPH_RDECLK0 = 120,   ///< 120 MHZ
    IPH_RDECLK1 = 160,   ///< 160 MHZ
    IPH_RDECLK2 = 192,   ///< 192 MHZ
    IPH_RDECLK3 = 240,   ///< 240 MHz
    ENUM_DUMMY4WORD(IPH_RDECLK)
}IPH_RDECLK;

/**
    RDE interrupt status enum.
*/
typedef enum _RDE_INTE_Status
{
    RDE__INT_DEC_END       =   0x00000001,                 ///< status of RDE interrupt of frame end
    RDE__INT_DEC_ERR       =   0x00000002,                 ///< status of RDE interrupt of coding error
    RDE__INT_DEC_ALL       =   0x00000003,                 ///< status of RDE all interrupts
    ENUM_DUMMY4WORD(RDE_INTE_Status)
}RDE_INTE_Status;


/**
    RDE sub function enable
*/
typedef enum _RDEFuncEn
{
    RDE_VFlipEn     =   0x00000001, ///< enable RDE function: Vertical flip
    RDE_HFlipEn     =   0x00000002, ///< enable RDE function: Horizontal flip
    RDE_DeGammaEn   =   0x00000004, ///< enable RDE function: Decode gamma
    ENUM_DUMMY4WORD(RDEFuncEn)
}RDEFuncEn;

/**
    Engine Source operation mode: Line/Frame
*/
typedef enum
{
    RDE_LineMode      = 0,         ///< encoder use line mode
    RDE_FrameMode     = 1,         ///< encoder use frame mode
    ENUM_DUMMY4WORD(RDE_OPMODE)
} RDE_OPMODE;

/**
    Engine Source operation mode: 1st/2nd Stripe
*/
typedef enum
{
    RDE_SSTRP        = 0,         ///< encoder use Single stripe
    RDE_MSTRP        = 1,         ///< encoder use Multiple stripe
    ENUM_DUMMY4WORD(RDE_STRPMODE)
} RDE_STRPMODE;

/**
    Stripe ID
*/
typedef enum
{
    RDE_STRP_1ST = 0,            ///< Line mode first stripe
    RDE_STRP_2ND = 1,            ///< Line mode second stripe
    ENUM_DUMMY4WORD(RDE_STRPID)
}RDE_STRPID;

/**
    Struct RDE image information (line mode)
*/
typedef struct _RDE_IMG_INFO
{
    UINT32 Width;               ///< image width
    UINT32 Height;              ///< image height
    UINT32 LineOfs;             ///< line offset
} RDE_IMG_INFO;

/**
    Struct RDE Crop Point
*/
typedef struct _RDE_CROP
{
    UINT32 x;          ///< Left point X of Crop image
    UINT32 y;          ///< Left point Y of Crop image
}RDE_CROP;

/**
    Struct RDE BS information (frame mode)
*/
typedef struct _RDE_BS_INFO
{
    UINT32 Addr;        ///< address of bitstream
    UINT32 Size;        ///< size of bitstream
}RDE_BS_INFO;

/**
     RDE sub function configuration
*/
typedef struct _RDE_SUBFUNC
{
    RDEFuncEn   RDEFuncEn;      ///< enable RDE sub function
    UINT32      GammaTblAddr;   ///< pointer of degamma table; type:UINT32, tab num: 33
}RDE_SUBFUNC;

//@}
#endif
