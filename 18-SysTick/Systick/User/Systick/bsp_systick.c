#include "bsp_systick.h"

static __IO uint32_t TimingDelay;

void SysTick_Init(void)
{
	if(HAL_SYSTICK_Config(SystemCoreClock / 100000))
	{
		while(1);
	}
}


void Delay_us(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);
}


void TimingDelay_Decrement(void)
{
	if(TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}

