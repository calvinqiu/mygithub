#ifndef _IQS_SETTINGFLOW_C
#define _IQS_SETTINGFLOW_C
#include "IPL_IQSIM_EVB_FF_Int.h"
#if CAL_ECS
#include "IPL_Cal_DataPassing_IQSIM_EVB_FF_Int.h"
#endif

UINT32 IQS_DCE_GLUT_Buf[Total_SENIDNum][65];
INT32 IQS_DCE_RLUT_Buf[Total_SENIDNum][65];
INT32 IQS_DCE_BLUT_Buf[Total_SENIDNum][65];
IPE_EDGEENH IQS_EdgeEnh_Buf[Total_SENIDNum] ;
UINT32 IQS_TBL_UpdateBit[Total_SENIDNum] = {0}, IQS_ENG_UpdateBit[Total_SENIDNum] = {0};


///////////////////////////////////////////////
// Body
///////////////////////////////////////////////
UINT32 IQS_GetISOIdx(UINT32 Id, IQS_FLOW_MSG MsgID)
{
    return CAP1;
}

static ER SIEx_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, SIE_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    IQSIM_CHINFO *ChInfo = (IQSIM_CHINFO *)IQSIM_GetChInfo(MsgID);
    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
        case IQS_AUTO_TRIG:
        case IQCB_PRV_SIE_INIT:
        case IQCB_PREIME_D2D_SIE_PRI:

            //enable function
            subf->sieFuncEn |= ChInfo->SIE_FuncEn;;


            //Setting parameter
            group->sieUpdate |= (
                                    SIE_SUBFEN          |   ///< update func enable/disable
                                    SIE_OBOFS_          |   ///< update ob offset
                                    SIE_ECS_            |   ///< update ECS/including ECS table addr
                                    0
                                 );

            ////////////////////
            // OB
            ////////////////////
            subf->OBSub = ChInfo->IQS_OBSub;

             ////////////////////
            // ECS
            ////////////////////
            subf->Ecs.EcsMap = ChInfo->IQS_ECSMap;
            subf->Ecs.Inaddr = (UINT32)ChInfo->IQS_ECSTab;
            subf->Ecs.pecs   = &ChInfo->IQS_ECSSet;
            break;
        case IQS_AWB_END:
        case IQS_OZOOM_END:
        case IQS_DZOOM_END:
        case IQCB_PREIME_D2D_SIE_SUB:
        default :
            DBG_ERR("SIE_IQparam() param mode(%d) selection error!\r\n",MsgID);
            return E_SYS;
    }
    return E_OK;
}


static ER DCE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, DCE_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    IQSIM_CHINFO *ChInfo = (IQSIM_CHINFO *)IQSIM_GetChInfo(MsgID);
    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
        case IQS_AUTO_TRIG:
        case IQCB_PRV_DCE_INIT:
        case IQCB_PREIME_D2D_DCE_SUB:
        case IQCB_PREIME_D2D_DCE_PRI:
            //enable function

            subf->dceFuncEn |=  ChInfo->DCE_FuncEn;
            group->dceUpdate |= (
                                DCE_CENTER         |    ///< update distortion center
                                DCE_DIST           |    ///< update H/V distance factor/ratio selection
                                DCE_RADIUS         |    ///< update radius norm term
                                DCE_CAB            |    ///< update color aberration factor
                                DCE_LUT            |    ///< update LUT table address
                                DCE_FOV            |    ///< update FOV
                                DCE_ENH            |    ///< update enhancement
                                DCE_H_FILT         |    ///< update h direction filter enable/disable(UV only)
                                DCE_DISTOR         |    ///< update distortion parameter set
                                DCE_CFAINFO        |    ///< update cfa interpolation
                                0
                              );
            subf->pDistortion = &ChInfo->IQS_Distortion;
            subf->Dce_Dist = ChInfo->IQS_DCE_Dist;
            subf->pCFAInterpolation = &ChInfo->IQS_CFAInter;
            break;
        case IQS_OZOOM_END:
        case IQS_DZOOM_END:
        case IQS_IMAGE_EFFECT:

        default :
            DBG_ERR("DCE_IQparam() param mode(%d) selection error!\r\n",MsgID);
            return E_SYS;
    }
    return E_OK;
}


static ER IFE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IFE_SUBFUNC *subf,IPL_HAL_GROUP* group)
{

    IQSIM_CHINFO *ChInfo = (IQSIM_CHINFO *)IQSIM_GetChInfo(MsgID);
    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
        case IQS_AUTO_TRIG:
        case IQCB_PRV_IFE_INIT:
        case IQCB_PREIME_D2D_IFE_PRI:
        case IQCB_PREIME_D2D_IFE_SUB:
            //enable function
            subf->ifeFuncEn |=  ChInfo->IFE_FuncEn;

            //Setting parameter
            group->ifeUpdate |= (
                                    IFE_SUBFEN      |       ///< update func enable/disable
                                    IFE_OUTLIER     |
                                    IFE_CGAIN       |       ///< update color gain
                                    IFE_CGOFS       |       ///< update offset(bayer stage)
                                    IFE_CGEFFECT    |       ///< update color gain output inverse/mask
                                    IFE_FILTER      |       ///< update noise reduction filter
                                    IFE_VIGPARAM    |       ///< update vignette parameters
                                    IFE_GBALPARAM   |       ///< update Gbalance parameters
                                0);

            /////////////////////////////////
            // OUTL & FILTER & GBAL & CGain
            /////////////////////////////////
            subf->pOutl    = &ChInfo->IQS_Outl;
            subf->pFilter  = &ChInfo->IQS_Filter;
            subf->pGbal = &ChInfo->IQS_Gbal;

            ChInfo->IQS_ColorGain.pCOfs = ChInfo->IQS_CGain_Offset;
            subf->CGain = ChInfo->IQS_ColorGain;

            //////////////////
            // VIG Gain
            //////////////////
            #if (0)
            IQS_VIG_INFO VigInfo;            
            VigInfo.uiCntrPosX = 500;
            VigInfo.uiCntrPosY = 500;
            IQSIM_IFE_GetVigSetting(Id, TRUE, &VigInfo);
            ChInfo->IQS_Vig.Center.R.x = VigInfo.uiVigX;
            ChInfo->IQS_Vig.Center.R.y = VigInfo.uiVigY;
            ChInfo->IQS_Vig.Center.G   = ChInfo->IQS_Vig.Center.R;
            ChInfo->IQS_Vig.Center.B   = ChInfo->IQS_Vig.Center.R;
            ChInfo->IQS_Vig.T       = VigInfo.uiVigT;
            ChInfo->IQS_Vig.XDIV    = VigInfo.uiVigXDiv;
            ChInfo->IQS_Vig.YDIV    = VigInfo.uiVigYDiv;
            ChInfo->IQS_Vig.bDither = FALSE;
            ChInfo->IQS_Vig.DitherRstEn = FALSE;
            #endif
            subf->pVig  = &ChInfo->IQS_Vig;
            break;
        case IQS_AWB_END:
        case IQS_AE_END:
        case IQS_OZOOM_END:
        case IQS_DZOOM_END:
        default :
            DBG_ERR("IFE_IQparam() param mode(%d) selection error!\r\n",MsgID);
            return E_SYS;
    }
    return E_OK;
}


static ER IPE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPE_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    IQSIM_CHINFO *ChInfo = (IQSIM_CHINFO *)IQSIM_GetChInfo(MsgID);
    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
        case IQS_AUTO_TRIG:
        case IQCB_PRV_IPE_INIT:
        case IQCB_PREIME_D2D_IPE_PRI:
        case IQCB_PREIME_D2D_IPE_SUB:
             //enable function
            subf->IpeFuncEn |= ChInfo->IPE_FuncEn;

            //Setting parameter
            group->ipeUpdate |= (
                                IPE_SUBFEN     |
                                IPE_RGBGAMMA   |
                                IPE_YCURVE     |
                                IPE_EDGEEXT_INFO |   ///< update edge extraction
                                IPE_EDGEENH_INFO |   ///< update positive/negative edge enhance/inverse
                                IPE_EDGEMAP_INFO |   ///< update edge map
                                IPE_RGBLPF_INFO  |   ///< update rgb low pass
                                IPE_CC_INFO      |   ///< update color correction
                                IPE_CST_INFO     |   ///< update color space transfor
                                IPE_C_CTRL       |   ///< update color control
                                IPE_C_CON       |   ///< update color contrast(Y/CC)
                                IPE_C_OFS       |   ///< update Cb/Cr offset
                                IPE_RANDNOISE   |   ///< update YCC random noise
                                IPE_YCCFIX_INFO |   ///< update YCC fix replace
                                IPE_CSTP        |   ///< update color space transfor protection
                                IPE_3DCC_INFO   |   ///< update 3d color correction dram addr
                                IPE_GAMRAND     |   ///< update gamma random noise
                                0
                              );

            //////////////////
            // Edge Ext
            //////////////////
            ChInfo->IQS_EdgeExt.EEXT_TblAddr = (UINT32)ChInfo->IQS_EdgeKernel;
            ChInfo->IQS_EdgeExt.EDirTabAddr = (UINT32)ChInfo->IQS_EDirTab;
            ChInfo->IQS_EdgeExt.pDirEdgeExt = &ChInfo->IQS_DirEdgeExt;
            ChInfo->IQS_EdgeExt.ScrCalTab = &ChInfo->IQS_DirScoreTab;
            ChInfo->IQS_EdgeExt.ScrCalThTab = &ChInfo->IQS_DirScoreTh;
            ChInfo->IQS_EdgeExt.ConParam = &ChInfo->IQS_DECon;
            ChInfo->IQS_EdgeExt.pEWA = &ChInfo->IQS_EWA;
            ChInfo->IQS_EdgeExt.pEWB = &ChInfo->IQS_EWB;
            ChInfo->IQS_EdgeExt.pEWParam = &ChInfo->IQS_EWParam;
            ChInfo->IQS_EdgeExt.ToneMapLutTabAddr = (UINT32)ChInfo->IQS_EdgeGammaLut;
            subf->pedgeExt          = &ChInfo->IQS_EdgeExt;

            //////////////////
            // Edge Map
            //////////////////
            ChInfo->IQS_EdgeMap.edMap = ChInfo->IQS_EDMap;
            ChInfo->IQS_EdgeMap.esMap = ChInfo->IQS_ESMap;
            subf->pedgeMap   = &ChInfo->IQS_EdgeMap;

            //////////////////
            // Edge Enh
            //////////////////
            subf->pedgeEnh   = &ChInfo->IQS_EdgeEnh;


            //////////////////
            // RGB lpf
            //////////////////
            subf->prgblpf    = &ChInfo->IQS_RGBLpf;

            //////////////////
            // Color Matrix
            //////////////////
            ChInfo->IQS_ColorCorrection.Fstab_TblAddr = (UINT32)ChInfo->IQS_FStab;
            ChInfo->IQS_ColorCorrection.Fdtab_TblAddr = (UINT32)ChInfo->IQS_FDtab;
            subf->pCC = &ChInfo->IQS_ColorCorrection;


            //////////////////
            // Color Control
            //////////////////
            ChInfo->IQS_Cctrl.HueTab_TblAddr = (UINT32)ChInfo->IQS_hueTab;
            ChInfo->IQS_Cctrl.SatTab_TblAddr = (UINT32)ChInfo->IQS_satTab;
            ChInfo->IQS_Cctrl.IntTab_TblAddr = (UINT32)ChInfo->IQS_intTab;
            ChInfo->IQS_Cctrl.EdgTab_TblAddr = (UINT32)ChInfo->IQS_edgTab;
            ChInfo->IQS_Cctrl.DDSTab_TblAddr = (UINT32)ChInfo->IQS_ddsTab;
            subf->pCctrl = &ChInfo->IQS_Cctrl;

            /////////////////
            //Image Effect
            /////////////////
            subf->pCbCrofs = &ChInfo->IQS_CCOfs;
            subf->pYCCcon = &ChInfo->IQS_YCCcon;

            subf->pRandNoise = &ChInfo->IQS_NoiseParam;

            ////////////////////////////
            // color space transfor
            ////////////////////////////
            subf->pCST = &ChInfo->IQS_CSTParam;

            ////////////////////////////
            // YCC fix effect
            ////////////////////////////
            subf->pYccFix = &ChInfo->IQS_YCCFix;

            //////////////////
            // GAMMA & YCurve
            //////////////////
            subf->GammaAddr = (UINT32)IPE_GammaReorgon(Id, (UINT32 *)ChInfo->IQS_GammaLut, (UINT32 *)ChInfo->IQS_GammaLut, (UINT32 *)ChInfo->IQS_GammaLut);
            subf->YGammaAddr = (UINT32)IPE_YCurveReorgon(Id, (UINT32 *)ChInfo->IQS_YCurveLut);
            subf->pGamRand = &ChInfo->IQS_GammaRand;

            //////////////////
            // 3D Color Control
            //////////////////
            subf->p3d_cc = &ChInfo->IQS_3DColorCtrl;
            break;
        case IQS_AE_END:
        case IQS_IMAGE_EFFECT:
        default :
            DBG_ERR("IPE_IQparam() param mode(%d) selection error!\r\n",MsgID);
            return E_SYS;
         break;
    }
    return E_OK;
}

static ER IME_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IME_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    IQSIM_CHINFO *ChInfo = (IQSIM_CHINFO *)IQSIM_GetChInfo(MsgID);
    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
        case IQS_AUTO_TRIG:
        case IQCB_PRV_IME_INIT:
        case IQCB_PREIME_D2D_IME_PRI:

            subf->imeFuncEn |= ChInfo->IME_FuncEn;
            group->imeUpdate |= (   IME_SUBFEN      |   ///< update function enable/disable
                                    IME_CHROMAA     |   ///< update chroma adaption
                                    IME_CHROMAA_LUM |   ///< update chroma lumaination adaption
                                    IME_CHROMAA_CLR |   ///< update chroma color adaption
                                    IME_CHROMAA_CTR_UV |   ///< update chroma adjust6ment center for UV channel(data from IFE2)
                                    IME_CHROMADB_SUP|   ///< update chroma suppression.
                                    IME_SUPER_RES   |   ///< update super resoltuion info
                                    0);
            if ( IQS_IsChromaAdaptEn(Id, IQS_IsCapFlow(MsgID)) == ENABLE )
                 subf->imeFuncEn |= ChromaAdaptEn;
            if ( IQS_IsChromaAdaptEn(Id, IQS_IsCapFlow(MsgID)) == DISABLE )
                 subf->imeFuncEn &= (~ChromaAdaptEn);
                 
            subf->ChromaAdap.pChromaAdaption      = &ChInfo->IQS_ChromaAdapt;
            subf->ChromaAdap.pChromaLumAdaption   = &ChInfo->IQS_ChromaAdapt_Lum;
            subf->ChromaAdap.pChromaColorAdaption = &ChInfo->IQS_ChromaAdapt_Color;
            subf->pChromaSuppres = &ChInfo->IQS_ChromaSupp;
            subf->pSuperRes = &ChInfo->IQS_SuperRes;
            break;
        case IQCB_PREIME_D2D_IME_SUB:
        case IQS_AE_END:
        case IQS_DZOOM_END:
        case IQS_AWB_END:
        case IQS_OZOOM_END:
        case IQS_IMAGE_EFFECT:
        default :
            DBG_ERR("IME_IQparam() param mode(%d) selection error!\r\n",MsgID);
            return E_SYS;
    }
    return E_OK;
}

static ER IFE2_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IFE2_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    IQSIM_CHINFO *ChInfo = (IQSIM_CHINFO *)IQSIM_GetChInfo(MsgID);
    switch((UINT32)MsgID)
    {
        case IQCB_PRV_IFE2_INIT:
        case IQS_AUTO_TRIG:
        case IQS_MANUAL_TRIG:
        case IQCB_D2D_IFE2_SUB:
        case IQCB_D2D_IFE2_PRI:
            group->ife2Update |= (  IFE2_REFCENTERPARA | //< update reference center para.
                                    IFE2_FILTERPARAM   | //< update filter para.
                                    IFE2_GRAYSTAL      | //< update filter para.
                                    0
                                 );
            subf->ife2FuncEn |= ChInfo->IFE2_FuncEn;

            subf->GrayStatal = ChInfo->IQS_GrayStatical;
            ChInfo->IQS_RefCenter.RefCentY = ChInfo->IQS_RefCenter_Y;
            ChInfo->IQS_RefCenter.RefCentUV = ChInfo->IQS_RefCenter_UV;
            subf->pRefcent = &ChInfo->IQS_RefCenter;

            subf->pFilter = &ChInfo->IQS_IFE2Filter.Filter;
            break;
        case IQS_AE_END:
        case IQS_AWB_END:
        case IQS_OZOOM_END:
        case IQS_IMAGE_EFFECT:
        default:
            DBG_ERR("IFE2_IQparam() param mode(%d) selection error!\r\n",MsgID);
            break;
    }
    return E_OK;
}

static ER DRE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, DRE_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
IQSIM_CHINFO *ChInfo = (IQSIM_CHINFO *)IQSIM_GetChInfo(MsgID);
    UINT32 ISO_Value, idx, i;
    ISO_Value = IQS_GetISOIdx(Id, MsgID);
    switch((UINT32)MsgID)
    {
        case IQCB_D2D_DRE_PRI:
        case IQS_MANUAL_TRIG:
            group->dreUpdate |= (   DRE_UPDATE_INIT  |
                                    DRE_UPDATE_NR    |
                                    0);

            //HDR DeGhost
            subf->DgCtrl.bDgImgRef = ChInfo->IQS_MSNR.bDgImgRef;
            subf->DgCtrl.bDgEn     = ChInfo->IQS_MSNR.bDgEn;
            subf->DgCtrl.uiDgLyrNum = ChInfo->IQS_MSNR.uiDgLyrNum;
            for ( i =0; i<DRE_DG_PROC_MAX_NUM; i++ )
            {
                idx = ChInfo->IQS_MSNR.DgMskInIdx[i];
                if ( idx >= IQS_MAX_DGMASK_NUM )
                {
                    DBG_ERR("Wrong DGMask Index (ISO_Value = %d, i = %d, Idx = %d)\r\n", ISO_Value, i, idx);
                    idx = 0 ;
                }
                subf->DgCtrl.DgMskIn[i] = ChInfo->IQS_DRE_DGMask[idx];
            }

            //MSNR
            subf->uiProcRepNum = ChInfo->IQS_MSNR.uiProcRepNum;
            for ( i =0; i<subf->uiProcRepNum; i++ )
            {
                idx = ChInfo->IQS_MSNR.uiProcRepIdx[i];
                if ( idx == 0 )
                {
                    DBG_ERR("Wrong MSNR Index (ISO_Value = %d, i = %d, Idx = %d)\r\n", ISO_Value, i, idx);
                    return E_OK;
                }

                subf->NrJBFilt[i].puiWt      = ChInfo->IQS_MSNR_NR[idx].IQS_Msnr_Filt_Weight;
                subf->NrJBFilt[i].puiRngThrY = ChInfo->IQS_MSNR_NR[idx].IQS_Msnr_Filt_RngThr_Y;
                subf->NrJBFilt[i].puiRngThrU = ChInfo->IQS_MSNR_NR[idx].IQS_Msnr_Filt_RngThr_U;
                subf->NrJBFilt[i].puiRngThrV = ChInfo->IQS_MSNR_NR[idx].IQS_Msnr_Filt_RngThr_V;

                subf->bNrGamYEn[i] = ChInfo->IQS_MSNR_NR[idx].IQS_Msnr_GamLutY_En;
                subf->bNrGamUEn[i] = ChInfo->IQS_MSNR_NR[idx].IQS_Msnr_GamLutU_En;
                subf->bNrGamVEn[i] = ChInfo->IQS_MSNR_NR[idx].IQS_Msnr_GamLutV_En;
                subf->GamLut[i].puiLutY = ChInfo->IQS_MSNR_NR[idx].IQS_Msnr_GamLut_Y;
                subf->GamLut[i].puiLutU = ChInfo->IQS_MSNR_NR[idx].IQS_Msnr_GamLut_U;
                subf->GamLut[i].puiLutV = ChInfo->IQS_MSNR_NR[idx].IQS_Msnr_GamLut_V;

                subf->YCMTbl[i].puiYcLutY = ChInfo->IQS_MSNR_NR[idx].IQS_Msnr_YCMLut_Y;
                subf->YCMTbl[i].puiYcLutC = ChInfo->IQS_MSNR_NR[idx].IQS_Msnr_YCMLut_C;

                subf->YCMCtrl[i] = ChInfo->IQS_MSNR_NR[idx].IQS_Msnr_YCMCtrl;
            }
            break;

        default:
            DBG_ERR("DRE_IQparam() param mode(%d) selection error!\r\n",MsgID);
            break;
    }
    return E_OK;
}

///////////////////////////////////////////////
// FlowSetting Interface
///////////////////////////////////////////////
ER IQS_SettingFlowFunc(UINT32 Id, IQS_FLOW_MSG MsgID)
{
    return E_OK;
}


///////////////////////////////////////////////
// CBSetting Interface
///////////////////////////////////////////////

ER IQCB_SettingFlowFunc(UINT32 Id, IQS_FLOW_MSG MsgID, ENG_SUBFUNC *Info, IPL_HAL_GROUP* group)
{
    switch((UINT32)MsgID)
    {
        //preview
        case IQCB_PRV_SIE_INIT:
        case IQCB_PREIME_D2D_SIE_PRI:
        case IQCB_PREIME_D2D_SIE_SUB:
            if(Info->pSie_sub == NULL || group == NULL)
                return E_PAR;
            SIEx_IQparam(Id, MsgID, Info->pSie_sub, group);
            break;

        case IQCB_PRV_IPE_INIT:
        case IQCB_PREIME_D2D_IPE_PRI:
        case IQCB_PREIME_D2D_IPE_SUB:
            if(Info->pIpe_sub == NULL || group == NULL)
                return E_PAR;
            IPE_IQparam(Id, MsgID, Info->pIpe_sub, group);
            break;

        case IQCB_PRV_DCE_INIT:
        case IQCB_PREIME_D2D_DCE_SUB:
        case IQCB_PREIME_D2D_DCE_PRI:
            if(Info->pDce_sub == NULL || group == NULL)
                return E_PAR;
            DCE_IQparam(Id, MsgID, Info->pDce_sub, group);
            break;

        case IQCB_PRV_IFE_INIT:
        case IQCB_PREIME_D2D_IFE_PRI:
        case IQCB_PREIME_D2D_IFE_SUB:
            if(Info->pIfe_sub == NULL || group == NULL)
                return E_PAR;
            IFE_IQparam(Id, MsgID, Info->pIfe_sub, group);
            break;

        case IQCB_PRV_IME_INIT:
        case IQCB_PREIME_D2D_IME_SUB:
        case IQCB_PREIME_D2D_IME_PRI:
            if(Info->pIme_sub == NULL || group == NULL)
                return E_PAR;
            IME_IQparam(Id, MsgID, Info->pIme_sub, group);
            break;

        case IQCB_PRV_IFE2_INIT:
        case IQCB_D2D_IFE2_SUB:
        case IQCB_D2D_IFE2_PRI:
            if(Info->pIfe2_sub == NULL || group == NULL)
                return E_PAR;
            IFE2_IQparam(Id, MsgID, Info->pIfe2_sub, group);
            break;

        case IQCB_D2D_DRE_PRI:
            if(Info->pDre_sub == NULL || group == NULL)
                return E_PAR;
            DRE_IQparam(Id, MsgID, Info->pDre_sub, group);
            break;
        default:
            DBG_ERR("^R-E- MsgID(%x)\r\n", MsgID);
            break;
    }
    return E_OK;
}

//#NT#2016/12/22#Wendy Liao -begin
//#NT#Support IPcam function
void IQS_SetIPcamFunction(UINT32 Enable)
{
}
//#NT#2016/12/22#Wendy Liao -end

void IQS_Init(UINT32 Id)
{
}

#endif
