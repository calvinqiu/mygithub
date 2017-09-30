/**
    IPH module (RHE type define)

    @file       IPL_Hal_RHE.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_RHE_H
#define _IPL_Hal_RHE_H
#include "IPL_Hal_RHE_Info.h"
#include "IPL_ISREvent.h"
/** \addtogroup mILibIPH */
//@{

/**
    query enum

    read from hw register
*/
typedef enum _IPH_RHE_QUE
{
    RHE_REG_CH0CURRDYADDR   = 0x00000006,   ///< read rhe current dma ready address                 Data type: UINT32
    RHE_REG_CH1CURRDYADDR   = 0x00000009,   ///< read rhe current dma ready address LA              Data type: UINT32
    ENUM_DUMMY4WORD(IPH_RHE_QUE)
}IPH_RHE_QUE;

/**
     enum of RHE update selection
*/
typedef enum _RHE_UPDATE
{
    RHE_DMA_IN          = 0x00000001,   ///< update input address
    RHE_DMA_OUT         = 0x00000002,   ///< update output address
    RHE_RAW_SIZE        = 0x00000004,   ///< update raw size
    RHE_DARK_FRAME_SUB  = 0x00000008,   ///< update dark frame subtration
    RHE_HDR_RAD         = 0x00000010,   ///< update hdr radiance
    RHE_HDR_REGU_INFO   = 0x00000020,   ///< update hdr regulation information
    RHE_HDR_RECRV_INFO  = 0x00000040,   ///< update hdr recurve information
    RHE_HDR_REF_LA_LPF  = 0x00000080,   ///< update hdr reference la low-pass-filter
    RHE_HDR_GBL_TONE    = 0x00000100,   ///< update hdr global tone
    RHE_HDR_LOC_TONE    = 0x00000200,   ///< update hdr local tone
    RHE_INIT            = 0x00000400,   ///< init rhe
    RHE_SUBF_EN         = 0x00000800,   ///< update func enable/disable
    RHE_CROP_INFO       = 0x00001000,   ///< update hdr crop information
    RHE_IN_LA_INFO      = 0x00002000,   ///< update LA information
    RHE_FLICKER_INFO    = 0x00004000,   ///< update flicker detection information
    ENUM_DUMMY4WORD(RHE_UPDATE)
}RHE_UPDATE;
/**
     enum of RHE read selection

     read from inner table, not from hw register.
*/
typedef enum _RHE_READ
{
    RHE_R_SUBF_EN   = 0x00000001,   ///< read func enable/disable,                          Data type: RheFuncEn
    RHE_R_INTE_EN   = 0x00000002,   ///< read rhe interrupt enable bit                      Data type: RHE_INTE_EN
    RHE_R_RAW_SIZE  = 0x00000003,   ///< read rhe input size                                Data type: RHE_SIZE
    ENUM_DUMMY4WORD(RHE_READ)
}RHE_READ;

typedef enum
{
    RHE_STAGGERED_HDR = 0, ///< Staggered HDR
    RHE_CURVED_HDR    = 1, ///< Curved HDR
    RHE_DFS           = 2, ///< Dark Frame Subtraction
    ENUM_DUMMY4WORD(RHE_MAIN_SEL)
} RHE_MAIN_SEL;

typedef enum
{
    RHE_TO_DRAM        = 0, ///< DRAM->RHE->DRAM
    RHE_TO_IFE         = 1, ///< DRAM->RHE->IFE
    ENUM_DUMMY4WORD(RHE_OUPUT_PATH_SEL)
} RHE_OUPUT_PATH_SEL;

/**
    Luminance accumulation information
*/
typedef struct _RHE_LAPARAM
{
    UINT32 WinNumH;      ///< la window number(hor.)
    UINT32 WinNumV;      ///< la window number(ver.)
    UINT32 WinSizeH;     ///< la window size(hor.)
    UINT32 WinSizeV;     ///< la window size(ver.)
    UINT32 WinSum;       ///< la window sum(only valid at HDR mode)
}RHE_LAPARAM;
/**
     RHE configuration

     all RHE configuration including all RHE sub function
*/
typedef struct _RHE_Manager
{
    RHE_MAIN_SEL                MainSel;                    ///< RHE operation mode
    RHE_OUPUT_PATH_SEL          OutPathSel;                 ///< RHE output path
    RHE_RAW_DECODE_INFO         RheDecodeInfo;              ///< RHE decode information
    RHE_CROP                    RawCrop;                    ///< RHE crop infomation(virtual)
    RHE_SIZE                    RawSize;                    ///< RHE input raw size
    RHE_LAPARAM                 LaInfo;                     ///< RHE input la information
    RHE_Dma_In                  DmaInputInfo;               ///< RHE dma input information
    RHE_Dma_Out                 DmaOutInfo;                 ///<
    RHE_SUBFUNC                 *pSubFunc;                  ///<
}RHE_Manager,*pRHE_Manager;

/**
     RHE configuration
*/
typedef RHE_Manager RHE;

//@}
#endif
