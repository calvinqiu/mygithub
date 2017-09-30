#ifndef _IQS_UTILITY_H_
#define _IQS_UTILITY_H_
/**
    IQS_Utility.h


    @file       IQS_Utility.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "Type.h"
#include "ErrorNo.h"
#include "IPL_Utility.h"
#include "IPL_Hal_DCE_Info.h"
#include "IQS_SettingFlow.h"

#include "sie1_lib.h"
#include "Sensor.h"

#define IQS_UD_INDEX_NOSET  999999

/** \addtogroup IQS_Utility */
//@{
#define IQS_SIE_ADDR  0xC0C00000
#define IQS_DCE_ADDR  0xC0C20000
#define IQS_IPE_ADDR  0xC0C30000
#define IQS_IME_ADDR  0xC0C40000
#define IQS_IFE_ADDR  0xC0C70000
#define IQS_RDE_ADDR  0xC0CB0000
#define IQS_DRE_ADDR  0xC0CD0000
#define IQS_RHE_ADDR  0xC0CE0000
#define IQS_IFE2_ADDR 0xC0D00000

#define IQS_SIE_REG_SIZE  0x600
#define IQS_IFE_REG_SIZE  0x130
#define IQS_DCE_REG_SIZE  0x2a4
#define IQS_IPE_REG_SIZE  0x1e10
#define IQS_IME_REG_SIZE  0x2e8
#define IQS_IFE2_REG_SIZE 0x9C

#define IQS_ECS_TAB_SIZE  0x4204
#define IQS_DRE_REG_SIZE  0x3300  //0x300 * 10 * 2

#define IQS_QV_SIEREG_FILE   "A:\\IQS\\QV\\SIE.BIN"
#define IQS_QV_IPEREG_FILE   "A:\\IQS\\QV\\IPE.BIN"
#define IQS_QV_IFEREG_FILE   "A:\\IQS\\QV\\IFE.BIN"
#define IQS_QV_DCEREG_FILE   "A:\\IQS\\QV\\DCE.BIN"
#define IQS_QV_IMEREG_FILE   "A:\\IQS\\QV\\IME.BIN"
#define IQS_QV_IFE2REG_FILE  "A:\\IQS\\QV\\IFE2.BIN"

#define IQS_QVREF_IMED2D_FILE "A:\\IQS\\QV\\IMEQVREF.BIN"
#define IQS_QV_IMED2D_FILE    "A:\\IQS\\QV\\IMED2D.BIN"

typedef struct
{
    UINT32 uiCntrPosX;  //in: ratio x/1000
    UINT32 uiCntrPosY;  //in
    UINT32 uiVigX;
    UINT32 uiVigY;
    UINT8  uiVigT;
    UINT32 uiVigXDiv;
    UINT32 uiVigYDiv;
} IQS_VIG_INFO;

typedef enum
{
    IQS_CAL_MULTIPLY,
    IQS_CAL_INTPL,
    IQS_CAL_HUE,
    IQS_ADJ_ITEM_MAX_CNT,
}IQS_CAL_WAY;

int IQS_Intpl(int Index, int LowValue, int HighValue, int MinIndex, int MaxIndex);
void IQS_IntplTbl(UINT32* aSourceL, UINT32* aSourceH, int aSize, UINT32* aTarget, int xIdx, int xIdxL, int xIdxH);
void IQS_IntplTblUint8(UINT8* aSourceL, UINT8* aSourceH, int aSize, UINT8* aTarget, int xIdx, int xIdxL, int xIdxH);
void IQS_Gamma_Intpl(UINT32* G1, UINT32* G2, INT32 X1, INT32 X2, INT32 X, UINT32 *G);
void IQS_CC_Intpl(UINT16* CC1, UINT16* CC2, INT32 X1, INT32 X2, INT32 X, UINT16 *CC);
int IQS_IQ_Cal(int Ratio, int Value, int MinValue, int MaxValue, IQS_CAL_WAY WAY);
INT32 IQS_IQ_Intpl(int Ratio, int MinRatio, int MaxRatio, int Value, int MinValue, int MaxValue);
void IQS_DCE_Intpl(UINT32* aSource, UINT32* aTarget, int xIdx, int xIdxL, int xIdxH);

UINT16* SIE_xfer2CAGamma(UINT32 Id, UINT32* IPELut);

/**
     re-organize gamma lut

     @note convert Gamma Lut
     @param[in]
     @return
         - @b UINT32.
*/
UINT32* IPE_GammaReorgon(UINT32 Id, UINT32* RLut, UINT32* GLut, UINT32* BLut);
UINT32* IPE_YCurveReorgon(UINT32 Id, UINT32* YLut);
UINT32* IPE_xfer2EdgeGamma(UINT32 Id, UINT32* IPELut);
UINT32* IQS_GetECStab( UINT32 Id, UINT32 CapFlag, UINT32* CaliECStab, UINT32 ECSsize, SENSOR_INFO *SenInfo);
void SIE_GetVigSetting(UINT32 Id, BOOL CapFlag, IQS_VIG_INFO *VigParam);
void IFE_GetVigSetting(UINT32 Id, BOOL CapFlag, IQS_VIG_INFO *VigParam);

/**
     The function to calc fov gain
     Return FOV gain

     @param[in] uiImgWidht    Image width
     @param[in] uiImgHeight   Image height

     @return
         - @b UINT32:       fov gain
*/
UINT32 DCE_GetFovGain(UINT32 Id, BOOL isCap, UINT32 *DCE_Lut);

/**
     The function to dump the IPP register

     @param[in] Msg    Message from capture flow

     @return
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern BOOL IQS_DumpEngRegister(IPL_FLOW_PROC_MSG Msg);
extern BOOL IQS_DumpPrvRegister(void);
extern void IQS_SetEngDumpBuffer(IPL_HAL_ENG Eng, UINT32 Addr, UINT32 Size);
extern void IQS_GetEngDumpBuffer(IPL_HAL_ENG Eng, UINT32 *Addr, UINT32 *Size);

typedef void (*IQS_MOVIELOG_FP)(UINT32 Id, char* Buf, UINT32 BufLen);
typedef UINT32 (*IQTUC_DUMP_FP)(UINT32 Id);
void IQS_SetMovieLogFP(IQS_MOVIELOG_FP fp);
void IQS_GetLogString(UINT32 Id, char* Buf, UINT32 BufLen);
void IQTUC_DumpFP(IQTUC_DUMP_FP fp);
void IQS_ToneCurveAdj(UINT32 *gammaIn, UINT32* gammaOut,UINT16 mid,UINT16 low,UINT16 high,UINT16 level);
//@}
#endif //_IQS_UTILITY_H_
