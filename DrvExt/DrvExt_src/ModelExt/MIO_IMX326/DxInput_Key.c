/**
    Copyright   Novatek Microelectronics Corp. 2009.  All rights reserved.

    @file       DetKey.c
    @ingroup    mIPRJAPKeyIO

    @brief      Scan key, modedial
                Scan key, modedial

    @note       Nothing.

    @date       2009/04/22
*/

/** \addtogroup mIPRJAPKeyIO */
//@{

#include "DxCfg.h"
#include "IOCfg.h"

#include "DxInput.h"
#include "Debug.h"
#include "KeyDef.h"
#include "rtc.h"
#include "HwPower.h"
#include "GSensor.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

void Get3180_I2C04OFF(void);
void Set3180_I2C04ON(void);
void Set3180_I2C04OFF(void);

////////////////////////////////////////////////////////////////////////////////
// ADC related
////////////////////////////////////////////////////////////////////////////////
// ADC related
//#NT#2009/11/16#Lincy -begin
#if (ADC_KEY == ENABLE)
#define VOLDET_KEY_ADC_RANGE         (110)
#define VOLDET_KEY_ADC_LVL0          (250-VOLDET_KEY_ADC_RANGE)

#define VOLDET_KEY_ADC_TH            (512)

#define VOLDET_KEY_LVL_UNKNOWN           0xFFFFFFFF
#define VOLDET_KEY_LVL_0                 0
#define VOLDET_KEY_LVL_1                 1
#define VOLDET_KEY_LVL_2                 2
#endif

#if (ADC_KEY == ENABLE)
static UINT32 VolDet_GetKey1ADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
    UINT32 uiADCValue;

    uiADCValue = adc_readData(ADC_CH_VOLDET_KEY1);
    // One-Shot Mode, trigger one-shot
    adc_triggerOneShot(ADC_CH_VOLDET_KEY1);

    return uiADCValue;
#else
    return adc_readData(ADC_CH_VOLDET_KEY1);
#endif
}
static UINT32 VolDet_GetKey2ADC(void)
{
#if (VOLDET_ADC_CONT_MODE == DISABLE)
    UINT32 uiADCValue;

    uiADCValue = adc_readData(ADC_CH_VOLDET_KEY2);
    // One-Shot Mode, trigger one-shot
    adc_triggerOneShot(ADC_CH_VOLDET_KEY2);

    return uiADCValue;
#else
    return adc_readData(ADC_CH_VOLDET_KEY2);
#endif
}
/**
  Get ADC key voltage level

  Get  ADC key  2 voltage level.

  @param void
  @return UINT32 key level, refer to VoltageDet.h -> VOLDET_MS_LVL_XXXX
*/
static UINT32 VolDet_GetKey1Level(void)
{
    UINT32          uiKey1ADC;

    uiKey1ADC = VolDet_GetKey1ADC();
    if (uiKey1ADC < 100)
    {

        // debug_msg("VOLDET_KEY_LVL_MODE \r\n");
        return VOLDET_KEY_LVL_0;
        }
    else if ((uiKey1ADC > (200))
             && (uiKey1ADC < 300))
        {

        //   debug_msg("VOLDET_KEY_LVL_MENU \r\n");
        return VOLDET_KEY_LVL_1;
    }
    else if ((uiKey1ADC > 350)
             && (uiKey1ADC < 450))
    {

        //   debug_msg("VOLDET_KEY_LVL_SOS \r\n");
        return VOLDET_KEY_LVL_2;
    }


    return VOLDET_KEY_LVL_UNKNOWN;
}
static UINT32 VolDet_GetKey2Level(void)
{
    static UINT32   uiRetKey1Lvl;
    UINT32          uiKey1ADC, uiCurKey2Lvl;

    uiKey1ADC = VolDet_GetKey2ADC();
    DBG_IND("uiKey2ADC %d \r\n", uiKey1ADC);
    if (uiKey1ADC < VOLDET_KEY_ADC_TH)
    {
        uiCurKey2Lvl = VOLDET_KEY_LVL_0;
    }
    else
    {
        uiCurKey2Lvl = VOLDET_KEY_LVL_UNKNOWN;
    }

    uiRetKey1Lvl = uiCurKey2Lvl;

    return VOLDET_KEY_LVL_UNKNOWN;
}
/**
  Detect Mode Switch state.

  Detect Mode Switch state.

  @param void
  @return UINT32 Mode Switch state (DSC Mode)
*/
#endif

////////////////////////////////////////////////////////////////////////////////
// GPIO related

//static BOOL g_bIsShutter2Pressed = FALSE;



/**
  Delay between toggle GPIO pin of input/output

  Delay between toggle GPIO pin of input/output

  @param void
  @return void
*/
#if 0
static void DrvKey_DetKeyDelay(void)
{
    gpio_readData(0);
    gpio_readData(0);
    gpio_readData(0);
    gpio_readData(0);
}
#endif

void DrvKey_Init(void)
{
}

#if 0
static UINT32 DrvKey_DetGsensorEvent(void)
{
    UINT32 uiKeyCode = 0;
    BOOL bGSensorStatus=FALSE;
    Gsensor_Data GS_Data = {0};

    bGSensorStatus = GSensor_GetStatus(&GS_Data);

    if(bGSensorStatus==TRUE)
    {
        uiKeyCode |= FLGKEY_CUSTOM1;
    }

    return uiKeyCode;
}
#endif
// <Layout>
//------------------------------//
//                              //
//                              //
//                              //
//                              //
//    SW1   SW2   SW3   SW4     //
//------------------------------//
/*
    SW1        C_GPIO_10
    SW2        ADC ch1 level    0
    SW3        ADC ch1 level ~250
    SW4        ADC ch2 level    0
*/
/**
  Detect normal key is pressed or not.

  Detect normal key is pressed or not.
  Return key pressed status (refer to KeyDef.h)

  @param void
  @return UINT32
*/


#include "Type.h"
#include "i2c.h"
#include "GSensor.h"
#include "Delay.h"
#include "DxCfg.h"

#if 1

#define uchar unsigned char
#define uint  unsigned int


#define KEY_TYPE2		1


#define COM_REG			0X04

#define POWEROFFCMD		0xaa
#define POWERONCMD		0x55


#if 0  //old

#define IIC_SDA 	P_GPIO_32
#define IIC_SCL 	P_GPIO_31
#else

#define IIC_SDA 	P_GPIO_31
#define IIC_SCL 	P_GPIO_32
#endif

#define SDA_HIGH	{gpio_setDir(IIC_SDA, GPIO_DIR_OUTPUT); gpio_setPin(IIC_SDA);}
#define SDA_LOW		{gpio_setDir(IIC_SDA, GPIO_DIR_OUTPUT); gpio_clearPin(IIC_SDA);}

#define SCL_HIGH	{gpio_setDir(IIC_SCL, GPIO_DIR_OUTPUT); gpio_setPin(IIC_SCL);}
#define SCL_LOW		{gpio_setDir(IIC_SCL, GPIO_DIR_OUTPUT); gpio_clearPin(IIC_SCL);}

#define SDA_IN		{gpio_setDir(IIC_SDA, GPIO_DIR_INPUT); pad_setPullUpDown(IIC_SDA, PAD_PULLUP);}
#define SDA_VAL     gpio_getPin(IIC_SDA)

#if 1
#define IIC2_SDA 	P_GPIO_12
#define IIC2_SCL 	P_GPIO_15
#else
#define IIC2_SDA 	P_GPIO_12
#define IIC2_SCL 	C_GPIO_13
#endif

#define SDA2_HIGH	{gpio_setDir(IIC2_SDA, GPIO_DIR_OUTPUT); gpio_setPin(IIC2_SDA);}
#define SDA2_LOW	{gpio_setDir(IIC2_SDA, GPIO_DIR_OUTPUT); gpio_clearPin(IIC2_SDA);}

#define SCL2_HIGH	{gpio_setDir(IIC2_SCL, GPIO_DIR_OUTPUT); gpio_setPin(IIC2_SCL);}
#define SCL2_LOW	{gpio_setDir(IIC2_SCL, GPIO_DIR_OUTPUT); gpio_clearPin(IIC2_SCL);}

#define SDA2_IN		{gpio_setDir(IIC2_SDA, GPIO_DIR_INPUT); pad_setPullUpDown(IIC2_SDA, PAD_PULLUP);}
#define SDA2_VAL     gpio_getPin(IIC2_SDA)


#define I2C_GROUD_1		2
#define I2C_GROUD_2		3


static void delay_10us(void)
{

	Delay_DelayUs( 6 );

	//int i;
	//for (i = 0 ; i <= 30; i++);
}


static void Start_I2c (uchar groud)
{
	if(I2C_GROUD_1 == groud) {

		SDA_HIGH							// Set data line high
		delay_10us();

		SCL_HIGH							// Set clock line high
		delay_10us();

		SDA_LOW							// Set data line low (START SIGNAL)
		delay_10us();

		SCL_LOW							// Set clock line low
		delay_10us();
	} else {

		SDA2_HIGH							// Set data line high
		delay_10us();

		SCL2_HIGH							// Set clock line high
		delay_10us();

		SDA2_LOW							// Set data line low (START SIGNAL)
		delay_10us();

		SCL2_LOW							// Set clock line low
		delay_10us();
	}
}


static void Stop_I2c (uchar groud)
{

	if(I2C_GROUD_1 == groud) {

		SDA_LOW							// Set data line low
		delay_10us();

		SCL_HIGH								// Set clock line low
		delay_10us();

		SDA_HIGH							// Set clock line high
		delay_10us();

		SCL_LOW
		delay_10us();

		SDA_IN
		delay_10us();
	} else {

		SDA2_LOW							// Set data line low
		delay_10us();

		SCL2_HIGH								// Set clock line low
		delay_10us();

		SDA2_HIGH							// Set clock line high
		delay_10us();

		SCL2_LOW
		delay_10us();

		SDA2_IN
		delay_10us();
	}
}


/****************产生起始信号*******************/
static void IIC_START(uchar groud)
{
	Start_I2c( groud );
}

/****************产生停止信号*****************/
static void IIC_Stop(uchar groud)
{
	Stop_I2c( groud );
}

#if 0
/*********主设备发出应答(用于读取数据)***************/
static void IIC_Ack(uchar groud)
{
	if(I2C_GROUD_1 == groud) {

		 SDA_LOW
		 delay_10us();
		 SCL_HIGH
		 delay_10us();
		 SCL_LOW
		 delay_10us();
	} else {

		SDA2_LOW
		delay_10us();
		SCL2_HIGH
		delay_10us();
		SCL2_LOW
		delay_10us();
	}
}
#endif

/***********主设备发出无应答(用于读取数据)***************/
static void IIC_NAck(uchar groud)
{
	if(I2C_GROUD_1 == groud) {

		SDA_HIGH
		delay_10us();
		SCL_HIGH
		delay_10us();
		SCL_LOW
		delay_10us();
	} else {

		SDA2_HIGH
		delay_10us();
		SCL2_HIGH
		delay_10us();
		SCL2_LOW
		delay_10us();

	}
}


static UINT32 IIC_Get_Ack(uchar groud) //用于向从机写入数据
{
	 UINT32 Error;

	 if(I2C_GROUD_1 == groud) {

		 SDA_HIGH
		 delay_10us();
		 delay_10us();
		 delay_10us();
		 delay_10us();
		 delay_10us();

		 //delay_10us();//

		 SCL_HIGH
		 delay_10us();

		 delay_10us();//
		 //delay_10us();//
		 //delay_10us();//


		 SDA_IN
		 delay_10us();

		 delay_10us();//
		 delay_10us();//

		 Error = SDA_VAL;

		 delay_10us();

		 //delay_10us();//
		 //delay_10us();//

		 SCL_LOW
		 delay_10us();
	 } else {

		 SDA2_HIGH
		 delay_10us();
		 delay_10us();
		 delay_10us();
		 delay_10us();
		 delay_10us();

		 //delay_10us();//

		 SCL2_HIGH
		 delay_10us();

		 delay_10us();//
		 //delay_10us();//
		 //delay_10us();//


		 SDA2_IN
		 delay_10us();

		 delay_10us();//
		 delay_10us();//

		 Error = SDA2_VAL;

		 delay_10us();

		 //delay_10us();//
		 //delay_10us();//


		 SCL2_LOW
		 delay_10us();
	 }

	 return Error;//反回0为从机应答,反回1为从机无应答。
}


/********************写入一个字节*************************/
static uchar IIC_Write_Byte(uchar dat, uchar groud)
{
	uchar val = 0;

	uchar i;

	if(I2C_GROUD_1 == groud) {


		 for(i=0;i<8;i++)
		 {
			if((dat<<i)&0x80)
				SDA_HIGH
			else
				SDA_LOW

			SCL_HIGH
			delay_10us();
			SCL_LOW
			delay_10us();
		 }
	} else {

		for(i=0;i<8;i++)
		{
		   if((dat<<i)&0x80)
			   SDA2_HIGH
		   else
			   SDA2_LOW

		   SCL2_HIGH
		   delay_10us();
		   SCL2_LOW
		   delay_10us();
		}
	}


	#if 0
	delay_10us();
	delay_10us();
	delay_10us();
	delay_10us();
	#endif

	val = IIC_Get_Ack( groud );
//  IIC_Stop( groud );


  return val;

}

/*********************向从机读取一个字节******************/
static uchar IIC_Read_Byte(uchar groud)
{
 uchar i,rbyte=0;
 UINT32 val = 0;


  if(I2C_GROUD_1 == groud) {

	 SDA_IN

	 for(i=0;i<8;i++)
	 {
		SCL_HIGH
		//delay_10us();

		val =  SDA_VAL;
		if(val) rbyte|=(0x80>>i);

		//if(SDA1) rbyte|=(0x80>>i); //SDA1为输入

		delay_10us();
		SCL_LOW
		delay_10us();
	 }
 } else {

	 SDA2_IN

	 for(i=0;i<8;i++)
	 {
		SCL2_HIGH
		delay_10us();

		val =  SDA2_VAL;
		if(val) rbyte|=(0x80>>i);

		//if(SDA1) rbyte|=(0x80>>i); //SDA1为输入

		delay_10us();
		SCL2_LOW
		delay_10us();
	 }

 }

// IIC_NAck( groud );
// IIC_Stop( groud );


 return rbyte; //反回读取到的值。
}

#if 0
static void write_IIC_3108(uchar addr,uchar dat, uchar groud)
{
	uchar i=0;

	for(i=0;i<3;i++)
	{
		IIC_START( groud );
		IIC_Write_Byte(addr, groud);//写入7位的地址和方向位
		// ACK=IIC_Get_Ack();//判定从机是否应答
	}

	IIC_Write_Byte(dat, groud);
	//while(IIC_Get_Ack());//判定从机是否接收成功,成功ACK否则NACK。
	IIC_Stop( groud );
}
#endif

/**************************************************/
static void write_IIC_3108III(uchar addr,uchar dat, uchar groud)
{

	IIC_START(groud);

	#ifdef KEY_TYPE2
	if(0 == IIC_Write_Byte(((0x08<<1) | 0X00), groud) )
	#else
	if(0 == IIC_Write_Byte(((0x37<<1) | 0X00), groud))
	#endif
	{
	}
	else
	{
		IIC_Stop( groud );
		return;
	}

   	if(0 == IIC_Write_Byte(addr, groud) )
   	{
   	}
	else
	{
		IIC_Stop( groud );
		return;
	}


	if(0 == IIC_Write_Byte(dat, groud))
	{
	}
	else
	{
		IIC_Stop( groud );
		return;
	}


	Delay_DelayMs( 3 );
	//Delay_DelayMs( 5 );

//	if(IIC_Get_Ack(groud)) //判定从机是否接收成功,成功ACK否则NACK。
//	{
//		return TRUE;
//	}
//	else
//	{
		IIC_Stop( groud );
//		return FALSE;
//	}
}

#ifndef KEY_TYPE2
static void write_IIC_3108II(uchar addr,uchar dat, uchar groud)
{
#if 1

	IIC_START(groud);
   	IIC_Write_Byte(addr, groud);
	IIC_Write_Byte(dat, groud);

	IIC_Stop( groud );
#else

	write_IIC_3108III(addr, dat, groud);
#endif
}
#endif


/*************从指定地址读取一个字节*****************/

static uchar Read_IIC_3108III(uchar add, uchar groud)//要读取的地址+方向位。
{
	uchar val = 0;

	IIC_START( groud );


	#ifdef KEY_TYPE2
		IIC_Write_Byte(((0x08<<1) | 0X00), groud);
	#else
		IIC_Write_Byte(((0x37<<1) | 0X00), groud);
	#endif

//	Delay_DelayMs(3);
	IIC_Write_Byte(add, groud);//add=0x6F

//	Delay_DelayMs(3);
	IIC_START( groud );

	delay_10us();delay_10us();delay_10us();

	#ifdef KEY_TYPE2
	IIC_Write_Byte(((0x08<<1) | 0X01), groud);
	#else
	IIC_Write_Byte(((0x37<<1) | 0X01), groud);
	#endif

	//while(IIC_Get_Ack(groud));//判定从机是否接收成功,成功ACK否则NACK。
	val = IIC_Read_Byte(groud);

	IIC_NAck( groud );
	IIC_Stop( groud );

	return val;
}


#ifndef KEY_TYPE2
static uchar Read_IIC_3108II(uchar add, uchar groud)//要读取的地址+方向位。
{

#if 1

	IIC_START( groud );

	IIC_Write_Byte(add, groud);//add=0x6F

	//while(IIC_Get_Ack(groud));//判定从机是否接收成功,成功ACK否则NACK。
	return IIC_Read_Byte(groud);
#else

	return Read_IIC_3108III(add, groud);
#endif
}
#endif


/**********3108的初始化***********/
static void CYBC_3108_INIT(uchar groud)
{
#ifdef KEY_TYPE2

	SCL_HIGH;
	//SCL2_HIGH;

#else

	int i;

	write_IIC_3108II(0x6e, 0, groud);

	for(i=0;i<0x7f;i++)
	{
		write_IIC_3108II(0x6e,i+1, groud);

		//  Read_IIC_3108(0x6f);//0x6f的前7位是地址第0位是方向位。
		Delay_DelayMs(5);
	}

	write_IIC_3108II(0x6e, 0xaa, groud);
#endif
}


/************读取3108的数值(用于检测按键)**************/
static uchar IIC_Read_3108value(uchar groud)
{
	#ifdef KEY_TYPE2
	return Read_IIC_3108III(0x0, groud);
	#else
	return Read_IIC_3108II(0x6f, groud);
	#endif
}

void Set3180_I2C04OFF(void)
{
#ifdef KEY_TYPE2

//	int timeoutnum = 0;

	write_IIC_3108III(COM_REG, POWEROFFCMD, I2C_GROUD_1);
//	while((POWEROFFCMD !=  Read_IIC_3108III(COM_REG, I2C_GROUD_1)) && (timeoutnum<50)) {

	//	write_IIC_3108III(COM_REG, POWEROFFCMD, I2C_GROUD_1);
//		timeoutnum++;
//	}


//	timeoutnum = 0;
//	write_IIC_3108III(COM_REG, POWEROFFCMD, I2C_GROUD_2);
//	while((POWEROFFCMD !=  Read_IIC_3108III(COM_REG, I2C_GROUD_2)) && (timeoutnum<50)) {

//		write_IIC_3108III(COM_REG, POWEROFFCMD, I2C_GROUD_2);
//		timeoutnum++;
	//}
#else

//	write_IIC_3108II(COM_REG, 0xaa, I2C_GROUD_1);
//	write_IIC_3108II(COM_REG, 0xaa, I2C_GROUD_2);
#endif

}

void Get3180_I2C04OFF(void)
{
	uchar result1;//, result2 = 0;

#ifdef KEY_TYPE2

	result1 = Read_IIC_3108III(COM_REG, I2C_GROUD_1);
	//result2 = Read_IIC_3108III(COM_REG, I2C_GROUD_2);
#else

//	result1 = Read_IIC_3108II(COM_REG, I2C_GROUD_1);
//	result2 = Read_IIC_3108II(COM_REG, I2C_GROUD_2);
#endif

//debug_msg("============ Get3180_I2C04OFF  result1: 0x%x  result2:  0x%x \r\n", result1, result2);
}

void Set3180_I2C04ON(void)
{

#ifdef KEY_TYPE2

	write_IIC_3108III(COM_REG, POWERONCMD, I2C_GROUD_1);
	//write_IIC_3108III(COM_REG, POWERONCMD, I2C_GROUD_2);
#else

//	write_IIC_3108II(COM_REG, 0x55, I2C_GROUD_1);
//	write_IIC_3108II(COM_REG, 0x55, I2C_GROUD_2);
#endif

//debug_msg("============Set3180_I2C04ON!\r\n");
}


#endif



//void  ReloadScreenSaveTime(void);


// 全体闪，选中的闪4次，3秒内完成
UINT32 DrvKey_DetNormalKey(void)
{
	UINT32 uiKeyVal = 0;

    static UINT32 uiKeyCode = 0;

	static unsigned char clear_i2c = 0;
	static unsigned char clear_times = 0;

	static unsigned char init_flags = 0;

static UINT32 time_enter = 50;//1000/20; //20ms

	//if(0 == uiKeyCode)
		uiKeyVal = IIC_Read_3108value(I2C_GROUD_2);

	switch(uiKeyVal)
	{
		case 0:
			if(clear_i2c) {

				clear_times++;
				if(clear_times > 8) {
				//if(clear_times > 4) {

					clear_i2c = 0;
					clear_times = 0;

					uiKeyCode = 0;
//debug_msg("==========1 clear \r\n");
				}
			} else {

				uiKeyCode = 0;
			}
			break;

		case 1:
			if(0 == uiKeyCode)
			{
				clear_i2c = 3;
				clear_times = 0;

				uiKeyCode |= FLGKEY_MENU;

				//ReloadScreenSaveTime();
			}
			break;

		case 2:

			if(time_enter < 50) {

			} else {


				if(0 == uiKeyCode)
				{
					clear_i2c = 3;
					clear_times = 0;

				time_enter = 0;

					uiKeyCode |= FLGKEY_ENTER;

					//ReloadScreenSaveTime();
				}
			}
			break;

		case 4:
			if(0 == uiKeyCode)
			{
				clear_i2c = 3;
				clear_times = 0;

				uiKeyCode |= FLGKEY_MODE;

				//ReloadScreenSaveTime();
			}
			break;

		case 8:

			break;;

		default:

//debug_msg("======= 0x%x \r\n");

			//IIC_NAck( I2C_GROUD_2 );
			IIC_Stop( I2C_GROUD_2 );
			break;
	}



	if(!init_flags) {

		init_flags = 3;

//debug_msg("=======init I2C touch key OK!\r\n");
		CYBC_3108_INIT(I2C_GROUD_1);

		CYBC_3108_INIT(I2C_GROUD_2);

		Set3180_I2C04ON();
	}

	time_enter++;
	return uiKeyCode;
}


#if 0
//如果返回 1 带便 PWR 按键按下，否则没按键触发
unsigned char Get3180_PWRKeyStatus(void)
{

	return Read_IIC_3108III(0x00, I2C_GROUD_1);
}
#endif


/**
  Detect power key is pressed or not.

  Detect power key is pressed or not.
  Return key pressed status (refer to KeyDef.h)

  @param void
  @return UINT32
*/

static volatile BOOL powerKeyClick = FALSE;
//static unsigned char clockNum = 0;

UINT32 DrvKey_DetPowerKey(void)
{
    static UINT32 uiKeyCode = 0;

	UINT32 uiKeyVal = 0;

	static unsigned char clear_i2c1 = 0;
	static unsigned char clear_times1 = 0;

//debug_msg("===============DrvKey_DetPowerKey!  \r\n");
	static unsigned char init_flags = 0;

	if(0 == uiKeyCode)
		uiKeyVal = IIC_Read_3108value(I2C_GROUD_1);

	switch(uiKeyVal)
	{
		case 0:

//debug_msg("===============key 0!  \r\n");
			if(clear_i2c1) {

				clear_times1++;
				//if(clear_times1 > 8) {
				if(clear_times1 > 2) {

					clear_i2c1 = 0;
					clear_times1 = 0;
					uiKeyCode = 0;
				}
			} else {

				uiKeyCode = 0;
			}
			break;

		case 1:
			if(0 == uiKeyCode) {

				clear_i2c1 = 3;
				clear_times1 = 0;

				uiKeyCode |= FLGKEY_CUSTOM1;
				//ReloadScreenSaveTime();
			}

			//if(clockNum < 10) {
			//	clockNum++;
				powerKeyClick = TRUE;
			//}
			break;

		case 2:
			if(0 == uiKeyCode) {

				clear_i2c1 = 3;
				clear_times1 = 0;

				uiKeyCode |= FLGKEY_UP;
				//ReloadScreenSaveTime();
			}
			break;

		case 4:
			if(0 == uiKeyCode) {

				clear_i2c1 = 3;
				clear_times1 = 0;

				uiKeyCode |= FLGKEY_DOWN;
				//ReloadScreenSaveTime();
			}
			break;

		case 8:
			if(0 == uiKeyCode) {

				clear_i2c1 = 3;
				clear_times1 = 0;

				uiKeyCode |= FLGKEY_SHUTTER1;
				//ReloadScreenSaveTime();
			}
			break;

		default:
//debug_msg("erro code:  0x%x \r\n", uiKeyVal);
			//IIC_NAck( I2C_GROUD_1 );
			IIC_Stop( I2C_GROUD_1 );
			break;
	}

	if(init_flags < 5) {

		init_flags++;

		Set3180_I2C04ON();
	}

    return uiKeyCode;
}

UINT32 DrvKey_DetStatusKey(DX_STATUS_KEY_GROUP KeyGroup)
{
    UINT32 uiReturn = STATUS_KEY_LVL_UNKNOWN;

    switch (KeyGroup)
    {
        case DX_STATUS_KEY_GROUP1:
#if 0//(ADC_KEY == ENABLE)
            uiReturn = VolDet_ModeSwitch();
#endif
            break;

        case DX_STATUS_KEY_GROUP2:
            break;

        case DX_STATUS_KEY_GROUP3:
            break;

        case DX_STATUS_KEY_GROUP4:
            break;

        case DX_STATUS_KEY_GROUP5:
            break;

        default:
            DBG_ERR("[StatusKey]no this attribute");
            break;
    }

	return uiReturn;
}

#if 0
BOOL Drv_HwPower_GetPowerOnState(void) {


//debug_msg("========= POWER_STATE_FIRST:  %d \r\n", HwPower_GetPowerOnState( POWER_STATE_FIRST ));
//debug_msg("========= POWER_STATE_SRC:  %d \r\n", HwPower_GetPowerOnState( POWER_STATE_SRC ));

	debug_msg("========= Drv_HwPower_GetPowerOnState:  %d \r\n", powerKeyClick);

	return powerKeyClick;
	//return HwPower_GetPowerOnState( POWER_STATE_SRC );
}
#endif

