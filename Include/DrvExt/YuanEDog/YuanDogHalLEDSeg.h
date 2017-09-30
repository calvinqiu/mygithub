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

//LED segment ��ʼ��
//����:			��
//����ֵ:		��
void YuanHalLEDSegInit(void);

//��������ʾ
//����:
//	*LED1		����1	//'0'~'9','A','b','C','d','E','F','g','H','L','P','q','S','U','-';
//	*LED2		����2	//'0'~'9','A','b','C','d','E','F','g','H','L','P','q','S','U','-';
//	*LED3		����3	//'0'~'9','A','b','C','d','E','F','g','H','L','P','q','S','U','-';
//����ֵ:		��
void YuanHalLEDSegTurnOnDigtal(uint8 *LED1,uint8 *LED2,uint8 *LED3);

//��������ʾ
//����:
//	*LED1		����1
//	*LED2		����2
//	*LED3		����3
//����ֵ:		��
void YuanHalLEDSegTurnoffDigtal(uint8 *LED1,uint8 *LED2,uint8 *LED3);

//��ʾ�״�״̬
//����:
//	Radar		�״�״̬(//0:OFF; 0x1:X; 0x2:La; 0x04:Ka; 0x08:K)
//����ֵ:		��
void YuanHalLEDSegTurnRadar(uint8 Radar);	//0:OFF; 0x1:X; 0x2:La; 0x04:Ka; 0x08:K;

//��ʾ����״̬
//����:
//	Direction	����(0:OFF; 0x01:N; 0x02:E; 0x04:S; 0x08:W)
//����ֵ:		��
void YuanHalLEDSegTurnDirection(uint8 Direction);	//0:OFF; 0x01:N; 0x02:E; 0x04:S; 0x08:W;

//��ʾ����1״̬
//����:
//	One			����1(0:OFF; 1:ON; 2:Up, 3:Down)
//����ֵ:		��
void YuanHalLEDSegTurnOne(uint8 One);	//0:OFF; 1:ON; 2:Up, 3:Down

//��ʾʱ�ӵ�״̬
//����:
//	DPoint		����(0:OFF; 1:ON)
//����ֵ:		��
void YuanHalLEDSegTurnDoublePoint(uint8 DPoint);	//0:OFF; 1:ON;

//��ʾ��״̬
//����:
//	Flower	����(0:OFF; 1:ON)
//����ֵ:		��
void YuanHalLEDSegTurnFlower(uint8 Flower);	//0:OFF; 1:ON;

//��ʾ��״̬
//����:
//	Cloud	����(0:OFF; 1:ON)
//����ֵ:		��
void YuanHalLEDSegTurnCloud(uint8 Cloud);	//0:OFF; 1:ON;

//��ʾ�����״̬
//����:
//	Point	����(0:OFF;  1:12Clock; 2:3Clock; 3:6Clock; 4:9Clock)
//����ֵ:		��
void YuanHalLEDSegTurnPoint(uint8 Point);	//0:OFF;  1:12Clock; 2:3Clock; 3:6Clock; 4:9Clock;

//��������ֵ
//����:			
//Light			����ֵ
//����ֵ:		��
void YuanHalLEDSegSetLight(int Light);	//0: Dark; 1: sligth Dark; 2: normal; 3:slight light; 4:light; 

//��������ֵ
//����:			��
//����ֵ:		��
void YuanHalLEDSegUpdatedImage(void);

#ifdef __cplusplus
}
#endif

#endif //YUANDOGHALLEDSEG_H__