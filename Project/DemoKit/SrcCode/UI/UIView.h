#ifndef _UIDISPLAY_H
#define _UIDISPLAY_H

#include "Type.h"
#include "UIDisplay.h"

//#NT#2016/09/07#Jeah Yen -begin
//#NT#support rotate with dual display
#define OSD_ATTR_ROTATE          0
//#NT#2016/09/07#Jeah Yen -end

//#NT#2014/03/25#KS Hung -begin
#define OSD_ROTATE_0_DEGREE      0
#define OSD_ROTATE_90_DEGREE     1
#define OSD_ROTATE_180_DEGREE    2
#define OSD_ROTATE_270_DEGREE    3
//#NT#2014/03/25#KS Hung -end

//OSD1
extern BOOL View_Window(UINT32 cmd, UINT32 param1, UINT32 param2);
//OSD2
extern BOOL View_Info(UINT32 cmd, UINT32 param1, UINT32 param2);
//VDO1
//VDO2
extern BOOL View_Background(UINT32 cmd, UINT32 param1, UINT32 param2);
extern BOOL View_Background_CK(UINT32 cmd, UINT32 param1, UINT32 param2);

//#NT#2016/09/07#Jeah Yen -begin
//#NT#support rotate with dual display
extern void View_Window_ConfigAttr(UINT32 did, UINT32 attr, UINT32 value);
//#NT#2016/09/07#Jeah Yen -end
extern DC* View_Window_BeginRotate(DC* pDC);
extern void View_Window_EndRotate(void);


#endif // _UIDISPLAY_H

