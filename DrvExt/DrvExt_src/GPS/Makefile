
#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = GPS

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------
PROVIDER_GENERAL := \
    General/GPS.c

PROVIDER_BROADCOM := \
    Dasheng/hal/hal.c \
    Dasheng/transport/transport_layer.c \
    Dasheng/utils/crc8bits.c \
    Dasheng/patch_reader/patch_reader.c \
    Dasheng/main.c \
    Dasheng/NvtDrv.c \
    Dasheng/NvtDrvID.c \
    Dasheng/patch.c

SRC = $(PROVIDER_BROADCOM) 

ASM =

#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = .. \
    ../../../Include \

#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = ../../../ARC/Drv

#----------------------------------------------------------------------
# add additional C and assembly flags here
#----------------------------------------------------------------------
CFLAGS_D =
ASMFLAGS_D =

CFLAGS_R =
ASMFLAGS_R =

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
MAKE_COMMON_DIR = ../../../MakeCommon
include $(MAKE_COMMON_DIR)/OutputLib.txt
