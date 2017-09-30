/**
    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.

    @file       SysMain_Flow_Init.c
    @ingroup    mSystemFlow

    @brief      PowerOn Flow

    @note
                1.提供power on的介面
                  System_PowerOn()
                  (a)System_PowerOn()執行
                    各mode不同的control condition
                    包含通知不同device init
                  (b)System_PowerOn()執行
                    各mode不同的control condition
                    包含控制不同device之間相互dependent的order

    @date       2012/1/1
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "UICommon.h"
#include "AppLib.h"
#include "GxeCosApi.h"
#include "GxLinuxApi.h"
#include "GxDSP.h"
#include "BinInfo.h"
#include "SysMain.h"

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysInit
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"

/////////////////////////////////////////////////////////////////////////////
//#include "DeviceSysMan.h"
//#include "DeviceNandMan.h"
//#include "NvtSystem.h"

/////////////////////////////////////////////////////////////////////////////

#if (POWERON_FAST_BOOT == ENABLE)
INT32 g_iSysPowerOnMode = SYS_POWERON_NORMAL;
#else
INT32 g_iSysPowerOnMode = SYS_POWERON_SAFE;
#endif

/**
  System initialization

  Do system initialization

  @param void
  @return void
*/
void System_PowerOn(UINT32 pwrOnType)
{
    DBG_FUNC_BEGIN("\r\n");
    g_iSysPowerOnMode = pwrOnType;

    Perf_Open();

    DBG_FUNC_END("\r\n");

    //avoid event in queue,and execute at the same time
    UI_UnlockEvent();
}

////////////////////////////////////////////////////////////////////////////////
#include "DxSys.h"
#include "DrvExt.h" //Dx_OnCommand, Dx_ConfigIO()
#include "Utility.h"
#include "ProjectInfo.h" //Prj version
/////////////////////////////////////////////////////////////////////////////
//Low level device module
extern void System_BindDeviceModule(void);

extern void SysMan_Config(void);
extern void NandMan_Config(void);
extern void UsbMan_Config(void);
extern void CardMan_Config(void);
extern void DispMan_Config(void);
extern void SensorMan_Config(void);
extern void LensMan_Config(void);
extern void AudioMan_Config(void);

extern UINT32 User_GetTempBuffer(UINT32 uiSize);

extern SX_CMD_ENTRY sys[]; //system flow: boot, shutdown, sleep, wakeup, display, sensor
extern SX_CMD_ENTRY mode[]; //system flow: app mode
extern SX_CMD_ENTRY dx[]; //system flow: dx control
extern SX_CMD_ENTRY user[]; //user custom cmd
extern SX_CMD_ENTRY nsys[]; //system dma
#if(WIFI_AP_FUNC==ENABLE)
extern SX_CMD_ENTRY wifi[]; //wifi command
#endif
//#NT#2016/05/20#Lincy Lin -begin
//#NT#Support NTP client
extern SX_CMD_ENTRY sntp[]; //sntp command
//#NT#2016/05/20#Lincy Lin -end
//#NT#2016/12/02#Niven Cho -begin
//#NT#AUTO_TEST_CMD
extern SX_CMD_ENTRY autotest[];
//#NT#2016/12/02#Niven Cho -end
void System_BindSysModule(void)
{
    SxCmd_AddTable(sys);
    SxCmd_AddTable(nsys);
}

void System_BindDeviceModule(void)
{
    //IF YOU Add Table HERE, YOU MUST PUT ITS ENTRY ON PART-1
    //add: dsc boot cmd, dsc shutdown cmd
    SxCmd_AddTable(mode);
    SxCmd_AddTable(dx);
    SxCmd_AddTable(user);
#if(WIFI_AP_FUNC==ENABLE)
    SxCmd_AddTable(wifi);
#endif
//#NT#2016/05/20#Lincy Lin -begin
//#NT#Support NTP client
    SxCmd_AddTable(sntp);
//#NT#2016/05/20#Lincy Lin -end
//#NT#2016/12/02#Niven Cho -begin
//#NT#AUTO_TEST_CMD
	SxCmd_AddTable(autotest);
//#NT#2016/12/02#Niven Cho -end

#if(_CPU2_TYPE_ == _CPU2_ECOS_)
    GxeCos_Init();
#endif

#if(_CPU2_TYPE_ == _CPU2_LINUX_)
    GxLinux_Init();
#endif

#if(_DSP_TYPE_ == _DSP_FREERTOS_)
    GxDSP_Init();
#endif
}


///////////////////////////////////////////////////////////////////////////////
//
//  START
//
///////////////////////////////////////////////////////////////////////////////
#include "ProjectInfo.h"
#include "BinInfo.h"
extern BININFO gBinInfo;

void System_BootStart(void)
{
    DBG_DUMP("^MSystem Boot begin\r\n");

    /* version messages */
    DBG_DUMP("\r\n");
    DBG_DUMP("---------------------------------------------------------\r\n");
    DBG_DUMP("LD VERISON: %s\r\n", gBinInfo.ld.LdInfo_1);
#if (FW_VERSION_RELEASE == ENABLE)
    DBG_DUMP("FW VERISON: %s\r\n", gBinInfo.fw.FwInfo_1);
#else
    DBG_DUMP("FW --- Daily Build: %s\r\n", Prj_GetCheckinDate());
#endif
    DBG_DUMP("---------------------------------------------------------\r\n");
    DBG_DUMP("\r\n");
}

void System_PowerOnBreak(void)
{
    DBG_DUMP("^MSystem Boot ----- BREAK!\r\n");
    DBG_DUMP("\r\n");
    System_OnOutputExit();

    DxSys_Exit();
}


///////////////////////////////////////////////////////////////////////////////
//
//  END
//
///////////////////////////////////////////////////////////////////////////////

void System_BootEnd(void)
{
    //Enable_WP
    DBG_DUMP("^MSystem Boot end\r\n");
}


///////////////////////////////////////////////////////////////////////////////
extern BININFO gBinInfo;
extern UINT32 UserSection_Order_full[];
extern void UserSection_LoadCb(const UINT32 Idx);

void SystemInit(void)
{
    DBG_MSG("^GInit Start\r\n");
    //if(gpio_getPin(66)) 
    {
        gpio_clearPin(66);//SD Pin power on
        debug_msg("gpio_getPin(66) = %d \r\n",gpio_getPin(66));
    }
//#NT#2016/02/25#Niven Cho -begin
//#NT#fine tune linux-cardv boot time
#if (!(_FW_COMPRESS_== _FW_COMPRESS_ON_ && _FW_PARTIAL_COMPRESS_==_FW_PARTIAL_COMPRESS_OFF_))
    if(!(gBinInfo.ld.LdCtrl & LDCF_PARTLOAD_EN))
    {
        DBG_DUMP("^RFW Full loaded, force used Safe mode.\r\n");
        //Full-load, Single-task
        System_SetState(SYS_STATE_POWERON, SYSTEM_POWERON_SAFE); //power-on = safe mode
        System_SetState(SYS_STATE_POWEROFF, SYSTEM_POWEROFF_SAFE); //power-off = safe mode
    }
#endif
//#NT#2016/02/25#Niven Cho -end

    /*
    //For Main code size debuging
    {
        extern void ScfFile_Debug(void);
        ScfFile_Debug();
    }*/
    System_OnIPC();
    // usb shoud be init here because System_OnPowerPreInit need to check if usb plug in
    System_OnUsbPreInit();
    System_OnPowerPreInit();
    ///////////////////////////////////////////
    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_SAFE)
    {
        //"safe power-on sequence"
        DBG_DUMP("Power On Sequence = Safe\r\n");
        System_OnStrgInit_EMBMEM();
        System_OnStrgInit_FWS();
        System_OnStrg_DownloadFW(UserSection_Order_full, UserSection_LoadCb);
        /////////////////////////////////
        System_OnTimerInit();
        System_OnOutputInit();
        /////////////////////////////////
        System_OnVideoInit();
        System_OnVideoInit2();
        System_OnVideoInit3();
        System_OnAudioInit();
        /////////////////////////////////
        System_OnStrgInit_EXMEM();
		//#NT#2016/12/14#Niven Cho -begin
        //#NT#EMMC_AS_PSTORE
#if (_CPU2_TYPE_==_CPU2_LINUX_ && _INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
		//in this case, PStore init moved to before Load_MenuInfo
		//bcz pstore use filesys.
#else
        System_OnStrgInit_PS();
#endif
		//#NT#2016/12/14#Niven Cho -end
        /////////////////////////////////
        #if(ONVIF_PROFILE_S!=ENABLE) //No File System
        System_OnStrgInit_FS();
        System_OnStrgInit_FS2();
        System_OnStrgInit_FS3();
        #endif
        /////////////////////////////////
        System_OnUsbInit();
        System_OnLensInit();
        System_OnSensorInit();
        System_OnSensorInit2();
        System_OnSensorInit3();
        System_OnPowerInit();
        System_OnInputInit();
    }
//#NT#2016/03/30#Jeah Yen -begin
//#NT#isolate fast boot code
#if (POWERON_FAST_BOOT == ENABLE)
    else if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
    {
        ///////////////////////////////////////////
        //Start Multi-UserMainProc

        INIT_SETFLAG(FLGINIT_BEGIN2);
        INIT_SETFLAG(FLGINIT_BEGIN3);
        INIT_SETFLAG(FLGINIT_BEGIN4);
        INIT_SETFLAG(FLGINIT_BEGIN5);
        INIT_SETFLAG(FLGINIT_BEGIN6);
        INIT_SETFLAG(FLGINIT_BEGIN7);
        INIT_SETFLAG(FLGINIT_BEGIN8);

        //"normal power-on sequence"
        DBG_DUMP("Power On Sequence = Normal\r\n");

        //PART-1

        //PART-2
        INIT_WAITFLAG(FLGINIT_LOADCODE2);  //wait until PART-2 finish
            System_OnTimerInit();
            System_OnOutputInit();

        //PART-3
        INIT_WAITFLAG(FLGINIT_LOADCODE3);  //wait until PART-3 finish
            System_OnPowerInit();
            System_OnInputInit();

        INIT_WAITFLAG(FLGINIT_END2);
        INIT_WAITFLAG(FLGINIT_END3);
        INIT_WAITFLAG(FLGINIT_END4);
        //INIT_WAITFLAG(FLGINIT_END5); //do not wait FS finish
        INIT_WAITFLAG(FLGINIT_END6);
        INIT_WAITFLAG(FLGINIT_END7);
        //INIT_WAITFLAG(FLGINIT_END8); //do not wait wifi finish
    }
#endif
//#NT#2016/03/30#Jeah Yen -end
    /*
    else if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_LOWPOWER)
    {
        //"lowpower power-on sequence"
        DBG_DUMP("Power On Sequence = LowPower\r\n");
    }
    else if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_CHARGE)
    {
        //"charge power-on sequence"
        DBG_DUMP("Power On Sequence = Charge\r\n");
    }
    */

    // Boot End
    DBG_MSG("^GInit End\r\n");
}

//#NT#2016/03/30#Jeah Yen -begin
//#NT#isolate fast boot code
#if (POWERON_FAST_BOOT == ENABLE)

void SystemInit2(void)
{
    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
    {
    //INIT_WAITFLAG(FLGINIT_PWROPEN); //wait until PWR dummyload finish
    DBG_MSG("^GInit 2 Start\r\n");
    System_OnStrgInit_EMBMEM();
    System_OnStrgInit_FWS();
    System_OnStrg_DownloadFW(UserSection_Order_full, UserSection_LoadCb);
//#NT#2016/02/25#Niven Cho -begin
//#NT#fine tune linux-cardv boot time
#if (_FW_COMPRESS_== _FW_COMPRESS_ON_ && _FW_PARTIAL_COMPRESS_==_FW_PARTIAL_COMPRESS_OFF_)
    INIT_SETFLAG(FLGINIT_LOADCODE1);
    INIT_SETFLAG(FLGINIT_LOADCODE2);
    INIT_SETFLAG(FLGINIT_LOADCODE3);
#endif
//#NT#2016/02/25#Niven Cho -end
    INIT_WAITFLAG(FLGINIT_LOADCODE3);
    System_OnStrgInit_PS();
    DBG_MSG("^GInit 2 End\r\n");
    }
}

void SystemInit3(void)
{
    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
    {
    //INIT_WAITFLAG(FLGINIT_PWROPEN); //wait until PWR dummyload finish
    DBG_MSG("^GInit 3 Start\r\n");
    INIT_WAITFLAG(FLGINIT_LOADCODE2);
    System_OnVideoInit();
    System_OnVideoInit2();
    INIT_WAITFLAG(FLGINIT_LOADCODE3);
    System_OnVideoInit3();
    DBG_MSG("^GInit 3 End\r\n");
    }
}

void SystemInit4(void)
{
    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
    {
    INIT_WAITFLAG(FLGINIT_LOADCODE3);
    //INIT_WAITFLAG(FLGINIT_PWROPEN); //wait until PWR dummyload finish
    DBG_MSG("^GInit 4 Start\r\n");
    System_OnAudioInit();
    DBG_MSG("^GInit 4 End\r\n");
    }
}

void SystemInit5(void)
{
    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
    {
    //INIT_WAITFLAG(FLGINIT_PWROPEN); //wait until PWR dummyload finish
    DBG_MSG("^GInit 5 Start\r\n");
    INIT_WAITFLAG(FLGINIT_LOADCODE2);
    System_OnStrgInit_EXMEM();
    System_OnStrgInit_FS();
    INIT_WAITFLAG(FLGINIT_LOADCODE3);
    System_OnStrgInit_FS2();
    INIT_WAITFLAG(FLGINIT_MOUNTFS);
    INIT_WAITFLAG(FLGINIT_MODE);
    System_OnStrgInit_FS3();
    DBG_MSG("^GInit 5 End\r\n");
    }
}

void SystemInit6(void)
{
    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
    {
    //INIT_WAITFLAG(FLGINIT_PWROPEN); //wait until PWR dummyload finish
    DBG_MSG("^GInit 6 Start\r\n");
    INIT_WAITFLAG(FLGINIT_LOADCODE3);
    System_OnUsbInit();
    DBG_MSG("^GInit 6 End\r\n");
    }
}

void SystemInit7(void)
{
    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
    {
    //INIT_WAITFLAG(FLGINIT_PWROPEN); //wait until PWR dummyload finish
    DBG_MSG("^GInit 7 Start\r\n");
    INIT_WAITFLAG(FLGINIT_LOADCODE2);
    System_OnSensorInit();
    System_OnSensorInit2();
    INIT_WAITFLAG(FLGINIT_LOADCODE3);
    System_OnLensInit();
    System_OnSensorInit3();
    DBG_MSG("^GInit 7 End\r\n");
    }
}

void System_WifiStart(void);

void SystemInit8(void)
{
    if(System_GetState(SYS_STATE_POWERON) == SYSTEM_POWERON_NORMAL)
    {
    //INIT_WAITFLAG(FLGINIT_PWROPEN); //wait until PWR dummyload finish
    DBG_MSG("^GInit 8 Start\r\n");
    INIT_WAITFLAG(FLGINIT_LOADCODE2);
    INIT_WAITFLAG(FLGINIT_LOADCODE3);
#if ((POWERON_FAST_CPU2_BOOT == ENABLE) && (_CPU2_TYPE_ == _CPU2_ECOS_))
    System_CPU2_WaitReady();
#endif
#if ((POWERON_FAST_WIFI == ENABLE) && (_CPU2_TYPE_ == _CPU2_ECOS_) && (_NETWORK_ != _NETWORK_NONE_))
    #if(POWERON_FAST_CPU2_BOOT != ENABLE)
    DBG_ERR("POWERON_FAST_WIFI is need to enable POWERON_FAST_CPU2_BOOT!\r\n");
    GxSystem_ShutdownNOW();
    #endif
    //BKG_PostEvent(NVTEVT_BKW_WIFI_ON);
    System_WifiStart();
#endif
    DBG_MSG("^GInit 8 End\r\n");
    }
}

#endif
//#NT#2016/03/30#Jeah Yen -end

