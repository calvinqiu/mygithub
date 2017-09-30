#include "SysCommon.h"
#include "FileSysTsk.h"
#include "FileDB.h"
#include "UIFramework.h"
#include "PrjCfg.h"
#include "UIInfo.h"
#include "UIAppPhoto.h"
#include "UIControl.h"
#include "UIControlWnd.h"
#include "uToken.h"
#include "NvtUctrlAPI.h"
#include "UCtrlMain.h"
#include "GxVideoFile.h"
#include "UCtrlAppCustom.h"
#include "SMediaRawEncAPI.h"
#include "MovieInterface.h"
#include "UIAppMovie.h"

#include "UCtrlAppEvent.h"
#include "GPS.h"
#include "GSensor.h"
#include "GyroDrv.h"
#if defined(YQCONFIG_DRIVE_EVENT_SUPPORT)
#include "DriveEvent.h"
#endif


#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlCustom
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
#include "DebugModule.h"


#define SXCMD_TEST          1

#define LINUX_STORAGE_ROOT   "/mnt/sd"

#if defined(YQCONFIG_EMMC_FLASH_SUPPORT)
#define UITRON_FS_ROOT_A      "A:\\"
#define UITRON_FS_ROOT_B      "B:\\"
#else
#define UITRON_FS_ROOT      "A:\\"
#endif

#define UCTRLCUSTOM_FLG_SNAPSHOT_DONE   FLGPTN_BIT(0)
#define UCTRLCUSTOM_FLG_REC_FINISH  	FLGPTN_BIT(1)

extern UINT32 _SECTION(".kercfg_data") UCTRLCUSTOM_FLG_ID;
UINT32 UCTRLCUSTOM_FLG_ID = 0;

static UINT32 m_uiUctrlCustomTempAddr = 0;
static UINT32 m_uiUctrlCustomTempSize = 0;
static UINT32 m_uiSnapShotResult = 0;
static FST_FILE          m_MovFilehdl = NULL;
static ER MovieReadCB(UINT32 pos, UINT32 size, UINT32 addr)
{
    ER result =E_SYS;

    DBG_IND("XML_VdoReadCB  pos=0x%x, size=%d, addr=0x%x\r\n",pos,size,addr);
    if(m_MovFilehdl)
    {
        FileSys_SeekFile(m_MovFilehdl, pos, FST_SEEK_SET);
        //not close file,close file in XML_GetThumbnail()
        result=FileSys_ReadFile(m_MovFilehdl, (UINT8 *)addr, &size, 0,0);
    }
    return result;
}
static INT32 Linux2NvtPath(const char* inPath, char* outPath, UINT32 outPathSize)
{

    //outPath+=sprintf(outPath,"A:");
#if defined(YQCONFIG_EMMC_FLASH_SUPPORT)
    if (strstr(inPath, "Front") != NULL)
    {
    	outPath+=snprintf(outPath,outPathSize,"A:");
    }
	else
	{
		outPath+=snprintf(outPath,outPathSize,"B:");
	}
#else
     outPath+=snprintf(outPath,outPathSize,"A:");
#endif
    outPathSize -= 2;
    //inPath+=strlen(LINUX_STORAGE_ROOT);
    while (*inPath !=0 && outPathSize>1)
    {
        if (*inPath == '/')
            *outPath = '\\';
        else
            *outPath = *inPath;
        inPath++;
        outPath++;
        outPathSize--;
    }
    //*outPath++ = '\\'; //If adding this, it will be regarded as folder.
    *outPath = 0;
    return 0;
}
static int Nvt2LinuxPath(const char* inPath, char* outPath, int outPathSize)
{
    char *temp;
#if defined(YQCONFIG_EMMC_FLASH_SUPPORT)
    temp = strstr(inPath, UITRON_FS_ROOT_A);
    if( 0 == temp)
    {
    	temp = strstr(inPath, UITRON_FS_ROOT_B);
    	if( 0 == temp)
    	{
	        DBG_ERR("Path ERROR(%s)\r\n",inPath);
	        return -1;
	    }
    }
    temp += strlen(UITRON_FS_ROOT_A);
#else
    temp = strstr(inPath, UITRON_FS_ROOT);
    if( 0 == temp)
    {
        DBG_ERR("Path ERROR(%s)\r\n",inPath);
        return -1;
    }
    temp += strlen(UITRON_FS_ROOT);
#endif
    //outPath+=sprintf(outPath,MOUNT_FS_ROOT);
    *outPath++ = '/';
    outPathSize--;
    while (*temp !=0 && outPathSize>1)
    {
        if (*temp == '\\')
            *outPath = '/';
        else
            *outPath = *temp;
        temp++;
        outPath++;
        outPathSize--;
    }
    *outPath = 0;
    return 0;
}
static BOOL CheckVideoFileType(char* pch)
{
	return ((strncmp(pch+1,"mp4",3)==0)||(strncmp(pch+1,"MP4",3)==0));
}
static BOOL CheckArgcArgv(INT32 argc, char* argv[])
{
	if (argc < 1 || argv[0] == NULL)
	{
        DBG_ERR("argc error  argc=%d\r\n",argc);
        return FALSE;
    }
    else
    	return TRUE;
}
static INT32 param_snapshot(INT32 argc, char* argv[])
{
	char RetStr[NVT_UCTRL_CMD_SIZE]={0};
	char RootPath[32];
	static int count=0;
	INT32 RetryCnt = 30;
	SMRAW_TRIGENC_PATH Trigger = SMRAW_FORMAT_PATH1;

	if(FALSE == CheckArgcArgv(argc, argv))
		return 0;

SNAPSHOT_RETRY:
	if(MovRec_IsRecording())
    {
    	UINT32 i;
    	FLGPTN FlgPtn;
    	FILEDB_INIT_OBJ WifiFDBInitObj={
                         RootPath,  //rootPath
                         NULL,  //defaultfolder
                         NULL,  //filterfolder
                         TRUE,  //bIsRecursive
                         TRUE,  //bIsCyclic
                         TRUE,  //bIsMoveToLastFile
                         TRUE, //bIsSupportLongName
                         FALSE, //bIsDCFFileOnly
                         TRUE,  //bIsFilterOutSameDCFNumFolder
                         {'N','V','T','I','M'}, //OurDCFDirName[5]
                         {'I','M','A','G'}, //OurDCFFileName[4]
                         FALSE,  //bIsFilterOutSameDCFNumFile
                         FALSE, //bIsChkHasFile
                         60,    //u32MaxFilePathLen
                         10000,  //u32MaxFileNum
                         (FILEDB_FMT_JPG),
                         0,     //u32MemAddr
                         0,     //u32MemSize
                         NULL   //fpChkAbort

		};
    	PFILEDB_INIT_OBJ        pFDBInitObj = &WifiFDBInitObj;
    	static FILEDB_HANDLE    FileDBHandle =0;
    	PFILEDB_FILE_ATTR        pFileAttr;


		if(*argv[0] == '0')
		{
			Trigger = SMRAW_FORMAT_PATH1;
        #if defined(YQCONFIG_EMMC_FLASH_SUPPORT)
			snprintf(RootPath,sizeof(RootPath), "A:\\CarDV\\Photo_Front\\");
        #else
			snprintf(RootPath,sizeof(RootPath), "A:\\CarDV\\Photo_Front\\");
        #endif
		}
		else
		{
			Trigger = SMRAW_FORMAT_PATH2;
        #if defined(YQCONFIG_EMMC_FLASH_SUPPORT)
			snprintf(RootPath,sizeof(RootPath), "B:\\CarDV\\Photo_Rear\\");
        #else
			snprintf(RootPath,sizeof(RootPath), "A:\\CarDV\\Photo_Rear\\");
        #endif
		}
	    DBG_IND("trigger path = %d\r\n",Trigger);
        if(MovRec_TriggerRawEncode(SMRAW_FORMAT_ONEPATH, Trigger))
        {
        	return 0;
        }
		wai_flg(&FlgPtn, UCTRLCUSTOM_FLG_ID, UCTRLCUSTOM_FLG_SNAPSHOT_DONE, TWF_ORW|TWF_CLR);
		pFDBInitObj->u32MemAddr = m_uiUctrlCustomTempAddr;

        if(!pFDBInitObj->u32MemAddr)
        {
            DBG_ERR("no temp buffer\r\n");
            return 0;
        }

        pFDBInitObj->u32MemSize = m_uiUctrlCustomTempSize;
        FileDBHandle = FileDB_Create(pFDBInitObj);
        //DBG_IND("createTime = %04d ms \r\n",Perf_GetDuration()/1000);
        //Perf_Mark();
        FileDB_SortBy(FileDBHandle,FILEDB_SORT_BY_CREDATE,TRUE);
        //DBG_IND("sortTime = %04d ms \r\n",Perf_GetDuration()/1000);
        //pFileAttr = FileDB_SearhFile(FileDBHandle, 0);
        UINT32 fileCount  = FileDB_GetTotalFileNum(FileDBHandle);
        if(fileCount)
        {
		#if 0
	        for(i=0;i<fileCount;i++)
	        {
	        	pFileAttr = FileDB_SearhFile(FileDBHandle, i);
	        	DBG_IND("[%02d]=%s\r\n",i,pFileAttr->filePath);
	       	}
		#endif
			pFileAttr = FileDB_SearhFile(FileDBHandle, 0);
			Nvt2LinuxPath(pFileAttr->filePath, RetStr, sizeof(RetStr));
		}
        FileDB_Release(FileDBHandle);
    }
    else
    {
    	 DBG_ERR("not in recording RetryCnt=%d\r\n", RetryCnt);
	#if 0
		 RetryCnt--;
		 if (RetryCnt > 0)
		 {
		 	SwTimer_DelayMs(100);
			goto SNAPSHOT_RETRY;
		 }
	#endif
    }

    DBG_IND("RetStr=%s\r\n",RetStr);

    NvtUctrl_SetRetString(RetStr);
    return 0;
}
static INT32 param_movielen(INT32 argc, char* argv[])
{
	char RetStr[NVT_UCTRL_CMD_SIZE]={0};
	char tempPath[NVT_UCTRL_CMD_SIZE];
	char* pch;
	UINT32   result;

	if(FALSE == CheckArgcArgv(argc, argv))
		return 0;

	Linux2NvtPath(argv[0], tempPath,sizeof(tempPath));
    pch=strchr(tempPath,'.');
    if(NULL == pch)
    {
    	DBG_ERR("path error %s\r\n",tempPath);
        return 0;
    }
	if(CheckVideoFileType(pch))
    {
        FST_FILE_STATUS FileStat={0};
        MEM_RANGE WorkBuf={0};
        GXVIDEO_INFO VideoInfo={0};


        m_MovFilehdl = FileSys_OpenFile(tempPath,FST_OPEN_READ);
        if(m_MovFilehdl)
        {
            FileSys_StatFile(m_MovFilehdl,&FileStat);
            //don't need TS format
            //GxVidFile_Query1stFrameWkBufSize(&uiBufferNeeded, FileStat.uiFileSize);  // 2016.09.22 unmark by Boyan : we have to call this function to update GxVideo's "guiGxVidTotalFileSize" so the TS format could calculate correct movie second.
            //parse video info for single mode only
            WorkBuf.Addr = m_uiUctrlCustomTempAddr;
            WorkBuf.Size = m_uiUctrlCustomTempSize;
            result = GxVidFile_ParseVideoInfo(MovieReadCB, &WorkBuf, (UINT32)FileStat.uiFileSize, &VideoInfo);
            if(result==GXVIDEO_PRSERR_OK)
            {
                snprintf(RetStr,sizeof(RetStr), "%d", VideoInfo.uiToltalSecs);
            }
            else
            {
                DBG_ERR("parse error\r\n");
            }

            FileSys_CloseFile(m_MovFilehdl);
        }
        else
        {
            DBG_ERR("no %s\r\n",tempPath);
        }

    }
    DBG_IND("RetStr=%s\r\n",RetStr);
    NvtUctrl_SetRetString(RetStr);
    return 0;
}
static INT32 param_movierec(INT32 argc, char* argv[])
{

	if(FALSE == CheckArgcArgv(argc, argv))
		return 0;

    if (System_GetState(SYS_STATE_CURRMODE) != PRIMARY_MODE_MOVIE)
    {
        DBG_ERR("not movie mode\r\n");
        return 0;
    }

    if('1' == *argv[0])//start record
    {
    	if(FALSE == MovRec_IsRecording())
    	{
	        //Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_REC_START, 0);
	        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
	    }
    }
    else  //stop rec
    {
    	if(MovRec_IsRecording())
    	{
	    	FLGPTN FlgPtn;
	        //Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_REC_STOP, 0);
	        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
	        wai_flg(&FlgPtn, UCTRLCUSTOM_FLG_ID, UCTRLCUSTOM_FLG_REC_FINISH, TWF_ORW|TWF_CLR);
        }
    }
    return 0;
}

#if (GPS_FUNCTION == ENABLE)
static INT32 param_gpsinfo(INT32 argc, char* argv[])
{
	char RetStr[NVT_UCTRL_CMD_SIZE]={0};

	RMCINFO stRMCInfo; 
	
	if (GPSRec_GetRMCDate(&stRMCInfo))
	{
		snprintf(RetStr,sizeof(RetStr), "{\"Latitude\":%f,\"Longitude\":%f,\"Speed\":%f,\"Angle\":%f}", 
			stRMCInfo.Latitude,stRMCInfo.Longitude,stRMCInfo.Speed,stRMCInfo.Angle);
	}
	else
	{
		snprintf(RetStr,sizeof(RetStr), "{\"Latitude\":%f,\"Longitude\":%f,\"Speed\":%f,\"Angle\":%f}", 
			0.0,0.0,0.0,0.0);
	}
    DBG_IND("RetStr=%s len=%d\r\n",RetStr,strlen(RetStr));

    NvtUctrl_SetRetString(RetStr);
    return 0;
}
#endif

#if defined(YQCONFIG_DRIVE_EVENT_SUPPORT)
static INT32 param_accinfo(INT32 argc, char* argv[])
{
	char RetStr[NVT_UCTRL_CMD_SIZE]={0};

	UINT32 uiKeyCode = 0;
    BOOL bGSensorStatus=TRUE;
    Gsensor_Data GS_Data = {0};
	float x, y, z;

	SMRAW_TRIGENC_PATH Trigger = SMRAW_FORMAT_PATH1;

	DriveEvent_Get_GSensor_Data(&x, &y, &z);

	snprintf(RetStr,sizeof(RetStr), "{\"accSpeedX\":%f,\"accSpeedY\":%f,\"accSpeedZ\":%f}", 
		x, y, z);
	
    DBG_IND("RetStr=%s\r\n",RetStr);

    NvtUctrl_SetRetString(RetStr);
    return 0;
}

static INT32 param_gyroinfo(INT32 argc, char* argv[])
{
	char RetStr[NVT_UCTRL_CMD_SIZE]={0};
	
	float x, y, z;
	
	SMRAW_TRIGENC_PATH Trigger = SMRAW_FORMAT_PATH1;

	if (DriveEvent_Get_GyroSensor_Data(&x, &y, &z))
	{
		snprintf(RetStr,sizeof(RetStr), "{\"gyroAnglePitch\":%f,\"gyroAngleRoll\":%f,\"gyroAngleYaw\":%f}",
			(float)x, (float)y, (float)z);
	}
	else
	{
		
		snprintf(RetStr,sizeof(RetStr), "{\"gyroAnglePitch\":%f,\"gyroAngleRoll\":%f,\"gyroAngleYaw\":%f}", 
			0.0, 0.0, 0.0);
	}

    DBG_IND("RetStr=%s\r\n",RetStr);

    NvtUctrl_SetRetString(RetStr);
    return 0;
}

static INT32 param_get_acc_msg_data(INT32 argc, char* argv[])
{
	char RetStr[NVT_UCTRL_CMD_SIZE]={0};
	EVENT_MSG_DATA stEventMsgData;
	
	INT32 x, y, z;
	
	SMRAW_TRIGENC_PATH Trigger = SMRAW_FORMAT_PATH1;

	if (DriveEvent_GetAccMsgData(&stEventMsgData))
	{
		snprintf(RetStr,sizeof(RetStr), "{\"begin_speed\":%f,\"end_speed\":%f,\"end_roll_angle\":%f,\"end_pitch_angle\":%f, \"duration\":%f}", 
			stEventMsgData.begin_speed, stEventMsgData.end_speed, stEventMsgData.end_roll_angle, stEventMsgData.end_pitch_angle, stEventMsgData.duration);
	}
	else
	{
		
		snprintf(RetStr,sizeof(RetStr), "{\"begin_speed\":%f,\"end_speed\":%f,\"end_roll_angle\":%f,\"end_pitch_angle\":%f, \"duration\":%f}", 
			0.0, 0.0, 0.0, 0.0, 0.0);
	}

    DBG_IND("RetStr=%s\r\n",RetStr);

    NvtUctrl_SetRetString(RetStr);
    return 0;
}

static INT32 param_get_acc_slow_msg_data(INT32 argc, char* argv[])
{
	char RetStr[NVT_UCTRL_CMD_SIZE]={0};
	EVENT_MSG_DATA stEventMsgData;
	
	INT32 x, y, z;
	
	SMRAW_TRIGENC_PATH Trigger = SMRAW_FORMAT_PATH1;

	if (DriveEvent_GetAccSlowMsgData(&stEventMsgData))
	{
		snprintf(RetStr,sizeof(RetStr), "{\"begin_speed\":%f,\"end_speed\":%f,\"end_roll_angle\":%f,\"end_pitch_angle\":%f, \"duration\":%f}", 
			stEventMsgData.begin_speed, stEventMsgData.end_speed, stEventMsgData.end_roll_angle, stEventMsgData.end_pitch_angle, stEventMsgData.duration);
	}
	else
	{
		
		snprintf(RetStr,sizeof(RetStr), "{\"begin_speed\":%f,\"end_speed\":%f,\"end_roll_angle\":%f,\"end_pitch_angle\":%f, \"duration\":%f}", 
			0.0, 0.0, 0.0, 0.0, 0.0);
	}

    DBG_IND("RetStr=%s\r\n",RetStr);

    NvtUctrl_SetRetString(RetStr);
    return 0;
}

static INT32 param_get_turn_msg_data(INT32 argc, char* argv[])
{
	char RetStr[NVT_UCTRL_CMD_SIZE]={0};
	EVENT_MSG_DATA stEventMsgData;
	
	INT32 x, y, z;
	
	SMRAW_TRIGENC_PATH Trigger = SMRAW_FORMAT_PATH1;

	if (DriveEvent_GetTurnMsgData(&stEventMsgData))
	{
		snprintf(RetStr,sizeof(RetStr), "{\"begin_speed\":%f,\"end_speed\":%f,\"end_roll_angle\":%f,\"end_pitch_angle\":%f, \"duration\":%f}", 
			stEventMsgData.begin_speed, stEventMsgData.end_speed, stEventMsgData.end_roll_angle, stEventMsgData.end_pitch_angle, stEventMsgData.duration);
	}
	else
	{
		
		snprintf(RetStr,sizeof(RetStr), "{\"begin_speed\":%f,\"end_speed\":%f,\"end_roll_angle\":%f,\"end_pitch_angle\":%f, \"duration\":%f}", 
			0.0, 0.0, 0.0, 0.0, 0.0);
	}

    DBG_IND("RetStr=%s\r\n",RetStr);

    NvtUctrl_SetRetString(RetStr);
    return 0;
}

static INT32 param_get_roll_over_msg_data(INT32 argc, char* argv[])
{
	char RetStr[NVT_UCTRL_CMD_SIZE]={0};
	EVENT_MSG_DATA stEventMsgData;
	
	INT32 x, y, z;
	
	SMRAW_TRIGENC_PATH Trigger = SMRAW_FORMAT_PATH1;

	if (DriveEvent_GetRollOverMsgData(&stEventMsgData))
	{
		snprintf(RetStr,sizeof(RetStr), "{\"begin_speed\":%f,\"end_speed\":%f,\"end_roll_angle\":%f,\"end_pitch_angle\":%f, \"duration\":%f}", 
			stEventMsgData.begin_speed, stEventMsgData.end_speed, stEventMsgData.end_roll_angle, stEventMsgData.end_pitch_angle, stEventMsgData.duration);
	}
	else
	{
		
		snprintf(RetStr,sizeof(RetStr), "{\"begin_speed\":%f,\"end_speed\":%f,\"end_roll_angle\":%f,\"end_pitch_angle\":%f, \"duration\":%f}", 
			0.0, 0.0, 0.0, 0.0, 0.0);
	}

    DBG_IND("RetStr=%s\r\n",RetStr);

    NvtUctrl_SetRetString(RetStr);
    return 0;
}

static INT32 param_get_lose_weight_data(INT32 argc, char* argv[])
{
	char RetStr[NVT_UCTRL_CMD_SIZE]={0};
	EVENT_MSG_DATA stEventMsgData;
	
	INT32 x, y, z;
	
	SMRAW_TRIGENC_PATH Trigger = SMRAW_FORMAT_PATH1;

	if (DriveEvent_GetLoseWeightMsgData(&stEventMsgData))
	{
		snprintf(RetStr,sizeof(RetStr), "{\"begin_speed\":%f,\"end_speed\":%f,\"end_roll_angle\":%f,\"end_pitch_angle\":%f, \"duration\":%f}", 
			stEventMsgData.begin_speed, stEventMsgData.end_speed, stEventMsgData.end_roll_angle, stEventMsgData.end_pitch_angle, stEventMsgData.duration);
	}
	else
	{
		
		snprintf(RetStr,sizeof(RetStr), "{\"begin_speed\":%f,\"end_speed\":%f,\"end_roll_angle\":%f,\"end_pitch_angle\":%f, \"duration\":%f}", 
			0.0, 0.0, 0.0, 0.0, 0.0);
	}

    DBG_IND("RetStr=%s\r\n",RetStr);

    NvtUctrl_SetRetString(RetStr);
    return 0;
}
#endif

static INT32 param_begin(char* str, INT32 len)
{
    DBG_IND("get: %s\r\n",str);
    return 0;
}

static INT32 param_end(UTOKEN_END_INFO* p_info)
{
    char retStr[64];
    DBG_IND("\r\n");
    if (p_info->err_code < 0)
    {
        //sprintf(retStr, "ERR: parm %s", p_info->failed_name);
        snprintf(retStr, sizeof(retStr), "ERR: parm %s", p_info->failed_name);
        NvtUctrl_SetRetString(retStr);
        DBG_ERR("%s\r\n",retStr);
        return p_info->err_code;
    }
    return 0;
}

static INT32 param_unknown(char* name)
{
    DBG_ERR("unknown param: %s\r\n",name);
    return 0;
}

UTOKEN_PARAM tbl_custom_param[] = {
	{"snapshot",param_snapshot},
	{"movielen",param_movielen},
	{"movierec",param_movierec},
#if (GPS_FUNCTION == ENABLE)
	{"gpsinfo",param_gpsinfo},
#endif     
#if defined(YQCONFIG_DRIVE_EVENT_SUPPORT)
	{"accinfo",param_accinfo},
	{"gyroinfo",param_gyroinfo},
	{"accmsg",param_get_acc_msg_data},
	{"accslowmsg",param_get_acc_slow_msg_data},
	{"turnmsg",param_get_turn_msg_data},
	{"rollmsg",param_get_roll_over_msg_data},
	{"loseweightmsg",param_get_lose_weight_data},
#endif
    {NULL,NULL}, //last tag, it must be
};

UTOKEN_MODULE module_custom = {
    "ucustom",
    tbl_custom_param,
    param_begin,
    param_end,
    param_unknown,
    NULL
};

#if SXCMD_TEST
static BOOL Cmd_ucustom_set(CHAR* strCmd)
{
    uToken_Parse(strCmd,&module_custom);
    return TRUE;
}

SXCMD_BEGIN(ucustom, "ucustom")
SXCMD_ITEM("set %", Cmd_ucustom_set, "")
SXCMD_END()
#endif

void UCtrlAppCustom_InstallID(void)
{
	OS_CONFIG_FLAG(UCTRLCUSTOM_FLG_ID);
}
void UCtrlAppCustom_InstallCmd(void)
{
#if SXCMD_TEST
    SxCmd_AddTable(ucustom);
#endif
}

void UCtrlAppCustom_SetData(UCTRL_CUSTOM_CONFIG_ID Config, UINT32 Value)
{
    switch(Config)
    {
    	case UCTRL_CUSTOM_BUFFER_ADDR:
    		m_uiUctrlCustomTempAddr = Value;
    	break;
    	case UCTRL_CUSTOM_BUFFER_SIZE:
    		m_uiUctrlCustomTempSize = Value;
    	break;
    	case UCTRL_CUSTOM_SNAPSHOT_DONE:
    		m_uiSnapShotResult = Value;
    		set_flg(UCTRLCUSTOM_FLG_ID, UCTRLCUSTOM_FLG_SNAPSHOT_DONE);
    	break;
    	case UCTRL_CUSTOM_REC_FINISH:
    		set_flg(UCTRLCUSTOM_FLG_ID, UCTRLCUSTOM_FLG_REC_FINISH);
    	break;
    	default:
    		DBG_ERR("param error\r\n");
    	break;
    }
}

