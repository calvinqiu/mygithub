/**
    Ctrl sample.

    @file       IPL_CtrlSample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "IPL_IMX291M_IPCAM_FF_Int.h"
#if (CAL_DP|CAL_LENSCEN)
#include "ipl_cal_datapassing_IMX291M_IPCAM_FF_int.h"
#endif
//#NT#2016/09/14#liwei cheng -begin
//#NT# add surpport liveview negative crop
//#NT#2016/07/11#Jarkko Chang -begin
//#NT# add support set crop offset
INT32 Crop_Ofs_X[IPL_ID_MAX_NUM] = {0, 0, 0, 0}, Crop_Ofs_Y[IPL_ID_MAX_NUM] = {0, 0, 0, 0};
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
