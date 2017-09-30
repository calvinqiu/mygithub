/**
    Ctrl sample.

    @file       IPL_Ctrl_IMX078CQK_JGHT_FF.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2016.  All rights reserved.
*/
#include "IPL_IMX078CQK_VR360_FF_Int.h"
#if (CAL_DP|CAL_LENSCEN)
#include "IPL_Cal_DataPassing_IMX078CQK_VR360_FF_Int.h"
#endif
//#NT#2016/09/14#liwei cheng -begin
//#NT# add surpport liveview negative crop
//#NT#2016/07/11#Jarkko Chang -begin
//#NT# add support set crop offset
INT32 Crop_Ofs_X[IPL_ID_MAX_NUM] = {-48, 0, 0, 0}, Crop_Ofs_Y[IPL_ID_MAX_NUM] = {-50, 0, 0, 0}; // Update crop size
void IPL_SIESetCropOfs(UINT32 Id, INT32 CropOfsX, INT32 CropOfsY)
{
    Crop_Ofs_X[Id] = CropOfsX;
    Crop_Ofs_Y[Id] = CropOfsY;
}
//#NT#2016/07/11#Jarkko Chang -end

void IPL_CheckCropOfsBound(UINT32 Id, UINT32* StartPixX,UINT32* StartPixY,UINT32 ActSizeH, UINT32 ActSizeV,UINT32 CropSizeH,UINT32 CropSizeV)
{
    if (Crop_Ofs_X[Id] < 0 && ((UINT32)abs(Crop_Ofs_X[Id]) >((ActSizeH - CropSizeH) >> 1)))
    {
        DBG_ERR(" Cap Crop Offset X underflow\r\n");
        DBG_ERR(" Id:%d,Crop.StartPix.x(%d) < 0, force Crop_Ofs_X(%d) = 0,\r\n",Id,((ActSizeH - CropSizeH) >> 1)+ Crop_Ofs_X[Id],Crop_Ofs_X[Id]);
        Crop_Ofs_X[Id] = 0;
    }
    *StartPixX = ((ActSizeH - CropSizeH) >> 1)+ Crop_Ofs_X[Id];
    if ((*StartPixX + CropSizeH) > ActSizeH )
    {
        DBG_ERR(" Cap Crop Offset X Overflow\r\n");
        DBG_ERR("Id: %d, crop start (%d) + crop size (%d) > act size (%d)\r\n", Id, *StartPixX,  CropSizeH, ActSizeH);
        *StartPixX = ActSizeH - CropSizeH;
    }

    if (Crop_Ofs_Y[Id] < 0 && ((UINT32)abs(Crop_Ofs_Y[Id]) >((ActSizeV - CropSizeV) >> 1)))
    {
        DBG_ERR(" Cap Crop Offset Y underflow\r\n");
        DBG_ERR(" Id:%d,Crop.StartPix.Y(%d) < 0, force Crop_Ofs_Y(%d) = 0\r\n",Id,((ActSizeV -CropSizeV) >> 1)+ Crop_Ofs_Y[Id],Crop_Ofs_Y[Id]);
        Crop_Ofs_Y[Id] = 0;
    }

    *StartPixY = ((ActSizeV - CropSizeV) >> 1)+ Crop_Ofs_Y[Id];
    if ((*StartPixY + CropSizeV) > ActSizeV )
    {
        DBG_ERR("Cap Crop Offset Y Overflow\r\n");
        DBG_ERR("Id: %d, crop start (%d) + crop size (%d) > act size (%d)\r\n", Id,*StartPixY, CropSizeV, ActSizeV);
        *StartPixY = ActSizeV - CropSizeV;
    }
}
//#NT#2016/09/14#liwei cheng -end
void IPL_SIESetOB_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_OBAVG *OBAvg, SIE_OBSUB *OBSub)
{
    SENSOR_INFO SenInfo = {0};
    ER rt = E_OK;

    if (Id != IPL_ID_1 && Id != IPL_ID_2)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_PRVAUTO_OB_EN, DISABLE);
        return;
    }

    rt = IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (rt != E_OK)
        return;

    //#NT#2016/02/16#Jarkko Chang -begin
    //#NT# remove unused code
    if (IPL_CtrlGetInfor(Id, IPLCTRL_PRVAUTO_OB_EN) == ENABLE)
    {
    //#NT#2016/02/16#Jarkko Chang -end
        OBAvg->StartPix.x = SenInfo.Mode->OB.TopWin[0];
        OBAvg->StartPix.y = SenInfo.Mode->OB.TopWin[1];
        OBAvg->SizeH = (SenInfo.Mode->OB.TopWin[2] - SenInfo.Mode->OB.TopWin[0]);
        OBAvg->SizeV = (SenInfo.Mode->OB.TopWin[3] - SenInfo.Mode->OB.TopWin[1]);
        OBSub->Offset = 0;
        OBAvg->SubRatio_X = 0;
        OBAvg->AvgActThres = 0x80;
        OBAvg->ObSubRatio = 0x20;
        *FuncEn |= (OBSubSelEn | OBAvgEn);
        *Update |= (SIE_OBOFS_ | SIE_OBAVG_ | SIE_SUBFEN);
    }
}

void IPL_SIESetCA_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_CACC *cacc, SIE_STATS_PATH_INFO *pPath)
{
    THRESHOL_SET CA_ThrInit = {0, 1023};

    if (Id != IPL_ID_1 && Id != IPL_ID_2)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CA_EN, DISABLE);
        return;
    }

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_EN) == ENABLE)
    {
        cacc->CACrop.StartPix.x = 0;
        cacc->CACrop.StartPix.y = 0;
        cacc->CACrop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
        cacc->CACrop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V);

        cacc->CA_WIN.WinNum_X = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_WINXNUM);
        cacc->CA_WIN.WinNum_Y = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_WINYNUM);

        //ca threshold init
        cacc->CA_TH_INFO.Gth = CA_ThrInit;
        cacc->CA_TH_INFO.Rth = CA_ThrInit;
        cacc->CA_TH_INFO.Bth = CA_ThrInit;
        cacc->CA_TH_INFO.Pth = CA_ThrInit;
        pPath->bVig = ENABLE;
        pPath->bCaThreshold = ENABLE;

        *FuncEn |= CACCEn;
        *Update |= (SIE_CA_SIZE | SIE_CA_TH | SIE_SUBFEN | SIE_STATS_PATH_CA);
    }
    else
    {
        *FuncEn &= ~CACCEn;
        *Update |= SIE_SUBFEN;
    }
}

void IPL_SIESetLA_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_LACC *lacc, SIE_STATS_PATH_INFO *pPath)
{
    SENSOR_INFO SenInfo = {0};
    ER rt = E_OK;

    rt = IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (rt != E_OK)
        return;

    if (Id != IPL_ID_1 && Id != IPL_ID_2)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_LA_EN, DISABLE);
        return;
    }

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_EN) == ENABLE)
    {
        lacc->LACrop.StartPix.x = (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H) - IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_H)) >> 1;
        lacc->LACrop.StartPix.y = (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V) - IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_V)) >> 1;
        lacc->LACrop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_H);
        lacc->LACrop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_V);

        lacc->LA_WIN.WinNum_X = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_WINXNUM);
        lacc->LA_WIN.WinNum_Y = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_WINYNUM);

        pPath->bLaCg = ENABLE;

        if (SenInfo.Mode->ModeType == SENSOR_MODE_STAGGER_HDR)
        {
            pPath->bLaGama1 = DISABLE;
        }
        else
        {
            pPath->bLaGama1 = ENABLE;
        }

        pPath->bHistogramSel = STATS_HISTO_POST_GAMMA;
        pPath->bLa1SrcSel = STATS_LA1_POST_CG;

        *FuncEn |= (LACCEn|HistoYEn);
        *Update |= (SIE_LA_SIZE | SIE_SUBFEN | SIE_STATS_PATH_LA);
    }
    else
    {
        *FuncEn &= ~LACCEn;
        *Update |= SIE_SUBFEN;
    }
}

//SIE_EDGETHRE Eth_Thres = {8, 32,128};    // move to IQS_SettingTable_xxxx.c
void IPL_SIESetEthOut_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *Sub)
{
    SENSOR_INFO Info;
    UINT32 BufSize;
    UINT32 Width = 0, Height = 0;
    ER rt = E_OK;

    if (Id != IPL_ID_1)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_ETH_EN, DISABLE);
        return;
    }

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_ETH_EN) == ENABLE)
    {
        IPL_BufGet(Id, IPLBUF_SIEETH_1, &Sub->EthOutCh4.Out.PPB0_Addr , &BufSize);
        IPL_BufGet(Id, IPLBUF_SIEETH_2, &Sub->EthOutCh4.Out.PPB1_Addr , &BufSize);
        IPL_BufGet(Id, IPLBUF_SIEETH_3, &Sub->EthOutCh4.Out.PPB2_Addr , &BufSize);

        Sub->EthOutCh4.Out.LineOffset = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_ETH_SIZE_HLOS);
        //Sub->pEdgeThreshold = &Eth_Thres;    // move to IQS_SettingFlow_xxxx.c
        IPL_DzoomGetSIEOutSize(Id, IPL_DzoomGetIndex(Id), &Width, &Height);

        rt = IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &Info);
        if (rt != E_OK)
            return;

        Width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio.RatioHV), Width, 4);
        Height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio.RatioHV), Height, 4);

        Sub->Va.VACrop.SizeH = Width;
        Sub->Va.VACrop.SizeV = Height-4;    //va limitation
        Sub->Va.VACrop.StartPix.x = 0;
        Sub->Va.VACrop.StartPix.y = 0;

        *FuncEn |= (EthEn);
        *InteEn |= (SIE__INTE_DRAM_OUT4_END);
        *Update |= (SIE_OUTADDR_ETH|SIE_SUBFEN|SIE_VA_CROP|SIE_IOSIZE_CH4/*|SIE_ETH_THRES_*/);
    }
    else
    {
        *FuncEn &= ~(EthEn);
        *InteEn &= ~(SIE__INTE_DRAM_OUT4_END);
        *Update |= (SIE_SUBFEN);
    }
}

void IPL_SIESetBP_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *Sub)
{
    //bp
    Sub->BP.BP1 = 10;
    Sub->BP.BP2 = 20;
    Sub->BP.BP3 = 30;

    *InteEn |= (SIE__INTE_BP1|SIE__INTE_BP2|SIE__INTE_BP3);
    *Update |= (SIE_BP_ | SIE_SUBFEN);
}

void IPL_SIESetDMAOutSrc_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update)
{
    if (Id == IPL_ID_1)
    {
        if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_EN) == ENABLE)
        {
            *FuncEn |= DMAOutCh0En;
            *Update |= SIE_SUBFEN;
        }
        *Update |= SIE_CH0Out_SRC;

        if ((IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH3_RAW_EN) == ENABLE) ||
            (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_VA_EN) == ENABLE))
        {
            *FuncEn |= DMAOutCh3En;
            *Update |= SIE_SUBFEN;
        }
        *Update |= SIE_CH3Out_SRC;
    }
    else if (Id == IPL_ID_2)
    {
        if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_EN) == ENABLE)
        {
            *FuncEn |= DMAOutCh0En;
            *Update |= SIE_SUBFEN;
        }
        *Update |= SIE_CH0Out_SRC;
    }
    else
    {
    }
}

void IPL_SIESetDP_FCB(UINT32 Id, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_UPDATE *Update, SIE_SUBFUNC *Sub)
{
#if CAL_DP
    pCAL_TAG pTag;
    CAL_DATA_INFO CaldataInfo = {0};
    IPL_MODE Mode;

    Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

    if (Mode == IPL_MODE_CAP)
    {
        CaldataInfo.SenMode = IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE);
    }
    else
    {
        CaldataInfo.SenMode = IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE);
    }

    CaldataInfo.StationID = SEC_DP;
    CaldataInfo.CalCondition[0] = CAL_CONDITION_DEFAULT;

    pTag = GetCalData(Id, CaldataInfo);

    *FuncEn &= ~DpcEn;
    *Update |= SIE_SUBFEN;

    if (pTag != NULL)
    {
        DBG_IND("get DP Calibration data\r\n");
        if(pTag->CAL_STATUS == _CAL_OK)
        {
            CAL_DP_FMT *pFmt;
            pFmt = (CAL_DP_FMT *)&pTag->TagData;
            Sub->Dpc.DP_TblAddr = (UINT32)&pFmt->DP_Pool;
            Sub->Dpc.TblSize = pTag->TagDataLen - sizeof(CAL_DPC_HEADER);
            Sub->Dpc.Weight = _50_Percent;

            *FuncEn |= DpcEn;
            *Update |= (SIE_DPC_ | SIE_SUBFEN);
            DBG_IND("DP Count =%d\r\n", pFmt->DPCnt.PixelCnt);
        }
        else
            DBG_IND("cal status not OK\r\n");
    }
    else
        DBG_IND("NO DP cal data\r\n");
#endif
}

#if 0
#endif

void IPL_SIESetRawEnc_FCB(UINT32 Id,  SIE_UPDATE *Update, SieFuncEn *FuncEn, SIE_RAWENC *RawEnc)
{
    if (Id != IPL_ID_1 && Id != IPL_ID_2)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAWENC_EN, DISABLE);
        return;
    }

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAWENC_EN) == ENABLE)
    {
        *Update |= SIE_BCC_ADJ;
        *FuncEn |= RawEncEn;
    }
    else
    {
        *FuncEn &= ~RawEncEn;
    }
}


void IPL_SIECCIR_FCB(UINT32 Id, SIE_DVI* pDvi)
{
    SENSOR_INFO SenInfo = {0};
    ER rt = E_OK;

    rt = IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (rt != E_OK)
        return;

    if ((SenInfo.Mode->ModeType != SENSOR_MODE_CCIR) && (SenInfo.Mode->ModeType != SENSOR_MODE_CCIR_INTERLACE))
    {
        return;
    }

    if (SenInfo.Mode->DVI->Fmt == SENSOR_DVI_CCIR601)
    {
        pDvi->SIE_DVIFmt = SIE_CCIR601;
    }
    else if (SenInfo.Mode->DVI->Fmt == SENSOR_DVI_CCIR656_EAV)
    {
        pDvi->SIE_DVIFmt = SIE_CCIR656_EAV;
    }
    else if (SenInfo.Mode->DVI->Fmt == SENSOR_DVI_CCIR656_ACT)
    {
        pDvi->SIE_DVIFmt = SIE_CCIR656_ACT;
    }
    else
    {
        DBG_ERR("Id: %d, unsupport SIE_DVIFmt = %d\r\n", Id, SenInfo.Mode->DVI->Fmt);
    }

    if (SenInfo.Mode->DVI->Mode == SENSOR_DVI_MODE_SD)
    {
        pDvi->SIE_DVIMode = SIE_SDMode;
    }
    else if (SenInfo.Mode->DVI->Mode == SENSOR_DVI_MODE_HD)
    {
        pDvi->SIE_DVIMode = SIE_HDMode;
    }
    else if (SenInfo.Mode->DVI->Mode == SENSOR_DVI_MODE_HD_INV)
    {
        pDvi->SIE_DVIMode = SIE_HDMode_Inv;
    }
    else
    {
        DBG_ERR("Id: %d, unsupport SIE_DVIMode = %d\r\n", Id, SenInfo.Mode->DVI->Mode);
    }

    if (SenInfo.Mode->DVI->DataFmt == SENSOR_DVI_YUYV)
    {
        pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYUYV;
    }
    else if (SenInfo.Mode->DVI->DataFmt == SENSOR_DVI_YVYU)
    {
        pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYVYU;
    }
    else if (SenInfo.Mode->DVI->DataFmt == SENSOR_DVI_UYVY)
    {
        pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPUYVY;
    }
    else if (SenInfo.Mode->DVI->DataFmt == SENOSR_DVI_VYUY)
    {
        pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPVYUY;
    }
    else
    {
        DBG_ERR("Id: %d, unsupport DVIOUT_SWAPSEL = %d\r\n", Id, SenInfo.Mode->DVI->DataFmt);
    }

    pDvi->FldEn = SenInfo.Mode->DVI->FldEn;
    pDvi->FldInvEn = SenInfo.Mode->DVI->FldInvEn;
    pDvi->OutSplit = SenInfo.Mode->DVI->OutSplitEn;
}

#if 0
#endif
void IPL_CtrlInitIpc_SIEAct(UINT32 Id, SENSOR_INFO *SenInfo, SieACTMode *ActMode)
{
    if ((SenInfo->DataType == SENSOR_DATA_LVDS) || (SenInfo->DataType == SENSOR_DATA_MIPI))
    {
        if (Id == IPL_ID_1)
        {
            *ActMode = SIE_IN_CSI_1;
        }
        else if (Id == IPL_ID_2)
        {
            if (SenInfo->Mode->ModeType == SENSOR_MODE_STAGGER_HDR)
                *ActMode = SIE_IN_CSI_1;
            else
                *ActMode = SIE_IN_CSI_2;
        }
        else
        {
            DBG_ERR("Id: %d, unsupport this Data type %d\r\n", Id, SenInfo->DataType);
        }
    }
    else if (SenInfo->DataType == SENSOR_DATA_PARALLEL)
    {
        if (SenInfo->CmdType == SENSOR_CMD_Vx1)
        {
            *ActMode = SIE_IN_VX1_SNR;
        }
        else if (SenInfo->Mode->ModeType == SENSOR_MODE_LINEAR)
        {
            if (SenInfo->SigType == SENSOR_SIGNAL_MASTER)
            {
                *ActMode = SIE_IN_PARA_MSTR_SNR;
            }
            else if (SenInfo->SigType == SENSOR_SIGNAL_SLAVE)
            {
                *ActMode = SIE_IN_PARA_SLAV_SNR;
            }
            else
            {
                DBG_ERR("Id: %d, DataType %d unsupport signal type %d\r\n", Id, SenInfo->DataType, SenInfo->SigType);
            }
        }
        else if (SenInfo->Mode->ModeType == SENSOR_MODE_CCIR || SenInfo->Mode->ModeType == SENSOR_MODE_CCIR_INTERLACE)
        {
            *ActMode = SIE_IN_PARA_MSTR_SNR;
        }
        else
        {
            DBG_ERR("Id: %d, unsupport mode type %d, Cmd Type %d\r\n", Id, SenInfo->Mode->ModeType, SenInfo->CmdType);
        }

    }
    else if (SenInfo->DataType == SENSOR_DATA_DUMMY)
    {
        *ActMode = SIE_IN_SELF_PATGEN;
    }
    else
    {
        *ActMode = SIE_IN_DRAM;
        DBG_ERR("Id: %d, unsupport Data Type %d, Set to Dram In\r\n", Id, SenInfo->DataType);
    }
}

void IPL_CtrlInitIpc_SIEBurstLength(UINT32 Id, SENSOR_INFO *SenInfo, SIE_BURSTLENGTH_SET *Length)
{
    //unnecessary to set burstlength
}

void IPL_CtrlInitIpc_SIEIn(UINT32 Id, SENSOR_INFO *SenInfo, SIE_WINDOW *Win)
{
    if ((Id == IPL_ID_2) && (SenInfo->Mode->ModeType == SENSOR_MODE_STAGGER_HDR))
    {
        Win->Act.StartPix.x = SenInfo->Mode->Trans2HD[0].DataStart;
        Win->Act.StartPix.y = SenInfo->Mode->Trans2VD[0].DataStart;

        Win->Act.SizeH = SenInfo->Mode->Trans2HD[0].DataSize;
        Win->Act.SizeV = SenInfo->Mode->Trans2VD[0].DataSize;
    }
    else
    {
        Win->Act.StartPix.x = SenInfo->Mode->TransHD[0].DataStart;
        Win->Act.StartPix.y = SenInfo->Mode->TransVD[0].DataStart;

        Win->Act.SizeH = SenInfo->Mode->TransHD[0].DataSize;
        Win->Act.SizeV = SenInfo->Mode->TransVD[0].DataSize;
    }

    Win->Act.CFA = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVSTPIX);

    Win->Crop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H);
    Win->Crop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V);
    Win->Crop.CFA = Win->Act.CFA;

    if ((Win->Act.SizeH < Win->Crop.SizeH) || (Win->Act.SizeV < Win->Crop.SizeV))
    {
        DBG_ERR("Id: %d, act size (%d %d) < crop Size(%d %d)\r\n", Id, Win->Act.SizeH, Win->Act.SizeV, Win->Crop.SizeH, Win->Crop.SizeV);
        Win->Crop.StartPix.x = 0;
        Win->Crop.StartPix.y = 0;
    }
    else
    {
		//#NT#2016/09/13#Silvia Wu -begin
        //#NT# add support lens center calibration
#if CAL_LENSCEN
            pCAL_TAG pTag;
            CAL_DATA_INFO CaldataInfo = {0};
            IPL_MODE Mode;

            Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

            if (Mode == IPL_MODE_CAP)
            {
                CaldataInfo.SenMode = IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE);
            }
            else
            {
                CaldataInfo.SenMode = IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE);
            }

            CaldataInfo.StationID = SEC_LENSCEN;
            CaldataInfo.CalCondition[0] = CAL_CONDITION_DEFAULT;

            pTag = GetCalData(Id, CaldataInfo);

            if (pTag != NULL)
            {
                DBG_IND("get LENSCEN Calibration data status 0x%x\r\n", pTag->CAL_STATUS);
                if(pTag->CAL_STATUS == _CAL_OK)
                {
                    CAL_LENSCEN_FMT *pFmt;
                    pFmt = (CAL_LENSCEN_FMT *)&pTag->TagData;
                    Crop_Ofs_X[Id] = pFmt->CenterSft_X + (pFmt->ActStr_X - Win->Act.StartPix.x);
                    Crop_Ofs_Y[Id] = pFmt->CenterSft_Y + (pFmt->ActStr_Y - Win->Act.StartPix.y);
                    DBG_IND("Crop_Ofs_X[%d] %d Crop_Ofs_Y[%d] %d\r\n", Id, Crop_Ofs_X[Id], Id, Crop_Ofs_Y[Id]);
                }
                else
                    DBG_ERR("cal status not OK\r\n");
            }
            else
                DBG_ERR("NO LENSCEN cal data\r\n");
#endif
        //#NT#2016/09/13#Silvia Wu -end
        //#NT#2016/09/14#liwei cheng -begin
        //#NT# add surpport up and left crop offset
        IPL_CheckCropOfsBound(Id, &Win->Crop.StartPix.x,&Win->Crop.StartPix.y,Win->Act.SizeH,Win->Act.SizeV,Win->Crop.SizeH,Win->Crop.SizeV);
        //#NT#2016/09/14#liwei cheng -end
    }

    //#NT#2016/03/15#Jarkko Chang -begin
    //#NT# add support ccir crop
    if (SenInfo->Mode->ModeType == SENSOR_MODE_CCIR || SenInfo->Mode->ModeType == SENSOR_MODE_CCIR_INTERLACE)
    {
        Win->Act.StartPix.x += Win->Crop.StartPix.x;
        Win->Act.StartPix.y += Win->Crop.StartPix.y;

        Win->Act.SizeH = Win->Crop.SizeH;
        Win->Act.SizeV = Win->Crop.SizeV;
    }
    //#NT#2016/03/15#Jarkko Chang -end
}

void IPL_CtrlInitIpc_SIESignal(UINT32 Id, UINT32 HDInv, UINT32 VDInv, UINT32 VDPhase, UINT32 HDPhase, UINT32 DPhase, IPP_SIE_SIGNAL *Signal)
{
    Signal->Sie_HDVD.HD_ini = HDInv;
    Signal->Sie_HDVD.VD_ini = VDInv;

    Signal->SensorPhase.VD = VDPhase;
    Signal->SensorPhase.HD = HDPhase;
    Signal->SensorPhase.Data = DPhase;
}

void IPL_CtrlInitIpc_SIECh0PPB(UINT32 Id, IPL_RAW_IMG_INFO *PPB0, UINT32 *EXT_PPB, UINT32 *StrpOfs)
{
    UINT32 BufSize;
    UINT32 i, RawBufNum = 0;

    PPB0->Width = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
    PPB0->Height = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V);
    PPB0->Bit = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT);
    PPB0->LineOffset = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS);
    PPB0->STPix = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVSTPIX);

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_BUFNUM) != 0)
    {
        IPL_BufGet(Id, IPLBUF_SIERAW_CH0_1, &PPB0->PixelAddr, &BufSize);

        RawBufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_BUFNUM)-1;
        for (i = 0; i < RawBufNum; i ++)
        {
            IPL_BufGet(Id, (IPLBUF_SIERAW_CH0_2 + i), &EXT_PPB[i], &BufSize);
        }
    }

	//#NT#2016/09/19#Jarkko Chang -begin
    //#NT# fixed stripe offset error issue, stripe offset need keep the same after IPL_MODE_INIT
    if (IPL_CtrlGetInfor(Id, IPLCTRL_CFMODE) == IPL_CF_2STRP)
        *StrpOfs = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_1STSTRP_LOS);
	//#NT#2016/09/19#Jarkko Chang -end
}

void IPL_CtrlInitIpc_SIECh1PPB(UINT32 Id, SIE_Dma_Out *DmaOut, UINT32 *EXT_PPB, UINT32 *AddrOfs)
{
    UINT32 BufSize;

    UINT32 i, CH1BufNum;

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH1_Y_EN) == ENABLE)
    {
        IPL_BufGet(Id, (IPLBUF_SIEY_CH1_1), &DmaOut->PPB0_Addr, &BufSize);
        IPL_BufGet(Id, (IPLBUF_SIEY_CH1_2), &DmaOut->PPB1_Addr, &BufSize);

        DmaOut->LineOffset = IPL_UTI_RAWH_CONV2_LOFS(IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H), IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT));

        CH1BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH1_BUFNUM) - 2;

        for (i = 0; i < CH1BufNum; i ++)
        {
            IPL_BufGet(Id, (IPLBUF_SIEY_CH1_3 + i), &EXT_PPB[i], &BufSize);
        }
    }


    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_EN) == ENABLE)
    {
        CH1BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH1_BUFNUM);

        if (CH1BufNum == 1)
        {
            IPL_BufGet(Id, IPLBUF_SIEY_CH1_1, &DmaOut->PPB0_Addr, &BufSize);
        }
        else if (CH1BufNum == 2)
        {
            IPL_BufGet(Id, IPLBUF_SIEY_CH1_1, &DmaOut->PPB0_Addr, &BufSize);
            IPL_BufGet(Id, IPLBUF_SIEY_CH1_2, &DmaOut->PPB1_Addr, &BufSize);
        }
        else
        {
            DBG_ERR("Id: %d, SIE CH1 PPB buffer error %d\r\n", Id, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH1_BUFNUM));
        }
    }

    DmaOut->HFlip = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH1_H_FLIP);
    DmaOut->VFlip = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH1_V_FLIP);
}

void IPL_CtrlInitIpc_SIECh2PPB(UINT32 Id, SIE_Dma_Out *DmaOut, UINT32 *EXT_PPB, UINT32 *AddrOfs)
{
    UINT32 BufSize;

    UINT32 i, CH2BufNum;

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_UV_EN) == ENABLE)
    {
        IPL_BufGet(Id, (IPLBUF_SIEUV_CH2_1), &DmaOut->PPB0_Addr, &BufSize);
        IPL_BufGet(Id, (IPLBUF_SIEUV_CH2_2), &DmaOut->PPB1_Addr, &BufSize);

        DmaOut->LineOffset = IPL_UTI_RAWH_CONV2_LOFS(IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H), IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT));

        CH2BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_BUFNUM) - 2;

        for (i = 0; i < CH2BufNum; i ++)
        {
            IPL_BufGet(Id, (IPLBUF_SIEUV_CH2_3 + i), &EXT_PPB[i], &BufSize);
        }
    }

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_EN) == ENABLE)
    {

        CH2BufNum = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_BUFNUM);

        if (CH2BufNum != 0)
        {
            IPL_BufGet(Id, IPLBUF_SIEUV_CH2_1, &DmaOut->PPB0_Addr, &BufSize);
            IPL_BufGet(Id, IPLBUF_SIEUV_CH2_2, &DmaOut->PPB1_Addr, &BufSize);

            CH2BufNum -= 2;
            //#NT#2016/02/26#Jarkko Chang -begin
            //#NT# LA always assign one more buffer for flip+SHDR working buffer.
            CH2BufNum += 1;
            //#NT#2016/02/26#Jarkko Chang -end
            for (i = 0; i < CH2BufNum; i ++)
            {
                IPL_BufGet(Id, (IPLBUF_SIEUV_CH2_3 + i), &EXT_PPB[i], &BufSize);
            }
        }
    }

    DmaOut->HFlip = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_H_FLIP);
    DmaOut->VFlip = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH2_V_FLIP);
}

void IPL_CtrlInitIpc_SIECh3PPB(UINT32 Id, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV)
{
    UINT32 BufSize;

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH3_RAW_BUFNUM) == 1)
    {
        IPL_BufGet(Id, IPLBUF_SIERAW_CH3_1, &Out->PPB0_Addr, &BufSize);
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH3_RAW_BUFNUM) == 2)
    {
        IPL_BufGet(Id, IPLBUF_SIERAW_CH3_1, &Out->PPB0_Addr, &BufSize);
        IPL_BufGet(Id, IPLBUF_SIERAW_CH3_2, &Out->PPB1_Addr, &BufSize);
    }
    else
    {
        DBG_ERR("Id: %d, SIE CH3 PPB buffer error %d\r\n", Id, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH3_RAW_BUFNUM));
    }

    *OutH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH3_RAW_SIZE_H);
    *OutV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH3_RAW_SIZE_V);
    Out->packBus = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH3_PRVBIT);
    Out->LineOffset = IPL_UTI_RAWH_CONV2_LOFS(*OutH, Out->packBus);
}

void IPL_CtrlInitIpc_SIEEthPPB(UINT32 Id, SIE_Eth_Out *EthOut)
{
    UINT32 BufSize;

    IPL_BufGet(Id, IPLBUF_SIEETH_1, &EthOut->PPB0_Addr, &BufSize);
    IPL_BufGet(Id, IPLBUF_SIEETH_2, &EthOut->PPB1_Addr, &BufSize);
    IPL_BufGet(Id, IPLBUF_SIEETH_3, &EthOut->PPB2_Addr, &BufSize);

    EthOut->LineOffset = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_ETH_SIZE_HLOS);
}

void IPL_CtrlInitIpc_SIEVaPPB(UINT32 Id, SIE_Dma_Out *Out)
{
    UINT32 BufSize;

    if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_VA_BUFNUM) == 1)
    {
        IPL_BufGet(Id, IPLBUF_SIEVA_1, &Out->PPB0_Addr, &BufSize);
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_VA_BUFNUM) == 2)
    {
        IPL_BufGet(Id, IPLBUF_SIEVA_1, &Out->PPB0_Addr, &BufSize);
        IPL_BufGet(Id, IPLBUF_SIEVA_2, &Out->PPB1_Addr, &BufSize);
    }
    else
    {
        DBG_ERR("Id: %d, VA PPB buffer error %d\r\n", Id, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_VA_BUFNUM));
    }
}

void IPL_CtrlInitIpc_IPLInPPB(UINT32 Id, IPL_RAW_IMG_INFO *PPB0, Coordinate *STPos)
{
	//#NT#2016/09/13#Jarkko Chang -begin
    //#NT# add smart mirror flow (DCE+IME), need one working buffer, use ipl in address
    UINT32 BufSize;

	if (IPL_CtrlGetInfor(Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_SMART_MIRROR){
		IPL_BufGet(Id, (IPLBUF_SIERAW_CH0_1), &PPB0->PixelAddr, &BufSize);
	}else{
	    PPB0->Width = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_H);
	    PPB0->Height = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_V);
	    STPos->x = (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H) - PPB0->Width)>>1;
	    STPos->y = (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V) - PPB0->Height)>>1;
	    PPB0->Bit = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_BIT);
	    PPB0->LineOffset = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_HLOS);
	    PPB0->STPix = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVSTPIX);
	}
	//#NT#2016/09/13#Jarkko Chang -end
}

void IPL_CtrlInitIpc_DCEIn(UINT32 Id, SENSOR_INFO *SenInfo, USIZE* pRef_1x, Coordinate *pCenter_1x, iCoordinate *pCenter_1xOFS, UINT32 *HRatio, UINT32 *VRatio)
{
    if (SenInfo->DataType == SENSOR_DATA_DUMMY)
    {
        pRef_1x->w = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H);
        pRef_1x->h = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V);
    }
    else
    {
        IQS_REFIMG_INFO RefImgInfo;
        IQS_GetRefImageInfo(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &RefImgInfo);
        pRef_1x->w = RefImgInfo.width;
        pRef_1x->h = RefImgInfo.height;
    }

    pCenter_1x->x = (pRef_1x->w >> 1);
    pCenter_1x->y = (pRef_1x->h >> 1);
    pCenter_1xOFS->x = 0;
    pCenter_1xOFS->y = 0;
    *HRatio = SenInfo->Mode->Ratio.RatioH;
    *VRatio = SenInfo->Mode->Ratio.RatioV;
}

//#NT#2016/07/20#Jarkko Chang -begin
//#NT# add suppor vr360 flow
void IPL_CtrlInitIpc_VR360Info(UINT32 Id, VR360_DATA *VR360Data)
{
    VR360Data->enable = IPL_CtrlGetInfor(Id, IPLCTRL_VR360_EN);
    VR360Data->uiHeightAdj = IPL_CtrlGetInfor(Id, IPLCTRL_VR360_IMG_HEI_ADJ);
    VR360Data->ori[0] = IPL_CtrlGetInfor(Id, IPLCTRL_VR360_ORI_L);
    VR360Data->ori[1] = IPL_CtrlGetInfor(Id, IPLCTRL_VR360_ORI_R);
    VR360Data->p1.l = IPL_CtrlGetInfor(Id, IPLCTRL_VR360_IME_P1_START_OFS);
    VR360Data->p2.l = IPL_CtrlGetInfor(Id, IPLCTRL_VR360_IME_P2_START_OFS);
    VR360Data->p3.l = IPL_CtrlGetInfor(Id, IPLCTRL_VR360_IME_P3_START_OFS);

    IPL_BufGet(Id, IPLBUF_IMEP1_1+IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUFNUM), &VR360Data->p1.uiRotateBufferAddr, &VR360Data->p1.uiRotateBufferSize);
    IPL_BufGet(Id, IPLBUF_IMEP2_1+IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUFNUM), &VR360Data->p2.uiRotateBufferAddr, &VR360Data->p2.uiRotateBufferSize);
    IPL_BufGet(Id, IPLBUF_IMEP3_1+IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUFNUM), &VR360Data->p3.uiRotateBufferAddr, &VR360Data->p3.uiRotateBufferSize);
}
//#NT#2016/07/20#Jarkko Chang -end

void IPL_CtrlInitIpc_DCEDeWarpInfo(UINT32 Id, DCE_Stripe *StripeInfo, _2DLUT_PARAM *Dce2DInfo)
{
{
    static UINT32 lut[2][65*65] = {
        // Top
        {
        0x0b3c0000,0x0aaf0000,0x0a22000a,0x0995001b,0x090a0033,0x08800052,0x07f80078,0x077200a4,0x06ee00d7,0x066d0111,0x05ee0150,0x05730196,0x04fc01e1,0x04880233,0x0419028a,0x03ae02e6,0x03480348,0x02e603ae,0x028a0419,0x02330488,0x01e104fc,0x01960573,0x015005ee,0x0111066d,0x00d706ee,0x00a40772,0x007807f8,0x00520880,0x0033090a,0x001b0995,0x000a0a22,0x00000aaf,0x00000b3c,0x00000bc9,0x000a0c56,0x001b0ce3,0x00330d6e,0x00520df8,0x00780e80,0x00a40f06,0x00d70f8a,0x0111100b,0x0150108a,0x01961105,0x01e1117c,0x023311f0,0x028a125f,0x02e612ca,0x03481330,0x03ae1392,0x041913ee,0x04881445,0x04fc1497,0x057314e2,0x05ee1528,0x066d1567,0x06ee15a1,0x077215d4,0x07f81600,0x08801626,0x090a1645,0x0995165d,0x0a22166e,0x0aaf1679,0x0b3c167c,0x0b3c0020,0x0ab00023,0x0a25002e,0x099b003f,0x09110057,0x08890075,0x0802009a,0x077e00c6,0x06fc00f8,0x067c0131,0x05ff0170,0x058601b5,0x051001ff,0x049e0250,0x043002a6,0x03c60301,0x03610361,0x030103c6,0x02a60430,0x0250049e,0x01ff0510,0x01b50586,0x017005ff,0x0131067c,0x00f806fc,0x00c6077e,0x009a0802,0x00750889,0x00570911,0x003f099b,0x002e0a25,0x00230ab0,0x00200b3c,0x00230bc8,0x002e0c53,0x003f0cdd,0x00570d67,0x00750def,0x009a0e76,0x00c60efa,0x00f80f7c,0x01310ffc,0x01701079,0x01b510f2,0x01ff1168,0x025011da,0x02a61248,0x030112b2,0x03611317,0x03c61377,0x043013d2,0x049e1428,0x05101479,0x058614c3,0x05ff1508,0x067c1547,0x06fc1580,0x077e15b2,0x080215de,0x08891603,0x09111621,0x099b1639,0x0a25164a,0x0ab01655,0x0b3c1658,0x0b3c0044,0x0ab20047,0x0a290051,0x09a00062,0x0918007a,0x08920098,0x080d00bd,0x078a00e8,0x0709011a,0x068b0152,0x06100190,0x059801d3,0x0524021d,0x04b3026d,0x044702c1,0x03de031b,0x037a037a,0x031b03de,0x02c10447,0x026d04b3,0x021d0524,0x01d30598,0x01900610,0x0152068b,0x011a0709,0x00e8078a,0x00bd080d,0x00980892,0x007a0918,0x006209a0,0x00510a29,0x00470ab2,0x00440b3c,0x00470bc6,0x00510c4f,0x00620cd8,0x007a0d60,0x00980de6,0x00bd0e6b,0x00e80eee,0x011a0f6f,0x01520fed,0x01901068,0x01d310e0,0x021d1154,0x026d11c5,0x02c11231,0x031b129a,0x037a12fe,0x03de135d,0x044713b7,0x04b3140b,0x0524145b,0x059814a5,0x061014e8,0x068b1526,0x0709155e,0x078a1590,0x080d15bb,0x089215e0,0x091815fe,0x09a01616,0x0a291627,0x0ab21631,0x0b3c1634,0x0b3c0068,0x0ab4006b,0x0a2c0075,0x09a50086,0x091f009d,0x089a00bb,0x081700df,0x0796010a,0x0717013b,0x069b0172,0x062101af,0x05ab01f2,0x0538023b,0x04c90289,0x045d02dd,0x03f60336,0x03940394,0x033603f6,0x02dd045d,0x028904c9,0x023b0538,0x01f205ab,0x01af0621,0x0172069b,0x013b0717,0x010a0796,0x00df0817,0x00bb089a,0x009d091f,0x008609a5,0x00750a2c,0x006b0ab4,0x00680b3c,0x006b0bc4,0x00750c4c,0x00860cd3,0x009d0d59,0x00bb0dde,0x00df0e61,0x010a0ee2,0x013b0f61,0x01720fdd,0x01af1057,0x01f210cd,0x023b1140,0x028911af,0x02dd121b,0x03361282,0x039412e4,0x03f61342,0x045d139b,0x04c913ef,0x0538143d,0x05ab1486,0x062114c9,0x069b1506,0x0717153d,0x0796156e,0x08171599,0x089a15bd,0x091f15db,0x09a515f2,0x0a2c1603,0x0ab4160d,0x0b3c1610,0x0b3c008c,0x0ab6008f,0x0a300099,0x09ab00aa,0x092600c1,0x08a300de,0x08220102,0x07a2012c,0x0725015c,0x06aa0193,0x063201cf,0x05bd0211,0x054c0259,0x04de02a6,0x047402f9,0x040f0351,0x03ad03ad,0x0351040f,0x02f90474,0x02a604de,0x0259054c,0x021105bd,0x01cf0632,0x019306aa,0x015c0725,0x012c07a2,0x01020822,0x00de08a3,0x00c10926,0x00aa09ab,0x00990a30,0x008f0ab6,0x008c0b3c,0x008f0bc2,0x00990c48,0x00aa0ccd,0x00c10d52,0x00de0dd5,0x01020e56,0x012c0ed6,0x015c0f53,0x01930fce,0x01cf1046,0x021110bb,0x0259112c,0x02a6119a,0x02f91204,0x03511269,0x03ad12cb,0x040f1327,0x0474137f,0x04de13d2,0x054c141f,0x05bd1467,0x063214a9,0x06aa14e5,0x0725151c,0x07a2154c,0x08221576,0x08a3159a,0x092615b7,0x09ab15ce,0x0a3015df,0x0ab615e9,0x0b3c15ec,0x0b3c00b0,0x0ab800b3,0x0a3300bd,0x09b000cd,0x092d00e4,0x08ac0101,0x082c0124,0x07ae014e,0x0733017d,0x06ba01b3,0x064301ef,0x05d00230,0x05600277,0x04f402c3,0x048b0315,0x0427036b,0x03c703c7,0x036b0427,0x0315048b,0x02c304f4,0x02770560,0x023005d0,0x01ef0643,0x01b306ba,0x017d0733,0x014e07ae,0x0124082c,0x010108ac,0x00e4092d,0x00cd09b0,0x00bd0a33,0x00b30ab8,0x00b00b3c,0x00b30bc0,0x00bd0c45,0x00cd0cc8,0x00e40d4b,0x01010dcc,0x01240e4c,0x014e0eca,0x017d0f45,0x01b30fbe,0x01ef1035,0x023010a8,0x02771118,0x02c31184,
        0x031511ed,0x036b1251,0x03c712b1,0x0427130d,0x048b1363,0x04f413b5,0x05601401,0x05d01448,0x06431489,0x06ba14c5,0x073314fb,0x07ae152a,0x082c1554,0x08ac1577,0x092d1594,0x09b015ab,0x0a3315bb,0x0ab815c5,0x0b3c15c8,0x0b3c00d4,0x0ab900d7,0x0a3700e1,0x09b500f1,0x09340107,0x08b50124,0x08370147,0x07bb0170,0x0741019f,0x06c901d4,0x0654020e,0x05e2024f,0x05740295,0x050902e0,0x04a20331,0x043f0386,0x03e003e0,0x0386043f,0x033104a2,0x02e00509,0x02950574,0x024f05e2,0x020e0654,0x01d406c9,0x019f0741,0x017007bb,0x01470837,0x012408b5,0x01070934,0x00f109b5,0x00e10a37,0x00d70ab9,0x00d40b3c,0x00d70bbf,0x00e10c41,0x00f10cc3,0x01070d44,0x01240dc3,0x01470e41,0x01700ebd,0x019f0f37,0x01d40faf,0x020e1024,0x024f1096,0x02951104,0x02e0116f,0x033111d6,0x03861239,0x03e01298,0x043f12f2,0x04a21347,0x05091398,0x057413e3,0x05e21429,0x0654146a,0x06c914a4,0x074114d9,0x07bb1508,0x08371531,0x08b51554,0x09341571,0x09b51587,0x0a371597,0x0ab915a1,0x0b3c15a4,0x0b3c00f8,0x0abb00fb,0x0a3a0105,0x09ba0114,0x093b012a,0x08bd0147,0x08410169,0x07c70192,0x074e01c0,0x06d801f4,0x0665022e,0x05f5026e,0x058802b3,0x051e02fd,0x04b9034c,0x045703a1,0x03fa03fa,0x03a10457,0x034c04b9,0x02fd051e,0x02b30588,0x026e05f5,0x022e0665,0x01f406d8,0x01c0074e,0x019207c7,0x01690841,0x014708bd,0x012a093b,0x011409ba,0x01050a3a,0x00fb0abb,0x00f80b3c,0x00fb0bbd,0x01050c3e,0x01140cbe,0x012a0d3d,0x01470dbb,0x01690e37,0x01920eb1,0x01c00f2a,0x01f40fa0,0x022e1013,0x026e1083,0x02b310f0,0x02fd115a,0x034c11bf,0x03a11221,0x03fa127e,0x045712d7,0x04b9132c,0x051e137b,0x058813c5,0x05f5140a,0x0665144a,0x06d81484,0x074e14b8,0x07c714e6,0x0841150f,0x08bd1531,0x093b154e,0x09ba1564,0x0a3a1573,0x0abb157d,0x0b3c1580,0x0b3c011c,0x0abd011f,0x0a3e0128,0x09c00138,0x0942014e,0x08c6016a,0x084c018c,0x07d301b3,0x075c01e1,0x06e80215,0x0676024e,0x0607028d,0x059c02d1,0x0534031a,0x04d00368,0x046f03bb,0x04130413,0x03bb046f,0x036804d0,0x031a0534,0x02d1059c,0x028d0607,0x024e0676,0x021506e8,0x01e1075c,0x01b307d3,0x018c084c,0x016a08c6,0x014e0942,0x013809c0,0x01280a3e,0x011f0abd,0x011c0b3c,0x011f0bbb,0x01280c3a,0x01380cb8,0x014e0d36,0x016a0db2,0x018c0e2c,0x01b30ea5,0x01e10f1c,0x02150f90,0x024e1002,0x028d1071,0x02d110dc,0x031a1144,0x036811a8,0x03bb1209,0x04131265,0x046f12bd,0x04d01310,0x0534135e,0x059c13a7,0x060713eb,0x0676142a,0x06e81463,0x075c1497,0x07d314c5,0x084c14ec,0x08c6150e,0x0942152a,0x09c01540,0x0a3e1550,0x0abd1559,0x0b3c155c,0x0b3c0140,0x0abf0143,0x0a41014c,0x09c5015c,0x09490171,0x08cf018c,0x085601ae,0x07df01d5,0x076a0202,0x06f70235,0x0687026e,0x061a02ac,0x05b002ef,0x05490337,0x04e60384,0x048703d6,0x042d042d,0x03d60487,0x038404e6,0x03370549,0x02ef05b0,0x02ac061a,0x026e0687,0x023506f7,0x0202076a,0x01d507df,0x01ae0856,0x018c08cf,0x01710949,0x015c09c5,0x014c0a41,0x01430abf,0x01400b3c,0x01430bb9,0x014c0c37,0x015c0cb3,0x01710d2f,0x018c0da9,0x01ae0e22,0x01d50e99,0x02020f0e,0x02350f81,0x026e0ff1,0x02ac105e,0x02ef10c8,0x0337112f,0x03841192,0x03d611f1,0x042d124b,0x048712a2,0x04e612f4,0x05491341,0x05b01389,0x061a13cc,0x0687140a,0x06f71443,0x076a1476,0x07df14a3,0x085614ca,0x08cf14ec,0x09491507,0x09c5151c,0x0a41152c,0x0abf1535,0x0b3c1538,0x0b3c0164,0x0ac00167,0x0a450170,0x09ca017f,0x09500194,0x08d801af,0x086001d0,0x07eb01f7,0x07780224,0x07070256,0x0698028d,0x062c02ca,0x05c4030d,0x055f0354,0x04fd03a0,0x04a003f1,0x04460446,0x03f104a0,0x03a004fd,0x0354055f,0x030d05c4,0x02ca062c,0x028d0698,0x02560707,0x02240778,0x01f707eb,0x01d00860,0x01af08d8,0x01940950,0x017f09ca,0x01700a45,0x01670ac0,0x01640b3c,0x01670bb8,0x01700c33,0x017f0cae,0x01940d28,0x01af0da0,0x01d00e18,0x01f70e8d,0x02240f00,0x02560f71,0x028d0fe0,0x02ca104c,0x030d10b4,0x03541119,0x03a0117b,0x03f111d8,0x04461232,0x04a01287,0x04fd12d8,0x055f1324,0x05c4136b,0x062c13ae,0x069813eb,0x07071422,0x07781454,0x07eb1481,0x086014a8,0x08d814c9,0x095014e4,0x09ca14f9,0x0a451508,0x0ac01511,0x0b3c1514,0x0b3c0188,0x0ac2018b,0x0a490194,0x09d001a3,0x095701b8,0x08e001d2,0x086b01f3,0x07f70219,0x07850245,0x07160276,0x06a902ad,0x063f02e9,0x05d8032b,0x05740371,0x051403bc,0x04b8040b,0x045f045f,0x040b04b8,0x03bc0514,0x03710574,0x032b05d8,0x02e9063f,0x02ad06a9,0x02760716,0x02450785,0x021907f7,0x01f3086b,
        0x01d208e0,0x01b80957,0x01a309d0,0x01940a49,0x018b0ac2,0x01880b3c,0x018b0bb6,0x01940c2f,0x01a30ca8,0x01b80d21,0x01d20d98,0x01f30e0d,0x02190e81,0x02450ef3,0x02760f62,0x02ad0fcf,0x02e91039,0x032b10a0,0x03711104,0x03bc1164,0x040b11c0,0x045f1219,0x04b8126d,0x051412bc,0x05741307,0x05d8134d,0x063f138f,0x06a913cb,0x07161402,0x07851433,0x07f7145f,0x086b1485,0x08e014a6,0x095714c0,0x09d014d5,0x0a4914e4,0x0ac214ed,0x0b3c14f0,0x0b3c01ac,0x0ac401af,0x0a4c01b8,0x09d501c6,0x095e01db,0x08e901f5,0x08750215,0x0803023b,0x07930266,0x07250297,0x06ba02cd,0x06510308,0x05ec0348,0x058a038e,0x052b03d8,0x04d00426,0x04790479,0x042604d0,0x03d8052b,0x038e058a,0x034805ec,0x03080651,0x02cd06ba,0x02970725,0x02660793,0x023b0803,0x02150875,0x01f508e9,0x01db095e,0x01c609d5,0x01b80a4c,0x01af0ac4,0x01ac0b3c,0x01af0bb4,0x01b80c2c,0x01c60ca3,0x01db0d1a,0x01f50d8f,0x02150e03,0x023b0e75,0x02660ee5,0x02970f53,0x02cd0fbe,0x03081027,0x0348108c,0x038e10ee,0x03d8114d,0x042611a8,0x047911ff,0x04d01252,0x052b12a0,0x058a12ea,0x05ec1330,0x06511370,0x06ba13ab,0x072513e1,0x07931412,0x0803143d,0x08751463,0x08e91483,0x095e149d,0x09d514b2,0x0a4c14c0,0x0ac414c9,0x0b3c14cc,0x0b3c01d0,0x0ac601d3,0x0a5001db,0x09da01ea,0x096501fe,0x08f20218,0x08800238,0x080f025d,0x07a10287,0x073502b7,0x06cb02ed,0x06640327,0x06000366,0x059f03ab,0x054203f3,0x04e80441,0x04920492,0x044104e8,0x03f30542,0x03ab059f,0x03660600,0x03270664,0x02ed06cb,0x02b70735,0x028707a1,0x025d080f,0x02380880,0x021808f2,0x01fe0965,0x01ea09da,0x01db0a50,0x01d30ac6,0x01d00b3c,0x01d30bb2,0x01db0c28,0x01ea0c9e,0x01fe0d13,0x02180d86,0x02380df8,0x025d0e69,0x02870ed7,0x02b70f43,0x02ed0fad,0x03271014,0x03661078,0x03ab10d9,0x03f31136,0x04411190,0x049211e6,0x04e81237,0x05421285,0x059f12cd,0x06001312,0x06641351,0x06cb138b,0x073513c1,0x07a113f1,0x080f141b,0x08801440,0x08f21460,0x0965147a,0x09da148e,0x0a50149d,0x0ac614a5,0x0b3c14a8,0x0b3c01f4,0x0ac701f7,0x0a5301ff,0x09df020e,0x096c0221,0x08fb023b,0x088a025a,0x081b027f,0x07af02a9,0x074402d8,0x06dc030c,0x06760346,0x06140384,0x05b503c7,0x0559040f,0x0500045b,0x04ac04ac,0x045b0500,0x040f0559,0x03c705b5,0x03840614,0x03460676,0x030c06dc,0x02d80744,0x02a907af,0x027f081b,0x025a088a,0x023b08fb,0x0221096c,0x020e09df,0x01ff0a53,0x01f70ac7,0x01f40b3c,0x01f70bb1,0x01ff0c25,0x020e0c99,0x02210d0c,0x023b0d7d,0x025a0dee,0x027f0e5d,0x02a90ec9,0x02d80f34,0x030c0f9c,0x03461002,0x03841064,0x03c710c3,0x040f111f,0x045b1178,0x04ac11cc,0x0500121d,0x05591269,0x05b512b1,0x061412f4,0x06761332,0x06dc136c,0x074413a0,0x07af13cf,0x081b13f9,0x088a141e,0x08fb143d,0x096c1457,0x09df146a,0x0a531479,0x0ac71481,0x0b3c1484,0x0b3c0218,0x0ac9021b,0x0a570223,0x09e50231,0x09730245,0x0903025e,0x0895027d,0x082802a1,0x07bc02ca,0x075302f8,0x06ed032c,0x06890365,0x062803a2,0x05ca03e4,0x056f042b,0x05180476,0x04c504c5,0x04760518,0x042b056f,0x03e405ca,0x03a20628,0x03650689,0x032c06ed,0x02f80753,0x02ca07bc,0x02a10828,0x027d0895,0x025e0903,0x02450973,0x023109e5,0x02230a57,0x021b0ac9,0x02180b3c,0x021b0baf,0x02230c21,0x02310c93,0x02450d05,0x025e0d75,0x027d0de3,0x02a10e50,0x02ca0ebc,0x02f80f25,0x032c0f8b,0x03650fef,0x03a21050,0x03e410ae,0x042b1109,0x04761160,0x04c511b3,0x05181202,0x056f124d,0x05ca1294,0x062812d6,0x06891313,0x06ed134c,0x07531380,0x07bc13ae,0x082813d7,0x089513fb,0x0903141a,0x09731433,0x09e51447,0x0a571455,0x0ac9145d,0x0b3c1460,0x0b3c023c,0x0acb023f,0x0a5a0247,0x09ea0255,0x097a0268,0x090c0281,0x089f029f,0x083402c2,0x07ca02eb,0x07630319,0x06fe034c,0x069b0384,0x063c03c0,0x05df0401,0x05860447,0x05310491,0x04df04df,0x04910531,0x04470586,0x040105df,0x03c0063c,0x0384069b,0x034c06fe,0x03190763,0x02eb07ca,0x02c20834,0x029f089f,0x0281090c,0x0268097a,0x025509ea,0x02470a5a,0x023f0acb,0x023c0b3c,0x023f0bad,0x02470c1e,0x02550c8e,0x02680cfe,0x02810d6c,0x029f0dd9,0x02c20e44,0x02eb0eae,0x03190f15,0x034c0f7a,0x03840fdd,0x03c0103c,0x04011099,0x044710f2,0x04911147,0x04df1199,0x053111e7,0x05861231,0x05df1277,0x063c12b8,0x069b12f4,0x06fe132c,0x0763135f,0x07ca138d,0x083413b6,0x089f13d9,0x090c13f7,0x097a1410,0x09ea1423,0x0a5a1431,0x0acb1439,0x0b3c143c,0x0b3c0260,0x0acd0263,0x0a5e026b,0x09ef0278,0x0981028b,0x091502a4,0x08aa02c1,0x084002e4,
        0x07d8030c,0x0772033a,0x070f036c,0x06ae03a2,0x065003de,0x05f5041e,0x059d0463,0x054904ab,0x04f804f8,0x04ab0549,0x0463059d,0x041e05f5,0x03de0650,0x03a206ae,0x036c070f,0x033a0772,0x030c07d8,0x02e40840,0x02c108aa,0x02a40915,0x028b0981,0x027809ef,0x026b0a5e,0x02630acd,0x02600b3c,0x02630bab,0x026b0c1a,0x02780c89,0x028b0cf7,0x02a40d63,0x02c10dce,0x02e40e38,0x030c0ea0,0x033a0f06,0x036c0f69,0x03a20fca,0x03de1028,0x041e1083,0x046310db,0x04ab112f,0x04f81180,0x054911cd,0x059d1215,0x05f5125a,0x0650129a,0x06ae12d6,0x070f130c,0x0772133e,0x07d8136c,0x08401394,0x08aa13b7,0x091513d4,0x098113ed,0x09ef1400,0x0a5e140d,0x0acd1415,0x0b3c1418,0x0b3c0284,0x0ace0286,0x0a61028f,0x09f4029c,0x098902af,0x091e02c7,0x08b402e4,0x084c0306,0x07e6032e,0x0782035a,0x0720038b,0x06c003c1,0x066403fc,0x060a043b,0x05b4047e,0x056104c6,0x05120512,0x04c60561,0x047e05b4,0x043b060a,0x03fc0664,0x03c106c0,0x038b0720,0x035a0782,0x032e07e6,0x0306084c,0x02e408b4,0x02c7091e,0x02af0989,0x029c09f4,0x028f0a61,0x02860ace,0x02840b3c,0x02860baa,0x028f0c17,0x029c0c84,0x02af0cef,0x02c70d5a,0x02e40dc4,0x03060e2c,0x032e0e92,0x035a0ef6,0x038b0f58,0x03c10fb8,0x03fc1014,0x043b106e,0x047e10c4,0x04c61117,0x05121166,0x056111b2,0x05b411fa,0x060a123d,0x0664127c,0x06c012b7,0x072012ed,0x0782131e,0x07e6134a,0x084c1372,0x08b41394,0x091e13b1,0x098913c9,0x09f413dc,0x0a6113e9,0x0ace13f2,0x0b3c13f4,0x0b3c02a8,0x0ad002aa,0x0a6502b2,0x09fa02c0,0x099002d2,0x092602ea,0x08be0306,0x08580328,0x07f4034f,0x0791037b,0x073103ab,0x06d303e0,0x0678041a,0x06200458,0x05cb049a,0x057904e1,0x052b052b,0x04e10579,0x049a05cb,0x04580620,0x041a0678,0x03e006d3,0x03ab0731,0x037b0791,0x034f07f4,0x03280858,0x030608be,0x02ea0926,0x02d20990,0x02c009fa,0x02b20a65,0x02aa0ad0,0x02a80b3c,0x02aa0ba8,0x02b20c13,0x02c00c7e,0x02d20ce8,0x02ea0d52,0x03060dba,0x03280e20,0x034f0e84,0x037b0ee7,0x03ab0f47,0x03e00fa5,0x041a1000,0x04581058,0x049a10ad,0x04e110ff,0x052b114d,0x05791197,0x05cb11de,0x06201220,0x0678125e,0x06d31298,0x073112cd,0x079112fd,0x07f41329,0x08581350,0x08be1372,0x0926138e,0x099013a6,0x09fa13b8,0x0a6513c6,0x0ad013ce,0x0b3c13d0,0x0b3c02cc,0x0ad202ce,0x0a6802d6,0x09ff02e3,0x099702f5,0x092f030c,0x08c90329,0x0864034a,0x08010370,0x07a0039b,0x074203cb,0x06e503ff,0x068c0438,0x06350475,0x05e204b6,0x059104fb,0x05440544,0x04fb0591,0x04b605e2,0x04750635,0x0438068c,0x03ff06e5,0x03cb0742,0x039b07a0,0x03700801,0x034a0864,0x032908c9,0x030c092f,0x02f50997,0x02e309ff,0x02d60a68,0x02ce0ad2,0x02cc0b3c,0x02ce0ba6,0x02d60c10,0x02e30c79,0x02f50ce1,0x030c0d49,0x03290daf,0x034a0e14,0x03700e77,0x039b0ed8,0x03cb0f36,0x03ff0f93,0x04380fec,0x04751043,0x04b61096,0x04fb10e7,0x05441134,0x0591117d,0x05e211c2,0x06351203,0x068c1240,0x06e51279,0x074212ad,0x07a012dd,0x08011308,0x0864132e,0x08c9134f,0x092f136c,0x09971383,0x09ff1395,0x0a6813a2,0x0ad213aa,0x0b3c13ac,0x0b3c02f0,0x0ad402f2,0x0a6c02fa,0x0a040307,0x099e0319,0x0938032f,0x08d3034b,0x0870036c,0x080f0391,0x07b003bc,0x075303eb,0x06f8041e,0x06a00456,0x064b0492,0x05f804d2,0x05a90516,0x055e055e,0x051605a9,0x04d205f8,0x0492064b,0x045606a0,0x041e06f8,0x03eb0753,0x03bc07b0,0x0391080f,0x036c0870,0x034b08d3,0x032f0938,0x0319099e,0x03070a04,0x02fa0a6c,0x02f20ad4,0x02f00b3c,0x02f20ba4,0x02fa0c0c,0x03070c74,0x03190cda,0x032f0d40,0x034b0da5,0x036c0e08,0x03910e69,0x03bc0ec8,0x03eb0f25,0x041e0f80,0x04560fd8,0x0492102d,0x04d21080,0x051610cf,0x055e111a,0x05a91162,0x05f811a6,0x064b11e6,0x06a01222,0x06f8125a,0x0753128d,0x07b012bc,0x080f12e7,0x0870130c,0x08d3132d,0x09381349,0x099e135f,0x0a041371,0x0a6c137e,0x0ad41386,0x0b3c1388,0x0b3c0314,0x0ad60316,0x0a6f031e,0x0a0a032a,0x09a5033c,0x09410352,0x08de036e,0x087c038e,0x081d03b3,0x07bf03dc,0x0764040a,0x070a043d,0x06b40474,0x066004af,0x060f04ee,0x05c20531,0x05770577,0x053105c2,0x04ee060f,0x04af0660,0x047406b4,0x043d070a,0x040a0764,0x03dc07bf,0x03b3081d,0x038e087c,0x036e08de,0x03520941,0x033c09a5,0x032a0a0a,0x031e0a6f,0x03160ad6,0x03140b3c,0x03160ba2,0x031e0c09,0x032a0c6e,0x033c0cd3,0x03520d37,0x036e0d9a,0x038e0dfc,0x03b30e5b,0x03dc0eb9,0x040a0f14,0x043d0f6e,0x04740fc4,0x04af1018,0x04ee1069,0x053110b6,0x05771101,0x05c21147,0x060f118a,0x066011c9,0x06b41204,0x070a123b,
        0x0764126e,0x07bf129c,0x081d12c5,0x087c12ea,0x08de130a,0x09411326,0x09a5133c,0x0a0a134e,0x0a6f135a,0x0ad61362,0x0b3c1364,0x0b3c0338,0x0ad7033a,0x0a730342,0x0a0f034e,0x09ac035f,0x09490375,0x08e80390,0x088903b0,0x082b03d4,0x07cf03fd,0x0775042a,0x071d045c,0x06c80492,0x067504cc,0x0626050a,0x05da054b,0x05910591,0x054b05da,0x050a0626,0x04cc0675,0x049206c8,0x045c071d,0x042a0775,0x03fd07cf,0x03d4082b,0x03b00889,0x039008e8,0x03750949,0x035f09ac,0x034e0a0f,0x03420a73,0x033a0ad7,0x03380b3c,0x033a0ba1,0x03420c05,0x034e0c69,0x035f0ccc,0x03750d2f,0x03900d90,0x03b00def,0x03d40e4d,0x03fd0ea9,0x042a0f03,0x045c0f5b,0x04920fb0,0x04cc1003,0x050a1052,0x054b109e,0x059110e7,0x05da112d,0x0626116e,0x067511ac,0x06c811e6,0x071d121c,0x0775124e,0x07cf127b,0x082b12a4,0x088912c8,0x08e812e8,0x09491303,0x09ac1319,0x0a0f132a,0x0a731336,0x0ad7133e,0x0b3c1340,0x0b3c035c,0x0ad9035e,0x0a760365,0x0a140372,0x09b30382,0x09520398,0x08f303b3,0x089503d2,0x083803f5,0x07de041d,0x0786044a,0x072f047b,0x06dc04b0,0x068b04e8,0x063d0525,0x05f20566,0x05aa05aa,0x056605f2,0x0525063d,0x04e8068b,0x04b006dc,0x047b072f,0x044a0786,0x041d07de,0x03f50838,0x03d20895,0x03b308f3,0x03980952,0x038209b3,0x03720a14,0x03650a76,0x035e0ad9,0x035c0b3c,0x035e0b9f,0x03650c02,0x03720c64,0x03820cc5,0x03980d26,0x03b30d85,0x03d20de3,0x03f50e40,0x041d0e9a,0x044a0ef2,0x047b0f49,0x04b00f9c,0x04e80fed,0x0525103b,0x05661086,0x05aa10ce,0x05f21112,0x063d1153,0x068b1190,0x06dc11c8,0x072f11fd,0x0786122e,0x07de125b,0x08381283,0x089512a6,0x08f312c5,0x095212e0,0x09b312f6,0x0a141306,0x0a761313,0x0ad9131a,0x0b3c131c,0x0b3c0380,0x0adb0382,0x0a7a0389,0x0a190395,0x09ba03a6,0x095b03bb,0x08fd03d5,0x08a103f3,0x08460416,0x07ed043e,0x0796046a,0x07420499,0x06f004cd,0x06a00505,0x06540541,0x060a0581,0x05c405c4,0x0581060a,0x05410654,0x050506a0,0x04cd06f0,0x04990742,0x046a0796,0x043e07ed,0x04160846,0x03f308a1,0x03d508fd,0x03bb095b,0x03a609ba,0x03950a19,0x03890a7a,0x03820adb,0x03800b3c,0x03820b9d,0x03890bfe,0x03950c5f,0x03a60cbe,0x03bb0d1d,0x03d50d7b,0x03f30dd7,0x04160e32,0x043e0e8b,0x046a0ee2,0x04990f36,0x04cd0f88,0x05050fd8,0x05411024,0x0581106e,0x05c410b4,0x060a10f7,0x06541137,0x06a01173,0x06f011ab,0x074211df,0x0796120e,0x07ed123a,0x08461262,0x08a11285,0x08fd12a3,0x095b12bd,0x09ba12d2,0x0a1912e3,0x0a7a12ef,0x0adb12f6,0x0b3c12f8,0x0b3c03a4,0x0add03a6,0x0a7d03ad,0x0a1f03b9,0x09c103c9,0x096403de,0x090803f7,0x08ad0415,0x08540438,0x07fd045e,0x07a70489,0x075404b8,0x070404eb,0x06b60522,0x066b055d,0x0622059b,0x05dd05dd,0x059b0622,0x055d066b,0x052206b6,0x04eb0704,0x04b80754,0x048907a7,0x045e07fd,0x04380854,0x041508ad,0x03f70908,0x03de0964,0x03c909c1,0x03b90a1f,0x03ad0a7d,0x03a60add,0x03a40b3c,0x03a60b9b,0x03ad0bfb,0x03b90c59,0x03c90cb7,0x03de0d14,0x03f70d70,0x04150dcb,0x04380e24,0x045e0e7b,0x04890ed1,0x04b80f24,0x04eb0f74,0x05220fc2,0x055d100d,0x059b1056,0x05dd109b,0x062210dd,0x066b111b,0x06b61156,0x0704118d,0x075411c0,0x07a711ef,0x07fd121a,0x08541240,0x08ad1263,0x09081281,0x0964129a,0x09c112af,0x0a1f12bf,0x0a7d12cb,0x0add12d2,0x0b3c12d4,0x0b3c03c8,0x0ade03ca,0x0a8103d1,0x0a2403dc,0x09c803ec,0x096c0401,0x0912041a,0x08b90437,0x08620459,0x080c047f,0x07b804a9,0x076704d7,0x07180509,0x06cb053f,0x06810579,0x063a05b6,0x05f705f7,0x05b6063a,0x05790681,0x053f06cb,0x05090718,0x04d70767,0x04a907b8,0x047f080c,0x04590862,0x043708b9,0x041a0912,0x0401096c,0x03ec09c8,0x03dc0a24,0x03d10a81,0x03ca0ade,0x03c80b3c,0x03ca0b9a,0x03d10bf7,0x03dc0c54,0x03ec0cb0,0x04010d0c,0x041a0d66,0x04370dbf,0x04590e16,0x047f0e6c,0x04a90ec0,0x04d70f11,0x05090f60,0x053f0fad,0x05790ff7,0x05b6103e,0x05f71081,0x063a10c2,0x068110ff,0x06cb1139,0x0718116f,0x076711a1,0x07b811cf,0x080c11f9,0x0862121f,0x08b91241,0x0912125e,0x096c1277,0x09c8128c,0x0a24129c,0x0a8112a7,0x0ade12ae,0x0b3c12b0,0x0b3c03ec,0x0ae003ee,0x0a8403f5,0x0a290400,0x09cf0410,0x09750424,0x091c043c,0x08c50459,0x086f047a,0x081b049f,0x07c904c9,0x077904f6,0x072c0527,0x06e1055c,0x06980595,0x065305d1,0x06100610,0x05d10653,0x05950698,0x055c06e1,0x0527072c,0x04f60779,0x04c907c9,0x049f081b,0x047a086f,0x045908c5,0x043c091c,0x04240975,0x041009cf,0x04000a29,0x03f50a84,0x03ee0ae0,0x03ec0b3c,0x03ee0b98,0x03f50bf4,
        0x04000c4f,0x04100ca9,0x04240d03,0x043c0d5c,0x04590db3,0x047a0e09,0x049f0e5d,0x04c90eaf,0x04f60eff,0x05270f4c,0x055c0f97,0x05950fe0,0x05d11025,0x06101068,0x065310a7,0x069810e3,0x06e1111c,0x072c1151,0x07791182,0x07c911af,0x081b11d9,0x086f11fe,0x08c5121f,0x091c123c,0x09751254,0x09cf1268,0x0a291278,0x0a841283,0x0ae0128a,0x0b3c128c,0x0b3c0410,0x0ae20412,0x0a880418,0x0a2f0424,0x09d60433,0x097e0447,0x0927045f,0x08d1047b,0x087d049b,0x082b04c0,0x07da04e8,0x078c0515,0x07400545,0x06f60579,0x06af05b0,0x066b05eb,0x06290629,0x05eb066b,0x05b006af,0x057906f6,0x05450740,0x0515078c,0x04e807da,0x04c0082b,0x049b087d,0x047b08d1,0x045f0927,0x0447097e,0x043309d6,0x04240a2f,0x04180a88,0x04120ae2,0x04100b3c,0x04120b96,0x04180bf0,0x04240c49,0x04330ca2,0x04470cfa,0x045f0d51,0x047b0da7,0x049b0dfb,0x04c00e4d,0x04e80e9e,0x05150eec,0x05450f38,0x05790f82,0x05b00fc9,0x05eb100d,0x0629104f,0x066b108d,0x06af10c8,0x06f610ff,0x07401133,0x078c1163,0x07da1190,0x082b11b8,0x087d11dd,0x08d111fd,0x09271219,0x097e1231,0x09d61245,0x0a2f1254,0x0a881260,0x0ae21266,0x0b3c1268,0x0b3c0434,0x0ae40436,0x0a8c043c,0x0a340447,0x09dd0456,0x0987046a,0x09310481,0x08dd049d,0x088b04bd,0x083a04e0,0x07eb0508,0x079e0534,0x07540563,0x070c0596,0x06c605cc,0x06830606,0x06430643,0x06060683,0x05cc06c6,0x0596070c,0x05630754,0x0534079e,0x050807eb,0x04e0083a,0x04bd088b,0x049d08dd,0x04810931,0x046a0987,0x045609dd,0x04470a34,0x043c0a8c,0x04360ae4,0x04340b3c,0x04360b94,0x043c0bec,0x04470c44,0x04560c9b,0x046a0cf1,0x04810d47,0x049d0d9b,0x04bd0ded,0x04e00e3e,0x05080e8d,0x05340eda,0x05630f24,0x05960f6c,0x05cc0fb2,0x06060ff5,0x06431035,0x06831072,0x06c610ac,0x070c10e2,0x07541115,0x079e1144,0x07eb1170,0x083a1198,0x088b11bb,0x08dd11db,0x093111f7,0x0987120e,0x09dd1222,0x0a341231,0x0a8c123c,0x0ae41242,0x0b3c1244,0x0b3c0458,0x0ae5045a,0x0a8f0460,0x0a39046b,0x09e4047a,0x098f048c,0x093c04a4,0x08ea04bf,0x089904de,0x084a0501,0x07fc0528,0x07b10553,0x07680581,0x072105b3,0x06dd05e8,0x069b0621,0x065c065c,0x0621069b,0x05e806dd,0x05b30721,0x05810768,0x055307b1,0x052807fc,0x0501084a,0x04de0899,0x04bf08ea,0x04a4093c,0x048c098f,0x047a09e4,0x046b0a39,0x04600a8f,0x045a0ae5,0x04580b3c,0x045a0b93,0x04600be9,0x046b0c3f,0x047a0c94,0x048c0ce9,0x04a40d3c,0x04bf0d8e,0x04de0ddf,0x05010e2e,0x05280e7c,0x05530ec7,0x05810f10,0x05b30f57,0x05e80f9b,0x06210fdd,0x065c101c,0x069b1057,0x06dd1090,0x072110c5,0x076810f7,0x07b11125,0x07fc1150,0x084a1177,0x0899119a,0x08ea11b9,0x093c11d4,0x098f11ec,0x09e411fe,0x0a39120d,0x0a8f1218,0x0ae5121e,0x0b3c1220,0x0b3c047c,0x0ae7047e,0x0a930484,0x0a3e048e,0x09eb049d,0x099804af,0x094604c6,0x08f604e1,0x08a704ff,0x08590522,0x080d0548,0x07c30572,0x077c059f,0x073605d0,0x06f40604,0x06b3063b,0x06760676,0x063b06b3,0x060406f4,0x05d00736,0x059f077c,0x057207c3,0x0548080d,0x05220859,0x04ff08a7,0x04e108f6,0x04c60946,0x04af0998,0x049d09eb,0x048e0a3e,0x04840a93,0x047e0ae7,0x047c0b3c,0x047e0b91,0x04840be5,0x048e0c3a,0x049d0c8d,0x04af0ce0,0x04c60d32,0x04e10d82,0x04ff0dd1,0x05220e1f,0x05480e6b,0x05720eb5,0x059f0efc,0x05d00f42,0x06040f84,0x063b0fc5,0x06761002,0x06b3103d,0x06f41074,0x073610a8,0x077c10d9,0x07c31106,0x080d1130,0x08591156,0x08a71179,0x08f61197,0x094611b2,0x099811c9,0x09eb11db,0x0a3e11ea,0x0a9311f4,0x0ae711fa,0x0b3c11fc,0x0b3c04a0,0x0ae904a2,0x0a9604a8,0x0a4404b2,0x09f204c0,0x09a104d2,0x095104e8,0x09020503,0x08b40520,0x08680542,0x081e0567,0x07d60590,0x079005bd,0x074c05ed,0x070a0620,0x06cb0656,0x068f068f,0x065606cb,0x0620070a,0x05ed074c,0x05bd0790,0x059007d6,0x0567081e,0x05420868,0x052008b4,0x05030902,0x04e80951,0x04d209a1,0x04c009f2,0x04b20a44,0x04a80a96,0x04a20ae9,0x04a00b3c,0x04a20b8f,0x04a80be2,0x04b20c34,0x04c00c86,0x04d20cd7,0x04e80d27,0x05030d76,0x05200dc4,0x05420e10,0x05670e5a,0x05900ea2,0x05bd0ee8,0x05ed0f2c,0x06200f6e,0x06560fad,0x068f0fe9,0x06cb1022,0x070a1058,0x074c108b,0x079010bb,0x07d610e8,0x081e1111,0x08681136,0x08b41158,0x09021175,0x09511190,0x09a111a6,0x09f211b8,0x0a4411c6,0x0a9611d0,0x0ae911d6,0x0b3c11d8,0x0b3c04c4,0x0aeb04c6,0x0a9a04cc,0x0a4904d6,0x09f904e3,0x09aa04f5,0x095b050b,0x090e0524,0x08c20542,0x08780563,0x082f0587,0x07e805af,0x07a405db,0x0761060a,0x0721063c,0x06e40671,
        0x06a906a9,0x067106e4,0x063c0721,0x060a0761,0x05db07a4,0x05af07e8,0x0587082f,0x05630878,0x054208c2,0x0524090e,0x050b095b,0x04f509aa,0x04e309f9,0x04d60a49,0x04cc0a9a,0x04c60aeb,0x04c40b3c,0x04c60b8d,0x04cc0bde,0x04d60c2f,0x04e30c7f,0x04f50cce,0x050b0d1d,0x05240d6a,0x05420db6,0x05630e00,0x05870e49,0x05af0e90,0x05db0ed4,0x060a0f17,0x063c0f57,0x06710f94,0x06a90fcf,0x06e41007,0x0721103c,0x0761106e,0x07a4109d,0x07e810c9,0x082f10f1,0x08781115,0x08c21136,0x090e1154,0x095b116d,0x09aa1183,0x09f91195,0x0a4911a2,0x0a9a11ac,0x0aeb11b2,0x0b3c11b4,0x0b3c04e8,0x0aec04ea,0x0a9d04ef,0x0a4e04f9,0x0a000507,0x09b20518,0x0966052d,0x091a0546,0x08d00563,0x08870583,0x084005a7,0x07fb05ce,0x07b805f9,0x07770626,0x07380657,0x06fc068b,0x06c206c2,0x068b06fc,0x06570738,0x06260777,0x05f907b8,0x05ce07fb,0x05a70840,0x05830887,0x056308d0,0x0546091a,0x052d0966,0x051809b2,0x05070a00,0x04f90a4e,0x04ef0a9d,0x04ea0aec,0x04e80b3c,0x04ea0b8c,0x04ef0bdb,0x04f90c2a,0x05070c78,0x05180cc6,0x052d0d12,0x05460d5e,0x05630da8,0x05830df1,0x05a70e38,0x05ce0e7d,0x05f90ec0,0x06260f01,0x06570f40,0x068b0f7c,0x06c20fb6,0x06fc0fed,0x07381021,0x07771052,0x07b8107f,0x07fb10aa,0x084010d1,0x088710f5,0x08d01115,0x091a1132,0x0966114b,0x09b21160,0x0a001171,0x0a4e117f,0x0a9d1189,0x0aec118e,0x0b3c1190,0x0b3c050c,0x0aee050d,0x0aa10513,0x0a54051d,0x0a07052a,0x09bb053b,0x09700550,0x09260568,0x08de0584,0x089705a4,0x085105c7,0x080d05ed,0x07cc0617,0x078c0643,0x074f0673,0x071406a6,0x06dc06dc,0x06a60714,0x0673074f,0x0643078c,0x061707cc,0x05ed080d,0x05c70851,0x05a40897,0x058408de,0x05680926,0x05500970,0x053b09bb,0x052a0a07,0x051d0a54,0x05130aa1,0x050d0aee,0x050c0b3c,0x050d0b8a,0x05130bd7,0x051d0c24,0x052a0c71,0x053b0cbd,0x05500d08,0x05680d52,0x05840d9a,0x05a40de1,0x05c70e27,0x05ed0e6b,0x06170eac,0x06430eec,0x06730f29,0x06a60f64,0x06dc0f9c,0x07140fd2,0x074f1005,0x078c1035,0x07cc1061,0x080d108b,0x085110b1,0x089710d4,0x08de10f4,0x09261110,0x09701128,0x09bb113d,0x0a07114e,0x0a54115b,0x0aa11165,0x0aee116b,0x0b3c116c,0x0b3c0530,0x0af00531,0x0aa40537,0x0a590540,0x0a0e054d,0x09c4055e,0x097b0572,0x0932058a,0x08eb05a5,0x08a605c4,0x086205e6,0x0820060c,0x07e00634,0x07a20660,0x0766068f,0x072c06c1,0x06f506f5,0x06c1072c,0x068f0766,0x066007a2,0x063407e0,0x060c0820,0x05e60862,0x05c408a6,0x05a508eb,0x058a0932,0x0572097b,0x055e09c4,0x054d0a0e,0x05400a59,0x05370aa4,0x05310af0,0x05300b3c,0x05310b88,0x05370bd4,0x05400c1f,0x054d0c6a,0x055e0cb4,0x05720cfd,0x058a0d46,0x05a50d8d,0x05c40dd2,0x05e60e16,0x060c0e58,0x06340e98,0x06600ed6,0x068f0f12,0x06c10f4c,0x06f50f83,0x072c0fb7,0x07660fe9,0x07a21018,0x07e01044,0x0820106c,0x08621092,0x08a610b4,0x08eb10d3,0x093210ee,0x097b1106,0x09c4111a,0x0a0e112b,0x0a591138,0x0aa41141,0x0af01147,0x0b3c1148,0x0b3c0554,0x0af20555,0x0aa8055b,0x0a5e0564,0x0a150571,0x09cc0581,0x09850595,0x093e05ac,0x08f905c7,0x08b505e5,0x08730606,0x0832062b,0x07f40652,0x07b7067d,0x077c06ab,0x074406db,0x070f070f,0x06db0744,0x06ab077c,0x067d07b7,0x065207f4,0x062b0832,0x06060873,0x05e508b5,0x05c708f9,0x05ac093e,0x05950985,0x058109cc,0x05710a15,0x05640a5e,0x055b0aa8,0x05550af2,0x05540b3c,0x05550b86,0x055b0bd0,0x05640c1a,0x05710c63,0x05810cac,0x05950cf3,0x05ac0d3a,0x05c70d7f,0x05e50dc3,0x06060e05,0x062b0e46,0x06520e84,0x067d0ec1,0x06ab0efc,0x06db0f34,0x070f0f69,0x07440f9d,0x077c0fcd,0x07b70ffb,0x07f41026,0x0832104d,0x08731072,0x08b51093,0x08f910b1,0x093e10cc,0x098510e3,0x09cc10f7,0x0a151107,0x0a5e1114,0x0aa8111d,0x0af21123,0x0b3c1124,0x0b3c0578,0x0af40579,0x0aab057f,0x0a630587,0x0a1c0594,0x09d505a4,0x098f05b7,0x094b05ce,0x090705e8,0x08c50605,0x08840626,0x0845064a,0x08080670,0x07cc069a,0x079306c7,0x075c06f6,0x07280728,0x06f6075c,0x06c70793,0x069a07cc,0x06700808,0x064a0845,0x06260884,0x060508c5,0x05e80907,0x05ce094b,0x05b7098f,0x05a409d5,0x05940a1c,0x05870a63,0x057f0aab,0x05790af4,0x05780b3c,0x05790b84,0x057f0bcd,0x05870c15,0x05940c5c,0x05a40ca3,0x05b70ce9,0x05ce0d2d,0x05e80d71,0x06050db3,0x06260df4,0x064a0e33,0x06700e70,0x069a0eac,0x06c70ee5,0x06f60f1c,0x07280f50,0x075c0f82,0x07930fb1,0x07cc0fde,0x08081008,0x0845102e,0x08841052,0x08c51073,0x09071090,0x094b10aa,0x098f10c1,0x09d510d4,0x0a1c10e4,0x0a6310f1,
        0x0aab10f9,0x0af410ff,0x0b3c1100,0x0b3c059b,0x0af5059d,0x0aaf05a2,0x0a6905ab,0x0a2305b7,0x09de05c7,0x099a05da,0x095705f0,0x09150609,0x08d40626,0x08950646,0x08570668,0x081c068e,0x07e206b7,0x07aa06e2,0x07750711,0x07410741,0x07110775,0x06e207aa,0x06b707e2,0x068e081c,0x06680857,0x06460895,0x062608d4,0x06090915,0x05f00957,0x05da099a,0x05c709de,0x05b70a23,0x05ab0a69,0x05a20aaf,0x059d0af5,0x059b0b3c,0x059d0b83,0x05a20bc9,0x05ab0c0f,0x05b70c55,0x05c70c9a,0x05da0cde,0x05f00d21,0x06090d63,0x06260da4,0x06460de3,0x06680e21,0x068e0e5c,0x06b70e96,0x06e20ece,0x07110f03,0x07410f37,0x07750f67,0x07aa0f96,0x07e20fc1,0x081c0fea,0x08571010,0x08951032,0x08d41052,0x0915106f,0x09571088,0x099a109e,0x09de10b1,0x0a2310c1,0x0a6910cd,0x0aaf10d6,0x0af510db,0x0b3c10dd,0x0b3c05bf,0x0af705c1,0x0ab205c6,0x0a6e05cf,0x0a2a05da,0x09e705ea,0x09a405fc,0x09630612,0x0923062a,0x08e30646,0x08a60665,0x086a0687,0x083006ac,0x07f706d4,0x07c106fe,0x078d072b,0x075b075b,0x072b078d,0x06fe07c1,0x06d407f7,0x06ac0830,0x0687086a,0x066508a6,0x064608e3,0x062a0923,0x06120963,0x05fc09a4,0x05ea09e7,0x05da0a2a,0x05cf0a6e,0x05c60ab2,0x05c10af7,0x05bf0b3c,0x05c10b81,0x05c60bc6,0x05cf0c0a,0x05da0c4e,0x05ea0c91,0x05fc0cd4,0x06120d15,0x062a0d55,0x06460d95,0x06650dd2,0x06870e0e,0x06ac0e48,0x06d40e81,0x06fe0eb7,0x072b0eeb,0x075b0f1d,0x078d0f4d,0x07c10f7a,0x07f70fa4,0x08300fcc,0x086a0ff1,0x08a61013,0x08e31032,0x0923104e,0x09631066,0x09a4107c,0x09e7108e,0x0a2a109e,0x0a6e10a9,0x0ab210b2,0x0af710b7,0x0b3c10b9,0x0b3c05e3,0x0af905e5,0x0ab605ea,0x0a7305f2,0x0a3105fe,0x09ef060c,0x09af061e,0x096f0633,0x0930064c,0x08f30667,0x08b70685,0x087c06a6,0x084406ca,0x080d06f1,0x07d8071a,0x07a50746,0x07740774,0x074607a5,0x071a07d8,0x06f1080d,0x06ca0844,0x06a6087c,0x068508b7,0x066708f3,0x064c0930,0x0633096f,0x061e09af,0x060c09ef,0x05fe0a31,0x05f20a73,0x05ea0ab6,0x05e50af9,0x05e30b3c,0x05e50b7f,0x05ea0bc2,0x05f20c05,0x05fe0c47,0x060c0c89,0x061e0cc9,0x06330d09,0x064c0d48,0x06670d85,0x06850dc1,0x06a60dfc,0x06ca0e34,0x06f10e6b,0x071a0ea0,0x07460ed3,0x07740f04,0x07a50f32,0x07d80f5e,0x080d0f87,0x08440fae,0x087c0fd2,0x08b70ff3,0x08f31011,0x0930102c,0x096f1045,0x09af105a,0x09ef106c,0x0a31107a,0x0a731086,0x0ab6108e,0x0af91093,0x0b3c1095,0x0b3c0607,0x0afb0609,0x0ab9060e,0x0a780616,0x0a380621,0x09f8062f,0x09b90641,0x097b0655,0x093e066d,0x09020687,0x08c806a5,0x088f06c5,0x085806e8,0x0822070e,0x07ef0736,0x07bd0761,0x078e078e,0x076107bd,0x073607ef,0x070e0822,0x06e80858,0x06c5088f,0x06a508c8,0x06870902,0x066d093e,0x0655097b,0x064109b9,0x062f09f8,0x06210a38,0x06160a78,0x060e0ab9,0x06090afb,0x06070b3c,0x06090b7d,0x060e0bbf,0x06160c00,0x06210c40,0x062f0c80,0x06410cbf,0x06550cfd,0x066d0d3a,0x06870d76,0x06a50db0,0x06c50de9,0x06e80e20,0x070e0e56,0x07360e89,0x07610ebb,0x078e0eea,0x07bd0f17,0x07ef0f42,0x08220f6a,0x08580f90,0x088f0fb3,0x08c80fd3,0x09020ff1,0x093e100b,0x097b1023,0x09b91037,0x09f81049,0x0a381057,0x0a781062,0x0ab9106a,0x0afb106f,0x0b3c1071,0x0b3c062b,0x0afc062d,0x0abd0632,0x0a7e0639,0x0a3f0644,0x0a010652,0x09c40663,0x09870677,0x094c068e,0x091206a8,0x08d906c5,0x08a106e4,0x086c0706,0x0838072b,0x08050752,0x07d5077b,0x07a707a7,0x077b07d5,0x07520805,0x072b0838,0x0706086c,0x06e408a1,0x06c508d9,0x06a80912,0x068e094c,0x06770987,0x066309c4,0x06520a01,0x06440a3f,0x06390a7e,0x06320abd,0x062d0afc,0x062b0b3c,0x062d0b7c,0x06320bbb,0x06390bfa,0x06440c39,0x06520c77,0x06630cb4,0x06770cf1,0x068e0d2c,0x06a80d66,0x06c50d9f,0x06e40dd7,0x07060e0c,0x072b0e40,0x07520e73,0x077b0ea3,0x07a70ed1,0x07d50efd,0x08050f26,0x08380f4d,0x086c0f72,0x08a10f94,0x08d90fb3,0x09120fd0,0x094c0fea,0x09871001,0x09c41015,0x0a011026,0x0a3f1034,0x0a7e103f,0x0abd1046,0x0afc104b,0x0b3c104d,0x0b3c064f,0x0afe0651,0x0ac00656,0x0a83065d,0x0a460668,0x0a0a0675,0x09ce0686,0x09930699,0x095a06af,0x092106c8,0x08ea06e4,0x08b40703,0x08800724,0x084d0748,0x081c076e,0x07ed0796,0x07c107c1,0x079607ed,0x076e081c,0x0748084d,0x07240880,0x070308b4,0x06e408ea,0x06c80921,0x06af095a,0x06990993,0x068609ce,0x06750a0a,0x06680a46,0x065d0a83,0x06560ac0,0x06510afe,0x064f0b3c,0x06510b7a,0x06560bb8,0x065d0bf5,0x06680c32,0x06750c6e,0x06860caa,0x06990ce5,0x06af0d1e,0x06c80d57,0x06e40d8e,
        0x07030dc4,0x07240df8,0x07480e2b,0x076e0e5c,0x07960e8b,0x07c10eb7,0x07ed0ee2,0x081c0f0a,0x084d0f30,0x08800f54,0x08b40f75,0x08ea0f94,0x09210fb0,0x095a0fc9,0x09930fdf,0x09ce0ff2,0x0a0a1003,0x0a461010,0x0a83101b,0x0ac01022,0x0afe1027,0x0b3c1029,0x0b3c0673,0x0b000675,0x0ac40679,0x0a880681,0x0a4d068b,0x0a120698,0x09d906a8,0x099f06bb,0x096706d1,0x093006e9,0x08fb0704,0x08c60722,0x08940742,0x08630764,0x08330789,0x080607b1,0x07da07da,0x07b10806,0x07890833,0x07640863,0x07420894,0x072208c6,0x070408fb,0x06e90930,0x06d10967,0x06bb099f,0x06a809d9,0x06980a12,0x068b0a4d,0x06810a88,0x06790ac4,0x06750b00,0x06730b3c,0x06750b78,0x06790bb4,0x06810bf0,0x068b0c2b,0x06980c66,0x06a80c9f,0x06bb0cd9,0x06d10d11,0x06e90d48,0x07040d7d,0x07220db2,0x07420de4,0x07640e15,0x07890e45,0x07b10e72,0x07da0e9e,0x08060ec7,0x08330eef,0x08630f14,0x08940f36,0x08c60f56,0x08fb0f74,0x09300f8f,0x09670fa7,0x099f0fbd,0x09d90fd0,0x0a120fe0,0x0a4d0fed,0x0a880ff7,0x0ac40fff,0x0b001003,0x0b3c1005,0x0b3c0697,0x0b020699,0x0ac7069d,0x0a8e06a4,0x0a5406ae,0x0a1b06bb,0x09e306cb,0x09ac06dd,0x097506f2,0x0940070a,0x090c0724,0x08d90741,0x08a80760,0x08780781,0x084a07a5,0x081e07cb,0x07f407f4,0x07cb081e,0x07a5084a,0x07810878,0x076008a8,0x074108d9,0x0724090c,0x070a0940,0x06f20975,0x06dd09ac,0x06cb09e3,0x06bb0a1b,0x06ae0a54,0x06a40a8e,0x069d0ac7,0x06990b02,0x06970b3c,0x06990b76,0x069d0bb1,0x06a40bea,0x06ae0c24,0x06bb0c5d,0x06cb0c95,0x06dd0ccc,0x06f20d03,0x070a0d38,0x07240d6c,0x07410d9f,0x07600dd0,0x07810e00,0x07a50e2e,0x07cb0e5a,0x07f40e84,0x081e0ead,0x084a0ed3,0x08780ef7,0x08a80f18,0x08d90f37,0x090c0f54,0x09400f6e,0x09750f86,0x09ac0f9b,0x09e30fad,0x0a1b0fbd,0x0a540fca,0x0a8e0fd4,0x0ac70fdb,0x0b020fdf,0x0b3c0fe1,0x0b3c06bb,0x0b0306bd,0x0acb06c1,0x0a9306c8,0x0a5b06d2,0x0a2406de,0x09ed06ed,0x09b806ff,0x09830713,0x094f072a,0x091d0743,0x08eb075f,0x08bc077e,0x088d079e,0x086107c1,0x083607e6,0x080d080d,0x07e60836,0x07c10861,0x079e088d,0x077e08bc,0x075f08eb,0x0743091d,0x072a094f,0x07130983,0x06ff09b8,0x06ed09ed,0x06de0a24,0x06d20a5b,0x06c80a93,0x06c10acb,0x06bd0b03,0x06bb0b3c,0x06bd0b75,0x06c10bad,0x06c80be5,0x06d20c1d,0x06de0c54,0x06ed0c8b,0x06ff0cc0,0x07130cf5,0x072a0d29,0x07430d5b,0x075f0d8d,0x077e0dbc,0x079e0deb,0x07c10e17,0x07e60e42,0x080d0e6b,0x08360e92,0x08610eb7,0x088d0eda,0x08bc0efa,0x08eb0f19,0x091d0f35,0x094f0f4e,0x09830f65,0x09b80f79,0x09ed0f8b,0x0a240f9a,0x0a5b0fa6,0x0a930fb0,0x0acb0fb7,0x0b030fbb,0x0b3c0fbd,0x0b3c06df,0x0b0506e1,0x0acf06e5,0x0a9806eb,0x0a6206f5,0x0a2d0701,0x09f8070f,0x09c40721,0x09910734,0x095f074b,0x092e0763,0x08fe077e,0x08d0079c,0x08a307bb,0x087807dd,0x084e0801,0x08260826,0x0801084e,0x07dd0878,0x07bb08a3,0x079c08d0,0x077e08fe,0x0763092e,0x074b095f,0x07340991,0x072109c4,0x070f09f8,0x07010a2d,0x06f50a62,0x06eb0a98,0x06e50acf,0x06e10b05,0x06df0b3c,0x06e10b73,0x06e50ba9,0x06eb0be0,0x06f50c16,0x07010c4b,0x070f0c80,0x07210cb4,0x07340ce7,0x074b0d19,0x07630d4a,0x077e0d7a,0x079c0da8,0x07bb0dd5,0x07dd0e00,0x08010e2a,0x08260e52,0x084e0e77,0x08780e9b,0x08a30ebd,0x08d00edc,0x08fe0efa,0x092e0f15,0x095f0f2d,0x09910f44,0x09c40f57,0x09f80f69,0x0a2d0f77,0x0a620f83,0x0a980f8d,0x0acf0f93,0x0b050f97,0x0b3c0f99,0x0b3c0703,0x0b070705,0x0ad20709,0x0a9d070f,0x0a690718,0x0a350724,0x0a020732,0x09d00743,0x099e0756,0x096e076b,0x093f0783,0x0910079d,0x08e407b9,0x08b807d8,0x088e07f9,0x0866081b,0x08400840,0x081b0866,0x07f9088e,0x07d808b8,0x07b908e4,0x079d0910,0x0783093f,0x076b096e,0x0756099e,0x074309d0,0x07320a02,0x07240a35,0x07180a69,0x070f0a9d,0x07090ad2,0x07050b07,0x07030b3c,0x07050b71,0x07090ba6,0x070f0bdb,0x07180c0f,0x07240c43,0x07320c76,0x07430ca8,0x07560cda,0x076b0d0a,0x07830d39,0x079d0d68,0x07b90d94,0x07d80dc0,0x07f90dea,0x081b0e12,0x08400e38,0x08660e5d,0x088e0e7f,0x08b80ea0,0x08e40ebf,0x09100edb,0x093f0ef5,0x096e0f0d,0x099e0f22,0x09d00f35,0x0a020f46,0x0a350f54,0x0a690f60,0x0a9d0f69,0x0ad20f6f,0x0b070f73,0x0b3c0f75,0x0b3c0727,0x0b090729,0x0ad6072c,0x0aa30733,0x0a70073b,0x0a3e0747,0x0a0d0754,0x09dc0764,0x09ac0777,0x097d078c,0x095007a3,0x092307bc,0x08f807d7,0x08ce07f5,0x08a50814,0x087e0836,0x08590859,0x0836087e,0x081408a5,0x07f508ce,0x07d708f8,0x07bc0923,0x07a30950,0x078c097d,
        0x077709ac,0x076409dc,0x07540a0d,0x07470a3e,0x073b0a70,0x07330aa3,0x072c0ad6,0x07290b09,0x07270b3c,0x07290b6f,0x072c0ba2,0x07330bd5,0x073b0c08,0x07470c3a,0x07540c6b,0x07640c9c,0x07770ccc,0x078c0cfb,0x07a30d28,0x07bc0d55,0x07d70d80,0x07f50daa,0x08140dd3,0x08360dfa,0x08590e1f,0x087e0e42,0x08a50e64,0x08ce0e83,0x08f80ea1,0x09230ebc,0x09500ed5,0x097d0eec,0x09ac0f01,0x09dc0f14,0x0a0d0f24,0x0a3e0f31,0x0a700f3d,0x0aa30f45,0x0ad60f4c,0x0b090f4f,0x0b3c0f51,0x0b3c074b,0x0b0b074d,0x0ad90750,0x0aa80756,0x0a77075f,0x0a47076a,0x0a170777,0x09e80786,0x09ba0798,0x098d07ac,0x096107c2,0x093507db,0x090c07f5,0x08e30812,0x08bc0830,0x08970851,0x08730873,0x08510897,0x083008bc,0x081208e3,0x07f5090c,0x07db0935,0x07c20961,0x07ac098d,0x079809ba,0x078609e8,0x07770a17,0x076a0a47,0x075f0a77,0x07560aa8,0x07500ad9,0x074d0b0b,0x074b0b3c,0x074d0b6d,0x07500b9f,0x07560bd0,0x075f0c01,0x076a0c31,0x07770c61,0x07860c90,0x07980cbe,0x07ac0ceb,0x07c20d17,0x07db0d43,0x07f50d6c,0x08120d95,0x08300dbc,0x08510de1,0x08730e05,0x08970e27,0x08bc0e48,0x08e30e66,0x090c0e83,0x09350e9d,0x09610eb6,0x098d0ecc,0x09ba0ee0,0x09e80ef2,0x0a170f01,0x0a470f0e,0x0a770f19,0x0aa80f22,0x0ad90f28,0x0b0b0f2b,0x0b3c0f2d,0x0b3c076f,0x0b0c0771,0x0add0774,0x0aad077a,0x0a7e0782,0x0a50078c,0x0a220799,0x09f407a8,0x09c807b9,0x099c07cd,0x097107e2,0x094807fa,0x09200813,0x08f9082f,0x08d3084c,0x08af086b,0x088c088c,0x086b08af,0x084c08d3,0x082f08f9,0x08130920,0x07fa0948,0x07e20971,0x07cd099c,0x07b909c8,0x07a809f4,0x07990a22,0x078c0a50,0x07820a7e,0x077a0aad,0x07740add,0x07710b0c,0x076f0b3c,0x07710b6c,0x07740b9b,0x077a0bcb,0x07820bfa,0x078c0c28,0x07990c56,0x07a80c84,0x07b90cb0,0x07cd0cdc,0x07e20d07,0x07fa0d30,0x08130d58,0x082f0d7f,0x084c0da5,0x086b0dc9,0x088c0dec,0x08af0e0d,0x08d30e2c,0x08f90e49,0x09200e65,0x09480e7e,0x09710e96,0x099c0eab,0x09c80ebf,0x09f40ed0,0x0a220edf,0x0a500eec,0x0a7e0ef6,0x0aad0efe,0x0add0f04,0x0b0c0f07,0x0b3c0f09,0x0b3c0793,0x0b0e0794,0x0ae00798,0x0ab3079d,0x0a8507a5,0x0a5807af,0x0a2c07bc,0x0a0007ca,0x09d607db,0x09ac07ed,0x09820802,0x095a0819,0x09340831,0x090e084c,0x08ea0868,0x08c70886,0x08a608a6,0x088608c7,0x086808ea,0x084c090e,0x08310934,0x0819095a,0x08020982,0x07ed09ac,0x07db09d6,0x07ca0a00,0x07bc0a2c,0x07af0a58,0x07a50a85,0x079d0ab3,0x07980ae0,0x07940b0e,0x07930b3c,0x07940b6a,0x07980b98,0x079d0bc5,0x07a50bf3,0x07af0c20,0x07bc0c4c,0x07ca0c78,0x07db0ca2,0x07ed0ccc,0x08020cf6,0x08190d1e,0x08310d44,0x084c0d6a,0x08680d8e,0x08860db1,0x08a60dd2,0x08c70df2,0x08ea0e10,0x090e0e2c,0x09340e47,0x095a0e5f,0x09820e76,0x09ac0e8b,0x09d60e9d,0x0a000eae,0x0a2c0ebc,0x0a580ec9,0x0a850ed3,0x0ab30edb,0x0ae00ee0,0x0b0e0ee4,0x0b3c0ee5,0x0b3c07b7,0x0b1007b8,0x0ae407bc,0x0ab807c1,0x0a8c07c9,0x0a6107d2,0x0a3707de,0x0a0d07ec,0x09e307fc,0x09bb080e,0x09930822,0x096d0837,0x0948084f,0x09230869,0x09010884,0x08df08a1,0x08bf08bf,0x08a108df,0x08840901,0x08690923,0x084f0948,0x0837096d,0x08220993,0x080e09bb,0x07fc09e3,0x07ec0a0d,0x07de0a37,0x07d20a61,0x07c90a8c,0x07c10ab8,0x07bc0ae4,0x07b80b10,0x07b70b3c,0x07b80b68,0x07bc0b94,0x07c10bc0,0x07c90bec,0x07d20c17,0x07de0c41,0x07ec0c6b,0x07fc0c95,0x080e0cbd,0x08220ce5,0x08370d0b,0x084f0d30,0x08690d55,0x08840d77,0x08a10d99,0x08bf0db9,0x08df0dd7,0x09010df4,0x09230e0f,0x09480e29,0x096d0e41,0x09930e56,0x09bb0e6a,0x09e30e7c,0x0a0d0e8c,0x0a370e9a,0x0a610ea6,0x0a8c0eaf,0x0ab80eb7,0x0ae40ebc,0x0b100ec0,0x0b3c0ec1,0x0b3c07db,0x0b1207dc,0x0ae707df,0x0abd07e5,0x0a9307ec,0x0a6a07f5,0x0a410801,0x0a19080e,0x09f1081d,0x09ca082e,0x09a40841,0x097f0856,0x095c086d,0x09390885,0x091708a0,0x08f708bb,0x08d908d9,0x08bb08f7,0x08a00917,0x08850939,0x086d095c,0x0856097f,0x084109a4,0x082e09ca,0x081d09f1,0x080e0a19,0x08010a41,0x07f50a6a,0x07ec0a93,0x07e50abd,0x07df0ae7,0x07dc0b12,0x07db0b3c,0x07dc0b66,0x07df0b91,0x07e50bbb,0x07ec0be5,0x07f50c0e,0x08010c37,0x080e0c5f,0x081d0c87,0x082e0cae,0x08410cd4,0x08560cf9,0x086d0d1c,0x08850d3f,0x08a00d61,0x08bb0d81,0x08d90d9f,0x08f70dbd,0x09170dd8,0x09390df3,0x095c0e0b,0x097f0e22,0x09a40e37,0x09ca0e4a,0x09f10e5b,0x0a190e6a,0x0a410e77,0x0a6a0e83,0x0a930e8c,0x0abd0e93,0x0ae70e99,0x0b120e9c,0x0b3c0e9d,0x0b3c07ff,0x0b130800,0x0aeb0803,0x0ac20808,0x0a9a080f,
        0x0a730818,0x0a4b0823,0x0a250830,0x09ff083e,0x09da084f,0x09b50861,0x09920875,0x0970088b,0x094e08a2,0x092e08bb,0x090f08d6,0x08f208f2,0x08d6090f,0x08bb092e,0x08a2094e,0x088b0970,0x08750992,0x086109b5,0x084f09da,0x083e09ff,0x08300a25,0x08230a4b,0x08180a73,0x080f0a9a,0x08080ac2,0x08030aeb,0x08000b13,0x07ff0b3c,0x08000b65,0x08030b8d,0x08080bb6,0x080f0bde,0x08180c05,0x08230c2d,0x08300c53,0x083e0c79,0x084f0c9e,0x08610cc3,0x08750ce6,0x088b0d08,0x08a20d2a,0x08bb0d4a,0x08d60d69,0x08f20d86,0x090f0da2,0x092e0dbd,0x094e0dd6,0x09700ded,0x09920e03,0x09b50e17,0x09da0e29,0x09ff0e3a,0x0a250e48,0x0a4b0e55,0x0a730e60,0x0a9a0e69,0x0ac20e70,0x0aeb0e75,0x0b130e78,0x0b3c0e79,0x0b3c0823,0x0b150824,0x0aee0827,0x0ac8082c,0x0aa10833,0x0a7b083b,0x0a560845,0x0a310852,0x0a0d0860,0x09e9086f,0x09c60881,0x09a40894,0x098408a9,0x096408bf,0x094508d7,0x092808f1,0x090b090b,0x08f10928,0x08d70945,0x08bf0964,0x08a90984,0x089409a4,0x088109c6,0x086f09e9,0x08600a0d,0x08520a31,0x08450a56,0x083b0a7b,0x08330aa1,0x082c0ac8,0x08270aee,0x08240b15,0x08230b3c,0x08240b63,0x08270b8a,0x082c0bb0,0x08330bd7,0x083b0bfd,0x08450c22,0x08520c47,0x08600c6b,0x086f0c8f,0x08810cb2,0x08940cd4,0x08a90cf4,0x08bf0d14,0x08d70d33,0x08f10d50,0x090b0d6d,0x09280d87,0x09450da1,0x09640db9,0x09840dcf,0x09a40de4,0x09c60df7,0x09e90e09,0x0a0d0e18,0x0a310e26,0x0a560e33,0x0a7b0e3d,0x0aa10e45,0x0ac80e4c,0x0aee0e51,0x0b150e54,0x0b3c0e55,0x0b3c0847,0x0b170848,0x0af2084b,0x0acd084f,0x0aa80856,0x0a84085e,0x0a600868,0x0a3d0873,0x0a1a0881,0x09f80890,0x09d708a1,0x09b708b3,0x099808c7,0x097908dc,0x095c08f3,0x0940090b,0x09250925,0x090b0940,0x08f3095c,0x08dc0979,0x08c70998,0x08b309b7,0x08a109d7,0x089009f8,0x08810a1a,0x08730a3d,0x08680a60,0x085e0a84,0x08560aa8,0x084f0acd,0x084b0af2,0x08480b17,0x08470b3c,0x08480b61,0x084b0b86,0x084f0bab,0x08560bd0,0x085e0bf4,0x08680c18,0x08730c3b,0x08810c5e,0x08900c80,0x08a10ca1,0x08b30cc1,0x08c70ce0,0x08dc0cff,0x08f30d1c,0x090b0d38,0x09250d53,0x09400d6d,0x095c0d85,0x09790d9c,0x09980db1,0x09b70dc5,0x09d70dd7,0x09f80de8,0x0a1a0df7,0x0a3d0e05,0x0a600e10,0x0a840e1a,0x0aa80e22,0x0acd0e29,0x0af20e2d,0x0b170e30,0x0b3c0e31,0x0b3c086b,0x0b19086c,0x0af5086f,0x0ad20873,0x0aaf0879,0x0a8d0881,0x0a6b088a,0x0a490895,0x0a2808a2,0x0a0808b0,0x09e808c0,0x09c908d2,0x09ac08e5,0x098f08f9,0x0973090f,0x09580926,0x093e093e,0x09260958,0x090f0973,0x08f9098f,0x08e509ac,0x08d209c9,0x08c009e8,0x08b00a08,0x08a20a28,0x08950a49,0x088a0a6b,0x08810a8d,0x08790aaf,0x08730ad2,0x086f0af5,0x086c0b19,0x086b0b3c,0x086c0b5f,0x086f0b83,0x08730ba6,0x08790bc9,0x08810beb,0x088a0c0d,0x08950c2f,0x08a20c50,0x08b00c70,0x08c00c90,0x08d20caf,0x08e50ccc,0x08f90ce9,0x090f0d05,0x09260d20,0x093e0d3a,0x09580d52,0x09730d69,0x098f0d7f,0x09ac0d93,0x09c90da6,0x09e80db8,0x0a080dc8,0x0a280dd6,0x0a490de3,0x0a6b0dee,0x0a8d0df7,0x0aaf0dff,0x0ad20e05,0x0af50e09,0x0b190e0c,0x0b3c0e0d,0x0b3c088f,0x0b1a0890,0x0af90893,0x0ad80897,0x0ab6089c,0x0a9608a4,0x0a7508ad,0x0a5508b7,0x0a3608c3,0x0a1708d1,0x09f908e0,0x09dc08f1,0x09c00903,0x09a40916,0x098a092b,0x09700941,0x09580958,0x09410970,0x092b098a,0x091609a4,0x090309c0,0x08f109dc,0x08e009f9,0x08d10a17,0x08c30a36,0x08b70a55,0x08ad0a75,0x08a40a96,0x089c0ab6,0x08970ad8,0x08930af9,0x08900b1a,0x088f0b3c,0x08900b5e,0x08930b7f,0x08970ba0,0x089c0bc2,0x08a40be2,0x08ad0c03,0x08b70c23,0x08c30c42,0x08d10c61,0x08e00c7f,0x08f10c9c,0x09030cb8,0x09160cd4,0x092b0cee,0x09410d08,0x09580d20,0x09700d37,0x098a0d4d,0x09a40d62,0x09c00d75,0x09dc0d87,0x09f90d98,0x0a170da7,0x0a360db5,0x0a550dc1,0x0a750dcb,0x0a960dd4,0x0ab60ddc,0x0ad80de1,0x0af90de5,0x0b1a0de8,0x0b3c0de9,0x0b3c08b3,0x0b1c08b4,0x0afc08b6,0x0add08ba,0x0abd08c0,0x0a9e08c7,0x0a8008cf,0x0a6108d9,0x0a4408e5,0x0a2708f2,0x0a0a0900,0x09ee0910,0x09d40921,0x09ba0933,0x09a00946,0x0988095b,0x09710971,0x095b0988,0x094609a0,0x093309ba,0x092109d4,0x091009ee,0x09000a0a,0x08f20a27,0x08e50a44,0x08d90a61,0x08cf0a80,0x08c70a9e,0x08c00abd,0x08ba0add,0x08b60afc,0x08b40b1c,0x08b30b3c,0x08b40b5c,0x08b60b7c,0x08ba0b9b,0x08c00bbb,0x08c70bda,0x08cf0bf8,0x08d90c17,0x08e50c34,0x08f20c51,0x09000c6e,0x09100c8a,0x09210ca4,0x09330cbe,0x09460cd8,0x095b0cf0,0x09710d07,0x09880d1d,0x09a00d32,
        0x09ba0d45,0x09d40d57,0x09ee0d68,0x0a0a0d78,0x0a270d86,0x0a440d93,0x0a610d9f,0x0a800da9,0x0a9e0db1,0x0abd0db8,0x0add0dbe,0x0afc0dc2,0x0b1c0dc4,0x0b3c0dc5,0x0b3c08d7,0x0b1e08d8,0x0b0008da,0x0ae208de,0x0ac408e3,0x0aa708ea,0x0a8a08f2,0x0a6e08fb,0x0a510906,0x0a360912,0x0a1b0920,0x0a01092e,0x09e8093e,0x09cf0950,0x09b70962,0x09a00976,0x098b098b,0x097609a0,0x096209b7,0x095009cf,0x093e09e8,0x092e0a01,0x09200a1b,0x09120a36,0x09060a51,0x08fb0a6e,0x08f20a8a,0x08ea0aa7,0x08e30ac4,0x08de0ae2,0x08da0b00,0x08d80b1e,0x08d70b3c,0x08d80b5a,0x08da0b78,0x08de0b96,0x08e30bb4,0x08ea0bd1,0x08f20bee,0x08fb0c0a,0x09060c27,0x09120c42,0x09200c5d,0x092e0c77,0x093e0c90,0x09500ca9,0x09620cc1,0x09760cd8,0x098b0ced,0x09a00d02,0x09b70d16,0x09cf0d28,0x09e80d3a,0x0a010d4a,0x0a1b0d58,0x0a360d66,0x0a510d72,0x0a6e0d7d,0x0a8a0d86,0x0aa70d8e,0x0ac40d95,0x0ae20d9a,0x0b000d9e,0x0b1e0da0,0x0b3c0da1,0x0b3c08fb,0x0b2008fc,0x0b0308fe,0x0ae70901,0x0acb0906,0x0ab0090c,0x0a950914,0x0a7a091d,0x0a5f0927,0x0a450933,0x0a2c093f,0x0a13094d,0x09fc095c,0x09e4096d,0x09ce097e,0x09b90991,0x09a409a4,0x099109b9,0x097e09ce,0x096d09e4,0x095c09fc,0x094d0a13,0x093f0a2c,0x09330a45,0x09270a5f,0x091d0a7a,0x09140a95,0x090c0ab0,0x09060acb,0x09010ae7,0x08fe0b03,0x08fc0b20,0x08fb0b3c,0x08fc0b58,0x08fe0b75,0x09010b91,0x09060bad,0x090c0bc8,0x09140be3,0x091d0bfe,0x09270c19,0x09330c33,0x093f0c4c,0x094d0c65,0x095c0c7c,0x096d0c94,0x097e0caa,0x09910cbf,0x09a40cd4,0x09b90ce7,0x09ce0cfa,0x09e40d0b,0x09fc0d1c,0x0a130d2b,0x0a2c0d39,0x0a450d45,0x0a5f0d51,0x0a7a0d5b,0x0a950d64,0x0ab00d6c,0x0acb0d72,0x0ae70d77,0x0b030d7a,0x0b200d7c,0x0b3c0d7d
        },
        // BOT
        {
        0x000008fb,0x001c08fc,0x003908fe,0x00550901,0x00710906,0x008c090c,0x00a70914,0x00c2091d,0x00dd0927,0x00f70933,0x0110093f,0x0129094d,0x0140095c,0x0158096d,0x016e097e,0x01830991,0x019809a4,0x01ab09b9,0x01be09ce,0x01cf09e4,0x01e009fc,0x01ef0a13,0x01fd0a2c,0x02090a45,0x02150a5f,0x021f0a7a,0x02280a95,0x02300ab0,0x02360acb,0x023b0ae7,0x023e0b03,0x02400b20,0x02410b3c,0x02400b58,0x023e0b75,0x023b0b91,0x02360bad,0x02300bc8,0x02280be3,0x021f0bfe,0x02150c19,0x02090c33,0x01fd0c4c,0x01ef0c65,0x01e00c7c,0x01cf0c94,0x01be0caa,0x01ab0cbf,0x01980cd4,0x01830ce7,0x016e0cfa,0x01580d0b,0x01400d1c,0x01290d2b,0x01100d39,0x00f70d45,0x00dd0d51,0x00c20d5b,0x00a70d64,0x008c0d6c,0x00710d72,0x00550d77,0x00390d7a,0x001c0d7c,0x00000d7d,0x000008d7,0x001e08d8,0x003c08da,0x005a08de,0x007808e3,0x009508ea,0x00b208f2,0x00ce08fb,0x00eb0906,0x01060912,0x01210920,0x013b092e,0x0154093e,0x016d0950,0x01850962,0x019c0976,0x01b1098b,0x01c609a0,0x01da09b7,0x01ec09cf,0x01fe09e8,0x020e0a01,0x021c0a1b,0x022a0a36,0x02360a51,0x02410a6e,0x024a0a8a,0x02520aa7,0x02590ac4,0x025e0ae2,0x02620b00,0x02640b1e,0x02650b3c,0x02640b5a,0x02620b78,0x025e0b96,0x02590bb4,0x02520bd1,0x024a0bee,0x02410c0a,0x02360c27,0x022a0c42,0x021c0c5d,0x020e0c77,0x01fe0c90,0x01ec0ca9,0x01da0cc1,0x01c60cd8,0x01b10ced,0x019c0d02,0x01850d16,0x016d0d28,0x01540d3a,0x013b0d4a,0x01210d58,0x01060d66,0x00eb0d72,0x00ce0d7d,0x00b20d86,0x00950d8e,0x00780d95,0x005a0d9a,0x003c0d9e,0x001e0da0,0x00000da1,0x000008b3,0x002008b4,0x004008b6,0x005f08ba,0x007f08c0,0x009e08c7,0x00bc08cf,0x00db08d9,0x00f808e5,0x011508f2,0x01320900,0x014e0910,0x01680921,0x01820933,0x019c0946,0x01b4095b,0x01cb0971,0x01e10988,0x01f609a0,0x020909ba,0x021b09d4,0x022c09ee,0x023c0a0a,0x024a0a27,0x02570a44,0x02630a61,0x026d0a80,0x02750a9e,0x027c0abd,0x02820add,0x02860afc,0x02880b1c,0x02890b3c,0x02880b5c,0x02860b7c,0x02820b9b,0x027c0bbb,0x02750bda,0x026d0bf8,0x02630c17,0x02570c34,0x024a0c51,0x023c0c6e,0x022c0c8a,0x021b0ca4,0x02090cbe,0x01f60cd8,0x01e10cf0,0x01cb0d07,0x01b40d1d,0x019c0d32,0x01820d45,0x01680d57,0x014e0d68,0x01320d78,0x01150d86,0x00f80d93,0x00db0d9f,0x00bc0da9,0x009e0db1,0x007f0db8,0x005f0dbe,0x00400dc2,0x00200dc4,0x00000dc5,0x0000088f,0x00220890,0x00430893,0x00640897,0x0086089c,0x00a608a4,0x00c708ad,0x00e708b7,0x010608c3,0x012508d1,0x014308e0,0x016008f1,0x017c0903,0x01980916,0x01b2092b,0x01cc0941,0x01e40958,0x01fb0970,0x0211098a,0x022609a4,0x023909c0,0x024b09dc,0x025c09f9,0x026b0a17,0x02790a36,0x02850a55,0x028f0a75,0x02980a96,0x02a00ab6,0x02a50ad8,0x02a90af9,0x02ac0b1a,0x02ad0b3c,0x02ac0b5e,0x02a90b7f,0x02a50ba0,0x02a00bc2,0x02980be2,0x028f0c03,0x02850c23,0x02790c42,0x026b0c61,0x025c0c7f,0x024b0c9c,0x02390cb8,0x02260cd4,0x02110cee,0x01fb0d08,0x01e40d20,0x01cc0d37,0x01b20d4d,0x01980d62,0x017c0d75,0x01600d87,0x01430d98,0x01250da7,0x01060db5,0x00e70dc1,0x00c70dcb,0x00a60dd4,0x00860ddc,0x00640de1,0x00430de5,0x00220de8,0x00000de9,0x0000086b,0x0023086c,0x0047086f,0x006a0873,0x008d0879,0x00af0881,0x00d1088a,0x00f30895,0x011408a2,0x013408b0,0x015408c0,0x017308d2,0x019008e5,0x01ad08f9,0x01c9090f,0x01e40926,0x01fe093e,0x02160958,0x022d0973,0x0243098f,0x025709ac,0x026a09c9,0x027c09e8,0x028c0a08,0x029a0a28,0x02a70a49,0x02b20a6b,0x02bb0a8d,0x02c30aaf,0x02c90ad2,0x02cd0af5,0x02d00b19,0x02d10b3c,0x02d00b5f,0x02cd0b83,0x02c90ba6,0x02c30bc9,0x02bb0beb,0x02b20c0d,0x02a70c2f,0x029a0c50,0x028c0c70,0x027c0c90,0x026a0caf,0x02570ccc,0x02430ce9,0x022d0d05,0x02160d20,0x01fe0d3a,0x01e40d52,0x01c90d69,0x01ad0d7f,0x01900d93,0x01730da6,0x01540db8,0x01340dc8,0x01140dd6,0x00f30de3,0x00d10dee,0x00af0df7,0x008d0dff,0x006a0e05,0x00470e09,0x00230e0c,0x00000e0d,0x00000847,0x00250848,0x004a084b,0x006f084f,0x00940856,0x00b8085e,0x00dc0868,0x00ff0873,0x01220881,0x01440890,0x016508a1,0x018508b3,0x01a408c7,0x01c308dc,0x01e008f3,0x01fc090b,0x02170925,0x02310940,0x0249095c,0x02600979,0x02750998,0x028909b7,0x029b09d7,0x02ac09f8,0x02bb0a1a,0x02c90a3d,0x02d40a60,0x02de0a84,0x02e60aa8,0x02ed0acd,0x02f10af2,0x02f40b17,0x02f50b3c,0x02f40b61,0x02f10b86,0x02ed0bab,0x02e60bd0,0x02de0bf4,0x02d40c18,0x02c90c3b,0x02bb0c5e,0x02ac0c80,0x029b0ca1,0x02890cc1,0x02750ce0,0x02600cff,
        0x02490d1c,0x02310d38,0x02170d53,0x01fc0d6d,0x01e00d85,0x01c30d9c,0x01a40db1,0x01850dc5,0x01650dd7,0x01440de8,0x01220df7,0x00ff0e05,0x00dc0e10,0x00b80e1a,0x00940e22,0x006f0e29,0x004a0e2d,0x00250e30,0x00000e31,0x00000823,0x00270824,0x004e0827,0x0074082c,0x009b0833,0x00c1083b,0x00e60845,0x010b0852,0x012f0860,0x0153086f,0x01760881,0x01980894,0x01b808a9,0x01d808bf,0x01f708d7,0x021408f1,0x0231090b,0x024b0928,0x02650945,0x027d0964,0x02930984,0x02a809a4,0x02bb09c6,0x02cd09e9,0x02dc0a0d,0x02ea0a31,0x02f70a56,0x03010a7b,0x03090aa1,0x03100ac8,0x03150aee,0x03180b15,0x03190b3c,0x03180b63,0x03150b8a,0x03100bb0,0x03090bd7,0x03010bfd,0x02f70c22,0x02ea0c47,0x02dc0c6b,0x02cd0c8f,0x02bb0cb2,0x02a80cd4,0x02930cf4,0x027d0d14,0x02650d33,0x024b0d50,0x02310d6d,0x02140d87,0x01f70da1,0x01d80db9,0x01b80dcf,0x01980de4,0x01760df7,0x01530e09,0x012f0e18,0x010b0e26,0x00e60e33,0x00c10e3d,0x009b0e45,0x00740e4c,0x004e0e51,0x00270e54,0x00000e55,0x000007ff,0x00290800,0x00510803,0x007a0808,0x00a2080f,0x00c90818,0x00f10823,0x01170830,0x013d083e,0x0162084f,0x01870861,0x01aa0875,0x01cc088b,0x01ee08a2,0x020e08bb,0x022d08d6,0x024a08f2,0x0266090f,0x0281092e,0x029a094e,0x02b10970,0x02c70992,0x02db09b5,0x02ed09da,0x02fe09ff,0x030c0a25,0x03190a4b,0x03240a73,0x032d0a9a,0x03340ac2,0x03390aeb,0x033c0b13,0x033d0b3c,0x033c0b65,0x03390b8d,0x03340bb6,0x032d0bde,0x03240c05,0x03190c2d,0x030c0c53,0x02fe0c79,0x02ed0c9e,0x02db0cc3,0x02c70ce6,0x02b10d08,0x029a0d2a,0x02810d4a,0x02660d69,0x024a0d86,0x022d0da2,0x020e0dbd,0x01ee0dd6,0x01cc0ded,0x01aa0e03,0x01870e17,0x01620e29,0x013d0e3a,0x01170e48,0x00f10e55,0x00c90e60,0x00a20e69,0x007a0e70,0x00510e75,0x00290e78,0x00000e79,0x000007db,0x002a07dc,0x005507df,0x007f07e5,0x00a907ec,0x00d207f5,0x00fb0801,0x0123080e,0x014b081d,0x0172082e,0x01980841,0x01bd0856,0x01e0086d,0x02030885,0x022508a0,0x024508bb,0x026308d9,0x028108f7,0x029c0917,0x02b70939,0x02cf095c,0x02e6097f,0x02fb09a4,0x030e09ca,0x031f09f1,0x032e0a19,0x033b0a41,0x03470a6a,0x03500a93,0x03570abd,0x035d0ae7,0x03600b12,0x03610b3c,0x03600b66,0x035d0b91,0x03570bbb,0x03500be5,0x03470c0e,0x033b0c37,0x032e0c5f,0x031f0c87,0x030e0cae,0x02fb0cd4,0x02e60cf9,0x02cf0d1c,0x02b70d3f,0x029c0d61,0x02810d81,0x02630d9f,0x02450dbd,0x02250dd8,0x02030df3,0x01e00e0b,0x01bd0e22,0x01980e37,0x01720e4a,0x014b0e5b,0x01230e6a,0x00fb0e77,0x00d20e83,0x00a90e8c,0x007f0e93,0x00550e99,0x002a0e9c,0x00000e9d,0x000007b7,0x002c07b8,0x005807bc,0x008407c1,0x00b007c9,0x00db07d2,0x010507de,0x012f07ec,0x015907fc,0x0181080e,0x01a90822,0x01cf0837,0x01f4084f,0x02190869,0x023b0884,0x025d08a1,0x027d08bf,0x029b08df,0x02b80901,0x02d30923,0x02ed0948,0x0305096d,0x031a0993,0x032e09bb,0x034009e3,0x03500a0d,0x035e0a37,0x036a0a61,0x03730a8c,0x037b0ab8,0x03800ae4,0x03840b10,0x03850b3c,0x03840b68,0x03800b94,0x037b0bc0,0x03730bec,0x036a0c17,0x035e0c41,0x03500c6b,0x03400c95,0x032e0cbd,0x031a0ce5,0x03050d0b,0x02ed0d30,0x02d30d55,0x02b80d77,0x029b0d99,0x027d0db9,0x025d0dd7,0x023b0df4,0x02190e0f,0x01f40e29,0x01cf0e41,0x01a90e56,0x01810e6a,0x01590e7c,0x012f0e8c,0x01050e9a,0x00db0ea6,0x00b00eaf,0x00840eb7,0x00580ebc,0x002c0ec0,0x00000ec1,0x00000793,0x002e0794,0x005c0798,0x0089079d,0x00b707a5,0x00e407af,0x011007bc,0x013c07ca,0x016607db,0x019007ed,0x01ba0802,0x01e20819,0x02080831,0x022e084c,0x02520868,0x02750886,0x029608a6,0x02b608c7,0x02d408ea,0x02f0090e,0x030b0934,0x0323095a,0x033a0982,0x034f09ac,0x036109d6,0x03720a00,0x03800a2c,0x038d0a58,0x03970a85,0x039f0ab3,0x03a40ae0,0x03a80b0e,0x03a90b3c,0x03a80b6a,0x03a40b98,0x039f0bc5,0x03970bf3,0x038d0c20,0x03800c4c,0x03720c78,0x03610ca2,0x034f0ccc,0x033a0cf6,0x03230d1e,0x030b0d44,0x02f00d6a,0x02d40d8e,0x02b60db1,0x02960dd2,0x02750df2,0x02520e10,0x022e0e2c,0x02080e47,0x01e20e5f,0x01ba0e76,0x01900e8b,0x01660e9d,0x013c0eae,0x01100ebc,0x00e40ec9,0x00b70ed3,0x00890edb,0x005c0ee0,0x002e0ee4,0x00000ee5,0x0000076f,0x00300771,0x005f0774,0x008f077a,0x00be0782,0x00ec078c,0x011a0799,0x014807a8,0x017407b9,0x01a007cd,0x01cb07e2,0x01f407fa,0x021c0813,0x0243082f,0x0269084c,0x028d086b,0x02b0088c,0x02d108af,0x02f008d3,0x030d08f9,0x03290920,0x03420948,0x035a0971,0x036f099c,0x038309c8,0x039409f4,0x03a30a22,
        0x03b00a50,0x03ba0a7e,0x03c20aad,0x03c80add,0x03cb0b0c,0x03cd0b3c,0x03cb0b6c,0x03c80b9b,0x03c20bcb,0x03ba0bfa,0x03b00c28,0x03a30c56,0x03940c84,0x03830cb0,0x036f0cdc,0x035a0d07,0x03420d30,0x03290d58,0x030d0d7f,0x02f00da5,0x02d10dc9,0x02b00dec,0x028d0e0d,0x02690e2c,0x02430e49,0x021c0e65,0x01f40e7e,0x01cb0e96,0x01a00eab,0x01740ebf,0x01480ed0,0x011a0edf,0x00ec0eec,0x00be0ef6,0x008f0efe,0x005f0f04,0x00300f07,0x00000f09,0x0000074b,0x0031074d,0x00630750,0x00940756,0x00c5075f,0x00f5076a,0x01250777,0x01540786,0x01820798,0x01af07ac,0x01db07c2,0x020707db,0x023007f5,0x02590812,0x02800830,0x02a50851,0x02c90873,0x02eb0897,0x030c08bc,0x032a08e3,0x0347090c,0x03610935,0x037a0961,0x0390098d,0x03a409ba,0x03b609e8,0x03c50a17,0x03d20a47,0x03dd0a77,0x03e60aa8,0x03ec0ad9,0x03ef0b0b,0x03f10b3c,0x03ef0b6d,0x03ec0b9f,0x03e60bd0,0x03dd0c01,0x03d20c31,0x03c50c61,0x03b60c90,0x03a40cbe,0x03900ceb,0x037a0d17,0x03610d43,0x03470d6c,0x032a0d95,0x030c0dbc,0x02eb0de1,0x02c90e05,0x02a50e27,0x02800e48,0x02590e66,0x02300e83,0x02070e9d,0x01db0eb6,0x01af0ecc,0x01820ee0,0x01540ef2,0x01250f01,0x00f50f0e,0x00c50f19,0x00940f22,0x00630f28,0x00310f2b,0x00000f2d,0x00000727,0x00330729,0x0066072c,0x00990733,0x00cc073b,0x00fe0747,0x012f0754,0x01600764,0x01900777,0x01bf078c,0x01ec07a3,0x021907bc,0x024407d7,0x026e07f5,0x02970814,0x02be0836,0x02e30859,0x0306087e,0x032808a5,0x034708ce,0x036508f8,0x03800923,0x03990950,0x03b0097d,0x03c509ac,0x03d809dc,0x03e80a0d,0x03f50a3e,0x04010a70,0x04090aa3,0x04100ad6,0x04130b09,0x04150b3c,0x04130b6f,0x04100ba2,0x04090bd5,0x04010c08,0x03f50c3a,0x03e80c6b,0x03d80c9c,0x03c50ccc,0x03b00cfb,0x03990d28,0x03800d55,0x03650d80,0x03470daa,0x03280dd3,0x03060dfa,0x02e30e1f,0x02be0e42,0x02970e64,0x026e0e83,0x02440ea1,0x02190ebc,0x01ec0ed5,0x01bf0eec,0x01900f01,0x01600f14,0x012f0f24,0x00fe0f31,0x00cc0f3d,0x00990f45,0x00660f4c,0x00330f4f,0x00000f51,0x00000703,0x00350705,0x006a0709,0x009f070f,0x00d30718,0x01070724,0x013a0732,0x016c0743,0x019e0756,0x01ce076b,0x01fd0783,0x022c079d,0x025807b9,0x028407d8,0x02ae07f9,0x02d6081b,0x02fc0840,0x03210866,0x0343088e,0x036408b8,0x038308e4,0x039f0910,0x03b9093f,0x03d1096e,0x03e6099e,0x03f909d0,0x040a0a02,0x04180a35,0x04240a69,0x042d0a9d,0x04330ad2,0x04370b07,0x04390b3c,0x04370b71,0x04330ba6,0x042d0bdb,0x04240c0f,0x04180c43,0x040a0c76,0x03f90ca8,0x03e60cda,0x03d10d0a,0x03b90d39,0x039f0d68,0x03830d94,0x03640dc0,0x03430dea,0x03210e12,0x02fc0e38,0x02d60e5d,0x02ae0e7f,0x02840ea0,0x02580ebf,0x022c0edb,0x01fd0ef5,0x01ce0f0d,0x019e0f22,0x016c0f35,0x013a0f46,0x01070f54,0x00d30f60,0x009f0f69,0x006a0f6f,0x00350f73,0x00000f75,0x000006df,0x003706e1,0x006d06e5,0x00a406eb,0x00da06f5,0x010f0701,0x0144070f,0x01780721,0x01ab0734,0x01dd074b,0x020e0763,0x023e077e,0x026c079c,0x029907bb,0x02c407dd,0x02ee0801,0x03160826,0x033b084e,0x035f0878,0x038108a3,0x03a008d0,0x03be08fe,0x03d9092e,0x03f1095f,0x04080991,0x041b09c4,0x042d09f8,0x043b0a2d,0x04470a62,0x04510a98,0x04570acf,0x045b0b05,0x045d0b3c,0x045b0b73,0x04570ba9,0x04510be0,0x04470c16,0x043b0c4b,0x042d0c80,0x041b0cb4,0x04080ce7,0x03f10d19,0x03d90d4a,0x03be0d7a,0x03a00da8,0x03810dd5,0x035f0e00,0x033b0e2a,0x03160e52,0x02ee0e77,0x02c40e9b,0x02990ebd,0x026c0edc,0x023e0efa,0x020e0f15,0x01dd0f2d,0x01ab0f44,0x01780f57,0x01440f69,0x010f0f77,0x00da0f83,0x00a40f8d,0x006d0f93,0x00370f97,0x00000f99,0x000006bb,0x003906bd,0x007106c1,0x00a906c8,0x00e106d2,0x011806de,0x014f06ed,0x018406ff,0x01b90713,0x01ed072a,0x021f0743,0x0251075f,0x0280077e,0x02af079e,0x02db07c1,0x030607e6,0x032f080d,0x03560836,0x037b0861,0x039e088d,0x03be08bc,0x03dd08eb,0x03f9091d,0x0412094f,0x04290983,0x043d09b8,0x044f09ed,0x045e0a24,0x046a0a5b,0x04740a93,0x047b0acb,0x047f0b03,0x04810b3c,0x047f0b75,0x047b0bad,0x04740be5,0x046a0c1d,0x045e0c54,0x044f0c8b,0x043d0cc0,0x04290cf5,0x04120d29,0x03f90d5b,0x03dd0d8d,0x03be0dbc,0x039e0deb,0x037b0e17,0x03560e42,0x032f0e6b,0x03060e92,0x02db0eb7,0x02af0eda,0x02800efa,0x02510f19,0x021f0f35,0x01ed0f4e,0x01b90f65,0x01840f79,0x014f0f8b,0x01180f9a,0x00e10fa6,0x00a90fb0,0x00710fb7,0x00390fbb,0x00000fbd,0x00000697,0x003a0699,0x0075069d,0x00ae06a4,0x00e806ae,0x012106bb,0x015906cb,0x019006dd,
        0x01c706f2,0x01fc070a,0x02300724,0x02630741,0x02940760,0x02c40781,0x02f207a5,0x031e07cb,0x034807f4,0x0371081e,0x0397084a,0x03bb0878,0x03dc08a8,0x03fb08d9,0x0418090c,0x04320940,0x044a0975,0x045f09ac,0x047109e3,0x04810a1b,0x048e0a54,0x04980a8e,0x049f0ac7,0x04a30b02,0x04a50b3c,0x04a30b76,0x049f0bb1,0x04980bea,0x048e0c24,0x04810c5d,0x04710c95,0x045f0ccc,0x044a0d03,0x04320d38,0x04180d6c,0x03fb0d9f,0x03dc0dd0,0x03bb0e00,0x03970e2e,0x03710e5a,0x03480e84,0x031e0ead,0x02f20ed3,0x02c40ef7,0x02940f18,0x02630f37,0x02300f54,0x01fc0f6e,0x01c70f86,0x01900f9b,0x01590fad,0x01210fbd,0x00e80fca,0x00ae0fd4,0x00750fdb,0x003a0fdf,0x00000fe1,0x00000673,0x003c0675,0x00780679,0x00b40681,0x00ef068b,0x012a0698,0x016306a8,0x019d06bb,0x01d506d1,0x020c06e9,0x02410704,0x02760722,0x02a80742,0x02d90764,0x03090789,0x033607b1,0x036207da,0x038b0806,0x03b30833,0x03d80863,0x03fa0894,0x041a08c6,0x043808fb,0x04530930,0x046b0967,0x0481099f,0x049409d9,0x04a40a12,0x04b10a4d,0x04bb0a88,0x04c30ac4,0x04c70b00,0x04c90b3c,0x04c70b78,0x04c30bb4,0x04bb0bf0,0x04b10c2b,0x04a40c66,0x04940c9f,0x04810cd9,0x046b0d11,0x04530d48,0x04380d7d,0x041a0db2,0x03fa0de4,0x03d80e15,0x03b30e45,0x038b0e72,0x03620e9e,0x03360ec7,0x03090eef,0x02d90f14,0x02a80f36,0x02760f56,0x02410f74,0x020c0f8f,0x01d50fa7,0x019d0fbd,0x01630fd0,0x012a0fe0,0x00ef0fed,0x00b40ff7,0x00780fff,0x003c1003,0x00001005,0x0000064f,0x003e0651,0x007c0656,0x00b9065d,0x00f60668,0x01320675,0x016e0686,0x01a90699,0x01e206af,0x021b06c8,0x025206e4,0x02880703,0x02bc0724,0x02ef0748,0x0320076e,0x034f0796,0x037b07c1,0x03a607ed,0x03ce081c,0x03f4084d,0x04180880,0x043908b4,0x045808ea,0x04740921,0x048d095a,0x04a30993,0x04b609ce,0x04c70a0a,0x04d40a46,0x04df0a83,0x04e60ac0,0x04eb0afe,0x04ed0b3c,0x04eb0b7a,0x04e60bb8,0x04df0bf5,0x04d40c32,0x04c70c6e,0x04b60caa,0x04a30ce5,0x048d0d1e,0x04740d57,0x04580d8e,0x04390dc4,0x04180df8,0x03f40e2b,0x03ce0e5c,0x03a60e8b,0x037b0eb7,0x034f0ee2,0x03200f0a,0x02ef0f30,0x02bc0f54,0x02880f75,0x02520f94,0x021b0fb0,0x01e20fc9,0x01a90fdf,0x016e0ff2,0x01321003,0x00f61010,0x00b9101b,0x007c1022,0x003e1027,0x00001029,0x0000062b,0x0040062d,0x007f0632,0x00be0639,0x00fd0644,0x013b0652,0x01780663,0x01b50677,0x01f0068e,0x022a06a8,0x026306c5,0x029b06e4,0x02d00706,0x0304072b,0x03370752,0x0367077b,0x039507a7,0x03c107d5,0x03ea0805,0x04110838,0x0436086c,0x045808a1,0x047708d9,0x04940912,0x04ae094c,0x04c50987,0x04d909c4,0x04ea0a01,0x04f80a3f,0x05030a7e,0x050a0abd,0x050f0afc,0x05110b3c,0x050f0b7c,0x050a0bbb,0x05030bfa,0x04f80c39,0x04ea0c77,0x04d90cb4,0x04c50cf1,0x04ae0d2c,0x04940d66,0x04770d9f,0x04580dd7,0x04360e0c,0x04110e40,0x03ea0e73,0x03c10ea3,0x03950ed1,0x03670efd,0x03370f26,0x03040f4d,0x02d00f72,0x029b0f94,0x02630fb3,0x022a0fd0,0x01f00fea,0x01b51001,0x01781015,0x013b1026,0x00fd1034,0x00be103f,0x007f1046,0x0040104b,0x0000104d,0x00000607,0x00410609,0x0083060e,0x00c40616,0x01040621,0x0144062f,0x01830641,0x01c10655,0x01fe066d,0x023a0687,0x027406a5,0x02ad06c5,0x02e406e8,0x031a070e,0x034d0736,0x037f0761,0x03ae078e,0x03db07bd,0x040607ef,0x042e0822,0x04540858,0x0477088f,0x049708c8,0x04b50902,0x04cf093e,0x04e7097b,0x04fb09b9,0x050d09f8,0x051b0a38,0x05260a78,0x052e0ab9,0x05330afb,0x05350b3c,0x05330b7d,0x052e0bbf,0x05260c00,0x051b0c40,0x050d0c80,0x04fb0cbf,0x04e70cfd,0x04cf0d3a,0x04b50d76,0x04970db0,0x04770de9,0x04540e20,0x042e0e56,0x04060e89,0x03db0ebb,0x03ae0eea,0x037f0f17,0x034d0f42,0x031a0f6a,0x02e40f90,0x02ad0fb3,0x02740fd3,0x023a0ff1,0x01fe100b,0x01c11023,0x01831037,0x01441049,0x01041057,0x00c41062,0x0083106a,0x0041106f,0x00001071,0x000005e3,0x004305e5,0x008605ea,0x00c905f2,0x010b05fe,0x014d060c,0x018d061e,0x01cd0633,0x020c064c,0x02490667,0x02850685,0x02c006a6,0x02f806ca,0x032f06f1,0x0364071a,0x03970746,0x03c80774,0x03f607a5,0x042207d8,0x044b080d,0x04720844,0x0496087c,0x04b708b7,0x04d508f3,0x04f00930,0x0509096f,0x051e09af,0x053009ef,0x053e0a31,0x054a0a73,0x05520ab6,0x05570af9,0x05590b3c,0x05570b7f,0x05520bc2,0x054a0c05,0x053e0c47,0x05300c89,0x051e0cc9,0x05090d09,0x04f00d48,0x04d50d85,0x04b70dc1,0x04960dfc,0x04720e34,0x044b0e6b,0x04220ea0,0x03f60ed3,0x03c80f04,0x03970f32,0x03640f5e,0x032f0f87,0x02f80fae,0x02c00fd2,
        0x02850ff3,0x02491011,0x020c102c,0x01cd1045,0x018d105a,0x014d106c,0x010b107a,0x00c91086,0x0086108e,0x00431093,0x00001095,0x000005bf,0x004505c1,0x008a05c6,0x00ce05cf,0x011205da,0x015505ea,0x019805fc,0x01d90612,0x0219062a,0x02590646,0x02960665,0x02d20687,0x030c06ac,0x034506d4,0x037b06fe,0x03af072b,0x03e1075b,0x0411078d,0x043e07c1,0x046807f7,0x04900830,0x04b5086a,0x04d708a6,0x04f608e3,0x05120923,0x052a0963,0x054009a4,0x055209e7,0x05620a2a,0x056d0a6e,0x05760ab2,0x057b0af7,0x057d0b3c,0x057b0b81,0x05760bc6,0x056d0c0a,0x05620c4e,0x05520c91,0x05400cd4,0x052a0d15,0x05120d55,0x04f60d95,0x04d70dd2,0x04b50e0e,0x04900e48,0x04680e81,0x043e0eb7,0x04110eeb,0x03e10f1d,0x03af0f4d,0x037b0f7a,0x03450fa4,0x030c0fcc,0x02d20ff1,0x02961013,0x02591032,0x0219104e,0x01d91066,0x0198107c,0x0155108e,0x0112109e,0x00ce10a9,0x008a10b2,0x004510b7,0x000010b9,0x0000059b,0x0047059d,0x008d05a2,0x00d305ab,0x011905b7,0x015e05c7,0x01a205da,0x01e505f0,0x02270609,0x02680626,0x02a70646,0x02e50668,0x0320068e,0x035a06b7,0x039206e2,0x03c70711,0x03fb0741,0x042b0775,0x045a07aa,0x048507e2,0x04ae081c,0x04d40857,0x04f60895,0x051608d4,0x05330915,0x054c0957,0x0562099a,0x057509de,0x05850a23,0x05910a69,0x059a0aaf,0x059f0af5,0x05a10b3c,0x059f0b83,0x059a0bc9,0x05910c0f,0x05850c55,0x05750c9a,0x05620cde,0x054c0d21,0x05330d63,0x05160da4,0x04f60de3,0x04d40e21,0x04ae0e5c,0x04850e96,0x045a0ece,0x042b0f03,0x03fb0f37,0x03c70f67,0x03920f96,0x035a0fc1,0x03200fea,0x02e51010,0x02a71032,0x02681052,0x0227106f,0x01e51088,0x01a2109e,0x015e10b1,0x011910c1,0x00d310cd,0x008d10d6,0x004710db,0x000010dd,0x00000578,0x00480579,0x0091057f,0x00d90587,0x01200594,0x016705a4,0x01ad05b7,0x01f105ce,0x023505e8,0x02770605,0x02b80626,0x02f7064a,0x03340670,0x0370069a,0x03a906c7,0x03e006f6,0x04140728,0x0446075c,0x04750793,0x04a207cc,0x04cc0808,0x04f20845,0x05160884,0x053708c5,0x05540907,0x056e094b,0x0585098f,0x059809d5,0x05a80a1c,0x05b50a63,0x05bd0aab,0x05c30af4,0x05c40b3c,0x05c30b84,0x05bd0bcd,0x05b50c15,0x05a80c5c,0x05980ca3,0x05850ce9,0x056e0d2d,0x05540d71,0x05370db3,0x05160df4,0x04f20e33,0x04cc0e70,0x04a20eac,0x04750ee5,0x04460f1c,0x04140f50,0x03e00f82,0x03a90fb1,0x03700fde,0x03341008,0x02f7102e,0x02b81052,0x02771073,0x02351090,0x01f110aa,0x01ad10c1,0x016710d4,0x012010e4,0x00d910f1,0x009110f9,0x004810ff,0x00001100,0x00000554,0x004a0555,0x0094055b,0x00de0564,0x01270571,0x01700581,0x01b70595,0x01fe05ac,0x024305c7,0x028705e5,0x02c90606,0x030a062b,0x03480652,0x0385067d,0x03c006ab,0x03f806db,0x042d070f,0x04610744,0x0491077c,0x04bf07b7,0x04ea07f4,0x05110832,0x05360873,0x055708b5,0x057508f9,0x0590093e,0x05a70985,0x05bb09cc,0x05cb0a15,0x05d80a5e,0x05e10aa8,0x05e70af2,0x05e80b3c,0x05e70b86,0x05e10bd0,0x05d80c1a,0x05cb0c63,0x05bb0cac,0x05a70cf3,0x05900d3a,0x05750d7f,0x05570dc3,0x05360e05,0x05110e46,0x04ea0e84,0x04bf0ec1,0x04910efc,0x04610f34,0x042d0f69,0x03f80f9d,0x03c00fcd,0x03850ffb,0x03481026,0x030a104d,0x02c91072,0x02871093,0x024310b1,0x01fe10cc,0x01b710e3,0x017010f7,0x01271107,0x00de1114,0x0094111d,0x004a1123,0x00001124,0x00000530,0x004c0531,0x00980537,0x00e30540,0x012e054d,0x0178055e,0x01c10572,0x020a058a,0x025105a5,0x029605c4,0x02da05e6,0x031c060c,0x035c0634,0x039a0660,0x03d6068f,0x041006c1,0x044706f5,0x047b072c,0x04ad0766,0x04dc07a2,0x050807e0,0x05300820,0x05560862,0x057808a6,0x059708eb,0x05b20932,0x05ca097b,0x05de09c4,0x05ef0a0e,0x05fc0a59,0x06050aa4,0x060b0af0,0x060c0b3c,0x060b0b88,0x06050bd4,0x05fc0c1f,0x05ef0c6a,0x05de0cb4,0x05ca0cfd,0x05b20d46,0x05970d8d,0x05780dd2,0x05560e16,0x05300e58,0x05080e98,0x04dc0ed6,0x04ad0f12,0x047b0f4c,0x04470f83,0x04100fb7,0x03d60fe9,0x039a1018,0x035c1044,0x031c106c,0x02da1092,0x029610b4,0x025110d3,0x020a10ee,0x01c11106,0x0178111a,0x012e112b,0x00e31138,0x00981141,0x004c1147,0x00001148,0x0000050c,0x004e050d,0x009b0513,0x00e8051d,0x0135052a,0x0181053b,0x01cc0550,0x02160568,0x025e0584,0x02a505a4,0x02eb05c7,0x032f05ed,0x03700617,0x03b00643,0x03ed0673,0x042806a6,0x046006dc,0x04960714,0x04c9074f,0x04f9078c,0x052507cc,0x054f080d,0x05750851,0x05980897,0x05b808de,0x05d40926,0x05ec0970,0x060109bb,0x06120a07,0x061f0a54,0x06290aa1,0x062f0aee,0x06300b3c,0x062f0b8a,0x06290bd7,
        0x061f0c24,0x06120c71,0x06010cbd,0x05ec0d08,0x05d40d52,0x05b80d9a,0x05980de1,0x05750e27,0x054f0e6b,0x05250eac,0x04f90eec,0x04c90f29,0x04960f64,0x04600f9c,0x04280fd2,0x03ed1005,0x03b01035,0x03701061,0x032f108b,0x02eb10b1,0x02a510d4,0x025e10f4,0x02161110,0x01cc1128,0x0181113d,0x0135114e,0x00e8115b,0x009b1165,0x004e116b,0x0000116c,0x000004e8,0x005004ea,0x009f04ef,0x00ee04f9,0x013c0507,0x018a0518,0x01d6052d,0x02220546,0x026c0563,0x02b50583,0x02fc05a7,0x034105ce,0x038405f9,0x03c50626,0x04040657,0x0440068b,0x047a06c2,0x04b106fc,0x04e50738,0x05160777,0x054307b8,0x056e07fb,0x05950840,0x05b90887,0x05d908d0,0x05f6091a,0x060f0966,0x062409b2,0x06350a00,0x06430a4e,0x064d0a9d,0x06520aec,0x06540b3c,0x06520b8c,0x064d0bdb,0x06430c2a,0x06350c78,0x06240cc6,0x060f0d12,0x05f60d5e,0x05d90da8,0x05b90df1,0x05950e38,0x056e0e7d,0x05430ec0,0x05160f01,0x04e50f40,0x04b10f7c,0x047a0fb6,0x04400fed,0x04041021,0x03c51052,0x0384107f,0x034110aa,0x02fc10d1,0x02b510f5,0x026c1115,0x02221132,0x01d6114b,0x018a1160,0x013c1171,0x00ee117f,0x009f1189,0x0050118e,0x00001190,0x000004c4,0x005104c6,0x00a204cc,0x00f304d6,0x014304e3,0x019204f5,0x01e1050b,0x022e0524,0x027a0542,0x02c40563,0x030d0587,0x035405af,0x039805db,0x03db060a,0x041b063c,0x04580671,0x049306a9,0x04cb06e4,0x05000721,0x05320761,0x056107a4,0x058d07e8,0x05b5082f,0x05d90878,0x05fa08c2,0x0618090e,0x0631095b,0x064709aa,0x065909f9,0x06660a49,0x06700a9a,0x06760aeb,0x06780b3c,0x06760b8d,0x06700bde,0x06660c2f,0x06590c7f,0x06470cce,0x06310d1d,0x06180d6a,0x05fa0db6,0x05d90e00,0x05b50e49,0x058d0e90,0x05610ed4,0x05320f17,0x05000f57,0x04cb0f94,0x04930fcf,0x04581007,0x041b103c,0x03db106e,0x0398109d,0x035410c9,0x030d10f1,0x02c41115,0x027a1136,0x022e1154,0x01e1116d,0x01921183,0x01431195,0x00f311a2,0x00a211ac,0x005111b2,0x000011b4,0x000004a0,0x005304a2,0x00a604a8,0x00f804b2,0x014a04c0,0x019b04d2,0x01eb04e8,0x023a0503,0x02880520,0x02d40542,0x031e0567,0x03660590,0x03ac05bd,0x03f005ed,0x04320620,0x04710656,0x04ad068f,0x04e606cb,0x051c070a,0x054f074c,0x057f0790,0x05ac07d6,0x05d5081e,0x05fa0868,0x061c08b4,0x06390902,0x06540951,0x066a09a1,0x067c09f2,0x068a0a44,0x06940a96,0x069a0ae9,0x069c0b3c,0x069a0b8f,0x06940be2,0x068a0c34,0x067c0c86,0x066a0cd7,0x06540d27,0x06390d76,0x061c0dc4,0x05fa0e10,0x05d50e5a,0x05ac0ea2,0x057f0ee8,0x054f0f2c,0x051c0f6e,0x04e60fad,0x04ad0fe9,0x04711022,0x04321058,0x03f0108b,0x03ac10bb,0x036610e8,0x031e1111,0x02d41136,0x02881158,0x023a1175,0x01eb1190,0x019b11a6,0x014a11b8,0x00f811c6,0x00a611d0,0x005311d6,0x000011d8,0x0000047c,0x0055047e,0x00a90484,0x00fe048e,0x0151049d,0x01a404af,0x01f604c6,0x024604e1,0x029504ff,0x02e30522,0x032f0548,0x03790572,0x03c0059f,0x040605d0,0x04480604,0x0489063b,0x04c60676,0x050106b3,0x053806f4,0x056c0736,0x059d077c,0x05ca07c3,0x05f4080d,0x061a0859,0x063d08a7,0x065b08f6,0x06760946,0x068d0998,0x069f09eb,0x06ae0a3e,0x06b80a93,0x06be0ae7,0x06c00b3c,0x06be0b91,0x06b80be5,0x06ae0c3a,0x069f0c8d,0x068d0ce0,0x06760d32,0x065b0d82,0x063d0dd1,0x061a0e1f,0x05f40e6b,0x05ca0eb5,0x059d0efc,0x056c0f42,0x05380f84,0x05010fc5,0x04c61002,0x0489103d,0x04481074,0x040610a8,0x03c010d9,0x03791106,0x032f1130,0x02e31156,0x02951179,0x02461197,0x01f611b2,0x01a411c9,0x015111db,0x00fe11ea,0x00a911f4,0x005511fa,0x000011fc,0x00000458,0x0057045a,0x00ad0460,0x0103046b,0x0158047a,0x01ad048c,0x020004a4,0x025204bf,0x02a304de,0x02f20501,0x03400528,0x038b0553,0x03d40581,0x041b05b3,0x045f05e8,0x04a10621,0x04e0065c,0x051b069b,0x055406dd,0x05890721,0x05bb0768,0x05e907b1,0x061407fc,0x063b084a,0x065e0899,0x067d08ea,0x0698093c,0x06b0098f,0x06c209e4,0x06d10a39,0x06dc0a8f,0x06e20ae5,0x06e40b3c,0x06e20b93,0x06dc0be9,0x06d10c3f,0x06c20c94,0x06b00ce9,0x06980d3c,0x067d0d8e,0x065e0ddf,0x063b0e2e,0x06140e7c,0x05e90ec7,0x05bb0f10,0x05890f57,0x05540f9b,0x051b0fdd,0x04e0101c,0x04a11057,0x045f1090,0x041b10c5,0x03d410f7,0x038b1125,0x03401150,0x02f21177,0x02a3119a,0x025211b9,0x020011d4,0x01ad11ec,0x015811fe,0x0103120d,0x00ad1218,0x0057121e,0x00001220,0x00000434,0x00580436,0x00b0043c,0x01080447,0x015f0456,0x01b5046a,0x020b0481,0x025f049d,0x02b104bd,0x030204e0,0x03510508,0x039e0534,0x03e80563,0x04300596,0x047605cc,0x04b90606,
        0x04f90643,0x05360683,0x057006c6,0x05a6070c,0x05d90754,0x0608079e,0x063407eb,0x065c083a,0x067f088b,0x069f08dd,0x06bb0931,0x06d20987,0x06e609dd,0x06f50a34,0x07000a8c,0x07060ae4,0x07080b3c,0x07060b94,0x07000bec,0x06f50c44,0x06e60c9b,0x06d20cf1,0x06bb0d47,0x069f0d9b,0x067f0ded,0x065c0e3e,0x06340e8d,0x06080eda,0x05d90f24,0x05a60f6c,0x05700fb2,0x05360ff5,0x04f91035,0x04b91072,0x047610ac,0x043010e2,0x03e81115,0x039e1144,0x03511170,0x03021198,0x02b111bb,0x025f11db,0x020b11f7,0x01b5120e,0x015f1222,0x01081231,0x00b0123c,0x00581242,0x00001244,0x00000410,0x005a0412,0x00b40418,0x010d0424,0x01660433,0x01be0447,0x0215045f,0x026b047b,0x02bf049b,0x031104c0,0x036204e8,0x03b00515,0x03fc0545,0x04460579,0x048d05b0,0x04d105eb,0x05130629,0x0551066b,0x058c06af,0x05c306f6,0x05f70740,0x0627078c,0x065407da,0x067c082b,0x06a1087d,0x06c108d1,0x06dd0927,0x06f5097e,0x070909d6,0x07180a2f,0x07240a88,0x072a0ae2,0x072c0b3c,0x072a0b96,0x07240bf0,0x07180c49,0x07090ca2,0x06f50cfa,0x06dd0d51,0x06c10da7,0x06a10dfb,0x067c0e4d,0x06540e9e,0x06270eec,0x05f70f38,0x05c30f82,0x058c0fc9,0x0551100d,0x0513104f,0x04d1108d,0x048d10c8,0x044610ff,0x03fc1133,0x03b01163,0x03621190,0x031111b8,0x02bf11dd,0x026b11fd,0x02151219,0x01be1231,0x01661245,0x010d1254,0x00b41260,0x005a1266,0x00001268,0x000003ec,0x005c03ee,0x00b803f5,0x01130400,0x016d0410,0x01c70424,0x0220043c,0x02770459,0x02cd047a,0x0321049f,0x037304c9,0x03c304f6,0x04100527,0x045b055c,0x04a40595,0x04e905d1,0x052c0610,0x056b0653,0x05a70698,0x05e006e1,0x0615072c,0x06460779,0x067307c9,0x069d081b,0x06c2086f,0x06e308c5,0x0700091c,0x07180975,0x072c09cf,0x073c0a29,0x07470a84,0x074e0ae0,0x07500b3c,0x074e0b98,0x07470bf4,0x073c0c4f,0x072c0ca9,0x07180d03,0x07000d5c,0x06e30db3,0x06c20e09,0x069d0e5d,0x06730eaf,0x06460eff,0x06150f4c,0x05e00f97,0x05a70fe0,0x056b1025,0x052c1068,0x04e910a7,0x04a410e3,0x045b111c,0x04101151,0x03c31182,0x037311af,0x032111d9,0x02cd11fe,0x0277121f,0x0220123c,0x01c71254,0x016d1268,0x01131278,0x00b81283,0x005c128a,0x0000128c,0x000003c8,0x005e03ca,0x00bb03d1,0x011803dc,0x017403ec,0x01d00401,0x022a041a,0x02830437,0x02da0459,0x0330047f,0x038404a9,0x03d504d7,0x04240509,0x0471053f,0x04bb0579,0x050205b6,0x054505f7,0x0586063a,0x05c30681,0x05fd06cb,0x06330718,0x06650767,0x069307b8,0x06bd080c,0x06e30862,0x070508b9,0x07220912,0x073b096c,0x075009c8,0x07600a24,0x076b0a81,0x07720ade,0x07740b3c,0x07720b9a,0x076b0bf7,0x07600c54,0x07500cb0,0x073b0d0c,0x07220d66,0x07050dbf,0x06e30e16,0x06bd0e6c,0x06930ec0,0x06650f11,0x06330f60,0x05fd0fad,0x05c30ff7,0x0586103e,0x05451081,0x050210c2,0x04bb10ff,0x04711139,0x0424116f,0x03d511a1,0x038411cf,0x033011f9,0x02da121f,0x02831241,0x022a125e,0x01d01277,0x0174128c,0x0118129c,0x00bb12a7,0x005e12ae,0x000012b0,0x000003a4,0x005f03a6,0x00bf03ad,0x011d03b9,0x017b03c9,0x01d803de,0x023403f7,0x028f0415,0x02e80438,0x033f045e,0x03950489,0x03e804b8,0x043804eb,0x04860522,0x04d1055d,0x051a059b,0x055f05dd,0x05a10622,0x05df066b,0x061a06b6,0x06510704,0x06840754,0x06b307a7,0x06de07fd,0x07040854,0x072708ad,0x07450908,0x075e0964,0x077309c1,0x07830a1f,0x078f0a7d,0x07960add,0x07980b3c,0x07960b9b,0x078f0bfb,0x07830c59,0x07730cb7,0x075e0d14,0x07450d70,0x07270dcb,0x07040e24,0x06de0e7b,0x06b30ed1,0x06840f24,0x06510f74,0x061a0fc2,0x05df100d,0x05a11056,0x055f109b,0x051a10dd,0x04d1111b,0x04861156,0x0438118d,0x03e811c0,0x039511ef,0x033f121a,0x02e81240,0x028f1263,0x02341281,0x01d8129a,0x017b12af,0x011d12bf,0x00bf12cb,0x005f12d2,0x000012d4,0x00000380,0x00610382,0x00c20389,0x01230395,0x018203a6,0x01e103bb,0x023f03d5,0x029b03f3,0x02f60416,0x034f043e,0x03a6046a,0x03fa0499,0x044c04cd,0x049c0505,0x04e80541,0x05320581,0x057805c4,0x05bb060a,0x05fb0654,0x063706a0,0x066f06f0,0x06a30742,0x06d20796,0x06fe07ed,0x07260846,0x074908a1,0x076708fd,0x0781095b,0x079609ba,0x07a70a19,0x07b30a7a,0x07ba0adb,0x07bc0b3c,0x07ba0b9d,0x07b30bfe,0x07a70c5f,0x07960cbe,0x07810d1d,0x07670d7b,0x07490dd7,0x07260e32,0x06fe0e8b,0x06d20ee2,0x06a30f36,0x066f0f88,0x06370fd8,0x05fb1024,0x05bb106e,0x057810b4,0x053210f7,0x04e81137,0x049c1173,0x044c11ab,0x03fa11df,0x03a6120e,0x034f123a,0x02f61262,0x029b1285,0x023f12a3,0x01e112bd,0x018212d2,0x012312e3,
        0x00c212ef,0x006112f6,0x000012f8,0x0000035c,0x0063035e,0x00c60365,0x01280372,0x01890382,0x01ea0398,0x024903b3,0x02a703d2,0x030403f5,0x035e041d,0x03b6044a,0x040d047b,0x046004b0,0x04b104e8,0x04ff0525,0x054a0566,0x059205aa,0x05d605f2,0x0617063d,0x0654068b,0x068c06dc,0x06c1072f,0x06f20786,0x071f07de,0x07470838,0x076a0895,0x078908f3,0x07a40952,0x07ba09b3,0x07ca0a14,0x07d70a76,0x07de0ad9,0x07e00b3c,0x07de0b9f,0x07d70c02,0x07ca0c64,0x07ba0cc5,0x07a40d26,0x07890d85,0x076a0de3,0x07470e40,0x071f0e9a,0x06f20ef2,0x06c10f49,0x068c0f9c,0x06540fed,0x0617103b,0x05d61086,0x059210ce,0x054a1112,0x04ff1153,0x04b11190,0x046011c8,0x040d11fd,0x03b6122e,0x035e125b,0x03041283,0x02a712a6,0x024912c5,0x01ea12e0,0x018912f6,0x01281306,0x00c61313,0x0063131a,0x0000131c,0x00000338,0x0065033a,0x00c90342,0x012d034e,0x0190035f,0x01f30375,0x02540390,0x02b303b0,0x031103d4,0x036d03fd,0x03c7042a,0x041f045c,0x04740492,0x04c704cc,0x0516050a,0x0562054b,0x05ab0591,0x05f105da,0x06320626,0x06700675,0x06aa06c8,0x06e0071d,0x07120775,0x073f07cf,0x0768082b,0x078c0889,0x07ac08e8,0x07c70949,0x07dd09ac,0x07ee0a0f,0x07fa0a73,0x08020ad7,0x08040b3c,0x08020ba1,0x07fa0c05,0x07ee0c69,0x07dd0ccc,0x07c70d2f,0x07ac0d90,0x078c0def,0x07680e4d,0x073f0ea9,0x07120f03,0x06e00f5b,0x06aa0fb0,0x06701003,0x06321052,0x05f1109e,0x05ab10e7,0x0562112d,0x0516116e,0x04c711ac,0x047411e6,0x041f121c,0x03c7124e,0x036d127b,0x031112a4,0x02b312c8,0x025412e8,0x01f31303,0x01901319,0x012d132a,0x00c91336,0x0065133e,0x00001340,0x00000314,0x00660316,0x00cd031e,0x0132032a,0x0197033c,0x01fb0352,0x025e036e,0x02c0038e,0x031f03b3,0x037d03dc,0x03d8040a,0x0432043d,0x04880474,0x04dc04af,0x052d04ee,0x057a0531,0x05c50577,0x060b05c2,0x064e060f,0x068d0660,0x06c806b4,0x06ff070a,0x07320764,0x076007bf,0x0789081d,0x07ae087c,0x07ce08de,0x07ea0941,0x080009a5,0x08120a0a,0x081e0a6f,0x08260ad6,0x08280b3c,0x08260ba2,0x081e0c09,0x08120c6e,0x08000cd3,0x07ea0d37,0x07ce0d9a,0x07ae0dfc,0x07890e5b,0x07600eb9,0x07320f14,0x06ff0f6e,0x06c80fc4,0x068d1018,0x064e1069,0x060b10b6,0x05c51101,0x057a1147,0x052d118a,0x04dc11c9,0x04881204,0x0432123b,0x03d8126e,0x037d129c,0x031f12c5,0x02c012ea,0x025e130a,0x01fb1326,0x0197133c,0x0132134e,0x00cd135a,0x00661362,0x00001364,0x000002f0,0x006802f2,0x00d002fa,0x01380307,0x019e0319,0x0204032f,0x0269034b,0x02cc036c,0x032d0391,0x038c03bc,0x03e903eb,0x0444041e,0x049c0456,0x04f10492,0x054404d2,0x05930516,0x05de055e,0x062605a9,0x066a05f8,0x06aa064b,0x06e606a0,0x071e06f8,0x07510753,0x078007b0,0x07ab080f,0x07d00870,0x07f108d3,0x080d0938,0x0823099e,0x08350a04,0x08420a6c,0x084a0ad4,0x084c0b3c,0x084a0ba4,0x08420c0c,0x08350c74,0x08230cda,0x080d0d40,0x07f10da5,0x07d00e08,0x07ab0e69,0x07800ec8,0x07510f25,0x071e0f80,0x06e60fd8,0x06aa102d,0x066a1080,0x062610cf,0x05de111a,0x05931162,0x054411a6,0x04f111e6,0x049c1222,0x0444125a,0x03e9128d,0x038c12bc,0x032d12e7,0x02cc130c,0x0269132d,0x02041349,0x019e135f,0x01381371,0x00d0137e,0x00681386,0x00001388,0x000002cc,0x006a02ce,0x00d402d6,0x013d02e3,0x01a502f5,0x020d030c,0x02730329,0x02d8034a,0x033b0370,0x039c039b,0x03fa03cb,0x045703ff,0x04b00438,0x05070475,0x055a04b6,0x05ab04fb,0x05f80544,0x06410591,0x068605e2,0x06c70635,0x0704068c,0x073d06e5,0x07710742,0x07a107a0,0x07cc0801,0x07f20864,0x081308c9,0x0830092f,0x08470997,0x085909ff,0x08660a68,0x086e0ad2,0x08700b3c,0x086e0ba6,0x08660c10,0x08590c79,0x08470ce1,0x08300d49,0x08130daf,0x07f20e14,0x07cc0e77,0x07a10ed8,0x07710f36,0x073d0f93,0x07040fec,0x06c71043,0x06861096,0x064110e7,0x05f81134,0x05ab117d,0x055a11c2,0x05071203,0x04b01240,0x04571279,0x03fa12ad,0x039c12dd,0x033b1308,0x02d8132e,0x0273134f,0x020d136c,0x01a51383,0x013d1395,0x00d413a2,0x006a13aa,0x000013ac,0x000002a8,0x006c02aa,0x00d702b2,0x014202c0,0x01ac02d2,0x021602ea,0x027e0306,0x02e40328,0x0348034f,0x03ab037b,0x040b03ab,0x046903e0,0x04c4041a,0x051c0458,0x0571049a,0x05c304e1,0x0611052b,0x065b0579,0x06a205cb,0x06e40620,0x07220678,0x075c06d3,0x07910731,0x07c10791,0x07ed07f4,0x08140858,0x083608be,0x08520926,0x086a0990,0x087c09fa,0x088a0a65,0x08920ad0,0x08940b3c,0x08920ba8,0x088a0c13,0x087c0c7e,0x086a0ce8,0x08520d52,0x08360dba,0x08140e20,0x07ed0e84,0x07c10ee7,0x07910f47,
        0x075c0fa5,0x07221000,0x06e41058,0x06a210ad,0x065b10ff,0x0611114d,0x05c31197,0x057111de,0x051c1220,0x04c4125e,0x04691298,0x040b12cd,0x03ab12fd,0x03481329,0x02e41350,0x027e1372,0x0216138e,0x01ac13a6,0x014213b8,0x00d713c6,0x006c13ce,0x000013d0,0x00000284,0x006e0286,0x00db028f,0x0148029c,0x01b302af,0x021e02c7,0x028802e4,0x02f00306,0x0356032e,0x03ba035a,0x041c038b,0x047c03c1,0x04d803fc,0x0532043b,0x0588047e,0x05db04c6,0x062a0512,0x06760561,0x06be05b4,0x0701060a,0x07400664,0x077b06c0,0x07b10720,0x07e20782,0x080e07e6,0x0836084c,0x085808b4,0x0875091e,0x088d0989,0x08a009f4,0x08ad0a61,0x08b60ace,0x08b80b3c,0x08b60baa,0x08ad0c17,0x08a00c84,0x088d0cef,0x08750d5a,0x08580dc4,0x08360e2c,0x080e0e92,0x07e20ef6,0x07b10f58,0x077b0fb8,0x07401014,0x0701106e,0x06be10c4,0x06761117,0x062a1166,0x05db11b2,0x058811fa,0x0532123d,0x04d8127c,0x047c12b7,0x041c12ed,0x03ba131e,0x0356134a,0x02f01372,0x02881394,0x021e13b1,0x01b313c9,0x014813dc,0x00db13e9,0x006e13f2,0x000013f4,0x00000260,0x006f0263,0x00de026b,0x014d0278,0x01bb028b,0x022702a4,0x029202c1,0x02fc02e4,0x0364030c,0x03ca033a,0x042d036c,0x048e03a2,0x04ec03de,0x0547041e,0x059f0463,0x05f304ab,0x064404f8,0x06910549,0x06d9059d,0x071e05f5,0x075e0650,0x079a06ae,0x07d0070f,0x08020772,0x083007d8,0x08580840,0x087b08aa,0x08980915,0x08b10981,0x08c409ef,0x08d10a5e,0x08d90acd,0x08dc0b3c,0x08d90bab,0x08d10c1a,0x08c40c89,0x08b10cf7,0x08980d63,0x087b0dce,0x08580e38,0x08300ea0,0x08020f06,0x07d00f69,0x079a0fca,0x075e1028,0x071e1083,0x06d910db,0x0691112f,0x06441180,0x05f311cd,0x059f1215,0x0547125a,0x04ec129a,0x048e12d6,0x042d130c,0x03ca133e,0x0364136c,0x02fc1394,0x029213b7,0x022713d4,0x01bb13ed,0x014d1400,0x00de140d,0x006f1415,0x00001418,0x0000023c,0x0071023f,0x00e20247,0x01520255,0x01c20268,0x02300281,0x029d029f,0x030802c2,0x037202eb,0x03d90319,0x043e034c,0x04a10384,0x050003c0,0x055d0401,0x05b60447,0x060b0491,0x065d04df,0x06ab0531,0x06f50586,0x073b05df,0x077c063c,0x07b8069b,0x07f006fe,0x08230763,0x085107ca,0x087a0834,0x089d089f,0x08bb090c,0x08d4097a,0x08e709ea,0x08f50a5a,0x08fd0acb,0x09000b3c,0x08fd0bad,0x08f50c1e,0x08e70c8e,0x08d40cfe,0x08bb0d6c,0x089d0dd9,0x087a0e44,0x08510eae,0x08230f15,0x07f00f7a,0x07b80fdd,0x077c103c,0x073b1099,0x06f510f2,0x06ab1147,0x065d1199,0x060b11e7,0x05b61231,0x055d1277,0x050012b8,0x04a112f4,0x043e132c,0x03d9135f,0x0372138d,0x030813b6,0x029d13d9,0x023013f7,0x01c21410,0x01521423,0x00e21431,0x00711439,0x0000143c,0x00000218,0x0073021b,0x00e50223,0x01570231,0x01c90245,0x0239025e,0x02a7027d,0x031402a1,0x038002ca,0x03e902f8,0x044f032c,0x04b30365,0x051403a2,0x057203e4,0x05cd042b,0x06240476,0x067704c5,0x06c60518,0x0711056f,0x075805ca,0x079a0628,0x07d70689,0x081006ed,0x08440753,0x087207bc,0x089b0828,0x08bf0895,0x08de0903,0x08f70973,0x090b09e5,0x09190a57,0x09210ac9,0x09240b3c,0x09210baf,0x09190c21,0x090b0c93,0x08f70d05,0x08de0d75,0x08bf0de3,0x089b0e50,0x08720ebc,0x08440f25,0x08100f8b,0x07d70fef,0x079a1050,0x075810ae,0x07111109,0x06c61160,0x067711b3,0x06241202,0x05cd124d,0x05721294,0x051412d6,0x04b31313,0x044f134c,0x03e91380,0x038013ae,0x031413d7,0x02a713fb,0x0239141a,0x01c91433,0x01571447,0x00e51455,0x0073145d,0x00001460,0x000001f4,0x007501f7,0x00e901ff,0x015d020e,0x01d00221,0x0241023b,0x02b2025a,0x0321027f,0x038d02a9,0x03f802d8,0x0460030c,0x04c60346,0x05280384,0x058703c7,0x05e3040f,0x063c045b,0x069004ac,0x06e10500,0x072d0559,0x077505b5,0x07b80614,0x07f60676,0x083006dc,0x08640744,0x089307af,0x08bd081b,0x08e2088a,0x090108fb,0x091b096c,0x092e09df,0x093d0a53,0x09450ac7,0x09480b3c,0x09450bb1,0x093d0c25,0x092e0c99,0x091b0d0c,0x09010d7d,0x08e20dee,0x08bd0e5d,0x08930ec9,0x08640f34,0x08300f9c,0x07f61002,0x07b81064,0x077510c3,0x072d111f,0x06e11178,0x069011cc,0x063c121d,0x05e31269,0x058712b1,0x052812f4,0x04c61332,0x0460136c,0x03f813a0,0x038d13cf,0x032113f9,0x02b2141e,0x0241143d,0x01d01457,0x015d146a,0x00e91479,0x00751481,0x00001484,0x000001d0,0x007601d3,0x00ec01db,0x016201ea,0x01d701fe,0x024a0218,0x02bc0238,0x032d025d,0x039b0287,0x040702b7,0x047102ed,0x04d80327,0x053c0366,0x059d03ab,0x05fa03f3,0x06540441,0x06aa0492,0x06fb04e8,0x07490542,0x0791059f,0x07d60600,0x08150664,0x084f06cb,0x08850735,
        0x08b507a1,0x08df080f,0x09040880,0x092408f2,0x093e0965,0x095209da,0x09610a50,0x09690ac6,0x096c0b3c,0x09690bb2,0x09610c28,0x09520c9e,0x093e0d13,0x09240d86,0x09040df8,0x08df0e69,0x08b50ed7,0x08850f43,0x084f0fad,0x08151014,0x07d61078,0x079110d9,0x07491136,0x06fb1190,0x06aa11e6,0x06541237,0x05fa1285,0x059d12cd,0x053c1312,0x04d81351,0x0471138b,0x040713c1,0x039b13f1,0x032d141b,0x02bc1440,0x024a1460,0x01d7147a,0x0162148e,0x00ec149d,0x007614a5,0x000014a8,0x000001ac,0x007801af,0x00f001b8,0x016701c6,0x01de01db,0x025301f5,0x02c70215,0x0339023b,0x03a90266,0x04170297,0x048202cd,0x04eb0308,0x05500348,0x05b2038e,0x061103d8,0x066c0426,0x06c30479,0x071604d0,0x0764052b,0x07ae058a,0x07f405ec,0x08340651,0x086f06ba,0x08a50725,0x08d60793,0x09010803,0x09270875,0x094708e9,0x0961095e,0x097609d5,0x09840a4c,0x098d0ac4,0x09900b3c,0x098d0bb4,0x09840c2c,0x09760ca3,0x09610d1a,0x09470d8f,0x09270e03,0x09010e75,0x08d60ee5,0x08a50f53,0x086f0fbe,0x08341027,0x07f4108c,0x07ae10ee,0x0764114d,0x071611a8,0x06c311ff,0x066c1252,0x061112a0,0x05b212ea,0x05501330,0x04eb1370,0x048213ab,0x041713e1,0x03a91412,0x0339143d,0x02c71463,0x02531483,0x01de149d,0x016714b2,0x00f014c0,0x007814c9,0x000014cc,0x00000188,0x007a018b,0x00f30194,0x016c01a3,0x01e501b8,0x025c01d2,0x02d101f3,0x03450219,0x03b70245,0x04260276,0x049302ad,0x04fd02e9,0x0564032b,0x05c80371,0x062803bc,0x0684040b,0x06dd045f,0x073104b8,0x07800514,0x07cb0574,0x081105d8,0x0853063f,0x088f06a9,0x08c60716,0x08f70785,0x092307f7,0x0949086b,0x096a08e0,0x09840957,0x099909d0,0x09a80a49,0x09b10ac2,0x09b40b3c,0x09b10bb6,0x09a80c2f,0x09990ca8,0x09840d21,0x096a0d98,0x09490e0d,0x09230e81,0x08f70ef3,0x08c60f62,0x088f0fcf,0x08531039,0x081110a0,0x07cb1104,0x07801164,0x073111c0,0x06dd1219,0x0684126d,0x062812bc,0x05c81307,0x0564134d,0x04fd138f,0x049313cb,0x04261402,0x03b71433,0x0345145f,0x02d11485,0x025c14a6,0x01e514c0,0x016c14d5,0x00f314e4,0x007a14ed,0x000014f0,0x00000164,0x007c0167,0x00f70170,0x0172017f,0x01ec0194,0x026401af,0x02dc01d0,0x035101f7,0x03c40224,0x04350256,0x04a4028d,0x051002ca,0x0578030d,0x05dd0354,0x063f03a0,0x069c03f1,0x06f60446,0x074b04a0,0x079c04fd,0x07e8055f,0x082f05c4,0x0872062c,0x08af0698,0x08e60707,0x09180778,0x094507eb,0x096c0860,0x098d08d8,0x09a80950,0x09bd09ca,0x09cc0a45,0x09d50ac0,0x09d80b3c,0x09d50bb8,0x09cc0c33,0x09bd0cae,0x09a80d28,0x098d0da0,0x096c0e18,0x09450e8d,0x09180f00,0x08e60f71,0x08af0fe0,0x0872104c,0x082f10b4,0x07e81119,0x079c117b,0x074b11d8,0x06f61232,0x069c1287,0x063f12d8,0x05dd1324,0x0578136b,0x051013ae,0x04a413eb,0x04351422,0x03c41454,0x03511481,0x02dc14a8,0x026414c9,0x01ec14e4,0x017214f9,0x00f71508,0x007c1511,0x00001514,0x00000140,0x007d0143,0x00fb014c,0x0177015c,0x01f30171,0x026d018c,0x02e601ae,0x035d01d5,0x03d20202,0x04450235,0x04b5026e,0x052202ac,0x058c02ef,0x05f30337,0x06560384,0x06b503d6,0x070f042d,0x07660487,0x07b804e6,0x08050549,0x084d05b0,0x0890061a,0x08ce0687,0x090706f7,0x093a076a,0x096707df,0x098e0856,0x09b008cf,0x09cb0949,0x09e009c5,0x09f00a41,0x09f90abf,0x09fc0b3c,0x09f90bb9,0x09f00c37,0x09e00cb3,0x09cb0d2f,0x09b00da9,0x098e0e22,0x09670e99,0x093a0f0e,0x09070f81,0x08ce0ff1,0x0890105e,0x084d10c8,0x0805112f,0x07b81192,0x076611f1,0x070f124b,0x06b512a2,0x065612f4,0x05f31341,0x058c1389,0x052213cc,0x04b5140a,0x04451443,0x03d21476,0x035d14a3,0x02e614ca,0x026d14ec,0x01f31507,0x0177151c,0x00fb152c,0x007d1535,0x00001538,0x0000011c,0x007f011f,0x00fe0128,0x017c0138,0x01fa014e,0x0276016a,0x02f0018c,0x036901b3,0x03e001e1,0x04540215,0x04c6024e,0x0535028d,0x05a002d1,0x0608031a,0x066c0368,0x06cd03bb,0x07290413,0x0781046f,0x07d404d0,0x08220534,0x086b059c,0x08af0607,0x08ee0676,0x092706e8,0x095b075c,0x098907d3,0x09b0084c,0x09d208c6,0x09ee0942,0x0a0409c0,0x0a140a3e,0x0a1d0abd,0x0a200b3c,0x0a1d0bbb,0x0a140c3a,0x0a040cb8,0x09ee0d36,0x09d20db2,0x09b00e2c,0x09890ea5,0x095b0f1c,0x09270f90,0x08ee1002,0x08af1071,0x086b10dc,0x08221144,0x07d411a8,0x07811209,0x07291265,0x06cd12bd,0x066c1310,0x0608135e,0x05a013a7,0x053513eb,0x04c6142a,0x04541463,0x03e01497,0x036914c5,0x02f014ec,0x0276150e,0x01fa152a,0x017c1540,0x00fe1550,0x007f1559,0x0000155c,0x000000f8,0x008100fb,0x01020105,0x01820114,0x0201012a,
        0x027f0147,0x02fb0169,0x03750192,0x03ee01c0,0x046401f4,0x04d7022e,0x0547026e,0x05b402b3,0x061e02fd,0x0683034c,0x06e503a1,0x074203fa,0x079b0457,0x07f004b9,0x083f051e,0x08890588,0x08ce05f5,0x090e0665,0x094806d8,0x097c074e,0x09aa07c7,0x09d30841,0x09f508bd,0x0a12093b,0x0a2809ba,0x0a370a3a,0x0a410abb,0x0a440b3c,0x0a410bbd,0x0a370c3e,0x0a280cbe,0x0a120d3d,0x09f50dbb,0x09d30e37,0x09aa0eb1,0x097c0f2a,0x09480fa0,0x090e1013,0x08ce1083,0x088910f0,0x083f115a,0x07f011bf,0x079b1221,0x0742127e,0x06e512d7,0x0683132c,0x061e137b,0x05b413c5,0x0547140a,0x04d7144a,0x04641484,0x03ee14b8,0x037514e6,0x02fb150f,0x027f1531,0x0201154e,0x01821564,0x01021573,0x0081157d,0x00001580,0x000000d4,0x008300d7,0x010500e1,0x018700f1,0x02080107,0x02870124,0x03050147,0x03810170,0x03fb019f,0x047301d4,0x04e8020e,0x055a024f,0x05c80295,0x063302e0,0x069a0331,0x06fd0386,0x075c03e0,0x07b6043f,0x080b04a2,0x085c0509,0x08a70574,0x08ed05e2,0x092e0654,0x096806c9,0x099d0741,0x09cc07bb,0x09f50837,0x0a1808b5,0x0a350934,0x0a4b09b5,0x0a5b0a37,0x0a650ab9,0x0a680b3c,0x0a650bbf,0x0a5b0c41,0x0a4b0cc3,0x0a350d44,0x0a180dc3,0x09f50e41,0x09cc0ebd,0x099d0f37,0x09680faf,0x092e1024,0x08ed1096,0x08a71104,0x085c116f,0x080b11d6,0x07b61239,0x075c1298,0x06fd12f2,0x069a1347,0x06331398,0x05c813e3,0x055a1429,0x04e8146a,0x047314a4,0x03fb14d9,0x03811508,0x03051531,0x02871554,0x02081571,0x01871587,0x01051597,0x008315a1,0x000015a4,0x000000b0,0x008400b3,0x010900bd,0x018c00cd,0x020f00e4,0x02900101,0x03100124,0x038e014e,0x0409017d,0x048201b3,0x04f901ef,0x056c0230,0x05dc0277,0x064802c3,0x06b10315,0x0715036b,0x077503c7,0x07d10427,0x0827048b,0x087904f4,0x08c50560,0x090c05d0,0x094d0643,0x098906ba,0x09bf0733,0x09ee07ae,0x0a18082c,0x0a3b08ac,0x0a58092d,0x0a6f09b0,0x0a7f0a33,0x0a890ab8,0x0a8c0b3c,0x0a890bc0,0x0a7f0c45,0x0a6f0cc8,0x0a580d4b,0x0a3b0dcc,0x0a180e4c,0x09ee0eca,0x09bf0f45,0x09890fbe,0x094d1035,0x090c10a8,0x08c51118,0x08791184,0x082711ed,0x07d11251,0x077512b1,0x0715130d,0x06b11363,0x064813b5,0x05dc1401,0x056c1448,0x04f91489,0x048214c5,0x040914fb,0x038e152a,0x03101554,0x02901577,0x020f1594,0x018c15ab,0x010915bb,0x008415c5,0x000015c8,0x0000008c,0x0086008f,0x010c0099,0x019100aa,0x021600c1,0x029900de,0x031a0102,0x039a012c,0x0417015c,0x04920193,0x050a01cf,0x057f0211,0x05f00259,0x065e02a6,0x06c802f9,0x072d0351,0x078f03ad,0x07eb040f,0x08430474,0x089604de,0x08e3054c,0x092b05bd,0x096d0632,0x09a906aa,0x09e00725,0x0a1007a2,0x0a3a0822,0x0a5e08a3,0x0a7b0926,0x0a9209ab,0x0aa30a30,0x0aad0ab6,0x0ab00b3c,0x0aad0bc2,0x0aa30c48,0x0a920ccd,0x0a7b0d52,0x0a5e0dd5,0x0a3a0e56,0x0a100ed6,0x09e00f53,0x09a90fce,0x096d1046,0x092b10bb,0x08e3112c,0x0896119a,0x08431204,0x07eb1269,0x078f12cb,0x072d1327,0x06c8137f,0x065e13d2,0x05f0141f,0x057f1467,0x050a14a9,0x049214e5,0x0417151c,0x039a154c,0x031a1576,0x0299159a,0x021615b7,0x019115ce,0x010c15df,0x008615e9,0x000015ec,0x00000068,0x0088006b,0x01100075,0x01970086,0x021d009d,0x02a200bb,0x032500df,0x03a6010a,0x0425013b,0x04a10172,0x051b01af,0x059101f2,0x0604023b,0x06730289,0x06df02dd,0x07460336,0x07a80394,0x080603f6,0x085f045d,0x08b304c9,0x09010538,0x094a05ab,0x098d0621,0x09ca069b,0x0a010717,0x0a320796,0x0a5d0817,0x0a81089a,0x0a9f091f,0x0ab609a5,0x0ac70a2c,0x0ad10ab4,0x0ad40b3c,0x0ad10bc4,0x0ac70c4c,0x0ab60cd3,0x0a9f0d59,0x0a810dde,0x0a5d0e61,0x0a320ee2,0x0a010f61,0x09ca0fdd,0x098d1057,0x094a10cd,0x09011140,0x08b311af,0x085f121b,0x08061282,0x07a812e4,0x07461342,0x06df139b,0x067313ef,0x0604143d,0x05911486,0x051b14c9,0x04a11506,0x0425153d,0x03a6156e,0x03251599,0x02a215bd,0x021d15db,0x019715f2,0x01101603,0x0088160d,0x00001610,0x00000044,0x008a0047,0x01130051,0x019c0062,0x0224007a,0x02aa0098,0x032f00bd,0x03b200e8,0x0433011a,0x04b10152,0x052c0190,0x05a401d3,0x0618021d,0x0689026d,0x06f502c1,0x075e031b,0x07c2037a,0x082103de,0x087b0447,0x08cf04b3,0x091f0524,0x09690598,0x09ac0610,0x09ea068b,0x0a220709,0x0a54078a,0x0a7f080d,0x0aa40892,0x0ac20918,0x0ada09a0,0x0aeb0a29,0x0af50ab2,0x0af80b3c,0x0af50bc6,0x0aeb0c4f,0x0ada0cd8,0x0ac20d60,0x0aa40de6,0x0a7f0e6b,0x0a540eee,0x0a220f6f,0x09ea0fed,0x09ac1068,0x096910e0,0x091f1154,0x08cf11c5,0x087b1231,0x0821129a,0x07c212fe,0x075e135d,0x06f513b7,
        0x0689140b,0x0618145b,0x05a414a5,0x052c14e8,0x04b11526,0x0433155e,0x03b21590,0x032f15bb,0x02aa15e0,0x022415fe,0x019c1616,0x01131627,0x008a1631,0x00001634,0x00000020,0x008c0023,0x0117002e,0x01a1003f,0x022b0057,0x02b30075,0x033a009a,0x03be00c6,0x044000f8,0x04c00131,0x053d0170,0x05b601b5,0x062c01ff,0x069e0250,0x070c02a6,0x07760301,0x07db0361,0x083b03c6,0x08960430,0x08ec049e,0x093d0510,0x09870586,0x09cc05ff,0x0a0b067c,0x0a4406fc,0x0a76077e,0x0aa20802,0x0ac70889,0x0ae50911,0x0afd099b,0x0b0e0a25,0x0b190ab0,0x0b1c0b3c,0x0b190bc8,0x0b0e0c53,0x0afd0cdd,0x0ae50d67,0x0ac70def,0x0aa20e76,0x0a760efa,0x0a440f7c,0x0a0b0ffc,0x09cc1079,0x098710f2,0x093d1168,0x08ec11da,0x08961248,0x083b12b2,0x07db1317,0x07761377,0x070c13d2,0x069e1428,0x062c1479,0x05b614c3,0x053d1508,0x04c01547,0x04401580,0x03be15b2,0x033a15de,0x02b31603,0x022b1621,0x01a11639,0x0117164a,0x008c1655,0x00001658,0x00000000,0x008d0000,0x011a000a,0x01a7001b,0x02320033,0x02bc0052,0x03440078,0x03ca00a4,0x044e00d7,0x04cf0111,0x054e0150,0x05c90196,0x064001e1,0x06b40233,0x0723028a,0x078e02e6,0x07f40348,0x085603ae,0x08b20419,0x09090488,0x095b04fc,0x09a60573,0x09ec05ee,0x0a2b066d,0x0a6506ee,0x0a980772,0x0ac407f8,0x0aea0880,0x0b09090a,0x0b210995,0x0b320a22,0x0b3d0aaf,0x0b400b3c,0x0b3d0bc9,0x0b320c56,0x0b210ce3,0x0b090d6e,0x0aea0df8,0x0ac40e80,0x0a980f06,0x0a650f8a,0x0a2b100b,0x09ec108a,0x09a61105,0x095b117c,0x090911f0,0x08b2125f,0x085612ca,0x07f41330,0x078e1392,0x072313ee,0x06b41445,0x06401497,0x05c914e2,0x054e1528,0x04cf1567,0x044e15a1,0x03ca15d4,0x03441600,0x02bc1626,0x02321645,0x01a7165d,0x011a166e,0x008d1679,0x0000167c
        },
    };

    // dce_stripe
    static UINT32 m_stripeTbl[2][16] = {
        {
            128, 128, 128, 192, 192,
            320, 320, 320, 192, 192,
            192, 192, 128, 128, 128,
            128
        },
        {
            128, 128, 128, 192, 192,
            320, 320, 320, 192, 192,
            192, 192, 128, 128, 128,
            128
        }
    };
    StripeInfo[0].stripeTbl = &m_stripeTbl[0][0];
    StripeInfo[1].stripeTbl = &m_stripeTbl[1][0];
    // RegAddr: 0x98
    StripeInfo[0].IncNum = 16;
    StripeInfo[0].DecNum = 16;
    StripeInfo[0].StrPos = 0;
    StripeInfo[0].Out_STP_Overlap = 0;

    StripeInfo[1].IncNum = 16;
    StripeInfo[1].DecNum = 16;
    StripeInfo[1].StrPos = 0;
    StripeInfo[1].Out_STP_Overlap = 0;
    // Ime func, need check
    StripeInfo[0].ImeSsrEn = FALSE;
    StripeInfo[0].ImeIsdEn = FALSE;

    StripeInfo[1].ImeSsrEn = FALSE;
    StripeInfo[1].ImeIsdEn = FALSE;

    // 2d lut    // RegAddr: 0x64, 0x68, 0x6c, 0x70
    Dce2DInfo[0]._2DLutNum = 0;
    Dce2DInfo[0]._2DLutScaleFact.HScaleFact = 2914;
    Dce2DInfo[0]._2DLutScaleFact.VScaleFact = 5833;
    Dce2DInfo[0]._2DLutXYOfs.XOfsFrac = 0;
    Dce2DInfo[0]._2DLutXYOfs.XOfsInt = 0;
    Dce2DInfo[0]._2DLutXYOfs.YOfsFrac = 0;
    Dce2DInfo[0]._2DLutXYOfs.YOfsInt = 0;

    Dce2DInfo[1]._2DLutNum = 0;
    Dce2DInfo[1]._2DLutScaleFact.HScaleFact = 2914;
    Dce2DInfo[1]._2DLutScaleFact.VScaleFact = 5833;
    Dce2DInfo[1]._2DLutXYOfs.XOfsFrac = 0;
    Dce2DInfo[1]._2DLutXYOfs.XOfsInt = 0;
    Dce2DInfo[1]._2DLutXYOfs.YOfsFrac = 0;
    Dce2DInfo[1]._2DLutXYOfs.YOfsInt = 0;

    Dce2DInfo[0]._2DLutAddr = (UINT32)&lut[0];
    Dce2DInfo[1]._2DLutAddr = (UINT32)&lut[1];

}
}

void IPL_CtrlInitIpc_IPESubOutPPB(UINT32 Id, UINT32 *PP0Addr, UINT32 *PP1Addr, IPL_YCC_CH_INFO *IpeOut)
{
    UINT32 BufSize;

    if (PP0Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IPEOUT_1, PP0Addr, &BufSize);
    }

    if (PP1Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IPEOUT_2, PP1Addr, &BufSize);
    }

    IpeOut->Width = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_SIZE_H);
    IpeOut->Height = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_SIZE_V);
    IpeOut->LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IPE_OUT_SIZE_YLOS);
}

void IPL_CtrlInitIpc_IFE2LcaPPB(UINT32 Id, UINT32 *PP0Addr, UINT32 *PP1Addr)
{
    UINT32 BufSize;

    if (PP0Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IFE2LCAOUT_1, PP0Addr, &BufSize);
    }

    if (PP1Addr != NULL)
    {
        IPL_BufGet(Id, IPLBUF_IFE2LCAOUT_2, PP1Addr, &BufSize);
    }
}

void IPL_CtrlInitIpc_IMEPxOutPPB(UINT32 Id, UINT32 PathEnIdx, IPL_YCC_IMG_INFO *Px_PPB0)
{
    UINT32 BufSize, Px;
    IPL_IMG_BUF_INFO BufInfo = {0};
    IPL_YCC_IMG_INFO YCCInfo = {0};
    ER rt = E_OK;

    UINT32 PxIdx[4][7] =
    {
        {IPLCTRL_IME_P1_FMT, IPLCTRL_IME_P1_BUF_SIZE_H, IPLCTRL_IME_P1_BUF_SIZE_V, IPLCTRL_IME_P1_OUT_SIZE_H, IPLCTRL_IME_P1_OUT_SIZE_V, IPLCTRL_IME_P1_OUT_SIZE_HLOS, IPLBUF_IMEP1_1},
        {IPLCTRL_IME_P2_FMT, IPLCTRL_IME_P2_BUF_SIZE_H, IPLCTRL_IME_P2_BUF_SIZE_V, IPLCTRL_IME_P2_OUT_SIZE_H, IPLCTRL_IME_P2_OUT_SIZE_V, IPLCTRL_IME_P2_OUT_SIZE_HLOS, IPLBUF_IMEP2_1},
        {IPLCTRL_IME_P3_FMT, IPLCTRL_IME_P3_BUF_SIZE_H, IPLCTRL_IME_P3_BUF_SIZE_V, IPLCTRL_IME_P3_OUT_SIZE_H, IPLCTRL_IME_P3_OUT_SIZE_V, IPLCTRL_IME_P3_OUT_SIZE_HLOS, IPLBUF_IMEP3_1},
        {IPLCTRL_IME_P4_FMT, IPLCTRL_IME_P4_BUF_SIZE_H, IPLCTRL_IME_P4_BUF_SIZE_V, IPLCTRL_IME_P4_OUT_SIZE_H, IPLCTRL_IME_P4_OUT_SIZE_V, IPLCTRL_IME_P4_OUT_SIZE_HLOS, IPLBUF_IMEP4_1},
    };

    switch (PathEnIdx)
    {
        case IPLCTRL_IME_P1_EN:
            Px = 0;
            break;

        case IPLCTRL_IME_P2_EN:
            Px = 1;
            break;

        case IPLCTRL_IME_P3_EN:
            Px = 2;
            break;

        case IPLCTRL_IME_P4_EN:
            Px = 3;
            break;

        default:
            DBG_ERR("Id: %d, PathEnIdx error %d\r\n", Id, PathEnIdx);
            return;
    }

	//#NT#2017/02/06#Jarkko Chang -begin
	//#NT# add support side by side left/right, up/down case
    //calculate address
    YCCInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][0]);
    YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][1]);
    YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][2]);
    YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, PxIdx[Px][1]);

	if(IPL_CtrlGetInfor(Id, IPLCTRL_SBS_TYPE) == SEL_SBS_LR)
	{
        YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (IPL_CtrlGetInfor(Id, PxIdx[Px][1]) << 1);
    }
	else if (IPL_CtrlGetInfor(Id, IPLCTRL_SBS_TYPE) == SEL_SBS_UD)
	{
		if (Id == IPL_ID_1)
	        YCCInfo.Ch[IPL_IMG_YUV_Y].Height = (IPL_CtrlGetInfor(Id, PxIdx[Px][2]) << 1);
		else
		{
	        YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][2]);
		    YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
	        YCCInfo.Ch[IPL_IMG_YUV_Y].Height += YCCInfo.Ch[IPL_IMG_YUV_U].Height;
		}
	}

    YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
    YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
    rt = IPL_BufGet(Id, PxIdx[Px][6], &BufInfo.Buf[0], &BufSize);
    if (rt != E_OK)
        return;

    BufInfo.Buf[1] = 0;
    BufInfo.Buf[2] = 0;
    IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, BufInfo, &YCCInfo);

	if(IPL_CtrlGetInfor(Id, IPLCTRL_SBS_TYPE) == SEL_SBS_LR)
	{
	    YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (IPL_CtrlGetInfor(Id, PxIdx[Px][1]) << 1);
	}
	else
	{
	    YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, PxIdx[Px][1]);
	}
	//#NT#2017/02/06#Jarkko Chang -end

    //calculate ycc information
    YCCInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][0]);
    YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][3]);
    YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][4]);
    YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, PxIdx[Px][5]);
    YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
    YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];

    *Px_PPB0 = YCCInfo;
}

    /* IME Output Image Set As below
    -----------------------------
    |           L_Sprt          |
    -----------------------------
    |             R             |
    -----------------------------
    |           |   |           |
    |           |   |           |
    |           |   |           |
    |      L    | S |   R_Sprt  |
    |           |   |           |
    |           |   |           |
    |           |   |           |
    -----------------------------
    */
void IPL_CtrlInitIpc_IMEPxOutPPB_Stitch(UINT32 Id, UINT32 PathEnIdx, IPL_YCC_IMG_INFO *Px_PPB0, STITCH_IMG *StitchImg)
{
    UINT32 BufSize, Px;
    IPL_IMG_BUF_INFO BufInfo = {0};
    IPL_YCC_IMG_INFO YCCInfo = {0};
    UINT32 YSprtAddr, CSprtAddr;
    UINT32 SprtR, SprtL, SeamH;    //Get from Stitch lib

    UINT32 PxIdx[3][10] =
    {
        {IPLCTRL_IME_P1_FMT, IPLCTRL_IME_P1_OUT_CROP_H, IPLCTRL_IME_P1_OUT_CROP_V, IPLCTRL_IME_P1_OUT_SIZE_HLOS, IPLBUF_IMEP1_1, IPLCTRL_IME_P1_SPRT_YBUFOFS, IPLCTRL_IME_P1_SPRT_CBUFOFS, IPLBUF_IMEP1_SPRT, IPLCTRL_IME_P1_OUT_SIZE_H, IPLCTRL_IME_P1_OUT_SIZE_V},
        {IPLCTRL_IME_P2_FMT, IPLCTRL_IME_P2_OUT_CROP_H, IPLCTRL_IME_P2_OUT_CROP_V, IPLCTRL_IME_P2_OUT_SIZE_HLOS, IPLBUF_IMEP2_1, IPLCTRL_IME_P2_SPRT_YBUFOFS, IPLCTRL_IME_P2_SPRT_CBUFOFS, IPLBUF_IMEP2_SPRT, IPLCTRL_IME_P2_OUT_SIZE_H, IPLCTRL_IME_P2_OUT_SIZE_V},
        {IPLCTRL_IME_P3_FMT, IPLCTRL_IME_P3_OUT_CROP_H, IPLCTRL_IME_P3_OUT_CROP_V, IPLCTRL_IME_P3_OUT_SIZE_HLOS, IPLBUF_IMEP3_1, IPLCTRL_IME_P3_SPRT_YBUFOFS, IPLCTRL_IME_P3_SPRT_CBUFOFS, IPLBUF_IMEP3_SPRT, IPLCTRL_IME_P3_OUT_SIZE_H, IPLCTRL_IME_P3_OUT_SIZE_V},
    };

    switch (PathEnIdx)
    {
        case IPLCTRL_IME_P1_EN:
            Px = 0;
            break;

        case IPLCTRL_IME_P2_EN:
            Px = 1;
            break;

        case IPLCTRL_IME_P3_EN:
            Px = 2;
            break;

        default:
            DBG_ERR("Id: %d, PathEnIdx error %d\r\n", Id, PathEnIdx);
            return;
    }

    SeamH = IPL_ALIGN_ROUNDUP(Stitch_GetStitchAreaWidthInPixel(IPL_CtrlGetInfor(Id, PxIdx[Px][1])),4);
    SprtL = IPL_ALIGN_ROUNDUP(Stitch_GetLeftImgCutPointInPixel(IPL_CtrlGetInfor(Id, PxIdx[Px][1])),4);
    SprtR = IPL_ALIGN_ROUNDUP(Stitch_GetRightImgCutPointInPixel(IPL_CtrlGetInfor(Id, PxIdx[Px][1])),4);

    if (Id == IPL_ID_1)
    {
        //calculate Separate img information
        YCCInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][0]);
        YCCInfo.Ch[IPL_IMG_YUV_Y].Width = SprtL;
        YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][2]);
        YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = YCCInfo.Ch[IPL_IMG_YUV_Y].Width;
        YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
        YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
        IPL_BufGet(Id, PxIdx[Px][7], &BufInfo.Buf[0], &BufSize);
        BufInfo.Buf[1] = 0;
        BufInfo.Buf[2] = 0;
        IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, BufInfo, &YCCInfo);

        StitchImg->StPos = IPL_CtrlGetInfor(Id, PxIdx[Px][1]) - SprtL;
        StitchImg->LineOfsY = YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs;
        StitchImg->LineOfsCC = YCCInfo.Ch[IPL_IMG_YUV_U].LineOfs;

        YSprtAddr = YCCInfo.PixelAddr[0];
        CSprtAddr = YCCInfo.PixelAddr[1];

        //calculate main output address
        YCCInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][0]);
        YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][1]) - SprtL;
        YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][2]);
        YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (YCCInfo.Ch[IPL_IMG_YUV_Y].Width + (IPL_CtrlGetInfor(Id, PxIdx[Px][1]) - SprtR)) + SeamH;
        YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
        YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
        IPL_BufGet(Id, PxIdx[Px][4], &BufInfo.Buf[0], &BufSize);
        BufInfo.Buf[1] = 0;
        BufInfo.Buf[2] = 0;
        IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, BufInfo, &YCCInfo);

        IPL_CtrlSetInfor( Id, PxIdx[Px][5], (YCCInfo.PixelAddr[0] - YSprtAddr));
        IPL_CtrlSetInfor( Id, PxIdx[Px][6], (YCCInfo.PixelAddr[1] - CSprtAddr));

        //calculate ycc information
        YCCInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][0]);
        YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][8]);
        YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][9]);
        YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (IPL_CtrlGetInfor(Id, PxIdx[Px][1]) - SprtL) + (IPL_CtrlGetInfor(Id, PxIdx[Px][1]) - SprtR) + SeamH;
        YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
        YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
    }
    else if (Id == IPL_ID_2)
    {
        //calculate Separate img information
        YCCInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][0]);
        YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][1]) - SprtR;
        YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][2]);
        YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (IPL_CtrlGetInfor(IPL_ID_1, PxIdx[Px][1]) - SprtL) + (IPL_CtrlGetInfor(Id, PxIdx[Px][1]) - SprtR) + SeamH;
        YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
        YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
        IPL_BufGet(Id, PxIdx[Px][4], &BufInfo.Buf[0], &BufSize);
        BufInfo.Buf[1] = 0;
        BufInfo.Buf[2] = 0;
        IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, BufInfo, &YCCInfo);

        StitchImg->StPos = SprtR;
        StitchImg->LineOfsY = YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs;
        StitchImg->LineOfsCC = YCCInfo.Ch[IPL_IMG_YUV_U].LineOfs;

        YSprtAddr = YCCInfo.PixelAddr[0];
        CSprtAddr = YCCInfo.PixelAddr[1];

        //calculate main output address
        YCCInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][0]);
        YCCInfo.Ch[IPL_IMG_YUV_Y].Width = SprtR;
        YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][2]);
        YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = YCCInfo.Ch[IPL_IMG_YUV_Y].Width;
        YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
        YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
        IPL_BufGet(Id, PxIdx[Px][7], &BufInfo.Buf[0], &BufSize);
        BufInfo.Buf[1] = 0;
        BufInfo.Buf[2] = 0;
        IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, BufInfo, &YCCInfo);

        IPL_CtrlSetInfor( Id, PxIdx[Px][5], (YSprtAddr - YCCInfo.PixelAddr[0]));
        IPL_CtrlSetInfor( Id, PxIdx[Px][6], (CSprtAddr - YCCInfo.PixelAddr[1]));

        //calculate ycc information
        YCCInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][0]);
        YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][8]);
        YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][9]);
        YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = SprtR;
        YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
        YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
    }

    *Px_PPB0 = YCCInfo;
}

void IPL_CtrlInitIpc_DispPxPPB(UINT32 Id, UINT32 PathEnIdx)
{
    UINT32 BufSize = 0, Px, i, Buf_num = 0;
    IPL_IMG_BUF_INFO BufInfo = {0};
    IPL_YCC_IMG_INFO YCCInfo = {0};
    ER rt = E_OK;

    UINT32 PxIdx[5][7] =
    {
        {IPLCTRL_IME_P1_BUFNUM, IPLCTRL_IME_P1_FMT, IPLCTRL_IME_P1_BUF_SIZE_H, IPLCTRL_IME_P1_BUF_SIZE_V, IPLBUF_IMEP1_1, IPLCTRL_IME_P1_OUT_CROP_H, IPLCTRL_IME_P1_OUT_CROP_V},
        {IPLCTRL_IME_P2_BUFNUM, IPLCTRL_IME_P2_FMT, IPLCTRL_IME_P2_BUF_SIZE_H, IPLCTRL_IME_P2_BUF_SIZE_V, IPLBUF_IMEP2_1, IPLCTRL_IME_P2_OUT_CROP_H, IPLCTRL_IME_P2_OUT_CROP_V},
        {IPLCTRL_IME_P3_BUFNUM, IPLCTRL_IME_P3_FMT, IPLCTRL_IME_P3_BUF_SIZE_H, IPLCTRL_IME_P3_BUF_SIZE_V, IPLBUF_IMEP3_1, IPLCTRL_IME_P3_OUT_CROP_H, IPLCTRL_IME_P3_OUT_CROP_V},
        {IPLCTRL_IME_P4_BUFNUM, IPLCTRL_IME_P4_FMT, IPLCTRL_IME_P4_BUF_SIZE_H, IPLCTRL_IME_P4_BUF_SIZE_V, IPLBUF_IMEP4_1, IPLCTRL_IME_P4_OUT_CROP_H, IPLCTRL_IME_P4_OUT_CROP_V},
        //for ccir
        {IPLCTRL_SIE_CH1_BUFNUM, IPL_IMG_Y_PACK_UV422, IPLCTRL_SIE_CH0_RAW_SIZE_H, IPLCTRL_SIE_CH0_RAW_SIZE_V, IPLBUF_SIEY_CH1_1, IPLCTRL_SIE_CH0_RAW_SIZE_H, IPLCTRL_SIE_CH0_RAW_SIZE_V},
    };

    switch (PathEnIdx)
    {
        case IPLCTRL_IME_P1_EN:
            Px = 0;
            break;

        case IPLCTRL_IME_P2_EN:
            Px = 1;
            break;

        case IPLCTRL_IME_P3_EN:
            Px = 2;
            break;

        case IPLCTRL_IME_P4_EN:
            Px = 3;
            break;

        case IPLCTRL_SIE_CH1_Y_EN:
            Px = 4;
            break;

        default:
            DBG_ERR("Id: %d, PathEnIdx error %d\r\n", Id, PathEnIdx);
            return;
    }

    if (PathEnIdx == IPLCTRL_SIE_CH1_Y_EN)
        YCCInfo.type = PxIdx[Px][1];
    else
        YCCInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][1]);

	//#NT#2017/02/06#Jarkko Chang -begin
	//#NT# add support side by side left/right, up/down case
    YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][2]);
    YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][3]);
    YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, PxIdx[Px][2]);

	if(IPL_CtrlGetInfor(Id, IPLCTRL_SBS_TYPE) == SEL_SBS_LR)
	{
        YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (IPL_CtrlGetInfor(Id, PxIdx[Px][2]) << 1);
    }
	else if (IPL_CtrlGetInfor(Id, IPLCTRL_SBS_TYPE) == SEL_SBS_UD)
	{
		if (Id == IPL_ID_1)
	        YCCInfo.Ch[IPL_IMG_YUV_Y].Height = (IPL_CtrlGetInfor(Id, PxIdx[Px][3]) << 1);
		else
		{
	        YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][3]);
		    YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
	        YCCInfo.Ch[IPL_IMG_YUV_Y].Height += YCCInfo.Ch[IPL_IMG_YUV_U].Height;
		}
	}


    YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
    YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];

    Buf_num = IPL_CtrlGetInfor(Id, PxIdx[Px][0]);
    if (Buf_num == IPL_CTRL_NO_USE)
    {
        DBG_ERR("Get buffer number error, Id: %d, PathEnIdx %d\r\n", Id, PathEnIdx);
        return;
    }

    //#NT#2016/07/26#Jarkko Chang -begin
    //#NT# fixed mantis bug id:105956
    //set dispaly buffer
    for (i = 0; i < Buf_num; i ++)
    {
        rt = IPL_BufGet(Id, (PxIdx[Px][4] + i), &BufInfo.Buf[0], &BufSize);
        if (rt != E_OK)
            return;

        BufInfo.Buf[1] = 0;
        BufInfo.Buf[2] = 0;
        IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, BufInfo, &YCCInfo);

		if(IPL_CtrlGetInfor(Id, IPLCTRL_SBS_TYPE) == SEL_SBS_LR)
		{
		    YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (IPL_CtrlGetInfor(Id, PxIdx[Px][2]) << 1);
		}
		else
		{
		    YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, PxIdx[Px][2]);
		}
	//#NT#2017/02/06#Jarkko Chang -end
        //#NT#2016/08/03#Jarkko Chang -begin
        //#NT# modify control flow for support ccir scale
        //#NT#2016/03/15#Jarkko Chang -begin
        //#NT# add support CCIR_DEINTERLACE2 flow
        if ((IPL_CtrlGetInfor(Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_DEINTERLACE)
            ||(IPL_CtrlGetInfor(Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_DEINTERLACE2)
            ||(IPL_CtrlGetInfor(Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_ISESCALE)
            || (PathEnIdx == IPLCTRL_SIE_CH1_Y_EN))
        {
            IPL_InitDisplayBuffer(0, i, &YCCInfo,Id);
            IPL_InitDisplayBuffer(1, i, &YCCInfo,Id);
            IPL_InitDisplayBuffer(2, i, &YCCInfo,Id);
        }
        else
        {
            IPL_InitDisplayBuffer(Px, i, &YCCInfo,Id);
        }
        //#NT#2016/03/15#Jarkko Chang -end
        //#NT#2016/08/03#Jarkko Chang -end
    }
    YCCInfo.Ch[0].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][5]);
    YCCInfo.Ch[0].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][6]);
    //#NT#2016/07/26#Jarkko Chang -end

    //#NT#2016/08/03#Jarkko Chang -begin
    //#NT# modify control flow for support ccir scale
    //#NT#2016/03/15#Jarkko Chang -begin
    //#NT# add support CCIR_DEINTERLACE2 flow
    if ((IPL_CtrlGetInfor(Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_DEINTERLACE)
        ||(IPL_CtrlGetInfor(Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_DEINTERLACE2)
        ||(IPL_CtrlGetInfor(Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_ISESCALE)
        || (PathEnIdx == IPLCTRL_SIE_CH1_Y_EN))
    {
        IPL_SetDeviceInputInfo(0, YCCInfo.Ch[0],Id);
        IPL_SetDeviceInputInfo(1, YCCInfo.Ch[0],Id);
        IPL_SetDeviceInputInfo(2, YCCInfo.Ch[0],Id);
    }
    else
    {
        IPL_SetDeviceInputInfo(Px, YCCInfo.Ch[0],Id);
    }
    //#NT#2016/03/15#Jarkko Chang -end
    //#NT#2016/08/03#Jarkko Chang -end
}

void IPL_CtrlInitIpc_DispPxPPB_Stitch(UINT32 Id, UINT32 PathEnIdx)
{
    UINT32 BufSize = 0, Px, i, Buf_num = 0;
    IPL_IMG_BUF_INFO BufInfo = {0};
    IPL_YCC_IMG_INFO YCCInfo = {0}, SprtYCCInfo = {0}, SeamYCCInfo = {0};
    ER rt = E_OK;
    UINT32 SprtR, SprtL, SeamH;    //Get from Stitch lib

    UINT32 PxIdx[3][7] =
    {
        {IPLCTRL_IME_P1_BUFNUM, IPLCTRL_IME_P1_FMT, IPLBUF_IMEP1_1, IPLCTRL_IME_P1_OUT_CROP_H, IPLCTRL_IME_P1_OUT_CROP_V, IPLCTRL_IME_P1_SPRT_YBUFOFS, IPLCTRL_IME_P1_SPRT_CBUFOFS},
        {IPLCTRL_IME_P2_BUFNUM, IPLCTRL_IME_P2_FMT, IPLBUF_IMEP2_1, IPLCTRL_IME_P2_OUT_CROP_H, IPLCTRL_IME_P2_OUT_CROP_V, IPLCTRL_IME_P2_SPRT_YBUFOFS, IPLCTRL_IME_P2_SPRT_CBUFOFS},
        {IPLCTRL_IME_P3_BUFNUM, IPLCTRL_IME_P3_FMT, IPLBUF_IMEP3_1, IPLCTRL_IME_P3_OUT_CROP_H, IPLCTRL_IME_P3_OUT_CROP_V, IPLCTRL_IME_P3_SPRT_YBUFOFS, IPLCTRL_IME_P3_SPRT_CBUFOFS},
    };

    switch (PathEnIdx)
    {
        case IPLCTRL_IME_P1_EN:
            Px = 0;
            break;

        case IPLCTRL_IME_P2_EN:
            Px = 1;
            break;

        case IPLCTRL_IME_P3_EN:
            Px = 2;
            break;

        default:
            DBG_ERR("Id: %d, PathEnIdx error %d\r\n", Id, PathEnIdx);
            return;
    }

    SeamH = IPL_ALIGN_ROUNDUP(Stitch_GetStitchAreaWidthInPixel(IPL_CtrlGetInfor(Id, PxIdx[Px][3])),4);
    SprtL = IPL_ALIGN_ROUNDUP(Stitch_GetLeftImgCutPointInPixel(IPL_CtrlGetInfor(Id, PxIdx[Px][3])),4);
    SprtR = IPL_ALIGN_ROUNDUP(Stitch_GetRightImgCutPointInPixel(IPL_CtrlGetInfor(Id, PxIdx[Px][3])),4);

    YCCInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][1]);

    if (Id == IPL_ID_1)
    {
        YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][3]) - SprtL;
        YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (IPL_CtrlGetInfor(Id, PxIdx[Px][3]) - SprtL) + (IPL_CtrlGetInfor(Id, PxIdx[Px][3]) - SprtR) + SeamH;
        YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][4]);
        YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
        YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
    }
    else
    {
        YCCInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][3]) - SprtR;
        YCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (IPL_CtrlGetInfor(Id, PxIdx[Px][3]) - SprtL) + (IPL_CtrlGetInfor(Id, PxIdx[Px][3]) - SprtR) + SeamH;
        YCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][4]);
        YCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(YCCInfo.type, YCCInfo.Ch[IPL_IMG_YUV_Y]);
        YCCInfo.Ch[IPL_IMG_YUV_V] = YCCInfo.Ch[IPL_IMG_YUV_U];
    }

    Buf_num = IPL_CtrlGetInfor(Id, PxIdx[Px][0]);
    if (Buf_num == IPL_CTRL_NO_USE)
    {
        DBG_ERR("Get buffer number error, Id: %d, PathEnIdx %d\r\n", Id, PathEnIdx);
        return;
    }

    //set dispaly buffer
    for (i = 0; i < Buf_num; i ++)
    {
        rt = IPL_BufGet(Id, (PxIdx[Px][2] + i), &BufInfo.Buf[0], &BufSize);
        if (rt != E_OK)
            return;

        BufInfo.Buf[1] = 0;
        BufInfo.Buf[2] = 0;
        IPL_UTI_SET_CONT_BUFINFO(YCCInfo.type, BufInfo, &YCCInfo);

        if (Id == IPL_ID_1)
        {
            //Stitch Image Left
            SprtYCCInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][1]);
            SprtYCCInfo.Ch[IPL_IMG_YUV_Y].Width = SprtL;
            SprtYCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = SprtL;
            SprtYCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][4]);
            SprtYCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(SprtYCCInfo.type, SprtYCCInfo.Ch[IPL_IMG_YUV_Y]);
            SprtYCCInfo.Ch[IPL_IMG_YUV_V] = SprtYCCInfo.Ch[IPL_IMG_YUV_U];

            SprtYCCInfo.PixelAddr[0] = YCCInfo.PixelAddr[0] - IPL_CtrlGetInfor(Id, PxIdx[Px][5]);
            SprtYCCInfo.PixelAddr[1] = YCCInfo.PixelAddr[1] - IPL_CtrlGetInfor(Id, PxIdx[Px][6]);
            SprtYCCInfo.PixelAddr[2] = YCCInfo.PixelAddr[2] - IPL_CtrlGetInfor(Id, PxIdx[Px][6]);

            //Seam YCCInfo Image
            SprtYCCInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][1]);
            SeamYCCInfo.Ch[IPL_IMG_YUV_Y].Width = SeamH;
            SeamYCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = (IPL_CtrlGetInfor(IPL_ID_1, PxIdx[Px][3]) - SprtL) + (IPL_CtrlGetInfor(Id, PxIdx[Px][3]) - SprtR) + SeamH;
            SeamYCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][4]);
            SeamYCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(SprtYCCInfo.type, SprtYCCInfo.Ch[IPL_IMG_YUV_Y]);
            SeamYCCInfo.Ch[IPL_IMG_YUV_V] = SeamYCCInfo.Ch[IPL_IMG_YUV_U];

            SeamYCCInfo.PixelAddr[0] = YCCInfo.PixelAddr[0] + YCCInfo.Ch[IPL_IMG_YUV_Y].Width;
            SeamYCCInfo.PixelAddr[1] = YCCInfo.PixelAddr[1] + YCCInfo.Ch[IPL_IMG_YUV_U].Width;
            SeamYCCInfo.PixelAddr[2] = YCCInfo.PixelAddr[2] + YCCInfo.Ch[IPL_IMG_YUV_V].Width;

            IPL_InitSeamBuffer(Px, i, &SeamYCCInfo);
        }
        else if (Id == IPL_ID_2)
        {
            YCCInfo.PixelAddr[0] -= IPL_CtrlGetInfor(Id, PxIdx[Px][5]);
            YCCInfo.PixelAddr[1] -= IPL_CtrlGetInfor(Id, PxIdx[Px][6]);
            YCCInfo.PixelAddr[2] -= IPL_CtrlGetInfor(Id, PxIdx[Px][6]);

            //Stitch Image Right
            SprtYCCInfo.type = IPL_CtrlGetInfor(Id, PxIdx[Px][1]);
            SprtYCCInfo.Ch[IPL_IMG_YUV_Y].Width = SprtR;
            SprtYCCInfo.Ch[IPL_IMG_YUV_Y].LineOfs = SprtR;
            SprtYCCInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][4]);
            SprtYCCInfo.Ch[IPL_IMG_YUV_U] = IPL_UTI_Y_INFO_CONV2(SprtYCCInfo.type, SprtYCCInfo.Ch[IPL_IMG_YUV_Y]);
            SprtYCCInfo.Ch[IPL_IMG_YUV_V] = SprtYCCInfo.Ch[IPL_IMG_YUV_U];

            SprtYCCInfo.PixelAddr[0] = YCCInfo.PixelAddr[0];
            SprtYCCInfo.PixelAddr[1] = YCCInfo.PixelAddr[1];
            SprtYCCInfo.PixelAddr[2] = YCCInfo.PixelAddr[2];
        }

        IPL_InitDisplayBuffer(Px, i, &YCCInfo,Id);
        IPL_InitSprtBuffer(Id, Px, i, &SprtYCCInfo);
    }

    if (Id == IPL_ID_1)
    {
        YCCInfo.Ch[0].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][3]) - SprtL;
    }
    else if (Id == IPL_ID_2)
    {
        YCCInfo.Ch[0].Width = IPL_CtrlGetInfor(Id, PxIdx[Px][3]) - SprtR;
    }
    YCCInfo.Ch[0].Height = IPL_CtrlGetInfor(Id, PxIdx[Px][4]);

    IPL_SetDeviceInputInfo(Px, YCCInfo.Ch[0],Id);
}

void IPL_CtrlInitIpc_DispCtrlEn(UINT32 Id, UINT32 PathEnIdx, UINT32 EnFlag)
{
    UINT32 Px;

    switch (PathEnIdx)
    {
        case IPLCTRL_IME_P1_EN:
            Px = 0;
            break;

        case IPLCTRL_IME_P2_EN:
            Px = 1;
            break;

        case IPLCTRL_IME_P3_EN:
            Px = 2;
            break;

        case IPLCTRL_IME_P4_EN:
        default:
            DBG_ERR("Id: %d, PathEnIdx error %d\r\n", Id, PathEnIdx);
            return;
    }

    IPL_SetPostImageProcessOnOff(Px, EnFlag);
}

#if 0
#endif
//#NT#2016/09/14#liwei cheng -begin
//#NT#  add surpport crop offset
INT32 Cap_Crop_Ofs_X[IPL_ID_MAX_NUM] = {0, 0, 0, 0}, Cap_Crop_Ofs_Y[IPL_ID_MAX_NUM] = {0, 0, 0, 0};
void IPL_SIESetCropOfs_Cap(UINT32 Id, INT32 CropOfsX, INT32 CropOfsY)
{
    Cap_Crop_Ofs_X[Id] = CropOfsX;
    Cap_Crop_Ofs_Y[Id] = CropOfsY;
}
//#NT#2016/09/14#liwei cheng -end
void IPL_CheckCropOfsBound_Cap(UINT32 Id, UINT32* StartPixX,UINT32* StartPixY,UINT32 ActSizeH, UINT32 ActSizeV,UINT32 CropSizeH,UINT32 CropSizeV)
{
    if (Cap_Crop_Ofs_X[Id] < 0 && ((UINT32)abs(Cap_Crop_Ofs_X[Id]) >((ActSizeH - CropSizeH) >> 1)))
    {
        DBG_ERR(" Cap Crop Offset X underflow\r\n");
        DBG_ERR(" Id:%d,Crop.StartPix.x(%d) < 0, force Crop_Ofs_X(%d) = 0,\r\n",Id,((ActSizeH - CropSizeH) >> 1)+ Cap_Crop_Ofs_X[Id],Cap_Crop_Ofs_X[Id]);
        Cap_Crop_Ofs_X[Id] = 0;
    }
    *StartPixX = ((ActSizeH - CropSizeH) >> 1)+ Cap_Crop_Ofs_X[Id];
    if ((*StartPixX + CropSizeH) > ActSizeH )
    {
        DBG_ERR(" Cap Crop Offset X Overflow\r\n");
        DBG_ERR("Id: %d, crop start (%d) + crop size (%d) > act size (%d)\r\n", Id, *StartPixX,  CropSizeH, ActSizeH);
        *StartPixX = ActSizeH - CropSizeH;
    }

    if (Cap_Crop_Ofs_Y[Id] < 0 && ((UINT32)abs(Cap_Crop_Ofs_Y[Id]) >((ActSizeV - CropSizeV) >> 1)))
    {
        DBG_ERR(" Cap Crop Offset Y underflow\r\n");
        DBG_ERR(" Id:%d,Crop.StartPix.Y(%d) < 0, force Crop_Ofs_Y(%d) = 0\r\n",Id,((ActSizeV -CropSizeV) >> 1)+ Cap_Crop_Ofs_Y[Id],Cap_Crop_Ofs_Y[Id]);
        Cap_Crop_Ofs_Y[Id] = 0;
    }

    *StartPixY = ((ActSizeV - CropSizeV) >> 1)+ Cap_Crop_Ofs_Y[Id];
    if ((*StartPixY + CropSizeV) > ActSizeV )
    {
        DBG_ERR("Cap Crop Offset Y Overflow\r\n");
        DBG_ERR("Id: %d, crop start (%d) + crop size (%d) > act size (%d)\r\n", Id,*StartPixY, CropSizeV, ActSizeV);
        *StartPixY = ActSizeV - CropSizeV;
    }
}
void IPL_CtrlInitIpc_SIEBurstLength_Cap(UINT32 Id, SIE_UPDATE *Update, SENSOR_INFO *SenInfo, SIE_BURSTLENGTH_SET *Length)
{
    //unnecessary to set burstlength
}

void IPL_CtrlInitIpc_SIEAct_Cap(UINT32 Id, SIE_UPDATE *Update, SieACTMode *ActMode)
{
    SENSOR_INFO SenInfo = {0};
    ER rt = E_OK;

    rt = IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE), &SenInfo);
    if (rt != E_OK)
        return;

    //not ready for VX1 and HDR Sensor
    //HDR Sensor => both SIE_1 and SIE_2 ActMode set to SIE_IN_CSI_1

    switch (SenInfo.DataType)
    {
        case SENSOR_DATA_LVDS:
        case SENSOR_DATA_MIPI:
            if (Id == IPL_ID_1)
            {
                *ActMode = SIE_IN_CSI_1;
            }
            else if (Id == IPL_ID_2)
            {
                if (SenInfo.Mode->ModeType == SENSOR_MODE_STAGGER_HDR)
                    *ActMode = SIE_IN_CSI_1;
                else
                    *ActMode = SIE_IN_CSI_2;
            }
            *Update |= SIE_INIT;
            break;

        case SENSOR_DATA_PARALLEL:
            if (SenInfo.SigType == SENSOR_SIGNAL_MASTER)
            {
                *ActMode = SIE_IN_PARA_MSTR_SNR;
                *Update |= SIE_INIT;
            }
            else if (SenInfo.SigType == SENSOR_SIGNAL_SLAVE)
            {
                *ActMode = SIE_IN_PARA_SLAV_SNR;
                *Update |= SIE_INIT;
            }
            else
            {
                DBG_ERR("Id: %d, unsupport signal type %d\r\n", Id, SenInfo.SigType);
            }
            break;

        case SENSOR_DATA_DUMMY:
            *ActMode = SIE_IN_SELF_PATGEN;
            *Update |= SIE_INIT;
            break;

        default:
            DBG_ERR("Id: %d, unsupport Data Type %d\r\n", Id, SenInfo.DataType);
            break;
    }
}

void IPL_CtrlInitIpc_SIESignal_Cap(UINT32 Id, SIE_UPDATE *Update, UINT32 HDInv, UINT32 VDInv, UINT32 VDPhase, UINT32 HDPhase, UINT32 DPhase, IPP_SIE_SIGNAL *Signal)
{
    Signal->Sie_HDVD.HD_ini = HDInv;
    Signal->Sie_HDVD.VD_ini = VDInv;

    Signal->SensorPhase.VD = VDPhase;
    Signal->SensorPhase.HD = HDPhase;
    Signal->SensorPhase.Data = DPhase;

    *Update |= (SIE_INIT);
}

void IPL_CtrlInitIpc_SIECh0PPB_Cap(UINT32 Id, SIE_UPDATE *Update, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV)
{
    *OutH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H);
    *OutV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V);
    Out->packBus = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAPBIT);
    Out->LineOffset = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS);
    *Update |= SIE_IOSIZE;
}

//#NT#2016/09/30#Silvia Wu -begin
//#NT# fixed ccir cap issue
void IPL_CtrlInitIpc_SIECh1PPB_Cap(UINT32 Id, SIE_UPDATE *Update, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV)// for CCIR Y
{
    if (IPL_CtrlGetInfor(Id, IPLCTRL_CFMODE) == IPL_CF_CCIR)
    {
    	*OutH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H);
	    Out->LineOffset = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS);
    }
	else
	{
    	*OutH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H)/2;
	    Out->LineOffset = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H)/2;
	}

    *OutV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V);
    Out->packBus = IPL_RAW_BIT_8;//IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAPBIT);
    //#NT#2016/10/12#Harry_Tsai -begin
    //#NT# modify to correct bit
    //*Update |= SIE_IOSIZE;
    *Update |= SIE_IOSIZE_CH1_CH2;
    //#NT#2016/10/12#Harry_Tsai -end
}

void IPL_CtrlInitIpc_SIECh2PPB_Cap(UINT32 Id, SIE_UPDATE *Update, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV)// for CCIR UV
{
	if (IPL_CtrlGetInfor(Id, IPLCTRL_CFMODE) == IPL_CF_CCIR)
	{
	    *OutH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H);
	    Out->LineOffset = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS);
	}
	else
	{
	    *OutH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H)/2;
	    Out->LineOffset = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H)/2;
	}

    *OutV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V);
    Out->packBus = IPL_RAW_BIT_8;//IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_CAPBIT);
    //#NT#2016/10/12#Harry_Tsai -begin
    //#NT# modify to correct bit
    //*Update |= SIE_IOSIZE;
    *Update |= SIE_IOSIZE_CH1_CH2;
    //#NT#2016/10/12#Harry_Tsai -end
}
//#NT#2016/09/30#Silvia Wu -end


void IPL_CtrlInitIpc_SIECh3PPB_Cap(UINT32 Id,  SIE_UPDATE *Update, SIE_Dma_Out *Out, UINT32 *OutH, UINT32 *OutV)
{
    *OutH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH3_CAP_RAW_SIZE_H);
    *OutV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH3_CAP_RAW_SIZE_V);
    Out->packBus = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH3_CAPBIT);
    Out->LineOffset = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH3_CAP_RAW_SIZE_HLOS);
    //#NT#2016/11/04#Harry_Tsai -begin
    //#NT# Only SIE1 have ch3
    if(Id > IPL_ID_1)
        return;
    //#NT#2016/11/04#Harry_Tsai -end
    //#NT#2016/10/12#Harry_Tsai -begin
    //#NT# modify to correct bit
    //*Update |= SIE_IOSIZE;
    *Update |= SIE_IOSIZE_CH3;
    //#NT#2016/10/12#Harry_Tsai -end
}

void IPL_CtrlInitIpc_SIEIn_Cap(UINT32 Id,  SIE_UPDATE *Update, SIE_WINDOW *Win)
{
    SENSOR_INFO SenInfo = {0};
    ER rt = E_OK;

    rt = IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE), &SenInfo);
    if (rt != E_OK)
        return;

    Win->Act.StartPix.x = SenInfo.Mode->TransHD[0].DataStart;
    Win->Act.StartPix.y = SenInfo.Mode->TransVD[0].DataStart;
    Win->Act.SizeH = SenInfo.Mode->TransHD[0].DataSize;
    Win->Act.SizeV = SenInfo.Mode->TransVD[0].DataSize;
    Win->Act.CFA = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CAPSTPIX);

    Win->Crop.SizeH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CAPCROP_SIZE_H);
    Win->Crop.SizeV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CAPCROP_SIZE_V);
    Win->Crop.CFA = Win->Act.CFA;

    if ((Win->Act.SizeH < Win->Crop.SizeH) || (Win->Act.SizeV < Win->Crop.SizeV))
    {
        DBG_ERR("Id: %d, act size (%d %d) < crop Size(%d %d)\r\n", Id, Win->Act.SizeH, Win->Act.SizeV, Win->Crop.SizeH, Win->Crop.SizeV);
        Win->Crop.StartPix.x = 0;
        Win->Crop.StartPix.y = 0;
    }
    else
    {
        //#NT#2016/09/14#liwei cheng -begin
        //#NT# add surpport crop offset
        IPL_CheckCropOfsBound_Cap(Id, &Win->Crop.StartPix.x,&Win->Crop.StartPix.y,Win->Act.SizeH,Win->Act.SizeV,Win->Crop.SizeH,Win->Crop.SizeV);
        //#NT#2016/09/14#liwei cheng -end
    }

    //#NT#2016/10/12#Harry_Tsai -begin
    //#NT# modify to correct bit
    //#NT#2016/03/21#Jarkko Chang -begin
    //#NT# add support ccir crop
    if (SenInfo.Mode->ModeType == SENSOR_MODE_CCIR || SenInfo.Mode->ModeType == SENSOR_MODE_CCIR_INTERLACE)
    {
        Win->Act.StartPix.x += Win->Crop.StartPix.x;
        Win->Act.StartPix.y += Win->Crop.StartPix.y;

        Win->Act.SizeH = Win->Crop.SizeH;
        Win->Act.SizeV = Win->Crop.SizeV;
    }
    else
        *Update |= SIE_IOSIZE;
    //#NT#2016/03/21#Jarkko Chang -end

    //*Update |= SIE_IOSIZE;
    //#NT#2016/10/12#Harry_Tsai -end

}

void IPL_CtrlInitIpc_SIEOB_Cap(UINT32 Id,  SIE_UPDATE *Update, SieFuncEn *FuncEn, SIE_INTE_EN *InteEn, SIE_OBAVG *OBAvg, SIE_OBSUB *OBSub)
{
    SENSOR_INFO SenInfo = {0};
    ER rt = E_OK;

    if ((Id != IPL_ID_1) && (Id != IPL_ID_2))
    {
        DBG_ERR("IPL ID Error %d : OB only support IPL_ID_1 and IPL_ID_2\r\n", Id);
        return;
    }

    rt = IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE), &SenInfo);
    if (rt != E_OK)
        return;

    if (IPL_CtrlGetInfor(Id, IPLCTRL_CAPAUTO_OB_EN) == DISABLE)
    {
        OBAvg->ObSubRatio = 0x20;
    }
    else
    {
        OBAvg->StartPix.x = SenInfo.Mode->OB.TopWin[0];
        OBAvg->StartPix.y = SenInfo.Mode->OB.TopWin[1];
        OBAvg->SizeH = (SenInfo.Mode->OB.TopWin[2] - SenInfo.Mode->OB.TopWin[0]);
        OBAvg->SizeV = (SenInfo.Mode->OB.TopWin[3] - SenInfo.Mode->OB.TopWin[1]);
        OBSub->Offset = 0;
        OBAvg->SubRatio_X = 0;
        OBAvg->AvgActThres = 0x80;
        OBAvg->ObSubRatio = 0x20;
        *FuncEn |= (OBSubSelEn | OBAvgEn);
        *Update |= (SIE_OBOFS_ | SIE_OBAVG_ | SIE_SUBFEN);
    }
}

void IPL_CtrlInitIpc_SIEGamma_Cap(UINT32 Id,  SIE_UPDATE *Update, SieFuncEn *FuncEn, SIE_GAMMA **pGamma)
{
    if ((Id != IPL_ID_1) && (Id != IPL_ID_2))
    {
        DBG_ERR("IPL ID Error %d : SIE Gamma only support IPL_ID_1 and IPL_ID_2\r\n", Id);
        return;
    }

    *Update |= SIE_GAMMA_;
    *FuncEn |= GammaEn;

    *pGamma = &SieRawEncTab;
}

void IPL_CtrlInitIpc_SIERawEnc_Cap(UINT32 Id,  SIE_UPDATE *Update, SieFuncEn *FuncEn, SIE_RAWENC *RawEnc, UINT32 QInit, UINT32 Ratio)
{
    if ((Id != IPL_ID_1) && (Id != IPL_ID_2))
    {
        DBG_ERR("IPL ID Error %d : SIE RawEnc only support IPL_ID_1 and IPL_ID_2\r\n", Id);
        return;
    }

    *Update |= SIE_BCC_ADJ;
    *FuncEn |= (RawEncEn|RawEncGAMMAEn);

    RawEnc->b2Strp = FALSE;
    RawEnc->BCC_OP_MODE = RAWENC_FRAME_MODE;
}
//#NT#2017/04/12#Bowen Li -begin
//#NT# fix CCIR cap crash issue
void IPL_SIECCIR_FCB_Cap(UINT32 Id, SIE_DVI* pDvi)
{
    SENSOR_INFO SenInfo = {0};
    ER rt = E_OK;

    rt = IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE), &SenInfo);
    if (rt != E_OK)
        return;

    if ((SenInfo.Mode->ModeType != SENSOR_MODE_CCIR) && (SenInfo.Mode->ModeType != SENSOR_MODE_CCIR_INTERLACE))
    {
        return;
    }

    if (SenInfo.Mode->DVI->Fmt == SENSOR_DVI_CCIR601)
    {
        pDvi->SIE_DVIFmt = SIE_CCIR601;
    }
    else if (SenInfo.Mode->DVI->Fmt == SENSOR_DVI_CCIR656_EAV)
    {
        pDvi->SIE_DVIFmt = SIE_CCIR656_EAV;
    }
    else if (SenInfo.Mode->DVI->Fmt == SENSOR_DVI_CCIR656_ACT)
    {
        pDvi->SIE_DVIFmt = SIE_CCIR656_ACT;
    }
    else
    {
        DBG_ERR("Id: %d, unsupport SIE_DVIFmt = %d\r\n", Id, SenInfo.Mode->DVI->Fmt);
    }

    if (SenInfo.Mode->DVI->Mode == SENSOR_DVI_MODE_SD)
    {
        pDvi->SIE_DVIMode = SIE_SDMode;
    }
    else if (SenInfo.Mode->DVI->Mode == SENSOR_DVI_MODE_HD)
    {
        pDvi->SIE_DVIMode = SIE_HDMode;
    }
    else if (SenInfo.Mode->DVI->Mode == SENSOR_DVI_MODE_HD_INV)
    {
        pDvi->SIE_DVIMode = SIE_HDMode_Inv;
    }
    else
    {
        DBG_ERR("Id: %d, unsupport SIE_DVIMode = %d\r\n", Id, SenInfo.Mode->DVI->Mode);
    }

    if (SenInfo.Mode->DVI->DataFmt == SENSOR_DVI_YUYV)
    {
        pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYUYV;
    }
    else if (SenInfo.Mode->DVI->DataFmt == SENSOR_DVI_YVYU)
    {
        pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPYVYU;
    }
    else if (SenInfo.Mode->DVI->DataFmt == SENSOR_DVI_UYVY)
    {
        pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPUYVY;
    }
    else if (SenInfo.Mode->DVI->DataFmt == SENOSR_DVI_VYUY)
    {
        pDvi->DVIOUT_SWAPSEL = DVIOUT_SWAPVYUY;
    }
    else
    {
        DBG_ERR("Id: %d, unsupport DVIOUT_SWAPSEL = %d\r\n", Id, SenInfo.Mode->DVI->DataFmt);
    }

    pDvi->FldEn = SenInfo.Mode->DVI->FldEn;
    pDvi->FldInvEn = SenInfo.Mode->DVI->FldInvEn;
    pDvi->OutSplit = SenInfo.Mode->DVI->OutSplitEn;
}
//#NT#2017/04/12#Bowen Li -end
#if 0
#endif
void IPL_CtrlInitPrvPProcInfo(UINT32 Id, IPL_POST_PROC_FUNC_EN FuncEn)
{
/*
    IPL_PPROC_INFO Info = {0};

    if (FuncEn & IPL_FUNC_DISP)
    {
        IPL_CtrlInitIpc_DispCtrlEn(Id, IPLCTRL_IME_P1_EN, ENABLE);
        IPL_CtrlInitIpc_DispCtrlEn(Id, IPLCTRL_IME_P2_EN, ENABLE);
        IPL_CtrlInitIpc_DispCtrlEn(Id, IPLCTRL_IME_P3_EN, ENABLE);
        IPL_CtrlInitIpc_DispCtrlEn(Id, IPLCTRL_IME_P4_EN, ENABLE);
        IPL_Display_Resume();
    }
    else
    {
        IPL_Display_Pause();
    }

    if ((FuncEn & IPL_FUNC_IMGEFFECT_WDR) && (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW) == ICF_FLOW_B))
    {
        IPL_BufGet(Id, IPLBUF_IMGEFFECT_WDR, &Info.bufaddr, &Info.bufsize);
        Info.ExtData[0] = IPL_CtrlGetHalSetIdPProc(Id);
        Info.IsLockfp = IPL_PPostHDR_IsLock;
        Info.PProcfp = IPL_PPostHDR_Proc;
        IPL_PProc_LoadFunc(&Info);
    }


    if (FuncEn & IPL_FUNC_PHOTOFRAME)
    {
//        PhotoFrame_Open();    //ipl tmp
        IPL_BufGet(Id, IPLBUF_PHOTOFRAME, &Info.bufaddr, &Info.bufsize);
        Info.IsLockfp = IPL_PPostPF_IsLock;
        Info.PProcfp = IPL_PPostPF_Proc;
        IPL_PProc_LoadFunc(&Info);
        {
            UINT32 i,j, data,temp;

            for(j=0;j<240;j++)
            {
                temp = (255-j);
                data = temp | (temp<<8) | (temp<<16) | (temp<<24);
                for(i=0;i<320;i+=4)
                    *(UINT32 *)(Info.bufaddr + j*320 +i) = data;
            }
        }
    }

    if (FuncEn & IPL_FUNC_STAMP)
    {
        IPL_BufGet(Id, IPLBUF_STAMP, &Info.bufaddr, &Info.bufsize);
        Info.IsLockfp = IPL_PPostDS_IsLock;
        Info.PProcfp = IPL_PPostDS_Proc;
        IPL_PProc_LoadFunc(&Info);
    }
*/
    /*  //ipl tmp: remove rsc control???
    if (FuncEn & IPL_FUNC_RSC)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_RSC_EN, ENABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_RSC_RATIO, 10);
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_RSC_EN, DISABLE);
        IPL_CtrlSetInfor(Id, IPLCTRL_RSC_RATIO, 0);
    }
    */
}


#if 0
#endif

SIE_DVI DVI_Cap = {SIE_CCIR601,SIE_SDMode,DVIOUT_SWAPYUYV,FALSE,FALSE,TRUE};

void IPL_CtrlInitCapIpcInfo(IPL_GET_CAP_RAW_DATA *CapInfo, IPC_SIE_Info *Info, IPLCTRL_CAP_EXT_INFO *ExtInfo)
{
    static UINT32 RawEncFirstFlag = TRUE;
    UINT32 Ch0HSize = 0, Ch0VSize = 0;
    IPL_HAL_READ readBit = {0};
    SENSOR_INFO SenInfo = {0};
    SIE_UPDATE tmpUpdate = 0;
    SIE_WINDOW tmpWin = {0};
    IPC_QINFO QInfo;
    IPL_HAL_GROUP Group = {0};
    ENG_SUBFUNC SubFunc = {0};
    SENSOR_INIT_OBJ SenObj = {0};
    ER rt = E_OK;

    rt = IPL_GetSensorStatus(CapInfo->Id, IPL_AlgGetUIInfo(CapInfo->Id, IPL_SEL_CAPSENMODE), &SenInfo);
    if (rt != E_OK)
        return;

    if (IPLCTRL_CAP_FLOW_INVID == IPL_CtrlGetInfor(CapInfo->Id, IPLCTRL_CAPFLOW))
    {
        //read sie original settings
        readBit.sieRead = SIE_R_SUBFEN;
        SIEx_ENG_Read(CapInfo->Id, IPL_CtrlGetHalSetId(CapInfo->Id), readBit, (void *)&Info->psie->pSubFunc->sieFuncEn);
        //readBit.sieRead = SIE_R_IO;
        //SIEx_ENG_Read(CapInfo->Id, IPL_CtrlGetHalSetId(CapInfo->Id), readBit, (void *)Info->psie);
        //readBit.sieRead = SIE_R_Out;
        //SIEx_ENG_Read(CapInfo->Id, IPL_CtrlGetHalSetId(CapInfo->Id), readBit, (void *)Info->psie);

        readBit.sieRead = SIE_R_INSIZE;
        SIEx_ENG_Read(CapInfo->Id, IPL_CtrlGetHalSetId(CapInfo->Id), readBit, (void *)&Info->psie->IMG_Window);

        //read pre dram in information(special case)
        QInfo = IPL_CTRLQuery(CapInfo->Id, ICF_IPLIO_RDY);

        /**************** save last sie information ****************/
        ExtInfo->Param[0] = Info->psie->CH0.Out.PPB0_Addr;
        ExtInfo->Param[1] = Info->psie->CH0.Out.PPB1_Addr;
        ExtInfo->Param[2] = Info->psie->CH0.Out.LineOffset;
        ExtInfo->Param[3] = IPL_CtrlGetInfor(CapInfo->Id, IPLCTRL_PRVBURSTLENGTH);
        ExtInfo->Param[4] = Info->psie->IMG_Window.Crop.StartPix.x;
        ExtInfo->Param[5] = Info->psie->IMG_Window.Crop.StartPix.y;
        ExtInfo->Param[6] = Info->psie->IMG_Window.Crop.SizeH;
        ExtInfo->Param[7] = Info->psie->IMG_Window.Crop.SizeV;
        //ExtInfo->Param[8] = QInfo.PreDmaIn_Rdy.InDma.PixelAddr;
        //ExtInfo->Param[9] = QInfo.PreDmaIn_Rdy.PPB1_Addr;
        /**************** load new sie information ****************/
        IPL_CtrlInitIpc_SIEBurstLength_Cap(CapInfo->Id, &Info->sieUpdate, &SenInfo, &Info->psie->BurstLength);
        IPL_CtrlInitIpc_SIEIn_Cap(CapInfo->Id, &tmpUpdate, &tmpWin);
        Info->psie->IMG_Window.Crop.StartPix.x = tmpWin.Crop.StartPix.x;
        Info->psie->IMG_Window.Crop.StartPix.y = tmpWin.Crop.StartPix.y;
        Info->psie->IMG_Window.Crop.SizeH = tmpWin.Crop.SizeH;
        Info->psie->IMG_Window.Crop.SizeV = tmpWin.Crop.SizeV;

        IPL_CtrlInitIpc_SIECh0PPB_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->CH0.Out, &Ch0HSize, &Ch0VSize);
        Info->psie->CH0.Src = NonScaled_Img;
        Info->sieUpdate |= SIE_CH0Out_SRC;
    }
    else if (IPLCTRL_CAP_FLOW_SIM == IPL_CtrlGetInfor(CapInfo->Id, IPLCTRL_CAPFLOW))
    {
    }
    else
    {
        IPL_CtrlInitIpc_SIEBurstLength_Cap(CapInfo->Id, &Info->sieUpdate, &SenInfo, &Info->psie->BurstLength);

        if(CapInfo->res[0] == 0x123)// capture CCIR image
        {
            UINT32 Ch1HSize, Ch1VSize,Ch2HSize, Ch2VSize, l_ofs = 0;

            //#NT#2016/10/12#Harry Tsai -begin
            //#NT# Modify CCIR flow
            ExtInfo->Param[0] = CapInfo->res[0];
            //#NT#2016/10/12#Harry Tsai -end

            IPL_CtrlInitIpc_SIESignal_Cap(CapInfo->Id, &Info->sieUpdate, FALSE, FALSE, _RISING, _RISING, _RISING, &Info->psie->Sie_Signal);
			//#NT#2017/04/12#Bowen Li -begin
			//#NT# fix CCIR cap crash issue
            SenObj = DrvSensor_GetObj(IPL_UTI_CONV2_SEN_ID(CapInfo->Id));
            IPL_CtrlInitIpc_SIEAct_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->ActMode);
            if(SenObj.CmdInfo.CmdType == SENSOR_CMD_Vx1)
            {
                Info->psie->ActMode = SIE_IN_VX1_SNR;
            }

            Info->psie->pSubFunc->pDvi = &DVI_Cap;
			IPL_SIECCIR_FCB_Cap(CapInfo->Id, Info->psie->pSubFunc->pDvi);
			if (Info->psie->pSubFunc->pDvi->SIE_DVIMode == SIE_SDMode)
			{
				l_ofs = 1;
			}
			IPL_CtrlInitIpc_SIECh1PPB_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->CH1.Out, &Ch1HSize, &Ch1VSize);
            IPL_CtrlInitIpc_SIECh2PPB_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->CH2.Out, &Ch2HSize, &Ch2VSize);
            //IPL_CtrlInitIpc_SIECh3PPB_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->CH3.Out, &Info->psie->DmaOutSizeH, &Info->psie->DmaOutSizeV);
            IPL_CtrlInitIpc_SIEIn_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->IMG_Window);
            if ((Info->psie->IMG_Window.Crop.SizeH != (Ch1HSize << l_ofs)) || (Info->psie->IMG_Window.Crop.SizeV != Ch1VSize))
            {
                DBG_ERR("Id: %d, ch0 output size (%d %d) < crop Size(%d %d)\r\n", CapInfo->Id, Ch1HSize, Ch1VSize, Info->psie->IMG_Window.Crop.SizeH, Info->psie->IMG_Window.Crop.SizeV);
            }
            Info->psie->pSubFunc->InteEn = SIE__INTE_FLDEND;
			//#NT#2017/04/12#Bowen Li -end

            Info->psie->CH1.Out.PPB0_Addr = CapInfo->BufAddrCh0;
            Info->psie->CH1.Out.PPB1_Addr = CapInfo->BufAddrCh0;
            Info->psie->CH1.SiePPBSW = SIE_BUF_OFF;
            Info->psie->CH2.Out.PPB0_Addr = CapInfo->BufAddrCh0_1;
            Info->psie->CH2.Out.PPB1_Addr = CapInfo->BufAddrCh0_1;
            Info->psie->CH2.SiePPBSW = SIE_BUF_OFF;
            Info->psie->pSubFunc->sieFuncEn |= DviEn;
            Info->sieUpdate |= (SIE_SUBFEN | SIE_OUTADDR_CH1 | SIE_OUTADDR_CH2|SIE_IOSIZE_CH1_CH2|SIE_DVI_);

        }
        else // capture raw image
        {
            if (IPLCTRL_CAP_FLOW_EBURST == IPL_CtrlGetInfor(CapInfo->Id, IPLCTRL_CAPFLOW))
            {
                /**************** load cap task information ****************/
                ExtInfo->Param[0] = CapInfo->res[0];

                if (ExtInfo->Param[0] == 0)
                {
                    RawEncFirstFlag = TRUE;
                    if(CapInfo->res[5] == 1) // H_Flip
                    {
                        Info->sieUpdate |= (SIE_FLIP_CH0|SIE_FLIP_CH3);
                        Info->psie->CH0.Out.HFlip = TRUE;
                        Info->psie->CH0.Out.VFlip = FALSE;
                        Info->psie->CH3.Out.HFlip = TRUE;
                        Info->psie->CH3.Out.VFlip = FALSE;
                    }
                    else if(CapInfo->res[5] == 2) // V_Flip
                    {
                        Info->sieUpdate |= (SIE_FLIP_CH0|SIE_FLIP_CH3);
                        Info->psie->CH0.Out.HFlip = FALSE;
                        Info->psie->CH0.Out.VFlip = TRUE;
                        Info->psie->CH3.Out.HFlip = FALSE;
                        Info->psie->CH3.Out.VFlip = TRUE;
                    }
                    else if(CapInfo->res[5] == 3) // H_V_Flip
                    {
                        Info->sieUpdate |= (SIE_FLIP_CH0|SIE_FLIP_CH3);
                        Info->psie->CH0.Out.HFlip = TRUE;
                        Info->psie->CH0.Out.VFlip = TRUE;
                        Info->psie->CH3.Out.HFlip = TRUE;
                        Info->psie->CH3.Out.VFlip = TRUE;
                    }
                    else // Original
                    {
                        //Info->sieUpdate |= (SIE_FLIP_CH0|SIE_FLIP_CH3);
                        Info->psie->CH0.Out.HFlip = FALSE;
                        Info->psie->CH0.Out.VFlip = FALSE;
                        Info->psie->CH3.Out.HFlip = FALSE;
                        Info->psie->CH3.Out.VFlip = FALSE;
                    }
                }
                else if (ExtInfo->Param[0] == 1)
                {
                    IPL_CtrlInitIpc_SIEGamma_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->pSubFunc->sieFuncEn, &Info->psie->pSubFunc->pGamma);
                    IPL_CtrlInitIpc_SIERawEnc_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->pSubFunc->sieFuncEn,
                                                    &Info->psie->pSubFunc->RawEnc, CapInfo->CompressQInit, CapInfo->CompressRatio);

                    //only update gamma at first time
                    if (RawEncFirstFlag == FALSE)
                    {
                        Info->sieUpdate &= ~SIE_GAMMA_;
                    }
                    RawEncFirstFlag = FALSE;
                }
            }
            else
            {
                if(CapInfo->res[5] == 1) // H_Flip
                {
                    Info->sieUpdate |= (SIE_FLIP_CH0|SIE_FLIP_CH3);
                    Info->psie->CH0.Out.HFlip = TRUE;
                    Info->psie->CH0.Out.VFlip = FALSE;
                    Info->psie->CH3.Out.HFlip = TRUE;
                    Info->psie->CH3.Out.VFlip = FALSE;
                }
                else if(CapInfo->res[5] == 2) // V_Flip
                {
                    Info->sieUpdate |= (SIE_FLIP_CH0|SIE_FLIP_CH3);
                    Info->psie->CH0.Out.HFlip = FALSE;
                    Info->psie->CH0.Out.VFlip = TRUE;
                    Info->psie->CH3.Out.HFlip = FALSE;
                    Info->psie->CH3.Out.VFlip = TRUE;
                }
                else if(CapInfo->res[5] == 3) // H_V_Flip
                {
                    Info->sieUpdate |= (SIE_FLIP_CH0|SIE_FLIP_CH3);
                    Info->psie->CH0.Out.HFlip = TRUE;
                    Info->psie->CH0.Out.VFlip = TRUE;
                    Info->psie->CH3.Out.HFlip = TRUE;
                    Info->psie->CH3.Out.VFlip = TRUE;
                }
                else // Original
                {
                    //Info->sieUpdate |= (SIE_FLIP_CH0|SIE_FLIP_CH3);
                    Info->psie->CH0.Out.HFlip = FALSE;
                    Info->psie->CH0.Out.VFlip = FALSE;
                    Info->psie->CH3.Out.HFlip = FALSE;
                    Info->psie->CH3.Out.VFlip = FALSE;
                }

                //#NT#2016/09/20#Harry_Tsai -begin
                //#NT# add the raw encoding option for capture flow
                if (CapInfo->res[0] == 1)
                {
                    IPL_CtrlInitIpc_SIEGamma_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->pSubFunc->sieFuncEn, &Info->psie->pSubFunc->pGamma);
                    IPL_CtrlInitIpc_SIERawEnc_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->pSubFunc->sieFuncEn,
                                                    &Info->psie->pSubFunc->RawEnc, CapInfo->CompressQInit, CapInfo->CompressRatio);
                    Info->psie->pSubFunc->sieFuncEn &= (~GammaEn);
                }
                //#NT#2016/09/20#Harry_Tsai -end
            }

            IPL_SIESetDP_FCB(CapInfo->Id, &Info->psie->pSubFunc->sieFuncEn, &Info->psie->pSubFunc->InteEn, &Info->sieUpdate, Info->psie->pSubFunc);
            IPL_CtrlInitIpc_SIESignal_Cap(CapInfo->Id, &Info->sieUpdate, FALSE, FALSE, _RISING, _RISING, _RISING, &Info->psie->Sie_Signal);
            IPL_CtrlInitIpc_SIEAct_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->ActMode);
            IPL_CtrlInitIpc_SIECh0PPB_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->CH0.Out, &Ch0HSize, &Ch0VSize);
            IPL_CtrlInitIpc_SIECh3PPB_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->CH3.Out, &Info->psie->DmaOutSizeH, &Info->psie->DmaOutSizeV);
            IPL_CtrlInitIpc_SIEIn_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->IMG_Window);
            Info->psie->CH0.Src = NonScaled_Img;
            Info->psie->CH3.Src = CH3_Scaled_Img;
            if ((Info->psie->IMG_Window.Crop.SizeH != Ch0HSize) || (Info->psie->IMG_Window.Crop.SizeV != Ch0VSize))
            {
                DBG_ERR("Id: %d, ch0 output size (%d %d) < crop Size(%d %d)\r\n", CapInfo->Id, Ch0HSize, Ch0VSize, Info->psie->IMG_Window.Crop.SizeH, Info->psie->IMG_Window.Crop.SizeV);
            }
            IPL_CtrlInitIpc_SIEOB_Cap(CapInfo->Id, &Info->sieUpdate, &Info->psie->pSubFunc->sieFuncEn, &Info->psie->pSubFunc->InteEn, &Info->psie->pSubFunc->OBAvg, &Info->psie->pSubFunc->OBSub);

            Group.sieUpdate = Info->sieUpdate;
            SubFunc.pSie_sub = Info->psie->pSubFunc;
            IQCB_SettingFlowFunc(CapInfo->Id, IQCB_PREIME_D2D_SIE_PRI, &SubFunc, &Group);
            Info->sieUpdate = Group.sieUpdate;
            //#NT#2016/10/12#Harry_Tsai -begin
            //#NT# move to correct position
            Info->sieUpdate |= (SIE_CH0Out_SRC|SIE_SUBFEN);
        }
        //Info->sieUpdate |= (SIE_CH0Out_SRC|SIE_SUBFEN);
        //#NT#2016/10/12#Harry_Tsai -end

        Info->psie->pSubFunc->InteEn = SIE__INTE_FLDEND;

        if (IPLCTRL_CAP_FLOW_MFHDR == IPL_CtrlGetInfor(CapInfo->Id, IPLCTRL_CAPFLOW))
        {
            if (CapInfo->OutCh0En == ENABLE)
            {
                Info->psie->CH0.Out.PPB0_Addr = ExtInfo->Param[0];
                Info->psie->CH0.Out.PPB1_Addr = ExtInfo->Param[0];
                Info->psie->pSubFunc->sieFuncEn |= DMAOutCh0En;
                Info->sieUpdate |= SIE_OUTADDR_CH0;
            }

            if (CapInfo->OutCh3En == ENABLE)
            {
                Info->psie->CH3.Out.PPB0_Addr = ExtInfo->Param[1];
                Info->psie->CH3.Out.PPB1_Addr = ExtInfo->Param[1];
                Info->psie->pSubFunc->sieFuncEn |= DMAOutCh3En;
                Info->sieUpdate |= (SIE_OUTADDR_CH3|SIE_IOSIZE_CH3|SIE_CH3Out_SRC);
            }
        }
        else
        {
            if (CapInfo->OutCh0En == ENABLE)
            {
                Info->psie->CH0.Out.PPB0_Addr = CapInfo->BufAddrCh0;
                Info->psie->CH0.Out.PPB1_Addr = CapInfo->BufAddrCh0;
                Info->psie->pSubFunc->sieFuncEn |= DMAOutCh0En;
                Info->sieUpdate |= SIE_OUTADDR_CH0;
            }

            if (CapInfo->OutCh3En == ENABLE)
            {
                Info->psie->CH3.Out.PPB0_Addr = CapInfo->BufAddrCh3;
                Info->psie->CH3.Out.PPB1_Addr = CapInfo->BufAddrCh3;
                Info->psie->pSubFunc->sieFuncEn |= DMAOutCh3En;
                Info->sieUpdate |= (SIE_OUTADDR_CH3|SIE_IOSIZE_CH3|SIE_CH3Out_SRC);
            }
        }
    }
}

void IPL_CtrlInitPrvIpcInfo(UINT32 Id, IPC_INFO *IpcInfo, IPL_ISR_OBJ_INFO *IsrInfo, IPL_CHGMODE_DATA *ChgMode)
{
    UINT32 i;


    switch (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW))
    {
        case ICF_FLOW_A:
            IPL_CtrlInitIpcInfoFlowA(Id, IpcInfo);
            break;

        case ICF_FLOW_C:
            IPL_CtrlInitIpcInfoFlowC(Id, IpcInfo);
            break;

        case ICF_FLOW_DEWARP:
            IPL_CtrlInitIpcInfoFlowD(Id, IpcInfo);
            break;

        default:
            DBG_ERR("Id: %d, unsupport IPL flow %d\r\n", Id, IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW));
            break;
    }

    IsrInfo->RAWHeader_CB = IPL_RAWHeaderCB;
    IsrInfo->H264_CB = ChgMode->CB_H264;
    IsrInfo->EventTab.FP[IPL_ISR_SIE1] = IPL_IsrSIECB;
    IsrInfo->EventTab.FP[IPL_ISR_SIE2] = IPL_IsrSIECB;
    IsrInfo->EventTab.FP[IPL_ISR_RDE] = NULL;
    IsrInfo->EventTab.FP[IPL_ISR_RHE] = NULL;
    IsrInfo->EventTab.FP[IPL_ISR_IFE] = NULL;
    IsrInfo->EventTab.FP[IPL_ISR_IPE] = IPL_IsrIPECB;
    IsrInfo->EventTab.FP[IPL_ISR_IME] = IPL_IsrIMECB;
    IsrInfo->EventTab.FP[IPL_ISR_IFE2] = NULL;
    IsrInfo->EventTab.FP[IPL_ISR_DCE] = IPL_IsrDCECB;

    for (i = 0; i < IPL_ISR_MAX_CNT; i ++)
    {
        IsrInfo->CB_FP.FP[i] = ChgMode->ISR_CB_FP[i];
    }
}

#if 0
#endif

IPL_TRIG_CTRL_CMD_INFO gTrigInfo = {IPL_TRIG_VD, 0, DISABLE, DISABLE};

void IPL_SetTriggerInfo(IPL_TRIG_CTRL_CMD_INFO TrigInfo)
{
     gTrigInfo.type = TrigInfo.type;
     gTrigInfo.InterVal = TrigInfo.InterVal;
     gTrigInfo.smode = TrigInfo.smode;
}

IPL_TRIG_CTRL_CMD_INFO IPL_GetTriggerInfo(void)
{
    return gTrigInfo;
}


ER IPL_GetSensorStatus(UINT32 Id, SENSOR_MODE Mode, SENSOR_INFO *Info)
{
    SENSOR_MODE SenMode = SENSOR_MODE_UNKNOWN;
    IPL_MODE_INFOR IPL_Info = {0};
    ER rt = E_OK;

	//#NT#2016/12/23#Jarkko#[0114861] Chang -begin
	//#NT# fixed dual sensor sleep/wakeup fail issue
    //get mode status, special case for hdr sensor
    if (Id == IPL_ID_2 && Sensor_IsOpen(IPL_UTI_CONV2_SEN_ID(IPL_ID_1)) == TRUE && IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR)
    {
        IPL_Info.Id = IPL_ID_1;
        rt = IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&IPL_Info);

        if (rt != E_OK)
        {
            DBG_ERR("Get IPL current mode fail Id:%d\r\n", Id);
            return rt;
        }
        if (IPL_Info.Mode == IPL_MODE_INIT)
            SenMode = IPL_AlgGetUIInfo(IPL_ID_1, IPL_SEL_PRVSENMODE);
        else
            Sensor_GetCurMode(IPL_UTI_CONV2_SEN_ID(IPL_ID_1), &SenMode);

        if (SenMode != SENSOR_MODE_UNKNOWN)
        {
            rt = Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(IPL_ID_1), SenMode, Info);
            if (rt != E_OK || Info->Mode == NULL)
            {
                DBG_ERR("Get Sensor Status Fail Id:%d, Sensor_Mode: %d\r\n", Id, Mode);
                return E_SYS;
            }

            if (Info->Mode->ModeType == SENSOR_MODE_STAGGER_HDR)
                return rt;
        }
    }
	//#NT#2016/12/23#Jarkko#[0114861] Chang -end
	//#NT#2016/09/13#Jarkko Chang -begin
	//#NT# fixed ipl2sensor id mapping issue
    if (Sensor_IsOpen(IPL_UTI_CONV2_SEN_ID(Id)) == TRUE)
	//#NT#2016/09/13#Jarkko Chang -end
    {
        rt = Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), Mode, Info);
        if (Info->Mode == NULL)
            rt = E_SYS;
    }
    else
    {
        DBG_ERR("Sensor not open! Id %d\r\n", Id);
        rt = E_SYS;
    }

    if (rt != E_OK)
        DBG_ERR("Get Sensor Status Fail Id:%d, Sensor_Mode: %d\r\n", Id, Mode);

    return rt;
}

UINT32 IPL_CtrlGetSIEVABufInfo(void)
{
    return (16 * 16 * 2 * 4);
}

void IPL_GetImePathInfo(UINT32 Id, IPL_IME_PATH PathId, IPL_IME_PATH_INFO *Info)
{
    if (Info == NULL)
    {
        DBG_ERR("Id: %d, input fail\r\n", Id);
        return;
    }

    switch (PathId)
    {
        case IPL_IME_PATH1:
            Info->OutputEn = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_EN);
            Info->ImgSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_H);
            Info->ImgSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_V);
            Info->ImgSizeLOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_HLOS);
            Info->ImgFmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_FMT);

            Info->CropStartX = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_STR_X);
            Info->CropStartY = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_STR_Y);
            Info->CropSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_H);
            Info->CropSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_V);
            break;

        case IPL_IME_PATH2:
            Info->OutputEn = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_EN);
            Info->ImgSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_H);
            Info->ImgSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_V);
            Info->ImgSizeLOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_HLOS);
            Info->ImgFmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_FMT);

            Info->CropStartX = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_STR_X);
            Info->CropStartY = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_STR_Y);
            Info->CropSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_H);
            Info->CropSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_V);
            break;

        case IPL_IME_PATH3:
            Info->OutputEn = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_EN);
            Info->ImgSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_H);
            Info->ImgSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_V);
            Info->ImgSizeLOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_HLOS);
            Info->ImgFmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_FMT);

            Info->CropStartX = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_STR_X);
            Info->CropStartY = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_STR_Y);
            Info->CropSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_H);
            Info->CropSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_V);
            break;

        case IPL_IME_PATH4:
            Info->OutputEn = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_EN);
            Info->ImgSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_SIZE_H);
            Info->ImgSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_SIZE_V);
            Info->ImgSizeLOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_SIZE_HLOS);
            Info->ImgFmt = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_FMT);

            Info->CropStartX = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_STR_X);
            Info->CropStartY = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_STR_Y);
            Info->CropSizeH = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_H);
            Info->CropSizeV = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_V);
            break;

        default:
            DBG_ERR("Id: %d, path id fail %d\r\n", Id, PathId);
            break;
    }
}

BOOL IPL_ChkImePathInfo(UINT32 Id, IPL_IME_PATH PathId, IPL_IME_PATH_INFO *Info)
{
    IPL_YCC_IMG_INFO orgInfo = {0}, newInfo = {0};
    UINT32 orgYSize, orgUVSize, newYSize, newUVSize;
    BOOL Crop_OverFlow = FALSE;

    if (Info == NULL)
    {
        DBG_ERR("Id: %d, input fail\r\n", Id);
        return FALSE;
    }

    switch (PathId)
    {
        case IPL_IME_PATH1:
            if (Info->OutputEn == DISABLE)
            {
                return TRUE;
            }
            //buf
            orgInfo.type = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_FMT);
            orgInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_H);
            orgInfo.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_H);
            orgInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_BUF_SIZE_V);
            orgInfo.Ch[IPL_IMG_YUV_UV] = IPL_UTI_Y_INFO_CONV2(orgInfo.type, orgInfo.Ch[IPL_IMG_YUV_Y]);

            //new
            newInfo.type = Info->ImgFmt;
            newInfo.Ch[IPL_IMG_YUV_Y].Width = Info->CropSizeH;
            newInfo.Ch[IPL_IMG_YUV_Y].LineOfs = Info->ImgSizeLOfs;
            newInfo.Ch[IPL_IMG_YUV_Y].Height = Info->CropSizeV;
            newInfo.Ch[IPL_IMG_YUV_UV] = IPL_UTI_Y_INFO_CONV2(newInfo.type, newInfo.Ch[IPL_IMG_YUV_Y]);
            break;

        case IPL_IME_PATH2:
            if (Info->OutputEn == DISABLE)
            {
                return TRUE;
            }
            //buf
            orgInfo.type = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_FMT);
            orgInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_H);
            orgInfo.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_H);
            orgInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_BUF_SIZE_V);
            orgInfo.Ch[IPL_IMG_YUV_UV] = IPL_UTI_Y_INFO_CONV2(orgInfo.type, orgInfo.Ch[IPL_IMG_YUV_Y]);

            //new
            newInfo.type = Info->ImgFmt;
            newInfo.Ch[IPL_IMG_YUV_Y].Width = Info->CropSizeH;
            newInfo.Ch[IPL_IMG_YUV_Y].LineOfs = Info->ImgSizeLOfs;
            newInfo.Ch[IPL_IMG_YUV_Y].Height = Info->CropSizeV;
            newInfo.Ch[IPL_IMG_YUV_UV] = IPL_UTI_Y_INFO_CONV2(newInfo.type, newInfo.Ch[IPL_IMG_YUV_Y]);
            break;

        case IPL_IME_PATH3:
            if (Info->OutputEn == DISABLE)
            {
                return TRUE;
            }
            //org
            orgInfo.type = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_FMT);
            orgInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_H);
            orgInfo.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_H);
            orgInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_BUF_SIZE_V);
            orgInfo.Ch[IPL_IMG_YUV_UV] = IPL_UTI_Y_INFO_CONV2(orgInfo.type, orgInfo.Ch[IPL_IMG_YUV_Y]);

            //new
            newInfo.type = Info->ImgFmt;
            newInfo.Ch[IPL_IMG_YUV_Y].Width = Info->CropSizeH;
            newInfo.Ch[IPL_IMG_YUV_Y].LineOfs = Info->ImgSizeLOfs;
            newInfo.Ch[IPL_IMG_YUV_Y].Height = Info->CropSizeV;
            newInfo.Ch[IPL_IMG_YUV_UV] = IPL_UTI_Y_INFO_CONV2(newInfo.type, newInfo.Ch[IPL_IMG_YUV_Y]);
            break;

        case IPL_IME_PATH4:
            if (Info->OutputEn == DISABLE)
            {
                return TRUE;
            }
            //#NT#2016/05/24#Jarkko Chang -begin
            //#NT# remove unused code
            //org
            //orgInfo.type = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_FMT);
            orgInfo.Ch[IPL_IMG_YUV_Y].Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_BUF_SIZE_H);
            orgInfo.Ch[IPL_IMG_YUV_Y].LineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_BUF_SIZE_H);
            orgInfo.Ch[IPL_IMG_YUV_Y].Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_BUF_SIZE_V);
            orgInfo.Ch[IPL_IMG_YUV_UV] = orgInfo.Ch[IPL_IMG_YUV_Y];

            //new
            //newInfo.type = Info->ImgFmt;
            newInfo.Ch[IPL_IMG_YUV_Y].Width = Info->CropSizeH;
            newInfo.Ch[IPL_IMG_YUV_Y].LineOfs = Info->ImgSizeLOfs;
            newInfo.Ch[IPL_IMG_YUV_Y].Height = Info->CropSizeV;
            newInfo.Ch[IPL_IMG_YUV_UV] = newInfo.Ch[IPL_IMG_YUV_Y];
            //#NT#2016/05/24#Jarkko Chang# -end
            break;

        default:
            DBG_ERR("Id: %d, path id fail %d\r\n", Id, PathId);
            return FALSE;
    }

    orgYSize = (orgInfo.Ch[IPL_IMG_YUV_Y].LineOfs * orgInfo.Ch[IPL_IMG_YUV_Y].Height);
    orgUVSize = (orgInfo.Ch[IPL_IMG_YUV_UV].LineOfs * orgInfo.Ch[IPL_IMG_YUV_UV].Height);

    newYSize = (newInfo.Ch[IPL_IMG_YUV_Y].LineOfs * newInfo.Ch[IPL_IMG_YUV_Y].Height);
    newUVSize = (newInfo.Ch[IPL_IMG_YUV_UV].LineOfs * newInfo.Ch[IPL_IMG_YUV_UV].Height);


    if ((Info->ImgSizeH < (Info->CropStartX + Info->CropSizeH)) || (Info->ImgSizeV < (Info->CropStartY + Info->CropSizeV)))
    {
        Crop_OverFlow = TRUE;
    }

    if ((orgYSize >= newYSize) && (orgUVSize >= newUVSize) && Crop_OverFlow == FALSE)
    {
        return TRUE;
    }

    DBG_ERR("Id: %d, Path: %d, buffer overflow Y(org 0x%.8x < new 0x%.8x)\r\n", Id, PathId, orgYSize, newYSize);
    DBG_ERR("Id: %d, Path: %d, buffer overflow UV(org 0x%.8x < new 0x%.8x)\r\n", Id, PathId, orgUVSize, newUVSize);
    DBG_ERR("Crop OverFlow Check Scale Out Size:(%d, %d), Crop Start:(%d, %d), Crop Size:(%d, %d)\r\n",
        Info->ImgSizeH, Info->ImgSizeV, Info->CropStartX, Info->CropStartY, Info->CropSizeH, Info->CropSizeV);

    return FALSE;
}

void IPL_SetImePathInfo(UINT32 Id, IPL_IME_PATH PathId, IPL_IME_PATH_INFO *Info)
{
    IPL_YCC_CH_INFO SizeInfo = {0};

    if (Info == NULL)
    {
        DBG_ERR("Id: %d, input fail\r\n", Id);
        return;
    }

    switch (PathId)
    {
	    //#NT#2017/02/06#Jarkko Chang -begin
    	//#NT# add support side by side left/right, up/down case
        case IPL_IME_PATH1:
            if (Info->OutputEn == ENABLE)
            {
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_H, Info->ImgSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_V, Info->ImgSizeV);
				if(IPL_CtrlGetInfor(Id, IPLCTRL_SBS_TYPE) == SEL_SBS_LR)
				{
                	IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_HLOS, (Info->ImgSizeLOfs << 1));
				}
				else
				{
                	IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_HLOS, Info->ImgSizeLOfs);
				}
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_FMT, Info->ImgFmt);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_STR_X, Info->CropStartX);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_STR_Y, Info->CropStartY);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_H, Info->CropSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_V, Info->CropSizeV);

                SizeInfo.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_H);
                SizeInfo.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_V);
                IPL_SetDeviceInputInfo(0, SizeInfo, Id);
            }
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_EN, Info->OutputEn);
            break;

        case IPL_IME_PATH2:
            if (Info->OutputEn == ENABLE)
            {
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_H, Info->ImgSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_V, Info->ImgSizeV);
				if(IPL_CtrlGetInfor(Id, IPLCTRL_SBS_TYPE) == SEL_SBS_LR)
				{
                	IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_HLOS, (Info->ImgSizeLOfs << 1));
				}
				else
				{
                	IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_SIZE_HLOS, Info->ImgSizeLOfs);
				}
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_FMT, Info->ImgFmt);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_STR_X, Info->CropStartX);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_STR_Y, Info->CropStartY);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_H, Info->CropSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_V, Info->CropSizeV);

                SizeInfo.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_H);
                SizeInfo.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P2_OUT_CROP_V);
                IPL_SetDeviceInputInfo(1, SizeInfo, Id);
            }
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P2_EN, Info->OutputEn);
            break;

        case IPL_IME_PATH3:
            if (Info->OutputEn == ENABLE)
            {
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_H, Info->ImgSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_V, Info->ImgSizeV);
				if(IPL_CtrlGetInfor(Id, IPLCTRL_SBS_TYPE) == SEL_SBS_LR)
				{
                	IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_HLOS, (Info->ImgSizeLOfs << 1));
				}
				else
				{
                	IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_SIZE_HLOS, Info->ImgSizeLOfs);
				}
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_FMT, Info->ImgFmt);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_STR_X, Info->CropStartX);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_STR_Y, Info->CropStartY);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_H, Info->CropSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_V, Info->CropSizeV);

                SizeInfo.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_H);
                SizeInfo.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P3_OUT_CROP_V);
                IPL_SetDeviceInputInfo(2, SizeInfo, Id);
            }
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P3_EN, Info->OutputEn);
            break;
   	    //#NT#2017/02/06#Jarkko Chang -end
        case IPL_IME_PATH4:
            if (Info->OutputEn == ENABLE)
            {
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_SIZE_H, Info->ImgSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_SIZE_V, Info->ImgSizeV);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_SIZE_HLOS, Info->ImgSizeLOfs);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_FMT, Info->ImgFmt);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_STR_X, Info->CropStartX);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_STR_Y, Info->CropStartY);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_H, Info->CropSizeH);
                IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_V, Info->CropSizeV);

                SizeInfo.Width = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_H);
                SizeInfo.Height = IPL_CtrlGetInfor(Id, IPLCTRL_IME_P4_OUT_CROP_V);
                IPL_SetDeviceInputInfo(3, SizeInfo, Id);
            }
            IPL_CtrlSetInfor(Id, IPLCTRL_IME_P4_EN, Info->OutputEn);
            break;

        default:
            DBG_ERR("Id: %d, path id fail %d\r\n", Id, PathId);
            break;
    }
}

void IPL_CtrlPrvSetDZoomInfo(UINT32 Id)
{
    SENSOR_INFO Info = {0};
    UINT32 Width = 0, Height = 0;
    UINT32 DzIdx;
    ER rt = E_OK;

    rt = IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &Info);
    if (rt != E_OK)
        return;

    //SHDR case
    if (Id == IPL_ID_2 && (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR))
    {
        DzIdx = IPL_DzoomGetIndex(IPL_ID_1);
    }
    else
    {
        DzIdx = IPL_DzoomGetIndex(Id);
    }

    //SIE crop
    IPL_DzoomGetSIECropSize(Id, DzIdx, FALSE, &Width, &Height);

	//#NT#2016/11/02#Silvia Wu -begin
    //#NT# fixed ccir align 8
	if (Info.Mode == NULL)
		return ;

	if (Info.Mode->ModeType == SENSOR_MODE_CCIR || Info.Mode->ModeType == SENSOR_MODE_CCIR_INTERLACE)
	{
		Width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio.RatioHV), Width, 8);
	}
	else
	{
		Width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio.RatioHV), Width, 4);
	}
    //Width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio.RatioHV), Width, 8);//4);
	//#NT#2016/11/02#Silvia Wu -end

    Height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio.RatioHV), Height, 4);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V, Height);

    //SIE ch0
    IPL_DzoomGetSIEOutSize(Id, DzIdx, &Width, &Height);

    Width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio.RatioHV), Width, 4);
    Height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio.RatioHV), Height, 4);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V, Height);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT, IPL_UTI_CONV_ALGINFO_RAW_BIT(IPL_AlgGetUIInfo(Id, IPL_SEL_RAW_BITDEPTH_PRV)));
    if (IPL_CtrlGetInfor(Id, IPLCTRL_CFMODE) == IPL_CF_2STRP)
        IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(Width, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT)));
    else
    {
        if (IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAWENC_EN) == ENABLE)
            IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS, IPL_ALIGN_ROUNDUP(IPL_UTI_RAWH_CONV2_LOFS(Width, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT))*RAW_ENC_BUF_RATIO/100, 4));
        else
            IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(Width, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT)));
    }

    //#NT#2016/02/02#Jarkko Chang -begin
    //#NT# fixed ccir_interlance sensor output size error
    if (IPL_CtrlGetInfor(Id, IPLCTRL_CFMODEEXT) == IPL_CFEXT_DEINTERLACE)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_H, Width);
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_V, Height*2);
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_STR_X, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_STR_Y, 0);
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_H, Width);
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_CROP_V, Height*2);
        IPL_CtrlSetInfor(Id, IPLCTRL_IME_P1_OUT_SIZE_HLOS, Width);
    }
    //#NT#2016/02/02#Jarkko Chang -end

    //SIE ch3
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_RAW_SIZE_H, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_PRVBIT, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_RAW_SIZE_HLOS, 0);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_RAW_SIZE_V, 0);

    //Set IPL IN for RDE/IFE IN
    IPL_DzoomGetRaw2YUVInSize(Id, DzIdx, &Width, &Height);

    Width  = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio.RatioHV), Width, 4);
    Height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_PRVCROPRATIO), IPL_UTI_CONV_SEN_RATIO(Info.Mode->Ratio.RatioHV), Height, 4);

    IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_BIT, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_PRVBIT));

    //#NT#2016/04/26#Jarkko Chang -begin
    //#NT# fixed RSC ratio error when ADAS on
    if(RSC_OnOffInfor(Id) == SEL_RSC_ON)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_SIZE_H, IPL_UTI_EISDIS_ADJ_HSIZE(Width, RSC_GetEISRatio(Id, IPL_MODE_NONE, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE)), 4));
        IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_SIZE_V, IPL_UTI_EISDIS_ADJ_VSIZE(Height,RSC_GetEISRatio(Id, IPL_MODE_NONE, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE)), 4));
    }
    else if (IPL_CtrlGetInfor(Id, IPLCTRL_DIS_EN) == ENABLE)
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_SIZE_H, IPL_UTI_EISDIS_ADJ_HSIZE(Width, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 4));
        IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_SIZE_V, IPL_UTI_EISDIS_ADJ_VSIZE(Height, IPL_CtrlGetInfor(Id, IPLCTRL_DIS_RATIO), 4));
    }
    else
    {
        IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_SIZE_H, Width);
        IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_SIZE_V, Height);
    }
    //#NT#2016/04/26#Jarkko Chang -end

    IPL_CtrlSetInfor(Id, IPLCTRL_IPL_IN_SIZE_HLOS,IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS));
}

void IPL_CtrlPrvGetDZoomInfo(UINT32 Id, IPC_Dzoom_Info *Info)
{
    SENSOR_INFO SenInfo = {0};
    ER rt = E_OK;

    //initial sensor parameter
    rt = IPL_GetSensorStatus(Id, IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (rt != E_OK)
        return;

    Info->Id = Id;
    Info->Idx = IPL_CtrlGetHalSetId(Id);
    //#NT#2016/09/14#liwei cheng -begin
    //#NT# add surpport up and left crop offset
    IPL_CheckCropOfsBound(Id, &Info->sieCropHini,&Info->sieCropVini,SenInfo.Mode->TransHD->DataSize,SenInfo.Mode->TransVD->DataSize,IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H),IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V));
    //#NT#2016/09/14#liwei cheng -end
    Info->sieCropH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_H);
    Info->sieCropV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_PRVCROP_SIZE_V);
    Info->sieOutH = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_H);
    Info->sieOutV = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_V);
    Info->sieOutLineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS);
    Info->iplCropX = ((Info->sieOutH - IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_H)) >> 1);
    Info->iplCropY = ((Info->sieOutV - IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_V)) >> 1);
    Info->iplInH = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_H);
    Info->iplInV = IPL_CtrlGetInfor(Id, IPLCTRL_IPL_IN_SIZE_V);
    Info->iplInLineOfs = IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH0_RAW_SIZE_HLOS);
    Info->bChgCAParam = (BOOL)IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CA_EN);
    Info->bChgLAParam = (BOOL)IPL_CtrlGetInfor(Id, IPLCTRL_SIE_LA_EN);
}

#if 0
#endif
void IPL_CtrlInitCapBufInfo(UINT32 Id, IPL_CHGMODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *BufInfo)
{
    BufInfo->Id = ChgMode->ProcSenId;
}

void IPL_CtrlInitPrvBufInfo(IPL_CHGMODE_DATA *ChgMode, IPLBUF_ENG2DRAMINFO *BufInfo)
{
    switch (IPL_CtrlGetInfor(ChgMode->ProcSenId, IPLCTRL_PRVFLOW))
    {
    case ICF_FLOW_A:
        IPL_CtrlInitBufInfoFlowA(ChgMode->ProcSenId, BufInfo);
        break;

    case ICF_FLOW_C:
        IPL_CtrlInitBufInfoFlowC(ChgMode->ProcSenId, BufInfo);
        break;

    case ICF_FLOW_DEWARP:
        IPL_CtrlInitBufInfoFlowD(ChgMode->ProcSenId, BufInfo);
        break;

    default:
        DBG_ERR("Id: %d, unsupport IPL flow %d\r\n", ChgMode->ProcSenId,IPL_CtrlGetInfor(ChgMode->ProcSenId, IPLCTRL_PRVFLOW));
        break;
    }

    BufInfo->Id = ChgMode->ProcSenId;
}

#if 0
#endif
void IPL_CtrlCapInit(UINT32 Id, UINT32 EventId, IPLCTRL_INFO *CtrlInfo)
{
    UINT32 Width = 0, Height = 0, BitNum = 0;

    switch (EventId)
    {
        case IPL_FLOW_NOR_GETRAW:
            IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_NORMAL);
            break;

        case IPL_FLOW_HDR_GETRAW:
            IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_HDR);
            break;

        case IPL_FLOW_VID_GETRAW:
            IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_INVID);
            break;

        case IPL_FLOW_HS_GETRAW:
            IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_HS);
            break;

        case IPL_FLOW_3D_GETRAW:
            IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_3D);
            break;

        case IPL_FLOW_EB_GETRAW:
            IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_EBURST);
            break;

        case IPL_FLOW_AEB_GETRAW:
            IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_AEB);
            break;

        case IPL_FLOW_AWB_GETRAW:
            IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_AWB);
            break;

        case IPL_FLOW_MFHDR_GETRAW:
            IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_MFHDR);
            break;

        case IPL_FLOW_MFANTISHAKE_GETRAW:
            IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_MFANTISHAKE);
            break;
//#NT#2016/03/30#Harry Tsai -begin
//#NT# Add side by side capture mode
        case IPL_FLOW_DUALSBS_GETRAW:
            IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_DUALSBS);
            break;
//#NT#2016/03/30#Harry Tsai -end

        default:
            IPL_CtrlSetInfor(Id, IPLCTRL_CAPFLOW, IPLCTRL_CAP_FLOW_NONE);
            DBG_ERR("Id: %d, unsupport EventId flow %d\r\n", Id,EventId);
            break;
    }

    IPL_CtrlSetInfor(Id, IPLCTRL_SEN_CAPCROPRATIO, CtrlInfo->ChgMode->CropRatio);

    IPL_CtrlSetInfor(Id, IPLCTRL_CAPAUTO_OB_EN, DISABLE);
    IPL_CtrlSetInfor(Id, IPLCTRL_CAPDF_EN, DISABLE);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_CAP_SUBRATIO, IQS_GetChromaAdaptRatio(Id, TRUE, FALSE));
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CAPSTPIX, IPL_UTI_CONV_SEN_STPIX(CtrlInfo->Info->Mode->StPix));
    IPL_CtrlSetInfor(Id, IPLCTRL_IPE_CAP_SUBRATIO, 100);

    if (IQS_IsChromaAdaptEn(Id, TRUE) == ENABLE)
        IPL_CtrlSetInfor(Id, IPLCTRL_IFE2_LCA_EN, ENABLE);
    else
        IPL_CtrlSetInfor(Id, IPLCTRL_IFE2_LCA_EN, DISABLE);

    if(IQS_GetMSNRRepTime(Id) > 0)
        IPL_CtrlSetInfor(Id, IPLCTRL_MSNR_CAP_EN, ENABLE);
    else
        IPL_CtrlSetInfor(Id, IPLCTRL_MSNR_CAP_EN, DISABLE);

    IPL_DzoomGetSIECropSize(Id, IPL_DzoomGetIndex(Id), TRUE, &Width, &Height);

    //#NT#2016/04/11#Harry Tsai -begin
    //#NT# Modify width to be 8x to let 12bit width = lineoffset. Or it will have wrong image with SIE mirror.
    Width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_CAPCROPRATIO), IPL_UTI_CONV_SEN_RATIO(CtrlInfo->Info->Mode->Ratio.RatioHV), Width, 8);
    //#NT#2016/04/11#Harry Tsai -end
    Height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_CAPCROPRATIO), IPL_UTI_CONV_SEN_RATIO(CtrlInfo->Info->Mode->Ratio.RatioHV), Height, 4);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CAPCROP_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CAPCROP_SIZE_V, Height);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_RAW_BUFNUM, 1);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H, Width);

    if (EventId == IPL_FLOW_EB_GETRAW)
    {
        BitNum = IPL_RAW_BIT_12;
    }
    else
    {
        BitNum = IPL_UTI_CONV_ALGINFO_RAW_BIT(IPL_AlgGetUIInfo(Id, IPL_SEL_RAW_BITDEPTH_CAP));
    }

	//#NT#2016/09/30#Silvia Wu -begin
    //#NT# fixed ccir cap issue
    if (IPL_CtrlGetInfor(Id, IPLCTRL_CFMODE) == IPL_CF_CCIR)
    {
		//#NT#2017/04/12#Bowen Li -begin
		//#NT# fix CCIR cap crash issue
		UINT32 r_ofs = 0;
		SIE_DVI dvi = {0};

		IPL_SIECCIR_FCB_Cap(Id, &dvi);

		if (dvi.SIE_DVIMode == SIE_SDMode)
		{
			r_ofs = 1;
		}
	    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H, (Width >> r_ofs));
	    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS, (Width >> r_ofs));
		//#NT#2017/04/12#Bowen Li -end
    }
	else
	{
	    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_H, Width);
	    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(Width, BitNum));
	}
    //#NT#2016/09/30#Silvia Wu -end

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAPBIT, BitNum);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH0_CAP_RAW_SIZE_V, Height);

    IPL_DzoomGetSIECropSize(Id, IPL_DzoomGetIndex(Id), TRUE, &Width, &Height);
    Width = IPL_UTI_SIESUB_ADJ_HSIZE(Width, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH3_CAP_SUBRATIO), 4);
    Height = IPL_UTI_SIESUB_ADJ_VSIZE(Height, IPL_CtrlGetInfor(Id, IPLCTRL_SIE_CH3_CAP_SUBRATIO), 4);
    //#NT#2016/04/11#Harry Tsai -begin
    //#NT# Modify width to be 8x to let 12bit width = lineoffset. Or it will have wrong image with SIE mirror.
    Width = IPL_UTI_SENCROPRATIO_ADJ_HSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_CAPCROPRATIO), IPL_UTI_CONV_SEN_RATIO(CtrlInfo->Info->Mode->Ratio.RatioHV), Width, 8);
    //#NT#2016/04/11#Harry Tsai -end
    Height = IPL_UTI_SENCROPRATIO_ADJ_VSIZE(IPL_CtrlGetInfor(Id, IPLCTRL_SEN_CAPCROPRATIO), IPL_UTI_CONV_SEN_RATIO(CtrlInfo->Info->Mode->Ratio.RatioHV), Height, 4);

    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_RAW_BUFNUM, 1);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_CAP_RAW_SIZE_H, Width);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_CAPBIT, BitNum);
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_CAP_RAW_SIZE_HLOS, IPL_UTI_RAWH_CONV2_LOFS(Width, BitNum));
    IPL_CtrlSetInfor(Id, IPLCTRL_SIE_CH3_CAP_RAW_SIZE_V, Height);
    IPL_CtrlSetInfor(Id, IPLCTRL_SENSOR_TYPE, CtrlInfo->Info->Mode->ModeType);

}

void IPL_CtrlPrvInit(UINT32 Id, IPLCTRL_INFO *CtrlInfo)
{
    switch (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW))
    {
        case ICF_FLOW_A:
            IPL_CtrlPrvInitFlowA(Id, CtrlInfo);
            break;

        case ICF_FLOW_C:
            IPL_CtrlPrvInitFlowC(Id, CtrlInfo);
            break;

        case ICF_FLOW_DEWARP:
            IPL_CtrlPrvInitFlowD(Id, CtrlInfo);
            break;

        default:
            DBG_ERR("Id: %d, unsupport IPL flow %d\r\n", Id, IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW));
            break;
    }
}

#if 0
#endif

IPL_HAL_PARASET IPL_CtrlGetHalSetId(UINT32 Id)
{

    IPL_MODE Mode;

    Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

    if ((IPLCTRL_CAP_FLOW_INVID == IPL_CtrlGetInfor(Id, IPLCTRL_CAPFLOW)) && (Mode == IPL_MODE_CAP))
    {
        switch (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW))
        {
            case ICF_FLOW_A:
                return IPL_CtrlGetHalSetIdFlowA(Id);

            case ICF_FLOW_C:
                return IPL_CtrlGetHalSetIdFlowC(Id);

            case ICF_FLOW_DEWARP:
                return IPL_CtrlGetHalSetIdFlowD(Id);

            default:
                DBG_ERR("Id: %d, unsupport IPL flow %d\r\n", Id, IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW));
                return IPL_HAL_PAR1;
        }
    }
    else
    {
        if (Mode == IPL_MODE_CAP)
        {
            return IPL_HAL_PAR1;
        }
        else
        {

            switch (IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW))
            {
                case ICF_FLOW_A:
                    return IPL_CtrlGetHalSetIdFlowA(Id);

                case ICF_FLOW_C:
                    return IPL_CtrlGetHalSetIdFlowC(Id);

                case ICF_FLOW_DEWARP:
                    return IPL_CtrlGetHalSetIdFlowD(Id);

                default:
                    DBG_ERR("Id: %d, unsupport IPL flow %d\r\n", Id, IPL_CtrlGetInfor(Id, IPLCTRL_PRVFLOW));
                    return IPL_HAL_PAR1;
            }
        }
    }
}
//#NT#2016/10/03#Harry Tsai -begin
//#NT# Added for Eth On/Off
void IPL_CtrlSIExSetEthEn(UINT32 Id, UINT32 EnFlag)
{
    SIE Sie = {0};
    SIE_SUBFUNC sieSubf = {0};
    IPL_HAL_READ    readBit;
    IPL_HAL_GROUP   group = {0};
    IPL_HAL_PARASET idx;

    idx = IPL_CtrlGetHalSetId(Id);

    Sie.pSubFunc = &sieSubf;
    //read previous function enable
    readBit.sieRead = SIE_R_SUBFEN;
    SIEx_ENG_Read(Id, IPL_CtrlGetHalSetId(Id), readBit, (void*)&sieSubf.sieFuncEn);

    if (EnFlag == ENABLE)
        sieSubf.sieFuncEn |= EthEn;
    else
        sieSubf.sieFuncEn &= ~EthEn;
    group.sieUpdate |= SIE_SUBFEN;
    SIEx_ENG_ChgParam(Id, idx, group, &Sie);
    SIEx_ENG_Load(Id, idx, group);
}
//#NT#2016/10/03#Harry Tsai -end
