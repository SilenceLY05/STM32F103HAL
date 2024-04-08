#ifndef __BSP_TOUCHPAD_H__
#define __BSP_TOUCHPAD_H__

#include "stm32f1xx.h"

#define TOUCHPAD_GATE_VAL              70

// 定义定时器预分频，定时器实际时钟频率为：72MHz/（BASIC_TIMx_PRESCALER+1）
#define TOUCHPAD_TIMx_PRESCALER               47


// 定义定时器周期，当定时器开始计数到BASIC_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define TOUCHPAD_TIMx_PERIOD                  0xFFFF




#define TOUCHPAD_TIMx                       TIM5
#define TOUCHPAD_TIM_RCC_CLK_ENABLE()       __HAL_RCC_TIM5_CLK_ENABLE()
#define TOUCHPAD_TIM_RCC_CLK_DISABLE()      __HAL_RCC_TIM5_CLK_DISABLE()


#define TOUCHPAD_GPIO_RCC_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define TOUCHPAD_GPIO_PIN                   GPIO_PIN_1
#define TOUCHPAD_GPIO                       GPIOA
#define TOUCHPAD_TIM_CHANNEL                TIM_CHANNEL_2
#define TOUCHPAD_TIM_FLAG_CCR               TIM_FLAG_CC2






void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base);
void TPAD_TIM_Mode_Config(void);
uint8_t TPAD_Init(void);
uint8_t TPAD_Scan(uint8_t mode);



#endif   /* __BSP_TOUCHPAD_H__ */

