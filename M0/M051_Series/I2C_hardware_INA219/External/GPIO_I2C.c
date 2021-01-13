#include "GPIO_I2C.h"
/*開始*/
void GPIO_IIC_Start(void)
{
	IIC_SDA = 1;
	IIC_SCL = 1;
	CLK_SysTickDelay(1);
	IIC_SDA = 0;
	CLK_SysTickDelay(1);
	IIC_SCL = 0;
	CLK_SysTickDelay(1);
}

/*結束*/
void GPIO_IIC_Stop(void)
{
	IIC_SCL = 0;
	IIC_SDA = 0;
	CLK_SysTickDelay(1);
	IIC_SCL = 1;
	IIC_SDA = 1;
	CLK_SysTickDelay(1);
}

/*發出應答信號的器件必須在來自主器件的SCLK 線電平
由低跳變到高之前，在tSU-DATA 時間內拉低SDA 線，
並在SCLK 線電平由高到低跳變之後的tH-DATA 時間內
保持低電平。*/

void GPIO_IIC_Ack(void)
{
	IIC_SCL = 0;
	IIC_SDA = 0;
	CLK_SysTickDelay(1);
	IIC_SCL = 1;
	CLK_SysTickDelay(1);
	IIC_SCL = 0;
}


/************************
等待從端回應
return 1 錯誤
return 0 成功
************************/
uint8_t GPIO_IIC_Read_Ack(void)
{
	uint8_t Err_count = 0;
	IIC_SDA = 1;               //發送完後釋放資料線，準備接收應答位
	CLK_SysTickDelay(1);
	IIC_SCL = 1;
	CLK_SysTickDelay(1);
	while(IIC_SDA)
	{
		Err_count++;
		if(Err_count > 250)
		{
			GPIO_IIC_Stop();
			//IIC_SDA = 0;//退出
			return 1;
		}
	}
	IIC_SCL = 0;
	return 0;
}

/*在讀數據過程中，一旦從器件讀出最後一位數據，主器
件必須通過不產生ACK 位來向從器件發送數據結束信
號。在這種情況下，從器件釋放數據線，以使
主器件產生停止條件。*/
void GPIO_IIC_Nack(void)
{
	IIC_SCL = 0;
	IIC_SDA = 1;
	CLK_SysTickDelay(1);
	IIC_SCL = 1;
	CLK_SysTickDelay(1);
	IIC_SCL = 0;
}

/*寫入*/
void GPIO_IIC_Write_Byte(unsigned char data)
{
	unsigned char count;
	IIC_SCL = 0; //拉低開始傳資料
	for(count = 0; count < 8; count++)
	{
		IIC_SDA = (data & 0x80) >> 7;
		data <<= 1;
		CLK_SysTickDelay(1);
		IIC_SCL = 1;
		CLK_SysTickDelay(1);
		IIC_SCL = 0;
		CLK_SysTickDelay(1);
	}
}

/*讀取*/
unsigned char GPIO_IIC_Read_Byte(void)
{
	unsigned char count, data = 0;
	for(count = 0; count < 8; count++)
	{
		IIC_SCL = 0;
		CLK_SysTickDelay(1);
		IIC_SCL = 1;
		data <<= 1;
		//data=(data<<1)|IIC_SDA;
		if(IIC_SDA == 1) data |= 0X01;
		IIC_SCL = 1;
		CLK_SysTickDelay(1);
		IIC_SCL = 0;
		CLK_SysTickDelay(1);
	}
	return data;
}

