/**
    IPH module (SIE type define)

    @file       IPL_Hal_SIE.h
    @ingroup    mILibIPH

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _IPL_Hal_SIE_COM_H
#define _IPL_Hal_SIE_COM_H
#include "IPL_Hal_SIE_Com_Info.h"
#include "IPL_ISREvent.h"
/** \addtogroup mILibIPH */
//@{

/**
    query enum

    read from hw register
*/
typedef enum _IPH_SIE_QUE
{
    SIE_REG_CARST           = 0x00000001,   ///< read CA result(R,G,B average)         Data type: SIE_CARST  eng: SIE1, SIE2
    SIE_REG_LARST           = 0X00000002,   ///< read LA result(Luminacne)              Data type: SIE_VARST  eng: SIE1, SIE2
    SIE_REG_LARST_MANUAL    = 0X00000003,   ///< read LA result(Luminacne)              Data type: SIE_VARST  eng: SIE1, SIE2
    SIE_REG_VARST           = 0X00000004,   ///< read VA result()                       Data type: SIE_VARST  eng: SIE1,
    SIE_REG_VA_INDEP_RST    = 0X00000005,   ///< read VA IndepWin result()              Data type: SIE_VARST  eng: SIE1,
    SIE_REG_RAWENC          = 0x00000006,   ///< read rawEncode Bitstream size          Data type: UINT32     eng: SIE1, SIE2
    SIE_REG_Ch0CURRDYADDR   = 0x00000007,   ///< read sie current dma ready address     Data type: UINT32     eng: SIE1, SIE2
    SIE_REG_OBDETRST        = 0x00000008,   ///< read sie ob detection rst              Data type: UINT32     eng: SIE1, SIE2
    SIE_REG_Ch1CURRDYADDR   = 0x00000009,   ///< read sie current dma ready address CA  Data type: UINT32     eng: SIE1, SIE2, SIE3, SIE4
    SIE_REG_Ch2CURRDYADDR   = 0x0000000A,   ///< read sie current dma ready address LA  Data type: UINT32     eng: SIE1, SIE2, SIE3, SIE4
    SIE_REG_Ch3CURRDYADDR   = 0x0000000B,   ///< read sie current dma ready address VA  Data type: UINT32     eng: SIE1
    SIE_REG_Ch4CURRDYADDR   = 0x0000000C,   ///< read sie current dma ready address ETH Data type: UINT32     eng: SIE1
    SIE_REG_LASUM           = 0x0000000D,   ///< read sie LA sum.                       Data type: UINT32     eng: SIE1, SIE2
    ENUM_DUMMY4WORD(IPH_SIE_QUE)
}IPH_SIE_QUE;


/**
     enum of SIE update selection
*/
typedef enum _SIE_UPDATE
{
    SIE_OUTADDR_CH0     = 0x0000000000000001LLU,   ///< update ch0 crop/crop & scale image outaddr                        eng: SIE1, SIE2
    SIE_OUTADDR_CH1     = 0x0000000000000002LLU,   ///< update ch1 CA/DVI Y outaddr                                       eng: SIE1, SIE2, SIE3, SIE4
    SIE_OUTADDR_CH2     = 0x0000000000000004LLU,   ///< update ch2 LA/DVI UV outaddr                                      eng: SIE1, SIE2, SIE3, SIE4
    SIE_OUTADDR_CH3     = 0x0000000000000008LLU,   ///< update ch3 VA/crop & scale image outaddr                          eng: SIE1
    SIE_OUTADDR_ETH     = 0x0000000000000010LLU,   ///< update ETH outaddr                                                eng: SIE1
    SIE_IOSIZE          = 0x0000000000000020LLU,   ///< update crop size/start pix/CFA,ch0 output size/lifeoffset         eng: SIE1, SIE2
    SIE_IOSIZE_CH1_CH2  = 0x0000000000000040LLU,   ///< update ch1/ch2 output lineoffset (for DVI output using, Ch1/Ch2 Output Size will update at the same time)
                                                   ///<                                                                   eng: SIE1, SIE2, SIE3, SIE4
    SIE_IOSIZE_CH3      = 0x0000000000000080LLU,   ///< update ch3output lineoffset                                       eng: SIE1
    SIE_IOSIZE_CH4      = 0x0000000000000100LLU,   ///< update ch4output packingbus/lineoffset                            eng: SIE1
    SIE_BURSTLENG       = 0x0000000000000200LLU,   ///< update output ch0 dram burst length                               eng: SIE1, SIE2, SIE3, SIE4
    SIE_INIT            = 0x0000000000000400LLU,   ///< init sie                                                          eng: SIE1, SIE2, SIE3, SIE4
    SIE_DVI_            = 0x0000000000000800LLU,   ///< update DVI(CCIR) SIE1 only support 8bit CCIR601/CCIR656           eng: SIE1, SIE2, SIE3, SIE4
    SIE_PFPC_           = 0x0000000000001000LLU,   ///< update pfpc                                                       eng: SIE1
    SIE_BCC_ADJ         = 0x0000000000002000LLU,   ///< update rawenc information                                         eng: SIE1, SIE2
    SIE_STATS_PATH_CA   = 0x0000000000004000LLU,   ///< update statistics path ca selection.                              eng: SIE1, SIE2
    SIE_STATS_PATH_LA   = 0x0000000000008000LLU,   ///< update statistics path la selection.                              eng: SIE1, SIE2
    SIE_STATS_PATH_VA   = 0x0000000000010000LLU,   ///< update statistics path va selection.                              eng: SIE1
    SIE_VIG_            = 0x0000000000020000LLU,   ///< update VIG                                                        eng: SIE1, SIE2
    SIE_CA_SIZE         = 0x0000000000040000LLU,   ///< update CA size,not include addr                                   eng: SIE1, SIE2
    SIE_LA_SIZE         = 0x0000000000080000LLU,   ///< update LA size,not include addr                                   eng: SIE1, SIE2
    SIE_CA_TH           = 0x0000000000100000LLU,   ///< update CA threshold                                               eng: SIE1, SIE2
    SIE_LA_CG           = 0x0000000000200000LLU,   ///< update LA color gain                                              eng: SIE1, SIE2
    SIE_VA_             = 0x0000000000400000LLU,   ///< update VACC size/group info                                       eng: SIE1
    SIE_VA_CG           = 0x0000000000800000LLU,   ///< update VA color gain                                              eng: SIE1
    SIE_VA_INDEP        = 0x0000000001000000LLU,   ///< update independent va window                                      eng: SIE1
    SIE_SUBFEN          = 0x0000000002000000LLU,   ///< update func enable/disable                                        eng: SIE1, SIE2, SIE3, SIE4
    SIE_DPC_            = 0x0000000004000000LLU,   ///< update defect pixel concealment                                   eng: SIE1, SIE2
    SIE_COLD_           = 0x0000000008000000LLU,   ///< update column defect                                              eng: SIE1, SIE2
    SIE_OBAVG_          = 0x0000000010000000LLU,   ///< update ob avg ( ob detection)                                     eng: SIE1, SIE2
    SIE_OBOFS_          = 0x0000000020000000LLU,   ///< update ob offset                                                  eng: SIE1, SIE2
    SIE_OBF_            = 0x0000000040000000LLU,   ///< update ob frame                                                   eng: SIE1
    SIE_OBP_            = 0x0000000080000000LLU,   ///< update ob plane                                                   eng: SIE1
    SIE_ECS_            = 0x0000000100000000LLU,   ///< update ECS/including ECS table addr                               eng: SIE1, SIE2
    SIE_DGAIN_          = 0x0000000200000000LLU,   ///< update Digital gain                                               eng: SIE1, SIE2
    SIE_GAMMA_          = 0x0000000400000000LLU,   ///< update 4-ch gamma                                                 eng: SIE1, SIE2
    SIE_CH0Out_SRC      = 0x0000000800000000LLU,   ///< update dma out ch0 source                                         eng: SIE1, SIE2
    SIE_CH3Out_SRC      = 0x0000001000000000LLU,   ///< update dma out ch3 source                                         eng: SIE1
    SIE_BP_             = 0x0000002000000000LLU,   ///< update BreakPoint1/2/3,ref 1/2/3 timing                           eng: SIE1, SIE2, SIE3, SIE4
    SIE_ETH_THRES_      = 0x0000004000000000LLU,   ///< update edge output(dma out4) threshold(excluding SIE_ETH_OutAddr) eng: SIE1
    SIE_LA_GAMMA        = 0x0000008000000000LLU,   ///< la gamma                                                          eng: SIE1, SIE2
    SIE_VA_GAMMA        = 0x0000010000000000LLU,   ///< va gamma                                                          eng: SIE1
    SIE_VA_CROP         = 0x0000200000000000LLU,   ///< va crop                                                           eng: SIE1
    SIE_FLIP_CH0        = 0x0004000000000000LLU,   ///< ch0 flip                                                          eng: SIE1, SIE2
    SIE_FLIP_CH1        = 0x0080000000000000LLU,   ///< ch1 flip                                                          eng: SIE1, SIE2, SIE3, SIE4
    SIE_FLIP_CH2        = 0x0100000000000000LLU,   ///< ch2 flip                                                          eng: SIE1, SIE2, SIE3, SIE4
    SIE_FLIP_CH3        = 0x0200000000000000LLU,   ///< ch3 flip                                                          eng: SIE1
}SIE_UPDATE;


#define SIE1_UPDATE_ENABLE_BIT (SIE_OUTADDR_CH0    |     \
                                SIE_OUTADDR_CH1    |     \
                                SIE_OUTADDR_CH2    |     \
                                SIE_OUTADDR_CH3    |     \
                                SIE_OUTADDR_ETH    |     \
                                SIE_IOSIZE         |     \
                                SIE_IOSIZE_CH1_CH2 |     \
                                SIE_IOSIZE_CH3     |     \
                                SIE_IOSIZE_CH4     |     \
                                SIE_BURSTLENG      |     \
                                SIE_INIT           |     \
                                SIE_DVI_           |     \
                                SIE_PFPC_          |     \
                                SIE_BCC_ADJ        |     \
                                SIE_STATS_PATH_CA  |     \
                                SIE_STATS_PATH_LA  |     \
                                SIE_STATS_PATH_VA  |     \
                                SIE_VIG_           |     \
                                SIE_CA_SIZE        |     \
                                SIE_LA_SIZE        |     \
                                SIE_CA_TH          |     \
                                SIE_LA_CG          |     \
                                SIE_VA_            |     \
                                SIE_VA_CG          |     \
                                SIE_VA_INDEP       |     \
                                SIE_SUBFEN         |     \
                                SIE_DPC_           |     \
                                SIE_COLD_          |     \
                                SIE_OBAVG_         |     \
                                SIE_OBOFS_         |     \
                                SIE_OBF_           |     \
                                SIE_OBP_           |     \
                                SIE_ECS_           |     \
                                SIE_DGAIN_         |     \
                                SIE_GAMMA_         |     \
                                SIE_CH0Out_SRC     |     \
                                SIE_CH3Out_SRC     |     \
                                SIE_BP_            |     \
                                SIE_ETH_THRES_     |     \
                                SIE_LA_GAMMA       |     \
                                SIE_VA_GAMMA       |     \
                                SIE_VA_CROP        |     \
                                SIE_FLIP_CH0       |     \
                                SIE_FLIP_CH1       |     \
                                SIE_FLIP_CH2       |     \
                                SIE_FLIP_CH3       |     \
                                0);



#define SIE2_UPDATE_ENABLE_BIT (SIE_OUTADDR_CH0    |     \
                                SIE_OUTADDR_CH1    |     \
                                SIE_OUTADDR_CH2    |     \
                                SIE_IOSIZE         |     \
                                SIE_IOSIZE_CH1_CH2 |     \
                                SIE_BURSTLENG      |     \
                                SIE_INIT           |     \
                                SIE_DVI_           |     \
                                SIE_BCC_ADJ        |     \
                                SIE_STATS_PATH_CA  |     \
                                SIE_STATS_PATH_LA  |     \
                                SIE_VIG_           |     \
                                SIE_CA_SIZE        |     \
                                SIE_LA_SIZE        |     \
                                SIE_CA_TH          |     \
                                SIE_LA_CG          |     \
                                SIE_SUBFEN         |     \
                                SIE_DPC_           |     \
                                SIE_COLD_          |     \
                                SIE_OBAVG_         |     \
                                SIE_OBOFS_         |     \
                                SIE_ECS_           |     \
                                SIE_DGAIN_         |     \
                                SIE_GAMMA_         |     \
                                SIE_CH0Out_SRC     |     \
                                SIE_BP_            |     \
                                SIE_LA_GAMMA       |     \
                                SIE_FLIP_CH0       |     \
                                SIE_FLIP_CH1       |     \
                                SIE_FLIP_CH2       |     \
                                SIE_FLIP_CH3       |     \
                                0);

#define SIE3_UPDATE_ENABLE_BIT (SIE_OUTADDR_CH1    |     \
                                SIE_OUTADDR_CH2    |     \
                                SIE_IOSIZE_CH1_CH2 |     \
                                SIE_BURSTLENG      |     \
                                SIE_INIT           |     \
                                SIE_DVI_           |     \
                                SIE_SUBFEN         |     \
                                SIE_BP_            |     \
                                SIE_FLIP_CH1       |     \
                                SIE_FLIP_CH2       |     \
                                0);

#define SIE4_UPDATE_ENABLE_BIT (SIE_OUTADDR_CH1    |     \
                                SIE_OUTADDR_CH2    |     \
                                SIE_IOSIZE_CH1_CH2 |     \
                                SIE_BURSTLENG      |     \
                                SIE_INIT           |     \
                                SIE_DVI_           |     \
                                SIE_SUBFEN         |     \
                                SIE_BP_            |     \
                                SIE_FLIP_CH1       |     \
                                SIE_FLIP_CH2       |     \
                                0);

/**
     enum of SIE read selection

     read from inner table, not from hw register.
*/
typedef enum _SIE_READ
{
    SIE_R_PHASE     = 0x00000001,   ///< read signal                                        Data type: IPP_SIE_SIGNAL  eng: SIE1, SIE2, SIE3, SIE4
    SIE_R_CH0_INFO  = 0x00000002,   ///< read sie ch0 dma addr/size/lineofs/bit             Data type: SIE             eng: SIE1, SIE2
    SIE_R_CH1_INFO  = 0x00000003,   ///< read sie ch0 dma addr/size/lineofs/bit             Data type: SIE             eng: SIE1, SIE2, SIE3, SIE4
    SIE_R_CH2_INFO  = 0x00000004,   ///< read sie ch0 dma addr/size/lineofs/bit             Data type: SIE             eng: SIE1, SIE2, SIE3, SIE4
    SIE_R_CH3_INFO  = 0x00000005,   ///< read sie ch0 dma addr/size/lineofs/bit             Data type: SIE             eng: SIE1
    SIE_R_CH4_INFO  = 0x00000006,   ///< read sie ch0 dma addr/size/lineofs/bit             Data type: pSubFunc        eng: SIE1
    SIE_R_BS_OUT    = 0x00000007,   ///< read sie bayer scale out size                      Data type: SIE             eng: SIE1, SIE2
    SIE_R_INSIZE    = 0x00000008,   ///< read act window size,crop window info              Data type: SIE_WINDOW      eng: SIE1, SIE2, SIE3, SIE4
    SIE_R_SUBFEN    = 0x00000009,   ///< read func enable/disable,                          Data type: SieFuncEn       eng: SIE1, SIE2, SIE3, SIE4
    SIE_R_DPC_      = 0X0000000a,   ///< read defect pixel concealment,                     Data type: SIE_DPC         eng: SIE1, SIE2
    SIE_R_COLD_     = 0X0000000b,   ///< read column defect                                 Data type: SIE_ColumnDef   eng: SIE1, SIE2
    SIE_R_OBAVG_    = 0x0000000c,   ///< read ob avg                                        Data type: SIE_OBAVG       eng: SIE1, SIE2
    SIE_R_OBOFS_    = 0X0000000d,   ///< read ob offset                                     Data type: SIE_OBSUB       eng: SIE1, SIE2
    SIE_R_OBF_      = 0X0000000e,   ///< read ob frame                                      Data type: SIE_OBFAVG      eng: SIE1
    SIE_R_OBP_      = 0X0000000f,   ///< read ob plane                                      Data type: SIE_OBPSUB      eng: SIE1
    SIE_R_ECS_      = 0X00000010,   ///< read ECS                                           Data type: SIE_EmbeddedCS  eng: SIE1, SIE2
    SIE_R_DGAIN_    = 0x00000011,   ///< read Digital gain                                  Data type: UINT32          eng: SIE1, SIE2
    SIE_R_VIG_      = 0x00000012,   ///< read VIG  para. set                                Data type: SIE_CA_VIG      eng: SIE1, SIE2
    SIE_R_CA_       = 0X00000013,   ///< read CACC para. set,not include addr               Data type: SIE_CACC        eng: SIE1, SIE2
    SIE_R_LA_       = 0X00000014,   ///< read LACC para. set,not include addr               Data type: SIE_LACC        eng: SIE1, SIE2
    SIE_R_VA_       = 0X00000015,   ///< read VACC para. set                                Data type: SIE_VACC        eng: SIE1
    SIE_R_VACROP_   = 0X00000016,   ///< read VA Crop para. set                             Data type: VACROP          eng: SIE1
    SIE_R_GAMMA_    = 0X00000017,   ///< read 4-ch gamma                                    Data type: SIE_GAMMA       eng: SIE1, SIE2
    SIE_R_PFPC_     = 0x00000018,   ///< read pfpc                                          Data type: SIE_PFP         eng: SIE1, SIE2
    SIE_R_BP_       = 0x00000019,   ///< read BreakPoint1/2/3,ref 1/2/3 timing              Data type: SIE_BP          eng: SIE1, SIE2, SIE3, SIE4
    SIE_R_RAWEN_    = 0x0000001a,   ///< read Raw encode                                    Data type: SIE_RAWENC      eng: SIE1, SIE2
    SIE_R_INTEEn_   = 0x0000001b,   ///< read sie interrupt enable bit                      Data type: SIE_INTE_EN     eng: SIE1, SIE2, SIE3, SIE4
    SIE_R_CA_PARAM_ = 0x0000001c,   ///< read ch2 infomration                               Data type: SIE_CAPARAM     eng: SIE1, SIE2
    SIE_R_LA_PARAM_ = 0x0000001d,   ///< read ch1 infomration                               Data type: SIE_LAPARAM     eng: SIE1, SIE2
    SIE_R_ETH_ADDR_ = 0x0000001e,   ///< read edge threshold output,addr,lineofs,V only     Data type: SIE_ETH_OUT     eng: SIE1
    SIE_R_BCC_PARAM_= 0x0000001f,   ///< read bcc information                               Data type: SIE_RAWENC      eng: SIE1, SIE2
    ENUM_DUMMY4WORD(SIE_READ)
}SIE_READ;


/**
     SIE configuration

     all SIE configuration including all SIE sub function
*/
typedef struct _SIE_Manager
{
    IPP_SIE_SIGNAL  Sie_Signal; ///< sensor information                         @SIE1/SIE2/SIE3/SIE4
    BOOL            bDir2IFE;   ///< Direct 2 ife mode                          @SIE1 ?????

    SieACTMode      ActMode;    ///< sie source selection                       @SIE1/SIE2

    struct
    {
        SIE_Dma_In In;          ///< bayer in for ActMode = SIE_IN_DRAM         @SIE1
    }RAW_IN;

    struct
    {
        SIE_Dma_Out Out;        ///< -                                          @SIE1/SIE2
        SIE_PPB_SW  SiePPBSW;   ///< output buffer control                      @SIE1/SIE2
        CH0_SRC     Src;        ///< output selection                           @SIE1/SIE2
    }CH0;                       ///< sie channel_0 output
    struct
    {
        SIE_Dma_Out Out;        ///< -                                          @SIE1/SIE2/SIE3/SIE4
        SIE_PPB_SW  SiePPBSW;   ///< -                                          @SIE1/SIE2/SIE3/SIE4
    }CH1;   ///< sie channel_1 output ( Color accumulation output)
    struct
    {
        SIE_Dma_Out Out;        ///< -                                          @SIE1/SIE2/SIE3/SIE4
        SIE_PPB_SW  SiePPBSW;   ///< -                                          @SIE1/SIE2/SIE3/SIE4
    }CH2;   ///< sie channel_1 output ( Color accumulation output)
    struct
    {
        SIE_Dma_Out Out;        ///< -                                          @SIE1
        SIE_PPB_SW  SiePPBSW;   ///< -                                          @SIE1
        CH3_SRC     Src;        ///< -                                          @SIE1
    }CH3;
    SIE_WINDOW  IMG_Window;     ///< act/crop wiondow                           @SIE1/SIE2/SIE3/SIE4
    UINT32  DmaOutSizeH;        ///< this size map to scaling output size,      @SIE1/SIE2
    UINT32  DmaOutSizeV;        ///< this size map to scaling output size,      @SIE1/SIE2

    SIE_BURSTLENGTH_SET BurstLength;    ///< sie burst length                   @SIE1/SIE2/SIE3/SIE4

    SIE_SRCIMG_WIN_INFO     SrcimgWin;///< Sie input source image size.         @SIE1/SIE2/SIE3/SIE4
    SIE_SUBFUNC *pSubFunc;      ///< -                                          @SIE1/SIE2/SIE3/SIE4

    //reverse
    //UINT32 rev;                     //reserve for IPH paraset idx
}SIE_Manager,*pSIE_Manager;

/**
     SIE configuration
*/
typedef SIE_Manager SIE;

//@}
#endif
