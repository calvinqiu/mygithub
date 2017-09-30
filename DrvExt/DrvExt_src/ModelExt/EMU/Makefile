include ../../../../Project/ProjectConfig.txt
include ../../../../Project/$(ACTIVE_PROJECT)/ModelConfig.txt
include ../../../../Project/$(ACTIVE_PROJECT)/ModelConfig_$(MODEL).txt
include ../../../../Project/$(ACTIVE_PROJECT)/MakeConfig.txt

#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = MODELEXT_EMU

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------
ifeq "$(ACTIVE_PROJECT)" "EmuKit"
SRC = \
    DxCfg.c \
    IntDirCfg.c \
    PinmuxCfg.c \
    DxStorage_Card.c \
    DxStorage_EmbMem.c \
    DxStorage_Ramdisk.c \
    PartitionInfo.c \
    IOCfg.c

ASM =
endif
#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = . \
    ../../../../Include

#----------------------------------------------------------------------
# set the image output directory here
#----------------------------------------------------------------------
IMG_BASE_DIR = ../../../../ARC/Drv

#----------------------------------------------------------------------
# add additional C and assembly flags here
#----------------------------------------------------------------------
CFLAGS_D = $(C_DEFINE_EXT)

ASMFLAGS_D = \
    $(CHIP_PARAM_ASM) \
    $(DRAMSIZE_PARAM_ASM) \
    $(MODEL_PARAM_ASM)

CFLAGS_R = $(C_DEFINE_EXT)

ASMFLAGS_R = \
    $(CHIP_PARAM_ASM) \
    $(DRAMSIZE_PARAM_ASM) \
    $(MODEL_PARAM_ASM)

GCCFLAGS = $(C_DEFINE_EXT)

#----------------------------------------------------------------------
# include common parts of the makefile
#----------------------------------------------------------------------
MAKE_COMMON_DIR = ../../../../MakeCommon
include $(MAKE_COMMON_DIR)/OutputLib.txt
