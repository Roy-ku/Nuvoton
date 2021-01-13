/**************************************************************************************
適用24C01~24C16
24c01(Type : 1) : 容量:1Kbit/128(0x7F)個Byte，共16頁，每頁8Byte，尋址8bit，A2、A1、A0
24c02(Type : 2) : 容量:2Kbit/256(0xFF)個Byte，共32頁，每頁8Byte，尋址8bit，A2、A1、A0 
24c04(Type : 4) : 容量:4Kbit/512(0x1FF)個Byte，共32頁，每頁16Byte，尋址9bit，A2、A1、A0(懸空)
24c08(Type : 8) : 容量:8Kbit/1024(0x3FF)個Byte，共64頁，每頁16Byte，尋址10bit，A2、A1(懸空)、A0(懸空)
24c16(Type : 16): 容量:16Kbit/2048(0x7FF)個Byte，共128頁，每頁16Byte，尋址11bit，A2(懸空)、A1(懸空)、A0(懸空)
**************************************************************************************/

#include "EEPROM_hardware_Driver.h"
/**************************************************************************************
 * 描 述 : 處理因EEPROM大小不同，尋址位而不同
 * 入 參 : u16_wordaddress:資料地址 
 * 返回值 :  u16_wordaddress的高位
 **************************************************************************************/

unsigned char EEPROM_Wordaddress_Handle(unsigned int u16_wordaddress)
{
    unsigned char u8_wordaddress_H;
    switch (EEPROM_Type)
    {
    case 1:
    case 2:
        u8_wordaddress_H = 0;
        break;
    case 4:
    case 8:
    case 16:
        u8_wordaddress_H = (u16_wordaddress / 256) << 1;
        break;
    default:
        break;
    }
    return u8_wordaddress_H & 0xFF;
}

/**************************************************************************************
 * 描 述 : 整頁清除值為0xFF
 * 入 參 : u8_slaveaddr:設備地址, u8_page:頁數 
 * 返回值 :  無
 **************************************************************************************/

void EEPROM_Page_Clear(unsigned char u8_slaveaddr, unsigned char u8_page)
{
    unsigned char u16_wordaddress = 0;
    unsigned char u8_wordaddress_H, u8_wordaddress_L;
    uint8_t u8Xfering = 1, u8Err = 0, u8Ctrl = 0;
    uint32_t u32txLen = 0;
    if (((int)u8_page <= EEPROM_Page_Max) && ((int)u8_page >= EEPROM_Page_Min))
    {
        u16_wordaddress = u8_page * EEPROM_Page_Write_Buffer;
        u8_wordaddress_H = EEPROM_Wordaddress_Handle(u16_wordaddress);
        u8_wordaddress_L = (u16_wordaddress % 256) & 0xFF;
			
        /* Send START */
        I2C_START(I2C_Module);
        while (u8Xfering && (u8Err == 0))
        {
            I2C_WAIT_READY(I2C_Module);
            switch (I2C_GET_STATUS(I2C_Module))
            {
            /*發送START信號成功*/
            case 0x08:
                I2C_SET_DATA(I2C_Module, (u8_slaveaddr | u8_wordaddress_H | EEPROM_write)); /* Write SLA+W to Register I2CDAT */
                u8Ctrl = I2C_I2CON_SI;                                                      /* Clear SI */
                break;
            /*發送地址+W成功並收到ACK*/
            case 0x18:                                      /* Slave Address ACK */
                I2C_SET_DATA(I2C_Module, u8_wordaddress_L); /* Write Lo byte address of register */
                u8Ctrl = I2C_I2CON_SI;                      /* Clear SI */
                break;
            /*發送數據成功並收到ACK*/
            case 0x28:
                if (u32txLen < EEPROM_Page_Write_Buffer)
                {
                    I2C_SET_DATA(I2C_Module, 0xFF);
                    u8Ctrl = I2C_I2CON_SI; /* Clear SI */
									  u32txLen++;
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
            I2C_SET_CONTROL_REG(I2C_Module, u8Ctrl); /* Write controlbit to I2C_CTL register */
        }
    }
    else
    {
         //printf("EEPROM_Error!!");
    }
}

/**************************************************************************************
 * 描 述 : 寫入一個Byte的資料
 * 入 參 : u8_slaveaddr:設備地址, u16_wordaddress:資料地址 , u8_data:資料
 * 返回值 :  無
 **************************************************************************************/
void EEPROM_BYTE_WRITE(unsigned char u8_slaveaddr, unsigned int u16_wordaddress, unsigned char u8_data)
{
    uint8_t u8Xfering = 1, u8Err = 0, u8Ctrl = 0, u8length = 0;
    unsigned char u8_wordaddress_H, u8_wordaddress_L;
    u8_wordaddress_H = EEPROM_Wordaddress_Handle(u16_wordaddress);
    u8_wordaddress_L = (u16_wordaddress % 256) & 0xFF;

    /* Send START */
    I2C_START(I2C_Module);
    while (u8Xfering && (u8Err == 0))
    {
        I2C_WAIT_READY(I2C_Module);
        switch (I2C_GET_STATUS(I2C_Module))
        {
        /*發送START信號成功*/
        case 0x08:
            I2C_SET_DATA(I2C_Module, (u8_slaveaddr | u8_wordaddress_H | EEPROM_write)); /* Write SLA+W to Register I2CDAT */
            u8Ctrl = I2C_I2CON_SI;                                                      /* Clear SI */
            break;
        /*發送地址+W成功並收到ACK*/
        case 0x18:                                      /* Slave Address ACK */
            I2C_SET_DATA(I2C_Module, u8_wordaddress_L); /* Write data or cmd */
            u8Ctrl = I2C_I2CON_SI;                      /* Clear SI */
            break;
        /*發送數據成功並收到ACK*/
        case 0x28:
            if (u8length < 1)
            {
                I2C_SET_DATA(I2C_Module, u8_data); /* Write data to I2CDAT */
                u8Ctrl = I2C_I2CON_SI;             /* Clear SI */
                u8length++;
            }
            else
            {
                u8Ctrl = I2C_I2CON_STO_SI; /* Clear SI and send STOP */
                u8Xfering = 0;
            }
            break;
        /*發送地址+W成功並收到NACK*/
        case 0x20:                     /* Slave Address NACK */
                                       /*發送數據成功並收到NACK*/
        case 0x30:                     /* Master transmit data NACK */
                                       /*Master發生仲裁失敗*/
        case 0x38:                     /* Arbitration Lost */
        default:                       /* Unknow status */
            u8Ctrl = I2C_I2CON_STO_SI; /* Clear SI and send STOP */
            u8Err = 1;
            break;
        }
        I2C_SET_CONTROL_REG(I2C_Module, u8Ctrl); /* Write controlbit to I2C_CTL register */
    }
}

/**************************************************************************************
 * 描 述 : 寫入整頁
 * 入 參 : u8_slaveaddr:設備地址, u8_page:頁數, u8_data:資料
 * 返回值 :  無
 **************************************************************************************/

void EEPROM_PAGE_WRITE(unsigned char u8_slaveaddr, unsigned char u8_page, unsigned char *u8_data)
{
    unsigned char u16_wordaddress = 0;
    unsigned char u8_wordaddress_H, u8_wordaddress_L;
    uint8_t u8Xfering = 1, u8Err = 0, u8Ctrl = 0;
    uint32_t u32txLen = 0;
    if (((int)u8_page <= EEPROM_Page_Max) && ((int)u8_page >= EEPROM_Page_Min))
    {
        u16_wordaddress = u8_page * EEPROM_Page_Write_Buffer;
        u8_wordaddress_H = EEPROM_Wordaddress_Handle(u16_wordaddress);
        u8_wordaddress_L = (u16_wordaddress % 256) & 0xFF;

        /* Send START */
        I2C_START(I2C_Module);
        while (u8Xfering && (u8Err == 0))
        {
            I2C_WAIT_READY(I2C_Module);
            switch (I2C_GET_STATUS(I2C_Module))
            {
            /*發送START信號成功*/
            case 0x08:
                I2C_SET_DATA(I2C_Module, (u8_slaveaddr | u8_wordaddress_H | EEPROM_write)); /* Write SLA+W to Register I2CDAT */
                u8Ctrl = I2C_I2CON_SI;                                                      /* Clear SI */
                break;
            /*發送地址+W成功並收到ACK*/
            case 0x18:                                      /* Slave Address ACK */
                I2C_SET_DATA(I2C_Module, u8_wordaddress_L); /* Write Lo byte address of register */
                u8Ctrl = I2C_I2CON_SI;                      /* Clear SI */
                break;
            /*發送數據成功並收到ACK*/
            case 0x28:
                if (u32txLen < EEPROM_Page_Write_Buffer)
                {
                    I2C_SET_DATA(I2C_Module, u8_data[u32txLen++]);
                    u8Ctrl = I2C_I2CON_SI; /* Clear SI */
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
            I2C_SET_CONTROL_REG(I2C_Module, u8Ctrl); /* Write controlbit to I2C_CTL register */
        }
    }
    else
    {
         //printf("EEPROM_Error!!");
    }
}

/**************************************************************************************
 * 描 述 : 隨機讀取一個Byte的資料
 * 入 參 : u8_slaveaddr:設備地址, u16_wordaddress:資料地址
 * 返回值 :  讀回來的資料
 **************************************************************************************/
unsigned char EEPROM_RANDOM_READ(unsigned char u8_slaveaddr, unsigned int u16_wordaddress)
{
    unsigned char u8Xfering = 1, u8Err = 0, u8_data = 0, u8Ctrl = 0;
    unsigned char u8_wordaddress_H, u8_wordaddress_L;
    u8_wordaddress_H = EEPROM_Wordaddress_Handle(u16_wordaddress);
    u8_wordaddress_L = (u16_wordaddress % 256) & 0xFF;
    /* Send START */
    I2C_START(I2C_Module);
    while (u8Xfering && (u8Err == 0))
    {
        I2C_WAIT_READY(I2C_Module);
        switch (I2C_GET_STATUS(I2C_Module))
        {
        /*發送START信號成功*/
        case 0x08:
            I2C_SET_DATA(I2C_Module, (u8_slaveaddr | u8_wordaddress_H | EEPROM_write)); /* Write SLA+W to Register I2CDAT */
            u8Ctrl = I2C_I2CON_SI;                                                      /* Clear SI */
            break;
        /*發送地址+W成功並收到ACK*/
        case 0x18:                                      /* Slave Address ACK */
            I2C_SET_DATA(I2C_Module, u8_wordaddress_L); /* Write Lo byte address of register */
            break;

        /*發送數據成功並收到ACK*/
        case 0x28:
            u8Ctrl = I2C_I2CON_STA_SI; /* Send repeat START */
            break;
        /*發送Re-START信號成功*/
        case 0x10:
            I2C_SET_DATA(I2C_Module, (u8_slaveaddr | u8_wordaddress_H | EEPROM_read)); /* Write SLA+R to Register I2CDAT */
            u8Ctrl = I2C_I2CON_SI;                                                     /* Clear SI */
            break;
        /*發送地址+R成功並收到ACK*/
        case 0x40:                 /* Slave Address ACK */
            u8Ctrl = I2C_I2CON_SI; /* Clear SI */
            break;
        /*收到數據並返回NACK*/
        case 0x58:
            u8_data = (uint8_t)I2C_GET_DATA(I2C_Module); /* Receive Data */
            u8Ctrl = I2C_I2CON_STO_SI;                   /* Clear SI and send STOP */
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
        I2C_SET_CONTROL_REG(I2C_Module, u8Ctrl); /* Write controlbit to I2C_CTL register */
    }
    if (u8Err)
        u8_data = 0; /* If occurs error, return 0 */
    return u8_data;  /* Return read data */
}

/**************************************************************************************
 * 描 述 : 讀取一串資料
 * 入 參 : u8_slaveaddr:設備地址, u16_wordaddress:資料地址, u8_rxdata:要儲存資料的陣列, u8_len:需要讀的長度
 * 返回值 :  讀回來的資料長度
 **************************************************************************************/

unsigned char EEPROM_SEQUENTIAL_READ(unsigned char u8_slaveaddr, unsigned int u16_wordaddress, unsigned char *u8_rxdata, unsigned char u8_len)
{
    unsigned char u8Xfering = 1, u8Err = 0, u8Ctrl = 0;
    unsigned char u8_rxlen = 0;
    unsigned char u8_wordaddress_H, u8_wordaddress_L;
    u8_wordaddress_H = EEPROM_Wordaddress_Handle(u16_wordaddress);
    u8_wordaddress_L = (u16_wordaddress % 256) & 0xFF;
    /* Send START */
    I2C_START(I2C_Module);
    while (u8Xfering && (u8Err == 0))
    {
        I2C_WAIT_READY(I2C_Module);
        switch (I2C_GET_STATUS(I2C_Module))
        {
        /*發送START信號成功*/
        case 0x08:
            I2C_SET_DATA(I2C_Module, (u8_slaveaddr | u8_wordaddress_H | EEPROM_write)); /* Write SLA+W to Register I2CDAT */
            u8Ctrl = I2C_I2CON_SI;                                                      /* Clear SI */
            break;
        /*發送地址+W成功並收到ACK*/
        case 0x18:                                      /* Slave Address ACK */
            I2C_SET_DATA(I2C_Module, u8_wordaddress_L); /* Write Lo byte address of register */
            break;
        /*發送數據成功並收到ACK*/
        case 0x28:
            u8Ctrl = I2C_I2CON_STA_SI; /* Send repeat START */
            break;
        /*發送Re-START信號成功*/
        case 0x10:
            I2C_SET_DATA(I2C_Module, (u8_slaveaddr | u8_wordaddress_H | EEPROM_read)); /* Write SLA+R to Register I2CDAT */
            u8Ctrl = I2C_I2CON_SI;                                                     /* Clear SI */
            break;
        /*發送地址+R成功並收到ACK*/
        case 0x40:                    /* Slave Address ACK */
            u8Ctrl = I2C_I2CON_SI_AA; /* Clear SI and set ACK */
            break;
        /*收到數據並返回ACK*/
        case 0x50:
            u8_rxdata[u8_rxlen++] = (uint8_t)I2C_GET_DATA(I2C_Module); /* Receive Data */
            if (u8_rxlen < (u8_len - 1))
                u8Ctrl = I2C_I2CON_SI_AA; /* Clear SI and set ACK */
            else
                u8Ctrl = I2C_I2CON_SI; /* Clear SI */
            break;
        /*收到數據並返回NACK*/
        case 0x58:
            u8_rxdata[u8_rxlen++] = (uint8_t)I2C_GET_DATA(I2C_Module); /* Receive Data */
            u8Ctrl = I2C_I2CON_STO_SI;                                 /* Clear SI and send STOP */
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
        I2C_SET_CONTROL_REG(I2C_Module, u8Ctrl); /* Write controlbit to I2C_CTL register */
    }
    return u8_rxlen;
}
