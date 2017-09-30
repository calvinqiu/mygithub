/**
    Public header file for IPL command

    This file is the header file that define the API and data type for IPL command.

    @file       IPL_Cmd.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IPL_CMD_H_
#define _IPL_CMD_H_
#include "Type.h"
#include "ErrorNo.h"
#include "IPL_Utility.h"
#include "IPL_CBMsg.h"
#include "IPL_ISREvent.h"
#include "IPL_Hal_SIE_Com_Info.h"


/***********************************************************/
/*                  IPL set command                        */
/***********************************************************/

/** \addtogroup mIIPL */
//@{

/**
    IPL SET Command(IPL_SetCmd())
*/
typedef enum _IPL_SET_CMD
{
    IPL_SET_CHGMODE = 0,       ///< change mode command, data type IPL_CHGMODE_DATA
    IPL_SET_IME,               ///< change IME settings command, data type IPL_IME_DATA
    IPL_SET_DZOOM,             ///< change digital idx command, data type IPL_DZOOM
    IPL_SET_DZOOM_WAIT_END,    ///< wait digital zoom finish, data type IPL_PROC_ID
    IPL_SET_SLEEP,             ///< enter sleep mode command, data type IPL_SLEEP_INFO
    IPL_SET_WAKEUP,            ///< exit sleep mode command, data type IPL_WAKEUP_INFO
    IPL_SET_GET_CAP_RAW,       ///< get cap raw command, data type IPL_GET_CAP_RAW_DATA
    IPL_SET_PAUSEDMA,          ///< pause all IPL dma, for system before change IC frequency, data type IPL_PROC_ID
    IPL_SET_RESUMEDMA,         ///< resume all IPL dma, for system after change IC frequency, data type IPL_PROC_ID
    IPL_SET_IMGRATIO,          ///< set SIE crop ratio & ime out size, data type IPL_SET_IMGRATIO_DATA
    IPL_SET_VA,                ///< set VA settings command, data type IPL_SET_VA_DATA
    IPL_SET_FLIP,              ///< set H/V flip command, data type IPL_SET_FLIP_DATA
    IPL_SET_MAX_CNT_DEF,
    ENUM_DUMMY4WORD(IPL_SET_CMD)
} IPL_SET_CMD;

#define IPL_SET_SWITCH_ALL  0xffffffff
#define IPL_SET_CMD_SWITCH_TAB  32      ///< maxinum value of IPL SET command

//check IPL_SET_CMD range
STATIC_ASSERT((IPL_SET_MAX_CNT_DEF <= IPL_SET_CMD_SWITCH_TAB));

/**
    IPL STATUS
*/
typedef enum _IPL_STATUS
{
    IPL_STATUS_OFF = 0,
    IPL_STATUS_NORMAL,
    IPL_STATUS_PAUSEDMA,
    IPL_STATUS_SLEEP,
    ENUM_DUMMY4WORD(IPL_STATUS)
} IPL_STATUS;


/**
    IPL ACT
*/
typedef enum _IPL_ACT
{
    IPL_NONE = 0,
    IPL_STOP,
    IPL_PAUSE,
    IPL_START,
    IPL_RESUME,
    IPL_OFF,
    ENUM_DUMMY4WORD(IPL_ACT)
} IPL_ACT;

/********* IPL_CMD_CHGMODE *********/
/**
    IPL MODE
*/
typedef enum _IPL_MODE
{
    IPL_MODE_OFF = 0,       ///< IPL off mode.
    IPL_MODE_INIT,          ///< IPL init mode, prepare buffer for all sensor in this stage.
    IPL_MODE_PREVIEW,       ///< IPL photo mode.
    IPL_MODE_VIDEO,         ///< IPL video mode.
    IPL_MODE_VIDEOREC,      ///< IPL video recording mode.
    IPL_MODE_CAP,           ///< IPL capture mode.
    IPL_MODE_NUM,
    IPL_MODE_NONE = 0xffffffff,
    ENUM_DUMMY4WORD(IPL_MODE)
} IPL_MODE;

/**
    IPL post process function, IPL_SET_CHGMODE command
*/
typedef enum _IPL_POST_PROC_FUNC_EN
{
    IPL_FUNC_NONE           = 0x00000000,       ///< disable post process function.
    IPL_FUNC_DISP           = 0x00000001,       ///< enable Display control function.
    IPL_FUNC_VA             = 0x00000002,       ///< allocate va output buffer.
    IPL_FUNC_DIS            = 0x00000004,       ///< prepare DIS function & working buffer.
    IPL_FUNC_WDR            = 0x00000008,       ///< prepare WDR function.
    IPL_FUNC_SHDR           = 0x00000010,       ///< prepare Sensor HDR function.
    IPL_FUNC_ADAS           = 0x00000020,       ///< prepare Lane Departure Warning System/Foward Collision Warning System working buffer
    IPL_FUNC_STITCH         = 0x00000040,       ///< prepare Image Stich buffer.
    IPL_FUNC_VR360          = 0x00000080,       ///< prepare vr360 buffer.
    ENUM_DUMMY4WORD(IPL_POST_PROC_FUNC_EN)
} IPL_POST_PROC_FUNC_EN;

/**
    IPL SIE crop ratio

    IPL_SET_CHGMODE command(only support stream mode, ex: preview, video, video record mode)
*/
typedef enum _IPL_SENCROPRATIO
{
    IPL_SENCROPRATIO_9_16 = 0,                  ///< 9:16
    IPL_SENCROPRATIO_2_3,                       ///< 2:3
    IPL_SENCROPRATIO_3_4,                       ///< 3:4
    IPL_SENCROPRATIO_1_1,                       ///< 1:1
    IPL_SENCROPRATIO_4_3,                       ///< 4:3
    IPL_SENCROPRATIO_3_2,                       ///< 3:2
    IPL_SENCROPRATIO_16_9,                      ///< 16:9
    IPL_SENCROPRATIO_MAX_CNT,
    IPL_SENCROPRATIO_NONE,
    ENUM_DUMMY4WORD(IPL_SENCROPRATIO)
} IPL_SENCROPRATIO;


/**
    IPL ctrl flow type
    For Project Layet reference to set ime d2d or ime-direct
*/
typedef enum _IPL_CTRL_FLOW_TYPE
{
    IPL_CTRLFLOW_NORMAL = 0,
    IPL_CTRLFLOW_2STRIPE,
    ENUM_DUMMY4WORD(IPL_CTRL_FLOW_TYPE)
} IPL_CTRL_FLOW_TYPE;

/**
    IPL flip information
*/
typedef enum _FLIP_TYPE
{
    FLIP_NONE = 0x00000000,         ///< no flip
    FLIP_H    = 0x00000001,         ///< H flip
    FLIP_V    = 0x00000002,         ///< V flip
    FLIP_H_V  = 0x00000003,         ///<H & V flip(Flip_H | Flip_V) don't modify
    ENUM_DUMMY4WORD(FLIP_TYPE)
} FLIP_TYPE;

/**
    IPL IME INIT parameter
    For Chg to IPL_MODE_INIT.

*/
typedef struct _IPL_IME_INIT_DATA
{
    UINT32 BufSizeH;             ///< maximum buffer H size, format reference ImgFmt
    UINT32 BufSizeV;             ///< maximum buffer V size, format reference ImgFmt
    IPL_YUV_IMG_TYPE ImgFmt;     ///< ime output format
} IPL_IME_INIT_DATA;

/**
    IPL IME ChgMode parameter
    For IPL MODE_PREVIEW/IPL_MODE_VIDEO/IPL_MODE_VIDEOREC/IPL_MODE_OFF.

*/
typedef struct _IPL_IME_MODE_DATA
{
    UINT32 ImgEn;                ///< ime out path enable
    BOOL   Direct2H264;          ///< Direct Output to H264(Only IME_Path_1 will reference this parameter)
    UINT32 ImgSizeH;             ///< ime H size, ImgSizeH x ImgSizeV must be <= BufSizeH x BufSizeV
    UINT32 ImgSizeV;             ///< ime V size, ImgSizeH x ImgSizeV must be <= BufSizeH x BufSizeV
    UINT32 ImgSizeLOfs;          ///< output image line offset
    IPL_YUV_IMG_TYPE ImgFmt;     ///< ime format
    UINT32 CropStartX;
    UINT32 CropStartY;
    UINT32 CropSizeH;
    UINT32 CropSizeV;
} IPL_IME_MODE_DATA;

/**
    IPL IME IMG parameter

    For IPL_CMD use to Combine IPL_IME_INIT_DATA and IPL_IME_MODE_DATA

*/
typedef struct _IPL_IME_IMG_DATA
{
    UINT32 ImgEn;                ///< ime out path enable
    BOOL   Direct2H264;          ///< Direct Output to H264(Only for IME Path 1)
    UINT32 BufSizeH;             ///< reserve buffer H size, format reference BufFmt
    UINT32 BufSizeV;             ///< reserve buffer V size, format reference BufFmt
    IPL_YUV_IMG_TYPE BufFmt;     ///< ime buffer format
    UINT32 ImgSizeH;             ///< ime H size, ImgSizeH x ImgSizeV must be <= BufSizeH x BufSizeV
    UINT32 ImgSizeV;             ///< ime V size, ImgSizeH x ImgSizeV must be <= BufSizeH x BufSizeV
    UINT32 ImgSizeLOfs;          ///< output image line offset
    IPL_YUV_IMG_TYPE ImgFmt;     ///< ime format
    UINT32 CropStartX;
    UINT32 CropStartY;
    UINT32 CropSizeH;
    UINT32 CropSizeV;
} IPL_IME_IMG_DATA;

/**
    IPL VR360 parameter

    For IPL_CMD use to set vr360 parameters

*/
typedef struct _IPL_VR360_DATA
{
    UINT8  ori[2];               ///< Rotation option for [0] upper(left) image and [1] lower(right) image
    UINT32 ImgHeightAdj;         /// < Image height adjust factor in Q10; default = 1024
    UINT32 IME_P1_Start_Offset;  ///< Height offset (in pixel)
    UINT32 IME_P2_Start_Offset;  ///< Height offset (in pixel)
    UINT32 IME_P3_Start_Offset;  ///< Height offset (in pixel)
} IPL_VR360_DATA;

/**
    IPL Init parameter

    Data Structure for IPL_SET_CHGMODE chage to IPL_MODE_INIT mode operation.

*/
typedef struct IPL_INIT_DATA
{
    UINT32 ProcSenId;               ///< assign IPL Id
    IPL_MODE Mode;                  ///< next mode

    UINT32 BufAddr;                 ///< input buffer start address for all IPL_ID (only IPL_ID_1 will reference this value and using for all IPL)
    UINT32 BufSize;                 ///< input & ouput total buffer size for all IPL_ID, return used buffer size (only IPL_ID_1 will reference this value)

    IPL_IME_INIT_DATA ImePath1;     ///< ime path1  information , h264 encode path
    IPL_IME_INIT_DATA ImePath2;     ///< ime path2  information , display path
    IPL_IME_INIT_DATA ImePath3;     ///< ime path3  information , dual display path
    IPL_IME_INIT_DATA ImePath4;     ///< ime path4  information , ADAS path

    IPL_POST_PROC_FUNC_EN FuncEn;   ///< notify IPL post process enable & prepare buffer
    struct IPL_INIT_DATA *pNext;    ///< Link to next property description. (Fill NULL if this is last item)
} IPL_INIT_DATA;

/**
    IPL mode parameter

    Data structure for IPL MODE_PREVIEW/IPL_MODE_VIDEO/IPL_MODE_VIDEOREC/IPL_MODE_OFF.
*/
typedef struct IPL_MODE_DATA
{
    UINT32 ProcSenId;                               ///< assign sensor Id
    IPL_MODE Mode;                                  ///< next mode

    IPL_SENCROPRATIO CropRatio;                     ///< sie crop ratio

    IPL_IME_MODE_DATA ImePath1;                     ///< ime path1  information , h264 encode path
    IPL_IME_MODE_DATA ImePath2;                     ///< ime path2  information , display path
    IPL_IME_MODE_DATA ImePath3;                     ///< ime path3  information , dual display path
    IPL_IME_MODE_DATA ImePath4;                     ///< ime path4  information , ADAS Path

    IPL_CBMSG_FP CB_FP;                             ///< flow callback function
    IPL_H264_CB CB_H264;                            ///< h264 cb function
    IPL_ISR_EVENT_FP ISR_CB_FP[IPL_ISR_MAX_CNT];    ///< ISR CB for photo mode

    IPL_POST_PROC_FUNC_EN FuncEn;                   ///< notify IPL post process enable & prepare buffer
    FLIP_TYPE FlipInfo;                             ///< flip information
    IPL_VR360_DATA VR360Info;                       ///< VR360 parameters, need enable IPL_FUNC_VR360 function in FunEn
    struct IPL_MODE_DATA *pNext;                    ///< Link to next property description. (Fill NULL if this is last item)
} IPL_MODE_DATA;

/**
    IPL mode parameter

    Data structure for change to IPL_MODE_CAP.
*/
typedef struct IPL_MODE_DATA_CAP
{
    UINT32 ProcSenId;                               ///< assign sensor Id
    IPL_MODE Mode;                                  ///< next mode

    UINT32 BufAddr;
    UINT32 BufSize;

    IPL_SENCROPRATIO CropRatio;                     ///< sie crop ratio
    UINT32 EventId;                                 ///< notify IPL current still event.
    FLIP_TYPE FlipInfo;                             ///< flip information
    struct IPL_MODE_DATA_CAP *pNext;                ///< Link to next property description. (Fill NULL if this is last item)
} IPL_MODE_DATA_CAP;

/********* IPL_SET_SETDZOOM *********/

/**
    IPL digital zoom parameter

    Data structure for IPL_SET_SETDZOOM command operation.
*/
typedef struct _IPL_DZOOM
{
    IPL_PROC_ID Id;             ///< ipl process id
    UINT32 Idx;                 ///< zoom index
} IPL_DZOOM;

/********* IPL_SET_PAUSEDMA *********/
/**
    IPL pause dma command parameter

    Data structure for IPL_SET_PAUSEDMA command operation.
*/
typedef struct _IPL_PAUSEDMA_INFO
{
    IPL_PROC_ID Id;             ///< ipl process id
} IPL_PAUSEDMA_INFO;


/********* IPL_SET_RESUMEDMA *********/
/**
    IPL Resume dma command parameter

    Data structure for IPL_SET_RESUMEDMA command operation.
*/
typedef struct _IPL_RESUMEDMA_INFO
{
    IPL_PROC_ID Id;             ///< ipl process id
} IPL_RESUMEDMA_INFO;

/********* IPL_SET_SLEEP *********/
/**
    IPL sleep command parameter

    Data structure for IPL_SET_SLEEP command operation.
*/
typedef struct _IPL_SLEEP_INFO
{
    IPL_PROC_ID Id;             ///< ipl process id
} IPL_SLEEP_INFO;

/********* IPL_SET_WAKEUP *********/
/**
    IPL WAKEUP command parameter

    Data structure for IPL_SET_WAKEUP command operation.
*/
typedef struct _IPL_WAKEUP_INFO
{
    IPL_PROC_ID Id;             ///< ipl process id
} IPL_WAKEUP_INFO;

/********* IPL_SET_IME *********/

/**
    IPL ime path id
*/
typedef enum _IPL_IME_PATH
{
    IPL_IME_PATH_NONE = 0x00000000,     ///< path null
    IPL_IME_PATH1 = 0x00000001,         ///< path id 1
    IPL_IME_PATH2 = 0x00000002,         ///< path id 2
    IPL_IME_PATH3 = 0x00000004,         ///< path id 3
    IPL_IME_PATH4 = 0x00000008,         ///< path id 4
    ENUM_DUMMY4WORD(IPL_IME_PATH)
} IPL_IME_PATH;

/**
    IPL ime path1 Output des. Select
*/
typedef enum _IPL_IME_PATH1_OUTSEL
{
    IPL_IME_PATH1_D2D,      ///< path1 Output to dram
    IPL_IME_PATH1_DIRECT,   ///< path1 direct to H264
    ENUM_DUMMY4WORD(IPL_IME_PATH1_OUTSEL)
} IPL_IME_PATH1_OUTSEL;

/**
    IPL ime path information
*/
typedef struct _IPL_IME_PATH_INFO
{
    UINT32 OutputEn;            ///<path output enable/disable
    UINT32 DmaOutEn;            ///<dma output enable/disable, only for path1
    UINT32 ImgSizeH;            ///<output image size H
    UINT32 ImgSizeV;            ///<output image size V
    UINT32 ImgSizeLOfs;         ///<output image line offset
    IPL_YUV_IMG_TYPE ImgFmt;    ///<output image format
    UINT32 CropStartX;
    UINT32 CropStartY;
    UINT32 CropSizeH;
    UINT32 CropSizeV;
} IPL_IME_PATH_INFO;

/**
    IPL ime path information
    Data structure for IPL_SET_IME command operation.
*/
typedef struct _IPL_IME_DATA
{
    IPL_PROC_ID Id;             ///< ipl process id
    IPL_IME_PATH PathId;        ///< config path id
    IPL_IME_PATH_INFO ImeP1;    ///< ime path1 config information
    IPL_IME_PATH_INFO ImeP2;    ///< ime path2 config information
    IPL_IME_PATH_INFO ImeP3;    ///< ime path3 config information
    IPL_IME_PATH_INFO ImeP4;    ///< ime path4 config information
} IPL_IME_DATA;

/********* IPL_SET_GET_CAP_RAW *********/
/**
    IPL get raw information
    Data structure for IPL_SET_GET_CAP_RAW command
*/
typedef struct _IPL_GET_CAP_RAW_DATA
{
    IPL_PROC_ID Id;             ///< ipl process id
    UINT32 CompressRatio;       ///< raw compress ratio 0 ~ 100%, 0, 100:disable, 1~99%:Enable
    UINT32 CompressQInit;       ///< raw compress maxinum bit stream size
    UINT32 OutCh0En;            ///< sie channel 0 output enable/disable
    UINT32 BufAddrCh0;          ///< sie channel 0 output buffer address
    UINT32 OutCh3En;            ///< sie channel 2 outout enable/disable
    UINT32 BufAddrCh3;          ///< sie channel 2 output buffer address
    UINT32 IsDarkFrame;         ///< notify get raw flow
    UINT32 BufAddrCh0_1;        ///< sie channel 0 output buffer address
    UINT32 BufAddrCh3_1;        ///< sie channel 2 output buffer address
    UINT32 res[10];
} IPL_GET_CAP_RAW_DATA;

/********* IPL_SET_IMGRATIO *********/

/**
    IPL set image ratio information
    Data structure for IPL_SET_IMGRATIO command
*/
typedef struct _IPL_SET_IMGRATIO_DATA
{
    IPL_PROC_ID Id;                     ///< ipl process id
    IPL_SENCROPRATIO CropRatio;         ///< sie crop ratio
    IPL_IME_PATH PathId;                ///< select ime path
    IPL_IME_PATH_INFO ImeP1;            ///< ime path1 information, only valid when PathId & path1 == 1
    IPL_IME_PATH_INFO ImeP2;            ///< ime path2 information, only valid when PathId & path2 == 1
    IPL_IME_PATH_INFO ImeP3;            ///< ime path3 information, only valid when PathId & path3 == 1
    IPL_IME_PATH_INFO ImeP4;            ///< ime path4 information, only valid when PathId & path4 == 1
} IPL_SET_IMGRATIO_DATA;

/********* IPL_SET_VA *********/

/**
    IPL set va information
    For IPL_SET_VA command
*/
typedef struct
{
    IPL_PROC_ID Id;                 ///< ipl process id
    UINT32 VAOutEn;                 ///< va output enable/disable
    SIE_VACC VAInfo;                ///< va information
    BOOL VAGammaEn;                 ///< va gamma enable/disable
    BOOL VACgEn;                    ///< va color gain enable/disable
} IPL_SET_VA_DATA;

/********* IPL_SET_FLIP *********/

/**
    IPL set H/V flip
    for IPL_SET_FLIP command
*/
typedef struct
{
    IPL_PROC_ID Id;                     ///< ipl process id
    FLIP_TYPE FlipInfo;                 ///< flip information
} IPL_SET_FLIP_DATA;

/********* IPL_SET_CMD *********/
typedef union
{
    IPL_MODE_DATA ChgMode;
    IPL_INIT_DATA InitInfo;
    IPL_MODE_DATA_CAP CapInfo;
    IPL_IME_DATA ImeInfo;
    IPL_DZOOM DZoom;
    IPL_SLEEP_INFO SleepInfo;
    IPL_WAKEUP_INFO WakeInfo;
    IPL_GET_CAP_RAW_DATA GetRaw;
    IPL_PAUSEDMA_INFO PauseInfo;
    IPL_RESUMEDMA_INFO ResumeInfo;
    IPL_SET_IMGRATIO_DATA CropInfo;
    IPL_SET_VA_DATA VaInfo;
    IPL_SET_FLIP_DATA FlipInfo;
} IPL_SET_CMD_DATA;

/**
     IPL SET command

     @note only one command can be executed each time

     @param[in] Cmd       ipl command (IPL_SET_CMD and user define command)
     @param[in] Data      ipl command data

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_SetCmd(UINT32 Cmd, void *Data);

/**
     wait ipl SET command finish

     @note only valid for SET command
*/
extern void IPL_WaitCmdFinish(void);
/***********************************************************/
/*                  IPL get command                        */
/***********************************************************/

/**
    IPL GET Command
*/
typedef enum _IPL_GET_CMD
{
    IPL_GET_CUR_MODE = 0,               ///< get current mode command, data type IPL_MODE_INFOR
    IPL_GET_CUR_STATUS,                 ///< get current ipl status command, data type IPL_STATUS_INFOR
    IPL_GET_CUR_BUF_INFOR,              ///< get current buffer information, data type IPL_BUF_INFOR
    IPL_GET_CUR_IPL_INFOR,              ///< get current ipl information command, data type IPL_CUR_INFOR
    IPL_GET_IME_INFOR,                  ///< get IME information, data IPL_IME_INFOR
    IPL_GET_IME_CUR_BUF_ADDR,           ///< get IME current output Address, data type IPL_IME_BUF_ADDR
    IPL_GET_SIE_CUR_CAP_CH0_RAW_INFOR,  ///< get SIE channel 0 current output size, data type IPL_SIE_RAW_INFO
    IPL_GET_SIE_CUR_CAP_CH3_RAW_INFOR,  ///< get SIE channel 3 current output size, data type IPL_SIE_RAW_INFO
    IPL_GET_SIE_CAP_MAX_RAW_INFOR,      ///< get SIE channel 0 max output size, data type IPL_SIE_RAW_INFO
    IPL_GET_CAP_INFOR,                  ///< get capture information data type IPL_CAP_INFOR
    IPL_GET_CAP_ENC_RST_INFOR,          ///< get image raw encode information, data type IPL_ENC_RST_INFOR
    IPL_GET_CUR_DZ_INFOR,               ///< get current digital zoom information, data type IPL_DZOOM_INFOR
    IPL_GET_CUR_CA_INFOR,               ///< get current image CA information, data type IPL_CA_INFOR
    IPL_GET_CA_INFOR,                   ///< get CA setting information, data type IPL_CA_SET_INFOR
    IPL_GET_CUR_LA_INFOR,               ///< get current image LA information, data type IPL_LA_INFOR
    IPL_GET_LA_INFOR,                   ///< get LA setting information, data type IPL_LA_SET_INFOR
    IPL_GET_CUR_VA_INFOR,               ///< get current image VA information data type IPL_VA_INFOR
    IPL_GET_RDE_CUR_RAW_INFOR,          ///< get current Rde raw information data type IPL_RDE_RAW_INFO
    IPL_GET_DRE_WORK_BUF_INFOR,         ///< get DRE working buffer size data type IPL_DRE_BUF_INFO
    IPL_GET_SIE_CUR_FRAME_INFOR,        ///< get SIE current frame information, data type IPL_SIE_CUR_FRAME_INFO
    IPL_GET_DCE_NOR_FACTOR_INFOR,       ///< get dce normal factor inforamtion(only for RSC), data type IPL_DCE_NOR_FACTOR_INFO
    IPL_GET_CUR_CTRLFLOW,               ///< get video recording ipl control flow command, data type IPL_CTRL_FLOW_INFO
    IPL_GET_MAX_CNT_DEF,
    ENUM_DUMMY4WORD(IPL_GET_CMD)
} IPL_GET_CMD;

#define IPL_GET_SWITCH_ALL  0xffffffff
#define IPL_GET_CMD_SWITCH_TAB  32      ///< maxinum value of IPL GET command

//check IPL_GET_CMD range
STATIC_ASSERT((IPL_GET_MAX_CNT_DEF <= IPL_GET_CMD_SWITCH_TAB));

/********* IPL_VA_INFOR *********/
/**
    IPL va window id
*/
typedef enum _VA_WINIDX
{
    VA_WIN_16X16 = 0,               ///< va 16 * 16 window
    VA_WIN_INDP0,                   ///< va independent window 0
    VA_WIN_INDP1,                   ///< va independent window 1
    VA_WIN_INDP2,                   ///< va independent window 2
    VA_WIN_INDP3,                   ///< va independent window 3
    VA_WIN_INDP4,                   ///< va independent window 4
    ENUM_DUMMY4WORD(VA_WINIDX)
} VA_WINIDX;

/**
    IPL current va information
*/
typedef struct _IPL_VA_INFOR
{
    IPL_PROC_ID Id;                 ///< input parameter, ipl process id
    UINT32 *Grp1H;                  ///< va group 1H output pointer
    UINT32 *Grp1V;                  ///< va group 1V output pointer
    UINT32 *Grp2H;                  ///< va group 2H output pointer
    UINT32 *Grp2V;                  ///< va group 2V output pointer
    VA_WINIDX VAWin;                ///< va output window
} IPL_VA_INFOR;

/********* IPL_CA_INFOR *********/
/**
    IPL current ca information
*/
typedef struct _IPL_CA_INFOR
{
    IPL_PROC_ID Id;             ///< input parameter, ipl process id
    UINT16 *R;                  ///< output parameter, need assign memory
    UINT16 *G;                  ///< output parameter, need assign memory
    UINT16 *B;                  ///< output parameter, need assign memory
    UINT16 *AccCnt;             ///< output parameter, need assign memory
    UINT32 Length;              ///< input array length for *R, *G, *B
} IPL_CA_INFOR;

/********* IPL_CA_SET_INFOR *********/

/**
    IPL ca setting information
*/
typedef struct _IPL_CA_SET_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    UINT32 WinXNum;             ///<output parameter, the number of the horizontal window
    UINT32 WinYNum;             ///<output parameter, the number of the vertical window
    UINT32 CropSizeH;           ///<output parameter, CA crop H size
    UINT32 CropSizeV;           ///<output parameter, CA corp V size
} IPL_CA_SET_INFOR;

/********* IPL_LA_INFOR *********/
/**
    IPL current lca information
*/
typedef struct _IPL_LA_INFOR
{
    IPL_PROC_ID Id;             ///< input parameter, ipl process id
    UINT16 *Lum1;               ///< 12 bit Pre  Gamma luminance
    UINT16 *Lum2;               ///< 10 bit Post Gamma luminance
    UINT16 *Histo;              ///< output: historgram
} IPL_LA_INFOR;

/********* IPL_LA_SET_INFOR *********/

/**
    IPL la setting information
*/
typedef struct _IPL_LA_SET_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    UINT32 WinXNum;             ///<output parameter, the number of the horizontal window
    UINT32 WinYNum;             ///<output parameter, the number of the vertical window
    UINT32 CropSizeH;           ///<output parameter, LA crop H size
    UINT32 CropSizeV;           ///<output parameter, LA corp V size
} IPL_LA_SET_INFOR;

/********* IPL_GET_CUR_DZ_INFOR *********/

/**
    IPL current digital zoom information
*/
typedef struct _IPL_DZOOM_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    UINT32 CurIdx;              ///<output parameter, digital zoom current index
    UINT32 MaxIdx;              ///<output parameter, digital zoom maxinum index
    UINT32 Ratio;               ///<output parameter, digital zoom ratio
} IPL_DZOOM_INFOR;

/********* IPL_GET_CAP_ENC_RST_INFOR *********/

/**
    IPL rde information(only valid when in capture mode)
*/
typedef struct _IPL_ENC_RST_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    struct
    {
        UINT32            uiOutputSize;       ///< Raw encoder - accumulated output byte count
        UINT32            uiOvflLine;
        UINT16            uiBrcLnCntLfn[8];
    } SIE_ENC_RSLT;
} IPL_ENC_RST_INFOR;

/********* IPL_CAP_INFOR *********/

/**
    IPL capture information(only valid when in capture mode)
*/
typedef struct _IPL_CAP_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    UINT32 SIECh3SubRatio;      ///<output parameter, sie channel3 output ratio(%)
    UINT32 IPESubRatio;         ///<output parameter, ipe subout ratio(%)
    UINT32 DFEn;                ///<output parameter, dark frame function enable/disable
    UINT32 MSNREn;              ///<output parameter, MSNR function enable/disable
    UINT32 LCAEn;               ///<output parameter, LCA function enable/disable
} IPL_CAP_INFOR;


/********* IPL_GET_CUR_MODE *********/

/**
    IPL current mode information
*/
typedef struct _IPL_MODE_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    IPL_MODE Mode;              ///<output parameter, current mode
} IPL_MODE_INFOR;


/********* IPL_GET_CUR_STATUS *********/

/**
    IPL current status information
*/
typedef struct _IPL_STATUS_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    IPL_STATUS Status;          ///<output parameter, current ipl status
} IPL_STATUS_INFOR;

/********* IPL_GET_CUR_IPL_INFOR *********/

/**
    IPL current information
*/
typedef struct _IPL_CUR_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    UINT32 Ipl_CF_Mode;         ///<ipl current control flow mode [NORMAL, CCIR, HDR, 2STRP]
} IPL_CUR_INFOR;

/********* IPL_GET_CUR_BUF_INFO *********/

/**
    IPL current buffer information
*/
typedef struct _IPL_BUF_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    UINT32 BufAddr;             ///<output parameter, current ipl buffer address
    UINT32 BufSize;             ///<output parameter, current ipl buffer size
} IPL_BUF_INFOR;

/********* IPL_GET_IME_INFOR *********/

/**
    IPL ime information
*/
typedef struct _IPL_IME_INFOR
{
    IPL_PROC_ID Id;             ///<input parameter, ipl process id
    UINT32 P1Num;               ///<output parameter, path1 buffer number
    UINT32 P1Enable;            ///<output parameter, path1 enable/disable
    IPL_YCC_IMG_INFO ImeP1[10]; ///<output parameter, path1 image setting(format, buffer address, size)
    struct
    {
        Coordinate   Start;                 ///< crop start
        UINT32       SizeH;                 ///< crop size H
        UINT32       SizeV;                 ///< crop size V
    }IME_P1_Crop;
    UINT32 P2Num;               ///<output parameter, path2 buffer number
    UINT32 P2Enable;            ///<output parameter, path2 enable/disable
    IPL_YCC_IMG_INFO ImeP2[10]; ///<output parameter, path2 image setting(format, buffer address, size)
    struct
    {
        Coordinate   Start;                 ///< crop start
        UINT32       SizeH;                 ///< crop size H
        UINT32       SizeV;                 ///< crop size V
    }IME_P2_Crop;
    UINT32 P3Num;               ///<output parameter, path3 buffer number
    UINT32 P3Enable;            ///<output parameter, path3 enable/disable
    IPL_YCC_IMG_INFO ImeP3[10]; ///<output parameter, path3 image setting(format, buffer address, size)
    struct
    {
        Coordinate   Start;                 ///< crop start
        UINT32       SizeH;                 ///< crop size H
        UINT32       SizeV;                 ///< crop size V
    }IME_P3_Crop;
    UINT32 P4Num;               ///<output parameter, path4 buffer number
    UINT32 P4Enable;            ///<output parameter, path4 enable/disable
    IPL_YCC_IMG_INFO ImeP4[10]; ///<output parameter, path4 image setting(format, buffer address, size)
    struct
    {
        Coordinate   Start;                 ///< crop start
        UINT32       SizeH;                 ///< crop size H
        UINT32       SizeV;                 ///< crop size V
    }IME_P4_Crop;
} IPL_IME_INFOR;

/********* IPL_GET_IME_CUR_BUF_ADDR *********/

/**
    IPL current ime output information
*/
typedef struct _IPL_IME_BUF_ADDR
{
    IPL_PROC_ID Id;                 ///<input parameter, ipl process id
    UINT32 P1Enable;                ///<output parameter, path1 enable/disable
    IPL_YCC_IMG_INFO ImeP1;         ///<output parameter, current path1 image setting(format, buffer address, size)
    UINT32 P2Enable;                ///<output parameter, path2 enable/disable
    IPL_YCC_IMG_INFO ImeP2;         ///<output parameter, current path2 image setting(format, buffer address, size)
    UINT32 P3Enable;                ///<output parameter, path3 enable/disable
    IPL_YCC_IMG_INFO ImeP3;         ///<output parameter, current path3 image setting(format, buffer address, size)
    UINT32 P4Enable;                ///<output parameter, path4 enable/disable
    IPL_YCC_IMG_INFO ImeP4;         ///<output parameter, current path4 image setting(format, buffer address, size)
} IPL_IME_BUF_ADDR;

/********* IPL_SIE_RAW_SIZE *********/

/**
    IPL raw information
*/
typedef struct _IPL_SIE_RAW_INFO
{
    IPL_PROC_ID Id;                 ///<input parameter, ipl process id
    UINT32 ImgWidth;                ///<output parameter, raw width
    UINT32 ImgHeight;               ///<output parameter, raw height
    UINT32 ImgLineOfs;              ///<output parameter, raw line offset
    IPL_RAW_BIT ImgBit;             ///<output parameter, raw bit
    IPL_RAW_PIX ImgPix;             ///<output parameter, raw start pixel
    UINT32 SensorType;             ///<output parameter, raw start pixel
} IPL_SIE_RAW_INFO;

/********* IPL_SIE_RAW_SIZE *********/

/**
    IPL raw information
*/
typedef IPL_SIE_RAW_INFO IPL_RDE_RAW_INFO;

/********* IPL DRE Working buffer size *********/

/**
    IPL DRE Working buffer information
*/
typedef struct _IPL_DRE_BUF_INFO
{
    IPL_PROC_ID Id;                 ///<input parameter, ipl process id
    BOOL HDRMode;                   ///<HDR mode on/off
    UINT32 ImgWidth;                ///<image size H
    UINT32 ImgHeight;               ///<image size V
    UINT32 ImgFmt;
    UINT32 BufSize;                 ///<output paramter
} IPL_DRE_BUF_INFO;

/********* IPL SIE frame cnt *********/

/**
    IPL SIE frame cnt infomation
*/
typedef struct _IPL_SIE_CUR_FRAME_INFO
{
    IPL_PROC_ID Id;                 ///<input parameter, ipl process id
    UINT32 FrameCnt;                ///<output parameter, frame cnt
    UINT32 OutSizeH;                ///<output parameter, sie out size H
    UINT32 OutSizeV;                ///<output parameter, sie out size V
    UINT32 OutSizeLofs;             ///<output parameter, sie out size line ofs
} IPL_SIE_CUR_FRAME_INFO;

/********* IPL Control Flow *********/

/**
    IPL Control Flow infomation
*/
typedef struct _IPL_CTRL_FLOW_INFO
{
    IPL_PROC_ID Id;                 ///<input parameter, ipl process id
    IPL_CTRL_FLOW_TYPE CtrlFlow;    ///<output parameter, ipl control flow type
} IPL_CTRL_FLOW_INFO;

/********* IPL DCE normal factor information *********/

/**
    IPL DCE normal factor infomation
*/
typedef struct _IPL_DCE_NOR_FACTOR_INFO
{
    IPL_PROC_ID Id;                 ///<input parameter, ipl process id
    UINT32 width;
    UINT32 height;
    UINT32 centX;
    UINT32 centY;
    UINT32 Xdisf;
    UINT32 Ydisf;
    UINT32 NormFact;
    UINT32 NormBit;
} IPL_DCE_NOR_FACTOR_INFO;

/********* IPL_GET_CMD *********/
typedef union
{
    IPL_MODE_INFOR ModeInfo;
    IPL_STATUS_INFOR StatusInfo;
    IPL_CUR_INFOR IplCurInfo;
    IPL_BUF_INFOR BufInfo;
    IPL_IME_INFOR ImeInfo;
    IPL_IME_BUF_ADDR ImeBufInfo;
    IPL_SIE_RAW_INFO RawInfo;
    IPL_CAP_INFOR CapInfo;
    IPL_ENC_RST_INFOR EncRstInfo;
    IPL_DZOOM_INFOR DZInfo;
    IPL_CA_INFOR CaInfo;
    IPL_CA_SET_INFOR CaSetInfo;
    IPL_LA_INFOR LaInfo;
    IPL_LA_SET_INFOR LaSetInfo;
    IPL_VA_INFOR VaInfo;
    IPL_RDE_RAW_INFO RdeRawInfo;
    IPL_DRE_BUF_INFO DreBufInfo;
    IPL_SIE_CUR_FRAME_INFO SIECurFrameInfo;
    IPL_DCE_NOR_FACTOR_INFO DCENorFactorInfo;
    IPL_CTRL_FLOW_INFO CtrlInfo;
} IPL_GET_CMD_DATA;

#define IPL_CMD_VAR2STR(x)   (#x)
#define IPL_CMD_DUMPD(x)      DBG_DUMP("%s = %d\r\n", IPL_CMD_VAR2STR(x), x)
#define IPL_CMD_DUMPH(x)      DBG_DUMP("%s = 0x%.8x\r\n", IPL_CMD_VAR2STR(x), x)

/**
     Get IPL command

     @param[in] Cmd       ipl command (IPL_GET_CMD)
     @param[in,out] Data  ipl command data

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_GetCmd(IPL_GET_CMD Cmd, void *Data);

/**
     IPL open
     initial ipl module

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_Open(void);

/**
     IPL close
     un-initial ipl module

     @return Description of data returned.
         - @b E_OK:       command ready
         - @b E_SYS:
*/
extern ER IPL_Close(void);

/**
     check IPL module is open or not

     @return Description of data returned.
         - @b TRUE: ipl opened
         - @b FALSE: ipl closed
*/
extern BOOL IPL_IsOpen(void);

/**
     install IPL control id
*/
extern void Install_IPLCtrlID(void) _SECTION(".kercfg_text");

/**
     install IPL extend id
*/
extern void IPL_InstallID(void) _SECTION(".kercfg_text");

/**
     IPL_DumpDataStatus
     dump ipl status

     @return Description of data returned.
         - @UINT32 data number
*/
extern UINT32 IPL_DumpDataStatus(UINT32 Id, UINT32* pData);

extern void IPL_DumpIPLInfo(void);
//@}
#endif //_IPL_CMD_H_
