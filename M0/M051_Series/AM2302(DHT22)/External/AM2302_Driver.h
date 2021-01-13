#ifndef __AM2302_DRIVER_H__
#define __AM2302_DRIVER_H__

#include "M051Series.h"
#include "User_Delay.h"

#define AM2302_SDA P17
/****************************************************************
函式庫列表
*****************************************************************/	
void AM2302_Init(void);
unsigned char AM2302_Start(void);
unsigned char AM2303_Read_Bit(void);
unsigned char AM2302_Read_Byte(void);
unsigned char AM2302_Get_Value(float *humi, float *temp);
#endif
