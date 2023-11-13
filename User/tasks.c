#include "tasks.h"

int16_t accx = 0, accy = 0, accz = 0;
int16_t gyrox = 0, gyroy = 0, gyroz = 0;
int16_t magx = 0, magy = 0, magz = 0;

extern int dutyCycleArray[6];

/* Time */
INT32U gy86_time = 0;
INT32U kalman_time = 0;
INT32U send_time = 0;

INT32U inner_loop_time = 0;
INT32U motor_time = 0;

INT32U outer_loop_time = 0;
/* Time End */

/* Task Stacks */
OS_STK GY86TaskStk[100];
OS_STK KalmanTaskStk[300];
OS_STK SendTaskStk[100];
OS_STK OLEDTaskStk[100];

OS_STK InnerLoopTaskStk[100];
OS_STK MotorTaskStk[100];

OS_STK OuterLoopTaskStk[100];
OS_STK BlinkTaskStk[100];
/* Task Stacks End */

/*
*********************************************************************************************************
*                                             1000Hz TASKS
*
* Includes:
* 1. GY86Task
* 2. KalmanTask
* 3. SendTask
* 4. OLEDTask
*********************************************************************************************************
*/

void GY86Task(void *p_arg)
{
	// Gy86: MPU6050(Accelerometer, Gyroscope), HMC5883(Magnetometer)
	while (1)
	{
		INT32U tick1 = OSTimeGet();

		accx = (int16_t)I2C1_GetAccX();
		accy = (int16_t)I2C1_GetAccY();
		accz = (int16_t)I2C1_GetAccZ();

		gyrox = (int16_t)I2C1_GetGyroX();
		gyroy = (int16_t)I2C1_GetGyroY();
		gyroz = (int16_t)I2C1_GetGyroZ();

		magx = (int16_t)I2C1_GetMagX();
		magy = (int16_t)I2C1_GetMagY();
		magz = (int16_t)I2C1_GetMagZ();

		INT32U tick2 = OSTimeGet();
		gy86_time = tick2 - tick1;

		OSTimeDlyHMSM(0, 0, 0, 1);
	}
}

void KalmanTask(void *p_arg)
{
	while (1)
	{
		INT32U tick1 = OSTimeGet();

		// TODO: Kalman Filter

		INT32U tick2 = OSTimeGet();
		kalman_time = tick2 - tick1;

		OSTimeDlyHMSM(0, 0, 0, 1);
	}
}

void SendTask(void *p_arg)
{
	while (1)
	{
		INT32U tick1 = OSTimeGet();

		int8_t data[12];

		data[0] = (int8_t)(accx >> 8);
		data[1] = (int8_t)(accx & 0xff);
		data[2] = (int8_t)(accy >> 8);
		data[3] = (int8_t)(accy & 0xff);
		data[4] = (int8_t)(accz >> 8);
		data[5] = (int8_t)(accz & 0xff);
		data[6] = (int8_t)(gyrox >> 8);
		data[7] = (int8_t)(gyrox & 0xff);
		data[8] = (int8_t)(gyroy >> 8);
		data[9] = (int8_t)(gyroy & 0xff);
		data[10] = (int8_t)(gyroz >> 8);
		data[11] = (int8_t)(gyroz & 0xff);
		FANO_Send_ACC_GRY(data);

		data[0] = (int8_t)(magx >> 8);
		data[1] = (int8_t)(magx & 0xff);
		data[2] = (int8_t)(magy >> 8);
		data[3] = (int8_t)(magy & 0xff);
		data[4] = (int8_t)(magz >> 8);
		data[5] = (int8_t)(magz & 0xff);
		FANO_Send_MAG(data);

		INT32U tick2 = OSTimeGet();
		send_time = tick2 - tick1;

		OSTimeDlyHMSM(0, 0, 0, 1);
	}
}

void OLEDTask(void *p_arg)
{
	int mode = 1;

	while (1)
	{
		OLED_ShowString(1, 1, "T:");
		OLED_ShowNum(1, 3, OSTime, 5);

		OLED_ShowString(1, 9, "COS:");
		OLED_ShowNum(1, 13, gy86_time, 4);

		if (mode == 0)
		{
			OLED_ShowString(2, 1, "X");
			OLED_ShowSignedNum(2, 2, accx, 4);
			OLED_ShowSignedNum(2, 7, gyrox, 4);
			OLED_ShowSignedNum(2, 12, magx, 4);

			OLED_ShowString(3, 1, "Y");
			OLED_ShowSignedNum(3, 2, accy, 4);
			OLED_ShowSignedNum(3, 7, gyroy, 4);
			OLED_ShowSignedNum(3, 12, magy, 4);

			OLED_ShowString(4, 1, "Z");
			OLED_ShowSignedNum(4, 2, accz, 4);
			OLED_ShowSignedNum(4, 7, gyroz, 4);
			OLED_ShowSignedNum(4, 12, magz, 4);
		}
		else if (mode == 1)
		{
			OLED_ShowNum(2, 1, dutyCycleArray[0], 3);
			OLED_ShowNum(2, 5, dutyCycleArray[1], 3);
			OLED_ShowNum(3, 1, dutyCycleArray[2], 3);
			OLED_ShowNum(3, 5, dutyCycleArray[3], 3);
			OLED_ShowNum(4, 1, dutyCycleArray[4], 3);
			OLED_ShowNum(4, 5, dutyCycleArray[5], 3);
		}

		OSTimeDlyHMSM(0, 0, 0, 1);
	}
}

/*
*********************************************************************************************************
*                                             500Hz TASKS
*
* Includes:
* 1.InnerLoopTask
* 2.MotorTask
*********************************************************************************************************
*/

void InnerLoopTask(void *p_arg)
{
	while (1)
	{
		INT32U tick1 = OSTimeGet();

		// TODO: Inner Loop

		INT32U tick2 = OSTimeGet();
		inner_loop_time = tick2 - tick1;

		OSTimeDlyHMSM(0, 0, 0, 2);
	}
}

void MotorTask(void *p_arg)
{
	while (1)
	{
		INT32U tick1 = OSTimeGet();

		PWM_output();

		INT32U tick2 = OSTimeGet();
		motor_time = tick2 - tick1;

		OSTimeDlyHMSM(0, 0, 0, 2);
	}
}

/*
*********************************************************************************************************
*                                             250Hz TASKS
*
* Includes:
* 1. OuterLoopTask
* 2. BlinkTask
*********************************************************************************************************
*/

void OuterLoopTask(void *p_arg)
{
	while (1)
	{
		INT32U tick1 = OSTimeGet();

		// TODO: Outer Loop

		INT32U tick2 = OSTimeGet();
		outer_loop_time = tick2 - tick1;

		OSTimeDlyHMSM(0, 0, 0, 4);
	}
}

void BlinkTask(void *p_arg)
{
	BOOLEAN flag = 0;

	while (1)
	{
		if (flag == 0)
			LED_On();
		else
			LED_Off();

		flag = !flag;

		OSTimeDlyHMSM(0, 0, 0, 4);
	}
}