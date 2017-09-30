#include "PrjCfg.h"
#include "CoreInfo.h"

/**
    CAUTION!! DO NOT MODIFY THIS FILE.
    Moidfy lds or PrjCfg to change those address and size.
*/

#define LABEL_NAME(_name_) _name_

extern char LABEL_NAME (_section_01_addr)[];
extern char LABEL_NAME (_section_all_size)[];

STATIC_ASSERT(sizeof(CORE_INFO) == 64);

CORE_INFO g_CoreInfo = {
    .InfoFourCC = MAKEFOURCC('C','O','R','E'),
    .InfoSize   = sizeof(CORE_INFO),
    .uItronAddr = (UINT32)_section_01_addr,
    .uItronSize = (UINT32)_section_all_size,

#if (_CPU2_TYPE_ == _CPU2_ECOS_)
    .eCosAddr   = COREMEM_ECOS_ADDR,
    .eCosSize   = COREMEM_ECOS_SIZE,
#elif (_CPU2_TYPE_ == _CPU2_LINUX_)
    .uBootAddr  = COREMEM_LINUX_SUBADDR_UBOOT,
    .uBootSize  = COREMEM_LINUX_SIZE - (COREMEM_LINUX_SUBADDR_UBOOT-COREMEM_LINUX_ADDR),
    .uImageAddr = COREMEM_LINUX_SUBADDR_UIMAGE,
    .uImageSize = COREMEM_LINUX_SUBADDR_UBOOT - COREMEM_LINUX_SUBADDR_UIMAGE,
    .LinuxAddr  = COREMEM_LINUX_ADDR,
    .LinuxSize  = COREMEM_LINUX_SIZE,
#if (COREMEM_LINUX_SUBADDR_ROOTFS!=0)
    .RootFsAddr = COREMEM_LINUX_SUBADDR_ROOTFS,
    .RootFsSize = COREMEM_LINUX_SUBADDR_UIMAGE-COREMEM_LINUX_SUBADDR_ROOTFS,
#endif
#endif

#if (_DSP_TYPE_ == _DSP_FREERTOS_)
    .DspAddr    = COREMEM_DSP_ADDR,
    .DspSize    = COREMEM_DSP_SIZE,
#endif
};

