#include "Type.h"
#include "Platform.h"
#include "PartitionInfo.h"

EMB_PARTITION gPartition[EMB_PARTITION_INFO_COUNT] _SECTION(".partitioninfo_data") = {

#if (_CPU2_TYPE_ == _CPU2_NONE_)
#if (_DSP_TYPE_ == _DSP_NONE_)
    [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x00380000, 0x00000000},
    [1] = {EMBTYPE_FAT,     0,  0x00380000, 0x00000000, 0x00000000},
    [2] = {EMBTYPE_PSTORE,  0,  0x00380000, 0x00080000, 0x00000000},
#else
    [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x00900000, 0x00000000},
    [1] = {EMBTYPE_DSP,     0,  0x00900000, 0x00100000, 0x00000000},
    [2] = {EMBTYPE_FAT,     0,  0x00A00000, 0x07100000, 0x00000000},
    [3] = {EMBTYPE_PSTORE,  0,  0x07B00000, 0x00500000, 0x00000000},
#endif
#endif

#if (_CPU2_TYPE_ == _CPU2_ECOS_)
#if (_DSP_TYPE_ == _DSP_NONE_)

//    [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x00300000, 0x00000000},//loader+fw   3M
//    [1] = {EMBTYPE_ECOS,    0,  0x00300000, 0x00200000, 0x00000000},//ecos        2M
//    [2] = {EMBTYPE_PSTORE,  0,  0x00500000, 0x00030000, 0x00000000},//menu        

    [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x00300000, 0x00000000},//loader+fw   3M
    [1] = {EMBTYPE_ECOS,    0,  0x00300000, 0x00260000, 0x00000000},//ecos        2M
    [2] = {EMBTYPE_PSTORE,  0,  0x00560000, 0x00030000, 0x00000000},//menu   


   // [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x002AF000, 0x00000000},//loader+fw   2M
  //  [1] = {EMBTYPE_ECOS,    0,  0x002AF000, 0x00121000, 0x00000000},//ecos        2M
   // [2] = {EMBTYPE_PSTORE,  0,  0x003D0000, 0x00030000, 0x00000000},//menu        

#else
    [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x00900000, 0x00000000},
    [1] = {EMBTYPE_ECOS,    0,  0x00900000, 0x00200000, 0x00000000},
    [2] = {EMBTYPE_DSP,     0,  0x00B00000, 0x00100000, 0x00000000},
    [3] = {EMBTYPE_FAT,     0,  0x00C00000, 0x06F00000, 0x00200000},
    [4] = {EMBTYPE_PSTORE,  0,  0x07B00000, 0x00500000, 0x00080000},
#endif
#endif

#if (_CPU2_TYPE_ == _CPU2_LINUX_)
#if (_DSP_TYPE_ == _DSP_NONE_)
    [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x00900000, 0x00000000},
    [1] = {EMBTYPE_UBOOT,   0,  0x00900000, 0x00200000, 0x00000000},
    [2] = {EMBTYPE_LINUX,   0,  0x00B00000, 0x00500000, 0x00000000},
    [3] = {EMBTYPE_UBIFS,   0,  0x01000000, 0x02800000, 0x00000000},
#else
    [0] = {EMBTYPE_UITRON,  0,  0x00000000, 0x00900000, 0x00000000},
    [1] = {EMBTYPE_UBOOT,   0,  0x00900000, 0x00200000, 0x00000000},
    [2] = {EMBTYPE_LINUX,   0,  0x00B00000, 0x00500000, 0x00000000},
    [3] = {EMBTYPE_UBIFS,   0,  0x01000000, 0x02800000, 0x00000000},
    [4] = {EMBTYPE_DSP,     0,  0x03800000, 0x00500000, 0x00000000},
#endif
#endif

};