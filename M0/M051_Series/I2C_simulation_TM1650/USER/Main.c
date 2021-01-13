#include <stdio.h>
#include "M051Series.h"
#include "User_Delay.h"
#include "TM1650_Driver.h"
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
		SystemCoreClockUpdate();

		GPIO_SetMode(P2,BIT0,GPIO_PMD_QUASI);
		GPIO_SetMode(P2,BIT1,GPIO_PMD_QUASI);
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
		TM1650_Init(TM1650_Light_level_3,TM1650_ON);
	  while(1)
		{		
			for(float count =0.0 ;count<999;count=count+0.1)
			{					 
					TM1650_Decimal_Send(count);
					Delay_ms(1000);			
			}						
		}
}


