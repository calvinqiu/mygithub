
ifeq "$(OPTION_LOADED)" ""
include ../MakeCommon/MakeOption.txt
endif

#xc modify for cust compile @20170207 start
ifeq ($(strip $(CUST_COMPILE_SUPPORT)), TRUE)
include ../resource/YqFeatures.mk
include ../Project/DemoKit/ModelConfig.txt
include ../Project/DemoKit/ModelConfig_$(MODEL).txt

CUST_LIB_MAKEFILE :=  \
	./LibExt_src/PluginPhoto/ADAS/ \
	./LibExt_src/DevLens/Lens_Sample/ \
	./LibExt_src/PluginPhoto/AutoVP_Sample/ \
	./LibExt_src/PluginPhoto/MD_Sample2/ \
	./LibExt_src/PluginPhoto/SnG_Parms/ \
	./LibExt_src/PluginWifi/WifiCmdParser/

ifneq ($(strip $(IPP)), IPL_FAKE)
CUST_LIB_MAKEFILE += $(wildcard ./LibExt_src/DevCamCAL/$(strip $(IPP))_CAL_DATA/)
CUST_LIB_MAKEFILE += $(wildcard ./LibExt_src/DevCamCAL/$(strip $(IPP))_CAL_ITEM/)
CUST_LIB_MAKEFILE += ./LibExt_src/DevCamIPL/$(strip $(IPP))/
endif

ifneq ($(strip $(AE)), AE_SAMPLE)
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/$(strip $(AE))/
else
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/AE_Sample/
endif

ifneq ($(strip $(AWB)), AWB_SAMPLE)
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/$(strip $(AWB))/
else
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/AWB_Sample/
endif

ifneq ($(strip $(AF)), AF_SAMPLE)
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/$(strip $(AF))/
else
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/AF_Sample/
endif

ifneq ($(strip $(CAF)), CAF_SAMPLE)
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/$(strip $(CAF))/
else
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/CAF_Sample/
endif

ifneq ($(strip $(FLASHLIGHT)), FLASHLIGHT_SAMPLE)
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/$(strip $(FLASHLIGHT))/
else
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/Flash_Sample/
endif

ifneq ($(strip $(MD)), MD_SAMPLE)
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/$(strip $(MD))/
else
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/MD_Sample/
endif

ifneq ($(strip $(IQUC)), IQUC_SAMPLE)
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/$(strip $(IQUC))/
else
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/IQUC_Sample/
endif

ifneq ($(strip $(RSC)), RSC_SAMPLE)
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/$(strip $(RSC))/
else
CUST_LIB_MAKEFILE += ./LibExt_src/PluginPhoto/RSC_Sample/
endif

mk_all := $(sort $(CUST_LIB_MAKEFILE))

else	#CUST_COMPILE_SUPPORT
	
ifeq "$(shell uname)" "Linux"
MAKEFILE_SEARCH := $(sort $(dir $(shell find . -name 'Makefile')))
else
MAKEFILE_SEARCH := $(sort $(dir $(shell find . -name \'Makefile\')))
endif
mk_all := $(filter-out ./, $(MAKEFILE_SEARCH))

endif	#CUST_COMPILE_SUPPORT
#xc modify for cust compile @20170207 start

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
LIB_R = $(addprefix "../ARC/Lib/Release/", $(addsuffix ".a", $(PRJ_NAME_ALL)))
LIB_D = $(addprefix "../ARC/Lib/Debug/", $(addsuffix "_D.a", $(PRJ_NAME_ALL)))

clean:
	@-echo Clean LibExt ... \
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
	@-echo Clean LibExt lint data ... \
	&& find ./ -type d -name 'CheckData' | xargs rm -rf --no-preserve-root lint*.txt
