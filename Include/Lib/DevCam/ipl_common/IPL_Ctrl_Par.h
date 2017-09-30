/**
    IPL_Ctrl module(type define)

    @file       IPL_Ctrl_Par.h
    @ingroup    mILibIPH
    @note

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _IPL_CTRL_PAR_H_
#define _IPL_CTRL_PAR_H_
#include "IPL_Cmd.h"
#include "IPL_Hal.h"
/** \addtogroup mILibIPH */
//@{

/**
     callback when pipeline output image dma ready

     Detailed description.
     @note (Optional)

     @param[in]     ImgIdx      source index of current output image during this streaming.
     @param[in]     fp_CB       pointer of PostProc. synchronization function
     @param[in,out] path1       in:current ready img addr and size. out: next frame output addr.
     @param[in,out] path2       in:current ready img addr and size. out: next frame output addr.
     @param[in,out] path3       in:current ready img addr and size. out: next frame output addr.
     @param[in,out] path4       in:current ready img addr and size. out: next frame output addr.
     @return Description: none
*/
typedef void (*fp_ImgRdy)(UINT32 ImgIdx, IPL_YCC_IMG_INFO* path1,IPL_YCC_IMG_INFO* path2,IPL_YCC_IMG_INFO* path3,IPL_YCC_IMG_INFO* path4,fpCallBack fp_CB, IPL_PROC_ID IPL_ID);
typedef IPH_CLK (*fpEngCLK)(UINT32);
typedef UINT32 (*fpSIEPclkPad)(UINT32);
typedef UINT32 (*fpSIEClkSrc)(UINT32);
typedef UINT32 (*fpEngInit)(UINT32, ENG_SUBFUNC*,IPL_HAL_GROUP*);//eng_subfunc and IPL_HAL_GROUP is output
typedef BOOL (*fpRegDump)(UINT32);
typedef UINT32(*fpccirFmt)(UINT32, SIE_DVI*);

/*
    return type is cascaded to void* from indicated SubFunction
*/
typedef void*   (*fpEngSubFunc)(UINT32, IPL_RAW_IMG_INFO, IPL_YCC_CH_INFO*);//IPL_YCC_CH_INFO ife2 image size

/**
     optional structure of ICF_FLOW

     this sturcture is optional configuration,ipl can start without these configuration.
     @note:
\n      fp_DceInit can't be NULL!! Because of hw stripe constraint.
\n      fp_SIEClk can't be NULL when sensor interface is LVDS!!  because LVDS + SIE hw constraint!
\n      if ime path1 and path3 need to be enabled during IPL running, IPL must be initialized with ime p1 and p3 output address
*/
typedef struct _IPLC_EXT
{
    fpEngCLK fp_SIEClk;                     ///< engine clk CB, default clk will be applied to when CB is NULL!
    fpSIEClkSrc fp_SIEClkSrc;               ///< sie engine clk source selection CB
    fpSIEPclkPad fp_SIEPclkSel;             ///< sie engine pclk selection CB
    fpEngCLK fp_RHEClk;                     ///< -
    fpEngCLK fp_RDEClk;                     ///< -
    fpEngCLK fp_IFEClk;                     ///< -
    fpEngCLK fp_DCEClk;                     ///< -
    fpEngCLK fp_IPEClk;                     ///< -
    fpEngCLK fp_IFE2Clk;                    ///< -
    fpEngCLK fp_IMEClk;                     ///< -
    fpEngCLK fp_ISEClk;                     ///< -
    fpccirFmt fp_CCIRFmt;                   ///< dvi Format setting
    fpEngInit fp_SieInit;                   ///< engine sub-func initial CB, default cfg will be applied to when CB is NULL!
    fpEngInit fp_RheInit;                   ///< -
    fpEngInit fp_RdeInit;                   ///< -
    fpEngInit fp_IfeInit;                   ///< -
    fpEngInit fp_DceInit;                   ///< -
    fpEngInit fp_IpeInit;                   ///< -
    fpEngInit fp_ImeInit;                   ///< -
    fpEngInit fp_IseInit;                   ///< -
    fpEngInit fp_Ife2Init;                  ///< -
}IPLC_EXT;

/**
     basic configuration of ICF_FLOW

     these configuration must be fulfilled, and address can't be 0!
     @note (Optional)
*/
typedef struct _IPLC_BASEINFO
{
    SieACTMode       SIE_Act;           ///< sie and sensor communication interface
    SIE_WINDOW       SIE_IN;            ///< sie input window
    IPP_SIE_SIGNAL   SenSorIn;          ///< sensor input format
    SIE_BURSTLENGTH_SET BurstLength;    ///< sie ch0 dram burst length
    IPL_RAW_IMG_INFO SIE_PPB0;          ///< out idx 0
    UINT32           SIE_EXT_PPB[10];   ///< out idx 1 ~ 9 total 10
    UINT32           SIE_PPB_OFS;       ///< for 2 stripe mode

    SIE_Dma_Out     SIE_CH1;            ///<ca out / CCIR Y ch1 idx 0~1
    UINT32          SIE_CH1_EXT_PPB[8]; ///<CCIR Y ch1 idx 2 ~ 9 total 10
    SIE_Dma_Out     SIE_CH2;            ///<La out / CCIR UV ch2 idx 0~1
    UINT32          SIE_CH2_EXT_PPB[8]; ///<La ext buffer only used for IPL_CF_RHEHDR/IPL_CF_RHEWDR mode / CCIR Y ch2 idx 2 ~ 9 total 10
    SIE_Eth_Out     Eth_Out;            ///<ETH

    struct _SIE_CH3
    {
        CH3_SRC Src;                    ///< ch3 output format
        struct
        {
            SIE_Dma_Out SubImgout;      ///< subimage info
        }Out;                           ///< -
    }SIE_CH3;                           ///< sie output ch3
    CH0_SRC          SIE_Src;           ///< sie output data source

    enum
    {
        IPL_CF_NORMAL           = 0x00000000,     ///< SIE + RDE + IFE + DCE + IPE + IME + IFE2
        IPL_CF_CCIR             = 0x00000001,     ///< CCIR mode SIE
        IPL_CF_RHEHDR           = 0x00000002,     ///< SIE1 + SIE2 + RHE + IFE + DCE + IPE + IME + IFE2
        IPL_CF_2STRP            = 0x00000003,     ///< SIE + RDE + IFE + DCE + IPE + IME + IFE2
        IPL_CF_RHEWDR           = 0x00000004,     ///< SIE + RHE + IFE + DCE + IPE + IME + IFE2
        IPL_CF_2STRP_F          = 0x00000005,     ///< SIE + RDE + IFE + DCE + IPE + IME + IFE2(always 2 stripe flow)
    }IPLCFMode;                                   ///< IPL control mode

    enum
    {
        IPL_CFEXT_NORMAL = 0x00000000,  ///< normal mode, do nothing
        IPL_CFEXT_SIE_ONLY = 0x00000001,   ///< notify only SIE
        IPL_CFEXT_DEINTERLACE  = 0x00000002, ///< notify used grph de-interlace only used at CCIR mode
        IPL_CFEXT_DEINTERLACE2 = 0x00000003, ///< notify used ISE de-interlace only used at CCIR mode
        IPL_CFEXT_SYNCTRIG =  0x00000004, ///< notify used VD sync header
        IPL_CFEXT_IMESCALE = 0x00000005, ///< notify used ime scale only used at CCIR mode without IPP flow
										 ///< enable reduce buffer mode (IPL_IME_PATH << 16 | IPL_CFEXT_IMESCALE)
										 ///< IPL_IME_PATH = sie out buffer address
										 ///< ex IPL_IME_PATH = sie out buffer address = path1, path1 scale to path2, path1 scale to path3
        IPL_CFEXT_ISESCALE = 0x00000006, ///< notify used ISE scale only used at CCIR mode
        IPL_CFEXT_DEINTERLACE3 = 0x00000007, ///< notify used IME de-interlace only used at CCIR mode
        IPL_CFEXT_SMART_MIRROR = 0x00000008, ///< notify used DCE+IME at CCIR mode
        									 ///< enable reduce buffer mode (IPL_IME_PATH << 16 | IPL_CFEXT_SMART_MIRROR)
        									 ///< IPL_IME_PATH = sie out buffer address
        									 ///< ex IPL_IME_PATH = sie out buffer address = path1, path1 scale to path2, path1 scale to path3
        IPL_CFEXT_ISESCALE_MULPATH = 0x00000009, ///< notify used ISE scale only used at CCIR mode
        										 ///< enable reduce buffer mode (IPL_IME_PATH << 16 | IPL_CFEXT_ISESCALE_MULPATH)
        										 ///< IPL_IME_PATH = sie out buffer address
        										 ///< ex IPL_IME_PATH = sie out buffer address = path1, path1 scale to path2, path1 scale to path3
        IPL_CFEXT_DEINTERLACE_ISESCALE_MULPATH = 0x0000000A, ///< notify used ISE scale only used at CCIR mode
    }IPLCFModeExt;

    RHE_MAIN_SEL     RheModeSel;

    IPL_RAW_IMG_INFO IPL_PPB0;              ///< pipeline dma in Buffer 0
    Coordinate       IPL_StPos;             ///< Start Position (left-top 0, 0)

    USIZE            DCE_Ref_1X;            ///< sensor output size(for dce reference)
    Coordinate       DCE_Ref_1XCenter;      ///< distortion center of sensor output
    iCoordinate      DCE_Ref_1XCenterOfs;   ///< distortion center offset
    USIZE            SIE_Ref_1X;            ///< sie crop output size(for dce reference)
    Coordinate       SIE_Ref_1XDCECropCen;  ///< distortion center on sie crop image
    Coordinate       SIE_Ref_1XVIGCropCen;  ///< vig center on sie crop image
    UINT32           SenHBinRatio;          ///< h ratio, default 1000(SEN_BINNING_UNIT)
    UINT32           SenVBinRatio;          ///< v ratio, default 1000(SEN_BINNING_UNIT)

    struct _IPE_PPB
    {
        UINT32           IPE_PPB0_ADDR;     ///<CbCr out.(CbCr addr are continuous)
        UINT32           IPE_PPB1_ADDR;     ///<CbCr out.(CbCr addr are continuous)
        IPL_YCC_CH_INFO  IPE_OUT;
    }IPE_PPB;

    UINT32           IFE2_PPB0_ADDR;        ///<ife2 output addr
    UINT32           IFE2_PPB1_ADDR;        ///<ife2 output addr

    BOOL             IME_P1_ENABLE;         ///< enable path1 dma output,path1 and path3 is optional,default is disable
    BOOL             IME_P2_ENABLE;         ///< enable path2 dma output
    BOOL             IME_P3_ENABLE;         ///< enable path3 dma output
    BOOL             IME_P4_ENABLE;         ///< enable path4 dma output
    BOOL             IME_P1_D2H264;         ///< path 1 output destination select
    IPL_YCC_IMG_INFO IME_P1_PPB0;           ///< ppb0 output address
    IPL_YCC_IMG_INFO IME_P2_PPB0;           ///< ime path2 dma output info.path2 always enable,for disp
    IPL_YCC_IMG_INFO IME_P3_PPB0;           ///< ppb0 output address
    IPL_YCC_IMG_INFO IME_P4_PPB0;           ///< ppb0 output address
    struct
    {
        Coordinate   Start;                 ///< crop start
        UINT32       SizeH;                 ///< crop size H
        UINT32       SizeV;                 ///< crop size V
    }IME_P1_Crop;
    struct
    {
        Coordinate   Start;                 ///< crop start
        UINT32       SizeH;                 ///< crop size H
        UINT32       SizeV;                 ///< crop size V
    }IME_P2_Crop;
    struct
    {
        Coordinate   Start;                 ///< crop start
        UINT32       SizeH;                 ///< crop size H
        UINT32       SizeV;                 ///< crop size V
    }IME_P3_Crop;
    struct
    {
        Coordinate   Start;                 ///< crop start
        UINT32       SizeH;                 ///< crop size H
        UINT32       SizeV;                 ///< crop size V
    }IME_P4_Crop;

    BOOL            IME_P4_EXT_OUT_ENABLE;  ///< enable path4 dma ext output
    UINT32          IME_P4_EdgeMapBufOfs;   ///< relative P4 Y output address
    UINT32          IME_P4_HistoBufOfs;     ///< relative P4 Y output address

    FLIP_TYPE       FlipInfo;               ///< image pipeline information
}IPLC_BASEINFO,*pIPLC_BASEINFO;

//@}
#endif
