/**
    IPL dzoom table sample.

    @file       IPL_dzoomTableSample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "IPL_OV12895M_EVB_FF_Int.h"

const UINT32 VDOZOOM_INFOR_MODE_1_TABLE[1][DZOOM_ITEM_MAX] =
{
    {4096, 3072, 2880, 2160, 2880, 2160, 4096, 3072}, //1
};

const UINT32 VDOZOOM_INFOR_MODE_2_TABLE[1][DZOOM_ITEM_MAX] =
{
    {3840, 2160, 2112, 1188, 2112, 1188, 3840, 2160}, //1
};

const UINT32 VDOZOOM_INFOR_MODE_3_TABLE[1][DZOOM_ITEM_MAX] =
{
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //1
};

const UINT32 VDOZOOM_INFOR_MODE_4_TABLE[20][DZOOM_ITEM_MAX] =
{
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //1
};

const UINT32 VDOZOOM_INFOR_MODE_3SSTRP_TABLE[20][DZOOM_ITEM_MAX] =
{
    {1024,  768, 1024,  768, 1024,  768, 1024,  768}, //1
};

const UINT32 VDOZOOM_INFOR_MODE_5_TABLE[20][DZOOM_ITEM_MAX] =
{
    {1024,  768, 1024,  768, 1024,  768, 1024,  768}, //1
};

UINT32* SenMode2Tbl(UINT32 Id, UINT32 SenMode, UINT32 *DzMaxidx)
{
    UINT32 *Ptr = NULL;
    //UINT32 Video_SIZE = IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE);
    //IPL_MODE IPL_Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

    switch(SenMode)
    {
        case SENSOR_MODE_1:
            Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_1_TABLE[0][0];
            *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_1_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
            break;

        case SENSOR_MODE_2:
            Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_2_TABLE[0][0];
            *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_2_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
            break;

        case SENSOR_MODE_3:
            //if ( Video_SIZE < SEL_VIDEOSIZE_1920x1080 && IPL_Mode != IPL_MODE_PREVIEW && IPL_Mode != IPL_MODE_VIDEO)
            {
                Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_3_TABLE[0][0];
                *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_3_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
            }
            /*else
            {
                Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_3SSTRP_TABLE[0][0];
                *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_3SSTRP_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
            }*/
            break;

        case SENSOR_MODE_4:
            Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_4_TABLE[0][0];
            *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_4_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
            break;

        case SENSOR_MODE_5:
            Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_5_TABLE[0][0];
            *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_5_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
            break;

        default:
            Ptr = NULL;
            *DzMaxidx = 0;
            break;
    }
    return Ptr;
}
