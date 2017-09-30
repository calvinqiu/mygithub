/**
    IPL dzoom table sample.

    @file       IPL_dzoomTableSample.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/

#include "IPL_AR0238CS_EVB_FF_Int.h"

//for AR0238CS mode 1 1920 x 1080 p30
const UINT32 VDOZOOM_INFOR_MODE_1_TABLE[61][DZOOM_ITEM_MAX] =
{
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //1
    {1920, 1080, 1920, 1080, 1856, 1044, 1856, 1044}, //2
    {1920, 1080, 1920, 1080, 1792, 1008, 1792, 1008}, //3
    {1920, 1080, 1920, 1080, 1732,  972, 1732,  972}, //4
    {1920, 1080, 1920, 1080, 1672,  940, 1672,  940}, //5
    {1920, 1080, 1920, 1080, 1616,  908, 1616,  908}, //6
    {1920, 1080, 1920, 1080, 1560,  876, 1560,  876}, //7
    {1920, 1080, 1920, 1080, 1508,  848, 1508,  848}, //8
    {1920, 1080, 1920, 1080, 1456,  820, 1456,  820}, //9
    {1920, 1080, 1920, 1080, 1404,  792, 1404,  792}, //10
    {1920, 1080, 1920, 1080, 1356,  764, 1356,  764}, //11
    {1920, 1080, 1920, 1080, 1312,  736, 1312,  736}, //12
    {1920, 1080, 1920, 1080, 1268,  712, 1268,  712}, //13
    {1920, 1080, 1920, 1080, 1224,  688, 1224,  688}, //14
    {1920, 1080, 1920, 1080, 1180,  664, 1180,  664}, //15
    {1920, 1080, 1920, 1080, 1140,  644, 1140,  644}, //16
    {1920, 1080, 1920, 1080, 1104,  620, 1104,  620}, //17
    {1920, 1080, 1920, 1080, 1064,  600, 1064,  600}, //18
    {1920, 1080, 1920, 1080, 1028,  580, 1028,  580}, //19
    {1920, 1080, 1920, 1080,  992,  560,  992,  560}, //20
    {1920, 1080, 1920, 1080,  960,  540,  960,  540}, //21
    {1920, 1080, 1920, 1080,  928,  520,  928,  520}, //22
    {1920, 1080, 1920, 1080,  896,  504,  896,  504}, //23
    {1920, 1080, 1920, 1080,  864,  488,  864,  488}, //24
    {1920, 1080, 1920, 1080,  836,  472,  836,  472}, //25
    {1920, 1080, 1920, 1080,  808,  456,  808,  456}, //26
    {1920, 1080, 1920, 1080,  780,  440,  780,  440}, //27
    {1920, 1080, 1920, 1080,  752,  424,  752,  424}, //28
    {1920, 1080, 1920, 1080,  728,  408,  728,  408}, //29
    {1920, 1080, 1920, 1080,  704,  396,  704,  396}, //30
    {1920, 1080, 1920, 1080,  680,  380,  680,  380}, //31
    {1920, 1080, 1920, 1080,  656,  368,  656,  368}, //32
    {1920, 1080, 1920, 1080,  632,  356,  632,  356}, //33
    {1920, 1080, 1920, 1080,  612,  344,  612,  344}, //34
    {1920, 1080, 1920, 1080,  592,  332,  592,  332}, //35
    {1920, 1080, 1920, 1080,  572,  320,  572,  320}, //36
    {1920, 1080, 1920, 1080,  552,  312,  552,  312}, //37
    {1920, 1080, 1920, 1080,  532,  300,  532,  300}, //38
    {1920, 1080, 1920, 1080,  516,  288,  516,  288}, //39
    {1920, 1080, 1920, 1080,  496,  280,  496,  280}, //40
    {1920, 1080, 1920, 1080,  480,  268,  480,  268}, //41
    {1920, 1080, 1920, 1080,  464,  260,  464,  260}, //42
    {1920, 1080, 1920, 1080,  448,  252,  448,  252}, //43
    {1920, 1080, 1920, 1080,  432,  244,  432,  244}, //44
    {1920, 1080, 1920, 1080,  416,  236,  416,  236}, //45
    {1920, 1080, 1920, 1080,  404,  228,  404,  228}, //46
    {1920, 1080, 1920, 1080,  388,  220,  388,  220}, //47
    {1920, 1080, 1920, 1080,  376,  212,  376,  212}, //48
    {1920, 1080, 1920, 1080,  364,  204,  364,  204}, //49
    {1920, 1080, 1920, 1080,  352,  196,  352,  196}, //50
    {1920, 1080, 1920, 1080,  340,  192,  340,  192}, //51
    {1920, 1080, 1920, 1080,  328,  184,  328,  184}, //52
    {1920, 1080, 1920, 1080,  316,  180,  316,  180}, //53
    {1920, 1080, 1920, 1080,  304,  172,  304,  172}, //54
    {1920, 1080, 1920, 1080,  296,  168,  296,  168}, //55
    {1920, 1080, 1920, 1080,  284,  160,  284,  160}, //56
    {1920, 1080, 1920, 1080,  276,  156,  276,  156}, //57
    {1920, 1080, 1920, 1080,  268,  148,  268,  148}, //58
    {1920, 1080, 1920, 1080,  256,  144,  256,  144}, //59
    {1920, 1080, 1920, 1080,  248,  140,  248,  140}, //60
    {1920, 1080, 1920, 1080,  240,  136,  240,  136}, //61
};
//for AR0238CS mode 2 1920 x 1080 p60
const UINT32 VDOZOOM_INFOR_MODE_2_TABLE[61][DZOOM_ITEM_MAX] =
{
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //1
    {1920, 1080, 1920, 1080, 1856, 1044, 1856, 1044}, //2
    {1920, 1080, 1920, 1080, 1792, 1008, 1792, 1008}, //3
    {1920, 1080, 1920, 1080, 1732,  972, 1732,  972}, //4
    {1920, 1080, 1920, 1080, 1672,  940, 1672,  940}, //5
    {1920, 1080, 1920, 1080, 1616,  908, 1616,  908}, //6
    {1920, 1080, 1920, 1080, 1560,  876, 1560,  876}, //7
    {1920, 1080, 1920, 1080, 1508,  848, 1508,  848}, //8
    {1920, 1080, 1920, 1080, 1456,  820, 1456,  820}, //9
    {1920, 1080, 1920, 1080, 1404,  792, 1404,  792}, //10
    {1920, 1080, 1920, 1080, 1356,  764, 1356,  764}, //11
    {1920, 1080, 1920, 1080, 1312,  736, 1312,  736}, //12
    {1920, 1080, 1920, 1080, 1268,  712, 1268,  712}, //13
    {1920, 1080, 1920, 1080, 1224,  688, 1224,  688}, //14
    {1920, 1080, 1920, 1080, 1180,  664, 1180,  664}, //15
    {1920, 1080, 1920, 1080, 1140,  644, 1140,  644}, //16
    {1920, 1080, 1920, 1080, 1104,  620, 1104,  620}, //17
    {1920, 1080, 1920, 1080, 1064,  600, 1064,  600}, //18
    {1920, 1080, 1920, 1080, 1028,  580, 1028,  580}, //19
    {1920, 1080, 1920, 1080,  992,  560,  992,  560}, //20
    {1920, 1080, 1920, 1080,  960,  540,  960,  540}, //21
    {1920, 1080, 1920, 1080,  928,  520,  928,  520}, //22
    {1920, 1080, 1920, 1080,  896,  504,  896,  504}, //23
    {1920, 1080, 1920, 1080,  864,  488,  864,  488}, //24
    {1920, 1080, 1920, 1080,  836,  472,  836,  472}, //25
    {1920, 1080, 1920, 1080,  808,  456,  808,  456}, //26
    {1920, 1080, 1920, 1080,  780,  440,  780,  440}, //27
    {1920, 1080, 1920, 1080,  752,  424,  752,  424}, //28
    {1920, 1080, 1920, 1080,  728,  408,  728,  408}, //29
    {1920, 1080, 1920, 1080,  704,  396,  704,  396}, //30
    {1920, 1080, 1920, 1080,  680,  380,  680,  380}, //31
    {1920, 1080, 1920, 1080,  656,  368,  656,  368}, //32
    {1920, 1080, 1920, 1080,  632,  356,  632,  356}, //33
    {1920, 1080, 1920, 1080,  612,  344,  612,  344}, //34
    {1920, 1080, 1920, 1080,  592,  332,  592,  332}, //35
    {1920, 1080, 1920, 1080,  572,  320,  572,  320}, //36
    {1920, 1080, 1920, 1080,  552,  312,  552,  312}, //37
    {1920, 1080, 1920, 1080,  532,  300,  532,  300}, //38
    {1920, 1080, 1920, 1080,  516,  288,  516,  288}, //39
    {1920, 1080, 1920, 1080,  496,  280,  496,  280}, //40
    {1920, 1080, 1920, 1080,  480,  268,  480,  268}, //41
    {1920, 1080, 1920, 1080,  464,  260,  464,  260}, //42
    {1920, 1080, 1920, 1080,  448,  252,  448,  252}, //43
    {1920, 1080, 1920, 1080,  432,  244,  432,  244}, //44
    {1920, 1080, 1920, 1080,  416,  236,  416,  236}, //45
    {1920, 1080, 1920, 1080,  404,  228,  404,  228}, //46
    {1920, 1080, 1920, 1080,  388,  220,  388,  220}, //47
    {1920, 1080, 1920, 1080,  376,  212,  376,  212}, //48
    {1920, 1080, 1920, 1080,  364,  204,  364,  204}, //49
    {1920, 1080, 1920, 1080,  352,  196,  352,  196}, //50
    {1920, 1080, 1920, 1080,  340,  192,  340,  192}, //51
    {1920, 1080, 1920, 1080,  328,  184,  328,  184}, //52
    {1920, 1080, 1920, 1080,  316,  180,  316,  180}, //53
    {1920, 1080, 1920, 1080,  304,  172,  304,  172}, //54
    {1920, 1080, 1920, 1080,  296,  168,  296,  168}, //55
    {1920, 1080, 1920, 1080,  284,  160,  284,  160}, //56
    {1920, 1080, 1920, 1080,  276,  156,  276,  156}, //57
    {1920, 1080, 1920, 1080,  268,  148,  268,  148}, //58
    {1920, 1080, 1920, 1080,  256,  144,  256,  144}, //59
    {1920, 1080, 1920, 1080,  248,  140,  248,  140}, //60
    {1920, 1080, 1920, 1080,  240,  136,  240,  136}, //61
};

//for AR0238CS mode 3 1920 x 1080 p30 HDR
const UINT32 VDOZOOM_INFOR_MODE_3_TABLE[61][DZOOM_ITEM_MAX] =
{
    {1920, 1080, 1920, 1080, 1920, 1080, 1920, 1080}, //1
    {1920, 1080, 1920, 1080, 1856, 1044, 1856, 1044}, //2
    {1920, 1080, 1920, 1080, 1792, 1008, 1792, 1008}, //3
    {1920, 1080, 1920, 1080, 1732,  972, 1732,  972}, //4
    {1920, 1080, 1920, 1080, 1672,  940, 1672,  940}, //5
    {1920, 1080, 1920, 1080, 1616,  908, 1616,  908}, //6
    {1920, 1080, 1920, 1080, 1560,  876, 1560,  876}, //7
    {1920, 1080, 1920, 1080, 1508,  848, 1508,  848}, //8
    {1920, 1080, 1920, 1080, 1456,  820, 1456,  820}, //9
    {1920, 1080, 1920, 1080, 1404,  792, 1404,  792}, //10
    {1920, 1080, 1920, 1080, 1356,  764, 1356,  764}, //11
    {1920, 1080, 1920, 1080, 1312,  736, 1312,  736}, //12
    {1920, 1080, 1920, 1080, 1268,  712, 1268,  712}, //13
    {1920, 1080, 1920, 1080, 1224,  688, 1224,  688}, //14
    {1920, 1080, 1920, 1080, 1180,  664, 1180,  664}, //15
    {1920, 1080, 1920, 1080, 1140,  644, 1140,  644}, //16
    {1920, 1080, 1920, 1080, 1104,  620, 1104,  620}, //17
    {1920, 1080, 1920, 1080, 1064,  600, 1064,  600}, //18
    {1920, 1080, 1920, 1080, 1028,  580, 1028,  580}, //19
    {1920, 1080, 1920, 1080,  992,  560,  992,  560}, //20
    {1920, 1080, 1920, 1080,  960,  540,  960,  540}, //21
    {1920, 1080, 1920, 1080,  928,  520,  928,  520}, //22
    {1920, 1080, 1920, 1080,  896,  504,  896,  504}, //23
    {1920, 1080, 1920, 1080,  864,  488,  864,  488}, //24
    {1920, 1080, 1920, 1080,  836,  472,  836,  472}, //25
    {1920, 1080, 1920, 1080,  808,  456,  808,  456}, //26
    {1920, 1080, 1920, 1080,  780,  440,  780,  440}, //27
    {1920, 1080, 1920, 1080,  752,  424,  752,  424}, //28
    {1920, 1080, 1920, 1080,  728,  408,  728,  408}, //29
    {1920, 1080, 1920, 1080,  704,  396,  704,  396}, //30
    {1920, 1080, 1920, 1080,  680,  380,  680,  380}, //31
    {1920, 1080, 1920, 1080,  656,  368,  656,  368}, //32
    {1920, 1080, 1920, 1080,  632,  356,  632,  356}, //33
    {1920, 1080, 1920, 1080,  612,  344,  612,  344}, //34
    {1920, 1080, 1920, 1080,  592,  332,  592,  332}, //35
    {1920, 1080, 1920, 1080,  572,  320,  572,  320}, //36
    {1920, 1080, 1920, 1080,  552,  312,  552,  312}, //37
    {1920, 1080, 1920, 1080,  532,  300,  532,  300}, //38
    {1920, 1080, 1920, 1080,  516,  288,  516,  288}, //39
    {1920, 1080, 1920, 1080,  496,  280,  496,  280}, //40
    {1920, 1080, 1920, 1080,  480,  268,  480,  268}, //41
    {1920, 1080, 1920, 1080,  464,  260,  464,  260}, //42
    {1920, 1080, 1920, 1080,  448,  252,  448,  252}, //43
    {1920, 1080, 1920, 1080,  432,  244,  432,  244}, //44
    {1920, 1080, 1920, 1080,  416,  236,  416,  236}, //45
    {1920, 1080, 1920, 1080,  404,  228,  404,  228}, //46
    {1920, 1080, 1920, 1080,  388,  220,  388,  220}, //47
    {1920, 1080, 1920, 1080,  376,  212,  376,  212}, //48
    {1920, 1080, 1920, 1080,  364,  204,  364,  204}, //49
    {1920, 1080, 1920, 1080,  352,  196,  352,  196}, //50
    {1920, 1080, 1920, 1080,  340,  192,  340,  192}, //51
    {1920, 1080, 1920, 1080,  328,  184,  328,  184}, //52
    {1920, 1080, 1920, 1080,  316,  180,  316,  180}, //53
    {1920, 1080, 1920, 1080,  304,  172,  304,  172}, //54
    {1920, 1080, 1920, 1080,  296,  168,  296,  168}, //55
    {1920, 1080, 1920, 1080,  284,  160,  284,  160}, //56
    {1920, 1080, 1920, 1080,  276,  156,  276,  156}, //57
    {1920, 1080, 1920, 1080,  268,  148,  268,  148}, //58
    {1920, 1080, 1920, 1080,  256,  144,  256,  144}, //59
    {1920, 1080, 1920, 1080,  248,  140,  248,  140}, //60
    {1920, 1080, 1920, 1080,  240,  136,  240,  136}, //61
};

UINT32* SenMode2Tbl(UINT32 Id, UINT32 SenMode, UINT32 *DzMaxidx)
{
    UINT32 *Ptr;
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
            Ptr = (UINT32*)&VDOZOOM_INFOR_MODE_3_TABLE[0][0];
            *DzMaxidx = (sizeof(VDOZOOM_INFOR_MODE_3_TABLE) / 4 / DZOOM_ITEM_MAX) - 1;
            break;

        default:
            Ptr = NULL;
            *DzMaxidx = 0;
            break;
    }
    return Ptr;
}
