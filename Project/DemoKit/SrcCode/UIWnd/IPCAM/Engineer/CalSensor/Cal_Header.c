#include "Cal_Header_Int.h"
#include "IPL_CAL_Debug.h"

#if   (_IPPLIB_ == _IPL_MN34110_EVB_FF_)

    ER Set2Preview(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize)
    {
        extern ER Set2Preview_MN34110_EVB_FF(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize) ;
        return Set2Preview_MN34110_EVB_FF(Id, IplNum, BufAddr,BufSize);
    }

    pCAL_ITEM_OBJ CAL_ITEM(IPL_PROC_ID Id)
    {
        extern pCAL_ITEM_OBJ CAL_ITEM_MN34110_EVB_FF(IPL_PROC_ID Id) ;
        return CAL_ITEM_MN34110_EVB_FF(Id);
    }

    BOOL CAL_ReadDebugConfig(void)
    {
        extern BOOL CAL_ReadDebugConfig_MN34110_EVB_FF(void);
        return CAL_ReadDebugConfig_MN34110_EVB_FF();
    }

#elif   (_IPPLIB_ == _IPL_AR0330M_EVB_FF_)
    ER Set2Preview(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize)
    {
        extern ER Set2Preview_AR0330M_EVB_FF(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize) ;
        return Set2Preview_AR0330M_EVB_FF(Id, IplNum, BufAddr,BufSize);
    }

    pCAL_ITEM_OBJ CAL_ITEM(IPL_PROC_ID Id)
    {
        extern pCAL_ITEM_OBJ CAL_ITEM_AR0330M_EVB_FF(IPL_PROC_ID Id) ;
        return CAL_ITEM_AR0330M_EVB_FF(Id);
    }

    BOOL CAL_ReadDebugConfig(void)
    {
        extern BOOL CAL_ReadDebugConfig_AR0330M_EVB_FF(void);
        return CAL_ReadDebugConfig_AR0330M_EVB_FF();
    }

#elif   (_IPPLIB_ == _IPL_OV4689_EVB_FF_)
    ER Set2Preview(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize)
    {
        extern ER Set2Preview_OV4689_EVB_FF(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize) ;
        return Set2Preview_OV4689_EVB_FF(Id, IplNum, BufAddr,BufSize);
    }

    pCAL_ITEM_OBJ CAL_ITEM(IPL_PROC_ID Id)
    {
        extern pCAL_ITEM_OBJ CAL_ITEM_OV4689_EVB_FF(IPL_PROC_ID Id) ;
        return CAL_ITEM_OV4689_EVB_FF(Id);
    }

    BOOL CAL_ReadDebugConfig(void)
    {
        extern BOOL CAL_ReadDebugConfig_OV4689_EVB_FF(void);
        return CAL_ReadDebugConfig_OV4689_EVB_FF();
    }

#elif   (_IPPLIB_ == _IPL_OV4689_IMX322LQJ_EVB_FF_)
    ER Set2Preview(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize)
    {
        extern ER Set2Preview_OV4689_IMX322LQJ_EVB_FF(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize) ;
        return Set2Preview_OV4689_IMX322LQJ_EVB_FF(Id, IplNum, BufAddr,BufSize);
    }

    pCAL_ITEM_OBJ CAL_ITEM(IPL_PROC_ID Id)
    {
        extern pCAL_ITEM_OBJ CAL_ITEM_OV4689_IMX322LQJ_EVB_FF(IPL_PROC_ID Id) ;
        return CAL_ITEM_OV4689_IMX322LQJ_EVB_FF(Id);
    }

    BOOL CAL_ReadDebugConfig(void)
    {
        extern BOOL CAL_ReadDebugConfig_OV4689_IMX322LQJ_EVB_FF(void);
        return CAL_ReadDebugConfig_OV4689_IMX322LQJ_EVB_FF();
    }

#elif   (_IPPLIB_ == _IPL_IMX078CQK_EVB_FF_)
    ER Set2Preview(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize)
    {
        extern ER Set2Preview_IMX078CQK_EVB_FF(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize) ;
        return Set2Preview_IMX078CQK_EVB_FF(Id, IplNum, BufAddr,BufSize);
    }

    pCAL_ITEM_OBJ CAL_ITEM(IPL_PROC_ID Id)
    {
        extern pCAL_ITEM_OBJ CAL_ITEM_IMX078CQK_EVB_FF(IPL_PROC_ID Id) ;
        return CAL_ITEM_IMX078CQK_EVB_FF(Id);
    }

    BOOL CAL_ReadDebugConfig(void)
    {
        extern BOOL CAL_ReadDebugConfig_IMX078CQK_EVB_FF(void);
        return CAL_ReadDebugConfig_IMX078CQK_EVB_FF();
    }

#elif   (_IPPLIB_ == _IPL_AR0230CS_OV9750_C10_FF_)
    ER Set2Preview(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize)
    {
        extern ER Set2Preview_AR0230CS_OV9750_C10_FF(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize) ;
        return Set2Preview_AR0230CS_OV9750_C10_FF(Id, IplNum, BufAddr,BufSize);
    }

    pCAL_ITEM_OBJ CAL_ITEM(IPL_PROC_ID Id)
    {
        extern pCAL_ITEM_OBJ CAL_ITEM_AR0230CS_OV9750_C10_FF(IPL_PROC_ID Id) ;
        return CAL_ITEM_AR0230CS_OV9750_C10_FF(Id);
    }

    BOOL CAL_ReadDebugConfig(void)
    {
        extern BOOL CAL_ReadDebugConfig_AR0230CS_OV9750_C10_FF(void);
        return CAL_ReadDebugConfig_AR0230CS_OV9750_C10_FF();
    }

#elif   (_IPPLIB_ == _IPL_AR0230CS_IPCAM_FF_)
    ER Set2Preview(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize)
    {
        extern ER Set2Preview_AR0230CS_IPCAM_FF(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize) ;
        return Set2Preview_AR0230CS_IPCAM_FF(Id, IplNum, BufAddr,BufSize);
    }

    pCAL_ITEM_OBJ CAL_ITEM(IPL_PROC_ID Id)
    {
        extern pCAL_ITEM_OBJ CAL_ITEM_AR0230CS_IPCAM_FF(IPL_PROC_ID Id) ;
        return CAL_ITEM_AR0230CS_IPCAM_FF(Id);
    }

    BOOL CAL_ReadDebugConfig(void)
    {
        extern BOOL CAL_ReadDebugConfig_AR0230CS_IPCAM_FF(void);
        return CAL_ReadDebugConfig_AR0230CS_IPCAM_FF();
    }
#elif   (_IPPLIB_ == _IPL_AR0230CS_EVB_FF_)
    ER Set2Preview(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize)
    {
        extern ER Set2Preview_AR0230CS_EVB_FF(IPL_PROC_ID Id, UINT32 IplNum, UINT32 BufAddr,UINT32 BufSize) ;
        return Set2Preview_AR0230CS_EVB_FF(Id, IplNum, BufAddr,BufSize);
    }

    pCAL_ITEM_OBJ CAL_ITEM(IPL_PROC_ID Id)
    {
        extern pCAL_ITEM_OBJ CAL_ITEM_AR0230CS_EVB_FF(IPL_PROC_ID Id) ;
        return CAL_ITEM_AR0230CS_EVB_FF(Id);
    }

    BOOL CAL_ReadDebugConfig(void)
    {
        extern BOOL CAL_ReadDebugConfig_AR0230CS_EVB_FF(void);
        return CAL_ReadDebugConfig_AR0230CS_EVB_FF();
    }
#else
#endif
