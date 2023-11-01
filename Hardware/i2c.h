#ifndef I2C_H
#define I2C_H
#include "stm32f4xx.h"

#define MPU6050_ADDRESS 0x68

#define MPU6050_AUX_VDDIO 0x01
#define MPU6050_SMPRT_DIV 0x19
#define MPU6050_CONFIG 0x1A
#define MPU6050_GYRO_CONFIG 0x1B
#define MPU6050_ACCEL_CONFIG 0x1C

/*Maybe will be used*/
#define MPU6050_USER_CTRL 0X6A
#define MPU6050_FF_THR 0x1D
#define MPU6050_FF_DUR 0x1E
#define MPU6050_FIFO_EN 0x23
#define MPU6050_FIFO_COUNT_H 0x72
#define MPU6050_FIFO_COUNT_L 0x73
#define MPU6050_FIFO_R_W 0x74
/*^^^   Reserved    ^^^*/

/*Accelerometer Mesurements*/
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_ACCEL_XOUT_L 0x3C
#define MPU6050_ACCEL_YOUT_H 0x3D
#define MPU6050_ACCEL_YOUT_L 0x3E
#define MPU6050_ACCEL_ZOUT_H 0x3F
#define MPU6050_ACCEL_ZOUT_L 0x40
/*Accelerometer Mesurements*/

/*Temperature Mesurements*/
#define MPU6050_TEMP_OUT_H 0x41
#define MPU6050_TEMP_OUT_L 0x42
/*Temperature Mesurements*/

/*Gyroscope Measurements*/
#define MPU6050_GYRO_XOUT_H 0x43
#define MPU6050_GYRO_XOUT_L 0x44
#define MPU6050_GYRO_YOUT_H 0x45
#define MPU6050_GYRO_YOUT_L 0x46
#define MPU6050_GYRO_ZOUT_H 0x47
#define MPU6050_GYRO_ZOUT_L 0x48
/*Gyroscope Measurements*/

/*Power Management*/
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_PWR_MGMT_2 0x6C
/*Power Management*/

/*Who Am I*/
#define MPU6050_WHO_AM_I 0x75
/*Who Am I*/

void GPIOB_I2C1_Init(uint8_t scl,uint8_t sda);
void I2C1_Init(void);
void I2C1_SetStart(FunctionalState NewState);
void I2C1_SetStop(FunctionalState NewState);
void I2C1_SendAddr(uint8_t addr);
void I2C1_CheckBUSY(void);
void I2C1_CheckMaster_TransmitterMode(void);
void I2C1_CheckMaster_ReceiverMode(void);
void I2C1_CheckSB(void);
void I2C1_CheckADDR(void);
void I2C1_CheckRxNE(void);
void I2C1_CheckTxE(void);
void I2C1_CheckBTF(void);
uint8_t I2C1_ReceiveByte(void);
void I2C1_Write7bitAddr(uint8_t addr,uint8_t Direction);
void I2C1_Write_7bitmode_Register(uint8_t DeviceAddr,uint8_t RegisterAddr,uint8_t Data);
uint8_t I2C1_Read_7bitmode_Register(uint8_t DeviceAddr,uint8_t RegisterAddr);
void I2C1_SendACK(FunctionalState NewState);
void I2C1_EventDeal(uint8_t EVnum,uint8_t EVfnum);
void EV5(void);
void EV6(char* str);
void EV7(void);
void EV7_1(void);
void EV8(void);
void EV8_1(void);
void EV8_2(void);
void MPU6050_Init(void);
uint16_t I2C1_GetMPU6050X(void);
void MPU6050_GetAccData(int16_t *acc_x,int16_t *acc_y,int16_t *acc_z);
void MPU6050_GetGyroData(int16_t *gyro_x,int16_t *gyro_y,int16_t *gyro_z);

#endif
