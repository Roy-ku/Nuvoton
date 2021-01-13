#include "1602A_hardware_Driver.h"
unsigned char LCD_data;
unsigned char digit[10] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39 }; //定義字元陣列顯示數位

uint8_t I2C_WriteByte(I2C_T *i2c, uint8_t u8SlaveAddr, uint8_t data)
{
    uint8_t u8Xfering = 1, u8Err = 0, u8Ctrl = 0;

    I2C_START(i2c);
    while(u8Xfering && (u8Err == 0))
    {
        I2C_WAIT_READY(i2c);
        switch(I2C_GET_STATUS(i2c))
        {
			/*發送START信號成功*/
            case 0x08:											 
                I2C_SET_DATA(i2c, (u8SlaveAddr << 1 | 0x00));    /* Write SLA+W to Register I2CDAT */
                u8Ctrl = I2C_I2CON_SI;                           /* Clear SI */
                break;
			/*發送地址+W成功並收到ACK*/
            case 0x18:                                           /* Slave Address ACK */
                I2C_SET_DATA(i2c, data);                         /* Write data to I2CDAT */
				        u8Ctrl = I2C_I2CON_SI;                           /* Clear SI */
                break;
			/*發送數據成功並收到ACK*/ 
            case 0x28:
                u8Ctrl = I2C_I2CON_STO_SI;                       /* Clear SI and send STOP */
                u8Xfering = 0;
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

void LCD1602A_Write(unsigned char data)
{
	I2C_WriteByte(I2C0,ADDRESS,data);
}

//********************液晶屏使能*********************
void EN_LCD()
{
	LCD_data |= (1 << (2));//E=1;
	LCD1602A_Write(LCD_data);
	Delay_us(1);
	LCD_data &= ~(1 << (2));//E=0;
	LCD1602A_Write(LCD_data);
}

//*************寫命令****************************
void LCD1602A_Write_Cmd(unsigned char command)
{
	LCD_data &= ~(1 << (0));//RS=0;
	LCD_data &= ~(1 << (1));//RW=0;
	LCD_data |=  (1 << (3));//點亮背光
	LCD1602A_Write(LCD_data);

	for(int i = 0; i < 2; i++)
	{
		LCD_data &= 0X0f; //清高四位
		LCD_data |= command & 0xf0; //寫高四位
		LCD1602A_Write(LCD_data);
		EN_LCD();
		command = command << 4; //低四位移到高四位
	}
	LCD_data = 0;
}

//*************寫數據****************************
void LCD1602A_Write_Data(unsigned char data)
{
	LCD_data |= (1 << (0));//RS=1;
	LCD_data &= ~(1 << (1));//RW=0;
	LCD_data |= (1 << (3));//點亮背光
	LCD1602A_Write(LCD_data);

	for(int i = 0; i < 2; i++)
	{
		LCD_data &= 0X0f; //清高四位
		LCD_data |= data & 0xf0; //寫高四位
		LCD1602A_Write(LCD_data);
		EN_LCD();
		data = data << 4; //低四位移到高四位
	}
	LCD_data = 0;
}

//**********************設置顯示位置*********************************
void LCD1602A_Set_Cursor(unsigned int x, unsigned int y)
{
	unsigned int pos;
	if(y == 0) pos = 0x80 + x;
	else if(y == 1)  pos = 0xC0 + x;

	LCD1602A_Write_Cmd(pos);
}

//**********************顯示字串*****************************
void Display_String(unsigned char x, unsigned char y, char* s)
{
	LCD1602A_Set_Cursor(x, y);
	while(*s)
	{
		LCD1602A_Write_Data(*s);
		s++;
	}
}

//********************顯示數位*******************************
void Display_Num(unsigned char x, unsigned char y, unsigned int num)
{
	unsigned char i, j, k, m, n;
	LCD1602A_Set_Cursor(x, y);
	i = num / 10000;
	j = num / 1000 % 10;
	k = num / 100 % 10;
	m = num / 10 % 10;
	n = num % 10;
	LCD1602A_Write_Data(digit[i]);  //  將萬位元數位的字元常量寫入1602A
	LCD1602A_Write_Data(digit[j]);  //  將千位元數位的字元常量寫入1602A
	LCD1602A_Write_Data(digit[k]);  //  將百位元數位的字元常量寫入1602A
	LCD1602A_Write_Data(digit[m]);  //  將十位元數位的字元常量寫入1602A
	//LCD1602A_Write_Data('.');
	LCD1602A_Write_Data(digit[n]);  //  將個位元數位的字元常量寫入1602A
}

//*************液晶初始化****************************
void LCD_init(void)
{
	Delay_ms(2);
	LCD1602A_Write_Cmd(0x28); //4位元顯示	
	Delay_ms(2);
	EN_LCD();
	Delay_ms(2);
	LCD1602A_Write_Cmd(0x0c); //顯示開
	LCD1602A_Write_Cmd(0x06); //地址自動+1
	LCD1602A_Write_Cmd(0x01); //清屏
	Delay_ms(2);
}
