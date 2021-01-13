#include "OLED_SSD1306_hardware_Driver.h"
#include "Font.h"
unsigned char SSD1306_Type;

uint8_t I2C_WriteByte(I2C_T *i2c, uint8_t u8SlaveAddr,uint8_t controlAddr, uint8_t data)
{
    uint8_t u8Xfering = 1, u8Err = 0, u8Ctrl = 0,u8length=0;

    I2C_START(i2c);
    while(u8Xfering && (u8Err == 0))
    {
        I2C_WAIT_READY(i2c);
        switch(I2C_GET_STATUS(i2c))
        {
			/*發送START信號成功*/
            case 0x08:											 
                I2C_SET_DATA(i2c, (u8SlaveAddr << 1 |0x00));    /* Write SLA+W to Register I2CDAT */
                u8Ctrl = I2C_I2CON_SI;                           /* Clear SI */
                break;
			/*發送地址+W成功並收到ACK*/
            case 0x18:                                           /* Slave Address ACK */
                I2C_SET_DATA(i2c, controlAddr);                  /* Write data or cmd */
				        u8Ctrl = I2C_I2CON_SI;                           /* Clear SI */
                break;
			/*發送數據成功並收到ACK*/ 
            case 0x28:
				if(u8length < 1)
				{
					I2C_SET_DATA(i2c, data);                       /* Write data to I2CDAT */
					u8Ctrl = I2C_I2CON_SI;                         /* Clear SI */
					u8length++;
				}
				else
				{									
					u8Ctrl = I2C_I2CON_STO_SI;                       /* Clear SI and send STOP */
					u8Xfering = 0;
				}
				break;
			/*發送地址+W成功並收到NACK*/
            case 0x20:                                           /* Slave Address NACK */
			/*發送數據成功並收到NACK*/
            case 0x30:                                           /* Master transmit data NACK */
			/*Master發生仲裁失敗*/
            case 0x38:                                           /* Arbitration Lost */
            default:                                             /* Unknow status */
                u8Ctrl = I2C_I2CON_STO_SI;                       /* Clear SI and send STOP */
                u8Err = 1;
                break;
        }
        I2C_SET_CONTROL_REG(i2c, u8Ctrl);                        /* Write controlbit to I2C_CTL register */
    }
    return (u8Err | u8Xfering);                                  /* return (Success)/(Fail) status */
}
///////////////////////////////////////////////////////
void SSD1306_Write(unsigned char controlAddr,unsigned char data)
{
	  I2C_WriteByte(I2C0,SSD1306_ADDRESS,controlAddr,data);
}

void SSD1306_Write_Cmd(unsigned char data)
{
    SSD1306_Write(0x00,data);
}

void SSD1306_Write_Data(unsigned char data)
{
    SSD1306_Write(0x40,data);
}

/*設置起始座標*/
void SSD1306_SetPos(unsigned char x, unsigned char y)
{
    SSD1306_Write_Cmd(0xB0+y);
    SSD1306_Write_Cmd(((x&0xF0)>>4)|0x10);
    SSD1306_Write_Cmd((x&0x0F));
}

/*顯示字串 TextSize -- 字符大小(1:6*8 ; 2:8*16)*/
void SSD1306_ShowStr(unsigned char x, unsigned char y,  char ch[], unsigned char TextSize)
{
    unsigned char c = 0,i = 0,j = 0;

    switch(TextSize)
    {
    case 1:
    {
        while(ch[j] != '\0')
        {
            c = ch[j] - 32;
            if(x > 126)
            {
                x = 0;
                y++;
            }
            SSD1306_SetPos(x,y);
            for(i=0; i<6; i++)
            {
                SSD1306_Write_Data(F6x8[c][i]);
            }
            x += 6;
            j++;
        }
    }
    break;

    case 2:
    {
        while(ch[j] != '\0')
        {
            c = ch[j] - 32;
            if(x > 120)
            {
                x = 0;
                y++;
            }
            SSD1306_SetPos(x,y);
            for(i=0; i<8; i++)
            {
                SSD1306_Write_Data(F8X16[c*16+i]);
            }
            SSD1306_SetPos(x,y+1);
            for(i=0; i<8; i++)
            {
                SSD1306_Write_Data(F8X16[c*16+i+8]);
            }
            x += 8;
            j++;
        }
    }
    break;

    }
}

/*顯示數字 ps.從右至左*/
void SSD1306_ShowNum(unsigned char x, unsigned char y,unsigned int num,unsigned char TextSize)
{
    unsigned char c = 0,i = 0;
    short count = 0;
    char str[5] = "",str_n[5];
    snprintf(str_n,sizeof(str_n),"%d",num);

    /*無效0*/
    for(int i = strlen(str_n); i < 5; i++)
    {
        strcat(str," ");
    }
    strcat(str,str_n);
    count=strlen(str);


    switch(TextSize)
    {
    case 1:
    {
        while(count != 0)
        {
            c = str[count-1] - 32;
            if(x > 126)
            {
                x = 0;
                y++; //當x溢位時是否換行
            }
            SSD1306_SetPos(x,y);
            for(i=0; i<6; i++)
            {
                SSD1306_Write_Data(F6x8[c][i]);
            }
            x -= 6;
            count--;
        }
    }
    break;

    case 2:
    {
        while(count != 0)
        {
            c = str[count-1] - 32;
            if(x > 120)
            {
                x = 0;
                y++; //當x溢位時是否換行
            }
            SSD1306_SetPos(x,y);
            for(i=0; i<8; i++)
            {
                SSD1306_Write_Data(F8X16[c*16+i]);
            }
            SSD1306_SetPos(x,y+1);
            for(i=0; i<8; i++)
            {
                SSD1306_Write_Data(F8X16[c*16+i+8]);
            }
            x -= 8;
            count--;
        }
    }
    break;

    }
}
/*****************************
說明:	傳送中文字(一次只能傳一個字)
參數:	X:X座標 Y:Y座標 tc_str[]:中文字串 N: 第幾個字
回傳:	無
******************************/
void SSD1306_ShowTC(unsigned char x, unsigned char y, const unsigned char *tc_str, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	SSD1306_SetPos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		SSD1306_Write_Data(tc_str[adder]);
		adder += 1;
	}
	SSD1306_SetPos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		SSD1306_Write_Data(tc_str[adder]);
		adder += 1;
	}
}
/*****************************
說明:	顯示BMP位圖
參數:	x0,y0 -- 起始點坐標(x0:0~127, y0:0~7); x1,y1 -- 起點對角線(結束點)的坐標(x1:1~128,y1:1~ 8)
回傳:	無
******************************/
void SSD1306_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[])
{
    unsigned int j=0;
    unsigned char x,y;

  if(y1%4==0)
        y = y1/4;
  else
        y = y1/4 + 1;
    for(y=y0;y<y1;y++)
    {
        SSD1306_SetPos(x0,y);
    for(x=x0;x<x1;x++)
        {
            SSD1306_Write_Data(BMP[j++]);
        }
    }
}

/*初始化設定*/
void SSD1306_Init(char type)
{
    CLK_SysTickDelay(100);   //必須延遲
    SSD1306_Write_Cmd(0xAE); //顯示關閉 0xAF是開啟 0xAE是關閉
    switch (type)
    {
    case 1:
        SSD1306_Type = 1;
        SSD1306_Write_Cmd(0xA8); //設定分辨率
        SSD1306_Write_Cmd(0x1F); //128*64:0x3F  128*32:0x1F
        SSD1306_Write_Cmd(0xDA); //設置COM硬體預設配置，適應分辨率
        SSD1306_Write_Cmd(0x02); //0x12:0.96-128*64    0x02:0.96-128*32
        break;
    case 2:
        SSD1306_Type = 2;
        SSD1306_Write_Cmd(0xA8); //設定分辨率
        SSD1306_Write_Cmd(0x3F); //128*64:0x3F  128*32:0x1F
        SSD1306_Write_Cmd(0xDA); //設置COM硬體預設配置，適應分辨率
        SSD1306_Write_Cmd(0x12); //0x12:0.96-128*64    0x02:0.96-128*32
        break;
    }
    SSD1306_Write_Cmd(0x20); //設置內存地址模式 有三種模式：水平，垂直，頁尋址（默認）
    SSD1306_Write_Cmd(0x10); //水平：0x00 垂直：0x01 頁尋址：0x02 Invalid :0x11
    SSD1306_Write_Cmd(0xB0); //為頁尋址模式設置頁面開始地址，0-7
    SSD1306_Write_Cmd(0xC8); //設置COM掃描方向 【0xc0上下反置COM0到COM N-1 左到右 】 【0xc8正常 COM N-1到COM0 右到左】
    SSD1306_Write_Cmd(0x00); //設置低列地址
    SSD1306_Write_Cmd(0x10); //設置高列地址
    SSD1306_Write_Cmd(0x40); //設置顯示開始行 0到63 第【5:0】位 01[xxxxx] 默認這五位是 000000b
    SSD1306_Write_Cmd(0x81); //對比度設置指令
    SSD1306_Write_Cmd(0xFF); //亮度 0x00~0xFF(亮度設置,越大越亮)
    SSD1306_Write_Cmd(0xA1); //設置segment重映射,對於IIC通訊的四腳OLED要設置成0xA1，如果設置成0xA1的話顯示會反置
    SSD1306_Write_Cmd(0xA6); //設置顯示方式,正常顯示:0xA6,反相顯示:0xA7
    SSD1306_Write_Cmd(0xA4); //0xA4,輸出遵循RAM內容 0xA5,輸出忽略RAM內容
    SSD1306_Write_Cmd(0xD3); //設置顯示偏移
    SSD1306_Write_Cmd(0x00); //默認值00 沒有偏移
    SSD1306_Write_Cmd(0xD5); //設置顯示時鐘分頻/振盪器頻率
    SSD1306_Write_Cmd(0xF0); //設置分率
    SSD1306_Write_Cmd(0xD9); //設置預充電時期
    SSD1306_Write_Cmd(0x22); //默認0x22
    SSD1306_Write_Cmd(0xDB); //Set VCOMH Deselect Level 不是很懂，按照默認的設置就行了
    SSD1306_Write_Cmd(0x20); //默認是0x20 0.77xVcc
    SSD1306_Write_Cmd(0x8D); //充電泵設置
    SSD1306_Write_Cmd(0x14); //0x14:允許在顯示開啟的時候使用 0x10:不允許在顯示開啟的時候使用
    SSD1306_Write_Cmd(0xAF); //顯示關閉 0xAF是開啟 0xAE是關閉
    SSD1306_Clear();
}

/*螢幕清空*/
void SSD1306_Clear(void)
{
    unsigned char i, n, page_len;
    if (SSD1306_Type == 1)
        page_len = 4;
    else
        page_len = 8;
    for (i = 0; i < page_len; i++)
    {
        SSD1306_Write_Cmd(0xB0 + i); //設置頁地址（0~7）
        SSD1306_Write_Cmd(0x00);     //設置顯示位置—列低地址
        SSD1306_Write_Cmd(0x10);     //設置顯示位置—列高地址
        for (n = 0; n < 128; n++)
            SSD1306_Write_Data(0x00);
    }
}

/*清空Page*/
void SSD1306_Clear_Page(unsigned char page, unsigned char TextSize)
{
    if (TextSize == 1)
    {
        if (SSD1306_Type == 1 && page >= 4)
            page = 3;
        switch (page)
        {
        case 0:
            SSD1306_Write_Cmd(0xB0 + 0); //設置頁地址（0~7）
            break;
        case 1:
            SSD1306_Write_Cmd(0xB0 + 1); //設置頁地址（0~7）
            break;
        case 2:
            SSD1306_Write_Cmd(0xB0 + 2); //設置頁地址（0~7）
            break;
        case 3:
            SSD1306_Write_Cmd(0xB0 + 3); //設置頁地址（0~7）
            break;
        case 4:
            SSD1306_Write_Cmd(0xB0 + 4); //設置頁地址（0~7）
            break;
        case 5:
            SSD1306_Write_Cmd(0xB0 + 5); //設置頁地址（0~7）
            break;
        case 6:
            SSD1306_Write_Cmd(0xB0 + 6); //設置頁地址（0~7）
            break;
        case 7:
            SSD1306_Write_Cmd(0xB0 + 7); //設置頁地址（0~7）
            break;
        }
        SSD1306_Write_Cmd(0x00); //設置顯示位置—列低地址
        SSD1306_Write_Cmd(0x10); //設置顯示位置—列高地址
        for (unsigned char n = 0; n < 128; n++)
            SSD1306_Write_Data(0x00);
    }
    else if (TextSize == 2)
    {
        unsigned char page_address_pos;
        if (SSD1306_Type == 1 && page >= 2)
            page = 2;
        switch (page)
        {
        case 0:
        case 1:
            page_address_pos = 0;
            break;
        case 2:
        case 3:
            page_address_pos = 2;
            break;
        case 4:
        case 5:
            page_address_pos = 4;
            break;
        case 6:
        case 7:
            page_address_pos = 6;
            break;
        }

        for (unsigned char i = page_address_pos; i <= page_address_pos + 1; i++)
        {
            SSD1306_Write_Cmd(0xB0 + i); //設置頁地址（0~7）
            SSD1306_Write_Cmd(0x00);     //設置顯示位置—列低地址
            SSD1306_Write_Cmd(0x10);     //設置顯示位置—列高地址
            for (unsigned char n = 0; n < 128; n++)
                SSD1306_Write_Data(0x00);
        }
    }
}

/*打開OLED*/
void SSD1306_ON()
{
    SSD1306_Write_Cmd(0x8D);
    SSD1306_Write_Cmd(0x14);
    SSD1306_Write_Cmd(0xAF);
}

/*關閉OLED*/
void SSD1306_OFF()
{
    SSD1306_Write_Cmd(0x8D);
    SSD1306_Write_Cmd(0x10);
    SSD1306_Write_Cmd(0xAE);
}
