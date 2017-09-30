/**
    IPL_Ctrl module (ISR Event)

    @file       IPL_ISREvent.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_ISREVENT_H_
#define _IPL_ISREVENT_H_
#include "Type.h"
#include "IPL_Utility.h"
#include "IPL_Hal_IME_Info.h"
/** \addtogroup mILibIPH */
//@{
typedef void (*DRV_ISR)(UINT32);//input is ISR status
//1st is process id, 2nd UINT32 is isr status,3rd UINT32 is img idx which start at 1 during streaming
typedef void (*IPL_ISR_EVENT_FP)(IPL_PROC_ID,UINT32,UINT32);
//1st is process id, 2nd UINT32 img idx which start at 1 during streaming
typedef void (*IPL_ISR_RAWHEADER_CB)(IPL_PROC_ID, UINT32);

typedef void (*IPL_H264_CB)(IMESTAMP_INFO *, UINT32, UINT32);
/**
    Engine ISR enum
*/
typedef enum _IPL_ISR
{
    IPL_ISR_UNKNOWN = 0,
    IPL_ISR_SIE1 = 1,   ///< SIE1
    IPL_ISR_SIE2 = 2,   ///< SIE2
    IPL_ISR_SIE3 = 3,   ///< SIE3
    IPL_ISR_SIE4 = 4,   ///< SIE4
    IPL_ISR_IFE  = 5,   ///< IFE
    IPL_ISR_IPE  = 6,   ///< IPE
    IPL_ISR_IME  = 7,   ///< IME
    IPL_ISR_IFE2 = 8,   ///< IFE2
    IPL_ISR_DCE  = 9,   ///< DCE
    IPL_ISR_RDE  = 10,  ///< RDE
    IPL_ISR_RHE  = 11,  ///< RHE
    IPL_ISR_MAX_CNT = 12,
    IPL_ISR_SIEx = 0x0000ffff, ///< for internal auto set
    ENUM_DUMMY4WORD(IPL_ISR)
} IPL_ISR;

/**
     isr status
*/
typedef enum _IPL_ISR_STATUS
{
    //ISR_REV         = 0,
    SIE_FLDEND      = 0x00000001,   ///< Field end
    SIE_BP1         = 0x00000002,   ///< Break point 1
    SIE_BP2         = 0x00000004,   ///< Break point 2
    SIE_BP3         = 0x00000008,   ///< Break point 3
    SIE_ACTST       = 0x00000010,   ///< Active window start
    SIE_CRPST       = 0x00000020,   ///< Crop window start
    SIE_DRAM_IO_ERR = 0x00000040,   ///< Enable interrupt
    SIE_DRAM0_END   = 0x00000080,   ///< Dram output channel 0 end
    SIE_DRAM1_END   = 0X00000100,   ///< Dram output channel 1 end
    SIE_DRAM2_END   = 0X00000200,   ///< Dram output channel 2 end
    SIE_DRAM3_END   = 0X00000400,   ///< Dram output channel 3 end
    SIE_DRAM4_END   = 0X00000800,   ///< Dram output channel 4 end
    SIE_DPCF        = 0X00001000,   ///< Defect Pixel/Column concealment warning,happened at continus bp.(more than 2 bp)
    SIE_RAWENC_BUFF = 0X00002000,   ///< Raw encoder output buffer full

    IPE_FMD         = 0x00000002,   ///< frame end
    IPE_STPE        = 0X00000004,   ///< stripe end
    IPE_FMS         = 0X00000008,   ///< stripe end
    IPE_YCC_DMAE    = 0x00000010,   ///< ycc dma output end

    IFE_FMD         = 0x00000001,   ///< frame end interrupt
    IFE_ROWDEFW     = 0x00000004,   ///< row defect warning interrupt

    IFE2_FMD        = 0x80000000,   ///<Frame end

    IME_STPE        = 0X40000000,   ///< stripe end
    IME_FMD         = 0x80000000,   ///< frame end

    DCE_FMS         = 0x00000001,   ///< frame start
    DCE_FMD         = 0x00000002,   ///< frame end
    DCE_STRPE       = 0x00000004,   ///< current stripe end

    RDE_FMD         = 0x00000001,   ///< frame end
    RDE_ERR         = 0x00000002,   ///< decode err(under/over flow,bitstream err)

    ISE_FMD         = 0x80000000,  ///< frame-end interrupt status

    RHE_FMD         = 0x00000001,   ///< frame end
    RHE_DPC_WARN    = 0x00000002,   ///< Defect Concealment warning
    RHE_IN0_DEC_ERR = 0x00000004,   ///< Decoder error for input-0
    RHE_IN1_DEC_ERR = 0x00000008,   ///< Decoder error for input-1
}IPL_ISR_STATUS;

//@}
#endif //_IPL_ISREVENT_H_