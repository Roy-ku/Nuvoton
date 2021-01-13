#ifndef __EEPROM_HARDWARE_Driver_H__
#define __EEPROM_HARDWARE_Driver_H__
#include "M051Series.h"
#include <stdio.h>
#include "User_Delay.h"

#define  I2C_Module ((I2C_T *) I2C0_BASE) //I2C0:I2C0_BASE I2C1:I2C1_BASE
#define  EEPROM_Type 2
#define  EEPROM_Page_Max 31
#define  EEPROM_Page_Min 0
#define  EEPROM_Page_Write_Buffer 8

#define  EEPROM_write	0
#define  EEPROM_read	1
/*EEPROM 地址*/
#define  EEPROM_SlaveAddr	0xA0 

/****************************************************************
函式庫列表
*****************************************************************/
unsigned char EEPROM_Wordaddress_Handle(unsigned int u16_wordaddress);
void EEPROM_Page_Clear(unsigned char u8_slaveaddr,unsigned char u8_page);
void EEPROM_BYTE_WRITE(unsigned char u8_slaveaddr,unsigned int u16_wordaddress,unsigned char u8_data);
void EEPROM_PAGE_WRITE(unsigned char u8_slaveaddr,unsigned char u8_page,unsigned char *u8_data);
unsigned char EEPROM_RANDOM_READ(unsigned char u8_slaveaddr,unsigned int u16_wordaddress);
unsigned char EEPROM_SEQUENTIAL_READ(unsigned char u8_slaveaddr,unsigned int u16_wordaddress,unsigned char *u8_rxdata,unsigned char u8_len);
#endif
