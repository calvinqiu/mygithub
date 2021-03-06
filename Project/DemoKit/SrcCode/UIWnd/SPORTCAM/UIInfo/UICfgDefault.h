#ifndef UI_CFG_DEFAULT_H
#define UI_CFG_DEFAULT_H

#include "UIFlow.h"

// Photo
#define DEFAULT_PHOTO_CAPTURE           SELFTIMER_OFF
#if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
#define DEFAULT_PHOTO_SIZE              PHOTO_SIZE_5M
#else
#define DEFAULT_PHOTO_SIZE              PHOTO_SIZE_12M
#endif
#define DEFAULT_PHOTO_QUALITY           QUALITY_FINE
#define DEFAULT_PHOTO_COLOR             COLOR_EFFECT_STANDARD
#define DEFAULT_SCENE                   SCENE_AUTO
#define DEFAULT_EV                      EV_00
#define DEFAULT_ISO                     ISO_AUTO
#define DEFAULT_WB                      WB_AUTO
#define DEFAULT_METERING                METERING_AIAE
#define DEFAULT_SHARPNESS               SHARPNESS_NORMAL
#define DEFAULT_SATURATION              SATURATION_NORMAL
#define DEFAULT_FD                      FD_OFF
#define DEFAULT_ANTISHAKING             ANTISHAKE_OFF
#if (_GYRO_EXT_==_GYRO_EXT_NONE_)
#define DEFAULT_RSC                     RSC_OFF
#else
#define DEFAULT_RSC                     RSC_OFF
#endif
#if (_MODEL_DSC_ == _MODEL_SUBEI_TFH9_)
#define DEFAULT_WDR                     WDR_ON
#else
#define DEFAULT_WDR                     WDR_OFF
#endif
#define DEFAULT_QUICK_REVIEW            QUICK_REVIEW_0SEC
#define DEFAULT_DATE_STAMP              DATE_STAMP_DATE_TIME
#define DEFAULT_SELFTIMER               SELFTIMER_OFF
#define DEFAULT_FLASH_MODE              FLASH_OFF
#if (_MODEL_DSC_ == _MODEL_TWIN_360_)
#define DEFAULT_CONTINUE_SHOT           CONTINUE_SHOT_SIDE
#else
#define DEFAULT_CONTINUE_SHOT           CONTINUE_SHOT_OFF
#endif
#define DEFAULT_SHDR                    SHDR_OFF
#if (PIP_VIEW_LR == ENABLE)
#define DEFAULT_DUAL_CAM                DUALCAM_LR_16_9
#else
#if (SENSOR_CAPS_COUNT == 1)
#define DEFAULT_DUAL_CAM                DUALCAM_FRONT
#endif
#if (SENSOR_CAPS_COUNT == 2)
#if (_MODEL_DSC_ == _MODEL_PTL_668_ || _MODEL_DSC_ == _MODEL_PWA6_668_ || _MODEL_DSC_ == _MODEL_SUBEI_TFH9_)
#define DEFAULT_DUAL_CAM                DUALCAM_BEHIND
#else
#define DEFAULT_DUAL_CAM                DUALCAM_BOTH
#endif
#endif
#endif
// Movie
#if (_MODEL_DSC_ == _MODEL_TWIN_360_||_MODEL_DSC_ == _MODEL_PTL_668_ || _MODEL_DSC_ == _MODEL_PWA6_668_ || _MODEL_DSC_ == _MODEL_SUBEI_TFH9_)
#define DEFAULT_MOVIE_SIZE             MOVIE_SIZE_FRONT_1920x1080P30// MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30
#else
//#NT#2016/08/12#Hideo Lin -begin
//#NT#For small size clone movie
#if (SMALL_CLONE_MOVIE == DISABLE)
#define DEFAULT_MOVIE_SIZE              MOVIE_SIZE_FRONT_1920x1080P60
#else
#define DEFAULT_MOVIE_SIZE              MOVIE_SIZE_CLONE_2560x1440P30_848x480P30
#endif
//#NT#2016/08/12#Hideo Lin -end
#endif

#define DEFAULT_MOVIE_SIZE              MOVIE_SIZE_FRONT_1920x1080P30//MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30


#if (_MODEL_DSC_ == _MODEL_MIO_MT39_) // MT39 E-Mirror cannot support dual recording due to bandwidth issue
#define DEFAULT_MOVIE_SIZE_DUAL         MOVIE_SIZE_FRONT_1920x1080P30
#else
#define DEFAULT_MOVIE_SIZE_DUAL         MOVIE_SIZE_DUAL_1920x1080P30_1920x1080P30
#endif
#define DEFAULT_MOVIE_SIZE_SINGLE       MOVIE_SIZE_FRONT_1920x1080P30

#define DEFAULT_MOVIE_QUALITY           MOVIE_QUALITY_FINE
#define DEFAULT_MOVIE_COLOR             MOVIE_COLOR_NORMAL
#define DEFAULT_MOVIE_SELFTIMER         MOVIE_SELFTIMER_OFF
#define DEFAULT_MOVIE_EV                EV_00
#define DEFAULT_MOVIE_VIVILINK          VIVILINK_NONE
#define DEFAULT_MOVIE_LDWS              MOVIE_LDWS_ON
#define DEFAULT_MOVIE_FCWS              MOVIE_FCW_ON
#define DEFAULT_MOVIE_CYCLICREC         MOVIE_CYCLICREC_3MIN
#define DEFAULT_MOVIE_MOTION_DET        MOVIE_MOTIONDET_OFF
#define DEFAULT_MOVIE_AUDIO             MOVIE_AUDIO_ON
#define DEFAULT_MOVIE_DATEIMPRINT       MOVIE_DATEIMPRINT_ON
#define DEFAULT_MOVIE_HDR               MOVIE_HDR_ON//MOVIE_HDR_OFF
#if (_MODEL_DSC_ == _MODEL_SUBEI_TFH9_)
#define DEFAULT_MOVIE_WDR               MOVIE_WDR_ON
#else
#define DEFAULT_MOVIE_WDR               MOVIE_WDR_ON//MOVIE_WDR_OFF
#endif
#define DEFAULT_MOVIE_REC_VOLUME        MOVIE_VOL_MAX
#define DEFAULT_MOVIE_REC_AUD           MOVIE_AUD_REC_ON
//#NT#2016/06/14#Charlie Chang -begin
//#NT# support contrast, two way audio in ,two way audio in sample rate , flip, movie quality set
#define DEFAULT_MOVIE_CONTRAST          MOVIE_CONTRAST_MIDDLE
#define DEFAULT_MOVIE_AUDIOIN           MOVIE_AUDIOIN_ON
#define DEFAULT_MOVIE_AUDIOIN_SR        MOVIE_AUDIOIN_SR_8000
#define DEFAULT_MOVIE_FLIP_MIRROR       MOVIE_FLIP_MIRROR_NONE
#define DEFAULT_MOVIE_QUALITY_SET       MOVIE_QUALITY_SET_MAX
//#NT#2016/06/14#Charlie Chang -end
//#NT#2017/03/03#Jeah Yen -begin
//#NT# support sbs mode
#define DEFAULT_MOVIE_SBS_MODE          MOVIE_SBS_LR
//#NT#2017/03/03#Jeah Yen -end
#if (_GYRO_EXT_==_GYRO_EXT_NONE_)
#define DEFAULT_MOVIE_RSC               MOVIE_RSC_OFF //RSC not Ready
#else
#define DEFAULT_MOVIE_RSC               MOVIE_RSC_OFF
#endif

#define DEFAULT_MCTFINDEX               MOVIE_MCTF_ON

#define DEFAULT_IR_CUT                  MOVIE_IR_CUT_OFF

#define DEFAULT_MOVIE_TIMELAPSE_REC     MOVIE_TIMELAPSEREC_OFF
#define DEFAULT_MOVIE_PIM               MOVIE_PIM_OFF

#define DEFAULT_MOVIE_PTZ                   MOVIE_PTZ_OFF
#define DEFAULT_MOVIE_URGENT_PROTECT_AUTO   MOVIE_URGENT_PROTECT_AUTO_ON//MOVIE_URGENT_PROTECT_AUTO_OFF//20171024 qiuhan modify
#define DEFAULT_MOVIE_URGENT_PROTECT_MANUAL  MOVIE_URGENT_PROTECT_MANUAL_ON//MOVIE_URGENT_PROTECT_MANUAL_OFF//20171024 qiuhan modify
#if (_MODEL_DSC_ == _MODEL_PTL_668_ || _MODEL_DSC_ == _MODEL_PWA6_668_ || _MODEL_DSC_ == _MODEL_SUBEI_TFH9_)
#define DEFAULT_MOVIE_DDD                   MOVIE_DDD_ON
#else
#define DEFAULT_MOVIE_DDD                   MOVIE_DDD_OFF
#endif
#if defined(YQCONFIG_ADAS_FUNC_SUPPORT)
#define DEFAULT_MOVIE_ADAS_CAL              MOVIE_ADAS_CAL_ON
#define DEFAULT_ADAS    MOVIE_ADAS_CAL_ON
#else
#define DEFAULT_MOVIE_ADAS_CAL              MOVIE_ADAS_CAL_OFF
#endif

#define DEFAULT_PARKING_MODE    PARKINGMODE_OFF
// Playback
#define DEFAULT_PROTECT                 PROTECT_ONE
#define DEFAULT_MOVIE_PLAY_VOLUME       (MOVIE_AUDIO_VOL_SETTING_MAX-1)

// System
#define DEFAULT_AUDIO_PLAY_VOLUME       AUDIO_VOL_MAX
#define DEFAULT_AUTO_POWER_OFF          POWER_ON
#define DEFAULT_LCD_OFF                 LCDOFF_ON
#define DEFAULT_BEEP                    BEEP_ON
#if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
    #if defined(YQCONFIG_COMB_LANGUAGE_CHOOSE_EN)
	      #define DEFAULT_LANGUAGE                LANG_EN
          #define DEFAULT_FREQUENCY               FREQUENCY_60HZ

    #elif defined(YQCONFIG_COMB_LANGUAGE_CHOOSE_SC)
	       #define DEFAULT_LANGUAGE                LANG_SC
          #define DEFAULT_FREQUENCY               FREQUENCY_50HZ

   #endif

#else
#define DEFAULT_LANGUAGE                LANG_EN
#define DEFAULT_FREQUENCY               FREQUENCY_60HZ
#endif
#define DEFAULT_TV_MODE                 TV_MODE_NTSC
#define DEFAULT_HDMI_MODE               VIDEOOUT2_FHD
#define DEFAULT_DUALDISP                DUALDISP_OFF
#define DEFAULT_SENSOR_ROTATE           SEN_ROTATE_OFF
#define DEFAULT_DATE_FORMAT             DATE_FORMAT_YMD
#define DEFAULT_OPENING_LOGO            OPENING_LOGO_ON
#define DEFAULT_LCD_DISPLAY             DISPOUT_NORMAL
#define DEFAULT_LCD_BRIGHTNESS          LCDBRT_LVL_05
#define DEFAULT_MACRO                   MACRO_OFF
#define DEFAULT_USB_MODE                USB_MODE_MSDC
#if (GSENSOR_FUNCTION == ENABLE)
#if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
#define DEFAULT_GSENSOR                 GSENSOR_MED//GSENSOR_LOW
#else
#define DEFAULT_GSENSOR                 GSENSOR_OFF

#endif
/* modify end by ZMD, 2017-02-14 */


#else
#define DEFAULT_GSENSOR                 GSENSOR_OFF
#endif
#define DEFAULT_EDGE                    MOVIE_EDGE_ON
#define DEFAULT_NR                      MOVIE_NR_ON

//-----------------------------------------------------------------------------
// System
//-----------------------------------------------------------------------------
#define UIDFT_FS_STATUS                 FS_NOT_INIT
#define UIDFT_CARD_STATUS               CARD_REMOVED
#define UIDFT_BATTERY_LVL               0
#define UIDFT_AUDIO_VOLUME              AUDIO_VOL_7

// Wi-Fi
#define DEFAULT_MOVIE_WIFI_AUTO_RECORDING_OPTION    WIFI_AUTO_RECORDING_ON
#define DEFAULT_WIFI_MODE                           NET_AP_MODE
#if (WIFI_FINALCAM_APP_STYLE == ENABLE || UCTRL_APP_MOVIE_FEATURE_SETGET == ENABLE || YOUKU_SDK == ENABLE)
#define DEFAULT_MOVIE_WIFI_APP_PREVIEW_SIZE    WIFI_MOVIE_APP_PREVIEW_SIZE_VGA_16_9
#endif

#endif
