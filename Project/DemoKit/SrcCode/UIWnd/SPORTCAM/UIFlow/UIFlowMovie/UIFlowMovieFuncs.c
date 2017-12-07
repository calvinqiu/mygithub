#include "Type.h"
#include "ImageApp_CamMovie.h"
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "UIFlow.h"
#define MOVIE_REC_TIME_MIN  1 // 1 sec

#define MOVIE_REC_MIN_CLUSTER_SIZE      (0x8000)    //32KB
#define MOVIE_REC_SD_CLASS              (4)         // class 4

MOV_TASK_DATA   gMovData = { 0 };

static UINT32  g_MovRecMaxTime = 0;
static UINT32  g_MovRecCurrTime = 0;
static UINT32  g_MovRecSelfTimerSec = 0;
static UINT32  g_MovRecSelfTimerID = NULL_TIMER;
#if _ADAS_FUNC_
extern UINT32 g_uiAdasAlertSecCnt;
BOOL g_StopWarningEn = FALSE;
extern UINT32 g_uiAdasAlertDistance;
#endif
#if defined(YQCONFIG_UART_TO_MTK_SUPPORT) && defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
extern UINT32 g_MTKSetADASEnSpeed;
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
    //#NT#2016/03/25#KCHong -begin
    //#NT#New ADAS
    #if (_ADAS_FUNC_ == ENABLE)
    // Fixed icon disappear issue when stop record during ADAS warning window
    UxCtrl_SetShow(&UIFlowWndMovie_Panel_Normal_DisplayCtrl, TRUE);
    UxCtrl_SetShow(&UIFlowWndMovie_ADAS_Alert_DisplayCtrl,FALSE);
    #endif
    //#NT#2016/03/25#KCHong -end

    UxState_SetData(&UIFlowWndMovie_Status_RECCtrl,STATE_CURITEM,UIFlowWndMovie_Status_REC_ICON_REC_TRANSPAENT);
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
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_PLEASE_INSERT_SD,FLOWWRNMSG_TIMER_2SEC);
        return TRUE;
    }

    // check storage error
    if(UIStorageCheck(STORAGE_CHECK_ERROR, NULL) == TRUE)
    {
#if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,FLOWWRNMSG_ISSUE_MEM_ERR,FLOWWRNMSG_TIMER_3SEC);
#else
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,FLOWWRNMSG_ISSUE_MEM_ERR,FLOWWRNMSG_TIMER_KEEP);
#endif
        return TRUE;
    }


    // check storage lock or directory read only
    if (UIStorageCheck(STORAGE_CHECK_LOCKED, NULL) == TRUE ||
        UIStorageCheck(STORAGE_CHECK_DCIM_READONLY, NULL) == TRUE)
    {
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_LOCKED,FLOWWRNMSG_TIMER_2SEC);
        return TRUE;
    }

    // check storage full
    if (TRUE == IsCheckFull)
    {
        g_MovRecMaxTime = MovieExe_GetMaxRecSec();
        if (g_MovRecMaxTime <= MOVIE_REC_TIME_MIN)
        {
            g_MovRecMaxTime = 0;
            Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_FULL,FLOWWRNMSG_TIMER_2SEC);
            return TRUE;
        }
    }

    // check folder full
    if (UIStorageCheck(STORAGE_CHECK_FOLDER_FULL, NULL) == TRUE)
    {
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_FULL,FLOWWRNMSG_TIMER_2SEC);
        return TRUE;
    }


#if (MOVIE_ENSURE_SD_CLASS4 == ENABLE)
    // check sd card whether faster than class 4
    pMiscInfo = sdio_getMiscInfo();
    if (pMiscInfo->uiWriteRate < MOVIE_REC_SD_CLASS)
    {
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_SD_CLASS4,FLOWWRNMSG_TIMER_2SEC);
        return TRUE;
    }
#endif

#if (MOVIE_ENSURE_SD_32KCLUSTER == ENABLE)
    // Check cluster size, need to be larger than 32KB
    uiClusterSize = FileSys_GetDiskInfo(FST_INFO_CLUSTER_SIZE);
    if (uiClusterSize < MOVIE_REC_MIN_CLUSTER_SIZE)
    {
        Ux_OpenWindow(&UIFlowWndWrnMsgCtrl,2,UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CLUSTER_WRONG,FLOWWRNMSG_TIMER_2SEC);
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
        if (UxCtrl_IsShow(&UIFlowWndMovie_YMD_StaticCtrl))
        {
            FlowMovie_IconDrawDateTime();
        }
        break;

    }
}


#if defined(YQCONFIG_START_RECORDING_DELETE_OLD_FILE)
#if USE_FILEDB
#include "FileDB.h"
#include "NameRule_FileDB.h"
#endif

void UIFlowWndMovie_OnDeleteOld(void)
{
    UINT32 filenum=0,sec=0;
    UINT32 index = 0;
    PFILEDB_FILE_ATTR  pfile;
    UINT32 reserveSpace = 0;
    UINT32 deleteFilenum = 0;

    debug_msg("\n\r UIFlowWndMovie_OnDeleteOld start \n\r");
    // delete old file and start recording again
    if (1)//(SysGetFlag(FL_MOVIE_CYCLIC_REC)!=MOVIE_CYCLICREC_OFF)
    {
        switch(SysGetFlag(FL_MOVIE_CYCLIC_REC))
        {
    /*    case MOVIE_CYCLICREC_1MIN:
            sec=60;
            break;
        case MOVIE_CYCLICREC_2MIN:
            sec=120;
            break;
            */
        case MOVIE_CYCLICREC_3MIN:
            sec=180;
            break;
        }

        reserveSpace =  180 * 4;  // ?????????????????,Harvey@20160530

        if(MovieExe_GetMaxRecSec()>(reserveSpace))
        {
            return;
        }

        FileDB_SortBy(0,FILEDB_SORT_BY_MODDATE,FALSE);
        FileDB_Refresh(0);
        filenum = FileDB_GetTotalFileNum(0);
        //debug_msg("UIFlowWndMovie_OnDeleteOld Total File :%d..\r\n",filenum);
        /*
        while(filenum--)
        {
            pfile = FileDB_SearhFile2(0,index);
          debug_msg("pfile->filePath:%s..\r\n",pfile->filePath);
            index++;
        }*/

        index =0;
        // filenum = FileDB_GetTotalFileNum(0);

        while(filenum--)
        {
            pfile = FileDB_SearhFile2(0,index);
            //debug_msg("pfile->filePath:%s..\r\n",pfile->filePath);

            if(!M_IsReadOnly(pfile->attrib))
            {
                // if (MovieExe_GetMaxRecSec()>(sec+20))
                //     break;

                //???????????
                deleteFilenum++;
                //debug_msg("deleteFilenum = %d, delete pfile->filePath:%s..\r\n",deleteFilenum,pfile->filePath);
                
                FileSys_DeleteFile(pfile->filePath);
                FileDB_DeleteFile(0,index);

                if (MovieExe_GetMaxRecSec()>(reserveSpace) && (deleteFilenum%2 == 0))
                {
                    break;
                }

            }
            else
            {
                index++;
            }
        }

        FileDB_Refresh(0);
    }

    debug_msg("\n\r UIFlowWndMovie_OnDeleteOld end \n\r");

}
#endif

