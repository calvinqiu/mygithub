#ifndef __TZD_LIB_H__
#define __TZD_LIB_H__

#include "Type.h"
#include "GxImage.h"

#define tzd_max_objects_number 64
#define tzd_max_nodes_number 32

UINT32 tzd_getnodenumintripzone(void);
UINT32 tzd_getminobjareapct(void);

void tzd_setminobjareapct(UINT32 pct);
void tzd_setnodetotripzone(UINT32 x, UINT32 y);
void tzd_settripzoneclear(void);
void tzd_setthfeature(UINT32 th);

UINT32 tzd_getpointsintripzone(UPOINT *ppoint, URECT* screencoord);             
UINT32 tzd_gettargetswithwarn(URECT *ptarget, BOOL *pwarn, URECT* screencoord); 

BOOL nvt_tzd_getisalarm(void);
UINT32 nvt_tzd_getalarmnum(void);

UINT32 nvt_tzd_calccachebuffsize(void);
UINT32 nvt_tzd_calcbuffsize(void);

void nvt_tzd_init(UINT32 buf, UINT32 cachebuf);
void nvt_tzd_process(IMG_BUF *pframe);
void nvt_tzd_reset(void);


#endif
