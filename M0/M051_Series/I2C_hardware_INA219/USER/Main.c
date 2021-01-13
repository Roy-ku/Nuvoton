#include <stdio.h>
#include "M051Series.h"
#include "User_Delay.h"
#include "INA219_hardware_Driver.h"

void I2C0_Init(void)
{
	/* Open I2C0 module and set bus clock */
	I2C_Open(I2C0, 100000);
	/* Get I2C0 Bus Clock */
	//printf("I2C0 clock %d Hz\n", I2C_GetBusClockFreq(I2C0));
}

void SYS_Init()
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
	/* Configure the SDA0 & SCL0 of I2C0 pins */
	SYS->P3_MFP &= ~(SYS_MFP_P34_Msk | SYS_MFP_P35_Msk);
	SYS->P3_MFP |= (SYS_MFP_P34_SDA0 | SYS_MFP_P35_SCL0);
}

int main(void)
{
	/* Unlock protected registers */
	SYS_UnlockReg();
	/* Init System, peripheral clock and multi-function I/O */
	SYS_Init();
	Delay_Init();
	/* Lock protected registers */
	SYS_LockReg();
	I2C0_Init();
	Delay_ms(1);
	INA219_Init();
	unsigned int Volatge, Power;
	int Current;
	P41 = 0;
	while (1)
	{
		Volatge = INA219_GET_Volatge();
		Current = INA219_GET_Current();
		Power = INA219_GET_Power();
		printf("%.2fV,", (float)Volatge / 1000);
		printf("%dmA,", Current);
		printf("%dmw\n", Power);
		Delay_ms(1000);
	}
}
