#ifndef _UCTRLAPP_IMAGE_H
#define _UCTRLAPP_IMAGE_H
#include "Type.h"
#include "IPL_AlgInfor.h"
#define NVT_FALSE   0
#define NVT_TRUE    1

typedef struct _UCtrlImageParmType
{
    UINT32 isSet;
    UINT32 value;
} UCtrlImageParmType;


typedef struct _UCtrlImageBackLightParmType
{
    UINT32 isSet;
    UINT32 mode;
    UINT32 lv;
} UCtrlImageBackLightParmType;

typedef struct _UCtrlImageAeParmType
{
    UINT32 isSet;
    UINT32 mode;
    UINT32 priority;
    UINT32 maxexp;
    UINT32 minexp;
    UINT32 maxgain;
    UINT32 mingain;
    UINT32 maxiris;
    UINT32 miniris;
    UINT32 exp;
    UINT32 gain;
    UINT32 iris;
} UCtrlImageAeParmType;

typedef struct _UCtrlImageAeWinParmType
{
    UINT32 isSet;
    UINT32 left;
    UINT32 right;
    UINT32 top;
    UINT32 bottom;
} UCtrlImageAeWinParmType;

typedef struct _UCtrlImageFocusParmType
{
    UINT32 isSet;
    UINT32 mode;
    UINT32 speed;
    UINT32 near;
    UINT32 far;
} UCtrlImageFocusParmType;

typedef struct _UCtrlImageWdrParmType
{
    UINT32 isSet;
    UINT32 mode;
    UINT32 lv;
} UCtrlImageWdrParmType;

typedef struct _UCtrlImageAwbParmType
{
    UINT32 isSet;
    UINT32 mode;
    UINT32 rgain;
    UINT32 bgain;
} UCtrlImageAwbParmType;

typedef struct _UCtrlImageDisParmType
{
    UINT32 isSet;
    UINT32 mode;
    UINT32 lv;
} UCtrlImageDisParmType;

typedef struct _UCtrlImageAntiFogParmType
{
    UINT32 isSet;
    UINT32 mode;
    UINT32 lv;
} UCtrlImageAntiFogParmType;

typedef struct _UCtrlImageFocusMoveAbsoluteParmType
{
    UINT32 isSet;
    UINT32 position;
    UINT32 speed;
} UCtrlImageFocusMoveAbsoluteParmType;

typedef struct _UCtrlImageFocusMoveContinuousParmType
{
    UINT32 isSet;
    UINT32 speed;
} UCtrlImageFocusMoveContinuousParmType;

typedef struct _UCtrlImageFocusMoveRelativeParmType
{
    UINT32 isSet;
    UINT32 distance;
    UINT32 speed;
} UCtrlImageFocusMoveRelativeParmType;

typedef struct UCtrlImageFocusStatusParmType
{
    UINT32 isSet;
    UINT32 status;
    UINT32 position;
    UINT32 errcode;
} UCtrlImageFocusStatusParmType;

typedef struct {
    UINT32 isSet;
    UINT32 mode;
    UINT32 ratio;
} UCtrlImageNrParmType;

typedef struct {
    UINT32 isSet;
    BOOL isEnable;
    UINT32 ratio;
} UCtrlImageLdcParmType;

typedef struct {
    UINT32 isSet;
    BOOL isEnable;
    UINT32 ratio;
    UINT32 CenterXOffset;
    UINT32 CenterYOffset;
} UCtrlImageLscParmType;

typedef struct _UCtrlImageInfoType
{
    BOOL               isErr;
    UCtrlImageParmType ipl_id;
    UCtrlImageBackLightParmType backlight;
    UCtrlImageParmType brightness;
    UCtrlImageParmType saturation;
    UCtrlImageParmType contrast;
    UCtrlImageParmType sharpness;
    UCtrlImageParmType hue;
    UCtrlImageParmType nr2D;
    UCtrlImageNrParmType nr3D;
    UCtrlImageLdcParmType LDC;
    UCtrlImageLscParmType LSC;
    UCtrlImageAeParmType ae;
    UCtrlImageAeWinParmType aewin;
    UCtrlImageFocusParmType focus;
    UCtrlImageParmType ircut;
    UCtrlImageWdrParmType wdr;
    UCtrlImageAwbParmType awb;
    UCtrlImageDisParmType dis;
    UCtrlImageAntiFogParmType antifog;
    UCtrlImageFocusMoveAbsoluteParmType focusmove_absolute;
    UCtrlImageFocusMoveContinuousParmType focusmove_continuous;
    UCtrlImageFocusMoveRelativeParmType focusmove_relative;
    UCtrlImageParmType focusmove_stop;
    UCtrlImageFocusStatusParmType focus_status;
	UCtrlImageParmType dewarp_enable;
} UCtrlImageInfoType;

/* Image Setting */
typedef enum {
    ISP_CTRL_MODE_AUTO,
    ISP_CTRL_MODE_MANUAL,
    ISP_CTRL_MODE_MAX,
} ISPCtrlMode;

typedef enum {
    ISP_COLOR_TYPE_NORMAL,
    ISP_COLOR_TYPE_GRAY,
    ISP_COLOR_TYPE_MAX,
} ISPColorType;

typedef enum {
    ISP_ROTATE_TYPE_NONE,
    ISP_ROTATE_TYPE_90,
    ISP_ROTATE_TYPE_180,
    ISP_ROTATE_TYPE_270,
    ISP_ROTATE_TYPE_MAX,
} ISPRotateType;

typedef enum {
    ISP_IRCUT_TYPE_NIGHT,
    ISP_IRCUT_TYPE_DAY,
    ISP_IRCUT_TYPE_AUTO,
    ISP_IRCUT_TYPE_MAX,
} ISPIRCutType;

typedef struct {
    UINT32 uiBrightness;
    UINT32 uiContrast;
    UINT32 uiSaturation;
    UINT32 uiHue;
    UINT32 uiSharpness;
    ISPColorType eColor;
    BOOL bMirror;
    BOOL bFlip;
    ISPRotateType eRotate;
    UINT32 aspect_ratio;
    ISPIRCutType eIRCutMode;
	BOOL bDewarp;
} ISPImageSetting;

/* Auto Exposure */
#define AE_EXP_ROW_NUM          8
#define AE_EXP_COLUMN_NUM       8
#define AE_LUMA_ROW_NUM         32
#define AE_LUMA_COLUMN_NUM      32
#define AE_LUMA_HIST_NUM        64

typedef enum {
    ISP_ANTIFLICKER_50HZ,
    ISP_ANTIFLICKER_60HZ,
    ISP_ANTIFLICKER_AUTO,
    ISP_ANTIFLICKER_NONE,
} ISPAntiFlicker;

typedef enum {
    ISP_AE_STRATEGY_HIGHLIGHT,
    ISP_AE_STRATEGY_LOWLIGHT,
    ISP_AE_STRATEGY_MAX,
} ISPAEStrategy;

typedef enum {
    ISP_AE_IRIS_TYPE_FIXED,
    ISP_AE_IRIS_TYPE_DC,
    ISP_AE_IRIS_TYPE_P,
    ISP_AE_IRIS_TYPE_MAX,
} ISPAEIrisType;

typedef enum {
    ISP_AE_IRIS_MODE_AUTO,
    ISP_AE_IRIS_MODE_CLOSE,
    ISP_AE_IRIS_MODE_OPEN,
    ISP_AE_IRIS_MODE_MANUAL, // P-Iris only
    ISP_AE_IRIS_MODE_MAX,
} ISPAEIrisMode;

typedef struct {
    ISPAEIrisType eIrisType;
    ISPAEIrisMode eIrisMode;
    UINT32 uiDCIrisSpeed;
    UINT32 uiPIrisManualSize;
    UINT32 uiPIrisAutoMaxSize;
    UINT32 uiPIrisAutoMinSize;
} ISPAEIrisSetting;

typedef struct {
    ISPCtrlMode eAEMode;
    UINT32 uiShutterSpeedMax;
    UINT32 uiShutterSpeedMin;
    UINT32 uiGainMax;
    UINT32 uiGainMin;
    UINT32 uiConvergenceSpeed;
    UINT32 uiConvergenceStep;
    UINT32 uiManualShutterSpeed;
    UINT32 uiManualGain;
    ISPAntiFlicker eAntiFlicker;
    ISPAEIrisSetting sIrisSetting;
    UINT32 uiEV;
    UINT32 uiAETolerance;
    UINT32 uiAEDelay;
    UINT8 uiExpAreaWeight[AE_EXP_ROW_NUM*AE_EXP_COLUMN_NUM];
    ISPAEStrategy eAEStrategy; /* lowlight/highlight first */
} ISPAESetting;

typedef struct {
    UINT32 uiShutter;
    UINT32 uiGain;
    UINT16 uiLumaValue[AE_LUMA_ROW_NUM*AE_LUMA_COLUMN_NUM];
    UINT32 uiLumaAverage;
    UINT16 uiLumaHistogram[AE_LUMA_HIST_NUM];
    UINT32 uiPreviewLumaValue;
} ISPAEStatus;

/* White Balance */
typedef struct {
    ISPCtrlMode eWBMode;
    UINT32 uiConvergenceSpeed;
    UINT32 uiConvergenceStep;
    UINT32 uiManualRGain;
    UINT32 uiManualGGain;
    UINT32 uiManualBGain;
} ISPWBSetting;

typedef struct {
    UINT32 uiRGain;
    UINT32 uiGGain;
    UINT32 uiBGain;
    UINT32 uiColorTemperature;
    UINT32 uiRColorAverage;
    UINT32 uiGColorAverage;
    UINT32 uiBColorAverage;
} ISPWBStatus;

/* WDR */
typedef struct {
    BOOL bWDREnable;
    ISPCtrlMode eWDRMode;
    UINT32 uiWDRStrength; // only for manual
} ISPWDRSetting;

/* Noise Reduction */
typedef struct {
    UINT32 uiNR2DStrength;
    UINT32 uiNR3DStrength;
} ISPNRSetting;

/* Digital Image Stabilization */
typedef struct {
    BOOL bDISEnable;
} ISPDISSetting;

/* Lens Distortion Correction */
typedef struct {
    BOOL bLDCEnable;
    UINT32 uiRatio;
} ISPLDCSetting;

/* Lens Shading Correction */
typedef struct {
    BOOL bLSCEnable;
    UINT32 uiRatio;
    UINT32 uiCenterXOffset;
    UINT32 uiCenterYOffset;
} ISPLSCSetting;

/* Anti Fog Correction */
typedef struct {
    BOOL bAntiFogEnable;
    ISPCtrlMode eAntiFogMode;
    UINT32 uiAntiFogStrength;
} ISPAntiFogSetting;

/* Gamma */
#define GAMMA_NODE_NUM 129

typedef enum {
    ISP_GAMMA_CURVE_MODE_LINEAR,
    ISP_GAMMA_CURVE_MODE_DEFAULT,
    ISP_GAMMA_CURVE_MODE_USER,
    ISP_GAMMA_CURVE_MODE_16,
    ISP_GAMMA_CURVE_MODE_18,
    ISP_GAMMA_CURVE_MODE_20,
    ISP_GAMMA_CURVE_MODE_22,
    ISP_GAMMA_CURVE_MODE_24,
    ISP_GAMMA_CURVE_MODE_MAX,
} ISPGammaCurveMode;

typedef struct {
    ISPGammaCurveMode eGammaCurveMode;
    UINT16 uiUserGammaTable[GAMMA_NODE_NUM];
} ISPGammaSetting;

/* Color Correction Matrix */
typedef struct {
    ISPCtrlMode eCCMMode;
    UINT32 uiHighColorTemp;
    UINT16 uiHighCCM[9];
    UINT32 uiMiddleColorTemp;
    UINT16 uiMiddleCCM[9];
    UINT32 uiLowColorTemp;
    UINT16 uiLowCCM[9];
} ISPCCMSetting;

typedef struct {
	SEL_SBS eSBSMode;
}ISPSBSSetting;

typedef struct {
	BOOL bEnable;
}ISPPIPSetting;


//extern void       UCtrlAppImage_InstallID(void) _SECTION(".kercfg_text");
extern void       UCtrlAppImage_Open(void);
extern void       UCtrlAppImage_Close(void);
extern void       UCtrlAppImage_InstallCmd(void);
#endif //_UCTRLAPP_IMAGE_H
