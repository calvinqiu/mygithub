/**
    IPH module (IME type define)

    @file       IPL_Hal_IME.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_IME_H
#define _IPL_Hal_IME_H
#include "IPL_Hal_IME_Info.h"
#include "IPL_ISREvent.h"
/** \addtogroup mILibIPH */
//@{

/**
    query enum

    read from hw register
*/
typedef enum _IPH_IME_QUE
{
    IME_GET_STATISTICS_INFO = 0x00000001,   ///< read statistics information from reg.      Data type: UINT32
    IME_GET_SSR_EN          = 0x00000002,   ///< read SSR on/off form reg.                  Data type: UINT32
    ENUM_DUMMY4WORD(IPH_IME_QUE)
}IPH_IME_QUE;

/**
     enum of IME update selection
*/
typedef enum _IME_UPDATE
{
    IME_IN_ADDR             = 0X0000000000000001LLU,   ///< update input addr
    IME_OUT_ADDR_P1         = 0x0000000000000002LLU,   ///< update path1 output addr
    IME_OUT_ADDR_P2         = 0x0000000000000004LLU,   ///< update path2 output addr
    IME_OUT_ADDR_P3         = 0x0000000000000008LLU,   ///< update path3 output addr
    IME_OUT_ADDR_P4         = 0x0000000000000010LLU,   ///< update path4 output addr
    IME_IN_SIZE             = 0X0000000000000020LLU,   ///< update input size/lineoffset
    IME_OUT_SIZE_P1         = 0X0000000000000040LLU,   ///< update path1 output scale size/lineoffset/crop window
    IME_OUT_SIZE_P2         = 0X0000000000000080LLU,   ///< update path2 output scale size/lineoffset/crop window
    IME_OUT_SIZE_P3         = 0X0000000000000100LLU,   ///< update path3 output scale size/lineoffset/crop window
    IME_OUT_SIZE_P4         = 0X0000000000000200LLU,   ///< update path4 output scale size/lineoffset/crop window
    IME_PATHEN_P1           = 0x0000000000000400LLU,   ///< update path1 enable/disable
    IME_PATHEN_P2           = 0x0000000000000800LLU,   ///< update path2 enable/disable
    IME_PATHEN_P3           = 0x0000000000001000LLU,   ///< update path3 enable/disable
    IME_PATHEN_P4           = 0x0000000000002000LLU,   ///< update path4 enable/disable
    IME_SCALE_METHOD        = 0x0000000000004000LLU,   ///< update path1/2/3/4 scale method
    IME_INIT                = 0x0000000000008000LLU,   ///< init ime
    IME_OUT_STITCH_P1       = 0x0000000000010000LLU,   ///< update path1 output stitch information
    IME_OUT_STITCH_P2       = 0x0000000000020000LLU,   ///< update path2 output stitch information
    IME_OUT_STITCH_P3       = 0x0000000000040000LLU,   ///< update path3 output stitch information
    IME_OUT_STITCH_ADDR_P1  = 0x0000000000080000LLU,   ///< update path1 output stitch address
    IME_OUT_STITCH_ADDR_P2  = 0x0000000000100000LLU,   ///< update path2 output stitch address
    IME_OUT_STITCH_ADDR_P3  = 0x0000000000200000LLU,   ///< update path3 output stitch address
    IME_SUBFEN              = 0x0000000000400000LLU,   ///< update function enable/disable
    IME_DATASTATS_EDGE      = 0x0000000000800000LLU,   ///< update Edge map of statistic
    IME_DATASTATS_HISTO     = 0x0000000001000000LLU,   ///< update Edge map of Historgram
    IME_DATASTATS_ROI       = 0x0000000002000000LLU,   ///< update ROI threshold of edge map
    IME_DATASTAMPLUT        = 0x0000000004000000LLU,   ///< update Data Stamp tab Y/U/V channel LUT, 16 elements &  stamp 0, 1, 2, 3 color key
    IME_DATASTAMP_S0        = 0x0000000008000000LLU,   ///< update Data Stamp set 0 image information
    IME_DATASTAMP_S1        = 0x0000000010000000LLU,   ///< update Data Stamp set 1 image information
    IME_DATASTAMP_S2        = 0x0000000020000000LLU,   ///< update Data Stamp set 2 image information
    IME_DATASTAMP_S3        = 0x0000000040000000LLU,   ///< update Data Stamp set 3 image information
    IME_CHROMAA_IN          = 0x0000000080000000LLU,   ///< update chroma adaption input information
    IME_CHROMAA             = 0x0000000100000000LLU,   ///< update chroma adaption
    IME_CHROMAA_LUM         = 0x0000000200000000LLU,   ///< update chroma lumaination adaption
    IME_CHROMAA_CLR         = 0x0000000400000000LLU,   ///< update chroma color adaption
    IME_CHROMAA_CTR_UV      = 0x0000000800000000LLU,   ///< update chroma adjust6ment center for UV channel(data from IFE2)
    IME_CHROMADB_SUP        = 0x0000001000000000LLU,   ///< update chroma suppression.
    IME_SUPER_RES           = 0x0000002000000000LLU,   ///< update super resoltuion info
    IME_FILMG               = 0x0000004000000000LLU,   ///< update film grain
    IME_SCALE_IQ_P1         = 0x0000008000000000LLU,   ///< update path1 scale enhancement & filter parameter set
    IME_SCALE_IQ_P2         = 0x0000010000000000LLU,   ///< update path2 scale enhancement & filter parameter set
    IME_SCALE_IQ_P3         = 0x0000020000000000LLU,   ///< update path3 scale enhancement & filter parameter set
    IME_SCALE_IQ_P4         = 0x0000040000000000LLU,   ///< update path4 scale enhancement & filter parameter set
    IME_EDGE_OUT_ADDR       = 0x0000080000000000LLU,   ///< update path4 edge map output address
    IME_HISTO_OUT_ADDR      = 0x0000100000000000LLU,   ///< update path4 histo output address
    IME_STP_INFO            = 0x0000200000000000LLU,   ///< update ime stripe information
    IME_DATASTATS_CTRL      = 0x0000400000000000LLU,   ///< update ime statistic information
} IME_UPDATE;

/**
     enum of IME read selection

     read from inner table, not from hw register.
*/
typedef enum _IME_READ
{
    IME_R_IO            = 0x00000001,   ///< read input info. exclude IME_SUBFUNC,          Data type: IME
    IME_R_SUBFEN        = 0X00000002,   ///< read func enable/disable,                      Data type: ImeFuncEn
    IME_R_PATHEN        = 0x00000003,   ///< read path enable/disable.                      Data type: IME_OUT
    IME_R_CHROMAA       = 0X00000004,   ///< read chromaAdaption,                           Data type: IME_CHRA_ADAP
    IME_R_CHROMAA_IN    = 0x00000005,   ///< read chormaAdaption input infomation           Data type: UINT32
    IME_R_CHROMAA_LUM   = 0X00000006,   ///< read chromaAdaption,lumiance suppress          Data type: IME_CHRA_LADAP
    IME_R_CHROMAA_CLR   = 0X00000007,   ///< read chromaAdaption,chroma adjust              Data type: IME_CHRA_CADAP
    IME_R_FILMG         = 0X00000008,   ///< read film grain,                               Data type: IME_FG
    IME_R_CSPACE        = 0X00000009,   ///< read color space transfor,                     Data type: IME_CSPACE
    IME_R_SCALEQ_ENH_P1 = 0X0000000A,   ///< read path1 scaling enhancement                 Data type: IMESCALE_Enh
    IME_R_SCALEQ_ENH_P2 = 0X0000000B,   ///< read path2 scaling enhancement                 Data type: IMESCALE_Enh
    IME_R_SCALEQ_ENH_P3 = 0X0000000C,   ///< read path3 scaling enhancement                 Data type: IMESCALE_Enh
    IME_R_SCALEQ_ENH_P4 = 0X0000000D,   ///< read path4 scaling enhancement                 Data type: IMESCALE_Enh
    IME_R_SCALEQ_F_P1   = 0X0000000E,   ///< read path1 scaling filter                      Data type: IMESCALE_Filt
    IME_R_SCALEQ_F_P2   = 0X0000000F,   ///< read path2 scaling filter                      Data type: IMESCALE_Filt
    IME_R_SCALEQ_F_P3   = 0X00000010,   ///< read path3 scaling filter                      Data type: IMESCALE_Filt
    IME_R_SCALEQ_F_P4   = 0X00000011,   ///< read path4 scaling filter                      Data type: IMESCALE_Filt
    IME_R_PATH1INFO     = 0x00000012,   ///< read path1 size/fmt/crop                       Data type: OUTCH
    IME_R_PATH2INFO     = 0x00000013,   ///< read path2 size/fmt/crop                       Data type: OUTCH
    IME_R_PATH3INFO     = 0x00000014,   ///< read path3 size/fmt/crop                       Data type: OUTCH
    IME_R_PATH4INFO     = 0x00000015,   ///< read path4 size/fmt/crop                       Data type: OUTCH
    IME_R_PATH1ADDR     = 0x00000016,   ///< read path1 output address                      Data type: IMEADDR
    IME_R_PATH2ADDR     = 0x00000017,   ///< read path2 output address                      Data type: IMEADDR
    IME_R_PATH3ADDR     = 0x00000018,   ///< read path3 output address                      Data type: IMEADDR
    IME_R_PATH4ADDR     = 0x00000019,   ///< read path4 output address                      Data type: IMEADDR
    IME_R_INTEEN        = 0x00000020,   ///< read interruppt enabe bit,                     Data type: ImeInteEn
    IME_R_PATHALL_SCALM = 0x00000021,   ///< read scale method for all path,                Data type: IME_SCALER array[all path]
    ENUM_DUMMY4WORD(IME_READ)
}IME_READ;

/**
     IME configuration

     all IME configuration including all IME sub function
*/
typedef struct _IME_Manager
{
    IME_SRCMODE IME_Ctrl;     ///< IME operation mode
    struct
    {
        struct
        {
            BOOL ManualEn;      ///< -
            IME_STRIPE  Horn;   ///< Input image H stripe
            //IME_STRIPE  Ver;    ///< Input image V stripe
        }Manual;
        IME_STPESEL HornSel;    ///< stripe overlap selection
        //IME_STPESEL VerSel;
    }STRP;                      ///< effective when (IME_Ctrl != IME_Ctrl_IPP)

    struct
    {
        struct
        {
            IMEADDR PPB0;       ///< PPB0 address
        }ADDR;                  ///< address
        IMESIZE Size;           ///< input size
        IMEFMT Fmt;             ///< input format
    }IME_In;                    ///< ime input information

    IME_OUT IME_Out;            ///< ime output info

    IME_SUBFUNC     *pSubFunc;  ///< -
    //reverse
    //UINT32 rev;               //reserve for IPH paraset idx
}IME_Manager,*pIME_Manager;

/**
     IME configuration
*/
typedef IME_Manager IME;
//@}
#endif
