#include "PCF8574_hardware_Driver.h"

static uint8_t PCF8574_I2C_WriteByte(I2C_T *i2c, uint8_t u8SlaveAddr, uint8_t data)
{
    uint8_t u8Xfering = 1, u8Err = 0, u8Ctrl = 0;

    I2C_START(i2c);
    while(u8Xfering && (u8Err == 0))
    {
        I2C_WAIT_READY(i2c);
        switch(I2C_GET_STATUS(i2c))
        {
			/*發送START信號成功*/
            case 0x08:											 
                I2C_SET_DATA(i2c, (u8SlaveAddr << 1 | write));    /* Write SLA+W to Register I2CDAT */
                u8Ctrl = I2C_I2CON_SI;                           /* Clear SI */
                break;
			/*發送地址+W成功並收到ACK*/
            case 0x18:                                           /* Slave Address ACK */
                I2C_SET_DATA(i2c, data);                         /* Write data to I2CDAT */
				        u8Ctrl = I2C_I2CON_SI;                           /* Clear SI */
                break;
			/*發送數據成功並收到ACK*/ 
            case 0x28:
                u8Ctrl = I2C_I2CON_STO_SI;                       /* Clear SI and send STOP */
                u8Xfering = 0;
                break;
			/*發送地址+W成功並收到NACK*/
            case 0x20:                                           /* Slave Address NACK */
			/*發送數據成功並收到NACK*/
            case 0x30:                                           /* Master transmit data NACK */
			/*Master發生仲裁失敗*/
            case 0x38:                                           /* Arbitration Lost */
            default:                                             /* Unknow status */
                u8Ctrl = I2C_I2CON_STO_SI;                       /* Clear SI and send STOP */
                u8Err = 1;
                break;
        }
        I2C_SET_CONTROL_REG(i2c, u8Ctrl);                        /* Write controlbit to I2C_CTL register */
    }
    return (u8Err | u8Xfering);                                  /* return (Success)/(Fail) status */
}

static uint8_t PCF8574_I2C_ReadByte(I2C_T *i2c, uint8_t u8SlaveAddr)
{
    uint8_t u8Xfering = 1, u8Err = 0, rdata = 0, u8Ctrl = 0;

    I2C_START(i2c);                                                /* Send START */
    while(u8Xfering && (u8Err == 0))
    {
        I2C_WAIT_READY(i2c);
        switch(I2C_GET_STATUS(i2c))
        {
			/*發送START信號成功*/
            case 0x08:
                I2C_SET_DATA(i2c, ((u8SlaveAddr << 1) | read));    /* Write SLA+R to Register I2CDAT */
                u8Ctrl = I2C_I2CON_SI;                             /* Clear SI */
                break;
			/*發送地址+R成功並收到ACK*/
            case 0x40:                                             /* Slave Address ACK */
                u8Ctrl = I2C_I2CON_SI;                             /* Clear SI */
                break;    
			/*收到數據並返回NACK*/
            case 0x58:
                rdata = (unsigned char) I2C_GET_DATA(i2c);         /* Receive Data */
                u8Ctrl = I2C_I2CON_STO_SI;                         /* Clear SI and send STOP */
                u8Xfering = 0;
                break;
			/*Master發生仲裁失敗*/
			case 0x48:                                             /* Slave Address NACK */
            case 0x38:                                             /* Arbitration Lost */
            default:                                               /* Unknow status */
                u8Ctrl = I2C_I2CON_STO_SI;                         /* Clear SI and send STOP */
                u8Err = 1;
                break;
        }
        I2C_SET_CONTROL_REG(i2c, u8Ctrl);                          /* Write controlbit to I2C_CTL register */
    }
    if(u8Err)
        rdata = 0;                                                 /* If occurs error, return 0 */
    return rdata;                                                  /* Return read data */
}

void PCF8574_Write(unsigned char data)
{
	PCF8574_I2C_WriteByte(I2C0,PCF8574_ADDRESS,data);
}

uint8_t PCF8574_Read(void)
{
	return PCF8574_I2C_ReadByte(I2C0,PCF8574_ADDRESS);
}
