/**
    @file       GPSParser.h
    @ingroup

    @brief      Sample module brief
                Sample module description

    @note       Nothing (or anything need to be mentioned).

    Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
*/

#ifndef _UART2MTK_H
#define _UART2MTK_H

#include "Type.h"
#include "UART.h"


/* add begin by ZMD, 2016-10-28, 原因:新版本管理*/
#define FW_VERSION_NUM_FUNC     ENABLE
/* add end by ZMD, 2016-10-28 */

#if(FW_VERSION_NUM_FUNC==ENABLE)
typedef enum
{
    FWVERSION_SETING_STATE_OFF,
    FWVERSION_SETING_STATE_ON,
    FWVERSION_SETING_STATE_OK,
    FWVERSION_SETING_STATE_OFF_MAX
} FWVERSION_SETING_STATE;
extern BOOL FwVersion_Seting_state;
#endif


#define RECEIVE_FROM_UART2        1
#define RECEIVE_FROM_UART3        2
#define RECEIVE_FROM_UART4        3

#define _UART_TO_MTK_RECEIVE_FROM_     RECEIVE_FROM_UART3

#define MTK_SENTENCE_SIZE       180

#define UART_TO_MTK_FLAG_DATAVALID      0x00000001
#define UART_TO_MTK_FLAG_LOG            0x00000002

//MTK to PW663 CMD  #PW#2015-10-22#lacy add

#define START_CHAR1 			 0xFF
#define START_CHAR2 			 0xAA
#define VERSION_CHAR		     0x01
#define STOP_CHAR				 0xEE


#define 	CMD_TOUCH   						0x01
#define 	CMD_TIME    							0x02
#define 	CMD_GPS   							0x06
#define 	CMD_FILE     						0x07
#define 	CMD_SETTING  						0x08
#define 	CMD_MOVIE_START    				0x09
#define 	CMD_MOVIE_STOP      				0x0A
#define 	CMD_MOVIE_FORMAT  				0x0B
#define   CMD_CAPTURE  						0x0C
#define   CMD_ADAS_SWITCH					0x0D//20160429 pgl add
#define   CMD_DAOCHE                                         0x0E  //pgl 20160705 add daoche
#define   CMD_CHANGE_DIAPLAYMODE                0x0F  //pgl 20160705 add PIPDISPLAY
#define   CMD_SWITCH_MODE                    0x10 ///* add begin by ZMD, 2016-10-28, 原因: */
//add by qiuhan on 20171028 for U15 begin
#define   CMD_OTG_TO_MTK                      0x20
#define   CMD_OTG_TO_NTK                      0x21
#define   CMD_EMR_VEDIO              0x23
#define   CMD_ADAS_LDWSL           0x24
#define   CMD_ADAS_LDWSR           0x25
#define   CMD_ADAS_FCWS           0x26
#define   CMD_LENS_POS_CAL_ON       0x27//add by qiuhan on 20171030 for   The lens position calibration
#define   CMD_LENS_POS_CAL_OFF       0x28//add by qiuhan on 20171030 for   The lens position calibration
#define   CMD_GSENSOR_VEDIO    0x29
#define   CMD_SHARP_TURN_VEDIO    0x30
#define   CMD_RUSH_VEDIO  0x31
#define   CMD_NASTY_BRAKE_VEDIO 0x32
//add end
#define 	CMD_GSENSOR_TRIG       				0x70
#define 	CMD_ADAS_RESQ       					0x71
#define 	CMD_MENUSETTING_RESQ       		0x72
#define 	CMD_SYSTEM_ERROR_CODE      		0x73
#define 	CMD_SYSTEM_POWEROFF       			0x74
#define 	CMD_SYSTEM_REBOOT_REQ       		0x75
#define   CMD_FWVERSION                                    0x76  ///* add begin by ZMD, 2016-10-28, 原因: */
#define   CMD_AUDIO_SWITCH                     0x77 ///* add begin by ZMD, 2016-10-28, 原因: */
#if defined(YQCONFIG_DATEPRINT_SUPPORT)
#define MENU_SETTING_MAX    10//9for water mark//8//pgl 20160628 add for audio_record_switch
#else
#define MENU_SETTING_MAX    9//8//pgl 20160628 add for audio_record_switch
#endif
extern UINT8 guiMTKMenuSetting[MENU_SETTING_MAX];

#define  SENSORMODE_REVERSE     1
#define  SENSORMODE_MIRROR       2
#define  SENSORMODE_NONE          0

//MTK to PW663 CMD  #PW#2015-10-22#lacy end

extern void UART_TO_MTK_InstallID(void) _SECTION(".kercfg_text");
extern ER UART_TO_MTKRec_Open(UART_BAUDRATE BaudRate);
extern ER UART_TO_MTKRec_Close(void);
extern void MTKComposeCMDRspFrame(UINT8 frame_id, UINT8 cmd,UINT8 * data, UINT8 data_len);
//extern void  XmodeStartSendFirstToMTK(char *pPath);
extern void  XmodeStartSendFirstToMTK(UINT32 Addr, UINT32 Size);
extern BOOL XmodemGetMTKMenuSetting(CHAR *Setting);
extern void XmodemSetMTKHeartBeatRes(BOOL En);
extern BOOL XmodemGetSpeed(FLOAT *Speed);

extern void XmodemSetBackSensorMode(int mode);
extern BOOL XmodemGetMTKReverseGearDet(void);
extern  BOOL XmodemCheckBackSensorMode(viod);
extern  BOOL XmodemCheckBackSensorNoramlDet(void);
extern  BOOL XmodemCheckBackSensorBackMirrorDet(void);
extern BOOL XmodemGetTPEventIsLock();
extern void XmodemSetTPEventLock(BOOL Lock);
extern UINT32 _SECTION(".kercfg_data") UART_TO_MTKRECEIVE_ID; // UART_TO_MTK Task
extern UINT32 _SECTION(".kercfg_data") FLG_ID_UART_TO_MTK; // UART_TO_MTK Flag
#endif




