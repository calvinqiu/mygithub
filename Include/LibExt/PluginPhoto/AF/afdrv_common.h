#ifndef _AFDRV_DCJ36_G2007_650_H_
#define _AFDRV_DCJ36_G2007_650_H_
#include "IPL_AlgInfor.h"
#include "af_alg.h"

typedef struct _AFDRV_MANUAL
{
    INT16 iManualStartIdx;
    INT16 iManualEndIdx;
    INT8  iManualStep;
} AFDRV_MANUAL;

typedef struct _AFDRV_PARMTYPE
{
    UINT32 mode;
    UINT32 speed;
    UINT32 near_limit;
    UINT32 far_limit;
} AFDRV_PARMTYPE;

typedef struct _AFDRV_ABSOLUTEPARMTYPE
{
    UINT32 position;
    UINT32 speed;
} AFDRV_ABSOLUTEPARMTYPE;

typedef struct _AFDRV_CONTINUOUSPARMTYPE
{
    UINT32 speed;
} AFDRV_CONTINUOUSPARMTYPE;

typedef struct _AFDRV_RELATIVEPARMTYPE
{
    UINT32 distance;
    UINT32 speed;
} AFDRV_RELATIVEPARMTYPE;

typedef struct _AFDRV_STOPPARMTYPE
{
    UINT32 value;
} AFDRV_STOPPARMTYPE;

typedef struct _AFDRV_STATUSPARMTYPE
{
    UINT32 status;
    UINT32 position;
    UINT32 errcode;
} AFDRV_STATUSPARMTYPE;

typedef struct _AFDRV_ONVIFPARM
{
    AFDRV_PARMTYPE            parmtype;
    AFDRV_ABSOLUTEPARMTYPE    absoluteparm;
    AFDRV_CONTINUOUSPARMTYPE  continuousparm;
    AFDRV_RELATIVEPARMTYPE    relativeparm;
    AFDRV_STOPPARMTYPE        stopparam;
    AFDRV_STATUSPARMTYPE      statusparm;
} AFDRV_ONVIFPARM;

typedef enum
{
    Model_Normal = 0,
    Model_FallOff_Strong,
    ENUM_DUMMY4WORD(AF_AETRANSFERMODE)
}AF_AETRANSFERMODE;//Decide AFAE transfer mode

typedef enum
{
    PARMTYPE = 0,
    ABSOLUTEPARM,
    CONTINUOUSPARM,
    RELATIVESPARM,
    STOPPARM,
    ENUM_DUMMY4WORD(AF_ONVIFCOMMAND)
}AF_ONVIFCOMMAND;



extern UINT16 AFdrv_GetFocusDist(void);
extern void AFdrv_SetManualRange(AFDRV_MANUAL ManualSet);
extern void AFdrv_GetCurrentRange(UINT32 id, PAFALG_INFO_IN af_Obj, SEL_AFMODE AFMode);
extern void AFdrv_AFPrepare(BOOL bOP, UINT32 step);
extern void AFdrv_AEPrepare(BOOL bOP, UINT32 step);
#if 0 // VA TBD
extern void AFdrv_VAPrepare(BOOL bface, BOOL bOP, UINT32 step);
#endif
extern void AFdrv_SetAFModeChange(BOOL EN);
extern BOOL AFdrv_CheckAFModeChange(void);
//extern void AFdrv_AASwitch(BOOL bOP);
extern AF_AETRANSFERMODE AFdrv_GetAFModel(void);

//Temp
extern UINT16 AF_GetFocusDist(void);
extern void AF_OnvifSet(IPL_PROC_ID Id,AF_ONVIFCOMMAND command,AFDRV_ONVIFPARM* afparm);
extern void AF_OnvifGet(IPL_PROC_ID Id);
#endif
