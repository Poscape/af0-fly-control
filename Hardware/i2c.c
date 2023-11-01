#include "i2c.h"
#include "string.h"

#define MPU6050_ADDR 0xD0
#define SCL 8
#define SDA 9
#define Transmitter 0
#define Receiver 1

uint8_t EVstate = 0;
extern int16_t acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z;

void GPIOB_I2C1_Init(uint8_t scl, uint8_t sda)
{
    /* ENABLE GPIOB & I2C1_clk */
    RCC->AHB1ENR |= 0x1 << 1; // gpio
    RCC->APB1ENR |= 1 << 21;  // iic
    /* Set pb8 & pb9 as af mode*/
    GPIOB->MODER |= 0x2 << 2 * scl;
    GPIOB->MODER |= 0x2 << 2 * sda;
    /* Set pb8 & pb9 as AF4 */
    GPIOB->AFR[1] |= 0x4 << 4 * 0;
    GPIOB->AFR[1] |= 0x4 << 4 * 1;
    /* Set pb8 & pb9 as up */
    GPIOB->PUPDR |= 0x1 << 2 * scl;
    GPIOB->PUPDR |= 0x1 << 2 * sda;
    /* Set pb8 & pb9 as high speed */
    GPIOB->OSPEEDR |= 0x1 << 2 * scl;
    GPIOB->OSPEEDR |= 0x1 << 2 * sda;
    /* Set pb8 & pb9 as open drain */
    GPIOB->OTYPER |= 0x1 << 1 * scl;
    GPIOB->OTYPER |= 0x1 << 1 * sda;
}

void I2C1_Init(void)
{

    /* I2C 配置 */
    // I2C_InitTypeDef I2C_InitStructure;
    // I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;            // I2C模式 */
    // I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;    // SCL 时钟线的占空比
    // I2C_InitStructure.I2C_OwnAddress1 = I2C_OWN_ADDRESS7; // 从机时，自身器件地址
    // I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;           // 使能ACK相应
    //                                                       // 7bit的寻址模式
    // I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    // I2C_InitStructure.I2C_ClockSpeed = 400000; // 通信速率<=400K
    // I2C_Init(I2C1, &I2C_InitStructure);        // I2C2 初始化
    // I2C_AcknowledgeConfig(I2C1, ENABLE);  //使能IIC ACK功能

    /*Init pb8 & pb9 as af mode*/
    GPIOB_I2C1_Init(SCL, SDA);
    /*reset i2c1*/
    // I2C1->CR1 |= 1 << 15;
    // I2C1->CR1 &= ~(1 << 15);

    /*Init Hardware I2C11     */
    /*Set Clk as 42MHz      */
    I2C1->CR2 &= ~(0x3F);
    I2C1->CR2 |= 0x2A;

    I2C1->CR1 &= ~(0x1 << 0);

    I2C1->TRISE &= ~(0x3F);
    I2C1->TRISE |= 42 + 1;

    /*Set SCL Frequency as 100KHz */
    /*Fscl = CCR * (1/PCLK)       */
    I2C1->CCR &= ~(0xFFF);
    I2C1->CCR |= 1 << 15;
    I2C1->CCR |= 210;

    I2C1->CR1 |= 0x1 << 0;

    // clear ack, smbtype, smbus
    I2C1->CR1 &= ~(0x1 << 10);
    I2C1->CR1 &= ~(0x1 << 3);
    I2C1->CR1 &= ~(0x1 << 1);

    /*ENABLE ack return after receiving an whole byte*/
    I2C1->CR1 |= 0x1 << 10;

    /*ENABLE I2C11           */
    I2C1->CR1 |= 0x1 << 0;

    // ack
    I2C1->CR1 |= 0x1 << 10;
}

void I2C1_SetStart(FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        I2C1->CR1 |= 0x1 << 8;
    }
    else
    {
        I2C1->CR1 &= ~(0x1 << 8);
    }
}

void I2C1_SetStop(FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        I2C1->CR1 |= 0x1 << 9;
    }
    else
    {
        I2C1->CR1 &= ~(0x1 << 9);
    }
}

void I2C1_SendAddr(uint8_t addr)
{
    I2C1->DR = addr;
}


/* @brief SR1:
0SB,1ADDR,2BTF,3ADD10,
4STOPF,6RxNE,7TxE,
8BERR,9ARLO,10AF,11OVR,
12PECERR,14TIMEOUT,15SMBALERT
*/
uint32_t SB_Flag(void)
{
    return I2C1->SR1 & 0x1 << 0;
}

uint32_t ADDR_Flag(void)
{
    return I2C1->SR1 & 0x1 << 1;
}

uint32_t TxE_Flag(void)
{
    return I2C1->SR1 & 0x1 << 7;
}

/* @brief SR2:
0MSL,1BUSY,2TRA,
4GENCALL,5SMBDEFAULT,6SMBHOST,7DUALF,
8-15PEC
*/
uint32_t MSL_Flag(void)
{
    return I2C1->SR2 & 0x1 << 0;
}

uint32_t BUSY_Flag(void)
{
    return I2C1->SR2 & 0x1 << 1;
}

uint32_t TRA_Flag(void)
{
    return I2C1->SR2 & 0x1 << 2;
}

void I2C1_CheckMaster_TransmitterMode(void)
{
     /* BUSY, MSL, ADDR, TXE and TRA flags */
    // while (!(I2C1->SR1 >> 1 & 0X1) && !(I2C1->SR1 >> 7 & 0X1) && !(I2C1->SR2 & 0x1) && !(I2C1->SR2 & 0x2) && !(I2C1->SR2 & 0x4))
    //     ;
    while (!(BUSY_Flag()) && !(MSL_Flag()) && !(ADDR_Flag()) && !(TxE_Flag()) && !(TRA_Flag()))
        ;
}

void I2C1_CheckMaster_ReceiverMode(void)
{
    while (!(I2C1->SR2 & 0x1) && !~(I2C1->SR2 & 0x2) && !(I2C1->SR2 | 0x4))
        ;
}

/*check if Start Bit is 1*/
void I2C1_CheckSB(void)
{
    while(!(I2C1->SR1 & 0x1 << 0));
}

void I2C1_CheckBUSY(void)
{
    // while (I2C1->SR2 & 0x2)
    //     ;
    while (BUSY_Flag())
        ;
}
/*check if the address sent was matched*/
void I2C1_CheckADDR(void)
{
    while (!(I2C1->SR1 & 0x1 << 1))
        ;
}

/*in receiver mode,check if data register is not empty*/
void I2C1_CheckRxNE(void)
{
    while ((I2C1->SR1 & 0x1 << 6))
        ;
}

/*in transmitter mode,check if data register is empty*/
void I2C1_CheckTxE(void)
{
    //   while(!(I2C1->SR1 & 0x1 << 7));
}

/*check if byte transfer finished*/
void I2C1_CheckBTF(void)
{
    while (!(I2C1->SR1 & 0x1 << 2))
        ;
}

uint8_t I2C1_ReceiveByte(void)
{
    return I2C1->DR;
}

/*Direction: Transmitter or Receiver*/
void I2C1_Write7bitAddr(uint8_t addr, uint8_t Direction)
{
    if (Direction == Transmitter)
    {
    }
    else
    {
        addr |= 0x1 << 0;
    }
    I2C1->DR = addr;
}

void I2C1_Write_7bitmode_Register(uint8_t DeviceAddr, uint8_t RegisterAddr, uint8_t Data)
{
    I2C1_CheckBUSY();
    I2C1_SetStart(ENABLE);
    /*the SB will be cleard by after sending data*/
    I2C1_Write7bitAddr(DeviceAddr, Transmitter);
    I2C1_CheckMaster_TransmitterMode();
    I2C1_EventDeal(6, 0);
    I2C1_EventDeal(8, 1);
    /*the ADDR will be cleard by after sending data*/
    I2C1_SendAddr(RegisterAddr);
    I2C1_EventDeal(8, 0);
    I2C1_SendAddr(Data);
    I2C1_EventDeal(8, 2);
}

uint8_t I2C1_Read_7bitmode_Register(uint8_t DeviceAddr, uint8_t RegisterAddr)
{
    uint8_t data = 0;
    I2C1_CheckBUSY();

    //I2C1_SendACK(ENABLE);
    //I2C1_SetStop(DISABLE);
    I2C1_SetStart(ENABLE);

    //    I2C1_EventDeal(5, 0);
    EV5();
    /*the SB will be cleard by after sending data*/
    I2C1_Write7bitAddr(DeviceAddr, Transmitter);
    // I2C1_CheckMaster_TransmitterMode();
    EV6("MASTER_TRANSMITTER_MODE");
    //    I2C1_SendACK(DISABLE);
    //    I2C1_SetStop(ENABLE);
    //		I2C1_SendACK(DISABLE);
    I2C1_SendAddr(RegisterAddr);
    EV8();

    I2C1_SetStart(ENABLE);
    EV5();
    //    I2C1_EventDeal(5, 0);
    /*the SB will be cleard by after sending data*/
    I2C1_Write7bitAddr(DeviceAddr, Receiver);
    //I2C1_CheckMaster_ReceiverMode();
    //		I2C1_SendACK(DISABLE);
    EV6("MASTER_RECEIVER_MODE");
    EV7();

    // I2C1_EventDeal(7, 1);
    // I2C1_CheckRxNE();
    data = I2C1_ReceiveByte();
    // I2C1_EventDeal(7, 0);

    //    I2C1_SetStop(ENABLE);
    I2C1_SendACK(DISABLE);
    I2C1_SetStop(ENABLE);
    I2C1_SendACK(ENABLE);
    return data;
}


// void EEPROM_Byte_Read(uint8_t addr,uint8_t *data,uint8_t numByteToRead)
// {
// 	I2C_GenerateSTART(EEPROM_I2C,ENABLE);//产生一个起始信号
// 	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);//检查EV5事件
// 	I2C_Send7bitAddress(EEPROM_I2C,I2C_EEPROM_Own_Addr,I2C_Direction_Transmitter);//发送7位设备地址
// 	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);//检查EV6事件
// 	I2C_SendData(EEPROM_I2C,addr);//检查EV6事件之后，发送要操作的存储位置的地址
// 	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING)==ERROR);//检查EV8事件
	
// 	//第二次起始信号
// 	I2C_GenerateSTART(EEPROM_I2C,ENABLE);//产生一个起始信号
// 	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);//检查EV5事件
// 	I2C_Send7bitAddress(EEPROM_I2C,I2C_EEPROM_Own_Addr,I2C_Direction_Receiver);//发送7位设备地址
// 	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==ERROR);//检查EV6事件
// 	while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_RECEIVED)==ERROR);//检查EV7事件,如果EV7事件被检测到，说明有新的数据接受到
// 	while(numByteToRead)
// 	{
// 		if(numByteToRead==1)
// 		{
// 			//如果是最后一个字节
// 			I2C_AcknowledgeConfig(EEPROM_I2C,DISABLE);
// 		}
// 		while(I2C_CheckEvent(EEPROM_I2C,I2C_EVENT_MASTER_BYTE_RECEIVED)==ERROR);//检查EV7事件,如果EV7事件被检测到，说明有新的数据接受到
// 	  *data=I2C_ReceiveData(EEPROM_I2C);
// 	  data++;
// 		numByteToRead--;
// 	}
// 	I2C_GenerateSTOP(EEPROM_I2C,ENABLE);//产生一个终止信号
// 	//发送数据完成后，ACk重新使能，以便下一次使用
// 	I2C_AcknowledgeConfig(EEPROM_I2C,ENABLE);
// }

// uint8_t I2C_CheckEvents(void)
//{
//     /*If bit ITEVTEN=1,there are all conditions satisfied:*/
//     /*-SB=1(Master Mode)*/
//     /*-ADDR=1(M/S Mode)*/
//     /*-ADD10=1(Master Mode)*/
//     /*-STOPF=1(Slave Mode)*/
//     /*-BTF=1(No TXE or RXNE event)*/
//     /*—ITBUFEN=1 && TxE=1*/
//     /*—ITBUFEN=1 && RxNE=1*/
//     return I2C1->CR2>>9;
// }

void I2C1_SendACK(FunctionalState NewState)
{
    if (NewState == ENABLE)
    {
        I2C1->CR1 |= 0x1 << 10;
    }
    else
    {
        I2C1->CR1 &= ~(0x1 << 10);
    }
}

void I2C1_EventDeal(uint8_t EVnum, uint8_t EVfnum)
{
    switch (EVnum)
    {
    case 5:
        EV5();
        break;
    case 6:
        //EV6();
        break;
    case 7:
        if (EVfnum == 1)
            EV7_1();
        else
            EV7();
        break;
    case 8:
        if (EVfnum == 1)
            EV8_1();
        else if (EVfnum == 2)
            EV8_2();
        else
            EV8();
        break;
    }
}

void EV5(void)
{
    // I2C1_CheckSB();
    /* BUSY, MSL and SB flag */
    while (!(BUSY_Flag()) && !(MSL_Flag()) && !(SB_Flag()))
        ;
}

void EV6(char* str)
{
    //    I2C1_CheckADDR();
    //    I2C1_CheckBUSY();
    if (strcmp(str, "MASTER_TRANSMITTER_MODE") == 0) {
        /* BUSY, MSL, ADDR, TXE and TRA flags */
        while (!(BUSY_Flag()) && !(MSL_Flag()) && !(ADDR_Flag()) && !(TxE_Flag()) && !(TRA_Flag()))
        ;
    } else if (strcmp(str, "MASTER_RECEIVER_MODE") == 0)
    {
        /* BUSY, MSL and ADDR flags */
        while (!(BUSY_Flag()) && !(MSL_Flag()) && !(ADDR_Flag()))
					;
    }   
}

void EV7(void)
{
    uint8_t i = 0;
    //		while(I2C1->SR1>>1 && i <1000)
    //			i++;
    //    while(!(I2C1->SR1>>6 & 0x1))
    //    {
    //        i++;
    //    }
    //    if(i >= 1000)
    //    {
    //        EVstate = 7;
    //        I2C_SetStop(ENABLE);
    //    }
    /* BUSY, MSL and RXNE flags */
    while (!(BUSY_Flag()) && !(MSL_Flag()) && !(I2C1->SR1 >> 6 & 0x1))
        ;
}

void EV7_1(void)
{
    I2C1_SetStop(ENABLE);
    I2C1_CheckRxNE();
}

void EV8(void)
{
    // I2C1_CheckTxE();
    /* TRA, BUSY, MSL, TXE flags */
    while (!(TRA_Flag()) && !(BUSY_Flag()) && !(MSL_Flag()) && !(TxE_Flag()))
        ;
}

void EV8_1(void)
{
    I2C1_CheckTxE();
}

void EV8_2(void)
{
    while (!(I2C1->SR1 >> 2 & 0X1) && !(I2C1->SR1 >> 7 & 0X1) && !(I2C1->SR2 & 0x1) && !(I2C1->SR2 & 0x2) && !(I2C1->SR2 & 0x4))
        ;
    I2C1_SetStop(ENABLE);
    //		I2C1_CheckBTF();
}

void MPU6050_Init(void)
{
    I2C1_Init();
    I2C1_Write_7bitmode_Register(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x00);
    I2C1_Write_7bitmode_Register(MPU6050_ADDR, MPU6050_PWR_MGMT_2, 0x00);
    /*DLPF_CFG=0*/
    I2C1_Write_7bitmode_Register(MPU6050_ADDR, MPU6050_CONFIG, 0x06);
    /*Sample rate is 1KHz now*/
    I2C1_Write_7bitmode_Register(MPU6050_ADDR, MPU6050_SMPRT_DIV, 0x07);
    /*Gyro Scale is from -1000°/s to +1000°/s*/
    I2C1_Write_7bitmode_Register(MPU6050_ADDR, MPU6050_GYRO_CONFIG, 0x18);
    /*ACCEL Scale is from -8g to +8g*/
    I2C1_Write_7bitmode_Register(MPU6050_ADDR, MPU6050_ACCEL_CONFIG, 0x18);
    /*Disable FIFO*/
    I2C1_Write_7bitmode_Register(MPU6050_ADDR, MPU6050_FIFO_EN, 0x00);
}

uint16_t I2C1_GetMPU6050X(void)
{
    uint16_t data = 0;
    data = (uint16_t)(I2C1_Read_7bitmode_Register(MPU6050_ADDR, MPU6050_ACCEL_XOUT_H) << 8) + I2C1_Read_7bitmode_Register(MPU6050_ADDR, MPU6050_ACCEL_XOUT_L);
    return data;
}

void MPU6050_GetAccData(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z)
{
}

void MPU6050_GetGyroData(int16_t *gryo_x, int16_t *gyro_y, int16_t *gryo_z)
{
}
