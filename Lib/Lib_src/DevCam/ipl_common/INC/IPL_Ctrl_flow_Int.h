/**
    IPL Ctrl module(streaming) data type

    Define all data type needed by IPL Ctrl module(streaming).

    @file       IPL_Ctrl_flow_Int.h
    @ingroup    mIIPL_Ctrl
    @note       None

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/


#ifndef _IPL_CTRL_FLOW_INT_H
#define _IPL_CTRL_FLOW_INT_H

#ifndef _IPL_COM_INT_INC
    #error "illegal include IPL common module internal .H file"
#endif


#include "SysKer.h"
#include "IPL_Utility.h"
#include "IPL_Ctrl.h"
#include "GxImage.h"

/** \addtogroup mIIPL_Ctrl */
//@{
/**
     IPL_CTRL Stop mode

     @note:inner status mechine(valid):
\n        run -> stop_all.

\n        run -> stop ycc ( -> ipc_pause_sie -> ipe_resume sie) -> ipc_stop_raw.
                                   |---<-----<----<----|

\n        run -> ipc_pause_sie -> ipc_resume_sie
                    |---<-----<----<----|

\n        run -> ipc_pause_ipl -> ipc_resume_ipl
                    |---<-----<----<----|
*/
typedef enum
{
    IPC_IGNORE = 0xffffffff,///< ignore command
    IPC_START_RUN = 0,      ///< RUN satus when each sie engine start run
    IPC_STOP_ALL,           ///< stop and close all engine
    IPC_STOP_YCC,           ///< stop and close all engine but sie, sie engine still in running
    IPC_PAUSE_SIE,          ///< stop sie (sie still open, not close)
    IPC_RESUME_SIE,         ///< start sie from status:IPC_PAUSE_SIE
    IPC_STOP_RAW,           ///< stop and close sie engine
    IPC_PAUSE_IPL,          ///< pause Image Pipeline, but all engine still in running.
    IPC_RESUME_IPL,         ///< resume Image Pipeline.
    ENUM_DUMMY4WORD(IPC_CTRL_CMD)
}IPC_CTRL_CMD;

typedef enum _IPLCTRL_STATUS_OFS
{
    IPLCTRL_UNKNOW_OFS      = 0x00,
    IPLCTRL_INIT_OFS        = 0x04,
    IPLCTRL_RUN_OFS         = 0x08,
    IPLCTRL_IPLPAUSE_OFS    = 0x0c,
    IPLCTRL_SIEPAUSE1_OFS   = 0x10,
    IPLCTRL_YCCSTOP_OFS     = 0x14,
    IPLCTRL_SIEPAUSE2_OFS   = 0x18,
    IPLCTRL_STOP_OFS        = 0x1c,
    ENUM_DUMMY4WORD(IPLCTRL_STATUS_OFS)
}IPLCTRL_STATUS_OFS;

typedef enum _IPLCTRL_STATUS
{
    IPLCTRL_FAIL            = 0x0fffffff,

    IPLCTRL_UNKNOW_1        = 0x00000001,
    IPLCTRL_UNKNOW_2        = 0x00000002,
    IPLCTRL_UNKNOW_3        = 0x00000004,
    IPLCTRL_UNKNOW_4        = 0x00000008,
    IPLCTRL_UNKNOW_ALL      = (IPLCTRL_UNKNOW_1|IPLCTRL_UNKNOW_2|IPLCTRL_UNKNOW_3|IPLCTRL_UNKNOW_4),

    IPLCTRL_INIT_1          = 0x00000010,
    IPLCTRL_INIT_2          = 0x00000020,
    IPLCTRL_INIT_3          = 0x00000040,
    IPLCTRL_INIT_4          = 0x00000080,
    IPLCTRL_INIT_ALL        = (IPLCTRL_INIT_1|IPLCTRL_INIT_2|IPLCTRL_INIT_3|IPLCTRL_INIT_4),

    IPLCTRL_RUN_1           = 0x00000100,
    IPLCTRL_RUN_2           = 0x00000200,
    IPLCTRL_RUN_3           = 0x00000400,
    IPLCTRL_RUN_4           = 0x00000800,
    IPLCTRL_RUN_ALL         = (IPLCTRL_RUN_1|IPLCTRL_RUN_2|IPLCTRL_RUN_3|IPLCTRL_RUN_4),

    IPLCTRL_IPLPAUSE_1      = 0x00001000,
    IPLCTRL_IPLPAUSE_2      = 0x00002000,
    IPLCTRL_IPLPAUSE_3      = 0x00004000,
    IPLCTRL_IPLPAUSE_4      = 0x00008000,
    IPLCTRL_IPLPAUSE_ALL    = (IPLCTRL_IPLPAUSE_1|IPLCTRL_IPLPAUSE_2|IPLCTRL_IPLPAUSE_3|IPLCTRL_IPLPAUSE_4),

    IPLCTRL_SIEPAUSE1_1     = 0x00010000,
    IPLCTRL_SIEPAUSE1_2     = 0x00020000,
    IPLCTRL_SIEPAUSE1_3     = 0x00040000,
    IPLCTRL_SIEPAUSE1_4     = 0x00080000,
    IPLCTRL_SIEPAUSE1_ALL   = (IPLCTRL_SIEPAUSE1_1|IPLCTRL_SIEPAUSE1_2|IPLCTRL_SIEPAUSE1_3|IPLCTRL_SIEPAUSE1_4),

    IPLCTRL_YCCSTOP_1       = 0x00100000,
    IPLCTRL_YCCSTOP_2       = 0x00200000,
    IPLCTRL_YCCSTOP_3       = 0x00400000,
    IPLCTRL_YCCSTOP_4       = 0x00800000,
    IPLCTRL_YCCSTOP_ALL     = (IPLCTRL_YCCSTOP_1|IPLCTRL_YCCSTOP_2|IPLCTRL_YCCSTOP_3|IPLCTRL_YCCSTOP_4),

    IPLCTRL_SIEPAUSE2_1     = 0x01000000,
    IPLCTRL_SIEPAUSE2_2     = 0x02000000,
    IPLCTRL_SIEPAUSE2_3     = 0x04000000,
    IPLCTRL_SIEPAUSE2_4     = 0x08000000,
    IPLCTRL_SIEPAUSE2_ALL   = (IPLCTRL_SIEPAUSE2_1|IPLCTRL_SIEPAUSE2_2|IPLCTRL_SIEPAUSE2_3|IPLCTRL_SIEPAUSE2_4),

    IPLCTRL_STOP_1          = 0x10000000,
    IPLCTRL_STOP_2          = 0x20000000,
    IPLCTRL_STOP_3          = 0x40000000,
    IPLCTRL_STOP_4          = 0x80000000,
    IPLCTRL_STOP_ALL        = (IPLCTRL_STOP_1|IPLCTRL_STOP_2|IPLCTRL_STOP_3|IPLCTRL_STOP_4),
    ENUM_DUMMY4WORD(IPLCTRL_STATUS)
}IPLCTRL_STATUS;

/**
     IPL_Ctrl RunTime chang IPL configuration

     only valid when IPL_Ctrl in run_status ,
     excluding :"IPC_SIE",   "IPC_Chg_ImeP1Out", "IPC_Chg_ImeP2Out",
                "IPC_Chg_ImeP3Out", "IPC_Chg_Crop", "IPC_Chg_Load" !

     @note :
\n           "IPC_SIE" only valid when IPL stop at "IPC_PAUSE_SIE"
\n           "IPC_Chg_ImeP1Out","IPC_Chg_ImeP2Out","IPC_Chg_ImeP3Out","IPC_Chg_Crop"
             these four enum will be latched until chg == "IPC_Chg_Load".
\n              ex:
\n                  IPL_CTRLRunTimeChg(IPC_Chg_ImeP2Out,ptr);
\n                  IPL_CTRLRunTimeChg(IPC_Chg_DCE,ptr);               this cmd will fail.
\n                  IPL_CTRLRunTimeChg(IPC_Chg_Load,NULL);

*/
typedef enum _IPC_RUNTIMECHG
{
    IPC_UNKNOW          = 0x00000000,
    IPC_Dzoom           = 0x00000001,   ///<                                                ,data type: IPC_Dzoom_Info
    IPC_WaitDzoomIdle   = 0x00000002,   ///<                                                ,data type: IPC_Dzoom_Info
    IPC_SIEx_           = 0x00000003,   ///< control sie switch.ex:prv2cap or cap2prv       ,data type :IPC_SIE_Info
    //IPC_SenChgMode      = 0x00000003,   ///< chg sensor mode when pipe line running         ,data type :IPC_SenChgMode_Info

    //IPC_Chg_ChromaA_Retinex = 0x00000004,   ///< chg chroma adaption/Retinex parameter,     ,data type :IPC_ChromaA_Retinex_Info

    IPC_Chg_IPL_INAddr  = 0x00000005,   ///< chg ipl input address                          ,data type :IPC_IPLInAddr
    //IPC_Chg_DCE         = 0x00000006,   ///< chg Distortion configuration                   ,data type :IPC_IQparam
    IPC_Chg_SubOut      = 0x00000007,   ///< chg SubImg size                                ,data type :IPE_SUB_OUT
    IPC_Flip            = 0x00000008,   ///< control H&V flip                               ,data type :IPC_Flip_Info

    IPC_Chg_ImeP1Out    = 0x00001000,   ///< chg path1 output size/lineofs/fmt/PathEn       ,data type :IPC_IMEOut_Info
    IPC_Chg_ImeP2Out    = 0x00002000,   ///< chg path2 output size/lineofs/fmt/PathEn       ,data type :IPC_IMEOut_Info
    IPC_Chg_ImeP3Out    = 0x00004000,   ///< chg path3 output size/lineofs/fmt/PathEn       ,data type :IPC_IMEOut_Info
    IPC_Chg_ImeP4Out    = 0x00008000,   ///< chg path4 output size/lineofs/fmt/PathEn       ,data type :IPC_IMEOut_Info
    IPC_Chg_Crop        = 0x00010000,   ///< chg sie out and pre in size/lineofs            ,data type :IPC_FRONTEND
    IPC_Chg_Load        = 0x00020000,   ///< load command 0x00001000~ 0x00008000 into reg.  ,data type :none
    ENUM_DUMMY4WORD(IPC_RUNTIMECHG)
}IPC_RUNTIMECHG;

/**
     IPL_Ctrl RunTime chang IPL IQ configuration

*/
typedef enum _IPC_RUNTIMECHG_IQ
{

    IPC_SIEx_IQparam    = 0x10000000,   ///< chg SIE iq parameter set                       ,data type :IPC_IQparam
    IPC_RHE_IQparam     = 0x20000000,   ///< chg RHE iq parameter set                       ,data type :IPC_IQparam

    IPC_IFE_IQparam     = 0x50000000,   ///< chg IFE iq parameter set                       ,data type :IPC_IQparam
    IPC_DCE_IQparam     = 0x60000000,   ///< chg DCE iq parameter set                       ,data type :IPC_IQparam
    IPC_IPE_IQparam     = 0x70000000,   ///< chg IPE iq parameter set                       ,data type :IPC_IQparam
    IPC_IME_IQparam     = 0x80000000,   ///< chg IME iq parameter set                       ,data type :IPC_IQparam
    IPC_IFE2_IQparam    = 0x90000000,   ///< chg IFE2 iq parameter set                      ,data type :IPC_IQparam

    IPC_IFE_IQLoad_ISR  = 0xa0000000,   ///< chg IFE iq parameter ISR load                  ,data type :IPC_IQparam
    IPC_IPE_IQLoad_ISR  = 0xb0000000,   ///< chg IPE iq parameter ISR load                  ,data type :IPC_IQparam
    IPC_RHE_IQLoad_ISR  = 0xc0000000,   ///< chg RHE iq parameter ISR load                  ,data type :IPC_IQparam
    ENUM_DUMMY4WORD(IPC_RUNTIMECHG_IQ)
}IPC_RUNTIMECHG_IQ;

/**
     IQ runTimeChg.

     definition for Engine Quality chg when IPL in "IPL_RUN" status.
*/
typedef struct
{
    IPL_HAL_PARASET Idx;
    IPL_HAL_GROUP   *pGrp;
    ENG_SUBFUNC     EngSubF;
}IPC_IQparam;

/**
     PRE dma input address runTimeChg.

     definition for change pre dma input address when IPL in "IPL_RUN" status.
*/
typedef struct
{
    IPL_HAL_PARASET Idx;
    UINT32 PPB0Addr;
    UINT32 PPB1Addr;
}IPC_IPLInAddr;

/**
     ime dma output configuration runTimeChg.

     definition for change ime dma output configuration when IPL in "IPL_RUN" status.
     @note:
\n   can't change output address and scaling mehtod here
\n   scale IPL output image.
\n   control IPL dma output pathEn
*/
typedef struct
{
    IPL_HAL_PARASET Idx;
    OUTCH           Path;//output address and ScalMethod are useless here
}IPC_IMEOut_Info;

/**
     IPL raw_image in/out size runTimeChg.

     definition for change sie output size/pre inout size when IPL in "IPL_RUN" status.
     @note:
\n   support digital zoom application.
\n   support cropping application.
*/
typedef struct
{
    IPL_HAL_PARASET Idx;
    IPL_PROC_ID     Id;
    UINT32 sieCropHini;
    UINT32 sieCropVini;
    UINT32 sieCropH;
    UINT32 sieCropV;
    UINT32 sieOutH;
    UINT32 sieOutV;
    UINT32 sieOutLineOfs;
    UINT32 iplCropX;
    UINT32 iplCropY;
    UINT32 iplInH;
    UINT32 iplInV;
    UINT32 iplInLineOfs;

    BOOL   bChgCAParam;
    BOOL   bChgLAParam;
    BOOL   bChgEthParam;
    UINT32 Rev;
}IPC_Dzoom_Info;
typedef IPC_Dzoom_Info IPC_FRONTEND;

/**
     SIE control.

     reset SIE engine.
*/
typedef struct
{
    IPL_HAL_PARASET Idx;
    SIE_UPDATE  sieUpdate;
    SIE *psie;
}IPC_SIE_Info;

/**
     flip control information.
*/



typedef struct
{
    IPL_PROC_ID     Id;
    FLIP_TYPE type;
}IPC_Flip_Info;

/**
    VR360 Data Type
*/
typedef struct
{
    UINT32 uiRotateBufferSize;    /// < Rotation buffer size
    UINT32 uiRotateBufferAddr;   /// < Rotation buffer address
    UINT32 l;                              /// < Height offset (in pixel)
    UINT32 pixAddr[3];                /// < Reserved for graphic operation
    UINT32 uiWidth;                    /// < Reserved for actual IME output width
    UINT32 uiHeight;                   /// < Reserved for actual IME output height
} VR_BUFFER_INFO;

typedef struct
{
    BOOL enable;            /// < VR360 enable bit
    UINT32 uiHeightAdj;    /// < Image height adjust factor in Q10; default = 1024
    INT8 ori[2];               ///< Rotation option for [0] upper(left) image and [1] lower(right) image

    VR_BUFFER_INFO p1;  /// < VR buffer for Path 1
    VR_BUFFER_INFO p2;  /// < VR buffer for Path 2
    VR_BUFFER_INFO p3;  /// < VR buffer for Path 3
} VR360_DATA;

#if 0
- structure of supported flow
#endif

/**
    IPL M task ctrl type
*/
typedef enum _IPL_M_CTRL_TYPE
{
    IPL_M_CTRL_TYPE_22 = 0,              ///< manage task ctrl type(default)
    IPL_M_CTRL_TYPE_13,                  ///< manage task ctrl type
    IPL_M_CTRL_TYPE_MAX,
    ENUM_DUMMY4WORD(IPL_M_CTRL_TYPE)
} IPL_M_CTRL_TYPE;


/**
     IPL_Ctrl flow.

     IPL_Ctrl module support following control flows.
*/
typedef enum _IPLCTRL_FLOW
{
    ICF_REV1        = 0,
    ICF_FLOW_A      = 1,    ///< all engines control by HW auto handshake
    ICF_FLOW_C      = 2,    ///< all engines control by FW handshake
    ICF_FLOW_DEWARP = 3,    ///< all engines control by FW handshake(dewarping)
    ICF_MAX         = 4,
    ENUM_DUMMY4WORD(IPLCTRL_FLOW)
}IPLCTRL_FLOW;


/**
    Enum for query IPL information
*/
typedef enum _IPLCTRL_QUERY
{
    ICF_FLOWA           = ICF_FLOW_A,   ///< minimum buffer number needed by flow_A.
    ICF_FLOWC           = ICF_FLOW_C,   ///< minimum buffer number needed by flow_C.
    ICF_FLOWDEWARP      = ICF_FLOW_DEWARP,  ///< minimum buffer number needed by flow_DEWARP.
    /******* below must be bigger than ICF_MAX *********/
    ICF_ETHOUT_RDY      = 11,           ///< current ready edge threshold output
    ICF_IPLIO_RDY       = 12,           ///< current ready Pre input ch0 info.
    ICF_LAPARAM_RDY     = 13,           ///< special case for sync timing
    ICF_FRAME_CNT       = 14,
    ICF_GDC_NOR_FACTOR  = 15,           ///< special case for RSC
    ICF_FLIP_TYPE       = 16,           ///< get image pipe line flip type
    ICF_IME_STATS_INFO  = 17,           ///< get ime statistic information
    ENUM_DUMMY4WORD(IPLCTRL_QUERY)
}IPLCTRL_QUERY;

/**
     data structure of ICF_FLOW_A

*/
typedef struct _ICF_FLOW_A_STRUCT
{
    IPL_HAL_PARASET  ParaIdx;           ///<indicate IPL_HAL parameter set index

    IPLC_BASEINFO    BaseInfo;          ///< IPL_Ctrl main-image configuration

    ///<optional information
    IPLC_EXT         Ext;
}ICF_FLOW_A_STRUCT,*PICF_FLOW_A_STRUCT;

/**
     data structure of ICF_FLOW_C

*/
typedef struct _ICF_FLOW_C_STRUCT
{
    IPL_HAL_PARASET  ParaIdx;           ///<indicate IPL_HAL parameter set index

    IPLC_BASEINFO    BaseInfo;          ///< IPL_Ctrl main-image configuration

    ///<ime chroma adapation CB
    fpEngSubFunc     fp_AdaptSetting;   ///<CB at each pre2ime fw_operation start,
                                        ///<return NULL if no need to update, otherwise,return ptr of update data
                                        ///<if funcEn is FALSE, must return NULL!

    /*
    -----------------------------           -----------------------------
    |                   |       |           |       |                   |
    |                   |       |           |       |                   |
    |                   |       |           |       |                   |
    |         L1        |  L2   |           |   R2  |        R1         |
    |                   |       |           |       |                   |
    |                   |       |           |       |                   |
    |                   |       |           |       |                   |
    -----------------------------           -----------------------------
    */

    BOOL             IME_P1_SprtEn;     ///< enable path1 SPRT output, must be IME_P1_ENABLE = 1
    BOOL             IME_P2_SprtEn;     ///< enable path2 SPRT output, must be IME_P2_ENABLE = 1
    BOOL             IME_P3_SprtEn;     ///< enable path3 SPRT output, must be IME_P3_ENABLE = 1

    STITCH_IMG       SprtP1Img;          ///< stitch p1 img L2 or R2
    INT32           SprtP1YBufOfs;       ///< stitch p1 Y buffer offset L2 or R2
    INT32           SprtP1UVBufOfs;      ///< stitch p1 UV buffer offset L2 or R2

    STITCH_IMG       SprtP2Img;          ///< stitch p2 img L2 or R2
    INT32           SprtP2YBufOfs;       ///< stitch p2 Y buffer offset L2 or R2
    INT32           SprtP2UVBufOfs;      ///< stitch p2 UV buffer offset L2 or R2

    STITCH_IMG       SprtP3Img;          ///< stitch p3 img L2 or R2
    INT32           SprtP3YBufOfs;       ///< stitch p3 Y buffer offset L2 or R2
    INT32           SprtP3UVBufOfs;      ///< stitch p3 UV buffer offset L2 or R2
    _2DLUT_PARAM     *Stitch2DInfo;      ///< stitch 2D table pointer L2 + L1 or R2 + R1

    ///<optional information
    IPLC_EXT         Ext;
}ICF_FLOW_C_STRUCT,*PICF_FLOW_C_STRUCT;


/**
     data structure of ICF_FLOW_D

*/
typedef struct _ICF_FLOW_D_STRUCT
{
    IPL_HAL_PARASET  ParaIdx;           ///<indicate IPL_HAL parameter set index

    IPLC_BASEINFO    BaseInfo;          ///< IPL_Ctrl main-image configuration

    ///<ime chroma adapation CB
    fpEngSubFunc     fp_AdaptSetting;   ///<CB at each pre2ime fw_operation start,
                                        ///<return NULL if no need to update, otherwise,return ptr of update data
                                        ///<if funcEn is FALSE, must return NULL!
    ///<optional information
    IPLC_EXT         Ext;

    DCE_Stripe StripeInfo[2];           ///< MST information 0: top, 1: buttom
    _2DLUT_PARAM _2DInfo[2];            ///< 2D information 0: top, 1: buttom

    VR360_DATA vrData;
}ICF_FLOW_D_STRUCT,*PICF_FLOW_D_STRUCT;


typedef struct _GDC_NOR_FACTOR_IN
{
    UINT32 width;
    UINT32 height;
    UINT32 centX;
    UINT32 centY;
    UINT32 Xdisf;
    UINT32 Ydisf;
} GDC_NOR_FACTOR_IN;

typedef struct _GDC_NOR_FACTOR_OUT
{
    UINT32 NormFact;
    UINT32 NormBit;
} GDC_NOR_FACTOR_OUT;


typedef struct _GDC_NOR_FACTOR_INFO
{
    void (*fp_GetInfo)(GDC_NOR_FACTOR_IN *InInfo, GDC_NOR_FACTOR_OUT *OutInfo);
} GDC_NOR_FACTOR_INFO;

/**
    feedback information when querry "ICF_FLOWA".
*/
typedef struct _QUERY_FLOW_A
{
    UINT32 nBUF_SIEPPB;     ///< sie buffer num
    UINT32 nBUF_CAPPB;      ///< sie ca buffer num
    UINT32 nBUF_LAPPB;      ///< sie la buffer num
    UINT32 nBUF_SIE_VAPPB;  ///< sie va buffer num
    UINT32 nBUF_IPE_LCAPPB; ///< ipe LCA CbCr buffer num
    UINT32 nBUF_IFE2PPB;    ///< ife2 working buffer num
    UINT32 nBUF_IMEPPB;     ///< only return ime 1 path requirement (all 3 pathes need the same pingpong buf num)
}QUE_FLOW_A;

/**
    feedback information when querry "ICF_FLOW_C".
*/
typedef struct _MSTRP_INFO
{
    void (*fp_Get2StrpInfo)(UINT32 OutW, UINT32 PackBit, SIE_RAWENC *RawEnc);
} MSTRP_INFO;

typedef struct _QUE_FLOW_C
{
    UINT32 nBUF_SIEPPB;     ///< sie buffer num
    UINT32 nBUF_CAPPB;      ///< sie ca buffer num
    UINT32 nBUF_LAPPB;      ///< sie la buffer num
    UINT32 nBUF_SIE_VAPPB;  ///< sie va buffer num
    UINT32 nBUF_IPE_LCAPPB; ///< ipe LCA CbCr buffer num
    UINT32 nBUF_IFE2PPB;    ///< ife2 working buffer num
    UINT32 nBUF_IMEPPB;     ///< only return ime 1 path requirement (all 3 pathes need the same pingpong buf num)

    //only needed when CCIR function enable
    UINT32 nBUF_BAYER2PPB;
    UINT32 nBUF_YCC2PPB;

    //for 2-stripe flow
    MSTRP_INFO MStrpInfo;
    UINT32 StripMinSize;

    //force 2-stripe flow
    MSTRP_INFO MStrpInfo_F;
}QUE_FLOW_C;

typedef struct _QUE_FLOW_D
{
    UINT32 nBUF_SIEPPB;     ///< sie buffer num
    UINT32 nBUF_CAPPB;      ///< sie ca buffer num
    UINT32 nBUF_LAPPB;      ///< sie la buffer num
    UINT32 nBUF_SIE_VAPPB;  ///< sie va buffer num
    UINT32 nBUF_IPE_LCAPPB; ///< ipe LCA CbCr buffer num
    UINT32 nBUF_IFE2PPB;    ///< ife2 working buffer num
    UINT32 nBUF_IMEPPB;     ///< only return ime 1 path requirement (all 3 pathes need the same pingpong buf num)

    //only needed when CCIR function enable
    UINT32 nBUF_BAYER2PPB;
    UINT32 nBUF_YCC2PPB;

    UINT32 StripMinSize;
}QUE_FLOW_D;

//@}
#endif
