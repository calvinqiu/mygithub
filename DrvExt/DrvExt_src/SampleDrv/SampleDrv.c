#include "Type.h"
#include "SampleDrv.h"
#include "SampleDrvID.h"

#define THIS_DBGLVL         2       //0=OFF, 1=ERROR, 2=TRACE
#define __MODULE__          SampleDrv
#define __DBGLVL__          THIS_DBGLVL
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"


// SAMPLE flag define
#define FLG_SAMPLE_START                FLGPTN_BIT(0)
#define FLG_SAMPLE_STOP                 FLGPTN_BIT(1)
#define FLG_SAMPLE_ACT                  FLGPTN_BIT(2)
#define FLG_SAMPLE_EXIT                 FLGPTN_BIT(3)
#define FLG_SAMPLE_IDLE                 FLGPTN_BIT(4)

static BOOL bIsOpen = FALSE;

static BOOL SampleDrv_Stop(void);

BOOL SampleDrv_open(PSAMPLE_INIT_INFO  pInitInfo)
{
    DBG_IND("\r\n");

    if (!SEM_ID_SAMPLE)
    {
        DBG_ERR("ID not installed\r\n");
        return FALSE;
    }
    if (!pInitInfo)
    {
        DBG_ERR("pInitInfo is NULL");
        return FALSE;
    }
    // already opened
    if (bIsOpen)
        return TRUE;
    wai_sem(SEM_ID_SAMPLE);
    // work buffer
    DBG_IND("workbuf Addr=0x%x, size=0x%x\r\n",pInitInfo->u32WorkbufAddr,pInitInfo->u32WorkbufSize);
    // start task
    sta_tsk(TSK_ID_SAMPLE, 0);
    bIsOpen = TRUE;
    sig_sem(SEM_ID_SAMPLE);
    return TRUE;
}

BOOL SampleDrv_close(void)
{
    DBG_IND("\r\n");
    if (!bIsOpen)
        return TRUE;
    wai_sem(SEM_ID_SAMPLE);
    SampleDrv_Stop();
    // terminate task
    ter_tsk(TSK_ID_SAMPLE);
    bIsOpen = FALSE;
    sig_sem(SEM_ID_SAMPLE);
    return TRUE;
}

BOOL SampleDrv_command(void)
{
    DBG_IND("\r\n");
    if (!bIsOpen)
    {
        DBG_ERR("SampleDrv is not opened\r\n");
        return FALSE;
    }
    set_flg(FLG_ID_SAMPLE, FLG_SAMPLE_ACT);
    return TRUE;
}


static BOOL SampleDrv_Stop(void)
{
    FLGPTN uiFlag;

    set_flg(FLG_ID_SAMPLE, FLG_SAMPLE_STOP);
    wai_flg(&uiFlag, FLG_ID_SAMPLE, FLG_SAMPLE_EXIT, TWF_ANDW);
    return TRUE;

}

void SampleDrv_Tsk(void)
{
    FLGPTN       uiFlag;
    kent_tsk();

    DBG_FUNC_BEGIN("\r\n");
    clr_flg(FLG_ID_SAMPLE, 0xFFFFFFFF);
    while(1)
    {
        wai_flg(&uiFlag, FLG_ID_SAMPLE, FLG_SAMPLE_STOP|FLG_SAMPLE_ACT, TWF_ORW|TWF_CLR);
        DBG_IND("uiFlag=0x%x\r\n",uiFlag);

        if (uiFlag & FLG_SAMPLE_STOP)
        {
            break;
        }
        if (uiFlag & FLG_SAMPLE_ACT)
        {
            // do something
            wai_sem(SEM_ID_SAMPLE);
            DBG_IND("do some action\r\n");
            sig_sem(SEM_ID_SAMPLE);
        }
    }
    set_flg(FLG_ID_SAMPLE, FLG_SAMPLE_EXIT);
    DBG_FUNC_END("\r\n");
    ext_tsk();
}

