/**
    IPH module (IME sub function type define)

    @file       IPL_Hal_IME_Info.h NT96660
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_IME_Info_H
#define _IPL_Hal_IME_Info_H
#include "Type.h"
#include "IPL_Utility.h"
#include "pll.h"
/** \addtogroup mILibIPH */
//@{

#define IME_MaxParaSet  2
/**
    ime engine supported clk
*/
typedef enum _IPH_IMECLK
{
    IPH_IMECLK0 = 192,  ///< -192 mhz
    IPH_IMECLK1 = 240,  ///< -240 mhz
    IPH_IMECLK2 = 270,  ///< -270 mhz
    IPH_IMECLK3 = 300,  ///< -300 mhz
    ENUM_DUMMY4WORD(IPH_IMECLK)
}IPH_IMECLK;


typedef enum _IME_SRCMODE
{
    IME_D2D             = 0,        ///< IME only
    IME_IPP             = 1,        ///< IFE->DCE->IPE->IME
    IME_RDE_IPP         = 2,        ///< RDE + IFE->DCE->IPE->IME
    IME_RHE_IPP         = 3,        ///< RHE + IFE->DCE->IPE->IME
    IME_SIE_IPP         = 4,        ///< SIE + IFE->DCE->IPE->IME (all direct)
    ENUM_DUMMY4WORD(IME_SRCMODE)
}IME_SRCMODE;                      ///< IME operation mode
/**
    IME in/out data format
*/
typedef IPL_YUV_IMG_TYPE IMEFMT;

typedef struct
{
    UINT32  Y;
    UINT32  Cb;
    UINT32  Cr;
}IMEADDR;

typedef struct
{
    IPL_YCC_CH_INFO Y;
    UINT32  Lineoffset_CC;
}IMESIZE;

/**
    IME interrupt enable enum.
*/
typedef enum _ImeInteEn
{
    IME__CLR        = 0,
    IME__INTE_STPE  = 0x40000000,   ///< enable interrupt: stipe end
    IME__INTE_FMD   = 0x80000000,   ///< enable interrupt: frame end
}ImeInteEn;

/**
    IME sub function enable
*/
typedef enum _ImeFuncEn
{
    ChromaAdaptEn   = 0x00000001,   ///< enable function: chroma adaption (LCA)
    ChromaMedianEn  = 0x00000002,   ///< enable function: chroma median filter
    ChromaAdjLchrom = 0x00000004,   ///< enable function: chroma adjustment of local chroma adaptation
    ChromaAdjLum    = 0x00000008,   ///< enable function: chroma adjustment of local luma adaptation
    ChromaDBSuppres = 0x00000010,   ///< enable function: dark and bright region chroma suppression
    SingleImgSuperR = 0x00000020,   ///< enable function: single image super-resolution
    FilmGrainEn     = 0x00000040,   ///< enable function: film grain
    IMEP2IEn        = 0x00000080,   ///< enable function: P frame to I frame
    RGBtoYCCEn      = 0x00000100,   ///< enable function: RGB spaec to YCC space
    YCCtoRGBEn      = 0x00000200,   ///< enable function: YCC space to RGB space
    ENUM_DUMMY4WORD(ImeFuncEn)
}ImeFuncEn;
typedef struct _IME_GET_STATS_INFO
{
    UINT32 uiGetHistMax0;      ///< max value of histogram area of set0
    UINT32 uiGetAccTagBin0;    ///< hit bin of histogram accumulation of set0

    UINT32 uiGetHistMax1;      ///< max value of histogram area of set1
    UINT32 uiGetAccTagBin1;    ///< hit bin of histogram accumulation of set1
} IME_GET_STATS_INFO;

/**
    stripe overlap enum
*/
typedef enum _IME_STPESEL
{
    IME_STPE_OVERLAP_16     = 0,    ///< overlap 16 pixels
    IME_STPE_OVERLAP_24     = 1,    ///< overlap 24 pixels
    IME_STPE_OVERLAP_32     = 2,    ///< overlap 32 pixels
    ENUM_DUMMY4WORD(IME_STPESEL)
}IME_STPESEL;

/**
    stripe unit
*/
typedef struct _IME_STRIPE
{
    UINT32  N;          ///< Size of stripe except for last one, Horn. unit : 16 pix, Ver. unit : 1 line
    UINT32  L;          ///< Size of last stripe, Horn. unit : 16 pix, Ver. unit : 1 line
    UINT32  M;          ///< Number of total stripe - 1
}IME_STRIPE;

/**
     ime scaling method
     @note integration is supported only when image scaling down.
*/
typedef enum _IME_SCALER
{
    IME_BICUBIC     = 0,    ///< bicubic interpolation
    IME_BILINEAR    = 1,    ///< bilinear interpolation
    IME_NEAREST     = 2,    ///< nearest interpolation
    IME_INTEGRATION = 3,    ///< integration interpolation,support only scale down

    _AUTO           = 0xff, ///< automatical calculation
    ENUM_DUMMY4WORD(IME_SCALER)
}IME_SCALER;

/**
    IME output configuration
*/
typedef struct _STITCH_IMG
{
    UINT32      LineOfsY;       ///< path output lineoffset for separating output
    UINT32      LineOfsCC;      ///< path output lineoffset for separating output
    UINT32      StPos;          ///< path output separating position
} STITCH_IMG;

typedef struct _STITCH_INFO
{
    IMEADDR     Addr;           ///< path output DMA address for separating output
    STITCH_IMG  Img;            ///< stitch image information
}STITCH_INFO;

typedef struct _OUTCH
{
    BOOL    PathEn;         ///< path enable
    BOOL    DmaOutEn;       ///< data output to dram enable
    BOOL    OutDestSel;     ///< data output destination select (to Dram or to H264)
    BOOL    StitchEn;       ///< path separated output to dram enable, also set "OutPathSprtPos"
    IMEFMT  Fmt;            ///< format
    IMESIZE Size;           ///< scale output size
    struct
    {
        Coordinate  Start;  ///< -
        UINT32  SizeH;      ///< -
        UINT32  SizeV;      ///< -
    }Crop;                  ///< ime output crop window
    IMEADDR Addr;           ///< YCC address
    IME_SCALER ScalMethod;  ///< CbCr channel have no bicubic, scaling up have no integration(all 3 path).
    STITCH_INFO StitchInfo; ///< stitch information
    /*
    enum
    {
        Type_Y  = 0,    ///< Y channel output data type - Y
        Type_V  = 1,    ///< Y channel output data type - V
        ENUM_DUMMY4WORD(Y_CH_TypeSel)
    }Y_CH_TypeSel;
    */
}OUTCH;

/**
    IME output path
*/
typedef struct _IME_OUT
{
    OUTCH Path1;            ///< path1 output (output fmt support H.264/Jpeg)
    OUTCH Path2;            ///< path2 output (output fmt support y/(uv pack))
    OUTCH Path3;            ///< path3 output (output fmt support y/(uv pack))
    OUTCH Path4;            ///< path4 output (output fmt support only y output)
    UINT32 EdgeMapOutAddr;  ///< path4 edge map output
    UINT32 HistoOutAddr;    ///< path4 histo output
}IME_OUT;

/**
    scaling quality(enhance) parameter set

    @note: only work when ScalMethod = _Bicubic
*/
typedef struct _IMESCALE_Enh
{
    UINT32  Factor;     ///< -
    UINT32  Norm_bit;   ///< -
}IMESCALE_Enh;

/**
    scaling quality(filter)parameter set
*/
typedef struct _IMESCALE_Filt
{
    BOOL    CoefUserDefEn;      ///< user define mode enable bit
    struct
    {
        BOOL    HsfEn;          ///< H direction scaling filter
        BOOL    VsfEn;          ///< V direction scaling filter
        UINT32  HsfCoef;        ///< H direction coefficient
        UINT32  VsfCoef;        ///< V direction coefficient
    }UserDefine;                ///< user defined
}IMESCALE_Filt;

/**
    @name IME enum - Value Range Selection
*/
//@{
typedef enum _RANGE_SEL
{
    RANGE_8   = 0,  ///< range 8
    RANGE_16  = 1,  ///< range 16
    RANGE_32  = 2,  ///< range 32
    RANGE_64  = 3,  ///< range 64
    RANGE_128 = 4,  ///< range 128
    ENUM_DUMMY4WORD(RANGE_SEL)
} RANGE_SEL;
//@}
/**
    chroma adaption parameter set
*/
typedef struct _IME_CHRA_ADAP
{
    struct
    {
        UINT32          LcaRefY;        ///< Chroma reference weighting for Y channels
        UINT32          LcaRefUV;       ///< Chroma reference weighting for UV channels
        UINT32          LcaOutUV;       ///< Chroma adaptation output weighting
    }LcaRefWt;
    struct
    {
        RANGE_SEL       LcaY;           ///< Chroma adaptation range selection for Y channels, Max = IME_RANGE_128
        UINT32          LcaYTh;         ///< Chroma adaptation range threshold for Y channels
        RANGE_SEL       LcaYWtPrc;      ///< Chroma adaptation weighting precision for Y channel, Max = IME_RANGE_64
        UINT32          LcaYWtS;        ///< Chroma adaptation weighting start value for Y channels
        UINT32          LcaYWtE;        ///< Chroma adaptation weighting end value for Y channels
    }LcaYinfo;
    struct
    {
        RANGE_SEL       LcaUV;           ///< Chroma adaptation range selection for UV channels, Max = IME_RANGE_128
        UINT32          LcaUVTh;         ///< Chroma adaptation range threshold for UV channels
        RANGE_SEL       LcaUVWtPrc;      ///< Chroma adaptation weighting precision for UV channel, Max = IME_RANGE_64
        UINT32          LcaUVWtS;        ///< Chroma adaptation weighting start value for UV channels
        UINT32          LcaUVWtE;        ///< Chroma adaptation weighting end value for UV channels
    }LcaUVinfo;
    UINT32  SubRatio;       ///< sub out size: size/(1<<(SubRatio))
}IME_CHRA_ADAP;

typedef struct _IME_CHRA_CADAP
{
    BOOL            LcaCLREn;           ///< Function enable
    struct
    {
        UINT32          LcaCaCentU;     ///< chroma adjustment reference center of U channel
        UINT32          LcaCaCentV;     ///< chroma adjustment reference center of V channel
    }LcaRefWt;

    struct
    {
        RANGE_SEL       LcaCLRRng;      ///< Color adaptation range selection for UV channels, Max = IME_RANGE_128
        UINT32          LcaCLRTh;       ///< Color adaptation range threshold for UV channels
        RANGE_SEL       LcaCLRWtPrc;    ///< Color adaptation weighting precision for UV channel, Max = IME_RANGE_64
        UINT32          LcaCLRWtS;      ///< Color adaptation weighting start value for UV channels
        UINT32          LcaCLRWtE;      ///< Color adaptation weighting end value for UV channels
    }LcaCLRinfo;
} IME_CHRA_CADAP;

typedef struct _IME_CHRA_LADAP
{
    BOOL            LcaLumEn;           ///< Function enable

    struct
    {
        UINT32          LcaLumRefWt;    ///< Luma adaptation reference weighting for UV channels
        UINT32          LcaLumOutWt;    ///< Luma adaptation output weighting
    }LcaRefWt;

    struct
    {
        RANGE_SEL       LcaLumRng;      ///< Luma adaptation range selection for UV channels, Max = IME_RANGE_128
        UINT32          LcaLumTh;       ///< Luma adaptation range threshold for UV channels
        RANGE_SEL       LcaLumWtPrc;    ///< Luma adaptation weighting precision for UV channel, Max = IME_RANGE_64
        UINT32          LcaLumWtS;      ///< Luma adaptation weighting start value for UV channels
        UINT32          LcaLumWtE;      ///< Luma adaptation weighting end value for UV channels
    }LcaLuminfo;
} IME_CHRA_LADAP;
/**
    @name IME enum - Dark and bright chroma suppression mode
*/
//@{
typedef enum _DBCS_MODE_SEL
{
    DBCS_DK_MODE  = 0,      ///< dark mode
    DBCS_BT_MODE  = 1,      ///< bright mode
    DBCS_BOTH_MODE= 2,      ///< dark and bright mode
    ENUM_DUMMY4WORD(DBCS_MODE_SEL)
} DBCS_MODE_SEL;
/**
    dark and bright region chroma supression parameters
*/
typedef struct _IME_ChromaSupp
{
    DBCS_MODE_SEL     ModeSel;     ///< Process mode
    struct
    {
        UINT32            uiU;      ///< Center value for U channel
        UINT32            uiV;      ///< Center value for V channel
    }Cetner;
    struct
    {
        UINT32            uiY;      ///< Step for luma
        UINT32            uiC;      ///<  Step for chroma
    }Step;
    UINT32            *puiWeightY;  ///< Weighting LUT for luma channel, 16 elements
    UINT32            *puiWeightC;  ///< Weighting LUT for luma channel, 16 elements
}IME_ChromaSupp;
/**
    Single Image Super-resolution setting structure
*/
//@{
typedef struct _IME_SuperRes
{
  UINT32        AutoModeEn; ///< SSR auto mode switch
  UINT32        DiagTh;     ///< Threahold for diagonal direction
  UINT32        HVDirTh;    ///< Threahold for horizontal and vertical direction
} IME_SuperRes;
/**
    color space transfor parameter set
*/
typedef struct _IME_CSPACE
{
    BOOL        YUV2RGBEn;  ///< enable bit for YUV domain transfor to RGB domain
    BOOL        RGB2YUVEn;  ///< -
}IME_CSPACE;
/**
    film grain parameter set
*/
typedef struct _IME_FG
{
    UINT32  LumiThres;      ///< luminance-Noise difference threshold

    UINT32  GNInit_p1;      ///< Grain noise initial value for path1
    UINT32  Weight_p1;      ///< path1 weighting between pixel and noise value
    UINT32  GNInit_p2;      ///< Grain noise initial value for path2
    UINT32  Weight_p2;      ///< path2 weighting between pixel and noise value
    UINT32  GNInit_p3;      ///< Grain noise initial value for path3
    UINT32  Weight_p3;      ///< path3 weighting between pixel and noise value
}IME_FG;

/**
    3D noise reduction prarmeter set
*/
typedef struct _IME_3D_NR
{
    UINT32  *pThresY;       ///< thresholds for Y channel, tab num:3, Data type:UINT32
    UINT32  *pWeightY;      ///< weightings for Y channel, tab num:4, Data type:UINT32
    UINT32  *pThresCb;      ///< thresholds for Cb channel, tab num:3, Data type:UINT32
    UINT32  *pWeightCb;     ///< weightings for Cb channel, tab num:4, Data type:UINT32
    UINT32  *pThresCr;      ///< thresholds for Cr channel, tab num:3, Data type:UINT32
    UINT32  *pWeightCr;     ///< weightings for Cr channel, tab num:4, Data type:UINT32
}IME_3D_NR;
typedef enum _IME_LCA_FORMAT
{
    LCAF_YCCYCC  = 0,       ///< YCbCrYCbCr data format
    LCAF_YCCP    = 1,       ///< Y / CbCr data format
    ENUM_DUMMY4WORD(IME_LCA_FORMAT)
} IME_LCA_FORMAT;
/**
    IME structure - Data Stamp parameter setting structure
*/
//@{
/**
    @name IME enum - Data stamp mode selection
*/
//@{
typedef enum _DS_MODE_SEL
{
    _2B  = 0,   ///< 2bit mode, image width must be the multiple of 4
    _4B  = 1,   ///< 4bit mode, image width must be the multiple of 4
    ENUM_DUMMY4WORD(DS_MODE_SEL)
} DS_MODE_SEL;
//@}
typedef struct _STAMP_IMAGE_INFO
{
    struct
    {
        UINT32 uiSizeH;             ///< horizontal size
        UINT32 uiSizeV;             ///< vertical size
    } DS_SIZE;                      ///< Image size
    DS_MODE_SEL     DsMode;         ///< Data operating mode, only for Set1/2/3
    Coordinate      DsBlendPos;     ///< Blending position
    UINT32          uiDSLineoffset; ///< Data lineoffset
    UINT32          uiDSAddress;    ///< Data address
} STAMP_IMAGE_INFO;


typedef struct _STAMP_IQ_INFO
{
    UINT32  uiDsColorKY;            ///< Color key of Y channel
    UINT32  uiDsColorKU;            ///< Color key of U channel
    UINT32  uiDsColorKV;            ///< Color key of V channel
    UINT32  uiBlendWt;              ///< Blending weighting
} STAMP_IQ_INFO;


typedef struct _STAMP_SET_INFO
{
    BOOL                DSEn;           ///< Function enable
    STAMP_IMAGE_INFO    DSImgInfo;      ///< Image information
    STAMP_IQ_INFO       DSIqInfo;       ///< Data stamp quality information
}STAMP_SET_INFO;


typedef struct _IMESTAMP_INFO
{
    STAMP_SET_INFO      DS_Set0;        ///< data stamp set0
    STAMP_SET_INFO      DS_Set1;        ///< data stamp set1
    STAMP_SET_INFO      DS_Set2;        ///< data stamp set2
    STAMP_SET_INFO      DS_Set3;        ///< data stamp set3
    UINT32              ColorLutYAddr;  ///< Y channel LUT, 16 elements
    UINT32              ColorLutUAddr;  ///< Y channel LUT, 16 elements
    UINT32              ColorLutVAddr;  ///< Y channel LUT, 16 elements
} IMESTAMP_INFO;
/**
    @name IME enum - Statistical edge kernel selection
*/
//@{
typedef enum _STATS_EKER_SEL
{
    STATS_EKER_HA  = 0,     ///< H map A
    STATS_EKER_HB  = 1,     ///< H map B
    STATS_EKER_VA  = 2,     ///< V map A
    STATS_EKER_VB  = 3,     ///< V map B
    ENUM_DUMMY4WORD(STATS_EKER_SEL)
} STATS_EKER_SEL;
//@}

typedef struct _IME_STATS_EDGE_INFO
{
    BOOL            StatsEdgeKerEn0;    ///< edge kernel enable for set0
    STATS_EKER_SEL  StatsEdgeKer0;      ///< edge map kernel selection for map0
    UINT32          uiStatsShift0;      ///< edge map shift for map0

    BOOL            StatsEdgeKerEn1;    ///< edge kernel enable for set1
    STATS_EKER_SEL  StatsEdgeKer1;      ///< edge map kernel selection for map1
    UINT32          uiStatsSft1;        ///< edge map shift for map1
} IME_STATS_EDGE_INFO;

typedef struct _IME_STATS_HIST_INFO
{
    Coordinate          StatsHistPos;       ///< start posotion for edge statistic
    struct
    {
                        UINT32 sizeH;       ///< horizontal size
                        UINT32 sizeV;       ///< vertical size
    }StatsHistSize;     ///< image size for edge statistic
    struct
    {
        UINT32 AccTag0;  ///< accmulation target of histogram of set0
        UINT32 AccTag1;  ///< accmulation target of histogram of set0
    } StatsHistAccTag;  ///< histogram accumulation target

} IME_STATS_HIST_INFO;

typedef struct _IME_STATS_ROITH_INFO
{
    UINT32  RoiTh0;     ///< statistical threshold of ROI for section0
    UINT32  RoiTh1;     ///< statistical threshold of ROI for section1
    UINT32  RoiTh2;     ///< statistical threshold of ROI for section2
} IME_STATS_ROITH_INFO;
typedef struct _IME_STATS_ROI_INFO
{
    struct
    {
        UINT32 uiStatsRow0;  ///< row position0
        UINT32 uiStatsRow1;  ///< row posotion1
    } StatsRowPos;      ///< row position of statistic
    IME_STATS_ROITH_INFO  StatsRoi0;            ///< ROI0 for edge map
    IME_STATS_ROITH_INFO  StatsRoi1;            ///< ROI1 for edge map
    IME_STATS_ROITH_INFO  StatsRoi2;            ///< ROI2 for edge map
    IME_STATS_ROITH_INFO  StatsRoi3;            ///< ROI3 for edge map
    IME_STATS_ROITH_INFO  StatsRoi4;            ///< ROI4 for edge map

} IME_STATS_ROI_INFO;
typedef struct _IME_STATS_INFO
{
    BOOL                StatsEnable;            ///< enable of statistic
    BOOL                StatsFilterEnable;      ///< filter enable of statistic
    BOOL                StatsImgOutBF;          ///< ime out before filter
    IME_STATS_EDGE_INFO *StatsEdgeMap;          ///< edge map of statistic
    IME_STATS_HIST_INFO *StatsHistogram;        ///< edge histogram
    IME_STATS_ROI_INFO  *StatsRoiThres;         ///< ROI threshold of edge map
} IME_STATS_INFO;
//@}
typedef struct _IME_SUBFUNC
{
    ImeInteEn   imeInteEn;  ///< interrupt enable
    ImeFuncEn   imeFuncEn;  ///< function enable

    struct
    {
        IME_CHRA_ADAP   *pChromaAdaption; ///< -   Function quality information for chroma channel
        IME_CHRA_LADAP  *pChromaLumAdaption;///< - Function quality information for luma channel
        IME_CHRA_CADAP  *pChromaColorAdaption;///< -Chroma adjustment information
        BOOL    PPBEn;
        UINT32  PPB0Addr;       ///< reference image address
        UINT32  PPB1Addr;
        IME_LCA_FORMAT Lcaformat;///< reference image format
        IMESIZE Size;    ///< reference image size
    }ChromaAdap;
    //IME_CSPACE      *pCspaceTrans;
    IME_FG          *pFilmGrain;    ///< film grain
    IME_ChromaSupp  *pChromaSuppres;///< chroma suppression info
    IME_SuperRes    *pSuperRes;     ///< super resolution info
    IMESCALE_Enh    *pP1_Enh;       ///< scaler quality enhancement for path1
    IMESCALE_Enh    *pP2_Enh;       ///< scaler quality enhancement for path2
    IMESCALE_Enh    *pP3_Enh;       ///< scaler quality enhancement for path3
    IMESCALE_Enh    *pP4_Enh;       ///< scaler quality enhancement for path4


    IMESCALE_Filt   *pP1_Filt;      ///< scaling filter for path1
    IMESCALE_Filt   *pP2_Filt;      ///< scaling filter for path2
    IMESCALE_Filt   *pP3_Filt;      ///< scaling filter for path3
    IMESCALE_Filt   *pP4_Filt;      ///< scaling filter for path4

    IMESTAMP_INFO   *pStampInfo;    ///< Data stamp parameters
    IME_STATS_INFO  *pStatsInfo;    ///< Edge statistic parameters
    //BOOL            P2IEn;
}IME_SUBFUNC;

#define SCALING_BASE    (10)
#define SCALING_LIMIT_MAX   (16 * SCALING_BASE) //overlap use 24 condition
#define SCALING_LIMIT_MIN   (2 * SCALING_BASE) //overlap use 24 condition
#define IPH_IME_SCAL_METHOD(InH,InV,OutH,OutV,Method)    \
{\
    {\
        if( (InH >= OutH) && (InV >= OutV))\
        {\
            if( ( (OutH*SCALING_LIMIT_MAX) > (InH*SCALING_BASE) ) && ( (OutV*SCALING_LIMIT_MAX) > (InV*SCALING_BASE) ) &&   \
                ( (OutH*SCALING_LIMIT_MIN) < (InH*SCALING_BASE) ) && ( (OutV*SCALING_LIMIT_MIN) < (InV*SCALING_BASE) ))     \
                *Method = IME_INTEGRATION;\
            else\
                *Method = IME_BILINEAR;\
        }\
        else\
            *Method = IME_BILINEAR;\
    }\
}
//@}
#endif
