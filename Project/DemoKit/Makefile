#----------------------------------------------------------------------
# set the ROOT directory here
#----------------------------------------------------------------------
ROOT_DIR = ../..

MAKE_COMMON_DIR = $(ROOT_DIR)/MakeCommon
include $(MAKE_COMMON_DIR)/MakeOption.txt
include ModelConfig.txt
#xc modify for code control @20170208 start
ifeq ($(strip $(CUST_COMPILE_SUPPORT)), TRUE)
include ../../resource/YqFeatures.mk
endif
#xc modify for code control @20170208 end
include ModelConfig_$(MODEL).txt
include MakeConfig.txt

#----------------------------------------------------------------------
# set the project name here
#----------------------------------------------------------------------
PRJ_NAME = DemoKit

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = . \
    $(ROOT_DIR)/Include

#----------------------------------------------------------------------
# Add/delete the project C source and assembly files here
#----------------------------------------------------------------------
ASM = \
    SrcCode/Kernel/CodeInfo.s

SRC = \
    SrcCode/Kernel/_main.c \
    SrcCode/Kernel/BinInfo.c \
    SrcCode/Kernel/CoreInfo.c \
    SrcCode/Kernel/SysCfg.c

################### CARDV ######################

SRC += \
    SrcCode/System/main.c \
    SrcCode/System/SysMain.c \
    SrcCode/System/SysMain_Exe.c \
    SrcCode/System/SysMain_Flow_Init.c \
    SrcCode/System/SysMain_Flow_Exit.c \
    SrcCode/System/SysMain_Flow_Mode.c \
    SrcCode/System/SysMain_Flow_Sleep.c \
    SrcCode/System/SysMain_State.c \
    SrcCode/System/SysLock.c \
    SrcCode/System/SysCmd_CB.c \
    SrcCode/System/SysOutput_Exe.c \
    SrcCode/System/SysVideo_Exe.c \
    SrcCode/System/SysVideo_CB.c \
    SrcCode/System/SysAudio_Exe.c \
    SrcCode/System/SysAudio_CB.c \
    SrcCode/System/SysUsb_Exe.c \
    SrcCode/System/SysUsb_CB.c \
    SrcCode/System/SysStrg_Exe.c \
    SrcCode/System/SysStrg_CB.c \
    SrcCode/System/SysInput_Exe.c \
    SrcCode/System/SysInput_CB.c \
    SrcCode/System/SysPower_Exe.c \
    SrcCode/System/SysPower_CB.c \
    SrcCode/System/SysSensor_Exe.c \
    SrcCode/System/SysSensor_CB.c \
    SrcCode/System/SysLens_Exe.c  \
    SrcCode/System/SysLens_CB.c  \
    SrcCode/System/SysIpc_Exe.c \
    SrcCode/System/SysUctrl_Exe.c \
    SrcCode/System/SysBT.c \
    SrcCode/ProjectInfo.c \
    SrcCode/UI/SysUICB.c \
    SrcCode/UI/UIDisplay.c \
    SrcCode/UI/UIGraphics.c \
    SrcCode/UI/UIGraphicsID.c \
    SrcCode/UI/UIView.c \
    SrcCode/UI/PlaybackView.c \
    SrcCode/UI/PhotoView.c \
    SrcCode/UI/MovieView.c \
    SrcCode/UI/UIBuffer.c \
    SrcCode/UIApp/DpofVendor.c  \
    SrcCode/UIApp/ExifVendor.c  \
    SrcCode/UIApp/MovieUdtaVendor.c \
    SrcCode/UIApp/MovieStamp/MovieStamp.c \
    SrcCode/UIApp/MovieStamp/MovieStampID.c \
    SrcCode/UIApp/MovieStamp/MovieStampTsk.c \
    SrcCode/UIApp/MovieStamp/DateStampFont10x16.c \
    SrcCode/UIApp/MovieStamp/DateStampFont12x20.c \
    SrcCode/UIApp/MovieStamp/DateStampFont18x30.c \
    SrcCode/UIApp/MovieStamp/DateStampFont20x44.c \
    SrcCode/UIApp/MovieStamp/DateStampFont26x44.c \
    SrcCode/UIApp/MovieStamp/UniFont.c \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb.c \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_Photo.c  \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_Adj.c  \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_UpdFw.c  \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_CustomSi.c \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_IQSim.c \
    SrcCode/UIApp/UsbMsdcCb/MsdcNvtCb_TimeSi.c \
    SrcCode/UIApp/UserCmd/UserCmd.c \
    SrcCode/UIApp/UserCmd/UserCmdID.c \

ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/UIApp/AppDisp_PipView.c \
    SrcCode/UIApp/AppDisp_SbsView.c \
    SrcCode/UIApp/AppDisp_RotateView.c \
    SrcCode/UIApp/AppDisp_ScaleView.c \
    SrcCode/UIApp/AppDisp_3DNRView.c
endif

ifeq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/UIApp/AppDisp_IPCamView.c \

endif

################### UIMode ######################
SRC += \
    SrcCode/Mode/UIMode.c \
    SrcCode/Mode/UIModeMain.c  \
    SrcCode/Mode/UIModeSleep.c \
    SrcCode/Mode/UIModeWifi.c \

################### UIWnd ######################
ifeq "$(UI_STYLE)" "UI_STYLE_CARDV"
SRC += \
    SrcCode/UIWnd/CARDV/FlowCommon.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuCommon/MenuCommonItem.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuCommon/MenuCommonOption.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuCommon/TabMenu.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuMovie/MenuMovie.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPhoto/MenuPhoto.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPhoto/UIMenuWndPhotoExposure.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPhoto/UIMenuWndPhotoWB.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPhoto/UIMenuWndPhotoColor.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPlay/MenuPlayback.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPlay/UIMenuWndPlayDel.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPlay/UIMenuWndPlayConfirmDel.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShow.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShowCB.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuSetup/MenuSetup.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuSetup/UIMenuWndSetupDateTime.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuSetup/UIMenuWndSetupDefaultSetting.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuSetup/UIMenuWndSetupFormat.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuSetup/UIMenuWndSetupFormatConfirm.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuSetup/UIMenuWndSetupVersion.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuWiFi/UIMenuWndWiFiWait.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuWiFi/UIMenuWndWiFiModuleLink.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuWiFi/UIMenuWndWiFiMobileLinkOK.c \
    SrcCode/UIWnd/CARDV/UIMenu/UIMenuUSB/UIMenuWndUSB.c \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowCommon/UIFlowWndWrnMsg.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowCommon/UIFlowWndWaitMoment.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowCommon/UIFlowWndUSB.c \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPhoto/UIFlowWndPhoto.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPhoto/UIFlowPhotoFuncs.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPhoto/UIFlowPhotoIcons.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowMovie/UIFlowWndMovie.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowMovie/UIFlowMovieFuncs.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowMovie/UIFlowMovieIcons.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPlay/UIFlowWndPlay.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPlay/UIFlowWndPlayThumb.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPlay/UIFlowWndPlayMagnify.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPlay/UIFlowCBSlideShow.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPlay/UIFlowPlayIcons.c  \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPlay/UIFlowPlayFuncs.c \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowWiFi/UIFlowWndWiFiPhoto.c \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowWiFi/UIFlowWndWiFiMovie.c \
    SrcCode/UIWnd/CARDV/UIFlow/UIFlowPhoto/UIFlowPhotoParams.c   \
    SrcCode/UIWnd/CARDV/Engineer/CalibrationWnd.c \
    SrcCode/UIWnd/CARDV/Engineer/Engineer_ID.c \
    SrcCode/UIWnd/CARDV/Engineer/EngineerModeTbl.c \
    SrcCode/UIWnd/CARDV/Engineer/DynamicMenu.c \
    SrcCode/UIWnd/CARDV/Engineer/CalSensor/Cal_Header.c \
    SrcCode/UIWnd/CARDV/Engineer/CalSensor/CalibrationItem.c \
    SrcCode/UIWnd/CARDV/Engineer/CalSensor/UI_Sample/IPL_Cal_KeyStrSample.c

else ifeq "$(UI_STYLE)" "UI_STYLE_SPORTCAM"
SRC += \
    SrcCode/UIWnd/SPORTCAM/FlowCommon.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuCommon/MenuCommonItem.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuCommon/MenuCommonOption.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuCommon/MenuCommonConfirm.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuCommon/TabMenu.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuCommon/MenuCommon.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuCommon/MenuMode.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuMovie/MenuMovie.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuPhoto/MenuPhoto.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuPlay/MenuPlayback.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuSetup/MenuSetup.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuSetup/MenuExtSetup.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuSetup/UIMenuWndSetupDateTime.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuWiFi/UIMenuWndWiFiWait.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuWiFi/UIMenuWndWiFiModuleLink.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuWiFi/UIMenuWndWiFiMobileLinkOK.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuUSB/UIMenuWndUSB.c \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowCommon/UIFlowWndWrnMsg.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowCommon/UIFlowWndWaitMoment.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowCommon/UIFlowWndUSB.c \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPhoto/UIFlowWndPhoto.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPhoto/UIFlowPhotoFuncs.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPhoto/UIFlowPhotoIcons.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPhoto/UIFlowPhotoParams.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowMovie/UIFlowWndMovie.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowMovie/UIFlowMovieFuncs.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowMovie/UIFlowMovieIcons.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPlay/UIFlowWndPlay.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPlay/UIFlowWndPlayThumb.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPlay/UIFlowWndPlayMagnify.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPlay/UIFlowCBSlideShow.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPlay/UIFlowPlayIcons.c  \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPlay/UIFlowPlayFuncs.c \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowWiFi/UIFlowWndWiFiPhoto.c \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowWiFi/UIFlowWndWiFiMovie.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/CalibrationWnd.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/Engineer_ID.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/EngineerModeTbl.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/DynamicMenu.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/CalSensor/Cal_Header.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/CalSensor/CalibrationItem.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/CalSensor/UI_Sample/IPL_Cal_KeyStrSample.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/BurnIn/BurnInWnd.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/BurnIn/KeyTestWnd.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/BurnIn/MovieRecTestWnd.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/BurnIn/MoviePlayTestWnd.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/BurnIn/LCDTestWnd.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/BurnIn/LEDTestWnd.c \
    SrcCode/UIWnd/SPORTCAM/Engineer/BurnIn/AudioTestWnd.c

else ifeq "$(UI_STYLE)" "UI_STYLE_TOUCH"
SRC += \
    SrcCode/UIWnd/TOUCHUI/FlowCommon.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuCommon/MenuCommonItem.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuCommon/MenuCommonOption.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuCommon/MenuCommonConfirm.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuCommon/TabMenu.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuCommon/MenuCommon.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuCommon/MenuMode.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuMovie/MenuMovie.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuPhoto/MenuPhoto.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuPlay/MenuPlayback.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuSetup/MenuSetup.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuSetup/MenuExtSetup.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuSetup/UIMenuWndSetupDateTime.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuWiFi/UIMenuWndWiFiWait.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuWiFi/UIMenuWndWiFiModuleLink.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuWiFi/UIMenuWndWiFiMobileLinkOK.c \
    SrcCode/UIWnd/TOUCHUI/UIMenu/UIMenuUSB/UIMenuWndUSB.c \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowCommon/UIFlowWndWrnMsg.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowCommon/UIFlowWndWaitMoment.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowCommon/UIFlowWndUSB.c \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPhoto/UIFlowWndPhoto.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPhoto/UIFlowPhotoFuncs.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPhoto/UIFlowPhotoIcons.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPhoto/UIFlowPhotoParams.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowMovie/UIFlowWndMovie.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowMovie/UIFlowMovieFuncs.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowMovie/UIFlowMovieIcons.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPlay/UIFlowWndPlay.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPlay/UIFlowWndPlayThumb.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPlay/UIFlowWndPlayMagnify.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPlay/UIFlowCBSlideShow.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPlay/UIFlowPlayIcons.c  \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowPlay/UIFlowPlayFuncs.c \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowWiFi/UIFlowWndWiFiPhoto.c \
    SrcCode/UIWnd/TOUCHUI/UIFlow/UIFlowWiFi/UIFlowWndWiFiMovie.c \
    SrcCode/UIWnd/TOUCHUI/Engineer/CalibrationWnd.c \
    SrcCode/UIWnd/TOUCHUI/Engineer/Engineer_ID.c \
    SrcCode/UIWnd/TOUCHUI/Engineer/EngineerModeTbl.c \
    SrcCode/UIWnd/TOUCHUI/Engineer/DynamicMenu.c \
    SrcCode/UIWnd/TOUCHUI/Engineer/CalSensor/Cal_Header.c \
    SrcCode/UIWnd/TOUCHUI/Engineer/CalSensor/CalibrationItem.c \
    SrcCode/UIWnd/TOUCHUI/Engineer/CalSensor/UI_Sample/IPL_Cal_KeyStrSample.c
else
SRC += \
    SrcCode/UIWnd/IPCAM/FlowCommon.c \
    SrcCode/UIWnd/IPCAM/FlowIPCam.c  \
    SrcCode/UIWnd/IPCAM/UIFlow/UIFlowMovie/UIFlowWndMovie.c  \
    SrcCode/UIWnd/IPCAM/UIFlow/UIFlowMovie/UIFlowMovieFuncs.c  \
    SrcCode/UIWnd/IPCAM/UIFlow/UIFlowMovie/UIFlowMovieIcons.c  \
    SrcCode/UIWnd/IPCAM/Engineer/CalibrationWnd.c \
    SrcCode/UIWnd/IPCAM/Engineer/Engineer_ID.c \
    SrcCode/UIWnd/IPCAM/Engineer/EngineerModeTbl.c \
    SrcCode/UIWnd/IPCAM/Engineer/DynamicMenu.c \
    SrcCode/UIWnd/IPCAM/Engineer/CalSensor/Cal_Header.c \
    SrcCode/UIWnd/IPCAM/Engineer/CalSensor/CalibrationItem.c \
    SrcCode/UIWnd/IPCAM/Engineer/CalSensor/UI_Sample/IPL_Cal_KeyStrSample.c

endif
################### UIInfo ######################
ifeq "$(UI_STYLE)" "UI_STYLE_CARDV"
SRC += \
    SrcCode/UIWnd/CARDV/UIInfo/UIMovieMapping.c \
    SrcCode/UIWnd/CARDV/UIInfo/UIPhotoMapping.c \
    SrcCode/UIWnd/CARDV/UIInfo/UIInfo.c \
    SrcCode/UIWnd/CARDV/UIInfo/DateTimeInfo.c \
    SrcCode/UIWnd/CARDV/UIInfo/GSInfo.c \
    SrcCode/UIWnd/CARDV/UIInfo/LensInfo.c

else ifeq "$(UI_STYLE)" "UI_STYLE_SPORTCAM"
    SRC += \
    SrcCode/UIWnd/SPORTCAM/UIInfo/UIMovieMapping.c \
    SrcCode/UIWnd/SPORTCAM/UIInfo/UIPhotoMapping.c \
    SrcCode/UIWnd/SPORTCAM/UIInfo/UIInfo.c \
    SrcCode/UIWnd/SPORTCAM/UIInfo/DateTimeInfo.c \
    SrcCode/UIWnd/SPORTCAM/UIInfo/GSInfo.c \
    SrcCode/UIWnd/SPORTCAM/UIInfo/LensInfo.c

else ifeq "$(UI_STYLE)" "UI_STYLE_TOUCH"
    SRC += \
    SrcCode/UIWnd/TOUCHUI/UIInfo/UIMovieMapping.c \
    SrcCode/UIWnd/TOUCHUI/UIInfo/UIPhotoMapping.c \
    SrcCode/UIWnd/TOUCHUI/UIInfo/UIInfo.c \
    SrcCode/UIWnd/TOUCHUI/UIInfo/DateTimeInfo.c \
    SrcCode/UIWnd/TOUCHUI/UIInfo/GSInfo.c \
    SrcCode/UIWnd/TOUCHUI/UIInfo/LensInfo.c

else
SRC += \
    SrcCode/UIWnd/IPCAM/UIInfo/UIMovieMapping.c \
    SrcCode/UIWnd/IPCAM/UIInfo/UIPhotoMapping.c \
    SrcCode/UIWnd/IPCAM/UIInfo/UIInfo.c \
    SrcCode/UIWnd/IPCAM/UIInfo/DateTimeInfo.c \
    SrcCode/UIWnd/IPCAM/UIInfo/GSInfo.c \
    SrcCode/UIWnd/IPCAM/UIInfo/LensInfo.c

endif
################### UIApp ######################
SRC += \
    SrcCode/UIApp/UIAppObj.c \

################### Network ######################
SRC += \
    SrcCode/UIApp/Network/UIAppNetwork.c \
    SrcCode/UIApp/Network/WifiAppXML.c \
    SrcCode/UIApp/Network/WifiAppCmd.c \
    SrcCode/UIApp/Network/WifiAppCmdMapping.c \
    SrcCode/UIApp/Network/NetFs.c \

ifeq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/UIApp/Network/NetAudQueID.c \
    SrcCode/UIApp/Network/NetAudQue.c \

endif

ifeq ($(strip $(YQCONFIG_ANDROID_SYSTEM_SUPPORT)),true)
SRC += \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuPlay/UIMenuWndPlayDel.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuPlay/UIMenuWndPlayConfirmDel.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShow.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuPlay/UIMenuWndPlaySlideShowCB.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuSetup/UIMenuWndSetupDefaultSetting.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuSetup/UIMenuWndSetupFormat.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuSetup/UIMenuWndSetupFormatConfirm.c \
    SrcCode/UIWnd/SPORTCAM/UIMenu/UIMenuSetup/UIMenuWndSetupVersion.c \
    SrcCode/UIWnd/SPORTCAM/UIFlow/UIFlowPlay/UIFlowWndPlayFileList.c 
endif


################### Movie ######################
SRC += \
    SrcCode/Mode/UIModeMovie.c \
    SrcCode/UIApp/Movie/UIAppMovie_Exe.c \
    SrcCode/UIApp/Movie/UIAppMovie_CB.c \
    SrcCode/UIApp/Movie/UIAppMovie_GetFreeRecSec.c \
    SrcCode/UIApp/Movie/UIAppMovie_SetUserData.c \
    SrcCode/UIApp/Movie/UIAppMovie_3DNR.c \
    SrcCode/UIApp/Movie/UIAppMovie_AudNR_Spec_Zoom.c \
    SrcCode/UIApp/Movie/UIAppMovie_AudNR_Spec_Focus.c \
    SrcCode/UIApp/Movie/UIAppMovie_AudNR_Spec_Iris.c \
    SrcCode/UIApp/Movie/UIAppMovie_CvHostEx.c \
    SrcCode/UIApp/Movie/UIAppMovie_NvtCvEx.c \
    SrcCode/UIApp/Movie/UIAppMovie_TimelapseLPR.c \

################### Photo ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/Mode/UIModePhoto.c \
    SrcCode/UIApp/Photo/UIAppPhoto_Exe.c \
    SrcCode/UIApp/Photo/UIAppPhoto_CB.c \
    SrcCode/UIApp/Photo/UIDateImprint.c \
    SrcCode/UIApp/Photo/UIStorageCheck.c \
    SrcCode/UIApp/Photo/UIPhotoFuncInfo.c \

endif
ifeq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/UIApp/Photo/UIStorageCheck.c \
    SrcCode/UIApp/Photo/UIPhotoFuncInfo.c \

endif

################### Play ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/Mode/UIModePlayback.c \
    SrcCode/UIApp/Play/UIAppPlay_Exe.c \
    SrcCode/UIApp/Play/UIAppPlay_CB.c \
    SrcCode/UIApp/Play/UIAppMoviePlay_Exe.c \
    SrcCode/UIApp/Play/UIPlaySlide.c \
    SrcCode/UIApp/Play/UIPlayWallpaper.c \
    SrcCode/UIApp/Play/UIPlayComm.c \

endif

################### Setup ######################
SRC += \
    SrcCode/UIApp/Setup/UISetup_Exe.c \

################### Background ######################
SRC += \
    SrcCode/UIApp/Background/UIBackgroundObj.c \

################### UsbCam ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/Mode/UIModeUsbCam.c \
    SrcCode/UIApp/UsbCam/UIAppUsbCam_Exe.c \

endif

################### UsbCommand ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/UIApp/UsbCommand/UsbCmd.c \
    SrcCode/UIApp/UsbCommand/UsbCmdID.c \
    SrcCode/UIApp/UsbCommand/UsbCmdTsk.c \

endif

################### UsbDisk ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/Mode/UIModeUsbDisk.c \
    SrcCode/UIApp/UsbDisk/UIAppUsbDisk_Exe.c \

endif

################### UsbPrint ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/UIApp/UsbPrint/UIAppUsbPrint_Exe.c \

endif

################### Youku ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/UIApp/Youku/YoukuAppCmd.c \
    SrcCode/UIApp/Youku/StatusData.c \

endif

################### UsbPTP ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/Mode/UIModeUsbPTP.c \

endif

################### UsbCharge ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/Mode/UIModeUsbCharge.c \
    SrcCode/UIApp/UsbCharge/UIAppUsbCharge_Exe.c \

endif

################### UsbMenu ######################
ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/Mode/UIModeUsbMenu.c \

endif

################### UCtrlApp ######################
SRC += \
    SrcCode/UCtrlApp/Main/UCtrlMain.c \
    SrcCode/UCtrlApp/Main/UCtrlMainID.c \
    SrcCode/UCtrlApp/Mode/UCtrlMode.c \
    SrcCode/UCtrlApp/Play/UCtrlAppPlay.c \
    SrcCode/UCtrlApp/Setup/UCtrlAppSetup.c \
    SrcCode/UCtrlApp/System/UCtrlAppSystem.c \
    SrcCode/UCtrlApp/Movie/UCtrlAppMovie.c \
    SrcCode/UCtrlApp/Onvif/UCtrlAppOnvif.c \
    SrcCode/UCtrlApp/Onvif/UCtrlAppOnvifID.c\
    SrcCode/UCtrlApp/Image/UCtrlAppImage.c \
    SrcCode/UCtrlApp/Event/UCtrlAppEvent.c \
    SrcCode/UCtrlApp/IQtool/UCtrlAppIQtool.c \
    

ifeq ($(strip $(YQCONFIG_UCTRL_CUSTOM_SUPPORT)), true)
    SRC += SrcCode/UCtrlApp/Custom/UCtrlAppCustom.c 
endif

ifneq "$(UI_STYLE)" "UI_STYLE_IPCAM"
SRC += \
    SrcCode/UCtrlApp/Photo/UCtrlAppPhoto.c \
    SrcCode/UCtrlApp/Photo/UCtrlAppPhotoID.c \

endif


################### Resource ######################
ifeq "$(UI_STYLE)" "UI_STYLE_CARDV"
SRC += \
    SrcCode/UIWnd/CARDV/Resource/UIResource.c \
    SrcCode/UIWnd/CARDV/Resource/BG_Images.c \
    SrcCode/UIWnd/CARDV/Resource/BG_Opening.c \
    SrcCode/UIWnd/CARDV/Resource/BG_Watermark.c \
    SrcCode/UIWnd/CARDV/Resource/Demo_BigFont.c \
    SrcCode/UIWnd/CARDV/Resource/Demo_SmallFont.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_Font.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_Image.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_Palette.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_EN.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_DE.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_ES.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_PO.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_IT.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_FR.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_SC.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_TC.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_JP.c \
    SrcCode/UIWnd/CARDV/Resource/DemoKit_String_RU.c \
    SrcCode/UIWnd/CARDV/Resource/SoundData.c

else ifeq "$(UI_STYLE)" "UI_STYLE_SPORTCAM"
SRC += \
    SrcCode/UIWnd/SPORTCAM/Resource/UIResource.c \
    SrcCode/UIWnd/SPORTCAM/Resource/BG_Images.c \
    SrcCode/UIWnd/SPORTCAM/Resource/BG_Opening.c \
    SrcCode/UIWnd/SPORTCAM/Resource/BG_Watermark.c \
    SrcCode/UIWnd/SPORTCAM/Resource/Demo_BigFont.c \
    SrcCode/UIWnd/SPORTCAM/Resource/Demo_SmallFont.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_Font.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_Image.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_Palette.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_PaletteOption.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_PaletteConfirm.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_EN.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_DE.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_ES.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_PO.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_IT.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_FR.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_SC.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_TC.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_JP.c \
    SrcCode/UIWnd/SPORTCAM/Resource/DemoKit_String_RU.c \
    SrcCode/UIWnd/SPORTCAM/Resource/SoundData.c

else ifeq "$(UI_STYLE)" "UI_STYLE_TOUCH"
SRC += \
    SrcCode/UIWnd/TOUCHUI/Resource/UIResource.c \
    SrcCode/UIWnd/TOUCHUI/Resource/UIResource2.c \
    SrcCode/UIWnd/TOUCHUI/Resource/BG_Images.c \
    SrcCode/UIWnd/TOUCHUI/Resource/BG_Opening.c \
    SrcCode/UIWnd/TOUCHUI/Resource/BG_Watermark.c \
    SrcCode/UIWnd/TOUCHUI/Resource/Demo_BigFont.c \
    SrcCode/UIWnd/TOUCHUI/Resource/Demo_SmallFont.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_Font.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_Image.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_Palette.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_PaletteOption.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_PaletteConfirm.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_EN.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_DE.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_ES.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_PO.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_IT.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_FR.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_SC.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_TC.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_JP.c \
    SrcCode/UIWnd/TOUCHUI/Resource/DemoKit_String_RU.c \
    SrcCode/UIWnd/TOUCHUI/Resource/SoundData.c

else
SRC += \
    SrcCode/UIWnd/IPCAM/Resource/UIResource.c \
    SrcCode/UIWnd/IPCAM/Resource/BG_Opening.c \
    SrcCode/UIWnd/IPCAM/Resource/BG_Watermark.c \
    SrcCode/UIWnd/IPCAM/Resource/Demo_BigFont.c \
    SrcCode/UIWnd/IPCAM/Resource/Demo_SmallFont.c \
    SrcCode/UIWnd/IPCAM/Resource/DemoKit_Font.c \
    SrcCode/UIWnd/IPCAM/Resource/DemoKit_Palette.c \
    SrcCode/UIWnd/IPCAM/Resource/SoundData.c

endif

#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = $(PRJ_NAME)_Data

#----------------------------------------------------------------------
# set the library directory here
#----------------------------------------------------------------------
ARC_BASE_DIR = $(ROOT_DIR)/ARC

#----------------------------------------------------------------------
# add additional C and assembly flags here, for example, -Otime
#----------------------------------------------------------------------
CFLAGS_D = $(C_DEFINE_EXT)

ASMFLAGS_D = \
    $(CHIP_PARAM_ASM)   \
    $(FWLOAD_PARAM_ASM)

CFLAGS_R = $(C_DEFINE_EXT)

ASMFLAGS_R = \
    $(CHIP_PARAM_ASM)   \
    $(FWLOAD_PARAM_ASM)

GCCFLAGS = $(C_DEFINE_EXT)

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
include $(MAKE_COMMON_DIR)/OutputImg.txt
