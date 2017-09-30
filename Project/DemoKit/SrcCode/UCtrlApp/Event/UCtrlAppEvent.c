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
#include "UCtrlAppEvent.h"
#include "gpio.h"
#define THIS_DBGLVL         1 //0=OFF, 1=ERROR, 2=TRACE
///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          UCtrlEvent
#define __DBGLVL__          ((THIS_DBGLVL>=PRJ_DBG_LVL)?THIS_DBGLVL:PRJ_DBG_LVL)
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
//#define __DBGFLT__          "[cap]"
#include "DebugModule.h"



static INT32 param_event_test(INT32 argc, char* argv[])
{
    DBG_DUMP("ucontrol param_event_test test!!!\r\n");
    UINT32 uiPinState = 0;
    gpio_open();

    gpio_setDir(C_GPIO_0, GPIO_DIR_OUTPUT);
    uiPinState = gpio_getPin(C_GPIO_0);
    DBG_DUMP("first test GPIO uiPinState=%d\r\n",uiPinState);
    gpio_setPin(C_GPIO_0);
    uiPinState = gpio_getPin(C_GPIO_0);
    DBG_DUMP("second test GPIO uiPinState=%d\r\n",uiPinState);
    gpio_clearPin(C_GPIO_0);
    uiPinState = gpio_getPin(C_GPIO_0);
    DBG_DUMP("3third test GPIO uiPinState=%d\r\n",uiPinState);
    gpio_close();

    return 0;
}
static INT32 param_event_test2(INT32 argc, char* argv[])
{


    UINT32 uiPinState = 0;
    gpio_open();
    gpio_setDir(C_GPIO_1, GPIO_DIR_OUTPUT);
    uiPinState = gpio_getPin(C_GPIO_1);
    DBG_DUMP("test GPIO_1 staus=%d\r\n",uiPinState);
    gpio_close();

    return 0;
}
static INT32 response_event_status(INT32 uiState, EVT_ALARM_TYPE event_type){

    if(event_type == EVT_ALARM_LEVEL_HIGH)
    {
         if(uiState == 1)
        {
            NvtUctrl_SetRetString(ALARM_HIGH);
        }
        else if(uiState == 0){
            NvtUctrl_SetRetString(ALARM_LOW);
        }
        else{
            NvtUctrl_SetRetString(TRIGGER_ERROR);
        }

    }
    else if(event_type == EVT_ALARM_LEVEL_LOW)
    {
        if(uiState == 1)
        {
            NvtUctrl_SetRetString(ALARM_LOW);
        }
        else if(uiState == 0){
            NvtUctrl_SetRetString(ALARM_HIGH);
        }
        else{
            NvtUctrl_SetRetString(TRIGGER_ERROR);
        }

    }
    else if(event_type == EVT_MOTION){
        if(uiState == 1)
        {
            NvtUctrl_SetRetString(TRIGGER_TRUE);
        }
        else if(uiState == 0){
            NvtUctrl_SetRetString(TRIGGER_FALSE);
        }
        else{
            NvtUctrl_SetRetString(TRIGGER_ERROR);
        }
    }


    return 0;
}
static INT32 param_event_pull_alarm_low(INT32 argc, char* argv[])
{


    UINT32 uiPinState = 0;
    gpio_open();
    gpio_setIntTypePol(GPIO_INT_01, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_NEGLOW);
    uiPinState = gpio_getIntStatus(GPIO_INT_01);
    gpio_close();

    response_event_status(uiPinState, EVT_ALARM_LEVEL_LOW);

    return 0;
}

static INT32 param_event_pull_alarm(INT32 argc, char* argv[])
{

    UINT32 uiPinState = 0;
    gpio_open();

    gpio_setDir(C_GPIO_9, GPIO_DIR_INPUT);   ///GPIO_INT_01 mapping C_GPIO_9
    uiPinState = gpio_getPin(C_GPIO_9);
    gpio_close();

    if(uiPinState == 1){

        NvtUctrl_SetRetString(ALARM1_HIGH);
    }
    else if(uiPinState == 0){

        NvtUctrl_SetRetString(ALARM1_LOW);
    }
    else{
        DBG_ERR("C_GPIO_9 error state=%d\r\n",uiPinState);
        NvtUctrl_SetRetString(TRIGGER_ERROR);
    }

    return 0;
}


static INT32 param_event_pull_alarm_high(INT32 argc, char* argv[])
{


    UINT32 uiPinState = 0;
    gpio_open();
    gpio_setIntTypePol(GPIO_INT_01, GPIO_INTTYPE_LEVEL, GPIO_INTPOL_POSHIGH);
    uiPinState = gpio_getIntStatus(GPIO_INT_01);
    gpio_close();

    response_event_status(uiPinState, EVT_ALARM_LEVEL_HIGH);
    return 0;

}

void Notify_alarm_event(void)
{

    DBG_DUMP("Notify_alarm_event start!!\r\n");

    UINT32 uiPinState = 0;
    gpio_open();

    gpio_setDir(C_GPIO_9, GPIO_DIR_INPUT);   ///GPIO_INT_01 mapping C_GPIO_9
    uiPinState = gpio_getPin(C_GPIO_9);

    gpio_close();

    if(uiPinState == 1){
        DBG_DUMP("send ALARM1_HIGH\r\n");
        NvtUctrl_Notify(ALARM1_HIGH);
    }
    else if(uiPinState == 0){
        DBG_DUMP("send ALARM1_low\r\n");
        NvtUctrl_Notify(ALARM1_LOW);
    }
    else{
        DBG_ERR("C_GPIO_9 error state=%d\r\n",uiPinState);
        DBG_DUMP("send error\r\n");
        NvtUctrl_Notify(TRIGGER_ERROR);
    }


//    NvtUctrl_Notify("test!!");

}

void register_event(void)
{
    //no need this function now
    gpio_open();
    gpio_setIntTypePol(GPIO_INT_01, GPIO_INTTYPE_EDGE, GPIO_INTPOL_BOTHEDGE);
    gpio_setIntIsr(GPIO_INT_01, (DRV_CB)Notify_alarm_event);
    gpio_enableInt(GPIO_INT_01);
    gpio_close();


}




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
    DBG_ERR("unknown param: %s\r\n",name);
    return 0;
}

static INT32 param_set_alarm_output(INT32 argc, char* argv[])
{
    char retStr[64];
    UINT32 uiPinState = 0;
    gpio_open();

    gpio_setDir(C_GPIO_10, GPIO_DIR_OUTPUT);

    if(!strcmp(argv[0],ALARM_OUTPUT_OPEN_CMD)){
        gpio_setPin(C_GPIO_10);
    }
    else if(!strcmp(argv[0],ALARM_OUTPUT_GROUNDED_CMD)){
        gpio_clearPin(C_GPIO_10);
    }
    else{
        DBG_ERR("set alarm output error argv[0]=%s\n",argv[0]);
    }
    uiPinState = gpio_getPin(C_GPIO_10);
    DBG_DUMP("alarm output set finish uiPinState=%d\r\n",uiPinState);
    strncpy(retStr, ALARM_OUTPUT_SET_OK, sizeof(retStr)-1);
    retStr[sizeof(retStr)-1]='\0';
    NvtUctrl_SetRetString(retStr);
    gpio_close();

    return 0;
}

static INT32 param_get_alarm_output(INT32 argc, char* argv[])
{
    char retStr[64];
    UINT32 uiPinState = 0;
    gpio_open();

    //gpio_setDir(C_GPIO_10, GPIO_DIR_OUTPUT);

    uiPinState = gpio_getPin(C_GPIO_10);
    if(uiPinState == 1){
        strncpy(retStr, ALARM_OUTPUT_OPEN_CMD, sizeof(retStr)-1);
    }
    else{
        strncpy(retStr, ALARM_OUTPUT_GROUNDED_CMD, sizeof(retStr)-1);
    }


    retStr[sizeof(retStr)-1]='\0';
    NvtUctrl_SetRetString(retStr);
    gpio_close();

    return 0;
}



UTOKEN_PARAM tbl_event_param[] = {
    {"test",param_event_test},
    {"test2",param_event_test2},
    {"pull_alarm_low",param_event_pull_alarm_low},
    {"pull_alarm_high",param_event_pull_alarm_high},
    {"pull_alarm",param_event_pull_alarm},
    {"alarm_output_set",param_set_alarm_output},
    {"alarm_output_get",param_get_alarm_output},
    {NULL,NULL}, //last tag, it must be
};

UTOKEN_MODULE module_event = {
    "uevent",
    tbl_event_param,
    param_begin,
    param_end,
    param_unknown,
    NULL
};


