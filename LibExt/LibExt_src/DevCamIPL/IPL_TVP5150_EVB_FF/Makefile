
#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = IPL_TVP5150_EVB_FF

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------
SRC = \
    IPL_Cmd_TVP5150_EVB_FF.c \
    IPL_Mode_TVP5150_EVB_FF.c \
    IPL_Isr_TVP5150_EVB_FF.c \
    IPL_Buf_TVP5150_EVB_FF.c \
    IPL_dzoomTab_TVP5150_EVB_FF.c \
    IPL_Ctrl_TVP5150_EVB_FF.c \
    IPL_CtrlNormal_TVP5150_EVB_FF.c \
    IPL_CtrlFW_TVP5150_EVB_FF.c \
	IPL_CtrlDeWarp_TVP5150_EVB_FF.c \
    IPL_D2DCB_TVP5150_EVB_FF.c \
    IPL_FlowCB_TVP5150_EVB_FF.c \
    IPL_ExtFunc_TVP5150_EVB_FF.c \
    IQS_SettingFlow_TVP5150_EVB_FF.c \
    IQS_SettingFunc_TVP5150_EVB_FF.c \
    IQS_SettingTable_TVP5150_EVB_FF.c \
    IQS_ImageEffectTable_TVP5150_EVB_FF.c \
    IQS_TuningIQFlow_TVP5150_EVB_FF.c \
    IQS_TuningIQTable_TVP5150_EVB_FF.c

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
