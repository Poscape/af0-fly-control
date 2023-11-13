#ifndef __TASKS_H
#define __TASKS_H

#include "stm32f4xx.h"

#include "ucos_ii.h"
#include "os_cpu.h"

#include "led.h"
#include "delay.h"
#include "oled.h"
#include "usart.h"
#include "tim.h"
#include "gy86.h"
#include "i2c.h"
#include "ano.h"

extern int dutyCycleArray[6];

/* Task Stacks */
extern OS_STK GY86TaskStk[];
extern OS_STK KalmanTaskStk[];
extern OS_STK SendTaskStk[];
extern OS_STK OLEDTaskStk[];

extern OS_STK InnerLoopTaskStk[];
extern OS_STK MotorTaskStk[];

extern OS_STK OuterLoopTaskStk[];
extern OS_STK BlinkTaskStk[];
/* Task Stacks End */

extern INT8U OSPrioCur;
extern INT8U OSPrioHighRdy;

// 1000Hz Tasks
void GY86Task(void *p_arg);
void KalmanTask(void *p_arg);
void SendTask(void *p_arg);
void OLEDTask(void *p_arg);

// 500Hz Tasks
void InnerLoopTask(void *p_arg);
void MotorTask(void *p_arg);

// 250Hz Tasks
void OuterLoopTask(void *p_arg);
void BlinkTask(void *p_arg);

#endif /* TASKS */