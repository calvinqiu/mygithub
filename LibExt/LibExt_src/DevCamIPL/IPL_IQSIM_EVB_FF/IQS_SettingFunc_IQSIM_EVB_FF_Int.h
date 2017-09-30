#ifndef IQS_SETTINGFUNC_IQSIM_EVB_FF_INT_H
#define IQS_SETTINGFUNC_IQSIM_EVB_FF_INT_H

#include "IQS_SettingFlow.h"

UINT32 IQS_IsCapFlow(IQS_FLOW_MSG MsgID);
UINT32 IQS_GetMSNRRepTime(UINT32 Id);
UINT32 IQS_GetIFE2LcaTime(UINT32 Id, IQS_FLOW_MSG MsgID);
UINT32 IQS_GetRetinexMinRatio(void);
UINT32 IQS_GetChromaAdaptRatio(UINT32 Id);
UINT32 IQS_IsChromaAdaptEn(UINT32 Id, BOOL IsCap);
void IQS_ResetMCTF(UINT32 Id);

UINT32 IQS_Rst3DNRCounter(void);

void SetTestAppInfo(void);
void IQSIM_IFE_GetVigSetting(UINT32 Id, BOOL CapFlag, IQS_VIG_INFO *VigInfo);

#endif //SENSOR_FLOW_FUNC_H
