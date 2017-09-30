/*
    System Storage Callback

    System Callback for Storage Module.

    @file       SysStrg_Exe.c
    @ingroup    mIPRJSYS

    @note       這個檔案負責一件事

                1.接收Storage Event的介面
                  可能的event有:

                     STRG_INSERT --- 代表有CARD插入
                       執行unmount之前需要做的事情
                       呼叫File unmound (NAND),
                       執行mount之前需要做的事情
                       呼叫File mount (CARD)

                     STRG_REMOVE --- 代表有CARD拔出
                       執行unmount之前需要做的事情
                       呼叫File unmount (CARD)
                       執行mount之前需要做的事情
                       呼叫File_mount (NAND)

                     STRG_ATTACH --- 代表File mount完畢
                       執行mount之後需要做的事情
                       這邊會傳入mount的結果status

                     STRG_DETACH --- 代表File unmount完畢
                       執行unmount之後需要做的事情
                       這邊會傳入unmount的結果status


    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/

////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
////////////////////////////////////////////////////////////////////////////////
#include "UIFrameworkExt.h"
#include "AppLib.h"
#include "dma.h"
#include "DxType.h"
#include "Dx.h"
#include "DxApi.h"
#include "DxWiFi.h"

#include "DxStorage.h"
#include "PStore.h"
#include "DCF.h"
#include "GxFlash.h"
#include "ExifVendor.h"
#include "GxeCosApi.h"
#include "GxLinuxApi.h"
#include "GxDSP.h"
#include "BinInfo.h"
#include "FileSysTsk.h"
#include "CoreInfo.h"
#include "PartitionInfo.h"
#include "lz.h"
#include "StrgDef.h"
#include "HwClock.h"
#include "UIAppMovie.h"
#include "NvtIpcAPI.h"
#include "PstoreIpcAPI.h"
#include "MsdcNvtCb.h"
//#NT#2016/02/18#Lincy Lin -begin
//#NT#Support logfile function
#if (LOGFILE_FUNC==ENABLE)
#include "LogFile.h"
#endif
//#NT#2016/02/18#Lincy Lin -end
//#NT#2016/04/27#Lincy Lin -begin
//#NT#Support userlog function
#if (USERLOG_FUNC == ENABLE)
#include "userlog.h"
#endif
//#NT#2016/04/27#Lincy Lin -end
//#NT#2016/08/17#Niven Cho -begin
//#NT#without flash, ecos is required to run
#include "nvtpack.h"
//#NT#2016/08/17#Niven Cho -end
//#NT#2017/03/02#Niven Cho -begin
//#NT#EMMC_AS_PSTORE
#include "UICommon.h"
//#NT#2017/03/02#Niven Cho -end

#if (_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
#define MAX_BLK_PER_SEC         128
#define MAX_SEC_NUM             8
#else
#define MAX_BLK_PER_SEC         512
#define MAX_SEC_NUM             64
#endif
#define LDC_HEADER_SIZE         16

//#NT#2017/01/24#Niven Cho#[0116149] -begin
//#NT#fix linux-cardv cannot update fw.
#if (_CPU2_TYPE_ == _CPU2_LINUX_)
#define FW_UPD_FW_TMP_MEM_SIZE  0x1E00000
#else
#define FW_UPD_FW_TMP_MEM_SIZE  0xA00000
#endif
//#NT#2017/01/24#Niven Cho#[0116149] -end

//#NT#2016/02/17#Niven Cho -begin
//#NT#Mechanism for Fw Updating Fw failed.
#define LOADER_UPD_FW_PATH "A:\\"_BIN_NAME_".BIN"
#define FW_DEL_INDIACTION_PATH "A:\\NVTDELFW"
//#NT#2016/02/17#Niven Cho -end

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
//local debug level: THIS_DBGLVL
#if defined(YQCONFIG_UART_TO_MTK_SUPPORT)
#include "UART_TO_MTK.h"
#endif
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysStrgExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////
//#NT#2016/08/01#Niven Cho -begin
//#NT#FIX compiler warning with _INTERSTORAGE_ == _INTERSTORAGE_NONE_

//#NT#2016/02/26#Niven Cho -begin
//#NT#eCos debug msg alway show on uITRON via IPC.
//Check definition options
#if (_CPU2_TYPE_ == _CPU2_ECOS_)
#if (LOGFILE_FUNC == ENABLE && ECOS_DBG_MSG_FORCE_UART1_DIRECT == ENABLE)
#error "LOGFILE_FUNC must turn off ECOS_DBG_MSG_FORCE_UART1_DIRECT"
#endif
#endif
//#NT#2016/02/26#Niven Cho -end

//#NT#2016/12/20#Niven Cho#[114852] -begin
//#NT#FIX Linux update F.W failed. (because of filesys' buffer get smaller on linux)
#define UITRON_FS_WORKING_BUF_SIZE (ALIGN_CEIL_32(0xAC000)+ALIGN_CEIL_32(0x80020))
//#NT#2016/12/20#Niven Cho#[114852] -end

extern BININFO gBinInfo;
static GXSTRG_FS_TYPE m_GxStrgType = GXSTRG_FS_TYPE_UITRON;
//#if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
static BOOL gbFirstPoweron = TRUE;
BOOL StrgStateChangeToMTKFlag=FALSE;
//#endif
///////////////////////////////////////////////////////////////////////////////
//
//  EMBMEM
//
///////////////////////////////////////////////////////////////////////////////
//Caculate FAT Start Addr

//#NT#2016/08/17#Niven Cho -begin
//#NT#without flash, ecos is required to run
#if (_CPU2_TYPE_ == _CPU2_ECOS_)
static BOOL xSysStrg_eCosRun(void);
#endif
#if (_CPU2_TYPE_ == _CPU2_LINUX_)
static BOOL xSysStrg_LinuxRun(void);
#endif
#if (_DSP_TYPE_ == _DSP_FREERTOS_)
static BOOL xSysStrg_DspRun(void);
#endif
//#NT#2016/08/17#Niven Cho -end

UINT32 System_OnStrgInit_EMBMEM_GetGxStrgType(void)
{
    return m_GxStrgType;
}

void System_OnStrgInit_EMBMEM(void)
{
    TM_BOOT_BEGIN("nand","init_embmem");
#if (_INTERSTORAGE_ != _INTERSTORAGE_NONE_)
//#NT#2016/12/07#Niven Cho -begin
//#NT#EMMC_PARTITION_BY_BLOCK
#if (_INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
#define SIZE_UNIT _EMBMEM_BLK_SIZE_ //block alignment
#else
#define SIZE_UNIT 1 //byte alignment
#endif
    static BOOL bStrg_init_EMBMEM = FALSE;
    UINT32 i=0;
    if(bStrg_init_EMBMEM)
        return;

    //check partition overlap
    //part 0(FW area) must match loader reserved size,and next area can not overlap previos area
    EMB_PARTITION* p = (EMB_PARTITION*)(OS_GetMemAddr(MEM_TOTAL)+EMB_PARTITION_INFO_OFFSET);

    if(p->EmbType != EMBTYPE_UITRON)
    {
        DBG_ERR("First partition must be uITRON\r\n");
        return;
    }
    else
    {

		//#NT#2016/12/07#Niven Cho -begin
        //#NT#EMMC_PARTITION_BY_BLOCK
        if(p->PartitionSize*SIZE_UNIT<gBinInfo.ld.FWResvSize)
        {
           DBG_ERR("part0 %x < FWResvSize %x,cannot init\r\n",p->PartitionSize,gBinInfo.ld.FWResvSize);
           return;
        }
		//#NT#2016/12/07#Niven Cho -end
    }

    for(i=0;i<EMB_PARTITION_INFO_COUNT;i++)
    {
        BOOL bNeedInit = FALSE;
        DXSTRG_INIT UserEmbMemInit ={0};
        UserEmbMemInit.buf.Addr = OS_GetMempoolAddr(POOL_ID_STORAGE_NAND);
        UserEmbMemInit.buf.Size = OS_GetMempoolSize(POOL_ID_STORAGE_NAND);
        UserEmbMemInit.prt.uiDxClassType = DX_CLASS_STORAGE_EXT;
        UserEmbMemInit.prt.uiPhyAddr = p->PartitionOffset;
        UserEmbMemInit.prt.uiPhySize = p->PartitionSize;
        UserEmbMemInit.prt.uiResvSize = p->ReversedSize;

        switch(p->EmbType)
        {
            case EMBTYPE_UITRON:
                bNeedInit = TRUE;
                UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_UITRON;
                UserEmbMemInit.prt.uiResvSize = gBinInfo.ld.LdResvSize; //loader size defined on loader, but partitioninfo.c
                DBG_IND("EMBTYPE_UITRON\r\n");
                break;

            case EMBTYPE_ECOS:
                bNeedInit = TRUE;
                UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_ECOS;
                DBG_IND("EMBTYPE_ECOS\r\n");
                break;

            case EMBTYPE_UBOOT:
                bNeedInit = TRUE;
                UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_UBOOT;
                DBG_IND("EMBTYPE_UBOOT\r\n");
                break;

            case EMBTYPE_LINUX:
                bNeedInit = TRUE;
                UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_LINUX;
                DBG_IND("EMBTYPE_LINUX\r\n");
                break;

            case EMBTYPE_RAMFS:
                bNeedInit = TRUE;
                UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_ROOTFS;
                DBG_IND("EMBTYPE_RAMFS\r\n");
                break;

            case EMBTYPE_DSP:
                bNeedInit = TRUE;
                UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_DSP;
                DBG_IND("EMBTYPE_DSP\r\n");
                break;

#if(_INTERSTORAGE_ != _INTERSTORAGE_SPI_)
            case EMBTYPE_FAT:
            case EMBTYPE_EXFAT:
                bNeedInit = TRUE;
                UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_FAT;
                DBG_IND("EMBTYPE_FAT\r\n");
                break;
#endif

            case EMBTYPE_PSTORE:
                bNeedInit = TRUE;
                UserEmbMemInit.prt.uiDxClassType |= USER_DX_TYPE_EMBMEM_PSTORE;
                DBG_IND("EMBTYPE_PSTORE\r\n");
                break;
        }

        if(bNeedInit)
        {
            DX_HANDLE DxNandDev = Dx_GetObject(UserEmbMemInit.prt.uiDxClassType);
            Dx_Init(DxNandDev,&UserEmbMemInit,0,STORAGE_VER);
        }
        p++;
    }

#if(_INTERSTORAGE_ == _INTERSTORAGE_SPI_) //if stoarge is SPI, use ram disk as internal FAT
    {
        DXSTRG_INIT UserEmbMemInit ={0};
        UserEmbMemInit.prt.uiDxClassType = DX_CLASS_STORAGE_EXT|USER_DX_TYPE_EMBMEM_FAT;
        UserEmbMemInit.buf.Addr = OS_GetMempoolAddr(POOL_ID_STORAGE_RAMDISK);
        UserEmbMemInit.buf.Size = OS_GetMempoolSize(POOL_ID_STORAGE_RAMDISK);
        DX_HANDLE DxNandDev = Dx_GetObject(UserEmbMemInit.prt.uiDxClassType);
        Dx_Init(DxNandDev,&UserEmbMemInit,0,STORAGE_VER);
    }
#endif

    bStrg_init_EMBMEM = TRUE;
#endif
    TM_BOOT_END("nand","init_embmem");
}

void System_OnStrgExit_EMBMEM(void)
{
    //PHASE-1 : Close Drv or DrvExt
}

///////////////////////////////////////////////////////////////////////////////
//
//  EXMEM
//
///////////////////////////////////////////////////////////////////////////////
//Drv or DrvExt
DXSTRG_INIT UserSdioInit;
#if(COPYCARD2CARD_FUNCTION == ENABLE)
DXSTRG_INIT UserSdio2Init;
#endif
//extern SX_CMD_ENTRY Cmd_card[]; //device
DX_HANDLE DxCardDev1 = 0;
//#NT#2016/12/06#Niven Cho -begin
//#NT#MULTI_DRIVE
typedef enum _BOOT_CARD_STATE {
	BOOT_CARD_STATE_UNKNOWN,
	BOOT_CARD_STATE_INSERTED,
	BOOT_CARD_STATE_REMOVED,
	ENUM_DUMMY4WORD(BOOT_CARD_STATE)
} BOOT_CARD_STATE;
//#NT#2016/12/09#Niven Cho -begin
//#NT#LINUX-EMMC-MULTI-STRG
static BOOT_CARD_STATE m_BootState_Drive[16] = {BOOT_CARD_STATE_UNKNOWN}; //DriveA, DriveB
static UINT32 m_FsDxTypeMap[2] = {FS_DX_TYPE_DRIVE_A, FS_DX_TYPE_DRIVE_B};
//#NT#2016/12/09#Niven Cho -end
//#NT#2016/12/06#Niven Cho -end

void System_OnStrgInit_EXMEM(void)
{
    static BOOL bStrg_init_EXMEM = FALSE;
    if(bStrg_init_EXMEM)
        return;
    TM_BOOT_BEGIN("sdio","init");

//#NT#2016/12/06#Niven Cho -begin
//#NT#MULTI_DRIVE
#if (FS_DX_TYPE_DRIVE_A >= DX_TYPE_CARD1 && FS_DX_TYPE_DRIVE_A <= DX_TYPE_CARD3)
    DX_HANDLE DxCardDev1 = Dx_GetObject(DX_CLASS_STORAGE_EXT|FS_DX_TYPE_DRIVE_A);
    UserSdioInit.buf.Addr = OS_GetMempoolAddr(POOL_ID_STORAGE_SDIO);
    UserSdioInit.buf.Size = OS_GetMempoolSize(POOL_ID_STORAGE_SDIO);
    Dx_Init(DxCardDev1,&UserSdioInit,0,STORAGE_VER);
#endif

#if (FS_MULTI_STRG_FUNC && FS_DX_TYPE_DRIVE_B >= DX_TYPE_CARD1 && FS_DX_TYPE_DRIVE_B <= DX_TYPE_CARD3)
    DX_HANDLE DxCardDev2 = Dx_GetObject(DX_CLASS_STORAGE_EXT|FS_DX_TYPE_DRIVE_B);
    UserSdio2Init.buf.Addr = OS_GetMempoolAddr(POOL_ID_STORAGE_SDIO2);
    UserSdio2Init.buf.Size = OS_GetMempoolSize(POOL_ID_STORAGE_SDIO2);
    Dx_Init(DxCardDev2,&UserSdio2Init,0,STORAGE_VER);
#endif
//#NT#2016/12/06#Niven Cho -end

    bStrg_init_EXMEM = TRUE;
    TM_BOOT_END("sdio","init");
}

void System_OnStrgExit_EXMEM(void)
{
    //PHASE-1 : Close Drv or DrvExt

}

///////////////////////////////////////////////////////////////////////////////
//
//  FILESYS
//
///////////////////////////////////////////////////////////////////////////////
//Lib or LibExt
extern void Strg_CB(UINT32 event, UINT32 param1, UINT32 param2);
void Card_DetInsert(void);
void Card_DetBusy(void);
#if (SDINSERT_FUNCTION == ENABLE)
SX_TIMER_ITEM(Card_DetInsert, Card_DetInsert, 2, FALSE)
#endif
SX_TIMER_ITEM(System_DetBusy, Card_DetBusy, 25, FALSE)
int SX_TIMER_DET_STRG_ID = -1;
int SX_TIMER_DET_SYSTEM_BUSY_ID = -1;

void System_OnStrgInit_FS(void)
{
    TM_BOOT_BEGIN("sdio","init_fs");
    {
        MEM_RANGE Pool;
        Pool.Addr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_FS_BUFFER));
		//#NT#2016/12/09#Niven Cho -begin
        //#NT#LINUX-EMMC-MULTI-STRG
        #if (FS_MULTI_STRG_FUNC == ENABLE)
        MEM_RANGE Pool2;
        Pool.Size = POOL_SIZE_FS_BUFFER;
        GxStrg_SetConfigEx(0, FILE_CFG_BUF, (UINT32)&Pool);
        Pool2.Addr = Pool.Addr + POOL_SIZE_FS_BUFFER;
        Pool2.Size = POOL_SIZE_FS_BUFFER;
        GxStrg_SetConfigEx(1, FILE_CFG_BUF, (UINT32)&Pool2);
		#if (_CPU2_TYPE_==_CPU2_LINUX_ && _INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
		MEM_RANGE Pool3;  // 3rd is for linux-pstore mounted by filesys
		Pool3.Addr = Pool2.Addr + POOL_SIZE_FS_BUFFER;
        Pool3.Size = POOL_SIZE_FS_BUFFER;
		//#NT#2016/12/14#Niven Cho -begin
        //#NT#EMMC_AS_PSTORE
        GxStrg_SetConfigEx(PST_DEV_ID, FILE_CFG_BUF, (UINT32)&Pool3);
		//#NT#2016/12/14#Niven Cho -end
		#endif
        #else
        Pool.Size = OS_GetMempoolSize(POOL_ID_FS_BUFFER);
        GxStrg_SetConfigEx(0, FILE_CFG_BUF, (UINT32)&Pool);
        #endif
		//#NT#2016/12/09#Niven Cho -end
    }

    GxStrg_SetConfigEx(0, FILE_CFG_FS_TYPE,m_GxStrgType);
    GxStrg_SetConfigEx(0, FILE_CFG_MOUNT_PATH,(UINT32)"/mnt/sd");
    //#NT#2016/02/18#Lincy Lin -begin
    //#NT#Support logfile function
    #if (LOGFILE_FUNC==ENABLE)
    GxStrg_SetConfigEx(0, FILE_CFG_MAX_OPEN_FILE,6);
    #endif
    //#NT#2016/02/18#Lincy Lin -end
    //#NT#2016/04/27#Lincy Lin -begin
    //#NT#Support userlog function
    #if (USERLOG_FUNC == ENABLE)
    GxStrg_SetConfigEx(0, FILE_CFG_MAX_OPEN_FILE,6);
    #endif
    //#NT#2016/04/27#Lincy Lin -end
    //#NT#2016/04/21#Lincy Lin -begin
    //#NT#Support curl (http client)
    #if (CURL_FUNC == ENABLE)
    GxStrg_SetConfigEx(0, FILE_CFG_MAX_OPEN_FILE,8);
    #endif
    //#NT#2016/04/21#Lincy Lin -end

    //#NT#2016/06/30#Isiah Chang -begin
    // Enable 32MB alignment recording. Default disable.
    // GxStrg_SetConfigEx(0, FILE_CFG_ALIGNED_SIZE, 32*1024*1024);
    //#NT#2016/06/30#Isiah Chang -end

	//#NT#2016/12/06#Niven Cho -begin
    //#NT#MULTI_DRIVE
    #if (FS_MULTI_STRG_FUNC)
    GxStrg_SetConfigEx(1, FILE_CFG_FS_TYPE,m_GxStrgType);
    GxStrg_SetConfigEx(1, FILE_CFG_MOUNT_PATH,(UINT32)"/mnt/sd");
    #endif
	//#NT#2016/12/06#Niven Cho -end

    GxStrg_RegCB(Strg_CB);         //Register CB function of GxStorage (NANR or CARD)
    {
        //1.設定init值
        //FileSys:
        //2.設定CB值,
        //3.註冊SxJob服務 ---------> System Job
        //4.註冊SxTimer服務 ---------> Detect Job
        #if (SDINSERT_FUNCTION == ENABLE)
        if(m_GxStrgType == GXSTRG_FS_TYPE_UITRON)
        {
            SX_TIMER_DET_STRG_ID = SxTimer_AddItem(&Timer_Card_DetInsert);
        }
        #endif
        //SX_TIMER_DET_SYSTEM_BUSY_ID = SxTimer_AddItem(&Timer_System_DetBusy);
        //5.註冊SxCmd服務 ---------> Cmd Function
        //System_AddSxCmd(Storage_OnCommand); //GxStorage

        //start scan
        SxTimer_SetFuncActive(SX_TIMER_DET_STRG_ID, TRUE);
        SxTimer_SetFuncActive(SX_TIMER_DET_SYSTEM_BUSY_ID, TRUE);
#if 0
        #if (SDINSERT_FUNCTION == ENABLE)
        {
            DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1);
            //提早detect storage可以得到兩個好處:
            //1.知道storage的type是哪一種
            //2.立刻"啟動File System"
            //  如果沒這樣, 就要等到SXOPEN之後, SX_TIMER_DET_STRG_ID開始跑才會"啟動File System",
            //  與其他task的競爭結果, 容易造成"啟動File System"的timing不固定
            GxStrg_Det((UINT32)pStrgDev);
            GxStrg_Det((UINT32)pStrgDev);
            Delay_DelayMs(30);
        }
        #endif
#endif
    }
    TM_BOOT_END("sdio","init_fs");

	//#NT#2016/12/06#Niven Cho -begin
    //#NT#MULTI_DRIVE
    if(m_GxStrgType == GXSTRG_FS_TYPE_UITRON)
    {
		#if (FS_MULTI_STRG_FUNC)
		UINT32 uiDxState=0;
		DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|FS_DX_TYPE_DRIVE_B);
        if (Dx_GetState((DX_HANDLE)pStrgDev, STORAGE_STATE_INSERT,&uiDxState)!=DX_OK || uiDxState==FALSE)
        {
            Ux_PostEvent(NVTEVT_STRG_REMOVE, 1, 1);
        }
		else
		{
			//temp solution to wait GxStrg support multi drive
			Ux_PostEvent(NVTEVT_STRG_INSERT, 1, 1);
		}
		#endif
    }
	//#NT#2016/12/06#Niven Cho -end

    {
        MEM_RANGE WorkBuf;
        WorkBuf.Addr = OS_GetMempoolAddr(POOL_ID_EXIF);
        WorkBuf.Size = POOL_SIZE_EXIF;
        EXIF_Init(EXIF_HDL_ID_1, &WorkBuf, ExifCB);
        #if (_SENSORLIB2_ != _SENSORLIB2_OFF_)
        WorkBuf.Addr += POOL_SIZE_EXIF;
        EXIF_Init(EXIF_HDL_ID_2, &WorkBuf, ExifCB);
        #endif
        #if (_SENSORLIB3_ != _SENSORLIB3_OFF_)
        WorkBuf.Addr += POOL_SIZE_EXIF;
        EXIF_Init(EXIF_HDL_ID_3, &WorkBuf, ExifCB);
        #endif
        #if (_SENSORLIB4_ != _SENSORLIB4_OFF_)
        WorkBuf.Addr += POOL_SIZE_EXIF;
        EXIF_Init(EXIF_HDL_ID_4, &WorkBuf, ExifCB);
        #endif
    }
    {
        // init DCF

        CHAR pFolderName[9]={0};
        CHAR pFileName[5]={0};
        // init DCF FolderID/FileID with RTC data
        struct tm tm_cur=HwClock_GetTime(TIME_ID_CURRENT);
        snprintf(pFolderName,sizeof(pFolderName),"%1d%02d%02d",tm_cur.tm_year%0x0A,tm_cur.tm_mon,tm_cur.tm_mday);
        snprintf(pFileName,sizeof(pFileName),"%02d%02d",tm_cur.tm_hour,tm_cur.tm_min);
        //DCF dir-name
        DCF_SetDirFreeChars( pFolderName );
        //DCF file-name
        DCF_SetFileFreeChars( DCF_FILE_TYPE_ANYFORMAT, pFileName);

        //DCF format
        DCF_SetParm(DCF_PRMID_SET_VALID_FILE_FMT, DCF_SUPPORT_FORMAT);
        DCF_SetParm(DCF_PRMID_SET_DEP_FILE_FMT, DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_WAV|DCF_FILE_TYPE_MPO);
        //TODO: [DCF] How to add an new format & its ext?
    }
}
void System_OnStrgInit_FS2(void)
{
    // update card status again
    if (GxStrg_GetDeviceCtrl(0, CARD_READONLY))
    {
        UI_SetData(FL_CardStatus, CARD_LOCKED);
    }
    else if (GxStrg_GetDeviceCtrl(0, CARD_INSERT))
    {
        UI_SetData(FL_CardStatus, CARD_INSERTED);
    }
    else
    {
        UI_SetData(FL_CardStatus, CARD_REMOVED);
    }
}
void System_OnStrgInit_FS3(void)
{
    static BOOL bFirst = TRUE;
    if(bFirst){TM_BOOT_BEGIN("sdio","fdb_create");}
    INT32 iCurrMode = System_GetBootFirstMode();
    if(iCurrMode == PRIMARY_MODE_MOVIE)
    {
        FlowMovie_FileDBCreate_Fast();
    }
    if(bFirst){TM_BOOT_END("sdio","fdb_create");}
    bFirst = FALSE;
}

void System_OnStrgExit_FS(void)
{
    //PHASE-2 : Close Lib or LibExt
    //#NT#2016/02/18#Lincy Lin -begin
    //#NT#Support logfile function
    #if (LOGFILE_FUNC==ENABLE)
    LogFile_Close();
    #endif
    //#NT#2016/02/18#Lincy Lin -end
    {
        GxStrg_CloseDevice(0); // unmount file system
    }
	//#NT#2016/12/06#Niven Cho -begin
    //#NT#MULTI_DRIVE
    #if (FS_MULTI_STRG_FUNC)
	GxStrg_CloseDevice(1);
	#endif
    //#NT#2016/12/06#Niven Cho -end

    #if(_CPU2_TYPE_ == _CPU2_LINUX_)
    GxLinux_Close();
    #endif
}

//# add init DCF Dir/File ID
//# add detect file system status
#if 0
static void FileSys_SetInitNumbering(void)
{
    UINT32     DCF_DirId=0, DCF_FileId=0;

    DCF_GetNextID(&DCF_DirId,&DCF_FileId);

    if (DCF_DirId < UI_GetData(FL_DCF_DIR_ID))
    {
        DCF_DirId = UI_GetData(FL_DCF_DIR_ID);
        DCF_FileId = UI_GetData(FL_DCF_FILE_ID);
    }
    else if(DCF_DirId == UI_GetData(FL_DCF_DIR_ID) && DCF_FileId < UI_GetData(FL_DCF_FILE_ID))
    {
        DCF_FileId = UI_GetData(FL_DCF_FILE_ID);
    }
    DCF_SetNextID(DCF_DirId,DCF_FileId);
}
#endif

static BOOL FileSys_DetBusy(void)
{
    return (BOOL)((INT32)FileSys_GetParam(FST_PARM_TASK_STS, 0) == FST_STA_BUSY);
}

void Card_DetInsert(void)
{
    //#NT#2016/12/06#Niven Cho -begin
    //#NT#MULTI_DRIVE
    GxStrg_Det(0, Dx_GetObject(DX_CLASS_STORAGE_EXT|FS_DX_TYPE_DRIVE_A));
	#if (0)//FS_MULTI_STRG_FUNC)
	GxStrg_Det(1, Dx_GetObject(DX_CLASS_STORAGE_EXT|FS_DX_TYPE_DRIVE_B));
	#endif
    //#NT#2016/12/06#Niven Cho -end
}

void Card_DetBusy(void)
{
    static BOOL bBusyLED = FALSE;
    //static BOOL bIsSendingStorageInit = FALSE;

    if(GxPower_GetControl(GXPWR_CTRL_SLEEP_LEVEL) <=1 && (!GxPower_GetControl(GXPWR_CTRL_BATTERY_CHARGE_EN)))
    {
        if (FileSys_DetBusy() || GxFlash_IsCharging() == TRUE)
        {
            if (bBusyLED == FALSE)
            {
                DBG_IND("System - busy\r\n");
                bBusyLED = TRUE;
                //GxLED_SetCtrl(KEYSCAN_LED_RED,SETLED_SPEED,GXLED_1SEC_LED_TOGGLE_CNT/5);
                //GxLED_SetCtrl(KEYSCAN_LED_RED,SET_TOGGLE_LED,TRUE);
                GxLED_SetCtrl(KEYSCAN_LED_GREEN,SETLED_SPEED,GXLED_1SEC_LED_TOGGLE_CNT/5);
                GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,FALSE);
                GxLED_SetCtrl(KEYSCAN_LED_GREEN,SET_TOGGLE_LED,TRUE);
            }
        }
        else
        {
            if (bBusyLED == TRUE)
            {
                DBG_IND("System - not busy\r\n");
                bBusyLED = FALSE;
                //GxLED_SetCtrl(KEYSCAN_LED_RED,SET_TOGGLE_LED,FALSE);
                //GxLED_SetCtrl(KEYSCAN_LED_RED,TURNON_LED,FALSE);
                GxLED_SetCtrl(KEYSCAN_LED_GREEN,SET_TOGGLE_LED,FALSE);
                GxLED_SetCtrl(KEYSCAN_LED_GREEN,TURNON_LED,FALSE);
            }
        }
    }
}

//#NT#2016/02/26#Niven Cho -begin
//#NT#eCos debug msg alway show on uITRON via IPC.
//#NT#2016/02/26#Niven Cho -end

//Device flow event
//#NT#2016/12/06#Niven Cho -begin
//#NT#MULTI_DRIVE
INT32 System_OnStrgInsert(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 stg_id = paramArray[0];
#if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
	UINT8   uiResqData[16]={0};
        if(gbFirstPoweron==FALSE)
        {
		Display_SetEnable(LAYER_VDO1, FALSE);
        	Display_SetEnable(LAYER_OSD1, FALSE);
        }
#endif
	if (m_BootState_Drive[stg_id] != BOOT_CARD_STATE_UNKNOWN)
	{
		if(stg_id == 0)
		{
#if defined(YQCONFIG_UART_TO_MTK_SUPPORT)
                StrgStateChangeToMTKFlag=TRUE;
                CHKPNT;
#endif
			#if(IPCAM_FUNC==DISABLE && SDHOTPLUG_FUNCTION == DISABLE)
			System_PowerOff(SYS_POWEROFF_NORMAL);
			#endif
		}
	}
	else
	{
		TM_BOOT_BEGIN("sdio","mount_fs");
		m_BootState_Drive[stg_id] = BOOT_CARD_STATE_INSERTED;
	}

#if(_CPU2_TYPE_ == _CPU2_LINUX_)
	GxStrg_SetConfigEx(stg_id, FILE_CFG_MOUNT_PATH,GxStrg_GetDeviceCtrl(stg_id, CARD_MNTPATH));
#endif

	//#NT#2016/12/14#Niven Cho -begin
    //#NT#EMMC_AS_PSTORE
	if(stg_id == PST_DEV_ID) // linux partition as PStore
	{
#if(_CPU2_TYPE_ == _CPU2_LINUX_)
		if (GxStrg_OpenDevice(stg_id, NULL) != TRUE)
	    {
	        DBG_ERR("Storage mount pstore fail\r\n");
	    }
		return NVTEVT_CONSUME;
#else
		DBG_FATAL("stg_id cannot be %d.\r\n", PST_DEV_ID);
#endif
	}
	//#NT#2016/12/14#Niven Cho -end

	DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|m_FsDxTypeMap[stg_id]);
    if (GxStrg_OpenDevice(stg_id, pStrgDev)!= TRUE)
    {
        char* pDxName="unknown";
        Dx_GetInfo(pStrgDev, DX_INFO_NAME,&pDxName);
        DBG_ERR("Storage mount %s fail\r\n",pDxName);
		return NVTEVT_CONSUME;
    }

	#if !USE_FILEDB
    {
        DCF_OPEN_PARM           dcfParm = {0};
        // Open DCF
        dcfParm.Drive = (stg_id==0) ? 'A' : 'B';
        dcfParm.WorkbuffAddr = dma_getCacheAddr(OS_GetMempoolAddr(POOL_ID_DCF_BUFFER));
        dcfParm.WorkbuffSize = POOL_SIZE_DCF_BUFFER;
        DCF_Open(&dcfParm);
        DCF_ScanObj();
    }
    #endif

	if (GxStrg_GetDeviceCtrl(stg_id, CARD_READONLY))
    {
        UI_SetData(FL_CardStatus, CARD_LOCKED);
        DBG_IND("Card Locked\r\n");
    }
    else
    {
        UI_SetData(FL_CardStatus, CARD_INSERTED);
        DBG_IND("Card inserted\r\n");
    }
    return NVTEVT_CONSUME;
}
//#NT#2016/12/06#Niven Cho -end

//#NT#2016/12/06#Niven Cho -begin
//#NT#MULTI_DRIVE
INT32 System_OnStrgRemove(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	UINT32 stg_id = paramArray[0];
    #if (FS_MULTI_STRG_FUNC == DISABLE)
    DX_HANDLE pStrgDevOld = GxStrg_GetDevice(0);
    #endif

	if (m_BootState_Drive[stg_id] != BOOT_CARD_STATE_UNKNOWN)
	{
		if(stg_id == 0)
		{
			//#NT#2016/02/23#Lincy Lin -begin
		    //#NT#Support logfile function
		    #if (LOGFILE_FUNC==ENABLE)
		    LogFile_Suspend();
		    LogFile_Close();
		    #endif
		    //#NT#2016/02/23#Lincy Lin -end
		    //#NT#2016/04/27#Lincy Lin -begin
		    //#NT#Support userlog function
		    #if (USERLOG_FUNC == ENABLE)
		    {
		        userlog_close();
		    }
		    #endif
		    //#NT#2016/04/27#Lincy Lin -end
		    //#NT#2016/08/31#Lincy Lin -begin
	        //#NT# Fix the error "DCF_GetInfoByHandle() Dcf Handle 0 Data may be overwritted" when card plug out/in
	        #if !USE_FILEDB
	        DCF_Close(0);
	        #endif
	        //#NT#2016/08/31#Lincy Lin -end
	        UI_SetData(FL_CardStatus, CARD_REMOVED);

		    GxStrg_CloseDevice(stg_id);
#if defined(YQCONFIG_UART_TO_MTK_SUPPORT)
                StrgStateChangeToMTKFlag=TRUE;
                CHKPNT;
#endif
			#if(IPCAM_FUNC==DISABLE && SDHOTPLUG_FUNCTION == DISABLE)
			System_PowerOff(SYS_POWEROFF_NORMAL);
			#endif
		}
	}
	else
	{
		TM_BOOT_BEGIN("sdio","mount_fs");
		m_BootState_Drive[stg_id] = BOOT_CARD_STATE_REMOVED;
		//#NT#2017/03/07#Niven Cho[117156] -begin
	    //#NT#FS_SWITCH_STRG_FUNC
		#if (FS_SWITCH_STRG_FUNC)
	    if (stg_id==0)
		{
			DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|FS_DX_TYPE_DRIVE_B);
	        if (GxStrg_OpenDevice(stg_id, pStrgDev)!= TRUE)
		    {
		        char* pDxName="unknown";
		        Dx_GetInfo(pStrgDev, DX_INFO_NAME,&pDxName);
		        DBG_ERR("Storage mount %s fail\r\n",pDxName);
				return NVTEVT_CONSUME;
		    }
			UI_SetData(FL_CardStatus, CARD_INSERTED);
			return NVTEVT_CONSUME;
	    }
		#else
		//#NT#2016/12/22#Niven Cho -begin
        //#NT#MULTI_DRIVE, on linux, fix filedb create fail on drive B.
		// boot without card, send attach to continue UI flow.
		// because of UserWaitEvent(NVTEVT_STRG_ATTACH, &paramNum, paramArray);
	    Ux_PostEvent(NVTEVT_STRG_ATTACH, 2, stg_id, 0xFF);
		//#NT#2016/12/22#Niven Cho -end
		#endif
		//#NT#2017/03/07#Niven Cho[117156] -end
	}

    #if (FS_MULTI_STRG_FUNC == ENABLE)
	DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|m_FsDxTypeMap[stg_id]);
    if (GxStrg_CloseDevice(stg_id)!= TRUE)
    {
        char* pDxName="unknown";
        Dx_GetInfo(pStrgDev, DX_INFO_NAME,&pDxName);
        DBG_ERR("Storage mount %s fail\r\n",pDxName);
		return NVTEVT_CONSUME;
    }
    #else
    if(pStrgDevOld == 0)
    {
        DX_HANDLE pStrgDevNew = Dx_GetObject(DX_CLASS_STORAGE_EXT|USER_DX_TYPE_EMBMEM_FAT);
        if (GxStrg_OpenDevice(0, pStrgDevNew) != TRUE)
        {
            char* pDxName="unknown";
            Dx_GetInfo(pStrgDevNew, DX_INFO_NAME,&pDxName);
            DBG_ERR("Storage mount %s fail\r\n", pDxName);
            return NVTEVT_CONSUME;
        }
    }
    #endif

    return NVTEVT_CONSUME;
}
//#NT#2016/12/06#Niven Cho -end

INT32 System_OnStrgAttach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
	//#NT#2017/03/02#Niven Cho -begin
    //#NT#EMMC_AS_PSTORE
    UINT32 stg_id = paramArray[0];
    UINT32 result = paramArray[1];
    if(m_GxStrgType == GXSTRG_FS_TYPE_LINUX)
    {
        //#NT#2016/03/02#Niven Cho -begin
        //#NT#Fix FAST_BOOT + LINUX + without card, boot failed.
        //Do Nothing
        //#NT#2016/03/02#Niven Cho -end
    }
    else //m_GxStrgType == GXSTRG_FS_TYPE_UITRON
    {
        //if spi use RamDisk as inner Memory,need to format RamDisk
        #if ((_INTERSTORAGE_ == _INTERSTORAGE_SPI_) && (FS_MULTI_STRG_FUNC == DISABLE))
        DX_HANDLE pStrgDevCur = (DX_HANDLE)GxStrg_GetDevice(0);
        DX_HANDLE pStrgDevRAM = (DX_HANDLE)Dx_GetObject(DX_CLASS_STORAGE_EXT|USER_DX_TYPE_EMBMEM_FAT);

        if((pStrgDevCur == pStrgDevRAM) && (result!=FST_STA_OK) && (stg_id == 0))
        {
            result = FileSys_FormatDisk(pStrgDevCur, TRUE);
        }
        #elif ((_INTERSTORAGE_ != _INTERSTORAGE_SPI_) && (FS_MULTI_STRG_FUNC == ENABLE))
        if(stg_id!=0) // stg_id=1 is interal storage FAT
        {
            return NVTEVT_CONSUME;
        }
        #endif
    }
	//#NT#2017/03/02#Niven Cho -end

    switch(result)
    {
    case FST_STA_OK:
		//#NT#2017/03/02#Niven Cho -begin
        //#NT#EMMC_AS_PSTORE
		if (stg_id == 0) // A:\\ existing
		{
	        //#NT#2013/03/14#Lincy Lin -begin
	        //#NT#Move out from GxStrg
	        #if !USE_FILEDB
	        if (!UI_GetData(FL_IsCopyToCarding))
	        {
	            DCF_ScanObj();
	        }
	        #endif
	        //#NT#2013/03/14#Lincy Lin -end
	        UI_SetData(FL_FSStatus, FS_INIT_OK);
	        //#NT#2016/02/17#Niven Cho -begin
	        //#NT#Mechanism for Fw Updating Fw failed.
	        if(gBinInfo.ld.LdCtrl2 & LDCF_UPDATE_FW)
	        {
	            FST_FILE hFile = FileSys_OpenFile(FW_DEL_INDIACTION_PATH,FST_OPEN_READ|FST_OPEN_EXISTING);
	            if(hFile != NULL)
	            {
	                DBG_DUMP("Detected %s, delete %s \r\n",FW_DEL_INDIACTION_PATH,LOADER_UPD_FW_PATH);
	                FileSys_CloseFile(hFile);
	                // Delete FW bin from A:
	                if(FileSys_DeleteFile(LOADER_UPD_FW_PATH)!=FST_STA_OK)
	                {
	                    DBG_ERR("delete "_BIN_NAME_".BIN failed .\r\n");
	                }
	                if(FileSys_DeleteFile(FW_DEL_INDIACTION_PATH)!=FST_STA_OK)
	                {
	                    DBG_ERR("delete %s failed .\r\n",FW_DEL_INDIACTION_PATH);
	                }
	            }
	        }
	        //#NT#2016/02/17#Niven Cho -end
	        //#NT#2016/05/30#Lincy Lin -begin
	        //#NT#Support logfile function
	        #if (LOGFILE_FUNC==ENABLE)
	        {
	            LOGFILE_OPEN    logOpenParm={0};
	            UINT32          maxFileNum = 32;
	            UINT32          maxFileSize = 0x100000; // 1MB
	            CHAR            rootDir[LOGFILE_ROOT_DIR_MAX_LEN+1]="A:\\Novatek\\LOG\\";

	            logOpenParm.maxFileNum = maxFileNum;
	            logOpenParm.maxFileSize = maxFileSize;
	            logOpenParm.isPreAllocAllFiles = TRUE;
	            strncpy(logOpenParm.rootDir,rootDir,LOGFILE_ROOT_DIR_MAX_LEN);
	            LogFile_Open(&logOpenParm);
	        }
	        #endif
	        //#NT#2016/05/30#Lincy Lin -end
	        //#NT#2016/04/27#Lincy Lin -begin
	        //#NT#Support userlog function
	        #if (USERLOG_FUNC == ENABLE)
	        {
	            userlog_open();
	        }
	        #endif
	        //#NT#2016/04/27#Lincy Lin -end
		}
		//#NT#2017/03/02#Niven Cho -end
        break;
    case FST_STA_DISK_UNFORMAT:
        UI_SetData(FL_FSStatus, FS_UNFORMATTED);
        break;
    case FST_STA_DISK_UNKNOWN_FORMAT:
        UI_SetData(FL_FSStatus, FS_UNKNOWN_FORMAT);
        break;
    case FST_STA_CARD_ERR:
        UI_SetData(FL_FSStatus, FS_DISK_ERROR);
        break;
    #if 0
    case FST_STA_OUT_OF_ID:
        UI_SetData(FL_FSStatus, FS_NUM_FULL);
        break;
    #endif
    default:
        UI_SetData(FL_FSStatus, FS_DISK_ERROR);
        break;
    }
    Ux_PostEvent(NVTEVT_STORAGE_INIT, 0, 0);
    GxPower_SetControl(GXPWR_CTRL_SLEEP_RESET,0);
    return NVTEVT_CONSUME;
}

INT32 System_OnStrgDetach(VControl *pCtrl, UINT32 paramNum, UINT32 *paramArray)
{
    if(m_GxStrgType == GXSTRG_FS_TYPE_LINUX)
    {
        UINT32 stg_id = paramArray[0];

        if(stg_id!=0) //not sd-1
        {
            return NVTEVT_CONSUME;
        }
    }

    switch(paramArray[1])
    {
    case FST_STA_OK:
        DBG_IND("FS: unmount OK\r\n");
        break;

    default:
        DBG_ERR("^RFS: unmount FAIL\r\n");
        break;
    }
    return NVTEVT_CONSUME;
}

//static BOOL bStorageInitsts = FALSE;
BOOL gChkCardPwrOn = FALSE;
BOOL gChkCardChange = FALSE;

void Storage_PowerOn_Start(void)
{
    gChkCardPwrOn = GxStrg_GetDeviceCtrl(0, CARD_INSERT);
    DBG_IND("^BStg Power On = %d\r\n", gChkCardPwrOn);
}
void Storage_UpdateSource(void)
{
    DBG_IND("^Y-------------CARD det\r\n");
    if(gChkCardPwrOn)
    {
        if (FALSE == GxStrg_GetDeviceCtrl(0, CARD_INSERT)) //CARD已拔除
        {
            gChkCardChange = TRUE;
        }
    }
    else
    {
        if (TRUE == GxStrg_GetDeviceCtrl(0, CARD_INSERT)) //CARD已插入
        {
            gChkCardChange = TRUE;
        }
    }
}
void Storage_PowerOn_End(void)
{
    Storage_UpdateSource();
    gChkCardPwrOn = GxStrg_GetDeviceCtrl(0, CARD_INSERT);

    if (TRUE == gChkCardChange) //CARD曾經插拔
    {
        System_PowerOff(SYS_POWEROFF_NORMAL); //關機
        return;
    }
}


///////////////////////////////////////////////////////////////////////////////
//
//  PSTORE
//
///////////////////////////////////////////////////////////////////////////////
void System_PS_Format(void)
{
    PSFMT gFmtStruct = {MAX_BLK_PER_SEC,MAX_SEC_NUM};
    PStore_Format(&gFmtStruct);
}
void System_OnStrgInit_PS(void)
{
    //PHASE-2 : Init & Open Lib or LibExt
    TM_BOOT_BEGIN("nand","init_ps");
#if PST_FUNC
    // Open PStore
    PSFMT gFmtStruct = {MAX_BLK_PER_SEC,MAX_SEC_NUM};
    PSTORE_INIT_PARAM gPStoreParam;
    UINT32 result=0;
    UINT8 *pBuf;
    //#NT#2017/03/02#Niven Cho -begin
    //#NT#EMMC_AS_PSTORE
    DX_HANDLE pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|USER_DX_TYPE_EMBMEM_PSTORE);	
#if (_CPU2_TYPE_==_CPU2_LINUX_ && _INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
	PStore_Init(PS_TYPE_FILESYS, PST_FS_DRIVE[0]);
    UINT32 paramNum;
    UINT32 paramArray[MAX_MESSAGE_PARAM_NUM];
	do {
    	UserWaitEvent(NVTEVT_STRG_ATTACH, &paramNum, paramArray);
    } while(paramArray[0] != PST_DEV_ID); //PStore will mount first before dev[0],dev[1]
#else
	PStore_Init(PS_TYPE_EMBEDED, 0);
#endif
	//#NT#2017/03/02#Niven Cho -end
    //PStore_InstallCmd();

    pBuf = (UINT8 *)OS_GetMempoolAddr(POOL_ID_PS_BUFFER);

    gPStoreParam.pBuf = pBuf;
    gPStoreParam.uiBufSize = POOL_SIZE_PS_BUFFER;
    DBG_ERR("PStore uiBufSize=%d\r\n",gPStoreParam.uiBufSize);
    result = PStore_Open(pStrgDev,&gPStoreParam);

    if(result!=E_PS_OK)
    {
         DBG_ERR("PStore Open fail %d format \r\n",result);
         //#NT#2016/05/25#Lincy Lin -begin
         //#NT#Fix coverity issue CID#60724
         PStore_Format(&gFmtStruct);
         //#NT#2016/05/25#Lincy Lin -end
    }
    //#NT#2013/05/28#Lincy Lin -begin
    //#NT#
    // read calibration data
    #if 0//(UI_STYLE!=UI_STYLE_CARDV)
    Cal_ReadSysData();
    Cal_ApplyData(CAL_SYS_DATA);
    #endif
    //#NT#2013/05/28#Lincy Lin -end
    #if(_CPU2_TYPE_==_CPU2_LINUX_)
    {
        PSTOREIPC_OPEN   PsopenObj = {0};
        // open pstore ipc
        PstoreIpc_Open(&PsopenObj);
    }
    #endif
#endif
//#NT#2016/03/30#Jeah Yen -begin
//#NT#isolate fast boot code
#if (POWERON_FAST_BOOT == ENABLE)
    INIT_SETFLAG(FLGINIT_MOUNTPS);
#endif
//#NT#2016/03/30#Jeah Yen -end
    TM_BOOT_END("nand","init_ps");
}
void System_OnStrgExit_PS(void)
{
#if PST_FUNC
    #if(_CPU2_TYPE_==_CPU2_LINUX_)
    PstoreIpc_Close();
    #endif
    PStore_Close();
#endif
}

///////////////////////////////////////////////////////////////////////////////
//
//  FWSTORE
//
///////////////////////////////////////////////////////////////////////////////
#include "DxStorage.h"
#include "FileSysTsk.h"
#include "FwSrvApi.h"
#include "MemCheck.h"
#include "GxStrg.h"

#if (_INTERSTORAGE_ != _INTERSTORAGE_NONE_)
static UINT32 gPL_BufAddr = 0;
static UINT32 gPL_BufSize = 0;
static FWSRV_INIT gPL_Init={0};
static FWSRV_CMD gPL_Cmd={0};
static FWSRV_PL_LOAD_BURST_IN gPL_In = {0};
#endif

void System_OnStrgInit_FWS(void)
{
    TM_BOOT_BEGIN("nand","init_fws");
#if (_INTERSTORAGE_ != _INTERSTORAGE_NONE_)
    BOOL bLZC = FALSE;
    ER er;
    UINT32 m_LdLoadedOffest;
    DX_HANDLE pStrgDev = 0;
    DBG_MSG("Init!\r\n");
    DBG_IND("^M LD_RSV_SIZE=%d\r\n", gBinInfo.ld.LdResvSize);
    DBG_IND("^M FW_MAX_SIZE=%08X\r\n", gBinInfo.ld.FWResvSize);
    DBG_IND("^MFW_validate-update:");
    //ValidateUpdateFW
    {
        UINT32 bUpdate = ((gBinInfo.ld.LdCtrl2 & LDCF_UPDATE_FW)!=0);
        UINT32 uiUpdateResult = ((gBinInfo.ld.LdCtrl2 & LDCF_UPDATE_FW_DONE)!=0);
        if(bUpdate)
        {
            DBG_IND("^MFW is just updated.\r\n");
            if(!uiUpdateResult)
            {
                DBG_FATAL("FW updated: fail!\r\n");
            }
        }
    }
    DBG_IND("^M ok\r\n");


    pStrgDev = Dx_GetObject(DX_CLASS_STORAGE_EXT|USER_DX_TYPE_EMBMEM_UITRON);

    bLZC = (gBinInfo.head.BinCtrl & HDCF_LZCOMPRESS_EN)?TRUE:FALSE;

    if(bLZC)
    {
        gPL_Init.PlInit.DataType = PARTLOAD_DATA_TYPE_COMPRESS_LZ;
        gPL_BufAddr = OS_GetMempoolAddr(POOL_ID_APP);
        gPL_BufSize = POOL_SIZE_APP;
    }
    else
    {
        gPL_Init.PlInit.DataType = PARTLOAD_DATA_TYPE_UNCOMPRESS;
        gPL_BufAddr = OS_GetMempoolAddr(POOL_ID_FWS_BUFFER);
        gPL_BufSize = POOL_SIZE_FWS_BUFFER;
    }

    m_LdLoadedOffest = gBinInfo.ld.LdLoadSize; //Get Ld loaded size

    gPL_Init.uiApiVer = FWSRV_API_VERSION;
    gPL_Init.TaskID = 0;
    gPL_Init.SemID = 0;
    gPL_Init.FlagID = 0;
    gPL_Init.DxMap.uITRON = USER_DX_TYPE_EMBMEM_UITRON;
    gPL_Init.DxMap.eCos = USER_DX_TYPE_EMBMEM_ECOS;
    gPL_Init.DxMap.uBoot = USER_DX_TYPE_EMBMEM_UBOOT;
    gPL_Init.DxMap.Linux = USER_DX_TYPE_EMBMEM_LINUX;
    gPL_Init.DxMap.RootFs = USER_DX_TYPE_EMBMEM_UITRON;
    gPL_Init.DxMap.DSP = USER_DX_TYPE_EMBMEM_DSP;
    gPL_Init.PlInit.uiApiVer = PARTLOAD_API_VERSION;
    gPL_Init.PlInit.hStrg = pStrgDev;
    gPL_Init.PlInit.uiAddrBegin = OS_GetMemAddr(MEM_CODE)+m_LdLoadedOffest; //Must be block boundary align
    gPL_Init.PlInit.uiWorkingAddr = gPL_BufAddr;
    gPL_Init.PlInit.uiWorkingSize = gPL_BufSize;
    er = FwSrv_Init(&gPL_Init);
    if(er != FWSRV_ER_OK)
    {
        DBG_ERR("Init failed!\r\n");
    }
    er = FwSrv_Open();
    if(er != FWSRV_ER_OK)
    {
        DBG_ERR("Open failed!\r\n");
    }
#endif
//#NT#2016/03/30#Jeah Yen -begin
//#NT#isolate fast boot code
#if (POWERON_FAST_BOOT == ENABLE)
    INIT_SETFLAG(FLGINIT_MOUNTFWS);
#endif
//#NT#2016/03/30#Jeah Yen -end
    TM_BOOT_END("nand","init_fws");
}

void System_OnStrgExit_FWS(void)
{
    ER er;
    er = FwSrv_Close();
    if(er != FWSRV_ER_OK)
    {
        DBG_ERR("Close failed!\r\n");
    }
}

void System_CPU2_Start(void)
{
    //#NT#2016/08/17#Niven Cho -begin
    //#NT#without flash, ecos is required to run
    #if (ECOS_POWER_OFF_FLOW == ENABLE)
    BOOL bStartCPU2 = FALSE;
    #else
    static BOOL bStartCPU2 = FALSE;
    #endif
    if(!bStartCPU2)
    {
#if (_CPU2_TYPE_ == _CPU2_LINUX_)
    xSysStrg_LinuxRun(); //if not support, this function returns directly.
#endif
#if (_CPU2_TYPE_ == _CPU2_ECOS_)
    xSysStrg_eCosRun(); //if not support, this function returns directly.
#endif
    bStartCPU2 = TRUE;
    }
    //#NT#2016/08/17#Niven Cho -end
}

void System_CPU2_Stop(void)
{
//#NT#2016/08/17#Niven Cho -begin
//#NT#without flash, ecos is required to run
#if (_CPU2_TYPE_ == _CPU2_LINUX_)
    GxLinux_Close();
#endif //(_CPU2_TYPE_ == _CPU2_LINUX_)
#if (_CPU2_TYPE_ == _CPU2_ECOS_)
#if (ECOS_POWER_OFF_FLOW == ENABLE)
    GxeCos_Close(); //if not support, this function returns directly.
#endif //(ECOS_POWER_OFF_FLOW == ENABLE)
#endif //(_CPU2_TYPE_ == _CPU2_ECOS_)
//#NT#2016/08/17#Niven Cho -end
}

void System_CPU2_WaitReady(void)
{
#if ((_CPU2_TYPE_ == _CPU2_ECOS_) || (_CPU2_TYPE_ == _CPU2_LINUX_))
    NvtIpc_WaitCPU2PowerOnReady();
#endif
}

void System_DSP_Start(void)
{
#if (_INTERSTORAGE_ != _INTERSTORAGE_NONE_)
    static BOOL bStartDSP = FALSE;
    if(!bStartDSP)
    {
#if (_DSP_TYPE_ == _DSP_FREERTOS_)
    xSysStrg_DspRun(); //if not support, this function returns directly.
#endif
    bStartDSP = TRUE;
    }
#endif
}

void System_DSP_WaitReady(void)
{
#if(_DSP_TYPE_ == _DSP_FREERTOS_)
    NvtIpc_WaitDSPPowerOnReady();
#endif
}

void System_OnStrg_DownloadFW(UINT32* SecOrderTable, void (*LoadCallback)(const UINT32 Idx))
{
    //#NT#2016/08/17#Niven Cho -begin
    //#NT#without flash, ecos is required to run
    TM_BOOT_BEGIN("nand","load2");
    DBG_MSG("Init!\r\n");

    //#NT#2016/07/19#Niven Cho -begin
    //#NT#use compressed DSP F.w
    //DSP must start before CPU2, because there is chance of cpu2 using POOL_APP which has used by compressed DSP.
    System_DSP_Start();
    //#NT#2016/07/19#Niven Cho -end

#if (_CPU2_TYPE_ == _CPU2_LINUX_)
    System_CPU2_Start();
#endif
#if ((_CPU2_TYPE_ == _CPU2_ECOS_) && (POWERON_FAST_CPU2_BOOT == ENABLE))
    System_CPU2_Start();
#endif

#if (_INTERSTORAGE_ != _INTERSTORAGE_NONE_)
    BOOL bEnablePartload = FALSE;
    BOOL bRunFromCard = (gBinInfo.ld.LdCtrl2 & LDCF_BOOT_CARD) ? TRUE : FALSE;
    int nSect = 0;

    DBG_IND("^G[LOAD-FW]\r\n");
    nSect = OS_GetMemSectionCount(MEM_CODE);  //How to get last section ZI
    DBG_IND("^GTotal Sections = %d\r\n", nSect);
    if(LoadCallback)
        LoadCallback(CODE_SECTION_01);
    DBG_IND("^M P1_LOAD_SIZE=%08X, TIME=%d\r\n", gBinInfo.ld.LdLoadSize, gBinInfo.ld.LdLoadTime);

    DBG_IND("^MPL_check_Ld:\r\n");
    DBG_IND("^M PL_EN=%08X\r\n", gBinInfo.ld.LdCtrl & LDCF_PARTLOAD_EN);
    DBG_IND("^M LZ_EN=%08X\r\n", gBinInfo.head.BinCtrl & HDCF_LZCOMPRESS_EN);
    bEnablePartload = ((gBinInfo.ld.LdCtrl & LDCF_PARTLOAD_EN)!=0);

    if(!bEnablePartload || bRunFromCard)
    {
        return;
    }

    DBG_IND("^MPL_validate-p1-range:");
    //ValidatePartOne
    {
        // Make Sure: Partloader start (PART-2 start) <= Loader loaded end
        int i;
        UINT32 FwSize = 0;
        UINT32 uiDestAddress = OS_GetMemAddr(MEM_CODE)+gBinInfo.ld.LdLoadSize;
        if((uiDestAddress != 0) && (uiDestAddress < OS_GetMemAddr(1)))
        {
            DBG_FATAL("PART_TWO start (%08x) > PART_ONE loaded end (%08x)!\r\n",
                OS_GetMemAddr(1), uiDestAddress );
        }

        // Make Sure: Partloader end (PART-last end) <= MEM_CODE end
        for(i=0;i<nSect;i++)
        {
            DBG_IND("Sect%d: Addr:%08X, Size:%08X\r\n",i,OS_GetMemAddr(i),OS_GetMemSize(i));
            FwSize += OS_GetMemSize(i);
        }
        if (OS_GetMemAddr(0)+FwSize > OS_GetMemAddr(MEM_CODE)+OS_GetMemSize(MEM_CODE))
        {
            DBG_FATAL("PART_LAST end (%08x) > MEM_CODE end (%08x)!\r\n",
                OS_GetMemAddr(0)+FwSize, OS_GetMemAddr(MEM_CODE)+OS_GetMemSize(MEM_CODE));
        }
    }
    DBG_IND("^M ok\r\n");

    DBG_IND("^MPL_begin\r\n");
    {
        ER er;
        gPL_In.puiIdxSequence = SecOrderTable;
        gPL_In.fpLoadedCb = LoadCallback;
        gPL_Cmd.Idx = FWSRV_CMD_IDX_PL_LOAD_BURST; //continue load
        gPL_Cmd.In.pData = &gPL_In;
        gPL_Cmd.In.uiNumByte = sizeof(gPL_In);
        gPL_Cmd.Prop.bExitCmdFinish = TRUE;
        er = FwSrv_Cmd(&gPL_Cmd);
        if(er != FWSRV_ER_OK)
        {
            DBG_ERR("Process failed!\r\n");
        }
    }
    DBG_IND("^MPL_end\r\n");
#endif
    //#NT#2016/08/17#Niven Cho -end
    TM_BOOT_END("nand","load2");
}

#define FW_CHECK_NOCHECK            0
#define FW_CHECK_CHECKSUM           1
#define FW_CHECK_CRC                2
#define FW_CHECK_METHOD             FW_CHECK_NOCHECK

#define FW_CHECK_PSEUDO_STR         "DSC_STR"
#define FW_UPDATE_FILE_LEN          16
static char uiUpdateFWName[FW_UPDATE_FILE_LEN] = FW_UPDATE_NAME;
static FWSRV_CMD Cmd={0};
//#NT#2016/02/18#Niven Cho -begin
//#NT#improve fw update fw flow
static BOOL m_bUpdRename = FALSE;
//#NT#2016/02/18#Niven Cho -end

//#NT#2016/02/18#Niven Cho -begin
//#NT#improve fw update fw flow
static void CheckSumOKCb(void)
{
    //#NT#2016/02/17#Niven Cho -begin
    //#NT#Mechanism for Fw Updating Fw failed.
    //Create a zero file to indicate deleting FW after loader update
    FST_FILE hFile = FileSys_OpenFile(FW_DEL_INDIACTION_PATH,FST_OPEN_WRITE|FST_OPEN_ALWAYS);
    if(hFile != NULL)
    {
        FileSys_CloseFile(hFile);
    }
    else
    {
        DBG_ERR("create indication %s failed.\r\n",FW_DEL_INDIACTION_PATH);
        return;
    }
    //Rename to be safe, that loader can update by name for next updating if FW updating FW failed.
    //#NT#2016/02/22#Niven Cho -begin
    //#NT#fix coverity to handle the return value of FileSys_RenameFile
    if(FileSys_RenameFile(_BIN_NAME_".BIN",uiUpdateFWName, TRUE)==FST_STA_OK)
    {
        m_bUpdRename = TRUE;
    }
    else
    {
        DBG_ERR("cannot rename bin file.\r\n");
    }
    //#NT#2016/02/22#Niven Cho -end
    //#NT#2016/02/17#Niven Cho -end
	//#NT#2017/04/18#Niven Cho -begin
    //#NT#for fw updating fw check failed can be returned.
	#if (_CPU2_TYPE_ == _CPU2_ECOS_)
    GxeCos_Close();
    #endif
	//#NT#2017/04/18#Niven Cho -end

}
//#NT#2016/02/18#Niven Cho -end

UINT32   System_OnStrg_UploadFW(UINT32 DevID)
{
    UINT32 m_MemSrc=0;
    UINT32 uiMemSize=0;
    //UINT32 m_SectSize = 0;
    DX_HANDLE pGxStrgDXH = NULL;
    DX_HANDLE pFSStrgDXH = NULL;
    FST_FILE hFile=NULL;
    UINT32 uiSize=0; //FileSize
    //BOOL bExist_uITRON = TRUE;
    INT32 fst_er;
#if (FW_CHECK_METHOD != FW_CHECK_NOCHECK)
    UINT32 uiSizeExt;
    MEMCHECK_PSEUDOSTR Pesudo = {0};
#endif
    FWSRV_ER fws_er;
	//#NT#2016/12/20#Niven Cho#[114852] -begin
	//#NT#FIX Linux update F.W failed. (because of filesys' buffer get smaller on linux)

	//#NT#2016/02/18#Niven Cho -begin
	//#NT#improve fw update fw flow
    m_bUpdRename = FALSE;
	//#NT#2016/02/18#Niven Cho -end

	//get file size first to calc working buffer
	uiSize = (UINT32)FileSys_GetFileLen(uiUpdateFWName);

	//#NT#2017/01/18#Niven Cho#[0116021] -begin
    //#NT#FIXED., after linux updatge no existing fw, system not shutdown if removing card.
	if (uiSize > FW_UPD_FW_TMP_MEM_SIZE) {
		DBG_ERR("cannot find %s or fw size too large: %d\r\n",uiUpdateFWName, uiSize);
		return UPDNAND_STS_FW_READ_ERR;
	}
	//#NT#2017/01/18#Niven Cho#[0116021] -end

    //alloc mem
    uiMemSize = 0;
#if (_CPU2_TYPE_ == _CPU2_LINUX_)
	//because we use uITRON's Filesys, we need to change filesys working buffer. origin is too small, which fits to linux
    uiMemSize += UITRON_FS_WORKING_BUF_SIZE;
#endif
    uiMemSize += ALIGN_CEIL_16(uiSize) + FW_UPD_FW_TMP_MEM_SIZE + 16; //8:margin for m_MemSrc is not align 16

	UINT32 uiTempMemAddr = ALIGN_CEIL_16(SxCmd_GetTempMem(uiMemSize));

#if (_CPU2_TYPE_ == _CPU2_LINUX_)
	UINT32 uiTempMemFsAddr = uiTempMemAddr;
	UINT32 uiTempMemFsSize = UITRON_FS_WORKING_BUF_SIZE;
	uiTempMemAddr += uiTempMemFsSize;
#endif
    m_MemSrc = uiTempMemAddr;

    if(m_MemSrc==0)
    {
        DBG_ERR("not enough memory to read FW\r\n");

        return UPDNAND_STS_FW_READ_ERR;
    }
//#NT#2017/04/18#Niven Cho -begin
//#NT#for fw updating fw check failed can be returned.
#if (_CPU2_TYPE_ == _CPU2_LINUX_)
    //Close CPU2
    GxStrg_CloseDevice(0);
    GxLinux_Close();
    //Change to uITRON's Filesys
    MEM_RANGE Pool;
	Pool.Addr = uiTempMemFsAddr;
	Pool.Size = uiTempMemFsSize;
	GxStrg_SetConfigEx(0, FILE_CFG_BUF, (UINT32)&Pool);
    GxStrg_SetConfig(FILE_CFG_FS_TYPE,GXSTRG_FS_TYPE_UITRON);
    if(GxStrg_OpenDevice(DevID, Dx_GetObject(DX_CLASS_STORAGE_EXT|FS_DX_TYPE_DRIVE_A))==TRUE)
    {
        FileSys_WaitFinish();
        pGxStrgDXH = (DX_HANDLE)GxStrg_GetDevice(DevID);
    }
#else
    pGxStrgDXH = (DX_HANDLE)GxStrg_GetDevice(DevID);
#endif
//#NT#2017/04/18#Niven Cho -end
    if (pGxStrgDXH == 0)
    {
        DBG_ERR("Incorrect storage\r\n");
        return UPDNAND_STS_FW_INVALID_STG;
    }

    FileSys_GetStrgObj(&pFSStrgDXH);
    if(pFSStrgDXH != pGxStrgDXH)
    {
        DBG_ERR("Incorrect storage\r\n");
        return UPDNAND_STS_FW_INVALID_STG;
    }

	//#NT#2016/12/20#Niven Cho#[114852] -end
    hFile = FileSys_OpenFile(uiUpdateFWName, FST_OPEN_READ);

    if(hFile != 0)
    {
        fst_er = FileSys_ReadFile(hFile, (UINT8*)m_MemSrc, &uiSize, 0, NULL);
        FileSys_CloseFile(hFile);
        if(fst_er != FST_STA_OK)
        {
            DBG_ERR("FW bin read fail. 1\r\n");
            return UPDNAND_STS_FW_READ2_ERR;
        }
    }
    else
    {
        //#NT#2016/02/17#Niven Cho -begin
        //#NT#Mechanism for Fw Updating Fw failed.
        DBG_ERR("cannot find %s\r\n",uiUpdateFWName);
        //#NT#2016/02/17#Niven Cho -end
        return UPDNAND_STS_FW_READ2_ERR;
    }

    FWSRV_BIN_UPDATE_ALL_IN_ONE FwInfo = {0};
    FwInfo.uiSrcBufAddr = m_MemSrc;
    FwInfo.uiSrcBufSize = uiSize;
    //#NT#2016/02/15#Niven Cho -begin
    //#NT#Update FW with protected memory
    FwInfo.uiWorkBufAddr = m_MemSrc+ALIGN_CEIL_16(uiSize);
    //#NT#2016/02/15#Niven Cho -end
    FwInfo.uiWorkBufSize = FW_UPD_FW_TMP_MEM_SIZE;
    FwInfo.fpItemCb = NULL;
    //#NT#2016/02/18#Niven Cho -begin
    //#NT#improve fw update fw flow
    FwInfo.fpCheckSumCb = CheckSumOKCb; //we rename bin after check sum OK
    //#NT#2016/02/18#Niven Cho -end
    memset(&Cmd,0,sizeof(Cmd));
    Cmd.Idx = FWSRV_CMD_IDX_BIN_UPDATE_ALL_IN_ONE;
    Cmd.In.pData = &FwInfo;
    Cmd.In.uiNumByte = sizeof(FwInfo);
    Cmd.Prop.bExitCmdFinish = TRUE;
    fws_er = FwSrv_Cmd(&Cmd);
    //#NT#2016/02/17#Niven Cho -begin
    //#NT#Mechanism for Fw Updating Fw failed.
    if(fws_er == FWSRV_ER_INVALID_UPDATED_DATA || fws_er == FWSRV_ER_WRITE_BLOCK)
    {
        DBG_ERR("update failed, start to use loader update mechanism.\r\n");
        GxSystem_EnableHWReset(0);
        GxSystem_PowerOff();
        return UPDNAND_STS_FW_WRITE_CHK_ERR;
    }
    else if(fws_er != FWSRV_ER_OK)
    {
        DBG_ERR("FW bin check failed %d\r\n",fws_er);
        //#NT#2016/02/18#Niven Cho -begin
        //#NT#improve fw update fw flow
        if(m_bUpdRename)
        {
            if(FileSys_RenameFile(&uiUpdateFWName[3],LOADER_UPD_FW_PATH, TRUE)==FST_STA_OK) //[3]: remove "A:\\"
            {
                FileSys_DeleteFile(FW_DEL_INDIACTION_PATH);
            }
        }
        //#NT#2016/02/18#Niven Cho -end
        return UPDNAND_STS_FW_READ_CHK_ERR;
    }

    //#NT#2016/02/18#Niven Cho -begin
    //#NT#improve fw update fw flow
    //if success, we rename back to original name and delete indication file
    if(m_bUpdRename)
    {
        if(FileSys_RenameFile(&uiUpdateFWName[3],LOADER_UPD_FW_PATH, TRUE)==FST_STA_OK) //[3]: remove "A:\\"
        {
            FileSys_DeleteFile(FW_DEL_INDIACTION_PATH);
        }
    }
    //#NT#2016/02/18#Niven Cho -end
    //#NT#2016/02/17#Niven Cho -end

    return UPDNAND_STS_FW_OK;
}

//#NT#2016/08/17#Niven Cho -begin
//#NT#without flash, ecos is required to run
//#NT#2016/08/17#Niven Cho -end

//#NT#2016/08/17#Niven Cho -begin
//#NT#without flash, ecos is required to run
#if(_CPU2_TYPE_ == _CPU2_ECOS_)
static BOOL xSysStrg_eCosRun(void)
{
    FWSRV_CMD Cmd={0};
    FWSRV_ECOS_BUF_DESC Desc={0};
    GXECOS_OPEN Open={0};
    BOOL bLoadTmp_eCos = FALSE;

    DBG_IND("^G[LOAD&RUN-ECOS]\r\n");

    //SxCmd_DoCommand("ecos uart"); //if ecos is forced on UART2, remove it mark

    Desc.Buf.uiAddr = OS_GetMempoolAddr(POOL_ID_CPU2);
    Desc.Buf.uiSize = OS_GetMempoolSize(POOL_ID_CPU2);

    if(Desc.Buf.uiSize==0)
    {
        return TRUE; //skip
    }

    Desc.hStrg = Dx_GetObject(DX_CLASS_STORAGE_EXT|USER_DX_TYPE_EMBMEM_ECOS);

    if(Desc.hStrg==NULL)
    {
        DBG_ERR("Desc.hStrg==NULL\r\n");
        return FALSE;
    }

    if(gBinInfo.ld.LdCtrl2 & LDCF_BOOT_CARD) //boot from card
    {
        DBG_WRN("LDCF_BOOT_CARD is detected.\r\n");

        BOOL bDePack = FALSE;
        BOOL bFilesysExist = (FileSys_WaitFinish()==FST_STA_OK)?TRUE:FALSE;

        if(!bFilesysExist)
        {
            DX_HANDLE DxCardDev1 = Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1);
            UserSdioInit.buf.Addr = OS_GetMempoolAddr(POOL_ID_STORAGE_SDIO);
            UserSdioInit.buf.Size = OS_GetMempoolSize(POOL_ID_STORAGE_SDIO);
            Dx_Init(DxCardDev1,&UserSdioInit,0,STORAGE_VER);

            MEM_RANGE Pool;
            Pool.Addr = OS_GetMempoolAddr(POOL_ID_FS_BUFFER);
            Pool.Size = OS_GetMempoolSize(POOL_ID_FS_BUFFER);
            GxStrg_SetConfig(FILE_CFG_BUF, (UINT32)&Pool);
            // coverity[var_deref_model]
            GxStrg_SetConfig(FILE_CFG_FS_TYPE,GXSTRG_FS_TYPE_UITRON);
            if(GxStrg_OpenDevice(0, Dx_GetObject(DX_CLASS_STORAGE_EXT|DX_TYPE_CARD1))!=TRUE)
            {
                DBG_ERR("Failed to GxStrg_OpenDevice).\r\n");
            }
        }

        memset(&Cmd,0,sizeof(Cmd));
        FWSRV_BIN_DEPACK DePack={0};
        DePack.pPath = "A:\\"_BIN_NAME_T_".BIN";

        if(!bFilesysExist)
        {
            //Run eCos at the time of start-up
            DePack.uiWorkBufAddr = OS_GetMempoolAddr(POOL_ID_APP);
            DePack.uiWorkBufSize = OS_GetMempoolSize(POOL_ID_APP);
            bDePack = TRUE;
        }
        else
        {
            //Run eCos at the time of menu selected
            UINT32 uiLen = ALIGN_CEIL_4(FileSys_GetFileLen(DePack.pPath));
            DePack.uiWorkBufAddr = SxCmd_GetTempMem(uiLen);
            DePack.uiWorkBufSize = uiLen;
            if(DePack.uiWorkBufAddr==0)
            {
                DBG_ERR("not enought memory to DePack eCos. try to use embedded storage's eCos F.W\r\n");
            }
            else
            {
                bDePack = TRUE;
            }
        }

        if(bDePack)
        {
            FST_FILE hFile = FileSys_OpenFile(DePack.pPath,FST_OPEN_READ);
            if(hFile != 0)
            {
                UINT32 uiLen = ALIGN_CEIL_4(FileSys_GetFileLen(DePack.pPath));
                FileSys_ReadFile(hFile, (UINT8 *)DePack.uiWorkBufAddr, (UINT32 *)&uiLen, 0, NULL);
                FileSys_CloseFile(hFile);

                NVTPACK_GET_PARTITION_INPUT input = {0};
                NVTPACK_MEM mem_out = {0};
                input.id = 1;
                input.mem.p_data = (void *)DePack.uiWorkBufAddr;
                input.mem.len = uiLen;
                if(nvtpack_get_partition(&input, &mem_out) == NVTPACK_ER_SUCCESS)
                {
                    UINT8* pMemCpu2 = (UINT8*)OS_GetMempoolAddr(POOL_ID_CPU2);
                    NVTPACK_BFC_HDR *pBfc = (NVTPACK_BFC_HDR *)mem_out.p_data;
                    if(pBfc->uiFourCC == MAKEFOURCC('B', 'C', 'L', '1'))
                    {
                         UINT32 uiSizeComp_le = UINT32_SWAP(pBfc->uiSizeComp);
                        if(uiSizeComp_le > OS_GetMempoolSize(POOL_ID_CPU2))
                        {
                            DBG_ERR("Compress F.W may be too large. (%d)\r\n",uiSizeComp_le);
                        }
                        else
                        {
                            LZ_Uncompress((unsigned char*)((UINT8*)mem_out.p_data+sizeof(NVTPACK_BFC_HDR))
                                ,(unsigned char*)(pMemCpu2)
                                ,uiSizeComp_le);
                        }
                    }
                    else
                    {
                        memcpy(pMemCpu2,mem_out.p_data,mem_out.len);
                    }
                    bLoadTmp_eCos = TRUE;
                }
            }

            if(!bFilesysExist)
            {
                GxStrg_CloseDevice(0);
            }

            if(bLoadTmp_eCos != TRUE)
            {
                DBG_WRN("eCos form Embedded Storage.\r\n");
            }
        }
    }


    if(bLoadTmp_eCos != TRUE)
    {
#if (_INTERSTORAGE_ != _INTERSTORAGE_NONE_)
        UINT32 er;
        TM_BOOT_BEGIN("nand","loadCPU2");
        memset(&Cmd,0,sizeof(Cmd));
        Cmd.Idx = FWSRV_CMD_IDX_ECOS_READ;
        Cmd.In.pData = &Desc;
        Cmd.In.uiNumByte = sizeof(Desc);
        Cmd.Prop.bExitCmdFinish = TRUE;
        er=FwSrv_Cmd(&Cmd);

        if(er!=0)
        {
            //#NT#2016/08/17#Niven Cho -begin
            //#NT#shutdown if load eCos failed.
            DBG_FATAL("Failed to load eCos.\r\n");
            //#NT#2016/08/17#Niven Cho -end
            return FALSE;
        }
        TM_BOOT_END("nand","loadCPU2");
#else
        DBG_FATAL("Failed to load eCos-T.\r\n");
        return FALSE;
#endif

    }
    else
    {
        DBG_WRN("eCos boot from temp all-in-one bin.\r\n");
    }

    Open.uiAddr = Desc.Buf.uiAddr;
    Open.uiSize = Desc.Buf.uiSize;

    TM_BOOT_BEGIN("flow","cpu2");
    if(!GxeCos_Open(&Open))
    {
        return FALSE;
    }
    NvtIpc_WaitCPU2PowerOnReady();
    TM_BOOT_END("flow","cpu2");

//#NT#2016/02/26#Niven Cho -begin
//#NT#eCos debug msg alway show on uITRON via IPC.
#if (ECOS_DBG_MSG_FORCE_UART1_DIRECT != ENABLE)
    const char cmd[] = "uart ipc";
    NVTIPC_SYS_MSG sysMsg;
    sysMsg.sysCmdID = NVTIPC_SYSCMD_UART_REQ;
    sysMsg.DataAddr = (UINT32)cmd;
    sysMsg.DataSize = sizeof(cmd);
    if (NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID,NVTIPC_SENDTO_CORE2,&sysMsg,sizeof(sysMsg)) <0)
    {
        DBG_ERR("NvtIPC_MsgSnd\r\n");
    }
#endif
//#NT#2016/02/26#Niven Cho -end

    return TRUE;
}
#endif
//#NT#2016/08/17#Niven Cho -end

#if(_CPU2_TYPE_ == _CPU2_LINUX_)
static void xSysStrg_LinuxRun_OnBoot(void)
{
    GXLINUX_OPEN Open={0};
    Open.uiAddr = OS_GetMempoolAddr(POOL_ID_CPU2);
    Open.uiSize = OS_GetMempoolSize(POOL_ID_CPU2);
    Open.Uart = GXLINUX_UART_2;
//#NT#2016/07/12#Niven Cho -begin
//#NT#LINUX_LOAD_BY_UBOOT
#if (LINUX_LOAD_BY_UBOOT)
    Open.bIgnoreKernelCheck = TRUE;
#endif
//#NT#2016/07/12#Niven Cho -end

    TM_BOOT_BEGIN("flow","cpu2");
    if(!GxLinux_Open(&Open))
    {
        return;
    }
    TM_BOOT_END("flow","cpu2");

    /**
        If want to use GXSTRG_FS_TYPE_UITRON with LINUX. Please Following Steps.
        1. NVT_SUPPORT_SDIO1 set to be '0' on nvt-platform.c
        2. remove 'mmcblk[0-2] root:root 660 * /etc/mdev-script/autosd.sh' h on root-fs/rootfs/etc/mdev.conf
        3. remove 'ugxstrg -status det_unmounted sd1' in root-fs/rootfs/etc/init.d/S10_InsMod
    */
    m_GxStrgType = GXSTRG_FS_TYPE_LINUX;
}

static BOOL xSysStrg_LinuxRun(void)
{
    UINT32 er;
    FWSRV_CMD Cmd={0};
    FWSRV_LINUX_BUF_DESC Desc={0};
    extern CORE_INFO g_CoreInfo;

    DBG_DUMP("^G[LOAD&RUN-LINUX]\r\n");

    Desc.hStrg_uBoot = Dx_GetObject(DX_CLASS_STORAGE_EXT|USER_DX_TYPE_EMBMEM_UBOOT);
    if(Desc.hStrg_uBoot==NULL)
    {
        DBG_ERR("Desc.hStrg_uBoot==NULL\r\n");
        return FALSE;
    }

    Desc.hStrg_uImage= Dx_GetObject(DX_CLASS_STORAGE_EXT|USER_DX_TYPE_EMBMEM_LINUX);
    if(Desc.hStrg_uImage==NULL)
    {
        DBG_ERR("Desc.hStrg_uImage==NULL\r\n");
        return FALSE;
    }
    Desc.hStrg_RootFs= Dx_GetObject(DX_CLASS_STORAGE_EXT|USER_DX_TYPE_EMBMEM_ROOTFS);
    if(Desc.hStrg_RootFs==NULL)
    {
        DBG_ERR("Desc.hStrg_RootFs==NULL\r\n");
        return FALSE;
    }

    Desc.Buf_uBoot.uiAddr = g_CoreInfo.uBootAddr;
    Desc.Buf_uBoot.uiSize = g_CoreInfo.uBootSize;

//#NT#2016/07/12#Niven Cho -begin
//#NT#LINUX_LOAD_BY_UBOOT
#if (!LINUX_LOAD_BY_UBOOT)
    Desc.Buf_uImage.uiAddr = g_CoreInfo.uImageAddr;
    Desc.Buf_uImage.uiSize = g_CoreInfo.uImageSize;
#endif
//#NT#2016/07/12#Niven Cho -end

#if (COREMEM_LINUX_SUBADDR_ROOTFS != DISABLE)
    Desc.Buf_RootFs.uiAddr = g_CoreInfo.RootFsAddr;
    Desc.Buf_RootFs.uiSize = g_CoreInfo.RootFsSize;
#endif

//#NT#2016/07/12#Niven Cho -begin
//#NT#LINUX_LOAD_BY_UBOOT
#if (!LINUX_LOAD_BY_UBOOT)
    Desc.fpBootCb = xSysStrg_LinuxRun_OnBoot;
#endif
//#NT#2016/07/12#Niven Cho -end

    //before starting linux, wifi should have prepared power on, for now
    DrvWiFi_PowerOn();

    TM_BOOT_BEGIN("nand","loadCPU2");
    Cmd.Idx = FWSRV_CMD_IDX_LINUX_READ;
    Cmd.In.pData = &Desc;
    Cmd.In.uiNumByte = sizeof(Desc);
    Cmd.Prop.bExitCmdFinish = TRUE;
    er=FwSrv_Cmd(&Cmd);
    TM_BOOT_END("nand","loadCPU2");

    if(er!=0)
    {
        DBG_ERR("Failed to load linux.\r\n");
        return FALSE;
    }

//#NT#2016/07/12#Niven Cho -begin
//#NT#LINUX_LOAD_BY_UBOOT
#if (LINUX_LOAD_BY_UBOOT)
     xSysStrg_LinuxRun_OnBoot();
#endif
//#NT#2016/07/12#Niven Cho -end

    return TRUE;
}
#endif

#if(_DSP_TYPE_ == _DSP_FREERTOS_)
static BOOL xSysStrg_DspRun(void)
{
    UINT32 er;
    FWSRV_CMD Cmd={0};
    //#NT#2016/07/19#Niven Cho -begin
    //#NT#use compressed DSP F.w
    FWSRV_DSP_BUF_DESC Desc={0};
    //#NT#2016/07/19#Niven Cho -end
    GXDSP_OPEN Open={0};

    DBG_DUMP("^G[LOAD&RUN-DSP]\r\n");

    Desc.Buf.uiAddr = OS_GetMempoolAddr(POOL_ID_DSP);
    Desc.Buf.uiSize = OS_GetMempoolSize(POOL_ID_DSP);
    //#NT#2016/07/19#Niven Cho -begin
    //#NT#use compressed DSP F.w
    Desc.WorkBuf.uiAddr = OS_GetMempoolAddr(POOL_ID_APP);
    Desc.WorkBuf.uiSize = OS_GetMempoolSize(POOL_ID_APP);
    //#NT#2016/07/19#Niven Cho -end

    if(Desc.Buf.uiSize==0)
    {
        return TRUE; //skip
    }

    Desc.hStrg = Dx_GetObject(DX_CLASS_STORAGE_EXT|USER_DX_TYPE_EMBMEM_DSP);

    if(Desc.hStrg==NULL)
    {
        DBG_ERR("Desc.hStrg==NULL\r\n");
        return FALSE;
    }

    TM_BOOT_BEGIN("nand","loadDSP");
    Cmd.Idx = FWSRV_CMD_IDX_DSP_READ;
    Cmd.In.pData = &Desc;
    Cmd.In.uiNumByte = sizeof(Desc);
    Cmd.Prop.bExitCmdFinish = TRUE;
    er=FwSrv_Cmd(&Cmd);
    TM_BOOT_BEGIN("nand","loadDSP");

    if(er!=0)
    {
        //#NT#2016/08/17#Niven Cho -begin
        //#NT#shutdown if DSP load fail.
        //if you don't want to shutdown, change DBG_FATAL to DBG_ERR for debug
        DBG_FATAL("Failed to load DSP.\r\n");
        //#NT#2016/08/17#Niven Cho -end
        return FALSE;
    }

    TM_BOOT_BEGIN("flow","dsp");
    Open.uiAddr = Desc.Buf.uiAddr;
    Open.uiSize = Desc.Buf.uiSize;
    if(!GxDSP_Open(&Open))
    {
        return FALSE;
    }
    TM_BOOT_BEGIN("flow","dsp");

    return TRUE;
}
#endif

//#NT#2016/08/01#Niven Cho -end
