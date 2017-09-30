
#include "PrjCfg.h"
#include "UIMenuWndWiFiModuleLink.h"
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
#include "UIMenuWndWiFiMobileLinkOK.h"
//#NT#2016/03/23#Isiah Chang -end
#include "UIFramework.h"
#include "UIFrameworkExt.h"
#include "SysMain.h"
//#NT#2017/02/24#Ben Wang -begin
//#NT#Fix that UVC_MULTIMEDIA_FUNC depends on WIFI_AP_FUNC.
#if((WIFI_AP_FUNC==ENABLE) || (UVC_MULTIMEDIA_FUNC== ENABLE))
//#NT#2017/02/24#Ben Wang -end
void UI_OpenWifiWnd(void)
{
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_UVC)
    {
        Ux_OpenWindow(&UIMenuWndWiFiMobileLinkOKCtrl,0);
        return;
    }
#endif
//#NT#2016/05/31#Ben Wang -end
    if(System_GetState(SYS_STATE_CURRSUBMODE)!=System_GetState(SYS_STATE_PREVSUBMODE))
    {
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
        #if(WIFI_UI_FLOW_VER == WIFI_UI_VER_1_0)
            Ux_OpenWindow(&UIMenuWndWiFiModuleLinkCtrl,0);
        #elif(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
            Ux_OpenWindow(&UIMenuWndWiFiMobileLinkOKCtrl,0);
        #endif
    }
//#NT#2016/03/23#Isiah Chang -end
}

void UI_CloseWifiWnd(void)
{
//#NT#2016/05/31#Ben Wang -begin
//#NT#Add UVC multimedia function.
#if(UVC_MULTIMEDIA_FUNC == ENABLE)
    if(System_GetState(SYS_STATE_CURRSUBMODE)==SYS_SUBMODE_UVC)
    {
        Ux_CloseWindow(&UIMenuWndWiFiMobileLinkOKCtrl,0);
        return;
    }
#endif
//#NT#2016/05/31#Ben Wang -end
    if(System_GetState(SYS_STATE_CURRSUBMODE)!=System_GetState(SYS_STATE_NEXTSUBMODE))
    {
//#NT#2016/03/23#Isiah Chang -begin
//#NT#add new Wi-Fi UI flow.
        #if(WIFI_UI_FLOW_VER == WIFI_UI_VER_1_0)
            Ux_CloseWindow(&UIMenuWndWiFiModuleLinkCtrl,0);
        #elif(WIFI_UI_FLOW_VER == WIFI_UI_VER_2_0)
            Ux_CloseWindow(&UIMenuWndWiFiMobileLinkOKCtrl,0);
        #endif
//#NT#2016/03/23#Isiah Chang -end
    }
}
#else
void UI_OpenWifiWnd(void)
{
}

void UI_CloseWifiWnd(void)
{
}
#endif
