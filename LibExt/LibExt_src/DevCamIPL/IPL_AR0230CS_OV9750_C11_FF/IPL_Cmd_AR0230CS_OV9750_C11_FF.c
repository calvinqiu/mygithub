/*
    IPL command API

    This file is the API of the IPL control.

    @file       IPL_CmdSample.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include "IPL_AR0230CS_OV9750_C11_FF_Int.h"
//note: do not delete
//avoid compiler optimize "weak function"
//WEAK_DUMMY_CALL(CMD, CTRL1)

IPL_CMD_CHGMODE_FP IPL_ChgModeFCB(IPL_MODE CurMode, IPL_MODE NextMode)
{
    return IPL_GetModeFp(CurMode, NextMode);
}

IPL_IME_PATH IPL_SetImeFCB(IPL_IME_DATA *Info, IPC_IMEOut_Info *IpcInfoP1, IPC_IMEOut_Info *IpcInfoP2, IPC_IMEOut_Info *IpcInfoP3, IPC_IMEOut_Info *IpcInfoP4)
{
    IPL_IME_PATH rt = IPL_IME_PATH_NONE;
    UINT32 BufAddr = 0, Size = 0;
    IPL_IME_PATH_INFO tmpImeInfo = {0};
    IPL_YCC_CH_INFO tmpInfo = {0};

    IPL_BufGet(Info->Id, IPLBUF_IMEP1_1, &BufAddr, &Size);
    if ((Info->PathId & IPL_IME_PATH1) && (Size != 0))
    {
        //p1
        if (IPL_ChkImePathInfo(Info->Id, IPL_IME_PATH1, &Info->ImeP1) == TRUE)
        {
            IpcInfoP1->Idx = IPL_CtrlGetHalSetId(Info->Id);

            IPL_SetImePathInfo(Info->Id, IPL_IME_PATH1, &Info->ImeP1);
            IPL_GetImePathInfo(Info->Id, IPL_IME_PATH1, &tmpImeInfo);
            IpcInfoP1->Path.PathEn = tmpImeInfo.OutputEn;
            IpcInfoP1->Path.DmaOutEn = Info->ImeP1.DmaOutEn;
            IpcInfoP1->Path.Size.Y.Width = tmpImeInfo.ImgSizeH;
            IpcInfoP1->Path.Size.Y.Height = tmpImeInfo.ImgSizeV;
            IpcInfoP1->Path.Size.Y.LineOfs = tmpImeInfo.ImgSizeLOfs;
            IpcInfoP1->Path.Fmt = tmpImeInfo.ImgFmt;
            IpcInfoP1->Path.Crop.Start.x = tmpImeInfo.CropStartX;
            IpcInfoP1->Path.Crop.Start.y = tmpImeInfo.CropStartY;
            IpcInfoP1->Path.Crop.SizeH = tmpImeInfo.CropSizeH;
            IpcInfoP1->Path.Crop.SizeV = tmpImeInfo.CropSizeV;
            tmpInfo = IPL_UTI_Y_INFO_CONV2(IpcInfoP1->Path.Fmt, IpcInfoP1->Path.Size.Y);
            IpcInfoP1->Path.Size.Lineoffset_CC = tmpInfo.LineOfs;
            rt |= IPL_IME_PATH1;
        }
        else
        {
            DBG_IND("Id: %d, IPL_ChkImePathInfo IPL_IME_PATH1 fail\r\n", Info->Id);
        }
    }

    IPL_BufGet(Info->Id, IPLBUF_IMEP2_1, &BufAddr, &Size);
    if ((Info->PathId & IPL_IME_PATH2) && (Size != 0))
    {
        //p2
        if (IPL_ChkImePathInfo(Info->Id, IPL_IME_PATH2, &Info->ImeP2) == TRUE)
        {
            IpcInfoP2->Idx = IPL_CtrlGetHalSetId(Info->Id);

            IPL_SetImePathInfo(Info->Id, IPL_IME_PATH2, &Info->ImeP2);
            IPL_GetImePathInfo(Info->Id, IPL_IME_PATH2, &tmpImeInfo);
            IpcInfoP2->Path.PathEn = TRUE;    //Path2 Always Set Path Enable
            IpcInfoP2->Path.DmaOutEn = tmpImeInfo.OutputEn;
            IpcInfoP2->Path.Size.Y.Width = tmpImeInfo.ImgSizeH;
            IpcInfoP2->Path.Size.Y.Height = tmpImeInfo.ImgSizeV;
            IpcInfoP2->Path.Size.Y.LineOfs = tmpImeInfo.ImgSizeLOfs;
            IpcInfoP2->Path.Fmt = tmpImeInfo.ImgFmt;
            IpcInfoP2->Path.Crop.Start.x = tmpImeInfo.CropStartX;
            IpcInfoP2->Path.Crop.Start.y = tmpImeInfo.CropStartY;
            IpcInfoP2->Path.Crop.SizeH = tmpImeInfo.CropSizeH;
            IpcInfoP2->Path.Crop.SizeV = tmpImeInfo.CropSizeV;
            tmpInfo = IPL_UTI_Y_INFO_CONV2(IpcInfoP2->Path.Fmt, IpcInfoP2->Path.Size.Y);
            IpcInfoP2->Path.Size.Lineoffset_CC = tmpInfo.LineOfs;
            rt |= IPL_IME_PATH2;
        }
        else
        {
            DBG_IND("Id: %d, IPL_ChkImePathInfo IPL_IME_PATH2 fail\r\n", Info->Id);
        }
    }

    IPL_BufGet(Info->Id, IPLBUF_IMEP3_1, &BufAddr, &Size);
    if ((Info->PathId & IPL_IME_PATH3) && (Size != 0))
    {
        //p3
        if (IPL_ChkImePathInfo(Info->Id, IPL_IME_PATH3, &Info->ImeP3) == TRUE)
        {
            IpcInfoP3->Idx = IPL_CtrlGetHalSetId(Info->Id);

            IPL_SetImePathInfo(Info->Id, IPL_IME_PATH3, &Info->ImeP3);
            IPL_GetImePathInfo(Info->Id, IPL_IME_PATH3, &tmpImeInfo);
            IpcInfoP3->Path.PathEn = tmpImeInfo.OutputEn;
            IpcInfoP3->Path.DmaOutEn = tmpImeInfo.OutputEn;
            IpcInfoP3->Path.Size.Y.Width = tmpImeInfo.ImgSizeH;
            IpcInfoP3->Path.Size.Y.Height = tmpImeInfo.ImgSizeV;
            IpcInfoP3->Path.Size.Y.LineOfs = tmpImeInfo.ImgSizeLOfs;
            IpcInfoP3->Path.Fmt = tmpImeInfo.ImgFmt;
            IpcInfoP3->Path.Crop.Start.x = tmpImeInfo.CropStartX;
            IpcInfoP3->Path.Crop.Start.y = tmpImeInfo.CropStartY;
            IpcInfoP3->Path.Crop.SizeH = tmpImeInfo.CropSizeH;
            IpcInfoP3->Path.Crop.SizeV = tmpImeInfo.CropSizeV;
            tmpInfo = IPL_UTI_Y_INFO_CONV2(IpcInfoP3->Path.Fmt, IpcInfoP3->Path.Size.Y);
            IpcInfoP3->Path.Size.Lineoffset_CC = tmpInfo.LineOfs;
            rt |= IPL_IME_PATH3;
        }
        else
        {
            DBG_IND("Id: %d, IPL_ChkImePathInfo IPL_IME_PATH3 fail\r\n", Info->Id);
        }
    }

    IPL_BufGet(Info->Id, IPLBUF_IMEP4_1, &BufAddr, &Size);
    if ((Info->PathId & IPL_IME_PATH4) && (Size != 0))
    {
        //p4
        if (IPL_ChkImePathInfo(Info->Id, IPL_IME_PATH4, &Info->ImeP4) == TRUE)
        {
            IpcInfoP4->Idx = IPL_CtrlGetHalSetId(Info->Id);

            IPL_SetImePathInfo(Info->Id, IPL_IME_PATH4, &Info->ImeP4);
            IPL_GetImePathInfo(Info->Id, IPL_IME_PATH4, &tmpImeInfo);
            IpcInfoP4->Path.PathEn = tmpImeInfo.OutputEn;
            IpcInfoP4->Path.DmaOutEn = tmpImeInfo.OutputEn;
            IpcInfoP4->Path.Size.Y.Width = tmpImeInfo.ImgSizeH;
            IpcInfoP4->Path.Size.Y.Height = tmpImeInfo.ImgSizeV;
            IpcInfoP4->Path.Size.Y.LineOfs = tmpImeInfo.ImgSizeLOfs;
            IpcInfoP4->Path.Fmt = tmpImeInfo.ImgFmt;
            IpcInfoP4->Path.Crop.Start.x = tmpImeInfo.CropStartX;
            IpcInfoP4->Path.Crop.Start.y = tmpImeInfo.CropStartY;
            IpcInfoP4->Path.Crop.SizeH = tmpImeInfo.CropSizeH;
            IpcInfoP4->Path.Crop.SizeV = tmpImeInfo.CropSizeV;
            tmpInfo = IPL_UTI_Y_INFO_CONV2(IpcInfoP4->Path.Fmt, IpcInfoP4->Path.Size.Y);
            IpcInfoP4->Path.Size.Lineoffset_CC = tmpInfo.LineOfs;
            rt |= IPL_IME_PATH4;
        }
        else
        {
            DBG_IND("Id: %d, IPL_ChkImePathInfo IPL_IME_PATH4 fail\r\n", Info->Id);
        }
    }
    return rt;
}

void IPL_SetDZoomFCB(IPL_DZOOM *Info, IPC_FRONTEND *SetInfo)
{
    IPL_CtrlPrvSetDZoomInfo(Info->Id);
    IPL_CtrlPrvGetDZoomInfo(Info->Id, SetInfo);

    //SHDR flow need update Crop information for both IPL_ID_1 and IPL_ID_2
    if (Info->Id == IPL_ID_1 && (IPL_CtrlGetInfor(Info->Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR))
    {
        IPL_CtrlPrvSetDZoomInfo(IPL_ID_2);
    }
}

//#NT#2016/12/23#Jarkko#[0114861] Chang -begin
//#NT# fixed dual sensor sleep/wakeup fail issue
SENSOR_MODE gSenCurMode[IPL_ID_MAX_NUM] = {SENSOR_MODE_UNKNOWN};

void IPL_SetSleepFCB(IPL_SLEEP_INFO *Info)
{
    SENSOR_INFO SenInfo = {0};
    ER rt = E_OK;
    IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};

    rt = IPL_GetSensorStatus(Info->Id, IPL_AlgGetUIInfo(Info->Id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (rt != E_OK)
        return;

    if (SenInfo.Mode->ModeType != SENSOR_MODE_CCIR && SenInfo.Mode->ModeType != SENSOR_MODE_CCIR_INTERLACE)
    {
	    AE_Pause(IPL_UTI_CONV2_AE_ID(Info->Id), ENABLE);
    	AWB_Pause(IPL_UTI_CONV2_AWB_ID(Info->Id), ENABLE);
    }

    IPC_WaitVD2(Info->Id,TRUE, IPL_TIMEOUT_MS);
    ProcCmd[Info->Id] = IPC_PAUSE_SIE;

    if ((Info->Id == IPL_ID_1) && (IPL_CtrlGetInfor(Info->Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR))
    {
        ProcCmd[IPL_ID_2] = IPC_PAUSE_SIE;
    }

    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);

    IPC_WaitVD2(Info->Id,FALSE, IPL_TIMEOUT_MS);
    Sensor_GetCurMode(IPL_UTI_CONV2_SEN_ID(Info->Id), &gSenCurMode[Info->Id]);
    Sensor_Close(IPL_UTI_CONV2_SEN_ID(Info->Id));

    DrvSensor_PowerSaveOn(IPL_UTI_CONV2_SEN_ID(Info->Id));

}

void IPL_SetWakeupFCB(IPL_WAKEUP_INFO *Info)
{
    SENSOR_INFO SenInfo = {0};
    ER rt = E_OK;
    SENSOR_INIT_OBJ SenObj;
    SENSOR_DRVTAB *SenTab;
    IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};

    //open sensor
    DrvSensor_PowerSaveOff(IPL_UTI_CONV2_SEN_ID(Info->Id));
    SenObj = DrvSensor_GetObj(IPL_UTI_CONV2_SEN_ID(Info->Id));
    SenTab = DrvSensor_GetTab(IPL_UTI_CONV2_SEN_ID(Info->Id));
    Sensor_Open(IPL_UTI_CONV2_SEN_ID(Info->Id), &SenObj, SenTab);

    Sensor_ChgMode(IPL_UTI_CONV2_SEN_ID(Info->Id), gSenCurMode[Info->Id]);

    IPL_ResetDisplayCtrlFlow(Info->Id);
    IPC_WaitVD2(Info->Id,TRUE, IPL_TIMEOUT_MS);

    ProcCmd[Info->Id] = IPC_RESUME_SIE;

    if ((Info->Id == IPL_ID_1) && (IPL_CtrlGetInfor(Info->Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR))
    {
        ProcCmd[IPL_ID_2] = IPC_RESUME_SIE;
    }

    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
    IPC_WaitVD2(Info->Id,FALSE, IPL_TIMEOUT_MS);

    rt = IPL_GetSensorStatus(Info->Id, IPL_AlgGetUIInfo(Info->Id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (rt != E_OK)
        return;

    if (SenInfo.Mode->ModeType != SENSOR_MODE_CCIR && SenInfo.Mode->ModeType != SENSOR_MODE_CCIR_INTERLACE)
    {
        AE_Resume(IPL_UTI_CONV2_AE_ID(Info->Id));
        AWB_Resume(IPL_UTI_CONV2_AWB_ID(Info->Id));
	}
}
//#NT#2016/12/23#Jarkko#[0114861] Chang -end

void IPL_GetCapRawFCB(IPL_GET_CAP_RAW_DATA *Info)
{
    IPC_SIE_Info SIEInfo = {0};
    SIE sie = {0};
    SIE_SUBFUNC SubFunc = {0};

    IPLCTRL_CAP_EXT_INFO ExtInfo = {0};

    //prepare ipc sie information
    SIEInfo.Idx = IPL_CtrlGetHalSetId(Info->Id);
    SIEInfo.psie = &sie;
    SIEInfo.psie->pSubFunc = &SubFunc;

    if (IPLCTRL_CAP_FLOW_HDR == IPL_CtrlGetInfor(Info->Id, IPLCTRL_CAPFLOW))
    {
        IPL_CtrlInitCapIpcInfo(Info, &SIEInfo, &ExtInfo);
        IPL_FCB_GetCapRaw(Info->Id, &SIEInfo, &ExtInfo);
    }
    else if(IPLCTRL_CAP_FLOW_MFHDR == IPL_CtrlGetInfor(Info->Id, IPLCTRL_CAPFLOW))
    {
        //output to raw buffer 0
        ExtInfo.Param[0] = Info->BufAddrCh0;
        ExtInfo.Param[1] = Info->BufAddrCh3;
        IPL_CtrlInitCapIpcInfo(Info, &SIEInfo, &ExtInfo);
        IPL_FCB_GetCapRaw(Info->Id, &SIEInfo, &ExtInfo);

        //output to raw buffer 1
        ExtInfo.Param[0] = Info->BufAddrCh0_1;
        ExtInfo.Param[1] = Info->BufAddrCh3_1;
        IPL_CtrlInitCapIpcInfo(Info, &SIEInfo, &ExtInfo);
        IPL_FCB_GetCapRaw(Info->Id, &SIEInfo, &ExtInfo);
    }
    //#NT#2016/03/30#Harry Tsai -begin
    //#NT# Add side by side capture mode
    else if(IPLCTRL_CAP_FLOW_DUALSBS == IPL_CtrlGetInfor(Info->Id, IPLCTRL_CAPFLOW))
    {
        //output to raw buffer 0
        ExtInfo.Param[6] = Info->res[6];
        IPL_CtrlInitCapIpcInfo(Info, &SIEInfo, &ExtInfo);
        IPL_FCB_GetCapRaw(Info->Id, &SIEInfo, &ExtInfo);
    }
    //#NT#2016/03/30#Harry Tsai -end
    else
    {
        IPL_CtrlInitCapIpcInfo(Info, &SIEInfo, &ExtInfo);
        IPL_FCB_GetCapRaw(Info->Id, &SIEInfo, &ExtInfo);
    }

    //AWB setting
    IPL_FCB_SetCapAWB(Info->Id, &SIEInfo);
}

SIE_SUBFUNC gSieSubFunc[IPL_ID_MAX_NUM] = {0}; //for Pauase/Resume DMA
void IPL_SetPauseDMAFCB(IPL_PAUSEDMA_INFO *Info)
{
    SIE Sie = {0};
    SIE_SUBFUNC SieSubFunc = {0};
    IPL_HAL_GROUP SieGroup = {0};
    IPL_HAL_READ readBit = {0};
    IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};

    AE_Pause(IPL_UTI_CONV2_AE_ID(Info->Id), ENABLE);
    AWB_Pause(IPL_UTI_CONV2_AWB_ID(Info->Id), ENABLE);

    Sie.pSubFunc = &SieSubFunc;
    readBit.sieRead = SIE_R_SUBFEN;
    SIEx_ENG_Read(Info->Id, IPL_CtrlGetHalSetId(Info->Id), readBit, (void*)&Sie.pSubFunc->sieFuncEn);
    gSieSubFunc[Info->Id].sieFuncEn = Sie.pSubFunc->sieFuncEn;

    SieGroup.sieUpdate = SIE_SUBFEN;
    Sie.pSubFunc->sieFuncEn = 0;
    SIEx_ENG_ChgParam(Info->Id, IPL_CtrlGetHalSetId(Info->Id), SieGroup, &Sie);
    IPC_WaitVD2(Info->Id,TRUE, IPL_TIMEOUT_MS);
    SIEx_ENG_Load(Info->Id, IPL_CtrlGetHalSetId(Info->Id), SieGroup);

    ProcCmd[Info->Id] = IPC_PAUSE_SIE;

    if ((Info->Id == IPL_ID_1) && (IPL_CtrlGetInfor(Info->Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR))
    {
        Sie.pSubFunc = &SieSubFunc;
        readBit.sieRead = SIE_R_SUBFEN;
        SIEx_ENG_Read(IPL_ID_2, IPL_CtrlGetHalSetId(IPL_ID_2), readBit, (void*)&Sie.pSubFunc->sieFuncEn);
        gSieSubFunc[IPL_ID_2].sieFuncEn = Sie.pSubFunc->sieFuncEn;

        SieGroup.sieUpdate = SIE_SUBFEN;
        Sie.pSubFunc->sieFuncEn = 0;
        SIEx_ENG_ChgParam(IPL_ID_2, IPL_CtrlGetHalSetId(IPL_ID_2), SieGroup, &Sie);
        IPC_WaitVD2(IPL_ID_2,TRUE, IPL_TIMEOUT_MS);
        SIEx_ENG_Load(IPL_ID_2, IPL_CtrlGetHalSetId(IPL_ID_2), SieGroup);

        ProcCmd[IPL_ID_2] = IPC_PAUSE_SIE;
    }

    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
    IPC_WaitVD2(Info->Id,FALSE, IPL_TIMEOUT_MS);
}

void IPL_SetResumeDMAFCB(IPL_RESUMEDMA_INFO *Info)
{
    SIE Sie = {0};
    IPL_HAL_GROUP SieGroup = {0};
    IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};

    SieGroup.sieUpdate = SIE_SUBFEN;
    Sie.pSubFunc = &gSieSubFunc[Info->Id];
    IPL_ResetDisplayCtrlFlow(Info->Id);
    SIEx_ENG_ChgParam(Info->Id, IPL_CtrlGetHalSetId(Info->Id), SieGroup, &Sie);
    IPC_WaitVD2(Info->Id,TRUE, IPL_TIMEOUT_MS);
    SIEx_ENG_Load(Info->Id, IPL_CtrlGetHalSetId(Info->Id), SieGroup);

    ProcCmd[Info->Id] = IPC_RESUME_SIE;

    if ((Info->Id == IPL_ID_1) && (IPL_CtrlGetInfor(Info->Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR))
    {
        SieGroup.sieUpdate = SIE_SUBFEN;
        Sie.pSubFunc = &gSieSubFunc[IPL_ID_2];
        SIEx_ENG_ChgParam(IPL_ID_2, IPL_CtrlGetHalSetId(IPL_ID_2), SieGroup, &Sie);
        SIEx_ENG_Load(IPL_ID_2, IPL_CtrlGetHalSetId(IPL_ID_2), SieGroup);

        ProcCmd[IPL_ID_2] = IPC_RESUME_SIE;
    }

    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
    IPC_WaitVD2(Info->Id,FALSE, IPL_TIMEOUT_MS);

    AE_Resume(IPL_UTI_CONV2_AE_ID(Info->Id));
    AWB_Resume(IPL_UTI_CONV2_AWB_ID(Info->Id));
}

IPL_IME_PATH IPL_SetImgRatioFCB(IPL_SET_IMGRATIO_DATA *Info, IPC_FRONTEND *IpcInfo, IPC_IMEOut_Info *IpcInfoP1, IPC_IMEOut_Info *IpcInfoP2, IPC_IMEOut_Info *IpcInfoP3, IPC_IMEOut_Info *IpcInfoP4)
{
    IPL_IME_PATH rt = IPL_IME_PATH_NONE;
    UINT32 BufAddr = 0, Size = 0;
    IPL_IME_PATH_INFO tmpImeInfo = {0};
    IPL_YCC_CH_INFO tmpInfo = {0};

    if (Info->CropRatio != IPL_SENCROPRATIO_NONE)
    {
        IPL_CtrlSetInfor(Info->Id, IPLCTRL_SEN_PRVCROPRATIO, Info->CropRatio);
    }

    IPL_CtrlPrvSetDZoomInfo(Info->Id);
    IPL_CtrlPrvGetDZoomInfo(Info->Id, IpcInfo);

    if (Info->Id == IPL_ID_1 && (IPL_CtrlGetInfor(Info->Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR))
    {
        if (Info->CropRatio != IPL_SENCROPRATIO_NONE)
        {
            IPL_CtrlSetInfor(IPL_ID_2, IPLCTRL_SEN_PRVCROPRATIO, Info->CropRatio);
        }
        IPL_CtrlPrvSetDZoomInfo(IPL_ID_2);
    }

    IpcInfoP1->Idx = IPL_CtrlGetHalSetId(Info->Id);
    IpcInfoP2->Idx = IPL_CtrlGetHalSetId(Info->Id);
    IpcInfoP3->Idx = IPL_CtrlGetHalSetId(Info->Id);
    IpcInfoP4->Idx = IPL_CtrlGetHalSetId(Info->Id);

    IPL_BufGet(Info->Id, IPLBUF_IMEP1_1, &BufAddr, &Size);
    if ((Info->PathId & IPL_IME_PATH1) && (Size != 0))
    {
        //p1
        if (IPL_ChkImePathInfo(Info->Id, IPL_IME_PATH1, &Info->ImeP1) == TRUE)
        {
            IPL_SetImePathInfo(Info->Id, IPL_IME_PATH1, &Info->ImeP1);
            IPL_GetImePathInfo(Info->Id, IPL_IME_PATH1, &tmpImeInfo);
            IpcInfoP1->Path.PathEn = tmpImeInfo.OutputEn;
            IpcInfoP1->Path.DmaOutEn = Info->ImeP1.DmaOutEn;
            IpcInfoP1->Path.Size.Y.Width = tmpImeInfo.ImgSizeH;
            IpcInfoP1->Path.Size.Y.Height = tmpImeInfo.ImgSizeV;
            IpcInfoP1->Path.Size.Y.LineOfs = tmpImeInfo.ImgSizeLOfs;
            IpcInfoP1->Path.Fmt = tmpImeInfo.ImgFmt;
            IpcInfoP1->Path.Crop.Start.x = tmpImeInfo.CropStartX;
            IpcInfoP1->Path.Crop.Start.y = tmpImeInfo.CropStartY;
            IpcInfoP1->Path.Crop.SizeH = tmpImeInfo.CropSizeH;
            IpcInfoP1->Path.Crop.SizeV = tmpImeInfo.CropSizeV;
            tmpInfo = IPL_UTI_Y_INFO_CONV2(IpcInfoP1->Path.Fmt, IpcInfoP1->Path.Size.Y);
            IpcInfoP1->Path.Size.Lineoffset_CC = tmpInfo.LineOfs;
            rt |= IPL_IME_PATH1;
        }
        else
        {
            DBG_IND("Id: %d, IPL_ChkImePathInfo IPL_IME_PATH1 fail\r\n", Info->Id);
        }
    }

    IPL_BufGet(Info->Id, IPLBUF_IMEP2_1, &BufAddr, &Size);
    if ((Info->PathId & IPL_IME_PATH2) && (Size != 0))
    {
        //p2
        if (IPL_ChkImePathInfo(Info->Id, IPL_IME_PATH2, &Info->ImeP2) == TRUE)
        {
            IPL_SetImePathInfo(Info->Id, IPL_IME_PATH2, &Info->ImeP2);
            IPL_GetImePathInfo(Info->Id, IPL_IME_PATH2, &tmpImeInfo);
            IpcInfoP2->Path.PathEn = TRUE;    //Path2 Always Set Path Enable
            IpcInfoP2->Path.DmaOutEn = tmpImeInfo.OutputEn;
            IpcInfoP2->Path.Size.Y.Width = tmpImeInfo.ImgSizeH;
            IpcInfoP2->Path.Size.Y.Height = tmpImeInfo.ImgSizeV;
            IpcInfoP2->Path.Size.Y.LineOfs = tmpImeInfo.ImgSizeLOfs;
            IpcInfoP2->Path.Fmt = tmpImeInfo.ImgFmt;
            IpcInfoP2->Path.Crop.Start.x = tmpImeInfo.CropStartX;
            IpcInfoP2->Path.Crop.Start.y = tmpImeInfo.CropStartY;
            IpcInfoP2->Path.Crop.SizeH = tmpImeInfo.CropSizeH;
            IpcInfoP2->Path.Crop.SizeV = tmpImeInfo.CropSizeV;
            tmpInfo = IPL_UTI_Y_INFO_CONV2(IpcInfoP2->Path.Fmt, IpcInfoP2->Path.Size.Y);
            IpcInfoP2->Path.Size.Lineoffset_CC = tmpInfo.LineOfs;
            rt |= IPL_IME_PATH2;
        }
        else
        {
            DBG_IND("Id: %d, IPL_ChkImePathInfo IPL_IME_PATH2 fail\r\n", Info->Id);
        }
    }

    IPL_BufGet(Info->Id, IPLBUF_IMEP3_1, &BufAddr, &Size);
    if ((Info->PathId & IPL_IME_PATH3) && (Size != 0))
    {
        //p3
        if (IPL_ChkImePathInfo(Info->Id, IPL_IME_PATH3, &Info->ImeP3) == TRUE)
        {
            IPL_SetImePathInfo(Info->Id, IPL_IME_PATH3, &Info->ImeP3);
            IPL_GetImePathInfo(Info->Id, IPL_IME_PATH3, &tmpImeInfo);
            IpcInfoP3->Path.PathEn = tmpImeInfo.OutputEn;
            IpcInfoP3->Path.DmaOutEn = tmpImeInfo.OutputEn;
            IpcInfoP3->Path.Size.Y.Width = tmpImeInfo.ImgSizeH;
            IpcInfoP3->Path.Size.Y.Height = tmpImeInfo.ImgSizeV;
            IpcInfoP3->Path.Size.Y.LineOfs = tmpImeInfo.ImgSizeLOfs;
            IpcInfoP3->Path.Fmt = tmpImeInfo.ImgFmt;
            IpcInfoP3->Path.Crop.Start.x = tmpImeInfo.CropStartX;
            IpcInfoP3->Path.Crop.Start.y = tmpImeInfo.CropStartY;
            IpcInfoP3->Path.Crop.SizeH = tmpImeInfo.CropSizeH;
            IpcInfoP3->Path.Crop.SizeV = tmpImeInfo.CropSizeV;
            tmpInfo = IPL_UTI_Y_INFO_CONV2(IpcInfoP3->Path.Fmt, IpcInfoP3->Path.Size.Y);
            IpcInfoP3->Path.Size.Lineoffset_CC = tmpInfo.LineOfs;
            rt |= IPL_IME_PATH3;
        }
        else
        {
            DBG_IND("Id: %d, IPL_ChkImePathInfo IPL_IME_PATH3 fail\r\n", Info->Id);
        }
    }

    IPL_BufGet(Info->Id, IPLBUF_IMEP4_1, &BufAddr, &Size);
    if ((Info->PathId & IPL_IME_PATH4) && (Size != 0))
    {
        //p4
        if (IPL_ChkImePathInfo(Info->Id, IPL_IME_PATH4, &Info->ImeP4) == TRUE)
        {
            IPL_SetImePathInfo(Info->Id, IPL_IME_PATH4, &Info->ImeP4);
            IPL_GetImePathInfo(Info->Id, IPL_IME_PATH4, &tmpImeInfo);
            IpcInfoP4->Path.PathEn = tmpImeInfo.OutputEn;
            IpcInfoP4->Path.DmaOutEn = tmpImeInfo.OutputEn;
            IpcInfoP4->Path.Size.Y.Width = tmpImeInfo.ImgSizeH;
            IpcInfoP4->Path.Size.Y.Height = tmpImeInfo.ImgSizeV;
            IpcInfoP4->Path.Size.Y.LineOfs = tmpImeInfo.ImgSizeLOfs;
            IpcInfoP4->Path.Fmt = tmpImeInfo.ImgFmt;

            IpcInfoP4->Path.Crop.Start.x = tmpImeInfo.CropStartX;
            IpcInfoP4->Path.Crop.Start.y = tmpImeInfo.CropStartY;
            IpcInfoP4->Path.Crop.SizeH = tmpImeInfo.CropSizeH;
            IpcInfoP4->Path.Crop.SizeV = tmpImeInfo.CropSizeV;

            tmpInfo = IPL_UTI_Y_INFO_CONV2(IpcInfoP4->Path.Fmt, IpcInfoP4->Path.Size.Y);
            IpcInfoP4->Path.Size.Lineoffset_CC = tmpInfo.LineOfs;
            rt |= IPL_IME_PATH4;
        }
        else
        {
            DBG_IND("Id: %d, IPL_ChkImePathInfo IPL_IME_PATH4 fail\r\n", Info->Id);
        }
    }

    return rt;
}

void IPL_SetVAFCB(IPL_SET_VA_DATA *Info, IPL_HAL_PARASET *HalParaIdx, SIE_Dma_Out *Out)
{
    UINT32 BufSize = 0;

    *HalParaIdx = IPL_CtrlGetHalSetId(Info->Id);

    IPL_BufGet(Info->Id, IPLBUF_SIEVA_1, &Out->PPB0_Addr, &BufSize);
    IPL_BufGet(Info->Id, IPLBUF_SIEVA_2, &Out->PPB1_Addr, &BufSize);
}

void IPL_GetIPLCurInfoFCB(IPL_CUR_INFOR *IplCurInfo)
{
    IplCurInfo->Ipl_CF_Mode = IPL_CtrlGetInfor(IplCurInfo->Id, IPLCTRL_CFMODE);
}

void IPL_GetIMEHalIdxFCB(IPL_IME_INFOR *ImeInfo, IPL_HAL_PARASET *HalParaIdx)
{
    *HalParaIdx = IPL_CtrlGetHalSetId(ImeInfo->Id);
}

IPL_YCC_IMG_INFO IPL_GetImeCurInfoFCB(IPL_PROC_ID Ipl_Id, IPL_IME_PATH Path_Id)
{
    IPL_YCC_IMG_INFO tmp = {0};
    UINT32 ImeP1Direct;

    ImeP1Direct = IPL_CtrlGetInfor(Ipl_Id, IPLCTRL_IME_P1_D2H264);

    if (ImeP1Direct == TRUE)
    {
        if (Path_Id == IPL_IME_PATH1)
        {
            tmp = IPL_GetIMEReadyBufferAddr2(0,Ipl_Id);
        }
        else if (Path_Id == IPL_IME_PATH2)
        {
            tmp = IPL_GetIMEReadyBufferAddr2(1,Ipl_Id);
        }
        else if (Path_Id == IPL_IME_PATH3)
        {
            tmp = IPL_GetIMEReadyBufferAddr2(2,Ipl_Id);
        }
        else if (Path_Id == IPL_IME_PATH4)
        {
            tmp = IPL_GetIMEReadyBufferAddr2(3,Ipl_Id);
        }
        else
        {
            DBG_ERR("Id: %d, IPL_GetImeCurInfoFCB fail\r\n", Ipl_Id);
        }
    }
    else
    {
        if (Path_Id == IPL_IME_PATH1)
        {
            tmp = IPL_GetIMEReadyBufferAddr(0,Ipl_Id);
        }
        else if (Path_Id == IPL_IME_PATH2)
        {
            tmp = IPL_GetIMEReadyBufferAddr(1,Ipl_Id);
        }
        else if (Path_Id == IPL_IME_PATH3)
        {
            tmp = IPL_GetIMEReadyBufferAddr(2,Ipl_Id);
        }
        else if (Path_Id == IPL_IME_PATH4)
        {
            tmp = IPL_GetIMEReadyBufferAddr(3,Ipl_Id);
        }
        else
        {
            DBG_ERR("Id: %d, IPL_GetImeCurInfoFCB fail\r\n", Ipl_Id);
        }
    }

    return tmp;
}

IPL_YCC_IMG_INFO IPL_GetImeInfoFCB(IPL_PROC_ID Ipl_Id, IPL_IME_PATH Path_Id, UINT32 BufId)
{
    IPL_YCC_IMG_INFO tmp = {0};

    if (Path_Id == IPL_IME_PATH1)
    {
        tmp = IPL_GetSpecifiedIMEoutputAddr(0, BufId, Ipl_Id);
    }
    else if (Path_Id == IPL_IME_PATH2)
    {
        tmp = IPL_GetSpecifiedIMEoutputAddr(1, BufId, Ipl_Id);
    }
    else if (Path_Id == IPL_IME_PATH3)
    {
        tmp = IPL_GetSpecifiedIMEoutputAddr(2, BufId, Ipl_Id);
    }
    else if (Path_Id == IPL_IME_PATH4)
    {
        tmp = IPL_GetSpecifiedIMEoutputAddr(3, BufId, Ipl_Id);
    }
    else
    {
        DBG_ERR("Id: %d, IPL_GetImeInfoFCB fail\r\n", Ipl_Id);
    }

    return tmp;
}

UINT32 IPL_GetIPLInfoFCB(IPL_PROC_ID Ipl_Id, SENSOR_INFO *SenInfo, IPL_IME_INFOR *ImeInfo, IPL_RDE_RAW_INFO *RawInfo, ImeFuncEn *FuncEn, UINT32 *DisOnOff)
{
    IPL_HAL_READ read = {0};
    SENSOR_MODE SenMode = 0;
    IPL_MODE_INFOR Info = {0};
    ER rt = E_OK;

    Info.Id = Ipl_Id;
    rt = IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);

    if (rt == E_OK)
    {
        if ((Info.Mode == IPL_MODE_INIT) || (Ipl_Id == IPL_ID_2 && (IPL_CtrlGetInfor(Ipl_Id, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR)))
            SenMode = IPL_AlgGetUIInfo(Ipl_Id, IPL_SEL_PRVSENMODE);
        else
            rt = Sensor_GetCurMode(IPL_UTI_CONV2_SEN_ID(Ipl_Id), &SenMode);

        rt |= IPL_GetSensorStatus(Ipl_Id, SenMode, SenInfo);
    }

    if (rt != E_OK)
    {
        DBG_ERR("Get IPL Info Fail %d\r\n", Ipl_Id);
        return 0;

    }

    if (Info.Mode != IPL_MODE_OFF)
    {
        if (RawInfo != NULL)
        {
            RawInfo->Id = Ipl_Id;
            IPL_GetCmd(IPL_GET_RDE_CUR_RAW_INFOR, RawInfo);
        }

        if (ImeInfo != NULL)
        {
            ImeInfo->Id = Ipl_Id;
            IPL_GetCmd(IPL_GET_IME_INFOR, (void *)ImeInfo);
        }

        if (FuncEn != NULL)
        {
            read.imeRead = IME_R_SUBFEN;
            IME_ENG_Read(Ipl_Id, IPL_CtrlGetHalSetId(Ipl_Id), read, (void *)FuncEn);
        }

        if (DisOnOff != NULL)
        {
            *DisOnOff = IPL_CtrlGetInfor(Ipl_Id, IPLCTRL_DIS_EN);
        }
    }
    return 0;
}

void IPL_SetIPLStopFCB(IPL_CHGMODE_DATA **IPLModeInfo, IPL_CHGMODE_DATA *ChgMode, UINT32 *TrigFlg)
{
    UINT32 i;
    SENSOR_INFO Info = {0};
    IPLCTRL_INFO CtrlInfo = {0};
    IPC_CTRL_CMD TrigPauseCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};
    IPC_CTRL_CMD TrigStopCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};
    ER rt = E_OK;

    for (i = 0 ; i < IPL_ID_MAX_NUM ; i++)
    {
        if ((IPLModeInfo[i]->Mode == IPL_MODE_PREVIEW) || (IPLModeInfo[i]->Mode == IPL_MODE_VIDEO) || (IPLModeInfo[i]->Mode == IPL_MODE_VIDEOREC))
        {
            //RSC CB MSG
            IPL_RSC_CBMsgProc(i, IPL_CBMSG_CHGMODE_START, NULL);

            if ((TrigFlg[i] == IPL_STOP) || (TrigFlg[i] == IPL_PAUSE))
            {
				//#NT#2016/09/13#Jarkko Chang -begin
				//#NT# fixed ipl2sensor id mapping error
                //get mode status
                rt = IPL_GetSensorStatus(i, IPL_AlgGetUIInfo(i, IPL_SEL_PRVSENMODE), &Info);
				//#NT#2016/09/13#Jarkko Chang -end
                if (rt != E_OK)
                    return;

                if (Info.Mode->ModeType != SENSOR_MODE_CCIR && Info.Mode->ModeType != SENSOR_MODE_CCIR_INTERLACE)
                {
                    if (i == IPL_ID_1)
                    {
                        AE_Pause(IPL_UTI_CONV2_AE_ID(i), ENABLE);
                        AWB_Pause(IPL_UTI_CONV2_AWB_ID(i), ENABLE);
                        IQS_Pause(IPL_UTI_CONV2_IQS_ID(i), ENABLE);

                        if (IPL_CtrlGetInfor(i, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR || IPL_CtrlGetInfor(i, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_WDR)
                        {
                            ICFFunc_Pause(i, ENABLE);
                        }
                    }
                    else if (i == IPL_ID_2)
                    {
                        if (IPL_CtrlGetInfor(i, IPLCTRL_EXT_FUNC_EN) != ICF_FUNC_SHDR)
                        {
                            if (IPL_CtrlGetInfor(i, IPLCTRL_EXT_FUNC_EN) != ICF_FUNC_STITCH)
                            {
                                AE_Pause(IPL_UTI_CONV2_AE_ID(i), ENABLE);
                                AWB_Pause(IPL_UTI_CONV2_AWB_ID(i), ENABLE);
                            }
                            IQS_Pause(IPL_UTI_CONV2_IQS_ID(i), ENABLE);

                            if (IPL_CtrlGetInfor(i, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_WDR)
                            {
                                ICFFunc_Pause(i, ENABLE);
                            }
                        }
                    }
                }

                if (TrigFlg[i] == IPL_STOP)
                {
                    if (i != IPL_ID_2 || IPL_CtrlGetInfor(i, IPLCTRL_EXT_FUNC_EN) != ICF_FUNC_SHDR)
                        IPL_FreezeDisplayImage(TRUE, i);

                    TrigStopCmd[i] = IPC_STOP_ALL;
                    TrigFlg[i] = IPL_START;
                }
                else
                {
                    if (ChgMode[i].EventId == IPL_FLOW_VID_GETRAW)
                    {
						//#NT#2016/09/13#Jarkko Chang -begin
						//#NT# fixed ipl2sensor id mapping error
                        //initial capture parameter
                        rt = IPL_GetSensorStatus(ChgMode[i].ProcSenId, IPL_AlgGetUIInfo(ChgMode[i].ProcSenId, IPL_SEL_PRVSENMODE), &Info);
						//#NT#2016/09/13#Jarkko Chang -end
                        if (rt != E_OK)
                            return;

                        CtrlInfo.Info = &Info;
                        CtrlInfo.ChgMode = &ChgMode[i];
                        IPL_CtrlCapInit(ChgMode[i].ProcSenId, ChgMode[i].EventId, &CtrlInfo);

                        ChgMode[i].BufSize = 0;
                    }
                    else
                    {
                        //PreAE Setting
                        IPL_FCB_PrepareCapAE(ChgMode[i].ProcSenId);
                        //PreFlash Setting
                        IPL_FCB_PreFlash(ChgMode[i].ProcSenId);
                        //stop IPL
                        IPL_FreezeDisplayImage(FALSE,ChgMode[i].ProcSenId);

                        TrigPauseCmd[ChgMode[i].ProcSenId] = IPC_STOP_YCC;
                        IPL_CTRLMode(TrigPauseCmd[IPL_ID_1],TrigPauseCmd[IPL_ID_2],TrigPauseCmd[IPL_ID_3],TrigPauseCmd[IPL_ID_4]);
                        IPC_WaitVD2(ChgMode[i].ProcSenId,FALSE, IPL_TIMEOUT_MS);
                        TrigPauseCmd[ChgMode[i].ProcSenId] = IPC_PAUSE_SIE;
                        IPL_CTRLMode(TrigPauseCmd[IPL_ID_1],TrigPauseCmd[IPL_ID_2],TrigPauseCmd[IPL_ID_3],TrigPauseCmd[IPL_ID_4]);

                        TrigPauseCmd[ChgMode[i].ProcSenId] = IPC_IGNORE;
                        IPL_RSC_CBMsgProc(ChgMode[i].ProcSenId, IPL_CBMSG_CHGMODE_IPLSTOP, NULL);
                    }
                    TrigFlg[i] = IPL_RESUME;
                }
            }
            else
            {
                TrigStopCmd[i] = IPC_IGNORE;
            }
        }
    }

    IPL_CTRLMode(TrigStopCmd[IPL_ID_1],TrigStopCmd[IPL_ID_2],TrigStopCmd[IPL_ID_3],TrigStopCmd[IPL_ID_4]);

    for (i = 0 ; i < IPL_ID_MAX_NUM ; i++)
    {
        if (TrigStopCmd[i] != IPC_IGNORE)
            IPL_RSC_CBMsgProc(i, IPL_CBMSG_CHGMODE_IPLSTOP, NULL);
    }
}

void IPL_SetIPLResumeFCB(IPL_CHGMODE_DATA **IPLModeInfo, IPL_CHGMODE_DATA *ChgMode, UINT32 *TrigFlg, IPC_CTRL_CMD *ProcCmd)
{
    UINT32 ZoomIdx[IPL_ID_MAX_NUM] = {0};
    IPL_ISR_OBJ_INFO IsrObjInfo = {0};
    IPC_INFO IpcInfo = {0};
    IPLCTRL_INFO CtrlInfo = {0};;
    SENSOR_INFO Info = {0};;
    IPL_TRIG_CTRL_CMD_INFO TrigInfo = {0};;
    UINT32 *ZoomTab, ZoomTabMax = 0;
    UINT32 i;
    ER rt = E_OK;

    //Prv2Cap not support resume any ipl
    for (i = 0 ; i < IPL_ID_MAX_NUM ; i++)
    {
        if (ChgMode[i].Mode == IPL_MODE_CAP || ChgMode[i].Mode == IPL_MODE_INIT)
        {
            return;
        }
    }

    for (i = 0 ; i < IPL_ID_MAX_NUM ; i++)
    {
        if ((IPLModeInfo[i]->Mode == IPL_MODE_PREVIEW) || (IPLModeInfo[i]->Mode == IPL_MODE_VIDEO) || (IPLModeInfo[i]->Mode == IPL_MODE_VIDEOREC))
        {
            if (TrigFlg[i] == IPL_START)
            {
				//#NT#2016/09/13#Jarkko Chang -begin
				//#NT# fixed ipl2sensor id mapping error
                //get mode status
                rt = IPL_GetSensorStatus(i, IPL_AlgGetUIInfo(i, IPL_SEL_PRVSENMODE), &Info);
				//#NT#2016/09/13#Jarkko Chang -end
                if (rt != E_OK)
                    return;

                ZoomTab = SenMode2Tbl(i, IPL_AlgGetUIInfo(i, IPL_SEL_PRVSENMODE), &ZoomTabMax);
                IPL_DZoomSetInfor(i, ZoomTab, ZoomTabMax);
                //decide preivew flow
                CtrlInfo.Info = &Info;
                CtrlInfo.ChgMode = IPLModeInfo[i];
                IPL_CtrlPrvInit(i, &CtrlInfo);

                if (i != IPL_ID_2 || IPL_CtrlGetInfor(i, IPLCTRL_EXT_FUNC_EN) != ICF_FUNC_SHDR)
                {
                    //change sensor mode
                    Sensor_ChgMode(IPL_UTI_CONV2_SEN_ID(i), IPL_AlgGetUIInfo(i, IPL_SEL_PRVSENMODE));
                }

                IPL_CtrlSetInfor(i, IPLCTRL_SEN_PRVCROPRATIO, IPLModeInfo[i]->CropRatio);
                //Load post process function
                IPL_CtrlInitPrvPProcInfo(i, IPLModeInfo[i]->FuncEn);
                //initial digital zoom
                ZoomIdx[i] = IPL_DzoomGetIndex(i);
                IPL_DzoomSetIndex(i, ZoomIdx[i]);
                IPL_CtrlPrvSetDZoomInfo(i);
                IPL_CtrlInitPrvIpcInfo(i, &IpcInfo, &IsrObjInfo, IPLModeInfo[i]);

                TrigInfo = IPL_GetTriggerInfo();
                IPL_CTRLSetTrigType(&TrigInfo);
                IPL_CTRLRun(i, IPL_CtrlGetInfor(i, IPLCTRL_PRVFLOW), &IpcInfo, &IsrObjInfo, IPL_CtrlPostImage);

                ProcCmd[i] = IPC_START_RUN;
                TrigFlg[i] = IPL_NONE;
            }
            else if (TrigFlg[i] == IPL_RESUME)
            {
                IPL_CtrlInitPrvIpcInfo(i, &IpcInfo, &IsrObjInfo, IPLModeInfo[i]);
                IPL_CTRLRun(i, IPL_CtrlGetInfor(i, IPLCTRL_PRVFLOW), &IpcInfo, &IsrObjInfo, IPL_CtrlPostImage);

                ProcCmd[i] = IPC_START_RUN;
                TrigFlg[i] = IPL_NONE;
            }
        }
        else if (TrigFlg[i] == IPL_OFF)
        {
            ProcCmd[i] = IPC_STOP_ALL;
            TrigFlg[i] = IPL_NONE;
        }
    }

    IPL_TrigFCB(&ProcCmd[0]);
}

void IPL_SetTrigInfoFCB(IPL_CHGMODE_DATA *ChgMode, UINT32 *TrigFlg)
{
	//#NT#2017/03/20#Jarkko Chang -begin
    //#NT# fixed trigger type error issue
    SENSOR_INFO SenInfo = {0};
    UINT32 SenMode = SENSOR_MODE_UNKNOWN;
    IPL_TRIG_CTRL_CMD_INFO OrgTrigInfo = {0};
    IPL_TRIG_CTRL_CMD_INFO NewTrigInfo = {0};
    static IPL_TRIG_TYPE InitTrigType =  IPL_TRIG_VD;
    IPL_TRIG_TYPE TrigType =  IPL_TRIG_VD;
    static UINT32 TrigInterval[2] = {0};
    static IPL_MODE PRV_MODE[IPL_ID_MAX_NUM] = {IPL_MODE_OFF,IPL_MODE_OFF,IPL_MODE_OFF,IPL_MODE_OFF};   //Previous IPL Mode
    BOOL CapFlg = FALSE;
    UINT32 i;
    ER rt = E_OK;

    for (i = 0 ; i < IPL_ID_MAX_NUM ; i++)
    {
        if (ChgMode[i].ActFlg == TRUE)
        {
            switch (ChgMode[i].Mode)
            {
                case IPL_MODE_INIT:
                    PRV_MODE[i] = ChgMode[i].Mode;
                    //#NT#2016/07/01#Jarkko Chang -begin
                    //#NT# fixed trig type error when frame sync case
                    //#NT#2016/05/17#Jarkko Chang -begin
                    //#NT#add support side by side frame sync case
                    if (PRV_MODE[IPL_ID_1] == IPL_MODE_INIT && PRV_MODE[IPL_ID_2] == IPL_MODE_INIT)
                    {
                        if (IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR)
                        {
                            InitTrigType = IPL_TRIG_VD;     //HDR Sensor
                        }
                        else
                        {
                            InitTrigType = IPL_TRIG_TIMER;  //Dual Sensor
                        }
                    }
                    else
                    {
                        InitTrigType = IPL_TRIG_VD;     //Single Sensor
                    }
                    //#NT#2016/05/17#Jarkko Chang -end
                    //#NT#2016/07/01#Jarkko Chang -end
                break;

                case IPL_MODE_CAP:
                    TrigFlg[i] = IPL_PAUSE; //Set IPL Pause for Capture Flow.
                    CapFlg= TRUE;
                break;

                case IPL_MODE_PREVIEW:
                case IPL_MODE_VIDEO:
                case IPL_MODE_VIDEOREC:
                    if (PRV_MODE[i] == IPL_MODE_CAP)
                    {
                        TrigFlg[i] = IPL_RESUME;
                    }
                    //#NT#2016/02/16#Jarkko Chang -begin
                    //#NT# update ipl flow control
                    else if (PRV_MODE[i] != IPL_MODE_INIT)
                    {
                        TrigFlg[i] = IPL_STOP;
                    }
                    //#NT#2016/02/16#Jarkko Chang -end

                    //#NT#2016/07/01#Jarkko Chang -begin
                    //#NT#add support side by side frame sync case
                    OrgTrigInfo = IPL_GetTriggerInfo();

                    if (ChgMode[i].ProcSenId == IPL_ID_1 || ChgMode[i].ProcSenId == IPL_ID_2)
                    {
                        if (IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_CFMODE) == IPL_CF_CCIR ||
                            IPL_CtrlGetInfor(IPL_ID_2, IPLCTRL_CFMODE) == IPL_CF_CCIR)
                        {
                            TrigType = IPL_TRIG_VD;
                        }
                        else if (IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_CFMODEEXT) == IPL_CFEXT_SYNCTRIG)
                        {
                            TrigType = IPL_TRIG_VD;  //Frame Sync Case
                        }
                        //Set Trigger Type
                        else if ((ChgMode[i].ImePath1.ImgEn == ENABLE) && (ChgMode[i].ImePath1.Direct2H264 == TRUE))
                        {
                            //Set to H264 Trigger when D2H264 Enable
                            TrigType = IPL_TRIG_H264;
                            NewTrigInfo.InterVal = 0;

                            //Set IPL Smode ON/OFF
                            NewTrigInfo.smode = ENABLE;
                        }
						else
						{
							TrigType = InitTrigType;
						}

                        NewTrigInfo.type = TrigType;

                        //Set Trigger Interval
                        if (NewTrigInfo.type == IPL_TRIG_TIMER)
                        {
                            SenMode = IPL_SetSensorMode(ChgMode[i].ProcSenId, ChgMode[i].Mode, FALSE);
                            rt = IPL_GetSensorStatus(ChgMode[i].ProcSenId, SenMode, &SenInfo);
                            if (rt != E_OK)
                                return;

                            if (SenInfo.Mode->FrameRate == 0)
                            {
                                DBG_ERR("Sensor %d Frame Rate Setting Error %d\n", ChgMode[i].ProcSenId, SenInfo.Mode->FrameRate);
                                SenInfo.Mode->FrameRate = 1;
                            }
                            TrigInterval[ChgMode[i].ProcSenId] = 10000000/SenInfo.Mode->FrameRate;

                            if (TrigInterval[IPL_ID_1] != 0 && TrigInterval[IPL_ID_2] != 0)
                            {
                                if (TrigInterval[IPL_ID_1] <= TrigInterval[IPL_ID_2])
                                    NewTrigInfo.InterVal = TrigInterval[IPL_ID_2]/2;
                                else
                                    NewTrigInfo.InterVal = TrigInterval[IPL_ID_1]/2;
                            }
                            else
                            {
                                NewTrigInfo.InterVal = TrigInterval[ChgMode[i].ProcSenId];
                            }
                        }
                        else
                        {
                            TrigInterval[ChgMode[i].ProcSenId] = 0;
                            NewTrigInfo.InterVal = 0;
                        }

                        if (NewTrigInfo.type != OrgTrigInfo.type)
                        {
                            TrigFlg[IPL_ID_1] = IPL_STOP;
                            TrigFlg[IPL_ID_2] = IPL_STOP;
                        }
                        IPL_SetTriggerInfo(NewTrigInfo);
                    }
                    //#NT#2016/07/01#Jarkko Chang -end
                break;

                case IPL_MODE_OFF:
                    //#NT#2017/03/15#Jarkko Chang#[0117212] -begin
                    //#NT# fixed ipl crash when mode switch
					if (PRV_MODE[ChgMode[i].ProcSenId] == IPL_MODE_INIT)
					{
	                    TrigFlg[i] = IPL_NONE;
					} else
					{
	                    TrigFlg[i] = IPL_OFF;
					}
                    //#NT#2017/03/15#Jarkko Chang#[0117212] -end
   	                TrigInterval[ChgMode[i].ProcSenId] = 0;
                break;

                default:
                    TrigFlg[i] = IPL_NONE;
                break;
            }
            PRV_MODE[i] = ChgMode[i].Mode;
        }
    }

    if (CapFlg == TRUE)
    {
        for (i = 0 ; i < IPL_ID_MAX_NUM ; i++)
        {
            if (PRV_MODE[i] == IPL_MODE_PREVIEW || PRV_MODE[i] == IPL_MODE_VIDEO || PRV_MODE[i] == IPL_MODE_VIDEOREC)
            {
                TrigFlg[i] = IPL_STOP;
            }
        }
    }
    //#NT#2017/03/20#Jarkko Chang -end
}

ER IPL_TrigFCB(IPC_CTRL_CMD *ProcCmd)
{
    ER ercode = E_OK;
    UINT32 Id, AE_StableCnt = 1;
    IPL_TRIG_CTRL_CMD_INFO TrigInfo;

    //#NT#2016/05/17#Jarkko Chang -begin
    //#NT#fixed side by side frame sync issue
    for (Id = IPL_ID_1; Id< IPL_ID_MAX_NUM ; Id++)
    {
        if (ProcCmd[Id] == IPC_START_RUN)
        {
            IPL_ResetDisplayCtrlFlow(Id);
        }
    }
    //#NT#2016/05/17#Jarkko Chang -end
    //#NT#2016/01/29#Jarkko Chang -begin
    //#NT# fixed dual sensor mode switch error issue
    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);
    //#NT#2016/01/29#Jarkko Chang -end

    TrigInfo = IPL_GetTriggerInfo();

    for (Id = IPL_ID_1; Id< IPL_ID_MAX_NUM ; Id++)
    {
        if (ProcCmd[Id] == IPC_START_RUN)
        {
            if (IPL_CtrlGetInfor(Id, IPLCTRL_CFMODE) != IPL_CF_CCIR)
            {
                if (Id == IPL_ID_1)
                {
                   //Wait IME Frame End Only when VD/Timer Trigger
                    if (TrigInfo.type != IPL_TRIG_H264)
                    {
                        IPC_WaitImeFMD2(Id, TRUE, IPL_TIMEOUT_MS);
                        IPL_CBMsgProc(Id, IPL_CBMSG_PREVIEW, NULL);
                    }

                    //Trigger RSC
                    IPL_RSC_CBMsgProc(Id, IPL_CBMSG_CHGMODE_END, NULL);

                    if (AE_IsPause(IPL_UTI_CONV2_AE_ID(Id)) == TRUE)
                        AE_Resume(IPL_UTI_CONV2_AE_ID(Id));

                    if (AWB_IsPause(IPL_UTI_CONV2_AWB_ID(Id)) == TRUE)
                        AWB_Resume(IPL_UTI_CONV2_AWB_ID(Id));

                    if (IQS_IsPause(IPL_UTI_CONV2_IQS_ID(Id)) == TRUE)
                        IQS_Resume(IPL_UTI_CONV2_IQS_ID(Id));

                    if (ICFFunc_IsPause(Id) == TRUE)
                        ICFFunc_Resume(Id);
                }
                else if (Id == IPL_ID_2)
                {
                    if (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) != ICF_FUNC_SHDR)
                    {
                        //Wait IME Frame End Only when VD/Timer Trigger
                        if (TrigInfo.type != IPL_TRIG_H264)
                        {
                            IPC_WaitImeFMD2(Id, TRUE, IPL_TIMEOUT_MS);
                            IPL_CBMsgProc(Id, IPL_CBMSG_PREVIEW, NULL);
                        }

                        //Trigger RSC
                        IPL_RSC_CBMsgProc(Id, IPL_CBMSG_CHGMODE_END, NULL);

                        if (IPL_CtrlGetInfor(Id, IPLCTRL_EXT_FUNC_EN) != ICF_FUNC_STITCH)
                        {
                            if (AE_IsPause(IPL_UTI_CONV2_AE_ID(Id)) == TRUE)
                                AE_Resume(IPL_UTI_CONV2_AE_ID(Id));

                            if (AWB_IsPause(IPL_UTI_CONV2_AWB_ID(Id)) == TRUE)
                                AWB_Resume(IPL_UTI_CONV2_AWB_ID(Id));
                        }

                        if (IQS_IsPause(IPL_UTI_CONV2_IQS_ID(Id)) == TRUE)
                            IQS_Resume(IPL_UTI_CONV2_IQS_ID(Id));

                        if (ICFFunc_IsPause(Id) == TRUE)
                            ICFFunc_Resume(Id);
                    }
                }
            }
        }
        else if (ProcCmd[Id] == IPC_STOP_ALL)
        {
            IPL_RSC_CBMsgProc(Id, IPL_CBMSG_CHGMODE_IPLSTOP, NULL);
            //uninitial interface/CB module
            IPL_CBMsgUnInit(Id);
            IPL_BufUnInit(Id);
        }
    }

    for (Id = IPL_ID_1; Id< IPL_ID_MAX_NUM ; Id++)
    {
        if (ProcCmd[Id] == IPC_START_RUN)
        {
			//#NT#2016/10/06#Jarkko Chang -begin
            //#NT# skip wait ae stable when ccir sensor
            if (IPL_CtrlGetInfor(Id, IPLCTRL_CFMODE) != IPL_CF_CCIR)
	            AE_WaitStableRF(IPL_UTI_CONV2_AE_ID(Id), AE_StableCnt);
			//#NT#2016/10/06#Jarkko Chang -end
			//reset display control
            //#NT#2016/05/17#Jarkko Chang -begin
            //#NT#fixed side by side frame sync issue
            //IPL_ResetDisplayCtrlFlow(Id);
            //#NT#2016/05/17#Jarkko Chang -end
            IPL_CBMsgProc(Id, IPL_CBMSG_PREVIEWSTABLE, NULL);
        }
    }

    return ercode;
}

void IPL_GetCtrlFlowFCB(IPL_CTRL_FLOW_INFO *CtrlInfo)
{
    UINT32 (*DzTable)[DZOOM_ITEM_MAX];
    UINT32 ZoomTabMax = 0, SenMode = 0;
    UINT32 SieOutH = 0;
    IPC_QINFO IpcInfo = {0};

    IpcInfo = IPL_CTRLQuery(CtrlInfo->Id, IPL_CtrlGetInfor(CtrlInfo->Id, IPLCTRL_PRVFLOW));
    SenMode = IPL_SetSensorMode(CtrlInfo->Id, IPL_MODE_VIDEOREC, FALSE);

    DzTable = (UINT32(*)[DZOOM_ITEM_MAX])SenMode2Tbl(CtrlInfo->Id, SenMode, &ZoomTabMax);
    SieOutH = DzTable[IPL_DzoomGetIndex(CtrlInfo->Id)][2];

    if (SieOutH > IpcInfo.QUE_FC.StripMinSize)
        CtrlInfo->CtrlFlow = IPL_CTRLFLOW_2STRIPE;
    else
        CtrlInfo->CtrlFlow = IPL_CTRLFLOW_NORMAL;
}
