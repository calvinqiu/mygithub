#ifndef _PRJCFG_DEMO_H_
#define _PRJCFG_DEMO_H_
#include "Type.h"
#include "SysKer.h"

//==============================================================================
//   GENERAL CONFIG
//==============================================================================
//..............................................................................
// FW Partial load
#define FW_PARTIAL_LOAD                 DISABLE

//..............................................................................
// FW version and name
#define FW_UPDATE_NAME                  "A:\\FW96655A.bin"
#define FW_VERSION_RELEASE              DISABLE //ENABLE
#define FW_VERSION_NUM                  "GSDEMO_0001_0027"

//..............................................................................
// Memory Partition
#define MEM_DRAM_ADDR                   0x80000000  //cached region of MIPS
#define MEM_DRAM_SIZE                   ALIGN_CEIL_32(0x20000000) //DRAM size = 512MB (for FPGA)


//..............................................................................
// CPU2 Memory Partition (User MUST design own's memory to meet below addresses)
/**
    COREMEM_ECOS_ADDR: The address depends on ecos firmware built.
    COREMEM_LINUX_ADDR: The address depends on linux-kernel firmware built.
    COREMEM_LINUX_SUBADDR_ROOTFS: This address should be larger than linux-kernel code section and in range of linux-handled memory.
    COREMEM_LINUX_SUBADDR_UIMAGE: uImage is compressed linux kernel. Put its address above u-boot and do not overlap root-fs's one.
    COREMEM_LINUX_SUBADDR_UBOOT: The address depends on u-boot firmware built.
*/
#if (MEM_DRAM_SIZE == ALIGN_CEIL_32(0x20000000))
#if (_CPU2_TYPE_==_CPU2_ECOS_ && _DSP_TYPE_== _DSP_NONE_)
#define COREMEM_ECOS_ADDR               0x9E800000
#define COREMEM_ECOS_SIZE               0x01800000
#elif (_CPU2_TYPE_==_CPU2_ECOS_ && _DSP_TYPE_== _DSP_FREERTOS_)
#define COREMEM_ECOS_ADDR               0x9E800000
#define COREMEM_ECOS_SIZE               0x01000000
#define COREMEM_DSP_ADDR                0x9F800000
#define COREMEM_DSP_SIZE                0x00800000
#elif (_CPU2_TYPE_==_CPU2_LINUX_ && _DSP_TYPE_== _DSP_NONE_)
#define COREMEM_LINUX_ADDR              0x90000000
#define COREMEM_LINUX_SIZE              0x10000000
//Sub-Address in address range of linux
#define COREMEM_LINUX_SUBADDR_ROOTFS    0x91000000
#define COREMEM_LINUX_SUBADDR_UIMAGE    0x96000000
#define COREMEM_LINUX_SUBADDR_UBOOT     0x97000000
#elif (_CPU2_TYPE_==_CPU2_LINUX_ && _DSP_TYPE_== _DSP_FREERTOS_)
#define COREMEM_LINUX_ADDR              0x90000000
#define COREMEM_LINUX_SIZE              0x0F800000
#define COREMEM_DSP_ADDR                0x9F800000
#define COREMEM_DSP_SIZE                0x00800000
//Sub-Address in address range of linux
#define COREMEM_LINUX_SUBADDR_ROOTFS    0x91000000
#define COREMEM_LINUX_SUBADDR_UIMAGE    0x96000000
#define COREMEM_LINUX_SUBADDR_UBOOT     0x97000000
#elif (_CPU2_TYPE_==_CPU2_NONE_ && _DSP_TYPE_== _DSP_FREERTOS_)
#define COREMEM_DSP_ADDR                0x9F800000
#define COREMEM_DSP_SIZE                0x00800000
#endif
#elif (MEM_DRAM_SIZE == ALIGN_CEIL_32(0x10000000))
#if (_CPU2_TYPE_==_CPU2_ECOS_ && _DSP_TYPE_== _DSP_NONE_)
#define COREMEM_ECOS_ADDR               0x8E800000
#define COREMEM_ECOS_SIZE               0x01800000
#elif (_CPU2_TYPE_==_CPU2_ECOS_ && _DSP_TYPE_== _DSP_FREERTOS_)
#define COREMEM_ECOS_ADDR               0x8E800000
#define COREMEM_ECOS_SIZE               0x01000000
#define COREMEM_DSP_ADDR                0x8F800000
#define COREMEM_DSP_SIZE                0x00800000
#elif (_CPU2_TYPE_==_CPU2_LINUX_ && _DSP_TYPE_== _DSP_NONE_)
#define COREMEM_LINUX_ADDR              0x8C000000
#define COREMEM_LINUX_SIZE              0x04000000
//Sub-Address in address range of linux
#define COREMEM_LINUX_SUBADDR_ROOTFS    0x8D000000
#define COREMEM_LINUX_SUBADDR_UIMAGE    0x8F000000
#define COREMEM_LINUX_SUBADDR_UBOOT     0x8F400000
#elif (_CPU2_TYPE_==_CPU2_LINUX_ && _DSP_TYPE_== _DSP_FREERTOS_)
#define COREMEM_LINUX_ADDR              0x8C000000
#define COREMEM_LINUX_SIZE              0x03800000
#define COREMEM_DSP_ADDR                0x8F800000
#define COREMEM_DSP_SIZE                0x00800000
//Sub-Address in address range of linux
#define COREMEM_LINUX_SUBADDR_ROOTFS    0x8D000000
#define COREMEM_LINUX_SUBADDR_UIMAGE    0x8F000000
#define COREMEM_LINUX_SUBADDR_UBOOT     0x8F400000
#elif (_CPU2_TYPE_==_CPU2_NONE_ && _DSP_TYPE_== _DSP_FREERTOS_)
#define COREMEM_DSP_ADDR                0x8F800000
#define COREMEM_DSP_SIZE                0x00800000
#endif
#endif

//..............................................................................
/**
     The definition of EmbMem is the storage without any card detection,
     which means the end-user cannot take that storage out. The EmbMem can also
     be a combination of partition such as partition 0 is spi-uITRON,
     partition 1 is ram-FAT these depend on what you designed on DxStorage_Mem.c
     Normally, in SPI case, we design the ram-disk as internal FAT

*/
// Internal Storage Mapping to DxStorage
#define USER_DX_TYPE_EMBMEM_UITRON      DX_TYPE_EMBMEM0 //STRG_OBJ_FW_RSV1
#define USER_DX_TYPE_EMBMEM_ECOS        DX_TYPE_EMBMEM1 //STRG_OBJ_FW_RSV2
#define USER_DX_TYPE_EMBMEM_UBOOT       DX_TYPE_EMBMEM1 //STRG_OBJ_FW_RSV2
#define USER_DX_TYPE_EMBMEM_LINUX       DX_TYPE_EMBMEM2 //STRG_OBJ_FW_RSV3
#define USER_DX_TYPE_EMBMEM_ROOTFS      DX_TYPE_EMBMEM3 //STRG_OBJ_FW_RSV4
#define USER_DX_TYPE_EMBMEM_DSP         DX_TYPE_EMBMEM4 //STRG_OBJ_FW_RSV5
#define USER_DX_TYPE_EMBMEM_FAT         DX_TYPE_EMBMEM5 //STRG_OBJ_FAT1
#define USER_DX_TYPE_EMBMEM_PSTORE      DX_TYPE_EMBMEM6 //STRG_OBJ_PSTORE

//==============================================================================
//   SYSTEM FUNCTIONS
//==============================================================================
//..............................................................................
// Boot
#define POWERON_TRACE                   DISABLE //stop and wait for user enter cmd: "dsc boot"
#define WAITPHOTO_FUNCTION              ENABLE
#define POWERONLOGO_FUNCTION            DISABLE //ENABLE
#define POWEROFFLOGO_FUNCTION           DISABLE //ENABLE
#define POWERONSOUND_FUNCTION           ENABLE
#define POWEROFFSOUND_FUNCTION          ENABLE
#define USER_LOGO_FUNCTION              ENABLE
#define _LOGO_                          _LOGO_NOVATEK_
#define LOGO_DISP_LAYER                 LAYER_VDO1
#define LOGO_FREEZE                     ENABLE
#define LENSERROFF_FUNCTION             DISABLE

//..............................................................................
// Input Config
//key
#define POWERKEY_FUNCTION               ENABLE //Auto Detect
#define NORMALKEY_FUNCTION              ENABLE //Auto Detect
#define STATUSKEY_FUNCTION              ENABLE //Auto Detect
#define MODEKEY_FUNCTION                ENABLE //Auto Detect

// copy card to card function
#define COPYCARD2CARD_FUNCTION          DISABLE

//..............................................................................
// Power-Saving Config
//battery level
#define PWRLEVEL_FUNCTION               ENABLE //Auto Detect
#define LOWPWROFF_FUNCTION              DISABLE //Auto Detect
//battery temp
#define TEMPERATURE_FUNCTION            DISABLE //Auto Detect
//auto power-off when battery level = empty
#define EMPTYPWROFF_FUNCTION            ENABLE
//auto sleep
#define AUTOSLEEP_FUNCTION              DISABLE //Auto Detect
//auto power-off
#define AUTOPWROFF_FUNCTION             ENABLE //Auto Detect

//..............................................................................
// Date-Time Config
#define DEF_YEAR                        2014
#define DEF_MONTH                       1
#define DEF_DAY                         1
#define MIN_YEAR                        2000
#define MAX_YEAR                        2050

//..............................................................................
// Display Config (for LCD, LCD2, TV and HDMI device)

#define DISPLAY_FUNC                    ENABLE

#if (DISPLAY_FUNC == ENABLE)

#define LCD1_FUNC                       ENABLE //NOTE: LCD1 output from DOUT1 only
#define LCD2_FUNC                       DISABLE //NOTE: LCD2 output from DOUT2 only, (MUST must enable with LCD1_FUNC!!!)
#define TV_FUNC                         ENABLE //NOTE: TV output from DOUT1 or DOUT2
#define HDMI_FUNC                       ENABLE //NOTE: HDMI output from DOUT1 or DOUT2
#define DUALDISP_FUNC                   DISABLE
//LCD related
#define LCD_AUTORESET_FUNCTION          DISABLE //Auto Reset - LCD register
#define LCD_AUTOBACKLIGHT_FUNCTION      DISABLE //Auto Adjust Backlight - with AE LV
#define LCD_ROTATE_FUNCTION             DISABLE //Auto Detect - LCD rotate (NOTE! this is not machine rotate)
//TV related
#define TV_INSERT_FUNCTION              ENABLE //Auto Detect - TV cable insert
//HDMI related
#define HDMI_INSERT_FUNCTION            ENABLE //Auto Detect - HDMI cable insert

#else

#define LCD1_FUNC                       DISABLE
#define LCD2_FUNC                       DISABLE
#define TV_FUNC                         DISABLE
#define HDMI_FUNC                       DISABLE
#define DUALDISP_FUNC                   DISABLE
//LCD related
#define LCD_AUTORESET_FUNCTION          DISABLE
#define LCD_AUTOBACKLIGHT_FUNCTION      DISABLE
#define LCD_ROTATE_FUNCTION             DISABLE
//TV related
#define TV_INSERT_FUNCTION              DISABLE
//HDMI related
#define HDMI_INSERT_FUNCTION            DISABLE

#endif

// Display Config (for UI window, FD window)

// OSD size and format
#define DISPLAY_OSD_W                   320//640
#define DISPLAY_OSD_H                   240
#define DISPLAY_OSD_FMT                 DISP_PXLFMT_INDEX8 // 256 colors
#define DISPLAY_OSD_BPP                 8 // 8 for DISP_PXLFMT_INDEX8, 4 for DISP_PXLFMT_INDEX4
//#define DISPLAY_OSD_FMT                 DISP_PXLFMT_RGBA5658_PK // 16bpp-colors
//#define DISPLAY_OSD_BPP                 24 // 24 for DISP_PXLFMT_RGBA5658_PK

//UI tool layout size setting
#define TOOL_LAYOUT_W                   320//640
#define TOOL_LAYOUT_H                   240//480

#define OSD_USE_DOUBLE_BUFFER           ENABLE //use double buffer
#define OSD_USE_ROTATE_BUFFER           DISABLE //use rotate buffer

#define OSD2_USE_DOUBLE_BUFFER          ENABLE //use double buffer


// Display Config (for UI background, App quickview, photo frame, ...)

// VDO size and format
#define DISPLAY_VDO_W                   640
#define DISPLAY_VDO_H                   480
#define DISPLAY_VDO_FMT                 DISP_PXLFMT_YUV422_PK
#define DISPLAY_VDO_BPP                 16

#define VDO_USE_DOUBLE_BUFFER           DISABLE //use double buffer
#define VDO_USE_PHOTO_SOURCE            ENABLE  //display photo video streaming
#define VDO_USE_ROTATE_BUFFER           DISABLE //use rotate buffer

#define VDO2_USE_DOUBLE_BUFFER          ENABLE //use double buffer
#define VDO2_USE_ROTATE_BUFFER          DISABLE //use rotate buffer


// Display Config (for DispSrv; Photo App, Movie App and Play App)
// VDO1 size and format (for LCD device)
//LCD1:
#if (_LCDTYPE_ == _LCDTYPE_OFF_)
#define LCDMODE                         DISABLE
#elif (_LCDTYPE_ == _LCDTYPE_AUCN01_)
#define LCDMODE                         DISP_LCDMODE_RGBD320 //DISP_LCDMODE_YUV640
#elif (_LCDTYPE_ == _LCDTYPE_PW27P05_ILI8961_ || _LCDTYPE_ == _LCDTYPE_PW35P00_HX8238D_)
#define LCDMODE                         DISP_LCDMODE_RGB_SERIAL
#elif (_LCDTYPE_ == _LCDTYPE_NT35510_DSI_)
#define LCDMODE                         DISP_LCDMODE_MIPI
#else
#error "Unknown _LCDTYPE_"
#endif

// VDO1 size and format (for LCD2 device)
//LCD2:
#if (_LCD2TYPE_ == _LCDTYPE_OFF_)
#define LCD2MODE                        DISABLE
#elif (_LCD2TYPE_ == _LCD2TYPE_AUCN01_)
#define LCD2MODE                        DISP_LCDMODE_RGBD320 //DISP_LCDMODE_YUV640
#else
#error "Unknown _LCD2TYPE_"
#endif

// VDO1 size and format (for TV device)
//TV: DISP_TVMODE_PAL mode
#define TVMODE                          DISP_TVMODE_NTSC

// VDO1 size and format (for HDMI device)
//HDMI: DISP_HDMIMODE_1920X1080I60 mode
#define HDMIMODE                        DISP_HDMIMODE_720X480P60


#define VDO_BUFFER_SIZE_OPTIMIZATION    ENABLE //DISABLE //ENABLE for optimize buffer size; DISABLE for default buffer size

//#NT#2015/11/04#KCHong -begin
#define DUALCAM_PIP_BEHIND_FLIP         DISABLE  //Flip 2nd cam image in PIP view. Need extra 1/2 VDO buffer
//#NT#2015/11/04#KCHong -end

//..............................................................................
// External Storage Config
#define SDINSERT_FUNCTION               ENABLE //Auto Detect

//..............................................................................
// File Config

#define USE_FILEDB                      DISABLE //ENABLE
#define FILEDB_CARDV_ROOT               "Novatek"   // the max length should not exceeds 16
//..............................................................................
// DCF/EXIF Config
#define DCF_SUBPATH                     "NVTIM"
#define DCF_IMAGE_NAME                  "IMAG"
#define DCF_VIDEO_NAME                  "IMAG"
#define DCF_AUDIO_NAME                  "IMAG"
#define DCF_RAW_NAME                    "IMAG"
#define DCF_TIFF_NAME                   "IMAG"
#define DCF_SUPPORT_FORMAT              (DCF_FILE_TYPE_JPG|DCF_FILE_TYPE_AVI|DCF_FILE_TYPE_MOV|DCF_FILE_TYPE_MP4)
//..............................................................................
// EXIF Config
// End string must be '\0', total char must be 31
#define EXIF_MAKER_NAME                 "Novatek"
// End string must be '\0', total char must be 31
#define EXIF_MODEL_NAME                 "96660"
// End string must be '\0', total char must be 31
#define EXIF_IMAGE_DESCROPTOIN          "NOVATEK CAMERA"

#define EXIF_SOFTWARE_VERSION           FW_VERSION_NUM    // <= 20 char (TagSoftVerLen)

//..............................................................................
// Movie Maker/Model name Config
// End string must be '\0', total char must plus 1
#define MOVAPP_COMP_MANU_STRING         "NVT-IM"
// End string must be '\0', total char must plus 1
#define MOVAPP_COMP_NAME_STRING         "CarDV-TURNKEY"
//..............................................................................
// Usb Config

#define USBINSERT_FUNCTION              ENABLE //Auto Detect

#define USB_VID                         0x0603
#define USB_PID_PCCAM                   0x8612 // not support pc cam
#define USB_PID_WRITE                   0x8614
#define USB_PID_PRINT                   0x8613
#define USB_PID_MSDC                    0x8611

#define USB_PRODUCT_REVISION            '1', '.', '0', '0'
#define USB_VENDER_DESC_STRING          'N', 0x00,'O', 0x00,'V', 0x00,'A', 0x00,'T', 0x00,'E', 0x00,'K', 0x00, 0x20, 0x00,0x00, 0x00 // NULL
#define USB_VENDER_DESC_STRING_LEN      0x09
#define USB_PRODUCT_DESC_STRING         'J', 0x00,'1', 0x00,'4', 0x00,'5', 0x00,'5', 0x00, 0x20, 0x00,0x00, 0x00 // NULL
#define USB_PRODUCT_DESC_STRING_LEN     0x07
#define USB_PRODUCT_STRING              'N','v','t','-','D','S','C'
#define USB_SERIAL_NUM_STRING           '9', '6', '4', '6', '0','0', '0', '0', '0', '0','0', '0', '1', '0', '0'
#define USB_SIDC_DESC_STRING            'J',0x00,'1',0x00,'4',0x00,'5',0x00,'5',0x00,0x20, 0x00,0x00, 0x00 // NULL
#define USB_SIDC_DESC_STRING_LEN        0x07
#define USB_MTP_FRIENDNAME_STRING       'J','1','4','5','5',0x00  //maximum length is 253 bytes
#define USB_MTP_FRIENDNAME_STRING_LEN   0x06                      //maximum length is 253 bytes
#define USB_VENDER_STRING               'N','O','V','A','T','E','K'
#define USB_VENDER_SIDC_DESC_STRING     'N', 0x00,'O', 0x00,'V', 0x00,'A', 0x00,'T', 0x00,'E', 0x00,'K', 0x00, 0x20, 0x00,0x00, 0x00 // NULL
#define USB_VENDER_SIDC_DESC_STRING_LEN 0x09

//..............................................................................
// Other Config
#define GSENSOR_FUNCTION                DISABLE  //G-sensor
#define GPS_FUNCTION                    DISABLE
#define LONG_STARTUP                    DISABLE  // Enable/Disable pressing power button to startup system
#define SHOW_ADC_ON_SCREEN              DISABLE

//#NT#2015/11/04#KCHong -begin
//#NT# Compiler switch for waterlogo and multilingual function
#define WATERLOGO_FUNCTION              DISABLE  // Enable/Disable waterlogo function

// Important Notice:
//    If enable MULTILINGUAL_FUNCTION, remember to update font table and make sure the font height is not exceed the buffer size!!
#define MULTILINGUAL_FUNCTION           DISABLE  // Enable/Disable Multi-Lingual movie stamp function
//#NT#2015/11/04#KCHong -end

//---------------------------------------------------------------------------
// For Video Debug Info
//---------------------------------------------------------------------------
#define _MOVIE_IQLOG_                   DISABLE     // Enable/Disable Video IQLOG, Spark Chou

//==============================================================================
//   PLAYBACK FUNCTIONS
//==============================================================================
#define SLIDESHOW_MUSIC_OPTION          DISABLE //longer no maintain

//==============================================================================
//   PHOTO FUNCTIONS
//==============================================================================

#define CIPAIMPROVE_FUNCTION            DISABLE
#define ASSIST_BEAM_FUNC                DISABLE
#define GDC_FUNC                        DISABLE
#define AF_BEAM_EV_THRESHOLD            540
#define STAB_FUNC                       DISABLE
#define HDR_FUNC                        DISABLE
#define _FD_FUNC_                       DISABLE
#define MAX_FDNUM                       10
#define _SD_FUNC_                       DISABLE
#define PIP_VIEW_FUNC                   DISABLE
#define PIP_VIEW_FASTSWITCH             DISABLE  // display single sensor method (ENABLE: always display by Pipview, DISABLE: display by direct path)
#define SBS_VIEW_FUNC                   DISABLE // SbsView (ENABLE: display 2 sensor by SbsView, DISABLE: display user selected sensor)
#define SHDR_FUNC                       ENABLE
#define WDR_FUNC                        ENABLE

//..............................................................................
#define _SCREENNAIL_SIZE_               _SCREENNAIL_SIZE_VGA_
#define _QUICKVIEW_SIZE_                _QUICKVIEW_SIZE_SUIT_FOR_DISP

//..............................................................................
#define LENS_FUNCTION                   DISABLE
#define _LENSCTRLTSK1_                  ENABLE
#define _LENSCTRLTSK2_                  ENABLE
#define _IRIS_FUNC_                     ENABLE
#define LENS_PATCH_ZOOM_RESET           ENABLE

//..............................................................................
// Photo Frame Pstore Version Config
#define PF_PSTORE_VER                   0x00000105

//==============================================================================
//   MOVIE FUNCTIONS
//==============================================================================
#define MOVIE_DIS                       DISABLE
#define MOVIE_RSC                       DISABLE
#define MOVIE_3DNR                      DISABLE
#define MOVIE_PIM_CAPTURE               DISABLE // picture in movie method 1 (capture full resolution image)
#define MOVIE_PIM_SNAPSHOT              DISABLE // picture in movie method 2 (snapshot the same resolution image)
#define MOVIE_GDC_FUNC                  DISABLE
#define MOVIE_SMEAR_R_FUNC              DISABLE
#define MOVIE_ENSURE_SD_CLASS4          DISABLE
#define MOVIE_ENSURE_SD_32KCLUSTER      DISABLE
#define MOVIE_MULTI_RECORD_FUNC         DISABLE
#define MOVIE_TEST_ENABLE               DISABLE
#define MOVIE_UPDATE_UI                 DISABLE
#define MOVIE_REC_YUVMERGE              DISABLE // slowly timelapse record mode, frame period >= 1S
#define MOVIE_AUTOREC_CYCLICTIME        DISABLE
#define MOVIE_AUTOREC_ACPLUG            DISABLE
#define MOVIE_AUTOREC_MOTIONDET         DISABLE
#define MOVIE_AUTOREC_MEDIASLOW         DISABLE
#define MOVIE_AUTOREC_MEDIAFULL         DISABLE

//==============================================================================
//   USBCAM FUNCTIONS
//==============================================================================

//==============================================================================
//   USBCHARGE FUNCTIONS
//==============================================================================
#define USB_CHARGE_FUNCTION             DISABLE
#define USB_CHARGE_VERIFY               DISABLE

//==============================================================================
//   USBDISK FUNCTIONS
//==============================================================================
#define _MSDC_MODE_                     _MSDC_SINGLE_

//==============================================================================
//   USBMSDC FUNCTIONS
//==============================================================================
#define MSDCVENDOR_NVT                  ENABLE

//==============================================================================
//   USBPRINT FUNCTIONS
//==============================================================================

//==============================================================================
//   USBPRINT FUNCTIONS
//==============================================================================

//==============================================================================
//   UI FUNCTIONS
//==============================================================================
//..............................................................................
// UI Resource Storage Config
#define _LANG_STORE_                    _INSIDECODE_
#define _JPG_STORE_                     _INSIDECODE_
#define SPECIAL_KEY_RESET_FUNC          DISABLE

//..............................................................................
#if (_INTERSTORAGE_ == _INTERSTORAGE_NONE_)
#define PST_FUNC                        DISABLE
#else
#define PST_FUNC                        ENABLE
#endif

//==============================================================================
//   NET FUNCTIONS
//==============================================================================
#define IP_LIMITE                       DISABLE
#define MAC_APPEN_SSID                  ENABLE
#define ACL_TIME                        15000 //minmum 15 sec
#define WIFI_APP_MANUFACTURER           "NVT"
#define WIFI_APP_MODLE                  "CarDV_WiFi"

#define HTTP_LIVEVIEW_FUNC              DISABLE
#define WIFI_AP_FUNC                    DISABLE //ENABLE
#define WIFI_FTP_FUNC                   DISABLE

#define HTTP_MJPG_W_4                   640
#define HTTP_MJPG_H_3                   480

#define HTTP_MJPG_W_16                  640
#define HTTP_MJPG_H_9                   360

#define AUTO_WIFI                       DISABLE

#define HTTP_LIVEVIEW                   1
#define RTSP_LIVEVIEW                   2
#define DUAL_REC_HTTP_LIVEVIEW          3
//#NT#2016/05/06#Jeah Yen -begin
//#NT#Support IOTP2P
#define IOTP2P_LIVEVIEW                 4
//#NT#2016/05/06#Jeah Yen -end
#define MOVIE_LIVEVIEW                  RTSP_LIVEVIEW

#if(WIFI_AP_FUNC==ENABLE)
#undef HTTP_LIVEVIEW_FUNC
#define HTTP_LIVEVIEW_FUNC              ENABLE

#if(MOVIE_LIVEVIEW==HTTP_LIVEVIEW)
#undef MOVIE_MULTI_RECORD_FUNC
#define MOVIE_MULTI_RECORD_FUNC             DISABLE
#else  //RTSP_LIVEVIEW and DUAL_REC_HTTP_LIVEVIEW
#undef MOVIE_MULTI_RECORD_FUNC
#define MOVIE_MULTI_RECORD_FUNC             ENABLE
#endif

#undef HTTP_MJPG_W_4
#undef HTTP_MJPG_H_3
#undef HTTP_MJPG_W_16
#undef HTTP_MJPG_H_9
#define HTTP_MJPG_W_4                   640
#define HTTP_MJPG_H_3                   480
#define HTTP_MJPG_W_16                  640
#define HTTP_MJPG_H_9                   360
#endif
#define XML_USE_APP_BUFFER             DISABLE

#define WIFI_APP_MANUFACTURER           "NVT"
#define WIFI_APP_MODLE                  "CarDV_WiFi"
#define WIFI_APP_DOWNLOAD_URL           "http://115.29.201.46:8020/download/filedesc.xml"

#define WIFI_FINALCAM_APP_STYLE         DISABLE
#define WIFI_BG_COLOR                   14
#endif //_PRJCFG_DEMO_H_
