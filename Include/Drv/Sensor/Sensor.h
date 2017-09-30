/**
    Public header file for Sensor driver

    This file is the header file that define the API and data type for Sensor driver.

    @file       Sensor.h
    @ingroup    mIDrvSensor
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/
#ifndef _SENSOR_H
#define _SENSOR_H

//----------Header file include-------------------------------------------------
#include "Type.h"
#include "ErrorNo.h"
#include "SensorI2C.h"

/** \addtogroup mISensor */
//@{

#define SEN_IGNORE  0xffffffff  ///< sensor ignore information
#define SEN_MFRAME_MAX_NUM  2   ///< dps support max frame number
/**
    Sensor ID
*/
typedef enum
{
    SENSOR_ID_1 = 0,                ///< sensor id 1
    SENSOR_ID_2,                    ///< sensor id 2
    SENSOR_ID_3,                    ///< sensor id 3
    SENSOR_ID_4,                    ///< sensor id 4
    SENSOR_ID_MAX_NUM,
    SENSOR_ID_NONE = 0xffffffff,    ///< sensor id null
} SENSOR_ID;

/**
    sensor signal type
*/
typedef enum
{
    SENSOR_SIGNAL_MASTER = 0,       ///< sensor output HD and VD signal
    SENSOR_SIGNAL_SLAVE,            ///< dsp output HD and VD signal
    SENSOR_SIGNAL_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_SIGNAL_TYPE)
} SENSOR_SIGNAL_TYPE;

/**
    sensor type
*/
typedef enum
{
    SENSOR_TYPE_CMOS = 0,           ///< CMOS
    SENSOR_TYPE_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_TYPE)
} SENSOR_TYPE;

/**
    sensor mode type
*/
typedef enum
{
    SENSOR_MODE_LINEAR = 0,         ///< linear raw
    SENSOR_MODE_BUILTIN_HDR,        ///< bulid in HDR
    SENSOR_MODE_STAGGER_HDR,        ///< stagger HDR
    SENSOR_MODE_CCIR,               ///< DVI
    SENSOR_MODE_CCIR_INTERLACE,    ///< DVI INTERLACE
    SENSOR_MODE_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_MODE_TYPE)
} SENSOR_MODE_TYPE;

/**
    sensor output data type
*/
typedef enum
{
    SENSOR_DATA_PARALLEL = 0,       ///< parallel
    SENSOR_DATA_LVDS,               ///< LVDS
    SENSOR_DATA_MIPI,               ///< MIPI
    SENSOR_DATA_DUMMY,
    SENSOR_DATA_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_DATA_TYPE)
} SENSOR_DATA_TYPE;

/**
    sensor command interface
*/
typedef enum
{
    SENSOR_CMD_UNKNOWN = 0,
    SENSOR_CMD_Vx1,                 ///< Vx1
    SENSOR_CMD_SIF,                 ///< serial
    SENSOR_CMD_I2C,                 ///< I2C
    SENSOR_CMD_IO,                 ///< IO
    SENSOR_CMD_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_CMD_TYPE)
} SENSOR_CMD_TYPE;

/**
    sensor frame rate type
*/
typedef enum
{
    SENSOR_FPS_UNKNOWN = 0,
    SENSOR_FPS_MUL_FPS,            ///< multiple of the frame rate
    SENSOR_FPS_DEPEND_ON_EXPT,     ///< depend on exposure time
    SENSOR_FPS_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_FPS_TYPE)
} SENSOR_FPS_TYPE;

/**
    Sensor mode
*/
typedef enum _SENSOR_MODE
{
    SENSOR_MODE_UNKNOWN = 0,    ///< Unknow Mode
    SENSOR_MODE_1       = 1,    ///< Sensor mode 1
    SENSOR_MODE_2       = 2,    ///< Sensor mode 2
    SENSOR_MODE_3       = 3,    ///< Sensor mode 3
    SENSOR_MODE_4       = 4,    ///< Sensor mode 4
    SENSOR_MODE_5       = 5,    ///< Sensor mode 5
    SENSOR_MODE_6       = 6,    ///< Sensor mode 6
    SENSOR_MODE_7       = 7,    ///< Sensor mode 7
    SENSOR_MODE_8       = 8,    ///< Sensor mode 8
    SENSOR_MODE_9       = 9,    ///< Sensor mode 9
    SENSOR_MODE_10      = 10,   ///< Sensor mode 10
    SENSOR_MODE_11      = 11,   ///< Sensor mode 11
    SENSOR_MODE_12      = 12,   ///< Sensor mode 12
    SENSOR_MODE_13      = 13,   ///< Sensor mode 13
    SENSOR_MODE_14      = 14,   ///< Sensor mode 14
    SENSOR_MODE_15      = 15,   ///< Sensor mode 15
    SENSOR_MODE_Num,
    ENUM_DUMMY4WORD(SENSOR_MODE)
} SENSOR_MODE, *PSENSOR_MODE;

/**
    Sensor mode ratio
*/
typedef enum _SENSOR_RATIO
{
    SENSOR_RATIO_4_3 = 0,       ///< Sensor ratio 4:3
    SENSOR_RATIO_3_2,           ///< Sensor ratio 3:2
    SENSOR_RATIO_16_9,          ///< Sensor ratio 16:9
    SENSOR_RATIO_1_1,           ///< Sensor ratio 1:1
    SENSOR_RATIO_Num,
    ENUM_DUMMY4WORD(SENSOR_RATIO)
} SENSOR_RATIO, *PSENSOR_RATIO;

/**
    Sensor start pixel
*/
typedef enum
{
    SENSOR_STPIX_R = 0,         ///< start pixel R
    SENSOR_STPIX_GR,            ///< start pixel GR
    SENSOR_STPIX_GB,            ///< start pixel GB
    SENSOR_STPIX_B,             ///< start pixel B
    SENSOR_STPIX_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_STPIX)
} SENSOR_STPIX;

/**
    Sensor data format
*/
typedef enum
{
    SENSOR_FMT_POGRESSIVE = 0,      ///< pogressive mode
    SENOSR_FMT_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_FMT)
} SENSOR_FMT;


/**
    Sensor Act Select(for CCIR656 only)
*/
typedef enum
{
    SENSOR_DVI_SEL_EAV = 0,         ///< EAV code recognition
    SENSOR_DVI_SEL_ACTWIN,          ///< Active window size setting
    SENOSR_DVI_SEL_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_DVI_SEL)
} SENSOR_DVI_SEL;

/**
    Sensor DVI MODE
*/
typedef enum
{
    SENSOR_DVI_MODE_SD = 0,         ///< DVI sd mode (8 bit)
    SENSOR_DVI_MODE_HD,             ///< DVI hd mode (16 bit)
    SENSOR_DVI_MODE_HD_INV,         ///< DVI hd mode (16 bit) with Byte Inverse
    SENOSR_DVI_MODE_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_DVI_MODE)
} SENSOR_DVI_MODE;


/**
    Sensor DVI format
*/
typedef enum
{
    SENSOR_DVI_CCIR601 = 0,         ///< DVI CCIR 601
    SENSOR_DVI_CCIR656_EAV,         ///< DVI CCIR 656 EAV
    SENSOR_DVI_CCIR656_ACT,         ///< DVI CCIR 656 ACT
    SENOSR_DVI_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_DVI_FMT)
} SENSOR_DVI_FMT;

/**
    Sensor DVI IN format
*/
typedef enum _SENSOR_DVI_IN_FMT
{
    SENSOR_DVI_YUYV = 0,
    SENSOR_DVI_YVYU,
    SENSOR_DVI_UYVY,
    SENOSR_DVI_VYUY,
    ENUM_DUMMY4WORD(SENSOR_DVI_IN_FMT)
} SENSOR_DVI_IN_FMT;
/**
    Sensor Option
*/
typedef enum _SENSOR_OPTION
{
    SENSOR_OPTION_NONE           = 0x00000000,    ///< none
    SENSOR_OPTION_MIRROR         = 0x00000001,    ///< Sensor mirror
    SENSOR_OPTION_FLIP           = 0x00000002,    ///< Sensor flip
/* modify begin by ZMD, 2017-02-15 new version management*/
#if defined(YQCONFIG_ANDROID_SYSTEM_SUPPORT)
   #if ((_SENSORLIB_ == _SENSORLIB_CMOS_IMX291M_)||(_SENSORLIB_==_SENSORLIB_CMOS_AR0238CSP_))
    SENSOR_OPTION_FLIP_MIRROR           = 0x00000003,    ///< Sensor flip
	#endif
#endif

/* modify end by ZMD, 2017-02-15 */
    SENSOR_OPTION_USER           = 0x00000004,    ///< User option
    SENSOR_OPTION_MAX_NUM,
    ENUM_DUMMY4WORD(SENSOR_OPTION)
} SENSOR_OPTION, *PSENSOR_OPTION;

/**
    Sensor signal

    @note relative falling edge
*/
typedef struct
{
    UINT32 Sync;            ///< sync
    UINT32 Period;          ///< period
    UINT32 DataStart;       ///< valid data start pos
    UINT32 DataSize;        ///< valid data size
} SENSOR_SIGNAL;

/**
    Sensor OB position
    @note relative falling edge
*/
typedef struct
{
    UINT32 TopWin[4];          ///< OB window left-top point(X1(Pt[0]), Y1(Pt[1])), right-bottom point(X2(Pt[2]), Y2(Pt[3]))
    UINT32 BottomWin[4];       ///< OB window left-top point(X1(Pt[0]), Y1(Pt[1])), right-bottom point(X2(Pt[2]), Y2(Pt[3]))
    UINT32 LeftWin[4];         ///< OB window left-top point(X1(Pt[0]), Y1(Pt[1])), right-bottom point(X2(Pt[2]), Y2(Pt[3]))
    UINT32 RightWin[4];        ///< OB window left-top point(X1(Pt[0]), Y1(Pt[1])), right-bottom point(X2(Pt[2]), Y2(Pt[3]))
  } SENSOR_OB;

/**
    Sensor img ratio information
*/
typedef struct
{
    SENSOR_RATIO RatioHV;   ///< ratio hv (16:9......)
    UINT32 RatioH;          ///< ratio h (1x = 1000) (relative 1x image size) ((sensor output width x 1000) / (1x width))
    UINT32 RatioV;          ///< ratio v (1x = 1000) (relative 1x image size) ((sensor output height x 1000) / (1x height))
} SENSOR_IMG_RATIO;

/**
    multi-frame information
*/
typedef struct
{
    UINT32 TotalFmt;                         ///< total frame number
    UINT32 SelFmtId[SEN_MFRAME_MAX_NUM];     ///< select frame id
} SENSOR_SEL_IMG_ID;

/**
    Sensor lvds ctrl pattern
*/
typedef struct
{
    UINT32 CtrlHD;          ///< lvds ctrl words
    UINT32 MaskHD;          ///< lvds mask
    UINT32 CtrlVD;          ///< lvds ctrl words
    UINT32 MaskVD;          ///< lvds mask
} SENSOR_LVDS_CTRLPTN;


/**
    Sensor lvds sync code pattern
*/
typedef struct
{
    UINT32 Num;             ///< sync code total number
    UINT32 Code[7];         ///< sync code
} SENSOR_LVDS_SYNC_CODE_PTN;

/**
    Sensor lvds information
*/

typedef struct
{
    SENSOR_SIGNAL XHS;                  ///< HD length for sony sensor only. (only use the Sync, Period)
    SENSOR_SIGNAL XVS;                  ///< VD length for sony sensor only. (only use the Sync, Period)
    UINT32 Width;                       ///< data output width
    UINT32 Height;                      ///< data output height
    UINT32 LaneNum;                     ///< data lanes number
    UINT32 BitDepth;                    ///< data bits
    UINT32 DataAlign;                   ///< data MSB/LSB
    UINT32 OutputPixelOrder[10];        ///< output data pixel order
} SENSOR_LVDS;

/**
    Sensor command structure
*/
typedef struct _SENSOR_CMD
{
    UINT32 uiAddr;                      ///< address
    UINT32 uiDataLen;                   ///< data length(bytes)
    UINT32 uiData[2];                   ///< data idx1(LSB) -> idx2(MSB)
} SENSOR_CMD;

/**
    Sensor DVI structure
*/
typedef struct _SENSOR_DVI
{
    SENSOR_DVI_FMT Fmt;                 ///< dvi format
    SENSOR_DVI_MODE Mode;               ///< dvi mode
    SENSOR_DVI_IN_FMT DataFmt;          ///< dvi in data format
    BOOL FldEn;                         ///< start input from field with internal field signal=0
    BOOL FldInvEn;                      ///< inverse the polarity of internal field signal
    BOOL OutSplitEn;                    ///< output Split en (YUYV -> YYYYYYY UVUVUV)
} SENSOR_DVI;

/**
    Sensor mode information
*/
typedef struct
{
    SENSOR_MODE_TYPE ModeType;      ///< sensor mode type(HDR or .....)
    UINT32 SIEFreq;                 ///< SIE clock frequence Hz
    UINT32 MCLKFreq;                ///< MCLK frequence Hz
    SENSOR_STPIX StPix;             ///< Sensor start pixel
    SENSOR_FMT Fmt;                 ///< Sensor data type
    SENSOR_IMG_RATIO Ratio;         ///< Sensor ratio information
    SENSOR_OB OB;                   ///< Sensor OB
    UINT32 FrameRate;               ///< frame rate X 10
    UINT32 Pclk;                    ///< pixel clock Hz
    UINT32 biningRatio;             ///< binning ratio X 100
    UINT32 StrLnT;                  ///< length from VD start to 1st active line(including OB), uint:us
    UINT32 EndLnT;                  ///< length from VD start to last active line(including OB), uint:us
    UINT32 TransDelyT;              ///< length from exposure end to start of data transmission, uint:us

    SENSOR_SEL_IMG_ID *SelImgId;    ///< sensor select frame information
    SENSOR_SIGNAL *TransHD;         ///< transfer HD signal
    SENSOR_SIGNAL *TransVD;         ///< transfer VD signal
    SENSOR_SIGNAL *Trans2HD;        ///< transfer HD signal (for HDR Sensor frame 2)
    SENSOR_SIGNAL *Trans2VD;        ///< transfer VD signal (for HDR Sensor frame 2)
    SENSOR_SIGNAL *SenHD;           ///< Sensor HD signal
    SENSOR_SIGNAL *SenVD;           ///< Sensor VD signal

    SENSOR_LVDS *LVDS;              ///< lvds information

    SENSOR_DVI *DVI;                ///< dvi information
} SENSOR_MODE_INFO;

/**
    Sensor information
*/
typedef struct
{
    SENSOR_TYPE SenType;                        ///< sensor type
    SENSOR_SIGNAL_TYPE SigType;                 ///< sensor mask or slave
    SENSOR_DATA_TYPE DataType;                  ///< transfer type
    UINT32 CellWidth;                           ///< cell width mm * 1000
    UINT32 CellHeight;                          ///< cell height mm * 1000
    UINT32 Width1X;                             ///< width (full scan pixel size)
    UINT32 Height1X;                            ///< height(full scan pixel size)
    UINT32 SyncTiming;                          ///< sync timing for Exposure time & gain(VD)
    SENSOR_CMD_TYPE CmdType;                    ///< protocol type
    SENSOR_FPS_TYPE FPSType;                    ///< control frame rate type
    SENSOR_MODE_INFO *Mode;  ///< sensor current mode
} SENSOR_INFO;

/**
    command information
*/
typedef struct _SEN_SIF
{
    UINT32      chanel;     ///< map to enum:SIF channel
    UINT32      busclk;     ///< unit:HZ
    UINT32      sen_d_s;    ///< map to sif duration, ref:SIF_CONFIG_ID_SENS,SIF_CONFIG_ID_SEND
    UINT32      sen_h;      ///< map to sif hold time,ref:SIF_CONFIG_ID_SENH
    BOOL        DMA_en;     ///< dma mode
}SEN_SIF;

/**
    I2C information
*/
typedef struct _SEN_I2C
{
    SENSOR_I2C I2c;                 ///< I2C channel
    UINT32  sclk;                   ///< I2C clock
    BOOL    extSlaveAddrEnable;     ///< using slave address or not
    UINT32  extSlaveRAddr;          ///< read command address
    UINT32  extSlaveWAddr;          ///< write command address
}SEN_I2C;

/**
    IO information
*/
typedef struct _SEN_IO
{
    ER          (*fpIO)(SENSOR_ID Id, UINT32 uiData0);          ///< GPIO function point
}SEN_IO;

/**
    Sensor command information
*/
typedef struct
{
    SENSOR_CMD_TYPE CmdType;        ///< command type
    union
    {
        SEN_SIF SIF;                ///< SIF information
        SEN_I2C I2C;                ///< I2C information
        SEN_IO  IO;                 ///< IO  information
    } INFO;
} SENSOR_CMD_INFO;

/**
    sensor initial object
*/
typedef struct
{
    SENSOR_CMD_INFO CmdInfo;        ///< command information
    UINT32 Sen2LVDSPinMap[10];      ///< hw 2 lvds pin map
    UINT32 ChgMclkEn;               ///< change mclk depend on sensor mode
    SENSOR_OPTION Option;           ///< Special Command
} SENSOR_INIT_OBJ;


/**
    sensor eshutter info
*/
typedef struct
{
    UINT32 uiFrame[SEN_MFRAME_MAX_NUM];         ///< exposure frame
    UINT32 uiLine[SEN_MFRAME_MAX_NUM];          ///< exposure line
    UINT32 uiPixel[SEN_MFRAME_MAX_NUM];         ///< exposure pixel
    UINT32 LineRatio;							///< exposure line ratio only for SHDR
} EXPOSURE_SETTING;

/**
    sensor gain info
*/
typedef struct
{
    UINT32 data1[SEN_MFRAME_MAX_NUM];         ///< depend on sensor register
    UINT32 data2[SEN_MFRAME_MAX_NUM];         ///< depend on sensor register
    UINT32 data3[SEN_MFRAME_MAX_NUM];         ///< depend on sensor register
} GAIN_SETTING;

/**
    sensor driver common interface
*/
typedef struct
{
    //Common
    ER    (*fpInit)(SENSOR_ID Id, SENSOR_INIT_OBJ *InitObj);              ///< initial sensor flow
    ER    (*fpUnInit)(SENSOR_ID Id);                                ///< un-initial sensor flow
    ER    (*fpPwron)(SENSOR_ID Id);                                 ///< power on flow
    ER    (*fpPwroff)(SENSOR_ID Id);                                ///< power off flow
    ER    (*fpSleep)(SENSOR_ID Id);                                 ///< enter sensor sleep mode
    ER    (*fpWakeup)(SENSOR_ID Id);                                ///< exit sensor sleep mode
    ER    (*fpWrite)(SENSOR_ID Id, SENSOR_CMD *Cmd);                      ///< write command
    ER    (*fpRead)(SENSOR_ID Id, SENSOR_CMD *Cmd);                       ///< read command

    ER    (*fpChgMode)(SENSOR_ID Id, SENSOR_MODE Mode);                   ///< change mode flow
    ER    (*fpGetCurMode)(SENSOR_ID Id, SENSOR_MODE *Mode);               ///< get current mode

    ER    (*fpGetStatus)(SENSOR_ID Id, SENSOR_MODE Mode, SENSOR_INFO *Info);  ///< get sensor mode information
    UINT32 (*fpGetModeNum)(SENSOR_ID Id);                               ///< get total sensor mode

    //AE
    ER  (*fpGetExpoSetting)(SENSOR_ID Id, UINT32 *ExpTime, SENSOR_MODE Mode, EXPOSURE_SETTING *ExpTSetting, UINT32 *CompensateRatio);   ///< depend on exposure time(us), get frame, line, pixel settings
    ER    (*fpSetAGC_ISOBase)(SENSOR_ID Id, BOOL Cal_Status, UINT32 ISOBase);   //set calibration agc gain
    ER    (*fpGetGainSetting)(SENSOR_ID Id, UINT32 *ISO, GAIN_SETTING *Gain);  ///< depend on ISO(50, 100...), get gain settings
    ER    (*fpSetFrame)(SENSOR_ID Id, EXPOSURE_SETTING *ExpTSetting, UINT32 *VTotal, UINT32 *HTotal);  ///< set frame rate
    ER    (*fpSetLine)(SENSOR_ID Id, EXPOSURE_SETTING *ExpTSetting);                        ///< set exposure line
    ER    (*fpSetPixel)(SENSOR_ID Id, EXPOSURE_SETTING *ExpTSetting);                      ///< set exposure pixel
    ER    (*fpSetGain)(SENSOR_ID Id, GAIN_SETTING *Gain);                 ///< set iso gain
    ER    (*fpGRSMode)(SENSOR_ID Id, void *Param);                        ///< set sensor exposure mode to global reset
    ER    (*fpGetHDRStatInfo) (SENSOR_ID Id, void *Param);                ///< get sensor hdr statistic information
    ER    (*fpSetHDRExpTRatio) (SENSOR_ID Id, UINT32 Raito);              ///< set snesor hdr expsoure time ratio
    ER    (*fpSetHDRGainRatio) (SENSOR_ID Id, UINT32 Raito);              ///< set snesor hdr gain ratio
    UINT32 (*fpGetUserOption) (SENSOR_ID Id);                                ///< get user option
    ER    (*fpSetUserOption) (SENSOR_ID Id, UINT32 option);                  ///< set user option
    ER    (*fpPresetMode) (SENSOR_ID Id, SENSOR_MODE Mode);               ///< preset sensor mode for special AE case
    ER    (*fpSensorTemperature) (SENSOR_ID Id);                          ///< read sensor temperature status
    void* (*fpGetName) (void);                                            ///< get sensor name
} SENSOR_DRVTAB;

/**
    General Sensor APIs
*/
/**
    open sensor

    @param[in] Id       sensor id
    @param[in] InitObj  sensor initial object
    @param[in] DrvTab   sensor driver table
    @return
        - @b 0 suceess, < 0 failed
 */
extern ER Sensor_Open(SENSOR_ID Id, SENSOR_INIT_OBJ *InitObj, SENSOR_DRVTAB *DrvTab);

/**
    close sensor

    @param[in] Id sensor id
    @return
        - @b 0 suceess, < 0 failed
 */
extern ER Sensor_Close(SENSOR_ID Id);

/**
    check sensor is open or not

    @param[in] Id sensor id
    @return
        - @b 0 close, 1 open
 */
extern UINT32 Sensor_IsOpen(SENSOR_ID Id);

/**
    Turn on sensor step by step

    @param[in] Id sensor id
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_PwrOn(SENSOR_ID Id);

/**
    Turn off sensor step by step

    @param[in] Id sensor id
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_PwrOff(SENSOR_ID Id);

/**
    Set sensor to standby state

    @param[in] Id sensor id
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_Sleep(SENSOR_ID Id);

/**
    Resume the sensor from the standby

    @param[in] Id sensor id
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_WakeUp(SENSOR_ID Id);


/**
    configure sensor register

    @param[in] Id    sensor id
    @param[in] Cmd   sensor cmd
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_WriteReg(SENSOR_ID Id, SENSOR_CMD *Cmd);

/**
    read sensor register

    @param[in] Id    sensor id
    @param[in] Cmd   sensor cmd
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_ReadReg(SENSOR_ID Id, SENSOR_CMD *Cmd);

/**
    Set the current operation mode

    @param[in] Id    sensor id
    @param[in] Mode  sensor mode
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_ChgMode(SENSOR_ID Id, SENSOR_MODE Mode);

/**
    Get the current mode

    @param[in] Id       sensor id
    @param[out] Mode    sensor mode
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_GetCurMode(SENSOR_ID Id, SENSOR_MODE *Mode);

/**
    Get idicate status of sensor

    @param[in] Id       sensor id
    @param[in] Mode     sensor mode
    @param[out] Info    sensor information
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_GetStatus(SENSOR_ID Id, SENSOR_MODE Mode, SENSOR_INFO *Info);

/**
    Get idicate total sensor mode of sensor

    @param[in] Id    sensor id

    @return
        - @b UINT32: total sensor number
 */
extern UINT32 Sensor_GetModeNum(SENSOR_ID Id);

/**
    Get exposure time settings

    @param[in] Id            sensor id
    @param[in] ExpTime       exposure time(us)
    @param[in] Mode          sensor mode
    @param[out] ExpTSetting  exposure settings
    @param[out] exposure ratio (HW settings exposure time)/(ExpTime) * 100
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_GetExpoSetting(SENSOR_ID Id, UINT32 *ExpTime, SENSOR_MODE Mode, EXPOSURE_SETTING *ExpTSetting, UINT32 *CompensateRatio);


/**
    Get iso gain settings

    @param[in] Id       sensor id
    @param[in] Gain    calibration gain
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_SetAGC_ISOBase(SENSOR_ID Id, BOOL Cal_Status, UINT32 ISOBase);

/**
    Get iso gain settings

    @param[in] Id       sensor id
    @param[in] ISO      iso gain
    @param[out] Gain    gain setting
    @return
        - @b ER: 0 suceess, < 0 failed
 */
extern ER Sensor_GetGainSetting(SENSOR_ID Id, UINT32 *ISO, GAIN_SETTING *Gain);

/**
    set exposure time

    @param[in] Id   sensor id
    @param[in] ExpT exposure settings
    @return
        - @b ER: 0 suceess, < 0 failed
*/
extern ER Sensor_SetExpTime(SENSOR_ID Id, EXPOSURE_SETTING *ExpT);

/**
    set sensor gain

    @param[in] Id    sensor id
    @param[in] Gain  gain settings
    @return
        - @b ER: 0 suceess, < 0 failed
*/
extern ER Sensor_SetGain(SENSOR_ID Id, GAIN_SETTING *Gain);

/**
    set sensor to GRS mode

    @param[in] SENSOR_ID    sensor id
    @param[in] Param GRS mode parameters
    @return
        - @b ER: 0 suceess, < 0 failed
*/
extern ER Sensor_SetGRS(SENSOR_ID Id, void *Param);

/**
    preset sensor mode, (Sensor_PresetLock(1) -> Sensor_PresetMode -> Sensor_SetGain or ...... ->Sensor_PresetLock(0))

    @param[in] SENSOR_ID    sensor id
    @param[in] Mode         sensor mode
    @return
        - @b ER: 0 suceess, < 0 failed
*/
extern ER Sensor_PresetMode(SENSOR_ID Id, SENSOR_MODE Mode);

/**
    preset lock

    @param[in] SENSOR_ID    sensor id
    @param[in] Mode         sensor mode
    @return
        - @b ER: 0 suceess, < 0 failed
*/
extern ER Sensor_PresetLock(SENSOR_ID Id, UINT32 flag);

extern ER Sensor_GetHDRStatInfo(SENSOR_ID Id, void *Param);

extern ER Sensor_SetHDRExpTRatio(SENSOR_ID Id, UINT32 Raito);

extern ER Sensor_SetHDRGainRatio(SENSOR_ID Id, UINT32 Raito);


extern UINT32 Sensor_GetUserOption(SENSOR_ID Id);
extern ER Sensor_SetUserOption(SENSOR_ID Id, UINT32 option);

extern void Sensor_InstallID(void) _SECTION(".kercfg_text");

extern ER Sensor_SensorTemperature(SENSOR_ID Id);

extern ER Sensor_SensorDumpInfo(SENSOR_ID Id);
extern ER Sensor_SensorDumpCtrlInfo(void);

//@}
#endif
