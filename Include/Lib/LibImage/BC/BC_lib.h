#ifndef __BC_LIB_H__
#define __BC_LIB_H__

#include "Type.h"
#include "GxImage.h"

#define BC_MAX_OBJ_NUM                              64
#define BC_MAX_BLK_NUM_H                        32
#define BC_MAX_BLK_NUM_W                        32
#define BC_MAX_BLK_NUM                           (BC_MAX_BLK_NUM_H * BC_MAX_BLK_NUM_W)

typedef enum
{
    BC_SET_MIN_OBJ_AREA_PCT = 0,
    BC_SET_MAX_FG_FRMS,
    BC_SET_NEIGHBORHOOD_UPDATE_EN, 
    BC_SET_BG_UPDATE_SPEED,
    BC_SET_BLK_NUM_H, 
    BC_SET_BLK_NUM_W, 
    BC_SET_MIN_OBJ_BLK_NUM, 
    BC_SET_FG_PER_BLK_PCT, 
    BC_SET_SENSI, 
    BC_SET_TH_CROSS_NUM,
    BC_SET_TH_FEATURE, 
    ENUM_DUMMY4WORD(BC_SET)
}BC_SET;

void BC_Init(UINT32 buf, UINT32 cachebuf);
void BC_Process(IMG_BUF *img);
void BC_Reset(void);

void BC_SetPrms(BC_SET set, UINT32 val);
UINT32 BC_GetPrms(BC_SET set);
void BC_SetMinObjAreaPct(UINT32 pct);
void BC_SetMaxFgFrms(UINT32 frms);
void BC_SetNeighborhoodUpdate(BOOL en);
void BC_SetBgUpdateSpeed(UINT32 flag);
void BC_SetBlkNum(UINT32 nx, UINT32 ny);
void BC_SetMinObjBlkNum(UINT32 num);
void BC_SetFgPerBlkPct(UINT32 pct);
void BC_SetSensi(UINT32 pct);
void BC_SetThCrossNum(UINT32 th);
void BC_SetThFeature(UINT32 th);

UINT32 BC_CalcCacheBuffSize(void);
UINT32 BC_CalcBuffSize(void);
UINT32 BC_GetTarget(URECT* Target, URECT* TargetCoord);
UINT32 BC_GetBlkTarget(URECT* Target, URECT* TargetCoord);
void BC_GetForeground(IMG_BUF *fg);
void BC_GetBackground(IMG_BUF *bg);
BOOL BC_GetRsltFlag(void);
BOOL BC_GetBlkRsltFlag(void);
void BC_ResetRsltFlag(void);
void BC_ResetBlkRsltFlag(void);

#endif
