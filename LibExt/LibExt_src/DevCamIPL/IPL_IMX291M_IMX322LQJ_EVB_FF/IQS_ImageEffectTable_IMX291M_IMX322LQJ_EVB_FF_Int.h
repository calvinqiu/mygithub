#ifndef IQS_IE_SETTINGTABLE_IMX291_IMX322LQJ_EVB_FF_H
#define IQS_IE_SETTINGTABLE_IMX291_IMX322LQJ_EVB_FF_H
#include "IQS_SettingFlow.h"

extern IQS_COLOR_EFFECT_PARAM ColorEffectTable_Normal;
extern IQS_COLOR_EFFECT_PARAM ColorEffectTable_BW;
extern IQS_COLOR_EFFECT_PARAM ColorEffectTable_SEPIA;
extern IQS_COLOR_EFFECT_PARAM ColorEffectTable_VIVID;

extern IPE_YCCFIX IE_ColorPencil_Param;
extern IPE_YCCFIX IE_PencilSketch_Param;
extern UINT32 IE_FISYEYE_LUT[65];
extern IQS_COLOR_EFFECT_PARAM ColorEffectTable;

extern INT32 IPE_SelectiveCC_SatTab[15][24];
extern IPE_YCCFIX IPE_RockEffect_YccFix;
extern UINT16 IQS_UserDefine_CC[3][9];
extern UINT32 IQS_GammaUserDefine1Lut[130];
extern UINT32 IQS_Gamma24Lut[130];
extern UINT32 IQS_Gamma22Lut[130];
extern UINT32 IQS_Gamma20Lut[130];
extern UINT32 IQS_Gamma18Lut[130];
extern UINT32 IQS_Gamma16Lut[130];
extern UINT32 IQS_UserGAMMALinear[130];
extern UINT32 IQS_UserGAMMA[130];
#endif

