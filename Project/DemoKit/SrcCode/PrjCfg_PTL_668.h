#ifndef _PRJCFG_DEMO_H_
#define _PRJCFG_DEMO_H_
#include "Type.h"
#include "SysKer.h"

//==============================================================================
//   GENERAL CONFIG
//==============================================================================
//..............................................................................
#if (_FW_PARTIAL_LOAD_ == _FW_PARTIAL_LOAD_ON_)
#define FW_PARTIAL_LOAD                 ENABLE  //< FW Partial load
#else
#define FW_PARTIAL_LOAD                 DISABLE //< FW Partial load
#endif

//..............................................................................
// FW version and name
#define FW_UPDATE_NAME                  "A:\\FW96660A.bin"
#define FW_VERSION_RELEASE              DISABLE //ENABLE
#define FW_VERSION_NUM                  "REGULAR_0223_OK1" //"GSDEMO_0508_1153"

//..............................................................................
// Memory Partition
#define MEM_DRAM_ADDR                   0x80000000  //cached region of MIPS
#define MEM_DRAM_SIZE                   ALIGN_CEIL_32(0x10000000) //DRAM size = 256MB (for EVB)


//..............................................................................
// CPU2 Memory Partition (User MUST design own's memory to meet below addresses)
/**
    COREMEM_ECOS_ADDR: The address depends on ecos firmware built.
    COREMEM_LINUX_ADDR: The address depends on linux-kernel firmware built.
    COREMEM_LINUX_SUBADDR_ROOTFS: for DRAM-FS, if no use this, set it to be DISABLE). This address should be larger than linux-kernel code section and in range of linux-handled memory.
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
#define COREMEM_ECOS_ADDR               0x8F600000
#define COREMEM_ECOS_SIZE               0x00A00000
#elif (_CPU2_TYPE_==_CPU2_ECOS_ && _DSP_TYPE_== _DSP_FREERTOS_)
#define COREMEM_ECOS_ADDR               0x8EE00000
#define COREMEM_ECOS_SIZE               0x00A00000
#define COREMEM_DSP_ADDR                0x8F800000
#define COREMEM_DSP_SIZE                0x00800000
#elif (_CPU2_TYPE_==_CPU2_LINUX_ && _DSP_TYPE_== _DSP_NONE_)
#define COREMEM_LINUX_ADDR              0x8DC00000
#define COREMEM_LINUX_SIZE              0x02400000
//Sub-Address in address range of linux
#define COREMEM_LINUX_SUBADDR_ROOTFS    DISABLE
#define COREMEM_LINUX_SUBADDR_UIMAGE    0x8F000000
#define COREMEM_LINUX_SUBADDR_UBOOT     0x8F400000
#elif (_CPU2_TYPE_==_CPU2_LINUX_ && _DSP_TYPE_== _DSP_FREERTOS_)
#define COREMEM_LINUX_ADDR              0x8DC00000
#define COREMEM_LINUX_SIZE              0x01C00000
#define COREMEM_DSP_ADDR                0x8F800000
#define COREMEM_DSP_SIZE                0x00800000
//Sub-Address in address range of linux
#define COREMEM_LINUX_SUBADDR_ROOTFS    DISABLE
#define COREMEM_LINUX_SUBADDR_UIMAGE    0x8F000000
#define COREMEM_LINUX_SUBADDR_UBOOT     0x8F400000
#elif (_CPU2_TYPE_==_CPU2_NONE_ && _DSP_TYPE_== _DSP_FREERTOS_)
#define COREMEM_DSP_ADDR                0x8F800000
#define COREMEM_DSP_SIZE                0x00800000
#endif
#elif (MEM_DRAM_SIZE == ALIGN_CEIL_32(0x8000000))
#if (_CPU2_TYPE_==_CPU2_ECOS_ && _DSP_TYPE_== _DSP_NONE_)
#define COREMEM_ECOS_ADDR               0x87600000
#define COREMEM_ECOS_SIZE               0x00A00000
#elif (_CPU2_TYPE_==_CPU2_ECOS_ && _DSP_TYPE_== _DSP_FREERTOS_)
#error "Not Support"
#elif (_CPU2_TYPE_==_CPU2_LINUX_ && _DSP_TYPE_== _DSP_NONE_)
#define COREMEM_LINUX_ADDR              0x85C00000
#define COREMEM_LINUX_SIZE              0x02400000
//Sub-Address in address range of linux
#define COREMEM_LINUX_SUBADDR_ROOTFS    DISABLE
#define COREMEM_LINUX_SUBADDR_UIMAGE    0x87000000
#define COREMEM_LINUX_SUBADDR_UBOOT     0x87400000
#elif (_CPU2_TYPE_==_CPU2_LINUX_ && _DSP_TYPE_== _DSP_FREERTOS_)
#error "Not Support"
#elif (_CPU2_TYPE_==_CPU2_NONE_ && _DSP_TYPE_== _DSP_FREERTOS_)
#error "Not Support"
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
#define POWERON_PERF                    ENABLE //enable performance benchmark
#define POWERON_TRACE                   DISABLE //stop and wait for user enter cmd: "dsc boot"
#define POWERON_FAST_BOOT               DISABLE
#define POWERON_FAST_CPU2_BOOT          DISABLE
#define POWERON_FAST_RECORD             DISABLE
#define POWERON_FAST_WIFI               DISABLE
#define WAITPHOTO_FUNCTION              ENABLE
#if (_MODEL_DSC_ == _MODEL_IPCAM1_EVB_ \
    || _MODEL_DSC_ == _MODEL_IPCAM1_ \
    || _MODEL_DSC_ == _MODEL_IPCAM2_ \
    || _MODEL_DSC_ == _MODEL_IPCAM3_)
#define POWERONLOGO_FUNCTION            DISABLE
#else
#define POWERONLOGO_FUNCTION            ENABLE
#endif
#define POWEROFFLOGO_FUNCTION           DISABLE //ENABLE
#if ((POWERON_FAST_BOOT == ENABLE) || (POWERON_FAST_RECORD == ENABLE))
#define POWERONSOUND_FUNCTION           DISABLE
#elif (_MODEL_DSC_ == _MODEL_IPCAM1_EVB_ \
    || _MODEL_DSC_ == _MODEL_IPCAM1_ \
    || _MODEL_DSC_ == _MODEL_IPCAM2_ \
    || _MODEL_DSC_ == _MODEL_IPCAM3_)
#define POWERONSOUND_FUNCTION           DISABLE
#else
#define POWERONSOUND_FUNCTION           ENABLE
#endif
#if (_MODEL_DSC_ == _MODEL_IPCAM1_EVB_ \
    || _MODEL_DSC_ == _MODEL_IPCAM1_ \
    || _MODEL_DSC_ == _MODEL_IPCAM2_ \
    || _MODEL_DSC_ == _MODEL_IPCAM3_)
#define POWEROFFSOUND_FUNCTION          DISABLE
#else
#define POWEROFFSOUND_FUNCTION          ENABLE
#endif
#define USER_LOGO_FUNCTION              ENABLE
#define _LOGO_                          _LOGO_NOVATEK_
#define LOGO_DISP_LAYER                 LAYER_VDO2
#define LOGO_FREEZE                     ENABLE
#define LENSERROFF_FUNCTION             DISABLE

#if (POWERON_PERF == ENABLE)
#define TM_BOOT_ARRAY(n)                static TIMEMARK tmbuf[(n)] = {0}
#define TM_BOOT_INIT(n)                 TM_Init(tmbuf, n)
#define TM_BOOT_BEGIN(a,b)              TM_Begin(a,b)
#define TM_BOOT_END(a,b)                TM_End(a,b)
#else
#define TM_BOOT_ARRAY(n)
#define TM_BOOT_INIT(n)
#define TM_BOOT_BEGIN(a,b)
#define TM_BOOT_END(a,b)
#endif

//..............................................................................
// Input Config
//key
#define POWERKEY_FUNCTION               ENABLE //Auto Detect
#define NORMALKEY_FUNCTION              ENABLE //Auto Detect
#define STATUSKEY_FUNCTION              DISABLE //Auto Detect
#define MODEKEY_FUNCTION                DISABLE //Auto Detect

//..............................................................................
// Output Config
//LED
#define LED_FUNCTION                    DISABLE //Auto Detect

//..............................................................................
// Power-Saving Config
//battery level
#if(_MODEL_DSC_ == _MODEL_DEMO1_)
#define PWRLEVEL_FUNCTION               ENABLE //Auto Detect
#else
#define PWRLEVEL_FUNCTION               DISABLE //Auto Detect
#endif
//auto power-off when battery level = empty
#define EMPTYPWROFF_FUNCTION            ENABLE
//auto sleep
#define AUTOSLEEP_FUNCTION              ENABLE //Auto Detect
//auto power-off
#define AUTOPWROFF_FUNCTION             ENABLE //Auto Detect

//..............................................................................
// Date-Time Config
#define DEF_YEAR                        2015
#define DEF_MONTH                       1
#define DEF_DAY                         1
#define MIN_YEAR                        2000
#define MAX_YEAR                        2050


//..............................................................................
// Sensor Config (for 1,2,4 devices)

//#NT#2016/10/18#Jeah Yen -begin
//#NT#support sensor map
#undef  IPL_PATH_1
#undef  IPL_PATH_2
#undef  IPL_PATH_3
#undef  IPL_PATH_4
#if  ((_SENSORLIB_ != _SENSORLIB_OFF_) && (_SENSORLIB2_ == _SENSORLIB2_OFF_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_))
#define SENSOR_CAPS_COUNT               1
#define SENSOR_CAPS_MASK                (SENSOR_1)
#define IPL_PATH_1                      SENSOR_ID_1
#define IPL_PATH_2                      NA
#define IPL_PATH_3                      NA
#define IPL_PATH_4                      NA
#elif ((_SENSORLIB_ == _SENSORLIB_OFF_) && (_SENSORLIB2_ != _SENSORLIB2_OFF_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_))
#define SENSOR_CAPS_COUNT               1
#define SENSOR_CAPS_MASK                (SENSOR_1)
#define IPL_PATH_1                      SENSOR_ID_2
#define IPL_PATH_2                      NA
#define IPL_PATH_3                      NA
#define IPL_PATH_4                      NA
#elif  ((_SENSORLIB_ == _SENSORLIB_OFF_) && (_SENSORLIB2_ == _SENSORLIB2_OFF_) && (_SENSORLIB3_ != _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_))
#define SENSOR_CAPS_COUNT               1
#define SENSOR_CAPS_MASK                (SENSOR_1)
#define IPL_PATH_1                      SENSOR_ID_3
#define IPL_PATH_2                      NA
#define IPL_PATH_3                      NA
#define IPL_PATH_4                      NA
#elif  ((_SENSORLIB_ == _SENSORLIB_OFF_) && (_SENSORLIB2_ == _SENSORLIB2_OFF_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ != _SENSORLIB4_OFF_))
#define SENSOR_CAPS_COUNT               1
#define SENSOR_CAPS_MASK                (SENSOR_1)
#define IPL_PATH_1                      SENSOR_ID_4
#define IPL_PATH_2                      NA
#define IPL_PATH_3                      NA
#define IPL_PATH_4                      NA
#elif ((_SENSORLIB_ != _SENSORLIB_OFF_) && (_SENSORLIB2_ != _SENSORLIB2_OFF_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_))
#define SENSOR_CAPS_COUNT               2
#define SENSOR_CAPS_MASK                (SENSOR_1|SENSOR_2)
#define IPL_PATH_1                      SENSOR_ID_1
#define IPL_PATH_2                      SENSOR_ID_2
#define IPL_PATH_3                      NA
#define IPL_PATH_4                      NA
#elif ((_SENSORLIB_ != _SENSORLIB_OFF_) && (_SENSORLIB2_ == _SENSORLIB2_OFF_) && (_SENSORLIB3_ != _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_))
#define SENSOR_CAPS_COUNT               2
#define SENSOR_CAPS_MASK                (SENSOR_1|SENSOR_2)
#define IPL_PATH_1                      SENSOR_ID_1
#define IPL_PATH_2                      SENSOR_ID_3
#define IPL_PATH_3                      NA
#define IPL_PATH_4                      NA
#elif ((_SENSORLIB_ != _SENSORLIB_OFF_) && (_SENSORLIB2_ == _SENSORLIB2_OFF_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ != _SENSORLIB4_OFF_))
#define SENSOR_CAPS_COUNT               2
#define SENSOR_CAPS_MASK                (SENSOR_1|SENSOR_2)
#define IPL_PATH_1                      SENSOR_ID_1
#define IPL_PATH_2                      SENSOR_ID_4
#define IPL_PATH_3                      NA
#define IPL_PATH_4                      NA
#elif ((_SENSORLIB_ == _SENSORLIB_OFF_) && (_SENSORLIB2_ != _SENSORLIB2_OFF_) && (_SENSORLIB3_ != _SENSORLIB3_OFF_) && (_SENSORLIB4_ == _SENSORLIB4_OFF_))
#define SENSOR_CAPS_COUNT               2
#define SENSOR_CAPS_MASK                (SENSOR_1|SENSOR_2)
#define IPL_PATH_1                      SENSOR_ID_2
#define IPL_PATH_2                      SENSOR_ID_3
#define IPL_PATH_3                      NA
#define IPL_PATH_4                      NA
#elif ((_SENSORLIB_ == _SENSORLIB_OFF_) && (_SENSORLIB2_ != _SENSORLIB2_OFF_) && (_SENSORLIB3_ == _SENSORLIB3_OFF_) && (_SENSORLIB4_ != _SENSORLIB4_OFF_))
#define SENSOR_CAPS_COUNT               2
#define SENSOR_CAPS_MASK                (SENSOR_1|SENSOR_2)
#define IPL_PATH_1                      SENSOR_ID_2
#define IPL_PATH_2                      SENSOR_ID_4
#define IPL_PATH_3                      NA
#define IPL_PATH_4                      NA
#elif ((_SENSORLIB_ == _SENSORLIB_OFF_) && (_SENSORLIB2_ == _SENSORLIB2_OFF_) && (_SENSORLIB3_ != _SENSORLIB3_OFF_) && (_SENSORLIB4_ != _SENSORLIB4_OFF_))
#define SENSOR_CAPS_COUNT               2
#define SENSOR_CAPS_MASK                (SENSOR_1|SENSOR_2)
#define IPL_PATH_1                      SENSOR_ID_3
#define IPL_PATH_2                      SENSOR_ID_4
#define IPL_PATH_3                      NA
#define IPL_PATH_4                      NA
#elif ((_SENSORLIB_ != _SENSORLIB_OFF_) && (_SENSORLIB2_ != _SENSORLIB2_OFF_) && (_SENSORLIB3_ != _SENSORLIB3_OFF_) && (_SENSORLIB4_ != _SENSORLIB4_OFF_))
#define SENSOR_CAPS_COUNT               4
#define SENSOR_CAPS_MASK                (SENSOR_1|SENSOR_2|SENSOR_3|SENSOR_4)
#define IPL_PATH_1                      SENSOR_ID_1
#define IPL_PATH_2                      SENSOR_ID_2
#define IPL_PATH_3                      SENSOR_ID_3
#define IPL_PATH_4                      SENSOR_ID_4
#else
#warning Not support this sensor config combination!
#warning Please check your "SENSOR" config in include "ModelConfig.txt"
#error (see above)
#endif
//#NT#2016/10/18#Jeah Yen -end


// Sensor Insert Mask
#if ((_SENSORLIB_ != _SENSORLIB_OFF_) && (_SENSORLIB2_ != _SENSORLIB2_OFF_))
#define SENSOR_INSERT_MASK              (SENSOR_2) // support insert sensor
#else
#define SENSOR_INSERT_MASK              (SENSOR_1) // support insert sensor
#endif
//#define SENSOR_INSERT_MASK              (0) // if NOT support insert sensor, please choose this

#define SENSOR_DEFAULT_ATTACH_MASK      (SENSOR_CAPS_MASK & ~(SENSOR_INSERT_MASK)) // hw default attach sensors

#define SENSOR_INSERT_FUNCTION          ((SENSOR_CAPS_MASK == SENSOR_DEFAULT_ATTACH_MASK)?(DISABLE):(ENABLE)) //Auto Detect (Dynamic attach)

#define SENSOR_DEFAULT_DISPLAY_MASK     (SENSOR_1)


//..............................................................................
// Flashlight Config

#define FLASHLIGHT_FUNCTION             DISABLE //DISABLE


//..............................................................................
// Lens Config

#define LENS_FUNCTION                   DISABLE
#define _LENSCTRLTSK1_                  ENABLE
#define _LENSCTRLTSK2_                  ENABLE
#define _IRIS_FUNC_                     ENABLE
#define LENS_PATCH_ZOOM_RESET           ENABLE


//..............................................................................
// Display Config (for LCD, LCD2, TV and HDMI device)

//#NT#2016/06/01#Jeah Yen -begin
//#NT#Support LCD2 as status display
#define LCD2_FUNC_USERSTATUS            DISABLE //use LCD2 as status display. (not for video display)
//#NT#2016/06/01#Jeah Yen -end

//NOTE: LCD1 output from DOUT1 only
#if (_LCDTYPE_ == _LCDTYPE_OFF_)
#define LCD1_FUNC                       ENABLE
#define LCD1_MASK                       0x00
#else
#define LCD1_FUNC                       ENABLE
#define LCD1_MASK                       0x01
#endif
//NOTE: TV output from DOUT1 or DOUT2
#if (_TVTYPE_ == _TV_OFF_)
#define TV_FUNC                         DISABLE
#define TV_MASK                         0x00
#else
#define TV_FUNC                         ENABLE
#define TV_MASK                         0x02
#endif
//NOTE: HDMI output from DOUT1 or DOUT2
#if (_HDMITYPE_ == _HDMI_OFF_)
#define HDMI_FUNC                       DISABLE
#define HDMI_MASK                       0x00
#else
#define HDMI_FUNC                       ENABLE
#define HDMI_MASK                       0x04
#endif
//NOTE: LCD2 output from DOUT2 only
#if (_LCD2TYPE_ == _LCD2TYPE_OFF_)
#define LCD2_FUNC                       DISABLE
#define LCD2_MASK                       0x00
#else
#define LCD2_FUNC                       ENABLE
#define LCD2_MASK                       0x08
#endif

//#NT#2016/06/01#Jeah Yen -begin
//#NT#Support LCD2 as status display
#if (LCD2_FUNC_USERSTATUS == ENABLE)
#if(_LCD2TYPE_ == _LCD2TYPE_OFF_)
#warning LCD2 is not exist?
#warning Please assign your "LCD2" device in include "ModelConfig.txt"
#error (see above)
#endif
#undef LCD2_FUNC
#undef LCD2_MASK
#define LCD2_FUNC                       DISABLE
#define LCD2_MASK                       0x00
#endif
//#NT#2016/06/01#Jeah Yen -end

#define DISPLAY_CAPS_COUNT              2 //1: only single display, 2: allow single display or dual display
#define DISPLAY_CAPS_MASK               (LCD1_MASK|TV_MASK|HDMI_MASK|LCD2_MASK)
#define DUALDISP_FUNC                   ((DISPLAY_CAPS_COUNT == 2)?ENABLE:DISABLE)

#define DISPLAY_DEFAULT_ATTACH_MASK     DISPLAY_1 // hw default attach display
#if (_MODEL_DSC_ == _MODEL_IPCAM1_EVB_ \
    || _MODEL_DSC_ == _MODEL_IPCAM1_ \
    || _MODEL_DSC_ == _MODEL_IPCAM2_ \
    || _MODEL_DSC_ == _MODEL_IPCAM3_)
#define DISPLAY_DEFAULT_ATTACH_DEVICE   LCD1_MASK
//#define DISPLAY_DEFAULT_ATTACH_DEVICE   TV_MASK
#endif

//LCD related
#define LCD_AUTORESET_FUNCTION          DISABLE //Auto Reset - LCD register
#define LCD_AUTOBACKLIGHT_FUNCTION      DISABLE //Auto Adjust Backlight - with AE LV
#define LCD_ROTATE_FUNCTION             DISABLE //Auto Detect - LCD rotate (enable to support LCD body mirror or flip)
//TV related
#define TV_INSERT_FUNCTION              TV_FUNC //Auto Detect - TV cable insert
//HDMI related
#define HDMI_INSERT_FUNCTION            HDMI_FUNC //Auto Detect - HDMI cable insert
//LCD2 related
#define LCD2_INSERT_FUNCTION            LCD2_FUNC //Auto Detect - LCD2 cable insert


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
#if (_LCDTYPE_ == _LCDTYPE_ILI9341_IF8B_)
#define OSD_USE_ROTATE_BUFFER           ENABLE  //use rotate buffer (enable to support LCD with stripe-type subpixel)
#else
#define OSD_USE_ROTATE_BUFFER           DISABLE //use rotate buffer (enable to support LCD with stripe-type subpixel)
#endif

#define OSD2_USE_DOUBLE_BUFFER          ENABLE //use double buffer


// Display Config (for UI background, App quickview, photo frame, ...)

// VDO size and format
#define DISPLAY_VDO_W                   640
#define DISPLAY_VDO_H                   480
#define DISPLAY_VDO_FMT                 DISP_PXLFMT_YUV422_PK
#define DISPLAY_VDO_BPP                 16

#define VDO_USE_DOUBLE_BUFFER           DISABLE //use double buffer
#define VDO_USE_PHOTO_SOURCE            ENABLE  //display photo video streaming
#if (_LCDTYPE_ == _LCDTYPE_ILI9341_IF8B_)
#define VDO_USE_ROTATE_BUFFER           ENABLE  //use rotate buffer (enable to support LCD with stripe-type subpixel)
#else
#define VDO_USE_ROTATE_BUFFER           DISABLE //use rotate buffer (enable to support LCD with stripe-type subpixel)
#endif

#define VDO2_USE_DOUBLE_BUFFER          ENABLE //use double buffer
#if (_LCDTYPE_ == _LCDTYPE_ILI9341_IF8B_)
#define VDO2_USE_ROTATE_BUFFER          ENABLE  //use rotate buffer (enable to support LCD with stripe-type subpixel)
#else
#define VDO2_USE_ROTATE_BUFFER          DISABLE //use rotate buffer (enable to support LCD with stripe-type subpixel)
#endif

#define LCD_PREVIEW_VDO_FMT             GX_IMAGE_PIXEL_FMT_YUV420_PACKED
#define LCD2_PREVIEW_VDO_FMT            GX_IMAGE_PIXEL_FMT_YUV420_PACKED
#define TV_PREVIEW_VDO_FMT              GX_IMAGE_PIXEL_FMT_YUV420_PACKED
#define HDMI_PREVIEW_VDO_FMT            GX_IMAGE_PIXEL_FMT_YUV420_PACKED

// Display Config (for DispSrv; Photo App, Movie App and Play App)
// VDO1 size and format (for LCD device)
//LCD1:
#if (_LCDTYPE_ == _LCDTYPE_OFF_)
#define LCDMODE                         DISABLE
#elif (_LCDTYPE_ == _LCDTYPE_AUCN01_ \
    || _LCDTYPE_ == _LCDTYPE_PW27P05_ILI8961_ )
#define LCDMODE                         DISP_LCDMODE_RGBD320 //DISP_LCDMODE_YUV640
#elif (_LCDTYPE_ == _LCDTYPE_PW35P00_HX8238D_ \
    || _LCDTYPE_ == _LCDTYPE_ILI9341_IF8B_ )
#define LCDMODE                         DISP_LCDMODE_RGB_SERIAL
#elif (_LCDTYPE_ == _LCDTYPE_T15P00_OTA5182A_)
#define LCDMODE                         DISP_LCDMODE_RGBD320
#elif (_LCDTYPE_ == _LCDTYPE_NT35510_DSI_ \
    || _LCDTYPE_ == _LCDTYPE_NT35410_DSI_)
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
#define HDMIMODE                        DISP_HDMIMODE_1920X1080I60


#define VDO_BUFFER_SIZE_OPTIMIZATION    ENABLE //DISABLE //ENABLE for optimize buffer size; DISABLE for default buffer size

//#NT#2015/11/04#KCHong -begin
#define DUALCAM_PIP_BEHIND_FLIP         DISABLE  //Flip 2nd cam image in PIP view. Need extra 1/2 VDO buffer
//#NT#2015/11/04#KCHong -end



//..............................................................................
// External Audio Config (for LineOut device)

//LINE related
#if (_MODEL_DSC_ == _MODEL_IPCAM1_EVB_ \
    || _MODEL_DSC_ == _MODEL_IPCAM1_ \
    || _MODEL_DSC_ == _MODEL_IPCAM2_ \
    || _MODEL_DSC_ == _MODEL_IPCAM3_)
#define SPK_MASK                        0x01
#define LOUT_MASK                       0x02
#define LINE_INSERT_FUNCTION            ENABLE //Auto Detect - LINE cable insert (audio out)
#define AOUT_DEFAULT_ATTACH_DEVICE      (SPK_MASK)
#define AUDIO_PLAY_FUNCTION             ENABLE //Auto Play
#endif

//..............................................................................
// External Storage Config
#define SDINSERT_FUNCTION               ENABLE //Auto Detect
//..............................................................................
// File Config

#define USE_FILEDB                      ENABLE //DISABLE
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
#define MOVAPP_COMP_MANU_STRING         "NOVATEK"
// End string must be '\0', total char must plus 1
#define MOVAPP_COMP_NAME_STRING         "DEMO1"
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
#define USB_PRODUCT_DESC_STRING         'D', 0x00,'E', 0x00,'M', 0x00,'O', 0x00,'1', 0x00, 0x20, 0x00, 0x00, 0x00 // NULL
#define USB_PRODUCT_DESC_STRING_LEN     0x07
#define USB_PRODUCT_STRING              'N','v','t','-','D','S','C'
#define USB_SERIAL_NUM_STRING           '9', '6', '6', '6', '0','0', '0', '0', '0', '0','0', '0', '1', '0', '0'
#define USB_SIDC_DESC_STRING            'D', 0x00,'E', 0x00,'M', 0x00,'O', 0x00,'1', 0x00, 0x20, 0x00, 0x00, 0x00 // NULL
#define USB_SIDC_DESC_STRING_LEN        0x07
#define USB_MTP_FRIENDNAME_STRING       'D','E','M','O','1',0x00  //maximum length is 253 bytes
#define USB_MTP_FRIENDNAME_STRING_LEN   0x06                      //maximum length is 253 bytes
#define USB_VENDER_STRING               'N','O','V','A','T','E','K'
#define USB_VENDER_SIDC_DESC_STRING     'N', 0x00,'O', 0x00,'V', 0x00,'A', 0x00,'T', 0x00,'E', 0x00,'K', 0x00, 0x20, 0x00,0x00, 0x00 // NULL
#define USB_VENDER_SIDC_DESC_STRING_LEN 0x09

//..............................................................................
// Other Config
#if(_MODEL_DSC_ == _MODEL_DEMO1_)
#define GSENSOR_FUNCTION                ENABLE  //G-sensor
#else
#define GSENSOR_FUNCTION                DISABLE  //G-sensor
#endif

#if (_GPS_TYPE_ == _GPS_GENERAL_)
#define GPS_FUNCTION                    ENABLE
#else
#define GPS_FUNCTION                    DISABLE
#endif

#define LONG_STARTUP                    DISABLE  // Enable/Disable pressing power button to startup system
#define SHOW_ADC_ON_SCREEN              DISABLE
#define CALIBRATION_FUNC                DISABLE  // Enable/Disable Calibration

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

//#NT#2016/02/26#Niven Cho -begin
//#NT#eCos debug msg alway show on uITRON via IPC.
#define ECOS_DBG_MSG_FORCE_UART1_DIRECT     DISABLE
//#NT#2016/02/26#Niven Cho -end


//==============================================================================
//   MODES
//==============================================================================
#if (_MODEL_DSC_ == _MODEL_IPCAM1_EVB_ \
    || _MODEL_DSC_ == _MODEL_IPCAM1_ \
    || _MODEL_DSC_ == _MODEL_IPCAM2_ \
    || _MODEL_DSC_ == _MODEL_IPCAM3_)
#define PLAY_MODE                       DISABLE
#define PHOTO_MODE                      DISABLE
#define MOVIE_MODE                      ENABLE
#define USB_MODE                        DISABLE
#endif

//==============================================================================
//   PLAYBACK FUNCTIONS
//==============================================================================
#define SLIDESHOW_MUSIC_OPTION          DISABLE //longer no maintain
// copy card to card function
#define COPYCARD2CARD_FUNCTION          DISABLE

//==============================================================================
//   PHOTO FUNCTIONS
//==============================================================================

#define CIPAIMPROVE_FUNCTION            DISABLE
#define ASSIST_BEAM_FUNC                DISABLE
#define GDC_FUNC                        DISABLE
#define AF_BEAM_EV_THRESHOLD            540
#define STAB_FUNC                       DISABLE
#define HDR_FUNC                        DISABLE
#if (_MODEL_DSC_ == _MODEL_EVB_ \
    || _MODEL_DSC_ == _MODEL_DEMO1_ \
    || _MODEL_DSC_ == _MODEL_IPCAM2_EVB_\
    || _MODEL_DSC_ == _MODEL_PTL_668_)
#define _FD_FUNC_                       ENABLE
#elif (_MODEL_DSC_ == _MODEL_IPCAM1_EVB_ \
    || _MODEL_DSC_ == _MODEL_IPCAM1_ \
    || _MODEL_DSC_ == _MODEL_IPCAM2_\
    || _MODEL_DSC_ == _MODEL_IPCAM3_)
#define _FD_FUNC_                       DISABLE
#else
#define _FD_FUNC_                       ENABLE
#endif
#define MAX_FDNUM                       10
#define _SD_FUNC_                       DISABLE
#if (_MODEL_DSC_ == _MODEL_IPCAM1_EVB_ \
    || _MODEL_DSC_ == _MODEL_IPCAM1_ \
    || _MODEL_DSC_ == _MODEL_IPCAM2_ \
    || _MODEL_DSC_ == _MODEL_IPCAM3_)
#define PIP_VIEW_FUNC                   DISABLE
#define PIP_VIEW_FASTSWITCH             DISABLE
#else
#define PIP_VIEW_FUNC                   ENABLE  // PipView (ENABLE: display 2 sensor by PipView, DISABLE: display user selected sensor)
#define PIP_VIEW_FASTSWITCH             ENABLE  // display single sensor method (ENABLE: always display by Pipview, DISABLE: display by direct path)
#endif
#define SBS_VIEW_FUNC                   DISABLE // SbsView (ENABLE: display 2 sensor by SbsView, DISABLE: display user selected sensor)

#if (_IPPLIB_ == _IPL_OV4689_EVB_FF_ \
    || _IPPLIB_ == _IPL_OV4689_IMX322LQJ_EVB_FF_ \
    || _IPPLIB_ == _IPL_IMX290LQR_EVB_FF_ \
    || _IPPLIB_ == _IPL_IMX123LQT_EVB_FF_ \
    || _IPPLIB_ == _IPL_AR0230CS_EVB_FF_)
#define SHDR_FUNC                       ENABLE
#else
#define SHDR_FUNC                       DISABLE
#endif

#define WDR_FUNC                        ENABLE

#if (_GYRO_EXT_ != _GYRO_EXT_NONE_)
#define RSC_FUNC                        ENABLE
#else
#define RSC_FUNC                        DISABLE
#endif
//..............................................................................
#define _SCREENNAIL_SIZE_               _SCREENNAIL_SIZE_VGA_
#define _QUICKVIEW_SIZE_                _QUICKVIEW_SIZE_SUIT_FOR_DISP

//..............................................................................
// Photo Frame Pstore Version Config
#define PF_PSTORE_VER                   0x00000105

//==============================================================================
//   MOVIE FUNCTIONS
//==============================================================================
#if (_GYRO_EXT_ != _GYRO_EXT_NONE_)
#define MOVIE_RSC                       ENABLE
#else
#define MOVIE_RSC                       DISABLE
#endif
#define MOVIE_PIM_CAPTURE               DISABLE // picture in movie method 1 (capture full resolution image)
#define MOVIE_PIM_SNAPSHOT              ENABLE  // picture in movie method 2 (snapshot the same resolution image)
#define MOVIE_GDC_FUNC                  DISABLE
#define MOVIE_SMEAR_R_FUNC              DISABLE
#define MOVIE_ENSURE_SD_CLASS4          DISABLE
#define MOVIE_ENSURE_SD_32KCLUSTER      DISABLE
#define MOVIE_MULTI_RECORD_FUNC         ENABLE
#define MOVIE_TEST_ENABLE               DISABLE
#define MOVIE_UPDATE_UI                 DISABLE
#define MOVIE_REC_YUVMERGE              DISABLE // slowly timelapse record mode, frame period >= 1S
#define MOVIE_AUTOREC_CYCLICTIME        ENABLE
#define MOVIE_AUTOREC_ACPLUG            ENABLE
#define MOVIE_AUTOREC_MOTIONDET         DISABLE
#define MOVIE_AUTOREC_MEDIASLOW         DISABLE
#define MOVIE_AUTOREC_MEDIAFULL         DISABLE
#if (_MODEL_DSC_ == _MODEL_IPCAM1_EVB_ \
    || _MODEL_DSC_ == _MODEL_IPCAM1_ \
    || _MODEL_DSC_ == _MODEL_IPCAM2_ \
    || _MODEL_DSC_ == _MODEL_IPCAM3_)
#define MOVIE_DIS                       DISABLE
#define MOVIE_FD_FUNC_                  ENABLE
#define UCTRL_APP_MOVIE_FEATURE_SETGET  ENABLE
#define IPCAM_FUNC                      ENABLE
#define MEDIA_VASTREAM                  DISABLE
//#NT#2016/02/22#Niven Cho -begin
//#NT#add IR_CUT_FUNC to enable IR CUT function.
#define IR_CUT_FUNC                     ENABLE
//#NT#2016/02/22#Niven Cho -end
//#NT#2016/02/23#Lincy Lin -begin
//#NT#Support logfile function
#define LOGFILE_FUNC                    ENABLE
//#NT#2016/02/23#Lincy Lin -end
//CROP source width, height
#define CROP_SOURCE_W                   1920
#define CROP_SOURCE_H                   1088

#define MOVIE_PATH1_MAX_W               1920
#define MOVIE_PATH1_MAX_H               1088
#define MOVIE_PATH1_MAX_TBR             3000*1024
#define MOVIE_PATH2_MAX_W                640
#define MOVIE_PATH2_MAX_H                480
#define MOVIE_PATH2_MAX_TBR              400*1024
#define MOVIE_PATH3_MAX_W                640
#define MOVIE_PATH3_MAX_H                480
#define MOVIE_PATH3_MAX_TBR              400*1024


#else
#define MOVIE_DIS                       DISABLE
#define MOVIE_FD_FUNC_                  ENABLE
#define UCTRL_APP_MOVIE_FEATURE_SETGET  DISABLE
#define IPCAM_FUNC                      DISABLE
#define MEDIA_VASTREAM                  DISABLE
//#NT#2016/02/22#Niven Cho -begin
//#NT#add IR_CUT_FUNC to enable IR CUT function.
#define IR_CUT_FUNC                     DISABLE
//#NT#2016/02/22#Niven Cho -end
//#NT#2016/02/23#Lincy Lin -begin
//#NT#Support logfile function
#define LOGFILE_FUNC                    DISABLE
//#NT#2016/02/23#Lincy Lin -end
#endif

//==============================================================================
//   MOVIE PLAY FUNCTIONS
//==============================================================================
#define SCALE_VIEW_FUNC                 ENABLE // ScaleView


//==============================================================================
//   USBCAM FUNCTIONS
//==============================================================================
#define UVAC_MODE_2_PATH                DISABLE
#if (UVAC_MODE_2_PATH==ENABLE)
#undef MOVIE_MULTI_RECORD_FUNC
#define MOVIE_MULTI_RECORD_FUNC         ENABLE
#endif
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
#define MSDCVENDOR_UPDFW                DISABLE //enable it will cause disalbing MSDC and MTP/PTP/UAVC Mode, MSDCVENDOR_NVT must enable

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
#define EXTRA_MENU                      DISABLE

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
#define ONVIF_PROFILE_S                 DISABLE  //Enable it to cause NO CARD DETECTED.
#define ECOS_POWER_OFF_FLOW             ENABLE

#define HTTP_LIVEVIEW_FUNC              DISABLE
#if (_NETWORK_ == _NETWORK_NONE_)
#define WIFI_AP_FUNC                    DISABLE
#else
#if (IPCAM_FUNC)
#define WIFI_AP_FUNC                    DISABLE
#else
#define WIFI_AP_FUNC                    ENABLE
#endif
#endif
#define WIFI_FTP_FUNC                   DISABLE

#define HTTP_MJPG_W_4                   640
#define HTTP_MJPG_H_3                   480

#define HTTP_MJPG_W_16                  640
#define HTTP_MJPG_H_9                   360

#if (IPCAM_FUNC)
#define AUTO_WIFI                       ENABLE
#else
#define AUTO_WIFI                       DISABLE
#endif

#define HTTP_LIVEVIEW                   1
#define RTSP_LIVEVIEW                   2
#define MOVIE_LIVEVIEW                  RTSP_LIVEVIEW

#if(WIFI_AP_FUNC==ENABLE)
#undef HTTP_LIVEVIEW_FUNC
#define HTTP_LIVEVIEW_FUNC              ENABLE
#define MOVIE_MULTI_RECORD_FUNC         ENABLE


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


#if(_CPU2_TYPE_==_CPU2_LINUX_)
#define UCTRL_FUNC                      ENABLE
#if (_MODEL_DSC_ == _MODEL_IPCAM1_EVB_ \
    || _MODEL_DSC_ == _MODEL_IPCAM1_ \
    || _MODEL_DSC_ == _MODEL_IPCAM2_ \
    || _MODEL_DSC_ == _MODEL_IPCAM3_)
#define ONVIF_FUNC                      ENABLE
#else
#define ONVIF_FUNC                      DISABLE  //CarDV-Linux is no using of ONVIF
#endif
#else
#define UCTRL_FUNC                      DISABLE
#define ONVIF_FUNC                      DISABLE
#endif

#define WIFI_FINALCAM_APP_STYLE         DISABLE
#define WIFI_BG_COLOR                   14

#define WIFI_STA_RECONNECT_MAX_COUNT 5
#define WIFI_STA_RECONNECT_INTERVAL 150

#define YOUKU_SDK                       DISABLE
#if (YOUKU_SDK == ENABLE)
#define WIFI_MOVIE_FOLDER             "MOVIE_WIFI"
#define CUT_MOVIE_FOLDER              "MOVIE_CUT"
//the following two functions should be exclusive
#define SAVE_DUAL_STREAM_FUNC           ENABLE
#define CUT_MOVIE_FUNC                  DISABLE
#endif

#define _DDD_FUNC_                      ENABLE
#define MOVIE_DDD_FUNC                  ENABLE
#define MOVIE_DDD_FD_FUNC               DISABLE
#define MOVIE_DDD_DRAW_VIDEO            ENABLE
#define MOVIE_DDD_SAVE_LOG              DISABLE
#define DDD_MODEL_IN_PSTORE              ENABLE

#endif //_PRJCFG_DEMO_H_
