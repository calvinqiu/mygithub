/**
    Header file of SMedia Recorder Task

    Exported header file of media recorder task.

    @file       VSMediaRecAPI.h
    @ingroup    mIAPPSMEDIAREC
    @note       add gSMediaRecPwrOffPT

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _VSMEDIARECAPI_H
#define _VSMEDIARECAPI_H

#include "MediaWriteLib.h"
#include "AVFile_MakerMov.h"
#include "AVFile_MakerAvi.h"
#include "MediaReadLib.h"
#include "AVFile_ParserMov.h"
#include "AVFile_ParserAvi.h"
#include "VideoEncode.h"
#include "MovieInterface.h"

/**
    @addtogroup mIAPPSMEDIAREC
*/
//@{




/**
    Stop type for SMediaRec_Stop()
*/
typedef enum
{
    VSMEDIAREC_NO_WAIT  = 0x0,  ///< stop recording and no wait
    VSMEDIAREC_WAIT_END = 0x1,   ///< stop recording and wait this action finished
    ENUM_DUMMY4WORD(VSMEDIAREC_STOP_TYPE)
} VSMEDIAREC_STOP_TYPE;

//#NT#2010/05/17#Meg Lin -begin
//#NT#add restriction type

/**
    check type for SMediaRec_SetWriteCard()
*/
typedef enum
{
    VSMEDIAREC_WRITE_PATH1  = 0x0,  ///< write path 1 to card
    VSMEDIAREC_WRITE_PATH2  = 0x1,  ///< write path 2 to card
    ENUM_DUMMY4WORD(VSMEDIAREC_WRITECARD_TYPE)
} VSMEDIAREC_WRITECARD_TYPE;


typedef struct
{
    UINT32 bsAddr;
    UINT32 bsSize;
    UINT32 iskey;    //is key-frame or not
} VSMEDIAREC_BS_FROMSENSOR;
/**
    @name SMedia record event ID

    Callback ID of Smedia recorder.
*/
//@{
#define     VSMEDIAREC_EVENT_RESULT_NORMAL        MOVREC_EVENT_RESULT_NORMAL ///< Finish normally
#define     VSMEDIAREC_EVENT_RESULT_HW_ERR        MOVREC_EVENT_RESULT_HW_ERR ///< Hardware error
#define     VSMEDIAREC_EVENT_RESULT_FULL          MOVREC_EVENT_RESULT_FULL   ///< Card full
#define     VSMEDIAREC_EVENT_RESULT_SLOW          MOVREC_EVENT_RESULT_SLOW   ///< Write card too slowly
#define     VSMEDIAREC_EVENT_RESULT_NOTREADY      MOVREC_EVENT_RESULT_NOTREADY///< Not ready
#define     VSMEDIAREC_EVENT_THRESHOLD            MOVREC_EVENT_THRESHOLD     ///< Threshold meets
#define     VSMEDIAREC_EVENT_RESULT_OVERTIME      MOVREC_EVENT_RESULT_OVERTIME///< Overtime
#define     VSMEDIAREC_EVENT_RESULT_WRITE_ERR     MOVREC_EVENT_RESULT_WRITE_ERR///< Filesystem write fails
#define     VSMEDIAREC_EVENT_STOPPED              MOVREC_EVENT_STOPPED       ///< Callback when stops totally
#define     VSMEDIAREC_EVENT_FILECOMPLETE         MOVREC_EVENT_FILECOMPLETE  ///< Write file completely
#define     VSMEDIAREC_EVENT_RESULT_CUT_FAIL      MOVREC_EVENT_RESULT_CUT_FAIL///< Cut, overlap fail (write slowly)
#define     VSMEDIAREC_EVENT_CUSTOM_UDATA         MOVREC_EVENT_CUSTOM_UDATA  ///< Set customized user data
#define     VSMEDIAREC_EVENT_ENCONE_OK            MOVREC_EVENT_ENCONE_OK     ///< enc one frame

#define     VSMEDIAREC_EVENT_H264BSCB_P0          MOVREC_EVENT_H264BSCB_P0   ///< path1 video enc ok
#define     VSMEDIAREC_EVENT_H264BSCB_P1          MOVREC_EVENT_H264BSCB_P1   ///< path2 video enc ok
#define     VSMEDIAREC_EVENT_RESULT_FLASHSTOP     MOVREC_EVENT_RESULT_FLASHSTOP///< 30 sec after flash stop
#define     VSMEDIAREC_EVENT_AUDBSCB              MOVREC_EVENT_AUDBSCB       ///< audio enc ok
#define     VSMEDIAREC_EVENT_LOOPREC_FULL         MOVREC_EVENT_LOOPREC_FULL  ///< no space for seamless recording
#define     VSMEDIAREC_EVENT_START                MOVREC_EVENT_START         ///< start recording
#define     VSMEDIAREC_EVENT_TIMELAPSE_ONESHOT    MOVREC_EVENT_TIMELAPSE_ONESHOT///< timelapse oneshot
#define     VSMEDIAREC_EVENT_SLOWSTART            MOVREC_EVENT_SLOWSTART     ///< slow stop begins
#define     VSMEDIAREC_EVENT_ADDONEVIDBS          MOVREC_EVENT_ADDONEVIDBS   ///< add one video BS
#define     VSMEDIAREC_EVENT_RESULT_PARAMERR      MOVREC_EVENT_RESULT_PARAMERR ///< paramters error
#define     VSMEDIAREC_EVENT_RESULT_YUVQ_FULL     MOVREC_EVENT_RESULT_YUVQ_FULL ///< yuv queue full
#define     VSMEDIAREC_EVENT_GETNAME_PATH0        MOVREC_EVENT_GETNAME_PATH0 ///< add one video BS
#define     VSMEDIAREC_EVENT_GETNAME_EMRPATH      MOVREC_EVENT_GETNAME_EMRPATH///< add one video BS

//@}

//#NT#2012/09/12#Meg Lin -begin
/**
    @name SMedia ImmediatelyProc callback ID

    Callback ID of Smedia ImmediatelyProc.
*/
//@{
#define     VSMEDIAREC_IMMPROC_CUTFILE_END        MOVREC_IMMPROC_CUTFILE_END ///< end of one cut file, no parameter
#define     VSMEDIAREC_IMMPROC_DELETE_FILE        MOVREC_IMMPROC_DELETE_FILE ///< end of deleting file, p1: directory ID, p2: file ID
#define     VSMEDIAREC_IMMPROC_ENC_SETTING        MOVREC_IMMPROC_ENC_SETTING ///< special encoding parameters setting, such as H.264 3DNR
//@}
//#NT#2012/09/12#Meg Lin -end

/**
    @name SMedia record parameters

    Media recording changeable parameters.
*/
//@{
#define VSMEDIAREC_RECPARAM_WIDTH             MOVREC_RECPARAM_WIDTH   ///< width, p1 valid, p2 fileid
#define VSMEDIAREC_RECPARAM_HEIGHT            MOVREC_RECPARAM_HEIGHT  ///< height, p1 valid, p2 fileid
#define VSMEDIAREC_RECPARAM_FRAMERATE         MOVREC_RECPARAM_FRAMERATE///< frameRate, p1 valid
#define VSMEDIAREC_RECPARAM_QUALITY           MOVREC_RECPARAM_QUALITY ///< quality, p1 valid
#define VSMEDIAREC_RECPARAM_TARGETRATE        MOVREC_RECPARAM_TARGETRATE ///< target rate (bytes per second), p1 valid, p2 fileid
#define VSMEDIAREC_RECPARAM_GOLFSHOT_ON       MOVREC_RECPARAM_GOLFSHOT_ON///< change recFormat to golfshot, p1 0 or 1
#define VSMEDIAREC_RECPARAM_CUTSEC            MOVREC_RECPARAM_CUTSEC  ///< seamless cut sec
#define VSMEDIAREC_RECPARAM_CUTSIZE           MOVREC_RECPARAM_CUTSIZE ///< seamless cut size
#define VSMEDIAREC_RECPARAM_ENDTYPE           MOVREC_RECPARAM_ENDTYPE ///< end type
#define VSMEDIAREC_RECPARAM_FLASH_ON          MOVREC_RECPARAM_FLASH_ON///< flash recording on/off, p1 0 or 1
#define VSMEDIAREC_RECPARAM_PWROFFPT          MOVREC_RECPARAM_PWROFFPT///< enable/disable poweroff_protection, default: on
#define VSMEDIAREC_RECPARAM_GPSON             MOVREC_RECPARAM_GPSON   ///< enable/disable gps info on, default: off
#define VSMEDIAREC_RECPARAM_TIMELAPSE         MOVREC_RECPARAM_TIMELAPSE///< set time lapse recording mode (ON/OFF)
#define VSMEDIAREC_RECPARAM_TIMELAPSE_TIME    MOVREC_RECPARAM_TIMELAPSE_TIME///< set time lapse recording time interval (in unit of ms, should be larger than 1/30 sec)
#define VSMEDIAREC_RECPARAM_THUMB_DATA        MOVREC_RECPARAM_THUMB_DATA///< movie thumbnail settings, p1:on/off, p2:width, p3:height
#define VSMEDIAREC_RECPARAM_AUD_SAMPLERATE    MOVREC_RECPARAM_AUD_SAMPLERATE///< audio sample rate, AUDIO_SR_8000 or others
#define VSMEDIAREC_RECPARAM_AUD_CHS           MOVREC_RECPARAM_AUD_CHS ///< audio channels, 1 or 2
#define VSMEDIAREC_RECPARAM_AUD_SRC           MOVREC_RECPARAM_AUD_SRC ///< audio source, MEDIAREC_AUDTYPE_LEFT or others
#define VSMEDIAREC_RECPARAM_AUD_CODEC         MOVREC_RECPARAM_AUD_CODEC///< audio codec type, MEDIAREC_ENC_PCM or others
#define VSMEDIAREC_RECPARAM_RECFORMAT         MOVREC_RECPARAM_RECFORMAT///< recording format, MEDIAREC_AUD_VID_BOTH or others
#define VSMEDIAREC_RECPARAM_EN_CUSTOMUDATA    MOVREC_RECPARAM_EN_CUSTOMUDATA///< enable custom user data or not
#define VSMEDIAREC_RECPARAM_VIDEOCODEC        MOVREC_RECPARAM_VIDEOCODEC///< video codec type, MEDIAREC_ENC_H264 or others
#define VSMEDIAREC_RECPARAM_H264GOPTYPE       MOVREC_RECPARAM_H264GOPTYPE///< h264 GOP type, MEDIAREC_H264GOP_IPONLY or others
#define VSMEDIAREC_RECPARAM_H2643DNRLEVEL     MOVREC_RECPARAM_H2643DNRLEVEL///< h264 3DNR level
#define VSMEDIAREC_RECPARAM_RAWENC_Q          MOVREC_RECPARAM_RAWENC_Q///< Q value for raw-encoding
#define VSMEDIAREC_RECPARAM_EN_AUDFILTER      MOVREC_RECPARAM_EN_AUDFILTER///< enable audio filter or not
#define VSMEDIAREC_RECPARAM_DROP_FRAME_NUM    MOVREC_RECPARAM_DROP_FRAME_NUM///< drop frame number as start recording (for AV sync)
#define VSMEDIAREC_RECPARAM_H264QPLEVEL       MOVREC_RECPARAM_H264QPLEVEL///< h264 QP initial level
//#NT#2013/04/17#Calvin Chang#Support Rotation information in Mov/Mp4 File format -begin
#define VSMEDIAREC_RECPARAM_MOV_ROTATE        MOVREC_RECPARAM_MOV_ROTATE///< MOV/MP4 file rotation, MEDIAREC_MOV_ROTATE_0
//#NT#2013/04/17#Calvin Chang -end
#define VSMEDIAREC_RECPARAM_FILETYPE_2        MOVREC_RECPARAM_FILETYPE_2///< file1 type, MEDIAREC_AVI as default
#define VSMEDIAREC_RECPARAM_MERGEYUV          MOVREC_RECPARAM_MERGEYUV
#define VSMEDIAREC_RECPARAM_FILETYPE          MOVREC_RECPARAM_FILETYPE ///< file type, MEDIAREC_MOV, MEDIAREC_AVI, MEDIAREC_MP4
#define VSMEDIAREC_RECPARAM_JPG_YUVFORMAT     MOVREC_RECPARAM_JPG_YUVFORMAT///< set jpeg yuv format
#define VSMEDIAREC_RECPARAM_MULTIREC_ON       MOVREC_RECPARAM_MULTIREC_ON
#define VSMEDIAREC_RECPARAM_LIVEVIEW          MOVREC_RECPARAM_LIVEVIEW ///< set MEDIAREC_LIVEVIEW recformat 2013/10/02 Meg
#define VSMEDIAREC_RECPARAM_DAR               MOVREC_RECPARAM_DAR      ///< video display aspect ratio
#define VSMEDIAREC_RECPARAM_PRE_ENCODE        MOVREC_RECPARAM_PRE_ENCODE///< video pre-encode (for applying 3DNR to 1st frame)
#define VSMEDIAREC_RECPARAM_SENSOR_FPS        MOVREC_RECPARAM_SENSOR_FPS///< video sensor fps
#define VSMEDIAREC_RECPARAM_SENSOR_NUM        MOVREC_RECPARAM_SENSOR_NUM///< input sensor number (for encoding path judgement)
#define VSMEDIAREC_RECPARAM_SVC               MOVREC_RECPARAM_SVC      ///< set SVC disable/enable and GOP number
#define VSMEDIAREC_RECPARAM_TINYMODE_ON       MOVREC_RECPARAM_TINYMODE_ON///< tiny mode on/off, p1 0 or 1
#define VSMEDIAREC_RECPARAM_RAWENC_ON         MOVREC_RECPARAM_RAWENC_ON///< tiny mode on/off, p1 0 or 1
#define VSMEDIAREC_RECPARMA_EMRLOOP_ON        MOVREC_RECPARMA_EMRLOOP_ON///< enbale emrloop, p2: vsID
//@}

/**
    @name Valid video frame rate

    Valid video frame rate.
*/
//@{
#define VSMEDIAREC_VRATE_2            2  ///< 2 fps, only for MEDIAREC_FAST_FWD
#define VSMEDIAREC_VRATE_10           10 ///< 10 fps
#define VSMEDIAREC_VRATE_15           15 ///< 15 fps
#define VSMEDIAREC_VRATE_20           20 ///< 20 fps
#define VSMEDIAREC_VRATE_24           24 ///< 24 fps
#define VSMEDIAREC_VRATE_25           25 ///< 25 fps
#define VSMEDIAREC_VRATE_30           30 ///< 30 fps
#define VSMEDIAREC_VRATE_48           48  ///< 48 fps, only for 960p/720p/VGA/QVGA
#define VSMEDIAREC_VRATE_50           50  ///< 50 fps, only for 960p/720p/VGA/QVGA
#define VSMEDIAREC_VRATE_60           60  ///< 60 fps, only for 720p/VGA/QVGA
#define VSMEDIAREC_VRATE_120          120 ///< 120 fps, only for VGA/QVGA
#define VSMEDIAREC_VRATE_240          240 ///< 240 fps, only for QVGA
//@}

/**
    @name Valid file type

    Valid file type.
*/
//@{
#define VSMEDIAREC_AVI    MEDIA_FILEFORMAT_AVI    ///< .avi file
#define VSMEDIAREC_MOV    MEDIA_FILEFORMAT_MOV    ///< .mov file
#define VSMEDIAREC_MP4    MEDIA_FILEFORMAT_MP4    ///< .mp4 file
#define VSMEDIAREC_TS     MEDIA_FILEFORMAT_TS     ///< .ts file
//@}

/**
    @name Valid video encode codec

    Valid video encode codec.
*/
//@{
#define VSMEDIAREC_ENC_JPG    MEDIAVIDENC_MJPG    ///< motion JPG
#define VSMEDIAREC_ENC_H264   MEDIAVIDENC_H264    ///< H.264
//@}

/**
    @name Valid audio encode codec

    Valid audio encode codec.
*/
//@{
#define VSMEDIAREC_ENC_PCM    MOVAUDENC_PCM   ///< PCM
#define VSMEDIAREC_ENC_AAC    MOVAUDENC_AAC   ///< AAC
#define VSMEDIAREC_ENC_PPCM   MOVAUDENC_PPCM  ///< PPCM
//@}

/**
    @name Valid video input YCbCr format

    Valid video input YCbCr format.
*/
//@{
#define VSMEDIAREC_JPEG_FORMAT_420    MOVREC_JPEG_FORMAT_420  ///< Y:Cb:Cr = 4:1:1
#define VSMEDIAREC_JPEG_FORMAT_422    MOVREC_JPEG_FORMAT_422  ///< Y:Cb:Cr = 2:1:1
#define VSMEDIAREC_JPEG_FORMAT_444    MOVREC_JPEG_FORMAT_444  ///< Y:Cb:Cr = 1:1:1
#define VSMEDIAREC_JPEG_FORMAT_100    MOVREC_JPEG_FORMAT_100  ///< Y:Cb:Cr = 4:1:1
//@}

/**
    @name Valid counting type

    Valid counting type for MediaRec_Disk2SecondWithType.
*/
//@{
#define VSMEDIAREC_COUNTTYPE_FS    MOVREC_COUNTTYPE_FS   ///< get freespace in storage
#define VSMEDIAREC_COUNTTYPE_4G    MOVREC_COUNTTYPE_4G   ///< using 4G to calculate
#define VSMEDIAREC_COUNTTYPE_USER  MOVREC_COUNTTYPE_USER ///< using given size to calculate
//@}

/**
    @name Valid end type

    Valid recording end type. Normal end, cut overlap, or cut till card full.
*/
//@{
#define VSMEDIAREC_ENDTYPE_NORMAL             MOVREC_ENDTYPE_NORMAL ///< normal end, such as press key to stop recording
#define VSMEDIAREC_ENDTYPE_CUTOVERLAP         MOVREC_ENDTYPE_CUTOVERLAP///< cut file and continue recording automatically; overlap the old files (cyclic recording)
#define VSMEDIAREC_ENDTYPE_CUT_TILLCARDFULL   MOVREC_ENDTYPE_CUT_TILLCARDFULL///< cut file and continue recording automatically till card full
#define VSMEDIAREC_ENDTYPE_MAX                MOVREC_ENDTYPE_MAX    ///< maximum number of recording end type
//@}

//#NT#2012/01/09#Hideo Lin -begin
//#NT#Add media recording source type (for test mode)
/**
    @name Media recording source type

    Media recording video source type: normal, fixed YUV pattern, bitstream;
    Media recording audio source type: normal, fixed PCM pattern.
*/
//@{
#define VSMEDIAREC_SRC_NORMAL         MOVREC_SRC_NORMAL     ///< normal source (video: sensor, audio: mic-in)
#define VSMEDIAREC_SRC_FIXED_YUV      MOVREC_SRC_FIXED_YUV  ///< fixed YUV pattern (for video)
#define VSMEDIAREC_SRC_BS_ONLY        MOVREC_SRC_BS_ONLY    ///< bitstream only (for video)
#define VSMEDIAREC_SRC_FIXED_PCM      MOVREC_SRC_FIXED_PCM  ///< fixed PCM pattern (for audio)
//@}
//#NT#2012/01/09#Hideo Lin -end

/**
    @name IPL mode change callback event

    Event ID for changing IPL mode callback function.
*/
//@{
#define VSMEDIAREC_IPLCHG_PREVIEW     MOVREC_IPLCHG_PREVIEW    ///< Set IPL to preview mode
#define VSMEDIAREC_IPLCHG_VIDEO       MOVREC_IPLCHG_VIDEO      ///< Set IPL to video mode
#define VSMEDIAREC_IPLCHG_INITBS      MOVREC_IPLCHG_INITBS     ///< initbs
//@}

/**
    @name Get IPL ready buffer callback event

    Event ID for getting IPL ready buffer callback function.
*/
//@{
#define VSMEDIAREC_IPLGET_READYBUF    MOVREC_IPLGET_READYBUF   ///< Get IPL ready buffer
//@}

/**
    @name Smedia record information

    SMedia recording getting information.
*/
//@{
#define     VSMEDIAREC_GETINFO_WIDTH_HEIGHT   MOVREC_GETINFO_WIDTH_HEIGHT    ///< p1: width (out), p2: height (out)
#define     VSMEDIAREC_GETINFO_VIDEO_CODEC    MOVREC_GETINFO_VIDEO_CODEC     ///< p1: videocodec type(out), MEDIAREC_ENC_JPG or others//2012/10/15 Meg
#define     VSMEDIAREC_GETINFO_AUDIO_SR       MOVREC_GETINFO_AUDIO_SR        ///< p1: audio samplerate(out), AUDIO_SR_32000 or others//2013/01/11 Meg
#define     VSMEDIAREC_GETINFO_AUDIO_CHS      MOVREC_GETINFO_AUDIO_CHS       ///< p1: audio channels(out), 1 or 2//2013/01/11 Meg
#define     VSMEDIAREC_GETINFO_AUDIO_BITS     MOVREC_GETINFO_AUDIO_BITS      ///< p1: audio bits per sample, 16
#define     VSMEDIAREC_GETINFO_H264_SPS       MOVREC_GETINFO_H264_SPS        ///< p1: addr (out), p2: size (out), p3: num(0 or 1)
#define     VSMEDIAREC_GETINFO_VIDEO_FPS      MOVREC_GETINFO_VIDEO_FPS       ///< p1: v fps (out)
#define     VSMEDIAREC_GETINFO_WIDTH_HEIGHT_P2 MOVREC_GETINFO_WIDTH_HEIGHT_P2  ///< p1: width of Path2 (out), p2: height of Path2(out)
#define     VSMEDIAREC_GETINFO_AUDIO_CODEC    MOVREC_GETINFO_AUDIO_CODEC     ///< p1: audio codec type of 1stpath(out), MEDIAREC_ENC_PCM or others
//@}

/**
    @name Smedia audio type information

    SMedia recording audio type information.
*/
//@{
#define     VSMEDIAREC_AUDTYPE_LEFT   MOVREC_AUDTYPE_LEFT   ///< left
#define     VSMEDIAREC_AUDTYPE_RIGHT  MOVREC_AUDTYPE_RIGHT  ///< right
#define     VSMEDIAREC_AUDTYPE_STEREO MOVREC_AUDTYPE_STEREO ///< stereo
#define     VSMEDIAREC_AUDTYPE_MONO   MOVREC_AUDTYPE_MONO   ///< mono
//@}

//#NT#2013/04/17#Calvin Chang#Support Rotation information in Mov/Mp4 File format -begin
/**
    @name Smedia rotation angle information

    SMedia recording roation angle information.
*/
//@{
#define     VSMEDIAREC_MOV_ROTATE_0       MOVREC_MOV_ROTATE_0   ///< angle 0
#define     VSMEDIAREC_MOV_ROTATE_90      MOVREC_MOV_ROTATE_90   ///< angle 90
#define     VSMEDIAREC_MOV_ROTATE_180     MOVREC_MOV_ROTATE_180   ///< angle 180
#define     VSMEDIAREC_MOV_ROTATE_270     MOVREC_MOV_ROTATE_270   ///< angle 270
//@}
//#NT#2013/04/17#Calvin Chang -end

/**
    Status type for SMediaRec_GetStatus()
*/
typedef enum
{
    VSMEDIAREC_STATUS_NOT_OPENED        = 0x1,  ///< media recorder task is not open
    VSMEDIAREC_STATUS_OPENED_NOT_RECORD = 0x2,  ///< media recorder task is open, not recording
    VSMEDIAREC_STATUS_RECORDING         = 0x3,  ///< media recorder task is recording
    ENUM_DUMMY4WORD(VSMEDIAREC_STATUS_TYPE)
} VSMEDIAREC_STATUS_TYPE;

/**
    Video ready buffer information for movie recording.

*/
typedef struct
{
    UINT32      y;              ///<[in]: y address
    UINT32      cb;             ///<[in]: cb address
    UINT32      cr;             ///<[in]: cr address
    UINT32      y_lot;          ///<[in]: y line offset
    UINT32      uv_lot;         ///<[in]: uv line offset
    UINT32      uiBufID;        ///<[in]: IME ready buffer ID
} VSMEDIAREC_READYBUF_INFO;

/*
    Public Smedia recording functions
*/
//MediaRecTsk.c
/**
    Open VSmedia recorder task.

    Open VSmedia recorder task to prepare for recording.

    @param[in] param    the media recorder object

    @return
        - @b E_OK:  open Smedia recorder task successfully
        - @b E_SYS: Smedia recorder task is already opened
*/
extern ER       VSMediaRec_Open(MEDIAREC_OBJ *param) __attribute__ ((weak));

/**
    Close VSmedia recorder task.

    Close VSmedia recorder related tasks and stop recording if necessary.

    @return
        - @b E_OK:  terminate media recorder task successfully
        - @b E_SYS: Smedia recorder task is not opened
*/
extern ER       VSMediaRec_Close(void) __attribute__ ((weak));

/**
    Start VSmedia recording.

    @return
        - @b E_OK:  start recording successfully
        - @b E_SYS: Smedia recorder is recording or not ready
        - @b E_PAR: recorder setting error (such as recording format, video codec type, ...)
*/
extern ER       VSMediaRec_Record(void) __attribute__ ((weak));

/**
    Stop VSmedia recording.

    @param[in] waitType waiting type as media recording stop, MEDIAREC_NO_WAIT or MEDIAREC_WAIT_END

    @return
        - @b E_OK:  stop recording successfully
        - @b E_SYS: media recorder is not recording
*/
extern ER       VSMediaRec_Stop(VSMEDIAREC_STOP_TYPE waitType) __attribute__ ((weak));

/**
    Change VSmedia recorder parameters.

    Change VSmedia recorder parameters, such as width, height, frame rate, target bit rate, ..., and so on.

    @note

    @param[in] type parameter type
    @param[in] p1   parameter1
    @param[in] p2   parameter2
    @param[in] p3   parameter3

    @return
        - @b E_OK:  change successfully
        - @b E_PAR: parameter value error
*/
extern ER       VSMediaRec_ChangeParameter(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3) __attribute__ ((weak));

/**
    Get VSmedia recorder parameter settings.

    Get VSmedia recorder parameter settings, only support getting width and height information currently.

    @note

    @param[in]  type    parameter type, only support SMEDIAREC_GETINFO_WIDTH_HEIGHT currently
    @param[out] p1      parameter1 value
    @param[out] p2      parameter2 value
    @param[out] p3      parameter3 value

    @return
        - @b E_OK:  get parameter successfully
        - @b E_SYS: parameter address error
        - @b E_PAR: parameter type error
*/
extern ER       VSMediaRec_GetInfo(UINT32 type, UINT32 *p1, UINT32 *p2, UINT32 *p3) __attribute__ ((weak));

/**
    Set user data.

    Set user data for media container.

    @note The user data is in 'JUNK' chunk for AVI file, and in 'udta' atom for MOV/MP4 file.

    @param[in] addr user data starting address
    @param[in] size user data size

    @return void
*/
extern void     VSMediaRec_SetUserData(UINT32 addr, UINT32 size) __attribute__ ((weak));
extern void     VSMediaRec_SetCustomDataWithID(UINT32 id, UINT32 tag, UINT32 addr, UINT32 size) __attribute__ ((weak));

/**
    Estimate remaining time for media recording.

    Estimate remaining time in seconds according to storage device space with different counting type.

    @note This function should be called before Smedia recording.

    @param[in] type counting type, SMEDIAREC_COUNTTYPE_FS (according to storage free space),
                    SMEDIAREC_COUNTTYPE_4G (FAT file size limitation), or MEDIAREC_COUNTTYPE_USER
                    (user defined space size, for special purpose)
    @param[in] size user defined space size, just for SMEDIAREC_COUNTTYPE_USER type

    @return Estimated remaining time in seconds.
*/
extern UINT32   VSMediaRec_Disk2SecondWithType(UINT32 type, UINT32 size) __attribute__ ((weak));

/**
    Set restriction for recording time.

    Set restriction to calculate remaining time for Smedia recording.

    @note

    @param[in] type     restriction type, SMEDIAREC_RESTR_INDEXBUF or SMEDIAREC_RESTR_FSLIMIT
    @param[in] value    set FALSE to disable, and others to enable the related restriction

    @return void
*/
extern void     VSMediaRec_SetDisk2SecRestriction(MEDIAREC_RESTR_TYPE type, UINT32 value) __attribute__ ((weak));

/**
    Set free user data.

    Set free user data for special purpose.

    @note It's only valid for MOV/MP4 file. The data will be put in 'fre1' atom.

    @param[in] addr free data starting address
    @param[in] size free data size

    @return void
*/
extern void     VSMediaRec_SetFre1Data(UINT32 addr, UINT32 size) __attribute__ ((weak));

/**
    Re-allocate buffer to Smedia recorder.

    Re-allocate buffer for Smedia video and audio recording usage.

    @note The buffer should be re-allocated before media recording.

    @param[in] memStart buffer starting address
    @param[in] memSize  buffer size

    @return
        - @b E_OK:  re-allocate buffer successfully
        - @b E_SYS: re-allocate buffer failed (memory is not enough)
*/
extern ER       VSMediaRec_ReAllocBuf2VA(UINT32 memStart, UINT32 memSize) __attribute__ ((weak));

/**
    Stop Smedia recording and set read-only.

    Stop Smedia recording and set previous file, current file and next file to be read-only.

    @return
        - @b E_OK:  stop recording successfully
        - @b E_SYS: media recorder is not recording
*/
extern ER       VSMediaRec_StopAndReadOnly(void) __attribute__ ((weak));//2012/02/17 Meg

/**
    To get Smedia recorder status.

    @return
        - @b VSMEDIAREC_STATUS_NOT_OPENED:        Smedia recorder is not opened
        - @b VSMEDIAREC_STATUS_OPENED_NOT_RECORD: Smedia recorder is opened but not recording
        - @b VSMEDIAREC_STATUS_RECORDING:         Smedia recorder is recording
*/
extern VSMEDIAREC_STATUS_TYPE   VSMediaRec_GetStatus(void) __attribute__ ((weak));
/**
    Set audio volume for SMedia Recorder.

    @param[in] percent  the percentage of audio gain level (0% - 100%)
    @return
        - @b E_OK:  set successfully
*/
extern ER VSMediaRec_SetAudioVolume(UINT32 percent) __attribute__ ((weak));


extern void     VSMediaRec_Sleep(void) __attribute__ ((weak));
extern void     VSMediaRec_Wakeup(void) __attribute__ ((weak));
extern void     VSMediaRec_PauseDMA(void) __attribute__ ((weak));
extern void     VSMediaRec_ResumeDMA(void) __attribute__ ((weak));
extern void     VSMediaRec_ChangeDisplaySize(UINT32 uiDisp_w, UINT32 uiDisp_h) __attribute__ ((weak));


//SMediaVideoRecTsk.c
/**
    Set ready buffer for Smedia recorder.

    Set ready buffer information, such as Y, Cb, Cr starting address and line offset for media recorder.

    @note This function is usually used in IPLGetReadyCB (get IPL ready buffer callback) of media recorder object.
          In the callback, get IPL ready buffer from IPL driver, then use this API to set to media recorder.

    @param[in] pInfo    ready buffer information for media recorder

    @return void
*/
extern void     VSMediaRec_ResetReadyBuf(void) __attribute__ ((weak));
extern void     VSMediaRec_PutReadyBuf(MEDIAREC_READYBUF_INFO *pInfo) __attribute__ ((weak));
extern void     VSMediaRec_GetReadyBuf(MEDIAREC_READYBUF_INFO *pInfo) __attribute__ ((weak));
extern void     VSMediaRec_SetReadyBuf(MEDIAREC_READYBUF_INFO *pInfo) __attribute__ ((weak));//2012/08/30 Meg


//SMediaAudioRecTsk.c
extern void     VSMediaRec_Pause(void) __attribute__ ((weak));
extern void     VSMediaRec_Resume(void) __attribute__ ((weak));

//SMediaRecFSTsk.c
/**
    Set GPS data.

    Set GPS data for Smedia container.

    @note It's only valid for AVI file currently. The GPS data is put in 'JUNK' chunk per second.

    @param[in] addr GPS data starting address
    @param[in] size GPS data size

    @return void
*/
extern void     VSMediaRec_SetGPSData(UINT32 addr, UINT32 size) __attribute__ ((weak));//2011/06/23 Meg

/**
    Set crash event.

    Set current recording file to be renamed and read-only.

    @note

    @return void
*/
extern void     VSMediaRec_SetCrash(void) __attribute__ ((weak));//2011/06/27 Meg

/**
    Cancel crash event.

    Cancel crash event.

    @note

    @return void
*/
extern void     VSMediaRec_CancelCrash(void) __attribute__ ((weak));//2013/11/18 Meg

/**
    Set event.

    Set current recording file to be renamed and read-only.

    @param[in] pathid 0 or 1
    @param[in] eventid MEDIAREC_CRASHTYPE_EV1 or others

    @return void
*/
extern void VSMediaRec_SetEvent(UINT32 pathid, UINT32 eventid) __attribute__ ((weak));

/**
    To use same crash DID or not.

    To use same or different DCF directory ID for crash event.

    @note Set TRUE to use same directory ID, FALSE to use different directory ID.

    @param[in] on   turn on/off same crash DID function

    @return void
*/
extern void     VSMediaRec_SetTheSameCrashDid(UINT8 on) __attribute__ ((weak));//2011/09/15 Meg
/**
    Set crash folder name.

    Set DCF folder name for crash event.

    @note It should follow the DCF naming rule, that is, 5 characters in 'A'~'Z', 'a'~'z', '0'~'9', and '_'.

    @param[in] pChar    pointer to the folder name

    @return void
*/
extern void     VSMediaRec_SetCrashFolderName(char *pChar) __attribute__ ((weak));//2011/09/15 Meg

/**
    Set naming rule for SMedia Recorder.

    @param[in] pHdl    pointer to the naming rule
    @return
        - @b E_OK:  set successfully
*/
extern ER VSMediaRec_SetFSNamingHandler(MEDIANAMINGRULE *pHdl) __attribute__ ((weak));


extern void VSMediaRecCmdTsk(void) __attribute__ ((weak));
extern void VSMediaRecTsk(void)__attribute__ ((weak));
extern void VSMediaVideoRecTsk(void)__attribute__ ((weak));
extern void VSMRawEncTsk(void)__attribute__ ((weak));
extern void VSMediaAudioEncTsk(void)__attribute__ ((weak));
extern void VSMediaRecFSTsk(void)__attribute__ ((weak));
extern void VSMediaRecVideoStrmTsk(void)__attribute__ ((weak));
extern void VSMediaRecMdatTsk(void)__attribute__ ((weak));
extern void VSFakeMediaTsk(void)__attribute__ ((weak));
extern void VSMediaRecTSTsk(void)__attribute__ ((weak));


extern void VSMediaRec_SetTestMode(MEDIAREC_TESTMODE *pTestMode)__attribute__ ((weak));
extern void VSMediaRec_SetIPLChangeCB(MediaRecIPLChangeCB *pIPLChangeCB)__attribute__ ((weak));
extern void VSMediaRec_VideoDoEnc(void)__attribute__ ((weak));
extern void VSMediaRec_GetEncInfo(UINT32 puiParam[7])__attribute__ ((weak));
extern void VSMediaRec_SetFileHandle(FST_FILE fhdl, UINT32 fileid)__attribute__ ((weak));
extern void VSMediaRecFS_SetWorkingID(UINT32 id)__attribute__ ((weak));
extern UINT32   VSMediaRecVid_GetNowFrames(UINT32 vsID)__attribute__ ((weak));

/**
    Open Smedia recording error msg.

    @param[in] on  1 to open error msg
    @return void
*/
extern void VSMediaRec_OpenErrMsg(UINT8 on) __attribute__ ((weak));

/**
    Reset Smedia recorder containerMaker.

    @return void
*/
extern void VSMediaRec_ResetConMaker(void) __attribute__ ((weak));

/**
    Open VSmedia recording warning msg.

    @param[in] on  1 to open warning msg
    @return void
*/
extern void VSMediaRec_OpenWrnMsg(UINT8 on) __attribute__ ((weak));

/**
    Open media recording seamless recording msg.

    @param[in] on  1 to open seamless recording msg
    @return void
*/
extern void VSMediaRec_OpenCutMsg(UINT8 on) __attribute__ ((weak));

/**
    Open media recording must msg.

    @param[in] on  1 to open must msg
    @return void
*/
extern void VSMediaRec_OpenMustMsg(UINT8 on) __attribute__ ((weak));

/**
    Check VSmedia recorder if recording.

    @param[in] void
    @return
        - @b TRUE:  recording
        - @b FALSE: not recording
*/
extern BOOL VSMediaRec_IsRecording(void) __attribute__ ((weak));

/**
    Set memory addr and size for MergeYUV.

    @param[in] addr memory address
    @param[in] size memory size
    @return void
*/
extern void VSMediaRec_SetGiveYUVBuf(UINT32 addr, UINT32 size) __attribute__ ((weak));

/**
    Set readyYUV address for MergeYUV.

    @param[in] ptr    pointer to YUV info
    @return void
*/
extern void VSMediaRec_GiveYUV(MEDIAREC_READYBUF_INFO *ptr) __attribute__ ((weak));

/**
    Set Extend crash.
    If seamless recording, this will set Crash after 30 sec.
    Max 3 times. If busy, return E_SYS.

    @param[in] void
    @return
        - @b E_OK: ok
        - @b E_SYS: busy
*/
extern ER VSMediaRec_SetExtendCrash(void) __attribute__ ((weak));//2013/09/04 Meg

/**
    Dump MediaStatus.

    @param[in] pData    pointer to data,p1=width,p2=height,p3=bpp,p4=fps
    @return number of data
*/
extern UINT32 VSMediaRec_DumpDataStatus(UINT32* pData) __attribute__ ((weak));//2013/08/15
/**
    Get lastest video bitstream addr,size.

    @param[in] frameBuf    pointer to video frame
    @return 0
*/
extern UINT32 VidStrm_GetLastV_BS(UINT32 pathID, MEDIAREC_MEM_RANGE* frameBuf) __attribute__ ((weak));

/**
    Force SMediaRec to check FS_DiskInfo.

    @param[in] void
    @return void
*/
extern void VSMediaRec_CheckFSDiskInfo(void) __attribute__ ((weak));

/**
    Open Smedia recording FSObj msg.

    @param[in] on  1 to open FSObj msg
    @return void
*/
extern void VSMediaRec_OpenFSOBJMsg(UINT8 on) __attribute__ ((weak));

/**
    Open VSmedia recording file msg.

    @param[in] on  1 to open file msg
    @return void
*/
extern void VSMediaRec_OpenFileMsg(UINT8 on) __attribute__ ((weak));

/**
    Set path2 write card or not.

    @param[in] on  1 to write path2 to card, if path2 enable
    @return void
*/
extern void VSMediaRec_SetPath2WriteCard(UINT32 on) __attribute__ ((weak));


/**
    Set path2 IPL get ready buffer callback.

    @param[in] IPLCb  path2 IPL get ready buffer callback
    @return void
*/
extern void VSMediaRec_RegisterCB2(MediaRecIPLGetReadyCB IPLCb) __attribute__ ((weak));

/**
    Minus filesize if some files written outside MediaREC.

    @param[in] size filesize, Must be cluster-alignment!!
    @return void
*/
extern void VSMediaRec_MinusFilesize(UINT32 size) __attribute__ ((weak));

extern void VSMediaRec_StopPreRecordStartWriting(void) __attribute__ ((weak));

/**
    Get H264 sps/pps for each video path.

    @param[in] fileid video path, 0 or 1
    @param[in] pAddr address to put sps/pps
    @param[in] pSize size of sps/pps
    @return void
*/
//extern void     VSMediaRec_VidGetDesc(UINT32 fileid, UINT32 *pAddr, UINT32 *pSize) __attribute__ ((weak));
//change to  VidStrmUti_GetDesc(UINT32 vsID, UINT32 *pAddr, UINT32 *pSize)//2014/01/09
/**
    Get minimum memory size for liveview mode.

    @param[in] void
    @return memory size
*/
extern UINT32 VSMediaRec_GetLiveViewMinMemSize(void) __attribute__ ((weak));

/**
    Update setting to calculate disk2second.

    @param[in] void
    @return void
*/
extern void VSMediaRec_UpdateSetting2CaluSec(void) __attribute__ ((weak));

/**
    Set Customized Data. (Put end of file with specific tag)

    @param[in] tag abcd=0x61626364
    @param[in] addr data address
    @param[in] size data size
    @return void
*/
extern void VSMediaRec_SetCustomData(UINT32 tag, UINT32 addr, UINT32 size) __attribute__ ((weak));

/**
    Set read-only percentage.

    @param[in] percent min=30, max=50
    @return void
*/
extern void VSMediaRec_SetROPercent(UINT32 percent) __attribute__ ((weak));

/**
    Enable end-char for single recording.

    @param[in] value on/off
    @return void
*/
extern void VSMediaRec_EnableEndChar4SingleRec(UINT8 value) __attribute__ ((weak));

/**
    Enable end-char for multiple recording.

    @param[in] value on/off
    @return void
*/
extern void VSMediaRec_EnableEndChar4MultiRec(UINT8 value) __attribute__ ((weak));

/**
    Get lastest audio bitstream.

    @param[in] frameBuf output audio bs addr/size
    @return void
*/
extern void VSMediaRec_GetLastA_BS(MEDIAREC_MEM_RANGE* frameBuf) __attribute__ ((weak));

/**
    Get lastest movie filepath.

    @param[in] pPath output filepath, length=TEST_PATHLENGTH (80)
    @return void
*/
extern void VSMediaRec_GetLastFilePath(char *pPath) __attribute__ ((weak));

/**
    Get lastest EMR filepath.

    @param[in] pPath output filepath, length=TEST_PATHLENGTH (80)
    @return void
*/
extern void VSMediaRec_GetLastEMRFilePath(char *pPath) __attribute__ ((weak));

/**
    Enable Check and delete read-only file when seamless recoring.

    @param[in] value on/off, default: on
    @return void
*/
extern void VSMediaRec_EnableCheckReadonly(UINT8 value) __attribute__ ((weak));
extern void VSMediaRec_VidReady2Get(UINT32 fileid) __attribute__ ((weak));//2014/01/09

/**
    Input settings to calculate recording seconds.

    @param[in] pSetting input recording setting
    @return recording seconds
*/
extern UINT32 VSMediaRec_CalcSecBySetting(MEDIAREC_CALCSEC_SETTING *pSetting) __attribute__ ((weak));

/**
    Set max customdata size. Card capacity will minus this size when start recording.

    @param[in] fileID
    @param[in] maxSize
    @return void
*/
extern void VSMediaRec_SetCustomDataMaxSize(UINT32 fileid, UINT32 maxSize) __attribute__ ((weak));

/**
    Set frameinfo size. Default: 4MB.

    @param[in] size
    @return void
*/
extern void VSMediaRec_SetFrameInfoSize(UINT32 size) __attribute__ ((weak));

/**
    Set this and last file as read-only.

    @param[in] void
    @return
        - @b MOVREC_SETCRASH_OK:            this and last both read-only
        - @b MOVREC_SETCRASH_NOLASTFILE:    this will be read-only (later), last not exist
        - @b MOVREC_SETCRASH_LASTFILERO:    this will be read-only (later), last has been RO already
*/
extern UINT32 VSMediaRec_SetReadonly_ThisandLastfile(void) __attribute__ ((weak));

/**
    Set end char for each path. 'A' and 'B' as default value.

    @param[in] endchar A~Z, a~z
    @param[in] fileid  0 as path1, 1 as path2...
    @return
        - @b E_OK:  OK
        - @b E_SYS: repeat as others
*/
extern ER VSMediaRec_SetEndChar(char endchar, UINT32 fileid) __attribute__ ((weak));

/**
    Delete files (including RO). It SHOULD NOT be executed when recording.

    @param[in] roDelete delete read-only files or not
    @param[in] roPercent  read-only max percent (30~50)
    @return void
*/
extern void VSMediaRec_Clean4Recording(UINT32 roDelete, UINT32 roPercent) __attribute__ ((weak));

/**
    Set deleting read-only files or not.

    @param[in] value 0: No delete RO files, 1: as default
    @return void
*/
extern void VSMediaRec_SetDeleteReadonly(UINT32 value) __attribute__ ((weak));

/**
    Get read-only files percent. (including freespace)

    @param[in] void
    @return RO percent
*/
extern UINT32 VSMediaRec_GetROPercent(void) __attribute__ ((weak));

/**
    Check and get H.264 encoding path after some settings (such as resolution, frame rate, ...).

    @param[in] void
    @return void
*/
extern void VSMediaRec_CheckEncPath(void) __attribute__ ((weak));

/**
    Enable/disable card writing.

    @param[in] pathid   0 or 1
    @param[in] on       0: not save, 1: as default
    @return void
*/
extern void VSMediaRec_SetWriteCard(UINT32 pathid, UINT32 on) __attribute__ ((weak));

/**
    Enable/disable EmergencyRecording for path0/1
    (If EMR is enable, it needs more memory!!)

    @param[in] pathid   0 or 1(if path1 is enable)
    @param[in] on       0: disable 1: enable
    @return void
*/
extern void VSMediaRec_SetEmergencyRecording(UINT32 pathid, UINT32 on) __attribute__ ((weak));

/**
    Stop EmergencyRecording for path0/1
    (If EMR is disable, it doesn't work!)

    @param[in] pathid   0 or 1(if path1 is enable)
    @return void
*/
extern ER VSMediaRec_StopFlash(UINT32 pathid) __attribute__ ((weak));

/**
    Set H.264 Max Qp level.

    @param[in] level H264_MAX_QP_LEVEL_0 as default
    @return void
*/
extern void VSMediaRec_SetH264MaxQp(UINT32 pathid, UINT32 level) __attribute__ ((weak));

/**
    Set H.264 GOP number.

    @param[in] gopnum 15 as default. Max: 120
    @return void
*/
extern void VSMediaRec_SetH264GopNum(UINT32 gopnum) __attribute__ ((weak));

/**
    Get SMedia recording parameters.

    @param[in] type MOVREC_GETRECPARAM_TARGETBR or others

    @return ER
        - @b E_OK:  OK
        - @b E_SYS: params error
*/
extern ER VSMediaRec_GetParam(UINT32 type, UINT32 *pParam1, UINT32 *pParam2, UINT32 *pParam3) __attribute__ ((weak));

/**
    Set SMedia Force no write card

    @param[in] value 1:no write

    @return void
*/
extern void VSMediaRec_ForceNoWriteCard(UINT32 value) __attribute__ ((weak));

/**
    Set SMedia max stopflash rollback sec

    @param[in] value 5~30 is valid, default: 30

    @return void
*/
extern void VSMediaRec_SetMaxStopFlashSec(UINT32 value) __attribute__ ((weak));

/**
    Set VSMedia max keep sec

    @param[in] value 5~30 is valid, default: 30

    @return void
*/
extern void VSMediaRec_SetMaxKeepSec(UINT32 value) __attribute__ ((weak));

/**
    Enable rollback or not

    @param[in] enable: 0 no overlap, 1 overlap 1sec

    @return void
*/
extern void VSMediaRec_EnableOverlap(UINT32 enable) __attribute__ ((weak));

/**
    Enable/disable ExtendCrash

    @param[in] enable: 0 no extend crash (1 as default)

    @return void
*/
extern void VSMediaRec_Enable_ExtendCrash(UINT32 value) __attribute__ ((weak));

/**
    Set NextSec to call StopFlash.

    @param[in] value 5 is default (smallest : 5)

    @return void
*/
extern void VSMediaRec_SetStopFlash_NextSec(UINT32 value) __attribute__ ((weak));

/**
    Start recording file (NOT EMR)

    @param[in] pathID (0 as path 1)

    @return void
*/
extern void VSMediaRec_StartFile(UINT32 pathID) __attribute__ ((weak));

/**
    Stop recording file (NOT EMR), it takes times to end file!

    @param[in] pathID (0 as path 1)

    @return void
*/
extern void VSMediaRec_StopFile(UINT32 pathID) __attribute__ ((weak));

/**
    Wait Raw-encode ok

    @param[in] pathid (0 as path1)

    @return void
*/
extern void VSMRawEnc_WaitJpgEnc(UINT32 pathid) __attribute__ ((weak));

/**
    Set RawEnc write JPG file or not.

    @param[in] value (default: 1)

    @return void
*/
extern void VSMRawEnc_EnableWriteCard(UINT32 enable) __attribute__ ((weak));

/**
    Add pure streaming for media 4.0

    @param[in] pinfo information to add

    @return ER E_OK:success
*/
extern ER       VSMediaRec_AddPureStreaming(MOVREC_ADD_INFO *pinfo) __attribute__ ((weak));

/**
    Enable/disable overwrite YUV test. default off

    @param[in] on: 1 to open test

    @return void
*/
extern void     VSMediaRec_overwriteYUVtest(UINT32 on) __attribute__ ((weak));

extern void VSMediaRec_StopLoopEMR(UINT32 pathID) __attribute__ ((weak));

extern void VSMediaRec_SetTimelapsePlayVideoFrameRate(UINT32 value) __attribute__ ((weak));

extern void VSMediaRec_SetExtendCrashUnit(MOVREC_EXTENDCRASH_UNIT value) __attribute__ ((weak));
extern void VSMediaRec_RecordStartWithWriting(UINT32 value) __attribute__ ((weak));

extern void VSMediaRec_SetValidVidBS(SMEDIAREC_BS_FROMSENSOR *pBS) __attribute__ ((weak));
extern void VSMediaRec_AllDirect_EncodeH264(void) __attribute__ ((weak));
extern UINT32 VSMediaRecVidTrig_GetOutputAddr(void) __attribute__ ((weak));
extern void VSMediaRec_VtrigOK(void) __attribute__ ((weak));
extern void VSMediaRec_IPLOK_GoNext(void) __attribute__ ((weak));
extern VIDENC_PATH VSMediaRec_GetEncPath(UINT32 uiVidEncId) __attribute__ ((weak));
extern void VSMediaRec_Vtrig_NoKickH264(UINT32 value) __attribute__ ((weak));
extern void VSMediaRec_Set3DNROutAddr(UINT32 pathID, UINT32 uiFrameId, UINT32 uiYAddr, UINT32 uiUVAddr) __attribute__ ((weak));
extern void VSMediaRec_Trigger3DNROut(UINT32 pathID) __attribute__ ((weak));
extern void VSMediaRec_SetEncPath(UINT32 uiVidEncId, VIDENC_PATH path) __attribute__ ((weak));
extern ER   VSMediaRec_AddOne(MOVREC_ADD_INFO *pinfo) __attribute__ ((weak));
extern void     VidStrm_VidGetDesc(UINT32 vsID, UINT32 *pAddr, UINT32 *pSize) __attribute__ ((weak));
extern ER       VSFakeMedia_Record(void) __attribute__ ((weak));
extern MOVREC_STATUS_TYPE VSFakeMedia_GetStatus(void) __attribute__ ((weak));
extern ER       VSFakeMedia_GetInfo(UINT32 type, UINT32 *p1, UINT32 *p2, UINT32 *p3) __attribute__ ((weak));
extern ER       VSFakeMedia_Open(MEDIAREC_OBJ *param) __attribute__ ((weak));
extern ER       VSFakeMedia_Stop(VSMEDIAREC_STOP_TYPE waitType) __attribute__ ((weak));
extern ER       VSFakeMedia_Close(void) __attribute__ ((weak));
extern UINT32   VSMediaRec_GetFreeRecSec(void) __attribute__ ((weak));
extern void     VSMediaRec_StopFile_Waiting(UINT32 pathID, UINT32 wait) __attribute__ ((weak));


extern void     VSMediaRec_SetvidBs_RevSec(UINT32 value) __attribute__ ((weak));
extern void     VSMediaRec_SetvidBs_NumInBs(UINT32 value) __attribute__ ((weak));

//@}
#endif
