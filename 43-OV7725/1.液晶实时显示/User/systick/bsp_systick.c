#include "bsp_systick.h"



static __IO uint32_t TimingDelay = 0;
__IO uint32_t g_ul_ms_ticks = 0;


void Systick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	if(SysTick_Config(SystemCoreClock /1000))
	{
		while(1);
	}
		
}


void TimingDelay_Decrement(void)
{
	if(TimingDelay != 0x00)
		TimingDelay--;
}


void mdelay(unsigned long nTime)
{
	TimingDelay = nTime;
	while(TimingDelay != 0);
}


int get_tick_count(unsigned long *count)
{
	count[0] = g_ul_ms_ticks;
	return 0;
}



void TimeStamp_Increment(void)
{
	g_ul_ms_ticks++;
}

