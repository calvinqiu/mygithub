/**
    Auto white balance.


    @file       awb_AR0330P_IPCAM_int.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _AWB_AR0330P_IPCAM_FF_INT_H_
#define _AWB_AR0330P_IPCAM_FF_INT_H_

/**
     @name AE default value
     @note
*/
//@{

#define AWB_PROC_CNT 2
#define AWB_CAPRAW_EMB_DEBUG_INFO 0

//#define AWB_RATE            4          ///< awb rate
#define AWB_SKIP_FRAME      1          ///< awb skip garbage frame
#define AWB_WEIGHT_SKIP_FRAME      2          ///< awb skip weight previos frame

#define AWB_DEF_KRGAIN     100
#define AWB_DEF_KBGAIN     100

                            //4000K
#define AWB_DEF_RGAIN       550 //402
#define AWB_DEF_GGAIN       256
#define AWB_DEF_BGAIN       458 //422

#define AWB_W_THRES         100

#define AWB_WHITE_TAB_CNT 2
#define AWB_GREEN_TAB_CNT 0
//#define AWB_PDL_TAB_CNT 1

#define AWB_MAX_WHITE_TAB 5
//#define AWB_MAX_PDL_TAB 5


#define AWB_REFWHITE_REMAP 0

#define FACE_COLOR_R 545
#define FACE_COLOR_G 460
#define FACE_COLOR_B 420
#define AWB_FACEIMG_WIDTH     640
#define AWB_FACEIMG_HEIGHT    480

//@}


/**
     Awb mode

     @note
*/
typedef enum _AWB_MODE
{
    AWB_MODE_AUTO = 0,
    AWB_MODE_DAYLIGHT,
    AWB_MODE_CLOUDY,
    AWB_MODE_TUNGSTEN,
    AWB_MODE_FLUORESCENT1,
    AWB_MODE_FLUORESCENT2,
    AWB_MODE_SUNSET,
    AWB_MODE_BIRTHDAYPARTY,
    AWB_MODE_CUSTOMER1,
    AWB_MODE_CUSTOMER2,
    AWB_MODE_CUSTOMER3,
    AWB_MODE_MGAIN,
    AWB_MODE_MAX,
    ENUM_DUMMY4WORD(AWB_MODE)
}AWB_MODE;

typedef struct _AWB_IRCUT_INFO
{
    UINT32 Id;
    UINT32 color_avg[3];
    UINT32 color_gain[3];
} AWB_IRCUT_INFO;


extern AWB_CTINFO AwbCTTable[AWB_PROC_CNT][AWB_CT_RANGE_MAX];
extern AWB_CAINFO AwbPreWhiteElement[AWB_PROC_CNT];
extern UINT32 MwbTAB[AWB_PROC_CNT][AWB_MODE_MAX][3];
extern AWBALG_TAB AwbWhiteTAB[AWB_PROC_CNT][AWB_MAX_WHITE_TAB];
extern AWBALG_CT_PARAM AwbCTParam[AWB_PROC_CNT];
extern AWBALG_POSTPROC_PARAM AwbPostParam[AWB_PROC_CNT];

#if 0
/**

     set color gain

     @note

     @param[in] R gain
     @param[in] G gain
     @param[in] B gain
*/
static void AWB_setGain(AWBALG_INFO *Info, UINT32 RGain, UINT32 GGain, UINT32 BGain);
/**
     get ca value

     @note

     @param[out] R acc value
     @param[out] G acc value
     @param[out] B acc value
     @param[in] window size

*/
static void AWB_getCA(UINT32 id, UINT16 *R, UINT16 *G, UINT16 *B, UINT32 Size);


/**
     awb mode map function
     @note

     @param[in] awb mode index
     @return
         - @b awb mode.
*/
static AWB_MODE AWB_getMode(UINT32 Idx);


/**
     awb auto process
     @note
*/
static void AWB_autoProc(AWBALG_INFO *Info);
//static UINT32 AWB_faceProc(AWBALG_INFO *Info, UINT32 *RGain, UINT32 *GGain, UINT32 *BGain, UINT32 wImgWidth, UINT32 wImgHeight);
/**
     awb manual process
     @note

     @param[in] awb mode index
*/
static void AWB_manualProc(AWBALG_INFO *Info, AWB_MODE Idx);
#endif

#if 0
/**
     awb get preview color gain
     @note

     @param[out] R gain
     @param[out] G gain
     @param[out] B gain
*/
extern void AWB_GetPrvGain(UINT32 id, UINT32 *Rgain, UINT32 *Ggain, UINT32 *Bgain);
#endif

extern UINT32 AWB_Get_IrcutInfo(void* data);
#endif //_AWB_SAMPLE_INT_H_
