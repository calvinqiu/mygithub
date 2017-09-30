#include "SysKer.h"
#include "NvtDrvID.h"
#include "GPS.h"

#define PRI_DASHENG                8

#define STKSIZE_DASHENG            8192

UINT32 SEM_ID_DASHENG = 0;
UINT32 GPSRECEIVE_ID = 0;
UINT32 FLG_ID_GPS = 0;

void GPS_InstallID(void)
{
    // task
    OS_CONFIG_TASK(GPSRECEIVE_ID,   PRI_DASHENG,  STKSIZE_DASHENG,  Brcm_Gps_Main_Tsk);
    // semaphore
    OS_CONFIG_SEMPHORE(SEM_ID_DASHENG, 0, 1, 1);
    // flag
    OS_CONFIG_FLAG(FLG_ID_GPS);
}
