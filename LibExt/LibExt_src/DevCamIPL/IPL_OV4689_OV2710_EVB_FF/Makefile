
#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = IPL_OV4689_OV2710_EVB_FF

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------
SRC = \
    IPL_Cmd_OV4689_OV2710_EVB_FF.c \
    IPL_Mode_OV4689_OV2710_EVB_FF.c \
    IPL_Isr_OV4689_OV2710_EVB_FF.c \
    IPL_Buf_OV4689_OV2710_EVB_FF.c \
    IPL_dzoomTab_OV4689_OV2710_EVB_FF.c \
    IPL_Ctrl_OV4689_OV2710_EVB_FF.c \
    IPL_CtrlNormal_OV4689_OV2710_EVB_FF.c \
    IPL_CtrlFW_OV4689_OV2710_EVB_FF.c \
	IPL_CtrlDeWarp_OV4689_OV2710_EVB_FF.c \
    IPL_D2DCB_OV4689_OV2710_EVB_FF.c \
    IPL_FlowCB_OV4689_OV2710_EVB_FF.c \
    IPL_ExtFunc_OV4689_OV2710_EVB_FF.c \
    IQS_SettingFlow_OV4689_OV2710_EVB_FF.c \
    IQS_SettingFunc_OV4689_OV2710_EVB_FF.c \
    IQS_SettingTable_OV4689_OV2710_EVB_FF.c \
    IQS_ImageEffectTable_OV4689_OV2710_EVB_FF.c \
    IQS_TuningIQFlow_OV4689_OV2710_EVB_FF.c \
    IQS_TuningIQTable_OV4689_OV2710_EVB_FF.c

ASM =

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = . \
	../../../../Include \
	../../DevCamCAL \
	../../../../Lib/Lib_src/DevCam/ipl_common \
	../../../../Lib/Lib_src/DevCam/iqs_common \

#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = ../../../../ARC/Lib

#----------------------------------------------------------------------
# add additional C and assembly flags here
#----------------------------------------------------------------------
CFLAGS_D = -D_IPL_COM_INT_INC
ASMFLAGS_D =

CFLAGS_R = -D_IPL_COM_INT_INC
ASMFLAGS_R =

GCCFLAGS = -D_IPL_COM_INT_INC

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
MAKE_COMMON_DIR = ../../../../MakeCommon
include $(MAKE_COMMON_DIR)/OutputLib.txt
