#ifndef __OLED_SSD1306_HARDWARE_DRIVER_H__
#define __OLED_SSD1306_HARDWARE_DRIVER_H__

#include <stdio.h>
#include <string.h>
#include "M051Series.h"

#define SSD1306_128x32  1
#define SSD1306_128x64  2
#define SSD1306_TextSize_F6x8 1
#define SSD1306_TextSize_F8X16 2
#define SSD1306_ADDRESS	0x3C //0x78和0x7A兩個地址 -- 默認0x78(0x3C)

/****************************************************************
函式庫列表
*****************************************************************/	
void SSD1306_Init(char type);
void SSD1306_Write(unsigned char addr,unsigned char data);
void SSD1306_Write_Cmd(unsigned char data);
void SSD1306_Write_Data(unsigned char data);
void SSD1306_SetPos(unsigned char x, unsigned char y) ;
void SSD1306_ShowStr(unsigned char x, unsigned char y,  char ch[], unsigned char TextSize);
void SSD1306_ShowNum(unsigned char x, unsigned char y,unsigned int num,unsigned char TextSize);
void SSD1306_ShowTC(unsigned char x, unsigned char y, const unsigned char *tc_str, unsigned char N);
void SSD1306_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);
void SSD1306_Clear(void);
void SSD1306_Clear_Page(unsigned char page,unsigned char TextSize);
void SSD1306_ON(void);
void SSD1306_OFF(void);
#endif
