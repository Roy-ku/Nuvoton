#ifndef __PCF8574_HARDWARE_Driver_H__
#define __PCF8574_HARDWARE_Driver_H__
#include "M051Series.h"
#include "User_Delay.h"

#define  write	0
#define  read	1
/*PCF8574 地址*/
#define  PCF8574_ADDRESS	0x20

/****************************************************************
函式庫列表
*****************************************************************/
static uint8_t PCF8574_I2C_WriteByte(I2C_T *i2c, uint8_t u8SlaveAddr, uint8_t data);
static uint8_t PCF8574_I2C_ReadByte(I2C_T *i2c, uint8_t u8SlaveAddr);
void PCF8574_Write(unsigned char data);
uint8_t PCF8574_Read(void);
#endif
