/**
    Copyright   Novatek Microelectronics Corp. 2008.  All rights reserved.

    @file       AF_Lib.c
    @ingroup    mISYSLens

    @brief      Auto focus library

    @note       Nothing.

    @version    V1.00.000
    @author     IM SA2
    @date       2014
*/
#include <stdio.h>
#include "Debug.h"
#include "SysKer.h"
#include "IPL_AlgInfor.h"
#include "Lens.h"
#include "AF_Lib.h"
#include "af_task.h"
#include "Utility.h"
//#include "ipe_lib.h"
#include "string.h"
#include "sie_lib.h"
//#include "ipe_lib.h"
#include "fd_lib.h"
#include <stdlib.h>
#include "afdrv_common.h"
#include "af_api.h"
#include "af_alg.h"
#include "Type.h"
#include "af_alg.h"
#include "af_api.h"
#include "ae_api.h"
#include "IPL_Cmd.h"
//#include "IPL_Hal_IPE.h"
//#include "IPL_Hal_IPE_Info.h"
#include "IPL_Ctrl.h"
#include "Perf.h"

#define AF_PRE_EXCITE_ALWAYS    DISABLE

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          AFSAMPLE
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#define AF_SAMPLE_TODO    0
#define AF_ALG_FIND_PEAK_INTERPOLATION ENABLE
#define AF_PERF_MEASURE_EN             DISABLE

#define AF_VA_CURVE_DROP_THRESHOLD     0.8

typedef enum
{
    AF_Alg1 = 0,
    AF_Alg2 = 1,
    AF_Alg3 = 2,
    AF_Alg4 = 3,
    AF_Alg5 = 4,
    ENUM_DUMMY4WORD(AF_Alg)
} AF_Alg;

AF_Alg AF_SearchingAlg = AF_Alg1;

/**
    IPL va group id
*/
typedef enum
{
    VA_GRP1H = 1,
    VA_GRP1V = 2,
    VA_GRP2H = 3,
    VA_GRP2V = 4,
    VA_GRP1  = 5,
    VA_GRP2  = 6,
    ENUM_DUMMY4WORD(VA_GRP)
}VA_GRP;

/**
    IPL va window id
*/
typedef enum
{
    _VA_WIN_16X16 = 0,               ///< va 16 * 16 window
    _VA_WIN_INDP0,                   ///< va independent window 0
    _VA_WIN_INDP1,                   ///< va independent window 1
    _VA_WIN_INDP2,                   ///< va independent window 2
    _VA_WIN_INDP3,                   ///< va independent window 3
    _VA_WIN_INDP4,                   ///< va independent window 4
    _VA_WIN_CROP,
    ENUM_DUMMY4WORD(VA_WIN_IDX)
}VA_WIN_IDX;

static IPL_SET_VA_DATA g_tVA_Data;

#if AF_SAMPLE_TODO
static FPAF_DETGPIOKEY g_bfpAfDetShutter2 = NULL;
#endif

static UINT32 g_AFInfoIdx = 0, VaildStartIdx=0;
static UINT32 g_AFGuessInfoIdx = 0;
static UINT32 g_AFMaxIdx = 0;
static BOOL   g_AFAlgCentralWinFound = 0;
static INT32 g_AFAlgMaxIdx = 0;
static AFDRV_MANUAL g_taf_manual = {0};
static UINT32 g_uiGetVAMoreTime = 0;
#if AF_PERF_MEASURE_EN
static UINT32 g_uiAFPerfTime = 0;
#endif

AF_INFO g_AFInfo = {0};
AF_GUESS_INFO g_AFGuessInfo = {0};
BOOL g_AFfocusforAllWin[AF_FOCUS_TOTAL_WIN] = {0};

AF_CBMSG_FP gAfCbFP = NULL; // Callback funtion to notify UI.

#if 0 // VA TBD.
AF_IPEINFO gAFIPEInfo={0};
#endif

INT32 Focus_TempComp_Near[] = {58,59,63,58,59,61,63,64,63,62,58,59,63};
INT32 Focus_TempComp_Far[] = {58,59,63,58,59,61,63,64,63,62,58,59,63};
static BOOL g_bAFChangeMode = TRUE;

UINT32 g_uiaf_peak_offset = 2;

UINT32 AF_CenterDistance = 80; // 0.8m

#define Damping_Max 6
#define AF_Status_OK 0x80
#define AF_Status_NG 0xFF

void AF_ResetAFGuessInfo(void);

AF_AETRANSFERMODE AFdrv_GetAFModel(void)
{
    return Model_Normal;
}

void AFdrv_SetAFModeChange(BOOL EN)
{
    if(EN)
        g_bAFChangeMode = TRUE;
    else
        g_bAFChangeMode = FALSE;
}

BOOL AFdrv_CheckAFModeChange(void)
{
    return g_bAFChangeMode;
}

void AFdrv_AFPrepare(BOOL bOP, UINT32 step)
{
}

void AFdrv_AEPrepare(BOOL bOP, UINT32 step)
{
}

void AFdrv_SetManualRange(AFDRV_MANUAL ManualSet)
{
    g_taf_manual = ManualSet;
}

void AFdrv_GetCurrentRange(UINT32 id, PAFALG_INFO_IN af_Obj, SEL_AFMODE AFMode)
{
    UINT8 uiZoomSection = Lens_Zoom_GetSection(ZOOM_CUR_SECTION);
    BOOL Compensate_enable = TRUE;

    //Step1
    switch(AFMode)
    {
        case SEL_AFMODE_SUPER_MACRO:
        case SEL_AFMODE_MACRO:
            af_Obj->iStartIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 6) - 20; // 50cm distance.;
            af_Obj->iEndIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 11) + 12; // 50cm distance.
            af_Obj->iStep = 8;
            af_Obj->iDefaultIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 6);
            Compensate_enable = FALSE;
            DBG_MSG("SEL_AFMODE_MACRO\r\n");
            break;

        case SEL_AFMODE_INIFINITE:
            af_Obj->iStartIdx = Lens_Focus_GetTable(uiZoomSection-1,0);
            af_Obj->iEndIdx = Lens_Focus_GetTable(uiZoomSection-1,1);
            af_Obj->iStep = 6+(4*(uiZoomSection-1))/12;
            //af_Obj->iStep = 12+(6*(uiZoomSection-1))/12;
            af_Obj->iDefaultIdx = Lens_Focus_GetTable(uiZoomSection-1,0);
            break;

        case SEL_AFMODE_MANUAL:
            af_Obj->iStartIdx = g_taf_manual.iManualStartIdx;
            af_Obj->iEndIdx = g_taf_manual.iManualEndIdx;
            af_Obj->iStep = g_taf_manual.iManualStep;
            af_Obj->iDefaultIdx = Lens_Focus_GetTable(uiZoomSection-1,2);
            Compensate_enable = FALSE;
            break;

        case SEL_AFMODE_CONTINUEFOCUS:
            af_Obj->iStartIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 0) - 12; // INF distance.
            af_Obj->iEndIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 11); // 50cm distance.
            break;

        case SEL_AFMODE_AUTO:
        default:
            af_Obj->iStartIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 0) - 20; // INF distance.
            af_Obj->iEndIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 6) + 100; // 50cm distance.
            af_Obj->iDefaultIdx = Lens_Focus_GetTable(Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1, 2) ;
            if(AF_Alg2 == AF_SearchingAlg)
            {
                if(uiZoomSection < 6)
                    af_Obj->iStep = 10;
                else if(uiZoomSection < 8)
                    af_Obj->iStep = 15;
                else
                    af_Obj->iStep = 20;
            }
            else
            {
                if(uiZoomSection < 6)
                    af_Obj->iStep = 4;
                else if(uiZoomSection < 8)
                    af_Obj->iStep = 8;
                else
                    af_Obj->iStep = 12;
            }

            Compensate_enable = FALSE;
            DBG_MSG("#SEL_AFMODE_AUTO\r\n");
            break;
    }

    if(IPL_AlgGetUIInfo(id, IPL_SEL_AFWINDOW) == AFALG_WINDOW_CENTER)
        af_Obj->iOffsetStep = (g_uiaf_peak_offset & 0x03);
    else
        af_Obj->iOffsetStep = 2; //1;

    af_Obj->iCurIdx = af_Obj->iStartIdx;

    DBG_MSG("AF mode = %d; %d -> %d ,Step = %d, offset TblIdx = %d\r\n", AFMode, af_Obj->iStartIdx, af_Obj->iEndIdx, af_Obj->iStep, af_Obj->iOffsetStep);
}

UINT16 AFdrv_GetFocusDist(void)
{
#if 1
    UINT8 i;
    UINT32 uiDist;
    UINT8 uiZoomSecMinusOne;
    //#Rearrange lens driver
    uiZoomSecMinusOne = Lens_Zoom_GetSection(ZOOM_CUR_SECTION)-1;

    for(i=0; i<11; i++)
    {
         if(Lens_Focus_GetPosition()<Lens_Focus_GetTable(uiZoomSecMinusOne,i))
            break;
    }

    if(i==0)
    {
        uiDist= Lens_Focus_GetDistTable(0);
    }
    else if(i==11)
    {
        uiDist= Lens_Focus_GetDistTable(10);
    }
    else
    {
        UINT32 uiAval, uiBval, uiAintvl, uiBintvl;

        uiAval = Lens_Focus_GetDistTable(i-1);
        uiBval = Lens_Focus_GetDistTable(i);
        uiAintvl = Lens_Focus_GetPosition() - Lens_Focus_GetTable(uiZoomSecMinusOne,i-1);
        uiBintvl = Lens_Focus_GetTable(uiZoomSecMinusOne,i)-Lens_Focus_GetPosition();
        uiDist = (uiAval*uiBintvl + uiBval*uiAintvl)/(uiAintvl+uiBintvl);
    }

    return (UINT16)uiDist;
#else
    return 100;
#endif
}

UINT32 AF_WriteDebugInfo(UINT32 Addr)
{
    UINT32 i;
    //Get AF information
    PAFALG_INFO pGetafinfo = AFALG_GetINFO();
    UINT8 uiZoomSection = Lens_Zoom_GetSection(ZOOM_CUR_SECTION);
    UINT32 buf_cnt;

    buf_cnt = snprintf(NULL, 0 , "\r\n\r\n\r\nAF Info------------------------------------------------------------------------------------------------------------------------\r\n");
    Addr += snprintf((char *)Addr,buf_cnt+1, "\r\n\r\n\r\nAF Info------------------------------------------------------------------------------------------------------------------------\r\n");

    buf_cnt = snprintf(NULL, 0 , "ZoomSec = \t%d\r\nOriPeak = \t%ld\r\nPredictPeak = \t%ld\r\nSts = \t%08lx\r\n");
    Addr += snprintf((char *)Addr,buf_cnt+1,"ZoomSec = \t%d\r\nOriPeak = \t%ld\r\nPredictPeak = \t%ld\r\nSts = \t%08lx\r\n",
                    uiZoomSection, g_AFMaxIdx, pGetafinfo->afalg_out.iPeak, pGetafinfo->afalg_out.uiStatus);

    buf_cnt = snprintf(NULL, 0 , "UI setting: AF mode=%d, AF Win=%d\r\n");
    Addr += snprintf((char *)Addr,buf_cnt+1, "UI setting: AF mode=%d, AF Win=%d\r\n", IPL_AlgGetUIInfo(pGetafinfo->afalg_in.id, IPL_SEL_AFMODE), IPL_AlgGetUIInfo(pGetafinfo->afalg_in.id, IPL_SEL_AFWINDOW));

    buf_cnt = snprintf(NULL, 0 , "VA =\r\n");
    Addr += snprintf((char *)Addr,buf_cnt+1, "VA =\r\n");

    for(i = 0; i < g_AFInfoIdx && i < AF_FOCUS_ARRAY_SIZE; i++)
    {
        buf_cnt = snprintf(NULL, 0 , "%d\t");
        Addr += snprintf((char *)Addr, buf_cnt+1,"%d\t", g_AFInfo.Value[i]);
    }

    buf_cnt = snprintf(NULL, 0 , "\r\nStep =\r\n");
    Addr += snprintf((char *)Addr, buf_cnt+1,"\r\nStep =\r\n");
    for(i = 0; i < g_AFInfoIdx && i < AF_FOCUS_ARRAY_SIZE; i++)
    {
        buf_cnt = snprintf(NULL, 0 , "%d\t");
        Addr += snprintf((char *)Addr, buf_cnt+1,"%d\t", g_AFInfo.Step[i]);
    }


    buf_cnt = snprintf(NULL, 0 , "\r\n");
    Addr += snprintf((char *)Addr, buf_cnt+1,"\r\n");

    return Addr;
}



void AF_SetManualRange(INT16 iStartIdx, INT16 iEndIdx, INT8 iStep)
{
    AFDRV_MANUAL Manual_Setting;

    Manual_Setting.iManualStartIdx = iStartIdx;
    Manual_Setting.iManualEndIdx = iEndIdx;
    Manual_Setting.iManualStep = iStep;
    AFdrv_SetManualRange(Manual_Setting);
}

void AF_GetResult(INT16* piStartIdx, INT16* piEndIdx, INT8* piStep, INT16* piOffsetStep, INT16 *piMaxIdx, UINT8 *puiSelectWin, UINT8 *puiStatus)
{
    PAFALG_INFO Getafinfo;
    Getafinfo = AFALG_GetINFO();
    if(piStartIdx!=NULL)
        *piStartIdx = Getafinfo->afalg_in.iStartIdx;
    if(piEndIdx!=NULL)
        *piEndIdx = Getafinfo->afalg_in.iEndIdx;
    if(piStep!=NULL)
        *piStep = Getafinfo->afalg_in.iStep;
    if(piMaxIdx!=NULL)
        *piMaxIdx = Getafinfo->afalg_out.iPeak;
    if(puiSelectWin!=NULL)
        *puiSelectWin = Getafinfo->afalg_out.uiSelectWin[0];
    if(piOffsetStep!=NULL)
        *piOffsetStep = Getafinfo->afalg_in.iOffsetStep;
    if(puiStatus!=NULL)
        *puiStatus = Getafinfo->afalg_out.uiStatus;
}

void AF_SetAlgSearchMode(AF_ALG_SEARCH_MODE af_alg_search_mode)
{
    PAFALG_INFO pGetafinfo = AFALG_GetINFO();

    if (af_alg_search_mode <= AF_ALG_SMODE_FALLNORMAL_NOFD)
        pGetafinfo->afalg_in.Search_mode = af_alg_search_mode;
    else
        DBG_ERR("AF alg search mode incorrect!\r\n");
}

AF_ALG_SEARCH_MODE AF_GetAlgSearchMode(void)
{
    PAFALG_INFO pGetafinfo = AFALG_GetINFO();

    return pGetafinfo->afalg_in.Search_mode;
}

void AF_RegisterCB(AF_CBMSG_FP CB)
{
    gAfCbFP = CB;
}

static INT32 Af_GetNextFocusPos(MOTOR_FOCUS_ACT dir, UINT32 step, UINT32 MinIdx, UINT32 MaxIdx)
{
    UINT32 NextIdx;

    if(dir == MOTOR_FOCUS_FWD)
        NextIdx=Lens_Focus_GetPosition() + step;
    else
        NextIdx=Lens_Focus_GetPosition() - step;

    if(NextIdx > MaxIdx+4*step)
        NextIdx = MaxIdx+4*step;
    else if(NextIdx < MinIdx-4*step)
        NextIdx = MinIdx-4*step;

    return (INT32)NextIdx;
}

static UINT32 Af_GetMaxIndex(UINT32 StartIdx, UINT32 EndIdx)
{
    UINT32 i, BestIdx = EndIdx, VAMAX = 0;

    //Sometimes 0 is unstable
    for(i=StartIdx; i<=EndIdx; i++)
    {
        if(g_AFInfo.Value[i] > VAMAX)
        {
            VAMAX = g_AFInfo.Value[i];
            BestIdx = i;
        }
        //DBG_MSG("i %d VA %d\r\n",i,Caf_Param.uiVAValue[i]);
    }
    //DBG_ERR("MAX =%d VA %d\r\n",BestIdx,g_AFInfo.Value[BestIdx]);
    return BestIdx;
}

static void AF_Init_Alg1(UINT32 id, UINT32 CurrentStatus)
{
    UINT32 uiAFmode;
    PAFALG_INFO pGetafinfo = AFALG_GetINFO();

    DBG_MSG("#AF_Init1\r\n");

    /* Setup searching range. */
    uiAFmode = IPL_AlgGetUIInfo(id, IPL_SEL_AFMODE);
    AFdrv_GetCurrentRange(id, &pGetafinfo->afalg_in, uiAFmode);
    g_uiGetVAMoreTime = pGetafinfo->afalg_in.iOffsetStep;

    pGetafinfo->afalg_in.bObj = FALSE;

    switch(IPL_AlgGetUIInfo(id, IPL_SEL_AFWINDOW))
    {
        case SEL_AFWINDOW_AUTO:
            pGetafinfo->afalg_in.Window_sel = AFALG_WINDOW_AUTO;
            break;
        case SEL_AFWINDOW_MANUALPOSITION:
            pGetafinfo->afalg_in.Window_sel = AFALG_WINDOW_MANUIAL;
            break;
        case SEL_AFWINDOW_CENTER:
            pGetafinfo->afalg_in.Window_sel = AFALG_WINDOW_CENTER;
            break;
        default:
            DBG_ERR("ERROR!! Exception AFWindow!\r\n");
            break;
    }

    /* Move focus to the start idx of searching range. */
    if(CurrentStatus == 0)
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, pGetafinfo->afalg_in.iStartIdx);
    else
        Lens_Focus_DoAction(FOCUS_RUN, pGetafinfo->afalg_in.iStartIdx);
    IPC_WaitVD(id,TRUE); // wait until VA stable.
}

static void AF_Init_Alg2(UINT32 id, UINT32 CurrentStatus)
{
    UINT32 uiAFmode;
    PAFALG_INFO pGetafinfo = AFALG_GetINFO();
    INT32 StartIdx, CurIdx;

    DBG_MSG("#AF_Init2\r\n");

    /* Setup searching range. */
    uiAFmode = IPL_AlgGetUIInfo(id, IPL_SEL_AFMODE);
    AFdrv_GetCurrentRange(id, &pGetafinfo->afalg_in, uiAFmode);
    g_uiGetVAMoreTime = pGetafinfo->afalg_in.iOffsetStep;

    pGetafinfo->afalg_in.bObj = FALSE;

    switch(IPL_AlgGetUIInfo(id, IPL_SEL_AFWINDOW))
    {
        case SEL_AFWINDOW_AUTO:
            pGetafinfo->afalg_in.Window_sel = AFALG_WINDOW_AUTO;
            break;
        case SEL_AFWINDOW_MANUALPOSITION:
            pGetafinfo->afalg_in.Window_sel = AFALG_WINDOW_MANUIAL;
            break;
        case SEL_AFWINDOW_CENTER:
            pGetafinfo->afalg_in.Window_sel = AFALG_WINDOW_CENTER;
            break;
        default:
            DBG_ERR("ERROR!! Exception AFWindow!\r\n");
            break;
    }


    CurIdx = Lens_Focus_GetPosition();
    if(AFdrv_GetFocusDist()>AF_CenterDistance)
    {
        if(pGetafinfo->afalg_in.iStartIdx < (CurIdx - (INT32)pGetafinfo->afalg_in.iStep*2))
            StartIdx = (CurIdx - pGetafinfo->afalg_in.iStep*2);
        else
            StartIdx = pGetafinfo->afalg_in.iStartIdx;
    }
    else
    {
        if(pGetafinfo->afalg_in.iEndIdx > (CurIdx + (INT32)pGetafinfo->afalg_in.iStep*2))
            StartIdx = (CurIdx + pGetafinfo->afalg_in.iStep*2);
        else
            StartIdx = pGetafinfo->afalg_in.iEndIdx;
    }

    //Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, StartIdx);
    Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN, StartIdx);
    IPC_WaitVD(id, TRUE); // wait until VA stable.
}

void AF_Init(UINT32 id, UINT32 CurrentStatus)
{
    UINT32 uiStatus = 0;
    //#NT#2016/05/25#Harry_Tsai -begin
    UINT32 uiG1h = 0, uiG1v = 0, uiG2h = 0, uiG2v = 0;
    //#NT#2016/05/25#Harry_Tsai -end
    UINT32 tempVA = 0;

#if AF_PERF_MEASURE_EN
        Perf_Open();
        g_uiAFPerfTime = Perf_GetCurrent();
#endif
    if(gAfCbFP)
        gAfCbFP(AF_CBMSG_AF_START, (void *)&uiStatus);

    AF_SetAlgSearchMode(AF_ALG_SMODE_FALLNORMAL);

    AFALG_SetParam(id);

    AF_ResetAFGuessInfo();

    /* Setup SIE VA. */
    AF_VA_Setup(id);

    /* Reset AF info. Do it before AF started. */
    AF_ResetAFInfo();

    AF_GetVA_Result(&uiG1h, &uiG1v, &uiG2h, &uiG2v, id, VA_WIN_INDP0);
    tempVA = uiG2h + uiG2v;

    if((tempVA <2000) || (IPL_AlgGetUIInfo(id, IPL_SEL_AFWINDOW) == SEL_AFWINDOW_AUTO) )
        AF_SearchingAlg = AF_Alg1;
    else
        AF_SearchingAlg = AF_Alg2;

    if(AF_Alg2 == AF_SearchingAlg)
        AF_Init_Alg2(id, 0);
    else
        AF_Init_Alg1(id, 0);


#if AF_PERF_MEASURE_EN
    //AFT3 = Perf_GetCurrent();
    DBG_ERR("AF init time = %d\r\n", (Perf_GetCurrent()-g_uiAFPerfTime)/1000);
#endif
}


static void AF_SingleWinProcess_Alg1(UINT32 id)
{
    BOOL bAFDone = FALSE;
    INT16 idx;
    //INT32 idx;
    INT32 i32Step = 0;
    UINT32 tempVA = 0;
    UINT32 uiStatus;
    //#NT#2016/05/25#Harry_Tsai -begin
    UINT32 uiG1h = 0, uiG1v = 0, uiG2h = 0, uiG2v = 0;
    //#NT#2016/05/25#Harry_Tsai -end
    //Get AF information
    PAFALG_INFO pGetafinfo = AFALG_GetINFO();


    //If AF status not AFALG_RUN, break
    if(AFALG_CheckStatus() == AFALG_QUIT) // Quit AF by UI.
    {
        bAFDone = TRUE;
        DBG_MSG("Quit!\r\n");
    }
    else if(AFALG_CheckStatus() != AFALG_RUN)
        return;

    AF_GetVA_Result(&uiG1h, &uiG1v, &uiG2h, &uiG2v, id, VA_WIN_INDP0);

    tempVA = uiG2h + uiG2v;

    idx = Lens_Focus_GetPosition(); // Get currnet focus position.
    //DBG_ERR("s=%d, tVA=%d, mVA=%d\r\n",  idx, tempVA, g_AFInfo.MaxVA);

    if(g_AFInfoIdx < (AF_FOCUS_ARRAY_SIZE-1))
    {
        g_AFInfo.Step[g_AFInfoIdx] = idx;
        g_AFInfo.Value[g_AFInfoIdx] = tempVA;
        g_AFInfoIdx++;
    }

    if((tempVA > g_AFInfo.MaxVA) && (g_AFInfoIdx>=3))
    {
        g_AFMaxIdx = idx;
        g_AFInfo.MaxVA = tempVA;
        g_AFInfo.MaxIdx = g_AFInfoIdx - 3;

        g_AFGuessInfo.bEnable = FALSE;
        //DBG_MSG("Found MaxVA!\r\n");
    }
    else
    {
        if(g_AFGuessInfo.bEnable)
        {
            g_AFGuessInfo.Value[1] = ((g_AFGuessInfo.Value[1] + g_AFInfo.Value[g_AFInfoIdx - 1]) >> 1);
            if (g_AFGuessInfo.Value[1] < (g_AFGuessInfo.Value[0] * AF_VA_CURVE_DROP_THRESHOLD))
            {
                bAFDone = TRUE;
                g_uiGetVAMoreTime = 0;
                DBG_MSG("^RVA value went down below threshold. Cur_2VA:%d, Max_2VA:%d\r\n", g_AFGuessInfo.Value[1], g_AFGuessInfo.Value[0]);
            }
            else
            {
                DBG_MSG("^GFake peak! VA value interpolation. Cur_2VA:%d, Max_2VA:%d\r\n", g_AFGuessInfo.Value[1], g_AFGuessInfo.Value[0]);
            }

            g_AFGuessInfo.bEnable = FALSE;
        }
        else if (tempVA < (g_AFInfo.MaxVA * AF_VA_CURVE_DROP_THRESHOLD) && tempVA > 1000) //0.6 is threshold.
        {
            if(!g_AFGuessInfo.bEnable && g_AFInfoIdx >= 2)
            {
                g_AFGuessInfo.Value[0] = ((g_AFInfo.Value[g_AFInfo.MaxIdx] + g_AFInfo.Value[g_AFInfo.MaxIdx + 1]) >> 1);
                g_AFGuessInfo.Value[1] = g_AFInfo.Value[g_AFInfoIdx - 1];
                g_AFGuessInfo.bEnable = TRUE;
                //DBG_MSG("^GFound peak!\r\n");
            }
        }
    }

    if( pGetafinfo->afalg_in.iCurIdx <= pGetafinfo->afalg_in.iEndIdx && !bAFDone)
    {
    #if 0
        IPC_WaitVD(FALSE,id);
    #else // clear VD before wait.
        IPC_WaitVD(id,TRUE);
    #endif

        pGetafinfo->afalg_in.iCurIdx += pGetafinfo->afalg_in.iStep;
        /* Move focus to next position. */
    #if AF_PRE_EXCITE_ALWAYS
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, pGetafinfo->afalg_in.iCurIdx);
    #else
        if(g_AFInfoIdx <= 1)
            Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN, pGetafinfo->afalg_in.iCurIdx);
        else
            Lens_Focus_DoAction(FOCUS_RUN, pGetafinfo->afalg_in.iCurIdx);
    #endif
    }
    else
    {
        if(pGetafinfo->afalg_in.iCurIdx > pGetafinfo->afalg_in.iEndIdx && g_uiGetVAMoreTime--)
        {
            //DBG_MSG("*Get boundary VA value\r\n");
            return;
        }
        /* VA result should be shifted to match actual focus position */
        for(idx = pGetafinfo->afalg_in.iOffsetStep; idx <= (INT32)g_AFInfoIdx-1; idx++)
        {
            if(g_AFInfo.MaxVA == g_AFInfo.Value[idx])
            {
                i32Step = idx - pGetafinfo->afalg_in.iOffsetStep;
                g_AFMaxIdx = g_AFInfo.Step[i32Step];
                DBG_MSG("OriStep:%d, ShiftStep:%d\r\n", g_AFInfo.Step[idx], g_AFMaxIdx);
                break;
            }
        }

        if((INT32)g_AFMaxIdx < (pGetafinfo->afalg_in.iStartIdx + (INT32)pGetafinfo->afalg_in.iStep))
        {
            pGetafinfo->afalg_out.uiStatus |= AFALG_STS_TOOFAR;
            pGetafinfo->afalg_out.iPeak = g_AFMaxIdx;
            DBG_ERR("^RToo far!\r\n");
        }
        else if(g_AFMaxIdx <= pGetafinfo->afalg_in.iEndIdx - pGetafinfo->afalg_in.iStep )
        {

        #if (AF_ALG_FIND_PEAK_INTERPOLATION == DISABLE)
            pGetafinfo->afalg_out.iPeak = g_AFMaxIdx;
        #else
            //i32Step = ( g_AFMaxIdx - pGetafinfo->afalg_in.iStartIdx ) / pGetafinfo->afalg_in.iStep;
            if(i32Step < AF_FOCUS_ARRAY_SIZE && i32Step > 0)
            {
                pGetafinfo->afalg_out.iPeak = AFALG_FindPeak( g_AFInfo.Value[i32Step + pGetafinfo->afalg_in.iOffsetStep - 1],
                                               g_AFInfo.MaxVA,//g_AFInfo.Value[i32Step],
                                               g_AFInfo.Value[i32Step + pGetafinfo->afalg_in.iOffsetStep + 1],
                                               g_AFMaxIdx,//g_AFInfo.Step[i32Step],
                                               pGetafinfo->afalg_in.iStep );
            }
            else
            {
                DBG_ERR("^RCan't do AF prediction:%d\r\n", i32Step);
                pGetafinfo->afalg_out.iPeak = g_AFMaxIdx;
            }
        #endif
        }
        else// if(piIdxMax[uiWinSelect] >= iEndIdx)
        {
            pGetafinfo->afalg_out.uiStatus |= AFALG_STS_TOONEAR;
            pGetafinfo->afalg_out.iPeak = g_AFMaxIdx;
            DBG_ERR("^RToo Near!\r\n");
        }

        //DBG_ERR("^GPeak position: Orig:%d, Predict:%d, Sts:%08lx\r\n", g_AFMaxIdx, pGetafinfo->afalg_out.iPeak, pGetafinfo->afalg_out.uiStatus);

        /* Move focus to peak position. */
        if(AFALG_CheckStatus() == AFALG_QUIT)
            pGetafinfo->afalg_out.iPeak = g_AFMaxIdx;

    #if AF_PRE_EXCITE_ALWAYS
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, pGetafinfo->afalg_out.iPeak);
    #else
        if(pGetafinfo->afalg_in.iCurIdx > pGetafinfo->afalg_in.iStartIdx || (g_AFInfoIdx >= 1)) // Has been pre-excited.
            Lens_Focus_DoAction(FOCUS_RUN|FOCUS_POSTEXC, pGetafinfo->afalg_out.iPeak);
    #endif

    #if 0
        AF_Release(IPL_UTI_CONV2_AF_ID(id), FALSE);
    #else
        AF_Pause(IPL_UTI_CONV2_AF_ID(id), FALSE);

        if(gAfCbFP)
        {
        #if AF_PERF_MEASURE_EN
            g_uiAFPerfTime = Perf_GetCurrent() - g_uiAFPerfTime;
            Perf_Close();
            DBG_ERR("AF time = %dms\r\n", g_uiAFPerfTime/1000);
        #endif
            uiStatus = AFALG_GetAFStatus();

            gAfCbFP(AF_CBMSG_AF_DONE, (void *)&uiStatus);
        }

        AFALG_SetStatus(AFALG_END);
    #endif
    }
}


static void AF_SingleWinProcess_Alg2(UINT32 id)
{
    BOOL bAFDone = FALSE, ValidCurve = FALSE;
    INT16 idx;
    UINT32 tempVA = 0;
    UINT32 uiStatus;
    //#NT#2016/05/25#Harry_Tsai -begin
    UINT32 uiG1h = 0, uiG1v = 0, uiG2h = 0, uiG2v = 0;
    //#NT#2016/05/25#Harry_Tsai -end
    //Get AF information
    PAFALG_INFO pGetafinfo = AFALG_GetINFO();
    static UINT8 AF_Status = 0, Direction = MOTOR_FOCUS_FWD, Damp_CNT = 0;
    static BOOL Inver_Dir = FALSE, AF_Falling= FALSE;
    UINT32 VAFirst=0,VASecond=0, VALast=0;
    INT32 NextIdx, BestIdx;
    static UINT32 PeakIdx, MinVA=0xffff;

    //If AF status not AFALG_RUN, break
    if(AFALG_CheckStatus() == AFALG_QUIT) // Quit AF by UI.
    {
        bAFDone = TRUE;
        DBG_MSG("Quit!\r\n");
    }
    else if(AFALG_CheckStatus() != AFALG_RUN)
        return;

    AF_GetVA_Result(&uiG1h, &uiG1v, &uiG2h, &uiG2v, id, VA_WIN_INDP0);

    tempVA = uiG2h + uiG2v;

    idx = Lens_Focus_GetPosition(); // Get currnet focus position.
    if((tempVA > g_AFInfo.MaxVA)&&(g_AFInfoIdx>=2))
    {
        g_AFInfo.MaxVA = tempVA;
        g_AFInfo.MaxIdx = g_AFInfoIdx;
    }

    if((tempVA < MinVA) &&(g_AFInfoIdx>=2))
        MinVA = tempVA;

    if((g_AFInfo.MaxVA*100) > (MinVA*102))
        ValidCurve = TRUE;

    //DBG_ERR("s=%d, tVA=%d, mVA=%d, Time=%d\r\n",  idx, tempVA, g_AFInfo.MaxVA, (AFT1-g_uiAFPerfTime)/1000);

    if(g_AFInfoIdx < (AF_FOCUS_ARRAY_SIZE-1))
    {
        g_AFInfo.Step[g_AFInfoIdx] = idx;
        g_AFInfo.Value[g_AFInfoIdx] = tempVA;
    }

    //DBG_ERR("Idx=%d, status=%d, Dir=%d, InvD=%d, VA=%d\r\n",g_AFInfoIdx,AF_Status,Direction,Inver_Dir,tempVA);

    switch(AF_Status)
    {
        //---------------------------------------------------------
        // Step0: Get VA and Guess direction
        //---------------------------------------------------------
        case 0:

            if(!Inver_Dir)
            {
                if(AFdrv_GetFocusDist()>AF_CenterDistance)
                    Direction = MOTOR_FOCUS_FWD;
                else
                    Direction = MOTOR_FOCUS_BWD;
                DBG_MSG("af %d\r\n",AFdrv_GetFocusDist());
            }
            else
            {
                Inver_Dir = FALSE;
            }
            NextIdx = Af_GetNextFocusPos(Direction, pGetafinfo->afalg_in.iStep, pGetafinfo->afalg_in.iStartIdx, pGetafinfo->afalg_in.iEndIdx);

            //Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN, NextIdx);
            Lens_Focus_DoAction(FOCUS_RUN, NextIdx);

            AF_Status ++;
            break;
        case 1:
            //---------------------------------------------------------
            // Step1: Continue Go step and record VA
            //---------------------------------------------------------

            NextIdx = Af_GetNextFocusPos(Direction, pGetafinfo->afalg_in.iStep, pGetafinfo->afalg_in.iStartIdx, pGetafinfo->afalg_in.iEndIdx);

            Lens_Focus_DoAction(FOCUS_RUN,NextIdx);

            AF_Status++;
            break;
        case 2:
            //---------------------------------------------------------
            // Step2: Continue Go step and record VA
            //---------------------------------------------------------
            NextIdx = Af_GetNextFocusPos(Direction, pGetafinfo->afalg_in.iStep, pGetafinfo->afalg_in.iStartIdx, pGetafinfo->afalg_in.iEndIdx);

            Lens_Focus_DoAction(FOCUS_RUN,NextIdx);

            AF_Status ++;
            break;
        case 3:
            //---------------------------------------------------------
            // Step3: Continue Go step and record VA
            //---------------------------------------------------------
            NextIdx = Af_GetNextFocusPos(Direction, pGetafinfo->afalg_in.iStep, pGetafinfo->afalg_in.iStartIdx, pGetafinfo->afalg_in.iEndIdx);

            Lens_Focus_DoAction(FOCUS_RUN,NextIdx);

            AF_Status ++;
            break;

        case 4:
            //---------------------------------------------------------
            // Step4: record VA and judge direction
            //---------------------------------------------------------
            //Record Last VA
            if(g_AFInfoIdx >= 4)
            {
                //VAFirst = (g_AFInfo.Value[g_AFInfoIdx-2]);
                //VASecond = (g_AFInfo.Value[g_AFInfoIdx-2]+g_AFInfo.Value[g_AFInfoIdx-1])/2;
                //VALast = (g_AFInfo.Value[g_AFInfoIdx-1]+g_AFInfo.Value[g_AFInfoIdx])/2;
                VAFirst = g_AFInfo.Value[g_AFInfoIdx-2];
                VASecond = g_AFInfo.Value[g_AFInfoIdx-1];
                VALast = g_AFInfo.Value[g_AFInfoIdx];
            }
            //VA curve is falling, direction is wrong, invert and back to 0!.
            if( (VALast < VASecond) && (VASecond < VAFirst) )
            {
                if(AF_Falling == FALSE)
                {
                    Direction = (Direction+1)%2;
                    //NextIdx = caf_GetNextFocusPos(CafAlg_Param.RT_AF_Direction,(CafAlg_Param.RT_AF_Step*3));
                    NextIdx = Af_GetNextFocusPos(Direction, pGetafinfo->afalg_in.iStep*2, pGetafinfo->afalg_in.iStartIdx, pGetafinfo->afalg_in.iEndIdx);
                    Lens_Focus_DoAction(FOCUS_RUN,NextIdx);
                    Inver_Dir = TRUE;
                    AF_Status = 0;
                    VaildStartIdx = g_AFInfoIdx;
                    AF_Falling = TRUE;
                }
                else
                {
                    AF_Falling = FALSE;
                }
            }
            //VA curve is rising, direction is right, keep going and go to 4!
            else if( (VALast >= VASecond) && (VASecond >= VAFirst) )
            {
                NextIdx = Af_GetNextFocusPos(Direction, pGetafinfo->afalg_in.iStep, pGetafinfo->afalg_in.iStartIdx, pGetafinfo->afalg_in.iEndIdx);
                Lens_Focus_DoAction(FOCUS_RUN,NextIdx);
                AF_Status ++;
            }
            else if( (VALast < VASecond) && (VASecond > VAFirst) )
            {
                if(ValidCurve == TRUE)
                {
                    PeakIdx = Af_GetMaxIndex(VaildStartIdx,g_AFInfoIdx);
                    AF_Status = AF_Status_OK;
                    goto  FOCUS_OK;
                }
                else
                {
                    Damp_CNT ++;
                    if (Damp_CNT > Damping_Max)
                    {
                        AF_Status = AF_Status_NG; //switch to normal search
                        Damp_CNT = 0;
                        goto  FOCUS_NG;
                    }
                }
            }
            //VA curve is damping, keep going to check again
            else
            {
                Damp_CNT ++;
                //More than damping (TH) times, check max VA index.
                if (Damp_CNT > Damping_Max)
                {
                    //PeakIdx = Af_GetMaxIndex(VaildStartIdx,g_AFInfoIdx);
                    AF_Status = AF_Status_NG; //switch to normal search
                    Damp_CNT = 0;
                    goto  FOCUS_NG;
                }
                else
                {
                    NextIdx = Af_GetNextFocusPos(Direction, pGetafinfo->afalg_in.iStep, pGetafinfo->afalg_in.iStartIdx, pGetafinfo->afalg_in.iEndIdx);
                    Lens_Focus_DoAction(FOCUS_RUN,NextIdx);
                }
            }
            break;
        case 5:
            //---------------------------------------------------------
            // Step5: VA is rising, keep going until falling.
            //---------------------------------------------------------
            if(g_AFInfoIdx >= 4)
            {
                //VAFirst = (g_AFInfo.Value[g_AFInfoIdx-3]+g_AFInfo.Value[g_AFInfoIdx-2])/2;
                //VASecond = (g_AFInfo.Value[g_AFInfoIdx-2]+g_AFInfo.Value[g_AFInfoIdx-1])/2;
                //VALast = (g_AFInfo.Value[g_AFInfoIdx-1]+g_AFInfo.Value[g_AFInfoIdx])/2;
                VAFirst = g_AFInfo.Value[g_AFInfoIdx-2];
                VASecond = g_AFInfo.Value[g_AFInfoIdx-1];
                VALast = g_AFInfo.Value[g_AFInfoIdx];
            }

            //VA curve is falling, Find Best index!.
            if( (VALast < VASecond) && (VASecond < VAFirst) )
            {
                if(ValidCurve == TRUE)
                {
                    PeakIdx = Af_GetMaxIndex(VaildStartIdx,g_AFInfoIdx);
                    AF_Status = AF_Status_OK;
                    goto  FOCUS_OK;
                }
                else
                {
                    Damp_CNT ++;
                    if (Damp_CNT > Damping_Max)
                    {
                        AF_Status = AF_Status_NG; //switch to normal search
                        Damp_CNT = 0;
                        goto  FOCUS_NG;
                    }
                }
            }
            //VA curve is rising, keep going.
            else if( (VALast >= VASecond) && (VASecond >= VAFirst) )
            {
                NextIdx = Af_GetNextFocusPos(Direction, pGetafinfo->afalg_in.iStep, pGetafinfo->afalg_in.iStartIdx, pGetafinfo->afalg_in.iEndIdx);
                if(NextIdx <= pGetafinfo->afalg_in.iStartIdx-3*(INT32)pGetafinfo->afalg_in.iStep)
                {
                    PeakIdx = g_AFInfoIdx;
                    pGetafinfo->afalg_out.uiStatus |= AFALG_STS_TOOFAR;
                    AF_Status = AF_Status_OK;
                    DBG_ERR("AF: Focus out of boundary %d (S=%d E=%d)!!!!\r\n",NextIdx,pGetafinfo->afalg_in.iStartIdx,pGetafinfo->afalg_in.iEndIdx);
                    DBG_ERR("^RToo far!\r\n");
                    goto  FOCUS_OK;
                }
                else if(NextIdx >= pGetafinfo->afalg_in.iEndIdx+3*(INT32)pGetafinfo->afalg_in.iStep)
                {
                    PeakIdx = g_AFInfoIdx;
                    AF_Status = AF_Status_OK;
                    pGetafinfo->afalg_out.uiStatus |= AFALG_STS_TOONEAR;
                    DBG_ERR("AF: Focus out of boundary %d (S=%d E=%d)!!!!\r\n",NextIdx,pGetafinfo->afalg_in.iStartIdx,pGetafinfo->afalg_in.iEndIdx);
                    DBG_ERR("^RToo near!\r\n");
                    goto  FOCUS_OK;
                }
                Lens_Focus_DoAction(FOCUS_RUN,NextIdx);
            }
            //VA curve is damping, keep going to check again
            else
            {
                Damp_CNT ++;
                //More than damping (TH) times, check max VA index.
                if (Damp_CNT > Damping_Max)
                {
                    //PeakIdx = Af_GetMaxIndex(VaildStartIdx,g_AFInfoIdx);
                    AF_Status = AF_Status_NG; //switch to normal search
                    Damp_CNT = 0;
                    goto  FOCUS_NG;
                }
                else
                {
                    NextIdx = Af_GetNextFocusPos(Direction, pGetafinfo->afalg_in.iStep, pGetafinfo->afalg_in.iStartIdx, pGetafinfo->afalg_in.iEndIdx);
                    if(NextIdx <= pGetafinfo->afalg_in.iStartIdx)
                    {
                        PeakIdx = g_AFInfoIdx;
                        //pGetafinfo->afalg_out.uiStatus |= AFALG_STS_TOOFAR;
                        AF_Status = AF_Status_OK;
                        DBG_ERR("AF: Focus out of boundary %d (S=%d E=%d)!!!!\r\n",NextIdx,pGetafinfo->afalg_in.iStartIdx,pGetafinfo->afalg_in.iEndIdx);
                        DBG_ERR("^RToo far!\r\n");
                        goto  FOCUS_OK;
                    }
                    else if(NextIdx >= pGetafinfo->afalg_in.iEndIdx)
                    {
                        PeakIdx = g_AFInfoIdx;
                        AF_Status = AF_Status_OK;
                        //pGetafinfo->afalg_out.uiStatus |= AFALG_STS_TOONEAR;
                        DBG_ERR("AF: Focus out of boundary %d (S=%d E=%d)!!!!\r\n",NextIdx,pGetafinfo->afalg_in.iStartIdx,pGetafinfo->afalg_in.iEndIdx);
                        DBG_ERR("^RToo near!\r\n");
                        goto  FOCUS_OK;
                    }
                    Lens_Focus_DoAction(FOCUS_RUN,NextIdx);
                }
            }
            break;
        case AF_Status_OK:// Alg2 finish
FOCUS_OK:
            if(gAfCbFP)
            {
                uiStatus = AFALG_GetAFStatus();
                gAfCbFP(AF_CBMSG_AF_DONE, (void *)&uiStatus);
            }
            if(PeakIdx>= (AF_FOCUS_ARRAY_SIZE-2))
                PeakIdx = AF_FOCUS_ARRAY_SIZE-2;

            if(Direction == MOTOR_FOCUS_FWD)
                BestIdx = AFALG_FindPeak(g_AFInfo.Value[PeakIdx-1], g_AFInfo.Value[PeakIdx], g_AFInfo.Value[PeakIdx+1], g_AFInfo.Step[PeakIdx-2], pGetafinfo->afalg_in.iStep);
            else
                BestIdx = AFALG_FindPeak(g_AFInfo.Value[PeakIdx+1], g_AFInfo.Value[PeakIdx], g_AFInfo.Value[PeakIdx-1], g_AFInfo.Step[PeakIdx-2], pGetafinfo->afalg_in.iStep);
            Lens_Focus_DoAction(FOCUS_RUN|FOCUS_POSTEXC,BestIdx);

            //DBG_ERR("Peak Idx=%d , Guess best position = %d!!!!\r\n",PeakIdx, BestIdx);
            Damp_CNT = 0;
            MinVA=0xffff;
#if AF_PERF_MEASURE_EN
                g_uiAFPerfTime = Perf_GetCurrent() - g_uiAFPerfTime;
                Perf_Close();
                DBG_ERR("AF time = %dms\r\n", g_uiAFPerfTime/1000);
#endif
            AF_Status = 0;
            AF_Pause(IPL_UTI_CONV2_AF_ID(id), FALSE);
            AFALG_SetStatus(AFALG_END);
            break;
        case AF_Status_NG:// switch to Alg1
FOCUS_NG:
            AF_Status = 0;
            MinVA=0xffff;
            AF_ResetAFInfo();
            AF_SearchingAlg = AF_Alg1;
            AF_Init_Alg1(id, 1);
            break;
    }

    //AFT3 = Perf_GetCurrent();
    //DBG_ERR("AF_TSK time = %d %d \r\n", AFT1/1000, AFT3/1000);
    g_AFInfoIdx++;
}

static void AF_SingleWinProcess(UINT32 id)
{
    if(AF_Alg2 == AF_SearchingAlg)
        AF_SingleWinProcess_Alg2(id);
    else
        AF_SingleWinProcess_Alg1(id);
}

static void AFALG_MultiWinProcess(UINT32 id)
{
    PAFALG_INFO_IN pInData = &AFALG_GetINFO()->afalg_in;
    PAFALG_INFO_OUT pOutData = &AFALG_GetINFO()->afalg_out;
    PAFALG_PARAM pAfAlgParam = AFALG_GetAFALGPARAM();
    static UINT32  winsum = 0,win, Idx = 0, curve_cnt=0,curve_num = 0,Stop_cnt;
    static UINT32 puiVACurr[AF_TOTAL_WIN]={0};
    static UINT32 uiVATremble[AF_TOTAL_WIN]={0};
    UINT32 uiVATrembleTmp=0;
    UINT32 uiCntr, uiNext, uiLast;
    UINT32 uiX, uiY, uiStartXY, uiTempXY;
    static INT32  MaxFoucusPos[AF_TOTAL_WIN]={0};
    UINT32 step = 0;
    static UINT32 uiContinusFalling[AF_TOTAL_WIN]={0};
    static UINT32 uiVATremble_Cnt[AF_TOTAL_WIN]={0};
    BOOL   bAFDone = FALSE;
    //#NT#2016/03/18#liwei cheng -begin
    //#NT# for decreasing memory
    AF_GRP HWVA_GRP = {0};
    //#NT#2016/03/18#liwei cheng -end

//    for(FocusPos=pInData->iStartIdx; FocusPos<=pInData->iEndIdx; FocusPos+=pInData->iStep)
//    {

    //If AF status not AFALG_RUN, break
    if(AFALG_CheckStatus() == AFALG_QUIT) // Quit AF by UI.
    {
        bAFDone = TRUE;
        DBG_MSG("Quit!\r\n");
    }
    else if(AFALG_CheckStatus() != AFALG_RUN)
        return;

    if(AFALG_GetInfoIdx() < 1)
    {
        memset(puiVACurr, 0, sizeof(puiVACurr));
        memset(uiVATremble, 0, sizeof(uiVATremble));
        memset(MaxFoucusPos, 0, sizeof(MaxFoucusPos));
        memset(uiContinusFalling, 0, sizeof(uiContinusFalling));
        memset(uiVATremble_Cnt, 0, sizeof(uiVATremble_Cnt));
        Idx = 0;
        Stop_cnt = 0;
        curve_cnt = 0;

        DBG_MSG("Reset alg info.\r\n");
        //AFALG_MSG("IdxOverRun:%d\r\n", pAfAlgParam->uiIdxOverRun);
    }


    // Get 16x16 VA value one time.
    //af_tmp
    //#NT#2016/03/18#liwei cheng -begin
    //#NT#for decreasing memory
    AF_GetVA_Result(&(HWVA_GRP.uiHWVA_G1H[0]), &(HWVA_GRP.uiHWVA_G1V[0]), &(HWVA_GRP.uiHWVA_G2H[0]), &(HWVA_GRP.uiHWVA_G2V[0]), id, VA_WIN_16X16);
    //#NT#2016/03/18#liwei cheng -end
    pAfAlgParam->afParam[Idx].iFocusPos = pInData->iCurIdx;

    for(win = 0;win < AF_TOTAL_WIN;win++)
    {
        //------------------------------------------------------------------
        // Step1: Check this window's score, if no score, skip it.
        //------------------------------------------------------------------
        if(pAfAlgParam->WindowScore[win] == 0)
            continue;

        //------------------------------------------------------------------
        // Step2: Record VA value
        //------------------------------------------------------------------
        #if 0
        puiVACurr[win] = g_fpGetVA(win);
        pAfAlgParam->afParam[Idx].uiALGVA[win] = puiVACurr[win];

        #else // Get va win from HW VA win(16x16).
        uiX = (win % AF_WIN_DIMENSION_W);
        uiY = (win / AF_WIN_DIMENSION_H);
        uiStartXY = (uiY * (AF_WIN_HW_H / AF_WIN_DIMENSION_H) * AF_WIN_HW_H) + (uiX * (AF_WIN_HW_W / AF_WIN_DIMENSION_W));
        uiY = (AF_WIN_HW_H / AF_WIN_DIMENSION_H);
        while(uiY--)
        {
            uiTempXY = uiStartXY;
            uiX = (AF_WIN_HW_W / AF_WIN_DIMENSION_W);
            while(uiX--)
            {
                //DBG_MSG("uiTempXY:%d, uiX:%d, uiY:%d\r\n", uiTempXY, uiX, uiY);
                if(uiStartXY < AF_VA_WINSIZE)
                {
                    //#NT#2016/03/18#liwei cheng -begin

                    //#NT#for decreasing memory
                    pAfAlgParam->afParam[Idx].uiALGVA[win] += (HWVA_GRP.uiHWVA_G1H[uiTempXY] + HWVA_GRP.uiHWVA_G1V[uiTempXY]);
                    //#NT#2016/03/18#liwei cheng -end
                    uiTempXY = uiTempXY + 1;
                }
                else
                {
                    DBG_ERR("^RVA win index out of bound!\r\n");
                    pOutData->uiStatus |= AFALG_STS_NOTARGET;
                    return;
                }
            }
            uiStartXY += AF_WIN_HW_H;
        }

        puiVACurr[win] = pAfAlgParam->afParam[Idx].uiALGVA[win];
        #endif

        #if 0
        if(AE_GetLV()<5 && puiVACurr[4] < g_uiNOLightVA)
        {
            Stop_cnt++;
        }
        #endif
        //------------------------------------------------------------------
        // Step3: Record VAMax/ VAMin/ VALast
        //------------------------------------------------------------------
        if(puiVACurr[win] < pAfAlgParam->VAMin[win])
        {
            pAfAlgParam->VAMin[win] = puiVACurr[win];
            //IdxMin[win] = iFocusPos;
            //puiVAMaxBeforeMinBeforeMax[win] = AF_ALG_PARAM.VAMax[win];

        }

        if(puiVACurr[win] > pAfAlgParam->VAMax[win])
        {
            pAfAlgParam->VAMax[win] = puiVACurr[win];
            MaxFoucusPos[win] = pInData->iCurIdx;//FocusPos;
            uiVATremble_Cnt[win] = 0;
            //puiVAMinBeforeMax[win] = puiVAMin[win];
        }

        pAfAlgParam->VALast[win] = puiVACurr[win];

        //---------------------------------------------------------
        // Step4: Record Tremble and continues falling
        //---------------------------------------------------------
        if(Idx>=2)
        {
            uiLast = pAfAlgParam->afParam[Idx-2].uiALGVA[win];
            uiCntr = pAfAlgParam->afParam[Idx-1].uiALGVA[win];
            uiNext = pAfAlgParam->afParam[Idx].uiALGVA[win];

            if(uiLast>uiCntr && uiCntr<uiNext)
            {
                uiVATrembleTmp = (uiLast + uiNext)/2 - uiCntr;
                uiVATremble_Cnt[win]++;
                if(uiVATrembleTmp > uiVATremble[win])
                    uiVATremble[win] = uiVATrembleTmp;
            }

            if(uiCntr > uiNext)
                uiContinusFalling[win] ++;
            else
            {
                if(uiVATremble_Cnt[win] > pAfAlgParam->uiIdxOverRun)
                    uiContinusFalling[win] = pAfAlgParam->uiIdxOverRun;
                else
                    uiContinusFalling[win] = 0;
            }
        }

        #if 0
        if( (uiVATremble[win]<1000) && (pAfAlgParam->VAMax[win] > 50000) && (AFdrv_GetAFModel() == Model_4) )
        {
            pAfAlgParam->uiIdxOverRun = 3;
        }
        #endif

        //---------------------------------------------------------
        // Step5: Judge if AF end or not
        //---------------------------------------------------------
        //if(win == AF_DEFAULT_WIN) // debug purpose.
        if(( (UINT32)AFALG_ABS(pInData->iCurIdx - MaxFoucusPos[win])      >=  (pAfAlgParam->uiIdxOverRun*pInData->iStep)                                              )/*overRun-check*/
        &&( (pAfAlgParam->VAMax[win]-puiVACurr[win])*100  >   pAfAlgParam->afalg_threshold.uivibrate_th*pAfAlgParam->VAMax[win]                               )/*falling-check*/
        //&&( (pAfAlgParam->VAMax[win]-puiVACurr[win])*100  >   (pAfAlgParam->afalg_threshold.uivibrate_th*(2+((pInData->bObj)?3:0))/2)*pAfAlgParam->VAMax[win] )/*FD falling-check*/
        &&( (pAfAlgParam->VAMax[win]-puiVACurr[win])*10   >   (uiVATremble[win]*pAfAlgParam->afalg_threshold.uitremble_th)                                                                           )/*tremble-check*/
        &&(  pAfAlgParam->VAMax[win]                      >    pAfAlgParam->afalg_threshold.uimax_th                                                          )/*magnitude-check*/
        &&(  Idx                                          >=   pAfAlgParam->afalg_threshold.uirun_th                                                          )/*At least run this far*/
        &&(  uiContinusFalling[win]                       >=    pAfAlgParam->uiIdxOverRun                                                                      )/*At least falling this far continuously*/
        &&(  MaxFoucusPos[win]                            !=    pInData->iStartIdx                                                                            )/*First one not the max */
          ){
            if(pAfAlgParam->WindowScore[win] != 0)
            {
                pAfAlgParam->Curve[curve_cnt][curve_num] = win;
                pAfAlgParam->Curve[curve_cnt][++curve_num] = 0xff; // set the end first
                // Central win first.
                if(!g_AFAlgCentralWinFound)
                {
                    // Central win must be modified if dimension was changed.
                    if(win == 27 || win == 28 || win == 35 || win == 36)
                    {
                        g_AFAlgCentralWinFound = TRUE;
                        g_AFAlgMaxIdx = MaxFoucusPos[win];
                    }
                    else if(MaxFoucusPos[win] > g_AFAlgMaxIdx)
                        g_AFAlgMaxIdx = MaxFoucusPos[win];

                }

            }
            pAfAlgParam->WindowScore[win] = 0;

            //AFALG_MSG("^GBingo!- Win:%d, MaxPos:%d, curve_cnt:%d, curve_num:%d, VaMax:%d\r\n", win, MaxFoucusPos[win], curve_cnt, curve_num, pAfAlgParam->VAMax[win]);
        }
        //AFALG_MSG("Idx=%d, Win=%d, AF=%d, VA=%d, Tremble=%d, winsum=%d, algWin=%d, confall=%d\r\n",Idx,win,pAfAlgParam->afParam[Idx].iFocusPos,pAfAlgParam->VALast[win],uiVATremble[win],winsum,pAfAlgParam->WindowScore[win],uiContinusFalling[win]);
        //AFALG_MSG("Idx=%d, Win=%d, AF=%d, VA=%d, winsum=%d, algWin=%d\r\n",Idx,win,pAfAlgParam->afParam[Idx].iFocusPos,pAfAlgParam->VALast[win],winsum,pAfAlgParam->WindowScore[win]);

        winsum += pAfAlgParam->WindowScore[win];
    }

    DBG_MSG("FPos:%d, VACur[%d] = %d\r\n", pInData->iCurIdx, 35, pAfAlgParam->VAMax[35]);

    //AFALG_MSG("--------------------------------  %d   %d\r\n",curve_cnt,curve_num);
    if(curve_num != 0)
        curve_cnt++;

    if(winsum == 0 || Stop_cnt>2) // the focus curve all of windows are falling down
    {
        //AFALG_SetStatus(AFALG_END);
        //g_fpStopLens(LENS_AFSTOP);//Set Lens stop
        if(Stop_cnt>2)
            pOutData->uiStatus |= AFALG_STS_NOTARGET;

        bAFDone = TRUE;
    }

    Idx++;
    if(Idx >= AF_TOTAL_STEP)
    {
        DBG_ERR("ERROR!! AF step more than 256!!!\r\n");
        bAFDone = TRUE;
    }
//    }

    if(pInData->iCurIdx <= (pInData->iEndIdx + (pInData->iOffsetStep * (INT32)pInData->iStep)) && !bAFDone)
    {
        IPC_WaitVD(id,TRUE);

        /* Move focus to next position. */
        pInData->iCurIdx += pInData->iStep;

        #if AF_PRE_EXCITE_ALWAYS
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, pInData->iCurIdx);
        #else
        if(AFALG_GetInfoIdx() < 1)
        {
            Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN, pInData->iCurIdx);
            AFALG_SetInfoIdx(AFALG_GetInfoIdx() + 1);
        }
        else
            Lens_Focus_DoAction(FOCUS_RUN, pInData->iCurIdx);
        #endif

        winsum = 0;
        curve_num = 0;
    }
    else
    {
        //---------------------------------------------------------
        // Step6: Select window if choose multi-AF
        //---------------------------------------------------------
        if(curve_cnt > 0)
        {
            for(win = (curve_cnt-1);; win--)
            {//Search back = Find Near-focus first
                for(step = 0;;step++)
                {
                    if(pAfAlgParam->Curve[win][step] == 0xff)
                        break;

                    #if 0
                    if(pAfAlgParam->WindowSelect == AF_NOSELECT_WIN)
                    #else // Find the Nearest-focus.
                    if(g_AFAlgMaxIdx == MaxFoucusPos[pAfAlgParam->Curve[win][step]])
                    #endif
                    {
                        pAfAlgParam->WindowSelect = pAfAlgParam->Curve[win][step];
                        pOutData->uiSelectWin[pAfAlgParam->Curve[win][step]] = TRUE;
                        //AFALG_MSG("^GSelWin:%d\r\n", pAfAlgParam->WindowSelect);
                    }
                }
                if(win == 0)
                    break;

                #if 0
                if(pAfAlgParam->WindowSelect == AF_NOSELECT_WIN)
                    continue;
                else
                    break;
                #endif
            }
        }
        if(pAfAlgParam->WindowSelect == AF_NOSELECT_WIN)
        {
            pAfAlgParam->WindowSelect = AF_DEFAULT_WIN;
            pOutData->uiStatus |= AFALG_STS_NOTARGET;
        }
        else
        {
            AFALG_SetStatus(AFALG_END);
        }
        //pOutData->uiSelectWin = pAfAlgParam->WindowSelect;
        //---------------------------------------------------------
        // Step7: Find Peak and record AF status
        //---------------------------------------------------------
        //Fullrange check (special case)
        if(pAfAlgParam->uiIdxOverRun == AF_TOTAL_STEP)
        {
            AFALG_SetStatus(AFALG_END);
        }
        if(MaxFoucusPos[pAfAlgParam->WindowSelect] < (pInData->iStartIdx + (INT32)pInData->iStep))
        {
            pOutData->uiStatus |= AFALG_STS_TOOFAR;
            DBG_ERR("^RToo Far!\r\n");
        }
        else if(( MaxFoucusPos[pAfAlgParam->WindowSelect] <= (pInData->iEndIdx - (INT32)(pInData->iStep*pAfAlgParam->uiIdxOverRun)))\
                || (pAfAlgParam->uiIdxOverRun == AF_TOTAL_STEP))
        {
            step = ( MaxFoucusPos[pAfAlgParam->WindowSelect] - pInData->iStartIdx ) / pInData->iStep;
            DBG_MSG("OriMaxPos:%d\r\n", pAfAlgParam->afParam[step].iFocusPos);
            if((INT32)step >= (pInData->iOffsetStep + 1))
            {
                step -= pInData->iOffsetStep;
                pOutData->iPeak = AFALG_FindPeak( pAfAlgParam->afParam[step-1].uiALGVA[pAfAlgParam->WindowSelect],
                                                   pAfAlgParam->afParam[step].uiALGVA[pAfAlgParam->WindowSelect],
                                                   pAfAlgParam->afParam[step+1].uiALGVA[pAfAlgParam->WindowSelect],
                                                   pAfAlgParam->afParam[step].iFocusPos,
                                                   pInData->iStep );

                DBG_MSG("Shited pos:%d\r\n", pAfAlgParam->afParam[step].iFocusPos);
                DBG_MSG("Interpolated pos:%d\r\n", pOutData->iPeak);
            }
            else
            {
                pOutData->iPeak = pAfAlgParam->afParam[step].iFocusPos;
                DBG_ERR("^RReached boundary\r\n");
            }
        }
        else// if(piIdxMax[uiWinSelect] >= iEndIdx)
        {
            pOutData->uiStatus |= AFALG_STS_TOONEAR;
            DBG_ERR("^RToo Near!\r\n");
        }
        DBG_MSG("AFALG=0x%.8x, Peak=%d,  overrun=%d, algstate=%d\r\n",pOutData->uiStatus,pOutData->iPeak,pAfAlgParam->uiIdxOverRun, AFALG_CheckStatus());
    /*
        if(pOutData->iPeak >= pInData->iEndIdx)
        {
            pOutData->iPeak = pInData->iEndIdx;
            pOutData->uiStatus |= AFALG_STS_TOONEAR;
        }
    */
        if(pAfAlgParam->VAMax[pAfAlgParam->WindowSelect] < pAfAlgParam->afalg_threshold.uimax_th)
        {
            pOutData->uiStatus |= AFALG_STS_LOWVA;
        }
        #if 0
        if((pAfAlgParam->VAMax[pAfAlgParam->WindowSelect]-pAfAlgParam->VALast[pAfAlgParam->WindowSelect])*10
            < (uiVATremble[pAfAlgParam->WindowSelect]*pAfAlgParam->afalg_threshold.uitremble_th))
        {
            pOutData->uiStatus |= AFALG_STS_TREMBLE;
        }
        if((pAfAlgParam->VAMax[pAfAlgParam->WindowSelect]-pAfAlgParam->VALast[pAfAlgParam->WindowSelect])*100
            < (pAfAlgParam->afalg_threshold.uivibrate_th*pAfAlgParam->VAMax[pAfAlgParam->WindowSelect]/4))
        {
            pOutData->uiStatus |= AFALG_STS_SMOOTH;
        }
        #endif
        //if(uiContinusFalling[pAfAlgParam->WindowSelect] < pAfAlgParam->iIdxOverRun)
        //{
        //    if(pAfAlgParam->iIdxOverRun != AF_TOTAL_STEP)//don't consider full range search
        //        pOutData->uiStatus |= AFALG_STS_VASMALL;
        //}
        //--------------------------------------------------------------------------
        // Step9: Peak Offset conpensated
        //--------------------------------------------------------------------------
        if(pOutData->uiStatus == AFALG_STS_SUCCESS && AFALG_CheckStatus() == AFALG_END)
        {
            //pOutData->iPeak += pInData->iOffsetStep;
        }
        else
        {
            if(MaxFoucusPos[pAfAlgParam->WindowSelect])
                pOutData->iPeak = MaxFoucusPos[pAfAlgParam->WindowSelect];
            else
                pOutData->iPeak = pInData->iDefaultIdx;
        }
        //For SN test use
        #if 0
        if(g_bSNTest)
            pOutData->iPeak = pInData->iStartIdx;
        #endif
        //--------------------------------------------------------------------------
        // Step10: Peak
        //--------------------------------------------------------------------------
        #if AF_PRE_EXCITE_ALWAYS
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, pOutData->iPeak);
        #else
        if(AFALG_GetInfoIdx() >= 1) // Has been pre-excited.
            Lens_Focus_DoAction(FOCUS_RUN|FOCUS_POSTEXC, pOutData->iPeak);
        #endif

        AF_Pause(IPL_UTI_CONV2_AF_ID(id), FALSE);

        AFALG_SetStatus(AFALG_END);

        DBG_MSG("AFALG status=0x%.8x, Window=%d, Peak=%d, offset=%d\r\n",pOutData->uiStatus,pAfAlgParam->WindowSelect,pOutData->iPeak,pInData->iOffsetStep);
        //return pOutData->iPeak;
    }
}

void AF_Process(UINT32 id, UINT32 CurrentStatus)
{
    if(IPL_AlgGetUIInfo(id, IPL_SEL_AFWINDOW) == AFALG_WINDOW_CENTER)
    {
        AF_SingleWinProcess(id);
    }
    else if(IPL_AlgGetUIInfo(id, IPL_SEL_AFWINDOW) == SEL_AFWINDOW_AUTO)
    {
        AFALG_MultiWinProcess(id);
        if(AFALG_CheckStatus() == AFALG_END)
        {
            if(gAfCbFP)
            {
                UINT32 uiStatus = AFALG_GetAFStatus();

                gAfCbFP(AF_CBMSG_AF_DONE, (void *)&uiStatus);
            }
        }
    }
    else
    {
        DBG_ERR("^R No AF window specified!\r\n");
    }

    /* Release SIE VA. */
    AF_VA_Release(id);
}

ER AF_Run(AF_PROC_ID Id, BOOL bWaitFinish)
{
    ER rt = E_OK;

    if(AF_GetCtrlObjStatus(Id) == AF_TASK_STATUS_OPEN)
    {
        if (AF_Start(Id, ENABLE) == E_OK)
            DBG_MSG("AF(%d) start ready\r\n", Id);
        else
            DBG_MSG("AF(%d) start failed\r\n", Id);

    }
    else
    {
        AF_WaitIdle(Id);
        DBG_MSG("#AF idle\r\n");
        if (AF_Resume(Id) == E_OK)
            DBG_MSG("AF(%d) resume ready\r\n", Id);
        else
            DBG_MSG("AF(%d) resume failed\r\n", Id);

    }

    if(bWaitFinish)
        AF_WaitIdle(Id);

    return rt;
}

ER AF_Release(AF_PROC_ID Id, BOOL bWaitFinish)
{
    ER er = E_OK;

    AFALG_SetStatus(AFALG_QUIT);

    if(bWaitFinish)
        AF_WaitIdle(Id);
#if 0
    if(gAfCbFP)
    {
        uiStatus = AFALG_GetAFStatus();

        gAfCbFP(AF_CBMSG_AF_DONE, (void *)&uiStatus);
    }
#else
    //DBG_MSG("AF process time:%d\r\n", (Perf_GetCurrent() - g_uiAFPerfTime) /1000);
#endif

    return er;
}

INT32 AF_SearchFullRange(UINT32 id, INT16 startIdx, INT16 endIdx, UINT8 jumpStep, UINT32 *pMaxVA)
{
    INT16 idx, maxIdx = 0, i;
    UINT32 tempVA=0, maxVA=0;
//#NT#2016/05/25#Harry_Tsai -begin
    UINT32 uiG1h = 0, uiG1v = 0, uiG2h = 0, uiG2v = 0;
//#NT#2016/05/25#Harry_Tsai -end

    DBG_ERR("#AF_SearchFullRange\r\n");

    #if 0 // VA TBD.
    if(g_fpGetVariation == NULL)
    {
        DBG_ERR"Get Variation FP is null!!\r\n");
        return 0;
    }
    #endif

    //Lens_Focus_DoAction(FOCUS_PREEXC, startIdx);
    for(idx = startIdx, i = 0; idx <= (endIdx+jumpStep-1); idx += jumpStep, i++)
    {
        #if 0
        Lens_Focus_Goto2(MOTOR_POWERON_PI2_ONLY, idx);
        #else
        Lens_Focus_DoAction(FOCUS_PREEXC|FOCUS_RUN|FOCUS_POSTEXC, idx);
        #endif

        //#NT#2012/05/29#KS Hung -end
        //IPC_WaitVD(TRUE,id);
        //IPC_WaitVD(TRUE,id);
        //ipe_waitFrameEnd(IPE_CLRFLG);
        //ipe_waitFrameEnd(IPE_CLRFLG);
        //ipe_waitFrameEnd(IPE_CLRFLG);
        IPC_WaitVD(id,TRUE);
        IPC_WaitVD(id,TRUE);
        IPC_WaitVD(id,TRUE);
        #if 0
        contrastCnt = ipe_getVACnt();

        while(contrastCnt == ipe_getVACnt())
            Delay_DelayMs(1);
        #endif

        //DBG_ERR("#wait VD done\r\n");
        AF_GetVA_Result(&uiG1h, &uiG1v, &uiG2h, &uiG2v, id, VA_WIN_INDP0);
        tempVA = uiG1h + uiG1v;

        //DBG_ERR("%s: step=%d, tempVA=%d, maxVA=%d\r\n", __func__, idx, tempVA, maxVA);

        if(g_AFInfoIdx < (AF_FOCUS_ARRAY_SIZE - 1))
        {
            g_AFInfo.Step[g_AFInfoIdx] = idx;
            g_AFInfo.Value[g_AFInfoIdx] = tempVA;
            g_AFInfoIdx++;
        }

        //#NT#2012/06/05#KS Hung -begin
        if(tempVA > maxVA)
        {
            maxIdx = idx;
            maxVA = tempVA;
        }
        else
        {
            if (tempVA < (maxVA * 0.2)) //0.2 is threshold.
            {
                //DBG_ERR("find peak\r\n");
                break;
            }
        }
        //#NT#2012/06/05#KS Hung -end
    }
    //Lens_Focus_DoAction(FOCUS_POSTEXC, 0);

    //Lens_Focus_Goto(maxIdx);
    if(pMaxVA != NULL)
        *pMaxVA = maxVA;

    DBG_MSG("^R%s: maxIdx=%d, maxVA=%d\r\n", __func__, maxIdx, maxVA);

    return maxIdx;
}

void AF_ResetAFInfo(void)
{
    g_AFInfoIdx = 0;
    g_AFMaxIdx = 0;
    VaildStartIdx = 2;
    g_AFAlgCentralWinFound = 0;
    g_AFAlgMaxIdx = 0;
    memset(&g_AFInfo, 0, sizeof(AF_INFO));
}

void AF_ResetAFGuessInfo(void)
{
    g_AFGuessInfoIdx = 0;
    memset((void *)&g_AFGuessInfo, 0, sizeof(AF_GUESS_INFO));
}

static void AF_SetVA_window(VA_WIN_IDX VA_win, VACROP win_info)
{
    VACROP *pWinSel;

    if(VA_win == _VA_WIN_16X16)
    {
        g_tVA_Data.VAInfo.VA_WIN.WinNum_X = 16;
        g_tVA_Data.VAInfo.VA_WIN.WinNum_Y = 16;
        g_tVA_Data.VAInfo.VA_WIN.WinSizeX = win_info.SizeH;
        g_tVA_Data.VAInfo.VA_WIN.WinSizeY = win_info.SizeV;
        g_tVA_Data.VAInfo.VA_WIN.WinSpaceX = 0;
        g_tVA_Data.VAInfo.VA_WIN.WinSpaceY = 0;
    }
    else
    {
        if(VA_win == _VA_WIN_INDP0)
        {
            pWinSel = &g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[0].INDEP_WIN;
        }
        else if(VA_win == _VA_WIN_INDP1)
        {
            pWinSel = &g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[1].INDEP_WIN;
        }
        else if(VA_win == _VA_WIN_INDP2)
        {
            pWinSel = &g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[2].INDEP_WIN;
        }
        else if(VA_win == _VA_WIN_INDP3)
        {
            pWinSel = &g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[3].INDEP_WIN;
        }
        else if(VA_win == _VA_WIN_INDP4)
        {
            pWinSel = &g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[4].INDEP_WIN;
        }
        else if(VA_win == _VA_WIN_CROP)
        {
            pWinSel = &g_tVA_Data.VAInfo.VACrop;
        }
        else
            pWinSel = &g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[0].INDEP_WIN;

        memcpy(pWinSel ,&win_info, sizeof(VACROP));
    }
}

static void AF_SetVA_Threshold(VA_GRP VA_grp, UINT32 THL, UINT32 THH)
{
    if(VA_grp == VA_GRP1H)
    {
        g_tVA_Data.VAInfo.G1.FilterH.thres.Upper = THH;
        g_tVA_Data.VAInfo.G1.FilterH.thres.Lower = THL;
    }
    else if(VA_grp == VA_GRP1V)
    {
        g_tVA_Data.VAInfo.G1.FilterV.thres.Upper = THH;
        g_tVA_Data.VAInfo.G1.FilterV.thres.Lower = THL;
    }
    else if(VA_grp == VA_GRP2H)
    {
        g_tVA_Data.VAInfo.G2.FilterH.thres.Upper = THH;
        g_tVA_Data.VAInfo.G2.FilterH.thres.Lower = THL;
    }
    else if(VA_grp == VA_GRP2V)
    {
        g_tVA_Data.VAInfo.G2.FilterV.thres.Upper = THH;
        g_tVA_Data.VAInfo.G2.FilterV.thres.Lower = THL;
    }
}

static void AF_SetVA_Enable(BOOL enable, VA_WIN_IDX VA_win)
{
    if(VA_win == _VA_WIN_16X16)
        g_tVA_Data.VAOutEn = enable;
    else if(VA_win == _VA_WIN_INDP0)
    {
        g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[0].bVA_INDEP_En = enable;
    }
    else if(VA_win == _VA_WIN_INDP1)
    {
        g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[1].bVA_INDEP_En = enable;
    }
    else if(VA_win == _VA_WIN_INDP2)
    {
        g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[2].bVA_INDEP_En = enable;
    }
    else if(VA_win == _VA_WIN_INDP3)
    {
        g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[3].bVA_INDEP_En = enable;
    }
    else if(VA_win == _VA_WIN_INDP4)
    {
        g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[4].bVA_INDEP_En = enable;
    }
}

static void AF_SetVA_LineMax_Enable(BOOL enable, VA_WIN_IDX VA_win, VA_GRP VA_grp)
{
    if(VA_win == _VA_WIN_16X16)
    {
        if(VA_grp == VA_GRP1)
            g_tVA_Data.VAInfo.G1.LineMax = enable;
        else if(VA_grp == VA_GRP2)
            g_tVA_Data.VAInfo.G2.LineMax = enable;
    }
    else if(VA_win == _VA_WIN_INDP0)
    {
        if(VA_grp == VA_GRP1)
            g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[0].LineMaxG1 = enable;
        else if(VA_grp == VA_GRP2)
            g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[0].LineMaxG2 = enable;
    }
    else if(VA_win == _VA_WIN_INDP1)
    {
        if(VA_grp == VA_GRP1)
            g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[1].LineMaxG1 = enable;
        else if(VA_grp == VA_GRP2)
            g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[1].LineMaxG2 = enable;
    }
    else if(VA_win == _VA_WIN_INDP2)
    {
        if(VA_grp == VA_GRP1)
            g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[2].LineMaxG1 = enable;
        else if(VA_grp == VA_GRP2)
            g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[2].LineMaxG2 = enable;
    }
    else if(VA_win == _VA_WIN_INDP3)
    {
        if(VA_grp == VA_GRP1)
            g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[3].LineMaxG1 = enable;
        else if(VA_grp == VA_GRP2)
            g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[3].LineMaxG2 = enable;
    }
    else if(VA_win == _VA_WIN_INDP4)
    {
        if(VA_grp == VA_GRP1)
            g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[4].LineMaxG1 = enable;
        else if(VA_grp == VA_GRP2)
            g_tVA_Data.VAInfo.VA_INDEP_WIN_INFO[4].LineMaxG2 = enable;
    }
}

static void AF_SetVA_HExtend_Enable(BOOL enable, VA_WIN_IDX VA_win, VA_GRP VA_grp)
{
    if(VA_win == _VA_WIN_16X16)
    {
        if(VA_grp == VA_GRP1)
            g_tVA_Data.VAInfo.G1.HExtend = enable;
        else if(VA_grp == VA_GRP2)
            g_tVA_Data.VAInfo.G2.HExtend = enable;
    }
}

//af_tmp
static void AF_SetVA_RGBGain(void)
{
    g_tVA_Data.VAInfo.VA_CG_INFO.RGain = 256;
    g_tVA_Data.VAInfo.VA_CG_INFO.GGain = 256;
    g_tVA_Data.VAInfo.VA_CG_INFO.BGain = 256;
}

//af_tmp
//gamma=2, 33 taps, 8bits
UINT16 AF_GammaLut[33]=
{
      0, 45, 63, 78, 90,100,110,119,127,135,
    142,149,156,162,168,174,180,185,191,196,
    201,206,211,216,220,225,229,234,238,242,
    246,250,255
};

//af_tmp
static void AF_SetVA_pGmaTbl(void)
{
    g_tVA_Data.VAInfo.GmaTbl = AF_GammaLut;
}

//af_tmp
static void AF_SetVA_pGmaTbl_Enable(BOOL enable)
{
    g_tVA_Data.VAGammaEn = enable;
}

//af_tmp
static void AF_SetVA_RGBGain_Enable(BOOL enable)
{
    g_tVA_Data.VACgEn = enable;
}

static void AF_SetVA_EdgeParam(VA_GRP VA_grp ,VA_FLTR_INFO edge_ker)
{
    VA_FLTR_INFO* GrpSel;

    if(VA_grp == VA_GRP1H)
        GrpSel = &g_tVA_Data.VAInfo.G1.FilterH;
    else if(VA_grp == VA_GRP1V)
        GrpSel = &g_tVA_Data.VAInfo.G1.FilterV;
    else if(VA_grp == VA_GRP2H)
        GrpSel = &g_tVA_Data.VAInfo.G2.FilterH;
    else if(VA_grp == VA_GRP2V)
        GrpSel = &g_tVA_Data.VAInfo.G2.FilterV;
    else
    {
        DBG_MSG("unknown VA_grp value\r\n");
        return;
    }

    memcpy(GrpSel ,&edge_ker, sizeof(VA_FLTR_INFO));
}

void AF_SetVAparam2IPL(UINT32 id) // please call this api after setting whole parameters or changing parameters
{
    //g_tVA_Data.VASettings.Vacc.ManualUpdateEn = VACC_AUTO_UPDATED;

    g_tVA_Data.Id = id;

    //af_tmp
//#NT#2016/05/25#Harry_Tsai -begin
//#NT# Give the same parameter for different conditions will cause CIM deffect
#if 0    
    if(IPL_AlgGetUIInfo(id, IPL_SEL_AFWINDOW) == AFALG_WINDOW_CENTER)
        g_tVA_Data.VAInfo.va_outsel = STATS_VA_OUT_GP_1n2;
    else
        g_tVA_Data.VAInfo.va_outsel = STATS_VA_OUT_GP_1n2;
#else
    g_tVA_Data.VAInfo.va_outsel = STATS_VA_OUT_GP_1n2;
#endif
//#NT#2016/05/25#Harry_Tsai -end

    IPL_SetCmd(IPL_SET_VA, (void *)&g_tVA_Data);
    IPL_WaitCmdFinish();
}

void AF_GetVA_Result(UINT32 *G1H, UINT32 *G1V, UINT32 *G2H, UINT32 *G2V, UINT32 id, VA_WINIDX WinIdx)
{
    IPL_VA_INFOR VAInfo;

    VAInfo.Grp1H = G1H;
    VAInfo.Grp1V = G1V;
    VAInfo.Grp2H = G2H;
    VAInfo.Grp2V = G2V;
    VAInfo.VAWin = WinIdx;
    VAInfo.Id = id;
    //af_tmp
    #if 0
    if(WinIdx == VA_WIN_16X16)
        VAInfo.Length = 256;
    else
        VAInfo.Length = 1;
    VAInfo.VAoutputFmt = g_tVA_Data.VASettings.Vacc.VAOutSel;
    #endif
    IPL_GetCmd(IPL_GET_CUR_VA_INFOR, (void *)&VAInfo);
}

static UINT32 AF_CalVA_ThresholdbyISO(UINT32 id)
{
//    UINT32 IsoIdx;
    UINT32 ISO, ExpT, Iris;
    UINT32 uiThreshold = 0;
    AE_GetPrvAEArg(id, &ISO, &ExpT, &Iris);

    if ( ISO <= 150 )
    {
        //IsoIdx =  PRVISO0100;
        uiThreshold = 0;
    }
    else if ( ISO <= 300 )
    {
        //IsoIdx =  PRVISO0200;
        uiThreshold = 0;
    }
    else if ( ISO <= 600 )
    {
        //IsoIdx =  PRVISO0400;
        uiThreshold = 0;
    }
    else if ( ISO <= 1200 )
    {
        //IsoIdx =  PRVISO0800;
        uiThreshold = 0;
    }
    else
    {
        //IsoIdx =  PRVISO1600;
        uiThreshold = 1;
    }

    DBG_MSG("ISO = %d, Th = %d\r\n", ISO, uiThreshold);

    return uiThreshold;
}

static BOOL bSet = FALSE;

void AF_SetVA_Crop(VACROP win_info)
{
    VACROP *pWinSel;

    pWinSel = &g_tVA_Data.VAInfo.VACrop;
    memcpy(pWinSel ,&win_info, sizeof(VACROP));

    bSet = TRUE;
}

#define AF_VA_DIV                4
#define AF_VA_THRESHOLD          1
#define AF_VA_WIN_W              432 //511 maximum
#define AF_VA_WIN_H              255 //255 maximum
#define AF_VA_WIN_STX(Width)     ((Width - AF_VA_WIN_W) >> 1)
#define AF_VA_WIN_STY(Height)    ((Height - AF_VA_WIN_H) >> 1)
extern void IPL_DzoomGetSIEOutSize(UINT32 Id, UINT32 Idx, UINT32 *SizeH, UINT32 *SizeV);
extern UINT32 IPL_DzoomGetIndex(UINT32 Id);
void AF_VA_Setup(UINT32 id)
{
    UINT32 Width = 0, Height= 0;
    //#NT#2016/05/06#Jarkko Chang -begin
    //#NT# fixed warning
    VACROP VA_Win_Info = {0};
    //#NT#2016/05/06#Jarkko Chang -end
    VA_FLTR_INFO VA_Ker;
    IPL_SIE_CUR_FRAME_INFO Info = {0};

    if(bSet == TRUE)
    {
        Width = g_tVA_Data.VAInfo.VACrop.SizeH;
        Height = g_tVA_Data.VAInfo.VACrop.SizeV;

        bSet = FALSE;
        //#NT#2016/05/10#Harry_Tsai -begin
        VA_Win_Info.StartPix.x = 0;
        VA_Win_Info.SizeH = Width;
        VA_Win_Info.StartPix.y = 0;
        VA_Win_Info.SizeV = Height;
        //#NT#2016/05/10#Harry_Tsai -end
    }
    else
    {
        //IPL_DzoomGetSIEOutSize(id, IPL_DzoomGetIndex(id), &Width, &Height);
        Info.Id = id;
        IPL_GetCmd(IPL_GET_SIE_CUR_FRAME_INFOR, &Info);
        Width = Info.OutSizeH;
        Height = Info.OutSizeV;

        if(Width <= 2304)
        {
            VA_Win_Info.StartPix.x = 0;
            VA_Win_Info.SizeH = Width;
        }
        else
        {
            VA_Win_Info.StartPix.x = (Width - 2304)/2;
            VA_Win_Info.SizeH = 2304;
        }

        if(Height <= 1294)
        {
            VA_Win_Info.StartPix.y = 0;
            VA_Win_Info.SizeV = Height-4;
        }
        else
        {
            VA_Win_Info.StartPix.y = (Height - 1292)/2;
            VA_Win_Info.SizeV = 1292;//Height-4;
        }
        AF_SetVA_window(_VA_WIN_CROP, VA_Win_Info);
    }
    DBG_MSG("w:%d, h:%d, stx:%d, sty:%d\r\n", Width, Height, AF_VA_WIN_STX(Width), AF_VA_WIN_STY(Height));

    //af_tmp
    AF_SetVA_RGBGain();
    AF_SetVA_pGmaTbl();
    AF_SetVA_RGBGain_Enable(0);
    AF_SetVA_pGmaTbl_Enable(1);

    VA_Win_Info.StartPix.x = 0;
    VA_Win_Info.StartPix.y = 0;
    //#NT#2016/05/04#Harry_Tsai -begin
    VA_Win_Info.SizeH = (VA_Win_Info.SizeH >> 4); //Width >>4
    VA_Win_Info.SizeV = (VA_Win_Info.SizeV >> 4); // Height >>4
    //#NT#2016/05/04#Harry_Tsai -end
    AF_SetVA_window(_VA_WIN_16X16, VA_Win_Info);

    VA_Win_Info.StartPix.x = AF_VA_WIN_STX(Width);
    VA_Win_Info.StartPix.y = AF_VA_WIN_STY(Height);
    VA_Win_Info.SizeH = AF_VA_WIN_W;
    VA_Win_Info.SizeV = AF_VA_WIN_H;
    AF_SetVA_window(_VA_WIN_INDP0, VA_Win_Info);

    VA_Win_Info.StartPix.x = 4;
    VA_Win_Info.StartPix.y = 6;
    VA_Win_Info.SizeH = 40;
    VA_Win_Info.SizeV = 20;
    AF_SetVA_window(_VA_WIN_INDP1, VA_Win_Info);

    VA_Win_Info.StartPix.x = 20;
    VA_Win_Info.StartPix.y = 40;
    VA_Win_Info.SizeH = 20;
    VA_Win_Info.SizeV = 10;
    AF_SetVA_window(_VA_WIN_INDP2, VA_Win_Info);

    VA_Win_Info.StartPix.x = 20;
    VA_Win_Info.StartPix.y = 40;
    VA_Win_Info.SizeH = 20;
    VA_Win_Info.SizeV = 10;
    AF_SetVA_window(_VA_WIN_INDP3, VA_Win_Info);

    VA_Win_Info.StartPix.x = 20;
    VA_Win_Info.StartPix.y = 40;
    VA_Win_Info.SizeH = 20;
    VA_Win_Info.SizeV = 10;
    AF_SetVA_window(_VA_WIN_INDP4, VA_Win_Info);

    AF_SetVA_Enable(1,_VA_WIN_16X16); //must to be enable
    AF_SetVA_Enable(1,_VA_WIN_INDP0);
    AF_SetVA_Enable(0,_VA_WIN_INDP1);
    AF_SetVA_Enable(0,_VA_WIN_INDP2);
    AF_SetVA_Enable(0,_VA_WIN_INDP3);
    AF_SetVA_Enable(0,_VA_WIN_INDP4);

    AF_SetVA_LineMax_Enable(0,_VA_WIN_16X16,VA_GRP1);
    AF_SetVA_LineMax_Enable(0,_VA_WIN_16X16,VA_GRP2);
    AF_SetVA_LineMax_Enable(0,_VA_WIN_INDP0,VA_GRP1);
    AF_SetVA_LineMax_Enable(0,_VA_WIN_INDP0,VA_GRP2);
    AF_SetVA_LineMax_Enable(0,_VA_WIN_INDP1,VA_GRP1);
    AF_SetVA_LineMax_Enable(0,_VA_WIN_INDP1,VA_GRP2);
    AF_SetVA_LineMax_Enable(0,_VA_WIN_INDP2,VA_GRP1);
    AF_SetVA_LineMax_Enable(0,_VA_WIN_INDP2,VA_GRP2);
    AF_SetVA_LineMax_Enable(0,_VA_WIN_INDP3,VA_GRP1);
    AF_SetVA_LineMax_Enable(0,_VA_WIN_INDP3,VA_GRP2);
    AF_SetVA_LineMax_Enable(0,_VA_WIN_INDP4,VA_GRP1);
    AF_SetVA_LineMax_Enable(0,_VA_WIN_INDP4,VA_GRP2);

    AF_SetVA_HExtend_Enable(0,_VA_WIN_16X16,VA_GRP1);
    AF_SetVA_HExtend_Enable(0,_VA_WIN_16X16,VA_GRP2);

    VA_Ker.FilterSize = STATS_VA_FLTR_SIZE_5;
    VA_Ker.FillterrSymmetry = STATS_VA_FLTR_MIRROR;
    VA_Ker.FILT_ITEMS.A = 2;
    VA_Ker.FILT_ITEMS.B = -1;
    VA_Ker.FILT_ITEMS.C = 0;
    VA_Ker.FILT_ITEMS.D = 0;
    VA_Ker.uiDiv = AF_VA_DIV;
    AF_SetVA_EdgeParam(VA_GRP1H, VA_Ker);

    VA_Ker.FilterSize = STATS_VA_FLTR_SIZE_5;
    VA_Ker.FillterrSymmetry = STATS_VA_FLTR_MIRROR;
    VA_Ker.FILT_ITEMS.A = 2;
    VA_Ker.FILT_ITEMS.B = -1;
    VA_Ker.FILT_ITEMS.C = 0;
    VA_Ker.FILT_ITEMS.D = 0;
    VA_Ker.uiDiv = AF_VA_DIV;
    AF_SetVA_EdgeParam(VA_GRP1V, VA_Ker);

    VA_Ker.FilterSize = STATS_VA_FLTR_SIZE_5;
    VA_Ker.FillterrSymmetry = STATS_VA_FLTR_MIRROR;
    VA_Ker.FILT_ITEMS.A = 2;
    VA_Ker.FILT_ITEMS.B = 0;
    VA_Ker.FILT_ITEMS.C = -1;
    VA_Ker.FILT_ITEMS.D = 0;
    VA_Ker.uiDiv = AF_VA_DIV;
    AF_SetVA_EdgeParam(VA_GRP2H, VA_Ker);

    VA_Ker.FilterSize = STATS_VA_FLTR_SIZE_5;
    VA_Ker.FillterrSymmetry = STATS_VA_FLTR_MIRROR;
    VA_Ker.FILT_ITEMS.A = 2;
    VA_Ker.FILT_ITEMS.B = 0;
    VA_Ker.FILT_ITEMS.C = -1;
    VA_Ker.FILT_ITEMS.D = 0;
    VA_Ker.uiDiv = AF_VA_DIV;
    AF_SetVA_EdgeParam(VA_GRP2V, VA_Ker);

    AF_SetVA_Threshold(VA_GRP1H, AF_CalVA_ThresholdbyISO(id), AF_CalVA_ThresholdbyISO(id));
    AF_SetVA_Threshold(VA_GRP1V, AF_CalVA_ThresholdbyISO(id), AF_CalVA_ThresholdbyISO(id));
    AF_SetVA_Threshold(VA_GRP2H, AF_CalVA_ThresholdbyISO(id), AF_CalVA_ThresholdbyISO(id));
    AF_SetVA_Threshold(VA_GRP2V, AF_CalVA_ThresholdbyISO(id), AF_CalVA_ThresholdbyISO(id));

    AF_SetVAparam2IPL(id);
}

void AF_VA_Release(UINT32 id)
{
    VACROP VA_Win_Info;

    VA_Win_Info.StartPix.x = 0;
    VA_Win_Info.StartPix.y = 0;
    VA_Win_Info.SizeH = (2 >> 4);
    VA_Win_Info.SizeV = (2 >> 4);
    AF_SetVA_window(_VA_WIN_16X16, VA_Win_Info);

    VA_Win_Info.StartPix.x = 0;//AF_VA_WIN_STX(Width);
    VA_Win_Info.StartPix.y = 0;//AF_VA_WIN_STY(Height);
    VA_Win_Info.SizeH = (2 >> 4);// AF_VA_WIN_W;
    VA_Win_Info.SizeV = (2 >> 4);//AF_VA_WIN_H;
    AF_SetVA_window(_VA_WIN_INDP0, VA_Win_Info);

    AF_SetVA_Enable(0,_VA_WIN_16X16);
    AF_SetVA_Enable(0,_VA_WIN_INDP0);
    AF_SetVAparam2IPL(id);
}
AFDRV_ONVIFPARM AFOnvifParm[IPL_ID_MAX_NUM] = {0};

void AF_OnvifSet(IPL_PROC_ID Id,AF_ONVIFCOMMAND command,AFDRV_ONVIFPARM* afparm)
{
    debug_msg("Auto focus setting %d  %d",Id,command);
    if(command == PARMTYPE)
    {
        AFOnvifParm[Id].parmtype.mode = afparm->parmtype.mode;
        AFOnvifParm[Id].parmtype.speed = afparm->parmtype.speed;
        AFOnvifParm[Id].parmtype.near_limit = afparm->parmtype.near_limit;
        AFOnvifParm[Id].parmtype.far_limit =  afparm->parmtype.far_limit;

    }
    else if(command == ABSOLUTEPARM)
    {
        AFOnvifParm[Id].absoluteparm.position = afparm->absoluteparm.position;
        AFOnvifParm[Id].absoluteparm.speed = afparm->absoluteparm.speed;

    }
    else if(command == CONTINUOUSPARM)
    {
        AFOnvifParm[Id].continuousparm.speed = afparm->continuousparm.speed;
    }
    else if(command == RELATIVESPARM)
    {
        AFOnvifParm[Id].relativeparm.distance = afparm->relativeparm.distance;
        AFOnvifParm[Id].relativeparm.speed = afparm->relativeparm.speed;

    }
    else if(command == STOPPARM)
    {
        AFOnvifParm[Id].stopparam.value =  afparm->stopparam.value;
    }
    else
        debug_msg("Auto focus dont's support(onvif)",Id,command);

}
void AF_OnvifGet(IPL_PROC_ID Id)
{
    debug_msg("Auto focus get status");
}
