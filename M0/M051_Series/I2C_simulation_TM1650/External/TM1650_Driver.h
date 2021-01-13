#ifndef __TM1650_DRIVER_H__
#define __TM1650_DRIVER_H__

#include "M051Series.h"
/*根據需求修改*/
#define  TM1650_SCL     P20 //CLOCK
#define  TM1650_SDA     P21 //DATA

#define WRITE 0
#define Read 1
#define TM1650_OFF 0x00
#define TM1650_ON 0x01
#define TM1650_Light_level_1 0x10
#define TM1650_Light_level_2 0x20
#define TM1650_Light_level_3 0x30
#define TM1650_Light_level_4 0x40
#define TM1650_Light_level_5 0x50
#define TM1650_Light_level_6 0x60
#define TM1650_Light_level_7 0x70
#define TM1650_Light_level_8 0x00
#define DIG1 0x68
#define DIG2 0x6A
#define DIG3 0x6C
#define DIG4 0x6E
#define Invalid 0x00 //無效0
/****************************************************************
函式庫列表
*****************************************************************/
void TM1650_IIC_Start(void);
void TM1650_IIC_Stop(void);
void TM1650_IIC_Ack(void);
uint8_t TM1650_IIC_Read_Ack(void);
void TM1650_IIC_Nack(void);
void TM1650_IIC_Write_Byte(unsigned char data);
unsigned char TM1650_IIC_Read_Byte(void);
void TM1650_Init(unsigned char brightness,unsigned char state);
void TM1650_Send_Data(unsigned char addr,unsigned char data);
void TM1650_Decimal_Send(float decimal);
void TM1650_Num_Send(uint16_t num);
#endif
