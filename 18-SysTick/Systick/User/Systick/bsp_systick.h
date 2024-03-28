#ifndef __BSP_SYSTICK_H__
#define __BSP_SYSTICK_H__


#include "stm32f1xx.h"

typedef uint32_t u32;



void SysTick_Init(void);
void Delay_us(__IO uint32_t nTime);
void TimingDelay_Decrement(void);






#endif /* __BSP_SYSTICK_H__ */

