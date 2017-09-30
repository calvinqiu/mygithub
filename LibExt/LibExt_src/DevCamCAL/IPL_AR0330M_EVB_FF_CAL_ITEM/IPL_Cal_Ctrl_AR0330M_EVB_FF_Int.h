#ifndef _IPL_CAL_CTRL_AR0330M_EVB_FF_INT_H_
#define _IPL_CAL_CTRL_AR0330M_EVB_FF_INT_H_
#include "IPL_CalApi.h"
#include "IPL_Calibration.h"
typedef struct
{
    //prv
    fptr  fpPrv_CtrlInit;
    //cap
    fptr  fpCalStation_CtrlInit;
    fptr  fpISO_CtrlInit;
    fptr  fpAWB_CtrlInit;
    fptr  fpECS_CtrlInit;
    fptr  fpMSHUT_CtrlInit;
    fptr  fpDP_CtrlInit;
    fptr  fpLENSCEN_CtrlInit;
}CAL_CTRL_OBJ,*pCAL_CTRL_OBJ;

typedef enum
{
    sie_raw_header_0,
    sie_raw_header_1,

    sie_CH0_header_0,
    sie_CH0_header_1,
    sie_CH0_header_2,
    sie_CH0_header_3,
    sie_CH0_header_4,
    sie_CH0_header_5,
    sie_CH0_header_6,
    sie_CH0_header_7,
    sie_CH0_header_8,
    sie_CH0_header_9,

    sie_raw_0,
    sie_raw_1,

    sie_CH0_0,
    sie_CH0_1,
    sie_CH0_2,
    sie_CH0_3,
    sie_CH0_4,
    sie_CH0_5,
    sie_CH0_6,
    sie_CH0_7,
    sie_CH0_8,
    sie_CH0_9,

    sie_ca_0,
    sie_ca_1,

    sie_la_0,
    sie_la_1,

    ipl_in,

    ipe_ppb_0,
    ipe_ppb_1,

    ife2_ppb_0,
    ife2_ppb_1,

    ime_p2_y_0,
    ime_p2_y_1,
    ime_p2_y_2,
    ime_p2_y_3,
    ime_p2_y_4,
    ime_p2_y_5,
    ime_p2_y_6,
    ime_p2_y_7,
    ime_p2_y_8,
    ime_p2_y_9,
    ime_p2_uv_0,
    ime_p2_uv_1,
    ime_p2_uv_2,
    ime_p2_uv_3,
    ime_p2_uv_4,
    ime_p2_uv_5,
    ime_p2_uv_6,
    ime_p2_uv_7,
    ime_p2_uv_8,
    ime_p2_uv_9,

    ecs_init_header,
    iso_init_header,
    dp_init_header,
    awb_init_header,
    lenscen_init_header,

    ecs_init,
    iso_init,
    dp_init,
    awb_init,
    lenscen_init,

    ide_y,
    ide_uv,

    ENUM_DUMMY4WORD(CAL_PRV_ENUM)
}CAL_PRV_ENUM;


extern pCAL_CTRL_OBJ Cal_CtrlInit(void);

extern BOOL CAL_ReadDebugConfig_AR0330M_EVB_FF(void);
extern ER Set2Preview_AR0330M_EVB_FF(IPL_PROC_ID Id, UINT32 iplNum,UINT32 BufAddr,UINT32 BufSize);
extern pCAL_ITEM_OBJ CAL_ITEM_AR0330M_EVB_FF(IPL_PROC_ID Id);

extern UINT32 IPL_Num_AR0330M;

#endif