/**********************************************************************
 Note:
    硬體參數：R（檢測電阻的阻值）= 0.1R，設置最大檢測電壓為:32V，設置電阻電壓範圍為320mV

    I_MAX（最大能檢測的電流） = 0.32V/0.1R = 3.2A
    I_LSB_MIN = I_MAX/(2^15) = 3.2/32767 = 97*10^-6
    選擇100*10^-6(方便計算)
    I_LSB = 100*10^-6 = 100uA = 0.0001A(0.1mA)（每位對應的電流大小）

    計算基準值：Cal = 0.04096/(I_LSB*R) = 0.04096/(0.0001A*0.1) = 4096 = 0x1000
    所以配置時05寄存器的值為 : 0x1000
            01寄存器的值為 : 0x3C1F
    計算功率LSB
    P_LSB=20*I_LSB=20*0.0001A=0.002W(2mW)
**********************************************************************/
#ifndef __INA219_HARDWAE_DRIVER_H__
#define __INA219_HARDWAE_DRIVER_H__

#include "M051Series.h"
/*I2C通道*/
#define INA219_I2C_Module ((I2C_T *)I2C0_BASE) //I2C0:I2C0_BASE I2C1:I2C1_BASE
/*INA219 地址*/
#define INA219_SlaveAddr 0x40
#define INA219_Write 0
#define INA219_Read 1
/****************************************************************
函式庫列表
*****************************************************************/
void INA219_Init(void);
void INA219_Write_Data(unsigned char u8_SlaveAddr, unsigned char u8_INA219_RegAddr, unsigned int u16_data);
void INA219_Read_Data(unsigned char u8_SlaveAddr, unsigned char u8_INA219_RegAddr, unsigned char *u8_data);
unsigned int INA219_GET_Volatge(void);
int INA219_GET_Current(void);
unsigned int INA219_GET_Power(void);
#endif
