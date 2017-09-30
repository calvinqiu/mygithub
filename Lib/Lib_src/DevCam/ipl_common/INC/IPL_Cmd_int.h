#ifndef _IPL_CMD_INT_H_
#define _IPL_CMD_INT_H_

#ifndef _IPL_COM_INT_INC
    #error "illegal include IPL common module internal .H file"
#endif

/**
    IPL_Cmd_int.h


    @file       IPL_Cmd_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "Type.h"
#include "IPL_Cmd.h"
#include "IPL_Ctrl_flow_Int.h"
#include "IPL_AlgInfor_Int.h"
#include "Sensor.h"


// Notice: This Structure will move to IPL_Cmd_int.h
// Replace Strcture   IPL_INIT_DATA, IPL_MODE_DATA, IPL_MODE_DATA_CAP
/**
    IPL change mode parameter

    Data structure for IPL CMD use!
*/
typedef struct IPL_CHGMODE_DATA
{
    UINT32 ProcSenId;                               ///< assign sensor Id
    IPL_MODE Mode;                                  ///< next mode

    UINT32 BufAddr;                                 ///< input buffer address
    UINT32 BufSize;                                 ///< input & ouput buffer size (return used buffer size)

    IPL_SENCROPRATIO CropRatio;                     ///< sie crop ratio

    IPL_IME_IMG_DATA ImePath1;                      ///< ime path1  information , h264 encode path
    IPL_IME_IMG_DATA ImePath2;                      ///< ime path2  information , display path
    IPL_IME_IMG_DATA ImePath3;                      ///< ime path3  information , dual display path
    IPL_IME_IMG_DATA ImePath4;                      ///< ime path4  information , ADAS path

    IPL_CBMSG_FP CB_FP;                             ///< flow callback function
    IPL_H264_CB CB_H264;                            ///< h264 cb function
    IPL_ISR_EVENT_FP ISR_CB_FP[IPL_ISR_MAX_CNT];    ///< ISR CB for photo mode

    IPL_POST_PROC_FUNC_EN FuncEn;                   ///< notify IPL post process enable & prepare buffer
    UINT32 EventId;                                 ///< only valid when in capture mode, notify IPL current still event.
    BOOL ActFlg;                                    ///<
    FLIP_TYPE FlipInfo;                             ///< flip information
    IPL_VR360_DATA VR360Info;                       ///< VR360 parameters, need enable IPL_FUNC_VR360 function in FunEn
} IPL_CHGMODE_DATA;

typedef ER (*IPL_CMD_CHGMODE_FP)(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode);

typedef ER (*IPL_CMD_PROC_FP)(void *Data);

typedef struct
{
    UINT32 TotalCnt;            //total table count
    IPL_CMD_PROC_FP *ProcFp;    //IPL_CMD_PROC_FP array address
} IPL_CMD_INFO;

/**

     IPL_GetCmdSETTab

     @note get command table information

     @return
         - @b IPL_CMD_INFO.
*/
extern IPL_CMD_INFO IPL_GetCmdSETTab(void);

/**

     IPL_GetCmdGETTab

     @note get command table information

     @return
         - @b IPL_CMD_INFO.
*/
extern IPL_CMD_INFO IPL_GetCmdGETTab(void);

/**

     IPL_GetExtCmdSETTab

     @note get extend command table information

     @return
         - @b IPL_CMD_INFO.
*/
extern IPL_CMD_INFO IPL_GetExtCmdSETTab(void) __attribute__ ((weak));

/**

     IPL_GetExtCmdGETTab

     @note get extend command table information

     @return
         - @b IPL_CMD_INFO.
*/
extern IPL_CMD_INFO IPL_GetExtCmdGETTab(void) __attribute__ ((weak));

/**

     IPL_GetChgModeFp

     @note need return  ProcFP

     @return
         - @b IPL_CMD_CHGMODE_FP.
*/
extern IPL_CMD_CHGMODE_FP IPL_ChgModeFCB(IPL_MODE CurMode, IPL_MODE NextMode) __attribute__ ((weak));

/**

     IPL_SetImeFCB

     @note set ime information
     @param[in] IPL_IME_DATA(ime information)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @return
         - @b IPL_IME_PATH(modify path).
*/
extern IPL_IME_PATH IPL_SetImeFCB(IPL_IME_DATA *Info, IPC_IMEOut_Info *IpcInfoP1, IPC_IMEOut_Info *IpcInfoP2, IPC_IMEOut_Info *IpcInfoP3, IPC_IMEOut_Info *IpcInfoP4) __attribute__ ((weak));

/**

     IPL_SetDZoomFCB

     @note get digital zoom information
     @param[in] IPL_DZOOM(digital zoom information)
     @param[in/out] IPC_Dzoom_Info(no need assign memory)
     @return
         - @b void.
*/
extern void IPL_SetDZoomFCB(IPL_DZOOM *Info, IPC_Dzoom_Info *SetInfo) __attribute__ ((weak));

/**

     IPL_SetSleepFCB

     @note enter sleep mode
     @param[in] IPL_SLEEP_INFO(enter sleep mode information)
     @return
         - @b void.
*/
extern void IPL_SetSleepFCB(IPL_SLEEP_INFO *Info) __attribute__ ((weak));

/**

     IPL_SetWakeupFCB

     @note exit sleep mode
     @param[in] IPL_WAKEUP_INFO(exit sleep mode information)
     @return
         - @b void.
*/
extern void IPL_SetWakeupFCB(IPL_WAKEUP_INFO *Info) __attribute__ ((weak));

/**
     IPL_GetCapRawFCB

     @note get cap raw flow
     @param[in] IPL_GET_CAP_RAW_DATA
     @return
         - @b void.
*/
extern void IPL_GetCapRawFCB(IPL_GET_CAP_RAW_DATA *Info) __attribute__ ((weak));

/**
     IPL_SetPauseDMAFCB

     @note set pause dma
     @param[in] void
     @return
         - @b void.
*/
extern void IPL_SetPauseDMAFCB(IPL_PAUSEDMA_INFO *Info) __attribute__ ((weak));

/**
     IPL_SetResumeDMAFCB

     @note set resume dma
     @param[in] void
     @return
         - @b void.
*/
extern void IPL_SetResumeDMAFCB(IPL_RESUMEDMA_INFO *Info) __attribute__ ((weak));


/**

     IPL_SetImgRatioFCB

     @note set sensor crop & ime out information
     @param[in] IPL_SET_IMGRATIO_DATA
     @param[out] IPC_FRONTEND(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @param[out] IPC_IMEOut_Info(no need assign memory)
     @return
         - @b IPL_IME_PATH(modify path).
*/
extern IPL_IME_PATH IPL_SetImgRatioFCB(IPL_SET_IMGRATIO_DATA *Info, IPC_FRONTEND *IpcInfo, IPC_IMEOut_Info *IpcInfoP1, IPC_IMEOut_Info *IpcInfoP2, IPC_IMEOut_Info *IpcInfoP3, IPC_IMEOut_Info *IpcInfoP4) __attribute__ ((weak));

/**

     IPL_SetVAFCB

     @note set va information
     @param[in] Info                va information
     @param[in, out] HalParaIdx     hal parameter index
     @param[in, out] Out            VA Out Addr
*/
extern void IPL_SetVAFCB(IPL_SET_VA_DATA *Info, IPL_HAL_PARASET *HalParaIdx, SIE_Dma_Out *Out)__attribute__ ((weak));

/**

     IPL_GetIPLCurInfoFCB

     @note get ipl current information
     @param[in] IPL_ID ipl id
     @param[out] ipl control flow mode
*/

extern void IPL_GetIPLCurInfoFCB(IPL_CUR_INFOR *IplCurInfo)__attribute__ ((weak));

/**

     IPL_GetIMEHalIdxFCB

     @note get ime Hal Idx
     @param[in] ImeInfo                ime information
     @param[in, out] HalParaIdx     hal parameter index
*/
extern void IPL_GetIMEHalIdxFCB(IPL_IME_INFOR *ImeInfo, IPL_HAL_PARASET *HalParaIdx)__attribute__ ((weak));

/**

     IPL_GetImeCurInfoFCB

     @note get ime current information
     @param[in] IPL_ID ipl id
     @param[in] IPL_IME_PATH  path id
     @return
         - @b IPL_YCC_IMG_INFO(address).

*/
extern IPL_YCC_IMG_INFO IPL_GetImeCurInfoFCB(IPL_PROC_ID Ipl_Id, IPL_IME_PATH Path_Id) __attribute__ ((weak));


/**

     IPL_GetImeInfoFCB

     @note get ime information
     @param[in] IPL_ID ipl id
     @param[in] IPL_IME_PATH  path id
     @param[in] UINT32  ime buffer id
     @return
         - @b IPL_YCC_IMG_INFO(address).

*/
extern IPL_YCC_IMG_INFO IPL_GetImeInfoFCB(IPL_PROC_ID Ipl_Id, IPL_IME_PATH Path_Id, UINT32 BufId) __attribute__ ((weak));

/**

     IPL_GetImeInfo2FCB

     @note get ime information
     @param[in] IPL_ID ipl id
     @param[in] IPL_IME_PATH  path id
     @param[in] UINT32  ime buffer id
     @return
         - @b IPL_YCC_IMG_INFO(address).

*/
extern IPL_YCC_IMG_INFO IPL_GetIme2InfoFCB(IPL_PROC_ID Ipl_Id, IPL_IME_PATH Path_Id, UINT32 BufId) __attribute__ ((weak));
/**

     IPL_GetIPLInfoFCB

     @note get IPL information
     @param[in] IPL_ID ipl id
     @param[out] sensor information
     @param[out] ime information
     @param[out] raw information
     @param[out] ime information
     @return
         - @b IPL_YCC_IMG_INFO(address).

*/
extern UINT32 IPL_GetIPLInfoFCB(IPL_PROC_ID Ipl_Id, SENSOR_INFO *SenInfo, IPL_IME_INFOR *ImeInfo, IPL_RDE_RAW_INFO *RawInfo, ImeFuncEn *FuncEn, UINT32 *DisOnOff) __attribute__ ((weak));

/**

     IPL_TrigFCB

     @note Trigger IPL Stop or Run
     @param[in] IPC cmd for all id
     @return
         - @b E_OK:
         - @b E_SYS:

*/
extern ER IPL_TrigFCB(IPC_CTRL_CMD *ProcCmd) __attribute__ ((weak));

/**

     IPL_SetTrigInfoFCB

     @note update Trigger Info
     @param[in] IPL Mode Info
     @param[out] IPC Stop cmd for all Id
     @return
         - @b IPL Stop Flag.

*/
extern void IPL_SetTrigInfoFCB(IPL_CHGMODE_DATA *ChgMode, UINT32 *TrigFlg) __attribute__ ((weak));


/**

     IPL_SetIPLStopFCB

     @note Set Stop specific IPL
     @param[in] All IPL Current Mode Info
     @param[in] IPL Chg Mode Info
     @param[in] IPC Stop cmd for all Id
     @return
         - @b IPL Stop Flag.

*/
extern void IPL_SetIPLStopFCB(IPL_CHGMODE_DATA **IPLModeInfo, IPL_CHGMODE_DATA *ChgMode, UINT32 *TrigFlg) __attribute__ ((weak));

/**

     IPL_SetIPLResumeFCB

     @note Set Resume specific IPL
     @param[in] All IPL Current Mode Info
     @param[in] IPL Chg Mode Info
     @param[in] IPL Stop Flg
     @param[out] IPC cmd
     @return
         - @b void.

*/
extern void IPL_SetIPLResumeFCB(IPL_CHGMODE_DATA **IPLModeInfo, IPL_CHGMODE_DATA *ChgMode, UINT32 *TrigFlg, IPC_CTRL_CMD *ProcCmd) __attribute__ ((weak));

/**

     IPL_GetCtrlFlowFCB

     @note get ipl control flow type, for h264 reference(2-Stripe not support IME-Direct flow)
     @param[in] CtrlInfo              ipl_ctrl information
     @param[in, out] control flow     normal, 2-stripe
*/
extern void IPL_GetCtrlFlowFCB(IPL_CTRL_FLOW_INFO *CtrlInfo)__attribute__ ((weak));
#endif //_IPL_CMD_INT_H_

