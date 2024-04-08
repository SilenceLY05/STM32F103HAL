#ifndef __BSP_GENERALTIM_H__
#define __BSP_GENERALTIM_H__

#include "stm32f1xx.h"



/************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 我们这里默认使用TIM3
#define GENERAL_TIM                         TIM3
#define GENERAL_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM3_CLK_ENABLE()
#define GENERAL_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM3_CLK_DISABLE()
// 定义定时器预分频，定时器实际时钟频率为：72MHz/（BASIC_TIMx_PRESCALER+1）
#define GENERAL_TIM_PRESCALER               71
// 定义定时器周期，当定时器开始计数到BASIC_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define GENERAL_TIM_PERIOD                  9


//TIM3输出比较通道1
#define GENERAL_TIM_CH1_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define GENERAL_TIM_CH1_GPIO_PIN            GPIO_PIN_6
#define GENERAL_TIM_CH1_GPIO                GPIOA


//TIM3输出比较通道2
#define GENERAL_TIM_CH2_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define GENERAL_TIM_CH2_GPIO_PIN            GPIO_PIN_7
#define GENERAL_TIM_CH2_GPIO                GPIOA


//TIM3输出比较通道3
#define GENERAL_TIM_CH3_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define GENERAL_TIM_CH3_GPIO_PIN            GPIO_PIN_0
#define GENERAL_TIM_CH3_GPIO                GPIOB


//TIM3输出比较通道4
#define GENERAL_TIM_CH4_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define GENERAL_TIM_CH4_GPIO_PIN            GPIO_PIN_1
#define GENERAL_TIM_CH4_GPIO                GPIOB












extern TIM_HandleTypeDef TIM_TimeBase;



void GENERAL_TIM_Init(void);


#endif   /* __BSP_GENERALTIM_H__ */

