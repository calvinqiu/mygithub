/*
    IPL flow Callback Function

    This file is the API of the flow callback function.

    @file       IPL_RSC_MN34110_EVB_FF.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/
#include "Lens.h"
#include "Delay.h"
#include "GyroDrv.h"
#include "rsc_common.h"
#include "rsc_task.h"
#include "rsc_taskctrl_int.h"
#include "DxSensor.h"
#include "iqs_api.h"
#include "IPL_Cmd.h"
#include "IPL_Ctrl.h"
#include "IPL_HeaderInfor_Raw.h"
#include "IPL_CBMsg.h"
#include "Platform.h"
#include "ae_api.h"

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          RSC_sample
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

static UINT32 RSC_DCE_GLUT_Buf[65];
static INT32 RSC_DCE_RLUT_Buf[65];
static INT32 RSC_DCE_BLUT_Buf[65];
static DCE_Distortion  RSC_DCE_Distortion = {0};
static DCE_SUBFUNC DceSubF;
static DCE_SUBFUNC *pRSC_DCE_subf = &DceSubF;
static IQS_FLOW_MSG MsgID[IPL_ID_MAX_NUM] = {IQS_FLOW_MSG_NUM};
static UINT32 Zoom_Value_Cur = 0, Zoom_Value_Prev = 0;
RSCCTRL_INIT InitInfo = {0};
RSC_LENS_DISTORT_INFO* RSC_DistorInfor = NULL;

UINT16 InitDistortTbl[33] =
{
    4096, 4062, 3981, 3856, 3697, 3518, 3330, 3143,
    2964, 2795, 2639, 2494, 2362, 2241, 2130, 2029,
    1935, 1850, 1771, 1698, 1631, 1568, 1510, 1456,
    1406, 1359, 1314, 1273, 1234, 1197, 1162, 1130,
    1099,
};

static BOOL RscFirstRunFlag = TRUE;
static HEADER_RAW_DATA CurRaw = {0};

static SEL_RSC_STATUS m_RSC_Status [IPL_ID_MAX_NUM]= {SEL_RSC_OFF};
static SEL_RSC_UISTATUS m_RSC_UISTATUS[IPL_ID_MAX_NUM] = {SEL_RSC_NONE};
static IPL_MODE m_RSC_CurMode[IPL_ID_MAX_NUM] = {IPL_MODE_NONE};
SEL_RSC_MODECHG rsc_modechg[IPL_ID_MAX_NUM] = {SEL_RSC_IDLE} ;
SEL_RSC_STATUS  rsc_OnOffInfor[IPL_ID_MAX_NUM] = {SEL_RSC_OFF};
static UINT32 RSC_CHGMODESIGNAL[IPL_ID_MAX_NUM] = {0};

static RSCCTRL_Q m_QInfo[IPL_ID_MAX_NUM] = {10, 10, 50};
//#NT#2016/04/25#liwei cheng -begin
//#NT#add axis control switch
RSCCTRL_AXIS_SWITCH Axis_Set[IPL_ID_MAX_NUM] = {TRUE,TRUE,TRUE};
//#NT#2016/04/25#liwei cheng -end
//#NT#2016/06/03#liwei cheng -begin
//#NT#add Dimension control and TYPE control
//#NT#2016/02/23#liwei cheng -begin
//#NT#
static RSCCTRL_STAB StabInfo[IPL_ID_MAX_NUM] =
{
    {1,1,1,3,3,3,100,100,100,100,100,100,RSC_DIM_CTRL,RSC_TYPE_CTRL},
    {1,1,1,3,3,3,100,100,100,100,100,100,RSC_DIM_CTRL,RSC_TYPE_CTRL},
    {1,1,1,3,3,3,100,100,100,100,100,100,RSC_DIM_CTRL,RSC_TYPE_CTRL},
    {1,1,1,3,3,3,100,100,100,100,100,100,RSC_DIM_CTRL,RSC_TYPE_CTRL},
};
//#NT#2016/02/23#liwei cheng -end
//#NT#2016/06/03#liwei cheng -end
static void RSC_FCB_RunTimeChg(IPL_PROC_ID Id, RSCCTRL_INFO *pInfo)
{
    IPL_DCE_NOR_FACTOR_INFO Info = {0};

    //Cap mode has no header to get information
    if(RSC_GetCurMode(Id)==IPL_MODE_CAP)
    {
        IPL_SIE_RAW_INFO RawInfo = {0};
        UINT32 ISO, ExpT, Iris;

        RawInfo.Id = Id;
        IPL_GetCmd(IPL_GET_SIE_CUR_CAP_CH0_RAW_INFOR, &RawInfo);
        pInfo->pIPLSize->SieCropSize.h = RawInfo.ImgHeight;
        pInfo->pIPLSize->SieCropSize.w = RawInfo.ImgWidth;
        pInfo->pIPLSize->SieOutSize.h  = RawInfo.ImgHeight;
        pInfo->pIPLSize->SieOutSize.w  = RawInfo.ImgWidth;
        pInfo->pIPLSize->IfeInSize.h   = RawInfo.ImgHeight;
        pInfo->pIPLSize->IfeInSize.w   = RawInfo.ImgWidth;
        pInfo->pIPLSize->DceOutSize.h  = RawInfo.ImgHeight;
        pInfo->pIPLSize->DceOutSize.w  = RawInfo.ImgWidth;

        AE_GetCapAEArg(Id, &ISO, &ExpT, &Iris);
        *pInfo->pExpT = ExpT;
        //#NT#2016/02/23#liwei cheng -begin
        //#NT#
        pInfo->pFlipInfor->uiHflipEn = InitInfo.Flip_Parm.uiHflipEn;
        pInfo->pFlipInfor->uiVflipEn = InitInfo.Flip_Parm.uiVflipEn;
        //#NT#2016/02/23#liwei cheng -end
    }
    else
    {
        if(IPL_HeaderRawGetCurInfo_Dly1VD(Id, &CurRaw) == E_SYS)
        {
            DBG_ERR("RSC: Get header error!!!\r\n");
        }
        // update exp when RscFirstRun and exposure time has been set
        if(CurRaw.exp.expt !=0 || RscFirstRunFlag)
        {
            *pInfo->pExpT = CurRaw.exp.expt;
        }
        pInfo->pIPLSize->SieCropSize.h = CurRaw.raw_sie_crop.Height;
        pInfo->pIPLSize->SieCropSize.w = CurRaw.raw_sie_crop.Width;
        pInfo->pIPLSize->SieOutSize.h = CurRaw.raw_sie_ch0_out.Height;
        pInfo->pIPLSize->SieOutSize.w = CurRaw.raw_sie_ch0_out.Width;
        pInfo->pIPLSize->IfeInSize.h = CurRaw.raw_ipl_in.Height;
        pInfo->pIPLSize->IfeInSize.w = CurRaw.raw_ipl_in.Width;
        pInfo->pIPLSize->DceOutSize.h = CurRaw.raw_ipl_in.Height;
        pInfo->pIPLSize->DceOutSize.w = CurRaw.raw_ipl_in.Width;
        pInfo->pDceInfo->GdcCacDzParam.DistNorm.uiNFact = CurRaw.dce.NFact;
        pInfo->pDceInfo->GdcCacDzParam.DistNorm.uiNBit = CurRaw.dce.NBit;

        //#NT#2016/02/23#liwei cheng -begin
        //#NT#
        //H/V Flip
        if(CurRaw.flip_info & FLIP_H)
            pInfo->pFlipInfor->uiHflipEn = TRUE;
        else
            pInfo->pFlipInfor->uiHflipEn = FALSE;
        if(CurRaw.flip_info & FLIP_V)
            pInfo->pFlipInfor->uiVflipEn = TRUE;
        else
            pInfo->pFlipInfor->uiVflipEn = FALSE;
        //#NT#2016/02/23#liwei cheng -end

    }

    Zoom_Value_Cur = Lens_Zoom_GetSection(ZOOM_CUR_SECTION);
    pInfo->pLensInfo->FocalLength = Lens_Focus_GetLength_um(Zoom_Value_Cur);


    if(RSC_DistorInfor == NULL)
    {
        pInfo->pDistorInfo->puiDistortTbl = InitDistortTbl;
        pInfo->pDistorInfo->uiDistortNorm = 128;
        //debug_msg("Null DistorTable Adrress\r\n");
    }
    else
    {
        pInfo->pDistorInfo->puiDistortTbl = RSC_DistorInfor->puiDistortTbl;
        pInfo->pDistorInfo->uiDistortNorm = RSC_DistorInfor->uiDistortNorm;
    }



    if ((RscFirstRunFlag == TRUE)||(Zoom_Value_Cur != Zoom_Value_Prev))
    {
        RSC_DCE_Distortion.GeoLUT.pGGeoLut = (UINT32*)RSC_DCE_GLUT_Buf;
        RSC_DCE_Distortion.GeoLUT.pRGeoLut = (INT32*)RSC_DCE_RLUT_Buf;
        RSC_DCE_Distortion.GeoLUT.pBGeoLut = (INT32*)RSC_DCE_BLUT_Buf;

        pRSC_DCE_subf->pDistortion = &RSC_DCE_Distortion;

        IQS_DCE_ParamSetting(Id, MsgID[Id], Zoom_Value_Cur, pRSC_DCE_subf->pDistortion, &pRSC_DCE_subf->Dce_Dist, &pRSC_DCE_subf->Radious);

        pInfo->pDceInfo->DcSel = _GDC_2DLUT_CASCADE; //RSC default open / GDC must open / if RSC opne --> CASCADE!

        pInfo->pDceInfo->ImgCent.iXCent = RSC_DCE_Distortion.Center.x;
        pInfo->pDceInfo->ImgCent.iYCent = RSC_DCE_Distortion.Center.y;

        pInfo->pDceInfo->GdcCacDzParam.ImgRat.uiXDist = pRSC_DCE_subf->Dce_Dist.Fact_X;
        pInfo->pDceInfo->GdcCacDzParam.ImgRat.uiYDist = pRSC_DCE_subf->Dce_Dist.Fact_Y;

        pInfo->pDceInfo->FovParam.uiFovGain = RSC_DCE_Distortion.Fov.Gain;
        if(RSC_DCE_Distortion.Fov.FOV_SEL == FOV_BOUND_DUPLICATE)
        {
            pInfo->pDceInfo->FovParam.FovSel = _FOV_BOUND_DUPLICATE;
        }
        else if(RSC_DCE_Distortion.Fov.FOV_SEL == FOV_BOUND_REG_RGB)
        {
            pInfo->pDceInfo->FovParam.FovSel = _FOV_BOUND_REG_RGB;
        }
        pInfo->pDceInfo->FovParam.FovRGB.uiRBound = RSC_DCE_Distortion.Fov.BoundryProcess.R;
        pInfo->pDceInfo->FovParam.FovRGB.uiGBound = RSC_DCE_Distortion.Fov.BoundryProcess.G;
        pInfo->pDceInfo->FovParam.FovRGB.uiBBound = RSC_DCE_Distortion.Fov.BoundryProcess.B;

        if(RSC_DCE_Distortion.ColorAber.Cacsel == CAC_RGB_GAIN)
        {
            pInfo->pDceInfo->GdcCacOzParam.CacSel = _RGB_GAIN;
        }
        else if(RSC_DCE_Distortion.ColorAber.Cacsel == CAC_RGB_LUT)
        {
            pInfo->pDceInfo->GdcCacOzParam.CacSel = _RGB_LUT;
        }
        pInfo->pDceInfo->GdcCacOzParam.CacGain.uiRLutGain = RSC_DCE_Distortion.ColorAber.CACGain.Fact_R;
        pInfo->pDceInfo->GdcCacOzParam.CacGain.uiGLutGain = RSC_DCE_Distortion.ColorAber.CACGain.Fact_G;
        pInfo->pDceInfo->GdcCacOzParam.CacGain.uiBLutGain = RSC_DCE_Distortion.ColorAber.CACGain.Fact_B;

        pInfo->pDceInfo->GdcCacOzParam.puiGGeoLut = RSC_DCE_Distortion.GeoLUT.pGGeoLut;
        pInfo->pDceInfo->GdcCacOzParam.piRGeoLut = RSC_DCE_Distortion.GeoLUT.pRGeoLut;
        pInfo->pDceInfo->GdcCacOzParam.piBGeoLut = RSC_DCE_Distortion.GeoLUT.pBGeoLut;
    }

    //Only for cap mode
    if(RSC_GetCurMode(Id)==IPL_MODE_CAP)
    {

        RSCCTRL_SEN_INFO SensorInfor = {0};
        Info.Id = Id;
        RSC_GetSensorInfor(&SensorInfor);
        Info.width = SensorInfor.Width1X;
        Info.height = SensorInfor.Height1X;
        Info.centX = (SensorInfor.Width1X)/2;
        Info.centY = (SensorInfor.Height1X)/2;
        Info.Xdisf = pInfo->pDceInfo->GdcCacDzParam.ImgRat.uiXDist;
        Info.Ydisf = pInfo->pDceInfo->GdcCacDzParam.ImgRat.uiYDist;
        IPL_GetCmd(IPL_GET_DCE_NOR_FACTOR_INFOR, (void *)&Info);
        if(Info.height == 0 || Info.width == 0)
            debug_msg("Width1X and Height1X are zero\r\n");

        pInfo->pDceInfo->GdcCacDzParam.DistNorm.uiNFact = Info.NormFact;
        pInfo->pDceInfo->GdcCacDzParam.DistNorm.uiNBit  = Info.NormBit;

    }

    Zoom_Value_Prev = Zoom_Value_Cur;
    if (RscFirstRunFlag == FALSE)
    {
        //gyro_getFreeRunData(&pInfo->pGDTbl->DataCnt, pInfo->pGDTbl->pGyroX, pInfo->pGDTbl->pGyroY, pInfo->pGDTbl->pGyroZ);
    }
    else
    {
        RscFirstRunFlag = FALSE;
    }
}

static UINT32 RSC_CalGD_NumbyFPS(UINT32 FPS)
{
    UINT32 GDNum;

    if ( FPS <= 30 )
    {
        GDNum = 32;
    }
    else if ( FPS <= 60 )
    {
        GDNum = 16;
    }
    else if ( FPS <= 120 )
    {
        GDNum = 8;
    }
    else
    {
        GDNum = 8;
    }

    return GDNum;
}

IPL_MODE RSC_GetCurMode(IPL_PROC_ID Id)
{
    return m_RSC_CurMode[Id];
}

void RSC_Process(IPL_PROC_ID Id, UINT32 Value)
{
    GYRO_OPENOBJ openObj;
    GYRO_FREE_RUN_PARAM frParam= {0};
    SENSOR_INFO SenInfo = {0};
    IPL_MODE_INFOR Info = {0};
    GYRO_RSCINFOINIT gyrorscInit;
    static RSCCTRL_Q m_QInfo_open = {0};
    //static RSCCTRL_Q m_QInfo_close = {0};
    static UINT32 m_TriggerIdx = 1;
    ER code;
    ER GYRO_code;
    IPL_DZOOM_INFOR CurInfo = {0};
    ER rt = E_OK;

    Info.Id = Id;
    IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);

    m_RSC_CurMode[Id] = Info.Mode;

    if ((Info.Mode != IPL_MODE_PREVIEW) &&
        (Info.Mode != IPL_MODE_VIDEO) &&
        (Info.Mode != IPL_MODE_VIDEOREC) &&
        (Info.Mode != IPL_MODE_CAP))
    {
        debug_msg("Mode error!! Get Mode = %d\r\n", Info.Mode);
        return;
    }

    if (Info.Mode != IPL_MODE_CAP)
    {
        MsgID[Id] = IQCB_PRV_DCE_INIT;
    }
    else
    {
        MsgID[Id] = IQCB_PREIME_D2D_DCE_PRI;
    }

    if (Value == SEL_RSC_ON)
    {

        rt=Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
        if (rt != E_OK || SenInfo.Mode == NULL)
            return;


        m_QInfo_open = m_QInfo[Id];
        if(Info.Mode == IPL_MODE_CAP)
        {
            m_QInfo_open.CompRatioEIS = 0 ;
            m_QInfo_open.CompRatioRSC = 20;
        }

        if(Info.Mode == IPL_MODE_CAP)
            InitInfo.IPL_MODE = RSCCTRL_MODE_CAP;
        else
            InitInfo.IPL_MODE = RSCCTRL_MODE_NORMAL;



        InitInfo.pRSC_Q = &m_QInfo_open;
        InitInfo.FuncEn = _RSC_EIS;
        InitInfo.GD_Num = RSC_CalGD_NumbyFPS((SenInfo.Mode->FrameRate)/10);
        InitInfo.TwoDLutSz = 9;
        InitInfo.SenSize.w = SenInfo.CellWidth;
        InitInfo.SenSize.h = SenInfo.CellHeight;

        gyro_RSCInfoInit(&gyrorscInit);
        InitInfo.GyroRef.GDFmt = (RSCCTRL_GD_FMT)gyrorscInit.gdFormat;
        InitInfo.GyroRef.GDAxis[0] = (RSCCTRL_GD_AXIS)gyrorscInit.AxisSelec[0];
        InitInfo.GyroRef.GDAxis[1] = (RSCCTRL_GD_AXIS)gyrorscInit.AxisSelec[1];
        InitInfo.GyroRef.GDAxis[2] = (RSCCTRL_GD_AXIS)gyrorscInit.AxisSelec[2];
        InitInfo.GyroRef.GDDir[0] =  (RSCCTRL_GD_DIR)gyrorscInit.DirSelec[0];
        InitInfo.GyroRef.GDDir[1] =  (RSCCTRL_GD_DIR)gyrorscInit.DirSelec[1];
        InitInfo.GyroRef.GDDir[2] =  (RSCCTRL_GD_DIR)gyrorscInit.DirSelec[2];
        InitInfo.GyroRef.GDunit = gyrorscInit.uiDataUnit;
        InitInfo.GyroRef.GDCalGain[0] = gyrorscInit.uiCalGain[0];
        InitInfo.GyroRef.GDCalGain[1] = gyrorscInit.uiCalGain[1];
        InitInfo.GyroRef.GDCalGain[2] = gyrorscInit.uiCalGain[2];
        InitInfo.GyroRef.GDCalOfs[0] = gyrorscInit.iCalOfs[0];
        InitInfo.GyroRef.GDCalOfs[1] = gyrorscInit.iCalOfs[1];
        InitInfo.GyroRef.GDCalOfs[2] = gyrorscInit.iCalOfs[2];
        InitInfo.fpRunTimeChg = RSC_FCB_RunTimeChg;
        InitInfo.fpGetGyroData = RSC_GetGyroData;
        //#NT#2016/03/02#Jarkko Chang -begin
        //#NT# DSP flip naming update
        //#NT#2016/02/23#liwei cheng -begin
        //#NT#
        InitInfo.StabInfor = &StabInfo[Id];
        switch(IPL_AlgGetUIInfo(Id, IPL_SEL_IMAGEFLIP))
        {
            case SEL_IMAGEFLIP_DSP_H:
                InitInfo.Flip_Parm.uiHflipEn = TRUE;
                break;
            case SEL_IMAGEFLIP_DSP_V:
                InitInfo.Flip_Parm.uiVflipEn = TRUE;
                break;
            case SEL_IMAGEFLIP_DSP_H_V:
                InitInfo.Flip_Parm.uiHflipEn = TRUE;
                InitInfo.Flip_Parm.uiVflipEn = TRUE;
                break;
            default:
                InitInfo.Flip_Parm.uiHflipEn = FALSE;
                InitInfo.Flip_Parm.uiVflipEn = FALSE;
                break;
        }
        //#NT#2016/04/25#liwei cheng -begin
        //#NT# add axis control switch
        InitInfo.Axiset.uiCompXEn = Axis_Set[Id].uiCompXEn;
        InitInfo.Axiset.uiCompYEn = Axis_Set[Id].uiCompYEn;
        InitInfo.Axiset.uiCompZEn = Axis_Set[Id].uiCompZEn;
        //#NT#2016/04/25#liwei cheng -begin
        //#NT#

        //#NT#2016/02/23#liwei cheng -end
        //#NT#2016/03/02#Jarkko Chang -end
        // do not use dzoom cmd when CHGMODE_END
        if(RSC_GetCurMode(Id) != IPL_MODE_CAP && RSC_CHGMODESIGNAL[Id] != IPL_CBMSG_CHGMODE_END)
        {
            IPL_DZOOM Info_DZ;

            CurInfo.Id = Id;
            IPL_GetCmd(IPL_GET_CUR_DZ_INFOR, (void *)&CurInfo);
            Info_DZ.Id = CurInfo.Id;
            Info_DZ.Idx = CurInfo.CurIdx;
            IPL_SetCmd(IPL_SET_DZOOM, (void *)&Info_DZ);
            IPL_WaitCmdFinish();
            IPC_WaitVD(0,FALSE);
            IPC_WaitVD(0,FALSE);
        }
        //clear RSC_CHGMODESIGNAL
        RSC_CHGMODESIGNAL[Id] = 0;


        code = RSCCTRL_OPEN(Id, &InitInfo);

        if(code == E_SYS)
        {
            debug_msg("RSC ERROR_SYSTEM \r\n" );
            return;
        }



        //check gyro ready
        openObj.FP_GYRO_CB = RSCCTRL_TRIG;
        GYRO_code=gyro_open(&openObj);
        if(GYRO_code == E_SYS)
        {
            debug_msg("GYRO OPEN Fail \r\n" );
            return;
        }
        frParam.uiPeriodUs = 10000000/SenInfo.Mode->FrameRate;
        frParam.uiDataNum = InitInfo.GD_Num;
        //rsc_tmp
        frParam.uiTriggerIdx = (InitInfo.GD_Num - 1) - m_TriggerIdx;
        frParam.triggerMode = GYRO_FREERUN_SIE_SYNC;
        gyro_setFreeRunParam(&frParam);
        gyro_setMode(GYRO_OP_MODE_FREE_RUN);

        m_RSC_Status[Id] = Value;
    }
    else if (Value == SEL_RSC_OFF)
    {

        m_RSC_Status[Id] = Value;

        while(RSC_IsIdle()==FALSE)
        {
             IPC_WaitVD(0,TRUE);
        }

        // enter free-run form single-access
        gyro_setMode(GYRO_OP_MODE_SINGLE_ACCESS);
        // close gyro
        gyro_close();
        //rsc off
        RSCCTRL_CLOSE(Id,RSC_CHGMODESIGNAL[Id]);
        RSC_CHGMODESIGNAL[Id] = 0;




        RscFirstRunFlag = TRUE;
    }
}

void RSC_SetStatus(IPL_PROC_ID Id, SEL_RSC_STATUS Value)
{
    IPL_MODE_INFOR Info = {0};

    Info.Id = Id;
    IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);
    if(Value == SEL_RSC_ON)
    {
        if(m_RSC_UISTATUS[Id] == SEL_RSC_PRV_CAP  || Info.Mode == IPL_MODE_CAP)
        {
            DBG_IND("RSC func = %d\r\n", Value);
            RSC_Process(Id, Value);
        }
        else if(m_RSC_UISTATUS[Id] == SEL_RSC_PRV)
        {
            DBG_IND("RSC func = %d\r\n", Value);
            RSC_Process(Id, Value);
        }
        else if(m_RSC_UISTATUS[Id] == SEL_RSC_CAP)
        {
            DBG_IND("RSC func = %d\r\n", Value);
            RSC_Process(Id, Value);
        }
        else
        {
            DBG_IND("SEL_RSC out of range\r\n");
            return;
        }

    }
    else if(Value == SEL_RSC_OFF)
    {
        DBG_IND("RSC func = %d\r\n", Value);
        RSC_Process(Id, Value);

    }
    else
    {
        debug_msg("RSC Set error!! Get SetStatus = %d\r\n", Value);
    }
}
void RSC_GetGyroData(RSCCTRL_GYRO_TBL* gyrodata)
{
    gyro_getFreeRunData(&gyrodata->DataCnt, gyrodata->pGyroX, gyrodata->pGyroY, gyrodata->pGyroZ);

}
SEL_RSC_STATUS RSC_GetStatus(IPL_PROC_ID Id)
{
    return m_RSC_Status[Id];
}

SEL_RSC_STATUS RSC_OnOffInfor(IPL_PROC_ID Id)
{
    return rsc_OnOffInfor[Id];
}
SEL_RSC_UISTATUS RSC_GetUIStatus(IPL_PROC_ID Id)
{
    return m_RSC_UISTATUS[Id];
}


UINT32 RSC_GetEISRatio(IPL_PROC_ID Id,IPL_MODE NextMode,SENSOR_MODE NextSensoeMode)
{
    UINT32 EIS_Ratio;
    UINT32 EIS_RatioPnt;
    static RSCCTRL_Q m_QInfo_open = {0};
    static RSCCTRL_Q m_QInfo_close = {0};


    m_QInfo_open = m_QInfo[Id];
    if(rsc_OnOffInfor[Id] == SEL_RSC_ON )
    {
        EIS_Ratio = m_QInfo_open.CompRatioEIS;
    }
    else
    {
        EIS_Ratio = m_QInfo_close.CompRatioEIS;
    }
    EIS_RatioPnt = (100*IMG_EISDIS_RATIO_UINT)/(100+EIS_Ratio);
    return EIS_RatioPnt;
}

static void RSC_IPL_CB(IPL_CBMSG Msg, void *Data)
{

    if(Msg == IPL_CBMSG_CHGMODE_START)
    {
        m_RSC_UISTATUS[IPL_ID_1] = SEL_RSC_NONE;

        if(RSC_GetStatus(IPL_ID_1) == SEL_RSC_OFF)
        {
           return;
        }

        RSC_CHGMODESIGNAL[IPL_ID_1] = Msg;
        RSC_SetStatus(IPL_ID_1, SEL_RSC_OFF);
        RSC_CloseTsk();
    }
   else if(Msg == IPL_CBMSG_CHGMODE_END)
    {
        if(RSC_GetStatus(IPL_ID_1) == SEL_RSC_ON)
        {

            return;
        }
        if(rsc_OnOffInfor[IPL_ID_1] == SEL_RSC_ON)
        {

            //open rsc
        }
        else
        {
            return;
        }
        RSC_CHGMODESIGNAL[IPL_ID_1] = Msg;
        m_RSC_UISTATUS[IPL_ID_1] = SEL_RSC_PRV_CAP;
        RSC_OpenTsk();
        RSC_SetStatus(IPL_ID_1, SEL_RSC_ON);

    }
    else if(Msg == IPL_CBMSG_CHGMODE_IPLSTOP)
    {
        //do nothing

    }
    else
    {
        debug_msg("RSC IPL CB error!! Get CBMSG = %d\r\n", Msg);
    }
}


void RSC_SetSwitch(IPL_PROC_ID Id, SEL_RSC_MODECHG Value ,SEL_RSC_STATUS Value2)
{
   GYRO_OPENOBJ openObj;
   ER GYRO_code;

   //check gyro ready
   openObj.FP_GYRO_CB = RSCCTRL_TRIG;
   GYRO_code=gyro_open(&openObj);
   if(GYRO_code == E_SYS)
   {
        debug_msg("GYRO OPEN Fail \r\n" );
        return;
   }

    rsc_modechg[Id] = Value;
    rsc_OnOffInfor[Id]  = Value2;

    if(rsc_OnOffInfor[Id] == SEL_RSC_ON)
        IPL_RSC_CBMsgInit(Id, RSC_IPL_CB);
    else
        IPL_RSC_CBMsgUnInit(Id);

    if (IPL_IsOpen())
    {
        IPL_MODE_INFOR Info = {0};

        if(Id == IPL_ID_1)
        {
            Info.Id = Id;
            IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);

            if ((Info.Mode != IPL_MODE_PREVIEW) &&
                (Info.Mode != IPL_MODE_VIDEO) &&
                (Info.Mode != IPL_MODE_VIDEOREC) &&
                (Info.Mode != IPL_MODE_CAP))
            {
                //debug_msg("RSC Set switch wrong mode = %d\r\n",Info.Mode);
            }
            else
            {
                if(rsc_modechg[Id] == SEL_RSC_RUNTIME && rsc_OnOffInfor[Id] == SEL_RSC_ON)
                {
                    m_RSC_UISTATUS[Id] = SEL_RSC_PRV_CAP;
                    if(RSC_GetStatus(Id) == SEL_RSC_ON)
                    {
                        return;
                    }
                    //cap flow do not open task here
                    if(m_RSC_UISTATUS[Id] != SEL_RSC_CAP)
                    {
                        IPL_RSC_CBMsgInit(Id, RSC_IPL_CB);
                        RSC_OpenTsk();
                        RSC_SetStatus(Id, SEL_RSC_ON);
                    }

                }
                else if (rsc_modechg[Id] == SEL_RSC_RUNTIME && rsc_OnOffInfor[Id] == SEL_RSC_OFF)
                {
                    m_RSC_UISTATUS[Id] = SEL_RSC_NONE;
                    if(RSC_GetStatus(Id) == SEL_RSC_OFF)
                    {
                        return;
                    }
                    //cap flow do not open task here
                    if(m_RSC_UISTATUS[Id] != SEL_RSC_CAP)
                    {
                       RSC_SetStatus(Id, SEL_RSC_OFF);
                       RSC_CloseTsk();
                       IPL_RSC_CBMsgUnInit(Id);
                    }

                }
                else
                {
                }

            }
        }
        else
        {
            debug_msg("RSC do not support IPL_ID = %d\r\n", Id);
        }
    }
}

RSC_DCE_2D_LUT_PARAM Dce2DLutNullInfo = {0};
RSC_IPP_INCROP_PARAM InCropNullInfo = {0};
RSC_DCE_GDCCNTR_PARAM GdcCntrNullInfo = {0};
UINT32 RSC_DCE_NULL2DP[DCE_2DLUT_MAX_TAP*DCE_2DLUT_MAX_TAP];
void RSC_GetNull2D(IPL_PROC_ID Id,UINT32 SieOutH,UINT32 SieOutV,UINT32 sizeH,UINT32 sizeV,DCE_SUBFUNC* pDce_sub,IPL_HAL_GROUP* group)
{
    if(Id != IPL_ID_1)
    {
        //debug_msg("RSC Init do not surpport Id \r\n" );
        return;

    }
    IPL_MODE_INFOR Info = {0};

    Info.Id = Id;
    IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);
    if(Info.Mode == IPL_MODE_CAP)
    {
        debug_msg("Cap do not use neutral RSC\r\n");
    }
    else
    {
        if(rsc_OnOffInfor[Id] == SEL_RSC_OFF)
        {
            return;
        }
        else
        {
            Dce2DLutNullInfo.Dce2dLutParam.ui2DLutAddr =(UINT32) RSC_DCE_NULL2DP;
            rsc_GenerateNeutralOutParam(&InCropNullInfo, &GdcCntrNullInfo, &Dce2DLutNullInfo, 0,0,sizeH,  sizeV,  sizeH, sizeV, m_QInfo[Id].CompRatioRSC, m_QInfo[Id].CompRatioRSC,0,0);

            pDce_sub->_2DLutParam = (_2DLUT_PARAM *)(&Dce2DLutNullInfo.Dce2dLutParam);
            group->dceUpdate |= DCE_2DLUT;
            IPL_HeaderRawSetChkStatus(Id, HEADER_RAW_STS_RSC_ON);

        }
    }

}
ER RSC_SetDistorTable(RSC_LENS_DISTORT_INFO* address)
{

    RSC_DistorInfor = address;
    return E_OK;

}
