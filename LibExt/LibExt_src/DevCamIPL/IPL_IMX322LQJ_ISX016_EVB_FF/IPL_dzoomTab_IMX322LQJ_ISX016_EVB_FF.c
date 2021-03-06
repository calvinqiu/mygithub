/**
    IPL dzoom table sample.

    @file       IPL_dzoomTableSample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "IPL_IMX322LQJ_ISX016_EVB_FF_Int.h"

const UINT32 VDOZOOM_INFOR1_MODE_1_TABLE[20][DZOOM_ITEM_MAX] =
{
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //1
    {1920, 1080, 1920, 1080, 1880, 1060, 1880, 1060}, //2
    {1920, 1080, 1920, 1080, 1816, 1024, 1816, 1024}, //3
    {1920, 1080, 1920, 1080, 1756,  988, 1756,  988}, //4
    {1920, 1080, 1920, 1080, 1696,  956, 1696,  956}, //5
    {1920, 1080, 1920, 1080, 1636,  920, 1636,  920}, //6
    {1920, 1080, 1920, 1080, 1580,  892, 1580,  892}, //7
    {1920, 1080, 1920, 1080, 1528,  860, 1528,  860}, //8
    {1920, 1080, 1920, 1080, 1476,  832, 1476,  832}, //9
    {1920, 1080, 1920, 1080, 1424,  804, 1424,  804}, //10
    {1920, 1080, 1920, 1080, 1376,  776, 1376,  776}, //11
    {1920, 1080, 1920, 1080, 1328,  748, 1328,  748}, //12
    {1920, 1080, 1920, 1080, 1284,  724, 1284,  724}, //13
    {1920, 1080, 1920, 1080, 1240,  700, 1240,  700}, //14
    {1920, 1080, 1920, 1080, 1200,  676, 1200,  676}, //15
    {1920, 1080, 1920, 1080, 1156,  652, 1156,  652}, //16
    {1920, 1080, 1920, 1080, 1120,  628, 1120,  628}, //17
    {1920, 1080, 1920, 1080, 1080,  608, 1080,  608}, //18
    {1920, 1080, 1920, 1080, 1044,  588, 1044,  588}, //19
    {1920, 1080, 1920, 1080, 1008,  568, 1008,  568}, //20
};


const UINT32 VDOZOOM_INFOR1_MODE_2_TABLE[20][DZOOM_ITEM_MAX] =
{
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //1
    {1280,  720, 1280,  720, 1196,  672, 1196,  672}, //2
    {1280,  720, 1280,  720, 1116,  628, 1116,  628}, //3
    {1280,  720, 1280,  720, 1040,  584, 1040,  584}, //4
    {1280,  720, 1280,  720,  972,  544,  972,  544}, //5
    {1280,  720, 1280,  720,  904,  508,  904,  508}, //6
    {1280,  720, 1280,  720,  844,  476,  844,  476}, //7
    {1280,  720, 1280,  720,  788,  444,  788,  444}, //8
    {1280,  720, 1280,  720,  736,  412,  736,  412}, //9
    {1280,  720, 1280,  720,  684,  384,  684,  384}, //10
    {1280,  720, 1280,  720,  640,  360,  640,  360}, //11
    {1280,  720, 1280,  720,  596,  336,  596,  336}, //12
    {1280,  720, 1280,  720,  556,  312,  556,  312}, //13
    {1280,  720, 1280,  720,  520,  292,  520,  292}, //14
    {1280,  720, 1280,  720,  484,  272,  484,  272}, //15
    {1280,  720, 1280,  720,  452,  256,  452,  256}, //16
    {1280,  720, 1280,  720,  424,  236,  424,  236}, //17
    {1280,  720, 1280,  720,  392,  220,  392,  220}, //18
    {1280,  720, 1280,  720,  368,  208,  368,  208}, //19
    {1280,  720, 1280,  720,  320,  180,  320,  180}, //20
};

const UINT32 VDOZOOM_INFOR2_MODE_1_TABLE[1][DZOOM_ITEM_MAX] =
{
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //1
};


UINT32* SenMode2Tbl(UINT32 Id, UINT32 SenMode, UINT32 *DzMaxidx)
{
    UINT32 *Ptr = NULL;
    //UINT32 Video_FPS = IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_FPS);
    //IPL_MODE IPL_Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

    if (Id == IPL_ID_1)
    {
        switch(SenMode)
        {
            case SENSOR_MODE_1:
                Ptr = (UINT32*)&VDOZOOM_INFOR1_MODE_1_TABLE[0][0];
                *DzMaxidx = (sizeof(VDOZOOM_INFOR1_MODE_1_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                break;

            case SENSOR_MODE_2:
                Ptr = (UINT32*)&VDOZOOM_INFOR1_MODE_1_TABLE[0][0];
                *DzMaxidx = (sizeof(VDOZOOM_INFOR1_MODE_1_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                break;
            default:
                Ptr = NULL;
                *DzMaxidx = 0;
                break;
        }
    }
	else if(Id == IPL_ID_2)
	{
		switch(SenMode)
	    {
	        case SENSOR_MODE_1:
	            Ptr = (UINT32*)&VDOZOOM_INFOR2_MODE_1_TABLE[0][0];
	            *DzMaxidx = (sizeof(VDOZOOM_INFOR2_MODE_1_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
	            break;

	        default:
	            Ptr = NULL;
	            *DzMaxidx = 0;
	            break;
	    }

	}
    return Ptr;
}
