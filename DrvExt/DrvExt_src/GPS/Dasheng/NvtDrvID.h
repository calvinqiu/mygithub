#ifndef _DRV_ID_H
#define _DRV_ID_H
#include "Type.h"

extern UINT32 _SECTION(".kercfg_data") SEM_ID_DASHENG;

extern void Brcm_Gps_Main_Tsk(void);

extern int dasheng_main(void);

extern void GPSRec_NMEAParser(CHAR *NMEA_str, UINT32 Len);

#endif //_DRV_ID_H