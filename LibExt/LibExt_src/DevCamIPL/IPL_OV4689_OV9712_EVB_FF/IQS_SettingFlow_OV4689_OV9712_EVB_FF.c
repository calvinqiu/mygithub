#ifndef _IQS_SETTINGFLOW_OV4689_OV9712_EVB_FF_C
#define _IQS_SETTINGFLOW_OV4689_OV9712_EVB_FF_C
#include "IPL_OV4689_OV9712_EVB_FF_Int.h"

#if CAL_ECS
#include "IPL_Cal_DataPassing_OV4689_OV9712_EVB_FF_Int.h"
#endif
#include "NOTE.h"
BOOL IQS_IPCAM_ENABLE = 0;
void iqs_param_init(UINT32 id);
UINT32 IQS_YCurveLut_Buf[Total_SENIDNum][130];
UINT32 IQS_EdgeGammaLut_Buf[Total_SENIDNum][65];
INT16 IQS_WDR_Sat_Buf[Total_SENIDNum];
IPE_YCCFIX IQS_YCCFix_Buf[Total_SENIDNum];

UINT32 IQS_GammaAddr_Buf[Total_SENIDNum];
UINT32 IQS_YCurveAddr_Buf[Total_SENIDNum];
BOOL Gamma_update[Total_SENIDNum] = {FALSE, FALSE};
BOOL Ycurve_update[Total_SENIDNum] = {FALSE, FALSE};
BOOL flag_ycurve[Total_SENIDNum] = {FALSE, FALSE};

UINT32 IQS_TBL_UpdateBit[Total_SENIDNum] = {0, 0};
UINT32 IQS_ENG_UpdateBit[Total_SENIDNum] = {0, 0};

extern UINT32 uiISOGain[AE_ID_MAX_NUM], uiExpoTime[AE_ID_MAX_NUM];
extern UINT32 g3DNR;
HDR_LOCAL_TONE_INFO     IQS_LocTnInfo_buf[Total_SENIDNum];

static sISP_ISOINDEX gISO_Index[Total_SENIDNum] = {0};

///////////////////////////////////////////////
// Body
///////////////////////////////////////////////
UINT32 IQS_GetISORange(UINT32 Id, IQS_FLOW_MSG MsgID, UINT32 *IsoValue, UINT32 *IsoStart, UINT32 *IsoEnd, UINT32 *IQIdxL, UINT32 *IQIdxH)
{
    UINT32 i, ISO, ExpT, Iris;
    ISOMAP *ISOMap;
    UINT32 SenId_Value = IQS_GetSensorIdx(Id);
    UINT32 CFMode = IPL_CtrlGetInfor(Id, IPLCTRL_CFMODE);

    if ( IQS_IsCapFlow(MsgID)==FALSE )  // movie mode
        AE_GetPrvAEArg(Id, &ISO, &ExpT, &Iris);
    else                                // capture mode
        AE_GetCapAEArg(Id, &ISO, &ExpT, &Iris);
    if ( uiISOGain[Id]!=0 )
        ISO = uiISOGain[Id];

    *IsoValue = ISO;

    if ( SenId_Value == SENSOR_ID_1 && CFMode == IPL_CF_2STRP )
    {
        if ( IQS_IsCapFlow(MsgID)==FALSE )  // movie mode
        {
            if ( ISO >=IQS_PRV1_ISOEND_VALUE )
            {
                *IQIdxL = IQS_PRV1_ISOEND;
                *IQIdxH = IQS_PRV1_ISOEND;
                *IsoStart = IQS_PRV1_ISOEND_VALUE;
                *IsoEnd = IQS_PRV1_ISOEND_VALUE;
                return FALSE;
            }
            ISOMap = &(IQS1_ISOMapTab[0][0]);
        }
        else                                // capture mode
        {
            if ( ISO >=IQS_CAP1_ISOEND_VALUE )
            {
                *IQIdxL = IQS_CAP1_ISOEND;
                *IQIdxH = IQS_CAP1_ISOEND;
                *IsoStart = IQS_CAP1_ISOEND_VALUE;
                *IsoEnd = IQS_CAP1_ISOEND_VALUE;
                return FALSE;
            }
            ISOMap = &(IQS1_ISOMapTab[1][0]);
        }
    }
    else if ( SenId_Value == SENSOR_ID_1 )
    {
        if ( IQS_IsCapFlow(MsgID)==FALSE )  // movie mode
        {
            if ( ISO >=IQS_PRV1S_ISOEND_VALUE )
            {
                *IQIdxL = IQS_PRV1S_ISOEND;
                *IQIdxH = IQS_PRV1S_ISOEND;
                *IsoStart = IQS_PRV1S_ISOEND_VALUE;
                *IsoEnd = IQS_PRV1S_ISOEND_VALUE;
                return FALSE;
            }
            ISOMap = &(IQS1S_ISOMapTab[0][0]);
        }
        else                                // capture mode
        {
            if ( ISO >=IQS_CAP1S_ISOEND_VALUE )
            {
                *IQIdxL = IQS_CAP1S_ISOEND;
                *IQIdxH = IQS_CAP1S_ISOEND;
                *IsoStart = IQS_CAP1S_ISOEND_VALUE;
                *IsoEnd = IQS_CAP1S_ISOEND_VALUE;
                return FALSE;
            }
            ISOMap = &(IQS1S_ISOMapTab[1][0]);
        }
    }
    else if ( SenId_Value == SENSOR_ID_2 )
    {
        if ( IQS_IsCapFlow(MsgID)==FALSE  )  // movie mode
        {
            if ( ISO >=IQS_PRV2_ISOEND_VALUE )
            {
                *IQIdxL = IQS_PRV2_ISOEND;
                *IQIdxH = IQS_PRV2_ISOEND;
                *IsoStart = IQS_PRV2_ISOEND_VALUE;
                *IsoEnd = IQS_PRV2_ISOEND_VALUE;
                return FALSE;
            }
            ISOMap = &(IQS2_ISOMapTab[0][0]);
        }
        else                                // capture mode
        {
            if ( ISO >=IQS_CAP2_ISOEND_VALUE )
            {
                *IQIdxL = IQS_CAP2_ISOEND;
                *IQIdxH = IQS_CAP2_ISOEND;
                *IsoStart = IQS_CAP2_ISOEND_VALUE;
                *IsoEnd = IQS_CAP2_ISOEND_VALUE;
                return FALSE;
            }
            ISOMap = &(IQS2_ISOMapTab[1][0]);
        }
    }
    else
    {
        DBG_ERR("wrong iso map\r\n");
        return FALSE;
    }

    for ( i=1; i<IQS_ISOMAPTAB_NUM; i++ )
    {
        if (ISO < ISOMap[i].ISO)
        {
            *IQIdxL = ISOMap[i-1].Index;
            *IQIdxH = ISOMap[i].Index;
            *IsoStart = ISOMap[i-1].ISO;
            *IsoEnd = ISOMap[i].ISO;
            break;
        }
    }
    return TRUE;
}


UINT32 IQS_GetISOIdx(UINT32 Id, IQS_FLOW_MSG MsgID)
{
    UINT32 IsoValue=0, IsoStart=0, IsoEnd=0, IQL=0, IQH=0, ret;
    ret = IQS_GetISORange(Id, MsgID, &IsoValue, &IsoStart, &IsoEnd, &IQL, &IQH);
    return IQL;
}

UINT32 IQS_GetSensorIdx(UINT32 Id)
{
    if ( IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_CFMODE) == IPL_CF_RHEHDR || (Id > IPL_ID_2))
        return IPL_UTI_CONV2_SEN_ID(IPL_ID_1);
    return IPL_UTI_CONV2_SEN_ID(Id);
}

static ER SIEx_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, SIE_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    UINT32 ISO_Value = IQS_GetISOIdx(Id, MsgID);
    UINT32 SenId_Value = IQS_GetSensorIdx(Id);
    UINT32 RGain=256, GGain=256, BGain=256;
    IQS_VIG_INFO VigInfo;
    SENSOR_INFO Info;

    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
        case IQS_AUTO_TRIG:
        case IQCB_PRV_SIE_INIT:
        case IQCB_PREIME_D2D_SIE_PRI:
            subf->sieFuncEn |=  (
                                    OBAvgEn     |   ///< enable OB average ( ob detection)
                                    ECSEn       |   ///< enable Color shading compensation
                                    0
                                );
            group->sieUpdate |= (
                                    SIE_SUBFEN          |   ///< update func enable/disable
                                    SIE_VIG_            |   ///< update VIG
                                    SIE_LA_GAMMA        |   ///< update LA Gamma
                                    SIE_OBOFS_          |   ///< update ob offset
                                    SIE_DGAIN_          |   ///< update Digital gain
                                    SIE_ECS_            |   ///< update ECS/including ECS table addr
                                    0
                                 );

            Info.Mode = NULL;
            if ( IQS_IsCapFlow(MsgID) == TRUE )
                IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE), &Info);
            else
                IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &Info);
            if (Info.Mode == NULL)
            {
                DBG_ERR("NULL Info.Mode\r\n");
                return E_SYS;
            }
            if ( Info.Mode->ModeType == SENSOR_MODE_CCIR || Info.Mode->ModeType == SENSOR_MODE_CCIR_INTERLACE )
            {
                subf->sieFuncEn &= ~ECSEn;
                group->sieUpdate &= ~SIE_ECS_;
            }
//#NT#2016/06/15#Ted -begin
//#NT# SIE sub correct
            if ( IQS_OBSubMode(Id, MsgID) == IQS_IFE_OBSUB )
                subf->sieFuncEn |= OBByPassEn;
            else
                subf->sieFuncEn &= ~OBByPassEn;


//#NT#2016/06/15#Ted -end

            if ( !(MsgID == IQS_MANUAL_TRIG || MsgID == IQCB_PRV_SIE_INIT || MsgID == IQCB_PREIME_D2D_SIE_PRI || MsgID == IQCB_PREIME_D2D_SIE_SUB || (IQS_TBL_UpdateBit[Id]&IQS_TBL_GAMMA)) )
                group->sieUpdate &= ~SIE_LA_GAMMA;

            if ( !(MsgID == IQS_MANUAL_TRIG || MsgID == IQCB_PRV_SIE_INIT || MsgID == IQCB_PREIME_D2D_SIE_PRI || MsgID == IQCB_PREIME_D2D_SIE_SUB || (IQS_TBL_UpdateBit[Id]&IQS_TBL_ECS)) )
                group->sieUpdate &= ~SIE_ECS_;

            if ( !(MsgID == IQS_MANUAL_TRIG || MsgID == IQCB_PRV_SIE_INIT || MsgID == IQCB_PREIME_D2D_SIE_PRI || MsgID == IQCB_PREIME_D2D_SIE_SUB || (IQS_TBL_UpdateBit[Id]&IQS_TBL_VIG)) )
                group->sieUpdate &= ~SIE_VIG_;

            if (IQS_IPCAM_ENABLE)
            {
                if( IQS_IPCAM_RatioTable[Id].LSC.isEnable == DISABLE )
                    group->sieUpdate &= ~SIE_VIG_;
            }

            if ( IPL_CtrlGetInfor(Id, IPLCTRL_SIE_ETH_EN) == ENABLE )
            {
                subf->pEdgeThreshold = &Eth_Thres[ISO_Value];
                group->sieUpdate |= SIE_ETH_THRES_;
            }

            ////////////////////
            // SIE-OB Substration
            ////////////////////
            subf->OBSub.Offset = IQS_OBSub_SIE_Buf[SenId_Value];

            ////////////////////
            // SIE-LA GAMMA
            ////////////////////
            if ( group->sieUpdate & SIE_LA_GAMMA )
                subf->La.GmaTbl = SIE_xfer2CAGamma(SenId_Value, (UINT32 *)IQS_GammaLut[SenId_Value]);

            //////////////////
            // SIE-VIG Gain
            //////////////////
            if ( group->sieUpdate & SIE_VIG_ )
            {
                if (IQS_IPCAM_ENABLE)
                {
                    VigInfo.uiCntrPosX = IQS_IPCAM_RatioTable[SenId_Value].LSC.CenterXOffset * 5;
                    VigInfo.uiCntrPosY = IQS_IPCAM_RatioTable[SenId_Value].LSC.CenterYOffset * 5;
                }else
                {
                    VigInfo.uiCntrPosX = 500;
                    VigInfo.uiCntrPosY = 500;
                }

                SIE_GetVigSetting(SenId_Value, IQS_IsCapFlow(MsgID), &VigInfo);
                IQS_CA_Vig[SenId_Value].Center.x = VigInfo.uiVigX;
                IQS_CA_Vig[SenId_Value].Center.y = VigInfo.uiVigY;
                IQS_CA_Vig[SenId_Value].T        = VigInfo.uiVigT;
                IQS_CA_Vig[SenId_Value].XDIV     = VigInfo.uiVigXDiv;
                IQS_CA_Vig[SenId_Value].YDIV     = VigInfo.uiVigYDiv;
                IQS_CA_Vig[SenId_Value].TabGain  = IQS_Vig_Set[SenId_Value].TabGain;
                IQS_CA_Vig[SenId_Value].LUT_TblAddr = (UINT32)IQS_Vig_Tab_Buf[SenId_Value];
                subf->pVig = &IQS_CA_Vig[SenId_Value];
            }

            ////////////////////
            // SIE-ECS
            ////////////////////
            if ( group->sieUpdate & SIE_ECS_ )
            {
                #if 0
                {
                    UINT32 i,j,Rgain,Ggain,Bgain,Data;
                    for(j=0;j<65;j++)
                    {
                        for(i=0;i<65;i++)
                        {
                            Rgain = 0x100*165/100;
                            Ggain = 0x100;
                            Bgain = 0x100*170/100;
                            Data = (Bgain & 0x3FF) | ((Ggain & 0x3FF)<<10) | ((Rgain & 0x3FF)<<20);
                            IQS_ECSTab[SenId_Value][j][i] = Data;
                        }
                    }
                }
                #endif

                #if CAL_ECS
                pCAL_TAG pTag;
                CAL_ECS_FMT* pFmt;
                CAL_DATA_INFO CaldataInfo = {0};

                CaldataInfo.StationID = SEC_ECS;
                CaldataInfo.SenMode = IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE);
                CaldataInfo.CalCondition[0] = CAL_CONDITION_DEFAULT;

                pTag = GetCalData(Id, CaldataInfo);

                if (pTag != NULL)
                {
                    DBG_IND("get ECS Calibration data\r\n");
                    if( pTag->CAL_STATUS == _CAL_OK )
                    {
                        pFmt = (CAL_ECS_FMT*)&pTag->TagData;

                        subf->Ecs.EcsMap = pFmt->header.Ecs_map;
                        subf->Ecs.Inaddr = (UINT32)&pFmt->ECSBlockWin;
                        subf->Ecs.pecs = &IQS_ECSSet;
                    }
                    else
                    {
                        DBG_IND("cal status not OK\r\n");
                        subf->Ecs.EcsMap = ecs65x65;
                        subf->Ecs.Inaddr = (UINT32)IQS_GetECStab(Id, IQS_IsCapFlow(MsgID), (UINT32*)IQS_ECSTab[SenId_Value], 65,  &Info);
                        subf->Ecs.pecs = &IQS_ECSSet;

                        subf->sieFuncEn &= ~ECSEn;
                        group->sieUpdate |= (SIE_ECS_ | SIE_SUBFEN);
                    }
                }
                else
                {
                    DBG_IND("NO ECS cal data\r\n");
                    subf->Ecs.EcsMap = ecs65x65;
                    subf->Ecs.Inaddr = (UINT32)IQS_GetECStab(Id, IQS_IsCapFlow(MsgID), (UINT32*)IQS_ECSTab[SenId_Value], 65,  &Info);
                    subf->Ecs.pecs = &IQS_ECSSet;

                    subf->sieFuncEn &= ~ECSEn;
                    group->sieUpdate |= (SIE_ECS_ | SIE_SUBFEN);
                }
                #else
                {
                    subf->Ecs.EcsMap = ecs65x65;
                    subf->Ecs.Inaddr = (UINT32)IQS_GetECStab(Id, IQS_IsCapFlow(MsgID), (UINT32*)IQS_ECSTab[SenId_Value], 65,  &Info);
                    subf->Ecs.pecs = &IQS_ECSSet;

                    subf->sieFuncEn &= ~ECSEn;
                    group->sieUpdate |= (SIE_ECS_ | SIE_SUBFEN);
                }
                #endif
            }
//OB CAL
#if 0
    subf->OBSub.Offset = 0;
    subf->sieFuncEn &= ~ECSEn;
    group->sieUpdate &= ~SIE_ECS_;
#endif
            //continue to awb

        case IQS_AWB_END:
            group->sieUpdate |=  SIE_CA_TH;
            group->sieUpdate |=  SIE_LA_CG;
            AWB_CAINFO awb_cainfo;
            AWB_GetCAInfo(Id, &awb_cainfo);
            subf->Ca.CA_TH_INFO.Rth.Upper = awb_cainfo.RGu;
            subf->Ca.CA_TH_INFO.Rth.Lower = awb_cainfo.RGl;
            subf->Ca.CA_TH_INFO.Gth.Upper = awb_cainfo.GGu;
            subf->Ca.CA_TH_INFO.Gth.Lower = awb_cainfo.GGl;
            subf->Ca.CA_TH_INFO.Bth.Upper = awb_cainfo.BGu;
            subf->Ca.CA_TH_INFO.Bth.Lower = awb_cainfo.BGl;
            subf->Ca.CA_TH_INFO.Pth.Upper = awb_cainfo.RBGu;
            subf->Ca.CA_TH_INFO.Pth.Lower = awb_cainfo.RBGl;

            AWB_GetColorGain(Id, &RGain, &GGain, &BGain);
            if ( IQS_ColorGain[SenId_Value].IfeGainSel == _2_8 ) //sie fix 3.7
            {
                RGain = RGain >> 1;
                GGain = GGain >> 1;
                BGain = BGain >> 1;
            }
            subf->La.LA_CG_INFO.RGain = RGain;
            subf->La.LA_CG_INFO.GGain = GGain;
            subf->La.LA_CG_INFO.BGain = BGain;
            break;
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
    UINT32 SenId_Value = IQS_GetSensorIdx(Id);
    UINT32 IMEffect_Value = IPL_AlgGetUIInfo(Id, IPL_SEL_IMAGEEFFECT);

    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
        case IQS_AUTO_TRIG:
        case IQCB_PRV_DCE_INIT:
        case IQCB_PREIME_D2D_DCE_SUB:
        case IQCB_PREIME_D2D_DCE_PRI:
            subf->dceFuncEn |=  (
                                    FUNC_CFA        |   ///< CFA interpolation function
                                 //   FUNC_DC         |   ///< Distortion correction function
                                 //   FUNC_CAC        |   ///< Chromatic aberration correction function
                                    0
                                );
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
                                DCE_SUBFEN         |    ///< update function switch
                                0
                              );

            if( IPL_AlgGetUIInfo(Id, IPL_SEL_GDCCOMP) == SEL_GDCCOMP_ON )
            {
				 subf->dceFuncEn |=  (
                     FUNC_DC         |   ///< Distortion correction function
                     FUNC_CAC        ///< Chromatic aberration correction function
                                );
                if (IQS_IPCAM_ENABLE)
                    ;
                else
                    memcpy(IQS_DCE_GLUT_Buf[SenId_Value], IQS_DCE_GLUT,         sizeof(UINT32)*65);
            }
            else
			{
				subf->dceFuncEn &=~ (
                     FUNC_DC         |   ///< Distortion correction function
                     FUNC_CAC        ///< Chromatic aberration correction function
                                );
                memcpy(IQS_DCE_GLUT_Buf[SenId_Value],  IQS_DCE_GLUT_Init,   sizeof(UINT32)*65);
			}
			
            if( IMEffect_Value == SEL_IMAGEEFFECT_FISHEYE )
                memcpy(IQS_DCE_GLUT_Buf[SenId_Value], IE_FISYEYE_LUT, 65);
            IQS_DCE_ParamSetting(Id, MsgID, SenId_Value, &IQS_Distortion[SenId_Value], &subf->Dce_Dist, &subf->Radious);

            subf->pDistortion = &IQS_Distortion[SenId_Value];
            subf->pCFAInterpolation = &IQS_CFAInter[SenId_Value];
            break;

        case IQS_OZOOM_END:
        case IQS_DZOOM_END:
        case IQS_IMAGE_EFFECT:
        default:
            DBG_ERR("DCE_IQparam() param mode(%d) selection error!\r\n",MsgID);
            return E_SYS;
    }
    return E_OK;
}
//#NT#2016/05/16#Ted Lin -begin
//#NT# for WDR2.5
static ER RHE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, RHE_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    UINT32 SenId_Value = IQS_GetSensorIdx(Id);
    static UINT8 uiWgtTbl[17] = {0,16,32,48,64,80,96,112,128,144,160,176,192,208,224,240,255};
    //#NT#2016/12/13#Linkin Huang -begin
    //#NT# for WDR2.5, reducing halo
    //static UINT8 uiLpf[8] = {7,6,5,4,3,2,1,0}; 
    static UINT8 uiLpf[8] = {7,2,1,1,0,0,0,0};
    //#NT#2016/12/13#Linkin Huang -end
    static UINT16 uiGblTnTbl[41] = {0,2,4,6,8,10,12,14,16,20,
                                    24,28,32,40,48,56,64,80,96,112,
                                    128,160,192,224,256,320,384,448,512,640,
                                    768,896,1024,1280,1536, 1792, 2048,2560,3072,3584,4095
                                };//
    //#NT#2016/10/24#Linkin Huang -begin
    //#NT# for WDR2.5, modified the black point bug in bright region
    static UINT8 uiARto[9] = {80,  124,  168,  212,  212,  212,  212,  212,  212};//
    
    static UINT8 uiAMax[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF };//
    static UINT8 uiAMin[9] = {0x0, 0x3, 0xC, 0x1E, 0x38, 0x5B, 0x88, 0xCF, 0xFF};//
    //#NT#2016/10/24#Linkin Huang -end
    static UINT8 uiBMax[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,0xFF };//
    static UINT8 uiBMin[9] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};//

    static UINT16 uiPointX[4] = {0x0, 0xFFF, 0xFFF, 0xFFF};//
    static UINT16 uiPointY[4] = {0x0, 0xFFFF, 0xFFFF, 0xFFFF};//
    static UINT16 uiSlopeM[4] = {0x10, 0x10, 0x10, 0x10};//
    static UINT8 uiSlopeS[4] = {0x0, 0x0, 0x0, 0x0};//


    switch((UINT32)MsgID)
    {
        case IQCB_PRV_RHE_INIT:
            subf->HdrRadianceInfo.puiWeightTbl = uiWgtTbl;
            subf->HdrRadianceInfo.uiIn0EvGap = 4;
            subf->HdrRadianceInfo.uiIn1EvGap = 4;
            subf->HdrRadianceInfo.Weight_SoruceSel = HDR_RAD_WGT_BY_IN0;

            subf->HdrRefLaLowPassfilterInfo.puiReflaLpf = uiLpf;
            subf->HdrRefLaLowPassfilterInfo.uiReflaLpfNormM = 0;
            subf->HdrRefLaLowPassfilterInfo.uiReflaLpfNormS = 0;

            subf->HdrGlobalToneInfo.puiGblToneTbl = uiGblTnTbl;

            subf->HdrRecurvenInfo.puiPointX = uiPointX;
            subf->HdrRecurvenInfo.puiPointY = uiPointY;
            subf->HdrRecurvenInfo.puiSlopeM = uiSlopeM;
            subf->HdrRecurvenInfo.puiSlopeS = uiSlopeS;

            subf->HdrRegulationInfo.puiDataARto = uiARto;
            subf->HdrRegulationInfo.puiDataAMax = uiAMax;
            subf->HdrRegulationInfo.puiDataAMin = uiAMin;
            subf->HdrRegulationInfo.puiDataBMax = uiBMax;
            subf->HdrRegulationInfo.puiDataBMin = uiBMin;

            subf->HdrLocalToneInfo.uiBand0Gain = 512;
            subf->HdrLocalToneInfo.uiBand1Gain = 512;

            group->rheUpdate |= (RHE_HDR_LOC_TONE|RHE_HDR_GBL_TONE|RHE_HDR_REF_LA_LPF|RHE_HDR_RAD|RHE_HDR_RECRV_INFO|RHE_HDR_REGU_INFO);
            break;
        case IQS_AUTO_TRIG:
            group->rheUpdate |=RHE_HDR_LOC_TONE;
            subf->HdrLocalToneInfo= IQS_LocTnInfo_buf[SenId_Value];
            break;
        default:
            break;
    }



    return E_OK;
}
//#NT#2016/04/27#Ted Lin -end

static ER IFE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IFE_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    UINT32 SenId_Value = IQS_GetSensorIdx(Id);
    UINT32 RGain=256, GGain=256, BGain=256;
    IQS_VIG_INFO VigInfo;

    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
        case IQS_AUTO_TRIG:
        case IQCB_PRV_IFE_INIT:
        case IQCB_PREIME_D2D_IFE_PRI:
        case IQCB_PREIME_D2D_IFE_SUB:
            subf->ifeFuncEn |=  (
                                    OutlierEn   |  ///< enable outLier
                                    FilterEn    |  ///< enable bilateral filter
                                    CgainEn     |  ///< enable color gain
                                    VIGEN       |  ///< enable function: vignette
                                    GBALEN      |   ///< enable function: Gbalance
                                    0
                                );
            group->ifeUpdate |= (
                                    IFE_SUBFEN      |       ///< update func enable/disable
                                    IFE_OUTLIER     |
                                    IFE_CGAIN       |       ///< update color gain
                                    IFE_CGOFS       |       ///< update offset(bayer stage)
                                    IFE_CGEFFECT    |       ///< update color gain output inverse/mask
                                    IFE_FILTER      |       ///< update noise reduction filter
                                    IFE_VIGPARAM    |       ///< update vignette parameters
                                    IFE_VIG_CEN     |       ///< update vig center parameters
                                    IFE_GBALPARAM   |       ///< update Gbalance parameters
                                    0
                                );

            if ( !(MsgID == IQS_MANUAL_TRIG || MsgID == IQCB_PRV_IFE_INIT || MsgID == IQCB_PREIME_D2D_IFE_PRI || MsgID == IQCB_PREIME_D2D_IFE_SUB || (IQS_TBL_UpdateBit[Id]&IQS_TBL_VIG)) )
                group->ifeUpdate &= ~(IFE_VIGPARAM | IFE_VIG_CEN);

            if (IQS_IPCAM_ENABLE)
            {
                if( IQS_IPCAM_RatioTable[Id].LSC.isEnable == DISABLE )
                {
                    subf->ifeFuncEn &= ~VIGEN;
                    group->ifeUpdate &= ~(IFE_VIGPARAM | IFE_VIG_CEN);
                }
            }else
            {
                subf->ifeFuncEn &= ~VIGEN;
                group->ifeUpdate &= ~(IFE_VIGPARAM | IFE_VIG_CEN);
            }

            if( IPL_AlgGetUIInfo(Id, IPL_SEL_IPPNR) == SEL_IPPNR_OFF )
            {
                subf->ifeFuncEn &= ~(GBALEN | OutlierEn);
                memset(IQS_RangeA_Thr_Buf[SenId_Value], 0, sizeof(UINT32)*6);
                memset(IQS_RangeB_Thr_Buf[SenId_Value], 0, sizeof(UINT32)*6);
                memset(IQS_RangeA_Lut_Buf[SenId_Value], 0, sizeof(UINT32)*17);
                memset(IQS_RangeB_Lut_Buf[SenId_Value], 0, sizeof(UINT32)*17);
            }

            //////////////////
            // IFE-Vignette
            //////////////////
            if ( group->ifeUpdate & IFE_VIGPARAM )
            {
                if (IQS_IPCAM_ENABLE)
                {
                VigInfo.uiCntrPosX = IQS_IPCAM_RatioTable[SenId_Value].LSC.CenterXOffset * 5;
                VigInfo.uiCntrPosY = IQS_IPCAM_RatioTable[SenId_Value].LSC.CenterYOffset * 5;
                }
                else{
                VigInfo.uiCntrPosX = 500;
                VigInfo.uiCntrPosY = 500;
                }

                IFE_GetVigSetting(Id, IQS_IsCapFlow(MsgID), &VigInfo);
                IQS_Vig[SenId_Value].Center.R.x = VigInfo.uiVigX;
                IQS_Vig[SenId_Value].Center.R.y = VigInfo.uiVigY;
                IQS_Vig[SenId_Value].Center.G = IQS_Vig[SenId_Value].Center.R;
                IQS_Vig[SenId_Value].Center.B = IQS_Vig[SenId_Value].Center.R;
                IQS_Vig[SenId_Value].T = VigInfo.uiVigT;
                IQS_Vig[SenId_Value].TabGain = IQS_Vig_Set[SenId_Value].TabGain;
                IQS_Vig[SenId_Value].XDIV = VigInfo.uiVigXDiv;
                IQS_Vig[SenId_Value].YDIV = VigInfo.uiVigYDiv;
                IQS_Vig[SenId_Value].R_LUT_TblAddr = (UINT32)IQS_Vig_Tab_Buf[SenId_Value];
                IQS_Vig[SenId_Value].G_LUT_TblAddr = (UINT32)IQS_Vig_Tab_Buf[SenId_Value];
                IQS_Vig[SenId_Value].B_LUT_TblAddr = (UINT32)IQS_Vig_Tab_Buf[SenId_Value];
                IQS_Vig[SenId_Value].bDither = FALSE;
                IQS_Vig[SenId_Value].DitherRstEn = FALSE;
            }

            //////////////////
            // IFE-Outlier
            //////////////////
            IQS_Outl[SenId_Value].pBrightThres = IQS_Outl_Bri_Buf[SenId_Value];
            IQS_Outl[SenId_Value].pDarkThres = IQS_Outl_Dark_Buf[SenId_Value];
            IQS_Outl[SenId_Value].IFE_OUTLCNT = _8_NB;

            //////////////////
            // IFE-Spatial Filter
            //////////////////
            IQS_SpatialFilter[SenId_Value].pWeight = IQS_SF_Weight[SenId_Value];
            IQS_Filter_Buf[SenId_Value].Spatial = &IQS_SpatialFilter[SenId_Value];

            //////////////////
            // IFE-Range Filter
            //////////////////
            IQS_RangeA[SenId_Value].pThres = IQS_RangeA_Thr_Buf[SenId_Value];
            IQS_RangeA[SenId_Value].pThLut = IQS_RangeA_Lut_Buf[SenId_Value];
            IQS_RangeB_Buf[SenId_Value].pThres = IQS_RangeB_Thr_Buf[SenId_Value];
            IQS_RangeB_Buf[SenId_Value].pThLut = IQS_RangeB_Lut_Buf[SenId_Value];
            IQS_RangeFilter[SenId_Value].A = &IQS_RangeA[SenId_Value];
            IQS_RangeFilter[SenId_Value].B = &IQS_RangeB_Buf[SenId_Value];
            IQS_Filter_Buf[SenId_Value].Range = &IQS_RangeFilter[SenId_Value];

            //////////////////
            // IFE-Gbalance
            //////////////////
            IQS_Gbal_Buf[SenId_Value].pGbalStab = IQS_Gbal_Stab_Buf[SenId_Value];
            IQS_Gbal_Buf[SenId_Value].pGbalDtab = IQS_Gbal_Dtab_Buf[SenId_Value];

            ///////////////////
            // IFE-Image Setting Adjust
            ///////////////////
            subf->pOutl = &IQS_Outl[SenId_Value];
            subf->pFilter = &IQS_Filter_Buf[SenId_Value];
            subf->pGbal = &IQS_Gbal_Buf[SenId_Value];
            if ( group->ifeUpdate & IFE_VIGPARAM )
                subf->pVig = &IQS_Vig[SenId_Value];

        //continue to awb
        case IQS_AWB_END:
            //////////////////
            // IFE-CGain & OB
            //////////////////
            AWB_GetColorGain(Id, &RGain, &GGain, &BGain);
            IQS_CGain[SenId_Value][0] = RGain;
            IQS_CGain[SenId_Value][1] = GGain;
            IQS_CGain[SenId_Value][2] = GGain;
            IQS_CGain[SenId_Value][3] = BGain;
            IQS_ColorGain[SenId_Value].CG_Inv.Inv = FALSE;
            IQS_ColorGain[SenId_Value].CG_Inv.Hinv = FALSE;
            IQS_ColorGain[SenId_Value].IfeGainSel = _2_8;
            IQS_ColorGain[SenId_Value].CG_Mask.GainMask = (UINT32)0xfff;
            IQS_ColorGain[SenId_Value].pCGain = IQS_CGain[SenId_Value];

            IQS_OBSub_IFE_Buf[SenId_Value][1] = IQS_OBSub_IFE_Buf[SenId_Value][0];
            IQS_OBSub_IFE_Buf[SenId_Value][2] = IQS_OBSub_IFE_Buf[SenId_Value][0];
            IQS_OBSub_IFE_Buf[SenId_Value][3] = IQS_OBSub_IFE_Buf[SenId_Value][0];
            IQS_ColorGain[SenId_Value].pCOfs = IQS_OBSub_IFE_Buf[SenId_Value];
            subf->CGain = IQS_ColorGain[SenId_Value];
            break;

        case IQS_AE_END:
        case IQS_OZOOM_END:
        case IQS_DZOOM_END:
        default:
            DBG_ERR("IFE_IQparam() param mode(%d) selection error!\r\n",MsgID);
            return E_SYS;
    }
    return E_OK;
}

static ER IPE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IPE_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    UINT32 ISO_Value = IQS_GetISOIdx(Id, MsgID);
    UINT32 SenId_Value = IQS_GetSensorIdx(Id);
    UINT32 IMEffect_Value = IPL_AlgGetUIInfo(Id, IPL_SEL_IMAGEEFFECT);
    UINT32 iCCID_Value = IPL_AlgGetUIInfo(Id, IPL_SEL_CCID);
    IQS_COLOR_EFFECT_PARAM ColorEffectTable;
    ER ret = E_OK;

    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
        case IQS_AUTO_TRIG:
        case IQCB_PRV_IPE_INIT:
        case IQCB_PREIME_D2D_IPE_PRI:
        case IQCB_PREIME_D2D_IPE_SUB:
            subf->IpeFuncEn |= (
                                    IPE_RGBLPEn         |       ///< RGB low-pass filter function enable
                                    IPE_RGBGAMMAEn      |       ///< RGB Gamma function enable
                                    IPE_YGAMMAEn        |       ///< Y Gamma function enable,(curve share with ife chroma curve table)
                                    IPE_CCREn           |       ///< Color correction function enable
                                    //IPE_3DCCREn         |       ///< 3D color correction function enable
                                    IPE_CSTEn           |       ///< Color space transform function enable, RGB->YCC
                                    IPE_CCTRLEn         |       ///< Color control function enable
                                    IPE_HADJEn          |       ///< Hue adjustment function enable
                                    IPE_CADJEn          |       ///< Color component adjust function enable
                                    IPE_CADJ_Y_ENHEn    |       ///< Color component edge enhance function enable, sub-function of IPE_CADJEn
                                    IPE_CADJ_Y_CONEn    |       ///< Color component Y contrast adjust function enable, sub-function of IPE_CADJEn
                                    IPE_CADJ_CC_CONEn   |       ///< Color component CbCr contrast adjust function enable, sub-function of IPE_CADJEn
                                    IPE_YCTHEn          |       ///< Color component YCbCr substitution function enable, sub-function of IPE_CADJEn
                                    IPE_CSTPEn          |       ///< Color space transform protect function enable, RGB->YCC
                                    0
                                );
            group->ipeUpdate |= (
                                    IPE_SUBFEN       |   ///< update func enable/disable
                                    IPE_RGBGAMMA     |   ///< update rgb gamma dram addr
                                    IPE_YCURVE       |   ///< update y gamma dram addr
                                    IPE_EDGEEXT_INFO |   ///< update edge extraction
                                    IPE_EDGEENH_INFO |   ///< update positive/negative edge enhance/inverse
                                    IPE_EDGEMAP_INFO |   ///< update edge map
                                    IPE_RGBLPF_INFO  |   ///< update rgb low pass
                                    IPE_CC_INFO      |   ///< update color correction
                                    IPE_CST_INFO     |   ///< update color space transfor
                                    IPE_C_CTRL       |   ///< update color control
                                    IPE_C_CON        |   ///< update color contrast(Y/CC)
                                    IPE_C_OFS        |   ///< update Cb/Cr offset
                                    IPE_RANDNOISE    |   ///< update YCC random noise
                                    IPE_YCCFIX_INFO  |   ///< update YCC fix replace
                                    IPE_CSTP         |   ///< update color space transfor protection
                                    0
                                );

            if ( !(MsgID == IQS_MANUAL_TRIG || MsgID == IQCB_PRV_IPE_INIT || MsgID == IQCB_PREIME_D2D_IPE_PRI || MsgID == IQCB_PREIME_D2D_IPE_SUB || (IQS_TBL_UpdateBit[Id]&IQS_TBL_GAMMA)) )
                group->ipeUpdate &= ~IPE_RGBGAMMA;

            if ( !(MsgID == IQS_MANUAL_TRIG || MsgID == IQCB_PRV_IPE_INIT || MsgID == IQCB_PREIME_D2D_IPE_PRI || MsgID == IQCB_PREIME_D2D_IPE_SUB || (IQS_TBL_UpdateBit[Id]&IQS_TBL_YCURVE)) )
                group->ipeUpdate &= ~IPE_YCURVE;

            if ( !(MsgID == IQS_MANUAL_TRIG || MsgID == IQCB_PRV_IPE_INIT || MsgID == IQCB_PREIME_D2D_IPE_PRI || MsgID == IQCB_PREIME_D2D_IPE_SUB || (IQS_TBL_UpdateBit[Id]&IQS_TBL_3DCC)) )
                group->ipeUpdate &= ~IPE_3DCC_INFO;

            if ( IPL_AlgGetUIInfo(Id, IPL_SEL_IPPNR) == SEL_IPPNR_OFF )
                subf->IpeFuncEn &= ~IPE_RGBLPEn;

            if ( IPL_AlgGetUIInfo(Id, IPL_SEL_IPPEDGE) == SEL_IPPEDGE_OFF )
                subf->IpeFuncEn &= ~IPE_CADJ_Y_ENHEn;

            //////////////////
            // Dynamic Gamma & CC
            //////////////////
            if ( group->ipeUpdate & IPE_RGBGAMMA )
            {
                UINT32 aLv = 0;
                static UINT32 aLv_old[Total_SENIDNum] = {0, 0};
                #define IQS_LV_DAY_END     120
                #define IQS_LV_NIGHT_START 100
                if(IQS_IsCapFlow(MsgID)==FALSE)
                    aLv = AE_GetPrvLVValue(Id);
                else
                    aLv = AE_GetCapLVValue(Id);
                if( (aLv != aLv_old[SenId_Value]) && !((aLv > IQS_LV_DAY_END) && (aLv_old[SenId_Value] > IQS_LV_DAY_END)) && !((aLv < IQS_LV_NIGHT_START) && (aLv_old[SenId_Value] < IQS_LV_NIGHT_START)) )
                    Gamma_update[SenId_Value] = TRUE;
                aLv_old[SenId_Value] = aLv;

                if( Gamma_update[SenId_Value] == TRUE )
                {
                    IQS_CC_Intpl(IQS_CC_Night[SenId_Value], IQS_CC_Day[SenId_Value], IQS_LV_NIGHT_START, IQS_LV_DAY_END, aLv, Normal_CC[SenId_Value]);
                    IQS_Gamma_Intpl(IQS_GammaLut_Night[SenId_Value], IQS_GammaLut_Day[SenId_Value], IQS_LV_NIGHT_START, IQS_LV_DAY_END, aLv, IQS_GammaLut[SenId_Value]);
                    IQS_GammaAddr_Buf[SenId_Value] = (UINT32)IPE_GammaReorgon(SenId_Value, (UINT32 *)IQS_GammaLut[SenId_Value], (UINT32 *)IQS_GammaLut[SenId_Value], (UINT32 *)IQS_GammaLut[SenId_Value]);
                }
            }

            //////////////////
            // IPE-YCURVE
            //////////////////
            if ( group->ipeUpdate & IPE_YCURVE )
                if( Ycurve_update[SenId_Value] == TRUE )
                    if( flag_ycurve[SenId_Value] == FALSE )
                        IQS_YCurveAddr_Buf[SenId_Value] = (UINT32)IPE_YCurveReorgon(SenId_Value, (UINT32 *)IQS_YCurveLut_Buf[SenId_Value]);

            //////////////////
            // Edge Gamma
            //////////////////
            if (IPL_AlgGetUIInfo(Id, IPL_SEL_WDR) != SEL_WDR_OFF)
                memcpy(IQS_EdgeGammaLut[SenId_Value], IQS_EdgeGammaLut_Buf[SenId_Value], sizeof(UINT32)*65);
            else if( Gamma_update[SenId_Value] == TRUE )
                memcpy(IQS_EdgeGammaLut[SenId_Value], IPE_xfer2EdgeGamma(SenId_Value, (UINT32 *)IQS_GammaLut[SenId_Value]), sizeof(UINT32)*65);

            //////////////////
            // IPE-Edge Ext
            //////////////////
            IQS_EdgeExt[SenId_Value].GamSel = E_POS_GAMMA;
            IQS_EdgeExt[SenId_Value].ChSel = EDGE_Y_CH;
            IQS_EdgeExt[SenId_Value].EEXT_TblAddr = (UINT32)IQS_EdgeKernel[SenId_Value];
            IQS_EdgeExt[SenId_Value].EdgeDiv = EdgeDiv_2;
            IQS_EdgeExt[SenId_Value].EDirTabAddr = (UINT32)IQS_EDirTab[SenId_Value];
            IQS_EdgeExt[SenId_Value].pDirEdgeExt = &IQS_DirEdgeExt[ISO_Value];
            IQS_EdgeExt[SenId_Value].ScrCalTab = &IQS_DirScoreTab[SenId_Value];
            IQS_EdgeExt[SenId_Value].ScrCalThTab = &IQS_DirScoreTh_Buf[SenId_Value];
            IQS_EdgeExt[SenId_Value].PowerSav = TRUE; // TRUE: DIRCONEN = DIRCON2EN = 0; FALSE: DIRCONEN = DIRCON2EN = 1
            IQS_EdgeExt[SenId_Value].ConParam = &IQS_DECon[SenId_Value];
            IQS_EdgeExt[SenId_Value].pEWA = &IQS_EWA[SenId_Value];
            IQS_EdgeExt[SenId_Value].pEWB = &IQS_EWB[SenId_Value];
            IQS_EdgeExt[SenId_Value].pEWParam = &IQS_EWParam_Buf[SenId_Value];
            IQS_EdgeExt[SenId_Value].ToneMapLutTabAddr = (UINT32)IQS_EdgeGammaLut[SenId_Value];

            //////////////////
            // IPE-Edge Enhance
            //////////////////
            IQS_EdgeEnh_Buf[SenId_Value].EnhP = (IQS_EdgeEnh_Buf[SenId_Value].EnhP * Sharpness[ISO_Value])>>7;
            IQS_EdgeEnh_Buf[SenId_Value].EnhN = (IQS_EdgeEnh_Buf[SenId_Value].EnhN * Sharpness[ISO_Value])>>7;

            //////////////////
            // IPE-Edge Map
            //////////////////
            IQS_EDMap_Buf[SenId_Value].TAB_TblAddr = (UINT32)IQS_EDTab_Buf[SenId_Value];
            IQS_ESMap_Buf[SenId_Value].TAB_TblAddr = (UINT32)IQS_EStab_Buf[SenId_Value];
            IQS_EdgeMap[SenId_Value].edMap = IQS_EDMap_Buf[SenId_Value];
            IQS_EdgeMap[SenId_Value].esMap = IQS_ESMap_Buf[SenId_Value];
            IQS_EdgeMap[SenId_Value].EMapOut = _EEXT;

            //////////////////
            // IPE-RGBLPF
            //////////////////
            IQS_RGBLpf_Buf[SenId_Value].G = IQS_RGBLpf_Buf[SenId_Value].R;
            IQS_RGBLpf_Buf[SenId_Value].B = IQS_RGBLpf_Buf[SenId_Value].R;

            //////////////////
            // IPE-Color Correction
            //////////////////
            IQS_ColorCorrection[SenId_Value].CCcoef_TblAddr = (UINT32)Normal_CC[SenId_Value];
            IQS_ColorCorrection[SenId_Value].Fstab_TblAddr = (UINT32)IQS_FStab_Buf[SenId_Value];
            IQS_ColorCorrection[SenId_Value].Fdtab_TblAddr = (UINT32)IQS_FDtab_Buf[SenId_Value];

            //////////////////
            // IPE-color space transfor
            //////////////////
            IQS_CSTParam[SenId_Value].CSTCoefTabAddr = (UINT32)IQS_CSTCoef[SenId_Value];

            ///////////////////
            // IPE-Image Effect
            ///////////////////
            if     ( IMEffect_Value == SEL_IMAGEEFFECT_BW )    ColorEffectTable = ColorEffectTable_BW;
            else if( IMEffect_Value == SEL_IMAGEEFFECT_SEPIA ) ColorEffectTable = ColorEffectTable_SEPIA;
            else if( IMEffect_Value == SEL_IMAGEEFFECT_VIVID ) ColorEffectTable = ColorEffectTable_VIVID;
            else                                               ColorEffectTable = ColorEffectTable_Normal;

            ///////////////////
            // IPE-Color Control
            ///////////////////
            //#NT#2016/02/19#Kristin Huang -begin
            //#NT# Move IQS_WDR_Sat_Buf adding from IQS_Cctrl.SatOfs to IQS_YCCcon.UVCon
            IQS_Cctrl[SenId_Value].SatOfs = ColorEffectTable.SatOfs;
            if (IPL_AlgGetUIInfo(Id, IPL_SEL_WDR) != SEL_WDR_OFF)
                IQS_Cctrl[SenId_Value].SatOfs = (INT16)IQS_CLAMP(IQS_Cctrl[SenId_Value].SatOfs + IQS_WDR_Sat_Buf[SenId_Value], -128, 127);
            //#NT#2016/02/19#Kristin Huang -end
            IQS_Cctrl[SenId_Value].IntOfs = (INT16)IQS_CLAMP(ColorEffectTable.IntOfs + IQS_Cctrl_IntOfs_Buf[SenId_Value], -128, 127);
            IQS_Cctrl[SenId_Value].HueRotateEn = ColorEffectTable.HueRotateEn;
            IQS_Cctrl[SenId_Value].CctrlSel = CCTRL_SEL_1STORDER;
            IQS_Cctrl[SenId_Value].uiVdetDiv = 24;
            IQS_Cctrl[SenId_Value].HueTab_TblAddr = (UINT32)IQS_hueTab_Buf[SenId_Value];
            IQS_Cctrl[SenId_Value].SatTab_TblAddr = (UINT32)IQS_satTab[SenId_Value];
            IQS_Cctrl[SenId_Value].IntTab_TblAddr = (UINT32)IQS_intTab[SenId_Value];
            IQS_Cctrl[SenId_Value].EdgTab_TblAddr = (UINT32)IQS_edgTab[SenId_Value];
            IQS_Cctrl[SenId_Value].DDSTab_TblAddr = (UINT32)IQS_ddsTab[SenId_Value];

            ///////////////////
            // IPE-Y/CC Contrast
            ///////////////////
            //#NT#2016/02/22#Kristin Huang -begin
            //#NT# Gray mode when IRcut night mode
            if((IPL_AlgGetUIInfo(Id, IPL_SEL_IRCUT) == SEL_IRCUT_AUTO && AE_GetIRCutEnable(Id) == FALSE) || IPL_AlgGetUIInfo(Id, IPL_SEL_IRCUT) == SEL_IRCUT_NIGHT)
                IQS_YCCcon[SenId_Value].UVCon = 0;
			//#NT#2017/03/07#Wendy Liao -begin
			//#NT# Gray setting at Special Mode
            else if((IMEffect_Value == SEL_IMAGEEFFECT_BW) || (IMEffect_Value == SEL_IMAGEEFFECT_SEPIA))
                IQS_YCCcon[SenId_Value].UVCon = (INT8)ColorEffectTable.CCon;
            //#NT#2017/03/07#Wendy Liao -end
			else
                IQS_YCCcon[SenId_Value].UVCon = (UINT8)IQS_CLAMP((INT16)ColorEffectTable.CCon/*=128*/ + Saturation_Buf[SenId_Value]/*-128~127*/, 0, 255);
            //#NT#2016/02/22#Kristin Huang -end
            IQS_YCCcon[SenId_Value].YCon = (UINT8)IQS_CLAMP((INT16)ColorEffectTable.YCon/*=128*/ + Contrast_Buf[SenId_Value]/*-128~127*/, 0, 255);

            ///////////////////
            // IPE-Cb/Cr Color Offset
            ///////////////////
            IQS_CCOfs[SenId_Value].Cbofs = ColorEffectTable.CbOfs;
            IQS_CCOfs[SenId_Value].Crofs = ColorEffectTable.CrOfs;

            ///////////////////
            // IPE-Y/CC Fix
            ///////////////////
            switch (IMEffect_Value)
            {
                case SEL_IMAGEEFFECT_COLORPENCIL:
                    memcpy(&IQS_YCCFix_Buf[SenId_Value], &IE_ColorPencil_Param, sizeof(IPE_YCCFIX));
                    break;

                case SEL_IMAGEEFFECT_SKETCH:
                    memcpy(&IQS_YCCFix_Buf[SenId_Value], &IE_PencilSketch_Param, sizeof(IPE_YCCFIX));
                    IQS_EdgeEnh_Buf[SenId_Value].InvP = ENABLE;
                    IQS_EdgeEnh_Buf[SenId_Value].InvN = DISABLE;
                    break;

                case SEL_IMAGEEFFECT_ROCK:
                    memcpy(&IQS_YCCFix_Buf[SenId_Value], &IPE_RockEffect_YccFix, sizeof(IPE_YCCFIX));
                    //IQS_IPE_CSTP.CSTPratio = 15;
                    break;

                case SEL_IMAGEEFFECT_CCID:
                    IQS_Cctrl[SenId_Value].SatTab_TblAddr = (UINT32)IPE_SelectiveCC_SatTab[iCCID_Value];
                    // fall through

                default:
                    memcpy(&IQS_YCCFix_Buf[SenId_Value], &IQS_YCCFix[SenId_Value], sizeof(IPE_YCCFIX));
                    break;
            }

            ///////////////////
            // IPE-Image Setting Adjust
            ///////////////////
            subf->pedgeExt = &IQS_EdgeExt[SenId_Value];
            subf->pedgeEnh = &IQS_EdgeEnh_Buf[SenId_Value];
            subf->pedgeMap = &IQS_EdgeMap[SenId_Value];
            subf->prgblpf = &IQS_RGBLpf_Buf[SenId_Value];
            subf->pCC = &IQS_ColorCorrection[SenId_Value];
            subf->pCST = &IQS_CSTParam[SenId_Value];
            subf->pCctrl = &IQS_Cctrl[SenId_Value];
            subf->pYCCcon = &IQS_YCCcon[SenId_Value];
            subf->pCbCrofs = &IQS_CCOfs[SenId_Value];
            subf->pRandNoise = &IQS_NoiseParam[SenId_Value];
            subf->pYccFix = &IQS_YCCFix_Buf[SenId_Value];
            if ( group->ipeUpdate & IPE_3DCC_INFO )
                subf->p3d_cc = &IQS_3DColorCtrl;
            if ( group->ipeUpdate & IPE_RGBGAMMA )
                subf->GammaAddr = IQS_GammaAddr_Buf[SenId_Value];
            if ( group->ipeUpdate & IPE_YCURVE )
                subf->YGammaAddr = IQS_YCurveAddr_Buf[SenId_Value];
            if ( group->ipeUpdate & IPE_GAMRAND )
                subf->pGamRand = &IQS_GammaRand[SenId_Value];

            Gamma_update[SenId_Value] = FALSE;
            Ycurve_update[SenId_Value] = FALSE;
            break;

        case IQS_AE_END:
        case IQS_IMAGE_EFFECT:
        default:
            DBG_ERR("IPE_IQparam() param mode(%d) selection error!\r\n",MsgID);
            ret = E_SYS;
         break;
    }
    return ret;
}

static ER IFE2_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IFE2_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    UINT32 SenId_Value = IQS_GetSensorIdx(Id);

    switch((UINT32)MsgID)
    {
        case IQCB_PRV_IFE2_INIT:
        case IQS_AUTO_TRIG:
        case IQS_MANUAL_TRIG:
        case IQCB_D2D_IFE2_SUB:
        case IQCB_D2D_IFE2_PRI:
        case IQS_AE_END:
            group->ife2Update |= (
                                    IFE2_REFCENTERPARA |    //< update reference center para.
                                    IFE2_FILTERPARAM   |    //< update filter para.
                                    IFE2_GRAYSTAL      |    //< update filter para.
                                    0
                                 );
            subf->ife2FuncEn |=  (
                                    YFtrEn             |    ///< enable y filter
                                    0
                                 );

            //////////////////
            // IFE2-Reference Center
            //////////////////
            IQS_RefCenter_Y_Buf[SenId_Value].uiRThAddr = (UINT32)IQS_RefCenter_YTh_Buf[SenId_Value];
            IQS_RefCenter_Y_Buf[SenId_Value].uiRWtAddr = (UINT32)IQS_RefCenter_YWt_Buf[SenId_Value];
            IQS_RefCenter[SenId_Value].RefCentY = IQS_RefCenter_Y_Buf[SenId_Value];
            IQS_RefCenter_UV_Buf[SenId_Value].uiRThAddr = (UINT32)IQS_RefCenter_UVTh_Buf[SenId_Value];
            IQS_RefCenter_UV_Buf[SenId_Value].uiRWtAddr = (UINT32)IQS_RefCenter_UVWt_Buf[SenId_Value];
            IQS_RefCenter[SenId_Value].RefCentUV = IQS_RefCenter_UV_Buf[SenId_Value];
            subf->pRefcent = &IQS_RefCenter[SenId_Value];

            //////////////////
            // IFE2-Filter
            //////////////////
            IQS_IFE2Filter_Buf[SenId_Value].Filter.FiltSetY.uiFtrThAddr = (UINT32)IQS_IFE2Filter_YTh_Buf[SenId_Value];
            IQS_IFE2Filter_Buf[SenId_Value].Filter.FiltSetY.uiFtrWtAddr = (UINT32)IQS_IFE2Filter_YWt_Buf[SenId_Value];
            IQS_IFE2Filter_Buf[SenId_Value].Filter.FiltSetU.uiFtrThAddr = (UINT32)IQS_IFE2Filter_UVTh_Buf[SenId_Value];
            IQS_IFE2Filter_Buf[SenId_Value].Filter.FiltSetV.uiFtrThAddr = (UINT32)IQS_IFE2Filter_UVTh_Buf[SenId_Value];
            IQS_IFE2Filter_Buf[SenId_Value].Filter.FiltSetU.uiFtrWtAddr = (UINT32)IQS_IFE2Filter_UVWt_Buf[SenId_Value];
            IQS_IFE2Filter_Buf[SenId_Value].Filter.FiltSetV.uiFtrWtAddr = (UINT32)IQS_IFE2Filter_UVWt_Buf[SenId_Value];
            subf->pFilter = &IQS_IFE2Filter_Buf[SenId_Value].Filter;

            //////////////////
            // IFE2-Gray Statistical
            //////////////////
            subf->GrayStatal = IQS_GrayStatical;
            break;

        case IQS_AWB_END:
        case IQS_OZOOM_END:
        case IQS_IMAGE_EFFECT:
        default:
            DBG_ERR("IFE2_IQparam() param mode(%d) selection error!\r\n",MsgID);
            break;
    }
    return E_OK;
}

static ER IME_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, IME_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    UINT32 SenId_Value = IQS_GetSensorIdx(Id);

    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
        case IQS_AUTO_TRIG:
        case IQCB_PRV_IME_INIT:
        case IQCB_PREIME_D2D_IME_PRI:
            subf->imeFuncEn |= (
                                    ChromaAdaptEn   |   ///< enable function: chroma adaption (LCA)
                                    ChromaMedianEn  |   ///< enable function: chroma median filter
                                    ChromaAdjLchrom |   ///< enable function: chroma adjustment of local chroma adaptation
                                    ChromaAdjLum    |   ///< enable function: chroma adjustment of local luma adaptation
                                    ChromaDBSuppres |   ///< enable function: dark and bright region chroma suppression
                                    //SingleImgSuperR |   ///< enable function: single image super-resolution
                                    0
                                );
            group->imeUpdate |= (
                                    IME_SUBFEN      |   ///< update function enable/disable
                                    IME_CHROMAA     |   ///< update chroma adaption
                                    IME_CHROMAA_LUM |   ///< update chroma lumaination adaption
                                    IME_CHROMAA_CLR |   ///< update chroma color adaption
                                    IME_CHROMAA_CTR_UV |   ///< update chroma adjust6ment center for UV channel(data from IFE2)
                                    IME_CHROMADB_SUP|   ///< update chroma suppression.
                                    IME_SUPER_RES   |   ///< update super resoltuion info
                                    0
                                );

            if ( IQS_IsChromaAdaptEn(Id, IQS_IsCapFlow(MsgID)) == DISABLE )
                 subf->imeFuncEn &= (~ChromaAdaptEn);

            if ( IPL_AlgGetUIInfo(Id, IPL_SEL_IPPNR) == SEL_IPPNR_OFF )
                subf->imeFuncEn &= ~(ChromaMedianEn | ChromaDBSuppres | ChromaAdjLum | ChromaAdjLchrom);

            //////////////////
            // IME-Chroma Adaption
            //////////////////
            subf->ChromaAdap.pChromaAdaption = &IQS_ChromaAdapt_Buf[SenId_Value];
            subf->ChromaAdap.pChromaLumAdaption = &IQS_ChromaAdapt_Lum_Buf[SenId_Value];
            subf->ChromaAdap.pChromaColorAdaption = &IQS_ChromaAdapt_Color_Buf[SenId_Value];

            //////////////////
            // IME-Chroma Suppression
            //////////////////
            IQS_ChromaSupp[SenId_Value].puiWeightY = IQS_ChromaSupp_WeightY[SenId_Value];
            IQS_ChromaSupp[SenId_Value].puiWeightC = IQS_ChromaSupp_WeightC[SenId_Value];
            subf->pChromaSuppres = &IQS_ChromaSupp[SenId_Value];

            //////////////////
            // IME-Super Resolution
            //////////////////
            subf->pSuperRes = &IQS_SuperRes;
            break;

        case IQCB_PREIME_D2D_IME_SUB:
        case IQS_AE_END:
        case IQS_DZOOM_END:
        case IQS_AWB_END:
        case IQS_OZOOM_END:
        case IQS_IMAGE_EFFECT:
        default:
            DBG_ERR("IME_IQparam() param mode(%d) selection error!\r\n",MsgID);
            return E_SYS;
    }

#if 1
    // LCA off when image flip index changes
    if ((UINT32)MsgID == IQS_AUTO_TRIG)
    {
        static UINT32 cnt[Total_SENIDNum] = {0, 0}, ImageFlipIdx[Total_SENIDNum] = {0, 0};
        if( ImageFlipIdx[SenId_Value] != IPL_AlgGetUIInfo(Id, IPL_SEL_IMAGEFLIP) )
        {
            subf->imeFuncEn &= ~(ChromaAdaptEn | ChromaMedianEn | ChromaDBSuppres | ChromaAdjLum | ChromaAdjLchrom);
            cnt[SenId_Value]++;
            if( cnt[SenId_Value] == 5 )
            {
                ImageFlipIdx[SenId_Value] = IPL_AlgGetUIInfo(Id, IPL_SEL_IMAGEFLIP);
                cnt[SenId_Value] = 0;
            }
        }
    }
#endif

    return E_OK;
}

static ER DRE_IQparam(UINT32 Id, IQS_FLOW_MSG MsgID, DRE_SUBFUNC *subf,IPL_HAL_GROUP* group)
{
    UINT32 idx, i;
    UINT32 ISO_Value = IQS_GetISOIdx(Id, MsgID);

    switch((UINT32)MsgID)
    {
        case IQCB_D2D_DRE_PRI:
        case IQS_MANUAL_TRIG:
            group->dreUpdate |= (
                                    DRE_UPDATE_INIT  |
                                    DRE_UPDATE_NR    |
                                    0
                                );

            //HDR DeGhost
            subf->DgCtrl.bDgImgRef = IQS_MSNR[ISO_Value].bDgImgRef;
            subf->DgCtrl.bDgEn     = IQS_MSNR[ISO_Value].bDgEn;
            subf->DgCtrl.uiDgLyrNum = IQS_MSNR[ISO_Value].uiDgLyrNum;
            for ( i =0; i<DRE_DG_PROC_MAX_NUM; i++ )
            {
                idx = IQS_MSNR[ISO_Value].DgMskInIdx[i];
                if ( idx >= IQS_MAX_DGMASK_NUM )
                {
                    DBG_ERR("Wrong DGMask Index (ISO_Value = %d, i = %d, Idx = %d)\r\n", ISO_Value, i, idx);
                    idx = 0 ;
                }
                subf->DgCtrl.DgMskIn[i] = IQS_DRE_DGMask[idx];
            }

            //MSNR
            subf->uiProcRepNum = IQS_MSNR[ISO_Value].uiProcRepNum;
            for ( i =0; i<subf->uiProcRepNum; i++ )
            {
                idx = IQS_MSNR[ISO_Value].uiProcRepIdx[i];
                if ( idx == 0 )
                {
                    DBG_ERR("Wrong MSNR Index (ISO_Value = %d, i = %d, Idx = %d)\r\n", ISO_Value, i, idx);
                    return E_OK;
                }

                subf->NrJBFilt[i].puiWt      = IQS_MSNR_NR[idx]->IQS_Msnr_Filt_Weight;
                subf->NrJBFilt[i].puiRngThrY = IQS_MSNR_NR[idx]->IQS_Msnr_Filt_RngThr_Y;
                subf->NrJBFilt[i].puiRngThrU = IQS_MSNR_NR[idx]->IQS_Msnr_Filt_RngThr_U;
                subf->NrJBFilt[i].puiRngThrV = IQS_MSNR_NR[idx]->IQS_Msnr_Filt_RngThr_V;

                subf->bNrGamYEn[i] = IQS_MSNR_NR[idx]->IQS_Msnr_GamLutY_En;
                subf->bNrGamUEn[i] = IQS_MSNR_NR[idx]->IQS_Msnr_GamLutU_En;
                subf->bNrGamVEn[i] = IQS_MSNR_NR[idx]->IQS_Msnr_GamLutV_En;
                subf->GamLut[i].puiLutY = IQS_MSNR_NR[idx]->IQS_Msnr_GamLut_Y;
                subf->GamLut[i].puiLutU = IQS_MSNR_NR[idx]->IQS_Msnr_GamLut_U;
                subf->GamLut[i].puiLutV = IQS_MSNR_NR[idx]->IQS_Msnr_GamLut_V;

                subf->YCMTbl[i].puiYcLutY = IQS_MSNR_NR[idx]->IQS_Msnr_YCMLut_Y;
                subf->YCMTbl[i].puiYcLutC = IQS_MSNR_NR[idx]->IQS_Msnr_YCMLut_C;

                subf->YCMCtrl[i] = IQS_MSNR_NR[idx]->IQS_Msnr_YCMCtrl;
            }
            break;

        default:
            DBG_ERR("DRE_IQparam() param mode(%d) selection error!\r\n",MsgID);
            break;
    }
    return E_OK;
}
//#NT#2016/04/27#Ted Lin -begin
//#NT# for WDR2.5
static ER IQS_SetWDRParam(UINT32 Id,IPL_HAL_PARASET  HalId)
{
            UINT32 SenId_Value = IQS_GetSensorIdx(Id);
    if (IPL_CtrlGetInfor(Id, IPLCTRL_WDR_VER) == WDR_VERSION_2_0)
    {
            if (IPL_AlgGetUIInfo(Id, IPL_SEL_WDR) == SEL_WDR_ON)
            {
                SR_WDR_PARAM WdrRslt = ExtFunc_GetWDRResult(Id);
                flag_ycurve[SenId_Value] = TRUE;
                memcpy(IQS_YCurveLut_Buf[SenId_Value], WdrRslt.puiGetToneCurve, sizeof(UINT32)*130);
                flag_ycurve[SenId_Value] = FALSE;
                memcpy(IQS_EdgeGammaLut_Buf[SenId_Value], WdrRslt.puiGetEdgeCurve, sizeof(UINT32)*65);
                IQS_WDR_Sat_Buf[SenId_Value] = *WdrRslt.piGetSat;
            }
            else
            {
                flag_ycurve[SenId_Value] = TRUE;
                memcpy(IQS_YCurveLut_Buf[SenId_Value], IQS_YCurveLut[SenId_Value], sizeof(UINT32)*130);
                flag_ycurve[SenId_Value] = FALSE;
            }
            Ycurve_update[SenId_Value] = TRUE;
            IQS_SetUpdateBit(Id, (IQS_ENG_IPE|IQS_TBL_YCURVE));
    }


     if (IPL_CtrlGetInfor(Id, IPLCTRL_WDR_VER) == WDR_VERSION_2_5)
     {
        if (IPL_AlgGetUIInfo(Id, IPL_SEL_WDR) != SEL_WDR_OFF)
        {
               SR_WDR_PARAM WdrRslt = ExtFunc_GetWDRResult(Id);
               UINT32 *ptr;

               //RHE
               ptr = WdrRslt.puiGetBandGain;
               IQS_LocTnInfo_buf[Id].uiBand0Gain = *ptr;
               IQS_LocTnInfo_buf[Id].uiBand1Gain = *(ptr+1);

               //IPE Ycurve
               flag_ycurve[Id] = TRUE;
               memcpy(IQS_YCurveLut_Buf[Id], WdrRslt.puiGetToneCurve, sizeof(UINT32)*130);
               //IQS_YCurveAddr_Buf[Id] = (UINT32)IPE_YCurveReorgon(Id, (UINT32 *)IQS_YCurveLut_Buf[Id]);
               Ycurve_update[Id] = TRUE;
               flag_ycurve[Id] = FALSE;
               memcpy(IQS_EdgeGammaLut_Buf[Id], WdrRslt.puiGetEdgeCurve, sizeof(UINT32)*65);
               IQS_WDR_Sat_Buf[Id] = *WdrRslt.piGetSat;

               //set to IQS_ctrl
               //IQS_setWDR_IQparam(Id,HalId,param);

            }else
            {
                flag_ycurve[Id] = TRUE;
                memcpy(IQS_YCurveLut_Buf[Id], IQS_YCurveLut[Id], sizeof(UINT32)*130);
                flag_ycurve[Id] = FALSE;
                //RHE
                IQS_LocTnInfo_buf[Id].uiBand0Gain = 512;
                IQS_LocTnInfo_buf[Id].uiBand1Gain = 512;
                //runtime change to close WDR
                Ycurve_update[Id] = TRUE;

                //IQS_setWDR(FALSE);
            }
            IQS_SetUpdateBit(Id, (IQS_ENG_RHE | IQS_ENG_IPE|IQS_TBL_YCURVE));
     }




    return E_OK;

}
 //#NT#2016/04/27#Ted Lin -end
///////////////////////////////////////////////
// FlowSetting Interface
///////////////////////////////////////////////
ER IQS_SettingFlowFunc(UINT32 Id, IQS_FLOW_MSG MsgID)
{
    IPL_HAL_PARASET HalId = IPL_CtrlGetHalSetId(Id);
    SENSOR_INFO SenInfo;
    UINT32 ISO_Value, SenId_Value = IQS_GetSensorIdx(Id);

    //#NT#2016/09/02#XL_Wei -begin
    //#NT#Update ISO Index for iqs tuning
    gISO_Index[Id].curIdx = IQS_GetISOIdx(Id, MsgID);
    //#NT#2016/09/02#XL_Wei -end

    switch((UINT32)MsgID)
    {
        case IQS_MANUAL_TRIG:
            IQS_Intpl_ISO(Id, MsgID);
            IQS_SIEx_IQparam(Id, MsgID, HalId, SIEx_IQparam);
            IQS_DCE_IQparam(Id, MsgID, HalId, DCE_IQparam);
            IQS_IFE_IQparam(Id, MsgID, HalId, IFE_IQparam);
            IQS_IPE_IQparam(Id, MsgID, HalId, IPE_IQparam);
            IQS_IFE2_IQparam(Id, MsgID, HalId, IFE2_IQparam);
            IQS_IME_IQparam(Id, MsgID, HalId, IME_IQparam);
            break;

        case IQS_AUTO_TRIG:
            IQS_Intpl_ISO(Id, MsgID);
            IQS_TBL_UpdateBit[SenId_Value] = IQS_GetUpdateBit(Id) & IQS_TBL_MASK;
            IQS_ENG_UpdateBit[SenId_Value] = IQS_GetUpdateBit(Id) & IQS_ENG_MASK;
            IQS_ClearUpdateBit(Id);
            if ( IQS_ENG_UpdateBit[SenId_Value] & IQS_ENG_SIE ) IQS_SIEx_IQparam(Id, MsgID, HalId, SIEx_IQparam);
            if ( IQS_ENG_UpdateBit[SenId_Value] & IQS_ENG_DCE ) IQS_DCE_IQparam(Id, MsgID, HalId, DCE_IQparam);
            if ( IQS_ENG_UpdateBit[SenId_Value] & IQS_ENG_IFE ) IQS_IFE_IQparam(Id, MsgID, HalId, IFE_IQparam);
            if ( IQS_ENG_UpdateBit[SenId_Value] & IQS_ENG_IPE ) IQS_IPE_IQparam(Id, MsgID, HalId, IPE_IQparam);
            if ( IQS_ENG_UpdateBit[SenId_Value] & IQS_ENG_IFE2) IQS_IFE2_IQparam(Id, MsgID, HalId, IFE2_IQparam);
            if ( IQS_ENG_UpdateBit[SenId_Value] & IQS_ENG_IME ) IQS_IME_IQparam(Id, MsgID, HalId, IME_IQparam);
            if ( IQS_ENG_UpdateBit[SenId_Value] & IQS_ENG_RHE ) IQS_RHE_IQparam(Id, MsgID, HalId, RHE_IQparam);
            IQS_SetIsrUpdateBit(Id, IQS_ENG_UpdateBit[SenId_Value]);
            IQS_ENG_UpdateBit[SenId_Value] = 0;
            IQS_TBL_UpdateBit[SenId_Value] = 0;
            break;

        case IQS_AWB_END:
            SenInfo.Mode = NULL;
            IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
            if (SenInfo.Mode == NULL)
            {
                DBG_ERR("NULL Info.Mode\r\n");
                return E_SYS;
            }
            if (SenInfo.Mode->ModeType == SENSOR_MODE_STAGGER_HDR)
            {
                IQS_SIEx_IQparam(IPL_ID_1, MsgID, HalId, SIEx_IQparam);
                IQS_SIEx_IQparam(IPL_ID_2, MsgID, HalId, SIEx_IQparam);
            }
            else
            {
                IQS_SetUpdateBit(Id, (IQS_ENG_SIE));
            }
            IQS_SetUpdateBit(Id, (IQS_ENG_IFE | IQS_ENG_IPE | IQS_TBL_GAMMA | IQS_TBL_YCURVE));
            break;

        case IQS_AE_END:
            ISO_Value = IQS_GetISOIdx(Id, MsgID);
            IQUCIF_SetCtrlIndex(Id, ISO_Value);
            IQS_SetUpdateBit(Id, (IQS_ENG_IFE | IQS_ENG_IPE | IQS_ENG_IFE2 | IQS_ENG_IME));
            break;

        case IQS_OZOOM_END:
            IQS_SetUpdateBit(Id, (IQS_ENG_SIE | IQS_ENG_DCE | IQS_ENG_IFE | IQS_TBL_ECS | IQS_TBL_3DCC));
            break;

        case IQS_DZOOM_END:
            //Dzoom crop between SIE & IFE
            //no ratio change, IFE & DCE no need to change parameter
            //do not impact ECS
            //IQS_SetUpdateBit(Id, (IQS_ENG_IFE | IQS_ENG_DCE | IQS_TBL_ECS));
            break;

        case IQS_IMAGE_EFFECT:
            IQS_SetUpdateBit(Id, (IQS_ENG_DCE | IQS_ENG_IPE));
            break;

        case IQS_WDR_END:
//#NT#2016/04/27#Ted Lin -begin
//#NT# for WDR2.5
            IQS_SetWDRParam(Id,HalId);
 //#NT#2016/04/27#Ted Lin -end
            break;

        default:
            DBG_ERR("^R-E- MsgID(%x)\r\n", MsgID);
            break;
    }
    return E_OK;
}

///////////////////////////////////////////////
// CBSetting Interface
///////////////////////////////////////////////

ER IQCB_SettingFlowFunc(UINT32 Id, IQS_FLOW_MSG MsgID, ENG_SUBFUNC *Info, IPL_HAL_GROUP* group)
{
    UINT32 SenId_Value = IQS_GetSensorIdx(Id);

    switch((UINT32)MsgID)
    {
        //preview
        case IQCB_PRV_SIE_INIT:
        case IQCB_PREIME_D2D_SIE_PRI:
        case IQCB_PREIME_D2D_SIE_SUB:
            IQS_Intpl_ISO(Id, MsgID);
            if(Info->pSie_sub == NULL || group == NULL)
                return E_PAR;
            SIEx_IQparam(Id, MsgID, Info->pSie_sub, group);
            break;

        case IQCB_PRV_IPE_INIT:
            Gamma_update[SenId_Value] = TRUE;
            Ycurve_update[SenId_Value] = TRUE;
            // fall through
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
            if(Info->pIme_sub == NULL || group == NULL)
                return E_PAR;
            IME_IQparam(Id, MsgID, Info->pIme_sub, group);
            break;

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

        case IQCB_PRV_RDE_INIT:
            break;

        case IQCB_D2D_DRE_PRI:
            if(Info->pDre_sub == NULL || group == NULL)
                return E_PAR;
            DRE_IQparam(Id, MsgID, Info->pDre_sub, group);
            break;
//#NT#2016/04/27#Ted Lin -begin
//#NT# for WDR2.5 init
        case IQCB_PRV_RHE_INIT:
            if(Info->pRhe_sub == NULL || group == NULL)
                return E_PAR;
            RHE_IQparam(Id, MsgID, Info->pRhe_sub, group);
            break;
 //#NT#2016/04/27#Ted Lin -end
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
    if(Enable)
        IQS_IPCAM_ENABLE = 1;
}
//#NT#2016/12/22#Wendy Liao -end

void IQS_Init(UINT32 Id)
{
    UINT32 SenId_Value = IQS_GetSensorIdx(Id);

    IQS_ENG_UpdateBit[SenId_Value] = 0;
    IQS_TBL_UpdateBit[SenId_Value] = 0;

    memcpy(IQS_Vig_Tab_Buf[SenId_Value],   IQS_Vig_Set[SenId_Value].Tab,  sizeof(UINT32)*17);
    //memcpy(IQS_DCE_GLUT_Buf[SenId_Value],  IQS_DCE_GLUT_Init,     sizeof(UINT32)*65);
    memcpy(IQS_hueTab_Buf[SenId_Value],    IQS_hueTab[SenId_Value],       sizeof(UINT8)*24);
    memcpy(IQS_YCurveLut_Buf[SenId_Value], IQS_YCurveLut[SenId_Value], sizeof(UINT32)*130);
    memcpy(IQS_GammaLut[SenId_Value], IQS_GammaLut_Day[SenId_Value], sizeof(UINT32)*130);
    memcpy(Normal_CC[SenId_Value], IQS_CC_Day[SenId_Value], sizeof(UINT16)*9);


    iqs_param_init(SenId_Value);
   
	Note_iqstInit(SenId_Value);

    IQUCIF_InitCtrlItem(Id);

#if IQUC_ENABLE
    UINT32 Addr, Size;
    IPL_BufGet(Id, IQUCBUF_MCTFOUT_1, &Addr, &Size);
    DBG_IND("IQUC BufAddr = 0x%08x, BufSize = %d\r\n", Addr, Size);
    IQUCIF_SetBufferInfo(Id, Addr, Size);
#endif
}


void iqs_param_init(UINT32 id)
{
	//iqs_set_range
	UINT32 *iq_param_addr[IQ_PARAM_MAX];
    static const UINT32 isoidx_offset[Total_SENIDNum] = {0, PRV2ISO0050};
    const UINT32 idxRange[Total_SENIDNum] = {PRV2ISO0050, Total_ISONum-PRV2ISO0050};
	static UINT32 num_3DNR_param = Total_UI_3DNR;
	gISO_Index[id].Id = id;
    gISO_Index[id].curIdx = 0;
    gISO_Index[id].num = idxRange[id];
	gISO_Index[id].offset = isoidx_offset[id];

	iq_param_addr[IQ_ISOIDX] = (UINT32*)&gISO_Index[id];

	iq_param_addr[IQ_ISOIDX_OFFSET	] = (UINT32*)&isoidx_offset[id];

	//iq_param_addr[IQ_OB_HDR	] = (UINT32*)&IQS_OBSub_BHDR[id];
	iq_param_addr[IQ_ECS_SET] = (UINT32*)&IQS_ECSSet;
	iq_param_addr[IQ_ECS_TAB	] =(UINT32*)IQS_ECSTab[id];

	iq_param_addr[IQ_VIG_SET	] = (UINT32*)&IQS_Vig_Set[id];

	iq_param_addr[IQ_DCE_CA	] = (UINT32*)&IQS_DCE_ColorAbbration[id];
	iq_param_addr[IQ_GDC_GLUT	] = (UINT32*)IQS_DCE_GLUT[id];
	iq_param_addr[IQ_CFAINTER	] = (UINT32*)&IQS_CFAInter[id];

	iq_param_addr[IQ_EDGEKER	] = (UINT32*)IQS_EdgeKernel[id];
	iq_param_addr[IQ_CST_COEF	] = (UINT32*)IQS_CSTCoef[id];
	iq_param_addr[IQ_YCCFIX	] = (UINT32*)&IQS_YCCFix[id];

	iq_param_addr[IQS_CCM_L	] = (UINT32*)IQS_CC_Day[id];//IQS_UserDefine_CC[id][0];
	iq_param_addr[IQS_CCM_M	] = (UINT32*)IQS_CC_Day[id];//IQS_UserDefine_CC[id][1];
	iq_param_addr[IQS_CCM_H	] = (UINT32*)IQS_CC_Day[id];//IQS_UserDefine_CC[id][2];

	iq_param_addr[IQ_GAMMA_DAY	] = (UINT32*)IQS_GammaLut_Day[id];
	iq_param_addr[IQ_GAMMA_NIGHT	] = (UINT32*)IQS_GammaLut_Night[id];

	iq_param_addr[IQ_HUETAB_L	] = (UINT32*)IQS_hueTab[id];
	iq_param_addr[IQ_HUETAB_M	] = (UINT32*)IQS_hueTab[id];
	iq_param_addr[IQ_HUETAB_H	] = (UINT32*)IQS_hueTab[id];
	iq_param_addr[IQ_SATTAB_L	] = (UINT32*)IQS_satTab[id];
	iq_param_addr[IQ_SATTAB_M	] = (UINT32*)IQS_satTab[id];
	iq_param_addr[IQ_SATTAB_H	] = (UINT32*)IQS_satTab[id];
	iq_param_addr[IQ_INTTAB_L	] = (UINT32*)IQS_intTab[id];
	iq_param_addr[IQ_INTTAB_M	] = (UINT32*)IQS_intTab[id];
	iq_param_addr[IQ_INTTAB_H	] = (UINT32*)IQS_intTab[id];
	//iq_param_addr[IQ_EDGTAB_L	] = (UINT32*)IQS_edgTab;
	//iq_param_addr[IQ_EDGTAB_M	] = (UINT32*)IQS_edgTab;
	//iq_param_addr[IQ_EDGTAB_H	] = (UINT32*)IQS_edgTab;
	iq_param_addr[IQ_EDGTAB	] = (UINT32*)IQS_edgTab[id];

	iq_param_addr[IQ_3DCC	] =(UINT32*)&IQS_3DColorCtrl;

	iq_param_addr[IQ_SUPER_RES	] = (UINT32*)&IQS_SuperRes;
	iq_param_addr[IQS_3DNR_PARAM	] = (UINT32*)IQS_3DNR_Param;

	iq_param_addr[IQ_OBSUB	] = (UINT32*)&IQS_OBSub[isoidx_offset[id]];
	iq_param_addr[IQ_OUTL_BRI	] = (UINT32*)&IQS_Outl_Bri[isoidx_offset[id]];
	iq_param_addr[IQ_OUTL_DARK	] = (UINT32*)&IQS_Outl_Dark[isoidx_offset[id]];
	iq_param_addr[IQ_RANGEA_THR	] = (UINT32*)&IQS_RangeA_Thr[isoidx_offset[id]];
	iq_param_addr[IQ_RANGEA_LUT	] = (UINT32*)&IQS_RangeA_Lut[isoidx_offset[id]];
	iq_param_addr[IQ_RANGEB_THR	] = (UINT32*)&IQS_RangeB_Thr[isoidx_offset[id]];
	iq_param_addr[IQ_RANGEB_LUT	] = (UINT32*)&IQS_RangeB_Lut[isoidx_offset[id]];
	iq_param_addr[IQ_RANGEB	] = (UINT32*)&IQS_RangeB[isoidx_offset[id]];
	iq_param_addr[IQ_FILTER	] = (UINT32*)&IQS_Filter[isoidx_offset[id]];
	iq_param_addr[IQ_GBAL_STAB	] = (UINT32*)&IQS_Gbal_Stab[isoidx_offset[id]];
	iq_param_addr[IQ_GBAL_DTAB	] = (UINT32*)&IQS_Gbal_Dtab[isoidx_offset[id]];
	iq_param_addr[IQ_GBAL	] = (UINT32*)&IQS_Gbal[isoidx_offset[id]];

	iq_param_addr[IQ_DCE_ENH	] = (UINT32*)&IQS_DCE_Enh[isoidx_offset[id]];
	iq_param_addr[IQ_DIREDGE_EXT	] = (UINT32*)&IQS_DirEdgeExt[isoidx_offset[id]];
	iq_param_addr[IQ_EWA	] = (UINT32*)&IQS_EWA[id];
	iq_param_addr[IQ_DIRSCORETH	] = (UINT32*)&IQS_DirScoreTh[isoidx_offset[id]];
	iq_param_addr[IQ_EWPARAM	] = (UINT32*)&IQS_EWParam[isoidx_offset[id]];
	iq_param_addr[IQ_EDGEENH	] = (UINT32*)&IQS_EdgeEnh[isoidx_offset[id]];
	iq_param_addr[IQ_EDMAP	] = (UINT32*)&IQS_EDMap[isoidx_offset[id]];
	iq_param_addr[IQ_EDTAB	] = (UINT32*)&IQS_EDTab[isoidx_offset[id]];
	iq_param_addr[IQ_ESMAP	] = (UINT32*)&IQS_ESMap[isoidx_offset[id]];
	iq_param_addr[IQ_ESTAB	] = (UINT32*)&IQS_EStab[isoidx_offset[id]];
	iq_param_addr[IQ_RGBLPF	] = (UINT32*)&IQS_RGBLpf[isoidx_offset[id]];
	iq_param_addr[IQ_FSTAB	] = (UINT32*)&IQS_FStab[isoidx_offset[id]];
	iq_param_addr[IQ_FDTAB	] = (UINT32*)&IQS_FDtab[isoidx_offset[id]];
	iq_param_addr[IQ_SATURATION	] = (UINT32*)&Saturation[isoidx_offset[id]];
	iq_param_addr[IQ_CONTRAST	] = (UINT32*)&Contrast[isoidx_offset[id]];

	iq_param_addr[IQ_REFCENTER_YTH	] = (UINT32*)&IQS_RefCenter_YTh[isoidx_offset[id]];
	iq_param_addr[IQ_REFCENTER_YWT	] = (UINT32*)&IQS_RefCenter_YWt[isoidx_offset[id]];
	iq_param_addr[IQ_REFCENTER_Y	] = (UINT32*)&IQS_RefCenter_Y[isoidx_offset[id]];
	iq_param_addr[IQ_REFCENTER_UVTH	] = (UINT32*)&IQS_RefCenter_UVTh[isoidx_offset[id]];
	iq_param_addr[IQ_REFCENTER_UVWT	] = (UINT32*)&IQS_RefCenter_UVWt[isoidx_offset[id]];
	iq_param_addr[IQ_REFCENTER_UV	] = (UINT32*)&IQS_RefCenter_UV[isoidx_offset[id]];
	iq_param_addr[IQ_IFE2FILTER_YTH	] = (UINT32*)&IQS_IFE2Filter_YTh[isoidx_offset[id]];
	iq_param_addr[IQ_IFE2FILTER_YWT	] = (UINT32*)&IQS_IFE2Filter_YWt[isoidx_offset[id]];
	iq_param_addr[IQ_IFE2FILTER_UVTH	] = (UINT32*)&IQS_IFE2Filter_UVTh[isoidx_offset[id]];
	iq_param_addr[IQ_IFE2FILTER_UVWT	] = (UINT32*)&IQS_IFE2Filter_UVWt[isoidx_offset[id]];
	iq_param_addr[IQ_IFE2FILTER	] = (UINT32*)&IQS_IFE2Filter[isoidx_offset[id]];

	iq_param_addr[IQ_CHROMAADAPT	] = (UINT32*)&IQS_ChromaAdapt[isoidx_offset[id]];
	iq_param_addr[IQ_CHROMAADAPT_COLOR	] = (UINT32*)&IQS_ChromaAdapt_Color[isoidx_offset[id]];
	iq_param_addr[IQ_CHROMAADAPT_LUM	] = (UINT32*)&IQS_ChromaAdapt_Color[isoidx_offset[id]];

	iq_param_addr[IQ_CHROMASUPP_WEIGHTY	] = (UINT32*)&IQS_ChromaSupp_WeightY[id];
	iq_param_addr[IQ_CHROMASUPP_WEIGHTC	] = (UINT32*)&IQS_ChromaSupp_WeightC[id];
	iq_param_addr[IQ_CHROMASUPP	] = (UINT32*)&IQS_ChromaSupp[id];
	iq_param_addr[IQ_3DNR_LEVEL	] = (UINT32*)&IQS_3DNR_Level[isoidx_offset[id]];
	iq_param_addr[IQ_3DNR_PARAM	] = (UINT32*)IQS_3DNR_Param;
	iq_param_addr[IQ_NUM_3DNR_PARAM	] = (UINT32*)&num_3DNR_param;

	iq_param_addr[IQ_AWDRLV	] = (UINT32*) &AutoWDRLevel[isoidx_offset[id]];


	iqs_set_addr(id, iq_param_addr);
}

#endif
