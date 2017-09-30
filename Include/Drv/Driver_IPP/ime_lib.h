/**
    Public header file for IME module.

    @file       ime_lib.h
    @ingroup    mIIPPIME
    
    @brief

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/


#ifndef _IME_LIB_H_
#define _IME_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Type.h"

typedef void (*IMEIntCB)(UINT32);

//------------------------------------------------------------------------------
//enum data type




//------------------------------------------------------------------------------
/** 
    @addtogroup mIDrvIPP_IME 
*/
//@{

//define vairable
#define IME_INTE_STRP_END    0x40000000  ///< stripe-end enable
#define IME_INTE_FRM_END     0x80000000  ///< frame-end enable
#define IME_INTE_ALL         (IME_INTE_FRM_END)   ///< all interrupt enable

#define IME_INT_STRP_END    0x40000000  ///< stripe-end enable
#define IME_INT_FRM_END     0x80000000  ///< frame-end status
#define IME_INT_ALL         (IME_INT_FRM_END)   ///< all interrupt status

#define IPE_To_IME  0         ///< IPE to IME
#define DRAM_To_IME 1         ///< DMA to IME



//------------------------------------------------------------------------------
// engine control
/**
    IME enum - function enalbe/disable
*/
typedef enum _IME_FUNC_EN
{
    IME_FUNC_DISABLE = 0,   ///< function disable
    IME_FUNC_ENABLE = 1,  ///< function enable
    ENUM_DUMMY4WORD(IME_FUNC_EN)
} IME_FUNC_EN;

/**
    IME enum - operation mode
*/
typedef enum _IME_OPMODE
{    
    IME_OPMODE_D2D     = 0,    ///< IME only
    IME_OPMODE_IPP     = 1,    ///< IFE->DCE->IPE->IME
    IME_OPMODE_RDE_IPP = 2,    ///< RDE + IFE->DCE->IPE->IME
    IME_OPMODE_RHE_IPP = 3,    ///< RDE + IFE->DCE->IPE->IME
    IME_OPMODE_SIE_IPP = 4,    ///< SIE + IFE->DCE->IPE->IME (all direct)
    IME_OPMODE_UNKNOWN = 5,    ///< Undefined mode
    ENUM_DUMMY4WORD(IME_OPMODE)
} IME_OPMODE;

/**
    IME enum - input ping-pong buffer selection
*/
typedef enum _IME_IN_BUF_SEL
{
    IME_IN_BUF_0 = 0,   ///< select to set ping-pong buffer0 address
    IME_IN_BUF_1 = 1,   ///< select to set ping-pong buffer1 address
    IME_IN_BUF_BOTH = 2,   ///< select to set ping-pong buffer0 and buffer1 address
    ENUM_DUMMY4WORD(IME_IN_BUF_SEL)
} IME_IN_BUF_SEL;

/**
    IME enum - load type of configned parameters
*/
typedef enum
{
    IME_START_LOAD   = 0,  ///< Frame start loaded
    IME_FRMEND_LOAD  = 1,  ///< Frame end loaded
    ENUM_DUMMY4WORD(IME_LOAD_TYPE)
} IME_LOAD_TYPE;



//------------------------------------------------------------------------------
// input image size control
/**
    IME enum - input stripe mode selection
*/
typedef enum _IME_STRIPE_MODE
{
    IME_STRIPE_AUTO_MODE = 0,    ///< Auto stripe information computation mode
    IME_STRIPE_USER_MODE = 1,    ///< User stripe information computation mode
    ENUM_DUMMY4WORD(IME_STRIPE_MODE)
} IME_STRIPE_MODE;

/**
    IME enum - input stripe overlap selection
*/
typedef enum _IME_H_STRIPE_OVLP_SEL
{
    IME_H_ST_OVLP_16P  = 0,    ///< Horizontal stripe overlap size: 16 pixels
    IME_H_ST_OVLP_24P  = 1,    ///< Horizontal stripe overlap size: 24 pixels
    IME_H_ST_OVLP_32P  = 2,    ///< Horizontal stripe overlap size: 32 pixels
    IME_H_ST_OVLP_USER = 3,    ///< Horizontal stripe overlap size: user define
    ENUM_DUMMY4WORD(IME_H_STRIPE_OVLP_SEL)
} IME_H_STRIPE_OVLP_SEL;

/**
    IME enum - input image data format selection
*/
typedef enum _IME_INPUT_FORMAT_SEL
{
    IME_INPUT_YCC_444  = 0,         ///< YCC444 separate planar format for input
    IME_INPUT_YCC_422  = 1,         ///< YCC422 separate planar format for input
    IME_INPUT_YCC_420  = 2,         ///< YCC420 separate planar format for input
    IME_INPUT_RESERVED = 3,         ///< reversed
    IME_INPUT_YCCP_422 = 4,         ///< YCC422 UV-packing format for input
    IME_INPUT_YCCP_420 = 5,         ///< YCC420 UV-packing format for input
    IME_INPUT_Y_ONLY   = 6,         ///< Only Y channel for input
    IME_INPUT_RGB      = 7,         ///< RGB for input (YCC444)
    ENUM_DUMMY4WORD(IME_INPUT_FORMAT_SEL)
} IME_INPUT_FORMAT_SEL;

/**
    IME enum - image data format selection of LCA function
*/
typedef enum _IME_LCA_FORMAT_SEL
{
    IME_LCAF_YCCYCC  = 0,    ///< YCbCrYCbCr data format
    IME_LCAF_YCCP    = 1,    ///< Y / CbCr data format
    ENUM_DUMMY4WORD(IME_LCA_FORMAT_SEL)
} IME_LCA_FORMAT_SEL;

//------------------------------------------------------------------------------
// output control
/**
    IME enum - output path selection
*/
typedef enum _IME_PATH_SEL
{
    IME_PATH1_SEL = 1,   ///< operation for output path1
    IME_PATH2_SEL = 2,   ///< operation for output path2
    IME_PATH3_SEL = 4,   ///< operation for output path3
    IME_PATH4_SEL = 8,   ///< operation for output path3
    ENUM_DUMMY4WORD(IME_PATH_SEL)
} IME_PATH_SEL;

/**
    IME enum - output buffer selection
*/
typedef enum _IME_PATH_OUTBUF_SEL
{
    IME_PATH_OUTBUF_SET0 = 0,   ///< output buffer set0 selection
    IME_PATH_OUTBUF_SET1 = 1,   ///< output buffer set1 selection
    ENUM_DUMMY4WORD(IME_PATH_OUTBUF_SEL)
} IME_PATH_OUTBUF_SEL;

/**
    IME enum - output image data format type
*/
typedef enum _IME_OUTPUT_FORMAT_TYPE
{
    IME_OUTPUT_YCC_PLANAR   = 0,         ///< Y/Cb/Cr separate planar type
    IME_OUTPUT_YCC_UVPACKIN = 1,         ///< Y/UV-packing type
    ENUM_DUMMY4WORD(IME_OUTPUT_FORMAT_TYPE)
} IME_OUTPUT_FORMAT_TYPE;

/**
    IME enum - output image data format selection
*/
typedef enum _IME_OUTPUT_IMG_FORMAT_SEL
{
    IME_OUTPUT_YCC_444     = 0,         ///< YCC444 format for output
    IME_OUTPUT_YCC_422_COS = 1,         ///< YCC422 cosited format for output
    IME_OUTPUT_YCC_422_CEN = 2,         ///< YCC422 centered format for output
    IME_OUTPUT_YCC_420_COS = 3,         ///< YCC420 cosited format for output
    IME_OUTPUT_YCC_420_CEN = 4,         ///< YCC420 centered format for output
    IME_OUTPUT_Y_ONLY      = 6,         ///< Y only for output
    IME_OUTPUT_RGB_444     = 7,         ///< RGB444 format for output
    ENUM_DUMMY4WORD(IME_OUTPUT_IMG_FORMAT_SEL)
} IME_OUTPUT_IMG_FORMAT_SEL;

/**
    IME enum - scale method selection
*/
typedef enum _IME_SCALE_METHOD_SEL
{
    IMEALG_BICUBIC     = 0,        ///< bicubic interpolation
    IMEALG_BILINEAR    = 1,        ///< bilinear interpolation
    IMEALG_NEAREST     = 2,        ///< nearest interpolation
    IMEALG_INTEGRATION = 3,        ///< nearest interpolation
    ENUM_DUMMY4WORD(IME_SCALE_METHOD_SEL)
} IME_SCALE_METHOD_SEL;

/**
    IME enum - scale filter computation mode selection
*/
typedef enum _IME_SCALE_FILTER_COEF_MODE
{
    IME_SCALE_FILTER_COEF_AUTO_MODE = 0,    ///< Auto Scale filter coefficeint computation mode
    IME_SCALE_FILTER_COEF_USER_MODE = 1,    ///< User Scale filter coefficeint computation mode
    ENUM_DUMMY4WORD(IME_SCALE_FILTER_COEF_MODE)
} IME_SCALE_FILTER_COEF_MODE;

/**
    IME enum - scale type selection
*/
typedef enum _IME_SCALE_TYPE_SEL
{
  IME_SCALE_DOWN = 0,  ///< scaling down enable
  IME_SCALE_UP = 1,    ///< scaling up enable
  ENUM_DUMMY4WORD(IME_SCALE_TYPE_SEL)
} IME_SCALE_TYPE_SEL;

/**
    IME enum - scale factor computation mode selection
*/
typedef enum _IME_SCALE_FACTOR_COEF_MODE
{
    IME_SCALE_FACTOR_COEF_AUTO_MODE = 0,    ///< Auto Scale factor coefficeint computation mode
    IME_SCALE_FACTOR_COEF_USER_MODE = 1,    ///< User Scale factor coefficeint computation mode
    ENUM_DUMMY4WORD(IME_SCALE_FACTOR_COEF_MODE)
} IME_SCALE_FACTOR_COEF_MODE;

/**
    IME enum - outupt path destination selection
*/
typedef enum _IME_OUTDST_CTRL_SEL
{
  IME_OUTDST_DRAM = 0,         ///< output to Dram
  IME_OUTDST_H264 = 1,         ///< output to H.264
  ENUM_DUMMY4WORD(IME_OUTDST_CTRL_SEL)
} IME_OUTDST_CTRL_SEL;

//------------------------------------------------------------------------------
/**
    IME enum - system flag clear selection
*/
typedef enum _IME_FLAG_CLEAR_SEL
{
  IME_FLAG_NO_CLEAR = 0,    ///< No clear flag
  IME_FLAG_CLEAR    = 1,    ///< Clear flag
  ENUM_DUMMY4WORD(IME_FLAG_CLEAR_SEL)
} IME_FLAG_CLEAR_SEL;

//------------------------------------------------------------------------------

/**
    IME enum - scale enhancing mode selection
*/
typedef enum _IME_SCALE_ENH_MODE
{
    IME_SCALE_ENH_MODE_AUTO = 0,  ///< Scaling enhancement, parameter setting by auto
    IME_SCALE_ENH_MODE_USER = 1,  ///< Scaling enhancement, parameter setting by user
    ENUM_DUMMY4WORD(IME_SCALE_ENH_MODE)
} IME_SCALE_ENH_MODE;


/**
    IME enum - value range selection

    @note used for chroma adaptation
*/
typedef enum _IME_RANGE_SEL
{
    IME_RANGE_8   = 0,  ///< range 8
    IME_RANGE_16  = 1,  ///< range 16
    IME_RANGE_32  = 2,  ///< range 32
    IME_RANGE_64  = 3,  ///< range 64
    IME_RANGE_128 = 4,  ///< range 128
    ENUM_DUMMY4WORD(IME_RANGE_SEL)
} IME_RANGE_SEL;


/**
    IME enum - dark and bright chroma suppression mode selection
*/
typedef enum _IME_DBCS_MODE_SEL
{
    IME_DBCS_DK_MODE  = 0,  ///< dark mode
    IME_DBCS_BT_MODE  = 1,  ///< bright mode
    IME_DBCS_BOTH_MODE  = 2,  ///< dark and bright mode
    ENUM_DUMMY4WORD(IME_DBCS_MODE_SEL)
} IME_DBCS_MODE_SEL;


/**
    IME enum - data stamp processing selection
*/
typedef enum _IME_DS_SETNUM
{
    IME_DS_SET0  = 0,  ///< Set0
    IME_DS_SET1  = 1,  ///< Set0
    IME_DS_SET2  = 2,  ///< Set0
    IME_DS_SET3  = 3,  ///< Set0
    ENUM_DUMMY4WORD(IME_DS_SETNUM)
} IME_DS_SETNUM;


/**
    IME enum - data stamp color LUT mode selection
*/
typedef enum _IME_DS_MODE_SEL
{
    IME_DS_MODE_2B  = 0,  ///< 2bit mode, image width must be the multiple of 4
    IME_DS_MODE_4B  = 1,  ///< 4bit mode, image width must be the multiple of 4
    ENUM_DUMMY4WORD(IME_DS_MODE_SEL)
} IME_DS_MODE_SEL;

/**
    IME enum - SSR processing mode
*/
typedef enum _IME_SSR_MODE_SEL
{
    IME_SSR_MODE_USER  = 0,  ///< user mode of SSR, function enable/disable is controlled by user
    IME_SSR_MODE_AUTO  = 1,  ///< auto mode of SSR, function enable/disable is controlled by IME driver
    ENUM_DUMMY4WORD(IME_SSR_MODE_SEL)
} IME_SSR_MODE_SEL;


/**
    IME enum - statistical edge kernel selection
*/
typedef enum _IME_STL_EKER_SEL
{
    IME_STL_EKER_HA  = 0,  ///< H map A
    IME_STL_EKER_HB  = 1,  ///< H map B
    IME_STL_EKER_VA  = 2,  ///< V map A
    IME_STL_EKER_VB  = 3,  ///< V map B
    ENUM_DUMMY4WORD(IME_STL_EKER_SEL)
} IME_STL_EKER_SEL;


/**
    IME enum - output image data type selection for Statistical
*/
typedef enum _IME_STL_IMGOUT_SEL
{
    IME_STL_IMGOUT_AF  = 0,  ///< output after filter
    IME_STL_IMGOUT_BF  = 1,  ///< output before filter
    ENUM_DUMMY4WORD(IME_STL_IMGOUT_SEL)
} IME_STL_IMGOUT_SEL;


/**
    IME enum - burst length selection
*/
typedef enum _IME_BURST_SEL
{
    IME_BURST_32W  = 0,  ///< burst size: 32 word
    IME_BURST_16W  = 1,  ///< burst size: 16 word
    IME_BURST_48W  = 2,  ///< burst size: 48 word
    IME_BURST_64W  = 3,  ///< burst size: 64 word
    ENUM_DUMMY4WORD(IME_BURST_SEL)
} IME_BURST_SEL;



/**
    IME enum - get burst size of channel selection
*/
typedef enum _IME_GET_BSTSIZE_SEL
{
    IME_GET_BST_IN_Y    = 0,  ///< get input channel: Y
    IME_GET_BST_IN_U    = 1,  ///< get input channel: U
    IME_GET_BST_IN_V    = 2,  ///< get input channel: V
    IME_GET_BST_OUTP1_Y = 3,  ///< get output path1 channel: Y
    IME_GET_BST_OUTP1_U = 4,  ///< get output path1 channel: U
    IME_GET_BST_OUTP1_V = 5,  ///< get output path1 channel: V
    IME_GET_BST_OUTP2_Y = 6,  ///< get output path2 channel: Y
    IME_GET_BST_OUTP2_U = 7,  ///< get output path2 channel: U
    IME_GET_BST_OUTP3_Y = 8,  ///< get output path3 channel: Y
    IME_GET_BST_OUTP3_U = 9,  ///< get output path3 channel: U
    IME_GET_BST_OUTP4_Y = 10,  ///< get output path4 channel: Y    
    IME_GET_BST_IN_LCA  = 11,  ///< input channel: LCA    
    ENUM_DUMMY4WORD(IME_GET_BSTSIZE_SEL)
} IME_GET_BSTSIZE_SEL;



//------------------------------------------------------------------------------
/**
    IME enum - get engine information selection
*/
typedef enum _IME_GET_INFO_SEL
{
    IME_GET_INPUT_SRC                   = 1,
    IME_GET_INT_ENABLE                  = 2,      ///< get interrupt enable information
    IME_GET_INT_STATUS                  = 3,       ///< get interrupt status
    IME_GET_LOCKED_STATUS               = 4,       ///< get IME engine locked status
    IME_GET_LCA_PPBID                   = 5,       ///< get IME ping-pong buffer ID of LCA

    IME_GET_P1_ENABLE_STATUS            = 21,      ///< get output path1 enable status
    IME_GET_P1_OUTPUT_FORMAT            = 22,      ///< get output path1 data format
    IME_GET_P1_OUTPUT_TYPE              = 23,      ///< get output path1 data format type
    IME_GET_P1_SPRTOUT_ENABLE           = 24,      ///< get output path1 data format type

    IME_GET_P2_ENABLE_STATUS            = 41,      ///< get output path2 enable status
    IME_GET_P2_OUTPUT_FORMAT            = 42,      ///< get output path2 data format
    IME_GET_P2_OUTPUT_TYPE              = 43,      ///< get output path2 data format type
    IME_GET_P2_SPRTOUT_ENABLE           = 44,      ///< get output path1 data format type

    IME_GET_P3_ENABLE_STATUS            = 61,      ///< get output path3 enable status
    IME_GET_P3_OUTPUT_FORMAT            = 62,      ///< get output path3 data format
    IME_GET_P3_OUTPUT_TYPE              = 63,      ///< get output path3 data format type
    IME_GET_P3_SPRTOUT_ENABLE           = 64,      ///< get output path1 data format type

    IME_GET_P4_ENABLE_STATUS            = 81,      ///< get output path3 enable status
    IME_GET_P4_OUTPUT_FORMAT            = 82,      ///< get output path3 data format
    IME_GET_P4_OUTPUT_TYPE              = 83,      ///< get output path3 data format type

    IME_GET_DEBUG_MESSAGE               = 998,      ///< get HW debug message
    IME_GET_FUNCTIONS_STATUS            = 999,      ///< get function enable status
    ENUM_DUMMY4WORD(IME_GET_INFO_SEL)
} IME_GET_INFO_SEL;
//------------------------------------------------------------------------------

//structure data type
/**
    IME structure - engine open object
*/
typedef struct _IME_OPENOBJ
{
  void (*FP_IMEISR_CB)(UINT32 intstatus);   ///< External ISR call-back function
  UINT32 uiImeClockSel;                     ///< Engine clock selection
} IME_OPENOBJ,*pIME_OPENOBJ;

/**
    IME structure - image size parameters for horizontal and vertical direction
*/
typedef struct _IME_SIZE_INFO
{
  UINT32 uiSizeH;                  ///< horizontal size
  UINT32 uiSizeV;                  ///< vertical size
} IME_SIZE_INFO;


/**
    IME structure - DMA address parameters for YUV channels
*/
typedef struct _IME_DMA_ADDR_INFO
{
  UINT32 uiAddrY;    ///< DMA address of Y channel
  UINT32 uiAddrCb;   ///< DMA address of Cb channel
  UINT32 uiAddrCr;   ///< DMA address of Cr channel
} IME_DMA_ADDR_INFO;


/**
    IME structure - image lineoffset parameters for luma and chroma channels
*/
typedef struct _IME_LINEOFFSET_INFO
{
  UINT32 uiLineOffsetY;   ///< line-offset of Y channel
  UINT32 uiLineOffsetCb;  ///< line-offset of Cb channel
} IME_LINEOFFSET_INFO;


/**
    IME structure - image cooridinate for horizontal and vertical directions
*/
typedef struct _IME_POS_INFO
{
  UINT32 uiPosX;
  UINT32 uiPosY;
} IME_POS_INFO;

/**
    IME structure - input stripe parameters
*/
typedef struct _IME_STRIPE_INFO
{
    UINT32    uiN;        ///< stripe size in each stripe
    UINT32    uiL;        ///< stripe size of last stripe
    UINT32    uiM;        ///< stripe number
} IME_STRIPE_INFO;

/**
    IME structure - horizontal and vertical stripe parameters
*/
typedef struct _IME_HV_STRIPE_INFO
{
  IME_STRIPE_INFO       StpH;           ///< hroizontal stripe parameters
  IME_STRIPE_INFO       StpV;           ///< Vertical stripe parameters

  IME_H_STRIPE_OVLP_SEL HOverlapSel;    ///< IME input stripe overlap size selection for horizontal direction
  UINT32 uiHOverlap;                    ///< IME input stripe overlap size for horizontal direction
  IME_STRIPE_MODE StripeCalMode;  ///< IME input stripe information setting mode
} IME_HV_STRIPE_INFO;


/**
    IME structure - scaling factors for horizontal and vertical direction
*/
typedef struct _IME_SCALE_FACTOR_INFO
{
    IME_SCALE_TYPE_SEL    HScaleUD;           ///< horizontal scale up/down selection
    UINT32                uiHScaleDR;         ///< horizontal scale down rate
    UINT32                uiHScaleFtr;        ///< horizontal scale factor
    UINT32                uiHIsdScaleBaseFtr; ///< horizontal scale base
    UINT32                uiIsdScaleFtrH[3];  ///< horizontal scale factor for integration method

    IME_SCALE_TYPE_SEL    VScaleUD;           ///< vertical scale up/down selection
    UINT32                uiVScaleDR;         ///< vertical scale down rate
    UINT32                uiVScaleFtr;        ///< vertical scale factor  
    UINT32                uiVIsdScaleBaseFtr; ///< horizontal scale base
    UINT32                uiIsdScaleFtrV[3];  ///< vertical scale factor for integration method

    IME_SCALE_FACTOR_COEF_MODE CalScaleFactorMode;  ///< scale factor mode
} IME_SCALE_FACTOR_INFO;

/**
    IME structure - scale filter parameters
*/
typedef struct _IME_SCALE_FILTER_INFO
{
    IME_FUNC_EN                 HScaleFilterEnable;           ///< scaling filter enable of horizontal direction
    UINT32                      uiHFilterCoef;     ///< scaling filter parameters of horizontal direction
    IME_FUNC_EN                 VScaleFilterEnable;           ///< scaling filter enable of vertical direction
    UINT32                      uiVFilterCoef;     ///< scaling filter parameters of vertical direction
    IME_SCALE_FILTER_COEF_MODE  CoefCalMode;   ///< filter parameter mode
} IME_SCALE_FILTER_INFO;


/**
    IME structure - scale enhancement parameters
*/
//@{
typedef struct _IME_SCALE_ENH_INFO
{
  UINT32  uiEnhFactor;         ///< scaling enhancement factor
  UINT32  uiEnhBit;            ///< scaling enhancement normailizaion bit
  IME_SCALE_ENH_MODE EnhMode;  ///< scaling enhancement mode
} IME_SCALE_ENH_INFO;
//@}


/**
    IME structure - output image data format parameters
*/
typedef struct _IME_OUTPUT_FORMAT_INFO
{
    IME_OUTPUT_IMG_FORMAT_SEL  OutFormatSel;    ///< path output format
    IME_OUTPUT_FORMAT_TYPE     OutFormatTypeSel;  ///< paht output format type
} IME_OUTPUT_FORMAT_INFO;


//=====================================================================================
// IQ function structure
/**
    IME structure - color space transformation parameters
*/
typedef struct _IME_CST_INFO
{
  IME_FUNC_EN   Rgb2YccEnable; ///< Function enable, RGB to YCbCr
  IME_FUNC_EN   Ycc2RgbEnable; ///< Function enable, YcbCr to RGB
} IME_CST_INFO;


/**
    IME structure - image parameters of local chroma adaptation
*/
typedef struct _IME_CHROMA_ADAPTION_IMAGE_INFO
{
    IME_SIZE_INFO       LcaImgSize;     ///< input reference image size
    IME_LINEOFFSET_INFO LcaLofs;        ///< input reference image lineoffset
    IME_LCA_FORMAT_SEL  LcaFmt;         ///< input reference image format
    IME_DMA_ADDR_INFO   LcaDmaAddr0;    ///< input reference imaeg buffer address0
    IME_DMA_ADDR_INFO   LcaDmaAddr1;    ///< input reference imaeg buffer address1
    IME_FUNC_EN         LcaPPBEnable;       ///< local chroma adaption - input ping-pong buffer enable
} IME_CHROMA_ADAPTION_IMAGE_INFO;

/**
    IME structure - local chroma adaptation parameters
*/
typedef struct _IME_CHROMA_ADAPTION_IQC_INFO
{
    UINT32              LcaRefYWt;      ///< Chroma reference weighting for Y channels
    UINT32              LcaRefUVWt;      ///< Chroma reference weighting for UV channels
    UINT32              LcaOutUVWt;        ///< Chroma adaptation output weighting

    IME_RANGE_SEL       LcaYRng;         ///< Chroma adaptation range selection for UV channels, Max = IME_RANGE_128
    IME_RANGE_SEL       LcaYWtPrc;        ///< Chroma adaptation weighting precision for UV channel, Max = IME_RANGE_64
    UINT32              LcaYTh;          ///< Chroma adaptation range threshold for UV channels
    UINT32              LcaYWtS;         ///< Chroma adaptation weighting start value for UV channels
    UINT32              LcaYWtE;        ///< Chroma adaptation weighting end value for UV channels


    IME_RANGE_SEL       LcaUVRng;         ///< Chroma adaptation range selection for UV channels, Max = IME_RANGE_128
    IME_RANGE_SEL       LcaUVWtPrc;        ///< Chroma adaptation weighting precision for UV channel, Max = IME_RANGE_64
    UINT32              LcaUVTh;          ///< Chroma adaptation range threshold for UV channels
    UINT32              LcaUVWtS;         ///< Chroma adaptation weighting start value for UV channels
    UINT32              LcaUVWtE;        ///< Chroma adaptation weighting end value for UV channels
} IME_CHROMA_ADAPTION_IQC_INFO;

/**
    IME structure - chroma adjustment parameters of local chroma adaptation
*/
typedef struct _IME_CHROMA_ADAPTION_CA_INFO
{
    IME_FUNC_EN     LcaCaEnable;         ///< Function enable
    UINT32          LcaCaCentU;      ///< chroma adjustment reference center of U channel
    UINT32          LcaCaCentV;      ///< chroma adjustment reference center of V channel
    IME_RANGE_SEL   LcaCaUVRng;         ///< Chroma adjustment range selection for UV channels, Max = IME_RANGE_128
    IME_RANGE_SEL   LcaCaUVWtPrc;        ///< Chroma adjustment weighting precision for UV channel, Max = IME_RANGE_64
    UINT32          LcaCaUVTh;          ///< Chroma adjustment range threshold for UV channels
    UINT32          LcaCaUVWtS;         ///< Chroma adjustment weighting start value for UV channels
    UINT32          LcaCaUVWtE;        ///< Chroma adjustment weighting end value for UV channels
} IME_CHROMA_ADAPTION_CA_INFO;

/**
    IME structure - luma suppression parameters of local chroma adaptation
*/
typedef struct _IME_CHROMA_ADAPTION_IQL_INFO
{
    IME_FUNC_EN     LcaLaEnable;         ///< Function enable
    UINT32          LcaLaRefWt;      ///< Luma adaptation reference weighting for UV channels
    UINT32          LcaLaOutWt;        ///< Luma adaptation output weighting
    IME_RANGE_SEL   LcaLaRng;         ///< Luma adaptation range selection for UV channels, Max = IME_RANGE_128
    IME_RANGE_SEL   LcaLaWtPrc;        ///< Luma adaptation weighting precision for UV channel, Max = IME_RANGE_64
    UINT32          LcaLaTh;          ///< Luma adaptation range threshold for UV channels
    UINT32          LcaLaWtS;         ///< Luma adaptation weighting start value for UV channels
    UINT32          LcaLaWtE;        ///< Luma adaptation weighting end value for UV channels
} IME_CHROMA_ADAPTION_IQL_INFO;

/**
    IME structure - local chroma adaptation parameters

    @note integrated structure
*/
typedef struct _IME_CHROMA_ADAPTION_INFO
{
    IME_FUNC_EN                     LcaEnable;          ///< Function enable

    IME_CHROMA_ADAPTION_IMAGE_INFO  LcaImageInfo;       ///< Reference image information
    IME_CHROMA_ADAPTION_IQC_INFO    LcaIqCInfo;           ///< Function quality information for chroma channel
    IME_CHROMA_ADAPTION_IQL_INFO    LcaIqLInfo;           ///< Function quality information for luma channel
    IME_CHROMA_ADAPTION_CA_INFO     LcaCaInfo;          ///< Chroma adjustment information
} IME_CHROMA_ADAPTION_INFO;


/**
    IME structure - quality parameters of dark and bright region chroma suppression
*/
typedef struct _IME_DBCS_IQ_INFO
{
    IME_DBCS_MODE_SEL OpMode;     ///< Process mode
    UINT32            uiCentU;    ///< Center value for U channel
    UINT32            uiCentV;    ///< Center value for V channel
    UINT32            uiStepY;    ///< Step for luma
    UINT32            uiStepC;   ///<  Step for chroma
    UINT32            *puiWtY;    ///< Weighting LUT for luma channel, 16 elements
    UINT32            *puiWtC;    ///< Weighting LUT for luma channel, 16 elements
} IME_DBCS_IQ_INFO;

/**
    IME structure - dark and bright region chroma suppression

    @note integrated structure
*/
typedef struct _IME_DBCS_INFO
{
  IME_FUNC_EN       DbcsEnable; ///< Function enable
  IME_DBCS_IQ_INFO  DbcsIqInfo; ///< function quality parameters
} IME_DBCS_INFO;


/**
    IME structure - edge direction thresholds of SSR function
*/
typedef struct _IME_SSR_IQ_INFO
{
  UINT32        uiDTh;   ///< Threahold for diagonal direction
  UINT32        uiHVTh;   ///< Threahold for horizontal and vertical direction
} IME_SSR_IQ_INFO;


/**
    IME structure - single image super-resolution parameters

    @note integrated structure
*/
typedef struct _IME_SSR_INFO
{
  IME_FUNC_EN       SsrEnable;    ///< Function enable
  IME_SSR_MODE_SEL  SsrMode;      ///< Processing mode
  IME_SSR_IQ_INFO   SsrIqInfo;    ///< Function quality parameters
} IME_SSR_INFO;



/**
    IME structure - quaility parameters of film grain noise
*/
typedef struct _IME_FILM_GRAIN_IQ_INFO
{
  UINT32    uiFgnLumTh;           ///< luminance value threshold
  UINT32    uiFgnNLP1;           ///< path1 noise level
  UINT32    uiFgnInitP1;          ///< Grain noise initial value for path1
  UINT32    uiFgnNLP2;           ///< path2 noise level
  UINT32    uiFgnInitP2;          ///< Grain noise initial value for path2
  UINT32    uiFgnNLP3;           ///< path3 noise level
  UINT32    uiFgnInitP3;          ///< Grain noise initial value for path3
} IME_FILM_GRAIN_IQ_INFO;

/**
    IME structure - film grain noise parameters

    @note integrated structure
*/
typedef struct _IME_FILM_GRAIN_INFO
{
  IME_FUNC_EN               FgnEnable;    ///< film grain enable
  IME_FILM_GRAIN_IQ_INFO    FgnIqInfo;    ///< function quality parameters
} IME_FILM_GRAIN_INFO;


/**
    IME structure - image parameters for data stamp
*/
typedef struct _IME_STAMP_IMAGE_INFO
{
    IME_SIZE_INFO   DsImgSize;  ///< Image size
    IME_DS_MODE_SEL DsMode;     ///< Data operating mode, only for Set1/2/3
    IME_POS_INFO    DsPos;      ///< Blending position
    UINT32          uiDsLofs;   ///< Data lineoffset
    UINT32          uiDsAddr;   ///< Data address
} IME_STAMP_IMAGE_INFO;

/**
    IME structure - image parameters for data stamp
*/
typedef struct _IME_STAMP_IQ_INFO
{
    UINT32 uiDsCkY;     ///< Color key of Y channel
    UINT32 uiDsCkU;     ///< Color key of U channel
    UINT32 uiDsCkV;     ///< Color key of V channel
    UINT32 uiBldWt;     ///< Blending weighting
} IME_STAMP_IQ_INFO;

/**
    IME structure - data stamp parameters of each set
*/
typedef struct _IME_STAMP_SET_INFO
{
    IME_FUNC_EN             DsEnable;      ///< Function enable
    IME_STAMP_IMAGE_INFO    DsImageInfo;   ///< Image information
    IME_STAMP_IQ_INFO       DsIqInfo;      ///< Data stamp quality information
} IME_STAMP_SET_INFO;

/**
    IME structure - YUV color LUT parameters of data stamp
*/
typedef struct _IME_STAMP_YUVLUT_INFO
{
    UINT32  *puiColorLutY; ///< Y channel LUT, 16 elements
    UINT32  *puiColorLutU; ///< Y channel LUT, 16 elements
    UINT32  *puiColorLutV; ///< Y channel LUT, 16 elements
} IME_STAMP_YUVLUT_INFO;

/**
    IME structure - data stamp parameters

    @note integrated structure
*/
typedef struct _IME_STAMP_INFO
{
    IME_STAMP_SET_INFO  DsSet0;         ///< data stamp set0
    IME_STAMP_SET_INFO  DsSet1;         ///< data stamp set1
    IME_STAMP_SET_INFO  DsSet2;         ///< data stamp set2
    IME_STAMP_SET_INFO  DsSet3;         ///< data stamp set3
    UINT32              *puiColorLutY; ///< Y channel LUT, 16 elements
    UINT32              *puiColorLutU; ///< Y channel LUT, 16 elements
    UINT32              *puiColorLutV; ///< Y channel LUT, 16 elements
} IME_STAMP_INFO;


/**
    IME structure - row position parameters of statistical
*/
typedef struct _IME_STL_ROWPOS_INFO
{
    UINT32 uiStlRow0;  ///< row position0
    UINT32 uiStlRow1;  ///< row posotion1
} IME_STL_ROWPOS_INFO;

/**
    IME structure - accmulation target parameters of statistical
*/
typedef struct _IME_STL_ACCTAG_INFO
{
    UINT32 uiAccTag0;  ///< accmulation target of histogram of set0
    UINT32 uiAccTag1;  ///< accmulation target of histogram of set0
} IME_STL_ACCTAG_INFO;

/**
    IME structure - edge map parameters of statistical
*/
typedef struct _IME_STL_EDGE_INFO
{
    IME_FUNC_EN      StlEdgeKerEn0; ///< edge kernel enable for set0  
    IME_STL_EKER_SEL StlEdgeKer0;   ///< edge map kernel selection for map0
    UINT32           uiStlSft0;     ///< edge map shift for map0

    IME_FUNC_EN      StlEdgeKerEn1; ///< edge kernel enable for set1
    IME_STL_EKER_SEL StlEdgeKer1;   ///< edge map kernel selection for map1
    UINT32           uiStlSft1;     ///< edge map shift for map1
} IME_STL_EDGE_INFO;

/**
    IME structure - ROI threshold parameters of statistical
*/
typedef struct _IME_STL_ROITH_INFO
{
    UINT32 uiRoiTh0;    ///< statistical threshold of ROI for section0
    UINT32 uiRoiTh1;    ///< statistical threshold of ROI for section1
    UINT32 uiRoiTh2;    ///< statistical threshold of ROI for section2
} IME_STL_ROITH_INFO;

/**
    IME structure - edge processing parameters of statistic

    @note integrated structure
*/
typedef struct _IME_STL_ROI_INFO
{
    IME_STL_ROWPOS_INFO StlRowPos;      ///< row position of statistic
    IME_STL_ROITH_INFO  StlRoi0;           ///< ROI0 for edge map
    IME_STL_ROITH_INFO  StlRoi1;           ///< ROI1 for edge map
    IME_STL_ROITH_INFO  StlRoi2;           ///< ROI2 for edge map
    IME_STL_ROITH_INFO  StlRoi3;           ///< ROI3 for edge map
    IME_STL_ROITH_INFO  StlRoi4;           ///< ROI4 for edge map
    UINT32              uiStlEdgeMapAddr; ///< edge map output address
} IME_STL_ROI_INFO;

/**
    IME structure - histogram of statistic

    @note integrated structure
*/
typedef struct _IME_STL_HIST_INFO
{
    IME_POS_INFO        StlHistPos;     ///< start posotion for edge statistic
    IME_SIZE_INFO       StlHistSize;     ///< image size for edge statistic
    IME_STL_ACCTAG_INFO StlHistAccTag;  ///< histogram accumulation target
    UINT32              uiStlHistAddr;    ///< output address for edge statistic
} IME_STL_HIST_INFO;

/**
    IME structure - statistical parameters

    @note integrated structure
*/
typedef struct _IME_STL_INFO
{
    IME_FUNC_EN         StlEnable;      ///< enable of statistic
    IME_FUNC_EN         StlFtrEnable;    ///< filter enable of statistic    
    IME_STL_EDGE_INFO   StlEdgeMap;     ///< edge map of statistic
    IME_STL_HIST_INFO   StlHist;        ///< edge histogram
    IME_STL_ROI_INFO    StlRoi;         ///< ROI threshold of edge map 
    IME_STL_IMGOUT_SEL  StlImgOutType;  ///< output before or after filter image
} IME_STL_INFO;

/**
    IME structure - get statistical parameters
*/
typedef struct _IME_GET_STL_INFO
{
    UINT32 uiGetHistMax0;      ///< max value of histogram area of set0
    UINT32 uiGetAccTagBin0;    ///< hit bin of histogram accumulation of set0
    
    UINT32 uiGetHistMax1;      ///< max value of histogram area of set1    
    UINT32 uiGetAccTagBin1;    ///< hit bin of histogram accumulation of set1    
} IME_GET_STL_INFO;


/**
    IME structure - stitching parameters
*/
typedef struct _IME_STITCH_INFO
{    
    UINT32 uiStitchPos;                     ///< stitching position of image
    BOOL bLofsUpdate;                       ///< stitching output lineoffset update, or not
    IME_LINEOFFSET_INFO StitchLineoffset;   ///< stitching image lineoffset
    BOOL bDmaAddrUpdate;                    ///< stitching output DMA address update, or not
    IME_DMA_ADDR_INFO StitchDmaAddr;        ///< stitching image DMA address
} IME_STITCH_INFO;




//------------------------------------------------------------------------------
// global flow structure

/**
    IME structure - grouped function parameters

    @note integrated structure - used to ime_setMode for initial state
*/
typedef struct _IME_IQ_FLOW_INFO
{
    IME_FUNC_EN                 *pCmfInfo;            ///< chroma median filter parameter
    IME_CHROMA_ADAPTION_INFO    *pChromaAdaptionInfo; ///< chroma adaption parameters
    IME_DBCS_INFO               *pDbcsInfo;             ///< dark and bright region chroma supression parameters
    IME_SSR_INFO                *pSsrInfo;              ///< single image supre-resolution parameters
    IME_CST_INFO                *pColorSpaceTrans;    ///< color space transformation parameters
    IME_FILM_GRAIN_INFO         *pFilmGrainInfo;      ///< film grain parameters
    IME_STAMP_INFO              *pDataStampInfo;        ///< Data stamp parameters
    IME_FUNC_EN                 *pP2IInfo;            ///< progressive to interlaced parameter
    IME_STL_INFO                *pStlInfo;            ///< edge statistic parameters
} IME_IQ_FLOW_INFO;


/**
    IME structure - all input path parameters

    @note integrated structure
*/
typedef struct _IME_INPATH_INFO
{
    IME_SIZE_INFO             InSize;               ///< input image size
    IME_INPUT_FORMAT_SEL      InFormat;                ///< input image format
    IME_LINEOFFSET_INFO       InLineoffset;            ///< input line offset
    IME_DMA_ADDR_INFO         InAddr;               ///< input DMA address
} IME_INPATH_INFO;


/**
    IME structure - all output path parameters

    @note integrated structure
*/
typedef struct _IME_OUTPATH_INFO
{    
    IME_FUNC_EN               OutPathEnable;             ///< path enable
    IME_FUNC_EN               OutPathDramEnable;          ///< path output to dram enable
    IME_FUNC_EN               OutPathSprtOutEnable;          ///< path separated output to dram enable, also set "OutPathSprtPos"
    IME_OUTDST_CTRL_SEL       OutPathOutDest;           ///< path output destination; path1: dram/H264, others: dram
    IME_OUTPUT_FORMAT_INFO    OutPathImageFormat;            ///< path output image format
    IME_SIZE_INFO             OutPathScaleSize;     ///< path image scaling output size   
    IME_SCALE_FACTOR_INFO     OutPathScaleFactors;  ///< path image scaling facotrs
    IME_SCALE_FILTER_INFO     OutPathScaleFilter;   ///< path scaling filter parameters
    IME_SCALE_METHOD_SEL      OutPathScaleMethod;   ///< path scale method
    IME_SCALE_ENH_INFO        OutPathScaleEnh;      ///< path scale enhancement, for bicubic scaling method only
    IME_DMA_ADDR_INFO         OutPathAddr;           ///< path output DMA address
    IME_POS_INFO              OutPathCropPos;           ///< path crop position
    IME_SIZE_INFO             OutPathOutSize;           ///< path image crop output size
    IME_LINEOFFSET_INFO       OutPathOutLineoffset;        ///< path output lineoffset
    
    IME_DMA_ADDR_INFO         OutPathAddr2;           ///< path output DMA address for separating output
    IME_LINEOFFSET_INFO       OutPathOutLineoffset2;        ///< path output lineoffset for separating output
    UINT32                    OutPathSprtPos;           ///< path output separating position
} IME_OUTPATH_INFO;


/**
    IME structure - full engine parameters

    @note integrated structure - used to ime_setMode for initial state
*/
typedef struct _IME_MODE_PRAM
{
  // input info
  IME_OPMODE                OperationMode;      ///< IME performing mode
  IME_INPATH_INFO           InPathInfo;         ///< IME input path parameters

  //output info
  IME_OUTPATH_INFO          OutPath1;           ///< IME output path1 parameters
  IME_OUTPATH_INFO          OutPath2;           ///< IME output path2 parameters
  IME_OUTPATH_INFO          OutPath3;           ///< IME output path3 parameters
  IME_OUTPATH_INFO          OutPath4;           ///< IME output path4 parameters 
  
  UINT32                    uiInterruptEnable;  ///< interrupt enable
  IME_HV_STRIPE_INFO        SetStripe;

  IME_IQ_FLOW_INFO          *pImeIQInfo;        ///< IQ related parameters
}IME_MODE_PRAM, *pIME_MODE_PRAM;


/**
    IME structure - change output scaling enhancement parameters
*/
typedef struct _IME_OUTPUT_SCALE_ENH
{
  IME_SCALE_ENH_INFO   P1OutScaleEnh;   ///< scale enhancement of path1
  IME_SCALE_ENH_INFO   P2OutScaleEnh;   ///< scale enhancement of path2
  IME_SCALE_ENH_INFO   P3OutScaleEnh;   ///< scale enhancement of path3 
  IME_SCALE_ENH_INFO   P4OutScaleEnh;   ///< scale enhancement of path4
} IME_OUTPUT_SCALE_ENH;


/**
    IME structure - change output scaling filter parameters
*/
typedef struct _IME_OUTPUT_SCALE_FILTER
{
  IME_SCALE_FILTER_INFO   P1OutScaleFilter;   ///< scale filter of path1
  IME_SCALE_FILTER_INFO   P2OutScaleFilter;   ///< scale filter of path2
  IME_SCALE_FILTER_INFO   P3OutScaleFilter;   ///< scale filter of path3 
  IME_SCALE_FILTER_INFO   P4OutScaleFilter;   ///< scale filter of path4
} IME_OUTPUT_SCALE_FILTER;

/**
    IME structure - change DMA channel burst length parameters
*/
typedef struct _IME_BURST_LENGTH
{
    IME_BURST_SEL BurstLenInputLCA;  // burst length of output path4 Y;    
    IME_BURST_SEL BurstLenOutputP1Y;  // burst length of output path1 Y; 
    IME_BURST_SEL BurstLenOutputP1U;  // burst length of output path1 U; 
    IME_BURST_SEL BurstLenOutputP1V;  // burst length of output path1 V;
    IME_BURST_SEL BurstLenOutputP2Y;  // burst length of output path2 Y; 
    IME_BURST_SEL BurstLenOutputP2U;  // burst length of output path2 U; 
    IME_BURST_SEL BurstLenOutputP3Y;  // burst length of output path3 Y; 
    IME_BURST_SEL BurstLenOutputP3U;  // burst length of output path3 U; 
    IME_BURST_SEL BurstLenOutputP4Y;  // burst length of output path4 Y;
} IME_BURST_LENGTH;

//------------------------------------------------------------------------------
// flow control APIs

/**
    IME engine is opened or not

    To check IME engine is opened or closed.

    @param[in] VOID

    @return BOOL  Status of performing this API.\n
    -@b TRUE:   Engine is opened.\n
    -@b FALSE:  Engine is closed.\n
*/
extern BOOL ime_isOpen(VOID);
//------------------------------------------------------------------------------

/**
    Open IME engine

    This function should be called before calling any other functions.

    @param[in] pImeOpenInfo Open and get engine resource

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_open(IME_OPENOBJ *pImeOpenInfo);
//------------------------------------------------------------------------------

/**
    Set IME engine mode and parameters

    @param[in] pEngineInfo mode information and parameters.\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_setMode(IME_MODE_PRAM *pEngineInfo);
//------------------------------------------------------------------------------

/**
    IME engine start

    @param[in] VOID

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_setStart(VOID);
//------------------------------------------------------------------------------

/**
    IME engine pause

    @param[in] VOID

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_pause(VOID);
//------------------------------------------------------------------------------

/**
    IME engine close

    @param[in] VOID

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_close(VOID);
//------------------------------------------------------------------------------

/**
    Wait system flag when frame-end occurred

    @param[in] IsClearFlag  Clear flag selection.\n
    
    @return ER error code\n
    -@b E_OK: desired flag is done.\n
*/
extern ER ime_waitFlagFrameEnd(IME_FLAG_CLEAR_SEL IsClearFlag);
//------------------------------------------------------------------------------


/**
    Clear frame-end flag of system
    
    @param[in] VOID\n

    @return VOID\n
*/
extern VOID ime_clearFlagFrameEnd(VOID);
//------------------------------------------------------------------------------


/**
    Set Stripe Information

    @param[in] pStripeInfo stripe information of horizontal and vertical directions.

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgStripeParam(IME_HV_STRIPE_INFO *pStripeInfo);
//------------------------------------------------------------------------------

/**
    Get input stripe size information

    @param[in] pGetStripeInfo  information of input stripe size

    @return VOID\n
*/
extern VOID ime_getInputPathStripeInfo(IME_HV_STRIPE_INFO *pGetStripeInfo);
//------------------------------------------------------------------------------    


/**
    Set input path parameters

    @param[in] pSetInInfo Input path setting\n
    @param[in] LoadType load type selection\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgInputPathParam(IME_INPATH_INFO *pSetInInfo);
//------------------------------------------------------------------------------

/**
    Set output path parameters 

    @param[in] OutPathSel output path selection\n
    @param[in] pSetOutInfo output path setting\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgOutputPathParam(IME_PATH_SEL OutPathSel, IME_OUTPATH_INFO *pSetOutInfo);
//------------------------------------------------------------------------------

/**
    Change Output Path Enable

    @param[in] PathSel  output path selection\n
    @param[in] SetEn    enable or disable setting\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgOutputPathEnable(IME_PATH_SEL PathSel, IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Output Path DRAM-Out Enable

    @param[in] PathSel  output path selection\n
    @param[in] SetEn    enable or disable setting\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgOutputPathDramOutEnable(IME_PATH_SEL PathSel, IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Output Scaling Filter Parameters

    @param[in] PathSel      output path selection\n
    @param[in] pSetSclFilter   scale filter setting\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgOutputScalingFilterParam(IME_PATH_SEL PathSel, IME_SCALE_FILTER_INFO *pSetSclFilter);
//------------------------------------------------------------------------------

/**
    Change Output Scaling Factor Parameters

    @param[in] PathSel      output path selection\n
    @param[in] pSetSclFactor   scale factor setting\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgOutputScalingFactorParam(IME_PATH_SEL PathSel, IME_SCALE_FACTOR_INFO *pSetSclFactor);
//------------------------------------------------------------------------------


/**
    Change Output Scaling Enhancement Parameters

    @param[in] PathSel      output path selection\n
    @param[in] pSetSclEnh   image enhancement setting\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgOutputScalingEnhanceParam(IME_PATH_SEL PathSel, IME_SCALE_ENH_INFO *pSetSclEnh);
//------------------------------------------------------------------------------

/**
    Change Chroma Median Filter Enable

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaMedianFilterEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation Enable

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation - Chroma Adjustment Enable

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptChromaAdjustEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation - Luma Suppression Enable

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptLumaSuppressEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation Image Parameters

    @param[in] pSetInfo       image information

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptImageParam(IME_CHROMA_ADAPTION_IMAGE_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation - Chroma Adjustment Parameters

    @param[in] pSetInfo       chroma adjustment parameters

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptChromaAdjustParam(IME_CHROMA_ADAPTION_CA_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation Parameters

    @param[in] pSetInfo       chroma adjustment parameters

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptParam(IME_CHROMA_ADAPTION_IQC_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Chroma Adaptation - Luma Suppress Parameters

    @param[in] pSetInfo       chroma adjustment parameters

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgChromaAdaptLumaSuppressParam(IME_CHROMA_ADAPTION_IQL_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Get Chroma Adaptation - Image size

    @param[in] pGetSize     get imaeg size

    @return VOID\n
*/
extern VOID ime_getChromaAdaptImageSizeInfo(IME_SIZE_INFO *pGetSize);
//------------------------------------------------------------------------------

/**
    Get Chroma Adaptation - Lineoffset

    @param[in] pGetLofs     get lineoffset size

    @return VOID\n
*/
extern VOID ime_getChromaAdaptLineoffsetInfo(IME_LINEOFFSET_INFO *pGetLofs);
//------------------------------------------------------------------------------

/**
    Get Chroma Adaptation - DMA address

    @param[in] pGetLofs     get DMA address

    @return VOID\n
*/
extern VOID ime_getChromaAdaptDmaAddrInfo(IME_DMA_ADDR_INFO *pGetAddr0, IME_DMA_ADDR_INFO *pGetAddr1);
//------------------------------------------------------------------------------


/**
    Change Color Space Transformation Parameters

    Change Color Space Transformation Parameters

    @param[in] pCstParam    Color space transformation enable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgColorSpaceTransformEnable(IME_CST_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Dark and Bright Region Chroma Suppression Enable

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgDrkBrtChromaSuppressEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------


/**
    Change Dark and Bright Region Chroma Suppression Parameters

    @param[in] pSetInfo      dark and rright region chroma suppression parameters

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgDrkBrtChromaSuppressParam(IME_DBCS_IQ_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Single Image Super-resolution Enable

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgSingleSuperResEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Single Image Super-resolution Parameters

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgSingleSuperResParam(IME_SSR_IQ_INFO *pSetInfo);
//------------------------------------------------------------------------------


/**
    Change Film Grain Noise Enable

    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgFilmGrainNoiseEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------


/**
    Change Film Grain Noise Parameters

    Change Film Grain Noise Parameters

    @param pFgnParam Film grain noise parameters, please refer to data sturcture "IME_FILM_GRAIN_INFO"

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgFilmGrainNoiseParam(IME_FILM_GRAIN_IQ_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Progressive to interlace Enable

    @param[in] pSetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgProgressiveToInterlaceEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Data Stamp Enable

    @param[in] SetNum        which Set selection
    @param[in] SetEn         function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgDataStampEnable(IME_DS_SETNUM SetNum, IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Data Stamp Image Parameters

    @param[in] SetNum        which Set selection
    @param[in] pSetInfo     image information

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgDataStampImageParam(IME_DS_SETNUM SetNum, IME_STAMP_IMAGE_INFO *pSetInfo);
//------------------------------------------------------------------------------


/**
    Change Data Stamp Effect Parameters

    @param[in] SetNum        which Set selection
    @param[in] pSetInfo     effect information

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgDataStampParam(IME_DS_SETNUM SetNum, IME_STAMP_IQ_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Data Stamp Color LUT Parameters
    
    @param[in] pSetInfo     YUV color information for data stamp

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgDataStampColorLutParam(IME_STAMP_YUVLUT_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Get Data Stamp Image Size

    @param[in] SetNum       which Set selection\n
    @param[in] pGetSizeInfo     get image size\n

    @return VOID\n
*/
extern VOID ime_getDataStampImageSizeInfo(IME_DS_SETNUM SetNum, IME_SIZE_INFO *pGetSizeInfo);
//------------------------------------------------------------------------------

/**
    Get Data Stamp Lineoffset

    @param[in] SetNum       which Set selection\n
    @param[in] pGetLofsInfo     get image lineoffset\n

    @return VOID\n
*/
extern VOID ime_getDataStampLineoffsetInfo(IME_DS_SETNUM SetNum, IME_LINEOFFSET_INFO *pGetLofsInfo);
//------------------------------------------------------------------------------

/**
    Get Data Stamp DMA Address

    @param[in] SetNum       which Set selection\n
    @param[in] pGetAddrInfo     get DMA address\n

    @return VOID\n
*/
extern VOID ime_getDataStampDmaAddrInfo(IME_DS_SETNUM SetNum, IME_DMA_ADDR_INFO *pGetAddrInfo);
//------------------------------------------------------------------------------


/**
    Get Engine Inforomation

    @param[in] GetImeInfoSel desired information selection

    @return UINT32      desired information\n
*/
extern UINT32 ime_getEngineInfo(IME_GET_INFO_SEL GetImeInfoSel);
//------------------------------------------------------------------------------

/**
    Get input path image size

    @param[in] pGetSize get input path image size

    @return VOID\n
*/
extern VOID ime_getInputPathImageSizeInfo(IME_SIZE_INFO *pGetSize);
//------------------------------------------------------------------------------

/**
    Get input path lineoffset

    @param[in] pGetLofs get input path lineoffset

    @return VOID\n
*/
extern VOID ime_getInputPathLineoffsetInfo(IME_LINEOFFSET_INFO *pGetLofs);
//------------------------------------------------------------------------------

/**
    Get input path DMA address

    @param[in] pGetAddr get input path DMA address

    @return VOID\n
*/
extern VOID ime_getInputPathDMAAddrInfo(IME_DMA_ADDR_INFO *pGetAddr);
//---------------------------------------------------------------------------------------------

/**
    Get output path image size

    @param[in] PathSel  output path selection\n
    @param[in] pGetSize get input path image size\n

    @return VOID\n
*/
extern VOID ime_getOutputPathImageSizeInfo(IME_PATH_SEL PathSel, IME_SIZE_INFO *pGetSize);
//------------------------------------------------------------------------------

/**
    Get output path lineoffset

    @param[in] PathSel  output path selection\n
    @param[in] BufSel   output path buffer set selection\n
    @param[in] pGetSize get input path image size\n

    @return VOID\n
*/
extern VOID ime_getOutputPathLineoffsetInfo(IME_PATH_SEL PathSel, IME_PATH_OUTBUF_SEL BufSel, IME_LINEOFFSET_INFO *pGetLofs);
//------------------------------------------------------------------------------

/**
    Get output path DMA Address

    @param[in] PathSel  output path selection\n
    @param[in] BufSel   output path buffer set selection\n
    @param[in] pGetAddr get input path image size\n

    @return VOID\n
*/
extern VOID ime_getOutputPathDMAAddrInfo(IME_PATH_SEL PathSel, IME_PATH_OUTBUF_SEL BufSel, IME_DMA_ADDR_INFO *pGetAddr);
//------------------------------------------------------------------------------

/**
    Change Statistic Enable

    @param[in] SetEn        function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStatisticEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Statistic Filter Enable

    @param[in] SetEn        function enable/disable

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStatisticFilterEnable(IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------

/**
    Change Output Image Type

    @param[in] ImgOutSel    image output type, output before or after filter image

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStatisticalImageOutputType(IME_STL_IMGOUT_SEL ImgOutSel);
//------------------------------------------------------------------------------


/**
    Change Statistic Edge Kernel Parameters

    @param[in] pSetInfo     effect information

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStatisticEdgeKerParam(IME_STL_EDGE_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Statistic Histogram Parameters

    @param[in] pSetInfo     effect information

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStatisticHistogramParam(IME_STL_HIST_INFO *pSetInfo);
//------------------------------------------------------------------------------

/**
    Change Statistic Edge Map Parameters

    @param[in] pSetInfo     effect information

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStatisticEdgeROIParam(IME_STL_ROI_INFO *pSetInfo);
//------------------------------------------------------------------------------


/**
    Get Statistic Information

    This API can get the max value with in hisgoram region and accmulation target histogram bin

    @param[in] pGetHistInfo     desired information\n

    @return VOID\n
*/
extern VOID ime_getStatisticInfo(IME_GET_STL_INFO *pGetHistInfo);
//------------------------------------------------------------------------------


/**
    Get Edge Map Output Address

    @param[in] pGetAddr     edge map output address

    @return VOID\n
*/
extern VOID ime_getStatisticEdgeMapDmaAddrInfo(IME_DMA_ADDR_INFO *pGetAddr);
//------------------------------------------------------------------------------

/**
    Get Histogram Output Address

    @param[in] pGetAddr     histogram output address

    @return VOID\n
*/
extern VOID ime_getStatisticHistogramDmaAddrInfo(IME_DMA_ADDR_INFO *pGetAddr);
//------------------------------------------------------------------------------


/**
    Change Stitching Enable

    @param[in] PathSel  select stitching function on output path\n
    @param[in] SetEn    disable / enable control\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStitchingEnable(IME_PATH_SEL PathSel, IME_FUNC_EN SetEn);
//------------------------------------------------------------------------------


/**
    Change Stitching Image Parameters

    @param[in] PathSel  path selection\n
    @param[in] pStitchInfo  Stitching parameters\n

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgStitchingImageParam(IME_PATH_SEL PathSel, IME_STITCH_INFO *pStitchInfo);
//------------------------------------------------------------------------------


/**
  Get IME Engine Clock Rate

  This function is used to get the in-used clock rate of IME
*/
extern UINT32 ime_getClockRate(VOID);
//------------------------------------------------------------------------------

/**
    Set input or output channel burst lenght

    @param[in] pSetInfo   burst size parameters

    @return ER      error code\n
    -@b E_OK:       setting done\n
    -@b Others:     setting error\n
*/
extern ER ime_chgBurstLength(IME_BURST_LENGTH *pSetInfo);
//------------------------------------------------------------------------------

/**
    Get available max stripe size for IPP mode 

    @param[in] VOID

    @return UINT32  max stripe size\n
*/
extern UINT32 ime_getInputMaxStripeSize(VOID);
//------------------------------------------------------------------------------

/**
    Change all engine parameters except color key and LUT of data stamp function

    @param[in] pEngineInfo mode information and parameters.\n

    @return ER  error code\n
    -@b E_OK:   setting done\n
    -@b Others: setting error\n
*/
extern ER ime_chgAllParam(IME_MODE_PRAM *pEngineInfo);
//------------------------------------------------------------------------------

/**
    Get input and output burst length information

    @param[in] VOID

    @return VOID

*/
extern INT32 ime_getBurstLength(IME_GET_BSTSIZE_SEL GetBstSizeSel);


//@}


#ifdef __cplusplus
} //extern "C" {
#endif


#endif



