/**
    Auto Exposure Algorithm.

    depend on current Luminance to calculate LV, ISO, Exposure Time, and Iris.

    @file       ae_alg.h
    @ingroup    mILibAEAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AE_ALG_H_
#define _AE_ALG_H_


/**
     @name AE Alg. default value
     @note
*/
//@{
#define AEALG_TAB_END                   0xffffffff      ///< AE table end flag
#define AEALG_BUF_SIZE                  8
#define AEALG_PREDICT_POINT_MAX         3
#define AEALG_DYNAMIC_EXPECT_LUM_MAX    21
#define AE_BLK_WIN_NUM                  6               //divide image to 6 block
#define AE_GEN_CURVE_NODE_MAX           40
//@}

/**
     AEALG error code.

     @note
*/
typedef enum _AEALG_ER
{
    AEALG_OK = 0,         ///< AE OK
    AEALG_PAR,            ///< AE Parameter error
    AEALG_MAX,
    ENUM_DUMMY4WORD(AEALG_ER)
}AEALG_ER;

/**
     Iris index.

     big iris -> small iris
     AEALG_IRIS_0 -> AEALG_IRIS_1 -> ....... -> AEALG_IRIS_8
     @note
*/
typedef enum _AEALG_IRIS_IDX
{
    AEALG_IRIS_BIG = 0,                 ///< iris index big
    AEALG_IRIS_0 = 0,                   ///< iris index 0
    AEALG_IRIS_1,                       ///< iris index 1
    AEALG_IRIS_2,                       ///< iris index 2
    AEALG_IRIS_3,                       ///< iris index 3
    AEALG_IRIS_4,                       ///< iris index 4
    AEALG_IRIS_5,                       ///< iris index 5
    AEALG_IRIS_6,                       ///< iris index 6
    AEALG_IRIS_7,                       ///< iris index 7
    AEALG_IRIS_8,                       ///< iris index 8
    AEALG_IRIS_SMALL = AEALG_IRIS_8,    ///< iris index small
    AEALG_IRIS_MAX,
    ENUM_DUMMY4WORD(AEALG_IRIS_IDX)
}AEALG_IRIS_IDX;


/**
     AE table information.

     AE table information.
     @note
*/
typedef enum _AEALG_TABLE_INFO
{
    AEALG_EV = 0,          ///< EV value
    AEALG_EXP,             ///< Exposure time
    AEALG_ISO,             ///< ISO value
    AEALG_IRIS,            ///< IRIS value
    AEALG_ADJ_TARGET,      ///< adjust flag
    AEALG_INFO_MAX,
    ENUM_DUMMY4WORD(AEALG_TABLE_INFO)
} AEALG_TABLE_INFO;

/**
     @name Ae adjust flag(AEALG_ADJ_TARGET)
     @note
*/
//@{
#define AEALG_ADJ_EXP           0x01    ///< Adjust exposure time
#define AEALG_ADJ_GAIN          0x02    ///< Adjust iso
#define AEALG_ADJ_IRIS          0x04    ///< Adjust iris idx
//@}

/**
     AEALG converge speed.

     @note
*/
typedef enum _AEALG_SPEED
{
    AEALG_SPEED_N3 = 0,         ///< speed -3
    AEALG_SPEED_N2,             ///< speed -2
    AEALG_SPEED_N1,             ///< speed -1
    AEALG_SPEED_NORMAL,         ///< normal
    AEALG_SPEED_P1,             ///< speed +1
    AEALG_SPEED_P2,             ///< speed +2
    AEALG_SPEED_P3,             ///< speed +3
    AEALG_SPEED_MAX,            ///< max number
    ENUM_DUMMY4WORD(AEALG_SPEED)
}AEALG_SPEED;

/**
     AE argument.

     AE argument.
     @note
*/
typedef struct _AEALG_ARG
{
    UINT32 EVValue;         ///< evvalue
    UINT32 ExpoTime;        ///< Exposure Time(us)
    UINT32 ISOGain;         ///< ISO value(50, 100, 200......)
    UINT32 Iris;            ///< Iris Value(idx)
}AEALG_ARG;

/**
     AE luminance
     @note
*/
typedef struct _AE_LUMINANCE
{
    UINT32 raw;     //raw luminance
    UINT32 y;       //yuv luminance
} AE_LUMINANCE;

/**
     AE boundary
     @note
*/
typedef struct _AE_BOUNDARY
{
    UINT32 h;       //high
    UINT32 l;       //low
} AE_BOUNDARY;

/**
     AE histogram
     @note
*/
typedef struct _AE_HISTOGRAM
{
    UINT32 bin_num; //total bin number
    UINT32 data_num; //total data number;
    UINT16 *y;      //yuv histogram pointer
} AE_HISTOGRAM;

/**
     AE weight item
     @note
*/
typedef struct _AE_WEIGHT
{
    UINT32 ratio_bound;  //ratio boundary
    UINT32 exp_w;     //expect Y ratio
} AE_WEIGHT;

/**
     AE converage table
     @note
*/
typedef struct _AE_CONV_TAB
{
    UINT32 thres_bound; // current average Y > thres_bound, using raw else yuv
    UINT32 weight_tab_num;
    AE_WEIGHT *weight_tab;
} AE_CONV_TAB;


typedef struct _AE_ENV_VALUE
{
    UINT32 LV;         // lv
    UINT32 EVValue;    // ev value
} AE_ENV_VALUE;

typedef enum _AE_PLINEMODE
{
    AE_PLINE_PREDEF = 0,
    AE_PLINE_MANUAL,
    AE_PLINE_AUTOGEN,
    AE_PLINE_END = 0xffffffff,
    ENUM_DUMMY4WORD(AE_PLINEMODE)
}AE_PLINEMODE;

typedef enum {
    AE_IRIS_TYPE_FIXED,
    AE_IRIS_TYPE_DC,
    AE_IRIS_TYPE_P,
    AE_IRIS_TYPE_MAX,
} AEIrisType;

typedef enum {
    AE_IRIS_MODE_AUTO,      // AE Control
    AE_IRIS_MODE_CLOSE,     // Minimum Iris
    AE_IRIS_MODE_OPEN,      // Maximum Iris
    AE_IRIS_MODE_MANUAL,    // P-Iris only
    AE_IRIS_MODE_MAX,
} AEIrisMode;

typedef enum {
    AE_STRATEGY_HIGHLIGHT,
    AE_STRATEGY_LOWLIGHT,
    AE_STRATEGY_MAX,
} AEStrategy;

/**
     AE algorithm event.
     @note
*/

typedef UINT32 (*GET_OVER_EXPO_RATIO)(UINT32 HistMaxNum, UINT16 *Hist);
typedef UINT32 (*GET_FNO_RATIO)(AEALG_IRIS_IDX Idx);
typedef UINT32 (*GET_CURVE_OFS_RATIO)(void);
//typedef UINT32 (*USER_JUDGECONDITION)(AE_LUMINANCE *ExpectLum, AE_LUMINANCE *CurLum);
typedef struct _AEALG_EVENT_TAB
{
    //AEAlg_getratio event
    GET_OVER_EXPO_RATIO GetOverExpoRatio;

    //AEAlg_Allot event(Fno x Fno x 10)
    GET_FNO_RATIO GetFnoRatio;

    //AEAlg_Allot event((FnoA^2 / FnoB^2) x 100)
    //FnoA = (Wide, big iris)
    //FnoB = (Current Section, big iris)
    GET_CURVE_OFS_RATIO GetCurveOfsRatio;
}AEALG_EVENT_TAB;

/**
     AE sub-settings
     @note
*/

typedef struct _AE_OVEREXP_INFO
{
    AE_LUMINANCE AdjLum;                 ///< adjust luminace
    UINT32 CurCnt;
    UINT32 CurTotalStep;                 ///< current adjust total step
    UINT32 CurMaxStep;                   ///< current adjust Max step
    UINT32 CurMaxCnt;
    UINT32 CurMinCnt;
    UINT32 CurThreshold;
    UINT32 *AdjLumThresholdTab;          ///< over exposure luminance threshold table (LV0 ~ LV20, max index 21), AdjLum * x%
    UINT32 *AdjLumTab;                   ///< over exposure adjust luminance table (LV0 ~ LV20, max index 21, current expected Lum - adj lum)
    UINT32 *AdjMaxCntRatioTab;           ///< over exposure luminance counter table (LV0 ~ LV20, max index 21) (x / 1000)
    UINT32 *AdjMinCntRatioTab;           ///< over exposure luminance counter table (LV0 ~ LV20, max index 21) (x / 1000)
    UINT32 UserDefRatio;                 ///< if TRUE, CurCnt = GET_OVER_EXPO_RATIO()(user can set any value pass through callback function)
} AE_OVEREXP_INFO;

/**
     AE sub-settings
     @note
*/

typedef struct _AE_ADJHISTO_INFO
{
    AE_LUMINANCE AdjLum;                 ///< (adjust luminance)
    AE_LUMINANCE Lum;
    UINT32 *RatioTab;
    UINT32 AvgL;
    UINT32 AvgLCnt;
    UINT32 AvgH;
    UINT32 AvgHCnt;
} AE_ADJHISTO_INFO;


typedef UINT32 (*AEALG_ADD_GAMMA)(UINT32);
typedef UINT32 (*AEALG_INV_GAMMA)(UINT32);
typedef struct _AE_EXT_SET
{
    UINT32 *PrvLumRatioTab;                             ///< dynamic expected luminance table (LV0 ~ LV20, max index 21, expectLum x ratio / 100)
    AE_LUMINANCE DyLum;
    UINT32 ExpLumDiv;                                   ///< expect Luminance adjust div, DyLum/Div, for shdr using

    UINT32 AntiOverExpEn;                                  ///< anti overexposure switch
    AE_OVEREXP_INFO OverExp;                            ///< sub function for reducing over exposure condition

    UINT32 AdjHistoEn;                                  ///< adjust histogram switch
    AE_ADJHISTO_INFO AdjHistoInfo;                      ///< adjust histogram range parameter

    UINT32 AdjCapEn;
    AE_LUMINANCE CapExpectLum;                          ///< expected luminance
    UINT32 *CapExpectTab;

    UINT32 UnStableDelay;                               ///< reconvergence delay counter
} AE_EXT_SET;

/**
     AE Information.

     AE Information.
     @note
*/
typedef struct _AE_INFO
{
    UINT32 Id;
    AE_LUMINANCE PrvExpectLum;                      ///< standard expected luminance
    AE_LUMINANCE PrvAvgLum;                         ///< standard average luminance
    AE_ENV_VALUE PrvAvgEV;                          ///< standard ev value for average luminance
    AE_BOUNDARY EV;                                 ///< max, min ev
    AE_BOUNDARY ISO;                                ///< Preview max, min iso
    AE_BOUNDARY CapISO;                             ///< Capture max, min iso
    AE_BOUNDARY StableRange;                        ///< stable tolerance (100 +H -L) %
    AE_BOUNDARY StableRangeRF;                      ///< stable tolerance (100 +H -L) %
    AE_HISTOGRAM histogram;                         ///< histogram information
    UINT32(* PrvCurve)[AEALG_INFO_MAX];             ///< preview Ae curve
    UINT32(* CapCurve)[AEALG_INFO_MAX];             ///< capture Ae curve
    UINT32 FirstRunFlag;                            ///< ae first run flag
    UINT32 Reset;                                   ///< ae reset flag
    UINT32 Counter;                                 ///< ae stable counter
    UINT32 CounterRF;                               ///< ae stable counter
    AE_LUMINANCE CurExpectLum;                      ///< current expected luminance
    AE_LUMINANCE CurLum;                            ///< current luminance
    UINT32 CurConvDir;                              ///< preview converage direction
    AE_ENV_VALUE PrvParam[AEALG_PREDICT_POINT_MAX]; ///< preview parameter
    AE_ENV_VALUE CapParam;                          ///< capture parameter
    AEALG_EVENT_TAB EventTab;
    AEALG_SPEED ConvSpeed;                          ///< converage speed
    AE_CONV_TAB *ConvTab;                           ///< manual converge table
    AEALG_INV_GAMMA InvGammaFunc;                   ///< inverse gamma function
    AEALG_ADD_GAMMA AddGammaFunc;                   ///< additive gamma function
    UINT32 IsRawLum;
    UINT32 EVCompRatio;                             ///< ev comp ratio (raw * ratio /100)
    UINT32 PrvBiningMode;                           ///< bining mode 1x, 2x, 3x, 4x......(x 100)
    UINT32 CapBiningMode;                           ///< bining mode 1x, 2x, 3x, 4x......(x 100)
    AE_EXT_SET ExtSet;                              ///< extend parameter for AE control
    UINT32 Rev[4];                                  ///< for debug do not access
} AE_INFO;
/**
     AE Block Y information

     @note
*/
typedef struct _AE_BLK_INFO
{
    UINT32 BLKLum_En;                   ///Block Luminance calculate function switch
    UINT32 LV;                          ///current LV value
    UINT32 FirstRunFlg;                 ///< BLK first run flag
    UINT16 *Yacc;                       ///Y acc
    UINT16 *Gacc;                       ///G acc
    UINT32 LA_Num_X;                    ///LA window X number
    UINT32 LA_Num_Y;                    ///LA window Y number
    AE_BOUNDARY Raw_OverBound_Lum;      ///raw over  exposure luminance boundary (up/low bound)
    AE_BOUNDARY Raw_UnderBound_Lum;     ///raw under exposure luminance boundary (up/low bound)
    UINT32 PrvBlk_Num[AE_BLK_WIN_NUM];  ///previous luminance
    AEALG_ADD_GAMMA AddGammaFunc;       ///< additive gamma function
    UINT32 Rev[5];                      ///< for debug do not access
} AE_BLK_INFO;

typedef struct _AE_PARAMINFO
{
    UINT32 InitEV;                  ///< Initial EV
    AE_BOUNDARY EV;                 ///< Maximum/Minimum EV
    AE_BOUNDARY PrvISO;             ///< Maximum/Minimum ISO for streaming mode
    AE_BOUNDARY CapISO;             ///< Maximum/Minimum ISO for capture mode
    AE_BOUNDARY AFD_Lum;            ///< Auto anti Flicker Parameter
    UINT32 ExpLum_Mov;              ///< @10b post-gamma Lum for Video/Recording mode
    UINT32 ExpLum_Photo;            ///< @10b post-gamma Lum for Photo mode
    UINT32 ExpLum_Histo;            ///< @10b post-gamma Lum for Histogram
    UINT32 ExpLum_OverExp;          ///< @10b post-gamma Lum for Anti over exposure
    AE_BOUNDARY Blk_OVER_Y_Thr;     ///< @10b post-gamma over exposure upbound/lowbound threshold
    AE_BOUNDARY Blk_UNDER_Y_Thr;    ///< @10b post-gamma under exposure upbound/lowbound threshold
} AE_PARAMINFO;

typedef struct {
    AEIrisType IrisType;
    AEIrisMode IrisMode;
    UINT32 DCIrisSpeed;
    UINT32 PIrisManualSize;     // P-Iris only
    AE_BOUNDARY PIris;          // P-Iris only
} AE_IRISINFO;

typedef struct _AE_SET_PARAMINFO
{
    AE_PLINEMODE PLineMode;
    AE_BOUNDARY ExpT;
    AE_BOUNDARY ISO;
    AE_IRISINFO IrisInfo;
    AE_BOUNDARY ConvRange;
    UINT32 ConvSpeed;
    UINT32 DelayCnt;
    UINT32 AntiOverExpEn;
} AE_SET_PARAMINFO;

typedef struct _AE_EXT_SET_INFO
{
    AE_PLINEMODE PLineMode;
    AE_BOUNDARY ExpTBound;            ///< exp time(us) boundary
    AE_BOUNDARY IsoBound;             ///< iso boundary
    UINT32 ExpT;
    UINT32 ISO;
    UINT32 ConvergenceSpeed;
    AE_IRISINFO IrisSetting;
    UINT32 AETolerance;
    UINT32 AEDelayCnt;
    UINT8 *ExpAreaWeight;
    AEStrategy AEStrategy; /* lowlight/highlight first */
} AE_EXT_SET_INFO;

/**

     AEAlg_allot function

     depend on EV value to calculate iso, exposure time, and iris.

     @note

     @param[in] EVValue    Current EVValue.
     @param[in] SenBinning Sensor binning ratio.
     @param[in] AECurve    AE Curve.
     @param[in] GET_FNO_RATIO callback function fp.
     @param[in] GET_CURVE_OFS_RATIO callback function fp.
     @param[out] Info    iso, exposure time, and iris.
     @return
         - @b AEALG_OK:   done with no error.
         - Others: Error occured.
*/
extern AEALG_ER AEAlg_Allot(UINT32 EVValue, UINT32 SenBinning, UINT32 (*AeCurve)[AEALG_INFO_MAX], GET_FNO_RATIO GetFnoCB, GET_CURVE_OFS_RATIO GetCurveOfsCB, AEALG_ARG *Info);

/**

     converage luminance

     @note

     @param[in] AE_INFO(AEInfo)
     @param[in] UINT32
     @param[out] preview iso, exposure time, iris
     @param[out] capture iso, exposure time, iris
     @return
         - @b void.
*/
extern void AEAlg_Converge(AE_INFO *AEInfo, UINT32 PointNum, AEALG_ARG *PrvInfo, AEALG_ARG *CapInfo);

/**

     convert ev value to lv value

     @note

     @param[in] ev vaule
     @return
         - @b lv value(x10).
*/
extern UINT32 AEAlg_Conv2LV(UINT32 EVValue);

/**

     calculate block based luminance

     @note

     @param[in] AE_BLK_INFO(AeBlkInfo)
     @param[in] Y and Raw CA, CA window number, LV
     @return
         - @b raw and y luminance
*/
extern AE_LUMINANCE AEAlg_GetBLKBasedLum(AE_BLK_INFO *AeBlkInfo);
extern UINT32 AE_BlkWriteDebugInfo(UINT32 Addr);
/**

     mdsc cb for debug use

     @note

     @return
         - @b bool.
*/
extern BOOL AEAlg_MsdcCB(void);



typedef enum _AE_GEN_IDX
{
    AE_GEN_IDX_MIN = 0,
    AE_GEN_IDX_MAX,
    AE_GEN_IDX_NUM,
    ENUM_DUMMY4WORD(AE_GEN_IDX)
}AE_GEN_IDX;

typedef enum _AE_GEN_FREQ
{
    AE_GEN_FREQ_NONE = 0,
    AE_GEN_FREQ_27_5,
    AE_GEN_FREQ_50,
    AE_GEN_FREQ_60,
    AE_GEN_FREQ_NUM,
    ENUM_DUMMY4WORD(AE_GEN_FREQ)
}AE_GEN_FREQ;

typedef struct _AE_GEN_INFO
{
    UINT32 FnoRat;                          ///< fno * fno * 10
    UINT32 FnoIdx;                          ///< fno Idx
    UINT32 ExpT[AE_GEN_IDX_NUM];            ///< exp time(us), 0: min, 1:max
    UINT32 Iso[AE_GEN_IDX_NUM];             ///< iso, 0: min, 1:max
    AE_GEN_FREQ Freq;                       ///< anti flicker frequence 50 or 60
    UINT32 FreqNode;                        ///< anti flicker node 50Hz = x/100 => bit0 = x, bit1 = 1/100, bit2 = 2/100, bit3 = 3/100......
                                            ///<                   60Hz = x/120 => bit0 = x, bit1 = 1/120, bit2 = 2/120, bit3 = 3/120......
                                            ///< anti flicker node max 8 (1~8)
    UINT32 Node1stExpT;                     ///< exp time(us): ExpT[AE_GEN_IDX_MIN] <= Node1stExpT <= ExpT[AE_GEN_IDX_MAX]
    UINT32 Node2ndISO;                      ///< iso: Iso[AE_GEN_IDX_MIN] <= Node2ndISO <= Iso[AE_GEN_IDX_MAX]
} AE_GEN_INFO;

ER AEAlg_GenCurve(AE_GEN_INFO *Info, UINT32 *CurveAdr, UINT32 *MaxEV, UINT32 *MinEV);




/*AE Tuning tool *************************************************************/
#if 0
#endif
typedef enum _AE_SET_ITEM
{
    AE_SET_EXPECTLUM        =   0,
    AE_SET_HISTOGRAMADJ,
    AE_SET_OVEREXP,
    AE_SET_BOUNDARY,
    AE_SET_CONVERGE,
    AE_SET_MANUAL,
    AE_SET_CURVEGEN,
    AE_SET_ALL,
    AE_SET_MAX
} AE_SET_ITEM;

typedef enum _AE_GET_ITEM
{
    AE_GET_EXPECTLUM        =   0,
    AE_GET_HISTOGRAMADJ,
    AE_GET_OVEREXP,
    AE_GET_BOUNDARY,
    AE_GET_CONVERGE,
    AE_GET_MANUAL,
    AE_GET_STATUS,
    AE_GET_CURVEGEN,
    AE_GET_ALL,
    AE_GET_MAX
} AE_GET_ITEM;

typedef enum _AE_TUNING_UPDATE
{
    AE_UPDATE_EXPECTLUM     = FLGPTN_BIT(AE_SET_EXPECTLUM),
    AE_UPDATE_HISTOGRAMADJ  = FLGPTN_BIT(AE_SET_HISTOGRAMADJ),
    AE_UPDATE_OVEREXP       = FLGPTN_BIT(AE_SET_OVEREXP),
    AE_UPDATE_BOUNDARY      = FLGPTN_BIT(AE_SET_BOUNDARY),
    AE_UPDATE_CURVEGEN      = FLGPTN_BIT(AE_SET_CURVEGEN),
    AE_UPDATE_CONVERGE      = FLGPTN_BIT(AE_SET_CONVERGE),
    AE_UPDATE_MANUAL        = FLGPTN_BIT(AE_SET_MANUAL),
    AE_UPDATE_ALL           = FLGPTN_BIT_ALL
} AE_TUNING_UPDATE;

typedef enum _AE_TABLE_TYPE
{
    AE_TABLE_PRVEXPLUM          =   0,
    AE_TABLE_PRVEXPLUM_IROFF,
    AE_TABLE_HISTOLUM,
    AE_TABLE_OVEREXP,
    AE_TABLE_OVEREXP_ADJ,
    AE_TABLE_OVEREXP_ADJ_IROFF,
    AE_TABLE_MAX
} AE_TABLE_TYPE;

typedef enum _AE_FUNC_MODE
{
    AE_FUNC_DISABLE  = 0,
    AE_FUNC_ENABLE,
    AE_FUNC_AUTO,
    AE_FUNC_MAX
} AE_FUNC_MODE;

typedef struct _AE_TUNING_EXPECTLUM
{
    UINT32 lum;
    UINT32 tab[AEALG_DYNAMIC_EXPECT_LUM_MAX];
    UINT32 tab_iroff[AEALG_DYNAMIC_EXPECT_LUM_MAX];
} AE_TUNING_EXPECTLUM;

typedef struct _AE_TUNING_HISTO
{
    AE_FUNC_MODE histoMode;
    UINT32 lum;
    UINT32 tab[AEALG_DYNAMIC_EXPECT_LUM_MAX];
} AE_TUNING_HISTO;

typedef struct _AE_TUNING_OVEREXP
{
    UINT32 overexpEn;
    UINT32 lum;
    UINT32 tab[AEALG_DYNAMIC_EXPECT_LUM_MAX];
    UINT32 tab_adj[AEALG_DYNAMIC_EXPECT_LUM_MAX];
	UINT32 tab_adj_iroff[AEALG_DYNAMIC_EXPECT_LUM_MAX];

    //UINT32 *tab_maxcnt;
    //UINT32 *tab_mincnt;
} AE_TUNING_OVEREXP;

typedef struct _AE_TUNING_BOUNDARY
{
    AE_BOUNDARY ev;
    AE_BOUNDARY prvISO;
} AE_TUNING_BOUNDARY;

typedef struct _AE_TUNING_CONVERGE
{
    AE_BOUNDARY convRange;
    UINT32 convSpeed;
} AE_TUNING_CONVERGE;

typedef struct _AE_TUNING_STATUS
{
    UINT32 curLum;
    UINT32 curEv;
    UINT32 curISO;
    UINT32 curExpoTime;
    UINT32 curExpectLum;
} AE_TUNING_STATUS;

typedef struct _AE_TUNING_MANUAL
{
    UINT32 manualEn;
    UINT32 manualFixISOEn;
    UINT32 manualExpT;
    UINT32 manualISO;
} AE_TUNING_MANUAL;

typedef struct _AE_TUNING_CURVEGEN
{
    BOOL enable;
    AE_BOUNDARY iso;
    AE_BOUNDARY expt;
} AE_TUNING_CURVEGEN;

typedef struct _AE_TUNING_ALL
{
    AE_TUNING_EXPECTLUM expectLum;
    AE_TUNING_OVEREXP   overExposure;
    AE_TUNING_HISTO     adjHistrogram;
    AE_TUNING_BOUNDARY  boundary;
    AE_TUNING_CURVEGEN  curvegen;
    AE_TUNING_CONVERGE  converge;
    AE_TUNING_MANUAL    manual;

    // READ ONLY
    AE_TUNING_STATUS    status;
} AE_TUNING_ALL;


extern UINT32 AETuning_Set_Cmd(UINT32 Id, AE_SET_ITEM item, UINT32 addr, UINT32 size);
extern UINT32 AETuning_Get_Cmd(UINT32 Id, AE_GET_ITEM item, UINT32 addr, UINT32 size);

/* Internel used function for ae_ext */
void AETuning_setInit(UINT32 Id, UINT32 addr, UINT32 size);
void AETuning_getPtr(UINT32 Id, UINT32 addr);
void AETuning_getUpdate(UINT32 Id, UINT32 addr);
void AETuning_setUpdate(UINT32 Id, UINT32 up);

#define AE_PROC_CNT                 2                           ///< NT96660 support maximum 2 AE Process

extern UINT32 uiISOGain[AE_PROC_CNT], uiExpoTime[AE_PROC_CNT]; //, uiIris[AE_ID_MAX_NUM];
extern UINT32 uiAEManualCmd[AE_PROC_CNT], uiAEManualISOCmd[AE_PROC_CNT];
extern UINT32 AE_stableLum[AE_PROC_CNT];
extern UINT32 AE_stableEV[AE_PROC_CNT];

/* AET UART MSG */
#define AE_UART_DUMP_VAR(var) (DBG_DUMP("AET %s = %d\r\n", #var, var))
#define AE_UART_DUMP_ARR(arr, len)      \
    do{                                 \
        UINT32 i;                       \
        DBG_DUMP("%s = { ", #arr);      \
        for(i=0; i<len; i++)            \
            DBG_DUMP("%d, ", arr[i]);   \
        DBG_DUMP("}\r\n");              \
    }while(0)

/******************************************************************************/
#endif //_AE_ALG_H_
