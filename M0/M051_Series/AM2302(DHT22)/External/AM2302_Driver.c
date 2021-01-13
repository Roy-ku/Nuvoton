#include "AM2302_Driver.h"

void AM2302_Init()
{
  GPIO_SetMode(P1, BIT7, GPIO_PMD_QUASI);
}
/**************************************************************************************
 * 描 述 :  開始AM2302
 * 入 參 :  無
 * 返回值 : unsigned char 0:正常,1:超時
 **************************************************************************************/
unsigned char AM2302_Start()
{ 
  /*Master 信號*/
  AM2302_SDA = 0;
  Delay_ms(1);
  AM2302_SDA = 1;
  Delay_us(30);

  /*從機應答*/
  unsigned char _am2302_count = 100;         //設定超時時間
  while ((AM2302_SDA == 1) && _am2302_count) //等待變LOW
  {
    _am2302_count--;
    Delay_us(1);
  }
  if (_am2302_count < 1)
    return 1;
  else
    _am2302_count = 100; //設定超時時間

  while ((AM2302_SDA == 0) && _am2302_count) //等待變HIGH
  {
    _am2302_count--;
    Delay_us(1);
  }
  if (_am2302_count < 1)
    return 1;
  return 0;
}

unsigned char AM2303_Read_Bit()
{
  unsigned char u8_data;
  unsigned char _am2302_count = 100;
  while (AM2302_SDA && _am2302_count) //等待50us到來 LOW間隙
  {
    _am2302_count--;
    Delay_us(1);
  }

  _am2302_count = 100;
  while (!AM2302_SDA && _am2302_count) //等待50us 間隙結束
  {
    _am2302_count--;
    Delay_us(1);
  }
  u8_data = 0;
  Delay_us(32); //這裡等待32us,越過28us（但不能大於28+50）,直接判斷是1
  if (AM2302_SDA == 1)
    u8_data = 1;
  _am2302_count = 100;
  while (AM2302_SDA && _am2302_count) //等待AM2302_SDA變回LOW
  {
    _am2302_count--;
    Delay_us(1);
  }
  return u8_data;
}

unsigned char AM2302_Read_Byte()
{
  unsigned char u8_data;
  for (unsigned char i = 0; i < 8; i++)
  {
    u8_data <<= 1;
    u8_data |= AM2303_Read_Bit();
  }
  return u8_data;
}
/**************************************************************************************
 * 描 述 :  取得濕度與溫度
 * 入 參 :  *Humi:濕度 ,*Temp:溫度
 * 返回值 : unsigned char 0:成功,1:失敗
 **************************************************************************************/
unsigned char AM2302_Get_Value(float *humi, float *temp)
{
  unsigned char u8_buf[5];
  if (AM2302_Start() == 0)
  {
    for (unsigned char i = 0; i < 5; i++)
    {
      u8_buf[i] = AM2302_Read_Byte();
    }
    /*校驗*/
    if ((unsigned char)(u8_buf[0] + u8_buf[1] + u8_buf[2] + u8_buf[3]) == u8_buf[4])
    {
      *humi = ((float)(u8_buf[0] << 8 | u8_buf[1])) / 10;
      *temp = ((float)(u8_buf[2] << 8 | u8_buf[3])) / 10;
      return 0;
    }
  }
  return 1;
}
