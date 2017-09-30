/**
    IPL dzoom table sample.

    @file       IPL_dzoomTable_TVP5150_EVB_FF.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "IPL_TVP5150_EVB_FF_Int.h"


const UINT32 VDOZOOM_INFOR_MODE_1_TABLE[1][DZOOM_ITEM_MAX] =
{
    {1440, 240, 720, 240, 720, 240, 1440, 240}, //1
};


UINT32* SenMode2Tbl(UINT32 Id, UINT32 SenMode, UINT32 *DzMaxidx)
{
    UINT32 *Ptr = NULL;

    switch(SenMode)
    {
        case SENSOR_MODE_2:
        case SENSOR_MODE_1:
            Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_1_TABLE[0][0];
            *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_1_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
            break;

        default:
            Ptr = NULL;
            *DzMaxidx = 0;
            break;
    }

    return Ptr;
}
