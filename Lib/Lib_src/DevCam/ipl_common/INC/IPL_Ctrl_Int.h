/**
    IPL Ctrl module(streaming)

    This controller define several operation to control image pipeline.
\n  Run: start IPL in defined mode.
\n  Stop: stop IPL.
\n  RunTimeChg: chg IPL configuration when IPL in Running stage.


    @file       IPL_Ctrl_Int.h
    @ingroup    mIIPL_Ctrl
    @note
\n              preview usage example:
\n              {
\n                  IPL_CTRL_OpenTsk();
\n                  ...
\n                  que = IPL_CTRLQuery(ICF_FLOW_A);
\n                  ...
\n                  IPL_CTRLRun(ICF_FLOW_A,...);
\n                  ...
\n                  IPL_CTRLRunTimeChg(IPC_Chg_ImeP1Out,...);
\n                  IPL_CTRLRunTimeChg(IPC_Chg_ImeP3Out,...);
\n                  IPL_CTRLRunTimeChg(IPC_Chg_Crop,...);
\n                  IPL_CTRLRunTimeChg(IPC_Chg_Load,...);
\n                  ...
\n                  IPL_CTRLRunTimeChg(IPC_SIE_IQparam,...);
\n                  ...
\n                  IPL_CTRLMode(IPC_STOP_ALL);
\n                  ...
\n                  IPL_CTRL_CloseTsk();
\n
\n               }
\n              preview -> capture -> preview usage example:
\n              {
\n                  IPL_CTRL_OpenTsk();
\n                  ...
\n                  que = IPL_CTRLQuery(ICF_FLOW_A);
\n                  ...
\n                  IPL_CTRLRun(ICF_FLOW_A,...);
\n                  ...
\n                  IPL_CTRLMode(IPC_STOP_YCC);
\n                  ...
\n                  IPL_CTRLMode(IPC_PAUSE_SIE);
\n                  ...
\n                  IPL_CTRLRunTimeChg(IPC_SIE_,...);
\n                  IPL_CTRLMode(IPC_RESUME_SIE);
\n                  ...
\n                  IPL_CTRLMode(IPC_STOP_RAW);
\n                  ...
\n                  IPL_CTRLRun(ICF_FLOW_A,...);
\n               }

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/

#ifndef _IPL_CTRL_INT_H
#define _IPL_CTRL_INT_H

#ifndef _IPL_COM_INT_INC
    #error "illegal include IPL common module internal .H file"
#endif


#include "IPL_Ctrl_flow_Int.h"
#include "IPL_ISREvent_Int.h"
#include "IPL_Hal.h"
/** \addtogroup mIIPL_Ctrl */
//@{
/**
     IPL_Ctrl information

     query IPL_Ctrl relative information
*/
typedef union _IPC_QINFO
{
    QUE_FLOW_A          QUE_FA;         ///< buffer number needed by flow A
    QUE_FLOW_C          QUE_FC;         ///< buffer number needed by Flow C
    QUE_FLOW_D          QUE_FD;         ///< buffer number needed by Flow D
    IPE_SUB_OUT         Rdy_IPESub;     ///< current ready IPE YCC sub-image
    SIE_ETH_DramRst     Rdy_Eth;        ///< current ready edge threshold output
    IPL_YCC_IMG_INFO    CCIR_Rdy;       ///< current ready CCIR output info
    SIE_LAPARAM         Rdy_LAParam;    ///< current ready ca window information(not statistic data)
    UINT32              FrameCnt;       ///< current frame count
    GDC_NOR_FACTOR_INFO NorFactorInfo;  ///< GDC normal factor information
    FLIP_TYPE           FlipType;       ///< current image pipe line flip type
    IME_GET_STATS_INFO  IMEStatsInfo;   ///< current ime statistic information
}IPC_QINFO,*PIPC_QINFO;

/**
     IPL_Ctrl configuration

     @configure IPL when ipl_start
*/
typedef union _IPC_INFO
{
    ICF_FLOW_A_STRUCT      ICF_FA;  ///< Flow A
    ICF_FLOW_C_STRUCT      ICF_FC;  ///< Flow C
    ICF_FLOW_D_STRUCT      ICF_FD;  ///< Flow D
}IPC_INFO,*PIPC_INFO;

/**
     Control IPL sie ppb buffer number
*/
extern ER IPL_CtrlSetSIEPPBNum(IPL_PROC_ID id, UINT32 buf_num);
/**
     Control IPL start to run
     @note
     IPL can run in 3 modes,plz reference IPLCTRL_FLOW, this api will open and then start IPL engines, and configure
     inner status into "running_status"
\n   IPL must be in all_stop_status when call it.
\n   IPL engines are including : SIE,PRE,IFE,DCE,IPE,IFE2,IME.

     @param[in] flow       start IPL by indicatied flow.
     @param[in] info       info. for IPL,each flow have corresponding data structure
                           ex:
                           fulfill "pInfo->ICF_FA" if flow is "FLOW_A"
     @param[in] Obj        isr notification CB,call back at each engine isr occurred
     @param[in] fpImg      ime frame end notification CB.

     @return Description of data returned.
         - @b E_OK:        IPL start.
         - @b E_SYS:       ICF task not opened or some of IPL engines not close yet, or IPL not in stop_status.
     Example: (Optional)
     @code
     {
        IPC_INFO info;
        info.ICF_FA
        ...

        IPL_CTRL_OpenTsk();
        ...
        IPL_CTRLRun(ICF_FLOW_A,&info,NULL,NULL);
        ...
     }
     @endcode
*/
extern ER IPL_CTRLRun(IPL_PROC_ID id, IPLCTRL_FLOW flow, IPC_INFO* pInfo, IPL_ISR_OBJ_INFO *pObj, fp_ImgRdy fpImg);
/**
     Control IPL mode switch
     @note
     pause IPL and close engine from running_status. and uninstall isr notification CB.
\n   detail state mechine plz reference enum:IPC_STOP

     @param[in] IPL1 Cmd
     @param[in] IPL2 Cmd
     @param[in] IPL3 Cmd
     @param[in] IPL4 Cmd

     @return Description of data returned.
         - @b E_OK:    IPL stop.
         - @b E_SYS:   IPL already stopped,or state mechine error.
     Example: (Optional)
     @code
     {
        IPL_CTRLRun(ICF_FLOW_A,&info,NULL,NULL);
        ...
        IPL_CTRLMode(IPC_STOP_ALL, IPC_STOP_ALL, IPC_STOP_ALL, IPC_STOP_ALL);
        ...
        IPL_CTRL_CloseTsk();
     }
     @endcode
*/
ER IPL_CTRLMode(IPC_CTRL_CMD Cmd1, IPC_CTRL_CMD Cmd2, IPC_CTRL_CMD Cmd3, IPC_CTRL_CMD Cmd4);

/**
     Get IPL mode switch status
     @note
     pause IPL and close engine from running_status. and uninstall isr notification CB.
\n   detail state mechine plz reference enum:IPC_STOP

     @param[in] IPL_PROC_ID
     @param[out] ER
     @param[out] ER
*/

void IPL_CTRLMode_GetErrCode(IPL_PROC_ID Id, ER *sts1, ER *sts2);
/**
     Control IPL trigger type
*/
ER IPL_CTRLSetTrigType(IPL_TRIG_CTRL_CMD_INFO *Info);

/**
    Control IPL trigger type, add adjustive timer trigger
*/
ER IPL_CTRLSetAdjustiveTrigger(IPL_TRIG_CTRL_CMD_INFO *Info);


/**
     Control IPL manage ctrl flow type
*/
ER IPL_CTRLSetMCtrlType(IPL_M_CTRL_TYPE MCtrlType);

/**
     get IPL_Ctrl information
     @note
     get IPL_Ctrl information according input enum.
\n   (no association with IPL in run/stop status.)
\n   if flow == ICF_FLOWA, return information in IPC_QINFO->QUE_FA
\n   if flow == QUE_CSUB_Rdy, return information in IPC_QINFO->Rdy_Csub,
     if return address = 0, means that sub-image do not output to dram yet!

     @param[in] id          input which IPL_ID need to be queried
     @param[in] event       input which event need to be queried

     @return Description of data returned.
         - @b IPC_QINFO:

     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern IPC_QINFO IPL_CTRLQuery(IPL_PROC_ID id,IPLCTRL_QUERY event);

/**
     Change IPL setting when IPL in running_status
     @note
     change IPL engine during IPL in running status, all Chg will load into engine registor.
     this api will wait until all configuration is latched,excluding
     "IPC_Chg_ImeP1Out","IPC_Chg_ImeP2Out","IPC_Chg_ImeP3Out","IPC_Chg_Crop","IPC_SIE_".
     detail plz reference enum:IPC_RUNTIMECHG !

     @param[in] Chg       input operation
     @param[in] ptr       operation info., data type plz reference enum:IPC_RUNTIMECHG

     @return Description of data returned.
         - @b E_OK:    IPL new configuration is latched
         - @b E_SYS:   IPL is not in running_status or state mechine error!
     Example: (Optional)
     @code
     {
        IPL_CTRL_OpenTsk();
        ...
        IPL_CTRLRun(ICF_FLOW_A,&info,NULL,NULL);
        ...

        IPL_CTRLRunTimeChg(IPC_Chg_ImeP2Out,(void*)&IPC_IMEOut_Info);
        IPL_CTRLRunTimeChg(IPC_Chg_Crop,(void*)&IPC_IMEOut_Info);
        IPL_CTRLRunTimeChg(IPC_Chg_Load,NULL);
        ...
        IPL_CTRLRunTimeChg(IPC_SIE_IQparam,(void*)&IPC_IQparam);
        ...
        IPL_CTRLMode(IPC_STOP_ALL);
        ...
        IPL_CTRL_CloseTsk();
     }
     @endcode
*/
extern ER IPL_CTRLRunTimeChg(IPL_PROC_ID Id, IPC_RUNTIMECHG Chg,void* ptr);

/**
     Change IPL IQ setting when IPL in running_status

     @note
     all enum:IPC_RUNTIMECHG_IQ have no timing control.

     @param[in] Chg       input operation
     @param[in] ptr       operation info., data type plz reference enum:IPC_RUNTIMECHG_IQ

     @return Description of data returned.
         - @b E_OK:    IPL new configuration is latched
         - @b E_SYS:   IPL is not in running_status or state mechine error!

     @endcode
*/
extern ER IPL_CTRLRunTimeChg_IQ(IPL_PROC_ID Id, IPC_RUNTIMECHG_IQ Chg,void* ptr);

/**
     Change IPL IQ setting when IPL in running_status

     @note
     all enum:IPC_RUNTIMECHG_IQ have no timing control.

     @param[in] Chg       input operation
     @param[in] ptr       operation info., data type plz reference enum:IPC_RUNTIMECHG_IQ

     @return Description of data returned.
         - @b E_OK:    IPL new configuration is latched
         - @b E_SYS:   IPL is not in running_status or state mechine error!

     @endcode
*/
extern ER IPL_CTRLRunTimeChg_IQ_ISR(IPL_PROC_ID Id, IPC_RUNTIMECHG_IQ Chg, void* ptr);


/**
     set timeout interval


     @param[in] ms       timeout interval ms

     @return Description of data returned.
         - @b E_OK:
         - @b E_SYS:

     @endcode
*/
extern ER IPL_CTRLSetTimeoutT(UINT32 ms);


/**
     set raw enc ratio (%)


     @param[in] ratio 61~100% default 70%

     @return Description of data returned.
         - @b E_OK:
         - @b E_SYS:

     @endcode
*/
extern ER IPL_CTRLSetRawEncRatio(UINT32 ratio);

/**
     open icf task

     open and initial ICF_Task, this task is dedicated to control IPL.

     @return Description of data returned.
         - @b E_OK:    Task open and in idle status
         - @b E_SYS:   task is already opened
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern void IPL_CTRL_OpenTsk(void);

/**
     close icf task

     un-registor all ipl control operations those are hooked on this task, and close task.

     @return Description of data returned.
         - @b E_OK:    Task close
         - @b E_SYS:   task is already closed
     Example: (Optional)
     @code
     {
        // Example for calling foo()
     }
     @endcode
*/
extern void IPL_CTRL_CloseTsk(void);

/*
    IPC_D2D_STRIPE callback function
*/
extern void _STRIPE_CB(IPL_PROC_ID id, USIZE *Ref_1x,Coordinate *Ref_1xCen,
                USIZE *SieInSize, USIZE *SieOutSize, UINT32 *SenHRatio,UINT32 *SenVRatio,
                DCE_SUBFUNC* subF, IPL_HAL_GROUP *Update, UINT32 eventID) __attribute__ ((weak));


/*
    for dce stripe table pre-calculation

    @param[in] SrcImg    source image width and height
    @param[in] pImeOut   ptr need to be available if ime is piped into dce d2d operation, otherwise,
\n                       give NULL to this ptr.
    @param[in] eventID   input event.
*/
extern DCE_Stripe IPC_D2D_STRIPE(IPL_PROC_ID id, IPL_HAL_PARASET paramIdx, USIZE SrcImg, UINT32 ImeIsdEn, UINT32 ImeSSrEn,
                                                UINT32 SenHBinRatio, UINT32 SenVBinRatio, IPL_RAW_PIX StPix, UINT32 eventID, DCE_SRCMODE DCEMode, BOOL DCE_IsOpened);
//@}
#endif
