#ifndef YUANDOGHALKEYPAD_H__
#define YUANDOGHALKEYPAD_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C"{
#endif 

/////////////////////////////////////////////////////////////////////
// typdef variable
#define YUAN_KEY_NONE		0
#define YUAN_KEY_MENU		0x01
#define YUAN_KEY_PLUS		0x02
#define YUAN_KEY_SUB		0x04

#define YUAN_KEYPAD_NOPRESS		0
#define YUAN_KEYPAD_PRESSED		1
#define YUAN_KEYPAD_LONGPRESSED	2

#define YUAN_KEY_ALL		(YUAN_KEY_MENU | YUAN_KEY_PLUS | YUAN_KEY_SUB)

#define YUAN_KEY_DOWN		0
#define YUAN_KEY_UP			1

#define YUAN_RADAR_LINK		0x10
#define YUAN_RADAR_XBAN		0x01
#define YUAN_RADAR_LASER	0x02
#define YUAN_RADAR_KABAN	0x04
#define YUAN_RADAR_KBAN		0x08
#define YUAN_RADAR_NONE		0

#define YUAN_VALID_HAL_RADAR(x) 			((x == YUAN_RADAR_XBAN) || (x == YUAN_RADAR_LASER) || (x == YUAN_RADAR_KABAN) || (x == YUAN_RADAR_KBAN))

#define YUAN_RADAR_SIGNAL_ULTRA		4
#define YUAN_RADAR_SIGNAL_STRONG	3
#define YUAN_RADAR_SIGNAL_NORMAL	2
#define YUAN_RADAR_SIGNAL_WEAK		1
#define YUAN_RADAR_SIGNAL_LEAVE		0

#define YUAN_RADAR_ALL		(YUAN_RADAR_LASER | YUAN_RADAR_KBAN | YUAN_RADAR_KABAN |YUAN_RADAR_XBAN)
/////////////////////////////////////////////////////////////////////
// static function


/////////////////////////////////////////////////////////////////////
// global function

/////////////////////////////////////////////////////////////////////
// KEYPAD function	
//KEYPAD 初始化函数
//参数:				无
//返回值:  
//	1: 成功；	0: 失败；
int	YuanHalKeypadInit(void);

/////////////////////////////////////////////////////////////////////
// KEYPAD function	
//KEYPAD KEY 事件
//参数:				
//	Charactor:		key 
//	'M'				menu(long  press)
//	'm'				menu(short press)
//	'+'				+(long  press)
//	'='				+(short press)
//	'_'				menu(long  press)
//	'-'				menu(short press)
//	'S'				'M' | '+' | '_'(long  press)
//	's'				'm' | '=' | '-'(short press)
//返回值:  
//	1: 成功；	0: 失败；
void KeypadEvent(char Charactor);

//初始化 AT 接口回调函数
//参数:				
//	msg:		message of command
//	rsp:		response of command
//	rsp_len:	lenght of response
//返回值:
//	1: 成功；	0: 失败；
typedef void (*YuanHalATMessage)(void *msg, 				//msg: message of command
								 void *rsp,					//rsp: response of command
								 unsigned int rsp_len);	 	//rsp_len: lenght of response
//初始化 AT 接口
//参数:				
//	YuanHalATMessage:	callback function of message
//返回值:
//	1: 成功；	0: 失败；
int YuanHalATInit( YuanHalATMessage on_message );	//on_message: callback function of message

#ifdef __cplusplus
}
#endif

#endif  //YUANDOGHALKEYPAD_H__