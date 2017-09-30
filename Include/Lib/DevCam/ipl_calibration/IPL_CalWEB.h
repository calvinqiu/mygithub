#ifndef _IPL_CALWEB_H_
#define _IPL_CALWEB_H_
#include "Type.h"
#include "IPL_Utility.h"

#define RGB_COLOR(r, g, b) ((UINT32)(UINT8)(b) | ((UINT32)(UINT8)(g) << 8) | ((UINT32)(UINT8)(r) << 16))

/**
     open calibration WEB mode

     @note

     @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalWEB_Open(void);

/**
     close calibration WEB mode

     @note

     @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalWEB_Close(void);

/**
    get option string for web

    @note include calibration function and debug items

    @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalWEB_Get_Option_String(char* buf);

/**
    Get ready file name

    @note

    @return
         - @b ER E_OK:OK, others:NG
*/
ER CalWEB_Get_File(char* buf);

/**
    Get max line number can show on web

    @note   currently is defined in IPL_CalWEB_Int.h

    @return
         - @b ER E_OK:OK, others:NG
*/

ER CalWEB_Get_Max_Linenum(UINT32* num);

/**
    Trigger Task to run calibration

    @note

    @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalWEB_Set_Proc(char* func_name, char* params);

/**
    Set Engineer mode ready

    @note

    @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalWEB_Set_Ready(void);

/**
    Set ready file path and waitkey for user downloading

    @note   only filename will be extract, find ":\" as key word
            ex: A:\test.txt -> test.txt

    @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalWEB_Set_File(char* path);

/**
    Set calibration function item

    @note

    @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalWEB_Set_Func(char* name, FP fp);

/**
    Set calibration debug item

    @note   enabled indicates if this debug item is enabled
            chkbox_enabled indicates if web interface allow user to modify

    @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalWEB_Set_Dbg(char* name, BOOL enabled, BOOL chkbox_enabled);

/**
    Set showline to IQtool

    @note   idx: -1     ->  clear all text
                 0      ->  append line
                 else   ->  insert at the specific index

    @return
         - @b ER E_OK:OK, others:NG
*/
ER CalWEB_SetLine(char* content, INT32 idx, UINT32 color, UINT32 bg_color);

/**
    Wait web setkey event

    @note

    @return
         - @b ER E_OK:OK, others:NG
*/
extern ER CalWEB_WaitKey(void);

#endif //IPL_CAL_WEB
