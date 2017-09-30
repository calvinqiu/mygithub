#ifndef NVT_USER_COMMAND_H
#define NVT_USER_COMMAND_H

//#include "UIFramework.h"
#include "../../../Include/App/UIFramework.h"
//typedef UINT32 NVTEVT;

// KEY event class

typedef enum
{
    NVTEVT_KEY_EVT_START        = DEV_KEY_EVENT_BASE, ///< Min value = 0x11060000

    NVTEVT_KEY_STATUS_START     = 0x11060000,    // Status key start
        NVTEVT_KEY_STATUS1      = 0x11060001,
        NVTEVT_KEY_STATUS2      = 0x11060002,
        NVTEVT_KEY_STATUS3      = 0x11060003,
        NVTEVT_KEY_STATUS4      = 0x11060004,
        NVTEVT_KEY_STATUS5      = 0x11060005,
        NVTEVT_KEY_STATUS6      = 0x11060006,
        NVTEVT_KEY_STATUS7      = 0x11060007,
        NVTEVT_KEY_STATUS8      = 0x11060008,
        NVTEVT_KEY_STATUS9      = 0x11060009,
        NVTEVT_KEY_STATUS10     = 0x1106000a,
        /* INSERT NEW EVENT HRER */
    NVTEVT_KEY_STATUS_END       = NVTEVT_KEY_STATUS_START + 0x100 -1, // Status key end

    NVTEVT_KEY_BUTTON_START     = 0x11060100,    // Button key start

        NVTEVT_KEY_PRESS_START  = 0x11060100,    // Press key start
            NVTEVT_KEY_PRESS    = 0x11060101,
            NVTEVT_KEY_POWER    = 0x11060102,
            NVTEVT_KEY_UP       = 0x11060103,
            NVTEVT_KEY_DOWN     = 0x11060104,
            NVTEVT_KEY_LEFT     = 0x11060105,
            NVTEVT_KEY_RIGHT    = 0x11060106,
            NVTEVT_KEY_ENTER    = 0x11060107,
            NVTEVT_KEY_TELE     = 0x11060108,
            NVTEVT_KEY_WIDE     = 0x11060109,
            NVTEVT_KEY_DEL      = 0x1106010a,
            NVTEVT_KEY_ZOOMIN   = 0x1106010b,
            NVTEVT_KEY_ZOOMOUT  = 0x1106010c,
            NVTEVT_KEY_SHUTTER1 = 0x1106010d,
            NVTEVT_KEY_SHUTTER2 = 0x1106010e,
            NVTEVT_KEY_FACEDETECT = 0x1106010f,
            NVTEVT_KEY_MODE     = 0x11060110,
            NVTEVT_KEY_MOVIE    = 0x11060111,
            NVTEVT_KEY_PLAYBACK = 0x11060112,
            NVTEVT_KEY_MENU     = 0x11060113,
            NVTEVT_KEY_DISPLAY  = 0x11060114,
            NVTEVT_KEY_I        = 0x11060115,
            NVTEVT_KEY_CUSTOM1  = 0x11060116,
            NVTEVT_KEY_CUSTOM2  = 0x11060117,
            NVTEVT_KEY_NEXT     = 0x11060118,
            NVTEVT_KEY_PREV     = 0x11060119,
            NVTEVT_KEY_SELECT   = 0x1106011a,
            NVTEVT_KEY_RC_SHUTTER2       = 0x1106011b,
            NVTEVT_KEY_RC_SHUTTER2_LONG  = 0x1106011c,
            NVTEVT_KEY_RC_MOVIEREC       = 0x1106011d,
            NVTEVT_KEY_RC_MOVIEREC_LONG  = 0x1106011e,
            //#NT#2016/06/23#Niven Cho -begin
            //#NT#Enter calibration by cgi event or command event
            NVTEVT_KEY_CALIBRATION = 0x1106011f,
            //#NT#2016/06/23#Niven Cho -end
            /* INSERT NEW EVENT HRER */
        NVTEVT_KEY_PRESS_END    = NVTEVT_KEY_PRESS_START + 0x100 -1, // Press key end

        NVTEVT_KEY_CONTINUE_START = 0x11060200, // Continue key start
            NVTEVT_KEY_CONTINUE = 0x11060201, // must = NVTEVT_KEY_PRESS + 0x100,
            NVTEVT_KEY_POWER_CONT = 0x11060202, // must = NVTEVT_KEY_POWER + 0x100,
            NVTEVT_KEY_UP_CONT  = 0x11060203, // must = NVTEVT_KEY_UP + 0x100,
            NVTEVT_KEY_DOWN_CONT = 0x11060204, // must = NVTEVT_KEY_DOWN + 0x100,
            NVTEVT_KEY_LEFT_CONT = 0x11060205, // must = NVTEVT_KEY_LEFT + 0x100,
            NVTEVT_KEY_RIGHT_CONT = 0x11060206, // must = NVTEVT_KEY_RIGHT + 0x100,
        NVTEVT_KEY_CONTINUE_END = NVTEVT_KEY_CONTINUE_START + 0x100 -1, // Continue key end

        NVTEVT_KEY_RELEASE_START = 0x11060400, // Release key start
            NVTEVT_KEY_RELEASE = 0x11060401, // must = NVTEVT_KEY_PRESS + 0x300,
            NVTEVT_KEY_POWER_REL = 0x11060402, // must = NVTEVT_KEY_POWER + 0x300,
            NVTEVT_KEY_UP_REL  = 0x11060403, // must = NVTEVT_KEY_UP + 0x300,
            NVTEVT_KEY_DOWN_REL = 0x11060404, // must = NVTEVT_KEY_DOWN + 0x300,
            NVTEVT_KEY_LEFT_REL = 0x11060405, // must = NVTEVT_KEY_LEFT + 0x300,
            NVTEVT_KEY_RIGHT_REL = 0x11060406, // must = NVTEVT_KEY_RIGHT + 0x300,
            NVTEVT_KEY_ZOOMRELEASE = 0x1106040b, // must = NVTEVT_KEY_ZOOMIN + 0x300,
            NVTEVT_KEY_SHUTTER1_REL = 0x1106040d, // must = NVTEVT_KEY_SHUTTER1 + 0x300,
            NVTEVT_KEY_SHUTTER2_REL = 0x1106040e, // must = NVTEVT_KEY_SHUTTER2 + 0x300,
        NVTEVT_KEY_RELEASE_END  = NVTEVT_KEY_RELEASE_START + 0x100 -1, // Release key end

    NVTEVT_KEY_BUTTON_END       = NVTEVT_KEY_BUTTON_START + 0x800 -1, // Button key end

    NVTEVT_KEY_EVT_END          = DEV_KEY_EVENT_BASE + 0x1000 -1, ///< Max value = 0x11060fff
}
NVT_KEY_EVENT;

// DEVICE event class

typedef enum
{
    NVTEVT_USERCMD_START        = USER_EVENT_MASK, ///< Min value = 0x80000000

    // UI event
    NVTEVT_UPDATE_INFO          = 0x80000000, ///< Update window info command
    /* INSERT NEW EVENT HRER */

    // timer event
    NVTEVT_TIMER                = 0x80001000,
    NVTEVT_ANIMATE_TIMER        = 0x80001001,
    NVTEVT_SELFTIMER           = 0x80001002,
    NVTEVT_DISPTIMER           = 0x80001003,
    NVTEVT_01SEC_TIMER          = 0x80001004,
    NVTEVT_05SEC_TIMER          = 0x80001005,
    NVTEVT_1SEC_TIMER           = 0x80001006,
    NVTEVT_5SEC_TIMER           = 0x80001007,
    NVTEVT_DISCONNECT_LAST_TIMER = 0x80001008,
    NVTEVT_DELAY_CLOSE_TIMER    = 0x80001009,
    NVTEVT_WARNING_TIMER        = 0x8000100a,
    /* INSERT NEW EVENT HRER */

    // power event
    NVTEVT_BATTERY              = 0x80002000,
    NVTEVT_BATTERY_LOW          = 0x80002001,
    NVTEVT_AC_Plug              = 0x80002002,
    NVTEVT_AC_UnPlug            = 0x80002003,
    /* INSERT NEW EVENT HRER */

    // storage event
    NVTEVT_STORAGE_INIT         = 0x80003000,
    NVTEVT_STORAGE_CHANGE       = 0x80003001,
    /* INSERT NEW EVENT HRER */
	NVTEVT_GSENSOR_TRIG 	    = 0x80003002,
    NVTEVT_TOUCHPANEL_KEY	    = 0x80003003,
    NVTEVT_UPDATE_GPS_TIME	    = 0x80003004,
    NVTEVT_MTK_MENU_SETTING=0x80003005,
    NVTEVT_MTK_GPSINFO=0x80003006,        
    NVTEVT_MTK_STARTREC=0x80003007,        
    NVTEVT_MTK_STOPREC=0x80003008,        
    NVTEVT_MTK_FORMATCARD=0x80003009,        
    NVTEVT_MTK_CAPTURE=0x8000300A,        
    NVTEVT_MTK_TRANSMIT_FINISH=0x8000300B,

    NVTEVT_MCU_UPDATA_START = 0x8000300C,
    NVTEVT_MCU_UPDATA_ERROR = 0x8000300D, 
    NVTEVT_MCU_UPDATA_FINISH = 0x8000300E,
#if defined(YQCONFIG_UART_TO_MTK_SUPPORT)
	NVTEVT_MTK_ADAS_SWITCH = 0x8000300F,
    NVTEVT_MTK_CHANGE_DIAPLAYMODE = 0x80003010,
    NVTEVT_MTK_CHANGE_MODE = 0x80003011,
#endif
    // wifi event
    NVTEVT_WIFI_EXE_PHOTO_SIZE  = 0x8000e000,         ///< Set Photo size by Wi-Fi App
    NVTEVT_WIFI_EXE_PHOTO_CAPTURE   = 0x8000e001,      ///< Execute photo capture shot by Wi-Fi App
    NVTEVT_WIFI_EXE_MOVIE_REC_SIZE  = 0x8000e002,     ///< Set Movie recording size by Wi-Fi App
    NVTEVT_WIFI_EXE_MOVIE_REC   = 0x8000e003,          ///< Movie recording by Wi-Fi App
    NVTEVT_WIFI_EXE_CYCLIC_REC  = 0x8000e004,
    NVTEVT_WIFI_EXE_MOVIE_WDR   = 0x8000e005,
    NVTEVT_WIFI_EXE_MOVIE_EV    = 0x8000e006,
    NVTEVT_WIFI_EXE_MOTION_DET  = 0x8000e007,
    NVTEVT_WIFI_EXE_MOVIE_AUDIO = 0x8000e008,
    NVTEVT_WIFI_EXE_MOVIE_GSENSOR_SENS  = 0x8000e009,  ///< Set G-sensor sensitivity
    NVTEVT_WIFI_EXE_DATEIMPRINT = 0x8000e00a,
    NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_SIZE = 0x8000e00b, ///< Set Movie preview size by Wi-Fi App
    NVTEVT_WIFI_EXE_SET_AUTO_RECORDING  = 0x8000e00c, ///< CarDV mode or Sport DV mode.
    NVTEVT_WIFI_EXE_MOVIE_REC_BITRATE   = 0x8000e00d,  ///< Set Movie recording bit rate by Wi-Fi App
    NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_BITRATE  = 0x8000e00e,  ///< Set Movie preview bit rate by Wi-Fi App
    NVTEVT_WIFI_EXE_MOVIE_LIVEVIEW_START    = 0x8000e00f,    ///< Set Movie preview bit rate by Wi-Fi App
    NVTEVT_WIFI_EXE_MOVIE_TRIGGER_RAWENC    = 0x8000e010,       ///< Trigger RAW encode in movie recording mode.
    NVTEVT_WIFI_EXE_QUERY       = 0x8000e011,              ///< Request to get whole of preferences by Wi-Fi App
    NVTEVT_WIFI_EXE_MODE        = 0x8000e012,               ///< Change mode by Wi-Fi App
    NVTEVT_WIFI_EXE_SET_DATE    = 0x8000e013,           ///< Set the Date by Wi-Fi App
    NVTEVT_WIFI_EXE_SET_TIME    = 0x8000e014,           ///< Set the Time by Wi-Fi App
    NVTEVT_WIFI_EXE_SET_SSID    = 0x8000e015,
    NVTEVT_WIFI_EXE_SET_PASSPHRASE  = 0x8000e016,
    NVTEVT_WIFI_EXE_DATEFORMAT  = 0x8000e017,
    NVTEVT_WIFI_EXE_TIMEFORMAT  = 0x8000e018,
    NVTEVT_WIFI_EXE_POWEROFF    = 0x8000e019,
    NVTEVT_WIFI_EXE_LANGUAGE    = 0x8000e01a,
    NVTEVT_WIFI_EXE_TVFORMAT    = 0x8000e01b,
    NVTEVT_WIFI_EXE_FORMAT      = 0x8000e01c,
    NVTEVT_WIFI_EXE_SYSRESET    = 0x8000e01d,
    NVTEVT_WIFI_EXE_VERSION     = 0x8000e01e,
    NVTEVT_WIFI_EXE_FWUPDATE    = 0x8000e01f,
    NVTEVT_WIFI_EXE_RECONNECT   = 0x8000e020,
    NVTEVT_WIFI_EXE_SAVEMENU    = 0x8000e021,
    NVTEVT_WIFI_EXE_REMOVE_USER = 0x8000e022,
    NVTEVT_WIFI_AUTHORIZED_OK   = 0x8000e023,
    NVTEVT_WIFI_DEAUTHENTICATED = 0x8000e024,
    NVTEVT_WIFI_STATE           = 0x8000e025,
    NVTEVT_WIFI_EXE_SEND_SSID_PASSPHRASE  = 0x8000e026,
    NVTEVT_WIFI_EXE_SET_WIFI_CONNECT_MODE = 0x8000e027,
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
    NVTEVT_WIFI_EXE_PIP_STYLE             = 0x8000e028,
    NVTEVT_WIFI_EXE_MODE_DONE             = 0x8000e029,
    NVTEVT_WIFI_EXE_APP_STARTUP           = 0x8000e02a,
    NVTEVT_WIFI_EXE_APP_SESSION_CLOSE     = 0x8000e02b,
    NVTEVT_WIFI_EXE_HEARTBEAT             = 0x8000e02c,
//#NT#2016/03/23#Isiah Chang -end

//#NT#2016/06/03#Charlie Chang -begin
//#NT# support contrast ,two-way audio, flip_mirror, quality set
    NVTEVT_WIFI_EXE_MOVIE_CONTRAST              = 0x8000e02d,
    NVTEVT_WIFI_EXE_MOVIE_AUDIOIN               = 0x8000e02e,
    NVTEVT_WIFI_EXE_MOVIE_AUDIOIN_SR            = 0x8000e02f,
    NVTEVT_WIFI_EXE_MOVIE_FLIP_MIRROR           = 0x8000e030,
    NVTEVT_WIFI_EXE_MOVIE_QUALITY_SET           = 0x8000e031,
//#NT#2016/06/03#Charlie Chang -end
    NVTEVT_WIFI_DHCP_REQ                        = 0x8000e032,
//#NT#2016/12/02#Isiah Chang -begin
//#NT#Add WiFiCmd Bitrate control.
    NVTEVT_WIFI_EXE_MOVIE_BRC_ADJUST            = 0x8000e033,
//#NT#2016/12/02#Isiah Chang -end
//#NT#2017/04/18#Isiah Chang -begin
//#NT#Add WiFiCmd RTSP Audio control.
    NVTEVT_WIFI_EXE_MOVIE_RTSP_AUD_CTRL         = 0x8000e034,
//#NT#2017/04/18#Isiah Chang -end
    /* INSERT NEW EVENT HRER */

    // return event
    NVTRET_CLOSE_PARENT_WND     = 0x8000f000, ///< Close parent window
    NVTRET_CLOSE_PARENT_WITH_PARAM = 0x8000f001, ///< Close parent window with parameters
    NVTRET_CANCEL               = 0x8000f002,
    NVTRET_CONFIRM              = 0x8000f003,
    NVTRET_DELETE               = 0x8000f004,
    NVTRET_COPY2CARD            = 0x8000f005,
    NVTRET_PROTECT              = 0x8000f006,
    NVTRET_THUMBNAIL            = 0x8000f007,
    NVTRET_MAGNIFY              = 0x8000f008,
    NVTRET_DELETEALL            = 0x8000f009,
    NVTRET_ENTER_MENU           = 0x8000f00a,
    NVTRET_FORMAT               = 0x8000f00b,
    NVTRET_WAITMOMENT           = 0x8000f00c,
    NVTRET_WARNING              = 0x8000f00d,
    NVTRET_MOVIESIZE_ERROR              = 0x8000f00e,
    NVTRET_ADAS_ERROR              = 0x8000f00f,    
    NVTRET_FILE_ERROR              = 0x8000f010,    
    /* INSERT NEW EVENT HRER */

    NVTEVT_USERCMD_END          = USER_EVENT_MASK + 0x10000 -1, ///< Min value = 0x8000ffff
}
NVT_USER_CMD;

#endif
