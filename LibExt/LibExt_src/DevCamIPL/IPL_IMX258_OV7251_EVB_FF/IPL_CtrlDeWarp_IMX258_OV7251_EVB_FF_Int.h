#ifndef IPL_CTRL_IMX258_OV7251_DeWarp_H_
#define IPL_CTRL_IMX258_OV7251_DeWarp_H_

/**
    IPL_CtrlDeWarp_IMX258_OV7251_EVB_FF_Int.h


    @file       IPL_CtrlDeWarp_IMX258_OV7251_EVB_FF_Int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
typedef struct
{
    IPL_HAL_PARASET IPL_HalParamId[IPL_ID_MAX_NUM];
    fpEngInit fp_IFE2Init;
    fpEngSubFunc fp_AdaptSetting;
    fpEngCLK fp_SIEClk;
    fpSIEClkSrc fp_SIEClkSrc;
    fpSIEPclkPad fp_SIEPclkSel;
    fpEngCLK fp_RHEClk;
    fpEngCLK fp_RDEClk;
    fpEngCLK fp_IFEClk;
    fpEngCLK fp_DCEClk;
    fpEngCLK fp_IPEClk;
    fpEngCLK fp_IFE2Clk;
    fpEngCLK fp_IMEClk;
    fpEngInit fp_SieInit;
    fpEngInit fp_RheInit;
    fpEngInit fp_RdeInit;
    fpEngInit fp_IfeInit;
    fpEngInit fp_DceInit;
    fpEngInit fp_IpeInit;
    fpEngInit fp_ImeInit;
} IPLCTRL_FLOWD_INFO;

void IPL_CtrlInitIpcInfoFlowD(UINT32 Id, IPC_INFO *IpcInfo);
void IPL_CtrlInitBufInfoFlowD(UINT32 Id, IPLBUF_ENG2DRAMINFO *BufInfo);
void IPL_CtrlPrvInitFlowD(UINT32 Id, IPLCTRL_INFO *CtrlInfo);
IPL_HAL_PARASET IPL_CtrlGetHalSetIdFlowD(UINT32 Id);
#endif //IPL_CTRL_IMX258_OV7251_DeWarp_H_
