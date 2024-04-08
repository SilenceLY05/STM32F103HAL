#ifndef __BSP_GENERALTIM_H__
#define __BSP_GENERALTIM_H__

#include "stm32f1xx.h"


/**************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当需要哪个定时器的时候，只需要把下面的宏定义改成1即可
#define GENERAL_TIM2          1
#define GENERAL_TIM3          0
#define GENERAL_TIM4          0
#define GENERAL_TIM5          0


#if     GENERAL_TIM2
#define GENERAL_TIM                         TIM2
#define GENERAL_TIM_RCC_CLK_ENABLE()       __HAL_RCC_TIM2_CLK_ENABLE()
#define GENERAL_TIM_RCC_CLK_DISABLE()      __HAL_RCC_TIM2_CLK_DISABLE()
// 定义定时器预分频，定时器实际时钟频率为：72MHz/（BASIC_TIMx_PRESCALER+1）
#define GENERAL_TIM_PRESCALER               71
// 定义定时器周期，当定时器开始计数到BASIC_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define GENERAL_TIM_PERIOD                  0xFFFF
#define GENERAL_TIM_IRQn                   TIM2_IRQn
#define GENERAL_TIM_IRQHandler             TIM2_IRQHandler


#elif   GENERAL_TIM3
#define GENERAL_TIM                         TIM3
#define GENERAL_TIM_RCC_CLK_ENABLE()       __HAL_RCC_TIM3_CLK_ENABLE()
#define GENERAL_TIM_RCC_CLK_DISABLE()      __HAL_RCC_TIM3_CLK_DISABLE()
// 定义定时器预分频，定时器实际时钟频率为：72MHz/（BASIC_TIMx_PRESCALER+1）
#define GENERAL_TIM_PRESCALER               71
// 定义定时器周期，当定时器开始计数到BASIC_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define GENERAL_TIM_PERIOD                  0xFFFF
#define GENERAL_TIM_IRQn                   TIM3_IRQn
#define GENERAL_TIM_IRQHandler             TIM3_IRQHandler


#elif   GENERAL_TIM4
#define GENERAL_TIM                         TIM4
#define GENERAL_TIM_RCC_CLK_ENABLE()       __HAL_RCC_TIM4_CLK_ENABLE()
#define GENERAL_TIM_RCC_CLK_DISABLE()      __HAL_RCC_TIM4_CLK_DISABLE()
// 定义定时器预分频，定时器实际时钟频率为：72MHz/（BASIC_TIMx_PRESCALER+1）
#define GENERAL_TIM_PRESCALER               71
// 定义定时器周期，当定时器开始计数到BASIC_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define GENERAL_TIM_PERIOD                  0xFFFF
#define GENERAL_TIM_IRQn                   TIM4_IRQn
#define GENERAL_TIM_IRQHandler             TIM4_IRQHandler


#elif   GENERAL_TIM5
#define GENERAL_TIM                         TIM5
#define GENERAL_TIM_RCC_CLK_ENABLE()       __HAL_RCC_TIM5_CLK_ENABLE()
#define GENERAL_TIM_RCC_CLK_DISABLE()      __HAL_RCC_TIM5_CLK_DISABLE()
// 定义定时器预分频，定时器实际时钟频率为：72MHz/（BASIC_TIMx_PRESCALER+1）
#define GENERAL_TIM_PRESCALER               71
// 定义定时器周期，当定时器开始计数到BASIC_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define GENERAL_TIM_PERIOD                  0xFFFF
#define GENERAL_TIM_IRQn                   TIM5_IRQn
#define GENERAL_TIM_IRQHandler             TIM5_IRQHandler


#endif



extern TIM_HandleTypeDef     TIM_TimeBase;



void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base);

void GENERAL_TIM_Mode_Config(void);


#endif   /* __BSP_GENERALTIM_H__ */

