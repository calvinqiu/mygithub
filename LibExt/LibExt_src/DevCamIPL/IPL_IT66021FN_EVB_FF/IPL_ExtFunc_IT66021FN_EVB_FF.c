/**
    ISR sample.

    @file       IPL_ExtFunc_IT66021FN_EVB_FF.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "IPL_IT66021FN_EVB_FF_Int.h"

static UINT16 gWdrY1[WDR_PROC_CNT][32 * 32] = {0};
static UINT16 gWdrY2[WDR_PROC_CNT][32 * 32] = {0};
static UINT16 gWdrHistBuf[WDR_PROC_CNT][64] = {0};
static UINT32 gWdrYCurve[WDR_PROC_CNT][130] = {0};
static UINT32 gWdrEdgeCurve[WDR_PROC_CNT][65] = {0};
static INT16  gWdrColorSat[WDR_PROC_CNT] = {0};
//#NT#2016/05/04#Jarkko Chang -begin
//#NT# Add support wdr2.5
static UINT32 gWdrBandGain[WDR_PROC_CNT][2] = {0};
//#NT#2016/05/04#Jarkko Chang -end
//#NT#2016/03/23#Jarkko Chang -begin
//#NT# add support different wdr trigger interval
UINT32 gWdrStep[WDR_PROC_CNT] = {0};
UINT32 TrigInt[WDR_PROC_CNT] = {1, 1};
BOOL Reset_Flg[WDR_PROC_CNT] = {TRUE, TRUE};
//#NT#2017/01/06#Jarkko Chang -begin
//#NT# modify for wdr_defog
UINT32 WDRMapId[WDR_PROC_CNT] = {SR_WDR_PROCID1, SR_WDR_PROCID2};
//#NT#2017/01/06#Jarkko Chang -end
//#NT#2017/03/02#Wendy Liao -begin
//#NT# modify for pre_WDR_mode
static SEL_WDR pre_WDR_mode[WDR_PROC_CNT] = {SEL_WDR_OFF};
//#NT#2017/03/02#Wendy Liao -end
//#NT#2017/03/13#Wendy Liao -begin
//#NT# add WDR auto mode
WDR_AUTO_MODE WDR_auto_mode[WDR_PROC_CNT] = {WDR_AUTO_LV, WDR_AUTO_LV};
AUTO_WDR_SET IQS_WDR_Set[IPL_ID_MAX_NUM] = {0};
//#NT#2017/03/13#Wendy Liao -end

WDR_INTERVAL_PARAM gTrigInterv[] =
{
    {SEL_VIDEOFPS_30,       2},
    {SEL_VIDEOFPS_60,       4},
    {SEL_VIDEOFPS_120,      8},
    {SEL_VIDEOFPS_MAX_CNT,  8},
    {WDR_TAB_END,           2}
};
//#NT#2016/03/23#Jarkko Chang -end

//#NT#2016/12/01#Jarkko Chang -begin
//#NT# update wdr/shdr re-init issue
void ExtFunc_Reset(UINT32 Id)
{
	Reset_Flg[Id] = TRUE;
}

void ExtFunc_Init(UINT32 Id, ENG_SUBFUNC *Info, IPL_HAL_GROUP* group)
{
	SHDR_PARAM shdr_param;

    if (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR)
    {
        SHDR_OPEN_INFO HdrOpenInfo;
        UINT32 IQSMapId[WDR_PROC_CNT] = {SENID1, SENID2};

        HdrOpenInfo.OperationMode = SHDR_STAGGERED_MODE;
        HdrOpenInfo.puiRGBGammaLut = &IQS_GammaLut[IQSMapId[Id]][0];
		if (Reset_Flg[Id] == TRUE)
		{
			Reset_Flg[Id] = FALSE;
	        SHDRCtrl_Init(&HdrOpenInfo);
		}

		SensorHDR_GetParam(&shdr_param);
        if (Info != NULL && group != NULL)
        {
		    Info->pRhe_sub->HdrRadianceInfo.Weight_SoruceSel = shdr_param.RadInfo.RadWgtSrc;
		    Info->pRhe_sub->HdrRadianceInfo.uiIn0EvGap = shdr_param.RadInfo.uiIn0EvGap;
		    Info->pRhe_sub->HdrRadianceInfo.uiIn1EvGap = shdr_param.RadInfo.uiIn1EvGap;
		    Info->pRhe_sub->HdrRadianceInfo.puiWeightTbl = &shdr_param.RadInfo.uiWgtTbl[0];

		    Info->pRhe_sub->HdrRefLaLowPassfilterInfo.uiReflaLpfNormM = shdr_param.RefLaLpfInfo.uiLpfNormM;
		    Info->pRhe_sub->HdrRefLaLowPassfilterInfo.uiReflaLpfNormS = shdr_param.RefLaLpfInfo.uiLpfNormS;
		    Info->pRhe_sub->HdrRefLaLowPassfilterInfo.puiReflaLpf = &shdr_param.RefLaLpfInfo.uiLpf[0];

            Info->pRhe_sub->HdrGlobalToneInfo.puiGblToneTbl = &shdr_param.GblTnInfo.uiGblTnTbl[0];
		    Info->pRhe_sub->HdrLocalToneInfo.uiBand0Gain = shdr_param.LocTnInfo.uiBand0Gain;
		    Info->pRhe_sub->HdrLocalToneInfo.uiBand1Gain = shdr_param.LocTnInfo.uiBand1Gain;

		    Info->pRhe_sub->HdrRegulationInfo.puiDataARto = &shdr_param.ReguInfo.uiARto[0];
		    Info->pRhe_sub->HdrRegulationInfo.puiDataAMax = &shdr_param.ReguInfo.uiAMax[0];
		    Info->pRhe_sub->HdrRegulationInfo.puiDataAMin = &shdr_param.ReguInfo.uiAMin[0];
		    Info->pRhe_sub->HdrRegulationInfo.puiDataBMax = &shdr_param.ReguInfo.uiBMax[0];
		    Info->pRhe_sub->HdrRegulationInfo.puiDataBMin = &shdr_param.ReguInfo.uiBMin[0];

		    Info->pRhe_sub->HdrRecurvenInfo.puiPointX = &shdr_param.ReCrvInfo.uiPointX[0];
		    Info->pRhe_sub->HdrRecurvenInfo.puiPointY = &shdr_param.ReCrvInfo.uiPointY[0];
		    Info->pRhe_sub->HdrRecurvenInfo.puiSlopeM = &shdr_param.ReCrvInfo.uiSlopeM[0];
		    Info->pRhe_sub->HdrRecurvenInfo.puiSlopeS = &shdr_param.ReCrvInfo.uiSlopeS[0];

            group->rheUpdate |= SHDRCtrl_GetUpdateBits();
        }
    }
    else
    {
        SR_WDR_OPEN_INFO wdrInfo;
		SR_WDR_PARAM 	 wdr_param;
        UINT32 IQSMapId[WDR_PROC_CNT] = {SENID1, SENID2};
        SR_WDR_CHG_PARAMS ChgParams = {0};

		//#NT#2017/03/02#Wendy Liao -begin
		//#NT# modify for pre_WDR_mode
        if( pre_WDR_mode[Id] != IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR) )
            ExtFunc_Reset(Id);
		//#NT#2017/03/02#Wendy Liao -end

        wdrInfo.puiRGBGammaLut = &IQS_GammaLut[IQSMapId[Id]][0];

        //#NT#2016/05/04#Jarkko Chang -begin
        //#NT# Add support wdr2.5
        //#NT#2016/04/08#Jarkko Chang -begin
        //#NT# fixed miss obmap update bit issue
        //#NT#2016/04/08#Jarkko Chang -end
        ChgParams.puiLumaWeightMap = &IQS_WDR_LumaMapingLut[Id][0];
        //#NT#2016/02/22#Jarkko Chang -begin
        //#NT# clear warning msg
        ChgParams.piContratOBMap = &IQS_WDR_ContratOBMap[Id][0];
        //#NT#2016/02/22#Jarkko Chang -end
        //#NT#2016/08/09#Jarkko Chang -begin
        //#NT# update for new version wdr library
        if (IPL_CtrlGetInfor(Id, IPLCTRL_WDR_VER) == WDR_VERSION_2_5)
        {
            wdrInfo.WdrProcSel = SR_WDR_PROC_25;
			if (Reset_Flg[Id] == TRUE)
			{
				//#NT#2017/03/13#Wendy Liao -begin
				//#NT# add DEFOG/WDR init setting
				if(IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR) == SEL_WDR_DEFOG)
				{
					wdrInfo.DefogProcEn = SR_DEFOG_PROC_ON;
				}
				else if( (IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR) == SEL_WDR_ON)||(IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR) == SEL_WDR_AUTO) )
				{
					wdrInfo.DefogProcEn = SR_DEFOG_PROC_OFF;
				}else
				{
                    wdrInfo.DefogProcEn = SR_DEFOG_PROC_OFF;
				}
				//#NT#2017/03/13#Wendy Liao -end

				Reset_Flg[Id] = FALSE;
            	SceneRender_WDR_Init(WDRMapId[Id], &wdrInfo);
			}
            //#NT#2017/01/06#Jarkko Chang -begin
            //#NT# modify for wdr_defog
            if(IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR) == SEL_WDR_DEFOG)
            {
				//#NT#2017/03/13#Wendy Liao -begin
				//#NT# add DEFOG/WDR init setting
                ChgParams.uiChgSel = SR_WDR_CHG_LUMA_DARK_LEVEL | SR_WDR_CHG_LUMA_BRIGHT_LEVEL | SR_WDR_CHG_BLACK_LEVEL | SR_WDR_CHG_EDGE_LEVEL | SR_WDR_CHG_LOCAL_GAIN;
                ChgParams.uiDarkLumaLevel = 150;
                ChgParams.uiBrightLumaLevel = 250;
                ChgParams.uiBlackLevel = SR_WDR_BLACK_LEVEL00;
                ChgParams.uiEdgeLevel = SR_WDR_EDGE_LEVEL00;
                ChgParams.puiLocalGain = &IQS_WDR_LocalGain[Id][0];
				//#NT#2017/03/13#Wendy Liao -end
            }
            else if( (IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR) == SEL_WDR_ON)||(IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR) == SEL_WDR_AUTO) )
            {
				//#NT#2017/03/13#Wendy Liao -begin
				//#NT# add DEFOG/WDR init setting
                ChgParams.uiChgSel = SR_WDR_CHG_LUMA_DARK_LEVEL | SR_WDR_CHG_LUMA_BRIGHT_LEVEL | SR_WDR_CHG_BLACK_LEVEL | SR_WDR_CHG_EDGE_LEVEL | SR_WDR_CHG_LOCAL_GAIN;
                ChgParams.uiDarkLumaLevel = 250;	//same with WDR level 50
                ChgParams.uiBrightLumaLevel = 105;
                ChgParams.uiBlackLevel= SR_WDR_BLACK_LEVEL00;
                ChgParams.uiEdgeLevel = SR_WDR_EDGE_LEVEL00;
                ChgParams.puiLocalGain = &IQS_WDR_LocalGain[Id][0];
				//#NT#2017/03/13#Wendy Liao -end
            }
            //#NT#2017/01/06#Jarkko Chang -end
            IQS_setWDR(TRUE);
        }
        else
        {
            wdrInfo.WdrProcSel = SR_WDR_PROC_20;
            wdrInfo.DefogProcEn = SR_DEFOG_PROC_OFF;    //no defog function in WDR2.0
			if (Reset_Flg[Id] == TRUE)
			{
				Reset_Flg[Id] = FALSE;
	            SceneRender_WDR_Init(WDRMapId[Id], &wdrInfo);
			}

            ChgParams.uiChgSel = SR_WDR_CHG_LUMA_WEIGHT_MAP|SR_WDR_CHG_CONTRAST_OB_MAP;
            ChgParams.puiLocalGain = NULL;   //for wdr2.0
            IQS_setWDR(FALSE);
        }
        //#NT#2016/08/09#Jarkko Chang -end
        //#NT#2016/05/04#Jarkko Chang -end
        SceneRender_WDR_ChgParams(WDRMapId[Id], &ChgParams);

        if (Info != NULL && group != NULL)
		{
            wdr_param.puiGetToneCurve = &gWdrYCurve[Id][0];
            wdr_param.puiGetEdgeCurve = &gWdrEdgeCurve[Id][0];
            wdr_param.piGetSat = &gWdrColorSat[Id];
            wdr_param.puiGetBandGain = &gWdrBandGain[Id][0];
			SceneRender_WDR_GetParam(WDRMapId[Id], &wdr_param);
			Info->pRhe_sub->HdrLocalToneInfo.uiBand0Gain = gWdrBandGain[Id][0];
			Info->pRhe_sub->HdrLocalToneInfo.uiBand1Gain = gWdrBandGain[Id][1];
		}
		//#NT#2017/03/02#Wendy Liao -begin
		//#NT# modify for pre_WDR_mode
        pre_WDR_mode[Id] = IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR);
		//#NT#2017/03/02#Wendy Liao -end
    }
}
//#NT#2016/12/01#Jarkko Chang -end

void ExtFunc_Process(UINT32 Id)
{
    if (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR)
    {
        IPC_QINFO Info;
        IPL_LA_INFOR LAInfo;
        SHDR_CAL_INFO HdrCalInfo;

        Info = IPL_CTRLQuery(IPL_ID_1, ICF_FRAME_CNT);
        LAInfo.Id = IPL_ID_1;
        LAInfo.Lum1 = &gWdrY1[IPL_ID_1][0];
        LAInfo.Lum2 = &gWdrY2[IPL_ID_1][0];
        LAInfo.Histo = &gWdrHistBuf[IPL_ID_1][0];
        IPL_GetCmd(IPL_GET_CUR_LA_INFOR, (void *)&LAInfo);

        LAInfo.Id = IPL_ID_2;
        LAInfo.Lum1 = &gWdrY1[IPL_ID_2][0];
        LAInfo.Lum2 = &gWdrY2[IPL_ID_2][0];
        LAInfo.Histo = &gWdrHistBuf[IPL_ID_2][0];
        IPL_GetCmd(IPL_GET_CUR_LA_INFOR, (void *)&LAInfo);

        HdrCalInfo.puiCurDarkLA32x32 = &gWdrY1[IPL_ID_1][0];
        HdrCalInfo.puiCurBrightLA32x32 = &gWdrY1[IPL_ID_2][0];
        HdrCalInfo.puiAeMatrix8x8 = NULL;    ///< 8x8 AE matrix
        HdrCalInfo.bAeMatrixUpdate = FALSE;        ///< AE matrix update, TRUE: enable update / FALSE: disable update
        HdrCalInfo.uiGlobalToneGain = AE_GetGTGain(Id); ///< global tone gain;  1x = 1024;
        HdrCalInfo.uiMaxTagLum = 255;
        HdrCalInfo.uiContrastGain = 0;
        HdrCalInfo.uiLumaUprBnd = 3968;
        HdrCalInfo.uiLumaLorBnd = 128;
        HdrCalInfo.uiWt = 16;

        SHDRCtrl_Proc(&HdrCalInfo, Info.FrameCnt);
    }
	//#NT#2017/01/06#Jarkko Chang -begin
    //#NT# modify for wdr_defog
    else if ((IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_WDR) && (IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR) == SEL_WDR_ON || IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR) == SEL_WDR_AUTO || IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR) == SEL_WDR_DEFOG))
	//#NT#2017/01/06#Jarkko Chang -end
    {
        SR_WDR_CAL_INFO wdrInfo;
        IPL_LA_INFOR LAInfo;
        AE_BOUNDARY LvBound;
        SR_WDR_AE_LV_INFO AeLVInfo;
        SR_WDR_PARAM GetWdrParams;
        SR_WDR_CHG_PARAMS ChgParams;

        //#NT#2016/03/23#Jarkko Chang -begin
        //#NT# add support different wdr trigger interval
        if (gWdrStep[Id] == 0)
        {
            LAInfo.Id = (IPL_PROC_ID)Id;
            LAInfo.Lum1 = &gWdrY1[Id][0];
            LAInfo.Lum2 = &gWdrY2[Id][0];
            LAInfo.Histo = &gWdrHistBuf[Id][0];
            IPL_GetCmd(IPL_GET_CUR_LA_INFOR, (void *)&LAInfo);

            LvBound = AE_GetLVBoundary(Id);
            AeLVInfo.uiAeLvCur = AE_GetPrvLVValue(Id);
            AeLVInfo.uiAeLvMax = LvBound.h;
            AeLVInfo.uiAeLvMin = LvBound.l;

            wdrInfo.puiLA32x32 = &gWdrY1[Id][0];
            wdrInfo.pAeLvInfo = &AeLVInfo;

            GetWdrParams.puiGetToneCurve = &gWdrYCurve[Id][0];
            GetWdrParams.puiGetEdgeCurve = &gWdrEdgeCurve[Id][0];
            GetWdrParams.piGetSat = &gWdrColorSat[Id];
            //#NT#2016/05/04#Jarkko Chang -begin
            //#NT# Add support wdr2.5
            GetWdrParams.puiGetBandGain = &gWdrBandGain[Id][0];
            //#NT#2016/05/04#Jarkko Chang -end

            //#NT#2016/04/08#Jarkko Chang -begin
            //#NT# add for wdr library need video fps and trigger interval information
            wdrInfo.uiVdoFrameRate = ExtFunc_getVideoFps(Id, IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_VIDEO_FPS));
            TrigInt[Id] = ExtFunc_getTrigInterval(Id, IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_VIDEO_FPS));
            wdrInfo.uiCalFrameRate = TrigInt[Id];
            //#NT#2016/04/08#Jarkko Chang -end

			//#NT#2017/03/13#Wendy Liao -begin
			//#NT# add WDR auto mode setting
			if(IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR) == SEL_WDR_AUTO)
			{
                if( WDR_auto_mode[Id]==WDR_AUTO_EXP )
                {
                    UINT32 OverExpAdj = 0;
                    OverExpAdj = AE_GetPrvOverExpAdjValue(Id);

                    if(IQS_autoWDR_adj[Id].OverExp_l >= IQS_autoWDR_adj[Id].OverExp_h)
                    {
                        DBG_ERR("Wrong WDR overexposure srttings!! min=%d,max=%d\r\n",IQS_autoWDR_adj[Id].OverExp_l,IQS_autoWDR_adj[Id].OverExp_h);
                        IQS_WDR_Set[Id].DarkGain = IQS_autoWDR_adj[Id].DarkGainTH.l;
                        IQS_WDR_Set[Id].BrightGain = IQS_autoWDR_adj[Id].BrightGainTH.l;
                        IQS_WDR_Set[Id].BlackLevel = IQS_autoWDR_adj[Id].BlackLevelTH.l;
                        IQS_WDR_Set[Id].Sharpness = IQS_autoWDR_adj[Id].SharpnessTH.l;
                    }
                    else
                    {
                        IQS_WDR_Set[Id].DarkGain = IQS_Intpl(OverExpAdj, (int)IQS_autoWDR_adj[Id].DarkGainTH.l, (int)IQS_autoWDR_adj[Id].DarkGainTH.h, (int)IQS_autoWDR_adj[Id].OverExp_l, (int)IQS_autoWDR_adj[Id].OverExp_h);
                        IQS_WDR_Set[Id].BrightGain = IQS_Intpl(OverExpAdj, (int)IQS_autoWDR_adj[Id].BrightGainTH.l, (int)IQS_autoWDR_adj[Id].BrightGainTH.h, (int)IQS_autoWDR_adj[Id].OverExp_l, (int)IQS_autoWDR_adj[Id].OverExp_h);
                        IQS_WDR_Set[Id].BlackLevel = IQS_Intpl(OverExpAdj, (int)IQS_autoWDR_adj[Id].BlackLevelTH.l, (int)IQS_autoWDR_adj[Id].BlackLevelTH.h, (int)IQS_autoWDR_adj[Id].OverExp_l, (int)IQS_autoWDR_adj[Id].OverExp_h);
                        IQS_WDR_Set[Id].Sharpness = IQS_Intpl(OverExpAdj, (int)IQS_autoWDR_adj[Id].SharpnessTH.l, (int)IQS_autoWDR_adj[Id].SharpnessTH.h, (int)IQS_autoWDR_adj[Id].OverExp_l, (int)IQS_autoWDR_adj[Id].OverExp_h);
                    }
                }
                else if( WDR_auto_mode[Id]==WDR_AUTO_LV )
                {
                    IQS_WDR_Set[Id].DarkGain = IQS_WDR_DarkGainMap[Id][AeLVInfo.uiAeLvCur];
                    IQS_WDR_Set[Id].BrightGain = IQS_WDR_BrightGainMap[Id][AeLVInfo.uiAeLvCur];
                    IQS_WDR_Set[Id].BlackLevel = (SR_WDR_BLACK_LEVEL)IQS_WDR_BlackLevelMap[Id][SceneRender_WDR_GetHistMinBin((SR_WDR_PROCID)Id)];
                    IQS_WDR_Set[Id].Sharpness = (SR_WDR_EDGE_LEVEL)IQS_WDR_SharpnessMap[Id][AeLVInfo.uiAeLvCur];
                }

                ChgParams.uiChgSel = SR_WDR_CHG_LUMA_DARK_LEVEL | SR_WDR_CHG_LUMA_BRIGHT_LEVEL | SR_WDR_CHG_BLACK_LEVEL | SR_WDR_CHG_EDGE_LEVEL;
        	    ChgParams.uiDarkLumaLevel   = IQS_WDR_Set[Id].DarkGain;
                ChgParams.uiBrightLumaLevel = IQS_WDR_Set[Id].BrightGain;
        	    ChgParams.uiBlackLevel      = (SR_WDR_BLACK_LEVEL)IQS_WDR_Set[Id].BlackLevel;
        	    ChgParams.uiEdgeLevel       = (SR_WDR_EDGE_LEVEL)IQS_WDR_Set[Id].Sharpness;
        	    SceneRender_WDR_ChgParams(WDRMapId[Id], &ChgParams);
			}
			//#NT#2017/03/13#Wendy Liao -end

            SceneRender_WDR_Proc(WDRMapId[Id], &wdrInfo);
            SceneRender_WDR_GetParam(WDRMapId[Id], &GetWdrParams);
            IQS_SettingFlowFunc(WDRMapId[Id], IQS_WDR_END);
        }

        gWdrStep[Id] = (gWdrStep[Id]+1)%TrigInt[Id];
        //#NT#2016/03/23#Jarkko Chang -end
    }
}

//#NT#2016/04/08#Jarkko Chang -begin
//#NT# add for wdr library need video fps information
UINT32 ExtFunc_getVideoFps(UINT32 Id, UINT32 fps)
{
    static UINT32 prvfps[WDR_PROC_CNT] = {SEL_VIDEOFPS_MAX_CNT, SEL_VIDEOFPS_MAX_CNT};
    static UINT32 videofps = 30;
    IPL_MODE_INFOR Info = {0};
    ER rt;

    Info.Id = Id;
    rt = IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);

    if (Info.Mode == IPL_MODE_VIDEOREC)
    {
        if (prvfps[Id] != fps)
        {
            prvfps[Id] = fps;

            switch(fps)
            {
                    case SEL_VIDEOFPS_30:
                        videofps = 30;
                    break;

                    case SEL_VIDEOFPS_60:
                        videofps = 60;
                    break;

                    case SEL_VIDEOFPS_120:
                        videofps = 120;
                    break;

                    case SEL_VIDEOFPS_240:
                        videofps = 240;
                    break;

                    default:
                        videofps = 30;
                    break;
            }
        }

        return videofps;
    }
    else
    {
        return 30;
    }
}
//#NT#2016/04/08#Jarkko Chang -end

//#NT#2016/03/23#Jarkko Chang -begin
//#NT# add support different wdr trigger interval
UINT32 ExtFunc_getTrigInterval(UINT32 Id, UINT32 fps)
{
    static UINT32 prvfps[WDR_PROC_CNT] = {SEL_VIDEOFPS_MAX_CNT, SEL_VIDEOFPS_MAX_CNT};
    static UINT32 trigInt = 1;
    UINT32 i;
    IPL_MODE_INFOR Info = {0};
    ER rt;

    Info.Id = Id;
    rt = IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);

    if (Info.Mode == IPL_MODE_VIDEOREC)
    {
        if (prvfps[Id] != fps)
        {
            prvfps[Id] = fps;

            for (i = 0; gTrigInterv[i].VideoFps != WDR_TAB_END; i ++)
            {
                if (gTrigInterv[i].VideoFps >= fps)
                {
                    trigInt = gTrigInterv[i].TrigIntervel;
                    break;
                }
            }
        }

        return trigInt;
    }
    else
    {
        return gTrigInterv[0].TrigIntervel;
    }
}
//#NT#2016/03/23#Jarkko Chang -end

SR_WDR_PARAM ExtFunc_GetWDRResult(UINT32 Id)
{
    SR_WDR_PARAM WdrParams = {0};

    WdrParams.puiGetToneCurve = &gWdrYCurve[Id][0];
    WdrParams.puiGetEdgeCurve = &gWdrEdgeCurve[Id][0];
    WdrParams.piGetSat = &gWdrColorSat[Id];
    //#NT#2016/05/04#Jarkko Chang -begin
    //#NT# Add support wdr2.5
    WdrParams.puiGetBandGain = &gWdrBandGain[Id][0];
    //#NT#2016/05/04#Jarkko Chang -end

    return WdrParams;
}

void ExtFunc_SetWDRLumaLevel(UINT32 Id, UINT32 Level)
{
    //#NT#2016/10/31#Wendy Liao -begin
    //Add WDR2.5 settings
    if(IPL_CtrlGetInfor(Id, IPLCTRL_WDR_VER) == WDR_VERSION_2_5)
    {
        //#NT#2017/01/06#Jarkko Chang -begin
        //#NT# modify for wdr_defog
        SR_WDR_CHG_PARAMS ChgParams = {0};

		//#NT#2017/03/13#Wendy Liao -begin
		//#NT# add WDR auto mode setting
        if(IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR) == SEL_WDR_DEFOG)
        {
            IQS_WDR_Set[Id].DarkGain = 150;
            IQS_WDR_Set[Id].BrightGain = 250;
            ChgParams.uiChgSel = SR_WDR_CHG_LUMA_DARK_LEVEL | SR_WDR_CHG_LUMA_BRIGHT_LEVEL | SR_WDR_CHG_BLACK_LEVEL | SR_WDR_CHG_EDGE_LEVEL;
    	    ChgParams.uiDarkLumaLevel   = IQS_WDR_Set[Id].DarkGain;
            ChgParams.uiBrightLumaLevel = IQS_WDR_Set[Id].BrightGain;
            ChgParams.uiBlackLevel = SR_WDR_BLACK_LEVEL00;
            ChgParams.uiEdgeLevel = SR_WDR_EDGE_LEVEL00;
            SceneRender_WDR_ChgParams(WDRMapId[Id], &ChgParams);
        }
        else if( IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WDR) == SEL_WDR_ON )
        {
            UINT32 tmpLevel = 0;

            tmpLevel = Level / 10;
            tmpLevel = tmpLevel*3 + 100;

            IQS_WDR_Set[Id].DarkGain = tmpLevel;
            IQS_WDR_Set[Id].BrightGain = 105;
            ChgParams.uiChgSel = SR_WDR_CHG_LUMA_DARK_LEVEL | SR_WDR_CHG_LUMA_BRIGHT_LEVEL | SR_WDR_CHG_BLACK_LEVEL | SR_WDR_CHG_EDGE_LEVEL;
    	    ChgParams.uiDarkLumaLevel   = IQS_WDR_Set[Id].DarkGain;
            ChgParams.uiBrightLumaLevel = IQS_WDR_Set[Id].BrightGain;
            ChgParams.uiBlackLevel = SR_WDR_BLACK_LEVEL00;
            ChgParams.uiEdgeLevel = SR_WDR_EDGE_LEVEL00;
            SceneRender_WDR_ChgParams(WDRMapId[Id], &ChgParams);
        }
		//#NT#2017/03/13#Wendy Liao -end
        //#NT#2017/01/06#Jarkko Chang -end
    }
    else    //WDR2.0
    //#NT#2016/10/31#Wendy Liao -end
    {
        SR_WDR_CHG_PARAMS ChgParams = {0};

        ChgParams.uiChgSel = SR_WDR_CHG_LUMA_BRIGHT|SR_WDR_CHG_LUMA_DARK;
        ChgParams.uiBrightLumaRatio = 1024 - Level;
        ChgParams.uiDarkLumaRatio = Level;
        SceneRender_WDR_ChgParams(WDRMapId[Id], &ChgParams);
    }
}

void ExtFunc_SetWDRContrastLevel(UINT32 Id, UINT32 ContrastLevel)
{
    SR_WDR_CHG_PARAMS ChgParams = {0};

    ChgParams.uiChgSel = SR_WDR_CHG_CONTRAST_LEVEL;
    ChgParams.uiContrastLevel = ContrastLevel;
    SceneRender_WDR_ChgParams(WDRMapId[Id], &ChgParams);
}
