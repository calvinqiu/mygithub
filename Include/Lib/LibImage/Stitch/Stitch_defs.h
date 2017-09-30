#ifndef _STITCH_DEFS_H_
#define _STITCH_DEFS_H_

/**
   Return state definitions of stitch APIs
*/
typedef enum _STITCH_STAT_
{
    STITCH_SUCCEED              = 1,

    STITCH_ERR_NOTSUP_IMGFMT    = -1000,
    STITCH_ERR_NOTSUP_IMGRES,
    STITCH_ERR_NOTSUP_DSAMPLE,
    STITCH_ERR_MEM_OUTOFALLOC_HIST,
    STITCH_ERR_MEM_OUTOFALLOC_ESTBUF,
    STITCH_ERR_NOT_INITIAL,
    STITCH_ERR_NOT_CAL_REQMEM,
    STITCH_ERR_BLENDRADIUS,
    STITCH_ERR_LUT_OUTIMGSPACE,
    STITCH_ERR_TOOSMALL_ESTSPACE,
    STITCH_ERR_LRIMG_SIZE_DIFF,
    STITCH_ERR_EST_DXRANGE,
    STITCH_ERR_NOTDEF_SEAMLINE,
    
    ENUM_DUMMY4WORD(STITCH_STAT)
} STITCH_STAT;


/**
    Image structure 
*/
typedef struct _STITCH_IM
{
    UINT32 uiAddr;                  ///< starting memory address
    ISIZE  Dim;                     ///< image dimension
    UINT32 uiOffset;                ///< offset
} STITCH_IM;


#endif
