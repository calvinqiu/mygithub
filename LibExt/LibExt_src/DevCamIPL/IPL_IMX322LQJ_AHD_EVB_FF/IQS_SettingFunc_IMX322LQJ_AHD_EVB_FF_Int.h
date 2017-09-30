#ifndef IQS_SETTINGFUNC_IMX322LQJ_AHD_INT_H
#define IQS_SETTINGFUNC_IMX322LQJ_AHD_INT_H

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

//#NT#2016/10/17#Wendy Liao -begin
//#NT#Define 3DNR parameters in LibExt
void Movie_UIDefine_3DNR(UINT32 level, UINT32 config);
//#NT#2016/10/17#Wendy Liao -end

UINT32 IQS_Rst3DNRCounter(void);

#endif //SENSOR_FLOW_FUNC_H