/*
    Public header file for RDE module.

    This file is the header file that define the API and data type for RHE
    module.

    @file       rhe_lib.h
    @ingroup    mIDrvIPPRHE
    @note

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _RHE_LIB_H
#define _RHE_LIB_H

#include "Type.h"

/**
    @addtogroup mIIPPRHE
*/
//@{

/**
    @name Define RHE function.
*/
//@{
//@}




#define RHE_FLKR_DET_EN         0x00000008 ///< Enable function flicker-detection (only in Staggered-HDR mode)
#define RHE_RDE_EN              0x00000010 ///< Enable function input-raw-decode


#define RHE_INT_FRAMEEND        0x00000001 ///< Interrupt - Frame End
#define RHE_INT_DPC_WARN        0x00000002 ///< Interrupt - Defect Concealment warning
#define RHE_INT_IN0_DEC_ERR     0x00000004 ///< Interrupt - Decoder error for input-0
#define RHE_INT_IN1_DEC_ERR     0x00000008 ///< Interrupt - Decoder error for input-0
#define RHE_INT_ALL             0xffffffff ///< Interrupt - All


//RHE_LOAD_SEL removed

typedef enum
{
    RHE_FUNC_STAGGERED_HDR = 0, ///< Staggered HDR
    RHE_FUNC_CURVED_HDR    = 1, ///< Curved HDR
    RHE_FUNC_DFS           = 2, ///< Dark Frame Subtraction
    ENUM_DUMMY4WORD(RHE_MAIN_FUNC_SEL)
} RHE_MAIN_FUNC_SEL;

typedef enum
{
    RHE_FUNC_DISABLE  = 0,  ///< Those bit=1 would be set, others remain
    RHE_FUNC_ENABLE ,       ///< Those bit=1 would be clear, others remain
    RHE_FUNC_SET ,          ///< All bit would be configured as 0/1 ther are
    ENUM_DUMMY4WORD(RHE_FUNC_SWITCH_SEL)
} RHE_FUNC_SWITCH_SEL;

typedef enum
{
    RHE_OUT_DRAM        = 0, ///< DRAM->RHE->DRAM
    RHE_OUT_IFE         = 1, ///< DRAM->RHE->IFE
    ENUM_DUMMY4WORD(RHE_OUT_PATH_SEL)
} RHE_OUT_PATH_SEL;

typedef enum
{
    RHE_WAIT_FRAMEEND         =  0,  ///<
    ENUM_DUMMY4WORD(RHE_WAIT_EVENT_SEL)
} RHE_WAIT_EVENT_SEL;


typedef enum
{
    RHE_CHG_RAW_LA_GRP      = 0,   ///< refer to "RHE_RAW_LA_GROUP_INFO"       , change input raw/la information
    RHE_CHG_DRAM_IO         = 1,   ///< refer to "RHE_CHG_DRAM_IO_INFO"        , change Dram in/out
    RHE_CHG_HDR_RAD         = 2,   ///< refer to "RHE_HDR_RAD_INFO"            , change HDR Radiance
    RHE_CHG_HDR_REF_LA_LPF  = 3,   ///< refer to "RHE_HDR_REF_LA_LPF_INFO"     , change HDR Reference LA low-pass-filter
    RHE_CHG_HDR_GBL_TN      = 4,   ///< refer to "RHE_HDR_GLOBAL_TONE_INFO"    , change HDR global tone
    RHE_CHG_HDR_LOC_TN      = 5,   ///< refer to "RHE_HDR_LOCAL_TONE_INFO"     , change HDR local tone
    RHE_CHG_HDR_REGU        = 6,   ///< refer to "SHDR_REGULATION_INFO"     , change HDR Regulation
    RHE_CHG_RECURVE			= 7,   ///< refer to "" 

    ENUM_DUMMY4WORD(RHE_CHANGE_FUN_PARAM_SEL)
} RHE_CHANGE_FUN_PARAM_SEL;


typedef enum
{
    RHE_PACKBUS_8  = 0, ///< 8 bits packing
    RHE_PACKBUS_10 ,    ///< 10 bits packing
    RHE_PACKBUS_12 ,    ///< 12 bits packing
    RHE_PACKBUS_16 ,    ///< 16 bits packing
    ENUM_DUMMY4WORD(RHE_PACKBUS_SEL)
} RHE_PACKBUS_SEL;

typedef enum
{
    RHE_DFS_DP_BY_IN0  = 0, ///< DP threshold on input0
    RHE_DFS_DP_BY_IN1 ,     ///< DP threshold on input1
    ENUM_DUMMY4WORD(RHE_DFS_DP_SRC_SEL)
} RHE_DFS_DP_SRC_SEL;

typedef enum
{
    RHE_DFS_OP_B_MINUS_A  = 0, ///< Input-1 minus Input-0
    RHE_DFS_OP_A_MINUS_B     , ///< Input-0 minus Input-1
    RHE_DFS_OP_A_PLUS_B      , ///< Input-0 plus  Input-1
    ENUM_DUMMY4WORD(RHE_DFS_OP_SEL)
} RHE_DFS_OP_SEL;

typedef enum
{
    RHE_DEF_WGT_50  = 0, ///< Defect concealment cross-channel weight = 50%
    RHE_DEF_WGT_25  = 1, ///< Defect concealment cross-channel weight = 25%
    RHE_DEF_WGT_0   = 2, ///< Defect concealment cross-channel weight =  0% (disabled)
    ENUM_DUMMY4WORD(RHE_DEF_WGT_SEL)
} RHE_DEF_WGT_SEL;

typedef enum
{
    RHE_HDR_RAD_WGT_BY_IN0  = 0, ///< Radiance weight table source = input-0
    RHE_HDR_RAD_WGT_BY_IN1  = 1, ///< Radiance weight table source = input-1
    ENUM_DUMMY4WORD(RHE_HDR_RAD_WGT_SRC_SEL)
} RHE_HDR_RAD_WGT_SRC_SEL;


typedef enum
{
    RHE_HDR_REF_LA_LSB16BIT  = 0, ///< LA source is the LSB 16 bits from a word(window)
    RHE_HDR_REF_LA_MSB16BIT  = 1, ///< LA source is the MSB 16 bits from a word(window)
    ENUM_DUMMY4WORD(RHE_HDR_REF_LA_SRC_SEL)
} RHE_HDR_REF_LA_SRC_SEL;

typedef enum
{
    RHE_CLKSRC_192M = 0, ///< RHE clock source as 192MHz
    RHE_CLKSRC_240M = 1, ///< RHE clock source as 240MHz
    RHE_CLKSRC_PLL6 = 2, ///< RHE clock source as PLL6
    RHE_CLKSRC_PLL13= 3, ///< RHE clock source as PLL13
} RHE_CLKSRC_SEL;


typedef enum
{
  RHE_BURST_32W = 0,   ///< 32 words
  RHE_BURST_48W = 1,   ///< 48 words
  ENUM_DUMMY4WORD(RHE_BURST_SEL)
} RHE_BURST_SEL;

typedef struct
{
    UINT32      uiWinSzX; ///< window horizontal size, from sie1_getLaWin()
    UINT32      uiWinSzY; ///< window vertical size, from sie1_getLaWin()
    UINT32      uiWinNmX; ///< window horizontal number, from sie1_getLaWin()
    UINT32      uiWinNmY; ///< window vertical number, from sie1_getLaWin()
    UINT32      uiWinSum; ///< window sum, from sie1_getLaWinSum()
} RHE_LA_INFO;

typedef struct
{
    UINT32      uiFullSzX; ///< full horizontal size
    UINT32      uiFullSzY; ///< full vertical size
    UINT32      uiCropStX; ///< crop horizontal starting position
    UINT32      uiCropStY; ///< crop vertical starting position
    UINT32      uiCropSzX; ///< crop horizontal size
    UINT32      uiCropSzY; ///< crop vertical size
} RHE_RAW_INFO;


typedef struct
{
    UINT32          uiIn0Addr;  ///< address of raw input-0, for the bright one
    UINT32          uiIn1Addr;  ///< address of raw input-1, for the dark one
    UINT32          uiIn2Addr;  ///< address of la input, form the bright one
    UINT32          uiIn0Lofs;  ///< line-offset of raw input-0
    UINT32          uiIn1Lofs;  ///< line-offset of raw input-1
    RHE_PACKBUS_SEL PackBus;    ///< bus bit-depth
} RHE_DRAM_IN_INFO;

typedef struct
{
    UINT32          uiOutAddr;  ///< address
    UINT32          uiLofs;     ///< line-offset
    RHE_PACKBUS_SEL PackBus;    ///< bus bit-depth
} RHE_DRAM_OUT_INFO;

typedef struct
{
    UINT32      uiIn0Addr; ///< input channel 0 address;        "0" as keep-previous-value
    UINT32      uiIn1Addr; ///< input channel 1 address;        "0" as keep-previous-value
    UINT32      uiIn2Addr; ///< input channel 2 address;        "0" as keep-previous-value
    UINT32      uiOutAddr; ///< output channel  address;        "0" as keep-previous-value
    UINT32      uiIn0Size; ///< input channel 0 Size;           "0" as keep-previous-value
    UINT32      uiIn1Size; ///< input channel 1 Size;           "0" as keep-previous-value
    UINT32      uiIn2Size; ///< input channel 2 Size;           "0" as keep-previous-value
    UINT32      uiOutSize; ///< output channel  Size;           "0" as keep-previous-value
    UINT32      uiIn0Lofs; ///< input channel 0 line-offset;    "0" as keep-previous-value
    UINT32      uiIn1Lofs; ///< input channel 1 line-offset;    "0" as keep-previous-value
    UINT32      uiOutLofs; ///< output channel  line-offset;    "0" as keep-previous-value
} RHE_CHG_DRAM_IO_INFO;

typedef struct
{
    UINT32              uiDpTh;     ///< Defect upper threshold
    RHE_DFS_DP_SRC_SEL  DpSrc;      ///< Defect thresholding source
    RHE_DFS_OP_SEL      DfsOp;      ///< Operation type
    RHE_DEF_WGT_SEL     DefWgt;     ///< Defect concealment cross-channel weight
} RHE_DFS_INFO;

typedef struct
{
    UINT32      uiStX;     ///< start position X
    UINT32      uiStY;     ///< start position Y
    UINT32      uiWinSzX;  ///< window size X
    UINT32      uiWinSzY;  ///< window size Y
    UINT32      uiWinSpY;  ///< window skip(spacing) Y
    UINT32      uiSmplX;   ///< subsample ratio X (inside window)
    UINT32      uiSmplY;   ///< subsample ratio Y (inside window)
    BOOL        bChSel[4];  ///< channel selection
    UINT32      uiSatTh;    ///< upper thershold
    UINT32      uiLowTh;    ///< lower thershold
} RHE_FLICKER_DETECT_INFO;

typedef struct
{
    UINT32      uiIn0Rslt[16];  ///< flicker detection result for input-0
    UINT32      uiIn1Rslt[16];  ///< flicker detection result for input-1
} RHE_FLICKER_DETECT_RSLT_INFO;

typedef struct
{
    UINT32      uiStX;     ///< start position X
    UINT32      uiStY;     ///< start position Y
    UINT32      uiWinSzX;  ///< window size X
    UINT32      uiWinSzY;  ///< window size Y
    UINT32      uiWinSpY;  ///< window skip(spacing) Y
    UINT32      uiSatTh;    ///< upper thershold
    UINT32      uiLowTh;    ///< lower thershold
    BOOL        bChSel[4];  ///< channel selection, for channel 0/1/2/3
    UINT32      uiCfaPat;   ///< the starting channel, 0~3
} RHE_FLICKER_DETECT_ADJ_INFO;


typedef struct
{
    UINT32      uiIn0EvGap;     ///< The EV-gap for input-0, for the bright one
    UINT32      uiIn1EvGap;     ///< The EV-gap for input-1, for the dark one
    RHE_HDR_RAD_WGT_SRC_SEL RadWgtSrc;  ///< Radiance weight table source
    UINT8       uiWgtTbl[17];   ///< radiance weight table
} RHE_HDR_RAD_INFO;

typedef struct
{
    UINT32      uiDc;           ///< Raw Y DC value
} RHE_HDR_REF_DC_INFO;

typedef struct
{
    UINT32      uiInNormM;      ///< input normalization factor M. No need to configure.
    UINT32      uiInNormS;      ///< input normalization factor S. No need to configure.
    RHE_HDR_REF_LA_SRC_SEL  LaSrcSel;   ///< LA source in a word(window)
} RHE_HDR_REF_LA_IN_INFO;

typedef struct
{
    UINT8       uiLpf[8];    ///< low pass filter A/B/C/D/E/F/G/H
    UINT32      uiLpfNormM;  ///< low-pass-filter normalization factor M. No need to configure.
    UINT32      uiLpfNormS;  ///< low-pass-filter normalization factor S. No need to configure.
} RHE_HDR_REF_LA_LPF_INFO;

typedef struct
{
    UINT32 uiFctrX;      ///< scaling factor X
    UINT32 uiFctrY;      ///< scaling factor Y
    UINT32 uiNegOfsX;      ///< input starting postion negative offset X, 4 bits as 0.4 bits
    UINT32 uiPosOfsX;      ///< input starting postion positive offset X, 8 bits as 4.4 bits
    UINT32 uiNegOfsY;      ///< input starting postion negative offset Y, 4 bits as 0.4 bits
    UINT32 uiPosOfsY;      ///< input starting postion positive offset Y, 8 bits as 4.4 bits
} RHE_HDR_REF_LA_SCL_INFO;

typedef struct
{
    UINT16 uiGblTnTbl[41];  ///< global tone table, 41taps x 12bits
} RHE_HDR_GLOBAL_TONE_INFO;

typedef struct
{
    UINT32  uiBand0Gain;    ///< gain for band-0, 10 bits as 1.9 bits
    UINT32  uiBand1Gain;    ///< gain for band-1, 10 bits as 1.9 bits
} RHE_HDR_LOCAL_TONE_INFO;

typedef struct
{
    UINT8   uiARto[9];    ///< reaulation on ratio, considering data A, 9taps x 8bits
    UINT8   uiAMax[9];    ///< reaulation on max. , considering data A, 9taps x 8bits
    UINT8   uiAMin[9];    ///< reaulation on min. , considering data A, 9taps x 8bits
    UINT8   uiBMax[9];    ///< reaulation on max. , considering data B, 9taps x 8bits
    UINT8   uiBMin[9];    ///< reaulation on min. , considering data B, 9taps x 8bits
} RHE_HDR_REGULATION_INFO;

typedef struct
{
    UINT16  uiPointX[4];
    UINT16  uiPointY[4];
    UINT16  uiSlopeM[4];
    UINT8   uiSlopeS[4];
} RHE_HDR_RECURVEN_INFO;

typedef struct
{
    BOOL bDeGamma;                  ///< enable De-Gamma, useless(always on)
    UINT16 *puiDgmaTbl;             ///< Pointer of degamma table, useless(always default configuration)
} RHE_RDE_INFO;


typedef struct
{
    RHE_LA_INFO         LaInfo;
    RHE_RAW_INFO        RawInfo;
} RHE_RAW_LA_GROUP_INFO;

typedef struct
{
    RHE_MAIN_FUNC_SEL   MainFuncSel;    ///<
    RHE_OUT_PATH_SEL    OutPath;        ///<
    UINT32  uiFuncEn;
    UINT32  uiIntrpEn;
    RHE_DFS_INFO        DpfInfo;        ///<
    RHE_RDE_INFO        RdeInfo;        ///<
    RHE_RAW_LA_GROUP_INFO RawLaGrpInfo; ///<
    RHE_DRAM_IN_INFO    DramInInfo;     ///<
    RHE_DRAM_OUT_INFO   DramOutInfo;    ///<
    RHE_FLICKER_DETECT_INFO FkDtInfo;   ///<
    RHE_HDR_RAD_INFO    HdrRadInfo;     ///<
    RHE_HDR_REF_LA_LPF_INFO     HdrRefLaLpfInfo;    ///<
    RHE_HDR_GLOBAL_TONE_INFO    HdrGblTnInfo;       ///<
    RHE_HDR_LOCAL_TONE_INFO     HdrLocTnInfo;       ///<
    RHE_HDR_REGULATION_INFO     HdrReguInfo;        ///<
    RHE_HDR_RECURVEN_INFO       HdrReCrvInfo;       ///<
} RHE_MODE_PARAM;

typedef struct
{
    void (*pfRheIsrCb)(UINT32 uiIntpStatus); ///< isr callback function
    UINT32 uiRheClkRat;         ///< RHE Clock Rate in MHz
    UINT32 uiRheClk2Rat;        ///< RHE Clock2(Decoder) Rate in MHz
} RHE_OPENOBJ;

typedef struct _RHE_BURST_LENGTH
{
    RHE_BURST_SEL       BurstLenIn01; ///< for input channel 0 and 1
} RHE_BURST_LENGTH;

/**
RHE API

@name   RHE_API
*/
//@{

// rhe_lib.c

/*
    RHE Interrupt Service Routine

    RHE interrupt service routine

    @return None.
*/
extern void rhe_isr(void);


/**
    RHE Open Driver

    Open RHE engine

    @param[in] pObjCB RHE open object

    @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER rhe_open(RHE_OPENOBJ *pObjCB);
//------------------------------------------------------------------------------


/**
    RHE Get Open Status

    Check if RHE engine is opened

    @return
        - @b FALSE: engine is not opened
        - @b TRUE: engine is opened
*/
extern BOOL rhe_isOpened(void);
//------------------------------------------------------------------------------


/**
    RHE Close Driver

    Close RHE engine

    @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER rhe_close(void);
//------------------------------------------------------------------------------

/**
    RHE Pause Operation

    Pause RHE engine

    @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER rhe_pause(void);
//------------------------------------------------------------------------------

/**
    RHE Start Operation

    Start RHE engine

    @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER rhe_start(void);
//------------------------------------------------------------------------------

/**
    RHE Set Mode

    Set RHE engine mode

    @param[in] pRheParam RHE configuration.

    @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER rhe_setMode(RHE_MODE_PARAM *pRheParam);
//------------------------------------------------------------------------------

/**
    RHE Change Function Enable

    Set RHE Function Enable

    @param[in] FuncSwitch function enable / disable
    @param[in] uiFuncSel function selection

    @return
        - @b E_OK: Done with no error.
        - Others: Error occured.
*/
extern ER rhe_chgFuncEn(RHE_FUNC_SWITCH_SEL FuncSwitch, UINT32 uiFuncSel);
//------------------------------------------------------------------------------

/**
    Dynamic change RHE parameters

    @param[in] pParam Parameters.
    @param[in] FunSel Change function selection.

    @return
    -@b E_OK  Engine set mode and parameters without error.\n
    -@b E_PAR Egnine set parameters error.\n
*/
extern ER rhe_chgParam(void *pParam, RHE_CHANGE_FUN_PARAM_SEL FunSel);
//------------------------------------------------------------------------------

/**
    RHE Wait VD End

    Wait for VD end flag.

    @param[in] uiVdCnt number of VD ends to wait after clearing flag.

    @return None.
*/
extern ER rhe_waitEvent(RHE_WAIT_EVENT_SEL WaitEvent, BOOL bClrFlag);
//------------------------------------------------------------------------------

/**
    RHE Get clock rate

    Get Engine clock

    @return None.
*/
extern UINT32 rhe_getClockRate(void);
//------------------------------------------------------------------------------

/**
    RHE Get Hdr Info

    @param[in] pHdrRadInfo Get HDR information

    @return None. 
*/
extern VOID rhe_getHdrRad(RHE_HDR_RAD_INFO *pHdrRadInfo);
//------------------------------------------------------------------------------

/**
    RHE Get Burst Length

    @param[in] pBurstLen Get burst length

    @return None. 
*/
extern VOID rhe_getBurstLength(RHE_BURST_LENGTH *pBurstLen);
//------------------------------------------------------------------------------



#endif
//@}

//@}


