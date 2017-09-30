

#include "DxPmu.h"

#define __MODULE__          PMU
//#define __DBGLVL__ 0        //OFF mode, show nothing
//#define __DBGLVL__ 1        //ERROR mode, show err, wrn only
#define __DBGLVL__ 1        //TRACE mode, show err, wrn, ind, msg and func and ind, msg and func can be filtering by __DBGFLT__ settings
#define __DBGFLT__ "*"      //*=All
#include "DebugModule.h"

static PMU_INFO g_PmuInfo;

BOOL Pmu_I2C_Init(PMU_INFO PmuInfo)
{
    UINT erReturn;

    g_PmuInfo.I2C_RegBytes = PmuInfo.I2C_RegBytes;
    g_PmuInfo.I2C_PinMux = PmuInfo.I2C_PinMux;
    g_PmuInfo.I2C_BusClock = PmuInfo.I2C_BusClock;
    g_PmuInfo.I2C_Slave_WAddr = PmuInfo.I2C_Slave_WAddr;
    g_PmuInfo.I2C_Slave_RAddr = PmuInfo.I2C_Slave_RAddr;

    if (g_PmuInfo.I2C_RegBytes > PMU_I2C_REGISTER_2BYTE)
    {
        debug_msg("Error register Byte Count !!\r\n");
        return FALSE;
    }

    if (g_PmuInfo.I2C_PinMux > I2C_PINMUX_2ND)
    {
        debug_msg("Error I2C Pin mux !!\r\n");
        return FALSE;
    }

    if (g_PmuInfo.I2C_BusClock > I2C_BUS_CLOCK_1MHZ)
    {
        debug_msg("Error I2C Bus Clock !!\r\n");
        return FALSE;
    }

    erReturn = i2c_open(&(g_PmuInfo.I2C_Channel));
    if(erReturn != I2C_STS_OK)
    {
        debug_msg(("Error open I2C driver!!\r\n"));
        return FALSE;
    }

    i2c_setConfig(g_PmuInfo.I2C_Channel, I2C_CONFIG_ID_PINMUX, g_PmuInfo.I2C_PinMux); //select to control I2C 1st device
    i2c_setConfig(g_PmuInfo.I2C_Channel, I2C_CONFIG_ID_BUSCLOCK, g_PmuInfo.I2C_BusClock); //up to 400KHZ

    return TRUE;
}

static I2C_STS Pmu_I2C_Receive(UINT32 *value, BOOL bNACK, BOOL bStop)
{
	I2C_DATA I2cData;
	I2C_BYTE I2cByte[1];
	I2C_STS ret;

	I2cData.VersionInfo = DRV_VER_96650;
	I2cData.ByteCount = I2C_BYTE_CNT_1;
	I2cData.pByte = I2cByte;
	I2cByte[0].Param = I2C_BYTE_PARAM_NONE;
	if ( bNACK == TRUE )
		I2cByte[0].Param |= I2C_BYTE_PARAM_NACK;
	if ( bStop == TRUE )
		I2cByte[0].Param |= I2C_BYTE_PARAM_STOP;

	ret = i2c_receive(&I2cData);
	if ( ret != I2C_STS_OK )
	{
		debug_msg("%s i2c ret = %02x!!\r\n",__func__, ret);
	}

	*value = I2cByte[0].uiValue;

	return ret;
}

static I2C_STS Pmu_I2C_Transmit(UINT32 value, BOOL bStart, BOOL bStop)
{
	I2C_DATA I2cData;
	I2C_BYTE I2cByte[1];
	I2C_STS ret;

	I2cData.VersionInfo = DRV_VER_96650;
	I2cData.ByteCount = I2C_BYTE_CNT_1;
	I2cData.pByte = I2cByte;
	I2cByte[0].uiValue = value & 0xff;
	I2cByte[0].Param = I2C_BYTE_PARAM_NONE;
	if ( bStart == TRUE )
		I2cByte[0].Param |= I2C_BYTE_PARAM_START;
	if ( bStop == TRUE )
		I2cByte[0].Param |= I2C_BYTE_PARAM_STOP;

	DBG_IND("I2cData VersionInfo=0x%x, ByteCount =0x%x, uiValue=0x%x, parm=0x%x\r\n",I2cData.VersionInfo,I2cData.ByteCount,I2cByte[0].uiValue,I2cByte[0].Param);
	ret = i2c_transmit(&I2cData);
	if ( ret != I2C_STS_OK )
	{
			debug_msg("%s i2c ret = %d!!\r\n",__func__, ret);
	}
	return ret;
}

void Pmu_I2C_WriteReg(UINT32 uiAddr, UINT32 uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1, ulData;
    UINT32      ulReg2 = 0;

    i2c_lock(g_PmuInfo.I2C_Channel);

    ulWriteAddr = g_PmuInfo.I2C_Slave_WAddr;
    if (g_PmuInfo.I2C_RegBytes == PMU_I2C_REGISTER_2BYTE)
    {
        ulReg1      = (uiAddr>>8);
        ulReg2      = ((uiAddr)&(0x000000ff));
    } else {
        ulReg1      = ((uiAddr)&(0x000000ff));
    }
    ulData      = ((uiValue)&(0x000000ff));
    do
    {
        erReturn=Pmu_I2C_Transmit(ulWriteAddr, 1, 0);
        if (erReturn != I2C_STS_OK)
        {
            debug_msg("Pmu Error transmit data (write addr)!!\r\n");
            break;
        }

        erReturn=Pmu_I2C_Transmit(ulReg1, 0, 0);
        if (erReturn != I2C_STS_OK)
        {
            debug_msg("Pmu Error transmit data (reg) !!\r\n");
            break;
        }

        if (g_PmuInfo.I2C_RegBytes == PMU_I2C_REGISTER_2BYTE)
        {
            erReturn=Pmu_I2C_Transmit(ulReg2, 0, 0);
            if (erReturn != I2C_STS_OK)
            {
                debug_msg("Pmu Error transmit data (reg) !!\r\n");
                break;
            }
        }

        erReturn=Pmu_I2C_Transmit(ulData, 0, 1);
        if (erReturn != I2C_STS_OK)
        {
            debug_msg("Pmu Error transmit data (data1) !!\r\n");
            break;
        }
    } while(erReturn != I2C_STS_OK);

    i2c_unlock(g_PmuInfo.I2C_Channel);
}

UINT32 Pmu_I2C_ReadReg(UINT32 uiAddr)
{
	UINT32      ulWriteAddr, ulReadAddr, ulReg1;
	static UINT32 ulData1;

    ulWriteAddr =g_PmuInfo.I2C_Slave_WAddr;
    ulReadAddr  =g_PmuInfo.I2C_Slave_RAddr;
    ulReg1      =(uiAddr&0x000000ff);
    ulData1     = 0;

    if(i2c_lock(g_PmuInfo.I2C_Channel) != E_OK)
    {
        debug_msg("Pmu: readReg I2C Lock Fail\r\n");
        return ulData1;
    }

    if (Pmu_I2C_Transmit(ulWriteAddr, 1, 0) != I2C_STS_OK)
    {
        debug_msg("Pmu: Error transmit data1!!\r\n");
        i2c_unlock(g_PmuInfo.I2C_Channel);
        return  ulData1;
    }

    if (Pmu_I2C_Transmit(ulReg1, 0, 0) != I2C_STS_OK)
    {
        debug_msg("Pmu: Error transmit data2!!\r\n");
        i2c_unlock(g_PmuInfo.I2C_Channel);
        return  ulData1;
    }

    if (Pmu_I2C_Transmit(ulReadAddr, 1, 0) != I2C_STS_OK)
    {
        debug_msg("Pmu: Error transmit data3!!\r\n");
        i2c_unlock(g_PmuInfo.I2C_Channel);
        return  ulData1;
    }

    if (Pmu_I2C_Receive(&ulData1, 1, 1) != I2C_STS_OK)
    {
        debug_msg("Pmu: Error Receive data!!\r\n");
        i2c_unlock(g_PmuInfo.I2C_Channel);
        return  ulData1;
    }

    if(i2c_unlock(g_PmuInfo.I2C_Channel) != E_OK)
    {
        debug_msg("Pmu: readReg I2C UnLock Fail\r\n");
        return  ulData1;
    }

    return  ulData1;
}


BOOL PMU_Init(void)
{
	PMU_INFO PmuInfo;
	UINT32 Temp = 0;
	
    PmuInfo.I2C_RegBytes = PMU_I2C_REGISTER_1BYTE;
    PmuInfo.I2C_PinMux = I2C_PINMUX_1ST;
    PmuInfo.I2C_BusClock = I2C_BUS_CLOCK_400KHZ;
    PmuInfo.I2C_Slave_WAddr = 0x68;
    PmuInfo.I2C_Slave_RAddr = 0x69;
	
    if (Pmu_I2C_Init(PmuInfo) == TRUE)
    {
    	Pmu_I2C_WriteReg(0x33,0xa2);
    	Pmu_I2C_WriteReg(0x19,0x19);
    	Pmu_I2C_WriteReg(0x21,0x0f);
    	Pmu_I2C_WriteReg(0x36,0x09);
    	Pmu_I2C_WriteReg(0x40,0x00);
    	Pmu_I2C_WriteReg(0x41,0x00);
    	Pmu_I2C_WriteReg(0x42,0x00);
    	Pmu_I2C_WriteReg(0x43,0x00);
    	Pmu_I2C_WriteReg(0x44,0x00);
    	Pmu_I2C_WriteReg(0x8F,0x81);

    	Pmu_I2C_WriteReg(0x92,0x03);	//GPIO1 功能设置
    	Pmu_I2C_WriteReg(0x93,0x0b);	//为LDO模式及输出高电平设置

		//Delay_DelayMs(20);		
		Temp = Pmu_I2C_ReadReg(0x10);
    	Pmu_I2C_WriteReg(0x10,Temp|0x10);	//DCDC4 开关控制
		//Delay_DelayMs(5);
		Temp = Pmu_I2C_ReadReg(0x10);
    	Pmu_I2C_WriteReg(0x10,Temp|0x08);	//DCDC3 开关控制
		//Delay_DelayMs(5);
		Temp = Pmu_I2C_ReadReg(0x12);
    	Pmu_I2C_WriteReg(0x12,Temp|0x20);	//ALDO3 开关控制
		
		#if 0
		//-----------------输入电源状态--------------------------		
    	debug_msg("Pmu Reg00 = 0x%x \r\n",Pmu_I2C_ReadReg(0x00));
		//-------------------------------------------------------
		//-----------------电源工作模式以及充电状态提示----------		
    	debug_msg("Pmu Reg01 = 0x%x \r\n",Pmu_I2C_ReadReg(0x01));
		//-------------------------------------------------------		
		
		//-----------------电源输出控制--------------------------
    	debug_msg("Pmu Reg10 = 0x%x \r\n",Pmu_I2C_ReadReg(0x10));
    	debug_msg("Pmu Reg12 = 0x%x \r\n",Pmu_I2C_ReadReg(0x12));
    	debug_msg("Pmu Reg13 = 0x%x \r\n",Pmu_I2C_ReadReg(0x13));
		//-------------------------------------------------------	

		//-----------------DCDC1输出电压设置---------------------
    	debug_msg("Pmu Reg21 = 0x%x \r\n",Pmu_I2C_ReadReg(0x21));
		//-------------------------------------------------------
		//-----------------DCDC2输出电压设置---------------------
    	debug_msg("Pmu Reg22 = 0x%x \r\n",Pmu_I2C_ReadReg(0x22));
		//-------------------------------------------------------
		//-----------------DCDC3输出电压设置---------------------
    	debug_msg("Pmu Reg23 = 0x%x \r\n",Pmu_I2C_ReadReg(0x23));
		//-------------------------------------------------------
		//-----------------DCDC4输出电压设置---------------------
    	debug_msg("Pmu Reg24 = 0x%x \r\n",Pmu_I2C_ReadReg(0x24));
		//-------------------------------------------------------

		
		//----------------VBUS-IPSOUT 通路管理-------------------
    	debug_msg("Pmu Reg30 = 0x%x \r\n",Pmu_I2C_ReadReg(0x30));
		//-------------------------------------------------------		
		//-----------关机设置，电池检测以及CHGLED管脚控制--------
    	debug_msg("Pmu Reg32 = 0x%x \r\n",Pmu_I2C_ReadReg(0x32));
		//-------------------------------------------------------
		//----------------充电控制1 & 2 & 3----------------------
    	debug_msg("Pmu Reg33 = 0x%x \r\n",Pmu_I2C_ReadReg(0x33));
    	debug_msg("Pmu Reg34 = 0x%x \r\n",Pmu_I2C_ReadReg(0x34));
    	debug_msg("Pmu Reg35 = 0x%x \r\n",Pmu_I2C_ReadReg(0x35));
		//-------------------------------------------------------
		//-----------------PEK按键参数设置-----------------------
    	debug_msg("Pmu Reg36 = 0x%x \r\n",Pmu_I2C_ReadReg(0x36));	
		//-------------------------------------------------------
		//-----------------OFFLEVEL关机延时设置------------------
    	debug_msg("Pmu Reg37 = 0x%x \r\n",Pmu_I2C_ReadReg(0x37));
		//-------------------------------------------------------

		//-----------------电池充电放电高低温门限设置------------
    	debug_msg("Pmu Reg38 = 0x%x \r\n",Pmu_I2C_ReadReg(0x38));
    	debug_msg("Pmu Reg39 = 0x%x \r\n",Pmu_I2C_ReadReg(0x39));
    	debug_msg("Pmu Reg3C = 0x%x \r\n",Pmu_I2C_ReadReg(0x3C));
    	debug_msg("Pmu Reg3D = 0x%x \r\n",Pmu_I2C_ReadReg(0x3D));
		//-------------------------------------------------------
		//-------------------IRQ使能1 2 3 4 5--------------------
    	debug_msg("Pmu Reg40 = 0x%x \r\n",Pmu_I2C_ReadReg(0x40));
    	debug_msg("Pmu Reg41 = 0x%x \r\n",Pmu_I2C_ReadReg(0x41));
    	debug_msg("Pmu Reg42 = 0x%x \r\n",Pmu_I2C_ReadReg(0x42));
    	debug_msg("Pmu Reg43 = 0x%x \r\n",Pmu_I2C_ReadReg(0x43));
    	debug_msg("Pmu Reg44 = 0x%x \r\n",Pmu_I2C_ReadReg(0x44));
		//-------------------------------------------------------
		//-------------------IRQ状态1 2 3 4 5--------------------
    	debug_msg("Pmu Reg48 = 0x%x \r\n",Pmu_I2C_ReadReg(0x48));
    	debug_msg("Pmu Reg49 = 0x%x \r\n",Pmu_I2C_ReadReg(0x49));
    	debug_msg("Pmu Reg4A = 0x%x \r\n",Pmu_I2C_ReadReg(0x4A));
    	debug_msg("Pmu Reg4B = 0x%x \r\n",Pmu_I2C_ReadReg(0x4B));
    	debug_msg("Pmu Reg4C = 0x%x \r\n",Pmu_I2C_ReadReg(0x4C));		
		//-------------------------------------------------------
		
		//-------------------内部温度监测------------------------
    	debug_msg("Pmu Reg56 = 0x%x \r\n",Pmu_I2C_ReadReg(0x56));
    	debug_msg("Pmu Reg57 = 0x%x \r\n",Pmu_I2C_ReadReg(0x57));
		//-------------------------------------------------------
		//-------------------电池电压----------------------------		
    	debug_msg("Pmu Reg78 = 0x%x \r\n",Pmu_I2C_ReadReg(0x78));
    	debug_msg("Pmu Reg79 = 0x%x \r\n",Pmu_I2C_ReadReg(0x79));
		//-------------------------------------------------------
		//----------------输出电压低压保护控制-------------------
    	debug_msg("Pmu Reg81 = 0x%x \r\n",Pmu_I2C_ReadReg(0x81));
		//-------------------------------------------------------
		//----------------过温关机等功能设置---------------------
    	debug_msg("Pmu Reg8F = 0x%x \r\n",Pmu_I2C_ReadReg(0x8F));
		//-------------------------------------------------------

		//----------------电池电量指示---------------------------
    	debug_msg("Pmu RegB9 = 0x%x \r\n",Pmu_I2C_ReadReg(0xB9));
		//-------------------------------------------------------

		//---------电池总容量设置1 & 2---------------------------
    	debug_msg("Pmu RegE0 = 0x%x \r\n",Pmu_I2C_ReadReg(0xE0));
    	debug_msg("Pmu RegE1 = 0x%x \r\n",Pmu_I2C_ReadReg(0xE1));
		//-------------------------------------------------------
		#endif
        debug_msg("-----------------------  PMU Init succeed !!\r\n");
        return TRUE;
    } else {
        debug_msg("-----------------------  PMU Init failed !!\r\n");
        return FALSE;
    }
}

void PMU_Off(void)
{
	UINT32 temp = 0;
	debug_msg("-----------------------	PMU_Off !!\r\n");
	temp = Pmu_I2C_ReadReg(0x32);
	Pmu_I2C_WriteReg(0x32,temp|0x80);
}
#if 0
UINT8 PMU_GetBatteryLevel(void)
{
	UINT8 temp = 0;
	UINT8 CurBattery = 0;
	static UINT8 LastBattery = 0;
	
	temp = Pmu_I2C_ReadReg(0xB9);
	if(temp&0x80)
	{
		CurBattery = temp&0xff;
	}
	else
	{
		CurBattery = LastBattery;
	}
	LastBattery = CurBattery;
	debug_msg("%s CurBattery = %d \r\n",__func__,CurBattery);
	return CurBattery;
}
#else
static  int axp_vbat_to_mV(UINT16 reg)
{
	return (reg) * 1100 / 1000;
}

static int axp_ibat_to_mA(UINT16 reg)
{
	return (reg) * 500 / 1000;
}

static int axp_bat_vol(UINT8 dir,int vol,int cur,int rdc)
{
	if(dir)
		return (vol-cur*rdc/1000);
	else
		return (vol+cur*rdc/1000);
}

static UINT8 axp_vol2cap(int ocv)
{
    if(ocv >= OCVVOLF)
    {
        return OCVREGF;
    }
    else if(ocv < OCVVOL0)
    {
        return OCVREG0;
    }
    else if(ocv < OCVVOL1)
    {
        return OCVREG0 + (OCVREG1 - OCVREG0) * (ocv - OCVVOL0) / (OCVVOL1 - OCVVOL0);
    }
    else if(ocv < OCVVOL2)
    {
        return OCVREG1 + (OCVREG2 - OCVREG1) * (ocv - OCVVOL1) / (OCVVOL2 - OCVVOL1);
    }
    else if(ocv < OCVVOL3)
    {
        return OCVREG2 + (OCVREG3 - OCVREG2) * (ocv - OCVVOL2) / (OCVVOL3 - OCVVOL2);
    }
    else if(ocv < OCVVOL4)
    {
        return OCVREG3 + (OCVREG4 - OCVREG3) * (ocv - OCVVOL3) / (OCVVOL4 - OCVVOL3);
    }
    else if(ocv < OCVVOL5)
    {
        return OCVREG4 + (OCVREG5 - OCVREG4) * (ocv - OCVVOL4) / (OCVVOL5 - OCVVOL4);
    }
    else if(ocv < OCVVOL6)                               
    {
        return OCVREG5 + (OCVREG6 - OCVREG5) * (ocv - OCVVOL5) / (OCVVOL6 - OCVVOL5);
    }
    else if(ocv < OCVVOL7)
    {
        return OCVREG6 + (OCVREG7 - OCVREG6) * (ocv - OCVVOL6) / (OCVVOL7 - OCVVOL6);
    }
    else if(ocv < OCVVOL8)
    {
        return OCVREG7 + (OCVREG8 - OCVREG7) * (ocv - OCVVOL7) / (OCVVOL8 - OCVVOL7);
    }
    else if(ocv < OCVVOL9)
    {
        return OCVREG8 + (OCVREG9 - OCVREG8) * (ocv - OCVVOL8) / (OCVVOL9 - OCVVOL8);
    }
    else if(ocv < OCVVOLA)
    {
        return OCVREG9 + (OCVREGA - OCVREG9) * (ocv - OCVVOL9) / (OCVVOLA - OCVVOL9);
    }
    else if(ocv < OCVVOLB)
    {
        return OCVREGA + (OCVREGB - OCVREGA) * (ocv - OCVVOLA) / (OCVVOLB - OCVVOLA);
    }
    else if(ocv < OCVVOLC)
    {
        return OCVREGB + (OCVREGC - OCVREGB) * (ocv - OCVVOLB) / (OCVVOLC - OCVVOLB);
    }
    else if(ocv < OCVVOLD)
    {
        return OCVREGC + (OCVREGD - OCVREGC) * (ocv - OCVVOLC) / (OCVVOLD - OCVVOLC);
    }
    else if(ocv < OCVVOLE)
    {
        return OCVREGD + (OCVREGE - OCVREGD) * (ocv - OCVVOLD) / (OCVVOLE - OCVVOLD);
    }
    else if(ocv < OCVVOLF)
    {
        return OCVREGE + (OCVREGF - OCVREGE) * (ocv - OCVVOLE) / (OCVVOLF - OCVVOLE);
    }
    else
    {
        return 0;
    }
}

 //电量计量函数，返回值为电量
UINT8 PMU_GetBatteryLevel(void)//UINT8 axp_ocv_restcap(void)
{
	int ocv,vbat,ibat ;
	UINT8 val1,val2,bat_current_direction; 
	UINT16 tmp1,tmp2;

	val1 = Pmu_I2C_ReadReg(0x00);//axp_read(0x00,val[0]);	 //	读00寄存器
	bat_current_direction=	val1 && (1<<2) ;//取00寄存器的bit2

	val1 = Pmu_I2C_ReadReg(0x78);//axp_reads(0x78,2,val);	 //	读78,79寄存器 ,计算电池电压
	val2 = Pmu_I2C_ReadReg(0x79);
	tmp1=((UINT16) val1 << 4 )| (val2 & 0x0f);	  //adc->vbat_res
	vbat=  axp_vbat_to_mV(tmp1);

	val1 = Pmu_I2C_ReadReg(0x7A);//axp_reads(0x7A,2,val);//读7A 7B 寄存器计算充电电流
	val2 = Pmu_I2C_ReadReg(0x7B);
	tmp1 =((UINT16) val1 << 5 )| (val2 & 0x1f);//adc->ichar_res
	val1 = Pmu_I2C_ReadReg(0x7C);//axp_reads(0x7C,2,val);	//读7C 7D 寄存器计算放电电流
	val2 = Pmu_I2C_ReadReg(0x7D);
	tmp2 =((UINT16) val1 << 5 )| (val2 & 0x1f);//adc->idischar_res
	ibat= ABS(axp_ibat_to_mA(tmp1)-axp_ibat_to_mA(tmp2));

	ocv = axp_bat_vol(bat_current_direction,vbat,ibat,100);
	
	return axp_vol2cap(ocv);
}
#endif

