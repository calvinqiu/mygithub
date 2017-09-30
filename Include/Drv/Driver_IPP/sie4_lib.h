/*
    Public header file for RDE module.

    This file is the header file that define the API and data type for SIE
    module.

    @file       sie4_lib.h
    @ingroup    mIDrvIPPSIE
    @note

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#ifndef _SIE4_LIB_H
#define _SIE4_LIB_H

#include "sie_lib.h"


/**
    SIE Entire Configuration.

    Structure of entire SIE configuration.
*/
//@{
typedef struct
{
    // No run-time change
    SIE_MAIN_INPUT_INFO         MainInInfo;
    SIE_DVI_INFO                DviInfo;
    SIE_PATGEN_INFO             PatGenInfo;

    // run-time change-able
    UINT32  uiFuncEn;
    UINT32  uiIntrpEn;
    SIE_BREAKPOINT_INFO BreakPointInfo;
    SIE_SRC_WIN_INFO    SrcWinInfo;
    SIE_ACT_WIN_INFO    ActWinInfo;
//NA//    SIE_CRP_WIN_INFO    CrpWinInfo;

//NA//    SIE_DRAM_IN0_INFO   DramIn0Info;
//NA//    SIE_DRAM_IN1_INFO   DramIn1Info;
//NA//    SIE_DRAM_IN2_INFO   DramIn2Info;
//NA//    SIE_DRAM_OUT0_INFO  DramOut0Info;
    SIE_DRAM_OUT1_INFO  DramOut1Info;
    SIE_DRAM_OUT2_INFO  DramOut2Info;
//NA//    SIE_DRAM_OUT3_INFO  DramOut3Info;
//NA//    SIE_DRAM_OUT4_INFO  DramOut4Info;
    SIE_DRAM_BURST_INFO DramBurstInfo;// would be removed soon

//NA//    SIE_OB_DT_INFO          ObDtInfo;
//NA//    SIE_OB_DT_RSLT_INFO     ObDtRsltInfo;// would be removed soon
//NA//    SIE_OB_OFS_INFO         ObOfsInfo;
//NA//    SIE_OB_FRAME_INFO       ObFrmInfo;
//NA//    SIE_OB_PLANE_INFO       ObPlnInfo;

//NA//    SIE_DEF_COL_INFO    DefectInfo;
//NA//    SIE_MAIN_GAMMA_INFO MainGmaInfo;
//NA//    SIE_ECS_INFO        EcsInfo;
//NA//    SIE_DGAIN_INFO      DgainInfo;

//NA//    SIE_BS_H_INFO       BshInfo;
//NA//    SIE_BS_V_INFO       BsvInfo;

//NA//    SIE_BCC_ADJ_INFO    BccAdjInfo;
//NA//    //there would be default inside driver//SIE_BCC_PARAM_INFO  BccParamInfo;
//NA//    //there would be default inside driver//SIE_BCC_TBL_INFO    BccTblInfo;

//NA//    SIE_STCS_PATH_INFO  StcsPathInfo;
//NA//    SIE_STCS_CALASIZE_GRP_INFO  StcsCaLaSzGrpInfo;
//NA//    SIE_VIG_INFO        StcsVigInfo;
//NA//    //SIE_STCS_CALASIZE_GRP_INFO//SIE_CA_CROP_INFO    StcsCaCrpInfo;
//NA//    //SIE_STCS_CALASIZE_GRP_INFO//SIE_CA_SCAL_INFO    StcsCaSmplInfo;
//NA//    SIE_CA_TH_INFO      StcsCaThInfo;
//NA//    //SIE_STCS_CALASIZE_GRP_INFO//SIE_CA_WIN_INFO     StcsCaWinInfo;
//NA//    //SIE_STCS_CALASIZE_GRP_INFO//SIE_LA_CROP_INFO    StcsLaCrpInfo;
//NA//    SIE_LA_CG_INFO      StcsLaCgInfo;
//NA//    SIE_LA_GMA_INFO     StcsLaGma1Info;
//NA//    //SIE_STCS_CALASIZE_GRP_INFO//SIE_LA_WIN_INFO     StcsLaWinInfo;
//NA//    SIE_VA_CG_INFO      StcsVaCgInfo;
//NA//    SIE_VA_CROP_INFO    StcsVaCrpInfo;
//NA//    SIE_VA_GMA_INFO     StcsVaGma2Info;
//NA//    SIE_VA_FLTR_GROUP_INFO    StcsVaFltrG1;
//NA//    SIE_VA_FLTR_GROUP_INFO    StcsVaFltrG2;
//NA//    SIE_VA_WIN_INFO     StcsVaWinInfo;
//NA//    SIE_VA_INDEP_WIN_INFO StcsVaIndepWinInfo[5];
//NA//    SIE_ETH_INFO          StcsEthInfo;

//NA//    SIE_PFPC_INFO       PfpcInfo;
} SIE4_MODE_PARAM;
//@}


//@}

/**
SIE API

@name   SIE_API
*/
//@{

// sie4_lib.c
extern void sie4_isr(void);
extern ER sie4_open(SIE_OPENOBJ *pObjCB);
extern BOOL sie4_isOpened(void);
extern ER sie4_close(void);
extern ER sie4_pause(void);
extern ER sie4_start(void);
extern ER sie4_setMode(SIE4_MODE_PARAM *pSieParam);
extern ER sie4_chgParam(void *pParam, SIE_CHANGE_FUN_PARAM_SEL FunSel);
extern ER sie4_chgFuncEn(SIE_FUNC_SWITCH_SEL FuncSwitch, UINT32 uiFuncSel);
extern ER sie4_waitEvent(SIE_WAIT_EVENT_SEL WaitEvent, BOOL bClrFlag);
extern ER sie4_abortEvent(SIE_WAIT_EVENT_SEL WaitEvent);
//NA//extern void sie4_getBccRslt_LastFrame(SIE_BCC_RSLT_INFO *pBccRslt);
extern UINT32 sie4_getClockRate(void);
// sie4_int.c
extern UINT32 sie4_getIntrStatus(void);
extern void sie4_getEngineStatus(SIE_ENGINE_STATUS_INFO *pEngineStatus);
extern UINT32 sie4_getFunction(void);
extern void sie4_getMainInput(SIE_MAIN_INPUT_INFO *pMainInInfo);
extern void sie4_getActiveWindow(SIE_ACT_WIN_INFO *pActWin);
extern void sie4_getCropWindow(SIE_CRP_WIN_INFO *pCrpWin);
//NA//extern void sie4_setDramInStart(void);
extern void sie4_getObOfs(SIE_OB_OFS_INFO *pObDtOfs);
extern void sie4_getDVI(SIE_DVI_INFO *pDviParam);
//NA//extern void sie4_calcObPlnScl(SIE_OB_PLANE_INFO *pObpParam, SIE_OB_PLANE_ADJ_INFO *pObpAdjParam);
//NA//extern void sie4_calcBSHScl(SIE_BS_H_INFO *pBshParam, SIE_BS_H_ADJ_INFO *pBshAdjParam);
//NA//extern void sie4_calcBSVScl(SIE_BS_V_INFO *pBsvParam, SIE_BS_V_ADJ_INFO *pBsvAdjParam);
//NA//extern void sie4_calcVIG(SIE_VIG_INFO *pVigParam, SIE_VIG_ADJ_INFO *pVigAdjParam);
//NA//extern void sie4_calcVIG_TwoImg(SIE_VIG_INFO *pVigParam, SIE_VIG_ADJ_2IMG_INFO *pVigAdjTwoImgParam);
//NA//extern void sie4_calcECSScl(SIE_ECS_INFO *pEcsParam, SIE_ECS_ADJ_INFO *pEcsAdjParam);
//NA//extern void sie4_calcCaLaSize(SIE_STCS_CALASIZE_GRP_INFO *pVaSzGrpInfo, SIE_STCS_CALASIZE_ADJ_INFO *pVaAdjInfo);
extern UINT32 sie4_getOutAdd(UINT32 uiOutChIdx, SIE_PINGPONG_BUFF_SEL uiBufSel);
extern UINT32 sie4_getOutAdd_Flip(UINT32 uiOutChIdx, SIE_PINGPONG_BUFF_SEL uiBufSel);
//NA//extern void sie4_getCAResult(UINT16 *puiBufR, UINT16 *puiBufG, UINT16 *puiBufB, UINT16 *puiAccCnt);
//NA//extern void sie4_getCAFlipResult(UINT16 *puiBufR, UINT16 *puiBufG, UINT16 *puiBufB, UINT16 *puiAccCnt, SIE_FLIP_SEL pFlipSel);
//NA//extern void sie3_getLaWin(SIE_LA_WIN_INFO *pSetting);
//NA//extern UINT32 sie4_getLaWinSum(void);
//NA//extern void sie4_getCAResultManual(UINT16 *puiBufR, UINT16 *puiBufG, UINT16 *puiBufB, UINT16 *puiAccCnt, SIE_CA_WIN_INFO *CaWinInfo, UINT32 uiBuffAddr);
//NA//extern void sie4_getCAFlipResultManual(UINT16 *puiBufR, UINT16 *puiBufG, UINT16 *puiBufB, UINT16 *puiAccCnt, SIE_CA_WIN_INFO *CaWinInfo, UINT32 uiBuffAddr, SIE_FLIP_SEL pFlipSel);
//NA//extern void sie4_getHisto(UINT16 *puiHisto);
//NA//extern void sie4_getLAResult(UINT16 *puiBufLa1, UINT16 *puiBufLa2);
//NA//extern void sie4_getLAFlipResult(UINT16 *puiBufLa1, UINT16 *puiBufLa2, SIE_FLIP_SEL pFlipSel);
//NA//extern void sie4_getLAResultManual(UINT16 *puiBufLa1, UINT16 *puiBufLa2, SIE_LA_WIN_INFO *LaWinInfo, UINT32 uiBuffAddr);
//NA//extern void sie4_getLAFlipResultManual(UINT16 *puiBufLa1, UINT16 *puiBufLa2, SIE_LA_WIN_INFO *LaWinInfo, UINT32 uiBuffAddr, SIE_FLIP_SEL pFlipSel);
//NA//extern void sie4_getVAResult(UINT32 *puiG1H, UINT32 *puiG1V, UINT32 *puiG2H, UINT32 *puiG2V);
//NA//extern void sie4_getVAFlipResult(UINT32 *puiG1H, UINT32 *puiG1V, UINT32 *puiG2H, UINT32 *puiG2V, SIE_FLIP_SEL pFlipSel);
//NA//extern void sie4_getVAResultManual(UINT32 *puiG1H, UINT32 *puiG1V, UINT32 *puiG2H, UINT32 *puiG2V, SIE_VA_WIN_INFO *VaWinInfo, UINT32 uiBuffAddr);
//NA//extern void sie4_getVAFlipResultManual(UINT32 *puiG1H, UINT32 *puiG1V, UINT32 *puiG2H, UINT32 *puiG2V, SIE_VA_WIN_INFO *VaWinInfo, UINT32 uiBuffAddr, SIE_FLIP_SEL pFlipSel);
//NA//extern void sie4_getBccRslt(SIE_BCC_RSLT_INFO *pBccRslt);
//NA//extern void sie4_getVaIndepWinRslt(SIE_VA_INDEP_WIN_RSLT_INFO *pSetting, UINT32 uiIdx);
extern void sie4_getBurstLength(SIE_BURST_LENGTH *pBurstLen);


//NA//extern ER   sie4_SIE4_CLK_openClock(void);
//NA//extern ER   sie4_SIE4_CLK_closeClock(void);
//NA//extern BOOL sie4_SIE4_CLK_isClockOpened(void);
//NA//extern ER   sie4_SIE4_CLK_chgClockSource(SIE_CLKSRC_SEL ClkSrc);
//NA//extern ER   sie4_SIE4_CLK_chgClockRate(UINT32 uiClkRate);


#endif//_SIE4_LIB_H
//@}

//@}


