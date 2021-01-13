#ifndef __USER_DELAY_H__
#define __USER_DELAY_H__

#include "M051Series.h"


/****************************************************************
函式庫列表
*****************************************************************/
void Delay_Init(void);
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);
#endif
