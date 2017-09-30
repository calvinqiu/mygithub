/*
    IPL mode API

    This file is IPL mode control.

    @file       IPL_ModeSample.c
    @ingroup    mISYSAlg
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#include "IPL_IMX322LQJ_EVB_FF_Int.h"

UINT32 IPL_SetSensorMode(IPL_PROC_ID Id, IPL_MODE Mode, BOOL SetFlg)
{
    UINT32 Video_FPS, Video_SIZE, SenMode = SENSOR_MODE_UNKNOWN;

    if (Id == IPL_ID_2 && IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR)
    {
        Video_FPS = IPL_AlgGetUIInfo(IPL_ID_1, IPL_SEL_VIDEO_FPS);
        Video_SIZE = IPL_AlgGetUIInfo(IPL_ID_1, IPL_SEL_VIDEO_SIZE);
    }
    else
    {
        Video_FPS = IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_FPS);
        Video_SIZE = IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE);
    }

    switch(Mode)
    {
        case IPL_MODE_INIT: //use largest sensor mode for buffer assign
            SenMode = SENSOR_MODE_1;
            break;

        case IPL_MODE_VIDEOREC:
        case IPL_MODE_PREVIEW:
        case IPL_MODE_VIDEO:
            SenMode = SENSOR_MODE_1;
            break;

        case IPL_MODE_CAP:
            SenMode = SENSOR_MODE_1;
            IPL_AlgSetUIInfo(Id, IPL_SEL_CAPSENMODE, SENSOR_MODE_1);
            IPL_AlgSetUIInfo(Id, IPL_SEL_RAW_BITDEPTH_CAP, SEL_RAW_BITDEPTH_10);
            break;

        default:
            DBG_ERR("Id: %d, ^Rsensor mode map fail %d\r\n", Id, Mode);
            SenMode = SENSOR_MODE_1;
            break;
    }

    if (SetFlg == TRUE && Mode != IPL_MODE_CAP)
    {
        IPL_AlgSetUIInfo(Id, IPL_SEL_PRVSENMODE, SenMode);
        IPL_AlgSetUIInfo(Id, IPL_SEL_RAW_BITDEPTH_PRV, SEL_RAW_BITDEPTH_10);
    }
    return SenMode;
}

static IPLBUF_MODE IPL_MapICF2BufMode(IPLCTRL_FLOW Flow)
{
    switch(Flow)
    {
        case ICF_FLOW_A:
            return IPLBUF_MODE_FLOW_A;

        case ICF_FLOW_C:
            return IPLBUF_MODE_FLOW_C;

        case ICF_FLOW_DEWARP:
            return IPLBUF_MODE_FLOW_D;

        default:
            DBG_ERR("flow map fail %d\r\n", Flow);
            return 0;
    }
}

static IPLCTRL_FLOW IPL_MapCtrlFlow2ICF(UINT32 CtrlFlow)
{
    switch(CtrlFlow)
    {
        case SEL_CTRLFLOW_ALLDIRECT:
            return ICF_FLOW_A;

        case SEL_CTRLFLOW_COMMON:
            return ICF_FLOW_C;

        case SEL_CTRLFLOW_DEWARP:
            return ICF_FLOW_DEWARP;

        default:
            DBG_ERR("flow map fail %d\r\n", CtrlFlow);
            return ICF_FLOW_C;
    }
}

//#NT#2017/03/15#Jarkko Chang#[0117212] -begin
//#NT# fixed ipl crash when mode switch
static ER IPL_None(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    return E_OK;
}
//#NT#2017/03/15#Jarkko Chang#[0117212] -end

static ER IPL_Init(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    UINT32 *ZoomTab, ZoomTabMax, ZoomIdx;
    SENSOR_INFO Info;
    IPL_BUF_OBJ_INFO BufObj;
    IPLBUF_ENG2DRAMINFO BufInfo;
    IPLBUF_RST BufRst = {0};
    IPLCTRL_INFO CtrlInfo;
    ER rt = E_OK;

    //decide preview flow
    IPL_CtrlSetInfor(ChgMode->ProcSenId, IPLCTRL_PRVFLOW, IPL_MapCtrlFlow2ICF(IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_CTRLFLOW)));
    //decide sensor mode
    IPL_SetSensorMode(ChgMode->ProcSenId, ChgMode->Mode, TRUE);

	//#NT#2016/09/13#Jarkko Chang -begin
	//#NT# fixed ipl2sensor id mapping error
    //get mode status
    rt = IPL_GetSensorStatus(ChgMode->ProcSenId, IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_PRVSENMODE), &Info);
	//#NT#2016/09/13#Jarkko Chang -end
    if (rt != E_OK)
        return rt;

    //IPL data initial
    ZoomTab = SenMode2Tbl(ChgMode->ProcSenId, IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_PRVSENMODE), &ZoomTabMax);
    IPL_DZoomSetInfor(ChgMode->ProcSenId, ZoomTab, ZoomTabMax);
    ZoomIdx = IPL_DzoomGetIndex(ChgMode->ProcSenId);
    IPL_DzoomSetIndex(ChgMode->ProcSenId, 0);

    //decide preivew flow
    CtrlInfo.Info = &Info;
    CtrlInfo.ChgMode = ChgMode;
    IPL_CtrlPrvInit(ChgMode->ProcSenId, &CtrlInfo);

    //allocate buffer & initial buffer size
    BufObj.Id = ChgMode->ProcSenId;
    BufObj.BufAddr = ChgMode->BufAddr;
    BufObj.BufSize = ChgMode->BufSize;

    IPL_BufInit(&BufObj);
    IPL_CtrlInitPrvBufInfo(ChgMode, &BufInfo);

    BufRst = IPL_BufAssign(IPL_MapICF2BufMode(IPL_CtrlGetInfor(ChgMode->ProcSenId, IPLCTRL_PRVFLOW)), &BufInfo);

    //IPL_BufDump(BufObj.Id);  //DEBUG Using

    ChgMode->BufSize = BufRst.BufSize;

    IPL_DzoomSetIndex(ChgMode->ProcSenId, ZoomIdx);


    IQS_Init(ChgMode->ProcSenId);

    return E_OK;
}

static ER IPL_Off2Stream(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    IPL_ISR_OBJ_INFO IsrObjInfo;
    ICFFUNC_OBJ_INFO ICFFuncObj = {0};
    AE_OBJ_INFO AeObj = {0};
    AWB_OBJ_INFO AwbObj = {0};
    AF_OBJ_INFO AfObj = {0};
    IQS_OBJ_INFO IqsObj = {0};
    IPC_INFO IpcInfo = {0};
    IPL_TRIG_CTRL_CMD_INFO TrigInfo;
    IPLCTRL_INFO CtrlInfo;
    SENSOR_INFO Info;
    UINT32 *ZoomTab, ZoomTabMax;
    ER rt = E_OK;

    //decide sensor mode
    IPL_SetSensorMode(ChgMode->ProcSenId, ChgMode->Mode, TRUE);
	//#NT#2016/09/13#Jarkko Chang -begin
	//#NT# fixed ipl2sensor id mapping error
    //get mode status
    rt = IPL_GetSensorStatus(ChgMode->ProcSenId, IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_PRVSENMODE), &Info);
	//#NT#2016/09/13#Jarkko Chang -end
    if (rt != E_OK)
        return rt;

    ZoomTab = SenMode2Tbl(ChgMode->ProcSenId, IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_PRVSENMODE), &ZoomTabMax);
    IPL_DZoomSetInfor(ChgMode->ProcSenId, ZoomTab, ZoomTabMax);
    //decide preivew flow
    CtrlInfo.Info = &Info;
    CtrlInfo.ChgMode = ChgMode;
    IPL_CtrlPrvInit(ChgMode->ProcSenId, &CtrlInfo);

    if (Info.Mode->ModeType != SENSOR_MODE_CCIR && Info.Mode->ModeType != SENSOR_MODE_CCIR_INTERLACE)
    {
        if (ChgMode->ProcSenId == IPL_ID_1)
        {
            IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_GDCCOMP, IPL_FCB_AlgGDC);
            IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_DISCOMP, IPL_FCB_AlgDIS);
            IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_IMAGEEFFECT, IPL_FCB_AlgIE);
            IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_IMAGEFLIP, IPL_FCB_ImageFlip);
            IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_WDR, IPL_FCB_AlgWDR);
            IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_WDR_LEVEL, IPL_FCB_AlgWDRLevel);

            IPL_CBMsgInit(ChgMode->ProcSenId, ChgMode->CB_FP);

            AeObj.EventTab.Start = AE_Init;
            AeObj.EventTab.Process = AE_Process;
            AeObj.EventTab.Pause = NULL;
            AeObj.EventTab.HdrNotify = NULL;
            if (IPL_CtrlGetInfor(ChgMode->ProcSenId, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR)
            {
                AeObj.EventTab.HdrNotify = ICFFunc_TrigEvent;
            }
            AwbObj.EventTab.Start = AWB_Init;
            AwbObj.EventTab.Process = AWB_Process;
            AwbObj.EventTab.Pause = NULL;

            AE_Open(IPL_UTI_CONV2_AE_ID(ChgMode->ProcSenId), &AeObj);
            AWB_Open(IPL_UTI_CONV2_AWB_ID(ChgMode->ProcSenId), &AwbObj);

            Sensor_PresetLock(IPL_UTI_CONV2_SEN_ID(ChgMode->ProcSenId), ENABLE);
            Sensor_PresetMode(IPL_UTI_CONV2_SEN_ID(ChgMode->ProcSenId), IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_PRVSENMODE));

            AE_Start(IPL_UTI_CONV2_AE_ID(ChgMode->ProcSenId), TRUE);
            AWB_Start(IPL_UTI_CONV2_AWB_ID(ChgMode->ProcSenId), TRUE);

            Sensor_PresetLock(IPL_UTI_CONV2_SEN_ID(ChgMode->ProcSenId), DISABLE);

            AfObj.EventTab.Start = AF_Init;
            AfObj.EventTab.Process = AF_Process;
            AfObj.EventTab.Pause = NULL;
            AF_Open(IPL_UTI_CONV2_AF_ID(ChgMode->ProcSenId), &AfObj);

            IqsObj.EventTab.Start = IQS_Ctrl_Init;
            IqsObj.EventTab.Process = IQS_Ctrl_Process;
            IqsObj.EventTab.Pause = NULL;
            IQS_Open(IPL_UTI_CONV2_IQS_ID(ChgMode->ProcSenId), &IqsObj);

            IQS_Start(IPL_UTI_CONV2_IQS_ID(ChgMode->ProcSenId), TRUE);

            ICFFuncObj.EventTab.Process = ExtFunc_Process;
            ICFFunc_Open(ChgMode->ProcSenId, &ICFFuncObj);

            //initial sensor mode
            Sensor_ChgMode(IPL_UTI_CONV2_SEN_ID(ChgMode->ProcSenId), IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_PRVSENMODE));
        }
        else if (ChgMode->ProcSenId == IPL_ID_2)
        {
            if (IPL_CtrlGetInfor(ChgMode->ProcSenId, IPLCTRL_EXT_FUNC_EN) != ICF_FUNC_SHDR)
            {
                IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_GDCCOMP, IPL_FCB_AlgGDC);
                IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_DISCOMP, IPL_FCB_AlgDIS);
                IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_IMAGEEFFECT, IPL_FCB_AlgIE);
                IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_IMAGEFLIP, IPL_FCB_ImageFlip);
                IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_WDR, IPL_FCB_AlgWDR);
                IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_WDR_LEVEL, IPL_FCB_AlgWDRLevel);

                IPL_CBMsgInit(ChgMode->ProcSenId, ChgMode->CB_FP);

                if (IPL_CtrlGetInfor(ChgMode->ProcSenId, IPLCTRL_EXT_FUNC_EN) != ICF_FUNC_STITCH)
                {
                    AeObj.EventTab.Start = AE_Init;
                    AeObj.EventTab.Process = AE_Process;
                    AeObj.EventTab.Pause = NULL;
                    AeObj.EventTab.HdrNotify = NULL;

                    AwbObj.EventTab.Start = AWB_Init;
                    AwbObj.EventTab.Process = AWB_Process;
                    AwbObj.EventTab.Pause = NULL;

                    AE_Open(IPL_UTI_CONV2_AE_ID(ChgMode->ProcSenId), &AeObj);
                    AWB_Open(IPL_UTI_CONV2_AWB_ID(ChgMode->ProcSenId), &AwbObj);

                    Sensor_PresetLock(IPL_UTI_CONV2_SEN_ID(ChgMode->ProcSenId), ENABLE);
                    Sensor_PresetMode(IPL_UTI_CONV2_SEN_ID(ChgMode->ProcSenId), IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_PRVSENMODE));

                    AE_Start(IPL_UTI_CONV2_AE_ID(ChgMode->ProcSenId), TRUE);
                    AWB_Start(IPL_UTI_CONV2_AWB_ID(ChgMode->ProcSenId), TRUE);

                    Sensor_PresetLock(IPL_UTI_CONV2_SEN_ID(ChgMode->ProcSenId), DISABLE);
                }

                IqsObj.EventTab.Start = IQS_Ctrl_Init;
                IqsObj.EventTab.Process = IQS_Ctrl_Process;
                IqsObj.EventTab.Pause = NULL;
                IQS_Open(IPL_UTI_CONV2_IQS_ID(ChgMode->ProcSenId), &IqsObj);

                IQS_Start(IPL_UTI_CONV2_IQS_ID(ChgMode->ProcSenId), TRUE);

                ICFFuncObj.EventTab.Process = ExtFunc_Process;
                ICFFunc_Open(ChgMode->ProcSenId, &ICFFuncObj);

                //initial sensor mode
                Sensor_ChgMode(IPL_UTI_CONV2_SEN_ID(ChgMode->ProcSenId), IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_PRVSENMODE));
            }
        }
        else
        {
            IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_GDCCOMP, IPL_FCB_AlgGDC);
            IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_DISCOMP, IPL_FCB_AlgDIS);
            IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_IMAGEEFFECT, IPL_FCB_AlgIE);
            IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_IMAGEFLIP, IPL_FCB_ImageFlip);
            IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_WDR, IPL_FCB_AlgWDR);
            IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_WDR_LEVEL, IPL_FCB_AlgWDRLevel);

            IPL_CBMsgInit(ChgMode->ProcSenId, ChgMode->CB_FP);

            //initial sensor mode
            Sensor_ChgMode(IPL_UTI_CONV2_SEN_ID(ChgMode->ProcSenId), IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_PRVSENMODE));
        }
    }
    else
    {
        IPL_CBMsgInit(ChgMode->ProcSenId, ChgMode->CB_FP);
        //initial sensor mode
        Sensor_ChgMode(IPL_UTI_CONV2_SEN_ID(ChgMode->ProcSenId), IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_PRVSENMODE));
    }

    IPL_CtrlSetInfor(ChgMode->ProcSenId, IPLCTRL_SEN_PRVCROPRATIO, ChgMode->CropRatio);

    //Load post process function
    IPL_CtrlInitPrvPProcInfo(ChgMode->ProcSenId, ChgMode->FuncEn);

    //initial digital zoom
    IPL_CtrlPrvSetDZoomInfo(ChgMode->ProcSenId);

    IPL_CtrlInitPrvIpcInfo(ChgMode->ProcSenId, &IpcInfo, &IsrObjInfo, ChgMode);

    //reset display control
    IPL_ResetDisplayCtrlFlow(ChgMode->ProcSenId);

    TrigInfo = IPL_GetTriggerInfo();
    IPL_CTRLSetTrigType(&TrigInfo);
    IPL_CTRLRun(ChgMode->ProcSenId, IPL_CtrlGetInfor(ChgMode->ProcSenId, IPLCTRL_PRVFLOW), &IpcInfo, &IsrObjInfo, IPL_CtrlPostImage);

    return E_OK;
}

static ER IPL_Stream2Cap(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    UINT32 *ZoomTab, ZoomTabMax;
    SENSOR_INFO Info;
    IPL_BUF_OBJ_INFO BufObj;
    IPLBUF_RST BufRst;
    IPLCTRL_INFO CtrlInfo;
    IPLBUF_ENG2DRAMINFO BufInfo;
    ER rt = E_OK;

    if (ChgMode->EventId != IPL_FLOW_VID_GETRAW)
    {
        IPL_SetSensorMode(ChgMode->ProcSenId, ChgMode->Mode, TRUE);
        IPC_WaitVD2(ChgMode->ProcSenId,FALSE, IPL_TIMEOUT_MS);

		//#NT#2016/09/13#Jarkko Chang -begin
		//#NT# fixed ipl2sensor id mapping error
        rt = IPL_GetSensorStatus(ChgMode->ProcSenId, IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_CAPSENMODE), &Info);
		//#NT#2016/09/13#Jarkko Chang -end
        if (rt != E_OK)
            return rt;

        ZoomTab = SenMode2Tbl(ChgMode->ProcSenId, IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_CAPSENMODE), &ZoomTabMax);
        IPL_DZoomSetInfor(ChgMode->ProcSenId, ZoomTab, ZoomTabMax);

        //initial capture parameter
        CtrlInfo.Info = &Info;
        CtrlInfo.ChgMode = ChgMode;
        IPL_CtrlCapInit(ChgMode->ProcSenId, ChgMode->EventId, &CtrlInfo);

        //conver to cap buffer
        BufObj.Id = ChgMode->ProcSenId;
        BufObj.BufAddr = ChgMode->BufAddr;
        BufObj.BufSize = ChgMode->BufSize;
        IPL_BufInit(&BufObj);
        IPL_CtrlInitCapBufInfo(BufObj.Id, ChgMode, &BufInfo);
        BufRst = IPL_BufAssign(IPLBUF_MODE_CAP, &BufInfo);
        ChgMode->BufSize = BufRst.BufSize;

#if IQS_DUMP_ENGREG
        UINT32 Addr, Size;
        IPL_BufGet(BufObj.Id, (IPLBUF_DRE_DBGBUF), &Addr, &Size);
        IQS_SetEngDumpBuffer(IPL_HAL_DRE, Addr, Size);
        DBG_ERR("dre info 0x%08x   0x%08x\r\n", Addr, Size);
#endif
    }

    return E_OK;
}

static ER IPL_Cap2Stream(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    UINT32 ZoomIdx[IPL_ID_MAX_NUM] = {0};
    IPL_IME_PATH_INFO PathInfo1[IPL_ID_MAX_NUM];
    IPL_IME_PATH_INFO PathInfo2[IPL_ID_MAX_NUM];
    IPL_IME_PATH_INFO PathInfo3[IPL_ID_MAX_NUM];
    IPL_IME_PATH_INFO PathInfo4[IPL_ID_MAX_NUM];
    IPC_CTRL_CMD ProcCmd[IPL_ID_MAX_NUM] = {IPC_IGNORE,IPC_IGNORE,IPC_IGNORE,IPC_IGNORE};
    UINT32 *ZoomTab, ZoomTabMax;
    //#NT#2016/04/21#Jarkko Chang -begin
    //#NT# fixed cap2stream hangs bug when dual sensor
    //#NT#2016/12/07#Silvia Wu -begin
    //#NT# fixed coverity warning
    SENSOR_INFO Info = {0};
	//#NT#2016/12/07#Silvia Wu -end
    ER rt;
    IPLCTRL_INFO CtrlInfo;
    //#NT#2016/04/21#Jarkko Chang -end

    IPL_GetImePathInfo(ChgMode->ProcSenId, IPL_IME_PATH1, &PathInfo1[ChgMode->ProcSenId]);
    IPL_GetImePathInfo(ChgMode->ProcSenId, IPL_IME_PATH2, &PathInfo2[ChgMode->ProcSenId]);
    IPL_GetImePathInfo(ChgMode->ProcSenId, IPL_IME_PATH3, &PathInfo3[ChgMode->ProcSenId]);
    IPL_GetImePathInfo(ChgMode->ProcSenId, IPL_IME_PATH4, &PathInfo4[ChgMode->ProcSenId]);

	//#NT#2016/11/02#Silvia Wu -begin
    //#NT# get cap sensor mode
	//#NT#2016/09/20#Silvia Wu -begin
    //#NT# fixed ccir/ccir interlace capture issue
	//decide sensor mode
	IPL_SetSensorMode(ChgMode->ProcSenId, ChgMode->Mode, TRUE);

    rt = IPL_GetSensorStatus(IPL_UTI_CONV2_SEN_ID(ChgMode->ProcSenId), IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_CAPSENMODE), &Info);
	if (rt != E_OK)
        return rt;
	if (Info.Mode == NULL)
		return E_SYS;
	//#NT#2016/11/02#Silvia Wu -end

	if (Info.Mode->ModeType == SENSOR_MODE_CCIR || Info.Mode->ModeType == SENSOR_MODE_CCIR_INTERLACE)
	{
	    ProcCmd[ChgMode->ProcSenId] = IPC_STOP_ALL;
	}
	else
	{
	    ProcCmd[ChgMode->ProcSenId] = IPC_STOP_RAW;
	}

    IPL_CTRLMode(ProcCmd[IPL_ID_1],ProcCmd[IPL_ID_2],ProcCmd[IPL_ID_3],ProcCmd[IPL_ID_4]);

	IPL_Init(ChgMode,PrvMode);
	//#NT#2016/09/20#Silvia Wu -end

    //#NT#2016/04/21#Jarkko Chang -begin
    //#NT# fixed cap2stream hangs bug when dual sensor
    //initial sensor mode
    Sensor_ChgMode(IPL_UTI_CONV2_SEN_ID(ChgMode->ProcSenId), IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_PRVSENMODE));

    ZoomTab = SenMode2Tbl(ChgMode->ProcSenId, IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_PRVSENMODE), &ZoomTabMax);
    IPL_DZoomSetInfor(ChgMode->ProcSenId, ZoomTab, ZoomTabMax);
	//#NT#2016/09/13#Jarkko Chang -begin
	//#NT# fixed ipl2sensor id mapping error
    //get mode status
    rt = IPL_GetSensorStatus(ChgMode->ProcSenId, IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_PRVSENMODE), &Info);
	//#NT#2016/09/13#Jarkko Chang -end
    if (rt != E_OK)
        return rt;

    //decide preivew flow
    CtrlInfo.Info = &Info;
    CtrlInfo.ChgMode = ChgMode;
    IPL_CtrlPrvInit(ChgMode->ProcSenId, &CtrlInfo);
    //#NT#2016/04/21#Jarkko Chang -end

    //initial digital zoom
    ZoomIdx[ChgMode->ProcSenId] = IPL_DzoomGetIndex(ChgMode->ProcSenId);
    IPL_DzoomSetIndex(ChgMode->ProcSenId, ZoomIdx[ChgMode->ProcSenId]);
    IPL_CtrlPrvSetDZoomInfo(ChgMode->ProcSenId);

	//#NT#2017/02/06#Jarkko Chang -begin
	//#NT# add support side by side left/right, up/down case
	if(IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_SBS) == SEL_SBS_LR)
	{
	    PathInfo1[ChgMode->ProcSenId].ImgSizeLOfs /= 2;
	    PathInfo2[ChgMode->ProcSenId].ImgSizeLOfs /= 2;
	    PathInfo3[ChgMode->ProcSenId].ImgSizeLOfs /= 2;
	    PathInfo4[ChgMode->ProcSenId].ImgSizeLOfs /= 2;
	}
	//#NT#2017/02/06#Jarkko Chang -end

    IPL_SetImePathInfo(ChgMode->ProcSenId, IPL_IME_PATH1, &PathInfo1[ChgMode->ProcSenId]);
    IPL_SetImePathInfo(ChgMode->ProcSenId, IPL_IME_PATH2, &PathInfo2[ChgMode->ProcSenId]);
    IPL_SetImePathInfo(ChgMode->ProcSenId, IPL_IME_PATH3, &PathInfo3[ChgMode->ProcSenId]);
    IPL_SetImePathInfo(ChgMode->ProcSenId, IPL_IME_PATH4, &PathInfo4[ChgMode->ProcSenId]);

    return E_OK;
}

static ER IPL_Cap2Off(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    DBG_IND("do nothing\r\n");
    return E_OK;
}

static ER IPL_Off2Cap(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    UINT32 *ZoomTab, ZoomTabMax;
    SENSOR_INFO Info;
    IPL_BUF_OBJ_INFO BufObj;
    IPLBUF_RST BufRst;
    IPLCTRL_INFO CtrlInfo;
    IPLBUF_ENG2DRAMINFO BufInfo;
    ER rt = E_OK;

    IPL_SetSensorMode(ChgMode->ProcSenId, ChgMode->Mode, TRUE);

	//#NT#2016/09/13#Jarkko Chang -begin
	//#NT# fixed ipl2sensor id mapping error
    rt = IPL_GetSensorStatus(ChgMode->ProcSenId, IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_CAPSENMODE), &Info);
	//#NT#2016/09/13#Jarkko Chang -end
    if (rt != E_OK)
        return rt;

    ZoomTab = SenMode2Tbl(ChgMode->ProcSenId, IPL_AlgGetUIInfo(ChgMode->ProcSenId, IPL_SEL_CAPSENMODE), &ZoomTabMax);
    IPL_DZoomSetInfor(ChgMode->ProcSenId, ZoomTab, ZoomTabMax);

    //initial capture parameter
    CtrlInfo.Info = &Info;
    CtrlInfo.ChgMode = ChgMode;
    IPL_CtrlCapInit(ChgMode->ProcSenId, ChgMode->EventId, &CtrlInfo);

    IPL_CtrlSetInfor(ChgMode->ProcSenId, IPLCTRL_CFMODEEXT, IPL_CFEXT_SIE_ONLY);

    //conver to cap buffer
    BufObj.Id = ChgMode->ProcSenId;
    BufObj.BufAddr = ChgMode->BufAddr;
    BufObj.BufSize = ChgMode->BufSize;
    IPL_BufInit(&BufObj);
    IPL_CtrlInitCapBufInfo(BufObj.Id, ChgMode, &BufInfo);
    BufRst = IPL_BufAssign(IPLBUF_MODE_CAP, &BufInfo);
    ChgMode->BufSize = BufRst.BufSize;

#if IQS_DUMP_ENGREG
    UINT32 Addr, Size;
    IPL_BufGet(BufObj.Id, (IPLBUF_DRE_DBGBUF), &Addr, &Size);
    IQS_SetEngDumpBuffer(IPL_HAL_DRE, Addr, Size);
    DBG_ERR("dre info 0x%08x   0x%08x\r\n", Addr, Size);
#endif

    return E_OK;
}

static ER IPL_Stream2Off(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{

    if (IPL_CtrlGetInfor(ChgMode->ProcSenId, IPLCTRL_CFMODE) != IPL_CF_CCIR)
    {
        IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_GDCCOMP, NULL);
        IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_DISCOMP, NULL);
        IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_IMAGEEFFECT, NULL);
        IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_IMAGEFLIP, NULL);
        IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_WDR, NULL);
        IPL_AlgSetEventFP(ChgMode->ProcSenId, IPL_SEL_WDR_LEVEL, NULL);

        if (ChgMode->ProcSenId == IPL_ID_1)
        {
            //close AWB
            AWB_Close(IPL_UTI_CONV2_AE_ID(ChgMode->ProcSenId));
            //close AE
            AE_Close(IPL_UTI_CONV2_AE_ID(ChgMode->ProcSenId));
            //close AF
            AF_Close(IPL_UTI_CONV2_AF_ID(ChgMode->ProcSenId));

            IQS_Close(IPL_UTI_CONV2_IQS_ID(ChgMode->ProcSenId));
            ICFFunc_Close(ChgMode->ProcSenId);

            IPL_FreezeDisplayImage(FALSE,ChgMode->ProcSenId);
        }
        else if (ChgMode->ProcSenId == IPL_ID_2)
        {
            if (IPL_CtrlGetInfor(ChgMode->ProcSenId, IPLCTRL_EXT_FUNC_EN) != ICF_FUNC_SHDR)
            {
                if (IPL_CtrlGetInfor(ChgMode->ProcSenId, IPLCTRL_EXT_FUNC_EN) != ICF_FUNC_STITCH)
                {
                    //close AWB
                    AWB_Close(IPL_UTI_CONV2_AE_ID(ChgMode->ProcSenId));
                    //close AE
                    AE_Close(IPL_UTI_CONV2_AE_ID(ChgMode->ProcSenId));
                }

                IQS_Close(IPL_UTI_CONV2_IQS_ID(ChgMode->ProcSenId));
                ICFFunc_Close(ChgMode->ProcSenId);

                IPL_FreezeDisplayImage(FALSE,ChgMode->ProcSenId);
            }
        }
    }

    return E_OK;
}

static ER IPL_Stream2Stream(IPL_CHGMODE_DATA *ChgMode, IPL_CHGMODE_DATA *PrvMode)
{
    IPL_CBMsgInit(ChgMode->ProcSenId, ChgMode->CB_FP);

    //decide sensor mode
    IPL_SetSensorMode(ChgMode->ProcSenId, ChgMode->Mode, TRUE);

    return E_OK;
}

static IPL_CMD_CHGMODE_FP ChgModeAct[IPL_MODE_NUM][IPL_MODE_NUM] =
{
    /*IPL_MODE_OFF,     IPL_MODE_INIT,      IPL_MODE_PREVIEW,   IPL_MODE_VIDEO,     IPL_MODE_VIDEOREC,  IPL_MODE_CAP*/
    {NULL,              IPL_Init,           IPL_Off2Stream,     IPL_Off2Stream,     IPL_Off2Stream,     IPL_Off2Cap},       /*IPL_MODE_OFF*/
    {IPL_None,          IPL_Init,           IPL_Off2Stream,     IPL_Off2Stream,     IPL_Off2Stream,     IPL_Off2Cap},       /*IPL_MODE_INIT*/
    {IPL_Stream2Off,    NULL,               IPL_Stream2Stream,  IPL_Stream2Stream,  IPL_Stream2Stream,  IPL_Stream2Cap},    /*IPL_MODE_PREVIEW*/
    {IPL_Stream2Off,    NULL,               IPL_Stream2Stream,  IPL_Stream2Stream,  IPL_Stream2Stream,  IPL_Stream2Cap},    /*IPL_MODE_VIDEO*/
    {IPL_Stream2Off,    NULL,               IPL_Stream2Stream,  IPL_Stream2Stream,  NULL,               IPL_Stream2Cap},    /*IPL_MODE_VIDEOREC*/
    {IPL_Cap2Off,       NULL,               IPL_Cap2Stream,     IPL_Cap2Stream,     IPL_Cap2Stream,     NULL},              /*IPL_MODE_CAP*/
};

IPL_CMD_CHGMODE_FP IPL_GetModeFp(IPL_MODE CurMode, IPL_MODE NextMode)
{
    if ((CurMode >= IPL_MODE_NUM) || (NextMode >= IPL_MODE_NUM))
    {
        return NULL;
    }
    return ChgModeAct[CurMode][NextMode];
}
