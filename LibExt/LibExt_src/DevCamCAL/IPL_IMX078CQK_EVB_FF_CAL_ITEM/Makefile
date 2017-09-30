
#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = IPL_IMX078CQK_EVB_FF_CAL_ITEM

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------
SRC = \
	IPL_Cal_Ctrl_IMX078CQK_EVB_FF.c \
	IPL_Cal_Item_IMX078CQK_EVB_FF.c \
	IPL_Cal_Setting_IMX078CQK_EVB_FF.c \

ASM =

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = . \
	../../../../Include \
	../../../../Lib/Lib_src/DevCam/ipl_calibration \
	../../../../Lib/Lib_src/DevCam/ipl_common \
	../ \

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
