#ifndef _SAMPLE_DRV_ID_H
#define _SAMPLE_DRV_ID_H
#include "Type.h"


extern UINT32 _SECTION(".kercfg_data") SEM_ID_SAMPLE;
extern UINT32 _SECTION(".kercfg_data") FLG_ID_SAMPLE;
extern UINT32 _SECTION(".kercfg_data") TSK_ID_SAMPLE;

extern void SampleDrv_Tsk(void);

#endif //_SAMPLE_DRV_ID_H