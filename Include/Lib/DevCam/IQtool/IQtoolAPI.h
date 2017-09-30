/*
    Copyright (c) 2016  Novatek Microelectronics Corporation

    @file IQtoolAPI.h

    @version

    @date
*/

#ifndef _IQTOOLAPI_H
#define _IQTOOLAPI_H

#include "Type.h"
#define IQTOOL_PARAM_MAX_LEN 900
#define MAX_SHOWLINE_COUNT  16
#define IQTOOL_IQTUC_CHAR      "iqtuc"
#define IQTOOL_CALTOOL_CHAR    "caltool"
#define IQTOOL_MOUNT_RAMDISK0  "/tmp/iq_ramdisk0" ////do not modify, this value is mapping linux

typedef enum _IQTOOL_TOOL_TYPE{
    IQTOOL_TOOL_TYPE_IQTUC,
    IQTOOL_TOOL_TYPE_CALTOOL

}IQTOOL_TOOL_TYPE;

typedef struct _IQTOOL_INFO_IQTUC{

    char cmd[64];
    char params[IQTOOL_PARAM_MAX_LEN];
    int sensor;

}IQTOOL_INFO_IQTUC;

typedef struct _IQTOOL_INFO_CAL{

    char cmd[64];
    char option[64];
    char params[IQTOOL_PARAM_MAX_LEN];

}IQTOOL_INFO_CAL;

typedef struct _IQTOOL_SHOWLINE{
    INT32  index;
    UINT32 BG_color[3];//background color  [0]:red [1]:green [2]:blue
    UINT32 color[3]; //[0]:Red  [1]:Green [2]:Blue
    char content[512];
}IQTOOL_SHOWLINE;




extern void IQtool_InstallID(void) _SECTION(".kercfg_text");
extern UINT32 IQtool_start_tsk(const char * tool, void *data_info);
extern void IQtool_set_tool_type(IQTOOL_TOOL_TYPE tool_type);
extern IQTOOL_TOOL_TYPE IQtool_get_tool_type(void);
extern void IQtool_set_IQtool_info(void *iqtool_info);
extern void IQtool_get_IQtool_info(void* iqtool_info);
extern void IQtool_set_ret_param(char *params);
extern void IQtool_get_ret_param(char *params);
extern void IQtool_set_ret_status(INT32 status);
extern INT32 IQtool_get_ret_status(void);
extern void IQtool_disable_setkey(void);
extern void IQtool_set_setkey(void);
extern void IQtool_wait_setkey(void);
extern INT32 IQtool_get_setkey(void);
extern void IQtool_set_finish_flag(INT32 enable);
extern INT32 IQtool_get_finish_flag(void);
extern void IQtool_set_showline(IQTOOL_SHOWLINE *showline);
extern void IQtool_get_showline(IQTOOL_SHOWLINE *showline);
extern UINT32 IQtool_get_show_line_count(void);
extern INT32 IQtool_mount_FS(void);
#endif /* _IQTOOLAPI_H  */
