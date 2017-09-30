#ifndef _UCTRLAPP_MOVIE_H
#define _UCTRLAPP_MOVIE_H
#include "Type.h"
#include "dis_lib.h"
#include "BC_lib.h"

#define NVT_FALSE   0
#define NVT_TRUE    1
#define NVT_MD_WND_X 16
#define NVT_MD_WND_Y 12

typedef enum {
    CODEC_VENC_INPUT_SOURCE_SENSOR1,
    CODEC_VENC_INPUT_SOURCE_SENSOR2,
    CODEC_VENC_INPUT_SOURCE_SENSOR3,
    CODEC_VENC_INPUT_SOURCE_SENSOR4,
    CODEC_VENC_INPUT_SOURCE_UVC,
    CODEC_VENC_INPUT_SOURCE_MAX,
} CodecVencInputSource;

typedef enum {
    CODEC_VENC_FORMAT_MJPEG,
    CODEC_VENC_FORMAT_H264,
    CODEC_VENC_FORMAT_MAX,
} CodecVencFormat;

typedef enum {
    CODEC_VENC_PROFILE_BASELINE,
    CODEC_VENC_PROFILE_MAIN,
    CODEC_VENC_PROFILE_HIGH,
    CODEC_VENC_PROFILE_MAX,
} CodecVencProfile;

typedef enum {
    CODEC_VENC_BIT_RATE_CTRL_CBR,
    CODEC_VENC_BIT_RATE_CTRL_VBR,
    CODEC_VENC_BIT_RATE_CTRL_MAX,
} CodecVencBitRateMode;

typedef struct {
    UINT32 uiGOP;
    CodecVencBitRateMode eBitrateMode;
    BOOL bDropFrameEnable;   	/* drop frame if bitrate exceed */
    UINT32 uiTargetBitrate;  	/* CBR item start */
    FLOAT fStatisticsTime;
    UINT32 uiQPDelta;
    UINT32 uiQPFrameRate;
    UINT32 uiIPWeight;			/* CBR item end */
    UINT32 uiBitrateTolerance;	/* CBR and VBR shared item start */
	UINT32 uiMaxQP;				/* CBR and VBR shared item end */
    UINT32 uiMinQP;				/* VBR item start */
    UINT32 uiMaxBitrate;
    UINT32 uiQPGOPDelta;		/* VBR item end */
} CodecVencBitrateCtrl;

typedef struct {
    BOOL bROIEnable;
    UINT32 uiX;
    UINT32 uiY;
    UINT32 uiWidth;
    UINT32 uiHeight;
    UINT32 uiLevel;
} CodecVencROISetting;

typedef struct {
    BOOL bCropEnable;
    UINT32 uiCoordinateX;
    UINT32 uiCoordinateY;
} CodecVencCrop;

typedef struct {
    BOOL bStart;
	UINT32 uiMaxWidth;
	UINT32 uiMaxHeight;
} CodecVencStatus;

typedef enum {
    CODEC_VENC_SVCT_DISABLE,
    CODEC_VENC_SVCT_LAYER_2,
    CODEC_VENC_SVCT_LAYER_3,
    CODEC_VENC_SVCT_MAX,
} CodecVencSVCT;

typedef struct {
    CodecVencSVCT eSVCTemporalLayer;
} CodecVencSVCSetting;

typedef struct {
    CodecVencInputSource eVencInputSource;
    CodecVencFormat  eVencFormat;
    CodecVencProfile eVencProfile;
    UINT32 uiTargetWidth;
    UINT32 uiTargetHeight;
    UINT32 uiTargetFrameRate;
    CodecVencBitrateCtrl sVencBitRateCtrl;
    CodecVencCrop sVencCrop;
    CodecVencSVCSetting sSVCSetting;
    UINT32 uiBufNum;
} CodecVencSetting;

typedef enum {
    CODEC_AIN_SOURCE_LINEIN,
    CODEC_AIN_SOURCE_MICIN,
    CODEC_AIN_SOURCE_MAX,
} CodecAinSource;

typedef enum {
    CODEC_AIO_SAMPLE_RATE_8000=8000,
    CODEC_AIO_SAMPLE_RATE_11025=11025,
    CODEC_AIO_SAMPLE_RATE_12000=12000,
    CODEC_AIO_SAMPLE_RATE_16000=16000,
    CODEC_AIO_SAMPLE_RATE_22050=22050,
    CODEC_AIO_SAMPLE_RATE_24000=24000,
    CODEC_AIO_SAMPLE_RATE_32000=32000,
    CODEC_AIO_SAMPLE_RATE_44100=41000,
    CODEC_AIO_SAMPLE_RATE_48000=48000,
    CODEC_AIO_SAMPLE_RATE_MAX,
} CodecAioSampleRate;

typedef enum {
    CODEC_AIO_BIT_WIDTH_8BIT,
    CODEC_AIO_BIT_WIDTH_16BIT,
    CODEC_AIO_BIT_WIDTH_32BIT,
    CODEC_AIO_BIT_WIDTH_MAX,
} CodecAioBitWidth;

typedef enum {
    CODEC_AIO_DUPLEX_MODE_HALF,
    CODEC_AIO_DUPLEX_MODE_FULL,
    CODEC_AIO_DUPLEX_MODE_MAX,
} CodecAioDuplexMode;

typedef enum {
    CODEC_AIO_SOUND_MODE_MONO,
    CODEC_AIO_SOUND_MODE_STEREO,
    CODEC_AIO_SOUND_MODE_LEFT,
    CODEC_AIO_SOUND_MODE_RIGHT,
    CODEC_AIO_SOUND_MODE_MAX,
} CodecAioSoundMode;

typedef enum {
    CODEC_AENC_FORMAT_PCM,
    CODEC_AENC_FORMAT_AAC,
    CODEC_AENC_FORMAT_ULAW,
    CODEC_AENC_FORMAT_ALAW,
    CODEC_AENC_FORMAT_MAX,
} CodecAencFormat;

typedef struct {
    CodecAinSource eAinSource;
    UINT32 uiAinVolume;
    CodecAioSampleRate eAinSampleRate;
    CodecAioBitWidth eAinBitWidth;
    CodecAioDuplexMode eAinDuplexMode;
    CodecAioSoundMode eAinSoundMode;
    CodecAencFormat eAinFormat;
    UINT32 uiAinBitRate;
} CodecAinSetting;

typedef enum {
    CODEC_VOUT_INTERFACE_CVBS_NTSC,
    CODEC_VOUT_INTERFACE_CVBS_PAL,
	CODEC_VOUT_INTERFACE_HDMI_720x480P60,
    CODEC_VOUT_INTERFACE_HDMI_1920x1080I60,
    CODEC_VOUT_INTERFACE_HDMI_AUTO,
    CODEC_VOUT_INTERFACE_MAX,
} CodecVoutInterface;

typedef struct {
    BOOL bVoutEnable;
    CodecVoutInterface eVoutInterface;
} CodecVoutSetting;

typedef struct {
    UINT16 uiCoordinateX;
    UINT16 uiCoordinateY;
    UINT16 uiWidth;
    UINT16 uiHeight;
} CodecIAObjData;

typedef struct {
    UINT16  stx;               ///< result start x
    UINT16  sty;               ///< result start y
    UINT16  width;             ///< result width
    UINT16  height;            ///< result height
    UINT32  id;                ///< result id
    UINT16  type;              ///< result type: 1--first appearance, other--not first.
    UINT16  clarity;           ///< result clarity
    UINT16  luma;
    INT32  angle_horizontal;  ///< result face horizontal angle
    INT32  angle_vertical;    ///< result face vertical angle
    INT32  angle_roll;        ///< result face roll angle
    INT32  score;             ///< result face quality reference score
} CodecFTGObjData;


typedef struct {
    UINT32 score;
	char overrall_pic[FTG_OVERRALL_MAX_BUF_SIZE];
	UINT32 overrall_size;
	char face_pic[FTG_FACE_MAX_BUF_SIZE];
	UINT32 face_size;

}PIC_DATA;
typedef struct {

    UINT32 id;
    UINT32 pic_num;
	PIC_DATA pic_list[FTG_MAX_FACE_PIC_NUM];
}FTG_SNAPSHOT;

typedef struct _UCtrlMovieCropPosParmType
{
    UINT32 isSet;
    BOOL   bCropEnable;
    UINT32 uiCoordinateX;
    UINT32 uiCoordinateY;
} UCtrlMovieCropPosParmType;

typedef struct _UCtrlMovieBitrateParmType
{
    UINT32 isSet;
    CodecVencBitRateMode eBitrateMode;
    UINT32 uiTargetBitrate;  /* CBR item start */
    FLOAT fStatisticsTime;
    UINT32 uiQPDelta;
    UINT32 uiQPFrameRate;
    UINT32 uiIPWeight;			/* CBR item end */
    UINT32 uiBitrateTolerance;	/* CBR and VBR shared item start */
	UINT32 uiMaxQP;				/* CBR and VBR shared item end */
    UINT32 uiMinQP;				/* VBR item start */
    UINT32 uiMaxBitrate;
    UINT32 uiQPGOPDelta;		/* VBR item end */
} UCtrlMovieBitrateParmType;

typedef enum {
    CODEC_AOUT_TARGET_LINEOUT,
    CODEC_AOUT_TARGET_SPEAKER,
    CODEC_AOUT_TARGET_MAX,
} CodecAoutTarget;

typedef struct {
    CodecAoutTarget eAoutTarget;
    CodecAioSampleRate eAoutSampleRate;
    UINT32 uiAoutVolume;
    CodecAioBitWidth eAoutBitWidth;
    CodecAioSoundMode eAoutSoundMode;
    CodecAencFormat eAoutFormat;
    BOOL bAoutAECEn;
} CodecAoutSetting;

/* HDR */
typedef enum {
    ISP_CTRL_MODE_AUTO,
    ISP_CTRL_MODE_MANUAL,
    ISP_CTRL_MODE_MAX,
} ISPCtrlMode;

typedef struct {
    BOOL bHDREnable;
    ISPCtrlMode eHDRMode;
    UINT32 uiHDRStrength; // only for manual
} ISPHDRSetting;

// ---------------- IVS related define  ---------------------------------

typedef struct {
    BOOL bGlobalMotionEnable;
    BOOL bROIMotionEnable;
    UINT8 uiROIStartX;
    UINT8 uiROIStartY;
    UINT8 uiROISizeX;
    UINT8 uiROISizeY;
    UINT32 uiInputSizeH;
    UINT32 uiInputSizeV;
} IVSMVSetting;

typedef struct {
    INT32 iX;
    INT32 iY;
    BOOL bValidVector;
} IVSMVROIData;

typedef struct {
    INT32   iX;         ///< x component
    INT32   iY;         ///< y component
    UINT32  uiSad;      ///< sum of absolute difference
    UINT32  uiCnt;      ///< number of edge pixels (src block)
    UINT32  uiIdx;      ///< block index
    BOOL    bValid;     ///< reliability of MV
} IVSMVInfor;

typedef struct {
    IVSMVInfor sIVSMVROIResult[MVNUMMAX];
    UINT32     uiNumH;
    UINT32     uiNumV;
} IVSMVData;

typedef struct {
    UINT32 uiMinObjectAreaPCT;
    UINT32 uiMaxFGFrame;
    BOOL   bNHUpdateEnable;
    UINT32 uiBGUpdateSpeed;
    UINT32 uiBLKNumW;
    UINT32 uiBLKNumH;
    UINT32 uiMinObjectBLKNum;
    UINT32 uiFGPerBlkPCT;
    UINT32 uiSensitivy;
} IVSBCSetting;

typedef struct {
    UINT32 uiCoordinateX;
    UINT32 uiCoordinateY;
    UINT32 uiWidth;
    UINT32 uiHeight;
} IVSObjData;

typedef struct {
    BOOL       bUpdated;
    UINT32     uiObjectNum;
    IVSObjData sObjectData[BC_MAX_OBJ_NUM];
} IVSBCData;

typedef struct {
    BOOL       bUpdated;
    UINT32     uiObjectNum;
    IVSObjData sObjectData[BC_MAX_BLK_NUM];
} IVSBCBLKData;

typedef enum {
    IVS_PIXEL_FMT_YUV422_PLANAR  = 0x00,    ///< 3 color plane, pixel=Y(w,h), U(w/2,h), and V(w/2,h)
    IVS_PIXEL_FMT_YUV420_PLANAR  = 0x01,    ///< 3 color plane, pixel=Y(w,h), U(w/2,h/2), and V(w/2,h/2)
    IVS_PIXEL_FMT_YUV422_PACKED  = 0x02,    ///< 2 color plane, pixel=Y(w,h), UV(w,h)
    IVS_PIXEL_FMT_YUV420_PACKED  = 0x03,    ///< 2 color plane, pixel=Y(w,h), UV(w,h/2)
    IVS_PIXEL_FMT_Y_ONLY         = 0x04,    ///< 1 color plane, pixel=Y(w,h),
    IVS_PIXEL_FMT_ARGB565        = 0x05,    ///< 2 color plane, pixel=A8(w,h),RGB565(2w,h)
    IVS_PIXEL_FMT_RGB888_PLANAR  = 0x06,    ///< 3 color plane, pixel=R(w,h), G(w,h), B(w,h)
    IVS_PIXEL_FMT_ARGB888_PACKED = 0x07,    ///< 1 color plane, pixel=ARGB(4w,h)
    IVS_PIXEL_FMT_MAX,
} IVSPixelFormat;

typedef struct {
    UINT32              uiWidth;                     ///< image width
    UINT32              uiHeight;                    ///< image height
    IVSPixelFormat      ePxlFmt;                     ///< pixel format
    UINT32              uiPxlAddr[MAX_PLANE_NUM];    ///< pixel address
    UINT32              uiLineOffset[MAX_PLANE_NUM]; ///< pixel lineoffset
} IVSImageBuf;

typedef struct {
    UINT32              uiAddr;                    ///< buffer address
    UINT32              uiSize;                    ///< buffer size
} IVSBuf;




extern void       UCtrlAppMovie_Open(void);
extern void       UCtrlAppMovie_Close(void);
#endif //_UCTRLAPP_MOVIE_H
