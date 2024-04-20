#ifndef __BSP_SYSTICK_H__
#define __BSP_SYSTICK_H__


#include "stm32f1xx.h"

#define TASK_ENABLE    0
#define NumOfTask      3



void Systick_Init(void);
void TimingDelay_Decrement(void);
void mdelay(unsigned long nTime);
int get_tick_count(unsigned long *count);
void TimeStamp_Increment(void);




#endif /* __BSP_SYSTICK_H__ */

