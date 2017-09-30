#ifndef _LDWSINT_H
#define _LDWSINT_H


#include "dis_lib.h"

void SnGParms_Detection(void);

/**
    Save motion to dram
*/
extern void SnGParms_dumpMvInfo2dram(MDS_DIM mdsDim);
/**
    Save motion to dram for special cases
*/
extern void SnGParms_dumpMvInfo2dram2(MDS_DIM mdsDim);
/**
    Save motion from dram to file
*/
extern void SnGParms_dumpMvInfo2file(void);


#endif
