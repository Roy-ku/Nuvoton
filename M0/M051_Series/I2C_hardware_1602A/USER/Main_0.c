#include "M051Series.h"
//#include "GPIO_I2C.h"
#include "User_Delay.h"
//I2C料號為PCF8574A
#define  SCL     P20 //時脈
#define  SDA     P21 //資料
#define  ADDRESS	0x7E
unsigned char ack;
unsigned char LCD_data;
///////////////////////////////////////////////////////
void SYS_Init()
{
    /****************************/
    /* Init System Clock*/
    /****************************/
     /* Enable Internal RC 22.1184MHz clock */
    CLK->PWRCON |= CLK_PWRCON_OSC22M_EN_Msk;

    /* Waiting for Internal RC clock ready */
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_OSC22M_STB_Msk));

    /* Switch HCLK clock source to Internal RC and and HCLK source divide 1 */
    CLK->CLKSEL0 &= ~CLK_CLKSEL0_HCLK_S_Msk;
    CLK->CLKSEL0 |= CLK_CLKSEL0_HCLK_S_HIRC;
    CLK->CLKDIV &= ~CLK_CLKDIV_HCLK_N_Msk;
    CLK->CLKDIV |= (CLK_CLKDIV_HCLK(5) << CLK_CLKDIV_HCLK_N_Msk);
	  
    /* Enable external 12 MHz XTAL */
    CLK->PWRCON = CLK_PWRCON_XTL12M_EN_Msk;
    /* Waiting for clock ready */
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_XTL12M_STB_Msk));

    /* Switch HCLK clock source to HXT*/
    CLK->CLKSEL0 &= ~CLK_CLKSEL0_HCLK_S_Msk;
    //CLK->CLKSEL0 |=  CLK_CLKSEL0_HCLK_S_HXT;
   CLK->CLKSEL0 |= CLK_CLKSEL0_HCLK_S_HIRC;
    /* Set core clock as PLL_CLOCK from PLL */
    /*CLK->PLLCON = CLK_PLLCON_50MHz_HXT;
    while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_PLL_STB_Msk));
    CLK->CLKSEL0 &= ~CLK_CLKSEL0_HCLK_S_Msk;
    CLK->CLKSEL0 |=  CLK_CLKSEL0_HCLK_S_PLL;*/

    /* Update System Core Clock */
    SystemCoreClockUpdate();
}

///////////////////////////////////////////////////////

void nop4()
{

 __nop();     //等待一個機器週期
 __nop();      __nop();      __nop();      __nop();
 __nop();      __nop();      __nop();      __nop();
 __nop();      __nop();      __nop();      __nop();
 __nop();      __nop();      __nop();      __nop();
 __nop();      __nop();      __nop();      __nop();
}
void Start()
{
 	SDA=1;

    SCL=1;
	nop4();nop4();
    SDA=0;
	nop4();
    SCL=0;
  nop4();

}

void Stop()
{
	SCL=0;
 	SDA=0;
  nop4();	


//	nop4();nop4();//>4us後SCL跳變
	SCL=1;
	nop4();nop4();nop4();
	SDA=1;
   nop4();

}
void  Write_A_Byte(unsigned char c)
{
 unsigned char BitCnt;
  for(BitCnt=0;BitCnt<8;BitCnt++)  //要傳送的資料長度為8位元
    {
     if((c<<BitCnt)&0x80)  SDA=1;   //判斷發送位
     else  SDA=0;                
     nop4();
     SCL=1;               //置時鐘線為高，通知被控器開始接收資料位元
     nop4(); 
     nop4();      
     SCL=0; 
    }  
    nop4();
    nop4();
    SDA=1;               //8位元發送完後釋放資料線，準備接收應答位
    nop4();
    nop4(); 
    SCL=1;
    nop4();
    nop4();
    nop4();
    if(SDA==1)ack=0;     
       else ack=1;        //判斷是否接收到應答信號
    SCL=0;
    nop4();
    nop4();
}
void LCD1602A_Write(unsigned char data)
{
    Start();
    Write_A_Byte(ADDRESS);
		if(ack==0)
    Write_A_Byte(data);
    if(ack==0)
    Stop();
}

void EN_LCD()
{
    LCD_data|=(1<<(2));//E=1;
    LCD1602A_Write(LCD_data);
    Delay_us(100);
    LCD_data&=~(1<<(2));//E=0;
    LCD1602A_Write(LCD_data);
}

void LCD1602A_Write_Cmd(unsigned char command)
{
		Delay_us(200);
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
		Delay_us(200);
		LCD_data|=(1<<(0));//RS=1;
		LCD_data&=~(1<<(1));//RW=0;
		//LCD_data|=(1<<(3));//點亮背光
    LCD1602A_Write(data);
	  
		for(int i = 0;i<2;i++)
		{
			LCD_data&=0X0f; //清高四位
			LCD_data|=data&0xf0; //寫高四位
			LCD1602A_Write(LCD_data);
			EN_LCD();
			data=data<<4; //低四位移到高四位
		}
		LCD_data = 0;
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
	  Delay_ms(2);
    LCD1602A_Write_Cmd(0x28);//16*2顯示 5*7大小 8bit長度
	  EN_LCD();
    Delay_ms(2);
    LCD1602A_Write_Cmd(0x28);//16*2顯示 5*7大小 8bit長度
    LCD1602A_Write_Cmd(0x0C);//開啟顯示關閉光標
    //LCD1602A_Write_Cmd(0x06);//地址自動+1
    LCD1602A_Write_Cmd(0x01);//清屏
    Delay_ms(3);
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
    int x = SystemCoreClock;
    LCD1602A_Init();
    
	while(1)
	{
		Display_String(4,1,"1234");
		Display_String(4,2,"5678");
		LCD1602A_Write(1<<(3));
		while(1);
	}
    
	  
	
}


