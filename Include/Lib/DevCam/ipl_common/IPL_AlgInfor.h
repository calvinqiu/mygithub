/**
    Public header file for image setting information

    This file is the header file that define the API and data type for image information.

    @file       IPL_AlgInfor.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IPL_ALGINFOR_H_
#define _IPL_ALGINFOR_H_

#include "Type.h"
#include "IPL_Utility.h"
#include "ErrorNo.h"

/** \addtogroup mIIPL */
//@{

#define IPL_INFOR_ERR   0xffffffff    ///< error code(IPL_AlgGetUIInfo())

#define SEL_NO_CHANG    0xF0


#define IPL_SEL_TAG   0x43210000

/**
    IPL informatin selection index(IPL_AlgSetUIInfo() or IPL_AlgGetUIInfo())
*/
typedef enum _IPL_SEL_ITEM
{
    IPL_SEL_DSCMODE = IPL_SEL_TAG,      ///<dsc mode, data type #SEL_DSCMODE
    IPL_SEL_SCENEMODE,                  ///<scene mode, data type #SEL_SCENEMODE
    IPL_SEL_SHARPNESS,                  ///<sharpness, data type #SEL_SHARPNESS
    IPL_SEL_SATURATION,                 ///<saturation, data type #SEL_SATURATION
    IPL_SEL_CONTRAST,                   ///<saturation, data type #SEL_CONTRAST
    IPL_SEL_FLASHMODE,                  ///<flash mode, data type #SEL_FLASHMODE
    IPL_SEL_FREQUENCY,                  ///<ae anti-frequency, data type #SEL_FREQUENCY
    IPL_SEL_IMAGEEFFECT,                ///<color effect, data type #SEL_IMAGEEFFECT
    IPL_SEL_AEMODE,                     ///<ae detect mode, data type #SEL_AEMODE
    IPL_SEL_AEEV,                       ///<ae ev compensation, data type #SEL_AEEV
    IPL_SEL_WBMODE,                     ///<wb mode, data type #SEL_WBMODE
    IPL_SEL_AFMODE,                     ///<af mode, data type #SEL_AFMODE
    IPL_SEL_AFWINDOW,                   ///<af window selection, data type #SEL_AFWINDOW
    IPL_SEL_AFPOSITION,                 ///<af position, data type, #SEL_AFPOSITION
    IPL_SEL_ISOINDEX,                   ///<ae iso index, data type #SEL_ISOINDEX

    IPL_SEL_RAW_BITDEPTH_PRV,           ///<prv mode raw bits, data type #SEL_RAW_BITDEPTH
    IPL_SEL_RAW_BITDEPTH_CAP,           ///<cap mode raw bits, data type #SEL_RAW_BITDEPTH

    IPL_SEL_IMAGEFLIP,                  ///<image flip mode, data type #SEL_IMAGEFLIP

    IPL_SEL_STAMP,                      ///<prv stamp mode lock or unlock, data type #SEL_STAMP
    IPL_SEL_IEMODE,                     ///<prv image effect. data type #SEL_IEMODE
    IPL_SEL_GDCCOMP,                    ///<prv GDC on/off, data type #SEL_GDCCOMP
    IPL_SEL_DISCOMP,                    ///<prv DIS on/off, data type #SEL_DISCOMP
    IPL_SEL_PHOTOFRAME,                 ///<prv photo frame on/off, data type #SEL_PHOTOFRAME

    IPL_SEL_PRVSENMODE,                 ///<prv sensor mode
    IPL_SEL_CAPSENMODE,                 ///<cap sensor mode
    IPL_SEL_COMMENT,                    ///<Scene mode comment
    IPL_SEL_VIDEO_FPS,                  ///<Video FPS, data type #SEL_VIDEOFPS
    IPL_SEL_VIDEO_SIZE,                 ///<Video SIZE, data type #SEL_VIDEOSIZE
    IPL_SEL_SUPERRESOL,                 ///<cap super resolution on/off data type #SEL_SUPERRESOL
    IPL_SEL_SHDR,                        ///<SHDR mode on/off, data type #SEL_SHDR
    IPL_SEL_WDR,                        ///<WDR mode on/off, data type #SEL_WDR
    IPL_SEL_WDR_LEVEL,                  ///<WDR Level, data type #SEL_WDR_LEVEL
    IPL_SEL_ANTISHAKE,                  ///<Anti Shake AE on/off, data type #SEL_ANTISHAKE
    IPL_SEL_FD,                         ///<Face detection on/off, data type #SEL_FD
    IPL_SEL_CCID,                       ///<image effect "selective color" index, data type #SEL_CCID
    IPL_SEL_IRCUT,                      ///<IR Cut on/off, data type #SEL_IRCUT
    IPL_SEL_CTRLFLOW,                   ///<IPL Flow select, data type #SEL_CTRLFLOW
    IPL_SEL_IPPNR,                      ///<IPL Flow select, data type #SEL_IPPNR
    IPL_SEL_IPPEDGE,                    ///<IPL Flow select, data type #SEL_IPPEDGE
    IPL_SEL_BLC,                        ///<BackLight Compensation mode on/off, data type #SEL_BLC
    IPL_SEL_BLC_LEVEL,                  ///<BackLight Compensation Level, data type #SEL_BLC_LEVEL
    IPL_SEL_HUE,                        ///<HUE, data type #SEL_HUE
    IPL_SEL_IPC_MODE,                 ///<USER GAMMA, data type #SEL_USER_GAMMA
    IPL_SEL_USER_GAMMA_MODE,
    IPL_SEL_USER_CC,                    ///<USER CC, Data type #SEL_USER_CC
    IPL_SEL_ANTI_FOG,
    IPL_SEL_ANTI_FOG_LEVEL,
//#NT#2016/03/04#Bowen Li -begin
//#NT# by project set alg. information
    IPL_USER_DEFINE1,
    IPL_USER_DEFINE2,
//#NT#2016/03/04#Bowen Li -end
//#NT#2017/02/06#Jarkko Chang -begin
//#NT# add support side by side left/right or up/down case
    IPL_SEL_SBS,                      ///<IPL Side by Side case select, data type #SEL_SBS
//#NT#2017/02/06#Jarkko Chang -end
    IPL_SEL_MAX_CNT,
    ENUM_DUMMY4WORD(IPL_SEL_ITEM)
} IPL_SEL_ITEM;

#define IPL_SEL_MAX_ITEM  (IPL_SEL_MAX_CNT - IPL_SEL_TAG)

/**
    dsc mode
*/
typedef enum _SEL_DSCMODE
{
    SEL_DSCMODE_AUTO = 0,           ///<-
    SEL_DSCMODE_MANUAL,             ///<-
    SEL_DSCMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_DSCMODE)
} SEL_DSCMODE;

/**
    scene mode
*/
typedef enum _SEL_SCENEMODE
{
    SEL_SCENEMODE_AUTO = 0,             ///<-
    SEL_SCENEMODE_LANDSCAPE,            ///<-
    SEL_SCENEMODE_POTRAIT,              ///<-
    SEL_SCENEMODE_SPORT,                ///<-
    SEL_SCENEMODE_NIGHTSCENE,           ///<-
    SEL_SCENEMODE_AV,                   ///<-
    SEL_SCENEMODE_TV,                   ///<-
    SEL_SCENEMODE_MANUAL,               ///<-
    SEL_SCENEMODE_NIGHTPOTRAIT,         ///<-
    SEL_SCENEMODE_NIGHTLANDSCAPE,       ///<-
    SEL_SCENEMODE_PARTY,                ///<-
    SEL_SCENEMODE_SUNSET,               ///<-
    SEL_SCENEMODE_BACKLIGHT,            ///<-
    SEL_SCENEMODE_DOCUMENT,             ///<-
    SEL_SCENEMODE_FIREWORK,             ///<-
    SEL_SCENEMODE_BEACH,                ///<-
    SEL_SCENEMODE_BMODE,                ///<-
    SEL_SCENEMODE_PROGRAM,              ///<-
    SEL_SCENEMODE_HIGHISO,              ///<-
    SEL_SCENEMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_SCENEMODE)
} SEL_SCENEMODE;


/**
    sharpness
*/
typedef enum _SEL_SHARPNESS
{
    SEL_SHARPNESS_N1      = 25,    ///< sharpness 25
    SEL_SHARPNESS_NORMAL  = 50,    ///< sharpness 50
    SEL_SHARPNESS_P1      = 75,    ///< sharpness 75
    SEL_SHARPNESS_MAX_CNT = 101,
    ENUM_DUMMY4WORD(SEL_SHARPNESS)
} SEL_SHARPNESS;

/**
    saturation
*/
typedef enum _SEL_SATURATION
{
    SEL_SATURATION_N1     = 25,    ///< saturation 25
    SEL_SATURATION_NORMAL = 50,    ///< saturation 50
    SEL_SATURATION_P1     = 75,    ///< saturation 75
    SEL_SATURATION_MAX_CNT= 101,
    ENUM_DUMMY4WORD(SEL_SATURATION)
} SEL_SATURATION;

/**
    contrast
*/
typedef enum _SEL_CONTRAST
{
    SEL_CONTRAST_N1       = 25,    ///< contrast 25
    SEL_CONTRAST_NORMAL   = 50,    ///< contrast 50
    SEL_CONTRAST_P1       = 75,    ///< contrast 75
    SEL_CONTRAST_MAX_CNT  = 101,
    ENUM_DUMMY4WORD(SEL_CONTRAST)
} SEL_CONTRAST;

/**
    HUE
*/
typedef enum _SEL_HUE
{
    SEL_HUE_0       = 0,    ///< HUE Normal
    SEL_HUE_MAX_CNT = 361,
    ENUM_DUMMY4WORD(SEL_HUE)
} SEL_HUE;

/**
    IPP NR on/off
*/
typedef enum _SEL_IPPNR
{
    SEL_IPPNR_OFF     = 0,
    SEL_IPPNR_ON,
    SEL_IPPNR_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_IPPNR)
} SEL_IPPNR;

/**
    GDC
*/
typedef enum _SEL_GDCCOMP
{
    SEL_GDCCOMP_OFF     = 0,
    SEL_GDCCOMP_ON,
    SEL_GDCCOMP_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_GDCCOMP)
} SEL_GDCCOMP;

/**
    flash mode
*/
typedef enum _SEL_FLASHMODE
{
    SEL_FLASHMODE_AUTO = 0,         ///< auto
    SEL_FLASHMODE_FORCEOFF,         ///< force off
    SEL_FLASHMODE_FORCEON,          ///< force on
    SEL_FLASHMODE_AUTOREDEYE,       ///< auto + red eye
    SEL_FLASHMODE_FORCEONREDEYE,    ///< force on + red eye
    SEL_FLASHMODE_NIGHTSTROBE,      ///< night strobe
    SEL_FLASHMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_FLASHMODE)
} SEL_FLASHMODE;

/**
    frequency
*/
typedef enum _SEL_FREQUENCY
{
    SEL_FREQUENCY_50HZ = 0,          ///< anti-freq 50Hz
    SEL_FREQUENCY_60HZ,              ///< anti-freq 60Hz
    SEL_FREQUENCY_AUTO,              ///< auto detect current freq
    SEL_FREQUENCY_NONE,
    SEL_FREQUENCY_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_FREQUENCY)
} SEL_FREQUENCY;

/**
    image effect: select color id
*/
typedef enum _SEL_CCID_
{
    SEL_CCID_NO_EFFECT = 0,
    SEL_CCID_ENHANCE_RED,
    SEL_CCID_ENHANCE_YELLOW,
    SEL_CCID_ENHANCE_GREEN,
    SEL_CCID_ENHANCE_BLUE,
    SEL_CCID_ENHANCE_CYAN,
    SEL_CCID_ENHANCE_ORANGE,
    SEL_CCID_ENHANCE_MAGENTA,
    SEL_CCID_REMOVAL_RED,
    SEL_CCID_REMOVAL_YELLOW,
    SEL_CCID_REMOVAL_GREEN,
    SEL_CCID_REMOVAL_BLUE,
    SEL_CCID_REMOVAL_CYAN,
    SEL_CCID_REMOVAL_ORANGE,
    SEL_CCID_REMOVAL_MAGENTA,
    SEL_CCID_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_CCID)
} SEL_CCID;

/**
    image effect
*/
typedef enum _SEL_IMAGEEFFECT
{
    SEL_IMAGEEFFECT_OFF = 0,            ///<-
    SEL_IMAGEEFFECT_BW,                 ///<-
    SEL_IMAGEEFFECT_SEPIA,              ///<-
    SEL_IMAGEEFFECT_VIVID,              ///<-
    SEL_IMAGEEFFECT_NEUTRAL,            ///<-
    SEL_IMAGEEFFECT_LOWSHARPING,        ///<-
    SEL_IMAGEEFFECT_ROCK,               ///<-
    SEL_IMAGEEFFECT_FASION2,            ///<-
    SEL_IMAGEEFFECT_CUSTOMMANUAL,       ///<-
    SEL_IMAGEEFFECT_RED,                ///<-
    SEL_IMAGEEFFECT_GREEN,              ///<- 10
    SEL_IMAGEEFFECT_BLUE,               ///<-
    SEL_IMAGEEFFECT_YELLOW,             ///<-
    SEL_IMAGEEFFECT_PURPLE,             ///<-
    SEL_IMAGEEFFECT_SKETCH,             ///<-
    SEL_IMAGEEFFECT_FISHEYE,            ///<-
    SEL_IMAGEEFFECT_COLORPENCIL,        ///<-
    SEL_IMAGEEFFECT_NEGATIVE_DEFOG,     ///<-
    SEL_IMAGEEFFECT_JAPANSTYLE,         ///<-
    SEL_IMAGEEFFECT_COOLGREEN,          ///<-
    SEL_IMAGEEFFECT_WARMYELLOW,         ///<-20
    SEL_IMAGEEFFECT_CCID,               ///<-
    SEL_IMAGEEFFECT_D2D_SHADING,
    SEL_IMAGEEFFECT_D2D_FANTASY,
    SEL_IMAGEEFFECT_D2D_FISHEYE,
    SEL_IMAGEEFFECT_D2D_MIRROR,
    SEL_IMAGEEFFECT_D2D_ROCK,
    SEL_IMAGEEFFECT_D2D_SKETCH,
    SEL_IMAGEEFFECT_D2D_MINIATURE,
    SEL_IMAGEEFFECT_D2D_SPARKLE,
    SEL_IMAGEEFFECT_GAMMA_Anti_Fog,
    SEL_IMAGEEFFECT_BLACKALL,

    SEL_IMAGEEFFECT_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_IMAGEEFFECT)
} SEL_IMAGEEFFECT;


/**
    Ae detect metrix
*/

typedef enum _SEL_AEMODE
{
    SEL_AEMODE_CENTERWEIGHTED = 0,      ///< center weighting
    SEL_AEMODE_SPOT,                    ///< spot
    SEL_AEMODE_MATRIX,                  ///< matic
    SEL_AEMODE_EVALUATIVE,              ///<-
    SEL_AEMODE_FACEDETECTION,           ///< face
    SEL_AEMODE_USERDEFINE,              ///< user define
    SEL_AEMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_AEMODE)
} SEL_AEMODE;

/**
    SEL_AEEV
*/
typedef enum _SEL_AEEV
{
    SEL_AEEV_N3 = 0,            ///< ev -3
    SEL_AEEV_N8_3,              ///< ev -8/3
    SEL_AEEV_N5_2,              ///< ev -5/2
    SEL_AEEV_N7_3,              ///< ev -7/3
    SEL_AEEV_N2,                ///< ev -2
    SEL_AEEV_N5_3,              ///< ev -5/3
    SEL_AEEV_N3_2,              ///< ev -3/2
    SEL_AEEV_N4_3,              ///< ev -4/3
    SEL_AEEV_N1,                ///< ev -1
    SEL_AEEV_N2_3,              ///< ev -2/3
    SEL_AEEV_N1_2,              ///< ev -1/2
    SEL_AEEV_N1_3,              ///< ev -1/3
    SEL_AEEV_00,                ///< ev +0
    SEL_AEEV_P1_3,              ///< ev +1/3
    SEL_AEEV_P1_2,              ///< ev +1/2
    SEL_AEEV_P2_3,              ///< ev +2/3
    SEL_AEEV_P1,                ///< ev +1
    SEL_AEEV_P4_3,              ///< ev +4/3
    SEL_AEEV_P3_2,              ///< ev +3/2
    SEL_AEEV_P5_3,              ///< ev +5/3
    SEL_AEEV_P2,                ///< ev +2
    SEL_AEEV_P7_3,              ///< ev +7/3
    SEL_AEEV_P5_2,              ///< ev +5/2
    SEL_AEEV_P8_3,              ///< ev +8/3
    SEL_AEEV_P3,                ///< ev +3
    SEL_AEEV_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_AEEV)
} SEL_AEEV;


/**
    white balance mode
*/
typedef enum _SEL_WBMODE
{
    SEL_WBMODE_AUTO = 0,        ///<auto mode
    SEL_WBMODE_DAYLIGHT,        ///<daylight mode
    SEL_WBMODE_CLOUDY,          ///<cloudy mode
    SEL_WBMODE_TUNGSTEN,        ///<tugsten mode
    SEL_WBMODE_FLUORESCENT1,    ///<FL1 mode
    SEL_WBMODE_FLUORESCENT2,    ///<FL2 mode
    SEL_WBMODE_SUNSET,          ///<sunset mode
    SEL_WBMODE_BIRTHDAYPARTY,   ///<party mode
    SEL_WBMODE_CUSTOMER1,       ///<-
    SEL_WBMODE_CUSTOMER2,       ///<-
    SEL_WBMODE_CUSTOMER3,       ///<-
    SEL_WBMODE_MGAIN,           ///<-manually R/G/B gain
    SEL_WBMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_WBMODE)
}SEL_WBMODE;

/**
    focus mode
*/
typedef enum _SEL_AFMODE
{
    SEL_AFMODE_AUTO = 0,        ///<auto focus
    SEL_AFMODE_MACRO,           ///<marco mode
    SEL_AFMODE_SUPER_MACRO,     ///<super marco mode
    SEL_AFMODE_INIFINITE,       ///<-
    SEL_AFMODE_CONTINUEFOCUS,   ///<-
    SEL_AFMODE_FACEDETECTION,   ///<-
    SEL_AFMODE_MANUAL,          ///<-
    SEL_AFMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_AFMODE)
} SEL_AFMODE;


/**
    select af window mode
*/
typedef enum _SEL_AFWINDOW
{
    SEL_AFWINDOW_CENTER = 0,        ///<center
    SEL_AFWINDOW_AUTO,              ///<auto
    SEL_AFWINDOW_MANUALPOSITION,    ///<manual position. ref. #SEL_AFPOSITION
    SEL_AFWINDOW_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_AFWINDOW)
} SEL_AFWINDOW;

/**
    af window position
*/
typedef enum _SEL_AFPOSITION
{
    SEL_AFPOSITION_1 = 0,           ///<-
    SEL_AFPOSITION_2,               ///<-
    SEL_AFPOSITION_3,               ///<-
    SEL_AFPOSITION_4,               ///<-
    SEL_AFPOSITION_5,               ///<-
    SEL_AFPOSITION_6,               ///<-
    SEL_AFPOSITION_7,               ///<-
    SEL_AFPOSITION_8,               ///<-
    SEL_AFPOSITION_9,               ///<-
    SEL_AFPOSITION_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_AFPOSITION)
} SEL_AFPOSITION;


/**
    iso index
*/
typedef enum _SEL_ISOINDEX
{
    SEL_ISOINDEX_AUTO = 0,      ///<-
    SEL_ISOINDEX_50,            ///<-
    SEL_ISOINDEX_80,            ///<-
    SEL_ISOINDEX_100,           ///<-
    SEL_ISOINDEX_200,           ///<-
    SEL_ISOINDEX_400,           ///<-
    SEL_ISOINDEX_800,           ///<-
    SEL_ISOINDEX_1600,          ///<-
    SEL_ISOINDEX_3200,          ///<-
    SEL_ISOINDEX_6400,          ///<-
    SEL_ISOINDEX_12800,         ///<-
    SEL_ISOINDEX_25600,         ///<-
    SEL_ISOINDEX_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_ISOINDEX)
} SEL_ISOINDEX;


/**
    bayer bit number
*/
typedef enum _SEL_RAW_BITDEPTH
{
    SEL_RAW_BITDEPTH_8 = 0,     ///<-
    SEL_RAW_BITDEPTH_10,        ///<-
    SEL_RAW_BITDEPTH_12,        ///<-
    SEL_RAW_BITDEPTH_16,        ///<-
    SEL_RAW_BITDEPTH_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_RAW_BITDEPTH)
} SEL_RAW_BITDEPTH;

/**
    image flip and mirror
*/
typedef enum _SEL_IMAGEFLIP
{
    SEL_IMAGEFLIP_OFF = 0,
    SEL_IMAGEFLIP_DSP_H,
    SEL_IMAGEFLIP_DSP_V,
    SEL_IMAGEFLIP_DSP_H_V,
    SEL_IMAGEFLIP_SEN_H,
    SEL_IMAGEFLIP_SEN_V,
    SEL_IMAGEFLIP_SEN_H_V,
    SEL_IMAGEFLIP_DSP_OFF,
    SEL_IMAGEFLIP_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_IMAGEFLIP)
} SEL_IMAGEFLIP;

/**
    data stamp on/off
*/
typedef enum _SEL_STAMP
{
    SEL_STAMP_OFF = 0,      ///<-
    SEL_STAMP_ON,           ///<-
    SEL_STAMP_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_STAMP)
} SEL_STAMP;

typedef enum _SEL_IEMODE
{
    SEL_IEMODE_NONE = 0,
    SEL_IEMODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_IEMODE)
} SEL_IEMODE;

/**
    dis on/off
*/
typedef enum _SEL_DISCOMP
{
    SEL_DISCOMP_OFF = 0,    ///<-
    SEL_DISCOMP_ON,         ///<-
    SEL_DISCOMP_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_DISCOMP)
} SEL_DISCOMP;

typedef struct    t_SceneModeInfo{
    UINT32    SceneMode;
    UINT32    FlashMode;
    UINT32    EVComp;
    UINT32    Saturation;
    UINT32    Sharpness;
    UINT32    WBMode;
    UINT32    Focus;
    UINT32    Comment;
}  SceneModeInfo;


typedef enum
{
    SEL_COMMENT_NONE = 0x00,
    SEL_ENHANCE_SKIN = 0x01,
    SEL_ENHANCE_RED = 0x02,
    SEL_ENHANCE_GREEN = 0x04,
    SEL_ENHANCE_BLUE = 0x08,
    SEL_ENHANCE_CONTRAST = 0x10,
    SEL_SOFT_SKIN = 0x20,
    SEL_SCENECOM_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_SCENEMODE_COMMENT)
} SEL_SCENEMODE_COMMENT;

/**
    photo frame on/off
*/
typedef enum _SEL_PHOTOFRAME
{
    SEL_PHOTOFRAME_OFF = 0,     ///<-
    SEL_PHOTOFRAME_ON,          ///<-
    SEL_PHOTOFRAME_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_PHOTOFRAME)
} SEL_PHOTOFRAME;

/**
    video resolution
*/
typedef enum _SEL_VIDEOSIZE
{
    SEL_VIDEOSIZE_2880x2160 = 0,    ///<-
    SEL_VIDEOSIZE_2560x1440,        ///<-
    SEL_VIDEOSIZE_1920x1080,        ///<-
    SEL_VIDEOSIZE_1280x960,         ///<-
    SEL_VIDEOSIZE_1280x720,         ///<-
    SEL_VIDEOSIZE_848x480,          ///<-
    SEL_VIDEOSIZE_640x480,          ///<-
    SEL_VIDEOSIZE_FULLRES,          ///<-
    SEL_VIDEOSIZE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_VIDEOSIZE)
} SEL_VIDEOSIZE;

//#NT#2016/12/16#Silvia Wu -begin
//#NT# support project set 27.5fps/55fps
/**
    video fps
*/
typedef enum _SEL_VIDEOFPS
{
    SEL_VIDEOFPS_10 = 0,        ///<-
    SEL_VIDEOFPS_15,            ///<-
    SEL_VIDEOFPS_20,            ///<-
    SEL_VIDEOFPS_24,            ///<-
    SEL_VIDEOFPS_25,            ///<-
    SEL_VIDEOFPS_30,            ///<-
    SEL_VIDEOFPS_45,            ///<-
    SEL_VIDEOFPS_50,            ///<-
    SEL_VIDEOFPS_60,            ///<-
    SEL_VIDEOFPS_100,           ///<-
    SEL_VIDEOFPS_120,           ///<-
    SEL_VIDEOFPS_150,           ///<-
    SEL_VIDEOFPS_200,           ///<-
    SEL_VIDEOFPS_240,           ///<-
    SEL_VIDEOFPS_FULLRES,       ///<-
    SEL_VIDEOFPS_27_5,          ///<-
    SEL_VIDEOFPS_55,            ///<-
    SEL_VIDEOFPS_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_VIDEOFPS)
} SEL_VIDEOFPS;
//#NT#2016/12/16#Silvia Wu -end

/**
    super resolution on/off
*/
typedef enum
{
    SEL_SUPERRESOL_OFF = 0,     ///<-
    SEL_SUPERRESOL_ON,          ///<-
    SEL_SUPERRESOL_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_SUPERRESOL)
} SEL_SUPERRESOL;

/**
    shdr on/off
*/
typedef enum _SEL_SHDR
{
    SEL_SHDR_OFF = 0,    ///<-
    SEL_SHDR_ON,         ///<-
    SEL_SHDR_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_SHDR)
} SEL_SHDR;

/**
    wdr on/off
*/
typedef enum _SEL_WDR
{
    SEL_WDR_OFF = 0,    ///<-
    SEL_WDR_ON,         ///<-
    SEL_WDR_AUTO,       ///<-
    SEL_WDR_DEFOG,
    SEL_WDR_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_WDR)
} SEL_WDR;

/**
    wdr level
    reference to SR_WDR_LUMA_COMP_LEVEL
*/
typedef enum _SEL_WDR_LEVEL
{
    SEL_WDR_LV0 = 0,        ///<-weak
    SEL_WDR_LV1 = 108,      ///<-
    SEL_WDR_LV2 = 208,      ///<-
    SEL_WDR_LV3 = 308,      ///<-
    SEL_WDR_LV4 = 412,      ///<-
    SEL_WDR_LV5 = 512,      ///<-normal
    SEL_WDR_LV6 = 612,      ///<-
    SEL_WDR_LV7 = 712,      ///<-
    SEL_WDR_LV8 = 812,      ///<-
    SEL_WDR_LV9 = 912,      ///<-
    SEL_WDR_LV10 = 1024,    ///<-strong
    SEL_WDR_LEVEL_MAX_CNT = SEL_WDR_LV10+1,
    ENUM_DUMMY4WORD(SEL_WDR_LEVEL)
} SEL_WDR_LEVEL;

/**
    anti shake AE on/off
*/
typedef enum
{
    SEL_ANTISHAKE_OFF = 0,            ///<-
    SEL_ANTISHAKE_ON,                 ///<-
    SEL_ANTISHAKE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_ANTISHAKE)
} SEL_ANTISHAKE;

/**
   Face Detection on/off
*/
typedef enum
{
    SEL_FD_OFF = 0,            ///<-
    SEL_FD_ON,                 ///<-
    SEL_FD_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_FD)
} SEL_FD;

/**
    IR Cut on/off
*/
typedef enum _SEL_IRCUT
{
    SEL_IRCUT_OFF,         ///<-invalid value
    SEL_IRCUT_ON,          ///<-invalid value
    SEL_IRCUT_AUTO,        ///<-
    SEL_IRCUT_DAY,         ///<-
    SEL_IRCUT_NIGHT,       ///<-
    SEL_IRCUT_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_IRCUT)
} SEL_IRCUT;

/**
    IPL Control Flow Sel
*/
typedef enum _SEL_CTRLFLOW
{
    SEL_CTRLFLOW_COMMON = 0,     ///<-
    SEL_CTRLFLOW_ALLDIRECT,      ///<-
    SEL_CTRLFLOW_DEWARP,         ///<-
    SEL_CTRLFLOW_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_CTRLFLOW)
} SEL_CTRLFLOW;

/**
    IPP EDGE on/off
*/
typedef enum _SEL_IPPEDGE
{
    SEL_IPPEDGE_OFF = 0,    ///<-
    SEL_IPPEDGE_ON,         ///<-
    SEL_IPPEDGE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_IPPEDGE)
} SEL_IPPEDGE;

/**
    BackLight Compensation on/off
*/
typedef enum _SEL_BLC
{
    SEL_BLC_OFF = 0,    ///<-
    SEL_BLC_ON,         ///<-
    SEL_BLC_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_BLC)
} SEL_BLC;

/**
    BLC level
*/
typedef enum _SEL_BLC_LEVEL
{
    SEL_BLC_LV0 = 0,        ///<-
    SEL_BLC_LV1,            ///<-
    SEL_BLC_LV2,            ///<-
    SEL_BLC_LV3,            ///<-
    SEL_BLC_LV4,            ///<-
    SEL_BLC_LV5,            ///<-
    SEL_BLC_LV6,            ///<-
    SEL_BLC_LV7,            ///<-
    SEL_BLC_LV8,            ///<-
    SEL_BLC_LV9,            ///<-
    SEL_BLC_LV10,           ///<-
    SEL_BLC_LEVEL_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_BLC_LEVEL)
} SEL_BLC_LEVEL;

/**
    User gamma
*/
typedef enum _SEL_IPC_MODE
{
    SEL_IPC_MODE_OFF = 0,    ///<-
    SEL_IPC_MODE_ON,

    SEL_IPC_MODE_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_IPC_MODE)
} SEL_IPC_MODE;

typedef enum _SEL_USER_GAMMA_MODE
{
    SEL_USER_GAMMA_LINEAR = 0,    ///<-
    SEL_USER_GAMMA_CURVE_DEFAULT,
    SEL_USER_GAMMA_CURVE_MODE_16,
    SEL_USER_GAMMA_CURVE_MODE_18,
    SEL_USER_GAMMA_CURVE_MODE_20,
    SEL_USER_GAMMA_CURVE_MODE_22,
    SEL_USER_GAMMA_CURVE_MODE_24,
    SEL_USER_GAMMA_CURVE_MODE_USER,
    SEL_USER_GAMMA_LEVEL_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_USER_GAMMA_MODE)
} SEL_USER_GAMMA_MODE;
/**
    User CC
*/
typedef enum _SEL_USER_CC
{
    SEL_USER_CC_OFF = 0,    ///<-
    SEL_USER_CC_AUTO,         ///<-
    SEL_USER_CC_MANUAL,
    SEL_USER_CC_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_USER_CC)
} SEL_USER_CC;

/**
    GAMMA_Anti_Fog_LEVEL
*/
typedef enum _SEL_ANTI_FOG_LEVEL
{

    SEL_ANTI_FOG_LV1= 0,            ///<-
    SEL_ANTI_FOG_LV2,            ///<-
    SEL_ANTI_FOG_LV3,            ///<-
    SEL_ANTI_FOG_LV4,            ///<-
    SEL_ANTI_FOG_LV5,            ///<-
    SEL_ANTI_FOG_LV6,            ///<-
    SEL_ANTI_FOG_LV7,            ///<-
    SEL_ANTI_FOG_LV8,            ///<-
    SEL_ANTI_FOG_LV9,            ///<-
    SEL_ANTI_FOG_LV10,           ///<-
    SEL_ANTI_FOG_LEVEL_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_GAMMA_ANTI_FOG_LEVEL)
} SEL_ANTI_FOG_LEVEL;


typedef enum _SEL_ANTI_FOG
{
    SEL_ANTI_FOG_OFF = 0,    ///<-
    SEL_ANTI_FOG_AUTO,         ///<-
    SEL_ANTI_FOG_MANUAL,
    SEL_ANTI_FOG_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_ANTI_FOG)
} SEL_ANTI_FOG;

//#NT#2017/02/06#Jarkko Chang -begin
//#NT# add support side by side left/right, up/down case
/**
    side by side select
*/
typedef enum _SEL_SBS
{
    SEL_SBS_OFF = 0,     ///<-normal case, side by side off
    SEL_SBS_LR,          ///<-Left-right side by side
    SEL_SBS_UD,          ///<-up-down side by side
    SEL_SBS_MAX_CNT,
    ENUM_DUMMY4WORD(SEL_SBS)
} SEL_SBS;
//#NT#2017/02/06#Jarkko Chang -end

/**
     IPL Get UI Information

     @param[in] Id    IPL process id
     @param[in] SelIdx    UI Select item

     @return
        -b UINT32: select item information.
*/
extern UINT32 IPL_AlgGetUIInfo(IPL_PROC_ID Id, IPL_SEL_ITEM SelIdx);

/**
     IPL Set UI Infomation

     @param[in] Id    IPL process id
     @param[in] SelIdx    UI Select item
     @param[in] Value     information for select item

     @return Description of data returned.
         - @b E_OK
         - @b E_SYS
*/
extern ER IPL_AlgSetUIInfo(IPL_PROC_ID Id, IPL_SEL_ITEM SelIdx, UINT32 Value);

//@}
#endif //_IPL_ALGINFOR_H_
