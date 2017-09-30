/**
    CPU module driver.

    This file is the driver of CPU module.

    @file       cache.h
    @ingroup    mIDrvSys_Core
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _CACHE_H
#define _CACHE_H

#include "Type.h"


/**
    @addtogroup mIDrvSys_Core
*/
//@{

/**
    @name MIPS cache line size

    Define MIPS I/D cache line size
*/
//@{
#define MIPS_ICACHE_LINE_SIZE       (32)    ///< Instruction cache line size (unit: byte)
#define MIPS_DCACHE_LINE_SIZE       (32)    ///< Data cache line size (unit: byte)
//@}

extern ER       cpu_lockDownDCache(UINT32 start, UINT32 end);
extern ER       cpu_lockDownICache(UINT32 start, UINT32 end);

#endif

//@}
