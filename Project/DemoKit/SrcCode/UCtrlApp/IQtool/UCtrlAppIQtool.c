#include "SysCommon.h"
#include "UIFramework.h"
#include "PrjCfg.h"
#include "UIInfo.h"
#include "UIControlWnd.h"
#include "uToken.h"
#include "NvtUctrlAPI.h"
#include "UCtrlMain.h"
#include "Exif.h"
#include "ExifDef.h"
#include "UCtrlAppIQtool.h"

#include <IQtoolAPI.h>
#define THIS_DBGLVL         2 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlIQTool
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
#include "DebugModule.h"



#define IQTOOL_RETURN_OK  "OK"
#define IQTOOL_RETURN_ERR "ERROR"




static INT32 param_begin(char* str, INT32 len)
{
    DBG_IND("get: %s\r\n",str);
    return 0;
}

static INT32 param_end(UTOKEN_END_INFO* p_info)
{
    char retStr[64];
    DBG_IND("\r\n");
    if (p_info->err_code < 0)
    {
        //sprintf(retStr, "ERR: parm %s", p_info->failed_name);
        snprintf(retStr, sizeof(retStr), "ERR: parm %s", p_info->failed_name);
        NvtUctrl_SetRetString(retStr);
        DBG_ERR("%s\r\n",retStr);
        return p_info->err_code;
    }
    return 0;
}

static INT32 param_unknown(char* name)
{
    DBG_ERR("[IQTool] unknown param: %s\r\n",name);
    return 0;
}

static INT32 iqtool_cmd(INT32 argc, char* argv[]){

    if(argc <= 1){
        DBG_ERR("[IQTool] iqtool cmd argc =%d error\r\n",argc);
        NvtUctrl_SetRetString(IQTOOL_RETURN_ERR);
    }

    if(!strcmp(argv[0],IQTOOL_IQTUC_CHAR)){

        IQTOOL_INFO_IQTUC iqtool_info_iqtuc={0};
        if(argc >= 3){
            ////no params
            strncpy(iqtool_info_iqtuc.cmd,argv[1],sizeof(iqtool_info_iqtuc.cmd));
            iqtool_info_iqtuc.cmd[sizeof(iqtool_info_iqtuc.cmd)-1]='\0';
            iqtool_info_iqtuc.sensor = atoi(argv[2]);
        }
        if(argc == 4){
           ////have params
            strncpy(iqtool_info_iqtuc.params,argv[3],sizeof(iqtool_info_iqtuc.params));
            iqtool_info_iqtuc.params[sizeof(iqtool_info_iqtuc.params)-1]='\0';
        }

        IQtool_start_tsk(IQTOOL_IQTUC_CHAR,(void *)&iqtool_info_iqtuc);


    }
    else if(!strcmp(argv[0],IQTOOL_CALTOOL_CHAR)){
        IQTOOL_INFO_CAL iqtool_info_cal={0};
        if(argc >= 2){
            ////get cmd
            strncpy(iqtool_info_cal.cmd,argv[1],sizeof(iqtool_info_cal.cmd));
            iqtool_info_cal.cmd[sizeof(iqtool_info_cal.cmd)-1]='\0';
        }

        if(argc >= 3){
           ////get option or params

            if(!strcmp(iqtool_info_cal.cmd, "start")){

                strncpy(iqtool_info_cal.option,argv[2],sizeof(iqtool_info_cal.option));
                iqtool_info_cal.option[sizeof(iqtool_info_cal.option)-1]='\0';
            }
            else{
                strncpy(iqtool_info_cal.params,argv[2],sizeof(iqtool_info_cal.params));
                iqtool_info_cal.params[sizeof(iqtool_info_cal.params)-1]='\0';
            }
        }
        if(argc == 4){
           ////get params
            strncpy(iqtool_info_cal.params,argv[3],sizeof(iqtool_info_cal.params));
            iqtool_info_cal.params[sizeof(iqtool_info_cal.params)-1]='\0';
        }
        IQtool_start_tsk(IQTOOL_CALTOOL_CHAR,(void *)&iqtool_info_cal);

    }
    else{
        DBG_ERR("[IQTool] tool type =%d error\r\n",argv[0]);
        NvtUctrl_SetRetString(IQTOOL_RETURN_ERR);
        return 0;
    }
    INT32 status=0;
    char tmp_buf[IQTOOL_PARAM_MAX_LEN]={0};
    char ret_buf[1024]={0};

    ////wait status value
    status = IQtool_get_ret_status();
    IQtool_get_ret_param(tmp_buf);
    if(strlen(tmp_buf) <=0){
        sprintf(ret_buf,"%d",status);
    }
    else{
        sprintf(ret_buf,"%d;%s",status,tmp_buf);
    }
    NvtUctrl_SetConfigData((void *)&ret_buf,sizeof(ret_buf));

    NvtUctrl_SetRetString(IQTOOL_RETURN_OK);
    return 0;
}


UTOKEN_PARAM tbl_iqtool_param[] = {
    {"iqtool_cmd",iqtool_cmd},
    {NULL,NULL}, //last tag, it must be
};

UTOKEN_MODULE module_iqtool = {
    "uiqtool",
    tbl_iqtool_param,
    param_begin,
    param_end,
    param_unknown,
    NULL
};


