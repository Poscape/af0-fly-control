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
		OLED_ShowString(1,1,"CH1:");
		OLED_ShowString(1,8,"CH2:");
		OLED_ShowString(2,1,"CH3:");
		OLED_ShowString(2,8,"CH4:");
		for(int i=0;i<1000000;i++);
		OLED_ShowNum(1,5,dutyCycleArray[0],3);
		OLED_ShowNum(1,12,dutyCycleArray[1],3);
		OLED_ShowNum(2,5,dutyCycleArray[2],3);
		OLED_ShowNum(2,12,dutyCycleArray[3],3);
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
		x = Get_MPU6050X();
		y = Get_MPU6050Y();
		z = Get_MPU6050Z();
		INT32U tick2 = OSTimeGet();
		INT32U tick3 = tick2 - tick1;
		OSTimeDly(15);
	}
}