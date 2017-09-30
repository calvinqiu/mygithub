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
#include "awb_IMX078CQK_EVB_FF_int.h"

#define AWB_CT1_8500K_RGAIN 184
#define AWB_CT1_8500K_BGAIN 128

#define AWB_CT1_7700K_RGAIN 186
#define AWB_CT1_7700K_BGAIN 143

#define AWB_CT1_7000K_RGAIN 189
#define AWB_CT1_7000K_BGAIN 159

#define AWB_CT1_5500K_RGAIN 178
#define AWB_CT1_5500K_BGAIN 184

#define AWB_CT1_5000K_RGAIN 163
#define AWB_CT1_5000K_BGAIN 187

#define AWB_CT1_4000K_RGAIN 130
#define AWB_CT1_4000K_BGAIN 251

#define AWB_CT1_2500K_RGAIN 79
#define AWB_CT1_2500K_BGAIN 346


#define AWB_CT2_8500K_RGAIN 184
#define AWB_CT2_8500K_BGAIN 128

#define AWB_CT2_7700K_RGAIN 186
#define AWB_CT2_7700K_BGAIN 143

#define AWB_CT2_7000K_RGAIN 189
#define AWB_CT2_7000K_BGAIN 159

#define AWB_CT2_5500K_RGAIN 178
#define AWB_CT2_5500K_BGAIN 184

#define AWB_CT2_5000K_RGAIN 163
#define AWB_CT2_5000K_BGAIN 187

#define AWB_CT2_4000K_RGAIN 130
#define AWB_CT2_4000K_BGAIN 251

#define AWB_CT2_2500K_RGAIN 79
#define AWB_CT2_2500K_BGAIN 346

// The color temperature must be steeled sort
AWB_CTINFO AwbCTTable[AWB_PROC_CNT][AWB_CT_RANGE_MAX] =
{
    {
        {2500 , {AWB_CT1_2500K_RGAIN, 256, AWB_CT1_2500K_BGAIN}},
        {4000 , {AWB_CT1_4000K_RGAIN, 256, AWB_CT1_4000K_BGAIN}},
        {5000 , {AWB_CT1_5000K_RGAIN, 256, AWB_CT1_5000K_BGAIN}},
        {5500 , {AWB_CT1_5500K_RGAIN, 256, AWB_CT1_5500K_BGAIN}},
        {7000 , {AWB_CT1_7000K_RGAIN, 256, AWB_CT1_7000K_BGAIN}},
        {7700 , {AWB_CT1_7700K_RGAIN, 256, AWB_CT1_7700K_BGAIN}},
        {8500 , {AWB_CT1_8500K_RGAIN, 256, AWB_CT1_8500K_BGAIN}},
    },
    {
        {2500 , {AWB_CT2_2500K_RGAIN, 256, AWB_CT2_2500K_BGAIN}},
        {4000 , {AWB_CT2_4000K_RGAIN, 256, AWB_CT2_4000K_BGAIN}},
        {5000 , {AWB_CT2_5000K_RGAIN, 256, AWB_CT2_5000K_BGAIN}},
        {5500 , {AWB_CT2_5500K_RGAIN, 256, AWB_CT2_5500K_BGAIN}},
        {7000 , {AWB_CT2_7000K_RGAIN, 256, AWB_CT2_7000K_BGAIN}},
        {7700 , {AWB_CT2_7700K_RGAIN, 256, AWB_CT2_7700K_BGAIN}},
        {8500 , {AWB_CT2_8500K_RGAIN, 256, AWB_CT2_8500K_BGAIN}},
    },
};
AWB_CAINFO AwbPreWhiteElement[AWB_PROC_CNT] =
{
    {
        32, 32,
        1191,       //UINT32 RGu; (R/G)<<10
        479,        //UINT32 RGl;
        (200<<4),   //UINT32 GGu; G (12Bits)
        (4<<4),     //UINT32 GGl;
        770,       //UINT32 BGu; (B/G)<<10
        290,        //UINT32 BGl;
        981,        //UINT32 RBGu;((R+B)/G)<<9
        385         //UINT32 RBGl;
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
    {AWBALG_TYPE_Y,      (200<<4),   (4<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     214,  86, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     354, 134, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    364,  71, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 440, 300, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   370, 235, {  0,  0}, {  0,  0}},
    {AWBALG_TAB_END,       0,   0, {  0,  0}, {  0,  0}}
};

AWBALG_ELEMET WhiteOutdoorElement1[] =
{
    {AWBALG_TYPE_Y,      (200<<4),   (4<<4), {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RG,     214,  86, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_BG,     354, 130, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBG,    364,  65, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RBGSUM, 440, 305, {  0,  0}, {  0,  0}},
    {AWBALG_TYPE_RXBG,   370, 230, {  0,  0}, {  0,  0}},
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
        {AWB_CT1_7700K_RGAIN, 256, AWB_CT1_7700K_BGAIN},   // HCTMinGain; 5500K
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
        {AWB_CT2_2500K_RGAIN, 256, AWB_CT2_2500K_BGAIN}    // LCTMinGain; 2556K
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


