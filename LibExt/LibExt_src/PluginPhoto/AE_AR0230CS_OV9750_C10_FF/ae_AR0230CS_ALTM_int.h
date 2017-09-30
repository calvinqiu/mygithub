#ifndef _AE_AR0230CS_ALTM_INT_H_
#define _AE_AR0230CS_ALTM_INT_H_

#define ALTM_VER_1  0
#define ALTM_VER_2  1
#define ALTM_NONE   3

#define ALTM_VERSION ALTM_NONE


extern void AE_SetTrig2Event(void);
extern void AE_ClearTrig2Event(void);
extern void AE_waitTrig2Event(void);

//ver 1
extern void ae_AR0230CS_ALTM_Proc(void);

//ver 2
extern void AR0230_ALTM_FW_Control_1(void);
extern void AR0230_ALTM_FW_Control_2(void);
extern void AR0230_ALTM_FW_Control_3(void);
extern void AR0230_SetSensorRigister(void);

#endif //_AE_AR0230CS_ALTM_INT_H_
