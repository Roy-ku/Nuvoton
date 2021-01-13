#include "M051Series.h"
#include <stdio.h>
#include "User_Delay.h"
#include "AM2302_Driver.h"
float Temp,Humi;
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
    AM2302_Init();
	AM2302_Get_Value(&Humi,&Temp);
    while (1)
    {
    }
}
