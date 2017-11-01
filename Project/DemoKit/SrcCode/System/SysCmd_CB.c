/*
    System Command Callback

    System Callback for Command-Line Console Module.

    @file       SysCmdCB.c
    @ingroup    mIPRJSYS
    @note       None

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

#include "SysCommon.h"
#include "SysMain.h"
#include "GxSystem.h"
#include "PrjCfg.h"
#include "ProjectInfo.h"
//#include "KerVerInfo.h"
//#include "Driver.h"
#include "SysCfg.h"
#include "AppVerInfo.h"
#include "UIModeMain.h"
#if(PHOTO_MODE==ENABLE)
#include "UIModePhoto.h"
#endif
#include "UIModeMovie.h"
#if(PLAY_MODE==ENABLE)
#include "UIModePlayback.h"
#endif
//#NT#2012/02/15#Lincy Lin -begin
//#NT#
#include "fs_file_op.h"
#include "FileSysTsk.h"
#include "DCF.h"

#include "UIFramework.h"
#include "NVTUserCommand.h"

#include "ImageStream.h"

#include "AppLib.h"
#include "UIAppCommon.h"
#include "Lens.h"
#include "LensCtrlTsk.h"
#include "af_api.h"
#include "PStore.h"
#include "ae_task.h"
#include "awb_task.h"
#include "UICommon.h"
#include "Dx.h"
#include "NvtVerInfo.h"
#include "UIAppMovie.h"  // Isiah, implement YUV merge mode of recording func.
#include "UIAppPhoto.h"
#include "UISetup.h"
#include "UIPhotoMapping.h"
#include "UIMovieMapping.h"
#include "WifiAppCmd.h"
//#NT#2017/03/02#Niven Cho -begin
//#NT#cmd to set target bitrate
#include "MovieInterface.h"
//#NT#2017/03/02#Niven Cho -end
#if(WIFI_AP_FUNC==ENABLE)
#include "UIAppNetwork.h"
#include "DhcpNvt.h"
#include "DxWiFi.h"
#endif
#if (PIP_VIEW_LR == ENABLE)
#include "AppDisp_PipView.h"
#endif
//#NT#2016/05/20#Lincy Lin -begin
//#NT#Support NTP client
#include "rtc.h"
#include "NvtIpcAPI.h"
//#NT#2016/05/20#Lincy Lin -end
#if(FACE_GRADE_TEST==ENABLE)
#include "FaceGIpcAPI.h"
#endif
//#NT#2016/12/02#Niven Cho -begin
//#NT#AUTO_TEST_CMD
#include "MsdcNvtCb_Exam.h"
#include "ImageUnit_CamFile.h"
#include "gpio.h"
//#NT#2016/12/02#Niven Cho -end
CHAR    FWFileName[] = "A:\\FW96650A.bin";
CHAR    LDFileName[] = "A:\\LD96650A.bin";

//#NT#2012/02/15#Lincy Lin -end

//---------------------SysCmdCB Debug Definition -----------------------------
//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysCmdCB
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

//---------------------SysCmdCB Global Variables -----------------------------
//---------------------SysCmdCB Prototype Declaration  -----------------------
//---------------------SysCmdCB Public API  ----------------------------------
//---------------------SysCmdCB Private API ----------------------------------

///////////////////////////////////////////////////////////////////////////////
// temp memory for user cmd
///////////////////////////////////////////////////////////////////////////////

//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
extern BOOL gb3DNROut;
#endif
//#NT#2016/09/20#Bob Huang -end


UINT32 User_GetTempBuffer(UINT32 uiSize)
{
    void *pBuf = 0;
    //#NT#2016/05/12#Jeah Yen -begin
    //#NT#return max size
    if(uiSize == TEMPBUF_GET_MAXSIZE)
        return POOL_SIZE_APP; //return max size
    //#NT#2016/05/12#Jeah Yen -end
    if(uiSize <= POOL_SIZE_APP)
    {
        pBuf = (void*)OS_GetMempoolAddr(POOL_ID_APP);
    }
    if(pBuf == NULL)
    {
        DBG_ERR("get buffer fail\r\n");
    }
    return (UINT32)pBuf;
}

//#NT#2016/03/18#Niven Cho -begin
//#NT#improve the using of gprof
#if (_GPROF_TOOL_ == _GPROF_TOOL_ON_)
_ALIGNED(4) static UINT8 m_gProfBuf[0x100000];
UINT32 System_GetGprofBuffer(UINT32 uiSize)
{
    void *pBuf = 0;
    //#NT#2016/05/12#Jeah Yen -begin
    //#NT#return max size
    if(uiSize == TEMPBUF_GET_MAXSIZE)
        return 0x100000; //return max size
    //#NT#2016/05/12#Jeah Yen -end
    if(uiSize <= sizeof(m_gProfBuf))
    {
        pBuf = (void*)m_gProfBuf;
    }
    if(pBuf == NULL)
    {
        DBG_ERR("get gprof buffer fail\r\n");
    }
    return (UINT32)pBuf;
}
#endif
//#NT#2016/03/18#Niven Cho -end

//#NT#2016/05/12#Jeah Yen -begin
//#NT#add tprof start
///////////////////////////////////////////////////////////////////////////////
// profile process
///////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "PStore.h"
#include "FileSysTsk.h"
//#include "DxPower.h"

CHAR sys_usage_str[17]={0};
CHAR sys_usage_str2[17]={0};
int sys_dump_cnt = 0;
BOOL sys_keeptop = FALSE;
BOOL sys_dotrace = FALSE;

void sys_usage_dump(BOOL bTrace)
{
    //fix for CID 62610,62611 - begin
    UINT32 dma_usage = 0;
    UINT32 cpu_usage = 0;
    //fix for CID 62610,62611 - end
    UINT32 mem_free, mem_total;
    UINT32 stg_pst_free, stg_pst_total;
    UINT64 stg_fat_free, stg_fat_total;
    //UINT32 pwr_lvl_usage;
    //UINT32 pwr_vot_usage;
    //UINT32 pwr_tmp_usage;
    profile_get(PROFILE_DMA,&dma_usage);if(dma_usage>100)dma_usage=0;
    profile_get(PROFILE_CPU,&cpu_usage);if(cpu_usage>100)cpu_usage=0;
    mem_free = SxCmd_GetTempMem(TEMPBUF_GET_MAXSIZE);mem_total = OS_GetMemSize(MEM_TOTAL);
    stg_pst_free = PStore_GetInfo(PS_INFO_FREE_SPACE);stg_pst_total = PStore_GetInfo(PS_INFO_TOT_SIZE);
    stg_fat_free = FileSys_GetDiskInfoEx('A', FST_INFO_FREE_SPACE);stg_fat_total = FileSys_GetDiskInfoEx('A', FST_INFO_DISK_SIZE);
    //pwr_lvl_usage = DrvPower_GetControl(DRVPWR_CTRL_BATTERY_LEVEL); //level
    //if(pwr_lvl_usage>4)pwr_lvl_usage=0;
    //pwr_vot_usage = DrvPower_GetControl(DRVPWR_CTRL_BATTERY_ADC_VALUE); //votiage
    //pwr_tmp_usage = DrvPower_GetControl(DRVPWR_CTRL_BATTERY_TEMPERATURE); //temparature
    if(!bTrace)
    {
        profile_msg("resource |  total |  usage\r\n");
        profile_msg("=================================\r\n");
        profile_msg("     DMA |  %5u |  %5u (%%)\r\n", 100, dma_usage);
        profile_msg("     CPU |  %5u |  %5u (%%)\r\n", 100, cpu_usage);
        profile_msg("     MEM |  %5u |  %5u (MB)\r\n", (mem_total)/1024/1024, (mem_total-mem_free)/1024/1024);
        profile_msg("   INMEM |  %5u |  %5u (MB)\r\n", (stg_pst_total)/1024/1024, (stg_pst_total-stg_pst_free)/1024/1024);
        profile_msg("   EXMEM |  %5u |  %5u (MB)\r\n", (UINT32)((stg_fat_total)/1024/1024), (UINT32)((stg_fat_total-stg_fat_free)/1024/1024));
        //profile_msg("     PWR |  %5u |  %5u\r\n", 4, pwr_lvl_usage);
        //profile_msg("PWR_VOT: %u/%u\r\n", pwr_vot_usage, 1000);
        //profile_msg("PWR_TMP: %u/%u\r\n", pwr_tmp_usage, 100);
    }
    else
    {
        if(sys_dump_cnt==0)
        {
        profile_msg("\r\n");
        profile_msg("             DMA             CPU             MEM           INMEM           EXMEM\r\n");
        profile_msg("================================================================================\r\n");
        }
        else
        {
        snprintf(sys_usage_str, sizeof(sys_usage_str), "%u/%u", dma_usage, 100);profile_msg("%16s", sys_usage_str);
        snprintf(sys_usage_str, sizeof(sys_usage_str), "%u/%u", cpu_usage, 100);profile_msg("%16s", sys_usage_str);
        snprintf(sys_usage_str, sizeof(sys_usage_str), "%u/%u", (mem_total-mem_free)/1024/1024, (mem_total)/1024/1024);profile_msg("%16s", sys_usage_str);
        snprintf(sys_usage_str, sizeof(sys_usage_str), "%u/%u", (stg_pst_total-stg_pst_free)/1024/1024, (stg_pst_total)/1024/1024);profile_msg("%16s", sys_usage_str);
        snprintf(sys_usage_str, sizeof(sys_usage_str), "%u/%u", (UINT32)((stg_fat_total-stg_fat_free)/1024/1024), (UINT32)((stg_fat_total)/1024/1024));profile_msg("%16s\r\n", sys_usage_str);
        }
    }
}

void sys_profile_process(void)
{
    //fix for CID 62612 - begin
    UINT32 mode = 0;
    //fix for CID 62612 - end
    profile_get(PROFILE_MODE,&mode);
    switch(mode)
    {
    case 1: //"sys task" or "sys task top"
        {
            if(sys_keeptop) //"sys task top"
            {
                profile_msg("\x1B[2J\x1B[H"); //cls
                profile_cmd(PROFILE_TASK,TRUE,0);
            }
            else //"sys task"
            {
                if(sys_dump_cnt>9)
                {
                profile_msg("\r\n");
                profile_cmd(PROFILE_TASK,TRUE,0);
                SxCmd_DoCommand("tprof stop");
                }
                else
                {
                profile_cmd(PROFILE_TASK,FALSE,0);
                profile_msg(".");
                }
            }
        }
        sys_dump_cnt++;
        break;
    case 2: //"sys cpu" or "sys cpu top" or "sys cpu trace"
        if(!sys_dotrace) //"sys cpu" or "sys cpu top"
        {
            if(sys_keeptop) //"sys cpu top"
            {
                profile_msg("\x1B[2J\x1B[H"); //cls
                profile_cmd(PROFILE_CPU,TRUE,0);
            }
            else //"sys cpu"
            {
                if(sys_dump_cnt>9)
                {
                profile_msg("\r\n");
                profile_cmd(PROFILE_CPU,TRUE,0);
                SxCmd_DoCommand("tprof stop");
                }
                else
                {
                profile_cmd(PROFILE_CPU,FALSE,0);
                profile_msg(".");
                }
            }
        }
        else //"sys cpu trace"
        {
                profile_cmd(PROFILE_CPU,TRUE,0);
        }
        sys_dump_cnt++;
        break;
    case 3: //"sys usage" or "sys usage top" or "sys usage trace"
        if(!sys_dotrace) //"sys usage" or "sys usage top"
        {
            if(sys_keeptop) //"sys cpu top"
            {
                profile_msg("\x1B[2J\x1B[H"); //cls
                sys_usage_dump(FALSE);
            }
            else //"sys cpu"
            {
                if(sys_dump_cnt>0)
                {
                profile_msg("\r\n");
                sys_usage_dump(FALSE);
                SxCmd_DoCommand("tprof stop");
                }
                else
                {
                profile_msg(".");
                }
            }
        }
        else //"sys usage trace"
        {
            sys_usage_dump(TRUE);
        }
        sys_dump_cnt++;
    break;
    }
}
//#NT#2016/05/12#Jeah Yen -end


///////////////////////////////////////////////////////////////////////////////
// user cmd
///////////////////////////////////////////////////////////////////////////////
extern UINT32 BeepPrd;
static BOOL Cmd_user_cmd(CHAR* strCmd)
{
    DBG_DUMP("this is sample cmd!\r\n");
	
UINT32 menuIdx, value;

    sscanf_s(strCmd,"%d", &value);
    debug_msg("this is sample cmd! %d\r\n",value);
//	g_LCDBacklightPWMInfo.uiPrd=value;
	BeepPrd = value;
	//GPIOMap_TurnOnLCDBeep
    return TRUE;
}

static BOOL Cmd_user_dump_pool(CHAR* strCmd)
{
    UINT32 i;
    DBG_DUMP("dump pool status\r\n");

    debug_msg("POOL_ID_STORAGE_NAND:%d- start addr:0x%08lX, size:0x%08lX\r\n", POOL_ID_STORAGE_NAND, OS_GetMempoolAddr(POOL_ID_STORAGE_NAND), OS_GetMempoolSize(POOL_ID_STORAGE_NAND));
    debug_msg("POOL_ID_APP:%d- start addr:0x%08lX, size:0x%08lX\r\n", POOL_ID_APP, OS_GetMempoolAddr(POOL_ID_APP), OS_GetMempoolSize(POOL_ID_APP));

    debug_msg("------------------------------------------------\r\n");
    for(i = 1; i <= POOL_ID_APP; i++)
    {
        debug_msg("POOL_ID_%d- start addr:0x%08lX, size:0x%08lX\r\n", i, OS_GetMempoolAddr(i), OS_GetMempoolSize(i));
    }

    return TRUE;
}

static BOOL Cmd_user_set_tbr(CHAR* strCmd)
{
    UINT32 pathID, value;

    if (sscanf_s(strCmd,"%d %d", &pathID, &value) == 2) {
		debug_msg("set path[%d] target bitrate = %d\r\n", pathID, value);
		MovRec_SetVidTBR(pathID, value);
    }
    return TRUE;
}

static BOOL Cmd_movie(CHAR* strCmd)
{
    UINT32 menuIdx, value;

    sscanf_s(strCmd,"%d %d", &menuIdx, &value);

    if (menuIdx ==0)
    {
#if (MOVIE_REC_YUVMERGE == ENABLE)
        BOOL bEnable;
        UINT32 uiInterval;
        DBG_DUMP("Test merge record\r\n");
        sscanf_s(strCmd,"%d %d %d", &menuIdx, &bEnable, &uiInterval);
        FlowMovie_RecSetYUVMergeRecInterval(uiInterval);
        FlowMovie_RecSetYUVMergeRecCounter(0);
        FlowMovie_RecSetYUVMergeMode(bEnable);
        if(bEnable)
            debug_msg("Enable YUV merge record mode, interval=%d\r\n", uiInterval);
        else
            debug_msg("Disable YUV merge record mode\r\n");
#endif
    }
    else if (menuIdx ==1)
    {
        DBG_DUMP("ADAS(LDWS+FCW) %d\r\n",value);
        Ux_SendEvent(&CustomMovieObjCtrl,   NVTEVT_EXE_MOVIE_LDWS,          1,  value);
        Ux_SendEvent(&CustomMovieObjCtrl,   NVTEVT_EXE_MOVIE_FCW,           1,  value);
    }
    else if (menuIdx ==2)
    {
        DBG_DUMP("Urgent Protect Manual %d\r\n",value);
        Ux_SendEvent(&CustomMovieObjCtrl,   NVTEVT_EXE_MOVIE_PROTECT_MANUAL,1,  value);
    }
    else if (menuIdx ==3)
    {
        DBG_DUMP("Image Rotation %d\r\n",value);
        Ux_SendEvent(&CustomMovieObjCtrl,   NVTEVT_EXE_MOVIE_SENSOR_ROTATE,1,  value);
    }
    else if (menuIdx ==5)
    {
        DBG_DUMP("Movie RSC %d\r\n",value);
        Ux_SendEvent(&CustomMovieObjCtrl,   NVTEVT_EXE_MOVIE_RSC,          1,  value);
    }
    else if (menuIdx ==7)
    {
        UI_SetData(FL_DUAL_CAM, value);
        DBG_DUMP("Sensor index %d\r\n",value);
        Ux_SendEvent(&CustomMovieObjCtrl,   NVTEVT_EXE_DUALCAM,      1, SysGetFlag(FL_DUAL_CAM));
    }
    else if (menuIdx ==11)
    {
        DBG_DUMP("Movie WDR index %d\r\n",value);
        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_MOVIE_WDR, 1, value);
    }
    else if (menuIdx ==13)
    {
#if (PIP_VIEW_LR == ENABLE)
        DBG_DUMP("enter value=%d\r\n",value);
        if(value>=DUALCAM_LR_16_9 && value<=DUALCAM_LR_BEHIND_FULL )
        {
        }
        else
        {
            value=DUALCAM_LR_16_9;
        }
        DBG_DUMP("out value=%d\r\n",value);
        UI_SetData(FL_DUAL_CAM, value);

        Ux_SendEvent(&CustomMovieObjCtrl, NVTEVT_EXE_IMAGE_RATIO,1, GetMovieSizeRatio(UI_GetData(FL_MOVIE_SIZE)));
        PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
#endif
    }

    return TRUE;
}


BOOL Cmd_photo(CHAR* strCmd)
{
#if (PHOTO_MODE==ENABLE)
    UINT32 menuIdx, value;

    sscanf_s(strCmd,"%d %d", &menuIdx, &value);

    if (menuIdx ==0)
    {
        DBG_DUMP("FD En %d\r\n",value);
        UI_SetData(FL_FD, value);
        Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_FD,          1,  SysGetFlag(FL_FD));

    }
    else if (menuIdx ==1)
    {
        DBG_DUMP("ContSot En %d\r\n",value);
        UI_SetData(FL_CONTINUE_SHOT, value);
    }
    else if (menuIdx ==2)
    {
        /*
        CONT_SHOT_OFF = 0,
        CONT_SHOT_INFINITE,
        CONT_SHOT_10,
        CONT_SHOT_3,
        */
        DBG_DUMP("ContSot index %d\r\n",value);
        //UI_SetData(FL_ContShotIndex, value);
    }
    else if (menuIdx ==3)
    {
        extern BOOL isS2Pressed;
        isS2Pressed = value;
        DBG_DUMP("isS2Pressed =%d\r\n",isS2Pressed);
    }
    else if (menuIdx ==4)
    {
        DBG_DUMP("ISO index %d\r\n",value);
        UI_SetData(FL_ISO, value);
        Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_ISO,          1,  SysGetFlag(FL_ISO));
    }
    else if (menuIdx ==5)
    {
        UI_SetData(FL_RSC, value);
        if (value == 1)
            DBG_DUMP("RSC on\r\n");
        else
            DBG_DUMP("RSC off\r\n");
        Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_RSC,          1,  SysGetFlag(FL_RSC));
    }
    // rsc onoff robust test
    else if (menuIdx ==6)
    {
        UINT32 i,delayMs;
        #define RANDOM_RANGE(n)     (randomUINT32() % (n))

        DBG_DUMP("loop %d\r\n",value);
        for (i=0;i<value;i++)
        {
            Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_RSC,      1,  1);
            delayMs = RANDOM_RANGE(3000);
            DBG_DUMP("B delayMs = %d\r\n",delayMs);
            SwTimer_DelayMs(delayMs);
            DBG_DUMP("E delayMs = %d\r\n",delayMs);
            Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_RSC,      1,  0);
            SwTimer_DelayMs(1000);
        }
    }
    else if (menuIdx ==7)
    {
        UI_SetData(FL_DUAL_CAM, value);
        DBG_DUMP("Sensor index %d\r\n",value);
        Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_DUALCAM,      1, SysGetFlag(FL_DUAL_CAM));
    }
    else if (menuIdx ==8)
    {
        #if 0
        UI_SetData(FL_CaptureIndex, value);
        DBG_DUMP("Capture index %d\r\n",value);
        Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_CAPTURE_ID,          1, SysGetFlag(FL_CaptureIndex));
        #endif
    }
    else if (menuIdx ==9)
    {
        UI_SetData(FL_SCENE, value);
        DBG_DUMP("scene index %d\r\n",value);
        Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_SCENEMODE,          1,  SysGetFlag(FL_SCENE));
    }
    else if (menuIdx ==10)
    {
        UI_SetData(FL_SHDR, value);
        DBG_DUMP("SHDR index %d\r\n",value);
        Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_SHDR,          1,  SysGetFlag(FL_SHDR));
    }
    else if (menuIdx ==11)
    {
        UI_SetData(FL_WDR, value);
        DBG_DUMP("WDR index %d\r\n",value);
        Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_WDR,          1,  SysGetFlag(FL_WDR));
    }
    else if (menuIdx ==12)
    {
        UI_SetData(FL_ANTISHAKE, value);
        DBG_DUMP("ANTISHAKE index %d\r\n",value);
        Ux_SendEvent(&CustomPhotoObjCtrl,   NVTEVT_EXE_ANTISHAKING,    1,  SysGetFlag(FL_ANTISHAKE));
    }
    else if (menuIdx ==13)
    {
#if (PIP_VIEW_LR == ENABLE)
        DBG_DUMP("enter value=%d\r\n",value);
        if(value>=DUALCAM_LR_16_9 && value<=DUALCAM_LR_BEHIND_FULL )
        {
        }
        else
        {
            value=DUALCAM_LR_16_9;
        }
        DBG_DUMP("out value=%d\r\n",value);
        UI_SetData(FL_DUAL_CAM, value);

        Ux_SendEvent(&CustomPhotoObjCtrl,NVTEVT_EXE_IMAGE_RATIO,1,GetPhotoSizeRatio(UI_GetData(FL_PHOTO_SIZE)));

        PipView_SetStyle(UI_GetData(FL_DUAL_CAM));
#endif
    }
#endif
    return TRUE;
}
//#NT#2017/04/12#Ben Wang -begin
//#NT#Add suspend/resuem test command.
#include "time.h"
#include "HwClock.h"
static BOOL Cmd_user_suspend(CHAR* strCmd)
{
	struct tm Curr_DateTime;
	struct tm* pAlarmTime = 0;
	time_t c_dt = -1;
	UINT32 suspendtime = 0;
    sscanf_s(strCmd,"%d", &suspendtime);

	if(suspendtime)
	{
	    Curr_DateTime= HwClock_GetTime(TIME_ID_CURRENT);
	    DBG_DUMP("Curr Time %ld/%ld/%ld ",Curr_DateTime.tm_year,Curr_DateTime.tm_mon,Curr_DateTime.tm_mday);
	    DBG_DUMP("%ld:%ld:%ld\r\n",Curr_DateTime.tm_hour,Curr_DateTime.tm_min,Curr_DateTime.tm_sec);

		Curr_DateTime.tm_year -= 1900;
		Curr_DateTime.tm_mon -= 1;
		c_dt = mktime(&Curr_DateTime);
		c_dt += suspendtime;

		pAlarmTime = localtime(&c_dt);
		if(pAlarmTime)
		{
			pAlarmTime->tm_year+=1900;
			pAlarmTime->tm_mon+=1;
			Curr_DateTime = *pAlarmTime;
		}
		DBG_DUMP("Alarm Time %ld/%ld/%ld ",Curr_DateTime.tm_year,Curr_DateTime.tm_mon,Curr_DateTime.tm_mday);
	    DBG_DUMP("%ld:%ld:%ld\r\n",Curr_DateTime.tm_hour,Curr_DateTime.tm_min,Curr_DateTime.tm_sec);

		HwClock_SetTime(TIME_ID_ALARM, Curr_DateTime, 0);
	}
	Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_SLEEP);
    return TRUE;
}
//#NT#2017/04/12#Ben Wang -end
#include "SMediaRecAPI.h"
extern void     SMediaRecUti_SetResult(UINT32 result);
BOOL Cmd_post_card_write_error(CHAR* strCmd)
{
    SMediaRecUti_SetResult(SMEDIAREC_EVENT_RESULT_WRITE_ERR);
    DBG_DUMP("Post MEDIAREC_EVENT_RESULT_WRITE_ERR\r\n");
    return TRUE;
}

BOOL Cmd_post_slow_card_error(CHAR* strCmd)
{
    SMediaRecUti_SetResult(SMEDIAREC_EVENT_RESULT_SLOW);
    DBG_DUMP("Post MEDIAREC_EVENT_RESULT_SLOW\r\n");
    return TRUE;
}

BOOL Cmd_post_card_full_error(CHAR* strCmd)
{
    SMediaRecUti_SetResult(SMEDIAREC_EVENT_RESULT_FULL);
    DBG_DUMP("Post MEDIAREC_EVENT_RESULT_FULL\r\n");
    return TRUE;
}
//#NT#2016/03/03#Ben Wang -begin
//#NT#Add debug message for script test.
BOOL Cmd_dbg(CHAR* strCmd)
{
    UINT32 ShowMsg = 0;
    sscanf_s(strCmd,"%d", &ShowMsg);
    g_bDbgTest = ShowMsg;
    return TRUE;
}
//#NT#2016/04/20#Jeah Yen -begin
//#NT#move "reset" cmd to "user" table
BOOL Cmd_reset(CHAR* strCmd)
{
    Ux_SendEvent(0,NVTEVT_EXE_SYSRESET,0);
    return TRUE;
}
//#NT#2016/04/20#Jeah Yen -end

BOOL Cmd_face_grade(CHAR* strCmd)
{
#if(FACE_GRADE_TEST==ENABLE)
    FaceGIpc_Open(OS_GetMempoolAddr(POOL_ID_FACEGIPC), POOL_SIZE_FACEGIPC);
#else
    DBG_ERR("FACE_GRADE_TEST is not turned on\r\n");
#endif
    return TRUE;
}

SXCMD_BEGIN(user, "user command")
SXCMD_ITEM("cmd %", Cmd_user_cmd, "sample code")
SXCMD_ITEM("dump_pool", Cmd_user_dump_pool, "dump pool")
SXCMD_ITEM("movie %", Cmd_movie, "movie mode test")
SXCMD_ITEM("photo %", Cmd_photo, "photo mode setting")
SXCMD_ITEM("write_err", Cmd_post_card_write_error, "write_error")
SXCMD_ITEM("slow_card", Cmd_post_slow_card_error, "slow_card")
SXCMD_ITEM("card_full", Cmd_post_card_full_error, "card_full")
SXCMD_ITEM("dbg %", Cmd_dbg, "show/hide DBG_TEST() message")
//#NT#2016/04/20#Jeah Yen -begin
//#NT#move "reset" cmd to "user" table
SXCMD_ITEM("reset", Cmd_reset,"reset to default setting")
//#NT#2016/04/20#Jeah Yen -end
SXCMD_ITEM("face_grade", Cmd_face_grade,"start face grade task")
//#NT#2017/03/02#Niven Cho -begin
//#NT#cmd to set target bitrate
SXCMD_ITEM("tbr %", Cmd_user_set_tbr, "set target bitrate")
//#NT#2017/03/02#Niven Cho -end
//#NT#2017/04/12#Ben Wang -begin
//#NT#Add suspend/resuem test command.
SXCMD_ITEM("suspend %", Cmd_user_suspend, "let device suspend n sec")
//#NT#2017/04/12#Ben Wang -end
SXCMD_END()
//#NT#2016/03/03#Ben Wang -end

///////////////////////////////////////////////////////////////////////////////
// sys cmd
///////////////////////////////////////////////////////////////////////////////

static BOOL Cmd_sys_mem(CHAR* strCmd)
{
#if 1
    MEM_RANGE* freeMem;
    char str1[32];

    sscanf_s(strCmd,"%s", str1, sizeof(str1)); //fix for CID 44998

    if(strncmp(str1, "su", 31)==0) //sort by unit
    {
        DBG_DUMP("[System]\r\n");
        Install_SystemMEM();
        DBG_DUMP("\r\n");
        freeMem = ImageStream_GetFreeBuffer();
        if(freeMem)
        {
            DBG_DUMP("[POOL_ID_APP]\r\n");
            AppMem_Dump();
            DBG_DUMP("\r\n");
            SxCmd_DoCommand("isf dump3");
        }
    }
    else //sort by addr
    {
        SxCmd_DoCommand("ker dumpmem");
        freeMem = ImageStream_GetFreeBuffer();
        if(freeMem)
        {
            DBG_DUMP("\r\n[POOL_ID_APP]\r\n");
            DBG_DUMP("=============================================================\r\n");
            AppMem_Dump();
            DBG_DUMP("=============================================================\r\n");

            DBG_DUMP("\r\n[ImageStream]\r\n");
            DBG_DUMP("=============================================================\r\n");
            SxCmd_DoCommand("isf dump2");
            DBG_DUMP("=============================================================\r\n");
            DBG_DUMP("\r\n");
        }
    }

    //#NT#2016/06/03#Niven Cho -begin
    //#NT#show media suggest size if in recording.
    if(1)
    {
        UINT32 uiMediaRecEn = 0;
        UINT32 uiID = 0;
        MovRec_GetInfo(MOVREC_GETINFO_PATHOPEN,&uiID,&uiMediaRecEn,NULL);
        if(uiMediaRecEn)
        {
            DBG_DUMP("[Meida Recoder]\r\n");
            DBG_DUMP("=============================================================\r\n");
            DBG_DUMP("    Meida Recoder Suggest Size: 0x%08X\r\n",MovRec_GetSuggestMem());
            DBG_DUMP("=============================================================\r\n");
        }
    }
    //#NT#2016/06/03#Niven Cho -end

    if(1)
    {
        DBG_DUMP("[Summary]\r\n");
        DBG_DUMP("=============================================================\r\n");
        DBG_DUMP("^Y   Total     : Range[0x%08X~0x%08X] Size=0x%08X\r\n",
            OS_GetMemAddr(MEM_TOTAL),
            OS_GetMemAddr(MEM_TOTAL)+OS_GetMemSize(MEM_TOTAL),
            OS_GetMemSize(MEM_TOTAL)
            );
        //fix for CID 44965 - begin
        if(freeMem)
        {
        DBG_DUMP("   Free      : Range[0x%08X~0x%08X] Size=0x%08X\r\n",
            freeMem->Addr - 0x20000000,
            freeMem->Addr - 0x20000000+freeMem->Size,
            freeMem->Size
            );
        }
        //fix for CID 44965 - end
    }
#endif
    return TRUE;
}
static BOOL Cmd_sys_boot(CHAR* strCmd)
{
    DBG_DUMP("Boot\r\n");
    //DscMain();
    // Start Dsc System
    //System_PowerOn(SYS_POWERON_NORMAL);
    GxSystem_PowerOn(); //boot start
    return TRUE;
}
static BOOL Cmd_sys_shutdown(CHAR* strCmd)
{
    DBG_DUMP("Shutdown\r\n");
    //System_PowerOff(SYS_POWEROFF_NORMAL);
    Ux_SendEvent(0, NVTEVT_SYSTEM_SHUTDOWN, 1, 0); //shutdown start
    return TRUE;
}
static BOOL Cmd_sys_sleep(CHAR* strCmd)
{
    DBG_DUMP("Sleep\r\n");
    return TRUE;
}
static BOOL Cmd_sys_wakeup(CHAR* strCmd)
{
    DBG_DUMP("Wakeup\r\n");
    return TRUE;
}

static BOOL Cmd_sys_disp(CHAR* strCmd)
{
    char str1[32];

    sscanf_s(strCmd,"%s", str1, sizeof(str1)); //fix for CID 44997

    if(strncmp(str1, "off", 31)==0)
    {
        DBG_DUMP("(OFF)!\r\n");
        //TBD
    }
    if(strncmp(str1, "lcd", 31)==0)
    {
        //#NT#2016/06/21#Jeah Yen -begin
        //#NT#Support display fast switch
        #if (DISPLAY_FAST_SWITCH == DISABLE)
        DBG_DUMP("LCD!\r\n");
        Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  0);
        SxCmd_DoCommand("dx tv vdet 0");
        SxCmd_DoCommand("dx hdmi vdet 0");
        SxCmd_DoCommand("dx lcd2 vdet 0");
        SwTimer_DelayMs(500); //wait for insert/remove detect
        SwTimer_DelayMs(500); //wait for insert/remove detect
        #else
        UINT32 device_def = (UINT32)Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_LCD);
        DBG_DUMP("LCD!\r\n");
        Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  0);
        if(GxVideo_GetDevice(DOUT1) != device_def)
        {
            Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT1));
        }
        if(GxVideo_GetDevice(DOUT2) != 0)
            Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT2));
        #endif
        //#NT#2016/06/21#Jeah Yen -end
    }
    if(strncmp(str1, "tv", 31)==0)
    {
        //#NT#2016/06/21#Jeah Yen -begin
        //#NT#Support display fast switch
        #if (DISPLAY_FAST_SWITCH == DISABLE)
        DBG_DUMP("TV!\r\n");
        Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  0);
        SxCmd_DoCommand("dx tv vdet 0");
        SxCmd_DoCommand("dx hdmi vdet 0");
        SxCmd_DoCommand("dx lcd2 vdet 0");
        SwTimer_DelayMs(500); //wait for insert/remove detect
        SxCmd_DoCommand("dx tv vdet 1");
        SwTimer_DelayMs(500); //wait for insert/remove detect
        #else
        UINT32 device_def = (UINT32)Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_LCD);
        UINT32 device = (UINT32)Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_TVOUT);
        DBG_DUMP("TV!\r\n");
        Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  0);
        if(GxVideo_GetDevice(DOUT1) != device)
        {
            if(GxVideo_GetDevice(DOUT1) != device_def)
                Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT1));
            Ux_PostEvent(NVTEVT_VIDEO_INSERT, 1, device);
        }
        if(GxVideo_GetDevice(DOUT2) != 0)
            Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT2));
        #endif
        //#NT#2016/06/21#Jeah Yen -end
    }
    if(strncmp(str1, "hd", 31)==0)
    {
        //#NT#2016/06/21#Jeah Yen -begin
        //#NT#Support display fast switch
        #if (DISPLAY_FAST_SWITCH == DISABLE)
        DBG_DUMP("HDMI!\r\n");
        Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  0);
        SxCmd_DoCommand("dx tv vdet 0");
        SxCmd_DoCommand("dx hdmi vdet 0");
        SxCmd_DoCommand("dx lcd2 vdet 0");
        SwTimer_DelayMs(500); //wait for insert/remove detect
        SxCmd_DoCommand("dx hdmi vdet 1");
        SwTimer_DelayMs(500); //wait for insert/remove detect
        #else
        UINT32 device_def = (UINT32)Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_LCD);
        UINT32 device = (UINT32)Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_HDMIOUT);
        DBG_DUMP("HDMI!\r\n");
        Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  0);
        if(GxVideo_GetDevice(DOUT1) != device)
        {
            if(GxVideo_GetDevice(DOUT1) != device_def)
                Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT1));
            Ux_PostEvent(NVTEVT_VIDEO_INSERT, 1, device);
        }
        if(GxVideo_GetDevice(DOUT2) != 0)
            Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT2));
        #endif
        //#NT#2016/06/21#Jeah Yen -end
    }
    if(strncmp(str1, "lcd2", 31)==0)
    {
        //#NT#2016/06/21#Jeah Yen -begin
        //#NT#Support display fast switch
        #if (DISPLAY_FAST_SWITCH == DISABLE)
        DBG_DUMP("LCD2!\r\n");
        Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  0);
        SxCmd_DoCommand("dx tv vdet 0");
        SxCmd_DoCommand("dx hdmi vdet 0");
        SxCmd_DoCommand("dx lcd2 vdet 0");
        SwTimer_DelayMs(500); //wait for insert/remove detect
        SxCmd_DoCommand("dx lcd2 vdet 1");
        SwTimer_DelayMs(500); //wait for insert/remove detect
        #else
        UINT32 device_def = (UINT32)Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_LCD);
        UINT32 device2 = (UINT32)Dx_GetObject(DX_CLASS_DISPLAY_EXT|DX_TYPE_LCD2);
        DBG_DUMP("LCD2!\r\n");
        Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  0);
        if(GxVideo_GetDevice(DOUT1) != device_def)
            Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT1));
        if(GxVideo_GetDevice(DOUT2) != device2)
        {
            if(GxVideo_GetDevice(DOUT2) != 0)
                Ux_PostEvent(NVTEVT_VIDEO_REMOVE, 1, (UINT32)GxVideo_GetDevice(DOUT2));
            Ux_PostEvent(NVTEVT_VIDEO_INSERT, 1, device2);
        }
        #endif
        //#NT#2016/06/21#Jeah Yen -end
    }

    if(strncmp(str1, "lcdtv", 31)==0)
    {
        DBG_DUMP("LCD+TV!\r\n");
        Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  1);
        SxCmd_DoCommand("dx tv vdet 0");
        SxCmd_DoCommand("dx hdmi vdet 0");
        SxCmd_DoCommand("dx lcd2 vdet 0");
        SwTimer_DelayMs(500); //wait for insert/remove detect
        SxCmd_DoCommand("dx tv vdet 1");
        SwTimer_DelayMs(500); //wait for insert/remove detect
    }
    if(strncmp(str1, "lcdhd", 31)==0)
    {
        DBG_DUMP("LCD+HDMI!\r\n");
        Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  1);
        SxCmd_DoCommand("dx tv vdet 0");
        SxCmd_DoCommand("dx hdmi vdet 0");
        SxCmd_DoCommand("dx lcd2 vdet 0");
        SwTimer_DelayMs(500); //wait for insert/remove detect
        SxCmd_DoCommand("dx hdmi vdet 1");
        SwTimer_DelayMs(500); //wait for insert/remove detect
    }
    if(strncmp(str1, "lcdlcd2", 31)==0)
    {
        DBG_DUMP("LCD+LCD2!\r\n");
        Ux_SendEvent(0, NVTEVT_EXE_DUALDISP, 1,  1);
        SxCmd_DoCommand("dx tv vdet 0");
        SxCmd_DoCommand("dx hdmi vdet 0");
        SxCmd_DoCommand("dx lcd2 vdet 0");
        SwTimer_DelayMs(500); //wait for insert/remove detect
        SxCmd_DoCommand("dx lcd2 vdet 1");
        SwTimer_DelayMs(500); //wait for insert/remove detect
    }

    if(strncmp(str1, "tvf1", 31)==0)
    {
        DBG_DUMP("TV FMT = NTSC\r\n");
        Ux_SendEvent(0, NVTEVT_EXE_TVFORMAT, 1, 0);
    }
    if(strncmp(str1, "tvf2", 31)==0)
    {
        DBG_DUMP("TV FMT = PAL\r\n");
        Ux_SendEvent(0, NVTEVT_EXE_TVFORMAT, 1, 1);
    }
    if(strncmp(str1, "hdf1", 31)==0)
    {
        DBG_DUMP("HDMI FMT = 1920x1080i60\r\n");
        Ux_SendEvent(0, NVTEVT_EXE_HDMIFORMAT, 1, 0);
    }
    if(strncmp(str1, "hdf2", 31)==0)
    {
        DBG_DUMP("HDMI FMT = 1280x720p30\r\n");
        Ux_SendEvent(0, NVTEVT_EXE_HDMIFORMAT, 1, 1);
    }

    if(strncmp(str1, "mvs1", 31)==0)
    {
        UINT32 uiSelect = MOVIE_SIZE_FRONT_1920x1080P30;
        DBG_DUMP("MOVIE SIZE = 1920x1080p30 (16:9)\r\n");
        UI_SetData(FL_MOVIE_SIZE, uiSelect);
        Ux_SendEvent(0, NVTEVT_EXE_MOVIESIZE, 1, uiSelect);
        //Ux_SendEvent(0, NVTEVT_EXE_IMAGE_RATIO, 1, GetMovieSizeRatio(uiSelect));
    }
    if(strncmp(str1, "mvs2", 31)==0)
    {
        UINT32 uiSelect = MOVIE_SIZE_FRONT_640x480P30;
        DBG_DUMP("MOVIE SIZE = 640x480p30 (4:3)\r\n");
        UI_SetData(FL_MOVIE_SIZE, uiSelect);
        Ux_SendEvent(0, NVTEVT_EXE_MOVIESIZE, 1, uiSelect);
        //Ux_SendEvent(0, NVTEVT_EXE_IMAGE_RATIO, 1, GetMovieSizeRatio(uiSelect));
    }
    if(strncmp(str1, "pts1", 31)==0)
    {
        UINT32 uiSelect = PHOTO_SIZE_2MHD;
        DBG_DUMP("PHOTO SIZE = 1920x1080 (16:9)\r\n");
        UI_SetData(FL_PHOTO_SIZE, uiSelect);
        Ux_SendEvent(0,NVTEVT_EXE_CAPTURE_SIZE,1, uiSelect);
        Ux_SendEvent(0,NVTEVT_EXE_IMAGE_RATIO,1, GetPhotoSizeRatio(uiSelect));
    }
    if(strncmp(str1, "pts2", 31)==0)
    {
        UINT32 uiSelect = PHOTO_SIZE_VGA;
        DBG_DUMP("PHOTO SIZE = 640x480 (4:3)\r\n");
        UI_SetData(FL_PHOTO_SIZE, uiSelect);
        Ux_SendEvent(0,NVTEVT_EXE_CAPTURE_SIZE,1, uiSelect);
        Ux_SendEvent(0,NVTEVT_EXE_IMAGE_RATIO,1, GetPhotoSizeRatio(uiSelect));
    }
    #if (PHOTO_MODE==ENABLE)
    if(strncmp(str1, "ptzi", 31)==0)
    {
        UINT32 bStop = FALSE;
        PhotoExe_DZoomInBK(&bStop);
    }
    if(strncmp(str1, "ptzo", 31)==0)
    {
        UINT32 bStop = FALSE;
        PhotoExe_DZoomOutBK(&bStop);
    }
    #endif
    return TRUE;
}
static BOOL Cmd_sys_aout(CHAR* strCmd)
{
    char str1[32];

    sscanf_s(strCmd,"%s", str1, sizeof(str1));

    if(strncmp(str1, "off", 31)==0)
    {
        DBG_DUMP("(OFF)!\r\n");
        //TBD
    }
    if(strncmp(str1, "spk", 31)==0)
    {
        DBG_DUMP("SPK!\r\n");
        SxCmd_DoCommand("dx aout vdet 0");
        SwTimer_DelayMs(500); //wait for insert/remove detect
    }
    if(strncmp(str1, "line", 31)==0)
    {
        DBG_DUMP("LINE!\r\n");
        SxCmd_DoCommand("dx aout vdet 1");
        SwTimer_DelayMs(500); //wait for insert/remove detect
    }
    return TRUE;
}
static BOOL Cmd_sys_sensor(CHAR* strCmd)
{
    char str1[32];

    sscanf_s(strCmd,"%s", str1, sizeof(str1)); //fix for CID 44999

    if(strncmp(str1, "off", 31)==0)
    {
        DBG_DUMP("NO SENSOR!\r\n");
        SxCmd_DoCommand("dx snr1 vdet 0");
        SxCmd_DoCommand("dx snr2 vdet 0");
        SwTimer_DelayMs(500); //wait for insert/remove detect
    }
    if(strncmp(str1, "s1", 31)==0)
    {
        DBG_DUMP("SENSOR1!\r\n");
        SxCmd_DoCommand("dx snr1 vdet 1");
        SxCmd_DoCommand("dx snr2 vdet 0");
        SwTimer_DelayMs(500); //wait for insert/remove detect
    }
    if(strncmp(str1, "s2", 31)==0)
    {
        DBG_DUMP("SENSOR2!\r\n");
        SxCmd_DoCommand("dx snr1 vdet 0");
        SxCmd_DoCommand("dx snr2 vdet 1");
        SwTimer_DelayMs(500); //wait for insert/remove detect
    }
    if(strncmp(str1, "s1s2", 31)==0)
    {
        DBG_DUMP("SENSOR1+SENSOR2!\r\n");
        SxCmd_DoCommand("dx snr1 vdet 1");
        SxCmd_DoCommand("dx snr2 vdet 1");
        SwTimer_DelayMs(500); //wait for insert/remove detect
    }

    if(strncmp(str1, "d1", 31)==0)
    {
        DBG_DUMP("display SENSOR1!\r\n");
        Ux_PostEvent(NVTEVT_SENSOR_DISPLAY, 1, SENSOR_1);
    }
    if(strncmp(str1, "d2", 31)==0)
    {
        DBG_DUMP("display SENSOR2!\r\n");
        Ux_PostEvent(NVTEVT_SENSOR_DISPLAY, 1, SENSOR_2);
    }
    if(strncmp(str1, "d3", 31)==0)
    {
        DBG_DUMP("display SENSOR3!\r\n");
        Ux_PostEvent(NVTEVT_SENSOR_DISPLAY, 1, SENSOR_3);
    }
    if(strncmp(str1, "d4", 31)==0)
    {
        DBG_DUMP("display SENSOR4!\r\n");
        Ux_PostEvent(NVTEVT_SENSOR_DISPLAY, 1, SENSOR_4);
    }
    if(strncmp(str1, "d12", 31)==0)
    {
        DBG_DUMP("display SENSOR 1 & 2!\r\n");
        Ux_PostEvent(NVTEVT_SENSOR_DISPLAY, 1, (SENSOR_1|SENSOR_2));
    }
    if(strncmp(str1, "dall", 31)==0)
    {
        DBG_DUMP("display SENSOR all!\r\n");
        Ux_PostEvent(NVTEVT_SENSOR_DISPLAY, 1, SENSOR_ALL);
    }
    return TRUE;
}

//#NT#2016/05/31#Jeah Yen -begin
//#NT#add event trace
extern UINT32 UI_EventTraceLvl;

static BOOL Cmd_sys_event(CHAR* strCmd)
{
    UINT32 lvl=0;

    //fix for CID 64224 - begin
    sscanf_s(strCmd,"%d", &lvl);
    //fix for CID 64224 - end
    if(lvl>5)
        lvl=5;

    UI_EventTraceLvl = lvl;
    DBG_DUMP("event trace level = %d!\r\n", UI_EventTraceLvl);
    return TRUE;
}
//#NT#2016/05/31#Jeah Yen -begin

static BOOL Cmd_sys_tm(CHAR* strCmd)
{
    char str1[32];

    sscanf_s(strCmd,"%s", str1, sizeof(str1)); //fix for CID 45000

    if(strncmp(str1, "boot", 31)==0)
    {
        DBG_MSG("Dump TM results of Boot:\r\n");
        TM_Dump("*");
    }
    return TRUE;
}

//#NT#2016/05/12#Jeah Yen -begin
//#NT#add tprof start
static BOOL Cmd_sys_usage(CHAR* strCmd)
{
    char str1[32];

    sscanf_s(strCmd,"%s", str1, sizeof(str1));

    sys_keeptop = FALSE;
    sys_dotrace = FALSE;
    sys_dump_cnt=0;
    if(strncmp(str1, "top", 31)==0)
    {
        sys_keeptop = TRUE;
    }
    if(strncmp(str1, "trace", 31)==0)
    {
        sys_dotrace = TRUE;
    }
    SxCmd_DoCommand("tprof start 3");
    return TRUE;
}

static BOOL Cmd_sys_cpu(CHAR* strCmd)
{
    char str1[32];

    sscanf_s(strCmd,"%s", str1, sizeof(str1));

    sys_keeptop = FALSE;
    sys_dotrace = FALSE;
    sys_dump_cnt=0;
    if(strncmp(str1, "top", 31)==0)
    {
        sys_keeptop = TRUE;
    }
    if(strncmp(str1, "trace", 31)==0)
    {
        //sys_dotrace = TRUE;
        return TRUE; //not support
    }
    SxCmd_DoCommand("tprof start 2");
    return TRUE;
}

static BOOL Cmd_sys_task(CHAR* strCmd)
{
    char str1[32];

    sscanf_s(strCmd,"%s", str1, sizeof(str1));

    sys_keeptop = FALSE;
    sys_dotrace = FALSE;
    sys_dump_cnt=0;
    if(strncmp(str1, "top", 31)==0)
    {
        sys_keeptop = TRUE;
    }
    if(strncmp(str1, "trace", 31)==0)
    {
        return TRUE; //not support
    }
    SxCmd_DoCommand("tprof start 1");
    return TRUE;
}
//#NT#2016/05/12#Jeah Yen -end
//#NT#2016/06/23#Niven Cho -begin
//#NT#Enter calibration by cgi event or command event
static BOOL Cmd_sys_calibration(CHAR* strCmd)
{
    #if (CALIBRATION_FUNC == ENABLE)
    Ux_PostEvent(NVTEVT_KEY_CALIBRATION, 0);
    #else
    DBG_ERR("CALIBRATION_FUNC must be ENABLE.\r\n");
    #endif
    return TRUE;
}
//#NT#2016/06/23#Niven Cho -end

SXCMD_BEGIN(sys, "system command")
//#NT#2016/05/12#Jeah Yen -begin
//#NT#add tprof start
SXCMD_ITEM("usage %", Cmd_sys_usage,"usage")
SXCMD_ITEM("cpu %", Cmd_sys_cpu,"cpu")
SXCMD_ITEM("task %", Cmd_sys_task,"task")
//#NT#2016/05/12#Jeah Yen -end
SXCMD_ITEM("mem %", Cmd_sys_mem,"mem")
SXCMD_ITEM("boot", Cmd_sys_boot,"boot")
SXCMD_ITEM("shutdown", Cmd_sys_shutdown,"shutdown")
SXCMD_ITEM("sleep", Cmd_sys_sleep, "sleep")
SXCMD_ITEM("wakeup", Cmd_sys_wakeup, "wakeup")
SXCMD_ITEM("disp %", Cmd_sys_disp, "change disp setting")
SXCMD_ITEM("aout %", Cmd_sys_aout, "change aout setting")
SXCMD_ITEM("sensor %", Cmd_sys_sensor, "change sensor setting")
//#NT#2016/05/31#Jeah Yen -begin
//#NT#add event trace
SXCMD_ITEM("event %", Cmd_sys_event, "event trace level")
//#NT#2016/05/31#Jeah Yen -end
SXCMD_ITEM("tm %", Cmd_sys_tm, "time mark")
//#NT#2016/06/23#Niven Cho -begin
//#NT#Enter calibration by cgi event or command event
SXCMD_ITEM("calibration", Cmd_sys_calibration, "enter calibration window")
//#NT#2016/06/23#Niven Cho -end
SXCMD_END()


///////////////////////////////////////////////////////////////////////////////
// key input
///////////////////////////////////////////////////////////////////////////////


//FILTER for KEY EMULATION
BOOL User_CommandFilter(CHAR* pcCmdStr, int len)
{
    int slen = strlen(pcCmdStr);

    switch (*pcCmdStr)
    {
    // Key Macro
    case 'k':
        if(!strncmp(pcCmdStr, "koff", 4) && (slen == 4))
        {
            strncpy(pcCmdStr, "key power", len);pcCmdStr[len+1] = 0;
        }
        else
        if (!strncmp(pcCmdStr, "kdel", 4) && (slen == 4))
        {
            strncpy(pcCmdStr, "key delete", len);pcCmdStr[len+1] = 0;
        }
        else
        if (!strncmp(pcCmdStr, "km", 2) && (slen == 2))
        {
            strncpy(pcCmdStr, "key menu", len);pcCmdStr[len+1] = 0;
        }
        else
        if (!strncmp(pcCmdStr, "kmd", 3) && (slen == 3))
        {
            strncpy(pcCmdStr, "key mode", len);pcCmdStr[len+1] = 0;
        }
        else
        if (!strncmp(pcCmdStr, "kmp", 3) && (slen == 3))
        {
            strncpy(pcCmdStr, "key playback", len);pcCmdStr[len+1] = 0;
        }
        else
        if (!strncmp(pcCmdStr, "kmv", 3) && (slen == 3))
        {
            strncpy(pcCmdStr, "key movie", len);pcCmdStr[len+1] = 0;
        }
        else
        if (!strncmp(pcCmdStr, "ku", 2) && (slen == 2))
        {
            strncpy(pcCmdStr, "key up", len);pcCmdStr[len+1] = 0;
        }
        else
        if (!strncmp(pcCmdStr, "kd", 2) && (slen == 2))
        {
            strncpy(pcCmdStr, "key down", len);pcCmdStr[len+1] = 0;
        }
        else
        if (!strncmp(pcCmdStr, "kr", 2) && (slen == 2))
        {
            strncpy(pcCmdStr, "key right", len);pcCmdStr[len+1] = 0;
        }
        else
        if (!strncmp(pcCmdStr, "kl", 2) && (slen == 2))
        {
            strncpy(pcCmdStr, "key left", len);pcCmdStr[len+1] = 0;
        }
        else
        if (!strncmp(pcCmdStr, "ke", 2) && (slen == 2))
        {
            strncpy(pcCmdStr, "key enter", len);pcCmdStr[len+1] = 0;
        }
        else
        if (!strncmp(pcCmdStr, "kzi", 3) && (slen == 3))
        {
            strncpy(pcCmdStr, "key zoomin", len);pcCmdStr[len+1] = 0;
        }
        else
        if (!strncmp(pcCmdStr, "kzo", 3) && (slen == 3))
        {
            strncpy(pcCmdStr, "key zoomout", len);pcCmdStr[len+1] = 0;
        }
        else
        if (!strncmp(pcCmdStr, "kzr", 3) && (slen == 3))
        {
            strncpy(pcCmdStr, "key zoom release", len);pcCmdStr[len+1] = 0;
        }
        else
        if (!strncmp(pcCmdStr, "ks1", 3) && (slen == 3))
        {
            strncpy(pcCmdStr, "key shutter1", len);pcCmdStr[len+1] = 0;
        }
        else
        if (!strncmp(pcCmdStr, "ks2", 3) && (slen == 3))
        {
            strncpy(pcCmdStr, "key shutter2", len);pcCmdStr[len+1] = 0;
        }
        if (!strncmp(pcCmdStr, "kn", 2) && (slen == 2))
        {
            strncpy(pcCmdStr, "key next", len);pcCmdStr[len+1] = 0;
        }
        //#NT#2016/03/16#Niven Cho -begin
        //#NT#add key previous
        else
        if (!strncmp(pcCmdStr, "kp", 2) && (slen == 2))
        {
            strncpy(pcCmdStr, "key prev", len);pcCmdStr[len+1] = 0;
        }
        //#NT#2016/03/16#Niven Cho -end
        else
        if (!strncmp(pcCmdStr, "ks", 2) && (slen == 2))
        {
            strncpy(pcCmdStr, "key select", len);pcCmdStr[len+1] = 0;
        }
        if (!strncmp(pcCmdStr, "ki", 2) && (slen == 2))
        {
            strncpy(pcCmdStr, "key i", len);pcCmdStr[len+1] = 0;
        }
        break;
    //#NT#2016/05/12#Jeah Yen -begin
    //#NT#add tprof stop hotkey
    case 'x':
        if(!strncmp(pcCmdStr, "x", 1) && (slen == 1))
        {
            if(sys_keeptop || sys_dotrace)
            {
                strncpy(pcCmdStr, "tprof stop", len);pcCmdStr[len+1] = 0;
            }
        }
        break;
    //#NT#2016/05/12#Jeah Yen -end
    }
    return FALSE;
}


//#Fix warning message
//UINT32  uiKeyTmpMsk;

static BOOL Cmd_key_power(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_XXX)
    {
        DBG_DUMP("Power Key\r\n");
        //Ux_PostEvent(NVTEVT_KEY_POWER, 1, NVTEVT_KEY_PRESS);
        Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
        return TRUE;
    }
}
static BOOL Cmd_key_delete(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_DEL)
    {
        DBG_DUMP("Del Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_DEL, 1, NVTEVT_KEY_PRESS);
        return TRUE;
    }
}
BOOL Cmd_key_menu(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_MENU)
    {
        DBG_DUMP("Menu Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_MENU, 1, NVTEVT_KEY_PRESS);
        return TRUE;
    }
}
static BOOL Cmd_key_mode(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_MODE)
    {
        DBG_DUMP("Mode Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_MODE, 1, NVTEVT_KEY_PRESS);
        return TRUE;
    }
}
static BOOL Cmd_key_playback(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_PLAYBACK)
    {
        DBG_DUMP("Playback Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_PLAYBACK, 1, NVTEVT_KEY_PRESS);
        return TRUE;
    }
}
static BOOL Cmd_key_movie(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_PLAYBACK)
    {
        DBG_DUMP("Movie Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_MOVIE, 1, NVTEVT_KEY_PRESS);
        return TRUE;
    }
}
static BOOL Cmd_key_up(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_UP)
    {
        DBG_DUMP("Up Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_UP, 1, NVTEVT_KEY_PRESS);
        return TRUE;
    }
}
static BOOL Cmd_key_down(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_DOWN)
    {
        DBG_DUMP("Down Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_DOWN, 1, NVTEVT_KEY_PRESS);
        return TRUE;
    }
}
static BOOL Cmd_key_right(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_RIGHT)
    {
        DBG_DUMP("Right Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_RIGHT, 1, NVTEVT_KEY_PRESS);
        return TRUE;
    }
}
static BOOL Cmd_key_left(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_LEFT)
    {
        DBG_DUMP("Left Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_LEFT, 1, NVTEVT_KEY_PRESS);
        return TRUE;
    }
}
static BOOL Cmd_key_enter(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_ENTER)
    {
        DBG_DUMP("Enter Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_ENTER, 1, NVTEVT_KEY_PRESS);
        return TRUE;
    }
}
static BOOL Cmd_key_zoomin(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_ZOOMIN)
    {
        DBG_DUMP("Zoom In Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_ZOOMIN, 1, NVTEVT_KEY_PRESS);
        return TRUE;
    }
}
static BOOL Cmd_key_zoomout(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_ZOOMOUT)
    {
        DBG_DUMP("Zoom Out Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_ZOOMOUT, 1, NVTEVT_KEY_PRESS);
        return TRUE;
    }
}

static BOOL Cmd_key_zoomin_release(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_ZOOMIN)
    {
        DBG_DUMP("Zoom In Key release\r\n");
        Ux_PostEvent(NVTEVT_KEY_ZOOMIN, 1, NVTEVT_KEY_RELEASE);
        return TRUE;
    }
}

static BOOL Cmd_key_zoomout_release(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_ZOOMOUT)
    {
        DBG_DUMP("Zoom Out Key release\r\n");
        Ux_PostEvent(NVTEVT_KEY_ZOOMOUT, 1, NVTEVT_KEY_RELEASE);
        return TRUE;
    }
}

static BOOL Cmd_key_zoomrelease(CHAR* strCmd)
{
    {
        DBG_DUMP("zoom release Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_ZOOMRELEASE, 1, NVTEVT_KEY_ZOOMRELEASE);
        return TRUE;
    }
}

static BOOL Cmd_key_shutter1(CHAR* strCmd)
{
    //if(uiKeyTmpMsk & FLGkey_SHUTTER1)
    {
        DBG_DUMP("Shutter1 Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_SHUTTER1, 1, NVTEVT_KEY_PRESS);
        return TRUE;
    }
}
#if (WIFI_AP_FUNC==ENABLE)
extern void FlowWiFiMovie_Stop2Idle(void);
#endif
static BOOL Cmd_key_shutter2(CHAR* strCmd)
{
#if(WIFI_AP_FUNC==ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_WIFI)
    {
        #if(IPCAM_FUNC!=ENABLE)
        FlowWiFiMovie_Stop2Idle();
        #endif
    }
#endif
    //if(uiKeyTmpMsk & FLGkey_SHUTTER2)
    {
        DBG_DUMP("Shutter2 Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_PRESS);
        return TRUE;
    }
}

static BOOL Cmd_key_shutter1rel(CHAR* strCmd)
{
    {
        DBG_DUMP("Shutter1 release Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_SHUTTER1, 1, NVTEVT_KEY_RELEASE);
        return TRUE;
    }
}

static BOOL Cmd_key_shutter2rel(CHAR* strCmd)
{
    {
        DBG_DUMP("Shutter2 release Key\r\n");
        Ux_PostEvent(NVTEVT_KEY_SHUTTER2, 1, NVTEVT_KEY_RELEASE);
        return TRUE;
    }
}
#if(WIFI_AP_FUNC==ENABLE)
#include "UIMenuWndWiFiWait.h"
static BOOL Cmd_key_i(CHAR* strCmd)
{
    DBG_DUMP("I Key\r\n");

    #if(IPCAM_FUNC!=ENABLE)
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
    #if(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
    if(UI_GetData(FL_WIFI_LINK)==WIFI_LINK_NG)
        BKG_PostEvent(NVTEVT_BKW_WIFI_ON);
    #else
    Ux_OpenWindow(&UIMenuWndWiFiWaitCtrl, 0);
    #endif
//#NT#2016/03/23#Isiah Chang -end
    #endif
    BKG_PostEvent(NVTEVT_BKW_WIFI_ON);
    return TRUE;

}
#endif
static BOOL Cmd_key_next(CHAR* strCmd)
{
    DBG_DUMP("next Key\r\n");
    Ux_PostEvent(NVTEVT_KEY_NEXT, 1, NVTEVT_KEY_PRESS);
    return TRUE;
}
//#NT#2016/03/16#Niven Cho -begin
//#NT#add key previous
static BOOL Cmd_key_prev(CHAR* strCmd)
{
    DBG_DUMP("prev Key\r\n");
    Ux_PostEvent(NVTEVT_KEY_PREV, 1, NVTEVT_KEY_PRESS);
    return TRUE;
}
//#NT#2016/03/16#Niven Cho -end
static BOOL Cmd_key_select(CHAR* strCmd)
{
    DBG_DUMP("select Key\r\n");
    Ux_PostEvent(NVTEVT_KEY_SELECT, 1, NVTEVT_KEY_PRESS);
    return TRUE;
}
static BOOL Cmd_key_cut(CHAR* strCmd)
{
    UINT32 ircut = 0;

    sscanf_s(strCmd,"%d ",&ircut);
    DBG_DUMP("ircut %d\r\n",ircut);
    Ux_PostEvent(NVTEVT_EXE_MOVIE_IR_CUT,         1,  ircut);
    return TRUE;
}

#if 0//(CURL_FUNC == ENABLE)
static void CurlCmd_dumpmem(UINT32 Addr, UINT32 size)
{
    UINT32 i,j;
    DBG_DUMP("\r\n Addr=0x%x, size =0x%x\r\n",Addr,size);


    for(j=Addr;j<(Addr + size);j+=0x10)
    {
        DBG_DUMP("^R 0x%8x:",j);
        for(i=0;i<16;i++)
            DBG_DUMP("0x%2x,",*(char*)(j+i) );
        DBG_DUMP("\r\n");
    }
    DBG_DUMP("Data %s\r\n",(char*)Addr);

}


static void curl_rw_cb(UINT32 MessageID, UINT32 Status, UINT32 isContine, UINT32 Param3)
{
    UINT32 *bContue = (UINT32*)isContine;

    if (bContue)
    {
        // set contine false to stop curl r/w
        //*bContue = FALSE;
    }
}
#endif

static BOOL Cmd_key_curl(CHAR* strCmd)
{
    #if 0 //(CURL_FUNC == ENABLE)

    #include "NetFs.h"

    static char tmpbuff[0x400000];
    UINT32 size,pos;
    char   ip[60]="192.168.0.3";
    char   rootdir[10]="A:\\";
    char   path[60]="A:\\test.bin";

    //sscanf(strCmd,"%d %d %d %d %d %d %d %d",&g_lviewframeRate,&g_lviewTargetRate,&g_lviewQueueFrame,&g_lviewPort,&g_lviewTaskPri, &g_hwmemcpy, &g_maxJpgSize, &g_reqDelay);
    sscanf(strCmd,"%d %s",&size,path);

    FST_FILE  filehdl;


    NetFs_SetParam(NETFS_PARAM_REMOTE_IP, (UINT32)ip);
    NetFs_SetParam(NETFS_PARAM_ROOT_DIR, (UINT32)rootdir);
    NetFs_SetParam(NETFS_PARAM_USE_SSL, FALSE);

    // test1
    pos = 0;
    filehdl = NetFs_OpenFile(path,FST_OPEN_READ);
    NetFs_SeekFile(filehdl,0,FST_SEEK_SET);
    size = 0x200000;
    NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
    pos += size;
    NetFs_SeekFile(filehdl,pos,FST_SEEK_SET);
    size = 0x200000;
    NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff+pos, &size, 0, curl_rw_cb);
    pos += size;
    NetFs_CloseFile(filehdl);

    size = 0x200000;
    filehdl = FileSys_OpenFile("A:\\CURL\\test1.bin",FST_OPEN_WRITE|FST_CREATE_ALWAYS);
    FileSys_WriteFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
    FileSys_CloseFile(filehdl);

    pos = 0;
    filehdl = NetFs_OpenFile("A:\\CURL\\testuplo.bin",FST_OPEN_WRITE|FST_CREATE_ALWAYS);

    NetFs_SetUploadSrcFilePath(filehdl,"A:\\test.bin");
    size = 0x100000;
    NetFs_WriteFile(filehdl, (UINT8 *)tmpbuff+pos, &size, 0, curl_rw_cb);
    pos += size;
    size = 0x000080;
    NetFs_WriteFile(filehdl, (UINT8 *)tmpbuff+pos, &size, 0, NULL);
    pos += size;
    size = 0x000080;
    NetFs_WriteFile(filehdl, (UINT8 *)tmpbuff+pos, &size, 0, NULL);
    pos += size;
    size = 0x0fff00;
    NetFs_WriteFile(filehdl, (UINT8 *)tmpbuff+pos, &size, 0, NULL);
    pos += size;
    NetFs_CloseFile(filehdl);


    /*

    // test2
    pos = 0;
    filehdl = NetFs_OpenFile(path,FST_OPEN_READ);
    NetFs_SeekFile(filehdl,0,FST_SEEK_SET);
    size = 0x10000;
    NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
    pos += size;
    NetFs_SeekFile(filehdl,pos,FST_SEEK_SET);
    size = 0x10000;
    NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff+pos, &size, 0, NULL);
    pos += size;
    NetFs_CloseFile(filehdl);

    size = 0x20000;
    filehdl = FileSys_OpenFile("A:\\CURL\\test2.bin",FST_OPEN_WRITE|FST_CREATE_ALWAYS);
    FileSys_WriteFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
    FileSys_CloseFile(filehdl);

    // test3
    pos = 0;
    filehdl = NetFs_OpenFile(path,FST_OPEN_READ);
    NetFs_SeekFile(filehdl,0,FST_SEEK_SET);
    size = 0x20000;
    NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
    pos += size;
    NetFs_SeekFile(filehdl,pos,FST_SEEK_SET);
    size = 0x20000;
    NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff+pos, &size, 0, NULL);
    pos += size;
    NetFs_CloseFile(filehdl);

    size = 0x40000;
    filehdl = FileSys_OpenFile("A:\\CURL\\test3.bin",FST_OPEN_WRITE|FST_CREATE_ALWAYS);
    FileSys_WriteFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
    FileSys_CloseFile(filehdl);

    // test4
    pos = 0;
    filehdl = NetFs_OpenFile(path,FST_OPEN_READ);
    size = 0x50000;
    NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
    pos += size;
    size = 0x50000;
    NetFs_ReadFile(filehdl, (UINT8 *)tmpbuff+pos, &size, 0, NULL);
    pos += size;
    NetFs_CloseFile(filehdl);

    size = 0xA0000;
    filehdl = FileSys_OpenFile("A:\\CURL\\test4.bin",FST_OPEN_WRITE|FST_CREATE_ALWAYS);
    FileSys_WriteFile(filehdl, (UINT8 *)tmpbuff, &size, 0, NULL);
    FileSys_CloseFile(filehdl);
    */
    #endif
    return TRUE;
}
SXCMD_BEGIN(key, "key command")
SXCMD_ITEM("power", Cmd_key_power, "power key")
SXCMD_ITEM("delete", Cmd_key_delete, "delete key")
SXCMD_ITEM("menu", Cmd_key_menu, "menu key")
SXCMD_ITEM("mode", Cmd_key_mode, "mode key")
SXCMD_ITEM("playback", Cmd_key_playback, "playback key")
SXCMD_ITEM("movie", Cmd_key_movie, "movie key")
SXCMD_ITEM("up", Cmd_key_up, "up key")
SXCMD_ITEM("down", Cmd_key_down, "down key")
SXCMD_ITEM("right", Cmd_key_right, "right key")
SXCMD_ITEM("left", Cmd_key_left, "left key")
SXCMD_ITEM("enter", Cmd_key_enter, "enter key")
SXCMD_ITEM("zoomin", Cmd_key_zoomin, "zoomin key pressed")
SXCMD_ITEM("zoomout", Cmd_key_zoomout, "zoomout key pressed")
SXCMD_ITEM("zoominrel", Cmd_key_zoomin_release, "zoomin key released")
SXCMD_ITEM("zoomoutrel", Cmd_key_zoomout_release, "zoomout key released")
SXCMD_ITEM("zoomrel", Cmd_key_zoomrelease, "zoom key released")
SXCMD_ITEM("shutter1", Cmd_key_shutter1, "shutter1 key pressed")
SXCMD_ITEM("shutter2", Cmd_key_shutter2, "suutter2 key pressed")
SXCMD_ITEM("shutter1rel", Cmd_key_shutter1rel, "shutter1 key released")
SXCMD_ITEM("shutter2rel", Cmd_key_shutter2rel, "shutter2 key released")
#if(WIFI_AP_FUNC==ENABLE)
SXCMD_ITEM("i", Cmd_key_i, "i key pressed")
#endif
SXCMD_ITEM("next", Cmd_key_next, "next key")
SXCMD_ITEM("select", Cmd_key_select, "select key")
SXCMD_ITEM("cut %", Cmd_key_cut, "cut key")
SXCMD_ITEM("curl %", Cmd_key_curl, "curl key")
//#NT#2016/03/16#Niven Cho -begin
//#NT#add key previous
SXCMD_ITEM("prev", Cmd_key_prev, "prev key")
//#NT#2016/03/16#Niven Cho -end
SXCMD_END()


///////////////////////////////////////////////////////////////////////////////
// mode input
///////////////////////////////////////////////////////////////////////////////

static BOOL Cmd_mode_main(CHAR* strCmd)
{
    DBG_DUMP("Main Mode\r\n");
    {
    int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
    int iNextMode = PRIMARY_MODE_MAIN;
    UI_SetData(FL_PreMode,iCurrMode);
    UI_SetData(FL_NextMode,iNextMode);
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
    }
    return TRUE;
}
static BOOL Cmd_mode_photo(CHAR* strCmd)
{
    #if(PHOTO_MODE==ENABLE)
    DBG_DUMP("Photo Mode\r\n");
    {
    int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
    int iNextMode = PRIMARY_MODE_PHOTO;
    UI_SetData(FL_PreMode,iCurrMode);
    UI_SetData(FL_NextMode,iNextMode);
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
    }
    #endif
    return TRUE;
}
static BOOL Cmd_mode_movie(CHAR* strCmd)
{
    DBG_DUMP("Movie Mode\r\n");
    {
    //#NT#2016/09/20#Bob Huang -begin
    //#NT#Support HDMI Display with 3DNR Out
    #if (_3DNROUT_FUNC == ENABLE)
    gb3DNROut = FALSE;
    #endif
    //#NT#2016/09/20#Bob Huang -end
    int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
    int iNextMode = PRIMARY_MODE_MOVIE;
    UI_SetData(FL_PreMode,iCurrMode);
    UI_SetData(FL_NextMode,iNextMode);
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
    }
    return TRUE;
}
static BOOL Cmd_mode_playback(CHAR* strCmd)
{
    #if(PLAY_MODE==ENABLE)
    DBG_DUMP("Playback Mode\r\n");
    {
    int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
    int iNextMode = PRIMARY_MODE_PLAYBACK;
    UI_SetData(FL_PreMode,iCurrMode);
    UI_SetData(FL_NextMode,iNextMode);
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
    }
    #endif
    return TRUE;
}
static BOOL Cmd_mode_msdc(CHAR* strCmd)
{
    #if(USB_MODE==ENABLE)
    DBG_DUMP("USB MSDC Mode\r\n");
    {
    int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
    int iNextMode = PRIMARY_MODE_USBMSDC;
    UI_SetData(FL_PreMode,iCurrMode);
    UI_SetData(FL_NextMode,iNextMode);
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
    }
    #endif
    return TRUE;
}
static BOOL Cmd_mode_sidc(CHAR* strCmd)
{
    #if(USB_MODE==ENABLE)
    DBG_DUMP("USB SIDC Mode\r\n");
    //Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, PRIMARY_MODE_USBSIDC);
    #endif
    return TRUE;
}
static BOOL Cmd_mode_pcc(CHAR* strCmd)
{
    #if(USB_MODE==ENABLE)
    DBG_DUMP("USB PCC Mode\r\n");
    {
    int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
    int iNextMode = PRIMARY_MODE_USBPCC;
    UI_SetData(FL_PreMode,iCurrMode);
    UI_SetData(FL_NextMode,iNextMode);
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
    }
    #endif
    return TRUE;
}
static BOOL Cmd_mode_sleep(CHAR* strCmd)
{
    DBG_DUMP("Sleep Mode\r\n");
    {
    int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
    int iNextMode = PRIMARY_MODE_SLEEP;
    UI_SetData(FL_PreMode,iCurrMode);
    UI_SetData(FL_NextMode,iNextMode);
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
    }
    return TRUE;
}

UINT32      g_lviewframeRate = 30,g_lviewTargetRate=(800*1024), g_lviewQueueFrame=2, g_lviewPort=8192, g_lviewTaskPri=6, g_hwmemcpy = 1, g_maxJpgSize = 204800, g_reqDelay=0;

static BOOL Cmd_mode_lview(CHAR* strCmd)
{
    sscanf_s(strCmd,"%d %d %d %d %d %d %d %d",&g_lviewframeRate,&g_lviewTargetRate,&g_lviewQueueFrame,&g_lviewPort,&g_lviewTaskPri, &g_hwmemcpy, &g_maxJpgSize, &g_reqDelay);
    return TRUE;
}
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
static BOOL Cmd_mode_uvc_multimedia(CHAR* strCmd)
{
    USB_UpdateSource();
    if((USB_GetSource() == USB_SRC_USB_PC) ||(USB_GetSource() == USB_SRC_USB_CHARGING_PC))
    {
        int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
        int iNextMode = PRIMARY_MODE_MOVIE;
        DBG_DUMP("UVC Multimedia Mode\r\n");
        UI_SetData(FL_PreMode,iCurrMode);
        UI_SetData(FL_NextMode,iNextMode);
        Ux_PostEvent(NVTEVT_SYSTEM_MODE, 2, iNextMode, SYS_SUBMODE_UVC);
    }
    else
    {
        DBG_ERR("Not connecting to a USB HOST!\r\n");
    }
    return TRUE;
}
#endif
//#NT#2016/05/31#Ben Wang -end

//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
static BOOL Cmd_mode_3dnr(CHAR* strCmd)
{
    DBG_DUMP("3DNR Mode\r\n");
    gb3DNROut = TRUE;
    int iCurrMode = System_GetState(SYS_STATE_CURRMODE);
    int iNextMode = PRIMARY_MODE_MOVIE;
    UI_SetData(FL_PreMode,iCurrMode);
    UI_SetData(FL_NextMode,iNextMode);
    Ux_PostEvent(NVTEVT_SYSTEM_MODE, 1, iNextMode);
    return TRUE;
}
#endif
//#NT#2016/09/20#Bob Huang -end

SXCMD_BEGIN(mode, "mode switch")
SXCMD_ITEM("main", Cmd_mode_main, "main mode")
SXCMD_ITEM("photo", Cmd_mode_photo, "photo mode")
SXCMD_ITEM("movie", Cmd_mode_movie, "movie mode")
SXCMD_ITEM("playback", Cmd_mode_playback, "playback mode")
SXCMD_ITEM("msdc", Cmd_mode_msdc, "msdc mode")
SXCMD_ITEM("sidc", Cmd_mode_sidc, "sidc mode")
SXCMD_ITEM("pcc", Cmd_mode_pcc, "pcc mode")
SXCMD_ITEM("sleep", Cmd_mode_sleep, "sleep mode")
SXCMD_ITEM("lview %", Cmd_mode_lview, "live view mode setting")
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
SXCMD_ITEM("uvc", Cmd_mode_uvc_multimedia, "uvc multimedia mode")
#endif
//#NT#2016/05/31#Ben Wang -end
//#NT#2016/09/20#Bob Huang -begin
//#NT#Support HDMI Display with 3DNR Out
#if (_3DNROUT_FUNC == ENABLE)
SXCMD_ITEM("3dnr", Cmd_mode_3dnr, "3dnr mode")
#endif
//#NT#2016/09/20#Bob Huang -end
SXCMD_END()


///////////////////////////////////////////////////////////////////////////////
// system integrate level
///////////////////////////////////////////////////////////////////////////////
#include "SMediaPlayAPI.h"
#include "MovieInterface.h"
#include "IPL_Ctrl.h"

static BOOL Cmd_nsys_dimage(CHAR* strCmd)
{
    UINT32  d;
    INT32   arg_num;
    arg_num=sscanf_s(strCmd, "%d", &d); //fix for CID 44994
    if (arg_num<1)
        return TRUE;

    DBG_DUMP("image detail msg = %s\r\n", (d!=0)?"ON":"OFF");
    if(d!=0)
    {
        IPL_CTRL_MSG(1);
    }
    else
    {
        IPL_CTRL_MSG(0);
    }
    return TRUE;
}

static BOOL Cmd_nsys_dvideo(CHAR* strCmd)
{
    UINT32  d;
    INT32   arg_num;
    arg_num=sscanf_s(strCmd, "%d", &d); //fix for CID 44995
    if (arg_num<1)
        return TRUE;

    DBG_DUMP("video detail msg = %s\r\n", (d!=0)?"ON":"OFF");
    if(d!=0)
    {
        //nvt_open_VPLAYMsg(1);
        //nvt_open_VRECMsg(1);
        MovRec_OpenErrMsg(1);
        MovRec_OpenWrnMsg(1);
    }
    else
    {
        //nvt_open_VPLAYMsg(0);
        //nvt_open_VRECMsg(0);
        MovRec_OpenErrMsg(0);
        MovRec_OpenWrnMsg(0);
    }

    return TRUE;
}

extern UINT32 IPL_DumpDataStatus(UINT32 Id, UINT32* pData);
extern UINT32 DxSys_DumpDispDataStatus(UINT32* pData);
extern UINT32 MovRec_DumpDataStatus(UINT32* pData);
extern UINT32 MediaPlay_DumpDataStatus(UINT32* pData);
static BOOL Cmd_nsys_dsysdma(CHAR* strCmd)
{
    int i=0;
    UINT32 dmadata[4*21];
    char dmaname[21][12]={
        "SIEOut","PREIn","IPEDisOut","DISIn","IMEIn","IMEIn3DNR","IMEOut1","IMEOut2","IMEOut3",
        "IDE1V1","IDE1V2","IDE1O1","IDE1O2","IDE2V1","IDE2O1",
        "H264EncPBB","H264EncPPP","JPEGEnc","H264DecPBB","H264DecPPP","JPEGDec"};
    UINT32* pData = dmadata;
    pData += IPL_DumpDataStatus(0, pData)*4;
    pData += DxSys_DumpDispDataStatus(pData)*4;
    pData += MovRec_DumpDataStatus(pData)*4;
#if (PLAY_MODE == ENABLE)
    pData += SMediaPlay_DumpDataStatus(pData)*4;
#endif
    DBG_DUMP("=== DMA data config status =============\r\n");
    pData = dmadata;
    for(i=0;i<21;i++)
    {
    DBG_DUMP("%-12s%-8d%-8d%-8d%-8d\r\n", dmaname[i], pData[0], pData[1], pData[2], pData[3]);
    pData+=4;
    }
    DBG_DUMP("========================================\r\n");
    return TRUE;
}

SXCMD_BEGIN(nsys, "system related cmd")
SXCMD_ITEM("dimage %", Cmd_nsys_dimage, "image cmd")
SXCMD_ITEM("dvideo %", Cmd_nsys_dvideo, "video cmd")
SXCMD_ITEM("dsysdma", Cmd_nsys_dsysdma, "system DMA cmd")
SXCMD_END()


#if(WIFI_AP_FUNC==ENABLE)
NVT_SS_STATUS_T UINet_ss_status; //site survey result
BOOL Cmd_wifi_sitesurvey(CHAR* strCmd)
{

    UINet_SiteSurvey("wlan0",&UINet_ss_status);
    UINet_DumpAllSSID(&UINet_ss_status);

    return TRUE;

}
BOOL Cmd_wifi_p2pscan(CHAR* strCmd)
{
    UINet_P2PScan("wlan0",&UINet_ss_status);
    UINet_P2PDump(&UINet_ss_status);
    return TRUE;

}
BOOL Cmd_wifi_p2pconnect(CHAR* strCmd)
{
    UINT32 idx = 0;

    sscanf_s(strCmd,"%d",&idx);
    UINet_P2PConnect(&UINet_ss_status,idx);
    return TRUE;

}

BOOL Cmd_wifi_disconnect(CHAR* strCmd)
{
    UINT32 discon = 0;

    sscanf_s(strCmd,"%d",&discon);

    DBGD(discon);

    if(discon)
    {
        Ux_SendEvent(0,NVTEVT_EXE_WIFI_STOP, 0);
    }
    else
    {
        Ux_SendEvent(0,NVTEVT_EXE_WIFI_START, 0);
    }
    return TRUE;

}
//need modify SetupExe_OnWifiStart
//UI_SetData(FL_NetWorkMode,NET_AP_MODE);
//UINet_SetAuthType(NET_AUTH_NONE);
BOOL Cmd_wifi_mode(CHAR* strCmd)
{
    UINT32 mode = 0;
    UINT32 security = 0;

    sscanf_s(strCmd,"%d %d",&mode,&security);

    DBG_ERR("mode %d,security %d\r\n",mode,security);

    UI_SetData(FL_NetWorkMode,mode);
    UINet_SetMode(mode);
    UINet_SetAuthType(security);

    return TRUE;

}

BOOL Cmd_wifi_ssid(CHAR* strCmd)
{
    char szSSID[NVT_WSC_MAX_SSID_LEN] = {0};

    sscanf_s(strCmd,"%s", szSSID, sizeof(szSSID));
    UINet_SetSSID(szSSID, strlen(szSSID));

    return TRUE;
}

BOOL Cmd_wifi_passphrase(CHAR* strCmd)
{
    char szpassphrase[NVT_MAX_WEP_KEY_LEN] = {0};

    sscanf_s(strCmd,"%s", szpassphrase , sizeof(szpassphrase));
    UINet_SetPASSPHRASE(szpassphrase, strlen(szpassphrase));

    return TRUE;
}


BOOL Cmd_wifi_ping(CHAR* strCmd)
{
    int packSize = 0;
    char tmpIP[NVT_WIFIIPC_IP_MAX_LEN]={0};
    unsigned int cnt = 0;

    sscanf_s(strCmd,"%s %d %u",tmpIP, sizeof(tmpIP),&packSize, &cnt);
    DBG_IND(":: %s => ip=%s,pckSize=%d,cnt=%d\r\n",strCmd, tmpIP, packSize, cnt);
    Dhcp_Net_Ping(tmpIP, packSize, cnt);

    return TRUE;
}

BOOL Cmd_wifi_apptest(CHAR* strCmd)
{
    UINT32 cmd = 0;
    UINT32 par = 0;

    sscanf_s(strCmd,"%d %d",&cmd,&par);

    DBG_DUMP("cmd %d,par %d\r\n",cmd, par);

    WifiApp_SendAutoTestCmd(cmd, par);

    return TRUE;
}

BOOL Cmd_wifi_on(CHAR* strCmd)
{
    DrvWiFi_PowerOn();
	DBG_DUMP("wifi module is powered on\r\n");

    return TRUE;
}

SXCMD_BEGIN(wifi, "wifi command")
SXCMD_ITEM("site", Cmd_wifi_sitesurvey, "wifi site survey")
SXCMD_ITEM("p2pscan", Cmd_wifi_p2pscan, "p2p scan")
SXCMD_ITEM("p2pcon %d", Cmd_wifi_p2pconnect, "p2p connect")
SXCMD_ITEM("discon %d", Cmd_wifi_disconnect, "1:disconnect 0:connect")
SXCMD_ITEM("mode %d", Cmd_wifi_mode, "wifi mode 0:ap 1:station 2:p2pdev")
SXCMD_ITEM("ssid %", Cmd_wifi_ssid, "wifi set SSID")
SXCMD_ITEM("pass %", Cmd_wifi_passphrase, "wifi set PASSPHRASE")
SXCMD_ITEM("ping %", Cmd_wifi_ping, "wifi ping")
SXCMD_ITEM("apptest %", Cmd_wifi_apptest, "wifi app auto tset")
SXCMD_ITEM("on", Cmd_wifi_on, "wifi power on")
SXCMD_END()

#endif

//#NT#2016/05/20#Lincy Lin -begin
//#NT#Support NTP client
BOOL Cmd_sntp_open(CHAR* strCmd)
{
    INT32            ret;
    NVTIPC_SYS_MSG   sysMsg;
    char             cmd[128];
    char             ntp_server[64]="192.168.0.4";

    sscanf_s(strCmd,"%s",&ntp_server,sizeof(ntp_server));
    snprintf(cmd,sizeof(cmd),"sntp -open %s",ntp_server);
    sysMsg.sysCmdID = NVTIPC_SYSCMD_SYSCALL_REQ;
    sysMsg.DataAddr = (UINT32)&cmd[0];
    sysMsg.DataSize = strlen(cmd)+1;
    if ((ret = NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE2, &sysMsg, sizeof(sysMsg))) < 0)
    {
        DBG_ERR("NvtIPC_MsgSnd %d\r\n",ret);
        return -1;
    }
    return TRUE;
}

BOOL Cmd_sntp_close(CHAR* strCmd)
{
    INT32            ret;
    NVTIPC_SYS_MSG   sysMsg;
    char             *cmd = "sntp -close";

    sysMsg.sysCmdID = NVTIPC_SYSCMD_SYSCALL_REQ;
    sysMsg.DataAddr = (UINT32)&cmd[0];
    sysMsg.DataSize = strlen(cmd)+1;
    if ((ret = NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID, NVTIPC_SENDTO_CORE2, &sysMsg, sizeof(sysMsg))) < 0)
    {
        DBG_ERR("NvtIPC_MsgSnd %d\r\n",ret);
        return -1;
    }
    return TRUE;
}


BOOL Cmd_sntp_settime(CHAR* strCmd)
{
    UINT32     ntptime = 0;

    DBG_DUMP("\r\nstrCmd =%s\r\n",strCmd);

    sscanf(strCmd,"%d",&ntptime);
    DBG_DUMP("ntptime =%d\r\n",ntptime);
    return TRUE;
}

SXCMD_BEGIN(sntp, "sntp command")
SXCMD_ITEM("open %", Cmd_sntp_open, "sntp open")
SXCMD_ITEM("close %", Cmd_sntp_close, "sntp close")
SXCMD_ITEM("settime %", Cmd_sntp_settime, "sntp settime callback")
SXCMD_END()
//#NT#2016/05/20#Lincy Lin -end

//#NT#2016/12/13#Niven Cho -begin
//#NT#AUTO_TEST_CMD
BOOL cmd_at_last_capture_path(CHAR* strCmd)
{
	MsdcNvtCb_ExamMsg("%s\r\n",ImageApp_CamFile_GetLastWriteFilePath());
	return TRUE;
}
BOOL cmd_at_last_video_path(CHAR* strCmd)
{
	static char m_cmd_at_last_video_path[256] = {0};
	MovRec_GetLastFilePath(m_cmd_at_last_video_path);
	MsdcNvtCb_ExamMsg("%s\r\n",m_cmd_at_last_video_path);
	return TRUE;
}
//#NT#2017/03/28#Niven Cho -begin
//#NT#AUTO_TEST
BOOL cmd_set_dst_power_en(CHAR* strCmd)
{
	gpio_setDir(P_GPIO_33,GPIO_DIR_OUTPUT);
	if (strncmp(strCmd, "on", 2) == 0) {
		gpio_setPin(P_GPIO_33);
	} else {
		gpio_clearPin(P_GPIO_33);
	}
	return TRUE;
}
//#NT#2017/03/28#Niven Cho -end
SXCMD_BEGIN(autotest, "auto test command")
SXCMD_ITEM("pathcap", cmd_at_last_capture_path, "last capture file path")
SXCMD_ITEM("pathvdo", cmd_at_last_video_path, "last video file path")
SXCMD_ITEM("dstpower %", cmd_set_dst_power_en, "set dst power on/off")
SXCMD_END()
//#NT#2016/12/13#Niven Cho -end
