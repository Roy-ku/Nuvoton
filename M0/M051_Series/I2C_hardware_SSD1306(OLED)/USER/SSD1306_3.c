#include <stdio.h>
#include <string.h>
#include "M051Series.h"
#include "OLED_SSD1306_hardware_Driver.h"
#include "User_Delay.h"

char Count_num[10];
/********************************************************************/
void I2C0_Init(void)
{
	/* Open I2C0 module and set bus clock */
	I2C_Open(I2C0, 100000);
	/* Get I2C0 Bus Clock */
	//printf("I2C0 clock %d Hz\n", I2C_GetBusClockFreq(I2C0));
}
/********************************************************************/
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
	I2C0_Init();
	SSD1306_Init(SSD1306_128x32);

	char CPU[] = "CPU: ", CoreClock[10];
	char I2C_C[] = "I2CClock:", I2C_Clock[10];

	sprintf(CoreClock, "%d", SystemCoreClock / 1000000);
	strcat(CPU, CoreClock);
	strcat(CPU, "MHZ");

	sprintf(I2C_Clock, "%d", I2C_GetBusClockFreq(I2C0) / 1000);
	strcat(I2C_C, I2C_Clock);
	strcat(I2C_C, "KHZ");
	SSD1306_Clear();
	SSD1306_ShowStr(0, 0, CPU, SSD1306_TextSize_F8X16);
	SSD1306_ShowStr(0, 2, I2C_C, SSD1306_TextSize_F8X16);
	Delay_ms(1000);
	SSD1306_Clear_Page(2, SSD1306_TextSize_F8X16);
	extern const unsigned char F16x16_TC_Str_1[];
	SSD1306_ShowTC(0 * 16, 2, F16x16_TC_Str_1, 0);
	SSD1306_ShowTC(1 * 16, 2, F16x16_TC_Str_1, 1);
	SSD1306_ShowTC(2 * 16, 2, F16x16_TC_Str_1, 2);
	SSD1306_ShowTC(3 * 16, 2, F16x16_TC_Str_1, 3);
	SSD1306_ShowTC(4 * 16, 2, F16x16_TC_Str_1, 4);
	SSD1306_Clear_Page(0, SSD1306_TextSize_F8X16);
	while (1)
	{

		static int count = 0;
		if (count == 20000)
			count = 0;
		sprintf(Count_num, "%d", count);
		SSD1306_ShowStr(64, 0, Count_num, SSD1306_TextSize_F8X16);
		count++;
		Delay_ms(1000);
	}
}
