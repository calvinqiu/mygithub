#ifndef _IPL_CAL_KEYSTRSAMPLE_INT_H_
#define _IPL_CAL_KEYSTRSAMPLE_INT_H_
#include "Type.h"
#include "IPL_Calibration.h"

#define isDemokit ENABLE //FALSE
//#NT#2016/07/19#Silvia Wu -begin
//#NT# move common funtion to Lib
typedef struct
{
    CalSTRCOLOR CalColor;   ///< calibration color define
    UINT32 PrjIdx;   ///< Project color idx define (CLRID_IDX_XX)
}CalColorMapPrj;
//#NT#2016/07/19#Silvia Wu -end

extern pCAL_UICB_OBJ CAL_UICB(void);
#endif
