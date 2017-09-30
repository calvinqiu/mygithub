////////////////////////////////////////////////////////////////////////////////
#include "SysCommon.h"
#include "AppCommon.h"
#include "NvtIpcAPI.h"
#include "SxCmd.h"
////////////////////////////////////////////////////////////////////////////////

//global debug level: PRJ_DBG_LVL
#include "PrjCfg.h"
#include "SysCfg.h"
//local debug level: THIS_DBGLVL
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          SysIPCExe
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

#if (_CPU2_TYPE_ != _CPU2_NONE_)
static BOOL CPU2_Cmd(CHAR* pStr)
{
    NVTIPC_SYS_MSG sysMsg;

    sysMsg.sysCmdID = NVTIPC_SYSCMD_UART_REQ;
    if(strcmp(pStr,"^c")==0)
    {
        sysMsg.DataAddr = (UINT32)"\3";
        sysMsg.DataSize = 2;
    }
    else
    {
        sysMsg.DataAddr = (UINT32)pStr;
        sysMsg.DataSize = strlen(pStr)+1;
    }
    if (NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID,NVTIPC_SENDTO_CORE2,&sysMsg,sizeof(sysMsg)) <0)
        DBG_ERR("NvtIPC_MsgSnd\r\n");
    return TRUE;
}
#endif

#if (_DSP_TYPE_!=_DSP_NONE_)
BOOL DSP_Cmd(CHAR* pStr)
{
    NVTIPC_SYS_MSG sysMsg;

    sysMsg.sysCmdID = NVTIPC_SYSCMD_UART_REQ;
    sysMsg.DataAddr = (UINT32)pStr;
    sysMsg.DataSize = strlen(pStr)+1;
    if (NvtIPC_MsgSnd(NVTIPC_SYS_QUEUE_ID,NVTIPC_SENDTO_CORE3,&sysMsg,sizeof(sysMsg)) <0)
        DBG_ERR("NvtIPC_MsgSnd\r\n");
    return TRUE;
}
#endif


static void system_poweroff(void)
{
    DBG_IND("system_poweroff\r\n");
    Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
}

static void system_reboot(void)
{
    DBG_IND("system_reboot\r\n");
    GxSystem_EnableHWReset(0);
    Ux_PostEvent(NVTEVT_SYSTEM_SHUTDOWN, 1, 0);
}


void System_OnIPC(void)
{
    NVTIPC_OPEN_OBJ   openObj = {0};
    TM_BOOT_BEGIN("ipc","init");

    openObj.workBuffAddr = OS_GetMempoolAddr(POOL_ID_NVTIPC);
    openObj.workBuffSize = OS_GetMempoolSize(POOL_ID_NVTIPC);
    openObj.SysCmdTbl = NULL;
    openObj.uartCmdFp = (UART_CMD_FP)SxCmd_DoCommand;
    openObj.poweroffFp = system_poweroff;
    openObj.rebootFp = system_reboot;
	//#NT#2016/12/15#Lincy Lin -begin
    //#NT#Change cpu2 linux init parameter because ecos also need core semaphore function.
    #if (_CPU2_TYPE_ != _CPU2_NONE_)
    openObj.bIsSupportCoresSem = TRUE;
    #endif
    #if (_CPU2_TYPE_ == _CPU2_LINUX_)
    openObj.bIsCpu2Linux = TRUE;
    #endif
	//#NT#2016/12/15#Lincy Lin -end
    NvtIpc_Open(&openObj);

#if (_CPU2_TYPE_ != _CPU2_NONE_)
    SxCmd_RegCPU2CB(CPU2_Cmd);
#endif
#if (_DSP_TYPE_!=_DSP_NONE_)
    SxCmd_RegDSPCB(DSP_Cmd);
#endif
    TM_BOOT_END("ipc","init");
}
