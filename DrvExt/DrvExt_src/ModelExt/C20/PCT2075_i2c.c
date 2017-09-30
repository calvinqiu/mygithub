//////////////////////////////////////////


#include "Type.h"
#include "Debug.h"
#include "i2c.h"
#include "PCT2075_i2c.h"
#include "Utility.h"

///////////////////////////////////////////////////////////////////////////////
#define __MODULE__          DxDrv
#define __DBGLVL__          1 // 0=OFF, 1=ERROR, 2=TRACE
#define __DBGFLT__          "*" //*=All, [mark]=CustomClass
#include "DebugModule.h"
///////////////////////////////////////////////////////////////////////////////

/*

Base on DEMO circuit layout (provided by MS), TempIC use:
    I2C_SDA / P_GPIO[0]*   PWR_SDA
    I2C_SCL / P_GPIO[1]*   PWR_SCL

Top:
    PIN_I2C_CFG_CH1 = 0x01,             ///< Enable channel 1 (on PGPIO[0..1])
    PIN_I2C_CFG_CH1_2ND_PINMUX = 0x02,  ///< Enable 2nd pinmux of channel 1 (on PGPIO[57..58])
    PIN_I2C_CFG_CH2 = 0x10,             ///< Enable channel 2 (on PGPIO[28..29])
    PIN_I2C_CFG_CH2_2ND_PINMUX = 0x20,  ///< Enable 2nd pinmux of channel 2 (on SGPIO[26..27])

>> This TempIC use "I2C CH1 first pinmux" = PIN_I2C_CFG_CH1

PinmuxCfg.c
>>  {PIN_FUNC_I2C,              PIN_I2C_CFG_CH1},

Driver:
    call i2c_open()
*/
#define I2C_SLAVE_ADDRESS 0x48  //1001000
#define TEMPIC_READ_ADDRESS (i2c_conv7bSARToData(I2C_SLAVE_ADDRESS) | 0x1)
#define TEMPIC_WRITE_ADDRESS (i2c_conv7bSARToData(I2C_SLAVE_ADDRESS) | 0x0)


static TEMPIC_INFO g_TempIcInfo;
/**
  Initialize Temp IIC bus

  Initialize Temp IIC bus

  @param void
  @return void
*/
BOOL TempIC_I2C_Init(void)
{
    UINT erReturn;

    g_TempIcInfo.I2C_RegBytes = TEMPIC_I2C_REGISTER_2BYTE;
    g_TempIcInfo.I2C_PinMux = I2C_PINMUX_1ST;
    g_TempIcInfo.I2C_BusClock = I2C_BUS_CLOCK_400KHZ;
    g_TempIcInfo.I2C_Slave_WAddr = TEMPIC_WRITE_ADDRESS;
    g_TempIcInfo.I2C_Slave_RAddr = TEMPIC_READ_ADDRESS;

    if (g_TempIcInfo.I2C_RegBytes > TEMPIC_I2C_REGISTER_2BYTE)
    {
        DBG_ERR("Error register Byte Count !!\r\n");
        return FALSE;
    }

    if (g_TempIcInfo.I2C_PinMux > I2C_PINMUX_2ND)
    {
        DBG_ERR("Error I2C Pin mux !!\r\n");
        return FALSE;
    }

    if (g_TempIcInfo.I2C_BusClock > I2C_BUS_CLOCK_1MHZ)
    {
        DBG_ERR("Error I2C Bus Clock !!\r\n");
        return FALSE;
    }

    erReturn = i2c_open(&(g_TempIcInfo.I2C_Channel));
    if(erReturn != I2C_STS_OK)
    {
        DBG_ERR("Error open I2C driver!!\r\n");
        return FALSE;
    }

    i2c_setConfig(g_TempIcInfo.I2C_Channel, I2C_CONFIG_ID_PINMUX, g_TempIcInfo.I2C_PinMux); //select to control I2C 1st device
    i2c_setConfig(g_TempIcInfo.I2C_Channel, I2C_CONFIG_ID_BUSCLOCK, g_TempIcInfo.I2C_BusClock); //up to 400KHZ

    return TRUE;
}
static I2C_STS TempIC_I2C_Receive(UINT32 *value, BOOL bNACK, BOOL bStop)
{
    I2C_DATA I2cData;
    I2C_BYTE I2cByte[1];
    I2C_STS ret;

    I2cData.VersionInfo = DRV_VER_96660;
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
        DBG_ERR("i2c ret = %02x!!\r\n", ret);
    }

    *value = I2cByte[0].uiValue;

    return ret;
}
static I2C_STS TempIC_I2C_Transmit(UINT32 value, BOOL bStart, BOOL bStop)
{
    I2C_DATA I2cData;
    I2C_BYTE I2cByte[1];
    I2C_STS ret;

    I2cData.VersionInfo = DRV_VER_96660;
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
            DBG_ERR("i2c ret = %d!!\r\n", ret);
    }
    return ret;
}
#if 0
static void TempIC_I2C_WriteReg(UINT32 uiAddr, UINT32 uiValue)
{
    UINT        erReturn;
    UINT32      ulWriteAddr, ulReg1, ulData;
    UINT32      ulReg2 = 0;

    i2c_lock(g_TempIcInfo.I2C_Channel);

    ulWriteAddr = g_TempIcInfo.I2C_Slave_WAddr;
    if (g_TempIcInfo.I2C_RegBytes == TEMPIC_I2C_REGISTER_2BYTE)
    {
        ulReg1      = (uiAddr>>8);
        ulReg2      = ((uiAddr)&(0x000000ff));
    } else {
        ulReg1      = ((uiAddr)&(0x000000ff));
    }
    ulData      = ((uiValue)&(0x000000ff));
    do
    {
        erReturn=TempIC_I2C_Transmit(ulWriteAddr, 1, 0);
        if (erReturn != I2C_STS_OK)
        {
            DBG_ERR("Temp Error transmit data (write addr)!!\r\n");
            break;
        }

        erReturn=TempIC_I2C_Transmit(ulReg1, 0, 0);
        if (erReturn != I2C_STS_OK)
        {
            DBG_ERR("Temp Error transmit data (reg) !!\r\n");
            break;
        }

        if (g_TempIcInfo.I2C_RegBytes == TEMPIC_I2C_REGISTER_2BYTE)
        {
            erReturn=TempIC_I2C_Transmit(ulReg2, 0, 0);
            if (erReturn != I2C_STS_OK)
            {
                DBG_ERR("Temp Error transmit data (reg) !!\r\n");
                break;
            }
        }

        erReturn=TempIC_I2C_Transmit(ulData, 0, 1);
        if (erReturn != I2C_STS_OK)
        {
            DBG_ERR("Temp Error transmit data (data1) !!\r\n");
            break;
        }
    } while(erReturn != I2C_STS_OK);

    i2c_unlock(g_TempIcInfo.I2C_Channel);
}
#endif

static UINT32 TempIC_I2C_ReadReg(UINT32 uiAddr)
{
    UINT32      ulWriteAddr, ulReadAddr, ulReg1;
    static UINT32 ulData1,ulData2;

    ulWriteAddr =g_TempIcInfo.I2C_Slave_WAddr;
    ulReadAddr  =g_TempIcInfo.I2C_Slave_RAddr;
    ulReg1      =(uiAddr&0x0000FFFF);
    ulData1     = 0;

    if(i2c_lock(g_TempIcInfo.I2C_Channel) != E_OK)
    {
        DBG_ERR("TempIC: readReg I2C Lock Fail\r\n");
        return ulData1;
    }

    if (TempIC_I2C_Transmit(ulWriteAddr, 1, 0) != I2C_STS_OK)
    {
        DBG_ERR("Error transmit data1!!\r\n");
        i2c_unlock(g_TempIcInfo.I2C_Channel);
        return  ulData1;
    }

    if (TempIC_I2C_Transmit(ulReg1, 0, 0) != I2C_STS_OK)
    {
        DBG_ERR("Error transmit data2!!\r\n");
        i2c_unlock(g_TempIcInfo.I2C_Channel);
        return  ulData1;
    }

    if (TempIC_I2C_Transmit(ulReadAddr, 1, 0) != I2C_STS_OK)
    {
        DBG_ERR("Error transmit data3!!\r\n");
        i2c_unlock(g_TempIcInfo.I2C_Channel);
        return  ulData1;
    }

    if (TempIC_I2C_Receive(&ulData1, 0, 0) != I2C_STS_OK)
    {
        DBG_ERR("Error Receive data!!\r\n");
        i2c_unlock(g_TempIcInfo.I2C_Channel);
        return  ulData1;
    }
#if 1
    if (TempIC_I2C_Receive(&ulData2, 1, 1) != I2C_STS_OK)
    {
        DBG_ERR("Error Receive data!!\r\n");
        i2c_unlock(g_TempIcInfo.I2C_Channel);
        return  ulData2;
    }
    //debug_msg("ulData1=%x,ulData2=%x\r\n",ulData1,ulData2);
#endif
    if(i2c_unlock(g_TempIcInfo.I2C_Channel) != E_OK)
    {
        DBG_ERR("TempIC: readReg I2C UnLock Fail\r\n");
        return  ulData1;
    }

    return  ((ulData1<<3)|(ulData2>>5));//ulData1;
}
UINT32 TempIC_GetTempValue(void)
{
    UINT32 u32Temp;
    u32Temp=TempIC_I2C_ReadReg(0x00);
    //debug_msg("TempIC_I2C_ReadReg(0x00)=%x\r\n",u32Temp);
    if(u32Temp&0x400)
    {
        u32Temp=-((((~u32Temp)&0x3FF)+1)*0.125);
    }
    else
    {
        u32Temp=u32Temp*0.125;
    }
    return u32Temp;
}
