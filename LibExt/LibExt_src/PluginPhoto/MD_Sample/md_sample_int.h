/**
    Motion Detection Sample.



    @file       MDAlg.h
    @ingroup    mISYSAlg
    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2011.  All rights reserved.
*/
#ifndef _MD_SAMPLE_H_
#define _MD_SAMPLE_H_

#define MD_ACC_BIT           8
#define MD_LAWIN_X           32
#define MD_LAWIN_Y           32
#define MD_DETWIN_X          16
#define MD_DETWIN_Y          12
#define MD_LAWIN_TOTAL       (MD_LAWIN_X * MD_LAWIN_Y)
#define MD_BLKDIFF_THR       160    //@8b
#define MD_BLKCNT_THR        64     //total block number which over than MD_BLKDIFF_THR
#define MD_DIFFSUM_THR       5120   //total block difference
#define MD_NORMAL_BASE       1000

#endif //_MD_SAMPLE_H_
