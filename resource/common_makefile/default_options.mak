YQCONFIG_PLAT_VERSION=ALPS.MT6753.L1C.MP3.V2.62_P1
YQCONFIG_SW_ENGINEER_BUILD_VERSION={YQCONFIG_SW_ENGINEER_VERSION}.{YQCONFIG_COMPILEMODE}.{YQCONFIG_PLAT_VERSION}
YQCONFIG_SW_USER_BUILD_VERSION=[YQCONFIG_SW_USER_VERSION]

#if using SMARTPA, close MTK platform speaker effect
if YQCONFIG_AUDIO_MIC_SWITCH_SUPPORT = no
YQCONFIG_TOUCH_AUDIOSWITCH_OPTION=no
else
YQCONFIG_TOUCH_AUDIOSWITCH_OPTION=no
endif