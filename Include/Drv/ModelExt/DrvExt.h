#ifndef _DRIVEREXT_H_
#define _DRIVEREXT_H_

#include "Type.h"
#include "ErrorNo.h"
#include "Debug.h"
#include "Utility.h"

// Init IO for external device
extern void Dx_InitIO(void);
// Uninit IO for external device
extern void Dx_UninitIO(void);

// command
extern BOOL Dx_OnCommand(CHAR *pcCmdStr);

/////////////////////////////////////////
// DriverExt

#include "DxStorage.h"
#include "DxDisplay.h"

#endif //_DRIVEREXT_H_
