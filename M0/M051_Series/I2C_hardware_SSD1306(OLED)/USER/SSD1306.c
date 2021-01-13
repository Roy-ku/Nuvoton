#include <stdio.h>
#include <string.h>
#include "M051Series.h"
#include "My_Function.h"
#include "OLED_SSD1306.h"

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
	
	/* Set core clock as PLL_CLOCK from PLL */
  /*CLK->PLLCON = CLK_PLLCON_50MHz_HXT;
  while(!(CLK->CLKSTATUS & CLK_CLKSTATUS_PLL_STB_Msk));
  CLK->CLKSEL0 &= ~CLK_CLKSEL0_HCLK_S_Msk;
	CLK->CLKSEL0 |=  CLK_CLKSEL0_HCLK_S_PLL;*/
	GPIO_SetMode(P2,BIT0,GPIO_PMD_QUASI);
	GPIO_SetMode(P2,BIT1,GPIO_PMD_QUASI);
	/* Update System Core Clock */
	SystemCoreClockUpdate();
}      
///////////////////////////////////////////////////////
int main(void)
{

	/* Unlock protected registers */
  SYS_UnlockReg();
  /* Init System, peripheral clock and multi-function I/O */
  SYS_Init();
  /* Lock protected registers */
  SYS_LockReg();
	
	//printf("System clock rate: %d Hz\n", SystemCoreClock);
	char CPU[]="CPU: ",CoreClock[10];
	sprintf(CoreClock,"%d", SystemCoreClock);
	
	strcat(CPU,CoreClock);
	strcat(CPU," HZ");
	
	SSD1306_Init();
	//SSD1306_Clear();
	
	SSD1306_ShowStr(0,2,"1111111111111133",2);
	SSD1306_ShowStr(0,4,CPU,2);
	
	while(1)
	{
		short int y=1;
		int count=0;
		for(int x=0;x>-1;x=x+y,count++)
		{
			if(x+32>128)
			{
				y=-1;
				CLK_SysTickDelay(1000);
			}
			
			SSD1306_ShowStr(x-1,0," ",2);
			SSD1306_ShowStr(x,0,"TEST",2);
      SSD1306_ShowStr(x+32,0," ",1);
      char c[10];	 		
      sprintf(c,"%d",count);			
      SSD1306_ShowStr(0,6,c,2); 
      			
		}
		SSD1306_ShowStr(0,6,"    ",2);
		CLK_SysTickDelay(1000000);
		
	}
}
