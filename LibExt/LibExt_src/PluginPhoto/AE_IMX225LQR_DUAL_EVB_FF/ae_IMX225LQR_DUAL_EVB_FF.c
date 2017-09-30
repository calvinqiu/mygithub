/**
    Auto Exposure flow.

    ae flow.

    @file       ae_sample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include <stdio.h>

#include "Type.h"
#include "IPL_AlgInfor.h"
#include "ae_alg.h"
#include "ae_IMX225LQR_DUAL_EVB_FF_int.h"
#include "ae_task.h"
#include "IPL_Cmd.h"
#include "iqs_api.h"
#include "aeafd_alg.h"
#include "NOTE.h"
#include "grph.h"
#include "IPL_Ctrl.h"
#include "Lens.h"
#include "IQS_Utility.h"
#include "SensorHDRLib.h"
#include "fd_lib.h"
#include "Delay.h"

#define CAL_ISO DISABLE
#if CAL_ISO
#include "IPL_Cal_DataPassing_IMX225LQR_DUAL_EVB_FF_Int.h"
#endif

#define __MODULE__ AE
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

#define AE_STAMPLOG  0

extern UINT32 IQS_GetGammaValue(UINT32 Id, UINT32 Data12Bit);
extern UINT32 IQS_GetInvGammaValue(UINT32 Id, UINT32 Data10Bit);

static void AE_SetCalGain(IPL_PROC_ID Id);

UINT32 AEUartMsg = DISABLE;

static UINT8 (*AeWin)[AE_WIN_X] = (UINT8(*)[AE_WIN_X])AE_Win_Evaluative;
static UINT8 (*AeWin_FD)[AE_WIN_X] = (UINT8(*)[AE_WIN_X])AE_Win_FD;
static UINT32 AeFlag[AE_PROC_CNT] = {0};
static AE_INFO AeInfo[AE_PROC_CNT] = {0};
static AE_INFO AeCapInfo[AE_PROC_CNT] = {0};
static AE_EVENT_TAB AEEventTAB = {0};
static UINT16 AeHistBuf[AE_PROC_CNT][AE_HIST_NUM];
static UINT16 AeHist2BufY[AE_PROC_CNT][AE_HIST_NUM];
static AEALG_ARG PrvNewAeArg[AE_PROC_CNT][3], CapNewAeArg[AE_PROC_CNT];
static AEALG_ARG PrvLastAeArg[AE_PROC_CNT][3];
static UINT32 AECnt[AE_PROC_CNT] = {0};
static UINT32 AEStep[AE_PROC_CNT] = {0};
static GAIN_SETTING gGain[AE_PROC_CNT][AE_TOTAL_PARAMNUM];  //Keep 6 frames setting.
static EXPOSURE_SETTING gExpTSetting[AE_PROC_CNT][AE_TOTAL_PARAMNUM];  //Keep 6 frames setting.

static AE_BLK_INFO AeBlkInfo[AE_PROC_CNT] = {0};
static AE_SYNC_INFO AeSyncInfo[AE_PROC_CNT][AE_SYNC_TAB_SIZE] = {0};
static UINT32 AeSyncCnt[AE_PROC_CNT] = {0};

//Auto Curve Gen Parameters
static AE_GEN_INFO AeCurveGenInfo[AE_PROC_CNT] = {0};
static UINT32 AeAutoCurveTab[AE_PROC_CNT][AE_GEN_CURVE_NODE_MAX][AEALG_INFO_MAX];
static BOOL AeCurveUpdate[AE_PROC_CNT] = {0};

static UINT32 Ratio[SEN_MFRAME_MAX_NUM] = {0};
static UINT32 ISOGain[SEN_MFRAME_MAX_NUM] = {0}, ExpoTime[SEN_MFRAME_MAX_NUM] = {0};

static UINT32 SHDR_GT_Gain = 0;
static BOOL IR_CUT_ENABLE[AE_PROC_CNT] = {FALSE, FALSE};
static AE_APPMODE AeAppMode = AE_APPMODE_NORMAL;
static UINT32 AeStichMapId[AE_PROC_CNT] = {IPL_ID_1, IPL_ID_2};    //[Left,Right]
static AE_INT_CTRL_INFO AeIntCtrlInfo[AE_PROC_CNT] = {{AE_NOR_CAL_SET_SHIFT, AE_NOR_PREDICT_POINT, AE_NOR_PEROID},
                                                      {AE_NOR_CAL_SET_SHIFT, AE_NOR_PREDICT_POINT, AE_NOR_PEROID}};
extern UINT32 uiISOGain[AE_ID_MAX_NUM], uiExpoTime[AE_ID_MAX_NUM], uiIris[AE_ID_MAX_NUM];
extern UINT32 uiAEUartCmd[AE_ID_MAX_NUM], uiAEOSDCmd[AE_ID_MAX_NUM];
extern UINT32 uiAEManualCmd[AE_ID_MAX_NUM], uiAEManualISOCmd[AE_ID_MAX_NUM];
static AE_HISTOGRAM weighting_histogram;
#if 0
static UINT32 GAcc[AE_WIN_X * AE_WIN_Y] = {0};
#endif
/* -3, -2.7, -2.5, -2.3, -2, -1.7, -1.5, -1.3, -1, -0.7, -0.5, -0.3, 0, 0.3, 0.5, 0.7, 1, 1.3, 1.5, 1.7, 2.0, 2.3, 2.5, 2.7, 3  */
static UINT32 EVTable[SEL_AEEV_MAX_CNT] =
{  13, 16, 18, 20, 25, 31, 35, 40, 50, 63, 71, 79, 100, 126, 141, 159, 200, 252, 283, 317, 400, 504, 566, 635, 800};

static AEALG_EVENT_TAB AeAlgEventTab =
{
    GetOverExpoRatio,
    GetFnoRatio,
    NULL
};

UINT32 AEAFD_START[AE_PROC_CNT] = {FALSE, FALSE};
UINT32 AEAFD_CurrentFlickerType = AE_FLICKER_60HZ;
UINT16 AfdBuffer[LA_WIN_X*LA_WIN_Y*AEAFD_DETECT_CYCLE]; //32*32*6
UINT16 Blk_Gacc[AE_PROC_CNT][LA_WIN_X*LA_WIN_Y], Blk_Yacc[AE_PROC_CNT][LA_WIN_X*LA_WIN_Y];
UINT32 AEB_Ratio[AE_PROC_CNT] = {100, 100};

//#NT#2016/08/08#XL_Wei -begin
//#NT#Used to calculate ev when manual mode
UINT32 AE_firstStable[AE_PROC_CNT] = {FALSE, FALSE};
UINT32 AE_stableLum[AE_PROC_CNT] = {1015, 1000};
UINT32 AE_stableEV[AE_PROC_CNT] = {540, 1000};
//#NT#2016/08/08#XL_Wei -end

#define AE_GET_GAMMA_VAL(Id) (Id == IPL_ID_1 ? AE_GetGammaValue_ID_1 : AE_GetGammaValue_ID_2)
#define AE_GET_INV_GAMMA_VAL(Id) (Id == IPL_ID_1 ? AE_GetInvGammaValue_ID_1 : AE_GetInvGammaValue_ID_2)

UINT32 AE_GetGammaValue_ID_1(UINT32 Data12Bit)
{
    return IQS_GetGammaValue(IPL_ID_1, Data12Bit);
}

UINT32 AE_GetGammaValue_ID_2(UINT32 Data12Bit)
{
    return IQS_GetGammaValue(IPL_ID_2, Data12Bit);
}

UINT32 AE_GetInvGammaValue_ID_1(UINT32 Data10Bit)
{
    return IQS_GetInvGammaValue(IPL_ID_1, Data10Bit);
}

UINT32 AE_GetInvGammaValue_ID_2(UINT32 Data10Bit)
{
    return IQS_GetInvGammaValue(IPL_ID_2, Data10Bit);
}

static void AE_SetIntCtrlInfo(UINT32 Id, AE_INT_CTRL_INFO CtrlInfo)
{
    AeIntCtrlInfo[Id] = CtrlInfo;
}

#if (AE_FACE_DETECTION == ENABLE)
static void AE_GetLASetInfo(IPL_LA_SET_INFOR *LAInfo)
{
    IPL_GetCmd(IPL_GET_LA_INFOR, (void *)LAInfo);
}
#endif

void AE_LoadEventTAB(AE_EVENT_TAB *EventTab)
{
    AEEventTAB = *EventTab;
}

//MovieStamp
#if AE_STAMPLOG
#include "awb_api.h"
extern UINT32 g3DNR[IPL_ID_MAX_NUM];
extern void AWB_GetColorGain(UINT32 id, UINT32 *Rg,UINT32 *Gg,UINT32 *Bg);
extern void AWB_GetStatus(UINT32 Id, AWBStatus *pAWBStatus);
extern void H264Test_GetEncInfo(INT32 puiParam[8]);
extern AUTO_WDR_SET IQS_WDR_Set[IPL_ID_MAX_NUM];

//#NT#2016/10/18#Adrian -begin
//#NT#Add QP infromation for multiple streaming
#include "VideoEncode.h"
extern INT32                   gStr_QP[VIDENC_ID_MAX];
static void AE_SetMovieLog(UINT32 Id, char* Buf, UINT32 BufLen)
{
    INT32 uiParam[8];
    UINT32 RGain, GGain, BGain;
    AWBStatus AWBStatus;

    H264Test_GetEncInfo( uiParam);
    AWB_GetColorGain(Id, &RGain, &GGain, &BGain);
    AWB_GetStatus(0, &AWBStatus);

    snprintf(Buf, BufLen, "96660 %3d %3d %3d %3d %3d %5d %4d %3d %3d %3d %3d %3d %3d %3d %3d %2d %2d %2d %4d\0",
        AeInfo[Id].PrvAvgEV.LV,
        AeInfo[Id].ExtSet.OverExp.CurCnt,
        AeInfo[Id].ExtSet.OverExp.CurTotalStep,
        AeInfo[Id].PrvAvgLum.y,
        AeInfo[Id].CurExpectLum.y,
        PrvNewAeArg[Id][AEStep[Id]].ExpoTime,
        PrvNewAeArg[Id][AEStep[Id]].ISOGain,
        g3DNR[Id], IPL_AlgGetUIInfo(Id, IPL_SEL_WDR),
        IQS_WDR_Set[Id].DarkGain,
        IQS_WDR_Set[Id].BrightGain,
        IQS_WDR_Set[Id].BlackLevel,
        IQS_WDR_Set[Id].Sharpness,
        RGain, BGain,
        gStr_QP[0],//stream0 QP
        gStr_QP[1],//stream1 QP
        gStr_QP[2],//stream2 QP
        AWBStatus.ColorTemperature
    );
}
#endif

void AE_Init (UINT32 Id, UINT32 CurrentStatus)
{
    SENSOR_MODE Mode;
    EXPOSURE_SETTING ExpTSetting = {0};
    GAIN_SETTING Gain = {0};
    static BOOL Init_Flag[AE_PROC_CNT] = {FALSE, FALSE};
    IPL_MODE_INFOR Info = {0};
    ER rt = E_OK;
    AE_EXT_SET_INFO ExtInfo = {0};
    UINT8 uiExpAreaWeight[AE_WIN_Y][AE_WIN_X];

    AeInfo[Id].Id = Id;

    AeInfo[Id].PrvBiningMode = AE_getPrvBiningMode(Id);
    AeInfo[Id].CapBiningMode = AE_getCapBiningMode(Id);
    //initial ae algorithm
    if (FALSE == Init_Flag[Id])
    {
        Init_Flag[Id] = TRUE;
        AE_getInfo(&AeInfo[Id], &AeBlkInfo[Id]);
        ExtInfo.ExpAreaWeight = &uiExpAreaWeight[0][0];
        AE_GetExtInfo(Id, &ExtInfo);
        AE_SetExtInfo(Id, ExtInfo);
        //get ae table
        AE_getPrvTab(AE_getIsoType(IPL_AlgGetUIInfo(Id, IPL_SEL_ISOINDEX)), AE_getFlickerType(IPL_AlgGetUIInfo(Id, IPL_SEL_FREQUENCY)), &AeInfo[Id]);
        //get & save iso, exposure time, iris
        AEAlg_Allot(AeInfo[Id].PrvAvgEV.EVValue, AeInfo[Id].PrvBiningMode, AeInfo[Id].PrvCurve, AeInfo[Id].EventTab.GetFnoRatio, AeInfo[Id].EventTab.GetCurveOfsRatio, &PrvNewAeArg[Id][0]);
        if (PrvNewAeArg[Id][0].ISOGain > AeParamInfo[Id].PrvISO.h) PrvNewAeArg[Id][0].ISOGain = AeParamInfo[Id].PrvISO.h;
        if (PrvNewAeArg[Id][0].ISOGain < AeParamInfo[Id].PrvISO.l) PrvNewAeArg[Id][0].ISOGain = AeParamInfo[Id].PrvISO.l;
    }
    else
    {
        //get ae table
        AE_getPrvTab(AE_getIsoType(IPL_AlgGetUIInfo(Id, IPL_SEL_ISOINDEX)), AE_getFlickerType(IPL_AlgGetUIInfo(Id, IPL_SEL_FREQUENCY)), &AeInfo[Id]);
        //get & save iso, exposure time, iris
        AEAlg_Allot(AeInfo[Id].PrvParam[0].EVValue, AeInfo[Id].PrvBiningMode, AeInfo[Id].PrvCurve, AeInfo[Id].EventTab.GetFnoRatio, AeInfo[Id].EventTab.GetCurveOfsRatio, &PrvNewAeArg[Id][0]);
        if (PrvNewAeArg[Id][0].ISOGain > AeParamInfo[Id].PrvISO.h) PrvNewAeArg[Id][0].ISOGain = AeParamInfo[Id].PrvISO.h;
        if (PrvNewAeArg[Id][0].ISOGain < AeParamInfo[Id].PrvISO.l) PrvNewAeArg[Id][0].ISOGain = AeParamInfo[Id].PrvISO.l;
    }

    AE_setFlag(Id, (AE_FLAG_SET_IRIS|AE_FLAG_SET_GAIN|AE_FLAG_SET_EXPT));
    AE_SetCalGain(Id);

    //Switch AE Function for different IPL mode.
    Info.Id = Id;
    rt = IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);
    if (rt != E_OK)
        return;

    rt = Sensor_GetCurMode(IPL_UTI_CONV2_SEN_ID(Id), &Mode);
    if (rt != E_OK)
        return;

    if (AeAppMode == AE_APPMODE_SHDR)
    {
        AeInfo[Id].ExtSet.ExpLumDiv = SensorHDR_GetAeRatio();
        if (AeInfo[Id].ExtSet.ExpLumDiv == SHDR_RATIO_GAIN_1X)   //1X HDR Ratio.
            AeInfo[Id].ExtSet.ExpLumDiv = 1;

        if ((Info.Mode == IPL_MODE_PREVIEW)||(Info.Mode == IPL_MODE_CAP))
        {
            switch (AeInfo[Id].ExtSet.ExpLumDiv)
            {
                case SHDR_RATIO_GAIN_16X:
                    AeInfo[Id].ExtSet.OverExp.AdjLumTab = ExptY_Photo_OverExpoAdj_HDR16X[Id];
                    break;

                case SHDR_RATIO_GAIN_4X:
                    AeInfo[Id].ExtSet.OverExp.AdjLumTab = ExptY_Photo_OverExpoAdj_HDR4X[Id];
                    break;

                default:
                case SHDR_RATIO_GAIN_1X:
                    AeInfo[Id].ExtSet.OverExp.AdjLumTab = ExptY_Photo_OverExpoAdj[Id];
                    break;
            }
        }
        else
        {
            switch (AeInfo[Id].ExtSet.ExpLumDiv)
            {
                case SHDR_RATIO_GAIN_16X:
                    AeInfo[Id].ExtSet.OverExp.AdjLumTab = ExptY_Mov_OverExpoAdj_HDR16X[Id];
                    break;

                case SHDR_RATIO_GAIN_4X:
                    AeInfo[Id].ExtSet.OverExp.AdjLumTab = ExptY_Mov_OverExpoAdj_HDR4X[Id];
                    break;

                default:
                case SHDR_RATIO_GAIN_1X:
                    AeInfo[Id].ExtSet.OverExp.AdjLumTab = ExptY_Mov_OverExpoAdj[Id];
                    break;
            }
        }

        ExpoTime[0] = PrvNewAeArg[Id][0].ExpoTime;
        ExpoTime[1] = PrvNewAeArg[Id][0].ExpoTime* AeInfo[Id].ExtSet.ExpLumDiv * AeInfo[Id].ExtSet.ExpLumDiv;    //bright image
        Sensor_GetExpoSetting(IPL_UTI_CONV2_SEN_ID(Id), &ExpoTime[0], Mode, &ExpTSetting, &Ratio[0]);

        ISOGain[0] = PrvNewAeArg[Id][0].ISOGain * Ratio[0] / 100;
        ISOGain[1] = (PrvNewAeArg[Id][0].ISOGain * Ratio[1] / 100);
        Sensor_GetGainSetting(IPL_UTI_CONV2_SEN_ID(Id), &ISOGain[0], &Gain);
    }
    else
    {
        AeInfo[Id].ExtSet.ExpLumDiv = 1;   // for linear mode

        ExpoTime[0] = PrvNewAeArg[Id][0].ExpoTime;
        Sensor_GetExpoSetting(IPL_UTI_CONV2_SEN_ID(Id), &ExpoTime[0], Mode, &ExpTSetting, &Ratio[0]);
        ISOGain[0] = PrvNewAeArg[Id][0].ISOGain * Ratio[0] / 100;
        Sensor_GetGainSetting(IPL_UTI_CONV2_SEN_ID(Id), &ISOGain[0], &Gain);
    }

    AE_setExpT(Id, &ExpTSetting);
    AE_setGain(Id, &Gain);

    if ((AeAppMode == AE_APPMODE_STITCH) && (Sensor_IsOpen(IPL_ID_2) == TRUE)) //Stitch Mode only Run AE_ID_1
    {
        AE_setFlag(IPL_ID_2, (AE_FLAG_SET_IRIS|AE_FLAG_SET_GAIN|AE_FLAG_SET_EXPT));
        AE_setExpT(IPL_ID_2, &ExpTSetting);
        AE_setGain(IPL_ID_2, &Gain);
    }
    //AE_setIris(Id, PrvNewAeArg[0].Iris);


    //Reset to default value
    AECnt[Id] = 0;
    AEStep[Id] = 0;
    AeInfo[Id].Counter = 0;
    AeInfo[Id].CounterRF = 0;
    memset(gGain[Id], 0xff, sizeof(gGain[Id]));
    memset(gExpTSetting[Id], 0xff, sizeof(gExpTSetting[Id]));
    AeSyncCnt[Id] = 0;
    memset(&AeSyncInfo[Id][0], 0x0, (sizeof(AE_SYNC_INFO) * AE_SYNC_TAB_SIZE));

#if AE_STAMPLOG
    IQS_SetMovieLogFP(AE_SetMovieLog);// for AE debug message log
#endif

    Note_Ae_Init(Id, (void *)&AeInfo[Id], (void *)&AeParamInfo[Id], (void *)&AeExtSetDefaultParamInfo[Id], (UINT32*)AE_Table_Ptr);
}

static void AE_SetCalGain(IPL_PROC_ID Id)
{
#if CAL_ISO
    pCAL_TAG pTag;
    CAL_ISO_FMT* pFmt;
    BOOL Cal_Status;
    UINT32 Cal_Gain = 0;
    CAL_DATA_INFO CaldataInfo = {0};
    CaldataInfo.StationID = SEC_ISO;
    CaldataInfo.SenMode = IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE);
    CaldataInfo.CalCondition[0] = CAL_CONDITION_DEFAULT;

    pTag = GetCalData(Id, CaldataInfo);

    Cal_Status = FALSE;

    if (pTag != NULL)
    {
        DBG_IND("get ISO Calibration data\r\n");
        if(pTag->CAL_STATUS == _CAL_OK)
        {
            pFmt = (CAL_ISO_FMT*)&pTag->TagData;

            if (pFmt->GainBase != 0)
            {
                Cal_Status = TRUE;
                Cal_Gain = pFmt->GainBase;
                DBG_IND("iso gain 0x%x\r\n", Cal_Gain);
            }
            else
            {
                Cal_Status = FALSE;
                DBG_ERR("read ISO Calibration gain Fail\r\n");
            }

            DBG_IND("read ISO Calibration gain = %d %d\r\n", pFmt->GainBase,IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE));
        }
        else
            DBG_IND("cal status not OK\r\n");
    }
    else
        DBG_IND("NO ISO cal data\r\n");

    Sensor_SetAGC_ISOBase(IPL_UTI_CONV2_SEN_ID(Id), Cal_Status, Cal_Gain);
#endif
}

static void AE_SetSyncInfo(UINT32 Id, AEALG_ARG *data)
{
    IPL_SIE_CUR_FRAME_INFO Info = {0};

    Info.Id = Id;
    IPL_GetCmd(IPL_GET_SIE_CUR_FRAME_INFOR, &Info);

    AeSyncInfo[Id][AeSyncCnt[Id]].frame_cnt = Info.FrameCnt + 3;
    AeSyncInfo[Id][AeSyncCnt[Id]].data = *data;

    AeSyncCnt[Id] ++;
    if (AeSyncCnt[Id] >= AE_SYNC_TAB_SIZE)
    {
        AeSyncCnt[Id] = 0;
    }
}

ER AE_GetSyncInfo(UINT32 Id, UINT32 frame_cnt, AEALG_ARG *data)
{
    UINT32 i;
    for (i = 0; i < AE_SYNC_TAB_SIZE; i ++)
    {
        if (frame_cnt == AeSyncInfo[Id][i].frame_cnt)
        {
            *data = AeSyncInfo[Id][i].data;
            return E_OK;
        }
    }
    return E_SYS;
}

void AE_Process (UINT32 Id, UINT32 CurrentStatus)
{
    UINT32 i = 0;
    AE_RESET AeRst = AE_RESET_FALSE;
    SENSOR_MODE Mode;
    static UINT32 InStrPos[AE_PROC_CNT];
    static SENSOR_INFO SenInfo;
    ER rt = E_OK;

    if(Id == IPL_ID_2)
        Delay_DelayMs(1);

    AECnt[Id] ++;

    if (AECnt[Id] < 6)
    {
        return;
    }

    //#NT#2016/04/28#Jarkko Chang -begin
    //#NT# stitch mode ae update
    if(AeAppMode == AE_APPMODE_STITCH)
    {
        if((Id != IPL_ID_1) || (Sensor_IsOpen(IPL_ID_2) == FALSE))
            return;
    }
    //#NT#2016/04/28#Jarkko Chang -end

    //Set ExpT and ISOGain by AE Uart CMD (for Debug).
    if ( uiISOGain[Id]!=0 && uiExpoTime[Id]!=0 )
    {

        PrvNewAeArg[Id][0].ExpoTime = uiExpoTime[Id];
        PrvNewAeArg[Id][0].ISOGain = uiISOGain[Id];

        if (AEUartMsg == ENABLE || uiAEUartCmd[Id] == ENABLE)
        {
            DBG_ERR("Manual_AE Id: %d, Exp: %d, ISO: %d, Iris: %d\r\n", Id, uiExpoTime[Id], uiISOGain[Id], uiIris[Id]);
        }

        rt = Sensor_GetCurMode(IPL_UTI_CONV2_SEN_ID(Id), &Mode);
        if (rt != E_OK)
            return;

        ExpoTime[0] = uiExpoTime[Id];
        ExpoTime[1] = uiExpoTime[AE_ID_2];
        Sensor_GetExpoSetting(IPL_UTI_CONV2_SEN_ID(Id), &ExpoTime[0], Mode, &gExpTSetting[Id][0], &Ratio[0]);
        ISOGain[0] = uiISOGain[Id];
        ISOGain[1] = uiISOGain[AE_ID_2];
        Sensor_GetGainSetting(IPL_UTI_CONV2_SEN_ID(Id), &ISOGain[0], &gGain[Id][0]);
        AE_setFlag(Id, AE_FLAG_SET_GAIN|AE_FLAG_SET_EXPT);
        AE_setExpT(Id, &gExpTSetting[Id][0]);
        AE_setGain(Id, &gGain[Id][0]);
        IQS_SettingFlowFunc(Id, IQS_AE_END);
        CapNewAeArg[Id].ExpoTime = uiExpoTime[Id];
        CapNewAeArg[Id].ISOGain = uiISOGain[Id];

        if (AeAppMode == AE_APPMODE_SHDR)
        {
            //Trigger SHDR Process here
            AE_HDRNotifyEvent(IPL_UTI_CONV2_AE_ID(Id));
        }

        return ;
    }

    if (AEStep[Id] == 0)
    {
        if (AECnt[Id]%2 == 0)
        {
            InStrPos[Id] = 0;
        }
        else
        {
            InStrPos[Id] = AE_TOTAL_PARAMNUM/2;
        }
        //set ae control parameter
        AE_getCtrlInfo(&AeInfo[Id], &AeBlkInfo[Id]);
        //set ae window
        AeRst = AE_setWin(Id, AE_getWinType(IPL_AlgGetUIInfo(Id, IPL_SEL_AEMODE)));
        //get preview ae curve
        AeRst |= AE_getPrvTab(AE_getIsoType(IPL_AlgGetUIInfo(Id, IPL_SEL_ISOINDEX)), AE_getFlickerType(IPL_AlgGetUIInfo(Id, IPL_SEL_FREQUENCY)), &AeInfo[Id]);
        //get manual ae curve for debug
        AeRst |= AE_getPrvManualTab(uiAEManualCmd[Id], uiAEManualISOCmd[Id], &AeInfo[Id]);
        //get capture ae curve
        AeRst |= AE_getCapTab(AE_getIsoType(IPL_AlgGetUIInfo(Id, IPL_SEL_ISOINDEX)), AE_getFlickerType(IPL_AlgGetUIInfo(Id, IPL_SEL_FREQUENCY)), AE_getSceneType(IPL_AlgGetUIInfo(Id, IPL_SEL_SCENEMODE)), &AeInfo[Id]);
        //get manual ae curve for debug
        AeRst |= AE_getCapManualTab(uiAEManualCmd[Id], uiAEManualISOCmd[Id], &AeInfo[Id]);
        AeInfo[Id].Reset = AeRst;

        // get statistic informaiton
        AE_getStatisticInfo(&AeInfo[Id], &AeBlkInfo[Id]);

        //comp ev
        AeInfo[Id].EVCompRatio = EVTable[IPL_AlgGetUIInfo(Id, IPL_SEL_AEEV)];

        rt = Sensor_GetCurMode(IPL_UTI_CONV2_SEN_ID(Id), &Mode);
        //for sensor hdr
        rt |= Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE), &SenInfo);
        if (rt != E_OK || SenInfo.Mode == NULL)
            return;

        if (AeAppMode == AE_APPMODE_SHDR)
        {
            AeInfo[Id].ExtSet.ExpLumDiv = SensorHDR_GetAeRatio();
            if (AeInfo[Id].ExtSet.ExpLumDiv == SHDR_RATIO_GAIN_1X)   //1X HDR Ratio.
                AeInfo[Id].ExtSet.ExpLumDiv = 1;

            AEAlg_Converge(&AeInfo[Id], AeIntCtrlInfo[Id].PredictPoint, &PrvNewAeArg[Id][0], &CapNewAeArg[Id]);

            for (i = 0; i < AeIntCtrlInfo[Id].PredictPoint; i ++)
            {
                ExpoTime[0] = PrvNewAeArg[Id][i].ExpoTime;  //dark image
                ExpoTime[1] = PrvNewAeArg[Id][i].ExpoTime* AeInfo[Id].ExtSet.ExpLumDiv * AeInfo[Id].ExtSet.ExpLumDiv;    //bright image
                Sensor_GetExpoSetting(IPL_UTI_CONV2_SEN_ID(Id), &ExpoTime[0], Mode, &gExpTSetting[Id][InStrPos[Id]+i+AeIntCtrlInfo[Id].CalSetShiftNum], &Ratio[0]);

                ISOGain[1] = (PrvNewAeArg[Id][i].ISOGain * Ratio[1] / 100);
                PrvNewAeArg[Id][i].ISOGain = (PrvNewAeArg[Id][i].ISOGain * Ratio[0] / 100);
                ISOGain[0] = PrvNewAeArg[Id][i].ISOGain;

                Sensor_GetGainSetting(IPL_UTI_CONV2_SEN_ID(Id), &ISOGain[0], &gGain[Id][(InStrPos[Id] + i + SenInfo.SyncTiming+AeIntCtrlInfo[Id].CalSetShiftNum)%AE_TOTAL_PARAMNUM]);
            }

            //update gt gain
            if (AeInfo[Id].PrvAvgLum.raw == 0)
                SHDR_GT_Gain = (AeInfo[Id].CurExpectLum.raw*AeInfo[Id].ExtSet.ExpLumDiv*1024);
            else
                SHDR_GT_Gain = (AeInfo[Id].CurExpectLum.raw*AeInfo[Id].ExtSet.ExpLumDiv*1024)/AeInfo[Id].PrvAvgLum.raw;
        }
        else
        {
            AEAlg_Converge(&AeInfo[Id], AeIntCtrlInfo[Id].PredictPoint, &PrvNewAeArg[Id][0], &CapNewAeArg[Id]);

            for (i = 0; i < AeIntCtrlInfo[Id].PredictPoint; i ++)
            {
                ExpoTime[0] = PrvNewAeArg[Id][i].ExpoTime;
                Sensor_GetExpoSetting(IPL_UTI_CONV2_SEN_ID(Id), &ExpoTime[0], Mode, &gExpTSetting[Id][InStrPos[Id]+i+AeIntCtrlInfo[Id].CalSetShiftNum], &Ratio[0]);
                PrvNewAeArg[Id][i].ISOGain = (PrvNewAeArg[Id][i].ISOGain * Ratio[0] / 100);
                ISOGain[0] = PrvNewAeArg[Id][i].ISOGain;
                Sensor_GetGainSetting(IPL_UTI_CONV2_SEN_ID(Id), &ISOGain[0], &gGain[Id][(InStrPos[Id] + i + SenInfo.SyncTiming+AeIntCtrlInfo[Id].CalSetShiftNum)%AE_TOTAL_PARAMNUM]);
            }

            if (IPL_AlgGetUIInfo(Id, IPL_SEL_IRCUT) == SEL_IRCUT_AUTO)
                AE_SetIRCut(Id, AeInfo[Id].PrvAvgEV.LV);
        }
    }
    else if (AEStep[Id] == 1)
    {
        if (AEUartMsg == ENABLE || uiAEUartCmd[Id] == ENABLE)
            AE_getStatisticInfo(&AeInfo[Id], &AeBlkInfo[Id]);
    }
    else if (AEStep[Id] == 2)
    {
        if (AEUartMsg == ENABLE || uiAEUartCmd[Id] == ENABLE)
            AE_getStatisticInfo(&AeInfo[Id], &AeBlkInfo[Id]);
    }

    if ((AeAppMode == AE_APPMODE_STITCH) && (Sensor_IsOpen(IPL_ID_2) == TRUE)) //Stitch Mode only Run AE_ID_1
    {
        //#NT#2016/05/03#Jarkko Chang -begin
        //#NT# fixed AE_APPMODE_STITCH mode frame sync issue
        AeInfo[IPL_ID_2] = AeInfo[Id] ;
        //#NT#2016/05/03#Jarkko Chang -end

        gExpTSetting[IPL_ID_2][InStrPos[Id]+AEStep[Id]] = gExpTSetting[Id][InStrPos[Id]+AEStep[Id]];
        gGain[IPL_ID_2][InStrPos[Id]+AEStep[Id]] = gGain[Id][InStrPos[Id]+AEStep[Id]];

        AE_setFlag(IPL_ID_2, AE_FLAG_SET_GAIN|AE_FLAG_SET_EXPT);

        AE_setExpT(IPL_ID_2, &gExpTSetting[IPL_ID_2][InStrPos[Id]+AEStep[Id]]);
        AE_setGain(IPL_ID_2, &gGain[IPL_ID_2][InStrPos[Id]+AEStep[Id]]);

        AE_SetSyncInfo(IPL_ID_2, &PrvNewAeArg[Id][AEStep[Id]]);
        IQS_SettingFlowFunc(IPL_ID_2, IQS_AE_END);
    }

    AE_setFlag(Id, AE_FLAG_SET_GAIN|AE_FLAG_SET_EXPT);
    AE_setExpT(Id, &gExpTSetting[Id][InStrPos[Id]+AEStep[Id]]);
    AE_setGain(Id, &gGain[Id][InStrPos[Id]+AEStep[Id]]);

    AE_SetSyncInfo(Id, &PrvNewAeArg[Id][AEStep[Id]]);
    IQS_SettingFlowFunc(Id, IQS_AE_END);

    if ((AEStep[Id] == 0) && (AeAppMode == AE_APPMODE_SHDR))
    {
        //Trigger SHDR Process here
        AE_HDRNotifyEvent(IPL_UTI_CONV2_AE_ID(Id));
    }

#if 0
    if (uiAEOSDCmd[Id] == ENABLE)
    {
        extern int UI_ForceShowString(CHAR * pStr, PURECT pRect, BOOL bClear);
        CHAR msg[100];
        URECT rect = {10, 30, 640, 20};
        snprintf((char *)msg, sizeof(msg), "%ld %3d %3d 06.16 ", g3DNR,  PrvNewAeArg[Id][AEStep[Id]].ISOGain,PrvNewAeArg[Id][AEStep[Id]].ExpoTime);
//vNewAeArg[Id][AEStep[Id]].ExpoTime,

        UI_ForceShowString(msg, &rect, 1);
    }
#endif

    if (AEUartMsg == ENABLE || uiAEUartCmd[Id] == ENABLE)
    {
        DBG_DUMP("%d: %3d %3d %3d %8d %8d %3d %3d %3d %3d %3d\r\n", Id, AeInfo[Id].PrvAvgLum.y, AeInfo[Id].CurExpectLum.y, AeInfo[Id].CurLum.y, AeInfo[Id].PrvAvgEV.EVValue, PrvNewAeArg[Id][AEStep[Id]].ExpoTime, PrvNewAeArg[Id][AEStep[Id]].ISOGain, IPL_AlgGetUIInfo(Id, IPL_SEL_WDR), AeInfo[Id].Counter, AeInfo[Id].CounterRF, AEStep[Id]);
    }

    AEStep[Id] = (AEStep[Id]+1)%AeIntCtrlInfo[Id].AEPeroid;  //AE Peroid
}

UINT32 AE_GetGTGain(UINT32 Id)
{
    return SHDR_GT_Gain;
}

void AE_GetLA(UINT32 Id, UINT16 *Lum1, UINT16 *Lum2, UINT16 *Histo)
{
    IPL_LA_INFOR LAInfo;

    LAInfo.Id = Id;
    LAInfo.Lum1 = Lum1;
    LAInfo.Lum2 = Lum2;
    LAInfo.Histo = Histo;

    IPL_GetCmd(IPL_GET_CUR_LA_INFOR, (void *)&LAInfo);
}

//#NT#2016/10/12#Jarkko Chang -begin
//#NT# avoid IPL_ID_3/IPL_ID_4 call ae api
void AE_GetPrvAEArg(UINT32 Id, UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris)
{
	if (Id >= AE_PROC_CNT)
	{
		*ExpT = 0;
		*ISO = 0;
		*Iris = 0;
	}
	else
	{
	    if ((Id == AE_ID_2) && (AeAppMode == AE_APPMODE_STITCH)) //Stitch Mode only Run AE_ID_1
	    {
	        *ExpT = PrvNewAeArg[AE_ID_1][0].ExpoTime;
	        *ISO = PrvNewAeArg[AE_ID_1][0].ISOGain;
	        *Iris = PrvNewAeArg[AE_ID_1][0].Iris;
	    }
	    else
	    {
	        *ExpT = PrvNewAeArg[Id][0].ExpoTime;
	        *ISO = PrvNewAeArg[Id][0].ISOGain;
	        *Iris = PrvNewAeArg[Id][0].Iris;
	    }
	}
}

void AE_GetCapAEArg(UINT32 Id, UINT32 *ISO, UINT32 *ExpT, UINT32 *Iris)
{
	if (Id >= AE_PROC_CNT)
	{
		*ExpT = 0;
		*ISO = 0;
		*Iris = 0;
	}
	else
	{
	    if ((Id == AE_ID_2) && (AeAppMode == AE_APPMODE_STITCH)) //Stitch Mode only Run AE_ID_1
	    {
	        if ( uiExpoTime[Id]==0 )
	        {
	            *ExpT = CapNewAeArg[AE_ID_1].ExpoTime;
	            *ISO = CapNewAeArg[AE_ID_1].ISOGain;
	            *Iris = CapNewAeArg[AE_ID_1].Iris;
	        }
	        else
	        {
	            *ExpT = uiExpoTime[AE_ID_1];
	            *ISO = uiISOGain[AE_ID_1];
	            *Iris = CapNewAeArg[AE_ID_1].Iris;
	        }

	        AeCapInfo[Id] = AeInfo[AE_ID_1];
	        PrvLastAeArg[Id][0] = PrvNewAeArg[AE_ID_1][0];
	        PrvLastAeArg[Id][1] = PrvNewAeArg[AE_ID_1][1];
	        PrvLastAeArg[Id][2] = PrvNewAeArg[AE_ID_1][2];
	    }
	    else
	    {
	        if ( uiExpoTime[Id]==0 )
	        {
	            *ExpT = CapNewAeArg[Id].ExpoTime;
	            *ISO = CapNewAeArg[Id].ISOGain;
	            *Iris = CapNewAeArg[Id].Iris;
	        }
	        else
	        {
	            *ExpT = uiExpoTime[Id];
	            *ISO = uiISOGain[Id];
	            *Iris = CapNewAeArg[Id].Iris;
	        }

	        AeCapInfo[Id] = AeInfo[Id];
	        PrvLastAeArg[Id][0] = PrvNewAeArg[Id][0];
	        PrvLastAeArg[Id][1] = PrvNewAeArg[Id][1];
	        PrvLastAeArg[Id][2] = PrvNewAeArg[Id][2];
	    }
	}
}
//#NT#2016/10/12#Jarkko Chang -end

void AE_GetWin(UINT32 Id, UINT8 *win)
{
    UINT32 i, j, idx;

    for (j = 0; j < AE_WIN_Y; j ++)
    {
        idx = (j * AE_WIN_X);
        for (i = 0; i < AE_WIN_X; i ++)
        {
            win[idx] = AeWin[j][i];
            idx += 1;
        }
    }
}

void AE_GetStatus(UINT32 Id, AEStatus *pAEStatus)
{
    pAEStatus->ExpT = PrvNewAeArg[Id][AEStep[Id]].ExpoTime;
    pAEStatus->Gain = PrvNewAeArg[Id][AEStep[Id]].ISOGain;
    pAEStatus->LumaValue = AeBlkInfo[Id].Yacc;
    pAEStatus->LumaAverage = AeInfo[Id].PrvAvgLum.y;
    pAEStatus->LumaHistogram = AeInfo[Id].histogram.y;
}

void AE_WaitStableRF(UINT32 Id, UINT32 StableCnt)
{
    UINT32 TimeOutCnt = 0;
    UINT32 TimeOutFrameCnt = 100;//0xfffff;

    if (StableCnt == 0)
        StableCnt = TimeOutFrameCnt;

    while(TimeOutCnt < TimeOutFrameCnt)
    {
        if(AeInfo[Id].CounterRF > StableCnt)
        {
            return;
        }
        else
            TimeOutCnt ++;

        Delay_DelayMs(3);
    }

    DBG_MSG("AE Stable Time Out but not stable %d\r\n", AeInfo[Id].CounterRF);
}

UINT32 AE_WriteDebugInfo(UINT32 Id, UINT32 Addr)
{
//    UINT32 i,j;
    UINT32 buf_cnt;

    buf_cnt = snprintf(NULL, 0, "\r\n\r\n\r\nAE Info------------------------------------------------------------------------------------------------------------------------\r\n");
    Addr += snprintf((char *)Addr, buf_cnt+1, "\r\n\r\n\r\nAE Info------------------------------------------------------------------------------------------------------------------------\r\n");

    buf_cnt = snprintf(NULL, 0,  "ExpectY = \t%ld\r\nAdd_Y = \t%ld\r\nLumY =  \t%ld\r\nStableCnt = \t%ld\r\n",
                                            AeCapInfo[Id].CurExpectLum.y, AeCapInfo[Id].ExtSet.OverExp.CurTotalStep, AeCapInfo[Id].CurLum.y, AeCapInfo[Id].Counter);
    Addr += snprintf((char *)Addr, buf_cnt+1, "ExpectY = \t%ld\r\nAdd_Y = \t%ld\r\nLumY =  \t%ld\r\nStableCnt = \t%ld\r\n",
                                            AeCapInfo[Id].CurExpectLum.y, AeCapInfo[Id].ExtSet.OverExp.CurTotalStep, AeCapInfo[Id].CurLum.y, AeCapInfo[Id].Counter);

    buf_cnt = snprintf(NULL, 0, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
                                            AeCapInfo[Id].PrvAvgEV.EVValue, AeCapInfo[Id].PrvAvgEV.LV);
    Addr += snprintf((char *)Addr, buf_cnt+1, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
                                            AeCapInfo[Id].PrvAvgEV.EVValue, AeCapInfo[Id].PrvAvgEV.LV);

    buf_cnt = snprintf(NULL, 0, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
                                            AeCapInfo[Id].PrvParam[0].EVValue, AeCapInfo[Id].PrvParam[0].LV);
    Addr += snprintf((char *)Addr, buf_cnt+1, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
                                            AeCapInfo[Id].PrvParam[0].EVValue, AeCapInfo[Id].PrvParam[0].LV);

    buf_cnt = snprintf(NULL, 0, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
                                            AeCapInfo[Id].PrvParam[1].EVValue, AeCapInfo[Id].PrvParam[1].LV);
    Addr += snprintf((char *)Addr, buf_cnt+1, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
                                            AeCapInfo[Id].PrvParam[1].EVValue, AeCapInfo[Id].PrvParam[1].LV);

    buf_cnt = snprintf(NULL, 0, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
                                            AeCapInfo[Id].PrvParam[2].EVValue, AeCapInfo[Id].PrvParam[2].LV);
    Addr += snprintf((char *)Addr, buf_cnt+1, "PrvEV_Value = \t%ld\r\nPrvLV = \t%ld\r\n",
                                            AeCapInfo[Id].PrvParam[2].EVValue, AeCapInfo[Id].PrvParam[2].LV);

    buf_cnt = snprintf(NULL, 0, "CapEV_Value = \t%ld\r\nCapLV = \t%ld\r\n",
                                            AeCapInfo[Id].CapParam.EVValue, AeCapInfo[Id].CapParam.LV);
    Addr += snprintf((char *)Addr, buf_cnt+1, "CapEV_Value = \t%ld\r\nCapLV = \t%ld\r\n",
                                            AeCapInfo[Id].CapParam.EVValue, AeCapInfo[Id].CapParam.LV);

    buf_cnt = snprintf(NULL, 0, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
                                            PrvLastAeArg[Id][0].ISOGain, PrvLastAeArg[Id][0].ExpoTime, PrvLastAeArg[Id][0].Iris);
    Addr += snprintf((char *)Addr, buf_cnt+1, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
                                            PrvLastAeArg[Id][0].ISOGain, PrvLastAeArg[Id][0].ExpoTime, PrvLastAeArg[Id][0].Iris);

    buf_cnt = snprintf(NULL, 0, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
                                            PrvLastAeArg[Id][1].ISOGain, PrvLastAeArg[Id][1].ExpoTime, PrvLastAeArg[Id][1].Iris);
    Addr += snprintf((char *)Addr, buf_cnt+1, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
                                            PrvLastAeArg[Id][1].ISOGain, PrvLastAeArg[Id][1].ExpoTime, PrvLastAeArg[Id][1].Iris);

    buf_cnt = snprintf(NULL, 0, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
                                            PrvLastAeArg[Id][2].ISOGain, PrvLastAeArg[Id][2].ExpoTime, PrvLastAeArg[Id][2].Iris);
    Addr += snprintf((char *)Addr, buf_cnt+1, "Prv_ISOGain = \t%ld\r\nPrv_ExpoTime = \t%ld\r\nPrv_Iris = \t%ld\r\n",
                                            PrvLastAeArg[Id][2].ISOGain, PrvLastAeArg[Id][2].ExpoTime, PrvLastAeArg[Id][2].Iris);

    buf_cnt = snprintf(NULL, 0, "Cap_ISOGain = \t%ld\r\nCap_ExpoTime = \t%ld\r\nCap_Iris = \t%ld\r\n",
                                            CapNewAeArg[Id].ISOGain, CapNewAeArg[Id].ExpoTime, CapNewAeArg[Id].Iris);
    Addr += snprintf((char *)Addr, buf_cnt+1, "Cap_ISOGain = \t%ld\r\nCap_ExpoTime = \t%ld\r\nCap_Iris = \t%ld\r\n",
                                            CapNewAeArg[Id].ISOGain, CapNewAeArg[Id].ExpoTime, CapNewAeArg[Id].Iris);

    buf_cnt = snprintf(NULL, 0 , "UI setting: Metermode=%d, ISO=%d, EV bios = %d, Flash=%d, Scene=%d, Freq=%d, ImgEffect=%d, WB=%d, WDR=%d, SHDR=%d\r\n",
                                            IPL_AlgGetUIInfo(Id, IPL_SEL_AEMODE), IPL_AlgGetUIInfo(Id, IPL_SEL_ISOINDEX), IPL_AlgGetUIInfo(Id, IPL_SEL_AEEV), IPL_AlgGetUIInfo(Id, IPL_SEL_FLASHMODE)
                                            ,IPL_AlgGetUIInfo(Id, IPL_SEL_SCENEMODE),IPL_AlgGetUIInfo(Id, IPL_SEL_FREQUENCY),IPL_AlgGetUIInfo(Id, IPL_SEL_IMAGEEFFECT),IPL_AlgGetUIInfo(Id, IPL_SEL_WBMODE),IPL_AlgGetUIInfo(Id, IPL_SEL_WDR),IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR));
    Addr += snprintf((char *)Addr, buf_cnt+1, "UI setting: Metermode=%d, ISO=%d, EV bios = %d, Flash=%d, Scene=%d, Freq=%d, ImgEffect=%d, WB=%d, WDR=%d, SHDR=%d\r\n",
                                            IPL_AlgGetUIInfo(Id, IPL_SEL_AEMODE), IPL_AlgGetUIInfo(Id, IPL_SEL_ISOINDEX), IPL_AlgGetUIInfo(Id, IPL_SEL_AEEV), IPL_AlgGetUIInfo(Id, IPL_SEL_FLASHMODE)
                                            ,IPL_AlgGetUIInfo(Id, IPL_SEL_SCENEMODE),IPL_AlgGetUIInfo(Id, IPL_SEL_FREQUENCY),IPL_AlgGetUIInfo(Id, IPL_SEL_IMAGEEFFECT),IPL_AlgGetUIInfo(Id, IPL_SEL_WBMODE),IPL_AlgGetUIInfo(Id, IPL_SEL_WDR),IPL_AlgGetUIInfo(Id, IPL_SEL_SHDR));

    Addr += snprintf((char *)Addr, 256, "------------------------------\r\n");
    Addr = AE_BlkWriteDebugInfo(Addr);
    Addr += snprintf((char *)Addr, 256, "------------------------------\r\n");
#if 0
    for(i=0;i<AE_HIST_NUM;i++)
    {
        Addr += snprintf((char *)Addr, sizeof(AeInfo[Id]), "%d\r\n", AeInfo[Id].histogram.raw[i]);
    }
    Addr += snprintf((char *)Addr, sizeof(AeInfo[Id]), "\r\n");

    for(i=0;i<AE_HIST_NUM;i++)
    {
        Addr += snprintf((char *)Addr, sizeof(AeInfo[Id]), "%d\r\n", AeInfo[Id].histogram.y[i]);
    }
#endif

//#NT#2016/11/01#Jarkko Chang -begin
//#NT# reduce unused code
#if 0
    for(j=0;j<LA_WIN_Y;j++)
    {
        for(i=0;i<LA_WIN_X;i++)
        {
            Addr += snprintf((char *)Addr, sizeof(AeBlkInfo[Id]), "%d\t", AeBlkInfo[Id].Gacc[j*LA_WIN_X+i]>>4);
        }
        Addr += snprintf((char *)Addr, sizeof(AeInfo[Id]), "\r\n");
    }
#endif
//#NT#2016/11/01#Jarkko Chang -end
    return Addr;
}

UINT32 AE_GetPrvLVValue(UINT32 Id)
{
        return AeInfo[Id].PrvAvgEV.LV;
}

UINT32 AE_GetCapLVValue(UINT32 Id)
{
    return AeInfo[Id].CapParam.LV;
}

UINT32 AE_GetPrvEVValue(UINT32 Id)
{
        return AeInfo[Id].PrvAvgEV.EVValue;
}

UINT32 AE_GetCapEVValue(UINT32 Id)
{
    return AeInfo[Id].CapParam.EVValue;
}

void AE_GetLAInfo(UINT32 Id, AE_LAINFO* LAInfo)
{
    LAInfo->WinNumX = LA_WIN_X;
    LAInfo->WinNumY = LA_WIN_Y;
}

UINT32 AE_GetCurRawLum(UINT32 Id, UINT32 LumBit)
{
    UINT32 Lum;

    if(LumBit < 12)
        Lum = AeInfo[Id].PrvAvgLum.raw >> (12-LumBit);
    else
        Lum = AeInfo[Id].PrvAvgLum.raw << (LumBit - 12);

    return Lum;
}

BOOL AE_GetIRCutEnable(UINT32 Id)
{
    return IR_CUT_ENABLE[Id];
}

AE_BOUNDARY AE_GetLVBoundary(UINT32 Id)
{
    AE_BOUNDARY Lv;

    Lv.h = AEAlg_Conv2LV(AeInfo[Id].EV.h);
    Lv.l = AEAlg_Conv2LV(AeInfo[Id].EV.l);

    return Lv;
}

UINT32 AE_GetPrvOverExpAdjValue(UINT32 Id)
{
    return AeInfo[Id].ExtSet.OverExp.CurTotalStep;
}

void AE_SetAppMode(AE_APPMODE AppMode)
{
    AeAppMode = AppMode;
}

#if 0
#endif
//------------------------------------------------------------------------------
UINT32 AE_judgePrvSetting(AEALG_ARG *CurInfo, AEALG_ARG *PrevInfo, UINT32 *Flag)
{
    UINT32 JudgeFlag = FALSE;

    *Flag = 0;

    if (PrevInfo->ExpoTime != CurInfo->ExpoTime)
    {
        *Flag |= AE_FLAG_SET_EXPT;
        JudgeFlag = TRUE;
    }

    if (PrevInfo->ISOGain != CurInfo->ISOGain)
    {
        *Flag |= AE_FLAG_SET_GAIN;
        JudgeFlag = TRUE;
    }

    if (PrevInfo->Iris != CurInfo->Iris)
    {
        *Flag |= AE_FLAG_SET_IRIS;
        JudgeFlag = TRUE;
    }

    return JudgeFlag;
}

UINT32 AE_getStatisticInfo(AE_INFO *AeInfo, AE_BLK_INFO *AeBlkInfo)
{
    UINT32 i, j;
    UINT32 GTotal, YTotal, WTotal;
    UINT32 Ratio_X, Ratio_Y;

    //initial
    GTotal = 0;
    WTotal = 0;
    YTotal = 0;
    memset(AeBlkInfo->Yacc, 0, LA_WIN_X*LA_WIN_Y*2);
    memset(AeBlkInfo->Gacc, 0, LA_WIN_X*LA_WIN_Y*2);
    memset(AeInfo->histogram.y, 0, AE_HIST_NUM*2);
    memset(weighting_histogram.y, 0, AE_HIST_NUM*2);

    Ratio_X = LA_WIN_X/AE_WIN_X;
    Ratio_Y = LA_WIN_Y/AE_WIN_Y;

    if ((AeAppMode == AE_APPMODE_STITCH) && (Sensor_IsOpen(IPL_ID_2) == TRUE)) //Stitch Mode only Run AE_ID_1
    {
        Ratio_X *= 2;   //for extend the ae weighting table

        AE_GetLA(AeStichMapId[AE_STICH_LEFT_POS], AeBlkInfo->Gacc, AeBlkInfo->Yacc, AeInfo->histogram.y);

        //calculate G/Y Lum with AE Weighting Window
        for (j = 0; j < LA_WIN_Y; j ++)
        {
            for (i = 0; i < LA_WIN_X; i ++)
            {
                GTotal += (AeWin[j/Ratio_Y][i/Ratio_X] * AeBlkInfo->Gacc[j * LA_WIN_X + i]);
                YTotal += (AeWin[j/Ratio_Y][i/Ratio_X] * AeBlkInfo->Yacc[j * LA_WIN_X + i]);
                WTotal += AeWin[j/Ratio_Y][i/Ratio_X];
            }
        }

        AE_GetLA(AeStichMapId[AE_STICH_RIGHT_POS], AeBlkInfo->Gacc, AeBlkInfo->Yacc, AeInfo->histogram.y);

        //calculate G/Y Lum with AE Weighting Window
        for (j = 0; j < LA_WIN_Y; j ++)
        {
            for (i = 0; i < LA_WIN_X; i ++)
            {
                GTotal += (AeWin[j/Ratio_Y][AE_WIN_X/2+i/Ratio_X] * AeBlkInfo->Gacc[j * LA_WIN_X + i]);
                YTotal += (AeWin[j/Ratio_Y][AE_WIN_Y/2+i/Ratio_X] * AeBlkInfo->Yacc[j * LA_WIN_X + i]);
                WTotal += AeWin[j/Ratio_Y][i/Ratio_X];
            }
        }

        AeInfo->PrvAvgLum.y = (YTotal / WTotal);
        AeInfo->PrvAvgLum.raw = (GTotal / WTotal);
    }
    else
    {
        AE_GetLA(AeInfo->Id, AeBlkInfo->Gacc, AeBlkInfo->Yacc, AeInfo->histogram.y);
        UINT32 HistIntervalY = (1 << 10) / weighting_histogram.bin_num;
        //calculate G/Y Lum with AE Weighting Window
        weighting_histogram.data_num = 0;
        for (j = 0; j < LA_WIN_Y; j ++)
        {
            for (i = 0; i < LA_WIN_X; i ++)
            {
                if (AeAppMode == AE_APPMODE_SHDR)
                {
                    AeBlkInfo->Yacc[j * LA_WIN_X + i] = IQS_GetGammaValue(AeInfo->Id, (AeBlkInfo->Yacc[j * LA_WIN_X + i] << 2));
                }

                GTotal += (AeWin[j/Ratio_Y][i/Ratio_X] * AeBlkInfo->Gacc[j * LA_WIN_X + i]);
                YTotal += (AeWin[j/Ratio_Y][i/Ratio_X] * AeBlkInfo->Yacc[j * LA_WIN_X + i]);
                WTotal += AeWin[j/Ratio_Y][i/Ratio_X];
                weighting_histogram.y[(AeBlkInfo->Yacc[j * LA_WIN_X + i]/HistIntervalY)] += AE_OverExpo_Win_Normal[j/Ratio_Y][i/Ratio_X];
                weighting_histogram.data_num += (AE_OverExpo_Win_Normal[j/Ratio_Y][i/Ratio_X]);
            }
        }

        if (AeBlkInfo->BLKLum_En == ENABLE)
        {
            AeBlkInfo->LV = AeInfo->PrvParam[0].LV;
            AeInfo->PrvAvgLum = AEAlg_GetBLKBasedLum(AeBlkInfo);
        }
        else
        {
            AeInfo->PrvAvgLum.y = (YTotal / WTotal);
            AeInfo->PrvAvgLum.raw = (GTotal / WTotal);
        }
    }

    return 0;
}

void AE_getCtrlInfo(AE_INFO *AeInfo, AE_BLK_INFO *AeBlkInfo)
{
    static IPL_MODE IplMode[AE_PROC_CNT] = {IPL_MODE_NONE, IPL_MODE_NONE};
    static AE_APPMODE AppMode = AE_APPMODE_END;
    IPL_MODE_INFOR Info = {0};
    AE_INT_CTRL_INFO AeCtrlInfo = {AE_NOR_CAL_SET_SHIFT, AE_NOR_PREDICT_POINT, AE_NOR_PEROID};
    UINT32 VideoFps;
    ER rt = E_OK;

    Info.Id = AeInfo->Id;
    rt = IPL_GetCmd(IPL_GET_CUR_MODE, (void *)&Info);
    if (rt != E_OK)
        return;

    AeInfo->ConvSpeed = AeExtSetDefaultParamInfo[AeInfo->Id].ConvSpeed;
    AeInfo->StableRange = AeExtSetDefaultParamInfo[AeInfo->Id].ConvRange;
    AeInfo->ExtSet.UnStableDelay = AeExtSetDefaultParamInfo[AeInfo->Id].DelayCnt;
    AeInfo->ExtSet.AntiOverExpEn = AeExtSetDefaultParamInfo[AeInfo->Id].AntiOverExpEn;

    if (IplMode[AeInfo->Id] == Info.Mode && AeAppMode == AppMode)
        return;

    if (AeAppMode == AE_APPMODE_SHDR)
    {
        AeInfo->ExtSet.AdjHistoEn = DISABLE;
        AeBlkInfo->BLKLum_En = DISABLE;

        AeInfo->ExtSet.ExpLumDiv = SensorHDR_GetAeRatio();
        if (AeInfo->ExtSet.ExpLumDiv == SHDR_RATIO_GAIN_1X)   //1X HDR Ratio.
            AeInfo->ExtSet.ExpLumDiv = 1;

        if ((Info.Mode == IPL_MODE_PREVIEW)||(Info.Mode == IPL_MODE_CAP))
        {
            AeInfo->ExtSet.PrvLumRatioTab = NULL;
            AeInfo->PrvExpectLum.y = 40<<2;//AeParamInfo[AeInfo->Id].ExpLum_Photo;
            AeInfo->PrvExpectLum.raw = IQS_GetInvGammaValue(AeInfo->Id, AeInfo->PrvExpectLum.y);

            switch (AeInfo->ExtSet.ExpLumDiv)
            {
                case SHDR_RATIO_GAIN_16X:
                    AeInfo->ExtSet.OverExp.AdjLumTab = ExptY_Photo_OverExpoAdj_HDR16X[AeInfo->Id];
                    break;

                case SHDR_RATIO_GAIN_4X:
                    AeInfo->ExtSet.OverExp.AdjLumTab = ExptY_Photo_OverExpoAdj_HDR4X[AeInfo->Id];
                    break;

                default:
                case SHDR_RATIO_GAIN_1X:
                    AeInfo->ExtSet.OverExp.AdjLumTab = ExptY_Photo_OverExpoAdj[AeInfo->Id];
                    break;
            }
        }
        else
        {
            AeInfo->ExtSet.PrvLumRatioTab = NULL;
            AeInfo->PrvExpectLum.y = 40<<2;//AeParamInfo[AeInfo->Id].ExpLum_Mov;
            AeInfo->PrvExpectLum.raw = IQS_GetInvGammaValue(AeInfo->Id, AeInfo->PrvExpectLum.y);

            switch (AeInfo->ExtSet.ExpLumDiv)
            {
                case SHDR_RATIO_GAIN_16X:
                    AeInfo->ExtSet.OverExp.AdjLumTab = ExptY_Mov_OverExpoAdj_HDR16X[AeInfo->Id];
                    break;

                case SHDR_RATIO_GAIN_4X:
                    AeInfo->ExtSet.OverExp.AdjLumTab = ExptY_Mov_OverExpoAdj_HDR4X[AeInfo->Id];
                    break;

                default:
                case SHDR_RATIO_GAIN_1X:
                    AeInfo->ExtSet.OverExp.AdjLumTab = ExptY_Mov_OverExpoAdj[AeInfo->Id];
                    break;
            }
        }
    }
    else
    {
        if ((Info.Mode == IPL_MODE_PREVIEW)||(Info.Mode == IPL_MODE_CAP))
        {
            AeBlkInfo->BLKLum_En = DISABLE;
            AeInfo->ExtSet.AdjHistoEn = DISABLE;
            AeInfo->ExtSet.PrvLumRatioTab = ExptY_Photo_Prv[AeInfo->Id];
            AeInfo->ExtSet.OverExp.AdjLumTab = ExptY_Photo_OverExpoAdj[AeInfo->Id];

            AeInfo->PrvExpectLum.y= AeParamInfo[AeInfo->Id].ExpLum_Photo;
            AeInfo->PrvExpectLum.raw = IQS_GetInvGammaValue(AeInfo->Id, AeInfo->PrvExpectLum.y);
        }
        else
        {
            AeBlkInfo->BLKLum_En = DISABLE;
            AeInfo->ExtSet.AdjHistoEn = DISABLE;
            AeInfo->ExtSet.PrvLumRatioTab = ExptY_Mov_Prv[AeInfo->Id];
            AeInfo->ExtSet.OverExp.AdjLumTab = ExptY_Mov_OverExpoAdj[AeInfo->Id];

            AeInfo->PrvExpectLum.y = AeParamInfo[AeInfo->Id].ExpLum_Mov;
            AeInfo->PrvExpectLum.raw = IQS_GetInvGammaValue(AeInfo->Id, AeInfo->PrvExpectLum.y);

            //Hi Speed AE Mode
            //Only Set 1 time for one AE Peroid
            if (Info.Mode == IPL_MODE_VIDEOREC)
            {

                VideoFps = IPL_AlgGetUIInfo(AeInfo->Id, IPL_SEL_VIDEO_FPS);
                if (VideoFps >= SEL_VIDEOFPS_100)
                {
                    AeCtrlInfo.CalSetShiftNum = AE_HISPED_CAL_SET_SHIFT;
                    AeCtrlInfo.PredictPoint = AE_HISPED_PREDICT_POINT;
                    AeCtrlInfo.AEPeroid = AE_HISPED_PEROID;
                }
            }
        }

        AeInfo->ExtSet.ExpLumDiv = 1;   // for linear mode
    }

    AE_SetIntCtrlInfo(AeInfo->Id, AeCtrlInfo);
    AppMode = AeAppMode;
    IplMode[AeInfo->Id] = Info.Mode;
}

void AE_getInfo(AE_INFO *AeInfo, AE_BLK_INFO *AeBlkInfo)
{
//    memset((void *)AeInfo, 0, sizeof(AE_INFO));

    AeInfo->PrvAvgEV.EVValue = AeParamInfo[AeInfo->Id].InitEV;
    AeInfo->PrvAvgEV.LV = AEAlg_Conv2LV(AeInfo->PrvAvgEV.EVValue);

    AeInfo->StableRange = AeExtSetDefaultParamInfo[AeInfo->Id].ConvRange;

    AeInfo->StableRangeRF.h = AE_COVER_RANGE_RF_H;
    AeInfo->StableRangeRF.l = AE_COVER_RANGE_RF_L;

    AeInfo->histogram.bin_num = AE_HIST_NUM;
    AeInfo->histogram.data_num = AE_HIST_DATA_NUM;
    AeInfo->histogram.y = AeHistBuf[AeInfo->Id];

    weighting_histogram.bin_num = AE_HIST_NUM;
    weighting_histogram.data_num = (LA_WIN_X * LA_WIN_Y);
    weighting_histogram.y = AeHist2BufY[AeInfo->Id];

    // block luminance calculate
    //used in photo preview and capture only
    AeBlkInfo->FirstRunFlg = TRUE;
    AeBlkInfo->LA_Num_X = LA_WIN_X;
    AeBlkInfo->LA_Num_Y = LA_WIN_Y;
    AeBlkInfo->Yacc = Blk_Yacc[AeInfo->Id];
    AeBlkInfo->Gacc = Blk_Gacc[AeInfo->Id];
    AeBlkInfo->Raw_OverBound_Lum.h = IQS_GetInvGammaValue(AeInfo->Id, AeParamInfo[AeInfo->Id].Blk_OVER_Y_Thr.h);
    AeBlkInfo->Raw_OverBound_Lum.l = IQS_GetInvGammaValue(AeInfo->Id, AeParamInfo[AeInfo->Id].Blk_OVER_Y_Thr.l);
    AeBlkInfo->Raw_UnderBound_Lum.h = IQS_GetInvGammaValue(AeInfo->Id, AeParamInfo[AeInfo->Id].Blk_UNDER_Y_Thr.h);
    AeBlkInfo->Raw_UnderBound_Lum.l = IQS_GetInvGammaValue(AeInfo->Id, AeParamInfo[AeInfo->Id].Blk_UNDER_Y_Thr.l);
    AeBlkInfo->AddGammaFunc = AE_GET_GAMMA_VAL(AeInfo->Id);

    AeInfo->FirstRunFlag = TRUE;
    AeInfo->Reset = TRUE;
    AeInfo->Counter = 0;
    AeInfo->CounterRF = 0;
    AeInfo->CurConvDir = 0;
    AeInfo->EventTab = AeAlgEventTab;
    AeInfo->ConvSpeed = AeExtSetDefaultParamInfo[AeInfo->Id].ConvSpeed;
    AeInfo->ConvTab = NULL;
    AeInfo->InvGammaFunc = AE_GET_INV_GAMMA_VAL(AeInfo->Id);
    AeInfo->AddGammaFunc = AE_GET_GAMMA_VAL(AeInfo->Id);

    //ext
    AeInfo->ExtSet.OverExp.AdjLum.y = AeParamInfo[AeInfo->Id].ExpLum_OverExp;
    AeInfo->ExtSet.OverExp.AdjLum.raw = IQS_GetInvGammaValue(AeInfo->Id, AeInfo->ExtSet.OverExp.AdjLum.y);
    AeInfo->ExtSet.OverExp.AdjMaxCntRatioTab = ExptY_OverExpoMaxCnt[AeInfo->Id];
    AeInfo->ExtSet.OverExp.AdjMinCntRatioTab = ExptY_OverExpoMinCnt[AeInfo->Id];
    AeInfo->ExtSet.OverExp.AdjLumThresholdTab = ExptY_OverExpoThreshold[AeInfo->Id];
    AeInfo->ExtSet.OverExp.CurCnt = 0;
    AeInfo->ExtSet.OverExp.CurTotalStep = 0;
    AeInfo->ExtSet.OverExp.CurMaxStep = 0;
    AeInfo->ExtSet.OverExp.CurMaxCnt = 0;
    AeInfo->ExtSet.OverExp.CurMinCnt = 0;
    AeInfo->ExtSet.OverExp.CurThreshold = 0;
    AeInfo->ExtSet.OverExp.UserDefRatio = TRUE;

    AeInfo->ExtSet.AdjHistoInfo.RatioTab = Histo_Prv[AeInfo->Id];
    AeInfo->ExtSet.AdjHistoInfo.AdjLum.y = AeParamInfo[AeInfo->Id].ExpLum_Histo;
    AeInfo->ExtSet.AdjHistoInfo.AdjLum.raw = IQS_GetInvGammaValue(AeInfo->Id, AeInfo->ExtSet.AdjHistoInfo.AdjLum.y);

    AeInfo->ExtSet.AdjCapEn = DISABLE;
    AeInfo->ExtSet.CapExpectLum.y = AeParamInfo[AeInfo->Id].ExpLum_Photo;
    AeInfo->ExtSet.CapExpectLum.raw = IQS_GetInvGammaValue(AeInfo->Id, AeInfo->ExtSet.CapExpectLum.y);
    AeInfo->ExtSet.CapExpectTab = ExptY_Cap[AeInfo->Id];

    AeInfo->ExtSet.UnStableDelay = AeExtSetDefaultParamInfo[AeInfo->Id].DelayCnt;

    //comp ev
    AeInfo->EVCompRatio = EVTable[IPL_AlgGetUIInfo(AeInfo->Id, IPL_SEL_AEEV)];
}

#if (AE_FACE_DETECTION == ENABLE)
static void AE_GetFaceWin(UINT32 Id)
{
    UINT32 i,j,k,tmp;
    UINT32 FaceNum = 0;
    FD_FACE faceRect[10];
    URECT reSize;
    IPL_LA_SET_INFOR LA_Info;
    UINT8 FaceWeighting = 32;
    Coordinate bin; //win number for face start
    USIZE binSize;  //win number for face size
    USIZE SizeofEachBin;

    LA_Info.Id = Id;
    AE_GetLASetInfo(&LA_Info);

    reSize.x = reSize.y = 0;

	reSize.w = LA_Info.CropSizeH;
    reSize.h = LA_Info.CropSizeV;
	//memset(AE_Win_Face,0,sizeof(UINT8)*AE_WIN_Y*AE_WIN_X);
    if((FaceNum = FD_GetRsltFace(faceRect,&reSize)) != 0)
    {
        SizeofEachBin.w = LA_Info.CropSizeH / AE_WIN_X;
        SizeofEachBin.h = LA_Info.CropSizeV / AE_WIN_Y;



        if(FaceNum > 10)
        {
            FaceNum = 10;
            DBG_ERR("\r\n");
        }

        for(i = 0 ; i < FaceNum ; i++)
        {
            tmp = faceRect[i].x;
            bin.x = 0;
            while(tmp >= SizeofEachBin.w)
            {
                tmp -= SizeofEachBin.w;
                bin.x++;
            }

            tmp = faceRect[i].y;
            bin.y = 0;
            while(tmp >= SizeofEachBin.h)
            {
                tmp -= SizeofEachBin.h;
                bin.y++;
            }

            binSize.w = (faceRect[i].w + SizeofEachBin.w - 1) / SizeofEachBin.w;
            binSize.h = (faceRect[i].h  + SizeofEachBin.h  - 1) / SizeofEachBin.h;

            //check boundary
            if( (bin.x + binSize.w) > AE_WIN_X)
                binSize.w = AE_WIN_X - bin.x;
            if( (bin.y + binSize.h) > AE_WIN_Y)
                binSize.h = AE_WIN_Y - bin.y;

            for(j = bin.y ; j < (bin.y + binSize.h) ; j++)
            {
                for(k = bin.x; k < (bin.x + binSize.w); k++)
                {

                    AE_Win_Face[j][k] = FaceWeighting;
                }
            }
        }
    }
	else
	{
	    for(j = 0 ; j < AE_WIN_X ; j++)
        {
            for(k = 0; k < AE_WIN_Y; k++)
            {
				if(AE_Win_Face[j][k] != 0)
                	AE_Win_Face[j][k] = AE_Win_Face[j][k] - 1;
				if(AE_Win_Face[j][k] < 0)
					AE_Win_Face[j][k] = 0;
            }
        }

		FD_FLAG = 0;
	}
}
#endif

void AE_SetExtInfo(UINT32 Id, AE_EXT_SET_INFO SetInfo)
{
    AeExtSetDefaultParamInfo[Id].ConvSpeed = SetInfo.ConvergenceSpeed;
    AeExtSetDefaultParamInfo[Id].ConvRange.h = SetInfo.AETolerance;
    AeExtSetDefaultParamInfo[Id].ConvRange.l = SetInfo.AETolerance;
    AeExtSetDefaultParamInfo[Id].DelayCnt = SetInfo.AEDelayCnt;

    AeExtSetDefaultParamInfo[Id].ExpT = SetInfo.ExpTBound;
    AeExtSetDefaultParamInfo[Id].ISO = SetInfo.IsoBound;

    memcpy(AE_Win_UserDef, SetInfo.ExpAreaWeight, sizeof(UINT8)*AE_WIN_Y*AE_WIN_X);

    if (SetInfo.AEStrategy == AE_STRATEGY_HIGHLIGHT)
        AeExtSetDefaultParamInfo[Id].AntiOverExpEn = ENABLE;
    else
        AeExtSetDefaultParamInfo[Id].AntiOverExpEn = DISABLE;

    AeExtSetDefaultParamInfo[Id].IrisInfo.IrisType = SetInfo.IrisSetting.IrisType;
    AeExtSetDefaultParamInfo[Id].IrisInfo.IrisMode = SetInfo.IrisSetting.IrisMode;
    AeExtSetDefaultParamInfo[Id].IrisInfo.PIris = SetInfo.IrisSetting.PIris;
    AeExtSetDefaultParamInfo[Id].IrisInfo.PIrisManualSize = SetInfo.IrisSetting.PIrisManualSize;
    AeExtSetDefaultParamInfo[Id].IrisInfo.DCIrisSpeed = SetInfo.IrisSetting.DCIrisSpeed;

    AE_SetPLINEInfo(Id, SetInfo);
}

void AE_GetExtInfo(UINT32 Id, AE_EXT_SET_INFO *SetInfo)
{
    SetInfo->ConvergenceSpeed = AeExtSetDefaultParamInfo[Id].ConvSpeed;
    SetInfo->AETolerance = AeExtSetDefaultParamInfo[Id].ConvRange.h;
    SetInfo->AEDelayCnt = AeExtSetDefaultParamInfo[Id].DelayCnt;
    SetInfo->ExpT = uiExpoTime[Id];
    SetInfo->ISO = uiISOGain[Id];
    SetInfo->IrisSetting.PIrisManualSize = uiIris[Id];

    SetInfo->PLineMode = AeExtSetDefaultParamInfo[Id].PLineMode;
    AE_GetWin(Id, SetInfo->ExpAreaWeight);

    if (AeExtSetDefaultParamInfo[Id].AntiOverExpEn == ENABLE)
        SetInfo->AEStrategy = AE_STRATEGY_HIGHLIGHT;
    else
        SetInfo->AEStrategy = AE_STRATEGY_LOWLIGHT;

    SetInfo->ExpTBound = AeExtSetDefaultParamInfo[Id].ExpT;
    SetInfo->IsoBound = AeExtSetDefaultParamInfo[Id].ISO;
    SetInfo->IrisSetting.IrisType = AeExtSetDefaultParamInfo[Id].IrisInfo.IrisType;
    SetInfo->IrisSetting.IrisMode = AeExtSetDefaultParamInfo[Id].IrisInfo.IrisMode;
    SetInfo->IrisSetting.PIris = AeExtSetDefaultParamInfo[Id].IrisInfo.PIris;
    SetInfo->IrisSetting.PIrisManualSize = AeExtSetDefaultParamInfo[Id].IrisInfo.PIrisManualSize;
    SetInfo->IrisSetting.DCIrisSpeed = AeExtSetDefaultParamInfo[Id].IrisInfo.DCIrisSpeed;
}

void AE_SetPLINEInfo(UINT32 Id, AE_EXT_SET_INFO SetInfo)
{
    if (SetInfo.PLineMode == AE_PLINE_AUTOGEN)
    {
        AeExtSetDefaultParamInfo[Id].PLineMode = AE_PLINE_AUTOGEN;
        //#NT#2016/06/28#Jarkko Chang -begin
        //#NT# fixed ipcam ae set boundary fail issue
        AeExtSetDefaultParamInfo[Id].IrisInfo.IrisMode = AE_IRIS_MODE_OPEN;
        //#NT#2016/06/28#Jarkko Chang -end

        if (SetInfo.ExpTBound.h <= SetInfo.ExpTBound.l)
        {
            DBG_ERR("Id: %d, Exposure Time boundary setting error, ExpTBound.h (%d) need larger than ExpTBound.l (%d)\r\n", Id, SetInfo.ExpTBound.h,SetInfo.ExpTBound.l);
            SetInfo.ExpTBound.h = SetInfo.ExpTBound.l+1;
        }

        if (SetInfo.IsoBound.h <= SetInfo.IsoBound.l)
        {
            DBG_ERR("Id: %d, ISO Gain boundary setting error, IsoBound.h (%d) need larger than IsoBound.l (%d)\r\n", Id, SetInfo.IsoBound.h,SetInfo.IsoBound.l);
            SetInfo.IsoBound.h = SetInfo.IsoBound.l+1;
        }

        AeCurveGenInfo[Id].ExpT[AE_GEN_IDX_MIN] = SetInfo.ExpTBound.l;
        AeCurveGenInfo[Id].ExpT[AE_GEN_IDX_MAX] = SetInfo.ExpTBound.h;

        AeCurveGenInfo[Id].Iso[AE_GEN_IDX_MIN] = SetInfo.IsoBound.l;
        AeCurveGenInfo[Id].Iso[AE_GEN_IDX_MAX] = SetInfo.IsoBound.h;

        if (SetInfo.IrisSetting.IrisType == AE_IRIS_TYPE_FIXED)
        {
            //#NT#2016/06/08#Jarkko Chang -begin
            //#NT# remove unused code
            //SetInfo.IrisSetting.IrisMode = AE_IRIS_MODE_OPEN;
            //#NT#2016/06/08#Jarkko Chang -end
            AeCurveGenInfo[Id].FnoIdx = AEALG_IRIS_BIG;
        }
        else
        {
            switch (SetInfo.IrisSetting.IrisMode)
            {
                case AE_IRIS_MODE_MANUAL:
                    if (SetInfo.IrisSetting.IrisType == AE_IRIS_TYPE_P)
                    {
                        //Set from Manual Iris -> SetInfo.IrisSetting.PIrisManualSize
                        AeCurveGenInfo[Id].FnoIdx = SetInfo.IrisSetting.PIrisManualSize;
                    }
                    else
                        DBG_ERR("Id: %d, UnSupport Iris Setting, Manual Mode only Support P-Iris, Current Iris type %d \r\n", Id, SetInfo.IrisSetting.IrisType);
                    break;

                case AE_IRIS_MODE_OPEN:
                    //SET Maximum Iris
                    AeCurveGenInfo[Id].FnoIdx = AEALG_IRIS_BIG;
                    break;

                case AE_IRIS_MODE_CLOSE:
                    //SET Minimum Iris
                    AeCurveGenInfo[Id].FnoIdx = AEALG_IRIS_SMALL;
                    break;

                case AE_IRIS_MODE_AUTO:
                    //auto Iris switch, for DC or P Iris
                    AeCurveGenInfo[Id].FnoIdx = AEALG_IRIS_BIG;
                    break;

                default:
                    DBG_ERR("Id: %d, Iris Mode Set Error %d\r\n", Id, SetInfo.IrisSetting.IrisMode);
                    break;
            }
        }

        AeCurveGenInfo[Id].FnoRat = GetFnoRatio(AeCurveGenInfo[Id].FnoIdx);

        //#NT#2016/06/30#Jarkko Chang -begin
        //#NT# fixed ipcam frequency set fail
        if (IPL_AlgGetUIInfo(Id, IPL_SEL_FREQUENCY) == SEL_FREQUENCY_50HZ)
        {
            AeCurveGenInfo[Id].FreqNode = AE_50FREQ_NODE;
            AeCurveGenInfo[Id].Freq = AE_GEN_FREQ_50;
            if (SetInfo.ExpTBound.h >= AE_50FREQ_EXPT_NODE_40000)
                AeCurveGenInfo[Id].Node1stExpT = AE_50FREQ_EXPT_NODE_40000;
            else
                AeCurveGenInfo[Id].Node1stExpT = AE_50FREQ_EXPT_NODE_20000;
        }
        else
        {
            AeCurveGenInfo[Id].Freq = AE_GEN_FREQ_60;
            AeCurveGenInfo[Id].FreqNode = AE_60FREQ_NODE;
            AeCurveGenInfo[Id].Node1stExpT = AE_60FREQ_EXPT_NODE_33333;
        }
        AeCurveGenInfo[Id].Node2ndISO = SetInfo.IsoBound.h;
        //#NT#2016/06/30#Jarkko Chang -end

        AeCurveUpdate[Id] = TRUE;
        uiExpoTime[Id] = 0;
        uiISOGain[Id] = 0;
        uiIris[Id] = 0;
    }
    else if (SetInfo.PLineMode == AE_PLINE_MANUAL)
    {
        AeExtSetDefaultParamInfo[Id].PLineMode = AE_PLINE_MANUAL;
        uiExpoTime[Id] = SetInfo.ExpT;
        uiISOGain[Id] = SetInfo.ISO;
        uiIris[Id] = SetInfo.IrisSetting.PIrisManualSize;
    }
    else
    {
        AeExtSetDefaultParamInfo[Id].PLineMode = AE_PLINE_PREDEF;
        memset(&AeCurveGenInfo[Id], 0, sizeof(AeCurveGenInfo[Id]));

        AeCurveUpdate[Id] = TRUE;
        uiExpoTime[Id] = 0;
        uiISOGain[Id] = 0;
        uiIris[Id] = 0;
    }
}

AE_RESET AE_setWin(UINT32 Id, AE_WIN mode)
{
	UINT32 i,j;
    static AE_WIN prvmode[AE_PROC_CNT] = {0xFF, 0xFF};

    switch(mode)
    {
        case AE_WIN_CENTERWEIGHT:
            AeWin_FD = (UINT8(*)[AE_WIN_X])AE_Win_CenterWeighted;
            break;

        case AE_WIN_SPOT:
            AeWin_FD = (UINT8(*)[AE_WIN_X])AE_Win_Spot;
            break;

        case AE_WIN_MARTIX:
            AeWin_FD = (UINT8(*)[AE_WIN_X])AE_Win_Multi;
            break;

        case AE_WIN_USERDEFINE:
            AeWin_FD = (UINT8(*)[AE_WIN_X])AE_Win_UserDef;
            break;

        default:
        case AE_WIN_EVALUATIVE:
            AeWin_FD = (UINT8(*)[AE_WIN_X])AE_Win_FD;
            break;
    }

        for(i =0; i < (AE_WIN_X)  ; i++)
        {
            for(j = 0; j < (AE_WIN_X); j++)
            {
                 AeWin[i][j] = AeWin_FD[i][j];

            }
        }
#if (AE_FACE_DETECTION == ENABLE)

    if (IPL_AlgGetUIInfo(Id, IPL_SEL_FD) == SEL_FD_ON)
    {
        //FD AE
        AE_GetFaceWin(Id);
        for(i =0; i < (AE_WIN_X)  ; i++)
        {
            for(j = 0; j < (AE_WIN_X); j++)
            {
                if(AE_Win_Face[i][j]!=0)
                {
                    AeWin[i][j] = AeWin_FD[i][j] + AE_Win_Face[i][j];

                }
            }
        }
/*
		for(i =0; i < (AE_WIN_X)  ; i++)
        {

            for(j = 0; j < (AE_WIN_X); j++)
            {

				if(j<(AE_WIN_X-1))
					debug_msg("%d\t   ",AeWin[i][j]);
				else
            		debug_msg("%d\r\n",AeWin[i][j]);
            }
        }
*/
    }
#endif

    if (mode == prvmode[Id])
    {
        return AE_RESET_FALSE;
    }

    prvmode[Id] = mode;
    return AE_RESET_TRUE;
}

UINT32 AE_getFlag(UINT32 Id)
{
    return AeFlag[Id];
}

void AE_setFlag(UINT32 Id, UINT32 flag)
{
    AeFlag[Id] |= flag;
}

void AE_clrFlag(UINT32 Id, UINT32 flag)
{
    AeFlag[Id] &= ~flag;
}

void AE_reset (UINT32 Id)
{
    AeInfo[Id].Reset = TRUE;
}

AE_RESET AE_getCapTab(AE_ISO nISO, AE_FLICKER nFlicker, AE_SCENE nScene, AE_INFO *AeInfo)
{
    static UINT32 Scene[AE_PROC_CNT] = {0xFF, 0xFF}, Iso[AE_PROC_CNT] = {0xFF, 0xFF}, Flicker[AE_PROC_CNT] = {0xFF, 0xFF};

    if (IPL_AlgGetUIInfo(AeInfo->Id, IPL_SEL_ANTISHAKE) == SEL_ANTISHAKE_ON)
    {
        AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_Action;

        AeInfo->EV = AeParamInfo[AeInfo->Id].EV;
        AeInfo->CapISO = AeParamInfo[AeInfo->Id].CapISO;

        return AE_RESET_TRUE;
    }

    if((Scene[AeInfo->Id] == nScene)&&(Iso[AeInfo->Id] == nISO)&&(Flicker[AeInfo->Id] == nFlicker))
    {
        return AE_RESET_FALSE;
    }

    if(nFlicker == AE_FLICKER_50HZ)
    {
        switch(nISO)
        {
            case AE_ISO_200:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO200_50Hz;
                break;

            case AE_ISO_400:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO400_50Hz;
                break;

            case AE_ISO_800:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO800_50Hz;
                break;

            case AE_ISO_1600:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO1600_50Hz;
                break;

            default:
            case AE_ISO_AUTO:
            case AE_ISO_25:
            case AE_ISO_50:
            case AE_ISO_80:
            case AE_ISO_100:
            case AE_ISO_3200:
            case AE_ISO_6400:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_Cap50;
                break;
        }
    }
    else if(nFlicker == AE_FLICKER_60HZ)
    {
        switch(nISO)
        {
            case AE_ISO_200:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO200_60Hz;
                break;

            case AE_ISO_400:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO400_60Hz;
                break;

            case AE_ISO_800:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO800_60Hz;
                break;

            case AE_ISO_1600:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO1600_60Hz;
                break;

            default:
            case AE_ISO_25:
            case AE_ISO_50:
            case AE_ISO_80:
            case AE_ISO_100:
            case AE_ISO_3200:
            case AE_ISO_6400:
            case AE_ISO_AUTO:
                AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_Cap60;
                break;
        }
    }
    else
    {
        DBG_ERR("N.S. \r\n");
    }

    AeInfo->EV = AeParamInfo[AeInfo->Id].EV;
    AeInfo->CapISO = AeParamInfo[AeInfo->Id].CapISO;

    Scene[AeInfo->Id] = nScene;
    Iso[AeInfo->Id] = nISO;
    Flicker[AeInfo->Id] = nFlicker;
    return AE_RESET_TRUE;
}

AE_RESET AE_getPrvTab(AE_ISO nISO, AE_FLICKER nFlicker, AE_INFO *AeInfo)
{
    static AE_FLICKER Flicker[AE_PROC_CNT] = {0xFF, 0xFF};
    static AE_ISO Iso[AE_PROC_CNT] = {0xFF, 0xFF};
    static UINT32 Fps[AE_PROC_CNT] = {0xFF, 0xFF};
    static AE_APPMODE CurAppMode = AE_APPMODE_END;
    static AE_PLINEMODE CurPLineMode[AE_PROC_CNT] = {AE_PLINE_END, AE_PLINE_END};
    UINT32 CurFps;
    UINT32 Curve = 0;

    CurFps = IPL_AlgGetUIInfo(AeInfo->Id, IPL_SEL_VIDEO_FPS);

    if((AeCurveUpdate[AeInfo->Id] == TRUE) || (AeExtSetDefaultParamInfo[AeInfo->Id].PLineMode == AE_PLINE_AUTOGEN && AeExtSetDefaultParamInfo[AeInfo->Id].IrisInfo.IrisMode == AE_IRIS_MODE_AUTO))
    {
        AeCurveUpdate[AeInfo->Id] = FALSE;
    }
    else
    {
        if ((Flicker[AeInfo->Id] == nFlicker)&&
            (Iso[AeInfo->Id] == nISO)&&
            (CurAppMode == AeAppMode)&&
            (CurFps == Fps[AeInfo->Id])&&
            (CurPLineMode[AeInfo->Id] == AeExtSetDefaultParamInfo[AeInfo->Id].PLineMode))
        {
            return AE_RESET_FALSE;
        }
    }

    if (AeExtSetDefaultParamInfo[AeInfo->Id].PLineMode == AE_PLINE_AUTOGEN)
    {
        //#NT#2016/06/28#Jarkko Chang -begin
        //#NT# fixed ipcam ae set boundary fail issue
        if (AeExtSetDefaultParamInfo[AeInfo->Id].IrisInfo.IrisMode == AE_IRIS_MODE_AUTO)
            AeCurveGenInfo[AeInfo->Id].FnoIdx = AE_judgeIris(AeInfo);
        //#NT#2016/06/28#Jarkko Chang -end

        AeCurveGenInfo[AeInfo->Id].FnoRat = GetFnoRatio(AeCurveGenInfo[AeInfo->Id].FnoIdx);

        memset(&AeAutoCurveTab[AeInfo->Id][0][0], 0x0, sizeof(UINT32) * (AE_GEN_CURVE_NODE_MAX * AEALG_INFO_MAX));
        //#NT#2016/08/10#XL_Wei -begin
        //#NT# fixed cpu exception when AEAlg_GenCurve failed
        UINT32 status = E_OK;
        status = AEAlg_GenCurve(&AeCurveGenInfo[AeInfo->Id], &Curve, &AeInfo->EV.h, &AeInfo->EV.l);
        if(status == E_OK){
            memcpy(&AeAutoCurveTab[AeInfo->Id][0][0], (UINT32 *)Curve, sizeof(UINT32) * (AE_GEN_CURVE_NODE_MAX * AEALG_INFO_MAX));

            AeInfo->ISO.h = AeCurveGenInfo[AeInfo->Id].Iso[AE_GEN_IDX_MAX];
            AeInfo->ISO.l = AeCurveGenInfo[AeInfo->Id].Iso[AE_GEN_IDX_MIN];
            AeInfo->EV.l = AeParamInfo[AeInfo->Id].EV.l;
            AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])&AeAutoCurveTab[AeInfo->Id][0][0];
        }
        else
        {
            DBG_ERR("AEAlg_GenCurve ERR = %d, Use Predefine mode\r\n", status);
            AeExtSetDefaultParamInfo[AeInfo->Id].PLineMode = AE_PLINE_PREDEF;
        }
        //#NT#2016/08/10#XL_Wei -end

    }

    //#NT#2016/08/10#XL_Wei -begin
    //#NT# fixed cpu exception when AEAlg_GenCurve failed
    if (AeExtSetDefaultParamInfo[AeInfo->Id].PLineMode != AE_PLINE_AUTOGEN)
    //#NT#2016/08/10#XL_Wei -end
    {
        if (AeAppMode == AE_APPMODE_SHDR)
        {
            if (SensorHDR_GetAeRatio() == SHDR_RATIO_GAIN_16X)
            {
                if (nFlicker == AE_FLICKER_50HZ)
                    AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_HDR16X_AntiFlicker50;
                else if (nFlicker == AE_FLICKER_60HZ)
                    AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_HDR16X_AntiFlicker60;
            }
            else if  (SensorHDR_GetAeRatio() == SHDR_RATIO_GAIN_4X)
            {
                if (nFlicker == AE_FLICKER_50HZ)
                    AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_HDR4X_AntiFlicker50;
                else if (nFlicker == AE_FLICKER_60HZ)
                    AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_HDR4X_AntiFlicker60;
            }
            else    //N.S. Ratio or 1X Ratio
            {
                if (nFlicker == AE_FLICKER_50HZ)
                    AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_HDR1X_AntiFlicker50;
                else if (nFlicker == AE_FLICKER_60HZ)
                    AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_HDR1X_AntiFlicker60;
            }

            AeInfo->EV = AeParamInfo[AeInfo->Id].EV;
            AeInfo->ISO = AeParamInfo[AeInfo->Id].PrvISO;
        }
        else if(nFlicker == AE_FLICKER_50HZ)
        {
            switch(nISO)
            {
                case AE_ISO_AUTO:
                case AE_ISO_25:
                case AE_ISO_50:
                case AE_ISO_80:
                case AE_ISO_100:
                case AE_ISO_200:
                case AE_ISO_400:
                case AE_ISO_800:
                case AE_ISO_1600:
                case AE_ISO_3200:
                case AE_ISO_6400:
                default:
                    if (CurFps == SEL_VIDEOFPS_240)
                        AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_AntiFlicker50_240fps;
                    else if (CurFps == SEL_VIDEOFPS_120)
                        AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_AntiFlicker50_120fps;
                    else if (CurFps == SEL_VIDEOFPS_60)
                        AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_AntiFlicker50_60fps;
                    else
                        AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_AntiFlicker50;

                    AeInfo->EV = AeParamInfo[AeInfo->Id].EV;
                    AeInfo->ISO = AeParamInfo[AeInfo->Id].PrvISO;
                    break;
            }
        }
        else if(nFlicker == AE_FLICKER_60HZ)
        {
            switch(nISO)
            {
                case AE_ISO_AUTO:
                case AE_ISO_25:
                case AE_ISO_50:
                case AE_ISO_80:
                case AE_ISO_100:
                case AE_ISO_200:
                case AE_ISO_400:
                case AE_ISO_800:
                case AE_ISO_1600:
                case AE_ISO_3200:
                case AE_ISO_6400:
                default:
                    if (CurFps == SEL_VIDEOFPS_240)
                        AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_AntiFlicker60_240fps;
                    else if (CurFps == SEL_VIDEOFPS_120)
                        AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_AntiFlicker60_120fps;
                    else if (CurFps == SEL_VIDEOFPS_60)
                        AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_AntiFlicker60_60fps;
                    else
                        AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_AntiFlicker60;

                    AeInfo->EV = AeParamInfo[AeInfo->Id].EV;
                    AeInfo->ISO = AeParamInfo[AeInfo->Id].PrvISO;
                    break;
            }
        }
        else
        {
            DBG_ERR("N.S. \r\n");
        }
    }

    Flicker[AeInfo->Id] = nFlicker;
    Iso[AeInfo->Id] = nISO;
    Fps[AeInfo->Id] = CurFps;
    CurAppMode = AeAppMode;
    CurPLineMode[AeInfo->Id] = AeExtSetDefaultParamInfo[AeInfo->Id].PLineMode;

    return AE_RESET_TRUE;
}

AE_RESET AE_getPrvManualTab(UINT32 ManualFlag, UINT32 nISO, AE_INFO *AeInfo)
{
    if (ManualFlag == ENABLE)
    {
        if (nISO == 25) AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO25_NOR;
        if (nISO == 50) AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO50_NOR;
        if (nISO == 100) AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO100_NOR;
        if (nISO == 200) AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO200_NOR;
        if (nISO == 400) AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO400_NOR;
        if (nISO == 800) AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO800_NOR;
        if (nISO == 1600) AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO1600_NOR;
        if (nISO == 3200) AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO3200_NOR;
        if (nISO == 6400) AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO6400_NOR;
        if (nISO == 12800) AeInfo->PrvCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO12800_NOR;
        return AE_RESET_TRUE;
    }
    return AE_RESET_FALSE;
}

AE_RESET AE_getCapManualTab(UINT32 ManualFlag, UINT32 nISO, AE_INFO *AeInfo)
{
    if (ManualFlag == ENABLE)
    {
        if (nISO == 25) AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO25_NOR;
        if (nISO == 50) AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO50_NOR;
        if (nISO == 100) AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO100_NOR;
        if (nISO == 200) AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO200_NOR;
        if (nISO == 400) AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO400_NOR;
        if (nISO == 800) AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO800_NOR;
        if (nISO == 1600) AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO1600_NOR;
        if (nISO == 3200) AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO3200_NOR;
        if (nISO == 6400) AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO6400_NOR;
        if (nISO == 12800) AeInfo->CapCurve = (UINT32(*)[AEALG_INFO_MAX])AeCurve_ISO12800_NOR;
        return AE_RESET_TRUE;
    }
    return AE_RESET_FALSE;
}

#if 0
#endif
//------------------------------------------------------------------------------
UINT32 GetOverExpoRatio(UINT32 HistMaxNum, UINT16 *Hist)
{
    UINT32 i;
    UINT32 AvgHCnt = 0;
    UINT32 step = (AeInfo[AE_ID_1].ExtSet.OverExp.CurThreshold - (((1 << 10) / weighting_histogram.bin_num) >> 1)) / ((1 << 10) / weighting_histogram.bin_num);

    for (i = 0; i < weighting_histogram.bin_num; i ++)
    {
        if (i < step)
        {
        }
        else
        {
            AvgHCnt += weighting_histogram.y[i];
        }
    }

    AvgHCnt = (AvgHCnt * 1000) / weighting_histogram.data_num;
    return AvgHCnt;
}

UINT32 GetFnoRatio(AEALG_IRIS_IDX Idx)
{
#if 1
    return 100;
#else
    //IRIS_POS Iris_Step;
    UINT32 FnRatio;

    switch(Idx)
    {
        case AEALG_IRIS_0:
            //Iris_Step = IRIS_POS_F1_8;
            FnRatio = 53;// sepc is 32
            break;
        case AEALG_IRIS_1:
            //Iris_Step = IRIS_POS_F2_8;
            FnRatio = 80;
            break;
        case AEALG_IRIS_2:
            //Iris_Step = IRIS_POS_F4_0;
            FnRatio = 160;
            break;
        case AEALG_IRIS_3:
            //Iris_Step = IRIS_POS_F5_6;
            FnRatio = 320;
            break;
        case AEALG_IRIS_4:
            //Iris_Step = IRIS_POS_F8_0;
            FnRatio = 640;
            break;
        default:
            FnRatio = 80;
            break;
    }

    return FnRatio;
#endif
}
#if 0
#endif
//------------------------------------------------------------------------------

AE_SCENE AE_getSceneType(UINT32 Idx)
{
    switch (Idx)
    {
        case SEL_SCENEMODE_AUTO:
            return AE_SCENE_AUTO;

        case SEL_SCENEMODE_LANDSCAPE:
            return AE_SCENE_LANDSCAPE;

        case SEL_SCENEMODE_POTRAIT:
            return AE_SCENE_POTRAIT;

        case SEL_SCENEMODE_SPORT:
            return AE_SCENE_SPORT;

        case SEL_SCENEMODE_NIGHTSCENE:
            return AE_SCENE_NIGHTSCENE;

        case SEL_SCENEMODE_AV:
            return AE_SCENE_AV;

        case SEL_SCENEMODE_TV:
            return AE_SCENE_TV;

        case SEL_SCENEMODE_MANUAL:
            return AE_SCENE_MANUAL;

        case SEL_SCENEMODE_NIGHTPOTRAIT:
            return AE_SCENE_NIGHTPOTRAIT;

        case SEL_SCENEMODE_NIGHTLANDSCAPE:
            return AE_SCENE_NIGHTLANDSCAPE;

        case SEL_SCENEMODE_PARTY:
            return AE_SCENE_PARTY;

        case SEL_SCENEMODE_SUNSET:
            return AE_SCENE_SUNSET;

        case SEL_SCENEMODE_BACKLIGHT:
            return AE_SCENE_BACKLIGHT;

        case SEL_SCENEMODE_DOCUMENT:
            return AE_SCENE_DOCUMENT;

        case SEL_SCENEMODE_FIREWORK:
            return AE_SCENE_FIREWORK;

        case SEL_SCENEMODE_BEACH:
            return AE_SCENE_BEACH;

        case SEL_SCENEMODE_BMODE:
            return AE_SCENE_BMODE;

        case SEL_SCENEMODE_PROGRAM:
            return AE_SCENE_PROGRAM;

        case SEL_SCENEMODE_HIGHISO:
            return AE_SCENE_HIGHISO;

        default:
            return AE_SCENE_AUTO;
    }
}

AE_FLICKER AE_getFlickerType(UINT32 Idx)
{
    if (Idx == SEL_FREQUENCY_AUTO)
    {
        if ( AEAFD_GetRslt() == AEAFD_FLICKER_50Hz )
            AEAFD_CurrentFlickerType = AE_FLICKER_50HZ;
        if ( AEAFD_GetRslt() == AEAFD_FLICKER_60Hz )
            AEAFD_CurrentFlickerType = AE_FLICKER_60HZ;
        return AEAFD_CurrentFlickerType;
    }
    else if (Idx == SEL_FREQUENCY_50HZ)
    {
        return AE_FLICKER_50HZ;
    }
    else
    {
        return AE_FLICKER_60HZ;
    }
}

AE_ANTISHAKE AE_getAntiShakeLv(UINT32 Idx)
{

    return AE_ANTISHAKE_LV0;
}

AE_ISO AE_getIsoType(UINT32 Idx)
{
    switch (Idx)
    {
        case SEL_ISOINDEX_AUTO:
            return AE_ISO_AUTO;

        case SEL_ISOINDEX_50:
            return AE_ISO_50;

        case SEL_ISOINDEX_80:
            return AE_ISO_80;

        case SEL_ISOINDEX_100:
            return AE_ISO_100;

        case SEL_ISOINDEX_200:
            return AE_ISO_200;

        case SEL_ISOINDEX_400:
            return AE_ISO_400;

        case SEL_ISOINDEX_800:
            return AE_ISO_800;

        case SEL_ISOINDEX_1600:
            return AE_ISO_1600;

        case SEL_ISOINDEX_3200:
            return AE_ISO_3200;

        case SEL_ISOINDEX_6400:
            return AE_ISO_6400;

        default:
            return AE_ISO_AUTO;
    }
}

AE_WIN AE_getWinType(UINT32 Idx)
{
    if (Idx == SEL_AEMODE_CENTERWEIGHTED) return AE_WIN_CENTERWEIGHT;
    if (Idx == SEL_AEMODE_SPOT) return AE_WIN_SPOT;
    if (Idx == SEL_AEMODE_MATRIX) return AE_WIN_MARTIX;
    if (Idx == SEL_AEMODE_EVALUATIVE) return AE_WIN_EVALUATIVE;
    if (Idx == SEL_AEMODE_USERDEFINE) return AE_WIN_USERDEFINE;
    return AE_WIN_CENTERWEIGHT;
}

#if 0
#endif
//------------------------------------------------------------------------------
UINT32 AE_getPrvBiningMode(UINT32 Id)
{
    SENSOR_INFO SensorInfo = {0};
    SENSOR_MODE PrvSensorMode;
    ER rt = E_OK;

    PrvSensorMode = (SENSOR_MODE)IPL_AlgGetUIInfo(Id, IPL_SEL_PRVSENMODE);
    rt = Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), PrvSensorMode, &SensorInfo);
    if (rt != E_OK || SensorInfo.Mode == NULL)
        return 100;
    else
        return SensorInfo.Mode->biningRatio;
}

UINT32 AE_getCapBiningMode(UINT32 Id)
{
    SENSOR_INFO SensorInfo = {0};
    SENSOR_MODE CapSensorMode;
    ER rt = E_OK;

    CapSensorMode = (SENSOR_MODE)IPL_AlgGetUIInfo(Id, IPL_SEL_CAPSENMODE);
    rt = Sensor_GetStatus(IPL_UTI_CONV2_SEN_ID(Id), CapSensorMode, &SensorInfo);
    if (rt != E_OK || SensorInfo.Mode == NULL)
        return 100;
    else
        return SensorInfo.Mode->biningRatio;
}

void AE_setExpT(UINT32 Id, EXPOSURE_SETTING *ExpTSetting)
{

    if ((AE_getFlag(Id) & AE_FLAG_SET_EXPT) == AE_FLAG_SET_EXPT)
    {
        if ((ExpTSetting->uiFrame[0] == 0xffffffff) && (ExpTSetting->uiLine[0] == 0xffffffff) && (ExpTSetting->uiPixel[0] == 0xffffffff))
            DBG_MSG("Exposure Line Value Invalid\r\n");
        else
            Sensor_SetExpTime(IPL_UTI_CONV2_SEN_ID(Id) ,ExpTSetting);

        ExpTSetting->uiFrame[0] = 0xffffffff;
        ExpTSetting->uiLine[0]  = 0xffffffff;
        ExpTSetting->uiPixel[0] = 0xffffffff;

        AE_clrFlag(Id, AE_FLAG_SET_EXPT);
    }
}

void AE_setGain(UINT32 Id, GAIN_SETTING *Gain)
{
    if ((AE_getFlag(Id) & AE_FLAG_SET_GAIN) == AE_FLAG_SET_GAIN)
    {
        if ((Gain->data1[0] == 0xffffffff) && (Gain->data2[0] == 0xffffffff) && (Gain->data3[0] == 0xffffffff))
            DBG_MSG("AGC Gain Value Invalid\r\n");
        else
            Sensor_SetGain(IPL_UTI_CONV2_SEN_ID(Id) ,Gain);

        Gain->data1[0] = 0xffffffff;
        Gain->data2[0] = 0xffffffff;
        Gain->data3[0] = 0xffffffff;

        AE_clrFlag(Id, AE_FLAG_SET_GAIN);
    }
}

void AE_setIris(UINT32 Id, AEALG_IRIS_IDX Idx)

{
    IRIS_POS Iris_Step;

    if ((AE_getFlag(Id) & AE_FLAG_SET_IRIS) == AE_FLAG_SET_IRIS)
    {
        switch(Idx)
        {
            case AEALG_IRIS_0:
                default:
                Iris_Step = IRIS_POS_F1_8;
                break;
            case AEALG_IRIS_1:
                Iris_Step = IRIS_POS_F2_8;
                break;
            case AEALG_IRIS_2:
                Iris_Step = IRIS_POS_F4_0;
                break;
            case AEALG_IRIS_3:
                Iris_Step = IRIS_POS_F5_6;
                break;
            case AEALG_IRIS_4:
            case AEALG_IRIS_8:
                Iris_Step = IRIS_POS_F8_0;
                break;
        }
        if(Lens_Module_GetState() != LENS_STATE_IDLE)
        {
            DBG_MSG("Switch aperture fail\r\n");
        }
        else
        {
            Lens_Aperture_Move(Iris_Step);
            AE_clrFlag(Id, AE_FLAG_SET_IRIS);
        }
    }
}

UINT32 AE_judgeIris(AE_INFO *AeInfo)
{
    UINT32 iris_idx = AEALG_IRIS_BIG;

    if (AeInfo->PrvAvgEV.LV < 40)
    {
        iris_idx = AEALG_IRIS_BIG;
    }
    else if (AeInfo->PrvAvgEV.LV < 60)
    {
        iris_idx = AEALG_IRIS_0;
    }
    else if (AeInfo->PrvAvgEV.LV < 80)
    {
        iris_idx = AEALG_IRIS_1;
    }
    else if (AeInfo->PrvAvgEV.LV < 100)
    {
        iris_idx = AEALG_IRIS_2;
    }
    else
    {
        iris_idx = AEALG_IRIS_SMALL;
    }

    if (iris_idx < AeExtSetDefaultParamInfo[AeInfo->Id].IrisInfo.PIris.l)
        iris_idx = AeExtSetDefaultParamInfo[AeInfo->Id].IrisInfo.PIris.l;
    else if (iris_idx > AeExtSetDefaultParamInfo[AeInfo->Id].IrisInfo.PIris.h)
        iris_idx = AeExtSetDefaultParamInfo[AeInfo->Id].IrisInfo.PIris.h;

    return iris_idx;
}

BOOL AE_Wait_Stable(UINT32 Id, UINT32 StableCnt, UINT32 TimeOutFrame)
{
    UINT32 TimeOutCnt=0;

    while(TimeOutCnt < TimeOutFrame)
    {
        if(AeInfo[Id].Counter > StableCnt)
            return TRUE;
        else
            TimeOutCnt ++;

        IPC_WaitVD2(Id, FALSE, AE_TIMEOUT_MS);
    }

    DBG_MSG("AE Stable Time Out but not stable\r\n");
    return FALSE;
}

void AE_StartAFD(UINT32 Id)
{
    AEAFD_START[Id] = TRUE;
}

void AE_GetExifInfo(UINT32 Id, UINT32 *ISO, UINT32 *ExpT, UINT32 *Aperture, UINT32 *MaxApture)
{
    UINT32 ApertureValue;
    UINT32 ISOMode = 0, ISOValue = 0, ExpTValue = 0;

    ISOMode = AE_getIsoType(IPL_AlgGetUIInfo(Id, IPL_SEL_ISOINDEX));

    switch(CapNewAeArg[Id].Iris)
    {
        case AEALG_IRIS_0:
            default:
            ApertureValue = 18;
            break;

        case AEALG_IRIS_1:
            ApertureValue = 28;
            break;

        case AEALG_IRIS_2:
            ApertureValue = 40;
            break;

        case AEALG_IRIS_3:
            ApertureValue = 56;
            break;

        case AEALG_IRIS_4:
            ApertureValue = 80;
            break;
    }

    switch(ISOMode)
    {
        case AE_ISO_100:
            ISOValue = 100;
            break;

        case AE_ISO_200:
            ISOValue = 200;
            break;

        case AE_ISO_400:
            ISOValue = 400;
            break;

        case AE_ISO_800:
            ISOValue = 800;
            break;

        case AE_ISO_1600:
            ISOValue = 1600;
            break;

        case AE_ISO_3200:
            ISOValue = 3200;
            break;

        case AE_ISO_6400:
            ISOValue = 6400;
            break;

        default:
            ISOValue = CapNewAeArg[Id].ISOGain;
            break;
    }

    if (CapNewAeArg[Id].ISOGain == 0)
        ExpTValue = CapNewAeArg[Id].ExpoTime;
    else
        ExpTValue = (CapNewAeArg[Id].ExpoTime*ISOValue)/CapNewAeArg[Id].ISOGain;

    if(ExpTValue == 0)
        ExpTValue = 1;

    *ExpT = ExpTValue;
    *ISO = ISOValue;
    *Aperture = ApertureValue;
    *MaxApture = 18;
}

UINT32 AEB_GetRatioFromEV(UINT32 Id, SEL_AEEV AEB_Step)
{
    AEB_Ratio[Id] = EVTable[AEB_Step];
    return AEB_Ratio[Id];
}

void AEB_SetRatio2Cap(UINT32 Id, UINT32 Ratio)
{

    AeInfo[Id].CapParam.EVValue = AeInfo[Id].PrvParam[0].EVValue * AE_EV_RATIO / Ratio;

    AEAlg_Allot(AeInfo[Id].CapParam.EVValue, AeInfo[Id].CapBiningMode, AeInfo[Id].CapCurve, AeInfo[Id].EventTab.GetFnoRatio, AeInfo[Id].EventTab.GetCurveOfsRatio, &CapNewAeArg[Id]);
}


AE_SETIRCUT_FP AE_fpSetIRCut = NULL;
void AE_SetIRCutFP(AE_SETIRCUT_FP fp)
{
    AE_fpSetIRCut = fp;
}

void AE_SetIRCut(UINT32 Id, UINT32 lv)
{
    static UINT32 delay_cnt_h = 0, delay_cnt_l = 0;
    static UINT32 cur_status = 0xff;

    if ( AE_fpSetIRCut != NULL )
    {
        if (lv > AE_IRCUT_LV_H && cur_status != ENABLE)
        {
            delay_cnt_h ++;
            delay_cnt_l = 0;
            if (delay_cnt_h > AE_IRCUT_DELAY_CNT)
            {
                cur_status = ENABLE;
                (AE_fpSetIRCut)(Id, ENABLE);
                IR_CUT_ENABLE[Id] = TRUE;
            }
        }
        else if  (lv < AE_IRCUT_LV_L && cur_status != DISABLE)
        {
            delay_cnt_l ++;
            delay_cnt_h = 0;
            if (delay_cnt_l > AE_IRCUT_DELAY_CNT)
            {
                 cur_status = DISABLE;
                (AE_fpSetIRCut)(Id, DISABLE);
                IR_CUT_ENABLE[Id] = FALSE;
            }
        }
        else
        {
            delay_cnt_l = 0;
            delay_cnt_h = 0;
        }
    }
    else
    {
        IR_CUT_ENABLE[Id] = FALSE;
    }
}

/*-------------- for debug in AE_Uart_Cmd.c ------------------*/
#if 0
BOOL Cmd_ae_SetET(CHAR* strCmd)
{
    UINT32 ET, LUM=0,i;
    SENSOR_MODE Mode;
    EXPOSURE_SETTING ExpTSetting;
    IPL_CA_INFOR CAInfo;
    ER rt = E_OK;

    AE_Lock(0, 1, ENABLE);
    sscanf(strCmd, "%d", &ET);
    rt = Sensor_GetCurMode(SENSOR_ID_1, &Mode);
    if (rt != E_OK)
        return;

    Sensor_GetExpoSetting(SENSOR_ID_1, ET, Mode, &ExpTSetting);
    Sensor_SetExpTime(SENSOR_ID_1,&ExpTSetting);

    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    //LUM = AE_getStatisticInfo(0, NULL);

    CAInfo.Id = IPL_ID_1;
    CAInfo.R = Racc;
    CAInfo.G = Gacc;
    CAInfo.B = Bacc;
    CAInfo.Length = 1024;
    IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CAInfo);

    for(i=0;i<CAInfo.Length;i++)
    {
        //Racc[i] = (Racc[i]>>4)&0xff;
        Gacc[i] = (Gacc[i]>>4)&0xff;
        //Bacc[i] = (Bacc[i]>>4)&0xff;

        LUM += Gacc[i];
    }

    LUM /= CAInfo.Length;

    DBG_ERR("Lum=%d, ET=%d, Frame=%d, Line=%d, pixel=%d\r\n",LUM,ET,ExpTSetting.uiFrame,ExpTSetting.uiLine,ExpTSetting.uiPixel);

    return TRUE;
}

BOOL Cmd_ae_SetISO(CHAR* strCmd)
{
    UINT32 ISO, LUM=0,i;
    GAIN_SETTING Gain;
    IPL_CA_INFOR CAInfo;

    AE_Lock(0, 1, ENABLE);
    sscanf(strCmd, "%d", &ISO);


    Sensor_GetGainSetting(SENSOR_ID_1, ISO, &Gain);
    Sensor_SetGain(SENSOR_ID_1,&Gain);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    //LUM = AE_getStatisticInfo(0, NULL);

    CAInfo.Id = IPL_ID_1;
    CAInfo.R = Racc;
    CAInfo.G = Gacc;
    CAInfo.B = Bacc;
    CAInfo.Length = 1024;
    IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CAInfo);

    for(i=0;i<CAInfo.Length;i++)
    {
        Gacc[i] = (Gacc[i]>>4)&0xff;

        LUM += Gacc[i];
    }

    LUM /= CAInfo.Length;

    DBG_ERR("Lum=%d, ISO=%d, Again=%d, Dgain=%d, xxx=%d\r\n",LUM,ISO,Gain.data1,Gain.data2,Gain.data3);
    return TRUE;
}

BOOL Cmd_ae_SetIris(CHAR* strCmd)
{
    UINT32 Iris, LUM=0,i;
    IPL_CA_INFOR CAInfo;

    AE_Lock(0, 1, ENABLE);
    sscanf(strCmd, "%d", &Iris);

    AE_setFlag((AE_FLAG_SET_IRIS));
    AE_setIris(Iris);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    IPC_WaitVD(FALSE);
    //LUM = AE_getStatisticInfo(0, NULL);

    CAInfo.Id = IPL_ID_1;
    CAInfo.R = Racc;
    CAInfo.G = Gacc;
    CAInfo.B = Bacc;
    CAInfo.Length = 1024;
    IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CAInfo);

    for(i=0;i<CAInfo.Length;i++)
    {
        Gacc[i] = (Gacc[i]>>4)&0xff;

        LUM += Gacc[i];
    }

    LUM /= CAInfo.Length;

    DBG_ERR("Lum=%d, Iris=%d\r\n",LUM,Iris);
    return TRUE;
}
#endif
/*--------------------------------------------------------*/
