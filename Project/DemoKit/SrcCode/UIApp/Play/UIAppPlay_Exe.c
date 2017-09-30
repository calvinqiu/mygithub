#include "PlaybackTsk.h"
#include "UIAppPlay.h"
#include "UIBackgroundObj.h"
#include "NVTUserCommand.h"
#include "SysCfg.h" //for memory POOL_ID
#include "UIInfo.h"
#include "UIAppCommon.h"
#include "FileDB.h"
#include "GxDisplay.h"
#include "UIAppMovie.h"
#include "PrjCfg.h"
#include "BG_Images.h"
#include "DPOF.h"
#include "DpofVendor.h"
#include "PBXFileList_DCF.h"
#include "PBXFileList_FileDB.h"
//#NT#2013/05/08#Calvin Chang#Register decode video file callback function -begin
#include "MovieUdtaVendor.h"      // for movie udta (user data) parsing
//#NT#2013/05/08#Calvin Chang -end
#include "SysMain.h"

#if(WIFI_AP_FUNC==ENABLE)
#include "RtspNvtApi.h"
#include "WifiAppCmd.h"
#include "WifiAppXML.h"
#endif
//#NT#2016/07/21#Charlie Chang -begin
//#NT# using for get temp data
static MEM_RANGE g_stream_app_mem;
//#NT#2016/07/21#Charlie Chang -end
#include "ImageUnit_CamDisp.h"
#include "SMediaPlayAPI.h"
#include "ImageApp_CamPlay.h"
#include "dma.h"

#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UIAppPlay
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

extern UINT32 System_GetEnableDisp(void);
extern UINT32 System_GetEnableSensor(void);

//#NT#2010/09/10#Ben Wang -begin
//#NT#Refine code for sort by date only
//#NT#2010/08/09#Lincy Lin -begin
//#NT#Add index view for playback
#define FILEDB_SUPPORT_FILENUM   60000
//#NT#2010/08/09#Lincy Lin -end
#define PB_MAX_BUFFER_WIDTH     1920
#define PB_MAX_BUFFER_HEIGHT    1080
#define RTSP_PLAY_FUNC          DISABLE

#define MAX_REC_TIME  60//sec
//#NT#2017/04/10#Ben Wang -begin
//#NT#Add warning message to standard playback UI flow.
#if 1//(SDHOTPLUG_FUNCTION == ENABLE)
#include "UIStorageCheck.h"
static BOOL m_bIsPbOpened = FALSE;
#endif
//#NT#2017/04/10#Ben Wang -end
//#NT#2016/07/20#Charlie Chang -begin
//#NT# allocate tmp buf
static UINT32 PlayExe_GetTmpBuf(UINT32 uisize){

    if(uisize >= g_stream_app_mem.Size){
        DBG_ERR("not enough buf(%d)  now buf=%d\r\n",uisize,g_stream_app_mem.Size);
        return 0;
    }
    return (g_stream_app_mem.Addr);

}
//#NT#2016/07/20#Charlie Chang -end
//#NT#2013/05/08#Calvin Chang#Register decode video file callback function -begin
BOOL PBDecVideoCB(UINT32 uiUserDataAddr, UINT32 uiUserDataSize)
{
    return MovieUdta_ParseVendorUserData(uiUserDataAddr, uiUserDataSize);
}
//#NT#2013/05/08#Calvin Chang -end

/**
  Initialize application for Playback mode

  Initialize application for Playback mode.

  @param void
  @return void
*/
INT32 PlayExe_OnOpen(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    MEM_RANGE Pool;
#if(WIFI_AP_FUNC==ENABLE)
#if (XML_USE_APP_BUFFER==ENABLE)
    MEM_RANGE xmlPool;
#endif
#endif
    #if (USE_DPOF==ENABLE)
    MEM_RANGE dpofPool;
    #endif
    UINT32 useFileDB = 0;
    PLAY_OBJ PlayObj;

    //#NT#2017/04/10#Ben Wang -begin
	//#NT#Add warning message to standard playback UI flow.
	#if 1//(SDHOTPLUG_FUNCTION == ENABLE)
    if( TRUE == m_bIsPbOpened || UIStorageCheck(STORAGE_CHECK_ERROR, NULL) == TRUE)
    {
    	return NVTEVT_CONSUME;
    }
    m_bIsPbOpened = TRUE;
    #endif
    //#NT#2017/04/10#Ben Wang -end
    //call default
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_OPEN,paramNum,paramArray);

    #if (USE_DPOF==ENABLE)
    dpofPool = AppMem_Alloc("DPOF", DPOF_BUFFER_SIZE);
    #endif

    #if(WIFI_AP_FUNC==ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        #if (XML_USE_APP_BUFFER==ENABLE)
        xmlPool = AppMem_Alloc("XML", POOL_SIZE_TEMP);
        XML_SetTempMem(xmlPool.Addr,xmlPool.Size);
        #endif
    }
    #endif

    Pool = AppMem_Alloc("ImageStream", APPMEM_ALL);
    ImageApp_CamPlay_Config(PLAY_CFG_POOL, (UINT32)&Pool);

    #if USE_FILEDB
    UI_SetData(FL_IsUseFileDB, 1);
    #else
    UI_SetData(FL_IsUseFileDB, 0);
    #endif
    useFileDB = UI_GetData(FL_IsUseFileDB);
    if (useFileDB)
    {
        MEM_RANGE FDBMem;
        CHAR*      rootPath="A:\\";
        //CHAR*      defaultfolder="A:\\CarDV\\";

        PPBX_FLIST_OBJ  pFlist = PBXFList_FDB_getObject();
        FDBMem.Addr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_FILEDB));
        FDBMem.Size = POOL_SIZE_FILEDB;
        pFlist->Config(PBX_FLIST_CONFIG_MEM,FDBMem.Addr,FDBMem.Size);
        pFlist->Config(PBX_FLIST_CONFIG_MAX_FILENUM,10000,0);
        pFlist->Config(PBX_FLIST_CONFIG_MAX_FILEPATH_LEN,60,0);
        pFlist->Config(PBX_FLIST_CONFIG_VALID_FILETYPE,PBX_FLIST_FILE_TYPE_JPG|PBX_FLIST_FILE_TYPE_AVI|PBX_FLIST_FILE_TYPE_MOV|PBX_FLIST_FILE_TYPE_MP4|PBX_FLIST_FILE_TYPE_TS,0);
        pFlist->Config(PBX_FLIST_CONFIG_DCF_ONLY,FALSE,0);
        //pFlist->Config(PBX_FLIST_CONFIG_SORT_TYPE,PBX_FLIST_SORT_BY_FILEPATH,0);
        pFlist->Config(PBX_FLIST_CONFIG_SORT_TYPE,PBX_FLIST_SORT_BY_MODDATE,0);
        pFlist->Config(PBX_FLIST_CONFIG_ROOT_PATH,(UINT32)rootPath,0);
        pFlist->Config(PBX_FLIST_CONFIG_SUPPORT_LONGNAME,1,0);
        //pFlist->Config(PBX_FLIST_CONFIG_DEFAULT_FOLDER,(UINT32)defaultfolder,0);
        //#NT#2016/08/12#Hideo Lin -begin
        //#NT#For small size clone movie (filter clone movie for playback)
        #if (SMALL_CLONE_MOVIE == ENABLE)
        // Note: the folder name should match which defined in FlowMovie_FileDBCreate()
        pFlist->Config(PBX_FLIST_CONFIG_ROOT_PATH,(UINT32)DV_ROOT_PATH,0);
        pFlist->Config(PBX_FLIST_CONFIG_FILTER_FOLDER, (UINT32)PLAY_FOLDER_FILTER, 0);
        #endif
        //#NT#2016/08/12#Hideo Lin -end
        PB_SetParam(PBPRMID_FILELIST_OBJ, (UINT32)pFlist);
    }
    else
    {
        PB_SetParam(PBPRMID_FILELIST_OBJ, (UINT32)PBXFList_DCF_getObject());
    }
    PB_SetParam(PBPRMID_DEC_VIDEO_CALLBACK, (UINT32)PBDecVideoCB);

    if (System_GetEnableDisp() & DISPLAY_1)
    {
        ImageUnit_Begin(&ISF_CamDisp, 0);
            ImageUnit_CfgImgSize(ISF_IN1, 0, 0); //buffer size = full device size
            ImageUnit_CfgAspect(ISF_IN1, 0, 0);
            ImageUnit_CfgWindow(ISF_IN1, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();
    }
    if (System_GetEnableDisp() & DISPLAY_2)
    {
        ImageUnit_Begin(&ISF_CamDisp, 0);
            ImageUnit_CfgImgSize(ISF_IN2, 0, 0); //buffer size = full device size
            ImageUnit_CfgAspect(ISF_IN2, 0, 0);
            ImageUnit_CfgWindow(ISF_IN2, 0, 0, 0, 0);  //window range = full device range
        ImageUnit_End();
    }

    PB_SetParam(PBPRMID_MAX_PANELSZ, PB_MAX_BUFFER_WIDTH*PB_MAX_BUFFER_HEIGHT);

    //open
    ImageApp_CamPlay_Open();


    // init the Playback object
    memset(&PlayObj, 0, sizeof(PLAY_OBJ));
    PlayObj.uiMemoryAddr  = Pool.Addr;
    PlayObj.uiMemorySize  = Pool.Size;
    PlayObj.uiPlayFileFmt = PBFMT_JPG | PBFMT_WAV | PBFMT_AVI | PBFMT_MOVMJPG | PBFMT_MP4 | PBFMT_TS;

    // Open playback task
    if (PB_Open(&PlayObj) != E_OK)
    {
        DBG_ERR("Error open playback task\r\n");
    }
    //#NT#2016/07/20#Charlie Chang -begin
    //#NT# for PlayExe_GetTmpBuf function to get buf
    g_stream_app_mem.Addr = Pool.Addr;
    g_stream_app_mem.Size = Pool.Size;
    //#NT#2016/07/20#Charlie Chang -end
#if(WIFI_AP_FUNC==ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        #if (RTSP_PLAY_FUNC==ENABLE)
        RTSPNVT_OPEN Open={0};
        Open.uiApiVer = RTSPNVT_API_VERSION;
        Open.Type = RTSPNVT_TYPE_PLAYBACK;
        Open.uiPortNum = 554;
        //#NT#2016/07/13#Charlie Chang -begin
        //#NT# re-allocate rtsp addr and size
        Open.uiWorkAddr = Pool.Addr;
        Open.uiWorkSize = Pool.Size;
        //#NT#2016/07/13#Charlie Chang -end
        RtspNvt_Open(&Open);
        //#NT#2016/07/20#Charlie Chang -begin
        //#NT# for PlayExe_GetTmpBuf function to get buf
        RtspNvt_GetWorkBuf(&g_stream_app_mem);
        //#NT#2016/07/20#Charlie Chang -end


        #endif
        //WifiCmd_Done(WIFIFLAG_MODE_DONE,E_OK);
        Ux_PostEvent(NVTEVT_WIFI_EXE_MODE_DONE,1,E_OK);
    }
#endif

//#NT#2016/03/18#Niven Cho -begin
//#NT#improve the using of gprof
#if (_GPROF_TOOL_ == _GPROF_TOOL_ON_)
    //take the place of SxCmd Temp Buffer after ImageApp_Open, becasue it has changed in Open()
    SxCmd_RegTempMemFunc(System_GetGprofBuffer);
#else
    //#NT#2016/07/20#Charlie Chang -begin
    //#NT# using appmem to get tmp buf
    SxCmd_RegTempMemFunc(PlayExe_GetTmpBuf);
    //#NT#2016/07/20#Charlie Chang -end
#endif
//#NT#2016/03/18#Niven Cho -end
    return NVTEVT_CONSUME;
}

//#NT#2016/06/21#Jeah Yen -begin
//#NT#Support display fast switch
INT32 PlayExe_OnVideoChange(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    // change to current UI mode again
    Ux_SendEvent(0, NVTEVT_SYSTEM_MODE, 1, System_GetState(SYS_STATE_CURRMODE));
    return NVTEVT_CONSUME;

    //return NVTEVT_PASS; //PASS this event to UIWnd!
}
//#NT#2016/06/21#Jeah Yen -end

INT32 PlayExe_OnClose(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	//#NT#2017/04/10#Ben Wang -begin
	//#NT#Add warning message to standard playback UI flow.
	#if 1//(SDHOTPLUG_FUNCTION == ENABLE)
	if( FALSE == m_bIsPbOpened)
    {
    	return NVTEVT_CONSUME;
    }
    m_bIsPbOpened = FALSE;
    #endif
    //#NT#2017/04/10#Ben Wang -end
#if (WIFI_AP_FUNC==ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        #if (RTSP_PLAY_FUNC==ENABLE)
        RtspNvt_Close();
        #endif
    }
#endif
#if (USE_DPOF==ENABLE)
    DpofVendor_Close();
#endif

    //PB_Close(PB_WAIT_INFINITE);
    PB_Close(PB_WAIT_INFINITE);

    //WavStudio_Close();

    ImageApp_CamPlay_Close();
    //call default
    Ux_DefaultEvent(pCtrl,NVTEVT_EXE_CLOSE,paramNum,paramArray);

    return NVTEVT_CONSUME;
}

////////////////////////////////////////////////////////////

EVENT_ENTRY CustomPlayObjCmdMap[] =
{
    {NVTEVT_EXE_OPEN,               PlayExe_OnOpen            },
    {NVTEVT_EXE_CLOSE,              PlayExe_OnClose            },
    //#NT#2012/07/31#Hideo Lin -end
    //#NT#2016/06/21#Jeah Yen -begin
    //#NT#Support display fast switch
    {NVTEVT_VIDEO_CHANGE,           PlayExe_OnVideoChange     },
    //#NT#2016/06/21#Jeah Yen -end
    {NVTEVT_NULL,                   0},  //End of Command Map
};

CREATE_APP(CustomPlayObj,APP_SETUP)

