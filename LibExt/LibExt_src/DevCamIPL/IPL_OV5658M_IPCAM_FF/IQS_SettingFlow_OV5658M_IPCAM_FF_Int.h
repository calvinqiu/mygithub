#ifndef IQS_SETTINGFLOW_OV5658M_IPCAM_FF_INT_H
#define IQS_SETTINGFLOW_OV5658M_IPCAM_FF_INT_H

typedef struct
{
    IQS_IQ_INFO brightness;
    IQS_IQ_INFO saturation;
    IQS_IQ_INFO contrast;
    IQS_IQ_INFO sharpness;
    IQS_IQ_INFO hue;
    IQS_IQ_INFO nr2D;
    IQS_3DNR_INFO nr3D;
    IQS_GDC_INFO GDC;
    IQS_LSC_INFO LSC;
} IQS_SET_IPCAM_INFO;

UINT32 IQS_GetISORange(UINT32 Id, IQS_FLOW_MSG MsgID, UINT32 *IsoValue, UINT32 *IsoStart, UINT32 *IsoEnd, UINT32 *IQIdxL, UINT32 *IQIdxH);
UINT32 IQS_GetISOIdx(UINT32 Id, IQS_FLOW_MSG MsgID);
UINT32 IQS_GetSensorIdx(UINT32 Id);
void IQS_Init(UINT32 Id);
ER IQCB_SettingFlowFunc(UINT32 Id, IQS_FLOW_MSG MsgID, ENG_SUBFUNC *Info,IPL_HAL_GROUP* group);

extern UINT16 IQS_UserDefine_CC[3][9];
extern UINT32 IQS_GammaUserDefine1Lut[130];
extern UINT32 IQS_Gamma24Lut[130];
extern UINT32 IQS_Gamma22Lut[130];
extern UINT32 IQS_Gamma20Lut[130];
extern UINT32 IQS_Gamma18Lut[130];
extern UINT32 IQS_Gamma16Lut[130];
extern UINT32 IQS_UserGAMMALinear[130];
extern UINT32 IQS_UserGAMMA[130];

void IQS_iqstInit(UINT32 Id);

#endif //SENSOR_FLOW_FUNC_H