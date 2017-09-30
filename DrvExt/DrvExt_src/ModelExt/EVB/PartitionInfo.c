#include "Type.h"
#include "Platform.h"
#include "PartitionInfo.h"
//#NT#2016/12/07#Niven Cho -begin
//#NT#EMMC_PARTITION_BY_BLOCK
#if (_INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
#define SIZE_UNIT(x) ((((x)%_EMBMEM_BLK_SIZE_)==0)?((x)/_EMBMEM_BLK_SIZE_):1/0) //block alignment
#else
#define SIZE_UNIT(x) ((((x)%_EMBMEM_BLK_SIZE_)==0)?(x):1/0) //byte alignment
#endif

// IMPORTANT: IF GOT FOLLOWING MESSAGE, that means your designed size is not block alignment
// warning: division by zero
// error: initializer element is not constant
// error: (near initialization for 'gPartition[0].PartitionSize')

EMB_PARTITION gPartition[EMB_PARTITION_INFO_COUNT] _SECTION(".partitioninfo_data") = {

#if (_CPU2_TYPE_ == _CPU2_NONE_)
#if (_DSP_TYPE_ == _DSP_NONE_)
    #if (_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
    [0] = {EMBTYPE_UITRON,  0,  SIZE_UNIT(0x00000000), SIZE_UNIT(0x00380000), SIZE_UNIT(0x00000000)},
    [1] = {EMBTYPE_FAT,     0,  SIZE_UNIT(0x00380000), SIZE_UNIT(0x00000000), SIZE_UNIT(0x00000000)},
    [2] = {EMBTYPE_PSTORE,  0,  SIZE_UNIT(0x00380000), SIZE_UNIT(0x00080000), SIZE_UNIT(0x00000000)},
    #else
    [0] = {EMBTYPE_UITRON,  0,  SIZE_UNIT(0x00000000), SIZE_UNIT(0x00900000), SIZE_UNIT(0x00000000)},
    [1] = {EMBTYPE_FAT,     0,  SIZE_UNIT(0x00900000), SIZE_UNIT(0x07200000), SIZE_UNIT(0x00000000)},
    [2] = {EMBTYPE_PSTORE,  0,  SIZE_UNIT(0x07B00000), SIZE_UNIT(0x00500000), SIZE_UNIT(0x00000000)},
    #endif
#else
    [0] = {EMBTYPE_UITRON,  0,  SIZE_UNIT(0x00000000), SIZE_UNIT(0x00900000), SIZE_UNIT(0x00000000)},
    [1] = {EMBTYPE_DSP,     0,  SIZE_UNIT(0x00900000), SIZE_UNIT(0x00500000), SIZE_UNIT(0x00000000)},
    [2] = {EMBTYPE_FAT,     0,  SIZE_UNIT(0x00E00000), SIZE_UNIT(0x06D00000), SIZE_UNIT(0x00000000)},
    [3] = {EMBTYPE_PSTORE,  0,  SIZE_UNIT(0x07B00000), SIZE_UNIT(0x00500000), SIZE_UNIT(0x00000000)},
#endif
#endif

#if (_CPU2_TYPE_ == _CPU2_ECOS_)
#if (_DSP_TYPE_ == _DSP_NONE_)
    #if (_INTERSTORAGE_ == _INTERSTORAGE_SPI_)
    [0] = {EMBTYPE_UITRON,  0,  SIZE_UNIT(0x00000000), SIZE_UNIT(0x0029A000), SIZE_UNIT(0x00000000)},
    [1] = {EMBTYPE_ECOS,    0,  SIZE_UNIT(0x0029A000), SIZE_UNIT(0x00121000), SIZE_UNIT(0x00000000)},
    [2] = {EMBTYPE_FAT,     0,  SIZE_UNIT(0x003BB000), SIZE_UNIT(0x00000000), SIZE_UNIT(0x00000000)},
    [3] = {EMBTYPE_PSTORE,  0,  SIZE_UNIT(0x003BB000), SIZE_UNIT(0x00045000), SIZE_UNIT(0x00000000)},
    #else
    [0] = {EMBTYPE_UITRON,  0,  SIZE_UNIT(0x00000000), SIZE_UNIT(0x00900000), SIZE_UNIT(0x00000000)},
    [1] = {EMBTYPE_ECOS,    0,  SIZE_UNIT(0x00900000), SIZE_UNIT(0x00300000), SIZE_UNIT(0x00000000)},
    [2] = {EMBTYPE_FAT,     0,  SIZE_UNIT(0x00C00000), SIZE_UNIT(0x06F00000), SIZE_UNIT(0x00200000)},
    [3] = {EMBTYPE_PSTORE,  0,  SIZE_UNIT(0x07B00000), SIZE_UNIT(0x00500000), SIZE_UNIT(0x00080000)},
    #endif
#else
    [0] = {EMBTYPE_UITRON,  0,  SIZE_UNIT(0x00000000), SIZE_UNIT(0x00900000), SIZE_UNIT(0x00000000)},
    [1] = {EMBTYPE_ECOS,    0,  SIZE_UNIT(0x00900000), SIZE_UNIT(0x00300000), SIZE_UNIT(0x00000000)},
    [2] = {EMBTYPE_DSP,     0,  SIZE_UNIT(0x00C00000), SIZE_UNIT(0x00500000), SIZE_UNIT(0x00000000)},
    [3] = {EMBTYPE_FAT,     0,  SIZE_UNIT(0x01100000), SIZE_UNIT(0x06A00000), SIZE_UNIT(0x00200000)},
    [4] = {EMBTYPE_PSTORE,  0,  SIZE_UNIT(0x07B00000), SIZE_UNIT(0x00500000), SIZE_UNIT(0x00080000)},
#endif
#endif

#if (_CPU2_TYPE_ == _CPU2_LINUX_)
#if (_DSP_TYPE_ == _DSP_NONE_)
    [0] = {EMBTYPE_UITRON,  0,  SIZE_UNIT(0x00000000), SIZE_UNIT(0x00900000), SIZE_UNIT(0x00000000)},
    [1] = {EMBTYPE_UBOOT,   0,  SIZE_UNIT(0x00900000), SIZE_UNIT(0x00200000), SIZE_UNIT(0x00000000)},
	[2] = {EMBTYPE_UENV,    0,  SIZE_UNIT(0x00B00000), SIZE_UNIT(0x00040000), SIZE_UNIT(0x00000000)},
    [3] = {EMBTYPE_LINUX,   0,  SIZE_UNIT(0x00B40000), SIZE_UNIT(0x00500000), SIZE_UNIT(0x00000000)},
    [4] = {EMBTYPE_UBIFS,   0,  SIZE_UNIT(0x01040000), SIZE_UNIT(0x06B00000), SIZE_UNIT(0x00000000)},
    [5] = {EMBTYPE_PSTORE,  0,  SIZE_UNIT(0x07B40000), SIZE_UNIT(0x004C0000), SIZE_UNIT(0x00100000)},
#if (_INTERSTORAGE_ == _INTERSTORAGE_EMMC_)
	[6] = {EMBTYPE_RAMFS,   0,  SIZE_UNIT(0x08000000), SIZE_UNIT(0x00000000), SIZE_UNIT(0x00000000)},
#endif
#else
    [0] = {EMBTYPE_UITRON,  0,  SIZE_UNIT(0x00000000), SIZE_UNIT(0x00900000), SIZE_UNIT(0x00000000)},
    [1] = {EMBTYPE_UBOOT,   0,  SIZE_UNIT(0x00900000), SIZE_UNIT(0x00200000), SIZE_UNIT(0x00000000)},
	[2] = {EMBTYPE_UENV,    0,  SIZE_UNIT(0x00B00000), SIZE_UNIT(0x00040000), SIZE_UNIT(0x00000000)},
    [3] = {EMBTYPE_LINUX,   0,  SIZE_UNIT(0x00B40000), SIZE_UNIT(0x00500000), SIZE_UNIT(0x00000000)},
    [4] = {EMBTYPE_UBIFS,   0,  SIZE_UNIT(0x01040000), SIZE_UNIT(0x06600000), SIZE_UNIT(0x00000000)},
    [5] = {EMBTYPE_DSP,     0,  SIZE_UNIT(0x07640000), SIZE_UNIT(0x00500000), SIZE_UNIT(0x00000000)},
    [6] = {EMBTYPE_PSTORE,  0,  SIZE_UNIT(0x07B40000), SIZE_UNIT(0x004C0000), SIZE_UNIT(0x00100000)},
#endif
#endif

};
//#NT#2016/12/07#Niven Cho -end