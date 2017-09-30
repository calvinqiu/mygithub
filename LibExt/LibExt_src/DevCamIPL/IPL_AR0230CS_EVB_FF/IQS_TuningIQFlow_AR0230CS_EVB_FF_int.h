#ifndef _IQS_TUNINGIQFLOW_AR0230CS_EVB_FF_INT_H_
#define _IQS_TUNINGIQFLOW_AR0230CS_EVB_FF_INT_H_

void IQS_SetIQInfo(UINT32 Id, IQS_EXT_SET_IQ_INFO IQSExtSetIQInfo);
void IQS_GetIQInfo(UINT32 Id, IQS_EXT_SET_IQ_INFO* IQSExtSetIQInfo);
void IQUC_GetTuningRatio(UINT32 Id, UINT32* IQUC_TuningIQRatio, UINT32* IQUC_TuningCACValue);
void IQUC_SetTuningRatio(UINT32 Id, UINT32* IQUC_TuningIQRatio, UINT32* IQUC_TuningCACValue);
void IQS_CalRatio(UINT32 SenId_Value, UINT32 IQIdxL, UINT32 IQIdxH);
void IQS_CalValue(UINT32 SenId_Value, UINT32 IQIdxL, UINT32 IQIdxH);
void IQS_Intpl_ISO(UINT32 Id, IQS_FLOW_MSG MsgID);

extern UINT32 IQS_DCE_GLUT_Buf[Total_SENIDNum][65];
extern UINT32 IQS_Vig_Tab_Buf[Total_SENIDNum][17];
extern UINT16 IQS_OBSub_SIE_Buf[Total_SENIDNum];
extern UINT32 IQS_OBSub_IFE_Buf[Total_SENIDNum][4];
extern UINT32 IQS_Outl_Bri_Buf[Total_SENIDNum][5];
extern UINT32 IQS_Outl_Dark_Buf[Total_SENIDNum][5];
extern UINT32 IQS_RangeA_Thr_Buf[Total_SENIDNum][6];
extern UINT32 IQS_RangeB_Thr_Buf[Total_SENIDNum][6];
extern UINT32 IQS_RangeA_Lut_Buf[Total_SENIDNum][17];
extern UINT32 IQS_RangeB_Lut_Buf[Total_SENIDNum][17];
extern RangeCFGB IQS_RangeB_Buf[Total_SENIDNum];
extern IFE_FILT IQS_Filter_Buf[Total_SENIDNum];
extern UINT8 IQS_Gbal_Dtab_Buf[Total_SENIDNum][16];
extern UINT8 IQS_Gbal_Stab_Buf[Total_SENIDNum][16];
extern IFE_GBAL IQS_Gbal_Buf[Total_SENIDNum];
extern IPE_DirScorCalTh IQS_DirScoreTh_Buf[Total_SENIDNum];
extern IPE_EdgeW IQS_EWParam_Buf[Total_SENIDNum];
extern IPE_EDGEENH IQS_EdgeEnh_Buf[Total_SENIDNum];
extern UINT8 IQS_EDTab_Buf[Total_SENIDNum][16];
extern UINT8 IQS_EStab_Buf[Total_SENIDNum][16];
extern EdgeMap IQS_EDMap_Buf[Total_SENIDNum];
extern EdgeMapS IQS_ESMap_Buf[Total_SENIDNum];
extern UINT8 IQS_FStab_Buf[Total_SENIDNum][16];
extern UINT8 IQS_FDtab_Buf[Total_SENIDNum][16];
extern IPE_RGBLPF IQS_RGBLpf_Buf[Total_SENIDNum];
extern INT16 Saturation_Buf[Total_SENIDNum];
extern INT16 Contrast_Buf[Total_SENIDNum];
extern INT16 IQS_Cctrl_IntOfs_Buf[Total_SENIDNum];
extern UINT8 IQS_hueTab_Buf[Total_SENIDNum][24];
extern UINT32 IQS_RefCenter_YTh_Buf[Total_SENIDNum][3];
extern UINT32 IQS_RefCenter_YWt_Buf[Total_SENIDNum][4];
extern IFE2_REFCENT_Set IQS_RefCenter_Y_Buf[Total_SENIDNum];
extern UINT32 IQS_RefCenter_UVTh_Buf[Total_SENIDNum][3];
extern UINT32 IQS_RefCenter_UVWt_Buf[Total_SENIDNum][4];
extern IFE2_REFCENT_Set IQS_RefCenter_UV_Buf[Total_SENIDNum];
extern UINT32 IQS_IFE2Filter_YTh_Buf[Total_SENIDNum][5];
extern UINT32 IQS_IFE2Filter_YWt_Buf[Total_SENIDNum][6];
extern UINT32 IQS_IFE2Filter_UVTh_Buf[Total_SENIDNum][5];
extern UINT32 IQS_IFE2Filter_UVWt_Buf[Total_SENIDNum][6];
extern IQS_IFE2_FILTER IQS_IFE2Filter_Buf[Total_SENIDNum];
extern IME_CHRA_ADAP IQS_ChromaAdapt_Buf[Total_SENIDNum];
extern IME_CHRA_LADAP IQS_ChromaAdapt_Lum_Buf[Total_SENIDNum];
extern IME_CHRA_CADAP IQS_ChromaAdapt_Color_Buf[Total_SENIDNum];
//#NT#2017/03/13#Wendy Liao -begin
//#Add WDR maxlevel by ISO index
extern AUTO_WDR_OVEREXP IQS_autoWDR_adj[Total_SENIDNum];
//#NT#2017/03/13#Wendy Liao -end
extern  UINT32 IQUC_DumpRatioTable(UINT32 Id);
#endif
