/**
    IPL dzoom table sample.

    @file       IPL_dzoomTableSample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "IPL_IMX225LQR_DUAL_EVB_FF_Int.h"

const UINT32 VDOZOOM_INFOR_MODE_1_TABLE[20][DZOOM_ITEM_MAX] =
{
    {1280,  960, 1280,  960, 1280,  960, 1280,  960}, //1
    {1280,  960, 1280,  960, 1280,  960, 1236,  928}, //2
    {1280,  960, 1280,  960, 1244,  932, 1196,  896}, //3
    {1280,  960, 1280,  960, 1200,  900, 1152,  864}, //4
    {1280,  960, 1280,  960, 1160,  872, 1116,  836}, //5
    {1280,  960, 1280,  960, 1120,  840, 1076,  808}, //6
    {1280,  960, 1280,  960, 1080,  812, 1040,  780}, //7
    {1280,  960, 1280,  960, 1044,  784, 1004,  752}, //8
    {1280,  960, 1280,  960, 1008,  756,  972,  728}, //9
    {1280,  960, 1280,  960,  976,  732,  936,  704}, //10
    {1280,  960, 1280,  960,  940,  708,  904,  680}, //11
    {1280,  960, 1280,  960,  908,  684,  876,  656}, //12
    {1280,  960, 1280,  960,  880,  660,  844,  632}, //13
    {1280,  960, 1280,  960,  848,  636,  816,  612}, //14
    {1280,  960, 1280,  960,  820,  616,  788,  592}, //15
    {1280,  960, 1280,  960,  792,  596,  760,  572}, //16
    {1280,  960, 1280,  960,  764,  576,  736,  552}, //17
    {1280,  960, 1280,  960,  740,  556,  712,  532}, //18
    {1280,  960, 1280,  960,  712,  536,  684,  516}, //19
    {1280,  960, 1280,  960,  688,  516,  664,  496}, //20
};

const UINT32 VDOZOOM_INFOR_MODE_2_TABLE[20][DZOOM_ITEM_MAX] =
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

UINT32* SenMode2Tbl(UINT32 Id, UINT32 SenMode, UINT32 *DzMaxidx)
{
    UINT32 *Ptr = NULL;
    // Mark unused paramter
    //UINT32 Video_FPS = IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_FPS);
    //UINT32 Video_SIZE = IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE);
    //IPL_MODE IPL_Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

    //Both Sensor are IMX225, Use same dzoom table
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

        default:
            Ptr = NULL;
            *DzMaxidx = 0;
            break;
    }
    return Ptr;
}