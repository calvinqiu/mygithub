#include "i2c.h"

#define I2C_SLAVE_ADDRESS 0x12  //0010010
#define PWRIC_READ_ADDRESS (i2c_conv7bSARToData(I2C_SLAVE_ADDRESS) | 0x1)
#define PWRIC_WRITE_ADDRESS (i2c_conv7bSARToData(I2C_SLAVE_ADDRESS) | 0x0)

extern BOOL PwrIC_Open(void);
extern BOOL PwrIC_Close(void);
extern BOOL PwrIC_DumpAll(void);
extern BOOL PwrIC_DoCmd(const UINT32* pCmdList, UINT32 param, UINT32* ret);

extern UINT32 cmd_PwrIC_init[];
extern UINT32 cmd_PwrIC_read[];
extern UINT32 cmd_PwrIC_write[];
extern UINT32 cmd_Sensor_open[];
extern UINT32 cmd_Sensor_close[];
extern UINT32 cmd_LCD_setbl[];
extern UINT32 cmd_LCD_getbl[];
extern UINT32 cmd_Charge_en[];
extern UINT32 cmd_Charge_setCurrent[];
extern UINT32 cmd_Charge_geten[];
extern UINT32 cmd_Charge_getstat[];

// ------ reg 0x07 ----------
// 1. charge en
#define CHARGE_EN_MASK            0x08
#define CHARGE_EN_OFF             0x08
#define CHARGE_EN_ON              0x00
// 2. charge time
#define CHARGE_TIME_MASK          0xF0
#define CHARGE_TIME_1HOUR         0x00
#define CHARGE_TIME_2HOUR         0x10
#define CHARGE_TIME_3HOUR         0x20
#define CHARGE_TIME_4HOUR         0x30
#define CHARGE_TIME_5HOUR         0x40
#define CHARGE_TIME_6HOUR         0x50
#define CHARGE_TIME_7HOUR         0x60
#define CHARGE_TIME_8HOUR         0x70
#define CHARGE_TIME_9HOUR         0x80
#define CHARGE_TIME_10HOUR        0x90
#define CHARGE_TIME_11HOUR        0xA0
#define CHARGE_TIME_12HOUR        0xB0
#define CHARGE_TIME_13HOUR        0xC0
#define CHARGE_TIME_14HOUR        0xD0
#define CHARGE_TIME_15HOUR        0xE0
#define CHARGE_TIME_16HOUR        0xF0
// 3. charge input current limit
#define INPUT_CURRENT_LIMIT_MASK  0x03
#define INPUT_CURRENT_LIMIT_95mA  0x00
#define INPUT_CURRENT_LIMIT_475mA 0x02
#define INPUT_CURRENT_LIMIT_1A    0x01
#define INPUT_CURRENT_LIMIT_1_5A  0x03


//getstat: return reg 0x0A
#define CHARGE_STAT_MASK    0x1C
#define CHARGE_STAT_NOBATT  0x10
#define CHARGE_STAT_FULL    0x08
#define CHARGE_POWER_GOOD   0x04


#define BATT_TEMP_MASK      0xE0   // battery temperature mask
#define BATT_TEMP_UNDER_0   0xC0   // temperature < 0
#define BATT_TEMP_0_10      0x80   // temperature 0 ~ 10
#define BATT_TEMP_10_45     0x00   // temperature 10 ~ 45
#define BATT_TEMP_45_60     0x20   // temperature 45 ~ 60
#define BATT_TEMP_LARGE_60  0x60   // temperature > 60


