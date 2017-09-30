#ifndef _IQS_IE_SETTINGTABLE_IMX291M_TL_IPC321_FF_C
#define _IQS_IE_SETTINGTABLE_IMX291M_TL_IPC321_FF_C
#include "IPL_IMX291M_TL_IPC321_FF_Int.h"

                                             //CbOfs CrOfs YCon CCon IntOfs SatOfs HueRotate
IQS_COLOR_EFFECT_PARAM ColorEffectTable_Normal  = {128, 128,  128, 128,   0,     0,   FALSE };    //SEL_IMAGEEFFECT_OFF = 0,
IQS_COLOR_EFFECT_PARAM ColorEffectTable_BW      = {128, 128,  128,   0,   0,     0,   FALSE };    //SEL_IMAGEEFFECT_BW,
IQS_COLOR_EFFECT_PARAM ColorEffectTable_SEPIA   = {116, 144,  128,   0,   0,     0,   FALSE };    //SEL_IMAGEEFFECT_SEPIA,
IQS_COLOR_EFFECT_PARAM ColorEffectTable_VIVID   = {128, 128,  128, 128,   0,    50,   FALSE };    //SEL_IMAGEEFFECT_VIVID,

//////////////////////////////////
// Color pencil
//////////////////////////////////
IPE_YCCFIX IE_ColorPencil_Param =
{
  //yth1{YTH,  ETH,  HIT1SEL,NHIT1SEL,HIT,NHIT},  yth2{YTH, HIT2SEL,NHIT2SEL,HIT,NHIT}, CC{ETH, YTG, CBTH,  CRTH, HITSEL,NHITSEL,CB_HIT,CB_NHIT,CR_HIT,CR_NHIT}
        {5,     0,   ENABLE,  ENABLE,  0, 192},      {0,    FALSE,  FALSE,    0,   0},   {0,   0,0,  0,0,  0,0,  FALSE,  FALSE,     0,      0,     0,     0},
};

//////////////////////////////////
// Sketch
//////////////////////////////////
IPE_YCCFIX IE_PencilSketch_Param =
{
  //yth1{YTH,  ETH,  HIT1SEL,NHIT1SEL,HIT,NHIT},  yth2{YTH, HIT2SEL,NHIT2SEL,HIT,NHIT}, CC{ETH, YTG, CBTH,  CRTH, HITSEL,NHITSEL,CB_HIT,CB_NHIT,CR_HIT,CR_NHIT}
        {10,    0,   ENABLE,  ENABLE,  0, 255},      {0,    FALSE,  FALSE,    0,   0},   {0,   0,0,  0,0,  0,0,  FALSE,  FALSE,     0,      0,     0,     0},
};

///////////////////////
// IPP Rock
///////////////////////
IPE_YCCFIX IPE_RockEffect_YccFix =
{
  //yth1{YTH,  ETH,  HIT1SEL,NHIT1SEL,HIT,NHIT},  yth2{YTH, HIT2SEL,NHIT2SEL,HIT,NHIT}, CC{ETH, YTG, CBTH,  CRTH, HITSEL,NHITSEL,CB_HIT,CB_NHIT,CR_HIT,CR_NHIT}
        {0,      0,    FALSE,  FALSE,    0,   0},    {128,   TRUE,  TRUE,    112,   0},   {0,  128,255,0,255,0,255, TRUE,  TRUE,    165,    128,   235,    128}
};

//////////////////////////////////
// Fisheye
//////////////////////////////////
UINT32 IE_FISYEYE_LUT[65] =
{
    55705, 55933, 56161, 56390, 56617, 56842, 57065, 57286, 57503, 57717,
    57927, 58133, 58335, 58533, 58726, 58915, 59100, 59281, 59458, 59631,
    59801, 59967, 60130, 60291, 60449, 60605, 60760, 60913, 61064, 61215,
    61365, 61515, 61666, 61816, 61967, 62118, 62270, 62423, 62577, 62732,
    62888, 63045, 63203, 63361, 63520, 63678, 63836, 63993, 64148, 64301,
    64450, 64596, 64737, 64872, 65000, 65119, 65228, 65326, 65411, 65481,
    65534, 65535, 65535, 65535, 65535
};

///////////////////////
// Selective CC
///////////////////////
INT32 IPE_SelectiveCC_SatTab[15][24] =
{
//  0                        5                       10                       15                       20
{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},          //CCID_NO_EFFECT,
{  99,  99,  99,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  99},          //CCID_ENHANCE_RED,
{   0,   0,   0,  99,  99,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},          //CCID_ENHANCE_YELLOW,
{   0,   0,   0,   0,   0,   0,   0,  99,  99,  99,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},          //CCID_ENHANCE_GREEN,
{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  99,  99,  99,   0,   0,   0,   0,   0,   0,   0},          //CCID_ENHANCE_BLUE,
{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  99,  99,   0,   0,   0,   0,   0,   0,   0,   0,   0},          //CCID_ENHANCE_CYAN,
{   0,   0,  99,  99,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},          //CCID_ENHANCE_ORANGE,
{   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  99,  99,  99,   0},          //CCID_ENHANCE_MAGENTA,
{   0,   0,   0,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,   0},          //CCID_REMOVAL_RED,
{-128,-128,-128,   0,   0,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128},          //CCID_REMOVAL_YELLOW,
{-128,-128,-128,-128,-128,-128,-128,   0,   0,   0,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128},          //CCID_REMOVAL_GREEN,
{-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,   0,   0,   0,-128,-128,-128,-128,-128,-128,-128},          //CCID_REMOVAL_BLUE,
{-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,   0,   0,-128,-128,-128,-128,-128,-128,-128,-128,-128},          //CCID_REMOVAL_CYAN,
{-128,-128,   0,   0,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128},          //CCID_REMOVAL_ORANGE,
{-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,-128,   0,   0,   0,-128},          //CCID_REMOVAL_MAGENTA

};

///////////////////////
// User Define CC
///////////////////////
UINT16 IQS_UserDefine_CC[3][9] =  //ipcam low temp.
{
    {
    0x0100,  0x0000,  0x0000,
    0x0000,  0x0100,  0x0000,
    0x0000,  0x0000,  0x0100
    },

    { //ipcam mid temp.
    0x021e,  0x06a1,  0x0041,
    0x0771,  0x01e7,  0x07a9,
    0x07f5,  0x06f4,  0x0217
    },

    {//ipcam high temp.
    0x0000,  0x0100,  0x0000,
    0x0000,  0x0100,  0x0000,
    0x0000,  0x0100,  0x0000
    }
};


UINT32 IQS_Gamma16Lut[130] =
{
0,7,11,14,16,27,47,67,86,104,123,141,158,
175,191,207,222,238,252,267,281,294,306,318,330,341,
353,364,374,385,394,405,414,424,434,443,453,463,472,
482,492,500,510,519,528,538,546,555,564,573,581,590,
599,608,616,625,634,641,650,659,666,675,683,690,698,
705,713,720,727,735,742,748,754,762,768,774,780,786,
792,799,805,811,817,822,828,833,838,844,849,855,860,
865,871,876,881,887,892,897,902,908,912,918,923,928,
932,936,941,945,950,953,957,961,966,970,974,978,982,
985,989,992,996,999,1003,1006,1010,1013,1017,1020,1023,1023,

};
UINT32 IQS_Gamma18Lut[130] =
{
    0,10,14,18,45,71,94,117,138,159,179,198,217,
    234,250,267,283,296,310,324,336,349,361,372,383,394,
    405,415,426,436,446,457,466,476,486,495,505,514,523,
    533,541,551,560,568,578,586,595,604,613,620,629,638,
    646,654,663,670,678,685,692,700,707,714,721,728,735,
    742,748,754,761,767,773,779,785,790,797,803,808,814,
    818,824,829,834,839,845,849,854,860,864,869,874,879,
    884,889,893,898,903,908,912,917,922,926,931,935,939,
    943,946,950,954,957,961,965,969,972,976,980,983,986,
    990,992,996,999,1002,1005,1008,1011,1014,1018,1020,1023,1023
};
UINT32 IQS_Gamma20Lut[130] =
{
    0,13,18,55,86,114,142,166,190,211,232,252,270,
    288,303,318,333,346,360,372,385,396,407,419,429,440,
    451,461,472,482,492,502,512,522,530,540,550,558,568,
    577,585,594,603,611,620,629,638,645,654,661,670,677,
    684,692,699,706,713,720,727,734,741,747,752,758,765,
    770,776,781,787,792,799,804,810,815,819,824,830,834,
    839,844,848,853,858,863,867,872,877,881,886,890,894,
    899,904,908,912,917,921,925,930,933,937,940,944,947,
    951,954,957,961,964,968,971,975,978,981,984,987,990,
    993,996,998,1001,1004,1007,1009,1012,1015,1018,1020,1023,1023
};
UINT32 IQS_Gamma22Lut[130] =
{
    0,16,53,92,128,159,189,214,238,260,281,299,316,
    333,348,362,376,389,402,414,426,437,449,460,471,482,
    493,504,513,523,533,542,552,562,571,580,589,598,606,
    615,624,633,641,649,658,665,673,680,687,694,702,709,
    716,723,729,736,743,749,754,761,766,772,777,783,788,
    794,799,805,810,815,819,824,829,833,838,843,848,851,
    856,861,865,869,874,878,882,887,891,895,899,904,908,
    911,916,920,923,928,932,935,938,942,945,948,951,954,
    957,960,964,967,970,973,976,979,982,985,988,990,993,
    995,998,1000,1003,1006,1008,1011,1013,1015,1018,1021,1023,1023
};
UINT32 IQS_Gamma24Lut[130] =
{
0,28,86,131,169,203,232,259,283,304,323,340,358,
373,387,401,414,427,440,452,464,476,487,499,508,519,
530,540,550,560,569,579,588,598,606,615,624,633,641,
650,658,666,673,682,689,695,702,709,716,723,730,736,
743,749,754,761,766,772,777,782,787,794,799,804,809,
814,818,822,827,832,836,841,845,849,854,858,863,866,
871,875,878,883,887,891,895,899,903,907,910,915,919,
923,926,930,933,936,939,943,946,949,951,954,957,960,
964,966,969,972,975,978,981,983,985,988,990,993,995,
998,1000,1003,1005,1007,1009,1012,1014,1016,1019,1021,1023,1023

};
UINT32 IQS_GammaUserDefine1Lut[130] =
{
 0,12,48,92,132,168,200,228,254,278,300,318,335,
351,366,380,393,406,418,430,442,454,466,477,488,499,
510,520,530,540,550,559,568,577,586,595,604,612,620,
628,636,643,650,657,664,671,677,683,689,695,701,707,
713,719,725,730,735,740,745,750,755,760,765,770,774,
778,782,786,790,794,798,802,806,810,813,816,819,822,
825,828,831,834,837,840,843,846,849,852,856,860,864,
868,872,876,880,884,888,892,896,900,904,908,912,916,
920,924,928,932,936,940,944,948,952,956,960,964,968,
972,976,980,984,988,992,996,1001,1006,1011,1017,1023,1023
};
UINT32 IQS_UserGAMMA[130] =
{
 0,12,48,92,132,168,200,228,254,278,300,318,335,
351,366,380,393,406,418,430,442,454,466,477,488,499,
510,520,530,540,550,559,568,577,586,595,604,612,620,
628,636,643,650,657,664,671,677,683,689,695,701,707,
713,719,725,730,735,740,745,750,755,760,765,770,774,
778,782,786,790,794,798,802,806,810,813,816,819,822,
825,828,831,834,837,840,843,846,849,852,856,860,864,
868,872,876,880,884,888,892,896,900,904,908,912,916,
920,924,928,932,936,940,944,948,952,956,960,964,968,
972,976,980,984,988,992,996,1001,1006,1011,1017,1023,1023
};
UINT32 IQS_UserGAMMALinear[130] =
{
0,8,16,24,32,40,48,56,64,72,80,88,96,
104,112,120,128,136,144,152,160,168,176,184,192,200,
208,216,224,232,240,248,256,264,272,280,288,296,304,
312,320,328,336,344,352,360,368,376,384,392,400,408,
416,424,432,440,448,456,464,472,480,488,496,504,512,
520,528,536,544,552,560,568,576,584,592,600,608,616,
624,632,640,648,656,664,672,680,688,696,704,712,720,
728,736,744,752,760,768,776,784,792,800,808,816,824,
832,840,848,856,864,872,880,888,896,904,912,920,928,
936,944,952,960,968,976,984,992,1000,1008,1016,1023,1023,

};

UINT32 IQS_antiFog_Y[130] =
{
 0,12,48,92,132,168,200,228,254,278,300,318,335,
351,366,380,393,406,418,430,442,454,466,477,488,499,
510,520,530,540,550,559,568,577,586,595,604,612,620,
628,636,643,650,657,664,671,677,683,689,695,701,707,
713,719,725,730,735,740,745,750,755,760,765,770,774,
778,782,786,790,794,798,802,806,810,813,816,819,822,
825,828,831,834,837,840,843,846,849,852,856,860,864,
868,872,876,880,884,888,892,896,900,904,908,912,916,
920,924,928,932,936,940,944,948,952,956,960,964,968,
972,976,980,984,988,992,996,1001,1006,1011,1017,1023,1023
};
#endif
