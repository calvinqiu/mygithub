#ifndef IQS_SETTINGFUNC_AHD_DUAL_INT_H
#define IQS_SETTINGFUNC_AHD_DUAL_INT_H

typedef enum
{
    IQS_SIE_OBSUB,
    IQS_IFE_OBSUB
} IQS_SUBOB_MODE;

UINT32 IQS_IsCapFlow(IQS_FLOW_MSG MsgID);
UINT32 IQS_OBSubMode(UINT32 Id, IQS_FLOW_MSG MsgID);
UINT32 IQS_GetMSNRRepTime(UINT32 Id);
UINT32 IQS_GetIFE2LcaTime(UINT32 Id, IQS_FLOW_MSG MsgID);
UINT32 IQS_GetRetinexMinRatio(void);
UINT32 IQS_GetChromaAdaptRatio(UINT32 Id, BOOL IsCap, BOOL GetMaxRatio);
UINT32 IQS_IsChromaAdaptEn(UINT32 Id, BOOL IsCap);

UINT32 IQS_Rst3DNRCounter(void);

#endif //SENSOR_FLOW_FUNC_H
