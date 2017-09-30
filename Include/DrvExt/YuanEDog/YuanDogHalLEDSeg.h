#ifndef YUANDOGHALLEDSEG_H__
#define YUANDOGHALLEDSEG_H__

#include "YuanDogUtils.h"

#ifdef __cplusplus
extern "C" {
#endif


#define YUAN_LEDSEG_TURNON			1
#define YUAN_LEDSEG_TURNOFF			0

#define YUAN_LEDSEG_RADAR_X			0x01
#define YUAN_LEDSEG_RADAR_LA		0x02
#define YUAN_LEDSEG_RADAR_KA		0x04
#define YUAN_LEDSEG_RADAR_K			0x08
		
#define YUAN_LEDSEG_DIRECTION_N		1
#define YUAN_LEDSEG_DIRECTION_E		2
#define YUAN_LEDSEG_DIRECTION_S		3
#define YUAN_LEDSEG_DIRECTION_W		4

#define YUAN_LEDSEG_ONE_OFF			0
#define YUAN_LEDSEG_ONE_ON			1
#define YUAN_LEDSEG_ONE_UP			2
#define YUAN_LEDSEG_ONE_DOWN		3


#define YUAN_LEDSEG_CLOCK_12		1
#define YUAN_LEDSEG_CLOCK_3			2
#define YUAN_LEDSEG_CLOCK_6			3
#define YUAN_LEDSEG_CLOCK_9			4

//LED segment 初始化
//参数:			无
//返回值:		无
void YuanHalLEDSegInit(void);

//打开数字显示
//参数:
//	*LED1		数字1	//'0'~'9','A','b','C','d','E','F','g','H','L','P','q','S','U','-';
//	*LED2		数字2	//'0'~'9','A','b','C','d','E','F','g','H','L','P','q','S','U','-';
//	*LED3		数字3	//'0'~'9','A','b','C','d','E','F','g','H','L','P','q','S','U','-';
//返回值:		无
void YuanHalLEDSegTurnOnDigtal(uint8 *LED1,uint8 *LED2,uint8 *LED3);

//关数字显示
//参数:
//	*LED1		数字1
//	*LED2		数字2
//	*LED3		数字3
//返回值:		无
void YuanHalLEDSegTurnoffDigtal(uint8 *LED1,uint8 *LED2,uint8 *LED3);

//显示雷达状态
//参数:
//	Radar		雷达状态(//0:OFF; 0x1:X; 0x2:La; 0x04:Ka; 0x08:K)
//返回值:		无
void YuanHalLEDSegTurnRadar(uint8 Radar);	//0:OFF; 0x1:X; 0x2:La; 0x04:Ka; 0x08:K;

//显示方向状态
//参数:
//	Direction	方向(0:OFF; 0x01:N; 0x02:E; 0x04:S; 0x08:W)
//返回值:		无
void YuanHalLEDSegTurnDirection(uint8 Direction);	//0:OFF; 0x01:N; 0x02:E; 0x04:S; 0x08:W;

//显示数字1状态
//参数:
//	One			数字1(0:OFF; 1:ON; 2:Up, 3:Down)
//返回值:		无
void YuanHalLEDSegTurnOne(uint8 One);	//0:OFF; 1:ON; 2:Up, 3:Down

//显示时钟点状态
//参数:
//	DPoint		方向(0:OFF; 1:ON)
//返回值:		无
void YuanHalLEDSegTurnDoublePoint(uint8 DPoint);	//0:OFF; 1:ON;

//显示花状态
//参数:
//	Flower	方向(0:OFF; 1:ON)
//返回值:		无
void YuanHalLEDSegTurnFlower(uint8 Flower);	//0:OFF; 1:ON;

//显示云状态
//参数:
//	Cloud	方向(0:OFF; 1:ON)
//返回值:		无
void YuanHalLEDSegTurnCloud(uint8 Cloud);	//0:OFF; 1:ON;

//显示方向点状态
//参数:
//	Point	方向(0:OFF;  1:12Clock; 2:3Clock; 3:6Clock; 4:9Clock)
//返回值:		无
void YuanHalLEDSegTurnPoint(uint8 Point);	//0:OFF;  1:12Clock; 2:3Clock; 3:6Clock; 4:9Clock;

//设置亮度值
//参数:			
//Light			亮度值
//返回值:		无
void YuanHalLEDSegSetLight(int Light);	//0: Dark; 1: sligth Dark; 2: normal; 3:slight light; 4:light; 

//设置亮度值
//参数:			无
//返回值:		无
void YuanHalLEDSegUpdatedImage(void);

#ifdef __cplusplus
}
#endif

#endif //YUANDOGHALLEDSEG_H__