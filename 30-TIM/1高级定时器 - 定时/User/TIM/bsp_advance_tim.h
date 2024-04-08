#ifndef __BSP_ADVANCE_TIM_H__
#define __BSP_ADVANCE_TIM_H__

#include "stm32f1xx.h"


// 定义定时器预分频，定时器实际时钟频率为：72MHz/（BASIC_TIMx_PRESCALER+1）
#define ADVANCE_TIMx_PRESCALER               71

//高级定时器重复计数寄存器值
#define ADVANCE_TIM_REPETITIONCOUNTER        9


// 定义定时器周期，当定时器开始计数到BASIC_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define ADVANCE_TIMx_PERIOD                  1000




#define ADVANCE_TIMx                       TIM1
#define ADVANCE_TIM_RCC_CLK_ENABLE()       __HAL_RCC_TIM1_CLK_ENABLE()
#define ADVANCE_TIM_RCC_CLK_DISABLE()      __HAL_RCC_TIM1_CLK_DISABLE()
#define ADVANCE_TIM_IRQn                   TIM1_UP_IRQn
#define ADVANCE_TIM_IRQHandler             TIM1_UP_IRQHandler


extern TIM_HandleTypeDef TIM_TimeBaseStructure;


void TIM_Mode_Config(void);


#endif   /* __BSP_ADVANCE_TIM_H__ */

