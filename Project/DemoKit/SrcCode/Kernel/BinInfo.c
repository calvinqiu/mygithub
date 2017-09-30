/*;   Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
 *;
 *;   @file       BinInfo.c
 *;   @ingroup
 *;
 *;   @brief      Binary file information
 *;               Binary file information
 *;
 *;   @version    V1.20.000
 *;   @author     Novatek
 *;   @date       2012/07/19
 */

#include "Type.h"
#include "Platform.h"
#include "PrjCfg.h"
#include "BinInfo.h"

BININFO gBinInfo =
{
    //HEADINFO
    {
    MEM_DRAM_ADDR+CODE_ENTRY_OFFSET, //<- fw CODE entry (4)
    {0}, ///<- reserved (4*19)
    _CHIP_NAME_, //<- CHIP-NAME (8)
    "10000000", //<- version (8)
    "20100701", //<- releasedate (8)
    0xffffffff, //<- Bin File Length (4)
    0xffffffff, //<- Check Sum or CRC (4)
    0,///<- Length check for CRC (4)
    0,///<- reserved (4)
    0,///<- Bin flag (4)
    0,///<- Binary Tag for CRC (4)
    },
    //LDINFO
    {
    "", ///<- LD-NAME (16)
    0x0                 ///<- Fw flag (4) - Begin
#if(FW_PARTIAL_LOAD==ENABLE && _GPROF_TOOL_ == _GPROF_TOOL_OFF_)
        | LDCF_PARTLOAD_EN
#endif
    ,                   ///<- Fw flag (4) - End
    0xffffffff, ///<- Ld flag (4)
    0xffffffff, ///<- Ld load size (4)
    0, ///<- Ld exec time (4)
    0, ///<- Ld size (n blocks) (4)
    0, ///<- FW reserved size (4)
    0, ///< LdPackage(2)
    0, ///< LdStorage(2)
    {0} ///<- (4*5) ---------- reserved 5 WORDs for project Ld
    },
    //DRAMINFO
    {
    "DRAMINFO        ", //DRAMINFO (16)
    0xffffffff, //dram ctrl (4)
    0xffffffff, //dram ctrl2 (4)
    {
    0, 0, //Total
    0, 0, //Code Region
    0, 0, //Stack Region
    0, 0, //Heap Region
    0, 0, //Reserved Region
    }
    },
    //STRGINFO
    {
    "STRGINFO        ", //STRGINFO (16)
    0, //nand block size (4)
    0, //Resv2 (4)
    {
    0
    },
    },
    //FWINFO
    {
    FW_VERSION_NUM, //PRJ-NAME (16)
    {0} ///<- (4*16)  --------- reserved for project Fw
    }
};


