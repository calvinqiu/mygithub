#ifndef _MSDCNVTCB_EXAM_H
#define _MSDCNVTCB_EXAM_H

//APIs for initial
extern void MsdcNvtCb_ExamInstallID(void) _SECTION(".kercfg_text");

//APIs for module use
extern INT32 MsdcNvtCb_ExamMsg(const char *fmtstr, ...);
extern INT32 MsdcNvtCb_ExamSetName(const char *name); //set name to 'TARGET' or 'MONITOR'
extern void MsdcNvtCb_RegCmdFilterCB(BOOL (*fpCmdFilterCB)(CHAR * pcCmdStr, int len));

//Register to MsdcNvt
extern BOOL MsdcNvtRegBi_Exam(void);

#endif
