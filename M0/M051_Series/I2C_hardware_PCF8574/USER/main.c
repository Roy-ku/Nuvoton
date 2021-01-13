#include "M051Series.h"
#include "PCF8574_hardware_Driver.h"
#include "User_Delay.h"
#include <stdio.h>

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
	/* Enable I2C0 module clock */
  CLK_EnableModuleClock(I2C0_MODULE);
	/* Set core clock as PLL_CLOCK from PLL */
	CLK_SetCoreClock(50000000);
	SystemCoreClockUpdate();
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
	PCF8574_Write(0xFF);
	while(1)
	{
//		uint8_t da;
		PCF8574_Write((0xFF&(~BIT3))|BIT2);
//		da = PCF8574_Read();
//		printf("%d\r\n",da);
		Delay_ms(500);
		PCF8574_Write((0xFF&(~BIT2))|BIT3);
//		da = PCF8574_Read();
//		printf("%d\r\n",da);
		Delay_ms(500);	

	}
	
}
