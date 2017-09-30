#ifndef _IQUC_SAMPLE_INT_H_
#define _IQUC_SAMPLE_INT_H_

//#define IQUC_ID_MAX_NUM 4
//#define IQUC_CTRLITEM_MAX_NUM 50

void IQUCLOG_OPEN(char *pFilename);
void IQUCLOG(char *format, ...);
void IQUCLOG_CLOSE(void);

extern IQUC_CTRL_ITEM* IQUC_GetCtrlItem(UINT32 Id, UINT32 Index);
extern UINT32 IQUC_SetCtrlIndex(UINT32 Id, UINT32 IQIndex);
extern UINT32 IQUC_GetCtrlIndex(UINT32 Id);
extern UINT32 IQUC_SetCtrlItem(UINT32 Id, UINT32 Index, IQUC_CTRL_ITEM *Item);
extern UINT32 IQUC_Set2ACtrlItem(UINT32 Id, UINT32 Index, IQUC_CTRL_ITEM *Item);
extern UINT32 IQUC_SettingFlow(UINT32 Id, UINT32 EngID, ENG_SUBFUNC *pEngSubF);
extern UINT32 IQUC_DumpUserControlTable(UINT32 Id);
extern UINT32 IQUC_DumpSettingTable(UINT32 Id);
extern UINT32 IQTUC_DumpRatioTable(UINT32 Id, CHAR *params, CHAR *file_name);
#endif //_IQUC_SAMPLE_INT_H_
