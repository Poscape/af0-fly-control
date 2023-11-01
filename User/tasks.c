#include "tasks.h"

uint16_t x = 0, y = 0, z = 0;

OS_STK OledTaskStk[100];
OS_STK TimTaskStk[100];
OS_STK MotorTaskStk[100];
OS_STK GY86TaskStk[100];

OS_STK TestTaskStk1[100];
OS_STK TestTaskStk2[100];
OS_STK TestTaskStk3[100];
OS_STK TestTaskStk4[100];

uint16_t data;


void TestTask1(void *p_arg)
{
	while(1){
		LED_On();
		delay_ms(100000000);
		OSTimeDly(10);
	}
}

void TestTask2(void *p_arg)
{
	while(1){
		LED_Off();
		OSTimeDly(10);
	}
}


void TestTask3(void *p_arg)
{
	while(1){
		SendString("abc\r\n");
		OSTimeDly(100);
	}
}


void TestTask4(void *p_arg)
{
	while(1){
		OLED_ShowChar(2,2,ReadByte());
		for(int i=0;i<10000000;i++);
		OLED_Clear();
		OSTimeDly(100);
	}
}


void OledTask(void *p_arg)
{
	while(1){
		OLED_ShowString(1,1,"CurPri:");
		OLED_ShowNum(1,8,OSPrioCur,8);
		OLED_ShowString(2,1,"HiRdy:");
		OLED_ShowNum(2,8,OSPrioHighRdy,8);
		OLED_ShowString(3,1,"OSTIME:");
		OLED_ShowNum(3,8,OSTime,8);
		OLED_ShowString(4,1,"6050X:");
		OLED_ShowNum(4,8,data,8);
		OSTimeDly(30);
	}
}


void TimTask(void *p_arg)
{
	while(1){
		PWM_output();
		OSTimeDly(15);
	}
	
}


void MotorInitTask(void *p_arg)
{
	
	setPWMDutyCycle(TIM1, 1,  80);
	setPWMDutyCycle(TIM1, 2,  80);
	setPWMDutyCycle(TIM1, 3,  80);
	setPWMDutyCycle(TIM1, 4,  80);
	delay_ms(1000000000);
	delay_ms(1000000000);
	delay_ms(1000000000);
	
	setPWMDutyCycle(TIM1, 1,  60);
	setPWMDutyCycle(TIM1, 2,  60);
	setPWMDutyCycle(TIM1, 3,  60);
	setPWMDutyCycle(TIM1, 4,  60);
	delay_ms(1000000000/2);
	delay_ms(1000000000);
	OSTaskDel(1);
}


void MPU6050Task(void *p_arg)
{
	while(1){
		INT32U tick1 = OSTimeGet();
		data = I2C1_GetMPU6050X();
		INT32U tick2 = OSTimeGet();
		INT32U tick3 = tick2 - tick1;
		OSTimeDly(15);
	}
}