#ifndef __TD_LIB_H__
#define __TD_LIB_H__

#include "Type.h"
#include "GxImage.h"
#include "BC_lib.h"

#define TD_MAX_BLK_H                        32
#define TD_MAX_BLK_W                       32
#define TD_MAX_BLK_NUM                  (TD_MAX_BLK_H * TD_MAX_BLK_W)

typedef enum
{
    TD_SET_FG_PCT = 0,
    TD_SET_FG_DURATION, 
    TD_SET_FG_LIFE_TIME, 
    TD_SET_NEIGHBORHOOD_UPDATE_EN, 
    TD_SET_BG_UPDATE_SPEED,
    TD_SET_BLK_NUM_W, 
    TD_SET_BLK_NUM_H,
    TD_SET_MIN_OBJ_BLK_NUM, 
    TD_SET_SENSI, 
    ENUM_DUMMY4WORD(TD_SET)
}TD_SET;

void TDSetPrms(TD_SET set, UINT32 val);
UINT32 TDGetPrms(TD_SET set);

void TDSetFgPct(UINT32 pct);
void TDSetFgDuration(UINT32 frms);
void TDSetFgLifeTime(UINT32 frms);
void TDSetNeighborhoodUpdate(BOOL en);
void TDSetBgUpdateSpeed(UINT32 flag);
void TDSetBlkNum(UINT32 nx, UINT32 ny);
void TDSetMinObjBlkNum(UINT32 num);
void TDSetSensi(UINT32 pct);

BOOL TDGetAlarm(void);
UINT32 TDGetTarget(URECT* Target, URECT* TargetCoord);
UINT32 TDCalcCacheBuffSize(void);
UINT32 TDCalcBuffSize(void);

void TDInit(UINT32 buf, UINT32 cachebuf);
void TDProcess(IMG_BUF *img);
void TDReset(void);

UINT32 TD_GetLog(UINT32 *bufAddr);


#endif
