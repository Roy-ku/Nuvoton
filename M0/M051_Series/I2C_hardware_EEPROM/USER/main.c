#include <string.h>
#include <stdio.h>
#include "M051Series.h"
#include "User_Delay.h"
#include "EEPROM_hardware_Driver.h"
char Data;
unsigned char Data_arr_W[8]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
unsigned char Data_arr[8];

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
	SystemCoreClockUpdate();
	/* Enable I2C0 module clock */
	CLK_EnableModuleClock(I2C0_MODULE);
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
	EEPROM_Page_Clear(EEPROM_SlaveAddr,0);
	//Delay_ms(5);
	//EEPROM_PAGE_WRITE(EEPROM_SlaveAddr,0,Data_arr_W);
	//EEPROM_BYTE_WRITE(EEPROM_SlaveAddr,0x02,0xFF);
	Delay_ms(5);
	//Data = EEPROM_RANDOM_READ(EEPROM_SlaveAddr,0x02);
	EEPROM_SEQUENTIAL_READ(EEPROM_SlaveAddr,0x00,Data_arr,8);
	while(1)
	{
		
	}
}
