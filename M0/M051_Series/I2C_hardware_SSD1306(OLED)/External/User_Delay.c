#include "User_Delay.h"
//#define CLKSRC_is_Msk //SysTick鐘源為HCLK鐘源

uint32_t Cycle_us;
/**************************************************************************************
 * 描 述 : Delay初始化設定
 * 入 參 : 無
 * 返回值 : 無
**************************************************************************************/
void Delay_Init()
{
	/*選擇SysTick鐘源*/
	#ifdef  CLKSRC_is_Msk	
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk;
	if(SystemCoreClock == __HXT) Cycle_us = __HXT/1000000;
	else if(SystemCoreClock == __HSI) Cycle_us = __HSI/1000000;//計時器會溢位不準
	else if(SystemCoreClock == __HIRC) Cycle_us = __HIRC/1000000;//計時器會溢位不準
	#else
    /*使用外部震盪器(12MHz)*/	
	SysTick->CTRL = (0 << SysTick_CTRL_CLKSOURCE_Pos);
	CLK->CLKSEL0 &= ~CLK_CLKSEL0_STCLK_S_Msk;
	CLK->CLKSEL0 |= CLK_CLKSEL0_STCLK_S_HXT_DIV2;//如果鐘源不是HCLK必須小於HCLK或HCLK/2	
	Cycle_us = __HXT/1000000/2;
	#endif
}
/**************************************************************************************
 * 描 述 : 延遲 us(單位)
 * 入 參 : us
 * 返回值 :  無
**************************************************************************************/
void Delay_us(uint32_t us)
{	  
	  SysTick->LOAD = us * Cycle_us;
    SysTick->VAL  = (0x00);//清空計數器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//開始計數器
  //  CLK_SysTickDelay
	  /* Waiting for down-count to zero */
    while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
	
    /* Disable SysTick counter */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//關閉計數器	  
}
/**************************************************************************************
 * 描 述 : 延遲 ms(單位)
 * 入 參 : ms 
 * 返回值 : 無  
**************************************************************************************/
void Delay_ms(uint32_t ms)
{
	Delay_us(1000*ms);
}
/**************************************************************************************
 * 描 述 : 延遲 ms(單位)
 * 入 參 : s
 * 返回值 : 無 
**************************************************************************************/
void Delay_s(uint32_t s)
{
	for(short int count=0;count<s;count++)
	{
	  Delay_us(1000000);
	}
}
