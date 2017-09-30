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
#include "awb_OV4689_OV2710_EVB_FF_int.h"

#define AWB_CT1_8500K_RGAIN 520
#define AWB_CT1_8500K_BGAIN 292

#define AWB_CT1_7500K_RGAIN 562
#define AWB_CT1_7500K_BGAIN 351

#define AWB_CT1_7000K_RGAIN 583
#define AWB_CT1_7000K_BGAIN 381

#define AWB_CT1_6500K_RGAIN 599
#define AWB_CT1_6500K_BGAIN 403

#define AWB_CT1_5500K_RGAIN 631
#define AWB_CT1_5500K_BGAIN 477

#define AWB_CT1_5000K_RGAIN 489
#define AWB_CT1_5000K_BGAIN 472

#define AWB_CT1_4500K_RGAIN 431
#define AWB_CT1_4500K_BGAIN 589

#define AWB_CT1_4000K_RGAIN 372
#define AWB_CT1_4000K_BGAIN 706

#define AWB_CT1_3500K_RGAIN 334
#define AWB_CT1_3500K_BGAIN 784

#define AWB_CT1_2500K_RGAIN 256
#define AWB_CT1_2500K_BGAIN 940


#define AWB_CT2_8500K_RGAIN 510
#define AWB_CT2_8500K_BGAIN 334

#define AWB_CT2_7500K_RGAIN 513
#define AWB_CT2_7500K_BGAIN 383

#define AWB_CT2_7000K_RGAIN 515
#define AWB_CT2_7000K_BGAIN 408

#define AWB_CT2_6500K_RGAIN 517
#define AWB_CT2_6500K_BGAIN 433

#define AWB_CT2_6000K_RGAIN 518
#define AWB_CT2_6000K_BGAIN 457

#define AWB_CT2_5500K_RGAIN 520
#define AWB_CT2_5500K_BGAIN 482

#define AWB_CT2_5000K_RGAIN 430
#define AWB_CT2_5000K_BGAIN 492

#define AWB_CT2_4500K_RGAIN 390
#define AWB_CT2_4500K_BGAIN 540

#define AWB_CT2_4300K_RGAIN 375
#define AWB_CT2_4300K_BGAIN 560

#define AWB_CT2_4000K_RGAIN 351
#define AWB_CT2_4000K_BGAIN 588

#define AWB_CT2_3700K_RGAIN 327 //*
#define AWB_CT2_3700K_BGAIN 616

#define AWB_CT2_3500K_RGAIN 311
#define AWB_CT2_3500K_BGAIN 636

#define AWB_CT2_3000K_RGAIN 272 //*
#define AWB_CT2_3000K_BGAIN 683

#define AWB_CT2_2500K_RGAIN 232
#define AWB_CT2_2500K_BGAIN 731

// The color temperature must be steeled sort
AWB_CTINFO AwbCTTable[AWB_PROC_CNT][AWB_CT_RANGE_MAX] =
{
    {
        {2500 , {AWB_CT1_2500K_RGAIN, 256, AWB_CT1_2500K_BGAIN}},
        {3500 , {AWB_CT1_3500K_RGAIN, 256, AWB_CT1_3500K_BGAIN}},
        {4500 , {AWB_CT1_4500K_RGAIN, 256, AWB_CT1_4500K_BGAIN}},
        {5500 , {AWB_CT1_5500K_RGAIN, 256, AWB_CT1_5500K_BGAIN}},
        {6500 , {AWB_CT1_6500K_RGAIN, 256, AWB_CT1_6500K_BGAIN}},
        {7500 , {AWB_CT1_7500K_RGAIN, 256, AWB_CT1_7500K_BGAIN}},
        {8500 , {AWB_CT1_8500K_RGAIN, 256, AWB_CT1_8500K_BGAIN}},
    },
    {
        {2500 , {AWB_CT2_2500K_RGAIN, 256, AWB_CT2_2500K_BGAIN}},
        {3500 , {AWB_CT2_3500K_RGAIN, 256, AWB_CT2_3500K_BGAIN}},
        {4500 , {AWB_CT2_4500K_RGAIN, 256, AWB_CT2_4500K_BGAIN}},
        {5500 , {AWB_CT2_5500K_RGAIN, 256, AWB_CT2_5500K_BGAIN}},
        {6500 , {AWB_CT2_6500K_RGAIN, 256, AWB_CT2_6500K_BGAIN}},
        {7500 , {AWB_CT2_7500K_RGAIN, 256, AWB_CT2_7500K_BGAIN}},
        {8500 , {AWB_CT2_8500K_RGAIN, 256, AWB_CT2_8500K_BGAIN}},
    },
};

AWB_CAINFO AwbPreWhiteElement[AWB_PROC_CNT] =
{
    {
        32, 32,
        1132,       //UINT32 RGu; (R/G)<<10
        292,        //UINT32 RGl;
        (200<<4),   //UINT32 GGu; G (12Bits)
        (4<<4),     //UINT32 GGl;
        2016,       //UINT32 BGu; (B/G)<<10
        319,        //UINT32 BGl;
        814,        //UINT32 RBGu;((R+B)/G)<<9
        409         //UINT32 RBGl;
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
    {AWBALG_TYPE_Y,      (200<<4),   (4<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     283,  73, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     504, 126, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    582,  55, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 603, 319, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   529, 262, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

AWBALG_ELEMET WhiteOutdoorElement1[] =
{
    {AWBALG_TYPE_Y,      (200<<4),   (4<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     283,  84, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     461, 126, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    423,  55, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 590, 319, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   529, 262, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

//0.97, 0.03, 0.07
AWBALG_ELEMET WhiteIndoorElement2[] =
{
                       //25,        50, 100
    {AWBALG_TYPE_Y,      (200<<4),   (4<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     228,  81, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     334, 139, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    367,  38, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 434, 321, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   427, 245, {  0,  0}, {  0,  0}},
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
        {(10<<10),  (10<<4),  WhiteIndoorElement1,  AWBALG_ENV_InDoor},
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
        {AWB_CT1_5500K_RGAIN, 256, AWB_CT1_5500K_BGAIN},   // HCTMinGain; 5500K
        {AWB_CT1_8500K_RGAIN, 256, AWB_CT1_8500K_BGAIN},   // HCTMaxGain; 8500K
        (10<<14),                                         // LCTMaxEV; LV14
        (10<<10),                                         // LCTMinEV; LV8
        {AWB_CT1_4000K_RGAIN, 256, AWB_CT1_4000K_BGAIN},   // LCTMaxGain; 4000K
        {AWB_CT1_2500K_RGAIN, 256, AWB_CT1_2500K_BGAIN},   // LCTMinGain; 2300K
    },
    {
        (10<<14),                                         // HCTMaxEV; LV14
        (10<<10),                                         // HCTMinEV; LV8
        {AWB_CT2_5500K_RGAIN, 256, AWB_CT2_5500K_BGAIN},   // HCTMinGain; 5500K
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
        120,  90,                                        // LCTRatio;  //100~200
        {AWB_CT1_4000K_RGAIN, 256, AWB_CT1_4000K_BGAIN},   // LCTMaxGain; 4500K
        {AWB_CT1_2500K_RGAIN, 256, AWB_CT1_2500K_BGAIN}    // LCTMinGain; 2556K
    },
    {
        100,  100,                                       // HCTRatio; //100~200
        {AWB_CT2_7000K_RGAIN, 256, AWB_CT2_7000K_BGAIN},   // HCTMaxGain;
        {AWB_CT2_8500K_RGAIN, 256, AWB_CT2_8500K_BGAIN},   // HCTMinGain;
        120,  90,                                        // LCTRatio;  //100~200
        {AWB_CT2_4000K_RGAIN, 256, AWB_CT2_4000K_BGAIN},   // LCTMaxGain; 4500K
        {AWB_CT2_3000K_RGAIN, 256, AWB_CT2_3000K_BGAIN}    // LCTMinGain; 2556K
    }
};

//#NT#2013/09/05#Spark Chou -end

UINT32 MwbTAB[AWB_PROC_CNT][AWB_MODE_MAX][3] =
{
    {
    { 256, 256, 256},
    { AWB_CT1_5500K_RGAIN, 256, AWB_CT1_5500K_BGAIN},//AWB_MODE_DAYLIGHT
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
    { AWB_CT2_5500K_RGAIN, 256, AWB_CT2_5500K_BGAIN},//AWB_MODE_DAYLIGHT
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


