#include "IPL_Cal_Setting_OV4689_IMX322LQJ_EVB_FF_Int.h"
//#include "IPL_Cal_DataPassing_OV4689_IMX322LQJ_EVB_FF_Int.h"
/*
calibration sensor infomation for per IPL (all sensor mode which need to calibration)
HD/VD inverse
Sony: FALSE
Pana: TRUE
*/
/**
    sensor mode information
    need to match with IPL_Cal_DataPassing CalSenModeMap()
*/
// OV4689_IMX322LQJ
Cal_SENSOR_INFO SensorMode_PRV_IPL1[] = {
    {SENSOR_MODE_3, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_UNKNOWN, 0,0,0,0,0,0,0,},
};

Cal_SENSOR_INFO SensorMode_AWB_IPL1[] = {
    {SENSOR_MODE_3, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_UNKNOWN, 0,0,0,0,0,0,0,},
};

Cal_SENSOR_INFO SensorMode_ECS_IPL1[] = {
    {SENSOR_MODE_3, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_9, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_UNKNOWN, 0,0,0,0,0,0,0,},
};

// need to match DPLimitCnt
Cal_SENSOR_INFO SensorMode_DP_IPL1[] = {
    {SENSOR_MODE_1, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_2, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_3, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_4, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_5, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_6, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_7, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_8, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_9, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_UNKNOWN, 0,0,0,0,0,0,0,},
};

Cal_SENSOR_INFO SensorMode_ISO_IPL1[] = {
    {SENSOR_MODE_3, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_UNKNOWN, 0,0,0,0,0,0,0,},
};

Cal_SENSOR_INFO SensorMode_AE_Check_IPL1[] = {
    {SENSOR_MODE_1, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_UNKNOWN, 0,0,0,0,0,0,0,},
};

Cal_SENSOR_INFO SensorMode_LENSCEN_IPL1[] = {
    {SENSOR_MODE_3, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_UNKNOWN, 0,0,0,0,0,0,0,},
};

// IMX322LQJ
Cal_SENSOR_INFO SensorMode_PRV_IPL2[] = {
    {SENSOR_MODE_1, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_UNKNOWN, 0,0,0,0,0,0,0,},
};

Cal_SENSOR_INFO SensorMode_AWB_IPL2[] = {
    {SENSOR_MODE_1, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_UNKNOWN, 0,0,0,0,0,0,0,},
};

Cal_SENSOR_INFO SensorMode_ECS_IPL2[] = {
    {SENSOR_MODE_1, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_2, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_UNKNOWN, 0,0,0,0,0,0,0,},
};

// need to match DPLimitCnt
Cal_SENSOR_INFO SensorMode_DP_IPL2[] = {
    {SENSOR_MODE_1, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_2, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_UNKNOWN, 0,0,0,0,0,0,0,},
};

Cal_SENSOR_INFO SensorMode_ISO_IPL2[] = {
    {SENSOR_MODE_1, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_UNKNOWN, 0,0,0,0,0,0,0,},
};

Cal_SENSOR_INFO SensorMode_AE_Check_IPL2[] = {
    {SENSOR_MODE_1, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_UNKNOWN, 0,0,0,0,0,0,0,},
};

Cal_SENSOR_INFO SensorMode_LENSCEN_IPL2[] = {
    {SENSOR_MODE_1, FALSE,FALSE,_RISING,_RISING,_RISING,0,0},
    {SENSOR_MODE_UNKNOWN, 0,0,0,0,0,0,0,},
};

Cal_SENSOR_INFO SenModeSection_End = {
    SENSOR_MODE_UNKNOWN, 0,0,0,0,0,0,0,
};

/**
    ae information
*/
CAL_AE_INFO_CONDITION AeInfo_PRV_IPL1[] = {
           /*adjust type  uiTarget   uiExpT       uiRatio    uiISO   uiISOBase uiAdjStep uiTimeout */
    {{TRUE,  AE_ADJ_GAIN,    38,  {20000,20000}, {100,100}, {50,50},     270,    20,         10},   CAL_CONDITION_DEFAULT},
    {{TRUE,  AE_FIX_ALL,     0,   {0,0},         {0,0},     {0,0},       0,      0,          0},    CAL_CONDITION_END},

};
CAL_AE_INFO_CONDITION AeInfo_ISO_IPL1[] = {
           /*adjust type  uiTarget   uiExpT       uiRatio    uiISO   uiISOBase uiAdjStep uiTimeout */
    {{TRUE,  AE_ADJ_ISOBASE, 38,  {1000,1000},   {100,100}, {50,50},     2000,   5,          50},   CAL_CONDITION_DEFAULT},
    {{TRUE,  AE_FIX_ALL,     0,   {0,0},         {0,0},     {0,0},       0,      0,          0},    CAL_CONDITION_END},
};
CAL_AE_INFO_CONDITION AeInfo_AWB_IPL1[] = {
           /*adjust type  uiTarget   uiExpT       uiRatio    uiISO   uiISOBase uiAdjStep uiTimeout */
    {{TRUE,  AE_ADJ_GAIN,    40,  {1000,1000},   {100,100}, {50,50},     270,    5,          10},   CAL_CONDITION_DEFAULT},
    {{TRUE,  AE_FIX_ALL,     0,   {0,0},         {0,0},     {0,0},       0,      0,          0},    CAL_CONDITION_END},
};
CAL_AE_INFO_CONDITION AeInfo_ECS_IPL1[] = {
           /*adjust type  uiTarget   uiExpT       uiRatio    uiISO   uiISOBase uiAdjStep uiTimeout */
    {{TRUE,  AE_ADJ_GAIN,    150, {2000,2000},   {100,100}, {50,50},     270,    20,         10},   CAL_CONDITION_DEFAULT},
    {{TRUE,  AE_FIX_ALL,     0,   {0,0},         {0,0},     {0,0},       0,      0,          0},    CAL_CONDITION_END},
};
CAL_AE_INFO_CONDITION AeInfo_DPB_IPL1[] = {
           /*adjust type  uiTarget   uiExpT       uiRatio    uiISO   uiISOBase uiAdjStep uiTimeout */
    {{TRUE,  AE_FIX_ALL,     1,   {66000,66000}, {100,100}, {200,200}, 270,    5,          10},   CAL_CONDITION_DEFAULT},
    {{TRUE,  AE_FIX_ALL,     0,   {0,0},         {0,0},     {0,0},       0,      0,          0},    CAL_CONDITION_END},
};
CAL_AE_INFO_CONDITION AeInfo_DPD_IPL1[] = {
           /*adjust type  uiTarget   uiExpT       uiRatio    uiISO   uiISOBase uiAdjStep uiTimeout */
    {{TRUE,  AE_ADJ_GAIN,    200, {4166,4166},   {100,100}, {25,25},     270,    5,          50},   CAL_CONDITION_DEFAULT},
    {{TRUE,  AE_FIX_ALL,     0,   {0,0},         {0,0},     {0,0},       0,      0,          0},    CAL_CONDITION_END},
};
CAL_AE_INFO_CONDITION AeInfo_LENSCEN_IPL1[] = {
           /*adjust type  uiTarget   uiExpT       uiRatio    uiISO   uiISOBase uiAdjStep uiTimeout */
    {{TRUE,  AE_ADJ_GAIN,    200, {4166,4166},   {100,100}, {25,25},     270,    5,          2},   CAL_CONDITION_DEFAULT},
    {{TRUE,  AE_FIX_ALL,     0,   {0,0},         {0,0},     {0,0},       0,      0,          0},    CAL_CONDITION_END},
};

CAL_AE_INFO_CONDITION AeInfo_PRV_IPL2[] = {
           /*adjust type  uiTarget   uiExpT       uiRatio    uiISO   uiISOBase uiAdjStep uiTimeout */
    {{TRUE,  AE_ADJ_GAIN,    38,  {20000,20000}, {100,100}, {50,50},     270,    20,         10},   CAL_CONDITION_DEFAULT},
    {{TRUE,  AE_FIX_ALL,     0,   {0,0},         {0,0},     {0,0},       0,      0,          0},    CAL_CONDITION_END},

};
CAL_AE_INFO_CONDITION AeInfo_ISO_IPL2[] = {
           /*adjust type  uiTarget   uiExpT       uiRatio    uiISO   uiISOBase uiAdjStep uiTimeout */
    {{TRUE,  AE_ADJ_ISOBASE, 38,  {4876,4876},   {100,100}, {50,50},     1500,   5,          50},   CAL_CONDITION_DEFAULT},
    {{TRUE,  AE_FIX_ALL,     0,   {0,0},         {0,0},     {0,0},       0,      0,          0},    CAL_CONDITION_END},
};
CAL_AE_INFO_CONDITION AeInfo_AWB_IPL2[] = {
           /*adjust type  uiTarget   uiExpT       uiRatio    uiISO   uiISOBase uiAdjStep uiTimeout */
    {{TRUE,  AE_ADJ_GAIN,    40,  {8333,8333},   {100,100}, {50,50},     270,    5,          10},   CAL_CONDITION_DEFAULT},
    {{TRUE,  AE_FIX_ALL,     0,   {0,0},         {0,0},     {0,0},       0,      0,          0},    CAL_CONDITION_END},
};
CAL_AE_INFO_CONDITION AeInfo_ECS_IPL2[] = {
           /*adjust type  uiTarget   uiExpT       uiRatio    uiISO   uiISOBase uiAdjStep uiTimeout */
    {{TRUE,  AE_ADJ_GAIN,    150, {4166,4166},   {100,100}, {25,25},     270,    20,         10},   CAL_CONDITION_DEFAULT},
    {{TRUE,  AE_FIX_ALL,     0,   {0,0},         {0,0},     {0,0},       0,      0,          0},    CAL_CONDITION_END},
};
CAL_AE_INFO_CONDITION AeInfo_DPB_IPL2[] = {
           /*adjust type  uiTarget   uiExpT       uiRatio    uiISO   uiISOBase uiAdjStep uiTimeout */
    {{TRUE,  AE_FIX_ALL,     38,  {1000000,1000000}, {100,100}, {200,200}, 270,    5,          10},   CAL_CONDITION_DEFAULT},
    {{TRUE,  AE_FIX_ALL,     0,   {0,0},         {0,0},     {0,0},       0,      0,          0},    CAL_CONDITION_END},
};
CAL_AE_INFO_CONDITION AeInfo_DPD_IPL2[] = {
           /*adjust type  uiTarget   uiExpT       uiRatio    uiISO   uiISOBase uiAdjStep uiTimeout */
    {{TRUE,  AE_ADJ_GAIN,    180, {4166,4166},   {100,100}, {25,25},     270,    5,          50},   CAL_CONDITION_DEFAULT},
    {{TRUE,  AE_FIX_ALL,     0,   {0,0},         {0,0},     {0,0},       0,      0,          0},    CAL_CONDITION_END},
};
CAL_AE_INFO_CONDITION AeInfo_LENSCEN_IPL2[] = {
           /*adjust type  uiTarget   uiExpT       uiRatio    uiISO   uiISOBase uiAdjStep uiTimeout */
    {{TRUE,  AE_ADJ_GAIN,    200, {4166,4166},   {100,100}, {25,25},     270,    5,          2},   CAL_CONDITION_DEFAULT},
    {{TRUE,  AE_FIX_ALL,     0,   {0,0},         {0,0},     {0,0},       0,      0,          0},    CAL_CONDITION_END},
};




/*
    cal information
*/
CAL_ISO_INFO Cal_iso_info_1 = {
    1800,   20
};
CAL_AWB_INFO Cal_awb_info_1 = {
    CC_2_8, 15, 5
};
CAL_ECS_INFO Cal_ecs_info_1 = {
    ECS_D2F8, ecs65x65, 90
};
CAL_DPC_THRES DPC_THRES_BRIGHT_1 = {
    TRUE, 20,32,32
};
CAL_DPC_THRES DPC_THRES_DARK_1 = {
    FALSE, 40,32,32
};
CAL_LENSCEN_INFO Cal_lenscen_info_1 = {
    10, 200, 20, 50, 75
};

CAL_ISO_INFO Cal_iso_info_2 = {
    1800,   20
};
CAL_AWB_INFO Cal_awb_info_2 = {
    CC_2_8, 15, 5
};
CAL_ECS_INFO Cal_ecs_info_2 = {
    ECS_D2F8, ecs65x65, 90
};
CAL_DPC_THRES DPC_THRES_BRIGHT_2 = {
    TRUE, 12,32,32
};
CAL_DPC_THRES DPC_THRES_DARK_2 = {
    FALSE, 12,32,32
};
CAL_LENSCEN_INFO Cal_lenscen_info_2 = {
    10, 200, 20, 50, 75
};

CAL_INFO Cal_info_IPL1 = {
    &Cal_iso_info_1,
    &Cal_awb_info_1,
    &Cal_ecs_info_1,
    &DPC_THRES_BRIGHT_1,
    &DPC_THRES_DARK_1,
    &Cal_lenscen_info_1,
};

CAL_INFO Cal_info_IPL2 = {
    &Cal_iso_info_2,
    &Cal_awb_info_2,
    &Cal_ecs_info_2,
    &DPC_THRES_BRIGHT_2,
    &DPC_THRES_DARK_2,
    &Cal_lenscen_info_2,
};
CAL_INFO Cal_info_IPL3 = {};
CAL_INFO Cal_info_IPL4 = {};

// need to match sensor mode information (SensorMode_DP_IPL)
UINT32 DPLimitCnt[IPL_ID_MAX_NUM][MaxSenModeNum] =
{
    {10000,10000,10000,10000,0,0,0,0,0,0},//IPL_1
    {10000,10000,10000,10000,0,0,0,0,0,0},//IPL_2
    {10000,10000,10000,10000,0,0,0,0,0,0},//IPL_3
    {10000,10000,10000,10000,0,0,0,0,0,0},//IPL_4


};

/*
    calibration section information
    (calibration item, sensor mode information, section size, AE setting)
*/
Cal_SECTION CalSection_IPL1[] = {
    /*CalItem           Sensor mode information         section size            ae informaiton          DP select*/
    {PRV_1,             &SensorMode_PRV_IPL1[0],        0,                      &AeInfo_PRV_IPL1[0],    TRUE},//PRV
    {SEC_AWB_1,         &SensorMode_AWB_IPL1[0],        0x100,                  &AeInfo_AWB_IPL1[0],    TRUE},//AWB
    {SEC_ECS_1,         &SensorMode_ECS_IPL1[0],        (65*65*4*5*1+0x100),    &AeInfo_ECS_IPL1[0],    TRUE},//ECS
    {SEC_DP_1,          &SensorMode_DP_IPL1[0],         (90000*4+0x100),         &AeInfo_DPB_IPL1[0],    TRUE},//DP bright
    {SEC_DP_1,          &SensorMode_DP_IPL1[0],         (90000*4+0x100),         &AeInfo_DPD_IPL1[0],    FALSE},//DP dark
    {SEC_MSHUT_1,       NULL,                           0x100,                  NULL,                   TRUE},//MSHUT
    {SEC_ISO_1,         &SensorMode_ISO_IPL1[0],        0x100,                  &AeInfo_ISO_IPL1[0],    TRUE},//ISO
    {SEC_AECHK_1,       &SensorMode_AE_Check_IPL1[0],   0x100,                  NULL,                   TRUE},//AECHK
    {SEC_LENSCEN_1,     &SensorMode_LENSCEN_IPL1[0],    0x100,                  &AeInfo_LENSCEN_IPL1[0],TRUE},//LENSCEN
    {IPL_Section_End,   &SenModeSection_End,            0x0,                    NULL,                   TRUE},//END
};

Cal_SECTION CalSection_IPL2[] = {
    /*CalItem           Sensor mode information         section size            ae informaiton          DP select*/
    {PRV_2,             &SensorMode_PRV_IPL2[0],        0,                      &AeInfo_PRV_IPL2[0],    TRUE},//PRV
    {SEC_AWB_2,         &SensorMode_AWB_IPL2[0],        0x100,                  &AeInfo_AWB_IPL2[0],    TRUE},//AWB
    {SEC_ECS_2,         &SensorMode_ECS_IPL2[0],        (65*65*4*2*1+0x100),    &AeInfo_ECS_IPL2[0],    TRUE},//ECS
    {SEC_DP_2,          &SensorMode_DP_IPL2[0],         (40000*4+0x100),        &AeInfo_DPB_IPL2[0],    TRUE},//DP bright
    {SEC_DP_2,          &SensorMode_DP_IPL2[0],         (40000*4+0x100),        &AeInfo_DPD_IPL2[0],    FALSE},//DP dark
    {SEC_MSHUT_2,       NULL,                           0x100,                  NULL,                   TRUE},//MSHUT
    {SEC_ISO_2,         &SensorMode_ISO_IPL2[0],        0x100,                  &AeInfo_ISO_IPL2[0],    TRUE},//ISO
    {SEC_AECHK_2,       &SensorMode_AE_Check_IPL2[0],   0x100,                  NULL,                   TRUE},//AECHK
    {SEC_LENSCEN_2,     &SensorMode_LENSCEN_IPL2[0],    0x100,                  &AeInfo_LENSCEN_IPL2[0],TRUE},//LENSCEN
    {IPL_Section_End,   &SenModeSection_End,            0x0,                    NULL,                   TRUE},//END
};

Cal_SECTION CalSection_IPL3[] = {
    /*CalItem           Sensor mode information         section size            ae informaiton          DP select*/
    {IPL_Section_End,   &SenModeSection_End,            0x0,                    NULL,                   TRUE},//END
};

Cal_SECTION CalSection_IPL4[] = {
    /*CalItem           Sensor mode information         section size            ae informaiton          DP select*/
    {IPL_Section_End,   &SenModeSection_End,            0x0,                    NULL,                   TRUE},//END
};

/*
    calibration section all information
    (calibration item, sensor mode information, section size, AE setting, cal information)
    for per IPL
*/
Cal_SECTION_INFO CalSectionInfo[IPL_ID_MAX_NUM] = {
    {&CalSection_IPL1[0], &Cal_info_IPL1},
    {&CalSection_IPL2[0], &Cal_info_IPL2},
    {&CalSection_IPL3[0], &Cal_info_IPL3},
    {&CalSection_IPL4[0], &Cal_info_IPL4},
};

/**
    calibration Debug
*/

CAL_DEBUG_INFO_DP CalDebugDP_IPL1 = {0};
CAL_DEBUG_INFO_ISO CalDebugISO_IPL1 = {0};
CAL_DEBUG_INFO_ECS CalDebugECS_IPL1 = {0};
CAL_DEBUG_INFO_AWB CalDebugAWB_IPL1 = {0};
CAL_DEBUG_INFO_LENSCEN CalDebugLENSCEN_IPL1 = {0};
CAL_DEBUG_INFO_DP CalDebugDP_IPL2 = {0};
CAL_DEBUG_INFO_ISO CalDebugISO_IPL2 = {0};
CAL_DEBUG_INFO_ECS CalDebugECS_IPL2 = {0};
CAL_DEBUG_INFO_AWB CalDebugAWB_IPL2 = {0};
CAL_DEBUG_INFO_LENSCEN CalDebugLENSCEN_IPL2 = {0};

CAL_DEBUG_INFO CalDebugInfo[IPL_ID_MAX_NUM] = {
    {&CalDebugDP_IPL1, &CalDebugISO_IPL1, &CalDebugECS_IPL1, &CalDebugAWB_IPL1, &CalDebugLENSCEN_IPL1},
    {&CalDebugDP_IPL2, &CalDebugISO_IPL2, &CalDebugECS_IPL2, &CalDebugAWB_IPL2, &CalDebugLENSCEN_IPL2},
    {NULL, NULL, NULL, NULL},
    {NULL, NULL, NULL, NULL},
};

#if 0
    - DCE cfg
#endif
DIST CAL_DCE_Dist =
{
    0xfff,0xfff
};

UINT32 CAL_DCE_GLUT[65] =
{
    65535, 65485, 65435, 65385, 65335, 65285, 65235, 65185, 65135, 65085, 65035, 64985, 64935, 64885, 64835, 64785, 64735, 64685, 64635, 64585, 64535, 64485, 64435, 64385, 64335, 64285, 64235, 64185, 64135, 64085, 64035, 63985, 63935, 63885, 63835, 63785, 63735, 63685, 63635, 63585, 63535, 63485, 63435, 63385, 63335, 63285, 63235, 63185, 63135, 63085, 63035, 62985, 62935, 62885, 62835, 62785, 62735, 62685, 62635, 62585, 62535, 62485, 62435, 62385, 62335,
};

INT32 CAL_DCE_RLUT[65] =
{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

INT32 CAL_DCE_BLUT[65] =
{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

DCE_Distortion Cal_DCE_Distortion = {
    {0,0},                                          //Center
    {0x400,FOV_BOUND_DUPLICATE,{1024,1024,1024}},   //FOV
    {96, 6, 16, 6},                                 //Enh
    {CAC_RGB_GAIN,{0x1000,0x1000,0x1000}},          //aber
    {CAL_DCE_GLUT,CAL_DCE_RLUT,CAL_DCE_BLUT},       //GDC/CAC G/R/B geo lut(tab num:65, Data type:UINT32/INT32/INT32)
    //0,                                            //lut table type,don't care
    FALSE                                           //Horizontal filter for UV channel
};

DCE_CFAPARAM CAL_DCE_IQS_CFAInter =
{
    //NsmarEdge,  NsmarDiff,  DIFNormBit, {GCEn, GCNBit, Thr},{RBCEn,RBCNBit, Thr, Thr2}
           0,            8,   0,         {TRUE,      2,   0},{TRUE,      2,   0,   0},
};

UINT32 CAL_IQS_CGain[4] = {256, 256, 256, 256};

UINT32 CAL_IQS_CGain_PRV[4] = {256, 256, 256, 256};

IPC_IQparam     Ipc_IQ = {0};

UINT32 IFE_spatial[6] = {1, 0, 0, 0, 0, 0};
UINT32 IFE_A_pThres[6] = {0};
UINT32 IFE_A_pThLut[17] = {0};
UINT32 IFE_B_pThres[6] = {0};
UINT32 IFE_B_pThLut[17] = {0};

UINT32 GainMask = 0xFFF;

IFE_SUBFUNC     Ife_sub1 = {0};

IPL_HAL_GROUP   Grp1 = {0};

IFE_FILT pFilter1 = {0};
Spatial_Filter Spatial1 = {0};
UINT32   pWeight1[6] = {0};

Range_Filter Range1 = {0};
RangeCFGA A11 = {0};
RangeCFGB B11 = {0};
UINT32 A_pThres1[6] = {0};
UINT32 A_pThLut1[17] = {0};
UINT32 B_pThres1[6] = {0};
UINT32 B_pThLut1[17] = {0};

UINT32 CAL_IQS_CGain_Offset[4] = { 0, 0, 0, 0};

IFE_CG CAL_IQS_ColorGain =
{
    {FALSE, FALSE}, _2_8, {0xfff}, CAL_IQS_CGain, CAL_IQS_CGain_Offset,
};

IPE_RGBLPF CAL_IQS_RGBLpf =
{
    //  lpfw,sonlyw,rangeth0,rangeth1,lpfsize;
    {    4,     0,       4,       8,    _5x5}, {    4,     0,       4,       8,    _5x5}, {    4,     0,       4,       8,    _5x5}
};

INT16 CAL_IQS_CSTCoef[9] =
{
     77, 150, 29, -43, -85, 128, 128, -107, -21,
};

IPE_CST CAL_IQS_CSTParam =
{
    (UINT32)CAL_IQS_CSTCoef, CST_SEL_MINUS128, 8,
};

UINT32 CAL_IQS_GammaLut[130]={

    0,32,84,129,169,205,237,265,293,321,345,369,390,410,
    430,450,466,482,498,514,530,546,558,574,586,602,610,
    622,634,642,655,663,675,683,691,699,707,719,727,735,
    739,747,755,763,767,775,779,783,791,795,799,803,807,
    815,819,823,827,831,835,839,843,847,851,855,859,863,
    867,871,875,879,883,887,891,891,895,899,903,903,907,
    911,915,915,919,924,924,928,932,932,936,940,940,944,
    944,948,952,952,956,956,960,960,964,968,968,972,972,
    976,976,980,984,984,988,988,992,992,996,996,1000,1000,1004,
    1004,1008,1008,1012,1012,1016,1020,1021,1022,1023,1023,
};

UINT16 CAL_Normal_CC[9] = {
    0x018d,0x0742,0x0030,
    0x07b9,0x0183,0x07c4,
    0x07fb,0x074e,0x01b7,
};

//FSTAB
UINT8 CAL_IQS_FStab[16]=
{
    32,16,8,0,0,0,0,0,0,0,0,0,0,32,128,255,
};

//FDTAB
UINT8 CAL_IQS_FDtab[16]=
{
    64,48,32,16,8,0,0,0,0,0,0,0,0,0,0,0,
};

IPE_CC CAL_IQS_ColorCorrection =
{
    CC_2_8, toIdentity, CC_POS_GAMMA, CC_SEL_MAX, (UINT32)&CAL_Normal_CC, (UINT32)CAL_IQS_FStab, (UINT32)CAL_IQS_FDtab,
};

IME_CHRA_ADAP CAL_IQS_ChromaAdapt =
{
    //LcaRefWt{RefY,RefUV,OutUV}, LcaYinfo{LcaY,LcaYTh,LcaYWtPrc,LcaYWtS,LcaYWtE}, LcaUVinfo{   UV, UVTh, UVWtPrc, UVWtS, UVWtE}, SubRatio
             {   0,    0,    0},      {RANGE_8,     0,  RANGE_8,      0,      0},        {RANGE_8,    0, RANGE_8,     0,     0}, 20,
};
/*
IME_CHRA_FILT CAL_IQS_ChromaFilter =
{
    //RangeFiltEn, RangeFilt, SpaceFilt
    TRUE,         {{ 1, 3, 5, 7}, { 1, 3, 5, 7}, { 7, 5, 3, 1, 0}}, {2, 1, 32},
};
*/
IME_CHRA_LADAP CAL_IQS_ChromaAdapt_Lum =
{
    //LcaLumEn, LcaRefWt{RefWt, OutWt}, LcaLuminfo{LumRng,LumTh,LumWtPrc,LumWtS,LumWtE}
       ENABLE,         {0,         0},           {     0,    0,       0,     0,     0},
};

IME_CHRA_CADAP CAL_IQS_ChromaAdapt_Color =
{
    //LcaCLREn, LcaRefWt{RefWt, OutWt}, LcaCLRinfo{LumRng,LumTh,LumWtPrc,LumWtS,LumWtE}
       ENABLE,         {0,         0},           {     0,    0,       0,     0,     0},
};

IFE2_GRAY_Statal CAL_IQS_GrayStatical = {0, 128, 0, 128};

UINT32 CAL_IQS_RefCenter_YTh[3] = {0, 0, 0};
UINT32 CAL_IQS_RefCenter_YWt[4] = {0, 0, 0, 0};
IFE2_REFCENT_Set CAL_IQS_RefCenter_Y = {(UINT32)CAL_IQS_RefCenter_YTh, (UINT32)CAL_IQS_RefCenter_YWt, 0, 0, 0};

UINT32 CAL_IQS_RefCenter_UVTh[3] = {0, 0, 0};
UINT32 CAL_IQS_RefCenter_UVWt[4] = {0, 0, 0, 0};
IFE2_REFCENT_Set CAL_IQS_RefCenter_UV = {(UINT32)CAL_IQS_RefCenter_UVTh, (UINT32)CAL_IQS_RefCenter_UVWt, 0, 0, 0};

IFE2_Refcent_Param CAL_IQS_RefCenter;

UINT32 CAL_IQS_IFE2Filter_YTh[5] =
{
    0, 0, 0, 0, 0,
};

UINT32 CAL_IQS_IFE2Filter_YWt[6] =
{
    0, 0, 0, 0, 0, 0,
};

UINT32 CAL_IQS_IFE2Filter_UTh[5] =
{
    0, 0, 0, 0, 0,
};

UINT32 CAL_IQS_IFE2Filter_UWt[6] =
{
    0, 0, 0, 0, 0, 0,
};

UINT32 CAL_IQS_IFE2Filter_VTh[5] =
{
    0, 0, 0, 0, 0,
};

UINT32 CAL_IQS_IFE2Filter_VWt[6] =
{
    0, 0, 0, 0, 0, 0,
};

IFE2_Filter_Param CAL_IQS_IFE2Filter =
{
    EKNL_SIZE_3x3, {0, 0}, FLTR_SIZE_3x3, {(UINT32)CAL_IQS_IFE2Filter_YTh, (UINT32)CAL_IQS_IFE2Filter_YWt},  {(UINT32)CAL_IQS_IFE2Filter_UTh, (UINT32)CAL_IQS_IFE2Filter_UWt}, {(UINT32)CAL_IQS_IFE2Filter_VTh, (UINT32)CAL_IQS_IFE2Filter_VWt},
};

UINT32 CAL_IQS_Outl_Bri[5] =
{
    126, 184, 304, 420, 512,
};

UINT32 CAL_IQS_Outl_Dark[5] =
{
    189, 276, 465, 630, 768,
};

IFE_OUTL CAL_IQS_Outl =
{
    CAL_IQS_Outl_Bri, CAL_IQS_Outl_Dark, _8_NB,
};

UINT32 CAL_IQS_SF_Weight[6] =
{
    18, 10, 9, 2, 1, 0,
};

Spatial_Filter CAL_IQS_SpatialFilter =
{
    DISABLE, Filt_3X3, CAL_IQS_SF_Weight,
};

UINT32 CAL_IQS_RangeA_Thr[6] =
{
    0, 3, 6, 9, 9, 9,
};

UINT32 CAL_IQS_RangeA_Lut[17] =
{
    0, 3, 6, 9, 11, 12, 14, 16, 18, 20, 22, 24, 26, 27, 29, 31, 33,
};

RangeCFGA CAL_IQS_RangeA =
{
    //pThres,                    pThLut
    CAL_IQS_RangeA_Thr, CAL_IQS_RangeA_Lut,
};

UINT32 CAL_IQS_RangeB_Thr[6] =
{
    0, 6, 12, 18, 18, 18,
};

UINT32 CAL_IQS_RangeB_Lut[17] =
{
    0,6,12,17,21,25,29,32,36,40,44,47,51,55,59,62,66,
};

RangeCFGB CAL_IQS_RangeB =
{
    //pThres,                    pThLut,                     BilatThEn, BilatTh1, BilatTh2
    CAL_IQS_RangeB_Thr, CAL_IQS_RangeB_Lut, {TRUE, 0, 0},
};

Range_Filter CAL_IQS_RangeFilter =
{
    //RangeCFGA,              RangeCFGB
    &CAL_IQS_RangeA, &CAL_IQS_RangeB,
};

IFE_FILT CAL_IQS_Filter =
{
    //Mode,    SpatialFilter,                  RangeFilter              ,clamp{Th,  Mul, Dlt}, bilat_w, Rth_w, Bin}
     Normal,  &CAL_IQS_SpatialFilter, &CAL_IQS_RangeFilter,     { 5, 128,   0},   4,      2,      0,
};

UINT8 CAL_IQS_Gbal_Stab[16] =
{
   1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,
};

UINT8 CAL_IQS_Gbal_Dtab[16] =
{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

IFE_GBAL CAL_IQS_Gbal =
{
    //Thres    {STab[16]}                   {DTab[16]}
      1023,   CAL_IQS_Gbal_Stab,   CAL_IQS_Gbal_Dtab,
};
