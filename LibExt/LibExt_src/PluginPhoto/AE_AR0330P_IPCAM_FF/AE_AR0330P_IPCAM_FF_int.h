/**
    Auto Exposure.


    @file       ae_sample_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AE_AR0330P_IPCAM_FF_INT_H_
#define _AE_AR0330P_IPCAM_FF_INT_H_

#include "Sensor.h"
#include "ae_api.h"

/**
     @name AE default value
     @note
*/
//@{
//FIXED PARAMETER BEGIN
#define AE_TOTAL_PARAMNUM           6                           ///< always keep six frames AE Setting. don't modify this value;
#define AE_PROC_CNT                 2                           ///< NT96660 support maximum 2 AE Process
#define AE_STICH_LEFT_POS           0
#define AE_STICH_RIGHT_POS          1
//FIXED PARAMETER END

//LA INFO
#define LA_WIN_X                    32                          ///< SIE LA window number x
#define LA_WIN_Y                    32                          ///< SIE LA window number y
#define AE_HIST_NUM                 64                          ///< historgram bin number
#define AE_HIST_DATA_NUM            65536                       ///< historgram total data number

//AE INFO
#define AE_WIN_X                    8                           ///< ae weighting window x
#define AE_WIN_Y                    8                           ///< ae weighting window y
#define AE_COVER_RANGE_H            3                           ///< cover range H
#define AE_COVER_RANGE_L            3                           ///< cover range L
#define AE_COVER_RANGE_RF_H        100                           ///< cover range H rougth
#define AE_COVER_RANGE_RF_L        100                           ///< cover range L rougth
#define AE_EV_RATIO                 (100)
#define AE_NOR_CAL_SET_SHIFT       0
#define AE_NOR_PREDICT_POINT       3
#define AE_NOR_PEROID              3
#define AE_HISPED_CAL_SET_SHIFT    1
#define AE_HISPED_PREDICT_POINT    1
#define AE_HISPED_PEROID           4

//Face Detection AE ON/OFF
#define AE_FACE_DETECTION           DISABLE

//IR CUT PARAM
#define AE_IRCUT_DELAY_CNT          10
#define AE_IRCUT_LV_H               50
#define AE_IRCUT_LV_L               40
#define AE_IRCUT_DELAY_TIME         333330

//#NT#2016/06/30#Jarkko Chang -begin
//#NT# fixed ip cam frequency set fail
#define AE_60FREQ_NODE              ((1 << 1) | (1 << 2) | (1 << 4))
#define AE_50FREQ_NODE              ((1 << 1) | (1 << 2) | (1 << 4) | (1 << 5) | (1 << 8))

#define AE_60FREQ_EXPT_NODE_33333   33333 //us
#define AE_50FREQ_EXPT_NODE_20000   20000 //us
#define AE_50FREQ_EXPT_NODE_40000   40000 //us
//#NT#2016/06/30#Jarkko Chang -end

#define AE_STABLE_THRESHOLD         10

//
#define AE_TIMEOUT_MS               500
//@}

/**
     @name AE flag
     @note
*/
//@{
#define AE_FLAG_SET_EXPT    (1 << 0)
#define AE_FLAG_SET_GAIN    (1 << 1)
#define AE_FLAG_SET_IRIS    (1 << 2)
//@}


/**
     AE flicker type.

     @note
*/
typedef enum _AE_FLICKER
{
    AE_FLICKER_50HZ = 0, ///< flicker 50Mz
    AE_FLICKER_60HZ,     ///< flicker 60Mz
    AE_FLICKER_MAX,
    ENUM_DUMMY4WORD(AE_FLICKER)
}AE_FLICKER;

/**
     AE ISO type.

     @note
*/
typedef enum _AE_ISO
{
    AE_ISO_AUTO = 0,    ///< ISO_AUTO
    AE_ISO_25,          ///< ISO_25
    AE_ISO_50,          ///< ISO_50
    AE_ISO_80,          ///< ISO_80
    AE_ISO_100,         ///< ISO_100
    AE_ISO_200,         ///< ISO_200
    AE_ISO_400,         ///< ISO_400
    AE_ISO_800,         ///< ISO_800
    AE_ISO_1600,        ///< ISO_1600
    AE_ISO_3200,        ///< ISO_3200
    AE_ISO_6400,        ///< ISO_6400
    AE_ISO_MAX,
    ENUM_DUMMY4WORD(AE_ISO)
}AE_ISO;

/**
     AE win type.

     @note
*/
typedef enum _AE_WIN
{
    AE_WIN_CENTERWEIGHT = 0,    ///< ae center weighting
    AE_WIN_SPOT,                ///< ae spot weighting
    AE_WIN_MARTIX,              ///< ae matrix weighting
    AE_WIN_EVALUATIVE,          ///< ae evaluative weighting
    AE_WIN_USERDEFINE,          ///< ae user define weighting
    AE_WIN_MAX,
    ENUM_DUMMY4WORD(AE_WIN)
}AE_WIN;

/**
     AE scene type.

     @note
*/
typedef enum _AE_SCENE
{
    AE_SCENE_AUTO = 0,          ///< auto
    AE_SCENE_LANDSCAPE,         ///< landscape
    AE_SCENE_POTRAIT,           ///< potrait
    AE_SCENE_SPORT,
    AE_SCENE_NIGHTSCENE,
    AE_SCENE_AV,
    AE_SCENE_TV,
    AE_SCENE_MANUAL,
    AE_SCENE_NIGHTPOTRAIT,
    AE_SCENE_NIGHTLANDSCAPE,
    AE_SCENE_PARTY,
    AE_SCENE_SUNSET,
    AE_SCENE_BACKLIGHT,
    AE_SCENE_DOCUMENT,
    AE_SCENE_FIREWORK,
    AE_SCENE_BEACH,
    AE_SCENE_BMODE,
    AE_SCENE_PROGRAM,
    AE_SCENE_HIGHISO,
    AE_SCENE_MAX,
    ENUM_DUMMY4WORD(AE_SCENE)
}AE_SCENE;


/**
     AE anti-shake level.

     @note
*/
typedef enum _AE_ANTISHAKE
{
    AE_ANTISHAKE_LV0 = 0,       ///< disable
    AE_ANTISHAKE_LV1,
    AE_ANTISHAKE_LV2,
    AE_ANTISHAKE_MAX,
    ENUM_DUMMY4WORD(AE_ANTISHAKE)
}AE_ANTISHAKE;


/**
     AE Reset Flag.

     @note
*/
typedef enum _AE_RESET
{
    AE_RESET_FALSE = 0, ///<
    AE_RESET_TRUE,      ///< set ae reset
    AE_RESET_MAX,
    ENUM_DUMMY4WORD(AE_RESET)
}AE_RESET;

#define AE_SYNC_TAB_SIZE    10
typedef struct _AE_SYNC_INFO
{
    UINT32 frame_cnt;
    AEALG_ARG data;
} AE_SYNC_INFO;

typedef struct _AE_INT_CTRL_INFO
{
    UINT32 CalSetShiftNum;  //Set this parameter to separate ae_converge and ae_set(ExpT/Gain) to different frame
    UINT32 PredictPoint;    //valid value 1~3
    UINT32 AEPeroid;        //minimum value: 3
} AE_INT_CTRL_INFO;

typedef struct _AE_IRCUT_INFO
{
    UINT32 Id;
    UINT32 avg_y;
    UINT32 ev;
    UINT32 iso;
    UINT32 expt;

} AE_IRCUT_INFO;

extern AE_PARAMINFO AeParamInfo[AE_PROC_CNT];
extern AE_SET_PARAMINFO AeExtSetDefaultParamInfo[AE_PROC_CNT];
extern UINT32 ExptY_Mov_Prv[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_Mov_IRoff_Prv[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_Photo_Prv[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 Histo_Prv[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_Mov_OverExpoAdj[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_Mov_OverExpoAdj_HDR16X[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_Mov_OverExpoAdj_HDR4X[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_Photo_OverExpoAdj[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_Photo_OverExpoAdj_HDR16X[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_Photo_OverExpoAdj_HDR4X[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_Cap[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_OverExpoThreshold[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_OverExpoMaxCnt[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX];
extern UINT32 ExptY_OverExpoMinCnt[AE_PROC_CNT][AEALG_DYNAMIC_EXPECT_LUM_MAX];

extern UINT8 AE_Win_Face[AE_WIN_Y][AE_WIN_X];
extern UINT8 AE_Win_UserDef[AE_WIN_Y][AE_WIN_X];
extern UINT8 AE_Win_Adaptive[AE_WIN_Y][AE_WIN_X];
extern const UINT8 AE_Win_CenterWeighted[AE_WIN_Y][AE_WIN_X];
extern const UINT8 AE_Win_Spot[AE_WIN_Y][AE_WIN_X];
extern const UINT8 AE_Win_Multi[AE_WIN_Y][AE_WIN_X];
extern const UINT8 AE_Win_Evaluative[AE_WIN_Y][AE_WIN_X];
extern const UINT8 AE_OverExpo_Win_Normal[AE_WIN_Y][AE_WIN_X];

extern const UINT32 AeCurve_HDR16X[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_HDR4X[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_HDR1X[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_AntiFlicker60[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_AntiFlicker60_60fps[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_AntiFlicker60_120fps[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_AntiFlicker60_240fps[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_AntiFlicker50[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_AntiFlicker50_60fps[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_AntiFlicker50_120fps[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_AntiFlicker50_240fps[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO50_60Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO50_50Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO80_60Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO80_50Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO100_60Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO100_50Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO200_60Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO200_50Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO400_60Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO400_50Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO800_60Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO800_50Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO1600_60Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO1600_50Hz[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_Landscape[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_NightLandscape[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_Action[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_Potrait[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_NightPotrait[][AEALG_INFO_MAX];

extern const UINT32 AeCurve_ISO25_NOR[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO50_NOR[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO100_NOR[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO200_NOR[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO400_NOR[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO800_NOR[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO1600_NOR[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO3200_NOR[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO6400_NOR[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_ISO12800_NOR[][AEALG_INFO_MAX];

extern const UINT32 AeCurve_Cap50[][AEALG_INFO_MAX];
extern const UINT32 AeCurve_Cap60[][AEALG_INFO_MAX];

/**

     get IPL_ID_1 Raw value by input Y value

     @note

     @param[in] 12 Bit Raw lum
     @param[out] 10 Bit Y lum
*/
extern UINT32 AE_GetGammaValue_ID_1(UINT32 Data12Bit);

/**

     get IPL_ID_2 Raw value by input Y value

     @note

     @param[in] 12 Bit Raw lum
     @param[out] 10 Bit Y lum
*/
extern UINT32 AE_GetGammaValue_ID_2(UINT32 Data12Bit);

/**

     get IPL_ID_1 10 Bit Y value by input 12 Bit Raw value

     @note

     @param[in] 10 Bit Y lum
     @param[out] 12 Bit Raw lum
*/
extern UINT32 AE_GetInvGammaValue_ID_1(UINT32 Data10Bit);

/**

     get IPL_ID_2 10 Bit Y value by input 12 Bit Raw value

     @note

     @param[in] 10 Bit Y lum
     @param[out] 12 Bit Raw lum
*/
extern UINT32 AE_GetInvGammaValue_ID_2(UINT32 Data10Bit);

/**

     get LA Information from SIE

     @note

     @param[in] process id
     @param[out] Luminance 1 : Pre-GG/Post-CG Lum @12b
     @param[out] Luminance 2 : Post-Gamma Lum @10b
     @param[out] Histogram :   @12b Pre-Gamma/@10b Post-Gamma
*/
extern void AE_GetLA(UINT32 Id, UINT16 *Lum1, UINT16 *Lum2, UINT16 *Histo);

/**

     judge preview current and previous setting

     @note

     @param[in] new setting
     @param[in] old setting
     @param[out] update flag
     @return
         - @b TRUE: update setting.
*/
extern UINT32 AE_judgePrvSetting(AEALG_ARG *CurInfo, AEALG_ARG *PrevInfo, UINT32 *Flag);

/**

     get statisic information

     @note

     @param[in/out] AEINFO
     @param[in/out] AEBLKINFO
     @return
         - @b TRUE: current luminance.
*/
extern UINT32 AE_getStatisticInfo(AE_INFO *AeInfo, AE_BLK_INFO *AeBlkInfo);

/**

     ae initial informaiton

     @note

     @param[out] ae information
     @param[out] AEBLKINFO
*/
extern void AE_getInfo(AE_INFO *AeInfo, AE_BLK_INFO *AeBlkInfo);

/**

     set ae window

     @note
     @param[in] process id

     @param[out] ae information
*/
extern AE_RESET AE_setWin(UINT32 Id, AE_WIN mode);

/**

     get ae flag

     @note
     @param[in] process id

     @return
         - @b ae flag.
*/
extern UINT32 AE_getFlag(UINT32 Id);

/**

     set ae flag

     @note
     @param[in] process id
     @param[in] ae flag
*/
extern void AE_setFlag(UINT32 Id, UINT32 flag);

/**

     clear ae flag

     @note
     @param[in] process id
     @param[in] ae flag
*/
extern void AE_clrFlag(UINT32 Id, UINT32 flag);

/**

     ae reset flag
     @note
     @param[in] process id
*/
extern void AE_reset (UINT32 Id);

/**

     ae get capture curve and information
     @note

     @param[in] iso parameter
     @param[in] flicker parameter
     @param[in] scene parameter
     @param[out] ae information
*/
extern AE_RESET AE_getCapTab(AE_ISO nISO, AE_FLICKER nFlicker, AE_SCENE nScene, AE_INFO *AeInfo);

/**

     ae get preview curve and information
     @note

     @param[in] iso parameter
     @param[in] flicker parameter
     @param[out] ae information
*/
extern AE_RESET AE_getPrvTab(AE_ISO nISO, AE_FLICKER nFlicker, AE_INFO *AeInfo);

/**

     get over exposure ratio
     @note

     @param[in] histogram size
     @param[out] histogram
     @return
         - @b over exposure cnt.
*/
extern UINT32 GetOverExpoRatio(UINT32 HistMaxNum, UINT16 *Hist);

/**

     get fno ratio
     @note

     @param[in] iris index
     @return
         - @b fno ratio.
*/
extern UINT32 GetFnoRatio(AEALG_IRIS_IDX Idx);

/**

     scene map function
     @note

     @param[in] Scene index
     @return
         - @b Scene type.
*/
extern AE_SCENE AE_getSceneType(UINT32 Idx);

/**
     flicker map function
     @note

     @param[in] flicker index
     @return
         - @b flicker type.
*/
extern AE_FLICKER AE_getFlickerType(UINT32 Idx);

/**
     anti-shake map function
     @note

     @param[in] anti-shake index
     @return
         - @b anti-shake type.
*/
extern AE_ANTISHAKE AE_getAntiShakeLv(UINT32 Idx);

/**
     iso map function
     @note

     @param[in] iso index
     @return
         - @b iso type.
*/
extern AE_ISO AE_getIsoType(UINT32 Idx);

/**
     window map function
     @note

     @param[in] window index
     @return
         - @b window type.
*/
extern AE_WIN AE_getWinType(UINT32 Idx);

/**
     get preview sensor bining mode (binning ratio * 100)
     @note

     @param[in] process id
     @return
         - @b bining mode
*/
extern UINT32 AE_getPrvBiningMode(UINT32 Id);

/**
     get capture sensor bining mode (binning ratio * 100)
     @note

     @param[in] process id
     @return
         - @b bining mode
*/
extern UINT32 AE_getCapBiningMode(UINT32 Id);

/**
     ae get control information
     @note

     @param[in/out] AEINFO
     @param[in/out] AEBLKINFO
*/
extern void AE_getCtrlInfo(AE_INFO *AeInfo, AE_BLK_INFO *AeBlkInfo);

/**
     ae set exposure time
     @note
     @param[in] process id
     @param[in] exposure time
*/
extern void AE_setExpT(UINT32 Id, EXPOSURE_SETTING *ExpTSetting);

/**
     ae set iso gain
     @note
     @param[in] process id
     @param[in] iso gain
*/
extern void AE_setGain(UINT32 Id, GAIN_SETTING *Gain);

/**
     ae set iris
     @note
     @param[in] process id
     @param[in] iris index
*/
extern void AE_setIris(UINT32 Id, AEALG_IRIS_IDX Idx);

/**
     ae wait preview stable
     @note

     @param[in] process id
     @param[in] StableCnt : wait stable count
     @param[in] TimeOutFrame : time out frame
     @return
         - @b TRUE: ae already stable
         - @b FALSE: ae not stable and time out
*/
extern BOOL AE_Wait_Stable(UINT32 Id, UINT32 StableCnt, UINT32 TimeOutFrame);

/**
     ae set ir cut switch
     @note
     @param[in] process id
     @param[in] current lv
*/
extern void AE_SetIRCut(UINT32 Id, UINT32 lv);

/**
     ae judge iris step
     @note
     @param[in] AEINFO
     @param[out] Iris Step
*/
extern UINT32 AE_judgeIris(AE_INFO *AeInfo);

extern AE_RESET AE_getPrvManualTab(UINT32 ManualFlag, UINT32 nISO, AE_INFO *AeInfo);
extern AE_RESET AE_getCapManualTab(UINT32 ManualFlag, UINT32 nISO, AE_INFO *AeInfo);

extern UINT32 AE_Get_IrcutInfo(void* data);

extern UINT32 *AE_Table_Ptr[AE_TABLE_MAX];

#endif //_AE_SAMPLE_INT_H_

