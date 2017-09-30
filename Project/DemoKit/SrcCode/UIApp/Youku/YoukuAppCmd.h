#ifndef _YOUKUAPPCMD_H_
#define _YOUKUAPPCMD_H_



#include "Type.h"
#include "ykcamera_common.h"

/**
    @name EXT_CMD_ITEM command from APP.
*/
//@{

typedef INT32 (*YOUKU_CMD_FUNC)(handler_parameter *param, void *userdata) ;
typedef struct _YOUKU_CMD_ENTRY
{
    UINT32                cmd;
    YOUKU_CMD_FUNC        fpFunc;
} YOUKU_CMD_ENTRY;
#define YOUKU_CMD_BEGIN(tbl) static YOUKU_CMD_ENTRY (Cmd_##tbl)[]={  ///< begin a command table
#define YOUKU_CMD_ITEM(cmd,func)  {(cmd), (func)}, ///< insert a command item in command table
#define YOUKU_CMD_END()    }; ///< end a command table

typedef struct _YOUKU_UI_ENTRY
{
    INT32    YoukuItem;
    INT32    NvtItem;
} YOUKU_UI_ENTRY;
#define YOUKU_UI_BEGIN(tbl) static YOUKU_UI_ENTRY (tbl)[]={
#define YOUKU_UI_ITEM(Youku,Nvt)  {(Youku), (Nvt)},
#define YOUKU_UI_END()    };

extern INT32 YoukuSdkOpen(void);
extern void YoukuSdkClose(void);
extern void YoukuSdkTriggerUpload(void);

#endif//_YOUKUAPPCMD_H_



