/**
    Public header file for IPL Callback message

    This file is the header file that define the API and data type for IPL Callback message.

    @file       IPL_CBMsg.h
    @ingroup    mILibIPLCom
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.
*/

#ifndef _IPL_CBMSG_H_
#define _IPL_CBMSG_H_
#include "Type.h"
#include "IPL_Utility.h"

/** \addtogroup mIIPL */
//@{

/**
     IPL callback event
*/
typedef enum _IPL_CBMSG
{
    IPL_CBMSG_PREVIEW = 0,          ///< Callback for preview first frame
    IPL_CBMSG_PREVIEWSTABLE,        ///< Callback for preview ae, awb stable

    IPL_CBMSG_CHGMODE_START,        ///< Callback for IPL Change Mode Start
    IPL_CBMSG_CHGMODE_IPLSTOP,      ///< Callback for IPL Change Mode Ipl Stop
    IPL_CBMSG_CHGMODE_END,          ///< Callback for IPL Change Mode End

    IPL_CBMSG_FOCUS,                ///< NULL
    IPL_CBMSG_FOCUSEND,             ///< NULL

    IPL_CBMSG_PREFLASH,             ///< Callback for enter preflash
    IPL_CBMSG_PREFLASHEND,          ///< Callback for exit preflash

    IPL_CBMSG_SETDATASTAMPINFOR,    ///< Callback for set datastamp information in preview mode, data type IPL_CBMSG_DS_INFOR
    IPL_CBMSG_SETPHOTOFRAMEINFOR,   ///< Callback for set datastamp information in preview mode, data type IPL_CBMSG_PF_INFOR

    IPL_CBMSG_CHGDISPSIZEINFOR,     ///< Callback for change display size, data type IPL_CBMSG_DISP_INFOR

    IPL_CBMSG_DZOOM,                ///< Callback for digital zoom end

    IPL_CBMSG_SHUTTER_SOUND,        ///< Callback for shutter sound event

    IPL_CBMSG_IMEFMD,               ///< ime frame end

    IPL_CBMSG_MAX_CNT,
    ENUM_DUMMY4WORD(IPL_CBMSG)
} IPL_CBMSG;

/**
     IPL_CBMSG_SETDATASTAMPINFOR callback data
*/
typedef struct _IPL_CBMSG_DS_INFOR
{
    IPL_YCC_IMG_INFO ImgInfo;           ///< image information
} IPL_CBMSG_DS_INFOR;

/**
     IPL_CBMSG_SETPHOTOFRAMEINFOR callback data
*/
typedef struct _IPL_CBMSG_PF_INFOR
{
    IPL_YCC_IMG_INFO ImgInfo;           ///< image information
} IPL_CBMSG_PF_INFOR;

/**
     IPL_CBMSG_CHGDISPSIZEINFOR callback data
*/
typedef struct _IPL_CBMSG_DISP_INFOR
{
    UINT32 DispPath;            ///< display path
    UINT32 ImgHoriSize;         ///< image hor. size(pixel)
    UINT32 ImgVertSize;         ///< image ver. size(pixel)
    UINT32 ImgLineOffset;       ///< image line offset(byte)
    UINT32 DvcHoriSize;         ///< device hor. size(pixel)
    UINT32 DvcVertSize;         ///< device ver. size(pixel)
} IPL_CBMSG_DISP_INFOR;

/**
     IPL_RSC_CBMSG_IPLModeInfo callback data
*/

typedef struct _IPL_CBMSG_CHGMODE_INFOR
{
    UINT32 PrvMode;
    UINT32 ChgMode;
} IPL_CBMSG_CHGMODE_INFOR;

/**
     IPL callback function prototype
*/
typedef void (*IPL_CBMSG_FP)(IPL_CBMSG Msg, void *Data);

/**
    callback function prototype
*/
typedef void (*IPL_RSC_CBMSG_FP)(IPL_CBMSG Msg, void *Data);

/**
     initial RSC callback message function

     @param[in] Id IPL process Id
     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern void IPL_RSC_CBMsgInit(IPL_PROC_ID Id, IPL_RSC_CBMSG_FP fp);

/**
     un-initial RSC callback message function

     @param[in] Id IPL process Id
     @return [ER]
         - @b E_OK: SUCCESS
         - @b E_SYS: FAIL
*/
extern void IPL_RSC_CBMsgUnInit(IPL_PROC_ID Id);

/**
     send RSC CB message

     @param[in] Id IPL process Id
     @param[in] IPL_CBMSG ipl callback message
     @param[in] void* data pointer
*/
extern void IPL_RSC_CBMsgProc(IPL_PROC_ID Id, IPL_CBMSG Msg, void *Data);

//@}
#endif //_IPL_CBMSG_H_