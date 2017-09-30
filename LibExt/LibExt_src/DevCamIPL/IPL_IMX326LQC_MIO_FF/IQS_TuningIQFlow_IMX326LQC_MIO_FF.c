#ifndef _IQS_TUNINGIQFLOW_IMX326LQC_MIO_FF_C_
#define _IQS_TUNINGIQFLOW_IMX326LQC_MIO_FF_C_

#include "IPL_IMX326LQC_MIO_FF_Int.h"

extern INT32 IQS_RatioTable[ADJ_IQRatio_MAX_CNT][Total_ISONum];
extern INT32 IQS_CAC_Table[Total_SENIDNum][ADJ_IQRatio_CAC_MAX_CNT];
extern IQS_SET_IPCAM_INFO IQS_IPCAM_RatioTable[Total_SENIDNum];

//# Final Ratio, [2] with ISOidxL & ISOidxH
INT32 IQS_FinalRatio[Total_SENIDNum][ADJ_IQRatio_MAX_CNT][2];

//# Temp IQ Parameters after Ratio, [2] with ISOidxL & ISOidxH
UINT16 IQS_OBSub_Temp[2];
UINT32 IQS_CAC_R_Temp[2];
UINT32 IQS_CAC_B_Temp[2];
UINT32 IQS_Outl_Bri_Temp[2][5];
UINT32 IQS_Outl_Dark_Temp[2][5];
UINT32 IQS_RangeA_Thr_Temp[2][6];
UINT32 IQS_RangeB_Thr_Temp[2][6];
UINT32 IQS_RangeA_Lut_Temp[2][17];
UINT32 IQS_RangeB_Lut_Temp[2][17];
UINT32 IQS_Filter_BilatW_Temp[2];
UINT32 IQS_Filter_RthW_Temp[2];
UINT32 IQS_Filter_Clamp_Mul_Temp[2];
UINT32 IQS_Filter_Clamp_Th_Temp[2];
UINT32 IQS_Gbal_Thr_Temp[2];
UINT32 IQS_Edge_EnhP_Temp[2];
UINT32 IQS_Edge_EnhN_Temp[2];
UINT8 IQS_Edge_EWDir_Temp[2][3];
UINT32 IQS_Edge_EthrL_Temp[2];
UINT32 IQS_Edge_EtabL_Temp[2];
INT16 IQS_Sat_Temp[2];
INT16 IQS_Con_Temp[2];
UINT32 IQS_LCA_OutUV_Temp[2];

//# Final IQ Parameters after Ratio
UINT32 IQS_DCE_GLUT_Buf[Total_SENIDNum][65];
UINT32 IQS_Vig_Tab_Buf[Total_SENIDNum][17];

UINT16 IQS_OBSub_SIE_Buf[Total_SENIDNum];
UINT32 IQS_OBSub_IFE_Buf[Total_SENIDNum][4];

UINT32 IQS_Outl_Bri_Buf[Total_SENIDNum][5];
UINT32 IQS_Outl_Dark_Buf[Total_SENIDNum][5];

UINT32 IQS_RangeA_Thr_Buf[Total_SENIDNum][6];
UINT32 IQS_RangeB_Thr_Buf[Total_SENIDNum][6];
UINT32 IQS_RangeA_Lut_Buf[Total_SENIDNum][17];
UINT32 IQS_RangeB_Lut_Buf[Total_SENIDNum][17];
RangeCFGB IQS_RangeB_Buf[Total_SENIDNum];
IFE_FILT IQS_Filter_Buf[Total_SENIDNum];

UINT8 IQS_Gbal_Stab_Buf[Total_SENIDNum][16];
UINT8 IQS_Gbal_Dtab_Buf[Total_SENIDNum][16];
IFE_GBAL IQS_Gbal_Buf[Total_SENIDNum];

IPE_DirScorCalTh IQS_DirScoreTh_Buf[Total_SENIDNum];
IPE_EdgeW IQS_EWParam_Buf[Total_SENIDNum];
IPE_EDGEENH IQS_EdgeEnh_Buf[Total_SENIDNum];

UINT8 IQS_EDTab_Buf[Total_SENIDNum][16];
UINT8 IQS_EStab_Buf[Total_SENIDNum][16];
EdgeMap IQS_EDMap_Buf[Total_SENIDNum];
EdgeMapS IQS_ESMap_Buf[Total_SENIDNum];

UINT8 IQS_FStab_Buf[Total_SENIDNum][16];
UINT8 IQS_FDtab_Buf[Total_SENIDNum][16];

IPE_RGBLPF IQS_RGBLpf_Buf[Total_SENIDNum];
//#NT#2017/03/13#Wendy Liao -begin
//#Add WDR maxlevel by ISO index
AUTO_WDR_OVEREXP IQS_autoWDR_adj[Total_SENIDNum];
//#NT#2017/03/13#Wendy Liao -end

INT16 Saturation_Buf[Total_SENIDNum];
INT16 Contrast_Buf[Total_SENIDNum];

INT16 IQS_Cctrl_IntOfs_Buf[Total_SENIDNum];
UINT8 IQS_hueTab_Buf[Total_SENIDNum][24];

UINT32 IQS_RefCenter_YTh_Buf[Total_SENIDNum][3];
UINT32 IQS_RefCenter_YWt_Buf[Total_SENIDNum][4];
IFE2_REFCENT_Set IQS_RefCenter_Y_Buf[Total_SENIDNum];
UINT32 IQS_RefCenter_UVTh_Buf[Total_SENIDNum][3];
UINT32 IQS_RefCenter_UVWt_Buf[Total_SENIDNum][4];
IFE2_REFCENT_Set IQS_RefCenter_UV_Buf[Total_SENIDNum];

UINT32 IQS_IFE2Filter_YTh_Buf[Total_SENIDNum][5];
UINT32 IQS_IFE2Filter_YWt_Buf[Total_SENIDNum][6];
UINT32 IQS_IFE2Filter_UVTh_Buf[Total_SENIDNum][5];
UINT32 IQS_IFE2Filter_UVWt_Buf[Total_SENIDNum][6];
IQS_IFE2_FILTER IQS_IFE2Filter_Buf[Total_SENIDNum];

IME_CHRA_ADAP IQS_ChromaAdapt_Buf[Total_SENIDNum];
IME_CHRA_LADAP IQS_ChromaAdapt_Lum_Buf[Total_SENIDNum];
IME_CHRA_CADAP IQS_ChromaAdapt_Color_Buf[Total_SENIDNum];

///////////////////
// IPCAM set IQ info
///////////////////
void IQS_SetIQInfo(UINT32 Id, IQS_EXT_SET_IQ_INFO IQSExtSetIQInfo)
{
if (IQS_IPCAM_ENABLE)
{
    UINT32 i, SenId_Value = IQS_GetSensorIdx(Id);

    // saturation
    if( IQSExtSetIQInfo.saturation.isSet )
    {
        IQS_IPCAM_RatioTable[SenId_Value].saturation.isSet = 1;
        IQS_IPCAM_RatioTable[SenId_Value].saturation.ratio = IQS_CLAMP(IQSExtSetIQInfo.saturation.ratio*2, 0, 200);
    }
    // contrast
    if( IQSExtSetIQInfo.contrast.isSet )
    {
        IQS_IPCAM_RatioTable[SenId_Value].contrast.isSet = 1;
        IQS_IPCAM_RatioTable[SenId_Value].contrast.ratio = IQS_CLAMP(IQSExtSetIQInfo.contrast.ratio*2, 0, 200);
    }
    // sharpness
    if( IQSExtSetIQInfo.sharpness.isSet )
    {
        IQS_IPCAM_RatioTable[SenId_Value].sharpness.isSet = 1;
        IQS_IPCAM_RatioTable[SenId_Value].sharpness.ratio = IQS_CLAMP(IQSExtSetIQInfo.sharpness.ratio*2, 0, 200);
    }
    // 2DNR
    if( IQSExtSetIQInfo.nr2D.isSet )
    {
        IQS_IPCAM_RatioTable[SenId_Value].nr2D.isSet = 1;
        IQS_IPCAM_RatioTable[SenId_Value].nr2D.ratio = IQS_CLAMP(IQSExtSetIQInfo.nr2D.ratio*2, 0, 200);
    }
    // 3DNR
    if( IQSExtSetIQInfo.nr3D.isSet )
    {
        IQS_IPCAM_RatioTable[SenId_Value].nr3D.isSet = 1;
        IQS_IPCAM_RatioTable[SenId_Value].nr3D.ratio = IQS_CLAMP(IQSExtSetIQInfo.nr3D.ratio, 0, 10);
    }
    // Brightness
    if( IQSExtSetIQInfo.brightness.isSet )
    {
    ///////////////////
    // IPE-Image Effect
    ///////////////////
    UINT32 IMEffect_Value = IPL_AlgGetUIInfo(Id, IPL_SEL_IMAGEEFFECT);
    IQS_COLOR_EFFECT_PARAM ColorEffectTable;
    if     ( IMEffect_Value == SEL_IMAGEEFFECT_BW )    ColorEffectTable = ColorEffectTable_BW;
    else if( IMEffect_Value == SEL_IMAGEEFFECT_SEPIA ) ColorEffectTable = ColorEffectTable_SEPIA;
    else if( IMEffect_Value == SEL_IMAGEEFFECT_VIVID ) ColorEffectTable = ColorEffectTable_VIVID;
    else                                               ColorEffectTable = ColorEffectTable_Normal;

        IQS_IPCAM_RatioTable[SenId_Value].brightness.isSet = 1;
        IQS_IPCAM_RatioTable[SenId_Value].brightness.ratio = IQS_CLAMP(IQSExtSetIQInfo.brightness.ratio*2, 0, 200);
        IQS_Cctrl_IntOfs_Buf[SenId_Value] = (INT16)IQS_IQ_Cal(IQS_IPCAM_RatioTable[SenId_Value].brightness.ratio, ColorEffectTable.IntOfs, -128, 127, IQS_CAL_INTPL);
    }
    // HUE
    if( IQSExtSetIQInfo.hue.isSet )
    {
        IQS_IPCAM_RatioTable[SenId_Value].hue.isSet = 1;
        IQS_IPCAM_RatioTable[SenId_Value].hue.ratio = IQS_CLAMP(IQSExtSetIQInfo.hue.ratio, 0, 360);
        for( i=0; i<24; i++ )
            IQS_hueTab_Buf[SenId_Value][i] = (UINT32)IQS_IQ_Cal(IQS_IPCAM_RatioTable[SenId_Value].hue.ratio , IQS_hueTab[SenId_Value][i], 0, 255, IQS_CAL_HUE);
    }
    // GDC
    if( IQSExtSetIQInfo.GDC.isSet )
    {
        IQS_IPCAM_RatioTable[SenId_Value].GDC.isSet = 1;
        IQS_IPCAM_RatioTable[SenId_Value].GDC.isEnable = IQSExtSetIQInfo.GDC.isEnable;
        if(IQS_IPCAM_RatioTable[SenId_Value].GDC.isEnable == ENABLE)
        {
            IQS_IPCAM_RatioTable[SenId_Value].GDC.ratio = IQS_CLAMP(IQSExtSetIQInfo.GDC.ratio*2, 0, 200);
            IQS_DCE_Intpl(IQS_DCE_GLUT[SenId_Value], IQS_DCE_GLUT_Buf[SenId_Value], IQS_IPCAM_RatioTable[SenId_Value].GDC.ratio, 0, 200);
            IPL_AlgSetUIInfo(Id, IPL_SEL_GDCCOMP, SEL_GDCCOMP_ON);
        }
        else
        {
            IPL_AlgSetUIInfo(Id, IPL_SEL_GDCCOMP, SEL_GDCCOMP_OFF);
        }
    }
    // LSC (Lens shading correction)
    if( IQSExtSetIQInfo.LSC.isSet )
    {
        IQS_IPCAM_RatioTable[SenId_Value].LSC.isSet = 1;
        IQS_IPCAM_RatioTable[SenId_Value].LSC.isEnable = IQSExtSetIQInfo.LSC.isEnable;
        if(IQS_IPCAM_RatioTable[SenId_Value].LSC.isEnable == ENABLE)
        {
            IQS_IPCAM_RatioTable[SenId_Value].LSC.ratio = IQS_CLAMP(IQSExtSetIQInfo.LSC.ratio*2, 0, 200);
            IQS_IPCAM_RatioTable[SenId_Value].LSC.CenterXOffset = IQS_CLAMP(IQSExtSetIQInfo.LSC.CenterXOffset*2, 0, 200);
            IQS_IPCAM_RatioTable[SenId_Value].LSC.CenterYOffset = IQS_CLAMP(IQSExtSetIQInfo.LSC.CenterYOffset*2, 0, 200);
            for( i=0; i<17; i++ )
                IQS_Vig_Tab_Buf[SenId_Value][i] = (UINT32)IQS_IQ_Cal(IQS_IPCAM_RatioTable[SenId_Value].LSC.ratio, IQS_Vig_Set[SenId_Value].Tab[i], 0, 1023, IQS_CAL_MULTIPLY);
            IQS_SettingFlowFunc(Id, IQS_MANUAL_TRIG);
        }
    }
}
}

void IQS_GetIQInfo(UINT32 Id, IQS_EXT_SET_IQ_INFO* IQSExtSetIQInfo)
{
if (IQS_IPCAM_ENABLE)
{
    UINT32 SenId_Value = IQS_GetSensorIdx(Id);

    IQSExtSetIQInfo->brightness.ratio  = IQS_IPCAM_RatioTable[SenId_Value].brightness.ratio/2;  // 0~100; default:50
    IQSExtSetIQInfo->saturation.ratio  = IQS_IPCAM_RatioTable[SenId_Value].saturation.ratio/2;  // 0~100; default:50
    IQSExtSetIQInfo->contrast.ratio    = IQS_IPCAM_RatioTable[SenId_Value].contrast.ratio/2;    // 0~100; default:50
    IQSExtSetIQInfo->sharpness.ratio   = IQS_IPCAM_RatioTable[SenId_Value].sharpness.ratio/2;   // 0~100; default:50
    IQSExtSetIQInfo->hue.ratio         = IQS_IPCAM_RatioTable[SenId_Value].hue.ratio;           // 0~360; default:0
    IQSExtSetIQInfo->nr2D.ratio        = IQS_IPCAM_RatioTable[SenId_Value].nr2D.ratio/2;        // 0~100; default:50

    IQSExtSetIQInfo->nr3D.ratio        = IQS_IPCAM_RatioTable[SenId_Value].nr3D.ratio;          // 0~10

    IQSExtSetIQInfo->GDC.isEnable      = IQS_IPCAM_RatioTable[SenId_Value].GDC.isEnable;
    IQSExtSetIQInfo->GDC.ratio         = IQS_IPCAM_RatioTable[SenId_Value].GDC.ratio/2;         // 0~100; default:50

    IQSExtSetIQInfo->LSC.isEnable      = IQS_IPCAM_RatioTable[SenId_Value].LSC.isEnable;
    IQSExtSetIQInfo->LSC.ratio         = IQS_IPCAM_RatioTable[SenId_Value].LSC.ratio/2;         // 0~100; default:0
    IQSExtSetIQInfo->LSC.CenterXOffset = IQS_IPCAM_RatioTable[SenId_Value].LSC.CenterXOffset/2; // 0~100; default:50
    IQSExtSetIQInfo->LSC.CenterYOffset = IQS_IPCAM_RatioTable[SenId_Value].LSC.CenterYOffset/2; // 0~100; default:50
}
}

void IQUC_GetTuningRatio(UINT32 Id, UINT32* IQUC_TuningIQRatio, UINT32* IQUC_TuningCACValue)
{
#if IQS_RATIOTABLE_ENABLE
    IQTUC_DumpFP(IQUC_DumpRatioTable);

    UINT32 IQIdxL = IQS_GetISOIdx(Id, IQS_MANUAL_TRIG);
    UINT32 RatioItem;

    for( RatioItem=0; RatioItem<ADJ_IQRatio_MAX_CNT; RatioItem++ )
        IQUC_TuningIQRatio[RatioItem] = IQS_RatioTable[RatioItem][IQIdxL];

    for( RatioItem=0; RatioItem<ADJ_IQRatio_CAC_MAX_CNT; RatioItem++ )
        IQUC_TuningCACValue[RatioItem] = IQS_CAC_Table[Id][RatioItem];
#endif
}

void IQUC_SetTuningRatio(UINT32 Id, UINT32* IQUC_TuningIQRatio, UINT32* IQUC_TuningCACValue)
{
#if IQS_RATIOTABLE_ENABLE
    UINT32 IQIdxL = IQS_GetISOIdx(Id, IQS_MANUAL_TRIG);
    UINT32 RatioItem;

    for( RatioItem=0; RatioItem<ADJ_IQRatio_MAX_CNT; RatioItem++ )
        IQS_RatioTable[RatioItem][IQIdxL] = IQUC_TuningIQRatio[RatioItem];

    for( RatioItem=0; RatioItem<ADJ_IQRatio_CAC_MAX_CNT; RatioItem++ )
        IQS_CAC_Table[Id][RatioItem] = IQUC_TuningCACValue[RatioItem];
#endif
}

void IQS_CalRatio(UINT32 SenId_Value, UINT32 IQIdxL, UINT32 IQIdxH)
{
    UINT32 RatioItem, RefItem;
    UINT32 UISat_Ratio   = 2 * IPL_AlgGetUIInfo(SenId_Value, IPL_SEL_SATURATION);
    UINT32 UICon_Ratio   = 2 * IPL_AlgGetUIInfo(SenId_Value, IPL_SEL_CONTRAST);
    UINT32 UISharp_Ratio = 2 * IPL_AlgGetUIInfo(SenId_Value, IPL_SEL_SHARPNESS);

// IQT-ADJ
#if IQS_RATIOTABLE_ENABLE
    for( RatioItem=0; RatioItem<ADJ_IQRatio_MAX_CNT; RatioItem++ )
    {
        IQS_FinalRatio[SenId_Value][RatioItem][0] = IQS_RatioTable[RatioItem][IQIdxL];
        IQS_FinalRatio[SenId_Value][RatioItem][1] = IQS_RatioTable[RatioItem][IQIdxH];
    }
#else
    IQS_FinalRatio[SenId_Value][ADJ_IQRatio_OB][0] = 0;
    IQS_FinalRatio[SenId_Value][ADJ_IQRatio_OB][1] = 0;
    for( RatioItem=1; RatioItem<ADJ_IQRatio_MAX_CNT; RatioItem++ )
    {
        IQS_FinalRatio[SenId_Value][RatioItem][0] = 100;
        IQS_FinalRatio[SenId_Value][RatioItem][1] = 100;
    }
#endif

// IQT-UI
    for( RefItem=0; RefItem<2; RefItem++ )
    {
        if( UISat_Ratio != 100 )
        {
            IQS_FinalRatio[SenId_Value][ADJ_IQRatio_EDGE_BLACK][RefItem] = (IQS_FinalRatio[SenId_Value][ADJ_IQRatio_EDGE_BLACK][RefItem] * UISharp_Ratio) / 100;
            IQS_FinalRatio[SenId_Value][ADJ_IQRatio_EDGE_WHITE][RefItem] = (IQS_FinalRatio[SenId_Value][ADJ_IQRatio_EDGE_WHITE][RefItem] * UISharp_Ratio) / 100;
        }
        if( UISat_Ratio != 100 )
            IQS_FinalRatio[SenId_Value][ADJ_IQRatio_SAT][RefItem]        = (IQS_FinalRatio[SenId_Value][ADJ_IQRatio_SAT][RefItem]        * UISat_Ratio  ) / 100;
        if( UICon_Ratio != 100 )
            IQS_FinalRatio[SenId_Value][ADJ_IQRatio_CONT][RefItem]       = (IQS_FinalRatio[SenId_Value][ADJ_IQRatio_CONT][RefItem]       * UICon_Ratio  ) / 100;
    }

// IQT-IPCAM
if (IQS_IPCAM_ENABLE)
{
    for( RefItem=0; RefItem<2; RefItem++ )
    {
        IQS_FinalRatio[SenId_Value][ADJ_IQRatio_NR][RefItem]         = (IQS_FinalRatio[SenId_Value][ADJ_IQRatio_NR][RefItem]         * IQS_IPCAM_RatioTable[SenId_Value].nr2D.ratio      ) / 100;

        IQS_FinalRatio[SenId_Value][ADJ_IQRatio_EDGE_BLACK][RefItem] = (IQS_FinalRatio[SenId_Value][ADJ_IQRatio_EDGE_BLACK][RefItem] * IQS_IPCAM_RatioTable[SenId_Value].sharpness.ratio ) / 100;
        IQS_FinalRatio[SenId_Value][ADJ_IQRatio_EDGE_WHITE][RefItem] = (IQS_FinalRatio[SenId_Value][ADJ_IQRatio_EDGE_WHITE][RefItem] * IQS_IPCAM_RatioTable[SenId_Value].sharpness.ratio ) / 100;

        IQS_FinalRatio[SenId_Value][ADJ_IQRatio_SAT][RefItem]        = (IQS_FinalRatio[SenId_Value][ADJ_IQRatio_SAT][RefItem]        * IQS_IPCAM_RatioTable[SenId_Value].saturation.ratio) / 100;

        IQS_FinalRatio[SenId_Value][ADJ_IQRatio_CONT][RefItem]       = (IQS_FinalRatio[SenId_Value][ADJ_IQRatio_CONT][RefItem]       * IQS_IPCAM_RatioTable[SenId_Value].contrast.ratio  ) / 100;
    }
}
}

void IQS_CalValue(UINT32 SenId_Value, UINT32 IQIdxL, UINT32 IQIdxH)
{
    UINT32 FilterSize, RefItem, ISOIdx;

    for( RefItem=0; RefItem<2; RefItem++ )
    {
        if( RefItem==0 )    ISOIdx = IQIdxL;
        else                ISOIdx = IQIdxH;

        // OB
        IQS_OBSub_Temp[RefItem] = (UINT16)IQS_CLAMP((INT32)IQS_OBSub[ISOIdx] + IQS_FinalRatio[SenId_Value][ADJ_IQRatio_OB][RefItem], 0, 1023);

        // Outlier
        for ( FilterSize=0; FilterSize<5; FilterSize++ )
        {
            IQS_Outl_Bri_Temp[RefItem][FilterSize]  = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_OUTL_BRI][RefItem], IQS_Outl_Bri[ISOIdx][FilterSize], 4095, 0, IQS_CAL_INTPL);
            IQS_Outl_Dark_Temp[RefItem][FilterSize] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_OUTL_DARK][RefItem], IQS_Outl_Dark[ISOIdx][FilterSize], 4095, 0, IQS_CAL_INTPL);
        }

        // NR
        for( FilterSize=0; FilterSize<6; FilterSize++ )
        {
            IQS_RangeA_Thr_Temp[RefItem][FilterSize] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_NR][RefItem], IQS_RangeA_Thr[ISOIdx][FilterSize], 0, 1023, IQS_CAL_MULTIPLY);
            IQS_RangeB_Thr_Temp[RefItem][FilterSize] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_NR][RefItem], IQS_RangeB_Thr[ISOIdx][FilterSize], 0, 1023, IQS_CAL_MULTIPLY);
        }
        for( FilterSize=0; FilterSize<17; FilterSize++ )
        {
            IQS_RangeA_Lut_Temp[RefItem][FilterSize] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_NR][RefItem], IQS_RangeA_Lut[ISOIdx][FilterSize], 0, 1023, IQS_CAL_MULTIPLY);
            IQS_RangeB_Lut_Temp[RefItem][FilterSize] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_NR][RefItem], IQS_RangeB_Lut[ISOIdx][FilterSize], 0, 1023, IQS_CAL_MULTIPLY);
        }
        IQS_Filter_BilatW_Temp[RefItem] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_NR_FP][RefItem], IQS_Filter[ISOIdx].Bilat_w, 0, 15, IQS_CAL_INTPL);
        IQS_Filter_RthW_Temp[RefItem] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_NR_SPATIAL][RefItem], IQS_Filter[ISOIdx].Rth_W, 0, 15, IQS_CAL_INTPL);

        IQS_Filter_Clamp_Mul_Temp[RefItem] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_CLAMP_MUL][RefItem], IQS_Filter[ISOIdx].Clamp.Mul, 0, 255, IQS_CAL_INTPL);
        IQS_Filter_Clamp_Th_Temp[RefItem] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_CLAMP_TH][RefItem], IQS_Filter[ISOIdx].Clamp.Th, 0, 4095, IQS_CAL_MULTIPLY);

        // GBalance
        IQS_Gbal_Thr_Temp[RefItem] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_GBAL][RefItem], IQS_Gbal[ISOIdx].GbalThr, 0, 1023, IQS_CAL_INTPL);

        // Sharpness
        IQS_Edge_EnhP_Temp[RefItem] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_EDGE_WHITE][RefItem], IQS_EdgeEnh[ISOIdx].EnhP, 0, 1023, IQS_CAL_MULTIPLY);
        IQS_Edge_EnhN_Temp[RefItem] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_EDGE_BLACK][RefItem], IQS_EdgeEnh[ISOIdx].EnhN, 0, 1023, IQS_CAL_MULTIPLY);

        IQS_Edge_EWDir_Temp[RefItem][0] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_EDGE_THICK][RefItem], IQS_EWParam[ISOIdx].uiEWDir0, 0, 16, IQS_CAL_INTPL);
        IQS_Edge_EWDir_Temp[RefItem][1] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_EDGE_THICK][RefItem], IQS_EWParam[ISOIdx].uiEWDir1, 0, 16, IQS_CAL_INTPL);
        IQS_Edge_EWDir_Temp[RefItem][2] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_EDGE_THICK][RefItem], IQS_EWParam[ISOIdx].uiEWDir2, 0, 16, IQS_CAL_INTPL);

        IQS_Edge_EthrL_Temp[RefItem] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_EDGE_START][RefItem], IQS_EDMap[ISOIdx].EthrL, 0, 256, IQS_CAL_INTPL);
        IQS_Edge_EtabL_Temp[RefItem] = (UINT32)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_EDGE_STEP][RefItem], IQS_EDMap[ISOIdx].EtabL, 0, 7, IQS_CAL_INTPL);

        // Saturation
        IQS_Sat_Temp[RefItem] = (INT16)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_SAT][RefItem], Saturation[ISOIdx], -128, 127, IQS_CAL_INTPL);

        // Contrast
        IQS_Con_Temp[RefItem] = (INT16)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_CONT][RefItem], Contrast[ISOIdx], -128, 127, IQS_CAL_INTPL);

        // LCA
        IQS_LCA_OutUV_Temp[RefItem] = (UINT8)IQS_IQ_Cal(IQS_FinalRatio[SenId_Value][ADJ_IQRatio_LCA][RefItem], IQS_ChromaAdapt[ISOIdx].LcaRefWt.LcaOutUV, 0, 31, IQS_CAL_INTPL);
    }
}

///////////////////
// IQ param interpolation for ISO
///////////////////
void IQS_Intpl_ISO(UINT32 Id, IQS_FLOW_MSG MsgID)
{
    UINT32 IsoValue=0, IsoStart=0, IsoEnd=0, IQIdxL=0, IQIdxH=0;
    UINT32 SenId_Value = IQS_GetSensorIdx(Id);
    IQS_GetISORange(Id, MsgID, &IsoValue, &IsoStart, &IsoEnd, &IQIdxL, &IQIdxH);
    UINT32 ISO_Idx = IQIdxL;

    IQS_RangeB_Buf[SenId_Value]= IQS_RangeB[ISO_Idx];
    IQS_Filter_Buf[SenId_Value]= IQS_Filter[ISO_Idx];
    IQS_Gbal_Buf[SenId_Value]= IQS_Gbal[ISO_Idx];

    IQS_DirScoreTh_Buf[SenId_Value]= IQS_DirScoreTh[ISO_Idx];
    IQS_EWParam_Buf[SenId_Value]= IQS_EWParam[ISO_Idx];
    IQS_EdgeEnh_Buf[SenId_Value]= IQS_EdgeEnh[ISO_Idx];
    IQS_EDMap_Buf[SenId_Value]= IQS_EDMap[ISO_Idx];
    IQS_ESMap_Buf[SenId_Value]= IQS_ESMap[ISO_Idx];
    IQS_RGBLpf_Buf[SenId_Value].R = IQS_RGBLpf[ISO_Idx];

    IQS_RefCenter_Y_Buf[SenId_Value]= IQS_RefCenter_Y[ISO_Idx];
    IQS_RefCenter_UV_Buf[SenId_Value]= IQS_RefCenter_UV[ISO_Idx];
    IQS_IFE2Filter_Buf[SenId_Value] = IQS_IFE2Filter[ISO_Idx];

    IQS_ChromaAdapt_Buf[SenId_Value]  = IQS_ChromaAdapt[ISO_Idx];
    IQS_ChromaAdapt_Lum_Buf[SenId_Value]  = IQS_ChromaAdapt_Lum[ISO_Idx];
    IQS_ChromaAdapt_Color_Buf[SenId_Value]= IQS_ChromaAdapt_Color[ISO_Idx];

// With ratio adjusting
    IQS_CalRatio(SenId_Value,IQIdxL,IQIdxH);
    IQS_CalValue(SenId_Value,IQIdxL,IQIdxH);

    IQS_OBSub_SIE_Buf[SenId_Value] = (UINT16)IQS_Intpl(IsoValue, IQS_OBSub_Temp[0], IQS_OBSub_Temp[1], IsoStart, IsoEnd);

    if ( IQS_OBSubMode(Id, MsgID) == IQS_IFE_OBSUB )
        IQS_OBSub_IFE_Buf[SenId_Value][0] = (UINT32)IQS_OBSub_SIE_Buf[SenId_Value];
    else
        IQS_OBSub_IFE_Buf[SenId_Value][0] = 0;

    IQS_IntplTbl(IQS_Outl_Bri_Temp[0], IQS_Outl_Bri_Temp[1], 5, IQS_Outl_Bri_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_IntplTbl(IQS_Outl_Dark_Temp[0], IQS_Outl_Dark_Temp[1], 5, IQS_Outl_Dark_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);

    IQS_IntplTbl(IQS_RangeA_Thr_Temp[0], IQS_RangeA_Thr_Temp[1], 6, IQS_RangeA_Thr_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_IntplTbl(IQS_RangeB_Thr_Temp[0], IQS_RangeB_Thr_Temp[1], 6, IQS_RangeB_Thr_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_IntplTbl(IQS_RangeA_Lut_Temp[0], IQS_RangeA_Lut_Temp[1], 17, IQS_RangeA_Lut_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_IntplTbl(IQS_RangeB_Lut_Temp[0], IQS_RangeB_Lut_Temp[1], 17, IQS_RangeB_Lut_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_Filter_Buf[SenId_Value].Bilat_w = IQS_Intpl(IsoValue, IQS_Filter_BilatW_Temp[0], IQS_Filter_BilatW_Temp[1], IsoStart, IsoEnd);
    IQS_Filter_Buf[SenId_Value].Rth_W = IQS_Intpl(IsoValue, IQS_Filter_RthW_Temp[0], IQS_Filter_RthW_Temp[1], IsoStart, IsoEnd);
    IQS_Filter_Buf[SenId_Value].Clamp.Mul = IQS_Intpl(IsoValue, IQS_Filter_Clamp_Mul_Temp[0], IQS_Filter_Clamp_Mul_Temp[1], IsoStart, IsoEnd);
    IQS_Filter_Buf[SenId_Value].Clamp.Th = IQS_Intpl(IsoValue, IQS_Filter_Clamp_Th_Temp[0], IQS_Filter_Clamp_Th_Temp[1], IsoStart, IsoEnd);

    IQS_Gbal_Buf[SenId_Value].GbalThr = IQS_Intpl(IsoValue, IQS_Gbal_Thr_Temp[0], IQS_Gbal_Thr_Temp[1], IsoStart, IsoEnd);

    IQS_EdgeEnh_Buf[SenId_Value].EnhP = IQS_Intpl(IsoValue, IQS_Edge_EnhP_Temp[0], IQS_Edge_EnhP_Temp[1], IsoStart, IsoEnd);
    IQS_EdgeEnh_Buf[SenId_Value].EnhN = IQS_Intpl(IsoValue, IQS_Edge_EnhN_Temp[0], IQS_Edge_EnhN_Temp[1], IsoStart, IsoEnd);

    IQS_EWParam_Buf[SenId_Value].uiEWDir0 = IQS_Intpl(IsoValue, IQS_Edge_EWDir_Temp[0][0], IQS_Edge_EWDir_Temp[1][0], IsoStart, IsoEnd);
    IQS_EWParam_Buf[SenId_Value].uiEWDir1 = IQS_Intpl(IsoValue, IQS_Edge_EWDir_Temp[0][1], IQS_Edge_EWDir_Temp[1][1], IsoStart, IsoEnd);
    IQS_EWParam_Buf[SenId_Value].uiEWDir2 = IQS_Intpl(IsoValue, IQS_Edge_EWDir_Temp[0][2], IQS_Edge_EWDir_Temp[1][2], IsoStart, IsoEnd);

    IQS_EDMap_Buf[SenId_Value].EthrL = IQS_Intpl(IsoValue, IQS_Edge_EthrL_Temp[0], IQS_Edge_EthrL_Temp[1], IsoStart, IsoEnd);
    IQS_EDMap_Buf[SenId_Value].EtabL = IQS_Intpl(IsoValue, IQS_Edge_EtabL_Temp[0], IQS_Edge_EtabL_Temp[1], IsoStart, IsoEnd);

    Saturation_Buf[SenId_Value] = IQS_Intpl(IsoValue, IQS_Sat_Temp[0], IQS_Sat_Temp[1], IsoStart, IsoEnd);
    Contrast_Buf[SenId_Value] = IQS_Intpl(IsoValue, IQS_Con_Temp[0], IQS_Con_Temp[1], IsoStart, IsoEnd);

    IQS_ChromaAdapt_Buf[SenId_Value].LcaRefWt.LcaOutUV = IQS_Intpl(IsoValue, IQS_LCA_OutUV_Temp[0], IQS_LCA_OutUV_Temp[1], IsoStart, IsoEnd);

// Without ratio adjusting
    // IFE
    IQS_RangeB_Buf[SenId_Value].Rangeparam.BilatTh1 = IQS_Intpl(IsoValue, IQS_RangeB[IQIdxL].Rangeparam.BilatTh1, IQS_RangeB[IQIdxH].Rangeparam.BilatTh1, IsoStart, IsoEnd);
    IQS_RangeB_Buf[SenId_Value].Rangeparam.BilatTh2 = IQS_Intpl(IsoValue, IQS_RangeB[IQIdxL].Rangeparam.BilatTh2, IQS_RangeB[IQIdxH].Rangeparam.BilatTh2, IsoStart, IsoEnd);

    //IQS_Filter_Buf[SenId_Value].Clamp.Th  = IQS_Intpl(IsoValue, IQS_Filter[IQIdxL].Clamp.Th,  IQS_Filter[IQIdxH].Clamp.Th, IsoStart, IsoEnd);
    //IQS_Filter_Buf[SenId_Value].Clamp.Mul = IQS_Intpl(IsoValue, IQS_Filter[IQIdxL].Clamp.Mul, IQS_Filter[IQIdxH].Clamp.Mul, IsoStart, IsoEnd);
    IQS_Filter_Buf[SenId_Value].Clamp.Dlt = IQS_Intpl(IsoValue, IQS_Filter[IQIdxL].Clamp.Dlt, IQS_Filter[IQIdxH].Clamp.Dlt, IsoStart, IsoEnd);
    IQS_IntplTblUint8(IQS_Gbal_Stab[IQIdxL], IQS_Gbal_Stab[IQIdxH], 16, IQS_Gbal_Stab_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_IntplTblUint8(IQS_Gbal_Dtab[IQIdxL], IQS_Gbal_Dtab[IQIdxH], 16, IQS_Gbal_Dtab_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);

    // IPE
    IQS_DirScoreTh_Buf[SenId_Value].uiScoreTh0 = IQS_Intpl(IsoValue, IQS_DirScoreTh[IQIdxL].uiScoreTh0,  IQS_DirScoreTh[IQIdxH].uiScoreTh0, IsoStart, IsoEnd);
    IQS_DirScoreTh_Buf[SenId_Value].uiScoreTh1 = IQS_Intpl(IsoValue, IQS_DirScoreTh[IQIdxL].uiScoreTh1,  IQS_DirScoreTh[IQIdxH].uiScoreTh1, IsoStart, IsoEnd);
    IQS_DirScoreTh_Buf[SenId_Value].uiScoreTh2 = IQS_Intpl(IsoValue, IQS_DirScoreTh[IQIdxL].uiScoreTh2,  IQS_DirScoreTh[IQIdxH].uiScoreTh2, IsoStart, IsoEnd);
    IQS_DirScoreTh_Buf[SenId_Value].uiScoreTh3 = IQS_Intpl(IsoValue, IQS_DirScoreTh[IQIdxL].uiScoreTh3,  IQS_DirScoreTh[IQIdxH].uiScoreTh3, IsoStart, IsoEnd);

    IQS_EDMap_Buf[SenId_Value].EthrH = IQS_Intpl(IsoValue, IQS_EDMap[IQIdxL].EthrH,  IQS_EDMap[IQIdxH].EthrH, IsoStart, IsoEnd);
    IQS_EDMap_Buf[SenId_Value].EtabH = IQS_Intpl(IsoValue, IQS_EDMap[IQIdxL].EtabH,  IQS_EDMap[IQIdxH].EtabH, IsoStart, IsoEnd);

    IQS_ESMap_Buf[SenId_Value].EthrL = IQS_Intpl(IsoValue, IQS_ESMap[IQIdxL].EthrL,  IQS_ESMap[IQIdxH].EthrL, IsoStart, IsoEnd);
    IQS_ESMap_Buf[SenId_Value].EthrH = IQS_Intpl(IsoValue, IQS_ESMap[IQIdxL].EthrH,  IQS_ESMap[IQIdxH].EthrH, IsoStart, IsoEnd);
    IQS_ESMap_Buf[SenId_Value].EtabL = IQS_Intpl(IsoValue, IQS_ESMap[IQIdxL].EtabL,  IQS_ESMap[IQIdxH].EtabL, IsoStart, IsoEnd);
    IQS_ESMap_Buf[SenId_Value].EtabH = IQS_Intpl(IsoValue, IQS_ESMap[IQIdxL].EtabH,  IQS_ESMap[IQIdxH].EtabH, IsoStart, IsoEnd);

    IQS_IntplTblUint8(IQS_EDTab[IQIdxL], IQS_EDTab[IQIdxH], 16, IQS_EDTab_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_IntplTblUint8(IQS_EStab[IQIdxL], IQS_EStab[IQIdxH], 16, IQS_EStab_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_IntplTblUint8(IQS_FStab[IQIdxL], IQS_FStab[IQIdxH], 16, IQS_FStab_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_IntplTblUint8(IQS_FDtab[IQIdxL], IQS_FDtab[IQIdxH], 16, IQS_FDtab_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);

    IQS_RGBLpf_Buf[SenId_Value].R.lpfw = IQS_Intpl(IsoValue, IQS_RGBLpf[IQIdxL].lpfw, IQS_RGBLpf[IQIdxH].lpfw, IsoStart, IsoEnd);
    IQS_RGBLpf_Buf[SenId_Value].R.rangeth0 = IQS_Intpl(IsoValue, IQS_RGBLpf[IQIdxL].rangeth0, IQS_RGBLpf[IQIdxH].rangeth0, IsoStart, IsoEnd);
    IQS_RGBLpf_Buf[SenId_Value].R.rangeth1 = IQS_Intpl(IsoValue, IQS_RGBLpf[IQIdxL].rangeth1, IQS_RGBLpf[IQIdxH].rangeth1, IsoStart, IsoEnd);

    //#NT#2017/03/13#Wendy Liao -begin
    //#Add WDR maxlevel by ISO index
    //WDR
    IQS_autoWDR_adj[SenId_Value].DarkGainTH.l = IQS_Intpl(IsoValue, AutoWDRLevel[IQIdxL].DarkGainTH.l, AutoWDRLevel[IQIdxH].DarkGainTH.l, IsoStart, IsoEnd);
    IQS_autoWDR_adj[SenId_Value].DarkGainTH.h = IQS_Intpl(IsoValue, AutoWDRLevel[IQIdxL].DarkGainTH.h, AutoWDRLevel[IQIdxH].DarkGainTH.h, IsoStart, IsoEnd);
    IQS_autoWDR_adj[SenId_Value].BrightGainTH.l = IQS_Intpl(IsoValue, AutoWDRLevel[IQIdxL].BrightGainTH.l, AutoWDRLevel[IQIdxH].BrightGainTH.l, IsoStart, IsoEnd);
    IQS_autoWDR_adj[SenId_Value].BrightGainTH.h = IQS_Intpl(IsoValue, AutoWDRLevel[IQIdxL].BrightGainTH.h, AutoWDRLevel[IQIdxH].BrightGainTH.h, IsoStart, IsoEnd);
    IQS_autoWDR_adj[SenId_Value].BlackLevelTH.l = IQS_Intpl(IsoValue, AutoWDRLevel[IQIdxL].BlackLevelTH.l, AutoWDRLevel[IQIdxH].BlackLevelTH.l, IsoStart, IsoEnd);
    IQS_autoWDR_adj[SenId_Value].BlackLevelTH.h = IQS_Intpl(IsoValue, AutoWDRLevel[IQIdxL].BlackLevelTH.h, AutoWDRLevel[IQIdxH].BlackLevelTH.h, IsoStart, IsoEnd);
    IQS_autoWDR_adj[SenId_Value].SharpnessTH.l = IQS_Intpl(IsoValue, AutoWDRLevel[IQIdxL].SharpnessTH.l, AutoWDRLevel[IQIdxH].SharpnessTH.l, IsoStart, IsoEnd);
    IQS_autoWDR_adj[SenId_Value].SharpnessTH.h = IQS_Intpl(IsoValue, AutoWDRLevel[IQIdxL].SharpnessTH.h, AutoWDRLevel[IQIdxH].SharpnessTH.h, IsoStart, IsoEnd);

    IQS_autoWDR_adj[SenId_Value].OverExp_l = IQS_Intpl(IsoValue, AutoWDRLevel[IQIdxL].OverExp_l, AutoWDRLevel[IQIdxH].OverExp_l, IsoStart, IsoEnd);
    IQS_autoWDR_adj[SenId_Value].OverExp_h = IQS_Intpl(IsoValue, AutoWDRLevel[IQIdxL].OverExp_h, AutoWDRLevel[IQIdxH].OverExp_h, IsoStart, IsoEnd);
    //#NT#2017/03/13#Wendy Liao -end

    // IFE2
    IQS_IntplTbl(IQS_RefCenter_YTh[IQIdxL], IQS_RefCenter_YTh[IQIdxH], 3, IQS_RefCenter_YTh_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_IntplTbl(IQS_RefCenter_YWt[IQIdxL], IQS_RefCenter_YWt[IQIdxH], 4, IQS_RefCenter_YWt_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_RefCenter_Y_Buf[SenId_Value].uiOutlDth = IQS_Intpl(IsoValue, IQS_RefCenter_Y[IQIdxL].uiOutlDth, IQS_RefCenter_Y[IQIdxH].uiOutlDth, IsoStart, IsoEnd);
    IQS_IntplTbl(IQS_RefCenter_UVTh[IQIdxL], IQS_RefCenter_UVTh[IQIdxH], 3, IQS_RefCenter_UVTh_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_IntplTbl(IQS_RefCenter_UVWt[IQIdxL], IQS_RefCenter_UVWt[IQIdxH], 4, IQS_RefCenter_UVWt_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_RefCenter_UV_Buf[SenId_Value].uiOutlDth = IQS_Intpl(IsoValue, IQS_RefCenter_UV[IQIdxL].uiOutlDth, IQS_RefCenter_UV[IQIdxH].uiOutlDth, IsoStart, IsoEnd);

    IQS_IntplTbl(IQS_IFE2Filter_YTh[IQIdxL], IQS_IFE2Filter_YTh[IQIdxH], 5, IQS_IFE2Filter_YTh_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_IntplTbl(IQS_IFE2Filter_YWt[IQIdxL], IQS_IFE2Filter_YWt[IQIdxH], 6, IQS_IFE2Filter_YWt_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_IntplTbl(IQS_IFE2Filter_UVTh[IQIdxL], IQS_IFE2Filter_UVTh[IQIdxH], 5, IQS_IFE2Filter_UVTh_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);
    IQS_IntplTbl(IQS_IFE2Filter_UVWt[IQIdxL], IQS_IFE2Filter_UVWt[IQIdxH], 6, IQS_IFE2Filter_UVWt_Buf[SenId_Value], IsoValue, IsoStart, IsoEnd);

    // IME
    UINT32 TwoPower[5] = {1,2,4,8,16};
    IQS_ChromaAdapt_Buf[SenId_Value].LcaRefWt.LcaRefY  = IQS_Intpl(IsoValue, IQS_ChromaAdapt[IQIdxL].LcaRefWt.LcaRefY, IQS_ChromaAdapt[IQIdxH].LcaRefWt.LcaRefY, IsoStart, IsoEnd);
    IQS_ChromaAdapt_Buf[SenId_Value].LcaRefWt.LcaRefUV = IQS_Intpl(IsoValue, IQS_ChromaAdapt[IQIdxL].LcaRefWt.LcaRefUV, IQS_ChromaAdapt[IQIdxH].LcaRefWt.LcaRefUV, IsoStart, IsoEnd);

    IQS_ChromaAdapt_Buf[SenId_Value].LcaYinfo.LcaY      = IQS_Intpl(IsoValue, IQS_ChromaAdapt[IQIdxL].LcaYinfo.LcaY, IQS_ChromaAdapt[IQIdxH].LcaYinfo.LcaY, IsoStart, IsoEnd);
    IQS_ChromaAdapt_Buf[SenId_Value].LcaYinfo.LcaYTh    = IQS_Intpl(IsoValue, IQS_ChromaAdapt[IQIdxL].LcaYinfo.LcaYTh, IQS_ChromaAdapt[IQIdxH].LcaYinfo.LcaYTh, IsoStart, IsoEnd);
    IQS_ChromaAdapt_Buf[SenId_Value].LcaYinfo.LcaYWtPrc = IQS_Intpl(IsoValue, IQS_ChromaAdapt[IQIdxL].LcaYinfo.LcaYWtPrc, IQS_ChromaAdapt[IQIdxH].LcaYinfo.LcaYWtPrc, IsoStart, IsoEnd);
    IQS_ChromaAdapt_Buf[SenId_Value].LcaYinfo.LcaYWtS   = 8 * TwoPower[IQS_ChromaAdapt_Buf[SenId_Value].LcaYinfo.LcaYWtPrc];
    IQS_ChromaAdapt_Buf[SenId_Value].LcaYinfo.LcaYWtE   = IQS_Intpl(IsoValue, IQS_ChromaAdapt[IQIdxL].LcaYinfo.LcaYWtE, IQS_ChromaAdapt[IQIdxH].LcaYinfo.LcaYWtE, IsoStart, IsoEnd);

    IQS_ChromaAdapt_Buf[SenId_Value].LcaUVinfo.LcaUV      = IQS_Intpl(IsoValue, IQS_ChromaAdapt[IQIdxL].LcaUVinfo.LcaUV, IQS_ChromaAdapt[IQIdxH].LcaUVinfo.LcaUV, IsoStart, IsoEnd);
    IQS_ChromaAdapt_Buf[SenId_Value].LcaUVinfo.LcaUVTh    = IQS_Intpl(IsoValue, IQS_ChromaAdapt[IQIdxL].LcaUVinfo.LcaUVTh, IQS_ChromaAdapt[IQIdxH].LcaUVinfo.LcaUVTh, IsoStart, IsoEnd);
    IQS_ChromaAdapt_Buf[SenId_Value].LcaUVinfo.LcaUVWtPrc = IQS_Intpl(IsoValue, IQS_ChromaAdapt[IQIdxL].LcaUVinfo.LcaUVWtPrc, IQS_ChromaAdapt[IQIdxH].LcaUVinfo.LcaUVWtPrc, IsoStart, IsoEnd);
    IQS_ChromaAdapt_Buf[SenId_Value].LcaUVinfo.LcaUVWtS   = 8 * TwoPower[IQS_ChromaAdapt_Buf[SenId_Value].LcaUVinfo.LcaUVWtPrc];
    IQS_ChromaAdapt_Buf[SenId_Value].LcaUVinfo.LcaUVWtE   = IQS_Intpl(IsoValue, IQS_ChromaAdapt[IQIdxL].LcaUVinfo.LcaUVWtE, IQS_ChromaAdapt[IQIdxH].LcaUVinfo.LcaUVWtE, IsoStart, IsoEnd);
}

const char* ADJIQ_ITEM_Strings[] =
{
    "ADJIQ_OB",
    "ADJIQ_OUTL_BRI",
    "ADJIQ_OUTL_DARK",
    "ADJIQ_NR",
    "ADJIQ_NR_FP",
    "ADJIQ_NR_SPATIAL",
    "ADJIQ_GBAL",
    "ADJIQ_EDGE_WHITE",
    "ADJIQ_EDGE_BLACK",
    "ADJIQ_EDGE_THICK",
    "ADJIQ_EDGE_START",
    "ADJIQ_EDGE_STEP",
    "ADJIQ_SAT",
    "ADJIQ_CONT",
    "ADJIQ_LCA",
    "ADJIQ_CLAMP_MUL",
    "ADJIQ_CLAMP_TH",
};

extern void IQUCLOG_OPEN(char *pFilename);
extern void IQUCLOG(char *format, ...);
extern void IQUCLOG_CLOSE(void);

UINT32 IQUC_DumpRatioTable(UINT32 Id)
{
#if IQS_RATIOTABLE_ENABLE
    extern INT32 IQS_RatioTable[ADJ_IQRatio_MAX_CNT][Total_ISONum];

    UINT32 i, j;
    IQUCLOG_OPEN("A:\\IQS_TuningIQTable.txt");

// IQS_RatioTable
    IQUCLOG("\r\n================IQS_TuningIQTable_xxxx.c================\r\n\r\n");
    IQUCLOG("INT32 IQS_RatioTable[ADJIQ_MAX_CNT][Total_ISONum] =\r\n");
    IQUCLOG("{\r\n");
    for(i=0; i<ADJ_IQRatio_MAX_CNT; i++)
    {
        IQUCLOG("    {   // %s\r\n    ", ADJIQ_ITEM_Strings[i]);
        for(j=0; j<Total_ISONum; j++)
        {
            IQUCLOG("%d, ", IQS_RatioTable[i][j]);
            if     (j == IQS_PRV1_ISOEND) IQUCLOG("    // PRV1\r\n    ");
            else if(j == IQS_CAP1_ISOEND) IQUCLOG("    // CAP1\r\n    ");
            else if(j == IQS_PRV2_ISOEND) IQUCLOG("                  // PRV2\r\n    ");
            else if(j == IQS_CAP2_ISOEND) IQUCLOG("                  // CAP2\r\n    ");
        }
        IQUCLOG("},\r\n", ADJIQ_ITEM_Strings[i]);
    }
    IQUCLOG("};\r\n\r\n");

// IQS_CAC_Table

    IQUCLOG("INT32 IQS_CAC_Table[Total_SENIDNum][ADJIQ_CAC_MAX_CNT] =\r\n");
    IQUCLOG("{\r\n// ADJIQ_CAC_R,ADJIQ_CAC_B\r\n");
    IQUCLOG("{%d,            %d},          // SENID1\r\n", IQS_CAC_Table[0][ADJ_IQRatio_CAC_R], IQS_CAC_Table[0][ADJ_IQRatio_CAC_B]);
    IQUCLOG("{%d,            %d},          // SENID2\r\n", IQS_CAC_Table[1][ADJ_IQRatio_CAC_R], IQS_CAC_Table[1][ADJ_IQRatio_CAC_B]);
    IQUCLOG("};\r\n\r\n");

    IQUCLOG("========================================================\r\n\r\n");
    IQUCLOG_CLOSE();

    return E_OK;
#endif
    return E_SYS;
}
#endif
