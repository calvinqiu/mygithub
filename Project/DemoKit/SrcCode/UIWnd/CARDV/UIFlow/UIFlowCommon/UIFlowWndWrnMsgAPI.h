//This source code is generated by UI Designer Studio.

#ifndef UIFLOWWNDWRNMSGAPI_H
#define UIFLOWWNDWRNMSGAPI_H

#include "UIFlowWndWrnMsg.h"

//-----------------------------------------------------------------------------
// UIFlowWndWrnMsg User Definitions
//-----------------------------------------------------------------------------
// Warning message issue
typedef enum
{
    FLOWWRNMSG_ISSUE_BATTERY_LOW    = STRID_BATTERY_LOW,
    FLOWWRNMSG_ISSUE_NO_CARD = STRID_NO_CARD,
    FLOWWRNMSG_ISSUE_CARD_FULL = STRID_CARD_FULL,
    FLOWWRNMSG_ISSUE_MEM_FULL = STRID_MEMORY_FULL,
    FLOWWRNMSG_ISSUE_CARD_ERR = STRID_CARDERROR,
    FLOWWRNMSG_ISSUE_MEM_ERR = STRID_MEMORYERROR,
    FLOWWRNMSG_ISSUE_CARD_LOCKED = STRID_CARD_LOCKED,
    FLOWWRNMSG_ISSUE_FILE_PROTECTED = STRID_FILE_PROTECT,
    FLOWWRNMSG_ISSUE_NO_IMAGE = STRID_NO_FILE,
    FLOWWRNMSG_ISSUE_NOT_JPEG = STRID_NOT_JPEG,
    FLOWWRNMSG_ISSUE_PICTURE_ERR = STRID_FILE_ERROR,
    FLOWWRNMSG_ISSUE_CANT_CROP = STRID_FILE_ERROR,
    FLOWWRNMSG_ISSUE_PLEASE_INSERT_SD = STRID_PLEASE_INSERT_SD,
    FLOWWRNMSG_ISSUE_CLUSTER_WRONG = STRID_CLUSTER_WRONG,
    FLOWWRNMSG_ISSUE_SD_CLASS4 = STRID_SD_CLASS4,
    FLOWWRNMSG_ISSUE_SD_CLASS6 = STRID_SD_CLASS6,
    FLOWWRNMSG_ISSUE_MAX
} FLOWWRNMSG_ISSUE;

// Warning message window timeout value
typedef enum
{
    FLOWWRNMSG_TIMER_KEEP    = 0,
    FLOWWRNMSG_TIMER_1SEC,
    FLOWWRNMSG_TIMER_2SEC,
    FLOWWRNMSG_TIMER_3SEC,
    FLOWWRNMSG_TIMER_4SEC,
    FLOWWRNMSG_TIMER_5SEC,
    FLOWWRNMSG_TIMER_MAX
} FLOWWRNMSG_TIMER;


//##### only for backward compatible ########
#define UIFlowWndWrnMsg_StatusTXT_Msg_STRID_BATTERY_LOW         FLOWWRNMSG_ISSUE_BATTERY_LOW
#define UIFlowWndWrnMsg_StatusTXT_Msg_STRID_NO_CARD             FLOWWRNMSG_ISSUE_NO_CARD
#define UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_FULL           FLOWWRNMSG_ISSUE_CARD_FULL
#define UIFlowWndWrnMsg_StatusTXT_Msg_STRID_MEMORY_FULL         FLOWWRNMSG_ISSUE_MEM_FULL
#define UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARDERROR           FLOWWRNMSG_ISSUE_CARD_ERR
#define UIFlowWndWrnMsg_StatusTXT_Msg_STRID_MEMORYERROR         FLOWWRNMSG_ISSUE_MEM_ERR
#define UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CARD_LOCKED         FLOWWRNMSG_ISSUE_CARD_LOCKED
#define UIFlowWndWrnMsg_StatusTXT_Msg_STRID_FILE_PROTECT        FLOWWRNMSG_ISSUE_FILE_PROTECTED
#define UIFlowWndWrnMsg_StatusTXT_Msg_STRID_NO_IMAGE            FLOWWRNMSG_ISSUE_NO_IMAGE
#define UIFlowWndWrnMsg_StatusTXT_Msg_STRID_NOT_JPEG            FLOWWRNMSG_ISSUE_NOT_JPEG
#define UIFlowWndWrnMsg_StatusTXT_Msg_STRID_NO_FILE             FLOWWRNMSG_ISSUE_NO_IMAGE
#define UIFlowWndWrnMsg_StatusTXT_Msg_STRID_PLEASE_INSERT_SD    FLOWWRNMSG_ISSUE_PLEASE_INSERT_SD
#define UIFlowWndWrnMsg_StatusTXT_Msg_STRID_CLUSTER_WRONG       FLOWWRNMSG_ISSUE_CLUSTER_WRONG
#define UIFlowWndWrnMsg_StatusTXT_Msg_STRID_SD_CLASS4           FLOWWRNMSG_ISSUE_SD_CLASS4
#define UIFlowWndWrnMsg_StatusTXT_Msg_STRID_SD_CLASS6           FLOWWRNMSG_ISSUE_SD_CLASS6

#endif
