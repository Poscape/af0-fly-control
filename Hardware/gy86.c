#include "gy86.h"

void MPU6050Init(void)
{
	MyI2C_WriteRegister(AddressMPU6050, USER_CTRL, 0x01);
	MyI2C_WriteRegister(AddressMPU6050, PWR_MGMT_1, 0x01 << 7); // DEVICE_RESET
	MyI2C_WriteRegister(AddressMPU6050, PWR_MGMT_1, 0x00);
	MyI2C_WriteRegister(AddressMPU6050, SMPRT_DIV, 0x02);	 // SMPLRT_DIV:2      Sample Rate = 333Hz
	MyI2C_WriteRegister(AddressMPU6050, CONFIG, 0x03);		 // DLPF_CFG = 3
	MyI2C_WriteRegister(AddressMPU6050, GYRO_CONFIG, 0x18);	 // FS_SEL = 3		Full Scale Range of gyroscope = �� 2000 ��/s
	MyI2C_WriteRegister(AddressMPU6050, ACCEL_CONFIG, 0x10); // AFS_SEL = 2		Full scale range of accelerometer = �� 8g
	MyI2C_WriteRegister(AddressMPU6050, INT_PIN_CFG, 0x00);	 // I2C_bypass_en
}

uint16_t Get_MPU6050X(void)
{
	return MyI2C_ReadRegister_2Bytes(AddressMPU6050, ACCEL_XOUT_H);
}

uint16_t Get_MPU6050Y(void)
{
	return MyI2C_ReadRegister_2Bytes(AddressMPU6050, ACCEL_YOUT_H);
}

uint16_t Get_MPU6050Z(void)
{
	return MyI2C_ReadRegister_2Bytes(AddressMPU6050, ACCEL_ZOUT_H);
}

void HMC5883Init(void)
{
	MyI2C_WriteRegisterHMC5883(AddressHMC5883, ConfigA, 0x01); // Outout rate : 15Hz
	MyI2C_WriteRegisterHMC5883(AddressHMC5883, ConfigB, 0x20);
	MyI2C_WriteRegisterHMC5883(AddressHMC5883, ModeRegister, 0x00); // Continuous-Measurement Mode
	// StatusRegister = 0x01 when data prepared
}

uint16_t GetXHMC5883(void)
{
	return MyI2C_ReadRegister_1Bytes(AddressHMC5883, OutputXMSB) << 8 + MyI2C_ReadRegister_1Bytes(AddressHMC5883, OutputXLSB);
}
uint16_t GetYHMC5883(void)
{
	return MyI2C_ReadRegister_1Bytes(AddressHMC5883, OutputYMSB) << 8 + MyI2C_ReadRegister_1Bytes(AddressHMC5883, OutputYLSB);
}
uint16_t GetZHMC5883(void)
{
	return MyI2C_ReadRegister_1Bytes(AddressHMC5883, OutputZMSB) << 8 + MyI2C_ReadRegister_1Bytes(AddressHMC5883, OutputZLSB);
}

void MS5611Init(void)
{
}
