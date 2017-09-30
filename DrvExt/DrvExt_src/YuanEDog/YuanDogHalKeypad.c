#include "YuanDogHalKeypad.h"
#include "YuanDogHalEvent.h"
#include "YuanDogHalLog.h"
#include <stdio.h>

YuanHalATMessage	AT_on_message = NULL;
//////////////////////////////////////////////////////////
//thread function
enum
{
	ATMODE_NONE,
	ATMODE_HEAD,
	ATMODE_BODY
};

//modify by Novatek
void KeypadEvent(char Charactor)
{
	YUANHAL_EVENT Event = {0};

	uint16	Keypad = 0, KeypadStatus = YUAN_KEYPAD_NOPRESS;

	if (Charactor == 'M' || Charactor == '+' || Charactor == '_' || Charactor == 'S' ||
		Charactor == 'm' || Charactor == '=' || Charactor == '-' || Charactor == 's')
	{
		if (Charactor == 'M' || Charactor == '+' || Charactor == '_' || Charactor == 'S')
			KeypadStatus = YUAN_KEYPAD_LONGPRESSED;
		else 
			KeypadStatus = YUAN_KEYPAD_PRESSED;								   

		if (Charactor == 'M' || Charactor == 'm')	Keypad = YUAN_KEY_MENU;
		if (Charactor == '+' || Charactor == '=')	Keypad = YUAN_KEY_PLUS;
		if (Charactor == '_' || Charactor == '-')	Keypad = YUAN_KEY_SUB;
		if (Charactor == 'S' || Charactor == 's')	Keypad = (YUAN_KEY_SUB | YUAN_KEY_PLUS | YUAN_KEY_MENU);

		Event.EventCode = YUANHAL_EVENT_KEYPAD;
		Event.wParam = Keypad<<16 | KeypadStatus;
		YuanHalSetEvent(&Event);
	}
}


int YuanHalKeypadInit()
{
	return 1;
}

int YuanHalATInit( YuanHalATMessage on_message )
{
	AT_on_message = on_message;

	return 1;
}