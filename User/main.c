#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

#include "ucos_ii.h"
#include "os_cpu.h"

#include "tasks.h"

extern uint32_t SystemCoreClock;

OS_STK InitTaskStk[300];
void InitTask(void *p_arg);

int main(void)
{
	OS_CPU_SysTickInitFreq(84000000);

	USARTInit();
	OSInit();

	OSTaskCreate(InitTask, NULL, &InitTaskStk[799], 1);

	OSStart();

	return 0;
}

void InitTask(void *p_arg)
{
	Tim_Init();

	GY86_Init();

	OLED_Init();
	Motor_Init();
	LED_Init();
	ekf_init();

	OSTaskCreate(Task1000HZ, NULL, &Task1000HZStk[599], 2);
	// OSTaskCreate(GY86Task, NULL, &GY86TaskStk[99], 2);
	// OSTaskCreate(KalmanTask, NULL, &KalmanTaskStk[299], 3);
	// OSTaskCreate(SendTask, NULL, &SendTaskStk[99], 4);
	// OSTaskCreate(OLEDTask, NULL, &OLEDTaskStk[99], 5);

	OSTaskCreate(Task500HZ, NULL, &Task500HZStk[199], 3);
	// OSTaskCreate(InnerLoopTask, NULL, &InnerLoopTaskStk[99], 6);
	// OSTaskCreate(MotorTask, NULL, &MotorTaskStk[99], 7);

	OSTaskCreate(Task250HZ, NULL, &Task250HZStk[199], 4);
	// OSTaskCreate(OuterLoopTask, NULL, &OuterLoopTaskStk[99], 8);
	// OSTaskCreate(BlinkTask, NULL, &BlinkTaskStk[99], 9);

	OSTaskDel(1);
}
