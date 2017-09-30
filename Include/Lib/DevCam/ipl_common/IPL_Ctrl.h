/**
    IPL_Ctrl module(D2D,dram to dram)

    This D2D module use 3 opeartion to model IPL D2D operation:
\n  IPC_D2D_Start   : start engine d2d behavior
\n  IPC_WaitFinish  : wait engine d2d behavior done
\n  IPC_D2D_End     : close engine.

    @file       IPL_Ctrl.h
    @ingroup    mILibIPH
    @note
\n  example of standard operation:
\n
\n          IPC_D2D_Start(...);
\n          IPC_WaitFinish(...);
\n          IPC_D2D_End(...);

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _IPL_CTRL_H_
#define _IPL_CTRL_H_
#include "IPL_Ctrl_Par.h"
/** \addtogroup mILibIPH */
//@{

/**
     D2D result location.

     decribe current ready data is in InputAddress or OutputAddress.
*/
typedef enum _DataDest
{
    _InputADDR  = 0,    ///< d2d output data in inputaddr
    _OutputADDR = 1,    ///< d2d output data in outputaddr
    ENUM_DUMMY4WORD(DataDest)
}DataDest;


/**
     SIE data structure for SIE_D2DWait

     SIE D2D output information.
*/
typedef struct _IPLC_SIE_OUT_INFO
{
    DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_SIE_OUT_INFO;

/**
     IFE data structure for IFE_D2DWait

     IFE D2D output information.
*/
typedef struct _IPLC_IFE_OUT_INFO
{
    DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_IFE_OUT_INFO;

/**
     DCE data structure for DCE_D2DWait

     DCE D2D output information.
*/
typedef struct _IPLC_DCE_OUT_INFO
{
    DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_DCE_OUT_INFO;

/**
     IPE data structure for IPE_D2DWait

     IPE D2D output information.
*/
typedef struct _IPLC_IPE_OUT_INFO
{
    DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_IPE_OUT_INFO;

/**
     IME data structure for IME_D2DWait

     IME D2D output information.
*/
typedef struct _IPLC_IME_OUT_INFO
{
    DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_IME_OUT_INFO;


/**
     ISE data structure for ISE_D2DWait

     ISE D2D output information.
*/
typedef struct _IPLC_ISE_OUT_INFO
{
    DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_ISE_OUT_INFO;

/**
     RDE data structure for RDE_D2DWait

     RDE D2D output information.
*/
typedef struct _IPLC_RDE_OUT_INFO
{
    DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_RDE_OUT_INFO;

/**
     DRE data structure for DRE_D2DWait

     DRE D2D output information.
*/
typedef struct _IPLC_DRE_OUT_INFO
{
    DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_DRE_OUT_INFO;

/**
     RHE data structure for RHE_D2DWait

     RHE D2D output information.
*/
typedef struct _IPLC_RHE_OUT_INFO
{
    DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_RHE_OUT_INFO;

/**
     IPLC_IFE2 data structure for IFE2_D2DWait

     IFE2 D2D output information.
*/
typedef struct _IPLC_IFE2_OUT_INFO
{
    DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
    IFE2_GRAY_AVG GrayAvg;          ///< information for IME LCA
} IPLC_IFE2_OUT_INFO;

/**
     IPLC_IFE2IME data structure for IFE2IME_D2DWait

     IFE2IME D2D output information.
*/
typedef struct _IPLC_IFE2IME_OUT_INFO
{
    DataDest OutIdx;                 ///< output index(_OutputADDR, or _InputAddr)
} IPLC_IFE2IME_OUT_INFO;

typedef union _D2DResult
{
    IPLC_SIE_OUT_INFO SIE;               ///< for SIE d2d
    IPLC_IFE_OUT_INFO IFE;               ///< for IFE d2d
    IPLC_DCE_OUT_INFO DCE;               ///< for DCE d2d
    IPLC_IPE_OUT_INFO IPE;               ///< for IPE d2d
    IPLC_IME_OUT_INFO IME;               ///< for IME d2d
    IPLC_IFE2_OUT_INFO IFE2;             ///< for IFE2 d2d
    IPLC_ISE_OUT_INFO ISE;               ///< for ISE d2d
    IPLC_RDE_OUT_INFO RDE;               ///< for RDE d2d
    IPLC_IFE2IME_OUT_INFO IFE2IME;       ///< for IFE2IME d2d
    IPLC_DRE_OUT_INFO DRE;               ///< for DRE d2d
    IPLC_RHE_OUT_INFO RHE;               ///< for RHE d2d
} D2DResult;

/**
     Supported D2D operation
*/
typedef enum _IPLC_D2D
{
    IPLC_IPE = 0,        ///< IPE D2D
    IPLC_DCE,        ///< DCE D2D
    IPLC_IFE,        ///< IFE D2D
    IPLC_IFE2,       ///< IFE2 D2D
    IPLC_IME,        ///< IME D2D
    IPLC_RDE,        ///< RDE D2D
    IPLC_IFEIME,     ///< IFE IN, IME OUT(all engine direct path)
    IPLC_DRE,        ///< DRE D2D
    IPLC_SIE,        ///< SIE D2D
    IPLC_ISE,        ///< ISE D2D
    IPLC_RHE,        ///< RHE D2D
    IPLC_MAX,
    ENUM_DUMMY4WORD(IPLC_D2D)
}IPLC_D2D;

/**
     IPLC_DRE data structure

     DRE D2D configuration. (only reference input[0] image size & format)
*/
typedef struct _IPLC_DRE
{
    IPL_HAL_PARASET ParIdx; ///< set configuration into indicated IPH inner table
    UINT32 eventID;         ///< map to DRE_D2D_CB eventID if DRE_D2D_CB exist

    DRE_IN_FMT fmt;         ///< input format
    BOOL HDRMode;           ///< Hdr mode
    DRE_IMG_INFO In[4];     ///< Y1, UV1, Y2, UV2
                            ///< input image information (HDRMode = 0 only 1 input image), (HDRMode = 1, 2 input image)
    DRE_IMG_INFO Out[2];       ///< output always 422 format
    UINT32 WorkBufAddr;     ///< working buffer adderss & size
    UINT32 WorkBufSize;     ///< working buffer adderss & size
    fpEngInit fp_Init;      ///< this CB will replace eventID's CB if existed.
}IPLC_DRE_INFO;

/**
     IPLC_SIE data structure

     SIE D2D configuration.
*/
typedef struct _IPLC_SIE
{
    IPL_HAL_PARASET ParIdx; ///< set configuration into indicated IPH inner table
    UINT32 eventID;         ///< map to SIE_D2D_CB eventID if SIE_D2D_CB exist
    UINT32 clk;             ///< engine clk freq.

    IPL_RAW_IMG_INFO  In;   ///< dram input info.
    IPL_RAW_IMG_INFO Out;   ///< dram output info.

    fpEngInit   fp_Init;    ///< this CB will replace eventID's CB if existed.
}IPLC_SIE_INFO;
/**
     IPLC_IPE data structure

     IPE D2D configuration.
*/
typedef struct _IPLC_IPE
{
    IPL_HAL_PARASET ParIdx;     ///< -
    UINT32 eventID;             ///< map to IPE_D2D_CB eventID if IPE_D2D_CB exist
    IPH_IPECLK  clk;            ///< engine clk freq.

    //in
    IPE_SIZE InSizeY;         ///< in Y information
    UINT32 InLofsC;           ///< in C lineoffset
    IPEFmt InFmt;             ///< in format
    UINT32 InAddr[2];         ///< in addr (Image: idx0 = y, idx1 = uv, parameter: idx0 = RGB gamma, idx1 = 3DCC, idx2 = Y gamma)

    //out
    IPE_SIZE OutSizeY;         ///< out Y information
    UINT32 OutLofsC;           ///< out C lineoffset
    IPEFmt OutFmt;             ///< out format
    UINT32 OutAddr[2];         ///< out addr (Image: idx0 = y, idx1 = uv)

    fpEngInit   fp_Init;        ///< this CB will replace eventID's CB if existed.
}IPLC_IPE_INFO;

/**
     IPLC_DCE data structure

     DCE D2D configuration.
*/
typedef struct _IPLC_DCE
{
    IPL_HAL_PARASET ParIdx; ///< -
    UINT32 eventID;         ///< map to DCE_D2D_CB eventID
    IPH_DCECLK  clk;        ///< engine clk freq.

    USIZE       Size;       ///< input/out size
    DCE_D2D_SETTING D2d_setting;    ///< D2D parameters

    MEM_RANGE*  pDceStpe;   ///< dce stripe table, NULL if no pre-calculation.
    fpEngInit   fp_Init;    ///< this CB will replace eventID's CB if existed.
}IPLC_DCE_INFO;

/**
     IPLC_IFE data structure

     IFE D2D configuration.
*/
typedef struct _IPLC_IFE
{
    IPL_HAL_PARASET ParIdx; ///< -
    UINT32 eventID;         ///< map to IFE_D2D_CB eventID if IFE_D2D_CB exist
    IPH_IFECLK  clk;        ///< engine clk freq.

    IFE_IN_DMA IFE_In;      ///< ife input information

    IFE_DMA_OUT IFE_Out;    ///< output info.

    IFEFMT IFEFmt;          ///< input data format
    fpEngInit   fp_Init;    ///< this CB will replace eventID's CB if existed.
}IPLC_IFE_INFO;

/**
     IPLC_IFE2 data structure

     IFE2 D2D configuration.
*/
typedef struct _IPLC_IFE2_INFO
{
    IPL_HAL_PARASET ParIdx; ///< -
    UINT32 eventID;         ///< map to IFE2_D2D_CB eventID if IFE2_D2D_CB exist
    IPH_IFE2CLK clk;        ///< engine clk freq.
    IFE2_ADDR InAddr;       ///< input data address
    IFE2_SIZE InSize;       ///< input size
    IFE2_LOFS InLofs;       ///< input line offset
    IFE2_FORMAT InFmt;         ///< input data format

    IFE2_ADDR OutAddr;       ///< output data address
    IFE2_LOFS OutLofs;       ///< output line offset
    IFE2_FORMAT OutFmt;      ///< output data format
    fpEngInit   fp_Init;    ///< this CB will replace eventID's CB if existed.
}IPLC_IFE2_INFO;

/**
     IPLC_IME data structure

     IME D2D configuration.
*/
typedef struct _IPLC_IME
{
    IPL_HAL_PARASET ParIdx; ///< -
    UINT32 eventID;         ///< map to IME_D2D_CB eventID if IME_D2D_CB exist
    IPH_IMECLK  clk;        ///< engine clk freq.

    struct
    {
        IMEADDR PPB0;       ///< -
        IMESIZE Size;       ///< -
        IMEFMT Fmt;         ///< -

        struct
        {
            IFE2_GRAY_AVG refval;
            IPL_YCC_CH_INFO Size;///< -
            UINT32 Addr;    ///< -
        }Adaption;          ///< chroma adaption dram input info
    }IME_In;                ///< input info.

    IME_OUT IME_Out;        ///< ime output info

    fpEngInit   fp_Init;        ///< this CB will replace eventID's CB if existed.
}IPLC_IME_INFO;

/**
     IPLC_RDE data structure

     RDE D2D configuration.
*/
typedef struct _IPLC_RDE_INFO
{
    IPL_HAL_PARASET ParIdx;     ///< -
    UINT32 eventID;             ///< map to RDE_D2D_CB eventID if RDE_D2D_CB exist
    IPH_RDECLK  clk;            ///< engine clk freq.

    RDE_BS_INFO In;             ///< Dram input info

    UINT32 ImgAddr;             ///< Dram output addr
    RDE_IMG_INFO ImgInfo;       ///< Dram output info
    UINT32 BRCQInit;            ///< init Q(must be the same as encode initial Q)
    UINT32 Flip_Enable;         ///< Flip (RDE_HFlipEn, RDE_VFlipEn, RDE_HFlipEn|RDE_VFlipEn)
}IPLC_RDE_INFO;


/**
     IPLC_IFEIME data structure(ife_ipe(dce)_ime)

     ife to ime in direct_path. (d2d frame mode)
*/
typedef struct _IPLC_IFEIME
{
    IPL_HAL_PARASET ParIdx; ///< -
    UINT32 eventID;         ///< map to IFEIME_D2D_xxxCB eventID if IFEIME_D2D_xxxCB exist
    IPH_IFECLK  IFEclk;     ///< engine clk freq.
    IPH_IPECLK  IPEclk;     ///< engine clk freq.
    IPH_DCECLK  DCEclk;     ///< engine clk freq.
    IPH_IMECLK  IMEclk;     ///< engine clk freq.

    IFE_IN_DMA  In;         ///< raw iamge input infor.

    struct
    {
        IFE2_GRAY_AVG refval;
        IPL_YCC_CH_INFO Size;   ///< -
        UINT32 Addr;            ///< -
    }Adaption;                  ///< chroma adaption input info

    IME_OUT IME_Out;        ///< ime output info

    struct
    {
        BOOL FuncEn;            ///<sub output enable bit
        IPL_YCC_CH_INFO SizeY;  ///<sub output size
        UINT32 LineOfsC;        ///<
        UINT32 Addr[2];         ///<sub output address
    }IPE_sub;                   ///<ipe sub-image output

    MEM_RANGE*  pDceStpe;   ///< dce stripe table, NULL if no pre-calculation.

    fpEngInit   fp_Init[IPL_HAL_MAX];    ///< this CB will replace eventID's CB if existed. effeicient index: IPL_HAL_IFE, IPL_HAL_IPE, IPL_HAL_DCE, IPL_HAL_IME
}IPLC_IFEIME_INFO;

/**
     IPLC_ISE data structure

     ISE D2D configuration.
*/
typedef struct _IPLC_ISE
{
    IPL_HAL_PARASET ParIdx;                 ///< set configuration into indicated IPH inner table
    UINT32 eventID;                         ///< map to ISE_D2D_CB eventID if ISE_D2D_CB exist
    IPH_ISECLK  clk;                        ///< engine clk freq.
    ISE_IN In;                              ///< dram input info.
    ISE_OUT Out;                            ///< ouput info.
    ISE_PACK_FMT InOutPackFormat;           ///< input and output packing format
    ISE_SCALE_METHOD_SEL ScaleMethod;       ///< scaling method selection
    fpEngInit   fp_Init;                    ///< this CB will replace eventID's CB if existed.
}IPLC_ISE_INFO;


/**
     IPLC_RHE data structure

     RHE D2D configuration.
*/
typedef struct _IPLC_RHE
{
    IPL_HAL_PARASET ParIdx;             ///< -
    UINT32 eventID;                     ///< map to RHE_D2D_CB eventID if RHE_D2D_CB exist
    IPH_RHECLK  clk;                    ///< engine clk freq.

    RHE_SIZE RawSize;                   ///< RHE input raw size
    RHE_Dma_In DmaInputInfo;            ///< RHE dma input information(1:image1 2:image2 3:LA
    RHE_Dma_Out DmaOutInfo;             ///<

    fpEngInit   fp_Init;                ///< this CB will replace eventID's CB if existed.
}IPLC_RHE_INFO;

/**
     IPLC_PREIME data structure

*/
typedef union _IPCL_D2D_STRUCT
{
    IPLC_IPE_INFO IPE;
    IPLC_DCE_INFO DCE;          ///< for dce d2d
    IPLC_IFE_INFO IFE;          ///< for ife d2d
    IPLC_IFE2_INFO IFE2;        ///< for ife2 d2d
    IPLC_IME_INFO IME;          ///< for ime d2d
    IPLC_RDE_INFO RDE;          ///< for rde d2d
    IPLC_IFEIME_INFO IFEIME;    ///< for d2d frame mode
    IPLC_DRE_INFO DRE;          ///< for dre d2d
    IPLC_SIE_INFO SIE;          ///< for sie d2d
    IPLC_ISE_INFO ISE;          ///< for ise d2d
    IPLC_RHE_INFO RHE;          ///< for rhe d2d
}IPLC_D2D_STRUCT;

/**
    IPL trigger type
*/
typedef enum _IPL_TRIG_TYPE
{
    IPL_TRIG_NULL,                      ///< Trigger type 0
    IPL_TRIG_VD,                        ///< Trigger type 1
    IPL_TRIG_TIMER,                     ///< Trigger type 2
    IPL_TRIG_H264,                      ///< Trigger type 3
} IPL_TRIG_TYPE;

/**
    IPL trig control information
*/
typedef struct _IPL_TRIG_CTRL_CMD_INFO
{
    IPL_TRIG_TYPE type;     ///< trigger type
                            ///< IPL_TRIG_VD: sensor number = 1 only support IPL2IME flow
                            ///< IPL_TRIG_TIMER: sensor number >= 1 only support IPL2IME flow
                            ///< IPL_TRIG_H264: sensor number >= 1 only support IPL2IME flow
    UINT32 InterVal;        ///< timer interval (us) only use for TIMER
    UINT32 smode;           ///< Enable:trigger Nth frame & pre-set N+1 frame
                            ///< Disable:set & trigger current frame
    UINT32 EngCloseFlag;    ///< only valid at smode = 0, if set 1, IPL engs close at each VD

                            ///< Below are for adjustive timer trigger
    UINT32 num;             ///< number of trigPoint, ipl number + 1(vd cnt)
    UINT32 trigPoint[2+1];  ///< trigger point to decide trigger timing
} IPL_TRIG_CTRL_CMD_INFO;
//@{
/**
     DCE d2d configuration

     a weak symbol for d2d opeartion, it will be called right before DCE d2d start, in order to get
     DCE configuration.
     @note
        if this symbol is undefined, DCE d2d will return error and drop start operation!(beccause of stripe hw constraint)

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    Ref_1x:     sensor output size
     @param[out]    Ref_1xCen   distortion center of sensor output
     @param[out]    subF:       dce configuration
     @param[out]    group:      IPH DCE update item

     @return Description of data returned.
         - @b return value is the repeat time of dce d2d operation.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 DCE_D2D_CB(IPL_PROC_ID id, USIZE *Ref_1x,Coordinate *Ref_1xCen,DCE_SUBFUNC* subF,DCE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     IPE d2d configuration

     a weak symbol for d2d operation. it will be called right before IPE d2d start, in order to get
     IPE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IPE configuration
     @param[out]    group:      IPH IPE update item

     @return Description of data returned.
         - @b return value is the repeat time of IPE d2d operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IPE_D2D_CB(IPL_PROC_ID id, IPE_SUBFUNC* subF,IPE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     IFE d2d configuration

     a weak symbol for d2d operation. it will be called right before IFE d2d start, in order to get
     IFE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IFE configuration
     @param[out]    group:      IPH IFE update item

     @return Description of data returned.
         - @b return value is the repeat time of IFE d2d operation.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IFE_D2D_CB(IPL_PROC_ID id, IFE_SUBFUNC* subF,IFE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     IFE2 d2d configuration

     a weak symbol for d2d operation. it will be called right before IFE2 d2d start, in order to get
     IFE2 configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IFE2 configuration
     @param[out]    group:      IPH IFE2 update item

     @return Description of data returned.
         - @b return value is the repeat time of IFE2 d2d operation.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IFE2_D2D_CB(IPL_PROC_ID id, IFE2_SUBFUNC* subF,IFE2_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     IME d2d configuration

     a weak symbol for d2d operation. it will be called right before IME d2d start, in order to get
     IME configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IME configuration
     @param[out]    group:      IPH IME update item

     @return Description of data returned.
         - @b return value is the repeat time of IME d2d operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IME_D2D_CB(IPL_PROC_ID id, IME_SUBFUNC* subF,IME_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));
/**
     RDE d2d configuration

     a weak symbol for d2d operation. it will be called right before RDE d2d start, in order to get
     RDE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       RDE configuration
     @param[out]    group:      IPH RDE update item

     @return Description of data returned.
         - @b return value is the repeat time of RDE d2d operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 RDE_D2D_CB(IPL_PROC_ID id, RDE_SUBFUNC* subF,RDE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     IFE configuration of ife2ime frame mode

     a weak symbol for ife2ime frame mode operation. it will be called right before frame mode start, in order to get
     IFE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IFE configuration
     @param[out]    group:      IPH IFE update item

     @return Description of data returned.
         - @b return value is the repeat time of frame mode operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IFEIME_D2D_IFECB(IPL_PROC_ID id, IFE_SUBFUNC* subF,IFE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     DCE configuration of ife2ime frame mode

     ife2ime frame mode operation. it will be called right before frame mode start, in order to get
     DCE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    Ref_1x:     sensor output size
     @param[out]    Ref_1xCen   distortion center of sensor output
     @param[out]    subF:       dce configuration
     @param[out]    group:      IPH DCE update item

     @return Description of data returned.
         - @b return value is the repeat time of frame mode operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IFEIME_D2D_DCECB(IPL_PROC_ID id, USIZE *Ref_1x,Coordinate *Ref_1xCen,DCE_SUBFUNC* subF,DCE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     IPE configuration of ife2ime frame mode

     a weak symbol for ife2ime frame mode operation. it will be called right before frame mode start, in order to get
     IPE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IPE configuration
     @param[out]    group:      IPH IPE update item

     @return Description of data returned.
         - @b return value is the repeat time of frame mode operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IFEIME_D2D_IPECB(IPL_PROC_ID id, IPE_SUBFUNC* subF,IPE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     IME configuration of ife2ime frame mode

     a weak symbol for ife2ime frame mode operation. it will be called right before frame mode start, in order to get
     IME configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fifegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       IME configuration
     @param[out]    group:      IPH IME update item

     @return Description of data returned.
         - @b return value is the repeat time of frame mode operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 IFEIME_D2D_IMECB(IPL_PROC_ID id, IME_SUBFUNC* subF,IME_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     DRE d2d configuration

     a weak symbol for d2d operation. it will be called right before DRE d2d start, in order to get
     RDE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       RDE configuration
     @param[out]    group:      IPH DRE update item

     @return Description of data returned.
         - @b return value is the repeat time of DRE d2d operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 DRE_D2D_CB(IPL_PROC_ID id, DRE_SUBFUNC* subF,DRE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));

/**
     SIE d2d configuration

     a weak symbol for d2d operation. it will be called right before SIE d2d start, in order to get
     RDE configuration.
     @note none

     @param[in]     eventID:    provided event plz reference "IPL_FLOW_PROC_MSG"
     @param[out]    fpRegD:     CB for dump reg, CB timing right before eng start.
     @param[out]    subF:       RDE configuration
     @param[out]    group:      IPH SIE update item

     @return Description of data returned.
         - @b return value is the repeat time of SIE d2d operation.(support only 1 time)
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern UINT32 SIE_D2D_CB(IPL_PROC_ID id, SIE_SUBFUNC* subF,SIE_UPDATE* group,UINT32 eventID,fpRegDump *fpRegD) __attribute__ ((weak));
//@}


/**
     start d2d operation.

     start d2d operation without wait operation finish.
     if previous d2d operation set not ready yet(under the same engine condition), current operation
     will wait until preview operation set done.
     @note
\n   this operation will trig multi-task protection("IPC_D2D_Start" and "IPC_D2D_End" are symmetrical.)

     @param[in] info         egnine configuration
     @param[in] d2dMode      supported d2d mode
     @return Description of data returned.
         - @b E_OK:    engine start.
         - @b E_SYS:   d2dMode not support.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPC_D2D_Start(IPL_PROC_ID id,IPLC_D2D_STRUCT* info,IPLC_D2D d2dMode);

/**
     wait d2d operation finish

     if d2d repeat more than one time, it will wait until last operation finish, and clear OS flag.

     @note:
\n     |--------|                         |--------|                            |--------|
\n     |  data  |   input addr            |        |   input addr               |  data  |   input addr
\n     |--------|                 1x      |--------|                    2x      |--------|
\n                                =>                                    =>
\n
\n     |--------|                         |--------|                            |--------|
\n     |        |   output addr           |  data  |   output addr              |        |   output addr
\n     |--------|                         |--------|                            |--------|
\n

     @param[in]     d2dMode         supported d2d mode
     @param[out]    DataInfo    return ready Data in InputAddr or OutputAddr or other data
     @return Description of data returned.
         - @b E_OK:     engine data output ready
         - @b E_NOSPT:  opeartion error.

     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPC_WaitFinish(IPL_PROC_ID id,IPLC_D2D d2dMode, D2DResult* DataInfo);

/**
     close d2d operation.

     close engine directly.

     @note
\n   this operation will trig multi-task protection("IPC_D2D_Start" and "IPC_D2D_End" are symmetrical.)
\n   if end operation being calling during engine still running, it will cause engine crash.

     @param[in] d2dMode      supported d2d mode
     @return Description of data returned.
         - @b E_OK:     engine closed
         - @b E_NOSPT:  opeartion error.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPC_D2D_End(IPL_PROC_ID id,IPLC_D2D d2dMode);

/**
     wait ime frame end

     wait until ime frame end, and clear ime frame end.
     @note

     @param[in] id:       pipeline index
     @param[in] bClrFlg:
\n   if bClrFlg == TRUE, clear frame end first , and then wait and clear ime frame end.
\n   if bClrFlg == FALSE, wait and clear ime frame end directly.
     @param[in] TimeOutMs: ms

     @return Description of data returned.
         - @b E_OK:    frame end ready.
         - @b E_SYS:   engine is not running.
         - @b E_TMOUT: timeout.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPC_WaitImeFMD2(IPL_PROC_ID Id, BOOL bClrFlg, UINT32 TimeOutMs);
#define IPC_WaitImeFMD(Id, bClrFlg) IPC_WaitImeFMD2(Id, bClrFlg, 0)

/**
     wait sie field end

     wait until sie field end, and clear sie field end.
     @note

     @param[in] id:       pipeline index
     @param[in] bClrFlg:
\n   if bClrFlg == TRUE, clear field end first , and then wait and clear sie field end.
\n   if bClrFlg == FALSE, wait and clear ime frame end directly.
     @param[in] TimeOutMs: ms

     @return Description of data returned.
         - @b E_OK:    field end ready.
         - @b E_SYS:   engine is not running.
         - @b E_TMOUT: timeout.
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern ER IPC_WaitVD2(IPL_PROC_ID Id, BOOL bClrFlg, UINT32 TimeOutMs);
#define IPC_WaitVD(Id, bClrFlg) IPC_WaitVD2(Id, bClrFlg, 0)
/**
     open ipl ctrl task

     open ipl ctrl inner task, which support all api provided by ipl_ctrl module.
     @note :none

     @return :none
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern void IPL_CTRL_OpenTsk(void);

/**
     close ipl ctrl task

     close ipl ctrl inner task.
     @note :none

     @return :none
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern void IPL_CTRL_CloseTsk(void);

/**
     Control IPL trigger
*/
extern ER IPL_CTRLManualTrig(IPL_PROC_ID ProcId);
extern UINT32 IPL_CTRLManualIsProcEnd(void);

/**
    enable IPL_Ctrl module message

    print warning msg under pipeline in running status when enabled.
    @note :none

    @return :none
    Example: none
    @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern void IPL_CTRL_MSG(BOOL bEnable);
//@}

/**
	IPL Tuning API
*/
typedef struct _IPL_TUNING_RAWINFO
{
	UINT32 width;
	UINT32 height;
	UINT32 bits;
	UINT32 startpix;
	UINT32 b2strp;
	UINT32 lineoffset[2];
	UINT32 overlap;
	UINT32 rawsize;
	UINT32 addr;
} IPL_TUNING_RAWINFO;

/**
	get rawinfo
	@param[in]	Id: pipeline index
	@param[out]	addr: address of IPL_TUNING_RAWINFO
	@param[in]	size: available size at addr
*/
extern void ipl_tuning_getrawinfo(UINT32 Id, UINT32 addr, UINT32 size);

/**
	get rawdata
	@param[in] Id: pipeline index
	@param[in] bPause: true->pause sie, false->resume sie
*/
extern void ipl_tuning_setsiemode(UINT32 Id, UINT32 bPause);

/**
	check if sie is running correctly
	@param[in] Id: pipeline index
	@param[in] timeout_t: timeout time, ms

	@return: SIE Current Status
			 0 -> OK
			 1 -> NG
			 2 -> Unknown(illegal ipl status)
*/
extern UINT32 ipl_chkstatus(IPL_PROC_ID Id, UINT32 timeout_t);


#endif
