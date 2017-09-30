/**
    IPL dzoom table sample.

    @file       IPL_dzoomTableSample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "IPL_OV4689_OV2710_EVB_FF_Int.h"

const UINT32 VDOZOOM_INFOR_MODE_1_TABLE[20][DZOOM_ITEM_MAX] =
{
    {2688, 1508, 2112, 1188, 2112, 1188, 2688, 1508}, //1
    {2608, 1472, 2112, 1188, 2112, 1188, 2608, 1472}, //2
    {2544, 1424, 2112, 1188, 2112, 1188, 2544, 1424}, //3
    {2480, 1392, 2112, 1188, 2112, 1188, 2480, 1392}, //4
    {2416, 1360, 2112, 1188, 2112, 1188, 2416, 1360}, //5
    {2352, 1312, 2112, 1188, 2112, 1188, 2352, 1312}, //6
    {2272, 1280, 2112, 1188, 2112, 1188, 2272, 1280}, //7
    {2208, 1232, 2112, 1188, 2112, 1188, 2208, 1232}, //8
    {2144, 1200, 2112, 1188, 2112, 1188, 2144, 1200}, //9
    {2112, 1188, 2112, 1188, 2112, 1188, 2080, 1168}, //10
    {2112, 1188, 2112, 1188, 2016, 1120, 2016, 1120}, //11
    {2112, 1188, 2112, 1188, 1936, 1088, 1936, 1088}, //12
    {2112, 1188, 2112, 1188, 1872, 1056, 1872, 1056}, //13
    {2112, 1188, 2112, 1188, 1808, 1008, 1808, 1008}, //14
    {2112, 1188, 2112, 1188, 1744,  976, 1744,  976}, //15
    {2112, 1188, 2112, 1188, 1680,  944, 1680,  944}, //16
    {2112, 1188, 2112, 1188, 1600,  896, 1600,  896}, //17
    {2112, 1188, 2112, 1188, 1536,  864, 1536,  864}, //18
    {2112, 1188, 2112, 1188, 1472,  816, 1472,  816}, //19
    {2112, 1188, 2112, 1188, 1408,  784, 1408,  784}, //20
};

const UINT32 VDOZOOM_INFOR_MODE_2_TABLE[20][DZOOM_ITEM_MAX] =
{
    {2688, 1508, 2112, 1188, 2112, 1188, 2688, 1508}, //1
    {2608, 1472, 2112, 1188, 2112, 1188, 2608, 1472}, //2
    {2544, 1424, 2112, 1188, 2112, 1188, 2544, 1424}, //3
    {2480, 1392, 2112, 1188, 2112, 1188, 2480, 1392}, //4
    {2416, 1360, 2112, 1188, 2112, 1188, 2416, 1360}, //5
    {2352, 1312, 2112, 1188, 2112, 1188, 2352, 1312}, //6
    {2272, 1280, 2112, 1188, 2112, 1188, 2272, 1280}, //7
    {2208, 1232, 2112, 1188, 2112, 1188, 2208, 1232}, //8
    {2144, 1200, 2112, 1188, 2112, 1188, 2144, 1200}, //9
    {2112, 1188, 2112, 1188, 2112, 1188, 2080, 1168}, //10
    {2112, 1188, 2112, 1188, 2016, 1120, 2016, 1120}, //11
    {2112, 1188, 2112, 1188, 1936, 1088, 1936, 1088}, //12
    {2112, 1188, 2112, 1188, 1872, 1056, 1872, 1056}, //13
    {2112, 1188, 2112, 1188, 1808, 1008, 1808, 1008}, //14
    {2112, 1188, 2112, 1188, 1744,  976, 1744,  976}, //15
    {2112, 1188, 2112, 1188, 1680,  944, 1680,  944}, //16
    {2112, 1188, 2112, 1188, 1600,  896, 1600,  896}, //17
    {2112, 1188, 2112, 1188, 1536,  864, 1536,  864}, //18
    {2112, 1188, 2112, 1188, 1472,  816, 1472,  816}, //19
    {2112, 1188, 2112, 1188, 1408,  784, 1408,  784}, //20
};

const UINT32 VDOZOOM_INFOR_MODE_3_TABLE[20][DZOOM_ITEM_MAX] =
{
    {2688, 1508, 2688, 1508, 2688, 1508, 2688, 1508}, //1
    {2608, 1472, 2608, 1472, 2608, 1472, 2608, 1472}, //2
    {2544, 1424, 2544, 1424, 2544, 1424, 2544, 1424}, //3
    {2480, 1392, 2480, 1392, 2480, 1392, 2480, 1392}, //4
    {2416, 1360, 2416, 1360, 2416, 1360, 2416, 1360}, //5
    {2352, 1312, 2352, 1312, 2352, 1312, 2352, 1312}, //6
    {2272, 1280, 2272, 1280, 2272, 1280, 2272, 1280}, //7
    {2208, 1232, 2208, 1232, 2208, 1232, 2208, 1232}, //8
    {2144, 1200, 2144, 1200, 2144, 1200, 2144, 1200}, //9
    {2080, 1168, 2080, 1168, 2080, 1168, 2080, 1168}, //10
    {2016, 1120, 2016, 1120, 2016, 1120, 2016, 1120}, //11
    {1936, 1088, 1936, 1088, 1936, 1088, 1936, 1088}, //12
    {1872, 1056, 1872, 1056, 1872, 1056, 1872, 1056}, //13
    {1808, 1008, 1808, 1008, 1808, 1008, 1808, 1008}, //14
    {1744,  976, 1744,  976, 1744,  976, 1744,  976}, //15
    {1680,  944, 1680,  944, 1680,  944, 1680,  944}, //16
    {1600,  896, 1600,  896, 1600,  896, 1600,  896}, //17
    {1536,  864, 1536,  864, 1536,  864, 1536,  864}, //18
    {1472,  816, 1472,  816, 1472,  816, 1472,  816}, //19
    {1408,  784, 1408,  784, 1408,  784, 1408,  784}, //20
};

const UINT32 VDOZOOM_INFOR_MODE_4_TABLE[20][DZOOM_ITEM_MAX] =
{
    {2688, 1508, 2112, 1188, 2112, 1188, 2688, 1508}, //1
    {2608, 1472, 2112, 1188, 2112, 1188, 2608, 1472}, //2
    {2544, 1424, 2112, 1188, 2112, 1188, 2544, 1424}, //3
    {2480, 1392, 2112, 1188, 2112, 1188, 2480, 1392}, //4
    {2416, 1360, 2112, 1188, 2112, 1188, 2416, 1360}, //5
    {2352, 1312, 2112, 1188, 2112, 1188, 2352, 1312}, //6
    {2272, 1280, 2112, 1188, 2112, 1188, 2272, 1280}, //7
    {2208, 1232, 2112, 1188, 2112, 1188, 2208, 1232}, //8
    {2144, 1200, 2112, 1188, 2112, 1188, 2144, 1200}, //9
    {2112, 1188, 2112, 1188, 2112, 1188, 2080, 1168}, //10
    {2112, 1188, 2112, 1188, 2016, 1120, 2016, 1120}, //11
    {2112, 1188, 2112, 1188, 1936, 1088, 1936, 1088}, //12
    {2112, 1188, 2112, 1188, 1872, 1056, 1872, 1056}, //13
    {2112, 1188, 2112, 1188, 1808, 1008, 1808, 1008}, //14
    {2112, 1188, 2112, 1188, 1744,  976, 1744,  976}, //15
    {2112, 1188, 2112, 1188, 1680,  944, 1680,  944}, //16
    {2112, 1188, 2112, 1188, 1600,  896, 1600,  896}, //17
    {2112, 1188, 2112, 1188, 1536,  864, 1536,  864}, //18
    {2112, 1188, 2112, 1188, 1472,  816, 1472,  816}, //19
    {2112, 1188, 2112, 1188, 1408,  784, 1408,  784}, //20
};


const UINT32 VDOZOOM_INFOR_MODE_3SSTRP_TABLE[20][DZOOM_ITEM_MAX] =
{
    {2688, 1508, 2112, 1188, 2112, 1188, 2688, 1508}, //1
    {2608, 1472, 2112, 1188, 2112, 1188, 2608, 1472}, //2
    {2544, 1424, 2112, 1188, 2112, 1188, 2544, 1424}, //3
    {2480, 1392, 2112, 1188, 2112, 1188, 2480, 1392}, //4
    {2416, 1360, 2112, 1188, 2112, 1188, 2416, 1360}, //5
    {2352, 1312, 2112, 1188, 2112, 1188, 2352, 1312}, //6
    {2272, 1280, 2112, 1188, 2112, 1188, 2272, 1280}, //7
    {2208, 1232, 2112, 1188, 2112, 1188, 2208, 1232}, //8
    {2144, 1200, 2112, 1188, 2112, 1188, 2144, 1200}, //9
    {2112, 1188, 2112, 1188, 2112, 1188, 2080, 1168}, //10
    {2112, 1188, 2112, 1188, 2016, 1120, 2016, 1120}, //11
    {2112, 1188, 2112, 1188, 1936, 1088, 1936, 1088}, //12
    {2112, 1188, 2112, 1188, 1872, 1056, 1872, 1056}, //13
    {2112, 1188, 2112, 1188, 1808, 1008, 1808, 1008}, //14
    {2112, 1188, 2112, 1188, 1744,  976, 1744,  976}, //15
    {2112, 1188, 2112, 1188, 1680,  944, 1680,  944}, //16
    {2112, 1188, 2112, 1188, 1600,  896, 1600,  896}, //17
    {2112, 1188, 2112, 1188, 1536,  864, 1536,  864}, //18
    {2112, 1188, 2112, 1188, 1472,  816, 1472,  816}, //19
    {2112, 1188, 2112, 1188, 1408,  784, 1408,  784}, //20
};
const UINT32 VDOZOOM_INFOR2_MODE_1_TABLE[20][DZOOM_ITEM_MAX] =
{
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //1
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //2
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //3
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //4
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //5
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080},//6
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //7
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //8
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //9
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //10
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //11
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //12
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //13
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //14
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //15
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //16
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080},//17
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //18
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //19
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080},//20
};
const UINT32 VDOZOOM_INFOR2_MODE_2_TABLE[20][DZOOM_ITEM_MAX] =
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

const UINT32 VDOZOOM_INFOR_MODE_5_TABLE[20][DZOOM_ITEM_MAX] =
{
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //1
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //2
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //3
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //4
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //5
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //6
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //7
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //8
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //9
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //10
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //11
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //12
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //13
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //14
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //15
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //16
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //17
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //18
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //19
    {1280,  720, 1280,  720, 1280,  720, 1280,  720}, //20
};


const UINT32 VDOZOOM_INFOR_MODE_6_TABLE[20][DZOOM_ITEM_MAX] =
{
    {668,  372, 668,  372, 668,  372, 668,  372}, //1
    {668,  372, 668,  372, 668,  372, 668,  372}, //2
    {668,  372, 668,  372, 668,  372, 668,  372}, //3
    {668,  372, 668,  372, 668,  372, 668,  372}, //4
    {668,  372, 668,  372, 668,  372, 668,  372}, //5
    {668,  372, 668,  372, 668,  372, 668,  372}, //6
    {668,  372, 668,  372, 668,  372, 668,  372}, //7
    {668,  372, 668,  372, 668,  372, 668,  372}, //8
    {668,  372, 668,  372, 668,  372, 668,  372}, //9
    {668,  372, 668,  372, 668,  372, 668,  372}, //10
    {668,  372, 668,  372, 668,  372, 668,  372}, //11
    {668,  372, 668,  372, 668,  372, 668,  372}, //12
    {668,  372, 668,  372, 668,  372, 668,  372}, //13
    {668,  372, 668,  372, 668,  372, 668,  372}, //14
    {668,  372, 668,  372, 668,  372, 668,  372}, //15
    {668,  372, 668,  372, 668,  372, 668,  372}, //16
    {668,  372, 668,  372, 668,  372, 668,  372}, //17
    {668,  372, 668,  372, 668,  372, 668,  372}, //18
    {668,  372, 668,  372, 668,  372, 668,  372}, //19
    {668,  372, 668,  372, 668,  372, 668,  372}, //20
};


const UINT32 VDOZOOM_INFOR_MODE_7_TABLE[20][DZOOM_ITEM_MAX] =
{
    {2688, 1508, 2688, 1508, 2688, 1508, 2688, 1508}, //1
    {2608, 1472, 2608, 1472, 2608, 1472, 2608, 1472}, //2
    {2544, 1424, 2544, 1424, 2544, 1424, 2544, 1424}, //3
    {2480, 1392, 2480, 1392, 2480, 1392, 2480, 1392}, //4
    {2416, 1360, 2416, 1360, 2416, 1360, 2416, 1360}, //5
    {2352, 1312, 2352, 1312, 2352, 1312, 2352, 1312}, //6
    {2272, 1280, 2272, 1280, 2272, 1280, 2272, 1280}, //7
    {2208, 1232, 2208, 1232, 2208, 1232, 2208, 1232}, //8
    {2144, 1200, 2144, 1200, 2144, 1200, 2144, 1200}, //9
    {2080, 1168, 2080, 1168, 2080, 1168, 2080, 1168}, //10
    {2016, 1120, 2016, 1120, 2016, 1120, 2016, 1120}, //11
    {1936, 1088, 1936, 1088, 1936, 1088, 1936, 1088}, //12
    {1872, 1056, 1872, 1056, 1872, 1056, 1872, 1056}, //13
    {1808, 1008, 1808, 1008, 1808, 1008, 1808, 1008}, //14
    {1744,  976, 1744,  976, 1744,  976, 1744,  976}, //15
    {1680,  944, 1680,  944, 1680,  944, 1680,  944}, //16
    {1600,  896, 1600,  896, 1600,  896, 1600,  896}, //17
    {1536,  864, 1536,  864, 1536,  864, 1536,  864}, //18
    {1472,  816, 1472,  816, 1472,  816, 1472,  816}, //19
    {1408,  784, 1408,  784, 1408,  784, 1408,  784}, //20
};

const UINT32 VDOZOOM_INFOR_MODE_8_TABLE[20][DZOOM_ITEM_MAX] =
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

const UINT32 VDOZOOM_INFOR_MODE_9_TABLE[1][DZOOM_ITEM_MAX] =
{
    //{640,  472, 640,  472, 640,  472, 640,  472}, //1
    {668,  372, 668,  372, 668,  372, 668,  372}, //1
};

UINT32* SenMode2Tbl(UINT32 Id, UINT32 SenMode, UINT32 *DzMaxidx)
{
    UINT32 *Ptr = NULL;
    //UINT32 Video_FPS = IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_FPS);
    UINT32 Video_SIZE = IPL_AlgGetUIInfo(Id, IPL_SEL_VIDEO_SIZE);
    IPL_MODE IPL_Mode = IPL_CtrlGetInfor(Id, IPLCTRL_CUR_MODE);

    if ( Id == IPL_ID_1 || (Id == IPL_ID_2 && IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_EXT_FUNC_EN) == ICF_FUNC_SHDR) )
    {
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
#if 0
                if (1) //IPL_CtrlGetInfor(IPL_ID_1, IPLCTRL_IME_P1_D2H264) == TRUE || IPL_CtrlGetInfor(IPL_ID_2, IPLCTRL_IME_P1_D2H264) == TRUE )
                {
                    Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_3SSTRP_TABLE[0][0];
                    *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_3SSTRP_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                }
                else if (Id == IPL_ID_1 && Sensor_IsOpen(IPL_ID_2) == TRUE)
                {
                    Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_3SSTRP_TABLE[0][0];
                    *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_3SSTRP_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                }
                else
                {
                    Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_3_TABLE[0][0];
                    *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_3_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                }
#endif
                if ( Video_SIZE < SEL_VIDEOSIZE_1920x1080 && IPL_Mode != IPL_MODE_PREVIEW && IPL_Mode != IPL_MODE_VIDEO)
                {
                    Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_3_TABLE[0][0];
                    *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_3_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                }
                else
                {
                    Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_3SSTRP_TABLE[0][0];
                    *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_3SSTRP_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                }
                break;

            case SENSOR_MODE_4:
                Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_4_TABLE[0][0];
                *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_4_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                break;

            case SENSOR_MODE_5:
                Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_5_TABLE[0][0];
                *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_5_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                break;

            case SENSOR_MODE_6:
                Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_6_TABLE[0][0];
                *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_6_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                break;

            case SENSOR_MODE_7:
                Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_7_TABLE[0][0];
                *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_7_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                break;

            case SENSOR_MODE_8:
                Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_8_TABLE[0][0];
                *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_8_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                break;

            case SENSOR_MODE_9:
                Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_9_TABLE[0][0];
                *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_9_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
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
                Ptr = (UINT32*)&VDOZOOM_INFOR2_MODE_1_TABLE[0][0];
                *DzMaxidx = (sizeof(VDOZOOM_INFOR2_MODE_1_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                break;

            case SENSOR_MODE_2:
                Ptr = (UINT32*)&VDOZOOM_INFOR2_MODE_2_TABLE[0][0];
                *DzMaxidx = (sizeof(VDOZOOM_INFOR2_MODE_2_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
                break;

            default:
                Ptr = NULL;
                *DzMaxidx = 0;
                break;
        }
    }
    return Ptr;
}
