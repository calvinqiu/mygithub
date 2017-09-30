#
# Pre-defined Make Rules and Modules Specific Compiler Options
#
export CUST_COMPILE_SUPPORT = TRUE

export PROJECT_ROOT_DIR	= $(shell $(PERL) ./MakeCommon/perl_script/get_root_fullpath.pl)

include MakeCommon/MakeOption.txt

PROJECT ?= $(sort $(project) $(P) $(p))

MODULE ?= $(sort $(modules) $(M) $(m))
MODULES =

ifneq ($(filter drv DRV Drv, $(MODULE)),)
MODULES += ./DrvExt
endif

ifneq ($(filter lib LIB Lib libext LIBEXT LibExt, $(MODULE)),)
MODULES += ./LibExt
endif

ifneq ($(filter project PROJECT project, $(MODULE)),)
MODULES += ./Project/DemoKit
endif

$(shell $(PERL) ./MakeCommon/perl_script/yq_prebuild.pl $(PROJECT))

-include ./resource/YqFeatures.mk

ifeq ($(strip $(YQCONFIG_USE_FIX_PROJECT_SUPPORT)),true)
PROJECT := ALL
endif


export CUST_C_MACRO := $(foreach t,$(YQ_AUTO_ADD_GLOBAL_DEFINE_BY_NAME),$(if $(filter-out no NO none NONE false FALSE,$($(t))),-D$(t)))
CUST_C_MACRO += $(foreach t, $(YQ_AUTO_ADD_GLOBAL_DEFINE_BY_NUMBER), -D$(t)=$($(t)))
CUST_C_MACRO += $(foreach t, $(YQ_AUTO_ADD_GLOBAL_DEFINE_BY_STRING), -D$(t)='$($(t))')

ifneq ($(strip $(YQCONFIG_PLATFORM_NAME)),)
CUST_C_MACRO += -DYQCONFIG_PLATFORM_NAME='"$(YQCONFIG_PLATFORM_NAME)"'
else
CUST_C_MACRO += -DYQCONFIG_PLATFORM_NAME='"ERR"'
endif

$(call CO_OPTION_TO_FILE,$(CUST_C_MACRO),./tmp_MACRO.txt)

CUST_CONFIG_FILE := $(wildcard ./resource/$(strip $(YQCONFIG_PLATFORM_NAME))/$(strip $(YQCONFIG_CUSTOM_PRODUCT)).mak)

ifeq ($(strip $(CUST_CONFIG_FILE)),)
  $(error no responsable CUST_CONFIG_FILE to the PROJECT!!!)
endif

ifneq ($(wildcard ./resource/$(strip $(YQCONFIG_PLATFORM_NAME))/$(strip $(YQCONFIG_CUSTOM_PRODUCT))/resource_overlay),)
overlay_files := $(sort $(shell find ./resource/$(strip $(YQCONFIG_PLATFORM_NAME))/$(strip $(YQCONFIG_CUSTOM_PRODUCT))/resource_overlay -name "*.[ch]"))
endif

.PHONY:new clean update clean_module $(MODULES) prepare_resource

new: prepare_resource
	@$(MAKE) --directory=MakeCommon PROJECT=$(PROJECT) clean
	@$(MAKE) --directory=MakeCommon PROJECT=$(PROJECT) release 2>&1 | tee build.log
	@cp ./Project/DemoKit/DemoKit_Data/Release/*.bin ./ -r -f
	@exit 0
prepare_resource:
	@$(ECHO) prepare_resource ... 
	$(foreach v, $(overlay_files), $(shell cp -f $(v) $(subst ./resource/$(strip $(YQCONFIG_PLATFORM_NAME))/$(strip $(YQCONFIG_CUSTOM_PRODUCT))/resource_overlay/,./,$(v))))
	
$(MODULES):
	@$(ECHO) $(MAKECMDGOALS)... $(MODULES) 
ifeq ($(MAKECMDGOALS),clean_module)
	@$(MAKE) --directory=$@ PROJECT=$(PROJECT) clean 
else
	@$(MAKE) --directory=$@ PROJECT=$(PROJECT) release $(JOB_FLAGS)
endif

clean_module:$(MODULES)

update:$(MODULES)	
ifeq ($(MODULES),)
	@$(MAKE) --directory=MakeCommon PROJECT=$(PROJECT) release 2>&1 | tee build.log
endif
	@exit 0	
	
clean:
ifneq ($(MODULES),)
	@$(MAKE) clean_module
else
	@$(MAKE) --directory=MakeCommon PROJECT=$(PROJECT) clean
endif
	@rm ./*.bin -f
	@exit 0

#
# Help infomation
#
.PHONY:help
help:
	@$(ECHO) Typing right command as shown on following:
	@$(ECHO) 1.[make all update] make all targets those have been modified:
	@$(ECHO) "    make PROJECT=projectname new or make p=projectname new"
	@$(ECHO) 2.[make clean] clean targets
	@$(ECHO) "    make PROJECT=projectname clean or make p=projectname clean"
	@exit 0
