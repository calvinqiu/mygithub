#ifndef _MOVIESTAMP_H
#define _MOVIESTAMP_H

#include "draw_lib.h"
#include "IPL_Ctrl.h"

// bit [0]
#define STAMP_ON                0x00000000
#define STAMP_OFF               0x00000001
#define STAMP_SWITCH_MASK       0x00000001

// bit [1]
#define STAMP_AUTO              0x00000000
#define STAMP_MANUAL            0x00000002
#define STAMP_OPERATION_MASK    0x00000002

// bit [3:2]
#define STAMP_DATE_TIME         0x00000000
#define STAMP_DATE_TIME_AMPM    0x00000004
#define STAMP_DATE              0x00000008
#define STAMP_TIME              0x0000000C
#define STAMP_DATE_TIME_MASK    0x0000000C

// bit [5:4]
#define STAMP_BOTTOM_LEFT       0x00000000
#define STAMP_BOTTOM_RIGHT      0x00000010
#define STAMP_TOP_LEFT          0x00000020
#define STAMP_TOP_RIGHT         0x00000030
#define STAMP_POSITION_MASK     0x00000030

// bit [7:6]
#define STAMP_YY_MM_DD          0x00000000
#define STAMP_DD_MM_YY          0x00000040
#define STAMP_MM_DD_YY          0x00000080
#define STAMP_DATE_FORMAT_MASK  0x000000C0

// bit [8]
#define STAMP_BG_TRANSPARENT    0x00000000  // default
#define STAMP_BG_FILL_COLOR     0x00000100
#define STAMP_BG_FORMAT_MASK    0x00000100

// bit [9]
#define STAMP_POS_NORMAL        0x00000000  // default
#define STAMP_POS_END           0x00000200
#define STAMP_POS_END_MASK      0x00000200

// bit [11:10]
#define STAMP_IMG_420UV         0x00000000  // default
#define STAMP_IMG_422UV         0x00000400
#define STAMP_IMG_422           0x00000800
#define STAMP_IMG_FORMAT_MASK   0x00000C00

#define STAMP_FORMAT_YUV        0   // stamp data is YUV format
#define STAMP_FORMAT_IDX        1   // stamp data is color index format

#define WATERLOGO_AUTO_POS      0xFFFFFFFF  // auto position for water logo
#define WATERLOGO_FILL_COLOR    0xFFFFFFFF  // fill all data for water logo
#if defined (WATERLOGO_FUNCTION) && (WATERLOGO_FUNCTION == ENABLE)
#define WATERLOGO_BUF_SIZE      ALIGN_CEIL_8(256*128*2) // max 256x128, YUV422                  // 64K
#else
#define WATERLOGO_BUF_SIZE      0
#endif
#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
#define MULTILINGUAL_BUF_SIZE           ALIGN_CEIL_8(1920*32*2) // max 1920x32, YUV422           // 120K
#define MULTILINGUAL_SCALLED_BUF_SIZE   ALIGN_CEIL_8(1920*44*2) // max 1920x44, YUV422           // 165K
#else
#define MULTILINGUAL_BUF_SIZE           0
#define MULTILINGUAL_SCALLED_BUF_SIZE   0
#endif

#define cst_prom0   21
#define cst_prom1   79
#define cst_prom2   29
#define cst_prom3   43
#define RGB_GET_Y(r,g,b)    (((INT32)g) + ((cst_prom1 * (((INT32)r)-((INT32)g))) >> 8) + ((cst_prom2 * (((INT32)b)-((INT32)g))) >> 8) )
#define RGB_GET_U(r,g,b)    (128 + ((cst_prom3 * (((INT32)g)-((INT32)r))) >> 8) + ((((INT32)b)-((INT32)g)) >> 1) )
#define RGB_GET_V(r,g,b)    (128 + ((cst_prom0 * (((INT32)g)-((INT32)b))) >> 8) + ((((INT32)r)-((INT32)g)) >> 1) )

#define icst_prom0  103
#define icst_prom1  88
#define icst_prom2  183
#define icst_prom3  198
#define YUV_GET_R(y,u,v)    ( ((INT32)y) + ((INT32)(v)-128) + ((icst_prom0*((INT32)(v)-128))>>8) )
#define YUV_GET_G(y,u,v)    ( ((INT32)y) - ((icst_prom1*((INT32)(u)-128))>>8) - ((icst_prom2*((INT32)(v)-128))>>8) )
#define YUV_GET_B(y,u,v)    ( ((INT32)y) + ((INT32)(u)-128) + ((icst_prom3*((INT32)(u)-128))>>8) )

typedef struct
{
    UINT32    uiX;              ///< the X offset
    UINT32    uiY;              ///< the Y offset
} STAMP_POS;

typedef struct
{
    UINT8     ucY;              ///< the Y value
    UINT8     ucU;              ///< the U value
    UINT8     ucV;              ///< the V value
    UINT8     rev;              ///< reserved
} STAMP_COLOR, *PSTAMP_COLOR;

typedef struct
{
    char    *pi8Str;            ///< The string of date.
    ICON_DB const *pDataBase;   ///< The font database for date.
    UINT32  ui32FontWidth;      ///< The max width of font
    UINT32  ui32FontHeight;     ///< The max height of font
    UINT32  ui32DstHeight;      ///< The destination height of font
    STAMP_COLOR Color[3];       ///< The stamp color (0: background, 1: frame, 2: foreground)
} STAMP_INFO, *PSTAMP_INFO;

typedef struct
{
    UINT32  uiYAddr;            ///< Stamp buffer Y address
    UINT32  uiUAddr;            ///< Stamp buffer U address
    UINT32  uiVAddr;            ///< Stamp buffer V address
    UINT32  uiYLineOffset;      ///< Stamp buffer Y line offset
    UINT32  uiUVLineOffset;     ///< Stamp buffer UV line offset
    UINT32  uiFormat;           ///< Stamp YUV format
} STAMP_BUFFER, *PSTAMP_BUFFER;

typedef struct
{
    UINT32  uiXPos;                     ///< WaterLogo x position
    UINT32  uiYPos;                     ///< WaterLogo y position
    UINT32  uiWidth;                    ///< WaterLogo width
    UINT32  uiHeight;                   ///< WaterLogo height
    UINT32  uiWaterLogoYAddr;           ///< WaterLogo buffer Y address
    UINT32  uiWaterLogoUVAddr;          ///< WaterLogo buffer U address
    UINT32  uiWaterLogoYLineOffset;     ///< WaterLogo buffer Y line offset
    UINT32  uiWaterLogoFormat;          ///< WaterLogo YUV format
    UINT32  uiWaterLogoColorKey;        ///< WaterLogo ColorKey
} WATERLOGO_BUFFER, *PWATERLOGO_BUFFER;

extern const WATERLOGO_BUFFER g_WaterLogo;

//#NT#2016/04/22#Nestor Yang -begin
//#NT# Support Driver Drowsiness Detection(DDD)
#if (MOVIE_DDD_FUNC && MOVIE_DDD_DRAW_VIDEO)
extern UINT32 g_DDD_DowsyState;
#endif
//#NT#2016/04/22#Nestor Yang -end

// Date stamp for movie
extern void     MovieStamp_Enable(void);
extern void     MovieStamp_Disable(void);
extern void     MovieStamp_Setup(UINT32 uiVidEncId, UINT32 uiFlag, UINT32 uiImageWidth, UINT32 uiImageHeight, WATERLOGO_BUFFER *pWaterLogoBuf);
extern void     MovieStamp_SetPos(UINT32 uiVidEncId, UINT32 x, UINT32 y);
extern void     MovieStamp_SetColor(UINT32 uiVidEncId, PSTAMP_COLOR pStampColorBg, PSTAMP_COLOR pStampColorFr, PSTAMP_COLOR pStampColorFg);
//extern UINT32   MovieStamp_GetMaxFontHeight(void);
extern void     MovieStamp_UpdateData(void);
extern void     MovieStamp_CopyData(UINT32 uiVidEncId, UINT32 yAddr, UINT32 cbAddr, UINT32 crAddr, UINT32 lineY, UINT32 imgWidth);
extern void     MovieStamp_SetImeStamp(UINT32 uiVidEncId, IMESTAMP_INFO *pImeStamp);

#if defined (MULTILINGUAL_FUNCTION) && (MULTILINGUAL_FUNCTION == ENABLE)
extern void     MultiLingualString_UpdateData(void);
#endif

#define MOVIE_AE_LOG    0

#define MOVIE_ADAS_LOG  0

//#NT#2016/07/22#YongChang Qui -begin
//#NT#MovieStamp uses unified memory as IPC & MovieStamp buffer for IPCam with AE_LOG off
#if (IPCAM_FUNC && !MOVIE_AE_LOG)
extern void     MovieStamp_SetBuffer(UINT32 uiAddr, UINT32 uiSize);
#else
extern void     MovieStamp_SetBuffer(UINT32 uiVidEncId, UINT32 uiAddr, UINT32 uiSize);
#endif
//#NT#2016/07/22#YongChang Qui -end

//#NT#2016/12/07#Charlie Chang  -begin
//#NT# for config set/get
typedef enum _MOVIESTAMP_CONFIG{
    MOVIESTAMP_FTG_DRAW_DELAY_COUNT = 0,
	MOVIESTAMP_FTG_RLST_TYPE,
	MOVIESTAMP_PIP_ENABLE,
	MOVIESTAMP_BLENDING,
    MOVIESTAMP_MAX_CONFIG_CNT,
    ENUM_DUMMY4WORD(MOVIESTAMP_CONFIG)

}MOVIESTAMP_CONFIG;
extern UINT32 MovieStamp_GetConfig(UINT32 type);
extern void MovieStamp_SetConfig(UINT32 type, UINT32 value);
//#NT#2016/12/07#Charlie Chang -end
//#NT#2017/02/16#Niven Cho -begin
//#NT# support blending
typedef struct{

    UINT32 uiSrcVideoID;
    UINT32 uiDesVideoID;
    BOOL bEnable;
	UINT32 uiAlpha;

}MOVIESTAMP_BLENDING_CFG;


//#NT#2017/02/16#Niven Cho -end

#endif
