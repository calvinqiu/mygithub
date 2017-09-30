include ../MakeCommon/MakeOption.txt

#xc modify for cust compile @20170207 start
ifeq ($(strip $(CUST_COMPILE_SUPPORT)), TRUE)
include ../Project/DemoKit/ModelConfig.txt
include ../resource/YqFeatures.mk
include ../Project/DemoKit/ModelConfig_$(MODEL).txt

CUST_MAKEFILE :=  \
	./DrvExt_src/Audio/Audio_ExtAIC3254/ \
	./DrvExt_src/Audio/Audio_ExtWM8978/ \
	./DrvExt_src/BlueTooth/Broadcom/ \
	./DrvExt_src/DispDevPanel/Disp_PANELCOMMON/ \
	./DrvExt_src/ModelExt/$(strip $(MODEL))/ \
	./DrvExt_src/SampleDrv/

ifneq ($(strip $(HDMI)), OFF)
CUST_MAKEFILE += ./DrvExt_src/DispDevHDMI/Disp_HDMI/
endif

ifneq ($(strip $(LCD)), Disp_VIRTUAL_LCD1_OFF)
CUST_MAKEFILE += ./DrvExt_src/DispDevPanel/$(strip $(LCD))/
else
CUST_MAKEFILE += ./DrvExt_src/DispDevPanel/Disp_VIRTUAL_LCD1_OFF/	
endif

ifneq ($(strip $(LCD2)), Disp_VIRTUAL_LCD2_OFF)
CUST_MAKEFILE += ./DrvExt_src/DispDevPanel/$(strip $(LCD2))/
else
CUST_MAKEFILE += ./DrvExt_src/DispDevPanel/Disp_VIRTUAL_LCD2_OFF/
endif

ifneq ($(strip $(TV)), OFF)
CUST_MAKEFILE += ./DrvExt_src/DispDevTV/Disp_TV/
endif

ifneq ($(strip $(GPS)), GPS_NONE)
CUST_MAKEFILE += ./DrvExt_src/GPS/
endif

ifneq ($(strip $(GSENSOREXT)), GSensor_None)
CUST_MAKEFILE += ./DrvExt_src/GSensor/$(strip $(GSENSOREXT))/
else
CUST_MAKEFILE += ./DrvExt_src/GSensor/GSensor_None/
endif

ifneq ($(strip $(GYROEXT)), Gyro_None)
CUST_MAKEFILE += ./DrvExt_src/Gyro/$(strip $(GYROEXT))/
else
CUST_MAKEFILE += ./DrvExt_src/Gyro/Gyro_None/
endif

ifneq ($(strip $(IRRX)), IrRx_NONE)
CUST_MAKEFILE += ./DrvExt_src/IrRx/NEC_IRRM/
else
CUST_MAKEFILE += ./DrvExt_src/IrRx/DUMMY_IRRM/
endif

ifeq ($(strip $(LENSDRV)), LENSDRV_FF1)
CUST_MAKEFILE += ./DrvExt_src/Lens/$(strip $(LENSDRV))/
else
CUST_MAKEFILE += ./DrvExt_src/Lens/LensDrv_FF1/
endif

ifneq ($(strip $(LENSMOTOR)), LENSMOTOR_FF1)
CUST_MAKEFILE += ./DrvExt_src/Lens/$(strip $(LENSMOTOR))/
else
CUST_MAKEFILE += ./DrvExt_src/Lens/LensMotor_FF1/
endif

ifeq ($(strip $(YQCONFIG_DRIVE_EVENT_SUPPORT)), true)
CUST_MAKEFILE += ./DrvExt_src/DriveEvent
endif

#xc add for mtk and mcu uart @20170209 start
ifeq ($(strip $(YQCONFIG_UART_TO_MTK_SUPPORT)), true)
CUST_MAKEFILE += ./DrvExt_src/UART_TO_MTK
endif
#xc add for mtk and mcu uart @20170209 end

#xc add for mcu uart @20170210 start
ifeq ($(strip $(YQCONFIG_UART_TO_MCU_SUPPORT)), true)
CUST_MAKEFILE += ./DrvExt_src/UART_TO_MCU
endif
#xc add for mcu uart @20170210 end

#xc add for yunaedog @20170209 start
ifeq ($(strip $(YQCONFIG_YUANEDOG_SUPPORT)), true)
CUST_MAKEFILE += ./DrvExt_src/YuanEDog
endif
#xc add for yunaedog @20170209 end

mk_all := $(sort $(CUST_MAKEFILE))

else	#CUST_COMPILE_SUPPORT

ifeq "$(shell uname)" "Linux"
MAKEFILE_SEARCH := $(sort $(dir $(shell find . -name 'Makefile')))
else
MAKEFILE_SEARCH := $(sort $(dir $(shell find . -name \'Makefile\')))
endif
mk_all := $(filter-out ./, $(MAKEFILE_SEARCH))

endif #CUST_COMPILE_SUPPORT
#xc modify for cust compile @20170207 end

ifeq "$(ISOLATE_DEP)" "ON"
.PHONY: all $(mk_all) clean rebuild debug release dep

all:
	@make dep
	@make debug
	@make release
else
.PHONY: all $(mk_all) clean rebuild debug release

all: rm_log $(mk_all)
endif

$(mk_all):
	@$(MAKE) --directory=$@ $(MAKECMDGOALS)

rm_log:
ifeq "$(CLEAN_LOG)" "ON"
	@-rm -f log*.txt
endif

PRJ_NAME_ALL = $(shell find -name 'Makefile' -not -path "./Makefile" -print | xargs grep "PRJ_NAME" | sed 's/[[:digit:][:alpha:][:punct:]]*:PRJ_NAME[ ]*=[ ]*//g')
LIB_R = $(addprefix "../ARC/Drv/Release/", $(addsuffix ".a", $(PRJ_NAME_ALL)))
LIB_D = $(addprefix "../ARC/Drv/Debug/", $(addsuffix "_D.a", $(PRJ_NAME_ALL)))

clean:
	@-echo Clean DrvExt ... \
	&& find ./ -type d -name '*_Data' | xargs rm -rf --no-preserve-root $(LIB_D) $(LIB_R) log*.txt lint*.txt

ifeq "$(ISOLATE_DEP)" "ON"
rebuild:
	@make clean
	@make dep
	@make debug
	@make release

dep: rm_log $(mk_all)
else
rebuild: rm_log $(mk_all)
endif

debug: rm_log $(mk_all)

release: rm_log $(mk_all)

rm_lint_log:
	@-rm -f lint*.txt

lint: rm_lint_log $(mk_all)

lintclean:
	@-echo Clean DrvExt lint data ... \
	&& find ./ -type d -name 'CheckData' | xargs rm -rf --no-preserve-root lint*.txt
