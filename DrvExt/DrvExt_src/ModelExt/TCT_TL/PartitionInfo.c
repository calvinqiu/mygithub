#include "Type.h"
#include "Platform.h"
#include "PartitionInfo.h"

EMB_PARTITION gPartition[EMB_PARTITION_INFO_COUNT] _SECTION(".partitioninfo_data") = {

#if (_CPU2_TYPE_ == _CPU2_NONE_)
#if (_DSP_TYPE_ == _DSP_NONE_)
    #if (_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
    [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x00380000, 0x00000000},
    [1] = {EMBTYPE_FAT,     0,  0x00380000, 0x00000000, 0x00000000},
    [2] = {EMBTYPE_PSTORE,  0,  0x00380000, 0x00080000, 0x00000000},
    #else
    [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x00900000, 0x00000000},
    [1] = {EMBTYPE_FAT,     0,  0x00900000, 0x07200000, 0x00000000},
    [2] = {EMBTYPE_PSTORE,  0,  0x07B00000, 0x00500000, 0x00000000},
    #endif
#else
    [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x00900000, 0x00000000},
    [1] = {EMBTYPE_DSP,     0,  0x00900000, 0x00500000, 0x00000000},
    [2] = {EMBTYPE_FAT,     0,  0x00E00000, 0x06D00000, 0x00000000},
    [3] = {EMBTYPE_PSTORE,  0,  0x07B00000, 0x00500000, 0x00000000},
#endif
#endif

#if (_CPU2_TYPE_ == _CPU2_ECOS_)
#if (_DSP_TYPE_ == _DSP_NONE_)
    #if (_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
    [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x0029A000, 0x00000000},
    [1] = {EMBTYPE_ECOS,    0,  0x0029A000, 0x00121000, 0x00000000},
    [2] = {EMBTYPE_FAT,     0,  0x003BB000, 0x00000000, 0x00000000},
    [3] = {EMBTYPE_PSTORE,  0,  0x003BB000, 0x00045000, 0x00000000},
    #else
    [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x00900000, 0x00000000},
    [1] = {EMBTYPE_ECOS,    0,  0x00900000, 0x00200000, 0x00000000},
    [2] = {EMBTYPE_FAT,     0,  0x00B00000, 0x07000000, 0x00200000},
    [3] = {EMBTYPE_PSTORE,  0,  0x07B00000, 0x00500000, 0x00080000},
    #endif
#else
    [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x00900000, 0x00000000},
    [1] = {EMBTYPE_ECOS,    0,  0x00900000, 0x00200000, 0x00000000},
    [2] = {EMBTYPE_DSP,     0,  0x00B00000, 0x00500000, 0x00000000},
    [3] = {EMBTYPE_FAT,     0,  0x01000000, 0x06B00000, 0x00200000},
    [4] = {EMBTYPE_PSTORE,  0,  0x07B00000, 0x00500000, 0x00080000},
#endif
#endif

#if (_CPU2_TYPE_ == _CPU2_LINUX_)
#if (_DSP_TYPE_ == _DSP_NONE_)
    [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x00900000, 0x00000000},
    [1] = {EMBTYPE_UBOOT,   0,  0x00900000, 0x00200000, 0x00000000},
	[2] = {EMBTYPE_UENV,    0,  0x00B00000, 0x00040000, 0x00000000},
    [3] = {EMBTYPE_LINUX,   0,  0x00B40000, 0x00500000, 0x00000000},
    [4] = {EMBTYPE_UBIFS,   0,  0x01040000, 0x05B00000, 0x00000000},
    [5] = {EMBTYPE_PSTORE,  0,  0x06B40000, 0x014C0000, 0x00100000},
#else
    [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x00900000, 0x00000000},
    [1] = {EMBTYPE_UBOOT,   0,  0x00900000, 0x00200000, 0x00000000},
	[2] = {EMBTYPE_UENV,    0,  0x00B00000, 0x00040000, 0x00000000},
    [3] = {EMBTYPE_LINUX,   0,  0x00B40000, 0x00500000, 0x00000000},
    [4] = {EMBTYPE_UBIFS,   0,  0x01040000, 0x05600000, 0x00000000},
    [5] = {EMBTYPE_DSP,     0,  0x06640000, 0x00500000, 0x00000000},
    [6] = {EMBTYPE_PSTORE,  0,  0x06B40000, 0x014C0000, 0x00100000},
#endif
#endif

};