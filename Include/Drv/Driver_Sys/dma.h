/**
    Header file for DMA module.

    This file is the header file that define the API for DMA module.

    @file       dma.h
    @ingroup    mIDrvDMA_DMA
    @note       Nothing.

    Copyright   Novatek Microelectronics Corp. 2014.  All rights reserved.
*/

#ifndef _DMA_H
#define _DMA_H

#include "Type.h"

/**
    @addtogroup mIDrvDMA_DMA
*/

/**
    Check DRAM address is cacheable or not

    Check DRAM address is cacheable or not.

    @param[in] addr     DRAM address

    @return Cacheable or not
        - @b TRUE   : Cacheable
        - @b FALSE  : Non-cacheable
*/
#define dma_isCacheAddr(addr)           ((((addr) & 0x20000000) == 0x20000000) ? FALSE : TRUE)

/**
    Translate DRAM address to cacheable address.

    Translate DRAM address to cacheable address.

    @param[in] addr     DRAM address

    @return Cacheable DRAM address
*/
#define dma_getCacheAddr(addr)          (((addr) & 0x1FFFFFFF) | 0x80000000)


/**
    DRAM type encoding

    DRAM type encoding returned by dma_getDramType()
*/
typedef enum
{
    DDR_TYPE_DDR2,          ///< DDR2 DRAM
    DDR_TYPE_DDR3,          ///< DDR3 DRAM
    ENUM_DUMMY4WORD(DMA_DDR_TYPE)
} DMA_DDR_TYPE;

extern DMA_DDR_TYPE     dma_getDramType(void);
extern UINT32           dma_getDramBaseAddr(void);

//@}

#endif
