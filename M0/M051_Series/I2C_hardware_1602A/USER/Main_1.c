#include "M051Series.h"
#include "GPIO_I2C.h"
#include "User_Delay.h"
//I2C料號為PCF8574A
#define  ADDRESS	0x7E
unsigned char LCD_data;
///////////////////////////////////////////////////////
void SYS_Init()
{
    /****************************/
    /* Init System Clock*/
    /****************************/

    /* Enable external 12 MHz XTAL */
    CLK->PWRCON = CLK_PWRCON_XTL12M_EN_Msk;
    /* Waiting for clock ready */
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_XTL12M_STB_Msk));

    /* Switch HCLK clock source to HXT*/
    CLK->CLKSEL0 &= ~CLK_CLKSEL0_HCLK_S_Msk;
    CLK->CLKSEL0 |=  CLK_CLKSEL0_HCLK_S_HXT;
    CLK->CLKDIV &= ~CLK_CLKDIV_HCLK_N_Msk;
	  CLK->CLKDIV |= CLK_CLKDIV_HCLK(1);
    /* Set core clock as PLL_CLOCK from PLL */
    /*CLK->PLLCON = CLK_PLLCON_50MHz_HXT;
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_PLL_STB_Msk));
    CLK->CLKSEL0 &= ~CLK_CLKSEL0_HCLK_S_Msk;
    CLK->CLKSEL0 |=  CLK_CLKSEL0_HCLK_S_PLL;*/
    
    /* Update System Core Clock */
    SystemCoreClockUpdate();
		
		P0->PMD = P0->PMD & ((~GPIO_PMD_PMD6_Msk) | (GPIO_PMD_QUASI << GPIO_PMD_PMD6_Pos));
	  P0->PMD = P0->PMD & ((~GPIO_PMD_PMD7_Msk) | (GPIO_PMD_QUASI << GPIO_PMD_PMD7_Pos));
}

///////////////////////////////////////////////////////
void LCD1602A_Write(unsigned char data)
{
    IIC_Start();
    IIC_Write_Byte(ADDRESS);
    Read_Ack();
    IIC_Write_Byte(data);
    Read_Ack();
    IIC_Stop();
}

void EN_LCD()
{
    LCD_data|=(1<<(2));//E=1;
    LCD1602A_Write(LCD_data);
    Delay_us(1);
    LCD_data&=~(1<<(2));//E=0;
    LCD1602A_Write(LCD_data);
}

void LCD1602A_Write_Cmd(unsigned char command)
{
		Delay_us(16);
    LCD_data&=~(1<<(0));//RS=0;
    LCD_data&=~(1<<(1));//RW=0;
    LCD1602A_Write(LCD_data);

    for(int i = 0; i<2; i++)
    {
        LCD_data &= 0X0f; //清高四位
        LCD_data |= command & 0xf0; //寫高四位
        LCD1602A_Write(LCD_data);
        EN_LCD();
        command = command<<4; //低四位移到高四位
    }
    LCD_data = 0;
}

void LCD1602A_Write_Data(unsigned char data)
{
		Delay_us(16);
		LCD_data|=(1<<(0));//RS=1;
		LCD_data&=~(1<<(1));//RW=0;
		LCD_data|=(1<<(3));//點亮背光
    LCD1602A_Write(LCD_data);
	  
		for(int i = 0;i<2;i++)
		{
			LCD_data&=0X0f; //清高四位
			LCD_data|=data&0xf0; //寫高四位
			LCD1602A_Write(LCD_data);
			EN_LCD();
			data=data<<4; //低四位移到高四位
		}
	//	LCD_data = 0;
}

void LCD1602A_Set_Cursor(unsigned int x, unsigned int y)
{
    unsigned int pos;
    if(y==1)
    {
        pos = 0x80 + x;
    }
    else
    {
        pos = 0xC0 + x;
    }

    LCD1602A_Write_Cmd(pos);
}

//**********************顯示字串*****************************
void Display_String(unsigned char x,unsigned char y,char *s)
{ 
    LCD1602A_Set_Cursor(x,y);
	while (*s) 
 	{     
		LCD1602A_Write_Data(*s);     
		s++;     
 	}
}

void LCD1602A_Init()
{
	  LCD1602A_Write_Cmd(0x28);//16*2顯示 5*7大小 8bit長度
    Delay_ms(1);
	  LCD1602A_Write_Cmd(0x28);//16*2顯示 5*7大小 8bit長度
	  EN_LCD();
    Delay_ms(1);
    LCD1602A_Write_Cmd(0x28);//16*2顯示 5*7大小 8bit長度
    LCD1602A_Write_Cmd(0x0C);//開啟顯示關閉光標
    //LCD1602A_Write_Cmd(0x06);//地址自動+1
    LCD1602A_Write_Cmd(0x01);//清屏
    Delay_ms(2);
}
///////////////////////////////////////////////////////
int main(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();
    /* Init System, peripheral clock and multi-function I/O */
    SYS_Init();
    Delay_Init();
    /* Lock protected registers */
    SYS_LockReg();

    LCD1602A_Init();

    Display_String(4,1,"Hello World");
}


