#include "DispSrvApi.h"
#include "PlaybackTsk.h"

typedef MEM_RANGE (*MoviePlay_GetBuf)(UINT32 id);

extern void MoviePlay_DispSrv_Cfg(IMG_BUF* Src, USIZE* pDispAR, MoviePlay_GetBuf fpGetBuf);
extern void MoviePlay_DispSrv_Start(void);
extern void MoviePlay_DispSrv_Stop(void);
extern void MoviePlay_DispSrv_Trigger(void);
extern void MoviePlay_ChangeSingleDstRegion(PB_SIGDISP_TYPE TypeID, PIRECT pDstRegion);

