#ifndef _IQS_IE_SETTINGTABLE_AR0238CSP_EVB_FF_C
#define _IQS_IE_SETTINGTABLE_AR0238CSP_EVB_FF_C
#include "IPL_AR0238CSP_EVB_FF_Int.h"

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
#endif
