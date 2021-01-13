#include "INA219_hardware_Driver.h"

/*初始化*/
void INA219_Init(void)
{
    //INA219_Write_Data(INA219_SlaveAddr,0x00,0x8000);//重新啟動
    INA219_Write_Data(INA219_SlaveAddr, 0x00, 0x3C1F);
    INA219_Write_Data(INA219_SlaveAddr, 0x05, 0x1000);
}

/*寫入資料*/
void INA219_Write_Data(unsigned char u8_SlaveAddr, unsigned char u8_INA219_RegAddr, unsigned int u16_data)
{
    unsigned char data_temp[2];
    unsigned char u8Xfering = 1, u8Err = 0, u8Ctrl = 0;
    unsigned char u8_txLen = 0, u8_Len = 2;
    data_temp[0] = (unsigned char)(u16_data >> 8);   //保留高8位
    data_temp[1] = (unsigned char)(u16_data & 0xFF); //保留低8位

    /* Send START */
    I2C_START(INA219_I2C_Module);
    while (u8Xfering && (u8Err == 0))
    {
        I2C_WAIT_READY(INA219_I2C_Module);
        switch (I2C_GET_STATUS(INA219_I2C_Module))
        {
        /*發送START信號成功*/
        case 0x08:
            I2C_SET_DATA(INA219_I2C_Module, (u8_SlaveAddr << 1 | INA219_Write)); /* Write SLA+W to Register I2CDAT */
            u8Ctrl = I2C_I2CON_SI;
            break;
        /*發送地址+W成功並收到ACK*/
        case 0x18:                                              /* Slave Address ACK */
            I2C_SET_DATA(INA219_I2C_Module, u8_INA219_RegAddr); /* Write Lo byte address of register */
            u8Ctrl = I2C_I2CON_SI;
            break;
        /*發送數據成功並收到ACK*/
        case 0x28:
            if (u8_txLen < u8_Len)
            {
                I2C_SET_DATA(INA219_I2C_Module, data_temp[u8_txLen++]);
                u8Ctrl = I2C_I2CON_SI;
            }
            else
            {
                u8Ctrl = I2C_I2CON_STO_SI; /* Clear SI and send STOP */
                u8Xfering = 0;
            }
            break;
        /*Master發生仲裁失敗*/
        case 0x20:                     /* Slave Address NACK */
        case 0x30:                     /* Master transmit data NACK */
        case 0x38:                     /* Arbitration Lost */
        default:                       /* Unknow status */
            u8Ctrl = I2C_I2CON_STO_SI; /* Clear SI and send STOP */
            u8Err = 1;
            break;
        }
        I2C_SET_CONTROL_REG(INA219_I2C_Module, u8Ctrl); /* Write controlbit to I2C_CTL register */
    }
}

/*讀取資料*/
void INA219_Read_Data(unsigned char u8_SlaveAddr, unsigned char u8_INA219_RegAddr, unsigned char *u8_data)
{
    unsigned char u8Xfering = 1, u8Err = 0, u8Ctrl = 0;
    unsigned char u8_rxLen = 0, u8_len = 2;

    /* Send START */
    I2C_START(INA219_I2C_Module);
    while (u8Xfering && (u8Err == 0))
    {
        I2C_WAIT_READY(INA219_I2C_Module);
        switch (I2C_GET_STATUS(INA219_I2C_Module))
        {
        /*發送START信號成功*/
        case 0x08:
            I2C_SET_DATA(INA219_I2C_Module, (u8_SlaveAddr << 1 | INA219_Write)); /* Write SLA+W to Register I2CDAT */
            u8Ctrl = I2C_I2CON_SI;                                               /* Clear SI */
            break;
        /*發送地址+W成功並收到ACK*/
        case 0x18:                                              /* Slave Address ACK */
            I2C_SET_DATA(INA219_I2C_Module, u8_INA219_RegAddr); /* Write Lo byte address of register */
            u8Ctrl = I2C_I2CON_SI;
            break;
        /*發送數據成功並收到ACK*/
        case 0x28:
            u8Ctrl = I2C_I2CON_STA_SI; /* Send repeat START */
            break;
        /*發送Re-START信號成功*/
        case 0x10:
            I2C_SET_DATA(INA219_I2C_Module, ((u8_SlaveAddr << 1) | INA219_Read)); /* Write SLA+R to Register I2CDAT */
            u8Ctrl = I2C_I2CON_SI;                                                /* Clear SI */
            break;
        /*發送地址+R成功並收到ACK*/
        case 0x40:                    /* Slave Address ACK */
            u8Ctrl = I2C_I2CON_SI_AA; /* Clear SI and set ACK */
            break;
        /*收到數據並返回ACK*/
        case 0x50:
            u8_data[u8_rxLen++] = (uint8_t)I2C_GET_DATA(INA219_I2C_Module); /* Receive Data */
            if (u8_rxLen < (u8_len - 1))
                u8Ctrl = I2C_I2CON_SI_AA; /* Clear SI and set ACK */
            else
                u8Ctrl = I2C_I2CON_SI; /* Clear SI */
            break;
        /*收到數據並返回NACK*/
        case 0x58:
            u8_data[u8_rxLen++] = (uint8_t)I2C_GET_DATA(INA219_I2C_Module); /* Receive Data */
            u8Ctrl = I2C_I2CON_STO_SI;                                      /* Clear SI and send STOP */
            u8Xfering = 0;
            break;
        /*Master發生仲裁失敗*/
        case 0x20:                     /* Slave Address NACK */
        case 0x30:                     /* Master transmit data NACK */
        case 0x38:                     /* Arbitration Lost */
        case 0x48:                     /* Slave Address NACK */
        default:                       /* Unknow status */
            u8Ctrl = I2C_I2CON_STO_SI; /* Clear SI and send STOP */
            u8Err = 1;
            break;
        }
        I2C_SET_CONTROL_REG(INA219_I2C_Module, u8Ctrl); /* Write controlbit to I2C_CTL register */
    }
}

/*得到電壓 單位:(mV)*/
unsigned int INA219_GET_Volatge(void)
{
    unsigned char u8_data_temp[2];
    unsigned int u16_volatge = 0;
    INA219_Read_Data(INA219_SlaveAddr, 0x02, u8_data_temp);
    u16_volatge = (unsigned int)((((u8_data_temp[0] << 8) | u8_data_temp[1]) >> 3) * 4);
    return u16_volatge;
}

/*得到電流 單位:(mA)*/
int INA219_GET_Current(void)
{
    unsigned char u8_data_temp[2];
    unsigned int u16_current = 0;
    INA219_Read_Data(INA219_SlaveAddr, 0x04, u8_data_temp);
    /*判斷電流正負*/
    if (((u8_data_temp[0] >> 7) & 1) == 0)
    {
        u16_current = (unsigned int)(((((u8_data_temp[0] << 8) | u8_data_temp[1])) * 0.1) + 0.5);
    }
    else if (((u8_data_temp[0] >> 7) & 1) == 1)
    {
        int current;
        u16_current = (unsigned int)(((u8_data_temp[0] << 8) | u8_data_temp[1]));
        u16_current = (~(u16_current - 1)) & 0xFFFF;
        current = -((u16_current * 0.1) + 0.5);
        return current;
    }
    return u16_current;
}

/*得到功率 單位:(mW)*/
unsigned int INA219_GET_Power(void)
{
    unsigned char u8_data_temp[2];
    unsigned int u16_power = 0;
    INA219_Read_Data(INA219_SlaveAddr, 0x03, u8_data_temp);
    u16_power = (unsigned int)((((u8_data_temp[0] << 8) | u8_data_temp[1])) * 2);
    return u16_power;
}
