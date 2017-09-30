/**
    Auto white balance parameter.

    awb parameter.

    @file       awb_sample_param.c
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#include "Type.h"
#include "awb_alg.h"
#include "awb_api.h"
#include "awb_AR0230CS_OV9750_C11_FF_int.h"


/*****************************************************************************/
/*                            old lens                                       */
/*****************************************************************************/

#define AWB_CT1_8500K_RGAIN 402
#define AWB_CT1_8500K_BGAIN 293

#define AWB_CT1_7000K_RGAIN 411
#define AWB_CT1_7000K_BGAIN 324

#define AWB_CT1_6000K_RGAIN 420
#define AWB_CT1_6000K_BGAIN 364

#define AWB_CT1_5000K_RGAIN 364
#define AWB_CT1_5000K_BGAIN 417

#define AWB_CT1_4000K_RGAIN 295
#define AWB_CT1_4000K_BGAIN 495

#define AWB_CT1_2500K_RGAIN 228
#define AWB_CT1_2500K_BGAIN 704

#define AWB_CT1_2000K_RGAIN 213
#define AWB_CT1_2000K_BGAIN 803

#define AWB_CT2_8500K_RGAIN 402
#define AWB_CT2_8500K_BGAIN 293

#define AWB_CT2_7000K_RGAIN 411
#define AWB_CT2_7000K_BGAIN 324

#define AWB_CT2_6000K_RGAIN 420
#define AWB_CT2_6000K_BGAIN 364

#define AWB_CT2_5000K_RGAIN 364
#define AWB_CT2_5000K_BGAIN 417

#define AWB_CT2_4000K_RGAIN 295
#define AWB_CT2_4000K_BGAIN 495

#define AWB_CT2_2500K_RGAIN 228
#define AWB_CT2_2500K_BGAIN 704

#define AWB_CT2_2000K_RGAIN 213
#define AWB_CT2_2000K_BGAIN 803

// The color temperature must be steeled sort
AWB_CTINFO AwbCTTable[AWB_PROC_CNT][AWB_CT_RANGE_MAX] =
{
    {
        {2000 , {AWB_CT1_2000K_RGAIN, 256, AWB_CT1_2000K_BGAIN}},
        {2500 , {AWB_CT1_2500K_RGAIN, 256, AWB_CT1_2500K_BGAIN}},
        {4000 , {AWB_CT1_4000K_RGAIN, 256, AWB_CT1_4000K_BGAIN}},
        {5000 , {AWB_CT1_5000K_RGAIN, 256, AWB_CT1_5000K_BGAIN}},
        {6000 , {AWB_CT1_6000K_RGAIN, 256, AWB_CT1_6000K_BGAIN}},
        {7000 , {AWB_CT1_7000K_RGAIN, 256, AWB_CT1_7000K_BGAIN}},
        {8500 , {AWB_CT1_8500K_RGAIN, 256, AWB_CT1_8500K_BGAIN}},
    },
    {
        {2000 , {AWB_CT2_2000K_RGAIN, 256, AWB_CT2_2000K_BGAIN}},
        {2500 , {AWB_CT2_2500K_RGAIN, 256, AWB_CT2_2500K_BGAIN}},
        {4000 , {AWB_CT2_4000K_RGAIN, 256, AWB_CT2_4000K_BGAIN}},
        {5000 , {AWB_CT2_5000K_RGAIN, 256, AWB_CT2_5000K_BGAIN}},
        {6000 , {AWB_CT2_6000K_RGAIN, 256, AWB_CT2_6000K_BGAIN}},
        {7000 , {AWB_CT2_7000K_RGAIN, 256, AWB_CT2_7000K_BGAIN}},
        {8500 , {AWB_CT2_8500K_RGAIN, 256, AWB_CT2_8500K_BGAIN}},
    },
};

AWB_CAINFO AwbPreWhiteElement[AWB_PROC_CNT] =
{
    {
        32, 32,
        4095,       //UINT32 RGu; (R/G)<<10
        0,        //UINT32 RGl;
        (240<<4),   //UINT32 GGu; G (12Bits)
        (2<<4),     //UINT32 GGl;
        4095,        //UINT32 BGu; (B/G)<<10
        0,        //UINT32 BGl;
        4095,       //UINT32 RBGu;((R+B)/G)<<9
        0           //UINT32 RBGl;
    },
    {
        32, 32,
        4095,       //UINT32 RGu; (R/G)<<10
        0,        //UINT32 RGl;
        (200<<4),   //UINT32 GGu; G (12Bits)
        (8<<4),     //UINT32 GGl;
        4095,        //UINT32 BGu; (B/G)<<10
        0,        //UINT32 BGl;
        4095,       //UINT32 RBGu;((R+B)/G)<<9
        0           //UINT32 RBGl;
    },
};

//#NT#2013/09/05#Spark Chou -begin
//#NT#
#if AWB_REFWHITE_REMAP
AWBALG_ELEMET WhiteIndoorElement1[] =
{
                       //25,        50, 100
    {AWBALG_TYPE_Y,      (160<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     255,  86, {  5, 10}, {  5, 10}},
    {AWBALG_TYPE_BG,     180, 140, {  5, 10}, {  5, 10}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

AWBALG_ELEMET WhiteOutdoorElement1[] =
{
    {AWBALG_TYPE_Y,      (160<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     141,  86, {  5, 10}, {  5, 10}},
    {AWBALG_TYPE_BG,     180, 140, {  5, 10}, {  5, 10}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

#else
//0.97, 0.03, 0.07
AWBALG_ELEMET WhiteNightElement1[] =
{
#if 0 // Including yellow light
    {AWBALG_TYPE_Y,      (200<<4),   (6<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     184,  60, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     460, 110, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    500,  70, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 530, 251, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   400, 171, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
#endif
#if 0 //Including a little yellow light
    {AWBALG_TYPE_Y,      (200<<4),   (6<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     184,  60, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     400, 110, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    440,  70, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 470, 251, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   370, 171, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
#endif
#if 1 // same as outdoor table
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     180,  60, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     350, 105, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    420,  60, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 420, 251, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   335, 160, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
#endif
};

AWBALG_ELEMET WhiteIndoorElement1[] =
{
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     180,  60, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     350, 105, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    420,  60, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 420, 251, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   335, 160, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

AWBALG_ELEMET WhiteOutdoorElement1[] =
{
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     184, 125, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     173, 110, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    130,  70, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 355, 251, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   320, 171, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

//0.97, 0.03, 0.07
AWBALG_ELEMET WhiteIndoorElement2[] =
{
                       //25,        50, 100
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     216,  80, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     371, 142, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    414,  80, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 469, 302, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   426, 252, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

AWBALG_ELEMET WhiteOutdoorElement2[] =
{
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     216, 140, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     216, 142, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    136,  80, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 433, 302, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   426, 252, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};
#endif
//#NT#2013/09/05#Spark Chou -end

AWBALG_TAB AwbWhiteTAB[AWB_PROC_CNT][AWB_MAX_WHITE_TAB] =
{
    {
        {(10<<3),   (10<<1),  WhiteNightElement1,  AWBALG_ENV_Night},
        {(10<<10),  (10<<5),  WhiteIndoorElement1,  AWBALG_ENV_InDoor},
        {(10<<26),  (10<<14), WhiteOutdoorElement1, AWBALG_ENV_OutDoor}
    },
    {
        {(10<<10),  (10<<4),  WhiteIndoorElement2,  AWBALG_ENV_InDoor},
        {(10<<28),  (10<<14), WhiteOutdoorElement2, AWBALG_ENV_OutDoor}
    }
};

//#NT#2013/09/05#Spark Chou -begin
//#NT#
AWBALG_CT_PARAM AwbCTParam[AWB_PROC_CNT] =
{
    {
        (10<<14),                                         // HCTMaxEV; LV14
        (10<<10),                                         // HCTMinEV; LV8
        {AWB_CT1_6000K_RGAIN, 256, AWB_CT1_6000K_BGAIN},   // HCTMinGain; 5500K
        {AWB_CT1_8500K_RGAIN, 256, AWB_CT1_8500K_BGAIN},   // HCTMaxGain; 8500K
        (10<<14),                                         // LCTMaxEV; LV14
        (10<<10),                                         // LCTMinEV; LV8
        {AWB_CT1_4000K_RGAIN, 256, AWB_CT1_4000K_BGAIN},   // LCTMaxGain; 4000K
        {AWB_CT1_2000K_RGAIN, 256, AWB_CT1_2000K_BGAIN},   // LCTMinGain; 2300K
    },
    {
        (10<<14),                                         // HCTMaxEV; LV14
        (10<<10),                                         // HCTMinEV; LV8
        {AWB_CT2_6000K_RGAIN, 256, AWB_CT2_6000K_BGAIN},   // HCTMinGain; 5500K
        {AWB_CT2_8500K_RGAIN, 256, AWB_CT2_8500K_BGAIN},   // HCTMaxGain; 8500K
        (10<<14),                                         // LCTMaxEV; LV14
        (10<<10),                                         // LCTMinEV; LV8
        {AWB_CT2_4000K_RGAIN, 256, AWB_CT2_4000K_BGAIN},   // LCTMaxGain; 4000K
        {AWB_CT2_2500K_RGAIN, 256, AWB_CT2_2500K_BGAIN},   // LCTMinGain; 2300K
    },

};

AWBALG_POSTPROC_PARAM AwbPostParam[AWB_PROC_CNT] =
{
    {
        100,  100,                                       // HCTRatio; //100~200
        {AWB_CT1_7000K_RGAIN, 256, AWB_CT1_7000K_BGAIN},   // HCTMaxGain;
        {AWB_CT1_8500K_RGAIN, 256, AWB_CT1_8500K_BGAIN},   // HCTMinGain;
        110,  85,                                        // LCTRatio;  //100~200
        {AWB_CT1_4000K_RGAIN, 256, AWB_CT1_4000K_BGAIN},   // LCTMaxGain; 4500K
        {AWB_CT1_2000K_RGAIN, 256, AWB_CT1_2000K_BGAIN}    // LCTMinGain; 2556K
    },
    {
        100,  100,                                       // HCTRatio; //100~200
        {AWB_CT2_7000K_RGAIN, 256, AWB_CT2_7000K_BGAIN},   // HCTMaxGain;
        {AWB_CT2_8500K_RGAIN, 256, AWB_CT2_8500K_BGAIN},   // HCTMinGain;
        120,  90,                                        // LCTRatio;  //100~200
        {AWB_CT2_4000K_RGAIN, 256, AWB_CT2_4000K_BGAIN},   // LCTMaxGain; 4500K
        {AWB_CT2_2500K_RGAIN, 256, AWB_CT2_2500K_BGAIN}    // LCTMinGain; 2556K
    }
};

#if 0
#endif
/*****************************************************************************/
/*                            new lens                                       */
/*****************************************************************************/
#define AWB_CT1_8500K_RGAIN2 402
#define AWB_CT1_8500K_BGAIN2 293

#define AWB_CT1_7000K_RGAIN2 411
#define AWB_CT1_7000K_BGAIN2 324

#define AWB_CT1_6000K_RGAIN2 420
#define AWB_CT1_6000K_BGAIN2 364

#define AWB_CT1_5000K_RGAIN2 364
#define AWB_CT1_5000K_BGAIN2 417

#define AWB_CT1_4000K_RGAIN2 295
#define AWB_CT1_4000K_BGAIN2 495

#define AWB_CT1_2500K_RGAIN2 228
#define AWB_CT1_2500K_BGAIN2 704

#define AWB_CT1_2000K_RGAIN2 213
#define AWB_CT1_2000K_BGAIN2 803

#define AWB_CT2_8500K_RGAIN2 402
#define AWB_CT2_8500K_BGAIN2 293

#define AWB_CT2_7000K_RGAIN2 411
#define AWB_CT2_7000K_BGAIN2 324

#define AWB_CT2_6000K_RGAIN2 420
#define AWB_CT2_6000K_BGAIN2 364

#define AWB_CT2_5000K_RGAIN2 364
#define AWB_CT2_5000K_BGAIN2 417

#define AWB_CT2_4000K_RGAIN2 295
#define AWB_CT2_4000K_BGAIN2 495

#define AWB_CT2_2500K_RGAIN2 228
#define AWB_CT2_2500K_BGAIN2 704

#define AWB_CT2_2000K_RGAIN2 213
#define AWB_CT2_2000K_BGAIN2 803

// The color temperature must be steeled sort
AWB_CTINFO AwbCTTable2[AWB_PROC_CNT][AWB_CT_RANGE_MAX] =
{
    {
        {2000 , {AWB_CT1_2000K_RGAIN2, 256, AWB_CT1_2000K_BGAIN2}},
        {2500 , {AWB_CT1_2500K_RGAIN2, 256, AWB_CT1_2500K_BGAIN2}},
        {4000 , {AWB_CT1_4000K_RGAIN2, 256, AWB_CT1_4000K_BGAIN2}},
        {5000 , {AWB_CT1_5000K_RGAIN2, 256, AWB_CT1_5000K_BGAIN2}},
        {6000 , {AWB_CT1_6000K_RGAIN2, 256, AWB_CT1_6000K_BGAIN2}},
        {7000 , {AWB_CT1_7000K_RGAIN2, 256, AWB_CT1_7000K_BGAIN2}},
        {8500 , {AWB_CT1_8500K_RGAIN2, 256, AWB_CT1_8500K_BGAIN2}},
    },
    {
        {2000 , {AWB_CT2_2000K_RGAIN2, 256, AWB_CT2_2000K_BGAIN2}},
        {2500 , {AWB_CT2_2500K_RGAIN2, 256, AWB_CT2_2500K_BGAIN2}},
        {4000 , {AWB_CT2_4000K_RGAIN2, 256, AWB_CT2_4000K_BGAIN2}},
        {5000 , {AWB_CT2_5000K_RGAIN2, 256, AWB_CT2_5000K_BGAIN2}},
        {6000 , {AWB_CT2_6000K_RGAIN2, 256, AWB_CT2_6000K_BGAIN2}},
        {7000 , {AWB_CT2_7000K_RGAIN2, 256, AWB_CT2_7000K_BGAIN2}},
        {8500 , {AWB_CT2_8500K_RGAIN2, 256, AWB_CT2_8500K_BGAIN2}},
    },
};


AWBALG_ELEMET WhiteNightElement1_2[] =
{
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     180,  60, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     350, 105, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    420,  60, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 420, 251, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   335, 160, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

AWBALG_ELEMET WhiteIndoorElement1_2[] =
{
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     180,  60, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     350, 105, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    420,  60, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 420, 251, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   335, 160, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

AWBALG_ELEMET WhiteOutdoorElement1_2[] =
{
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     184, 125, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     173, 110, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    130,  70, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 355, 251, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   320, 171, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

//0.97, 0.03, 0.07
AWBALG_ELEMET WhiteIndoorElement2_2[] =
{
                       //25,        50, 100
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     216,  80, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     371, 142, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    414,  80, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 469, 302, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   426, 252, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

AWBALG_ELEMET WhiteOutdoorElement2_2[] =
{
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     216, 140, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     216, 142, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    136,  80, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 433, 302, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   426, 252, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};


AWBALG_TAB AwbWhiteTAB2[AWB_PROC_CNT][AWB_MAX_WHITE_TAB] =
{
    {
        {(10<<3),   (10<<1),  WhiteNightElement1_2,  AWBALG_ENV_Night},
        {(10<<10),  (10<<5),  WhiteIndoorElement1_2,  AWBALG_ENV_InDoor},
        {(10<<26),  (10<<14), WhiteOutdoorElement1_2, AWBALG_ENV_OutDoor}
    },
    {
        {(10<<10),  (10<<4),  WhiteIndoorElement2_2,  AWBALG_ENV_InDoor},
        {(10<<28),  (10<<14), WhiteOutdoorElement2_2, AWBALG_ENV_OutDoor}
    }
};

AWBALG_CT_PARAM AwbCTParam2[AWB_PROC_CNT] =
{
    {
        (10<<14),                                         // HCTMaxEV; LV14
        (10<<10),                                         // HCTMinEV; LV8
        {AWB_CT1_6000K_RGAIN2, 256, AWB_CT1_6000K_BGAIN2},   // HCTMinGain; 5500K
        {AWB_CT1_8500K_RGAIN2, 256, AWB_CT1_8500K_BGAIN2},   // HCTMaxGain; 8500K
        (10<<14),                                         // LCTMaxEV; LV14
        (10<<10),                                         // LCTMinEV; LV8
        {AWB_CT1_4000K_RGAIN2, 256, AWB_CT1_4000K_BGAIN2},   // LCTMaxGain; 4000K
        {AWB_CT1_2000K_RGAIN2, 256, AWB_CT1_2000K_BGAIN2},   // LCTMinGain; 2300K
    },
    {
        (10<<14),                                         // HCTMaxEV; LV14
        (10<<10),                                         // HCTMinEV; LV8
        {AWB_CT2_6000K_RGAIN2, 256, AWB_CT2_6000K_BGAIN2},   // HCTMinGain; 5500K
        {AWB_CT2_8500K_RGAIN2, 256, AWB_CT2_8500K_BGAIN2},   // HCTMaxGain; 8500K
        (10<<14),                                         // LCTMaxEV; LV14
        (10<<10),                                         // LCTMinEV; LV8
        {AWB_CT2_4000K_RGAIN2, 256, AWB_CT2_4000K_BGAIN2},   // LCTMaxGain; 4000K
        {AWB_CT2_2500K_RGAIN2, 256, AWB_CT2_2500K_BGAIN2},   // LCTMinGain; 2300K
    },

};

AWBALG_POSTPROC_PARAM AwbPostParam2[AWB_PROC_CNT] =
{
    {
        100,  100,                                       // HCTRatio; //100~200
        {AWB_CT1_7000K_RGAIN2, 256, AWB_CT1_7000K_BGAIN2},   // HCTMaxGain;
        {AWB_CT1_8500K_RGAIN2, 256, AWB_CT1_8500K_BGAIN2},   // HCTMinGain;
        110,  85,                                        // LCTRatio;  //100~200
        {AWB_CT1_4000K_RGAIN2, 256, AWB_CT1_4000K_BGAIN2},   // LCTMaxGain; 4500K
        {AWB_CT1_2000K_RGAIN2, 256, AWB_CT1_2000K_BGAIN2}    // LCTMinGain; 2556K
    },
    {
        100,  100,                                       // HCTRatio; //100~200
        {AWB_CT2_7000K_RGAIN2, 256, AWB_CT2_7000K_BGAIN2},   // HCTMaxGain;
        {AWB_CT2_8500K_RGAIN2, 256, AWB_CT2_8500K_BGAIN2},   // HCTMinGain;
        120,  90,                                        // LCTRatio;  //100~200
        {AWB_CT2_4000K_RGAIN2, 256, AWB_CT2_4000K_BGAIN2},   // LCTMaxGain; 4500K
        {AWB_CT2_2500K_RGAIN2, 256, AWB_CT2_2500K_BGAIN2}    // LCTMinGain; 2556K
    }
};
/*****************************************************************************/
/*                            MWB                                            */
/*****************************************************************************/

UINT32 MwbTAB[AWB_PROC_CNT][AWB_MODE_MAX][3] =
{
    {
    { 256, 256, 256},
    { AWB_CT1_6000K_RGAIN, 256, AWB_CT1_6000K_BGAIN},//AWB_MODE_DAYLIGHT
    { AWB_CT1_7000K_RGAIN, 256, AWB_CT1_7000K_BGAIN},//AWB_MODE_CLOUDY
    { AWB_CT1_4000K_RGAIN, 256, AWB_CT1_4000K_BGAIN},//AWB_MODE_TUNGSTEN
    { AWB_CT1_5000K_RGAIN, 256, AWB_CT1_5000K_BGAIN},//AWB_MODE_FLUORESCENT1
    { 256, 256, 256},
    { 256, 256, 256},
    { 256, 256, 256},
    { 256, 256, 256},
    { 256, 256, 256},
    { 256, 256, 256},
    { 256, 256, 256}
    },
    {
    { 256, 256, 256},
    { AWB_CT2_6000K_RGAIN, 256, AWB_CT2_6000K_BGAIN},//AWB_MODE_DAYLIGHT
    { AWB_CT2_7000K_RGAIN, 256, AWB_CT2_7000K_BGAIN},//AWB_MODE_CLOUDY
    { AWB_CT2_4000K_RGAIN, 256, AWB_CT2_4000K_BGAIN},//AWB_MODE_TUNGSTEN
    { AWB_CT2_5000K_RGAIN, 256, AWB_CT2_5000K_BGAIN},//AWB_MODE_FLUORESCENT1
    { 256, 256, 256},
    { 256, 256, 256},
    { 256, 256, 256},
    { 256, 256, 256},
    { 256, 256, 256},
    { 256, 256, 256},
    { 256, 256, 256}
    }
};


