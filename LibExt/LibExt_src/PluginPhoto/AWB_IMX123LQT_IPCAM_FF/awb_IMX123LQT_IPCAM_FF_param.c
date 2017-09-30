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
#include "awb_IMX123LQT_IPCAM_FF_int.h"

#define AWB_CT1_8500K_RGAIN 513
#define AWB_CT1_8500K_BGAIN 202

#define AWB_CT1_7000K_RGAIN 537
#define AWB_CT1_7000K_BGAIN 380

#define AWB_CT1_6500K_RGAIN 545
#define AWB_CT1_6500K_BGAIN 439

#define AWB_CT1_6000K_RGAIN 545
#define AWB_CT1_6000K_BGAIN 439

#define AWB_CT1_5000K_RGAIN 481
#define AWB_CT1_5000K_BGAIN 516

#define AWB_CT1_4000K_RGAIN 394
#define AWB_CT1_4000K_BGAIN 686

#define AWB_CT1_2500K_RGAIN 265
#define AWB_CT1_2500K_BGAIN 939

#define AWB_CT1_2000K_RGAIN 222
#define AWB_CT1_2000K_BGAIN 1023

#define AWB_CT2_8500K_RGAIN 520
#define AWB_CT2_8500K_BGAIN 292

#define AWB_CT2_7000K_RGAIN 560
#define AWB_CT2_7000K_BGAIN 368

#define AWB_CT2_6500K_RGAIN 580
#define AWB_CT2_6500K_BGAIN 406

#define AWB_CT2_6000K_RGAIN 600
#define AWB_CT2_6000K_BGAIN 444

#define AWB_CT2_5000K_RGAIN 485
#define AWB_CT2_5000K_BGAIN 460

#define AWB_CT2_4000K_RGAIN 396
#define AWB_CT2_4000K_BGAIN 570

#define AWB_CT2_2500K_RGAIN 308
#define AWB_CT2_2500K_BGAIN 682

#define AWB_CT2_2000K_RGAIN 308
#define AWB_CT2_2000K_BGAIN 682

// The color temperature must be steeled sort
AWB_CTINFO AwbCTTable[AWB_PROC_CNT][AWB_CT_RANGE_MAX] =
{
    {
        {2000 , {AWB_CT1_2000K_RGAIN, 256, AWB_CT1_2000K_BGAIN}},
        {2500 , {AWB_CT1_2500K_RGAIN, 256, AWB_CT1_2500K_BGAIN}},
        {4000 , {AWB_CT1_4000K_RGAIN, 256, AWB_CT1_4000K_BGAIN}},
        {5000 , {AWB_CT1_5000K_RGAIN, 256, AWB_CT1_5000K_BGAIN}},
        {6000 , {AWB_CT1_6000K_RGAIN, 256, AWB_CT1_6000K_BGAIN}},
        {6500 , {AWB_CT1_6500K_RGAIN, 256, AWB_CT1_6500K_BGAIN}},
        {7000 , {AWB_CT1_7000K_RGAIN, 256, AWB_CT1_7000K_BGAIN}},
        {8500 , {AWB_CT1_8500K_RGAIN, 256, AWB_CT1_8500K_BGAIN}},
    },
    {
        {2000 , {AWB_CT2_2000K_RGAIN, 256, AWB_CT2_2000K_BGAIN}},
        {2500 , {AWB_CT2_2500K_RGAIN, 256, AWB_CT2_2500K_BGAIN}},
        {4000 , {AWB_CT2_4000K_RGAIN, 256, AWB_CT2_4000K_BGAIN}},
        {5000 , {AWB_CT2_5000K_RGAIN, 256, AWB_CT2_5000K_BGAIN}},
        {6000 , {AWB_CT2_6000K_RGAIN, 256, AWB_CT2_6000K_BGAIN}},
        {6500 , {AWB_CT2_6500K_RGAIN, 256, AWB_CT2_6500K_BGAIN}},
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
        (200<<4),   //UINT32 GGu; G (12Bits)
        (8<<4),     //UINT32 GGl;
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
AWBALG_ELEMET WhiteIndoorElement1[] =
{
                       //25,        50, 100
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     235,  70, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     440, 112, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    504,  20, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 518, 237, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   430, 150, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};
AWBALG_ELEMET WhiteIndoorElement_evening[] =
{
                       //25,        50, 100
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     235,  70, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     440, 133, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    504,  20, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 518, 321, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   430, 278, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};
//0.97, 0.03, 0.07
AWBALG_ELEMET WhiteIndoorElement_night[] =
{
                       //25,        50, 100
    {AWBALG_TYPE_Y,      (200<<4),   (5<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     235,  70, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     750, 112, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    750,  60, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 820, 237, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   690, 150, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

AWBALG_ELEMET WhiteOutdoorElement1[] =
{
    {AWBALG_TYPE_Y,      (200<<4),   (8<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     235, 119, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     204, 120, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    119,  63, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 436, 241, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   430, 162, {  0,  0}, {  0,  0}},
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
        {(10<<5),  (10<<1),  WhiteIndoorElement_night,  AWBALG_ENV_Night},
        {(10<<10),  (10<<8),  WhiteIndoorElement1,  AWBALG_ENV_InDoor},
        {(10<<26),  (10<<14), WhiteOutdoorElement1, AWBALG_ENV_OutDoor}
    },
    {
        {(10<<5),  (10<<4),  WhiteIndoorElement1,  AWBALG_ENV_InDoor},
        {(10<<10),  (10<<8),  WhiteIndoorElement2,  AWBALG_ENV_InDoor},
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
        100,  100,                                        // LCTRatio;  //100~200
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

//#NT#2013/09/05#Spark Chou -end

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


