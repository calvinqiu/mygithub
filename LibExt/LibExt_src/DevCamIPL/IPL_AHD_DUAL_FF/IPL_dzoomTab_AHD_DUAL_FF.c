/**
    IPL dzoom table sample.

    @file       IPL_dzoomTableSample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "IPL_AHD_DUAL_FF_Int.h"

const UINT32 VDOZOOM_INFOR_AHD_MODE_1_TABLE[1][DZOOM_ITEM_MAX] =
{
    {2560, 720, 1280, 720, 1280, 720, 2560, 720}, //1
};

UINT32* SenMode2Tbl(UINT32 Id, UINT32 SenMode, UINT32 *DzMaxidx)
{
    UINT32 *Ptr = NULL;
    //UINT32 Video_FPS = IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_FPS);
    //UINT32 Video_SIZE = IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE);
    //IPL_MODE IPL_Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

    if ( Id == IPL_ID_1 || (Id == IPL_ID_2 && IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR) )
    {
        switch(SenMode)
        {
            case SENSOR_MODE_1:
                Ptr = (UINT32*)&VDOZOOM_INFOR_AHD_MODE_1_TABLE[0][0];
                *DzMaxidx = (sizeof(VDOZOOM_INFOR_AHD_MODE_1_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                break;
            default:
                Ptr = NULL;
                *DzMaxidx = 0;
                break;
        }
    }
    else if ( Id == IPL_ID_2 )
    {
        switch(SenMode)
        {
            case SENSOR_MODE_1:
                Ptr = (UINT32*)&VDOZOOM_INFOR_AHD_MODE_1_TABLE[0][0];
                *DzMaxidx = (sizeof(VDOZOOM_INFOR_AHD_MODE_1_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                break;
            default:
                Ptr = NULL;
                *DzMaxidx = 0;
                break;
        }
    }
    return Ptr;
}
