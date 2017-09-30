#ifndef __TWD_LIB_H__
#define __TWD_LIB_H__

#include "Type.h"
#include "GxImage.h"

#define twd_max_objects_number  64
#define twd_max_nodes_number    32

typedef enum 
{
    around_detection = 0,
    bottom_detection = 1
}twd_type;

twd_type twd_getdetectiontype(void);
UINT32 twd_getnodenumintripwire(void);
UINT32 twd_getminobjareapct(void);

void twd_setdetectiontype(twd_type type);
void twd_setminobjareapct(UINT32 pct);
void twd_setnodetotripwire(UINT32 x, UINT32 y);
void twd_settripwireclear(void);
void twd_setthfeature(UINT32 th);

UINT32 twd_getpointsintripwire(UPOINT *ppoint, URECT* screencoord);              
UINT32 twd_gettargetswithwarn(URECT *ptarget, BOOL *pwarn, URECT* screencoord);  
BOOL nvt_twd_getisalarm(void);
UINT32 nvt_twd_getalarmnum(void);

UINT32 nvt_twd_calccachebuffsize(void);
UINT32 nvt_twd_calcbuffsize(void);

void nvt_twd_init(UINT32 buf, UINT32 cachebuf);
void nvt_twd_process(IMG_BUF *pframe);
void nvt_twd_reset(void);


#endif
