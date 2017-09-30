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
#include "awb_IMX322LQJ_MIO_C032_FF_int.h"

#define AWB_CT1_8500K_RGAIN 512
#define AWB_CT1_8500K_BGAIN 363

#define AWB_CT1_7000K_RGAIN 512
#define AWB_CT1_7000K_BGAIN 420

#define AWB_CT1_5500K_RGAIN 512
#define AWB_CT1_5500K_BGAIN 512

#define AWB_CT1_5000K_RGAIN 435
#define AWB_CT1_5000K_BGAIN 522

#define AWB_CT1_4500K_RGAIN 389
#define AWB_CT1_4500K_BGAIN 576

#define AWB_CT1_4000K_RGAIN 343
#define AWB_CT1_4000K_BGAIN 630

#define AWB_CT1_2500K_RGAIN 251
#define AWB_CT1_2500K_BGAIN 939


#define AWB_CT2_8500K_RGAIN 392
#define AWB_CT2_8500K_BGAIN 274

#define AWB_CT2_7000K_RGAIN 455
#define AWB_CT2_7000K_BGAIN 404

#define AWB_CT2_6000K_RGAIN 504
#define AWB_CT2_6000K_BGAIN 504

#define AWB_CT2_5000K_RGAIN 400
#define AWB_CT2_5000K_BGAIN 500

#define AWB_CT2_4500K_RGAIN 378
#define AWB_CT2_4500K_BGAIN 549

#define AWB_CT2_4300K_RGAIN 371
#define AWB_CT2_4300K_BGAIN 565

#define AWB_CT2_4000K_RGAIN 356
#define AWB_CT2_4000K_BGAIN 598

#define AWB_CT2_3700K_RGAIN 342 //*
#define AWB_CT2_3700K_BGAIN 630

#define AWB_CT2_3500K_RGAIN 284
#define AWB_CT2_3500K_BGAIN 748

#define AWB_CT2_3000K_RGAIN 227 //*
#define AWB_CT2_3000K_BGAIN 865

// The color temperature must be steeled sort
AWB_CTINFO AwbCTTable[AWB_PROC_CNT][AWB_CT_RANGE_MAX] =
{
    {
        {2500 , {AWB_CT1_2500K_RGAIN, 256, AWB_CT1_2500K_BGAIN}},
        {4000 , {AWB_CT1_4000K_RGAIN, 256, AWB_CT1_4000K_BGAIN}},
        {4500 , {AWB_CT1_4500K_RGAIN, 256, AWB_CT1_4500K_BGAIN}},
        {5000 , {AWB_CT1_5000K_RGAIN, 256, AWB_CT1_5000K_BGAIN}},
        {5500 , {AWB_CT1_5500K_RGAIN, 256, AWB_CT1_5500K_BGAIN}},
        {7000 , {AWB_CT1_7000K_RGAIN, 256, AWB_CT1_7000K_BGAIN}},
        {8500 , {AWB_CT1_8500K_RGAIN, 256, AWB_CT1_8500K_BGAIN}},
    },
    {
        {3000 , {AWB_CT2_3000K_RGAIN, 256, AWB_CT2_3000K_BGAIN}},
        {3500 , {AWB_CT2_3500K_RGAIN, 256, AWB_CT2_3500K_BGAIN}},
        {4000 , {AWB_CT2_4000K_RGAIN, 256, AWB_CT2_4000K_BGAIN}},
        {4500 , {AWB_CT2_4500K_RGAIN, 256, AWB_CT2_4500K_BGAIN}},
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
        (200<<4),   //UINT32 GGu; G (12Bits)
        (4<<4),     //UINT32 GGl;
        4095,       //UINT32 BGu; (B/G)<<10
        0,        //UINT32 BGl;
        4095,        //UINT32 RBGu;((R+B)/G)<<9
        0         //UINT32 RBGl;
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
    {AWBALG_TYPE_RG,     220,  88, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     402, 147, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    410,  73, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 510, 327, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   440, 280, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

AWBALG_ELEMET WhiteOutdoorElement1[] =
{
    {AWBALG_TYPE_Y,      (200<<4),   (4<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     220, 157, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     223, 147, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    127,  73, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 440, 327, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   440, 290, {  0,  0}, {  0,  0}},
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
        {AWB_CT2_6000K_RGAIN, 256, AWB_CT2_6000K_BGAIN},   // HCTMinGain; 5500K
        {AWB_CT2_8500K_RGAIN, 256, AWB_CT2_8500K_BGAIN},   // HCTMaxGain; 8500K
        (10<<14),                                         // LCTMaxEV; LV14
        (10<<10),                                         // LCTMinEV; LV8
        {AWB_CT2_4000K_RGAIN, 256, AWB_CT2_4000K_BGAIN},   // LCTMaxGain; 4000K
        {AWB_CT2_3000K_RGAIN, 256, AWB_CT2_3000K_BGAIN},   // LCTMinGain; 2300K
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
        {AWB_CT1_2500K_RGAIN, 256, AWB_CT1_2500K_BGAIN}    // LCTMinGain; 2556K
    },
    {
        100,  100,                                       // HCTRatio; //100~200
        {AWB_CT2_7000K_RGAIN, 256, AWB_CT2_7000K_BGAIN},   // HCTMaxGain;
        {AWB_CT2_8500K_RGAIN, 256, AWB_CT2_8500K_BGAIN},   // HCTMinGain;
        100,  100,                                        // LCTRatio;  //100~200
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


