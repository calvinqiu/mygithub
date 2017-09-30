#include "Type.h"
#include "ImageApp_CamMovie.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIFlow.h"
#define __MODULE__          UIFlowMovieFuncs
//#define __DBGLVL__ 0        //OFF mode, show nothing
#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
//#define __DBGLVL__ 2        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

#define MOVIE_REC_TIME_MIN  1 // 1 sec

#define MOVIE_REC_MIN_CLUSTER_SIZE      (0x8000)    //32KB
#define MOVIE_REC_SD_CLASS              (4)         // class 4

MOV_TASK_DATA   gMovData = { 0 };

static UINT32  g_MovRecMaxTime = 0;
static UINT32  g_MovRecCurrTime = 0;
static UINT32  g_MovRecSelfTimerSec = 0;
static UINT32  g_MovRecSelfTimerID = NULL_TIMER;
#if _ADAS_FUNC_
static ADAS_OSD_COORDINATE g_AdasOsdCoord = {0};
#endif
UINT8 FlowMovie_GetMovDataState(void)
{
    return gMovData.State;
}

void FlowMovie_StartRec(void)
{
    if (g_MovRecSelfTimerSec == 0)
    {
        g_MovRecCurrTime = 0;
#if (USE_FILEDB == DISABLE)
        // update DCF Folder/File name
        FlowDCF_UpdateName();
#endif
        if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_SAFE)
        {
        // wait playing sound stop
        GxSound_WaitStop();
        }
        Ux_SendEvent(&CustomMovieObjCtrl,NVTEVT_EXE_MOVIE_REC_START,NULL);

        //#NT#2012/10/23#Philex Lin - begin
        // disable auto power off/USB detect timer
        KeyScan_EnableMisc(FALSE);
        //#NT#2012/10/23#Philex Lin - end
    }
    else
    {
        debug_err(("FlowMovie_StartRec: Not yet,g_MovSelfTimerSec=%d\r\n", g_MovRecSelfTimerSec));
    }
}

void FlowMovie_StopRec(void)
{
#if 0
    UxState_SetData(&UIFlowWndMovie_Status_RECCtrl,STATE_CURITEM,UIFlowWndMovie_Status_REC_ICON_REC_TRANSPAENT);
#endif
    Ux_SendEvent(&CustomMovieObjCtrl,NVTEVT_EXE_MOVIE_REC_STOP,NULL);

    //#NT#2012/10/23#Philex Lin - begin
    // enable auto power off/USB detect timer
    KeyScan_EnableMisc(TRUE);
    //#NT#2016/06/21#Niven Cho -begin
    //#NT#Remove danger code
    //#NT#2016/06/21#Niven Cho -end

    gMovData.State= MOV_ST_VIEW;
    //#NT#2012/10/23#Philex Lin - end

}

void FlowMovie_StopRecSelfTimer(void)
{
    if (g_MovRecSelfTimerID != NULL_TIMER)
    {
        GxTimer_StopTimer(&g_MovRecSelfTimerID);
        g_MovRecSelfTimerID = NULL_TIMER;
    }
    g_MovRecSelfTimerSec = 0;
}

UINT8 FlowMovie_GetDataState(void)
{
    return gMovData.State;
}

void FlowMovie_SetRecMaxTime(UINT32 RecMaxTime)
{
    g_MovRecMaxTime = RecMaxTime;
}

UINT32 FlowMovie_GetRecMaxTime(void)
{
    return g_MovRecMaxTime;
}

void FlowMovie_SetRecCurrTime(UINT32 RecCurrTime)
{
    g_MovRecCurrTime = RecCurrTime;
}

UINT32 FlowMovie_GetRecCurrTime(void)
{
    return g_MovRecCurrTime;
}

BOOL FlowMovie_IsStorageErr(BOOL IsCheckFull)
{
#if (MOVIE_ENSURE_SD_32KCLUSTER == ENABLE)
    UINT32  uiClusterSize;
#endif
#if (MOVIE_ENSURE_SD_CLASS4 == ENABLE)
    PSDIO_MISC_INFORMATION pMiscInfo;
#endif

    // check card inserted
    if (UI_GetData(FL_CardStatus) == CARD_REMOVED)
    {
        //Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_PLEASE_INSERT_SD,FLOWWRNMSG_TIMER_2SEC);
        DBG_ERR("***** STRG CHECK: Card Removed! *****\r\n");
        return TRUE;
    }

    // check storage error
    if(UIStorageCheck(STORAGE_CHECK_ERROR, NULL) == TRUE)
    {
        //Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,FLOWWRNMSG_ISSUE_MEM_ERR,FLOWWRNMSG_TIMER_KEEP);
        DBG_ERR("***** STRG CHECK: Card ERROR! *****\r\n");
        return TRUE;
    }


    // check storage lock or directory read only
    if (UIStorageCheck(STORAGE_CHECK_LOCKED, NULL) == TRUE ||
        UIStorageCheck(STORAGE_CHECK_DCIM_READONLY, NULL) == TRUE)
    {
        //Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_LOCKED,FLOWWRNMSG_TIMER_2SEC);
        DBG_ERR("***** STRG CHECK: Card READ ONLY or Folder READ ONLY! *****\r\n");
        return TRUE;
    }

    // check storage full
    if (TRUE == IsCheckFull)
    {
        g_MovRecMaxTime = MovieExe_GetMaxRecSec();
        if (g_MovRecMaxTime <= MOVIE_REC_TIME_MIN)
        {
            g_MovRecMaxTime = 0;
            //Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_FULL,FLOWWRNMSG_TIMER_2SEC);
            DBG_ERR("***** STRG CHECK: Card FULL! *****\r\n");
            return TRUE;
        }
    }

    // check folder full
    if (UIStorageCheck(STORAGE_CHECK_FOLDER_FULL, NULL) == TRUE)
    {
        //Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_FULL,FLOWWRNMSG_TIMER_2SEC);
        DBG_ERR("***** STRG CHECK: Folder FULL! *****\r\n");
        return TRUE;
    }

#if (MOVIE_ENSURE_SD_CLASS4 == ENABLE)
    // check sd card whether faster than class 4
    pMiscInfo = sdio_getMiscInfo();
    if (pMiscInfo->uiWriteRate < MOVIE_REC_SD_CLASS)
    {
        //Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_SD_CLASS4,FLOWWRNMSG_TIMER_2SEC);
        DBG_ERR("***** STRG CHECK: WriteRate < MOVIE_REC_SD_CLASS! *****\r\n");
        return TRUE;
    }
#endif

#if (MOVIE_ENSURE_SD_32KCLUSTER == ENABLE)
    // Check cluster size, need to be larger than 32KB
    uiClusterSize = FileSys_GetDiskInfo(FST_INFO_CLUSTER_SIZE);
    if (uiClusterSize < MOVIE_REC_MIN_CLUSTER_SIZE)
    {
        //Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CLUSTER_WRONG,FLOWWRNMSG_TIMER_2SEC);
        DBG_ERR("***** STRG CHECK: ClusterSize < MOVIE_REC_MIN_CLUSTER_SIZE! *****\r\n");
        return TRUE;
    }
#endif

    return FALSE;
}

UINT32 FlowMovie_GetSelfTimerID(void)
{
    return g_MovRecSelfTimerID;
}

#if 0
BOOL FlowMovie_ChkDrawStoreFullFolderFull(void)
{

    if(UIStorageCheck(STORAGE_CHECK_FULL, &(g_MovRecMaxTime)) == TRUE)
    {
        g_MovRecMaxTime = UIMovRecObj_GetData(RECMOVIE_MAXSECOND);
        if (UI_GetData(FL_CardStatus) == CARD_REMOVED)
        {
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,FLOWWRNMSG_ISSUE_MEM_FULL,FLOWWRNMSG_TIMER_2SEC);
            return TRUE;
        }
        else
        {
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,FLOWWRNMSG_ISSUE_CARD_FULL,FLOWWRNMSG_TIMER_2SEC);
            return TRUE;
        }
    }

    return FALSE;
}
#endif

void FlowMovie_GetPictureInMovie(void)
{
#if (0)//(_MOVIE_PIM_MODE_ == ENABLE)
    UINT32  sec, unsafe;

    switch (gMovData.State)
    {
    case MOV_ST_REC:
        sec = MovRec_Disk2Second();
        unsafe = MovRec_CheckUnsavedFileBlocks();

        if ((sec > 3)&&(unsafe == 0))
        {
            RawEnc_SetCaptureOne();
            //UxCtrl_SetShow(&UIFlowWndMovie_Static_pimCtrl, TRUE);
        }
        else
        {
            if (unsafe)
            {
                debug_err(("unsafe to take picture !\r\n"));
            }
            else
            {
                debug_err(("no space to encode RAW !\r\n"));
            }
        }
        break;
    }
#endif
}

void FlowMovie_OnTimer1SecIndex(void)
{
    switch (gMovData.State)
    {
    case MOV_ST_VIEW:
    case MOV_ST_VIEW | MOV_ST_ZOOM:
    case MOV_ST_REC:
    case MOV_ST_REC | MOV_ST_ZOOM:
        gMovData.SysTimeCount++;
#if 0
        if (UxCtrl_IsShow(&UIFlowWndMovie_YMD_StaticCtrl))
        {
            FlowMovie_IconDrawDateTime();
        }
#endif
        break;

    }
}

#if _ADAS_FUNC_
void FlowMovie_AdasProcess(ADAS_OSD_COORDINATE AdasOsdCoord, ADAS_RESULT_INFO *pAdasInfo)
{
    g_AdasOsdCoord = AdasOsdCoord;

    /* Enable LDWS show */
    if ( (pAdasInfo->LdwsRsltInfo.Failure == LDWS_FAILURE_FALSE) &&
         ((pAdasInfo->LdwsRsltInfo.DepartureDirVoice == LDWS_DEPARTURE_LEFT) || (pAdasInfo->LdwsRsltInfo.DepartureDirVoice == LDWS_DEPARTURE_RIGHT)) )
    {
        UISound_Play(DEMOSOUND_SOUND_LDWS_TONE);
        if (!UxCtrl_IsShow(&UIFlowWndMovie_ADAS_Alert_DisplayCtrl))
        {
            UxCtrl_SetShow(&UIFlowWndMovie_Panel_Normal_DisplayCtrl, FALSE);
            UxCtrl_SetShow(&UIFlowWndMovie_ADAS_Alert_DisplayCtrl,TRUE);
            UxCtrl_SetShow(&UIFlowWndMovie_StaticICN_LDWS_AlertCtrl,TRUE);
            UxCtrl_SetShow(&UIFlowWndMovie_StaticICN_FCWS_AlertCtrl,FALSE);
        }
    }

    else if ( (pAdasInfo->LdwsRsltInfo.Failure == LDWS_FAILURE_FALSE) && (pAdasInfo->FcwsRsltInfo.FCSoundWarning == TRUE) )
    {
        //UISound_Play(DEMOSOUND_SOUND_FCS_TONE);
        if (!UxCtrl_IsShow(&UIFlowWndMovie_ADAS_Alert_DisplayCtrl))
        {
            UxCtrl_SetShow(&UIFlowWndMovie_Panel_Normal_DisplayCtrl, FALSE);
            UxCtrl_SetShow(&UIFlowWndMovie_ADAS_Alert_DisplayCtrl,TRUE);
            UxCtrl_SetShow(&UIFlowWndMovie_StaticICN_LDWS_AlertCtrl,FALSE);
            UxCtrl_SetShow(&UIFlowWndMovie_StaticICN_FCWS_AlertCtrl,TRUE);
        }
    }


    /* Enable FD frame show */
    UxCtrl_SetShow(&UIFlowWndMovie_ADAS_DrawingLineCtrl,TRUE);

    /* Redraw all window OSD */
    UxCtrl_SetDirty(&UIFlowWndMovieCtrl, TRUE);
}

UINT32 FlowMovie_GetAdasOsdIno(void)
{
    return (UINT32)&g_AdasOsdCoord;
}
#endif
