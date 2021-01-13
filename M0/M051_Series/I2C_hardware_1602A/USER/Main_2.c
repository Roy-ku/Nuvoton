#include "M051Series.h"
#include "1602A_hardware_Driver.h"
#include "User_Delay.h"
#include <string.h>
#include <stdio.h>

char CLOCK[7];//預留1空間放'\0'

void I2C0_Init(void)
{
	  /* Open I2C0 module and set bus clock */
    I2C_Open(I2C0, 100000);
	  /* Get I2C0 Bus Clock */
    //printf("I2C0 clock %d Hz\n", I2C_GetBusClockFreq(I2C0));
	  
}
void SYS_Init(void)
{
	/* Enable external XTAL 12MHz clock */
	CLK_EnableXtalRC(CLK_PWRCON_XTL12M_EN_Msk);
	/* Waiting for clock ready */
	CLK_WaitClockReady(CLK_CLKSTATUS_XTL12M_STB_Msk);
	/* Switch HCLK clock source to HXT*/
	CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HXT, CLK_CLKDIV_HCLK(1));
	/* Set core clock as PLL_CLOCK from PLL */
  CLK_SetCoreClock(50000000);
	/* Enable I2C0 module clock */
  CLK_EnableModuleClock(I2C0_MODULE);
	SystemCoreClockUpdate();
  //GPIO_SetMode(P2,BIT0,GPIO_PMD_QUASI);
	//GPIO_SetMode(P2,BIT1,GPIO_PMD_QUASI);
	/* Configure the SDA0 & SCL0 of I2C0 pins */
  SYS->P3_MFP &= ~(SYS_MFP_P34_Msk | SYS_MFP_P35_Msk);
  SYS->P3_MFP |= (SYS_MFP_P34_SDA0 | SYS_MFP_P35_SCL0);
}

/*---------------------------------------------------------------------------------------------------------*/
/*  Main Function                                                                                          */
/*---------------------------------------------------------------------------------------------------------*/
int32_t main(void)
{
	/* Unlock protected registers */
	SYS_UnlockReg();
	/* Init System, peripheral clock and multi-function I/O */
	SYS_Init();
	Delay_Init();
	/* Lock protected registers */
	SYS_LockReg();
	I2C0_Init();
	LCD_init();
	sprintf(CLOCK, "%d", I2C_GetBusClockFreq(I2C0) / 1000);
	strncat(CLOCK, "KHz", 3);
	Display_String(0, 0, "Clock:"); //顯示一段文字
	Display_String(6, 0, CLOCK); //顯示一段文字
	while(1)
	{
		static int count = 100;
		Display_Num(5, 1, count);
		count++;
		if(count == 60000)
		{
			count = 0;
		}
		Delay_ms(1000);
	}
}
