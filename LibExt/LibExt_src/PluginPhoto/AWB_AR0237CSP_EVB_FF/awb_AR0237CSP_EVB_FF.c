/**
    Auto white balance flow.

    awb flow.

    @file       awb_AR0237CSP_EVB_FF.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include <stdio.h>

#include "Type.h"
#include "awb_alg.h"
#include "awb_api.h"
#include "awb_task.h"
#include "awb_AR0237CSP_EVB_FF_int.h"
#include "ae_api.h"
#include "NOTE.h"
#include "IPL_AlgInfor.h"
#include "IPL_Cmd.h"
#include "iqs_api.h"
#include "Sensor.h"

#define CAL_AWB DISABLE
#if CAL_AWB
#include "ipl_cal_datapassing_AR0230CS_EVB_FF_int.h"
#endif

#define __MODULE__ AWB
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

extern UINT32 uiRGain[AWB_ID_MAX_NUM], uiGGain[AWB_ID_MAX_NUM], uiBGain[AWB_ID_MAX_NUM];
extern UINT32 uiAWBUartCmd[AWB_ID_MAX_NUM];

static AWB_APPMODE AwbAppMode = AWB_APPMODE_NORMAL;
static AWBALG_CT_CALMODE AwbCTCalMode = AWBALG_CT_CAL_RB_GAIN;
static AWBALG_INFO AwbInfo[AWB_PROC_CNT] = {0};
static UINT16 AccTab[AWB_PROC_CNT][AWBALG_CH_MAX][AWB_CABUFFER_SIZE];
static UINT16 AccCnt[AWB_PROC_CNT][AWB_CABUFFER_SIZE];
static UINT16 CalTab[AWB_PROC_CNT][AWBALG_CH_MAX][AWB_CABUFFER_SIZE];
static UINT16 WTab[AWB_PROC_CNT][AWB_CABUFFER_SIZE];
static UINT16 FlagTab[AWB_PROC_CNT][AWB_CABUFFER_SIZE];

static void AWB_GetTabPt(UINT32 id);
static void AWB_setGain(AWBALG_INFO *Info, UINT32 RG, UINT32 GG, UINT32 BG);
static void AWB_getCA(UINT32 id, UINT16 *R, UINT16 *G, UINT16 *B, UINT16 *Cnt, UINT32 Size);
static AWB_MODE AWB_getMode(UINT32 Idx);
static void AWB_autoProc(AWBALG_INFO *Info);
static void AWB_manualProc(AWBALG_INFO *Info, AWB_MODE Idx);

AWB_PARAM_Pr AWB_Table_Ptr[AWB_ID_MAX_NUM] = {0};

////////////////////////////////////////////////////////
//
// AWB alg callback
//
////////////////////////////////////////////////////////
static UINT32 fpUserChkWhite(UINT32 id, UINT32 R, UINT32 G, UINT32 B)
{
    return 100;
}


static UINT32 fpUserAdjGain(UINT32 id, UINT32 *Rgain, UINT32 *Bgain)
{
    return 0;
}


////////////////////////////////////////////////////////
//
// extern function
//
////////////////////////////////////////////////////////
UINT32 AWB_GetCAInfo(UINT32 Id, AWB_CAINFO* CAInfo)
{
    CAInfo->WinNumX = AwbPreWhiteElement[Id].WinNumX;
    CAInfo->WinNumY = AwbPreWhiteElement[Id].WinNumY;
    CAInfo->RGl = AwbPreWhiteElement[Id].RGl;
    CAInfo->RGu = AwbPreWhiteElement[Id].RGu;
    CAInfo->GGl = AwbPreWhiteElement[Id].GGl;
    CAInfo->GGu = AwbPreWhiteElement[Id].GGu;
    CAInfo->BGl = AwbPreWhiteElement[Id].BGl;
    CAInfo->BGu = AwbPreWhiteElement[Id].BGu;
    CAInfo->RBGl = AwbPreWhiteElement[Id].RBGl;
    CAInfo->RBGu = AwbPreWhiteElement[Id].RBGu;
    return 0;
}

UINT32 AWB_GetBufferSize(void)
{
    return 0;
}

void AWB_SetBuffer(UINT32 addr, UINT32 size)
{
    // to do...
}

void AWB_SetInfo(UINT32 Id, AwbSetItem awbItem, AwbSetParam *pAWBSetting)
{
    if ( Id >= AWB_PROC_CNT )
    {
        DBG_ERR("wrong AWB ID (%d)\r\n", Id);
        return ;
    }

    AWB_MODE Mode = AWB_MODE_AUTO;
    Mode = AWB_getMode(IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WBMODE));

    switch (awbItem)
    {
        case AWB_SET_SPEED:
            if (pAWBSetting->uiConvSpeed >= SEL_AWB_CONV_SPEED_LEVEL_MAX_CNT)
            {
                DBG_ERR("convergence speed level %d > max level %d\r\n", pAWBSetting->uiConvSpeed, SEL_AWB_CONV_SPEED_LV10);
                break;
            }
            AwbInfo[Id].AwbConvSpeed = pAWBSetting->uiConvSpeed;
            if (Mode != AWB_MODE_AUTO)
                DBG_WRN("wb gain are fixed\r\n");
            break;
        case AWB_SET_STEP:
            if (pAWBSetting->uiConvStep >= SEL_AWB_CONV_STEP_LEVEL_MAX_CNT)
            {
                DBG_ERR("convergence step level %d > max level %d\r\n", pAWBSetting->uiConvStep, SEL_AWB_CONV_STEP_LV10);
                break;
            }
            AwbInfo[Id].AwbConvStep = pAWBSetting->uiConvStep;
            if (Mode != AWB_MODE_AUTO)
                DBG_WRN("wb gain are fixed\r\n");
            break;
        case AWB_SET_GAIN:
            switch (Mode)
            {
                case AWB_MODE_AUTO:
                    DBG_WRN("awb cannot set gain in AWB_MDOE_AUTO\r\n");
                    break;
                case AWB_MODE_CUSTOMER1:
                case AWB_MODE_CUSTOMER2:
                case AWB_MODE_CUSTOMER3:
                case AWB_MODE_MGAIN:
                    MWB_WriteColorGain(Id, Mode, pAWBSetting->uiManualRGain, pAWBSetting->uiManualGGain, pAWBSetting->uiManualBGain);
                    break;
                case AWB_MODE_DAYLIGHT:
                case AWB_MODE_CLOUDY:
                case AWB_MODE_TUNGSTEN:
                case AWB_MODE_FLUORESCENT1:
                case AWB_MODE_FLUORESCENT2:
                case AWB_MODE_SUNSET:
                case AWB_MODE_BIRTHDAYPARTY:
                    DBG_WRN("not ready\r\n");
                    break;
                default:
                    DBG_WRN("wrong awb mode %d \r\n", Mode);
                    break;
            }
            break;
        default:
            DBG_WRN("wrong awb set item\r\n");
            break;
    }
}

void AWB_GetInfo(UINT32 Id, AwbSetParam *pAWBSetting)
{
    if ( Id >= AWB_PROC_CNT )
    {
        DBG_ERR("wrong AWB ID (%d)\r\n", Id);
        return ;
    }

    AWB_MODE Mode = AWB_MODE_AUTO;
    Mode = AWB_getMode(IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WBMODE));

    pAWBSetting->uiConvSpeed = AwbInfo[Id].AwbConvSpeed;
    pAWBSetting->uiConvStep = AwbInfo[Id].AwbConvStep;
    if (Mode == AWB_MODE_AUTO)
    {
        pAWBSetting->uiManualRGain = 256;
        pAWBSetting->uiManualGGain = 256;
        pAWBSetting->uiManualBGain = 256;
    }
    else
    {
        MWB_ReadColorGain(Id, Mode, &pAWBSetting->uiManualRGain,&pAWBSetting->uiManualGGain,&pAWBSetting->uiManualBGain);
    }
}

void AWB_GetStatus(UINT32 Id, AWBStatus *pAWBStatus)
{
    if ( Id >= AWB_PROC_CNT )
    {
        DBG_ERR("wrong AWB ID (%d)\r\n", Id);
        return ;
    }

    pAWBStatus->ConvSpeed = AwbInfo[Id].AwbConvSpeed;
    pAWBStatus->ConvStep = AwbInfo[Id].AwbConvStep;

    AWB_MODE Mode = AWB_MODE_AUTO;
    Mode = AWB_getMode(IPL_AlgGetUIInfo((IPL_PROC_ID)Id, IPL_SEL_WBMODE));
    if (Mode == AWB_MODE_AUTO)
    {
        pAWBStatus->RGain = AwbInfo[Id].AWBGain.RGain;
        pAWBStatus->GGain = AwbInfo[Id].AWBGain.GGain;
        pAWBStatus->BGain = AwbInfo[Id].AWBGain.BGain;
    }
    else
    {
        MWB_ReadColorGain(Id, Mode, &pAWBStatus->RGain,&pAWBStatus->GGain,&pAWBStatus->BGain);
    }

    AWB_GetCurCT(Id, AwbCTCalMode, AwbInfo[Id].AWBGain.RGain, AwbInfo[Id].AWBGain.BGain, &pAWBStatus->ColorTemperature);
}

void AWB_GetTabPt(UINT32 id)
{
    //#NT#2017/02/15#Wendy Liao -begin
    //#NT# Get AWB Parameters' Pr
    AWB_Table_Ptr[id].Now_AWB_Info = &AwbInfo[id];
    AWB_Table_Ptr[id].Now_CA_Info = &AwbPreWhiteElement[id];
    AWB_Table_Ptr[id].Now_CT_Tab = &AwbCTTable[id][0];
    AWB_Table_Ptr[id].Now_W_Tab = &AwbWhiteTAB[id][0];
    AWB_Table_Ptr[id].Now_CT_Param = &AwbCTParam[id];
    AWB_Table_Ptr[id].Now_Post_Param = &AwbPostParam[id];
    AWB_Table_Ptr[id].Now_MWB_Tab = &MwbTAB[id][0][0];
};

void AWB_Init(UINT32 id, UINT32 CurrentStatus)
{
    SENSOR_INFO SenInfo = {0};
    SENSOR_ID SensorID = IPL_UTI_CONV2_SEN_ID(id);
    static BOOL Init_Flag[AWB_PROC_CNT] = {FALSE, FALSE};
    ER rt = E_OK;

    if ( id >= AWB_PROC_CNT )
    {
        DBG_ERR("wrong AWB ID (%d)\r\n", id);
        return ;
    }

    rt = Sensor_GetStatus(SensorID, IPL_AlgGetUIInfo((IPL_PROC_ID)id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (rt != E_OK || SenInfo.Mode == NULL)
        return;

    if (SenInfo.Mode->ModeType == SENSOR_MODE_STAGGER_HDR)
        AwbAppMode = AWB_APPMODE_SHDR;
    else
        AwbAppMode = AWB_APPMODE_NORMAL;

    AwbInfo[id].Id = id;
    #if 1
    AWB_SetAlgMode(AWBALG_MODE_NVTALG_R2);
    AwbInfo[id].FuncCtrl = (AWB_FUNCCTRL_R2_PPROC_EN | AWB_FUNCCTRL_R2_CTTAB_EN | AWB_FUNCCTRL_R2_STABLE_EN /*| AWB_FUNCCTRL_R2_MULTICT_EN*/);
    #else
    AWB_SetAlgMode(AWBALG_MODE_GRAYWORLD);
    AwbInfo[id].FuncCtrl = 0;
    #endif

    #if AWB_REFWHITE_REMAP
    AWBALG_RGB_INFO p1 = {28, 62, 46};
    AWBALG_RGB_INFO p2 = {23, 45, 28};
    AWBALG_RGB_INFO p3 = {42, 48, 18};
    AwbInfo[id].FuncCtrl |= AWB_FUNCCTRL_R2_REMAP_EN;
    AwbInfo[id].AwbRemapInfo.HCTInfo = p1;
    AwbInfo[id].AwbRemapInfo.MCTInfo = p2;
    AwbInfo[id].AwbRemapInfo.LCTInfo = p3;
    #endif

#if CAL_AWB
    pCAL_TAG pTag;
    CAL_AWB_FMT* pFmt;
    CAL_DATA_INFO CaldataInfo = {0};
    CaldataInfo.StationID = SEC_AWB;
    CaldataInfo.SenMode = IPL_AlgGetUIInfo((IPL_PROC_ID)id, IPL_SEL_CAPSENMODE);
    CaldataInfo.CalCondition[0] = CAL_CONDITION_DEFAULT;

    pTag = GetCalData(id, CaldataInfo);

    if (pTag != NULL)
    {
        DBG_IND("get AWB Calibration data\r\n");
        if(pTag->CAL_STATUS == _CAL_OK)
        {
            pFmt = (CAL_AWB_FMT*)&pTag->TagData;

            if (pFmt->Rg != 0 && pFmt->Bg != 0)
            {
                AwbInfo[id].KGainInfo.KGain[0].KRGain = pFmt->Rg;
                AwbInfo[id].KGainInfo.KGain[0].KBGain = pFmt->Bg;
            }
            else
            {
                DBG_ERR("read AWB Calibration data Fail\r\n");
                AwbInfo[id].KGainInfo.KGain[0].KRGain = AWB_DEF_KRGAIN;
                AwbInfo[id].KGainInfo.KGain[0].KBGain = AWB_DEF_KBGAIN;
            }

            DBG_IND("AWB Cal Rg=%d, Bg=%d, SenMode=%d\r\n", pFmt->Rg,pFmt->Bg,IPL_AlgGetUIInfo((IPL_PROC_ID)id, IPL_SEL_CAPSENMODE));
        }
        else
        {
            DBG_IND("cal status not OK\r\n");
            AwbInfo[id].KGainInfo.KGain[0].KRGain = AWB_DEF_KRGAIN;
            AwbInfo[id].KGainInfo.KGain[0].KBGain = AWB_DEF_KBGAIN;
        }
    }
    else
    {
        DBG_IND("NO AWB cal data\r\n");
        AwbInfo[id].KGainInfo.KGain[0].KRGain = AWB_DEF_KRGAIN;
        AwbInfo[id].KGainInfo.KGain[0].KBGain = AWB_DEF_KBGAIN;
    }
#else
    AwbInfo[id].KGainInfo.KGain[0].KRGain = AWB_DEF_KRGAIN;
    AwbInfo[id].KGainInfo.KGain[0].KBGain = AWB_DEF_KBGAIN;
#endif

    if ( Init_Flag[id] == FALSE )
    {
        Init_Flag[id] = TRUE;
        AwbInfo[id].DefGain.RGain = AWB_DEF_RGAIN*AwbInfo[id].KGainInfo.KGain[0].KRGain/AWB_BASE_KGAIN;
        AwbInfo[id].DefGain.GGain = 256;
        AwbInfo[id].DefGain.BGain = AWB_DEF_BGAIN*AwbInfo[id].KGainInfo.KGain[0].KBGain/AWB_BASE_KGAIN;
    }
    else
    {
        AwbInfo[id].DefGain = AwbInfo[id].AWBGain;
    }

    AwbInfo[id].PreGain = AwbInfo[id].DefGain;
    AwbInfo[id].AWBGain = AwbInfo[id].DefGain;

    AwbInfo[id].WinNumX = AwbPreWhiteElement[id].WinNumX;
    AwbInfo[id].WinNumY = AwbPreWhiteElement[id].WinNumY;
    AwbInfo[id].AccTab[AWBALG_CH_R] = AccTab[id][AWBALG_CH_R];
    AwbInfo[id].AccTab[AWBALG_CH_G] = AccTab[id][AWBALG_CH_G];
    AwbInfo[id].AccTab[AWBALG_CH_B] = AccTab[id][AWBALG_CH_B];
    AwbInfo[id].AccCnt = AccCnt[id];
    AwbInfo[id].CalTab[AWBALG_CH_R] = CalTab[id][AWBALG_CH_R];
    AwbInfo[id].CalTab[AWBALG_CH_G] = CalTab[id][AWBALG_CH_G];
    AwbInfo[id].CalTab[AWBALG_CH_B] = CalTab[id][AWBALG_CH_B];
    AwbInfo[id].FlagTab = FlagTab[id];
    AwbInfo[id].WTab = WTab[id];
    AwbInfo[id].EventFp.UserChkWhite = fpUserChkWhite;
    AwbInfo[id].EventFp.UserAdjGain  = fpUserAdjGain;

    AwbInfo[id].StableGain.Tolerance = 268;  //268/256=5%
    AwbInfo[id].StableGain.Score = 0;
    AwbInfo[id].StableGain.RGain = AwbInfo[id].DefGain.RGain;
    AwbInfo[id].StableGain.GGain = 256;
    AwbInfo[id].StableGain.BGain = AwbInfo[id].DefGain.BGain;

    if (AwbInfo[id].AwbConvSpeed == 0)
    {
        AwbInfo[id].AwbConvSpeed = SEL_AWB_CONV_SPEED_LV4;
    }
    if (AwbInfo[id].AwbConvStep == 0)
    {
        AwbInfo[id].AwbConvStep = SEL_AWB_CONV_STEP_LV2;
    }

    AwbInfo[id].MinWTotal = AWB_W_THRES;

    //--------------------------------------------------
    AwbInfo[id].CTParam = &AwbCTParam[id];
    AwbInfo[id].PostParam = &AwbPostParam[id];

    AwbInfo[id].Tab = AwbWhiteTAB[id];
    AwbInfo[id].TabCnt = AWB_WHITE_TAB_CNT;

    AwbInfo[id].PDLightTab = NULL;
    AwbInfo[id].PDLightTabCnt = 0;
    //--------------------------------------------------


    AwbInfo[id].AWBCnt = 0;
    //AwbInfo[id].AWBEnter = 1;
    AwbInfo[id].AWBEnterCnt = 0;

    AwbInfo[id].AwbWeightEn = TRUE;

    AWBALG_Init(&AwbInfo[id]);

    CA_DumpFP(AWB_DumpCAInfo);
    Flag_DumpFP(AWB_DumpFlagInfo);

    AWB_GetTabPt(id);
    NOTE_AWB_Init(id, AWB_Table_Ptr[id]);
    //#NT#2017/01/18#Wendy Liao -end
}

void AWB_Process(UINT32 id, UINT32 CurrentStatus)
{
    AWB_MODE Mode;
    UINT32 AutoRGain = AwbInfo[id].AWBGain.RGain;
    UINT32 AutoBGain = AwbInfo[id].AWBGain.BGain;
    UINT32 AWB_WIN_X = AwbInfo[id].WinNumX;
    UINT32 AWB_WIN_Y = AwbInfo[id].WinNumY;
    UINT32 AWB_WIN_TOTAL = AWB_WIN_X * AWB_WIN_Y;

    AwbInfo[id].AWBCnt ++;
    if (AWB_SKIP_FRAME < 1)
    {
        DBG_ERR("AWB_SKIP_FRAME = %d, AWB must skip at least a frame to avoid garbage\r\n", AWB_SKIP_FRAME);
    }
    //if ((0 != (AwbInfo[id].AWBCnt % AWB_RATE)) && ((AwbInfo[id].AWBCnt > AWB_RATE) || (AwbInfo[id].AWBCnt <= AWB_SKIP_FRAME)))
    if ((0 != (AwbInfo[id].AWBCnt % AwbInfo[id].AwbConvSpeed)) && ((AwbInfo[id].AWBCnt > AwbInfo[id].AwbConvSpeed) || (AwbInfo[id].AWBCnt <= AWB_SKIP_FRAME)))
    {
        return;
    }
    else
    {
        // set awb weighting previous frame enable flag
        AwbInfo[id].AWBEnterCnt++;
        if (AwbInfo[id].AWBEnterCnt <= AWB_WEIGHT_SKIP_FRAME)
        {
            AwbInfo[id].AwbWeightEn = FALSE;
        }
        else
        {
            AwbInfo[id].AwbWeightEn = TRUE;
        }
        DBG_IND("AWB_Process in %d  enter in %d\r\n", AwbInfo[id].AWBCnt, AwbInfo[id].AWBEnterCnt);
    }

    Mode = AWB_getMode(IPL_AlgGetUIInfo((IPL_PROC_ID)id, IPL_SEL_WBMODE));
    // for uart cmd debug
    if ( uiRGain[id]!=0 && uiGGain[id]!=0 && uiBGain[id]!=0 )
    {
        AWB_setGain(&AwbInfo[id], uiRGain[id], uiGGain[id], uiBGain[id]);
    }
    else if(Mode == AWB_MODE_AUTO)
    {
        AwbInfo[id].CurEV = AE_GetPrvEVValue(id);

        AWB_getCA(  id,
                    AwbInfo[id].AccTab[AWBALG_CH_R],
                    AwbInfo[id].AccTab[AWBALG_CH_G],
                    AwbInfo[id].AccTab[AWBALG_CH_B],
                    AwbInfo[id].AccCnt,
                    AWB_WIN_TOTAL);
        AWB_autoProc(&AwbInfo[id]);
        AutoRGain = AwbInfo[id].AWBGain.RGain;
        AutoBGain = AwbInfo[id].AWBGain.BGain;

        AWB_setGain(&AwbInfo[id], AwbInfo[id].AWBGain.RGain, AwbInfo[id].AWBGain.GGain, AwbInfo[id].AWBGain.BGain);
        AwbInfo[id].DefGain = AwbInfo[id].AWBGain;
    }
    else
    {
        AWB_manualProc(&AwbInfo[id], Mode);
    }

    if ( (AwbInfo[id].AWBCnt % 120) == 0 )
    {
        if (uiAWBUartCmd[id] == ENABLE)
        {
            DBG_ERR("AWB_ID = %d AWBCnt = %d, GAIN = (%3d %3d %3d)\r\n", id, AwbInfo[id].AWBCnt, AwbInfo[id].AWBGain.RGain, AwbInfo[id].AWBGain.GGain, AwbInfo[id].AWBGain.BGain);
        }
    }
}

void AWB_SetColorGain(UINT32 id, UINT32 Rg,UINT32 Gg,UINT32 Bg)
{
    AWB_setGain(&AwbInfo[id], Rg, Gg, Bg);
}

void AWB_GetColorGain(UINT32 id, UINT32 *Rg,UINT32 *Gg,UINT32 *Bg)
{
    UINT32 AwbId = id;
    SENSOR_ID SensorID = IPL_UTI_CONV2_SEN_ID(id);
    SENSOR_INFO SenInfo = {0};
    ER rt = E_OK;

    if ( AwbAppMode == AWB_APPMODE_SHDR )
    {
        AwbId = AWB_ID_1;
    }

    if((AwbInfo[AwbId].AWBGain.RGain==0)||(AwbInfo[AwbId].AWBGain.GGain==0)||(AwbInfo[AwbId].AWBGain.BGain==0))
    {
        *Rg = AWB_DEF_RGAIN;
        *Gg = AWB_DEF_GGAIN;
        *Bg = AWB_DEF_BGAIN;
    }
    else
    {
        *Rg = AwbInfo[AwbId].AWBGain.RGain;
        *Gg = AwbInfo[AwbId].AWBGain.GGain;
        *Bg = AwbInfo[AwbId].AWBGain.BGain;
    }

    //for sensor hdr
    rt = Sensor_GetStatus(SensorID, IPL_AlgGetUIInfo((IPL_PROC_ID)id, IPL_SEL_PRVSENMODE), &SenInfo);
    if (rt != E_OK || SenInfo.Mode == NULL)
        return;
    if (SenInfo.Mode->ModeType == SENSOR_MODE_BUILTIN_HDR)
    {
        *Rg = 256;
        *Gg = 256;
        *Bg = 256;
    }
}

UINT32 AWB_WriteDebugInfo(UINT32 id, UINT32 Addr)
{
    UINT32 i, j;
    UINT32 AWB_WIN_X = AwbInfo[id].WinNumX;
    UINT32 AWB_WIN_Y = AwbInfo[id].WinNumY;
    UINT32 buf_cnt = 0;

    buf_cnt = snprintf(NULL, 0, "AWBalg mode = %ld\t%08x\r\n", AWB_GetAlgMode(), AwbInfo[id].FuncCtrl);
    Addr += snprintf((char *)Addr, buf_cnt+1, "AWBalg mode = %ld\t%08x\r\n", AWB_GetAlgMode(), AwbInfo[id].FuncCtrl);

    buf_cnt = snprintf(NULL, 0, "Kgain = %ld\t%ld\r\n", AwbInfo[id].KGainInfo.KGain[0].KRGain, AwbInfo[id].KGainInfo.KGain[0].KBGain);
    Addr += snprintf((char *)Addr, buf_cnt+1, "Kgain = %ld\t%ld\r\n", AwbInfo[id].KGainInfo.KGain[0].KRGain, AwbInfo[id].KGainInfo.KGain[0].KBGain);

    buf_cnt = snprintf(NULL, 0, "RGain = \t%ld\r\nGGain = \t%ld\r\nBGain = \t%ld\r\n", AwbInfo[id].AWBGain.RGain, AwbInfo[id].AWBGain.GGain, AwbInfo[id].AWBGain.BGain);
    Addr += snprintf((char *)Addr, buf_cnt+1, "RGain = \t%ld\r\nGGain = \t%ld\r\nBGain = \t%ld\r\n", AwbInfo[id].AWBGain.RGain, AwbInfo[id].AWBGain.GGain, AwbInfo[id].AWBGain.BGain);

    buf_cnt = snprintf(NULL, 0, "Flag Table:\r\n");
    Addr += snprintf((char *)Addr, buf_cnt+1, "Flag Table:\r\n");
    for(j = 0; j < AWB_WIN_Y; j++)
    {
        buf_cnt = snprintf(NULL, 0, "F  \t: ");
        Addr += snprintf((char *)Addr, buf_cnt+1,"F  \t: ");

        for (i = 0; i < AWB_WIN_X; i++)
        {
            buf_cnt = snprintf(NULL, 0, "%d\t", AwbInfo[id].FlagTab[i + (j * AWB_WIN_X)]);
            Addr += snprintf((char *)Addr, buf_cnt+1,"%d\t", AwbInfo[id].FlagTab[i + (j * AWB_WIN_X)]);
        }

        buf_cnt = snprintf(NULL, 0, "\r\n");
        Addr += snprintf((char *)Addr, buf_cnt+1,"\r\n");
    }
//#NT#2016/11/01#Jarkko Chang -begin
//#NT# remove unused code
#if 0
    buf_cnt = snprintf(NULL, 0, "Weight Table:\r\n");
    Addr += snprintf((char *)Addr, buf_cnt+1, "Weight Table:\r\n");

    for(j = 0; j < AWB_WIN_Y; j++)
    {
        buf_cnt = snprintf(NULL, 0, "W  \t: ");
        Addr += snprintf((char *)Addr, buf_cnt+1,"W  \t: ");

        for (i = 0; i < AWB_WIN_X; i++)
        {
            buf_cnt = snprintf(NULL, 0, "%3ld\t", AwbInfo[id].WTab[i + (j * AWB_WIN_X)]);
            Addr += snprintf((char *)Addr, buf_cnt+1,"%3ld\t", AwbInfo[id].WTab[i + (j * AWB_WIN_X)]);
        }

        buf_cnt = snprintf(NULL, 0, "\r\n");
        Addr += snprintf((char *)Addr, buf_cnt+1,"\r\n");
    }
#endif
//#NT#2016/11/01#Jarkko Chang -end
    buf_cnt = snprintf(NULL, 0, "\t");
    Addr += snprintf((char *)Addr, buf_cnt+1,"\t");

    for(j = 0; j < 10; j++)
    {
        buf_cnt = snprintf(NULL, 0, "-");
        Addr += snprintf((char *)Addr, buf_cnt+1,"-");
    }

    buf_cnt = snprintf(NULL, 0, "\r\n");
    Addr += snprintf((char *)Addr, buf_cnt+1,"\r\n");

//#NT#2016/11/01#Jarkko Chang -begin
//#NT# code arrange
    for(j = 0; j < AWB_WIN_Y; j++)
    {
        buf_cnt = snprintf(NULL, 0, "RGB(8b) \t: ");
        Addr += snprintf((char *)Addr, buf_cnt+1,"RGB(8b) \t: ");

        for (i = 0; i < AWB_WIN_X; i++)
        {
            buf_cnt = snprintf(NULL, 0, "%d %d %d\t",AwbInfo[id].AccTab[AWBALG_CH_R][j*AWB_WIN_Y+i]>>2,AwbInfo[id].AccTab[AWBALG_CH_G][j*AWB_WIN_Y+i]>>2,AwbInfo[id].AccTab[AWBALG_CH_B][j*AWB_WIN_Y+i]>>2 );
            Addr += snprintf((char *)Addr, buf_cnt+1,"%d %d %d\t",AwbInfo[id].AccTab[AWBALG_CH_R][j*AWB_WIN_Y+i]>>2,AwbInfo[id].AccTab[AWBALG_CH_G][j*AWB_WIN_Y+i]>>2,AwbInfo[id].AccTab[AWBALG_CH_B][j*AWB_WIN_Y+i]>>2 );
        }

        buf_cnt = snprintf(NULL, 0, "\r\n");
        Addr += snprintf((char *)Addr, buf_cnt+1,"\r\n");
    }
//#NT#2016/11/01#Jarkko Chang -end

//#NT#2016/11/01#Jarkko Chang -begin
//#NT# remove unused code
#if 0
    for(j = 0; j < AWB_WIN_Y; j++)
    {
        buf_cnt = snprintf(NULL, 0, "Cal_RGB  \t: ");
        Addr += snprintf((char *)Addr, buf_cnt+1,"Cal_RGB  \t: ");

        for (i = 0; i < AWB_WIN_X; i++)
        {
            buf_cnt = snprintf(NULL, 0, "%3ld %3ld %3ld\t",
                (AwbInfo[id].AccTab[AWBALG_CH_R][j*AWB_WIN_Y+i] * AwbInfo[id].KGainInfo.KGain[0].KRGain) / 100,
                AwbInfo[id].AccTab[AWBALG_CH_G][j*AWB_WIN_Y+i],
                (AwbInfo[id].AccTab[AWBALG_CH_B][j*AWB_WIN_Y+i] * AwbInfo[id].KGainInfo.KGain[0].KBGain) / 100);
            Addr += snprintf((char *)Addr, buf_cnt+1,"%3ld %3ld %3ld\t",
                (AwbInfo[id].AccTab[AWBALG_CH_R][j*AWB_WIN_Y+i] * AwbInfo[id].KGainInfo.KGain[0].KRGain) / 100,
                AwbInfo[id].AccTab[AWBALG_CH_G][j*AWB_WIN_Y+i],
                (AwbInfo[id].AccTab[AWBALG_CH_B][j*AWB_WIN_Y+i] * AwbInfo[id].KGainInfo.KGain[0].KBGain) / 100);
        }

        buf_cnt = snprintf(NULL, 0, "\r\n");
        Addr += snprintf((char *)Addr, buf_cnt+1,"\r\n");
    }
#endif
//#NT#2016/11/01#Jarkko Chang -end
    return Addr;
}

ER MWB_CalcCustomerColorGain(UINT32 id, SEL_WBMODE mode, UINT32 *RGain,UINT32 *GGain,UINT32 *BGain)
{
    DBG_ERR("not ready\r\n");
#if 0
    UINT32 AWB_WIN_X = AwbInfo[id].WinNumX;
    UINT32 AWB_WIN_Y = AwbInfo[id].WinNumY;
    UINT32 AWB_WIN_TOTAL = AWB_WIN_X * AWB_WIN_Y;

    AWB_getCA(  id,
                AwbInfo[id].AccTab[AWBALG_CH_R],
                AwbInfo[id].AccTab[AWBALG_CH_G],
                AwbInfo[id].AccTab[AWBALG_CH_B],
                AwbInfo[id].AccCnt,
                AWB_WIN_TOTAL);

    UINT32 x, y, idx;
    UINT32 RTotal, GTotal, BTotal, WTotal;
    RTotal = 0;
    GTotal = 0;
    BTotal = 0;
    WTotal = 0;
    for ( y=10; y<20; y++ )
    {
        for ( x=10; x<20; x++ )
        {
            idx =  y * AWB_WIN_X + x;
            if ( AwbInfo[id].AccTab[AWBALG_CH_G][idx]>(5>>4) && AwbInfo[id].AccTab[AWBALG_CH_G][idx]<(200<<4) )
            {
                RTotal += AwbInfo[id].AccTab[AWBALG_CH_R][idx];
                GTotal += AwbInfo[id].AccTab[AWBALG_CH_G][idx];
                BTotal += AwbInfo[id].AccTab[AWBALG_CH_B][idx];
                WTotal += 1;
            }
        }
    }
    if ( RTotal == 0 || BTotal == 0 )
        return E_SYS;
    *RGain = 256 * GTotal / RTotal;
    *GGain = 256;
    *BGain = 256 * GTotal / BTotal;

    DBG_IND("MWB Customer1: %d %d %d\r\n", *RGain, *GGain, *BGain);
#endif
    return E_OK;
}

void MWB_WriteColorGain(UINT32 id, SEL_WBMODE mode, UINT32 Rg,UINT32 Gg,UINT32 Bg)
{
    AWB_MODE AWBMode = AWB_getMode((UINT32)mode);

    if ((Rg > 2048)||(Gg > 2048)||(Bg > 2048))
    {
        DBG_ERR("MWB gain is out of range 2048(Rg %d Gg %d Bg %d)\r\n", Rg, Gg, Bg);
    }
    else
    {
        MwbTAB[id][AWBMode][0] = Rg;
        MwbTAB[id][AWBMode][1] = Gg;
        MwbTAB[id][AWBMode][2] = Bg;
    }
}

void MWB_ReadColorGain(UINT32 id, SEL_WBMODE mode, UINT32 *Rg,UINT32 *Gg,UINT32 *Bg)
{
    AWB_MODE AWBMode = AWB_getMode((UINT32)mode);

    *Rg = MwbTAB[id][AWBMode][0];
    *Gg = MwbTAB[id][AWBMode][1];
    *Bg = MwbTAB[id][AWBMode][2];
}

#if AWB_CAPRAW_EMB_DEBUG_INFO
AWBALG_DEBUG_INFO awbDebugInfo;
#endif

UINT32 AWB_SetEmbDebugInfo(UINT32 id, UINT32 *aDebugInfo)
{
    *aDebugInfo = 0;

#if AWB_CAPRAW_EMB_DEBUG_INFO
    *aDebugInfo = (UINT32)&awbDebugInfo;

    IPL_SIE_RAW_INFO RawInfo;
    RawInfo.Id = id;
    IPL_GetCmd(IPL_GET_SIE_CUR_CAP_CH0_RAW_INFOR, &RawInfo);

    UINT32 AWB_WIN_X = AwbInfo[id].WinNumX;
    UINT32 AWB_WIN_Y = AwbInfo[id].WinNumY;
    UINT32 AWB_WIN_TOTAL = AWB_WIN_X * AWB_WIN_Y;
    UINT32 Rg, Gg, Bg;
    Rg = AwbInfo[id].AWBGain.RGain;
    Gg = AwbInfo[id].AWBGain.GGain;
    Bg = AwbInfo[id].AWBGain.BGain;

    awbDebugInfo.StartTag    = 0x12345678;
    awbDebugInfo.ImgWidth    = RawInfo.ImgWidth;
    awbDebugInfo.ImgHeight   = RawInfo.ImgHeight;
    awbDebugInfo.ImgBitDepth = RawInfo.ImgBit;
    awbDebugInfo.CurRgain = Rg;
    awbDebugInfo.CurGgain = Gg;
    awbDebugInfo.CurBgain = Bg;
    awbDebugInfo.PrevRgain = Rg;
    awbDebugInfo.PrevGgain = Gg;
    awbDebugInfo.PrevBgain = Bg;
    awbDebugInfo.CapRgain = Rg;
    awbDebugInfo.CapGgain = Gg;
    awbDebugInfo.CapBgain = Bg;
    awbDebugInfo.EV = AwbInfo[id].CurEV;
    awbDebugInfo.LV = AwbInfo[id].CurEV;
    awbDebugInfo.CABits = AWB_ACC_BIT;
    memcpy(awbDebugInfo.CA_R, AwbInfo[id].AccTab[AWBALG_CH_R], sizeof(UINT16)*AWB_WIN_TOTAL);
    memcpy(awbDebugInfo.CA_G, AwbInfo[id].AccTab[AWBALG_CH_G], sizeof(UINT16)*AWB_WIN_TOTAL);
    memcpy(awbDebugInfo.CA_B, AwbInfo[id].AccTab[AWBALG_CH_B], sizeof(UINT16)*AWB_WIN_TOTAL);
    memcpy(awbDebugInfo.CACnt, AwbInfo[id].AccCnt, sizeof(UINT16)*AWB_WIN_TOTAL);
#if 0
    URECT TargetCoord;
    UINT32 FDCnt = 0, i;
    FD_FACE Face_rslt[FD_NUM_MAX];
    TargetCoord.x = 0;
    TargetCoord.y = 0;
    TargetCoord.w = awbDebugInfo.ImgWidth;
    TargetCoord.h = awbDebugInfo.ImgHeight;
    FDCnt = FD_GetRsltFace(Face_rslt, &TargetCoord);
    awbDebugInfo.FaceTabCnt = FDCnt;
    for ( i=0; i<FDCnt; i++ )
    {
        awbDebugInfo.FaceTab[i].x = Face_rslt[i].x;
        awbDebugInfo.FaceTab[i].y = Face_rslt[i].y;
        awbDebugInfo.FaceTab[i].w = Face_rslt[i].w;
        awbDebugInfo.FaceTab[i].h = Face_rslt[i].h;
        DBG_ERR("%d, %d %d\r\n", i, awbDebugInfo.FaceTab[i].x, awbDebugInfo.FaceTab[i].y);
    }
#endif
    *aDebugInfo = (UINT32)&awbDebugInfo;
    return sizeof(AWBALG_DEBUG_INFO);
#else
    return 0;
#endif
}

////////////////////////////////////////////////////////
//
// static function
//
////////////////////////////////////////////////////////

static void AWB_setGain(AWBALG_INFO *Info, UINT32 RG, UINT32 GG, UINT32 BG)
{
    SENSOR_INFO SenInfo = {0};
    SENSOR_ID SensorID = IPL_UTI_CONV2_SEN_ID(Info->Id);
    ER rt = E_OK;

    if( Sensor_IsOpen(Info->Id)==0 )
    {
        DBG_ERR("Sensor %d is not open!!!!\r\n",Info->Id);
        return;
    }

    //for sensor hdr
    rt = Sensor_GetStatus(SensorID, IPL_AlgGetUIInfo((IPL_PROC_ID)Info->Id, IPL_SEL_PRVSENMODE), &SenInfo);

    if (rt != E_OK || SenInfo.Mode == NULL)
        return;

    if (SenInfo.Mode->ModeType == SENSOR_MODE_BUILTIN_HDR)
    {

        SENSOR_CMD Cmd[4];

        //G1
        Cmd[0].uiAddr = 0x3056;
        Cmd[0].uiData[0] = (GG * 128 * 1040) / 256 / 1000;
        Cmd[0].uiData[1] = 0;
        Cmd[0].uiDataLen = 2;
        //B
        Cmd[1].uiAddr = 0x3058;
        Cmd[1].uiData[0] = (BG * 128 * 1040) / 256 / 1000;
        Cmd[1].uiData[1] = 0;
        Cmd[1].uiDataLen = 2;
        //R
        Cmd[2].uiAddr = 0x305A;
        Cmd[2].uiData[0] = (RG * 128 * 1040) / 256 / 1000;
        Cmd[2].uiData[1] = 0;
        Cmd[2].uiDataLen = 2;
        //G2
        Cmd[3].uiAddr = 0x305c;
        Cmd[3].uiData[0] = (GG * 128 * 1040) / 256 / 1000;
        Cmd[3].uiData[1] = 0;
        Cmd[3].uiDataLen = 2;

        Sensor_WriteReg(IPL_UTI_CONV2_SEN_ID(Info->Id), &Cmd[0]);
        Sensor_WriteReg(IPL_UTI_CONV2_SEN_ID(Info->Id), &Cmd[1]);
        Sensor_WriteReg(IPL_UTI_CONV2_SEN_ID(Info->Id), &Cmd[2]);
        Sensor_WriteReg(IPL_UTI_CONV2_SEN_ID(Info->Id), &Cmd[3]);

        Info->AWBGain.RGain = RG;
        Info->AWBGain.GGain = GG;
        Info->AWBGain.BGain = BG;
    }
    else
    {
        Info->AWBGain.RGain = RG;
        Info->AWBGain.GGain = GG;
        Info->AWBGain.BGain = BG;
        //DBG_ERR("%d %d %d\r\n", RG, GG, BG);
    }
    IQS_SettingFlowFunc(Info->Id, IQS_AWB_END);
}

static void AWB_getCA(UINT32 id, UINT16 *R, UINT16 *G, UINT16 *B, UINT16 *Cnt, UINT32 Size)
{
    UINT16 i, j;
    IPL_CA_INFOR CA1Info, CA2Info;

    UINT32 AWB_WIN_X = AwbInfo[id].WinNumX;
    UINT32 AWB_WIN_Y = AwbInfo[id].WinNumY;
    UINT32 AWB_WIN_TOTAL = AWB_WIN_X * AWB_WIN_Y;
    SENSOR_INFO SenInfo = {0};
    SENSOR_ID SensorID = IPL_UTI_CONV2_SEN_ID(id);
    ER rt = E_OK;

    if ( AwbAppMode == AWB_APPMODE_NORMAL )
    {
        CA1Info.Id = id;
        CA1Info.R = R;
        CA1Info.G = G;
        CA1Info.B = B;
        CA1Info.AccCnt = Cnt;
        CA1Info.Length = AWB_WIN_TOTAL;
        IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CA1Info);
        for ( i=0; i< AWB_WIN_TOTAL; i++ )
        {
            CA1Info.R[i] = (CA1Info.R[i]>>(12-AWB_ACC_BIT));
            CA1Info.G[i] = (CA1Info.G[i]>>(12-AWB_ACC_BIT));
            CA1Info.B[i] = (CA1Info.B[i]>>(12-AWB_ACC_BIT));
            if ( CA1Info.AccCnt[i] == 0 )
                CA1Info.AccCnt[i] = 1;
        }

        //for sensor hdr
        rt = Sensor_GetStatus(SensorID, IPL_AlgGetUIInfo((IPL_PROC_ID)id, IPL_SEL_PRVSENMODE), &SenInfo);
        if (rt != E_OK || SenInfo.Mode == NULL)
            return;

        if (SenInfo.Mode->ModeType == SENSOR_MODE_BUILTIN_HDR)
        {
            for ( i=0; i< AWB_WIN_TOTAL; i++ )
            {
                CA1Info.R[i] = (CA1Info.R[i]*256)/AwbInfo[id].AWBGain.RGain;
                CA1Info.G[i] = (CA1Info.G[i]*256)/AwbInfo[id].AWBGain.GGain;
                CA1Info.B[i] = (CA1Info.B[i]*256)/AwbInfo[id].AWBGain.BGain;
            }
        }
    }
    else if ( AwbAppMode == AWB_APPMODE_STITCH && id == AWB_ID_1 )
    {
        if ( AwbPreWhiteElement[AWB_ID_1].WinNumX!=AwbPreWhiteElement[AWB_ID_2].WinNumX ||
             AwbPreWhiteElement[AWB_ID_1].WinNumY!=AwbPreWhiteElement[AWB_ID_2].WinNumY)
             DBG_ERR("Wrong CA win size for STITCH mode\r\n");

        CA1Info.Id = IPL_ID_1;
        CA1Info.R = AccTab[AWB_ID_1][AWBALG_CH_R];
        CA1Info.G = AccTab[AWB_ID_1][AWBALG_CH_G];
        CA1Info.B = AccTab[AWB_ID_1][AWBALG_CH_B];
        CA1Info.AccCnt = AccCnt[AWB_ID_1];
        CA1Info.Length = AWB_WIN_TOTAL;
        IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CA1Info);

        CA2Info.Id = IPL_ID_2;
        CA2Info.R = AccTab[AWB_ID_2][AWBALG_CH_R];
        CA2Info.G = AccTab[AWB_ID_2][AWBALG_CH_G];
        CA2Info.B = AccTab[AWB_ID_2][AWBALG_CH_B];
        CA2Info.AccCnt = AccCnt[AWB_ID_2];
        CA2Info.Length = AWB_WIN_TOTAL;
        IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CA2Info);

        for ( j=0; j<AWB_WIN_Y; j++ )
        {
            UINT32 LineHead = j*AWB_WIN_X;
            for ( i=0; i<(AWB_WIN_X>>1); i++ )
            {
                R[LineHead+i] = (CA1Info.R[LineHead+i*2]>>(12-AWB_ACC_BIT));
                G[LineHead+i] = (CA1Info.G[LineHead+i*2]>>(12-AWB_ACC_BIT));
                B[LineHead+i] = (CA1Info.B[LineHead+i*2]>>(12-AWB_ACC_BIT));
                Cnt[LineHead+i] = (CA1Info.AccCnt[LineHead+i*2]);
            }
            for ( i=(AWB_WIN_X>>1); i<AWB_WIN_X; i++ )
            {
                R[LineHead+i] = (CA2Info.R[LineHead+i*2-AWB_WIN_X]>>(12-AWB_ACC_BIT));
                G[LineHead+i] = (CA2Info.G[LineHead+i*2-AWB_WIN_X]>>(12-AWB_ACC_BIT));
                B[LineHead+i] = (CA2Info.B[LineHead+i*2-AWB_WIN_X]>>(12-AWB_ACC_BIT));
                Cnt[LineHead+i] = (CA2Info.AccCnt[LineHead+i*2-AWB_WIN_X]);
            }
        }
    }
    else if ( AwbAppMode == AWB_APPMODE_SHDR && id == AWB_ID_1 )
    {
        if ( AwbPreWhiteElement[AWB_ID_1].WinNumX!=AwbPreWhiteElement[AWB_ID_2].WinNumX ||
             AwbPreWhiteElement[AWB_ID_1].WinNumY!=AwbPreWhiteElement[AWB_ID_2].WinNumY)
             DBG_ERR("Wrong CA win size for SHDR mode\r\n");

        CA1Info.Id = IPL_ID_1;
        CA1Info.R = AccTab[AWB_ID_1][AWBALG_CH_R];
        CA1Info.G = AccTab[AWB_ID_1][AWBALG_CH_G];
        CA1Info.B = AccTab[AWB_ID_1][AWBALG_CH_B];
        CA1Info.AccCnt = AccCnt[AWB_ID_1];
        CA1Info.Length = AWB_WIN_TOTAL;
        IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CA1Info);

        CA2Info.Id = IPL_ID_2;
        CA2Info.R = AccTab[AWB_ID_2][AWBALG_CH_R];
        CA2Info.G = AccTab[AWB_ID_2][AWBALG_CH_G];
        CA2Info.B = AccTab[AWB_ID_2][AWBALG_CH_B];
        CA2Info.AccCnt = AccCnt[AWB_ID_2];
        CA2Info.Length = AWB_WIN_TOTAL;
        IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CA2Info);

        for ( j=0; j<AWB_WIN_TOTAL; j++ )
        {
            if ( CA1Info.AccCnt[j] > CA2Info.AccCnt[j] )
            {
                R[j] = (CA1Info.R[j]>>(12-AWB_ACC_BIT));
                G[j] = (CA1Info.G[j]>>(12-AWB_ACC_BIT));
                B[j] = (CA1Info.B[j]>>(12-AWB_ACC_BIT));
                Cnt[j] = (CA1Info.AccCnt[j]);
            }
            else
            {
                R[j] = (CA2Info.R[j]>>(12-AWB_ACC_BIT));
                G[j] = (CA2Info.G[j]>>(12-AWB_ACC_BIT));
                B[j] = (CA2Info.B[j]>>(12-AWB_ACC_BIT));
                Cnt[j] = (CA2Info.AccCnt[j]);
            }
        }
    }
}

static AWB_MODE AWB_getMode(UINT32 Idx)
{
    if (Idx == SEL_WBMODE_AUTO) return AWB_MODE_AUTO;
    if (Idx == SEL_WBMODE_DAYLIGHT) return AWB_MODE_DAYLIGHT;
    if (Idx == SEL_WBMODE_CLOUDY) return AWB_MODE_CLOUDY;
    if (Idx == SEL_WBMODE_TUNGSTEN) return AWB_MODE_TUNGSTEN;
    if (Idx == SEL_WBMODE_FLUORESCENT1) return AWB_MODE_FLUORESCENT1;
    if (Idx == SEL_WBMODE_FLUORESCENT2) return AWB_MODE_FLUORESCENT2;
    if (Idx == SEL_WBMODE_SUNSET) return AWB_MODE_SUNSET;
    if (Idx == SEL_WBMODE_BIRTHDAYPARTY) return AWB_MODE_BIRTHDAYPARTY;
    if (Idx == SEL_WBMODE_CUSTOMER1) return AWB_MODE_CUSTOMER1;
    if (Idx == SEL_WBMODE_CUSTOMER2) return AWB_MODE_CUSTOMER2;
    if (Idx == SEL_WBMODE_CUSTOMER3) return AWB_MODE_CUSTOMER3;
    if (Idx == SEL_WBMODE_MGAIN) return AWB_MODE_MGAIN;
    return AWB_MODE_AUTO;
}

static void AWB_autoProc(AWBALG_INFO *Info)
{
    UINT32 NewRGain, NewGGain, NewBGain;
    AWBALG_ER ER_code;
    NewGGain = AWB_DEF_GGAIN;
    ER_code = AWBSim(Info, &NewRGain, &NewGGain, &NewBGain);
    if(ER_code == AWBALG_OK)
    {
        /*if (Info->AWBEnter == 1)
        {
            Info->AWBEnter = 0;
        }*/
        Info->AWBGain.RGain = NewRGain;
        Info->AWBGain.GGain = NewGGain;
        Info->AWBGain.BGain = NewBGain;
    }
    Info->PreGain = Info->AWBGain;
}

static void AWB_manualProc(AWBALG_INFO *Info, AWB_MODE Idx)
{
    UINT32 KRGain, KBGain;
    Info->AWBGain.RGain = MwbTAB[Info->Id][Idx][0];
    Info->AWBGain.GGain = MwbTAB[Info->Id][Idx][1];
    Info->AWBGain.BGain = MwbTAB[Info->Id][Idx][2];

    if ( (Idx != AWB_MODE_CUSTOMER1) && (Idx != AWB_MODE_CUSTOMER2) && (Idx != AWB_MODE_CUSTOMER3) )
    {
        AWBALG_GetKGain(Info, Info->AWBGain.RGain, Info->AWBGain.BGain, &KRGain, &KBGain);
        Info->AWBGain.RGain = Info->AWBGain.RGain*100/KRGain;
        Info->AWBGain.BGain = Info->AWBGain.BGain*100/KBGain;
    }
    AWB_setGain(Info, Info->AWBGain.RGain, Info->AWBGain.GGain, Info->AWBGain.BGain);
}

void AWB_Get_ColorAvg(UINT32 Id, UINT32* r, UINT32* g, UINT32* b)
{
    UINT32 AWB_WIN_X = AwbInfo[Id].WinNumX;
    UINT32 AWB_WIN_Y = AwbInfo[Id].WinNumY;
    UINT32 AWB_WIN_TOTAL = AWB_WIN_X * AWB_WIN_Y;
    IPL_CA_INFOR CAInfo;
    CAInfo.Id = Id;
    CAInfo.Length = AWB_WIN_TOTAL;
    CAInfo.R = AwbInfo[Id].AccTab[AWBALG_CH_R];
    CAInfo.G = AwbInfo[Id].AccTab[AWBALG_CH_G];
    CAInfo.B = AwbInfo[Id].AccTab[AWBALG_CH_B];
    CAInfo.AccCnt = AwbInfo[Id].AccCnt;
    IPL_GetCmd(IPL_GET_CUR_CA_INFOR, (void *)&CAInfo);

    UINT32 i;
    (*r) = 0;
    (*g) = 0;
    (*b) = 0;
    for(i=0; i<AWB_WIN_TOTAL; i++){
        (*r) += (CAInfo.R[i]>>(12-AWB_ACC_BIT));
        (*g) += (CAInfo.G[i]>>(12-AWB_ACC_BIT));
        (*b) += (CAInfo.B[i]>>(12-AWB_ACC_BIT));

    }
    (*r) /= AWB_WIN_TOTAL;
    (*g) /= AWB_WIN_TOTAL;
    (*b) /= AWB_WIN_TOTAL;

}


