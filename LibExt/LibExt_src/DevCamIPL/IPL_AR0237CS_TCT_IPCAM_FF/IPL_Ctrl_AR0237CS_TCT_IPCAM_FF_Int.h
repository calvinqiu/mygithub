#ifndef IPL_CTRL_AR0237CS_TCT_IPCAM_H_
#define IPL_CTRL_AR0237CS_TCT_IPCAM_H_

/**
    IPL_CtrlSample_Int.h


    @file       IPL_CtrlSample_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

/**
    information for Image-Pipeline
    Add ones if necessary
*/
typedef enum
{
    IPLCTRL_MAX_CNT = IPLCTRL_MAX_CNT_DEF,
    ENUM_DUMMY4WORD(IPLCTRL_IDX)
} IPLCTRL_IDX;

//check IPLCTRL_IDX range
STATIC_ASSERT((IPLCTRL_MAX_CNT <= IPL_CTRL_ELEMENT_MAX));

/**
    set information for Image-Pipeline
    Add ones if necessary
*/
typedef enum
{
    IPLCTRL_CAP_FLOW_NONE = 0,
    IPLCTRL_CAP_FLOW_NORMAL,
    IPLCTRL_CAP_FLOW_HDR,
    IPLCTRL_CAP_FLOW_3D,
    IPLCTRL_CAP_FLOW_HS,
    IPLCTRL_CAP_FLOW_EBURST,
    IPLCTRL_CAP_FLOW_INVID,
    IPLCTRL_CAP_FLOW_AEB,
    IPLCTRL_CAP_FLOW_AWB,
    IPLCTRL_CAP_FLOW_SIM,
    IPLCTRL_CAP_FLOW_MFHDR,
    IPLCTRL_CAP_FLOW_MFANTISHAKE,
//#NT#2016/03/30#Harry Tsai -begin
//#NT# Add side by side capture mode
    IPLCTRL_CAP_FLOW_DUALSBS,
//#NT#2016/03/30#Harry Tsai -end
    IPLCTRL_CAP_FLOW_MAX,
    ENUM_DUMMY4WORD(IPLCTRL_CAP_FLOW)
} IPLCTRL_CAP_FLOW;

typedef enum
{
    IPLCTRL_CA_TYPE_32BIT = 0,
    IPLCTRL_CA_TYPE_64BIT,
    IPLCTRL_CA_TYPE_MAX,
    ENUM_DUMMY4WORD(IPLCTRL_CA_TYPE)
}IPLCTRL_CA_TYPE;

typedef enum
{
    IPL_OB_FIX = 0,
    IPL_OB_AUTO,
    ENUM_DUMMY4WORD(IPL_OB_TYPE)
} IPL_OB_TYPE;

typedef struct
{
    SENSOR_INFO *Info;
    IPL_CHGMODE_DATA *ChgMode;
} IPLCTRL_INFO;

typedef struct
{
    UINT32 Param[10];
} IPLCTRL_CAP_EXT_INFO;

void IPL_CtrlInitPrvPProcInfo(UINT32 Id, IPL_POST_PROC_FUNC_EN FuncEn);

void IPL_CtrlInitCapIpcInfo(IPL_GET_CAP_RAW_DATA *CapInfo, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo);
void IPL_CtrlInitPrvIpcInfo(UINT32 Id, IPC_INFO *IpcInfo, IPL_ISR_OBJ_INFO *IsrInfo, IPL_CHGMODE_DATA *ChgMode);

void IPL_CtrlInitPrvBufInfo(IPL_CHGMODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *BufInfo);
void IPL_CtrlInitCapBufInfo(UINT32 Id, IPL_CHGMODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *BufInfo);
void IPL_CtrlPrvInit(UINT32 Id, IPLCTRL_INFO *CtrlInfo);
void IPL_CtrlCapInit(UINT32 Id, UINT32 EventId, IPLCTRL_INFO *CtrlInfo);

ER IPL_GetSensorStatus(UINT32 Id, SENSOR_MODE Mode, SENSOR_INFO *Info);

UINT32 IPL_CtrlGetSIEVABufInfo(void);

void IPL_CtrlPrvSetDZoomInfo(UINT32 Id);
void IPL_CtrlPrvGetDZoomInfo(UINT32 Id, IPC_Dzoom_Info *Info);
BOOL IPL_ChkImePathInfo(UINT32 Id, IPL_IME_PATH PathId, IPL_IME_PATH_INFO *Info);
void IPL_SetImePathInfo(UINT32 Id, IPL_IME_PATH PathId, IPL_IME_PATH_INFO *Info);
void IPL_GetImePathInfo(UINT32 Id, IPL_IME_PATH PathId, IPL_IME_PATH_INFO *Info);

//sie flow callback
void IPL_SIESetOB_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_OBAVG *OBAvg, SIE_OBSUB *OBSub);
void IPL_SIESetCA_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_CACC *cacc, SIE_STATS_PATH_INFO *pPath);
void IPL_SIESetLA_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_LACC *lacc, SIE_STATS_PATH_INFO *pPath);
void IPL_SIESetEthOut_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *Sub);
void IPL_SIESetBP_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *Sub);
void IPL_SIESetDMAOutSrc_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update);
void IPL_SIESetDP_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *subs);
void IPL_SIESetRawEnc_FCB(UINT32 Id,  SIE_UPDATE *Update, SieFuncEn *FuncEn, SIE_RAWENC *RawEnc);
void IPL_SIECCIR_FCB(UINT32 Id, SIE_DVI* pDvi);
//#NT#2017/04/12#Bowen Li -begin
//#NT# fix CCIR cap crash issue
void IPL_SIECCIR_FCB_Cap(UINT32 Id, SIE_DVI* pDvi);
//#NT#2017/04/12#Bowen Li -end
void IPL_CtrlInitIpc_SIEAct_Cap(UINT32 Id, SIE_UPDATE *Update, SieACTMode *ActMode);
void IPL_CtrlInitIpc_SIEBurstLength_Cap(UINT32 Id, SIE_UPDATE *Update, SENSOR_INFO *SenInfo, SIE_BURSTLENGTH_SET *Length);
void IPL_CtrlInitIpc_SIESignal_Cap(UINT32 Id, SIE_UPDATE *Update, UINT32 HDInv, UINT32 VDInv, UINT32 VDPhase, UINT32 HDPhase, UINT32 DPhase, IPP_SIE_SIGNAL *Signal);
void IPL_CtrlInitIpc_SIECh0PPB_Cap(UINT32 Id, SIE_UPDATE *Update, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV);
void IPL_CtrlInitIpc_SIECh1PPB_Cap(UINT32 Id, SIE_UPDATE *Update, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV);
void IPL_CtrlInitIpc_SIECh2PPB_Cap(UINT32 Id, SIE_UPDATE *Update, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV);
void IPL_CtrlInitIpc_SIECh3PPB_Cap(UINT32 Id, SIE_UPDATE *Update, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV);
void IPL_CtrlInitIpc_SIEIn_Cap(UINT32 Id, SIE_UPDATE *Update, SIE_WINDOW *Win);
void IPL_CtrlInitIpc_SIEOB_Cap(UINT32 Id, SIE_UPDATE *Update1, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_OBAVG *OBAvg, SIE_OBSUB *OBSub);
void IPL_CtrlInitIpc_SIEGamma_Cap(UINT32 Id,  SIE_UPDATE *Update1, SieFuncEn *FuncEn, SIE_GAMMA **pGamma);
void IPL_CtrlInitIpc_SIERawEnc_Cap(UINT32 Id,  SIE_UPDATE *Update1, SieFuncEn *FuncEn, SIE_RAWENC *RawEnc, UINT32 QInit, UINT32 Ratio);

void IPL_CtrlInitIpc_SIEAct(UINT32 Id, SENSOR_INFO *SenInfo, SieACTMode *ActMode);
void IPL_CtrlInitIpc_SIEBurstLength(UINT32 Id, SENSOR_INFO *SenInfo, SIE_BURSTLENGTH_SET *Length);
void IPL_CtrlInitIpc_SIEIn(UINT32 Id, SENSOR_INFO *SenInfo, SIE_WINDOW *Win);
void IPL_CtrlInitIpc_SIESignal(UINT32 Id, UINT32 HDInv, UINT32 VDInv, UINT32 VDPhase, UINT32 HDPhase, UINT32 DPhase, IPP_SIE_SIGNAL *Signal);
void IPL_CtrlInitIpc_SIECh0PPB(UINT32 Id, IPL_RAW_IMG_INFO *PPB0, UINT32 *EXT_PPB, UINT32 *StrpOfs);
void IPL_CtrlInitIpc_SIECh1PPB(UINT32 Id, SIE_Dma_Out *DmaOut, UINT32 *EXT_PPB, UINT32 *AddrOfs);
void IPL_CtrlInitIpc_SIECh2PPB(UINT32 Id, SIE_Dma_Out *DmaOut, UINT32 *EXT_PPB, UINT32 *AddrOfs);
void IPL_CtrlInitIpc_SIECh3PPB(UINT32 Id, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV);
void IPL_CtrlInitIpc_SIEEthPPB(UINT32 Id, SIE_Eth_Out *EthOut);
void IPL_CtrlInitIpc_SIEVaPPB(UINT32 Id, SIE_Dma_Out *Out);
void IPL_CtrlInitIpc_IPLInPPB(UINT32 Id, IPL_RAW_IMG_INFO *PPB0, Coordinate *STPos);
void IPL_CtrlInitIpc_DCEIn(UINT32 Id, SENSOR_INFO *SenInfo, USIZE* pRef_1x, Coordinate *pCenter_1x, iCoordinate *pCenter_1xOFS, UINT32 *HRatio, UINT32 *VRatio);
void IPL_CtrlInitIpc_DCEDeWarpInfo(UINT32 Id, DCE_Stripe *StripeInfo, _2DLUT_PARAM *Dce2DInfo);
//#NT#2016/07/20#Jarkko Chang -begin
//#NT# add suppor vr360 flow
void IPL_CtrlInitIpc_VR360Info(UINT32 Id, VR360_DATA *VR360Data);
//#NT#2016/07/20#Jarkko Chang -end
void IPL_CtrlInitIpc_IPESubOutPPB(UINT32 Id, UINT32 *PP0Addr, UINT32 *PP1Addr, IPL_YCC_CH_INFO *IpeOut);
void IPL_CtrlInitIpc_IFE2LcaPPB(UINT32 Id, UINT32 *PP0Addr, UINT32 *PP1Addr);
void IPL_CtrlInitIpc_IMEPxOutPPB(UINT32 Id, UINT32 PathEnIdx, IPL_YCC_IMG_INFO *Px_PPB0);
void IPL_CtrlInitIpc_IMEPxOutPPB_Stitch(UINT32 Id, UINT32 PathEnIdx, IPL_YCC_IMG_INFO *Px_PPB0, STITCH_IMG *StitchImg);
void IPL_CtrlInitIpc_DispPxPPB(UINT32 Id, UINT32 PathEnIdx);
void IPL_CtrlInitIpc_DispPxPPB_Stitch(UINT32 Id, UINT32 PathEnIdx);
void IPL_CtrlInitIpc_DispCtrlEn(UINT32 Id, UINT32 PathEnIdx, UINT32 EnFlag);

void IPL_SetTriggerInfo(IPL_TRIG_CTRL_CMD_INFO TrigInfo);
IPL_TRIG_CTRL_CMD_INFO IPL_GetTriggerInfo(void);

IPL_HAL_PARASET IPL_CtrlGetHalSetId(UINT32 Id);
//#NT#2016/10/03#Harry Tsai -begin
//#NT# Added for Eth On/Off
void IPL_CtrlSIExSetEthEn(UINT32 Id, UINT32 EnFlag);
//#NT#2016/10/03#Harry Tsai -end
#endif //IPL_CTRL_SAMPLE_H_
