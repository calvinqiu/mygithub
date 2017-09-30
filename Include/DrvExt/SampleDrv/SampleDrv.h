
#ifndef _SAMPLE_DRV_H
#define _SAMPLE_DRV_H

/**
     The BT init parameter.
*/
typedef struct _PSAMPLE_INIT_INFO
{
    UINT32               u32WorkbufAddr;                           ///<  The work buffer address for storing files info.
    UINT32               u32WorkbufSize;                           ///<  The work buffer size
} SAMPLE_INIT_INFO, *PSAMPLE_INIT_INFO;


/**
     Install flag and semaphore id.

*/
extern void SampleDrv_InstallID(void) _SECTION(".kercfg_text");
extern BOOL SampleDrv_open(PSAMPLE_INIT_INFO  pInitInfo);
extern BOOL SampleDrv_close(void);
extern BOOL SampleDrv_command(void);

#endif
