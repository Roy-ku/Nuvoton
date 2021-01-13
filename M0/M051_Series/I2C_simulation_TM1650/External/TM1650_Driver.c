#include "TM1650_Driver.h"
/*共陰*/
const uint8_t Num_Table[10]={0x6F,0x28,0x37,0x3B,0x78,0x5B,0x5F,0x69,0x7F,0x7B};
/****GPIO I2C Driver****/
/*Start*/
void TM1650_IIC_Start(void)
{
	TM1650_SDA = 1;
	TM1650_SCL = 1;
	CLK_SysTickDelay(1);
	TM1650_SDA = 0;
	CLK_SysTickDelay(1);
	TM1650_SCL = 0;
	CLK_SysTickDelay(1);
}
/*Stop*/
void TM1650_IIC_Stop(void)
{
	TM1650_SCL = 0;
	TM1650_SDA = 0;
	CLK_SysTickDelay(1);
	TM1650_SCL = 1;
	TM1650_SDA = 1;
	CLK_SysTickDelay(1);
}
/*ACK*/
void TM1650_IIC_Ack(void)
{
	TM1650_SCL = 0;
	TM1650_SDA = 0;
	CLK_SysTickDelay(1);
	TM1650_SCL = 1;
	CLK_SysTickDelay(1);
	TM1650_SCL = 0;
}
/************************
Read_Ack
等待從端回應
return 1 錯誤
return 0 成功
************************/
uint8_t TM1650_IIC_Read_Ack(void)
{
	uint8_t Err_count = 0;
	TM1650_SDA = 1;               //發送完後釋放資料線，準備接收應答位
	CLK_SysTickDelay(1);
	TM1650_SCL = 1;
	CLK_SysTickDelay(1);
	while(TM1650_SDA)
	{
		Err_count++;
		if(Err_count > 250)
		{
			TM1650_IIC_Stop();
			return 1;
		}
	}
	TM1650_SCL = 0;
	return 0;
}
/*NCAK*/
void TM1650_IIC_Nack(void)
{
	TM1650_SCL = 0;
	TM1650_SDA = 1;
	CLK_SysTickDelay(1);
	TM1650_SCL = 1;
	CLK_SysTickDelay(1);
	TM1650_SCL = 0;
}
/*Write*/
void TM1650_IIC_Write_Byte(unsigned char data)
{
	unsigned char count;
	TM1650_SCL = 0; //拉低開始傳資料
	for(count = 0; count < 8; count++)
	{
		TM1650_SDA = (data & 0x80) >> 7;
		data <<= 1;
		CLK_SysTickDelay(1);
		TM1650_SCL = 1;
		CLK_SysTickDelay(1);
		TM1650_SCL = 0;
		CLK_SysTickDelay(1);
	}
}
/*Read*/
unsigned char TM1650_IIC_Read_Byte(void)
{
	unsigned char count, data = 0;
	for(count = 0; count < 8; count++)
	{
		TM1650_SCL = 0;
		CLK_SysTickDelay(1);
		TM1650_SCL = 1;
		data <<= 1;
		if(TM1650_SDA == 1) data |= 0X01;
		TM1650_SCL = 1;
		CLK_SysTickDelay(1);
		TM1650_SCL = 0;
		CLK_SysTickDelay(1);
	}
	return data;
}

/****TM1650 Driver****/
/**************************************************************************************
 * 描 述 : TM1650初始化設定
 * 入 參 : brightness TM1650_Light_level_X 1~8級
					 state 0:關 1:開
 * 返回值 : 無
**************************************************************************************/
void TM1650_Init(unsigned char brightness,unsigned char state)
{ 
    if(state)TM1650_Send_Data(0x48,(brightness)|0x01); //開啟顯示
		else TM1650_Send_Data(0x48,0); //關閉顯示
}
void TM1650_Num_Send(uint16_t num)
{
	uint16_t temp[4];
	temp[3]=Num_Table[num/1000%10];
	temp[2]=Num_Table[num/100%10];
	temp[1]=Num_Table[num/10%10];
	temp[0]=Num_Table[num%10];
	if(temp[3]==0x6F)
	{		
		temp[3] = Invalid;
		if(temp[2] == 0x6F)
		{
			temp[2] = Invalid;
			if(temp[1] == 0x6F)
			{
				temp[1] = Invalid;
			}
		}
	}
	TM1650_Send_Data(DIG4,temp[3]);
	TM1650_Send_Data(DIG3,temp[2]);
	TM1650_Send_Data(DIG2,temp[1]);
	TM1650_Send_Data(DIG1,temp[0]);
}
void TM1650_Decimal_Send(float decimal)
{
	uint16_t temp[4];
	uint16_t int_decimal;
	int_decimal = decimal*10;
	
	temp[3]=Num_Table[int_decimal/1000%10];
	temp[2]=Num_Table[int_decimal/100%10];
	temp[1]=Num_Table[int_decimal/10%10];
	temp[0]=Num_Table[int_decimal%10];
	if(temp[3]==0x6F)
	{		
		temp[3] = Invalid;
		if(temp[2] == 0x6F)
		{
			temp[2] = Invalid;
		}
	}
	TM1650_Send_Data(DIG4,temp[3]);
	TM1650_Send_Data(DIG3,temp[2]);
	TM1650_Send_Data(DIG2,temp[1]|0x80);
	TM1650_Send_Data(DIG1,temp[0]);
}
void TM1650_Send_Data(unsigned char addr,unsigned char data)
{
	uint8_t err=0;
	do
	{
		TM1650_IIC_Start();
		TM1650_IIC_Write_Byte(addr);
		err = TM1650_IIC_Read_Ack();
		TM1650_IIC_Write_Byte(data);
		err = TM1650_IIC_Read_Ack();
		TM1650_IIC_Stop();
	}while(err == 1);
}
