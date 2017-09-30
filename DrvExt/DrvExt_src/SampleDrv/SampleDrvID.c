#include "SysKer.h"
#include "SampleDrvID.h"
#include "SampleDrv.h"

#define PRI_SAMPLE                8

#define STKSIZE_SAMPLE            2048

UINT32 SEM_ID_SAMPLE = 0;
UINT32 FLG_ID_SAMPLE = 0;
UINT32 TSK_ID_SAMPLE = 0;

void SampleDrv_InstallID(void)
{
    // semaphore
    OS_CONFIG_SEMPHORE(SEM_ID_SAMPLE, 0, 1, 1);
    // flag
    OS_CONFIG_FLAG(FLG_ID_SAMPLE);
    // task
    OS_CONFIG_TASK(TSK_ID_SAMPLE,   PRI_SAMPLE,  STKSIZE_SAMPLE,  SampleDrv_Tsk);
}
