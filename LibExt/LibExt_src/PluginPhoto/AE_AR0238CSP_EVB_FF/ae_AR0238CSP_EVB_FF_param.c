/**
    Auto Exposure parameter.

    ae parameter.

    @file       ae_sample_param.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "Type.h"
#include "ae_alg.h"
#include "ae_AR0238CSP_EVB_FF_int.h"

AE_PARAMINFO AeParamInfo[AE_PROC_CNT] =
{
	#if 0//fying
    {
        ((1 << 8) * 10),
        {((1 << 18) * 10), ((1 << 0) * 10)},
        {6400, 125},
        {6400, 125},
        {60<<4, 30<<4},
        496,//484,//492,//428,
        496,//500,//428,
        486,
        880,//869,
        {220<<2, 200<<2},
        {40<<2, 24<<2},
    },
    #else

     {
        ((1 << 8) * 10),
        {((1 << 18) * 10), ((1 << 0) * 10)},
        {6400, 50},
        {6400, 50},
        {60<<4, 30<<4},
        484,//496,//492,//428,
        486,//496,//500,//428,
        486,
        880,//869,
        {220<<2, 200<<2},
        {40<<2, 24<<2},
    },
#endif
    {
        ((1 << 8) * 10),
        {((1 << 18) * 10), ((1 << 3) * 10)},
        {1600, 125},
        {1600, 125},
        {60<<4, 30<<4},
        484,//492,//428,
        496,//500,//428,
        486,
        869,
        {220<<2, 200<<2},
        {40<<2, 24<<2},
    },
};

AE_SET_PARAMINFO AeExtSetDefaultParamInfo[AE_PROC_CNT] =
{
    {
        AE_PLINE_PREDEF, {33333,100}, {6400, 50},
        AE_IRIS_TYPE_FIXED, AE_IRIS_MODE_OPEN, 10, AEALG_IRIS_BIG, {AEALG_IRIS_BIG, AEALG_IRIS_SMALL},
        {AE_COVER_RANGE_H, AE_COVER_RANGE_L},AEALG_SPEED_N2,0,ENABLE,
    },
    {
        AE_PLINE_PREDEF, {33333,100}, {1600, 125},
        AE_IRIS_TYPE_FIXED, AE_IRIS_MODE_OPEN, 10, AEALG_IRIS_BIG, {AEALG_IRIS_BIG, AEALG_IRIS_SMALL},
        {AE_COVER_RANGE_H, AE_COVER_RANGE_L},AEALG_SPEED_P1,0,ENABLE,
    },
};

UINT32 ExptY_Mov_Prv[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX] =
{
//LV  0,   1,   2,   3,   4,   5,   6,   7,    8,    9,    10,   11,    12,   13,   14,   15    16    17    18    19    20
    //{40,  40,  40,  45,  50,  65,  70,  72,   72,   72,    73,   74,    75,   75,  85,  85,  85,  85,  85,  85,  85},    //20151111  14~ 10%
    //{40,  40,  40,  45,  55,  65,  75,  83,   95,   96,    97,   98,    98,   99,  100,  100,  100,  100,  100,  100,  100},    //0911
//    {45,  45,  45,  45,  55,  65,  75,  83,   95,   95,    95,   95,    95,   95,  95,  95,  95,  95,  95,  95,  95},    //0118
//      {45,  45,  45,  48,  55,  68,  78,  86,   95,   95,    95,   95,    95,   95,  95,  95,  95,  95,  95,  95,  95},    //0119
//      {30,  40,  45,  48,  55,  68,  78,  86,   95,   95,    95,   95,    95,   95,  95,  95,  95,  95,  95,  95,  95},    //0126
     // {20,  32,  45,  48,  55,  68,  78,  86,   92,   92,    92,   92,    92,   92,  92,  92,  95,  95,  95,  95,  95},    //0219
	//{20,	32,  45,  48,  55,	68,  78,  80,	80,   80,	 80,   80,	  80,	80,  80,  80, 80,	80,  80,  80,  80},    //0219
		{30,  30,  30,	30,  46,  52,  62,	66,   75,	80,    92,	 90,	90,  90,	90,  90,	90,  80,  50,  30,	10}, 

//    {40,  40,  40,  45,  50,  65,  70,  72,   72,   72,    78,   82,    85,   85,  85,  85,  85,  85,  85,  85,  85},    //20151204 +10%
//    {40,  40,  40,  45,  50,  65,  70,  72,   72,   72,    75,   78,    80,   80,  80,  80,  80,  80,  80,  80,  80},    //20151204 + 5%
//    {40,  40,  40,  45,  50,  65,  70,  72,   72,   72,    73,   74,    75,   75,  75,  75,  75,  75,  75,  75,  75},    //20151111
//    {40,  40,  40,  45,  50,  65,  70,  75,   90,   96,    97,   98,    98,   99,  100,  100,  100,  100,  100,  100,  100},    //20151111
//    {40,  40,  40,  40,  50,  65,  75,  83,   95,   96,    97,   98,    98,   99,  100,  100,  100,  100,  100,  100,  100},    //0911
    {42,  42,  42,  42,  54,  70,  80,  88,   95,   96,    97,   98,    98,   99,  100,  100,  100,  100,  100,  100,  100},    //0820
//  {33,  33,  33,  33,  33,  33,  50,  75,   85,   90,   100,  100,   100,  100,  100,  100,  100,  100,  100,  100,  100},    //0427
//  {33,  33,  33,  33,  33,  33,  50,  75,   85,   90,   100,  100,   100,  100,  100,  100,  100,  100,  100,  100,  100},    //0427
//  {33,  33,  33,  35,  38,  42,  58,  80,   90,   95,   100,  100,   100,  100,  100,  100,  100,  100,  100,  100,  100},    //sync IMX322
//  {33,  33,  33,  33,  33,  33,  50,  70,   80,   90,   100,  100,   100,  100,  100,  100,  100,  100,  100,  100,  100},    //first ver
//  {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},// Fixed Mov expY
};


UINT32 ExptY_Mov_PrvHDR[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX] =
{
//LV  0,   1,   2,   3,   4,   5,   6,   7,    8,    9,    10,   11,    12,   13,   14,   15    16    17    18    19    20
//{39,39,39,39,39,39,39,63,76,84,100,100,100,100,100,100,100,100,100,100,100},
//{39,39,39,39,39,39,39,63,76,84,100,100,100,100,100,100,100,100,100,100,100},
  {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},// Fixed Mov expY
  {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},// Fixed Mov expY
//  {33,  33,  33,  35,  38,  42,  58,  80,   90,   95,   100,  100,   100,  100,  100,  100,  100,  100,  100,  100,  100},    //sync IMX322
//  {33,  33,  33,  33,  33,  33,  50,  70,   80,   90,   100,  100,   100,  100,  100,  100,  100,  100,  100,  100,  100},    //first ver
//  {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},// Fixed Mov expY
};



UINT32 ExptY_Photo_Prv[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX] =
{
//LV 0,   1,   2,   3,   4,   5,   6,   7,    8,    9,    10,   11,   12,   13,   14,   15    16    17    18    19    20
    {60,  60,  65,  70,  76,  83,  90,  96,  99,   99,   99,   100,   100,  100,  100,  100,  100,  100,  100,  100,  100},     //first ver
    {60,  60,  65,  70,  76,  83,  90,  96,  99,   99,   99,   100,   100,  100,  100,  100,  100,  100,  100,  100,  100},     //first ver
//     {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},// Fixed Photo expY
};

UINT32 Histo_Prv[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX] =
{
    //LV 0,   1,   2,   3,   4,   5,   6,   7,    8,    9,    10,   11,   12,   13,   14,   15    16    17    18    19    20
   // {15,  25,  30,  40,  50,  50,  50,  50,   50,   70,    90,  100,   100,  100,  100,  100,  100,  100,  100,  100,  100},     //first ver
	{15,  25,  30,	40,  50,  50,  50,	50,   50,	70,    90,	100,   100,  100,  100,  100,  100,  100,  100,  100,  100},	 //first ver

    {50,  50,  50,  50,  50,  50,  50,  50,   50,   70,    90,  100,   100,  100,  100,  100,  100,  100,  100,  100,  100},     //first ver
    //100, 100, 100, 100, 100, 100, 100, 100,  100,   100,   100,   100,   100,  100,  100,  100,  100,  100,  100,  100,  100
};

UINT32 ExptY_OverExpoThreshold[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX] =
{
//LV  0,   1,   2,   3,   4,   5,  6,  7,   8,    9,    10,   11,   12,   13,   14,   15    16    17    18    19    20
    {100,  100,   100,  100,  100,  100, 100, 100,  100,   100,   100,   100,  100,  100,  100,  100,  100,  100,  100,  100,  100},    //IMX322
    {100,  100,   100,  100,  100,  100, 100, 100,  100,   100,   100,   100,  100,  100,  100,  100,  100,  100,  100,  100,  100},    //IMX322
//     {0, 0, 0, 60, 60, 60, 60, 70, 80, 90, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100},    //first ver
};

UINT32 ExptY_OverExpoMaxCnt[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX] =
{
//LV 0,   1,   2,   3,   4,   5,  6,  7,   8,    9,    10,   11,   12,   13,   14,   15    16    17    18    19    20
     {40, 40,  40,  40,  40,  40,  40,  40,  30,   30,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20}, //0729
//   {40, 40,  40,  40,  40,  40,  40,  40,  30,   30,   20,   18,   18,   18,   18,   18,   18,   18,   18,   18,   18}, //0729
     {40, 40,  40,  40,  40,  40,  40,  40,  30,   30,   20,   18,   18,   18,   18,   18,   18,   18,   18,   18,   18}, //0729
//   {40, 40,  40,  40,  40,  40,  40,  40,  30,   30,   20,   15,   15,   15,   15,   15,   15,   15,   15,   15,   15},   //IMX322
//   {40, 40,  40,  40,  40,  40,  40,  40,  30,   30,   20,   15,   15,   15,   15,   15,   15,   15,   15,   15,   15},   //IMX322
//   {10, 10,  10,  10,  10,  10, 10, 10,  10,   12,    15,   15,   15,   15,   15,   15,   15,   15,   15,   15,   15},   //first ver
//   {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024}, //disable
};

UINT32 ExptY_OverExpoMinCnt[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX] =
{
//LV 0,   1,   2,   3,   4,   5,  6,  7,   8,    9,    10,   11,   12,   13,   14,   15    16    17    18    19    20
     {3,   3,   3,   3,   3,   3,  3,  3,   3,    3,     5,    5,    8,    8,    8,    8,    8,    8,    8,    8,    8},    //IMX322
//     {3,   3,   3,   3,   3,   3,  3,  3,   3,    3,     3,    3,    3,    3,    3,    3,    3,    3,    3,    3,    3},    //IMX322
     {3,   3,   3,   3,   3,   3,  3,  3,   3,    3,     3,    3,    3,    3,    3,    3,    3,    3,    3,    3,    3},    //IMX322
//     {3,  3,  3,   3,   3,   3,  3,  3,   3,    5,     8,    8,    8,    8,    8,    8,    8,    8,    8,    8,   8},     //first ver
//     {0,  0,   0,   0,   0,   0,  0,  0,   0,    0,     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}, //disable
};

UINT32 ExptY_Mov_OverExpoAdj[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX] =
{
//LV 0,   1,   2,   3,   4,   5,  6,  7,   8,    9,    10,   11,   12,   13,   14,   15    16    17    18    19    20
//    {20,  20,  25,  35,  45,  50, 50, 60, 85,  113,   113,  115,  115,  120,  120,  120,  120,  120,  120,  120,  120},  //20160122
//  {20,  20,  25,  35,  45,  50, 50, 60, 85,  110,   110,  115,  115,  120,  120,  120,  120,  120,  120,  120,  120},  //20151125
//  {20,  20,  25,  35,  45,  50, 50, 60, 85,  110,   110,  120,  120,  120,  120,  120,  120,  120,  120,  120,  120},  //20151124
//  {20,  20,  25,  35,  45,  50, 50, 60, 85,  110,   110,  120,  130,  130,  130,  130,  130,  130,  130,  130,  130},  //20151117
//  {20,  20,  25,  35,  45,  50, 50, 60, 85,  110,   110,  110,  110,  110,  110,  110,  110,  110,  110,  110,  110},  //20151106
{20,  20,  25,  35,  45,  50, 50, 60, 85,  113,   113,  115,  115,  120,  120,  120,  120,  120,  120,  120,  120},  //20160122

// {20,  20,  25,  35,  45,  50, 50, 60, 85,  110,   136,  140,  180,  180,  180,  180,  180,  180,  180,  180,  180},  //0820
    {20,  20,  25,  35,  45,  50, 50, 60, 85,  110,   136,  140,  160,  160,  160,  160,  160,  160,  160,  160,  160},  //0820
//  {10,  10,  20,  30,  40,  50, 50, 60, 85,  110,   137,  147,  152,  156,  156,  156,  156,  156,  156,  156,  156},  //0428
//  {10,  10,  20,  30,  40,  50, 50, 60, 85,  110,   137,  147,  152,  156,  156,  156,  156,  156,  156,  156,  156},  //0428
//  {10,  10,  20,  30,  40,  50, 50, 60, 85,  110,   137,  147,  157,  165,  165,  165,  165,  165,  165,  165,  165},  //0427
//  {10,  10,  20,  30,  40,  50, 50, 60,  85,  110,   137,  147,  147,  147,  147,  147,  147,  147,  147,  147,  147},  //IMX322
//   {0,   0,  0,   10,  10,   10, 10, 23,  48,   64,  100,  120,  120,  120,  120,  120,  120,  120,  120,  120,  120},
//   {0,   0,  0,   10,  10,   10, 10, 23,  48,   64,  100,  120,  137,  137,  137,  137,  137,  137,  137,  137,  137},
//   {0,  0,   0,   0,   0,   0,  0,  0,   0,    0,     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}, //disable
};

UINT32 ExptY_Mov_OverExpoAdj_HDR16X[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX] =
{
//LV 0,   1,   2,   3,   4,   5,  6,  7,   8,    9,    10,   11,   12,   13,   14,   15    16    17    18    19    20
     {0,   0,  0,   2,   2,   2,  2,  6,   12,   16,   25,   30,   34,   34,   34,   34,   34,   34,   34,   34,   34},
     {0,   0,  0,   2,   2,   2,  2,  6,   12,   16,   25,   30,   34,   34,   34,   34,   34,   34,   34,   34,   34},
//     {0,  0,   0,   0,   0,   0,  0,  0,   0,    0,     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}, //disable
};

UINT32 ExptY_Mov_OverExpoAdj_HDR4X[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX] =
{
//LV 0,   1,   2,   3,   4,   5,  6,  7,   8,    9,    10,   11,   12,   13,   14,   15    16    17    18    19    20
     {0,   0,  0,   5,   5,   5,  5,  11,  24,   32,   50,   60,   68,   68,   68,   68,   68,   68,   68,   68,   68},
     {0,   0,  0,   5,   5,   5,  5,  11,  24,   32,   50,   60,   68,   68,   68,   68,   68,   68,   68,   68,   68},
//     {0,  0,   0,   0,   0,   0,  0,  0,   0,    0,     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}, //disable
};

UINT32 ExptY_Photo_OverExpoAdj[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX] =
{
//LV 0,   1,   2,   3,   4,   5,  6,  7,   8,    9,    10,   11,   12,   13,   14,   15    16    17    18    19    20
    {0,   0,   0,   10,  10, 10, 10, 23,  48,   64,  100,  120,  132,  132,  132,  132,  132,  132,  132,  132,  132},  //0820
    {0,   0,   0,   10,  10, 10, 10, 23,  48,   64,  100,  120,  132,  132,  132,  132,  132,  132,  132,  132,  132},  //0820
//     {0,   0,  0,   10,  10,   10, 10, 23,  48,   64,  100,  120,  137,  137,  137,  137,  137,  137,  137,  137,  137},
//     {0,   0,  0,   10,  10,   10, 10, 23,  48,   64,  100,  120,  137,  137,  137,  137,  137,  137,  137,  137,  137},
//     {0,  0,   0,   0,   0,   0,  0,  0,   0,    0,     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0},
//     {0,  0,   0,   0,   0,   0,  0,  0,   0,    0,     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0},
};

UINT32 ExptY_Photo_OverExpoAdj_HDR16X[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX] =
{
//LV 0,   1,   2,   3,   4,   5,  6,  7,   8,    9,    10,   11,   12,   13,   14,   15    16    17    18    19    20
     {0,   0,  0,   2,   2,   2,  2,  6,   12,   16,   25,   30,   34,   34,   34,   34,   34,   34,   34,   34,   34},
     {0,   0,  0,   2,   2,   2,  2,  6,   12,   16,   25,   30,   34,   34,   34,   34,   34,   34,   34,   34,   34},
//     {0,  0,   0,   0,   0,   0,  0,  0,   0,    0,     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}, //disable
};

UINT32 ExptY_Photo_OverExpoAdj_HDR4X[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX] =
{
//LV 0,   1,   2,   3,   4,   5,  6,  7,   8,    9,    10,   11,   12,   13,   14,   15    16    17    18    19    20
     {0,   0,  0,   5,   5,   5,  5,  11,  24,   32,   50,   60,   68,   68,   68,   68,   68,   68,   68,   68,   68},
     {0,   0,  0,   5,   5,   5,  5,  11,  24,   32,   50,   60,   68,   68,   68,   68,   68,   68,   68,   68,   68},
//     {0,  0,   0,   0,   0,   0,  0,  0,   0,    0,     0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0}, //disable
};

UINT32 ExptY_Cap[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX] =
{
//LV  0,   1,   2,   3,   4,   5,   6,   7,   8,    9,   10,    11,    12,   13,   14,   15    16    17    18    19    20
     {60,  60,  65,  70,  76,  83,  90,  96,  99,   99,   99,   100,   100,  100,  100,  100,  100,  100,  100,  100,  100},
     {60,  60,  65,  70,  76,  83,  90,  96,  99,   99,   99,   100,   100,  100,  100,  100,  100,  100,  100,  100,  100},
    //100, 100, 100, 100, 100, 100, 100, 100,  100,   100,   100,   100,   100,  100,  100,  100,  100,  100,  100,  100,  100
};

UINT8 AE_Win_Face[AE_WIN_Y][AE_WIN_X] =
{
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

UINT8 AE_Win_UserDef[AE_WIN_Y][AE_WIN_X] =
{
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1}
};

UINT8 AE_Win_Adaptive[AE_WIN_Y][AE_WIN_X] =
{
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1}
};

const UINT8 AE_Win_CenterWeighted[AE_WIN_Y][AE_WIN_X] =
{
    {1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,1},
    {1,2,3,3,3,3,2,1},
    {1,2,3,4,4,3,2,1},
    {1,2,3,4,4,3,2,1},
    {1,2,3,3,3,3,2,1},
    {1,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,1,1}
};

const UINT8 AE_Win_Spot[AE_WIN_Y][AE_WIN_X] =
{
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
};

const UINT8 AE_Win_Multi[AE_WIN_Y][AE_WIN_X] =
{
    {3,3,2,1,1,2,3,3},
    {3,3,2,2,2,2,3,3},
    {2,2,6,6,6,6,2,2},
    {1,2,6,8,8,6,2,1},
    {1,2,6,8,8,6,2,1},
    {2,2,6,6,6,6,2,2},
    {3,3,2,2,2,2,3,3},
    {3,3,2,1,1,2,3,3}
};

const UINT8 AE_Win_Evaluative[AE_WIN_Y][AE_WIN_X] =
{
#if 1   //20151201
    {0,0,0,0,0,0,0,0},
    {1,1,0,0,0,0,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,2,2,1,1,1},
    {1,2,2,3,3,2,2,1},
    {1,3,5,6,6,5,3,1},
    {0,1,2,3,3,2,1,0},
    {0,0,0,0,0,0,0,0}
#endif
#if 0   //20151106
    {0,1,0,0,0,0,1,0},
    {1,1,0,0,0,0,1,1},
    {1,1,1,0,0,1,1,1},
    {1,1,1,2,2,1,1,1},
    {1,2,2,3,3,2,2,1},
    {1,3,5,6,6,5,3,1},
    {0,1,2,3,3,2,1,0},
    {0,0,0,0,0,0,0,0}
#endif
#if 0   //0618_CarDV
    {0,1,0,0,0,0,1,0},
    {1,1,0,0,0,0,1,1},
    {1,1,1,0,0,1,1,1},
    {1,1,1,3,3,1,1,1},
    {1,1,3,6,6,3,1,1},
    {1,2,3,6,6,3,2,1},
    {0,1,2,3,3,2,1,0},
    {0,0,0,0,0,0,0,0}
#endif
#if 0
    {0,1,0,0,0,0,1,0},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,2,2,3,3,2,2,1},
    {1,2,3,6,6,3,2,1},
    {1,2,4,6,6,4,2,1},
    {1,2,3,4,4,3,2,1},
    {0,1,1,1,1,1,1,0}
#endif
};

const UINT8 AE_OverExpo_Win_Normal[AE_WIN_Y][AE_WIN_X] =
{
#if 1
    {0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1},
    {1,1,1,3,3,1,1,1},
    {1,1,2,4,4,2,1,1},
    {1,3,4,6,6,4,3,1},
    {0,1,2,3,3,2,1,0},
    {0,0,0,0,0,0,0,0}
#endif
#if 0
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,3,3,1,1,1},
    {1,1,3,6,6,3,1,1},
    {1,1,3,6,6,3,1,1},
    {1,1,1,1,1,1,1,1}
#endif
#if 0
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1}
#endif
};


// use dark target Y for ae converge,
const UINT32 AeCurve_HDR16X[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,        0,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {87104,        980,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {43552,        980,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {21776,       1960,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {10888,       1960,       400,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {5444,        1960,       800,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {2722,        1960,      1600,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {1361,        1960,      3200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {680,         1960,      3200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {  0,         1960,      3200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

// use dark target Y for ae converge,
const UINT32 AeCurve_HDR4X[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,        0,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {102400,       833,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {51200,        833,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {25600,       1667,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {12800,       3333,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {6400,        6667,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {3200,        6667,       400,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {1600,        6667,       800,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {800,         6667,      1600,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {400,         6667,      3200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {200,         6667,      3200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {  0,         6667,      3200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

// use dark target Y for ae converge,
const UINT32 AeCurve_HDR1X[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,        0,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,       8333,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {5120,        8333,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {2560,       16666,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {1280,       16666,       400,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {640,        16666,       800,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {320,        16666,      1600,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {160,        16666,      3200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {  0,        16666,      3200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

//Anti-flicker:60Hz
const UINT32 AeCurve_AntiFlicker60[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,        0,       125,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    //{10240,       8333,       125,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {8192,        8333,       125,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {5120,        8333,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {2560,       16666,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {1280,       33333,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {640,        33333,       400,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {320,        33333,       800,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {160,        33333,      1600,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    { 80,        33333,      3200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {  0,        33333,      3200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

//Anti-flicker:60Hz 60fps
const UINT32 AeCurve_AntiFlicker60_60fps[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,        0,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,       8333,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {5120,        8333,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {2560,       16666,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {1280,       16666,       400,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {640,        16666,       800,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {320,        16666,      1600,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {  0,        16666,      3200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

//Anti-flicker:60Hz 120fps
const UINT32 AeCurve_AntiFlicker60_120fps[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,     8333,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {5120,      8333,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {  0,       8333,      3200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

//Anti-flicker:50Hz
const UINT32 AeCurve_AntiFlicker50[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,        0,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {8533,       10000,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {4266,       10000,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {2133,       20000,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {1422,       30000,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {711,        30000,       400,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {355,        30000,       800,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {177,        30000,      1600,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {    0,      30000,      3200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

//Anti-flicker:50Hz 60fps
const UINT32 AeCurve_AntiFlicker50_60fps[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,        0,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {8533,       10000,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {4266,       10000,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {    0,      15000,      1600,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

//Anti-flicker:50Hz 120fps
const UINT32 AeCurve_AntiFlicker50_120fps[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,        0,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {11377,       7500,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {5689,        7500,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {    0,       7500,      1600,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO50_60Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {10240,    16666,       50,     AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {5120,     33333,       50,     AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {2560,     66666,       50,     AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {1280,    133333,       50,     AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {640,     266666,       50,     AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {320,     533333,       50,     AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {160,    1066667,       50,     AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {0,      1066667,       50,     AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO50_50Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {8533,     20000,       50,     AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/100
    {4266,     40000,       50,     AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/50
    {2133,     80000,       50,     AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/25
    {1066,    160000,       50,     AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/12.5
    {533,     320000,       50,     AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {266,     640000,       50,     AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/3.125
    {133,    1280000,       50,     AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/1.5625
    {0,      1280000,       50,     AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/1.5625
    {AEALG_TAB_END, 0, 0, 0, 0}
};


const UINT32 AeCurve_ISO80_60Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {8192,      8333,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/120
    {4096,     16666,        80,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/60
    {2048,     33333,        80,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/30
    {1024,     66666,        80,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15
    {512,     133333,        80,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {256,     266667,        80,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {128,     533333,        80,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {0,       533333,        80,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {AEALG_TAB_END, 0, 0, 0, 0}
};
     
const UINT32 AeCurve_ISO80_50Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {7082,     10000,        80,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/100
    {3412,     20000,        80,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/50
    {1706,     40000,        80,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/25
    {1066,     80000,        80,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/12.5
    {852,     160000,        80,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {212,     320000,        80,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {106,     640000,        80,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {0,       640000,        80,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {AEALG_TAB_END, 0, 0, 0, 0}  
};

const UINT32 AeCurve_ISO100_60Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,     8333,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/120
    {5120,     16666,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/60
    {2560,     33333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/30
    {1280,     66666,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15
    {640,     133333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {320,     266667,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {160,     533333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {0,       533333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO100_50Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {8533,     10000,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/100
    {4266,     20000,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/50
    {2133,     40000,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/25
    {1066,     80000,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/12.5
    {533,     160000,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {266,     320000,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {133,     640000,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {0,       640000,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO200_60Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,     4166,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/240
    {5120,      8333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/120
    {2560,     16666,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/60
    {1280,     33333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/30
    {640,      66666,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15
    {320,     133333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {160,     266667,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {0,       266667,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO200_50Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {34133,     5000,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/200
    {8533,      5000,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/200
    {4266,     10000,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/100
    {2133,     20000,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/50
    {1066,     40000,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/25
    {533,      80000,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/12.5
    {266,     160000,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {133,     320000,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {0,       320000,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO400_60Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,     2083,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/480
    {5120,      4166,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/240
    {2560,      8333,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/120
    {1280,     16666,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/60
    {640,      33333,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/30
    {320,      66666,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15
    {160,     133333,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {0,       133333,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO400_50Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {8533,      2500,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/400
    {4266,      5000,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/200
    {2133,     10000,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/100
    {1066,     20000,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/50
    {533,      40000,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/25
    {266,      80000,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/12.5
    {133,     160000,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {0,       160000,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO800_60Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,     1042,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/480
    {5120,      2083,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/240
    {2560,      4166,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/120
    {1280,      8333,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/60
    {640,      16666,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/30
    {320,      33333,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15
    {160,      66666,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {0,        66666,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO800_50Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {8533,      1250,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/400
    {4266,      2500,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/200
    {2133,      5000,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/100
    {1066,     10000,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/50
    {533,      20000,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/25
    {266,      40000,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/12.5
    {133,      80000,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {0,        80000,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO1600_60Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,      521,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/480
    {5120,      1042,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/240
    {2560,      2083,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/120
    {1280,      4166,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/60
    {640,       8333,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/30
    {320,      16666,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15
    {160,      33333,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {0,        66666,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO1600_50Hz[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {8533,       625,      1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/400
    {4266,      1250,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/200
    {2133,      2500,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/100
    {1066,      5000,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/50
    {533,      10000,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/25
    {266,      20000,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/12.5
    {133,      40000,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {0,        80000,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/6.25
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_Landscape[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,     8333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN}, // 1/60*2
    {5120,     16666,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN}, // 1/60*2
    {2560,     33333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN}, // 1/60*2
    {1280,     33333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {640,      33333,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {320,      33333,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15*2
    {160,      66666,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {  0,     133333,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_NightLandscape[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,     8333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN}, // 1/60*2
    {5120,     16666,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN}, // 1/60*2
    {2560,     33333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN}, // 1/60*2
    {1280,     66666,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {640,     133333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {320,     266667,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15*2
    {160,     533333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    { 80,     533333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {  0,     533333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_Action[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,     8333,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},//
    {5120,      8333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {2560,      8333,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {1280,     16666,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {640,      16666,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {320,      16666,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {160,      33333,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {  0,     133333,      1600,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},//
    {AEALG_TAB_END, 0, 0, 0, 0}
};


const UINT32 AeCurve_Potrait[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,     8333,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP}, // 1/60*2
    {5120,      8333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/60*2
    {2560,     16666,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/30*2
    {1280,     33333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15*2
    {640,      33333,       400,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15*2
    {320,      33333,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/15*2
    {160,      66666,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {  0,     133333,       800,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},// 1/7.5*2
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_NightPotrait[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,     8333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {5120,     16666,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {2560,     33333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {1280,     66666,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {640,     133333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {320,     266666,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {160,     533333,       100,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    { 80,     533333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {  0,     533333,       200,    AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO25_NOR[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       25,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {  0,     133333,       25,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO50_NOR[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       50,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {  0,     133333,       50,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO100_NOR[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {  0,     133333,       100,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO200_NOR[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {  0,     133333,       200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO400_NOR[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {  0,     133333,       400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO800_NOR[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {  0,     133333,       800,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO1600_NOR[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {  0,     133333,       1600,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO3200_NOR[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       3200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {  0,     133333,       3200,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

const UINT32 AeCurve_ISO6400_NOR[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       6400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {  0,     133333,       6400,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};


const UINT32 AeCurve_ISO12800_NOR[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,      0,       12800,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {  0,     133333,       12800,    AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

//Anti-flicker:60Hz
const UINT32 AeCurve_Cap60[][AEALG_INFO_MAX] =
{
    {3000000,        0,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {10240,       8333,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {5120,        8333,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {2560,       16666,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {1280,       33333,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {640,        66666,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {320,        66666,       400,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {160,        66666,       800,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {  0,        66666,      1600,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {AEALG_TAB_END, 0, 0, 0, 0}
};

//Anti-flicker:50Hz
const UINT32 AeCurve_Cap50[][AEALG_INFO_MAX] =
{
    // EV       ExpoTime    ISO     IrisStep
    {3000000,        0,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {8533,       10000,       100,  AEALG_IRIS_0,          AEALG_ADJ_EXP},
    {4266,       10000,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {2133,       20000,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {1422,       30000,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {710,        60000,       200,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {355,        60000,       400,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {178,        60000,       800,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {    0,      60000,      1600,  AEALG_IRIS_0,          AEALG_ADJ_GAIN},
    {AEALG_TAB_END, 0, 0, 0, 0}
};
