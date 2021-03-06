/**
    Header file of SMedia Recorder Task

    Exported header file of media recorder task.

    @file       SMediaRecAPI.h
    @ingroup    mIAPPSMEDIAREC
    @note       add gSMediaRecPwrOffPT

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _VIDSTRMAPI_H
#define _VIDSTRMAPI_H

#include "movieinterface_def.h"
extern void VidStrm_InstallID(void);


/**
     Install Task, Sem, Flag ID for VidStrm.

*/
extern void VidStrm_InstallID(void) _SECTION(".kercfg_text") __attribute__ ((weak));

typedef struct
{
    UINT32 MAXwidth;
    UINT32 MAXheight;
    UINT32 MAXtargetbitrate;   //MAX Byte per second
    UINT32 width;
    UINT32 height;
    UINT32 codectype[VIDEOENC_CODECTYPE_MAX];       //MEDIAVIDENC_H264/MEDIAVIDENC_MJPG
    UINT32 targetbitrate;   //Byte per second
    UINT32 vidframerate;    //FPS
    UINT32 vsid; //out
} VIDSTRM_VID_INFO;

/**
    Dump FakeMediaStatus.

    @param[in] pData    pointer to data,p1=width,p2=height,p3=bpp,p4=fps
    @return number of data
*/
extern UINT32 VSFakeMedia_DumpDataStatus(UINT32* pData) __attribute__ ((weak));

/**
    Get SMedia recording parameters.

    @param[in] type MOVREC_GETRECPARAM_TARGETBR or others

    @return ER
        - @b E_OK:  OK
        - @b E_SYS: params error
*/
extern ER VSFakeMedia_GetParam(UINT32 type, UINT32 *pParam1, UINT32 *pParam2, UINT32 *pParam3) __attribute__ ((weak));

extern ER       VidStrm_Stop(void) __attribute__ ((weak));
extern ER       VidStrm_OpenAlone(MEDIAREC_OBJ *param) __attribute__ ((weak));
extern ER       VidStrm_Close(void) __attribute__ ((weak));
extern ER       VidStrm_Record(void) __attribute__ ((weak));
extern ER       VSFakeMedia_ReAllocBuf2VA(UINT32 memStart, UINT32 memSize) __attribute__ ((weak));
extern ER       VidStrm_Stop(void) __attribute__ ((weak));
extern ER       VidStrm_PutReadyBufWithID(UINT32 vsID, MEDIAREC_READYBUF_INFO *pInfo) __attribute__ ((weak));
extern void     VidStrm_GetReadyBufWithID(UINT32 vsID, MEDIAREC_READYBUF_INFO *pInfo) __attribute__ ((weak));
extern void     VidStrm_SetReadyBufWithID(UINT32 vsID, MEDIAREC_READYBUF_INFO *pInfo) __attribute__ ((weak));
extern ER       VidStrm_OpenFromMedia(void) __attribute__ ((weak));
extern UINT32   VidStrm_ResetAllPath(void) __attribute__ ((weak));
extern UINT32   VidStrm_AddOne(VIDSTRM_VID_INFO *pinfo) __attribute__ ((weak));
extern void     VidStrm_PauseAndStop(UINT32 result) __attribute__ ((weak));
//extern void     VidStrm_SetH264GopNum(UINT32 gopnum) __attribute__ ((weak));
extern ER       VidStrm_GetInfo(UINT32 type, UINT32 *p1, UINT32 *p2, UINT32 *p3) __attribute__ ((weak));
extern void     VidStrm_PathN_Start_Stop(UINT32 path, UINT32 enable) __attribute__ ((weak));
extern void     VidStrm_InstallCmd(void) __attribute__ ((weak));
extern void     VidStrm_SetH264GopNumWithID(UINT32 gopnum, UINT32 vsID) __attribute__ ((weak));

//backward compatible
extern void     VSMediaRec_VideoDoEnc(void) __attribute__ ((weak));
extern void     VidStrm_SetReadyBuf4RawEnc(UINT32 fileid, MEDIAREC_READYBUF_INFO *pInfo) __attribute__ ((weak));
extern ER       VidStrm_ChangeParam(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3) __attribute__ ((weak));
extern void     VidStrm_SetValidPathNum(UINT32 value) __attribute__ ((weak));
extern void     VidStrm_PathAll_Stop(UINT32 stop) __attribute__ ((weak));
extern ER       VSFakeMediaWithAudio_Open(MEDIAREC_OBJ *param) __attribute__ ((weak));
extern ER       VidStrm_UpdateParam(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3) __attribute__ ((weak));
extern BOOL     VSFakeMedia_IsRecording(void) __attribute__ ((weak));
extern MOVREC_STATUS_TYPE VSFakeMedia_GetStatus(void) __attribute__ ((weak));
extern void     VSWA_Record_OpenAll(void) __attribute__ ((weak));
extern UINT32   VidStrm_GetLastV_YUV(UINT32 vsID, MEDIAREC_YUV_INFO* pInfo) __attribute__ ((weak));
extern ER       VSMediaRecAudClose(void) __attribute__ ((weak));

extern void     VidStrm_ChangeVideoTBR(UINT32 vsID, UINT32 value) __attribute__ ((weak));
extern void     VidStrm_PathN_GoStreaming(UINT32 path, UINT32 enable) __attribute__ ((weak));
extern void     VSMediaRec_UpdateParam(UINT32 wid, UINT32 hei, UINT32 fps, UINT32 TBR) __attribute__ ((weak));
extern void 	VidStrmEnc_Set3DNRCallback(UINT32 pathid) __attribute__ ((weak));
extern void     VidStrmEnc_Register3DNRCb(UINT32 pathid, MEDIAREC_3DNRCB CB) __attribute__ ((weak));
extern void     VSMediaRec_RegisterCB_Start(MediaRecCB_Start IPLCb) __attribute__ ((weak));
extern void     VSMediaRec_RegisterCB_Get(MediaRecCB_Get IPLCb) __attribute__ ((weak));
extern void     VidStrm_ResetI(UINT32 vsID) __attribute__ ((weak));
extern void     VidStrm_EnLastQP(UINT32 en) __attribute__ ((weak));
extern void     VidStrm_ChangeH264DB(UINT32 vsID, UINT32 value) __attribute__ ((weak));
extern void     VidStrm_ChangeH264DB_Alpha(UINT32 vsID, UINT32 value) __attribute__ ((weak));
extern void     VidStrm_ChangeH264DB_Beta(UINT32 vsID, UINT32 value) __attribute__ ((weak));
#if 0

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
    SMEDIAREC_NO_WAIT  = 0x0,  ///< stop recording and no wait
    SMEDIAREC_WAIT_END = 0x1,   ///< stop recording and wait this action finished
    ENUM_DUMMY4WORD(SMEDIAREC_STOP_TYPE)
} SMEDIAREC_STOP_TYPE;

//#NT#2010/05/17#Meg Lin -begin
//#NT#add restriction type

/**
    check type for SMediaRec_SetWriteCard()
*/
typedef enum
{
    SMEDIAREC_WRITE_PATH1  = 0x0,  ///< write path 1 to card
    SMEDIAREC_WRITE_PATH2  = 0x1,  ///< write path 2 to card
    ENUM_DUMMY4WORD(SMEDIAREC_WRITECARD_TYPE)
} SMEDIAREC_WRITECARD_TYPE;


typedef struct
{
    UINT32 bsAddr;
    UINT32 bsSize;
    UINT32 iskey;    //is key-frame or not
} SMEDIAREC_BS_FROMSENSOR;


//#NT#2012/09/12#Meg Lin -begin
/**
    @name SMedia ImmediatelyProc callback ID

    Callback ID of Smedia ImmediatelyProc.
*/
//@{
#define     SMEDIAREC_IMMPROC_CUTFILE_END        MOVREC_IMMPROC_CUTFILE_END ///< end of one cut file, no parameter
#define     SMEDIAREC_IMMPROC_DELETE_FILE        MOVREC_IMMPROC_DELETE_FILE ///< end of deleting file, p1: directory ID, p2: file ID
#define     SMEDIAREC_IMMPROC_ENC_SETTING        MOVREC_IMMPROC_ENC_SETTING ///< special encoding parameters setting, such as H.264 3DNR
//@}
//#NT#2012/09/12#Meg Lin -end

/**
    @name SMedia record parameters

    Media recording changeable parameters.
*/
//@{
#define SMEDIAREC_RECPARAM_WIDTH             MOVREC_RECPARAM_WIDTH   ///< width, p1 valid, p2 fileid
#define SMEDIAREC_RECPARAM_HEIGHT            MOVREC_RECPARAM_HEIGHT  ///< height, p1 valid, p2 fileid
#define SMEDIAREC_RECPARAM_FRAMERATE         MOVREC_RECPARAM_FRAMERATE///< frameRate, p1 valid
#define SMEDIAREC_RECPARAM_QUALITY           MOVREC_RECPARAM_QUALITY ///< quality, p1 valid
#define SMEDIAREC_RECPARAM_TARGETRATE        MOVREC_RECPARAM_TARGETRATE ///< target rate (bytes per second), p1 valid, p2 fileid
#define SMEDIAREC_RECPARAM_GOLFSHOT_ON       MOVREC_RECPARAM_GOLFSHOT_ON///< change recFormat to golfshot, p1 0 or 1
#define SMEDIAREC_RECPARAM_CUTSEC            MOVREC_RECPARAM_CUTSEC  ///< seamless cut sec
#define SMEDIAREC_RECPARAM_CUTSIZE           MOVREC_RECPARAM_CUTSIZE ///< seamless cut size
#define SMEDIAREC_RECPARAM_ENDTYPE           MOVREC_RECPARAM_ENDTYPE ///< end type
#define SMEDIAREC_RECPARAM_FLASH_ON          MOVREC_RECPARAM_FLASH_ON///< flash recording on/off, p1 0 or 1
#define SMEDIAREC_RECPARAM_PWROFFPT          MOVREC_RECPARAM_PWROFFPT///< enable/disable poweroff_protection, default: on
#define SMEDIAREC_RECPARAM_GPSON             MOVREC_RECPARAM_GPSON   ///< enable/disable gps info on, default: off
#define SMEDIAREC_RECPARAM_TIMELAPSE         MOVREC_RECPARAM_TIMELAPSE///< set time lapse recording mode (ON/OFF)
#define SMEDIAREC_RECPARAM_TIMELAPSE_TIME    MOVREC_RECPARAM_TIMELAPSE_TIME///< set time lapse recording time interval (in unit of ms, should be larger than 1/30 sec)
#define SMEDIAREC_RECPARAM_THUMB_DATA        MOVREC_RECPARAM_THUMB_DATA///< movie thumbnail settings, p1:on/off, p2:width, p3:height
#define SMEDIAREC_RECPARAM_AUD_SAMPLERATE    MOVREC_RECPARAM_AUD_SAMPLERATE///< audio sample rate, AUDIO_SR_8000 or others
#define SMEDIAREC_RECPARAM_AUD_CHS           MOVREC_RECPARAM_AUD_CHS ///< audio channels, 1 or 2
#define SMEDIAREC_RECPARAM_AUD_SRC           MOVREC_RECPARAM_AUD_SRC ///< audio source, MEDIAREC_AUDTYPE_LEFT or others
#define SMEDIAREC_RECPARAM_AUD_CODEC         MOVREC_RECPARAM_AUD_CODEC///< audio codec type, MEDIAREC_ENC_PCM or others
#define SMEDIAREC_RECPARAM_RECFORMAT         MOVREC_RECPARAM_RECFORMAT///< recording format, MEDIAREC_AUD_VID_BOTH or others
#define SMEDIAREC_RECPARAM_EN_CUSTOMUDATA    MOVREC_RECPARAM_EN_CUSTOMUDATA///< enable custom user data or not
#define SMEDIAREC_RECPARAM_VIDEOCODEC        MOVREC_RECPARAM_VIDEOCODEC///< video codec type, MEDIAREC_ENC_H264 or others
#define SMEDIAREC_RECPARAM_H264GOPTYPE       MOVREC_RECPARAM_H264GOPTYPE///< h264 GOP type, MEDIAREC_H264GOP_IPONLY or others
#define SMEDIAREC_RECPARAM_H2643DNRLEVEL     MOVREC_RECPARAM_H2643DNRLEVEL///< h264 3DNR level
#define SMEDIAREC_RECPARAM_RAWENC_Q          MOVREC_RECPARAM_RAWENC_Q///< Q value for raw-encoding
#define SMEDIAREC_RECPARAM_EN_AUDFILTER      MOVREC_RECPARAM_EN_AUDFILTER///< enable audio filter or not
#define SMEDIAREC_RECPARAM_DROP_FRAME_NUM    MOVREC_RECPARAM_DROP_FRAME_NUM///< drop frame number as start recording (for AV sync)
#define SMEDIAREC_RECPARAM_H264QPLEVEL       MOVREC_RECPARAM_H264QPLEVEL///< h264 QP initial level
//#NT#2013/04/17#Calvin Chang#Support Rotation information in Mov/Mp4 File format -begin
#define SMEDIAREC_RECPARAM_MOV_ROTATE        MOVREC_RECPARAM_MOV_ROTATE///< MOV/MP4 file rotation, MEDIAREC_MOV_ROTATE_0
//#NT#2013/04/17#Calvin Chang -end
#define SMEDIAREC_RECPARAM_FILETYPE_2        MOVREC_RECPARAM_FILETYPE_2///< file1 type, MEDIAREC_AVI as default
#define SMEDIAREC_RECPARAM_MERGEYUV          MOVREC_RECPARAM_MERGEYUV
#define SMEDIAREC_RECPARAM_FILETYPE          MOVREC_RECPARAM_FILETYPE ///< file type, MEDIAREC_MOV, MEDIAREC_AVI, MEDIAREC_MP4
#define SMEDIAREC_RECPARAM_JPG_YUVFORMAT     MOVREC_RECPARAM_JPG_YUVFORMAT///< set jpeg yuv format
#define SMEDIAREC_RECPARAM_MULTIREC_ON       MOVREC_RECPARAM_MULTIREC_ON
#define SMEDIAREC_RECPARAM_LIVEVIEW          MOVREC_RECPARAM_LIVEVIEW ///< set MEDIAREC_LIVEVIEW recformat 2013/10/02 Meg
#define SMEDIAREC_RECPARAM_DAR               MOVREC_RECPARAM_DAR      ///< video display aspect ratio
#define SMEDIAREC_RECPARAM_PRE_ENCODE        MOVREC_RECPARAM_PRE_ENCODE///< video pre-encode (for applying 3DNR to 1st frame)
#define SMEDIAREC_RECPARAM_SENSOR_FPS        MOVREC_RECPARAM_SENSOR_FPS///< video sensor fps
#define SMEDIAREC_RECPARAM_SENSOR_NUM        MOVREC_RECPARAM_SENSOR_NUM///< input sensor number (for encoding path judgement)
#define SMEDIAREC_RECPARAM_SVC               MOVREC_RECPARAM_SVC      ///< set SVC disable/enable and GOP number
#define SMEDIAREC_RECPARAM_TINYMODE_ON       MOVREC_RECPARAM_TINYMODE_ON///< tiny mode on/off, p1 0 or 1
#define SMEDIAREC_RECPARAM_RAWENC_ON         MOVREC_RECPARAM_RAWENC_ON///< tiny mode on/off, p1 0 or 1
//@}

/**
    @name Valid video frame rate

    Valid video frame rate.
*/
//@{
#define SMEDIAREC_VRATE_2            2  ///< 2 fps, only for MEDIAREC_FAST_FWD
#define SMEDIAREC_VRATE_10           10 ///< 10 fps
#define SMEDIAREC_VRATE_15           15 ///< 15 fps
#define SMEDIAREC_VRATE_20           20 ///< 20 fps
#define SMEDIAREC_VRATE_24           24 ///< 24 fps
#define SMEDIAREC_VRATE_25           25 ///< 25 fps
#define SMEDIAREC_VRATE_30           30 ///< 30 fps
#define SMEDIAREC_VRATE_48           48  ///< 48 fps, only for 960p/720p/VGA/QVGA
#define SMEDIAREC_VRATE_50           50  ///< 50 fps, only for 960p/720p/VGA/QVGA
#define SMEDIAREC_VRATE_60           60  ///< 60 fps, only for 720p/VGA/QVGA
#define SMEDIAREC_VRATE_120          120 ///< 120 fps, only for VGA/QVGA
#define SMEDIAREC_VRATE_240          240 ///< 240 fps, only for QVGA
//@}

/**
    @name Valid file type

    Valid file type.
*/
//@{
#define SMEDIAREC_AVI    MEDIA_FILEFORMAT_AVI    ///< .avi file
#define SMEDIAREC_MOV    MEDIA_FILEFORMAT_MOV    ///< .mov file
#define SMEDIAREC_MP4    MEDIA_FILEFORMAT_MP4    ///< .mp4 file
//@}

/**
    @name Valid video encode codec

    Valid video encode codec.
*/
//@{
#define SMEDIAREC_ENC_JPG    MEDIAVIDENC_MJPG    ///< motion JPG
#define SMEDIAREC_ENC_H264   MEDIAVIDENC_H264    ///< H.264
//@}

/**
    @name Valid audio encode codec

    Valid audio encode codec.
*/
//@{
#define SMEDIAREC_ENC_PCM    MOVAUDENC_PCM   ///< PCM
#define SMEDIAREC_ENC_AAC    MOVAUDENC_AAC   ///< AAC
#define SMEDIAREC_ENC_PPCM   MOVAUDENC_PPCM  ///< PPCM
//@}

/**
    @name Valid video input YCbCr format

    Valid video input YCbCr format.
*/
//@{
#define SMEDIAREC_JPEG_FORMAT_420    MOVREC_JPEG_FORMAT_420  ///< Y:Cb:Cr = 4:1:1
#define SMEDIAREC_JPEG_FORMAT_422    MOVREC_JPEG_FORMAT_422  ///< Y:Cb:Cr = 2:1:1
#define SMEDIAREC_JPEG_FORMAT_444    MOVREC_JPEG_FORMAT_444  ///< Y:Cb:Cr = 1:1:1
#define SMEDIAREC_JPEG_FORMAT_100    MOVREC_JPEG_FORMAT_100  ///< Y:Cb:Cr = 4:1:1
//@}

/**
    @name Valid counting type

    Valid counting type for MediaRec_Disk2SecondWithType.
*/
//@{
#define SMEDIAREC_COUNTTYPE_FS    MOVREC_COUNTTYPE_FS   ///< get freespace in storage
#define SMEDIAREC_COUNTTYPE_4G    MOVREC_COUNTTYPE_4G   ///< using 4G to calculate
#define SMEDIAREC_COUNTTYPE_USER  MOVREC_COUNTTYPE_USER ///< using given size to calculate
//@}

/**
    @name Valid end type

    Valid recording end type. Normal end, cut overlap, or cut till card full.
*/
//@{
#define SMEDIAREC_ENDTYPE_NORMAL             MOVREC_ENDTYPE_NORMAL ///< normal end, such as press key to stop recording
#define SMEDIAREC_ENDTYPE_CUTOVERLAP         MOVREC_ENDTYPE_CUTOVERLAP///< cut file and continue recording automatically; overlap the old files (cyclic recording)
#define SMEDIAREC_ENDTYPE_CUT_TILLCARDFULL   MOVREC_ENDTYPE_CUT_TILLCARDFULL///< cut file and continue recording automatically till card full
#define SMEDIAREC_ENDTYPE_MAX                MOVREC_ENDTYPE_MAX    ///< maximum number of recording end type
//@}

//#NT#2012/01/09#Hideo Lin -begin
//#NT#Add media recording source type (for test mode)
/**
    @name Media recording source type

    Media recording video source type: normal, fixed YUV pattern, bitstream;
    Media recording audio source type: normal, fixed PCM pattern.
*/
//@{
#define SMEDIAREC_SRC_NORMAL         MOVREC_SRC_NORMAL     ///< normal source (video: sensor, audio: mic-in)
#define SMEDIAREC_SRC_FIXED_YUV      MOVREC_SRC_FIXED_YUV  ///< fixed YUV pattern (for video)
#define SMEDIAREC_SRC_BS_ONLY        MOVREC_SRC_BS_ONLY    ///< bitstream only (for video)
#define SMEDIAREC_SRC_FIXED_PCM      MOVREC_SRC_FIXED_PCM  ///< fixed PCM pattern (for audio)
//@}
//#NT#2012/01/09#Hideo Lin -end

/**
    @name IPL mode change callback event

    Event ID for changing IPL mode callback function.
*/
//@{
#define SMEDIAREC_IPLCHG_PREVIEW     MOVREC_IPLCHG_PREVIEW    ///< Set IPL to preview mode
#define SMEDIAREC_IPLCHG_VIDEO       MOVREC_IPLCHG_VIDEO      ///< Set IPL to video mode
#define SMEDIAREC_IPLCHG_INITBS      MOVREC_IPLCHG_INITBS     ///< initbs
//@}

/**
    @name Get IPL ready buffer callback event

    Event ID for getting IPL ready buffer callback function.
*/
//@{
#define SMEDIAREC_IPLGET_READYBUF    MOVREC_IPLGET_READYBUF   ///< Get IPL ready buffer
//@}

/**
    @name Smedia record information

    SMedia recording getting information.
*/
//@{
#define     SMEDIAREC_GETINFO_WIDTH_HEIGHT   MOVREC_GETINFO_WIDTH_HEIGHT    ///< p1: width (out), p2: height (out)
#define     SMEDIAREC_GETINFO_VIDEO_CODEC    MOVREC_GETINFO_VIDEO_CODEC     ///< p1: videocodec type(out), MEDIAREC_ENC_JPG or others//2012/10/15 Meg
#define     SMEDIAREC_GETINFO_AUDIO_SR       MOVREC_GETINFO_AUDIO_SR        ///< p1: audio samplerate(out), AUDIO_SR_32000 or others//2013/01/11 Meg
#define     SMEDIAREC_GETINFO_AUDIO_CHS      MOVREC_GETINFO_AUDIO_CHS       ///< p1: audio channels(out), 1 or 2//2013/01/11 Meg
#define     SMEDIAREC_GETINFO_AUDIO_BITS     MOVREC_GETINFO_AUDIO_BITS      ///< p1: audio bits per sample, 16
#define     SMEDIAREC_GETINFO_H264_SPS       MOVREC_GETINFO_H264_SPS        ///< p1: addr (out), p2: size (out), p3: num(0 or 1)
#define     SMEDIAREC_GETINFO_VIDEO_FPS      MOVREC_GETINFO_VIDEO_FPS       ///< p1: v fps (out)
#define     SMEDIAREC_GETINFO_WIDTH_HEIGHT_P2 MOVREC_GETINFO_WIDTH_HEIGHT_P2  ///< p1: width of Path2 (out), p2: height of Path2(out)
#define     SMEDIAREC_GETINFO_AUDIO_CODEC    MOVREC_GETINFO_AUDIO_CODEC     ///< p1: audio codec type of 1stpath(out), MEDIAREC_ENC_PCM or others
//@}

/**
    @name Smedia audio type information

    SMedia recording audio type information.
*/
//@{
#define     SMEDIAREC_AUDTYPE_LEFT   MOVREC_AUDTYPE_LEFT   ///< left
#define     SMEDIAREC_AUDTYPE_RIGHT  MOVREC_AUDTYPE_RIGHT  ///< right
#define     SMEDIAREC_AUDTYPE_STEREO MOVREC_AUDTYPE_STEREO ///< stereo
#define     SMEDIAREC_AUDTYPE_MONO   MOVREC_AUDTYPE_MONO   ///< mono
//@}

//#NT#2013/04/17#Calvin Chang#Support Rotation information in Mov/Mp4 File format -begin
/**
    @name Smedia rotation angle information

    SMedia recording roation angle information.
*/
//@{
#define     SMEDIAREC_MOV_ROTATE_0       MOVREC_MOV_ROTATE_0   ///< angle 0
#define     SMEDIAREC_MOV_ROTATE_90      MOVREC_MOV_ROTATE_90   ///< angle 90
#define     SMEDIAREC_MOV_ROTATE_180     MOVREC_MOV_ROTATE_180   ///< angle 180
#define     SMEDIAREC_MOV_ROTATE_270     MOVREC_MOV_ROTATE_270   ///< angle 270
//@}
//#NT#2013/04/17#Calvin Chang -end

/**
    Status type for SMediaRec_GetStatus()
*/
typedef enum
{
    SMEDIAREC_STATUS_NOT_OPENED        = 0x1,  ///< media recorder task is not open
    SMEDIAREC_STATUS_OPENED_NOT_RECORD = 0x2,  ///< media recorder task is open, not recording
    SMEDIAREC_STATUS_RECORDING         = 0x3,  ///< media recorder task is recording
    ENUM_DUMMY4WORD(SMEDIAREC_STATUS_TYPE)
} SMEDIAREC_STATUS_TYPE;

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
} SMEDIAREC_READYBUF_INFO;

/**
     Install Task, Sem, Flag ID for SMediaRec.

*/
extern void SMediaRec_InstallID(void) _SECTION(".kercfg_text");

/*
    Public Smedia recording functions
*/
//MediaRecTsk.c
/**
    Open Smedia recorder task.

    Open Smedia recorder task to prepare for recording.

    @param[in] param    the media recorder object

    @return
        - @b E_OK:  open Smedia recorder task successfully
        - @b E_SYS: Smedia recorder task is already opened
*/
extern ER       SMediaRec_Open(MEDIAREC_OBJ *param);

/**
    Close Smedia recorder task.

    Close Smedia recorder related tasks and stop recording if necessary.

    @return
        - @b E_OK:  terminate media recorder task successfully
        - @b E_SYS: Smedia recorder task is not opened
*/
extern ER       SMediaRec_Close(void);

/**
    Start Smedia recording.

    @return
        - @b E_OK:  start recording successfully
        - @b E_SYS: Smedia recorder is recording or not ready
        - @b E_PAR: recorder setting error (such as recording format, video codec type, ...)
*/
extern ER       SMediaRec_Record(void);

/**
    Stop Smedia recording.

    @param[in] waitType waiting type as media recording stop, MEDIAREC_NO_WAIT or MEDIAREC_WAIT_END

    @return
        - @b E_OK:  stop recording successfully
        - @b E_SYS: media recorder is not recording
*/
extern ER       SMediaRec_Stop(SMEDIAREC_STOP_TYPE waitType);

/**
    Change Smedia recorder parameters.

    Change Smedia recorder parameters, such as width, height, frame rate, target bit rate, ..., and so on.

    @note

    @param[in] type parameter type
    @param[in] p1   parameter1
    @param[in] p2   parameter2
    @param[in] p3   parameter3

    @return
        - @b E_OK:  change successfully
        - @b E_PAR: parameter value error
*/
extern ER       SMediaRec_ChangeParameter(UINT32 type, UINT32 p1, UINT32 p2, UINT32 p3);

/**
    Get Smedia recorder parameter settings.

    Get Smedia recorder parameter settings, only support getting width and height information currently.

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
extern ER       SMediaRec_GetInfo(UINT32 type, UINT32 *p1, UINT32 *p2, UINT32 *p3);

/**
    Set user data.

    Set user data for media container.

    @note The user data is in 'JUNK' chunk for AVI file, and in 'udta' atom for MOV/MP4 file.

    @param[in] addr user data starting address
    @param[in] size user data size

    @return void
*/
extern void     SMediaRec_SetUserData(UINT32 addr, UINT32 size);
extern void     SMediaRec_SetCustomDataWithID(UINT32 id, UINT32 tag, UINT32 addr, UINT32 size);

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
extern UINT32   SMediaRec_Disk2SecondWithType(UINT32 type, UINT32 size);//2010/01/25 Meg

/**
    Set restriction for recording time.

    Set restriction to calculate remaining time for Smedia recording.

    @note

    @param[in] type     restriction type, SMEDIAREC_RESTR_INDEXBUF or SMEDIAREC_RESTR_FSLIMIT
    @param[in] value    set FALSE to disable, and others to enable the related restriction

    @return void
*/
extern void     SMediaRec_SetDisk2SecRestriction(MEDIAREC_RESTR_TYPE type, UINT32 value);//2010/05/17 Meg Lin

/**
    Set free user data.

    Set free user data for special purpose.

    @note It's only valid for MOV/MP4 file. The data will be put in 'fre1' atom.

    @param[in] addr free data starting address
    @param[in] size free data size

    @return void
*/
extern void     SMediaRec_SetFre1Data(UINT32 addr, UINT32 size);

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
extern ER       SMediaRec_ReAllocBuf2VA(UINT32 memStart, UINT32 memSize);

/**
    Stop Smedia recording and set read-only.

    Stop Smedia recording and set previous file, current file and next file to be read-only.

    @return
        - @b E_OK:  stop recording successfully
        - @b E_SYS: media recorder is not recording
*/
extern ER       SMediaRec_StopAndReadOnly(void);//2012/02/17 Meg

/**
    To get Smedia recorder status.

    @return
        - @b SMEDIAREC_STATUS_NOT_OPENED:        Smedia recorder is not opened
        - @b SMEDIAREC_STATUS_OPENED_NOT_RECORD: Smedia recorder is opened but not recording
        - @b SMEDIAREC_STATUS_RECORDING:         Smedia recorder is recording
*/
extern SMEDIAREC_STATUS_TYPE   SMediaRec_GetStatus(void);

/**
    Set audio volume for SMedia Recorder.

    @param[in] percent  the percentage of audio gain level (0% - 100%)
    @return
        - @b E_OK:  set successfully
*/
extern ER SMediaRec_SetAudioVolume(UINT32 percent);


extern void     SMediaRec_Sleep(void);
extern void     SMediaRec_Wakeup(void);
extern void     SMediaRec_PauseDMA(void);
extern void     SMediaRec_ResumeDMA(void);
extern void     SMediaRec_ChangeDisplaySize(UINT32 uiDisp_w, UINT32 uiDisp_h);


//SMediaVideoRecTsk.c
/**
    Set ready buffer for Smedia recorder.

    Set ready buffer information, such as Y, Cb, Cr starting address and line offset for media recorder.

    @note This function is usually used in IPLGetReadyCB (get IPL ready buffer callback) of media recorder object.
          In the callback, get IPL ready buffer from IPL driver, then use this API to set to media recorder.

    @param[in] pInfo    ready buffer information for media recorder

    @return void
*/
extern void     SMediaRec_ResetReadyBuf(void);
extern void     SMediaRec_PutReadyBuf(MEDIAREC_READYBUF_INFO *pInfo);
extern void     SMediaRec_GetReadyBuf(MEDIAREC_READYBUF_INFO *pInfo);
extern void     SMediaRec_SetReadyBuf(MEDIAREC_READYBUF_INFO *pInfo);//2012/08/30 Meg
extern ER       SMediaRec_PutReadyBufWithID(UINT32 fileid, MEDIAREC_READYBUF_INFO *pInfo);
extern void     SMediaRec_GetReadyBufWithID(UINT32 fileid, MEDIAREC_READYBUF_INFO *pInfo);
extern void     SMediaRec_SetReadyBufWithID(UINT32 fileid, MEDIAREC_READYBUF_INFO *pInfo);//2012/08/30 Meg
extern void     SMediaRec_SetReadyBuf4RawEnc(UINT32 fileid, MEDIAREC_READYBUF_INFO *pInfo);


//SMediaAudioRecTsk.c
extern void     SMediaRec_Pause(void);
extern void     SMediaRec_Resume(void);

//SMediaRecFSTsk.c
/**
    Set GPS data.

    Set GPS data for Smedia container.

    @note It's only valid for AVI file currently. The GPS data is put in 'JUNK' chunk per second.

    @param[in] addr GPS data starting address
    @param[in] size GPS data size

    @return void
*/
extern void     SMediaRec_SetGPSData(UINT32 addr, UINT32 size);//2011/06/23 Meg

/**
    Set crash event.

    Set current recording file to be renamed and read-only.

    @note

    @return void
*/
extern void     SMediaRec_SetCrash(void);//2011/06/27 Meg

/**
    Cancel crash event.

    Cancel crash event.

    @note

    @return void
*/
extern void     SMediaRec_CancelCrash(void);//2013/11/18 Meg


/**
    To use same crash DID or not.

    To use same or different DCF directory ID for crash event.

    @note Set TRUE to use same directory ID, FALSE to use different directory ID.

    @param[in] on   turn on/off same crash DID function

    @return void
*/
extern void     SMediaRec_SetTheSameCrashDid(UINT8 on);//2011/09/15 Meg

/**
    Set crash folder name.

    Set DCF folder name for crash event.

    @note It should follow the DCF naming rule, that is, 5 characters in 'A'~'Z', 'a'~'z', '0'~'9', and '_'.

    @param[in] pChar    pointer to the folder name

    @return void
*/
extern void     SMediaRec_SetCrashFolderName(char *pChar);//2011/09/15 Meg

/**
    Set naming rule for SMedia Recorder.

    @param[in] pHdl    pointer to the naming rule
    @return
        - @b E_OK:  set successfully
*/
extern ER SMediaRec_SetFSNamingHandler(MEDIANAMINGRULE *pHdl);


extern void SMediaRecCmdTsk(void);
extern void SMediaRecTsk(void);
extern void SMediaVideoRecTsk(void);
extern void SMRawEncTsk(void);
extern void SMediaAudioEncTsk(void);
extern void SMediaRecFSTsk(void);
extern void SMediaRecVideoStrmTsk(void);
extern void SMediaRecMdatTsk(void);


extern void SMediaRec_SetTestMode(MEDIAREC_TESTMODE *pTestMode);
extern void SMediaRec_SetIPLChangeCB(MediaRecIPLChangeCB *pIPLChangeCB);
extern void SMediaRec_VideoDoEnc(void);
extern void SMediaRec_GetEncInfo(UINT32 puiParam[7]);
extern void SMediaRec_SetFileHandle(FST_FILE fhdl, UINT32 fileid);
extern void SMediaRecFS_SetWorkingID(UINT32 id);
extern UINT32   SMediaRec_GetNowVideoFrames(void);

/**
    Open Smedia recording error msg.

    @param[in] on  1 to open error msg
    @return void
*/
extern void SMediaRec_OpenErrMsg(UINT8 on);

/**
    Reset Smedia recorder containerMaker.

    @return void
*/
extern void SMediaRec_ResetConMaker(void);

/**
    Open Smedia recording warning msg.

    @param[in] on  1 to open warning msg
    @return void
*/
extern void SMediaRec_OpenWrnMsg(UINT8 on);

/**
    Open media recording seamless recording msg.

    @param[in] on  1 to open seamless recording msg
    @return void
*/
extern void SMediaRec_OpenCutMsg(UINT8 on);

/**
    Open media recording must msg.

    @param[in] on  1 to open must msg
    @return void
*/
extern void SMediaRec_OpenMustMsg(UINT8 on);

/**
    Check Smedia recorder if recording.

    @param[in] void
    @return
        - @b TRUE:  recording
        - @b FALSE: not recording
*/
extern BOOL SMediaRec_IsRecording(void);

/**
    Set memory addr and size for MergeYUV.

    @param[in] addr memory address
    @param[in] size memory size
    @return void
*/
extern void SMediaRec_SetGiveYUVBuf(UINT32 addr, UINT32 size);

/**
    Set readyYUV address for MergeYUV.

    @param[in] ptr    pointer to YUV info
    @return void
*/
extern void SMediaRec_GiveYUV(MEDIAREC_READYBUF_INFO *ptr);

/**
    Set Extend crash.
    If seamless recording, this will set Crash after 30 sec.
    Max 3 times. If busy, return E_SYS.

    @param[in] void
    @return
        - @b E_OK: ok
        - @b E_SYS: busy
*/
extern ER SMediaRec_SetExtendCrash(void);//2013/09/04 Meg

/**
    Dump MediaStatus.

    @param[in] pData    pointer to data,p1=width,p2=height,p3=bpp,p4=fps
    @return number of data
*/
extern UINT32 SMediaRec_DumpDataStatus(UINT32* pData);//2013/08/15

/**
    Get lastest video bitstream addr,size.

    @param[in] frameBuf    pointer to video frame
    @return 0
*/
extern UINT32 SMediaRec_GetLastV_BS(UINT32 pathID, MEDIAREC_MEM_RANGE* frameBuf);

/**
    Force SMediaRec to check FS_DiskInfo.

    @param[in] void
    @return void
*/
extern void SMediaRec_CheckFSDiskInfo(void);

/**
    Open Smedia recording FSObj msg.

    @param[in] on  1 to open FSObj msg
    @return void
*/
extern void SMediaRec_OpenFSOBJMsg(UINT8 on);

/**
    Open Smedia recording file msg.

    @param[in] on  1 to open file msg
    @return void
*/
extern void SMediaRec_OpenFileMsg(UINT8 on);

/**
    Set path2 write card or not.

    @param[in] on  1 to write path2 to card, if path2 enable
    @return void
*/
extern void SMediaRec_SetPath2WriteCard(UINT32 on);


/**
    Set path2 IPL get ready buffer callback.

    @param[in] IPLCb  path2 IPL get ready buffer callback
    @return void
*/
extern void SMediaRec_RegisterCB2(MediaRecIPLGetReadyCB IPLCb);

/**
    Minus filesize if some files written outside MediaREC.

    @param[in] size filesize, Must be cluster-alignment!!
    @return void
*/
extern void SMediaRec_MinusFilesize(UINT32 size);

extern void SMediaRec_StopPreRecordStartWriting(void);

/**
    Get H264 sps/pps for each video path.

    @param[in] fileid video path, 0 or 1
    @param[in] pAddr address to put sps/pps
    @param[in] pSize size of sps/pps
    @return void
*/
extern void     SMediaRec_VidGetDesc(UINT32 fileid, UINT32 *pAddr, UINT32 *pSize);

/**
    Get minimum memory size for liveview mode.

    @param[in] void
    @return memory size
*/
extern UINT32 SMediaRec_GetLiveViewMinMemSize(void);

/**
    Update setting to calculate disk2second.

    @param[in] void
    @return void
*/
extern void SMediaRec_UpdateSetting2CaluSec(void);

/**
    Set Customized Data. (Put end of file with specific tag)

    @param[in] tag abcd=0x61626364
    @param[in] addr data address
    @param[in] size data size
    @return void
*/
extern void SMediaRec_SetCustomData(UINT32 tag, UINT32 addr, UINT32 size);

/**
    Set read-only percentage.

    @param[in] percent min=30, max=50
    @return void
*/
extern void SMediaRec_SetROPercent(UINT32 percent);

/**
    Enable end-char for single recording.

    @param[in] value on/off
    @return void
*/
extern void SMediaRec_EnableEndChar4SingleRec(UINT8 value);

/**
    Enable end-char for multiple recording.

    @param[in] value on/off
    @return void
*/
extern void SMediaRec_EnableEndChar4MultiRec(UINT8 value);

/**
    Get lastest audio bitstream.

    @param[in] frameBuf output audio bs addr/size
    @return void
*/
extern void SMediaRec_GetLastA_BS(MEDIAREC_MEM_RANGE* frameBuf);

/**
    Get lastest movie filepath.

    @param[in] pPath output filepath, length=TEST_PATHLENGTH (80)
    @return void
*/
extern void SMediaRec_GetLastFilePath(char *pPath);

/**
    Enable Check and delete read-only file when seamless recoring.

    @param[in] value on/off, default: on
    @return void
*/
extern void SMediaRec_EnableCheckReadonly(UINT8 value);
extern void SMediaRec_VidReady2Get(UINT32 fileid);//2014/01/09

/**
    Input settings to calculate recording seconds.

    @param[in] pSetting input recording setting
    @return recording seconds
*/
extern UINT32 SMediaRec_CalcSecBySetting(MEDIAREC_CALCSEC_SETTING *pSetting);

/**
    Set max customdata size. Card capacity will minus this size when start recording.

    @param[in] fileID
    @param[in] maxSize
    @return void
*/
extern void SMediaRec_SetCustomDataMaxSize(UINT32 fileid, UINT32 maxSize);

/**
    Set frameinfo size. Default: 4MB.

    @param[in] size
    @return void
*/
extern void SMediaRec_SetFrameInfoSize(UINT32 size);

/**
    Set this and last file as read-only.

    @param[in] void
    @return
        - @b MOVREC_SETCRASH_OK:            this and last both read-only
        - @b MOVREC_SETCRASH_NOLASTFILE:    this will be read-only (later), last not exist
        - @b MOVREC_SETCRASH_LASTFILERO:    this will be read-only (later), last has been RO already
*/
extern UINT32 SMediaRec_SetReadonly_ThisandLastfile(void);

/**
    Set end char for each path. 'A' and 'B' as default value.

    @param[in] endchar A~Z, a~z
    @param[in] fileid  0 as path1, 1 as path2...
    @return
        - @b E_OK:  OK
        - @b E_SYS: repeat as others
*/
extern ER SMediaRec_SetEndChar(char endchar, UINT32 fileid);

/**
    Delete files (including RO). It SHOULD NOT be executed when recording.

    @param[in] roDelete delete read-only files or not
    @param[in] roPercent  read-only max percent (30~50)
    @return void
*/
extern void SMediaRec_Clean4Recording(UINT32 roDelete, UINT32 roPercent);

/**
    Set deleting read-only files or not.

    @param[in] value 0: No delete RO files, 1: as default
    @return void
*/
extern void SMediaRec_SetDeleteReadonly(UINT32 value);

/**
    Get read-only files percent. (including freespace)

    @param[in] void
    @return RO percent
*/
extern UINT32 SMediaRec_GetROPercent(void);

/**
    Check and get H.264 encoding path after some settings (such as resolution, frame rate, ...).

    @param[in] void
    @return void
*/
extern void SMediaRec_CheckEncPath(void);

/**
    Enable/disable card writing.

    @param[in] pathid   0 or 1
    @param[in] on       0: not save, 1: as default
    @return void
*/
extern void SMediaRec_SetWriteCard(UINT32 pathid, UINT32 on);

/**
    Enable/disable EmergencyRecording for path0/1
    (If EMR is enable, it needs more memory!!)

    @param[in] pathid   0 or 1(if path1 is enable)
    @param[in] on       0: disable 1: enable
    @return void
*/
extern void SMediaRec_SetEmergencyRecording(UINT32 pathid, UINT32 on);

/**
    Stop EmergencyRecording for path0/1
    (If EMR is disable, it doesn't work!)

    @param[in] pathid   0 or 1(if path1 is enable)
    @return void
*/
extern void SMediaRec_StopFlash(UINT32 pathid);

/**
    Set H.264 Max Qp level.

    @param[in] level H264_MAX_QP_LEVEL_0 as default
    @return void
*/
extern void SMediaRec_SetH264MaxQp(UINT32 level);

/**
    Set H.264 GOP number.

    @param[in] gopnum 15 as default. Max: 120
    @return void
*/
extern void SMediaRec_SetH264GopNum(UINT32 gopnum);

/**
    Get SMedia recording parameters.

    @param[in] type MOVREC_GETRECPARAM_TARGETBR or others

    @return ER
        - @b E_OK:  OK
        - @b E_SYS: params error
*/
extern ER SMediaRec_GetParam(UINT32 type, UINT32 *pParam1, UINT32 *pParam2, UINT32 *pParam3);

extern void SMediaRec_SetTimelapsePlayVideoFrameRate(UINT32 value);

extern void SMediaRec_SetExtendCrashUnit(MOVREC_EXTENDCRASH_UNIT value);

extern void SMediaRec_SetValidVidBS(SMEDIAREC_BS_FROMSENSOR *pBS);
extern void SMediaRec_AllDirect_EncodeH264(void);
extern UINT32 SMediaRecVidTrig_GetOutputAddr(void);
extern void SMediaRec_VtrigOK(void);
extern void SMediaRec_IPLOK_GoNext(void);
extern VIDENC_PATH SMediaRec_GetEncPath(UINT32 uiVidEncId);
extern void SMediaRec_Vtrig_NoKickH264(UINT32 value);
#endif
#endif//_VIDSTRMAPI_H

//@}
