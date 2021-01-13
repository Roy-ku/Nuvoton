#ifndef __GPIO_I2C_H__
#define __GPIO_I2C_H__

#include "M051Series.h"
/*根據需求修改*/
#define  IIC_SCL     P20 //時脈
#define  IIC_SDA     P21 //資料

/********
********************************************************
函式庫列表
*****************************************************************/
void GPIO_IIC_Start(void);
void GPIO_IIC_Stop(void);
void GPIO_IIC_Ack(void);
uint8_t GPIO_IIC_Read_Ack(void);
void GPIO_IIC_Nack(void);
void GPIO_IIC_Write_Byte(unsigned char data);
unsigned char GPIO_IIC_Read_Byte(void);
#endif
