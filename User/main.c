#include "stm32f4xx.h"
#include "system_stm32f4xx.h"

extern uint32_t SystemCoreClock;

int main(void)
{
	SystemCoreClockUpdate();
	return 0;
}