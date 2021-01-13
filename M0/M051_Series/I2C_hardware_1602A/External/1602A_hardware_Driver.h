#ifndef __1602A_HARDWARE_Driver_H__
#define __1602A_HARDWARE_Driver_H__
#include "M051Series.h"
#include "User_Delay.h"
/*1602A 寫入地址*/
//#define  ADDRESS	0x7E
#define  ADDRESS	0x3F
/****************************************************************
函式庫列表
*****************************************************************/
void LCD_init(void);
void EN_LCD(void);
void LCD1602A_Write(unsigned char data);
void LCD1602A_Write_Cmd(unsigned char command);
void LCD1602A_Write_Data(unsigned char data);
void LCD1602A_Set_Cursor(unsigned int x, unsigned int y);
void Display_String(unsigned char x, unsigned char y, char* s);
void Display_Num(unsigned char x, unsigned char y, unsigned int num);
#endif
