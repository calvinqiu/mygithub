#ifndef _IQS_CTRL_H_
#define _IQS_CTRL_H_
/**
    IQS_Ctrl.h


    @file       IQS_Ctrl.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "Type.h"
#include "ErrorNo.h"
#include "IPL_Utility.h"
#include "IQS_SettingFlow.h"

/** \addtogroup IQS_Ctrl */
//@{
#define IQS_CLAMP(in,lb,ub) (((INT32)(in) < (INT32)(lb)) ? (INT32)(lb) : (((INT32)(in) > (INT32)(ub)) ? (INT32)(ub) : (INT32)(in)))

#define IQS_ENG_MASK    0x0000ffff
#define IQS_TBL_MASK    0xffff0000

#define IQS_ENG_SIE     0x00000001
#define IQS_ENG_PRE     0x00000002
#define IQS_ENG_DCE     0x00000004
#define IQS_ENG_IFE     0x00000008
#define IQS_ENG_IPE     0x00000010
#define IQS_ENG_IFE2    0x00000020
#define IQS_ENG_IME     0x00000040
#define IQS_ENG_RHE     0x00000080

#define IQS_TBL_GAMMA   0x00010000
#define IQS_TBL_3DCC    0x00020000
#define IQS_TBL_ECS     0x00040000
#define IQS_TBL_YCURVE  0x00080000
#define IQS_TBL_VIG     0x00100000

typedef ER (fpSIEx_IQPARAM)(UINT32 Id, IQS_FLOW_MSG MsgID, SIE_SUBFUNC *subf,IPL_HAL_GROUP* group);
typedef ER (fpRHE_IQPARAM)(UINT32 Id, IQS_FLOW_MSG MsgID, RHE_SUBFUNC *subf,IPL_HAL_GROUP* group);
typedef ER (fpIFE_IQPARAM)(UINT32 Id, IQS_FLOW_MSG MsgID, IFE_SUBFUNC *subf,IPL_HAL_GROUP* group);
typedef ER (fpDCE_IQPARAM)(UINT32 Id, IQS_FLOW_MSG MsgID, DCE_SUBFUNC *subf,IPL_HAL_GROUP* group);
typedef ER (fpIPE_IQPARAM)(UINT32 Id, IQS_FLOW_MSG MsgID, IPE_SUBFUNC *subf,IPL_HAL_GROUP* group);
typedef ER (fpIFE2_IQPARAM)(UINT32 Id, IQS_FLOW_MSG MsgID, IFE2_SUBFUNC *subf,IPL_HAL_GROUP* group);
typedef ER (fpIME_IQPARAM)(UINT32 Id, IQS_FLOW_MSG MsgID, IME_SUBFUNC *subf,IPL_HAL_GROUP* group);
typedef struct
{
   UINT32 rhe_param;
   UINT32 ipe_param;
}IQS_WDRparam;
extern ER IQS_SIEx_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPL_HAL_PARASET idx, fpSIEx_IQPARAM cb);
extern ER IQS_RHE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPL_HAL_PARASET idx, fpRHE_IQPARAM cb);
extern ER IQS_IFE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPL_HAL_PARASET idx, fpIFE_IQPARAM cb);
extern ER IQS_DCE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPL_HAL_PARASET idx, fpDCE_IQPARAM cb);
extern ER IQS_IPE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPL_HAL_PARASET idx, fpIPE_IQPARAM cb);
extern ER IQS_IFE2_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPL_HAL_PARASET idx, fpIFE2_IQPARAM cb);
extern ER IQS_IME_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPL_HAL_PARASET idx, fpIME_IQPARAM cb);
extern ER IQS_setWDR_IQparam(UINT32 Id, IPL_HAL_PARASET idx, IQS_WDRparam param);
extern void IQS_setWDR(BOOL bEnable);
extern void IQS_SetUpdateBit(UINT32 Id, UINT32 UpdateBit);
extern void IQS_SetIsrUpdateBit(UINT32 Id, UINT32 UpdateBit);
extern void IQS_ClearUpdateBit(UINT32 Id);
extern void IQS_ClearIsrUpdateBit(UINT32 Id);
extern UINT32 IQS_GetUpdateBit(UINT32 Id);
extern UINT32 IQS_GetIsrUpdateBit(UINT32 Id);
extern void IQS_SetRate(UINT32 rate);
extern void IQS_Ctrl_Init(UINT32 Id, UINT32 CurrentStatus);
extern void IQS_Ctrl_Process(UINT32 Id, UINT32 CurrentStatus);
extern void IQS_Ctrl_ISR(UINT32 Id);
//@}
#endif //_IQS_CTRL_H_
