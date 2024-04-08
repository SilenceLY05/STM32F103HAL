#ifndef __BSP_GENERALTIM_H__
#define __BSP_GENERALTIM_H__

#include "stm32f1xx.h"


// 定义定时器预分频，定时器实际时钟频率为：72MHz/（BASIC_TIMx_PRESCALER+1）
#define GENERAL_TIMx_PRESCALER               71


// 定义定时器周期，当定时器开始计数到BASIC_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define GENERAL_TIMx_PERIOD                  0xFFFF




#define GENERAL_TIMx                       TIM5
#define GENERAL_TIM_RCC_CLK_ENABLE()       __HAL_RCC_TIM5_CLK_ENABLE()
#define GENERAL_TIM_RCC_CLK_DISABLE()      __HAL_RCC_TIM5_CLK_DISABLE()
#define GENERAL_TIM_GPIO_RCC_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()
#define GENERAL_TIM_CHANNELx               TIM_CHANNEL_1
#define GENERAL_TIM_IT_CCx                 TIM_IT_CC1

#define GENERAL_TIM_IRQn                   TIM5_IRQn
#define GENERAL_TIM_IRQHandler             TIM5_IRQHandler

#define KEY1_RCC_CLK_ENABLE()              __HAL_RCC_GPIOA_CLK_ENABLE()
#define KEY1_GPIO_PIN                      GPIO_PIN_0
#define KEY1_GPIO                          GPIOA



//测量的起始边沿
#define GENERAL_TIM_START_ICPolarity       TIM_INPUTCHANNELPOLARITY_RISING
//测量的结束边沿
#define GENERAL_TIM_END_ICPolarity         TIM_INPUTCHANNELPOLARITY_FALLING


//定时器输入捕获用户自定义变量结构体声明
typedef struct
{
	uint8_t ucFinishing;        //捕获结束标志位
	uint8_t ucStartFlag;        //捕获开始标志位
	uint16_t usCtr;             //捕获寄存器的值
	uint16_t usPeriod;          //自动重装载寄存器更新标志
}STRUCT_CAPTURE;



extern TIM_HandleTypeDef TIM_TimeBase;
extern STRUCT_CAPTURE TIM_ICUserValueStructure;


void GENERAL_TIM_Mode_Config(void);


#endif   /* __BSP_GENERALTIM_H__ */

