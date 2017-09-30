#ifndef _IQS_API_H_
#define _IQS_API_H_
#include "IPL_AlgInfor.h"
#include "IQS_SettingFlow.h"

typedef struct
{
    UINT32 isSet;
    UINT32 ratio;
} IQS_IQ_INFO;

typedef enum {
    IQS_NR_MODE_AUTO,
    IQS_NR_MODE_MANUAL,
    IQS_NR_MODE_MAX,
} IQS_NR_MODE;

typedef struct
{
    UINT32 isSet;
    IQS_NR_MODE mode;
    UINT32 ratio;
} IQS_3DNR_INFO;

typedef struct {
    UINT32 isSet;
    BOOL isEnable;
    UINT32 ratio;
} IQS_GDC_INFO;

typedef struct {
    UINT32 isSet;
    BOOL isEnable;
    UINT32 ratio;
    UINT32 CenterXOffset;
    UINT32 CenterYOffset;
} IQS_LSC_INFO;

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
} IQS_EXT_SET_IQ_INFO;

extern void IQS_SetIQInfo(UINT32 Id, IQS_EXT_SET_IQ_INFO IQSExtSetIQInfo);
extern void IQS_GetIQInfo(UINT32 Id, IQS_EXT_SET_IQ_INFO* IQSExtSetIQInfo);

extern void IQS_DCE_ParamSetting(UINT32 Id, UINT32 MsgID, UINT32 ZoomIdx, DCE_Distortion* pDistortion, DIST* pDce_Dist, Fact_Norm* pRadious);
extern UINT32 IQS_GetMCTFLevel(UINT32 Id);
//#NT#2016/12/06#Wendy Liao -begin
//#NT#Define 3DNR parameters in LibExt
void Movie_UIDefine_3DNR(UINT32 level, UINT32 config);
//#NT#2016/12/06#Wendy Liao -end

extern UINT32 IQS_GetGammaValue(UINT32 Id, UINT32 Data12Bit);
extern UINT32 IQS_GetInvGammaValue(UINT32 Id, UINT32 Data10Bit);

UINT32 IQS_setUserDefineCC(UINT16 id,UINT16* IQS_User_CC);
UINT32 IQS_getUserDefineCC(UINT16 id,UINT16* IQS_User_CC);
UINT32 IQS_getUserDefineGamma(UINT16* IQS_User_gamma);
UINT32 IQS_setUserDefineGamma(UINT16* IQS_User_gamma);

//#NT#2016/12/22#Wendy Liao -begin
//#NT#Support IPcam function
extern void IQS_SetIPcamFunction(UINT32 Enable);
//#NT#2016/12/22#Wendy Liao -end
#endif //_IQS_API_H_
