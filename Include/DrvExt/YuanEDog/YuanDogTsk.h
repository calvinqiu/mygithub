/**
    @file       GPSParser.h
    @ingroup

    @brief      Sample module brief
                Sample module description

    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _YUANDOG_H
#define _YUANDOG_H

extern void YuanDog_InstallID(void);
extern ER YuanDogTask_Open();
extern ER YuanDogTask_Close(void);
extern void YuanDogTsk(void);
extern void DogBark();

#endif




