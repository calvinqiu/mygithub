include ../../../../Project/ProjectConfig.txt
include ../../../../Project/$(ACTIVE_PROJECT)/ModelConfig.txt
include ../../../../Project/$(ACTIVE_PROJECT)/ModelConfig_$(MODEL).txt
include ../../../../Project/$(ACTIVE_PROJECT)/MakeConfig.txt

#----------------------------------------------------------------------
# set the library name here
#----------------------------------------------------------------------
PRJ_NAME = MODELEXT_PWA6

#----------------------------------------------------------------------
# add/delete source files here
#----------------------------------------------------------------------
ifeq ($(ACTIVE_PROJECT), $(filter $(ACTIVE_PROJECT),DemoKit TestKit))
SRC = \
    DxCfg.c \
    IntDirCfg.c \
    PinmuxCfg.c \
    IOCfg.c \
    DxStorage_Card.c \
    DxStorage_EmbMem.c \
    DxDisplay_LCD.c \
    DxDisplay_LCD2.c \
    DxDisplay_TV.c \
    DxDisplay_HDMI.c \
    DxUsb.c \
	DxWiFi.c \
    DxInput_Key.c \
    DxCamera_Sensor.c \
    DxCamera_Lens.c \
    DxCamera_Flash.c \
    DxSound_Audio.c \
    DxPower_Battery.c \
    DxOutput_LED.c \
    DxCmd.c \
    PartitionInfo.c

ASM =
endif
#----------------------------------------------------------------------
# set the include directory here
#----------------------------------------------------------------------
INC_DIR = . \
	../../../../Include \

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
