#ifndef _IQS_TUNINGIQTABLE_IMX322LQJ_ISX016_EVB_FF_C_
#define _IQS_TUNINGIQTABLE_IMX322LQJ_ISX016_EVB_FF_C_

#include "IPL_IMX322LQJ_ISX016_EVB_FF_Int.h"

#if IQS_RATIOTABLE_ENABLE
INT32 IQS_RatioTable[ADJ_IQRatio_MAX_CNT][Total_ISONum] =
{
    {   // ADJIQ_OB
    0, 0, 0, 0, 0, 0, 0, 0,    // PRV1
    0, 0, 0, 0, 0, 0, 0, 0,    // PRV1S
    0, 0, 0, 0, 0, 0, 0, 0,    // CAP1
    0, 0, 0, 0, 0, 0,          // PRV2
    0, 0, 0, 0, 0, 0,          // CAP2
    },
    {   // ADJIQ_OUTL_BRI
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
    {   // ADJIQ_OUTL_DARK
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
    {   // ADJIQ_NR
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
    {   // ADJIQ_NR_FP
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
    {   // ADJIQ_NR_SPATIAL
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
    {   // ADJIQ_GBAL
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
    {   // ADJIQ_EDGE_WHITE
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
    {   // ADJIQ_EDGE_BLACK
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
    {   // ADJIQ_EDGE_THICK
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
    {   // ADJIQ_EDGE_START
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
    {   // ADJIQ_EDGE_STEP
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
    {   // ADJIQ_SAT
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
    {   // ADJIQ_CONT
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
    {   // ADJIQ_LCA
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
    {   // ADJIQ_CLAMP_MUL
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
    {   // ADJIQ_CLAMP_TH
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1
    100, 100, 100, 100, 100, 100, 100, 100,    // PRV1S
    100, 100, 100, 100, 100, 100, 100, 100,    // CAP1
    100, 100, 100, 100, 100, 100,              // PRV2
    100, 100, 100, 100, 100, 100,              // CAP2
    },
};

INT32 IQS_CAC_Table[Total_SENIDNum][ADJ_IQRatio_CAC_MAX_CNT] =
{
// ADJIQ_CAC_R,ADJIQ_CAC_B
    {0,        0},          // SEN1
    {0,        0},          // SEN2
};
#endif  // IQS_RATIOTABLE_ENABLE

IQS_SET_IPCAM_INFO IQS_IPCAM_RatioTable[Total_SENIDNum] =
{
    //brightness, saturation, contrast, sharpness, hue,    nr2D,              nr3D,                GDC,             LSC
    {  {0,100},     {0,100},  {0,100},   {0,100}, {0,0},  {0,100}, {0,IQS_NR_MODE_AUTO,5},  {0,DISABLE,100},  {0,DISABLE,100,100,100}},
    {  {0,100},     {0,100},  {0,100},   {0,100}, {0,0},  {0,100}, {0,IQS_NR_MODE_AUTO,5},  {0,DISABLE,100},  {0,DISABLE,100,100,100}},
};

#endif
