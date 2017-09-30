#ifndef _IPL_CAL_DATAPASSING_AR0230CS_EVB_FF_INT_H_
#define _IPL_CAL_DATAPASSING_AR0230CS_EVB_FF_INT_H_
#include "IPL_Hal.h"
#include "IPL_CalRW.h"
#include "Sensor.h"

typedef struct
{
    UINT32      Rg;
    UINT32      Bg;
    CCRange    BitSel;
}CAL_AWB_FMT;

typedef struct
{
    ECS_BitRatio    Ecs_bit;
    ECSMAP_ENUM     Ecs_map;
}CAL_ECS_HEADER;

typedef struct
{
    CAL_ECS_HEADER  header;
    UINT32          ECSBlockWin;
}CAL_ECS_FMT;

typedef struct
{
    UINT32 PixelCnt;    ///< total defect pixel number
    UINT32 LineCnt;     ///< total line number
}CAL_DPC_HEADER;

typedef struct
{
    CAL_DPC_HEADER     DPCnt;
    UINT32          DP_Pool;    ///< defect pixel coordinate pool(pool size can refer to DPCnt)
}CAL_DP_FMT;

typedef struct
{
    UINT32  GainBase;
}CAL_ISO_FMT;

typedef struct
{
    UINT32  DelayUS;
}CAL_MSHUT_FMT;

typedef struct
{
    UINT32  ChkStatus;
}CAL_AECHK_FMT;

typedef struct
{
    UINT32  ActStr_X;
    UINT32  ActStr_Y;
    INT32   CenterSft_X;
    INT32   CenterSft_Y;
    UINT32  Reliability;
}CAL_LENSCEN_FMT;

typedef enum
{
    PRV_1 = 1,
    PRV_2 = 2,
    PRV_3 = 3,
    PRV_4 = 4,
    SEC_AWB_1     = CAL_SEC_A_1,
    SEC_ECS_1     = CAL_SEC_B_1,
    SEC_DP_1      = CAL_SEC_C_1,
    SEC_MSHUT_1   = CAL_SEC_D_1,
    SEC_ISO_1     = CAL_SEC_E_1,
    SEC_AECHK_1   = CAL_SEC_F_1,
    SEC_LENSCEN_1 = CAL_SEC_G_1,
    SEC_AWB_2     = CAL_SEC_A_2,
    SEC_ECS_2     = CAL_SEC_B_2,
    SEC_DP_2      = CAL_SEC_C_2,
    SEC_MSHUT_2   = CAL_SEC_D_2,
    SEC_ISO_2     = CAL_SEC_E_2,
    SEC_AECHK_2   = CAL_SEC_F_2,
    SEC_LENSCEN_2 = CAL_SEC_G_2,
    SEC_AWB_3     = CAL_SEC_A_3,
    SEC_ECS_3     = CAL_SEC_B_3,
    SEC_DP_3      = CAL_SEC_C_3,
    SEC_MSHUT_3   = CAL_SEC_D_3,
    SEC_ISO_3     = CAL_SEC_E_3,
    SEC_AECHK_3   = CAL_SEC_F_3,
    SEC_LENSCEN_3 = CAL_SEC_G_3,
    SEC_AWB_4     = CAL_SEC_A_4,
    SEC_ECS_4     = CAL_SEC_B_4,
    SEC_DP_4      = CAL_SEC_C_4,
    SEC_MSHUT_4   = CAL_SEC_D_4,
    SEC_ISO_4     = CAL_SEC_E_4,
    SEC_AECHK_4   = CAL_SEC_F_4,
    SEC_LENSCEN_4 = CAL_SEC_G_4,
    IPL_Section_End = 0xffff,
    ENUM_DUMMY4WORD(CAL_SECMAP)
}CAL_SECMAP;

/**
    for IPP get calibration data
*/
typedef enum
{
    SEC_AWB     = 0,
    SEC_ECS     = 1,
    SEC_DP      = 2,
    SEC_MSHUT   = 3,
    SEC_ISO     = 4,
    SEC_AECHK   = 5,
    SEC_LENSCEN = 6,
    Section_End = 7,
    ENUM_DUMMY4WORD(CAL_SECMAP_GETDATA)
}CAL_SECMAP_GETDATA;

//base on sensor mode, focuse, ...etc.
typedef enum
{
    TAG_NONE    = 0,

    TAG_AWB      = 0x01000000,
    TAG_ECS      = 0x02000000,
    TAG_DP       = 0x03000000,
    TAG_MSHUT    = 0x04000000,
    TAG_ISO      = 0x05000000,
    TAG_AECHK    = 0x06000000,
    TAG_LENSCEN  = 0x07000000,
    ENUM_DUMMY4WORD(CAL_TAGID)
}CAL_TAGID;

// other condition
typedef enum
{
    CAL_CONDITION_DEFAULT       = 0xFFFF,
    CAL_CONDITION_ZOOM0         = 0,
    CAL_CONDITION_ZOOM1         = 1,
    CAL_CONDITION_ZOOM2         = 2,
    CAL_CONDITION_ZOOM3         = 3,
    CAL_CONDITION_HIGH_CT       = 4,
    CAL_CONDITION_LOW_CT        = 5,
    CAL_CONDITION_END           = 6,
    ENUM_DUMMY4WORD(CAL_CONDITION)
}CAL_CONDITION;


typedef struct _CAL_DATA_INFO
{
    CAL_SECMAP_GETDATA  StationID;
    SENSOR_MODE         SenMode;
    UINT32              CalCondition[10];
}CAL_DATA_INFO;

/**
    for IPL sample to get calibration information
    param:
        input:
            Idx:(ex)
            when SecID == SEC_ECS, Idx is zoom section
                 SecID == SEC_AWB, Idx is sensor mode
    return:
        NULL if no matched.
        Otherwise,return pointer of Current Dram data
*/
extern CAL_TAGID GetTagID(CAL_SECMAP SecID,UINT32 Idx, UINT32 option);
extern CAL_TAG* GetCalData(IPL_PROC_ID Id, CAL_DATA_INFO CalDataInfo);
UINT32 CalSenModeMap(IPL_PROC_ID Id,CAL_SECMAP SecID, UINT32 Input_SenMode);
#endif
