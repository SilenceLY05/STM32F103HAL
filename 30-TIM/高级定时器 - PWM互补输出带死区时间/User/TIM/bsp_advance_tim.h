#ifndef __BSP_ADVANCE_TIM_H__
#define __BSP_ADVANCE_TIM_H__

#include "stm32f1xx.h"


/************�߼���ʱ��TIM�������壬ֻ��TIM1��TIM8************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������ʹ�ø߼����ƶ�ʱ��TIM1
#define ADVANCE_TIMx                       TIM1
#define ADVANCE_TIM_RCC_CLK_ENABLE()       __HAL_RCC_TIM1_CLK_ENABLE()
#define ADVANCE_TIM_RCC_CLK_DISABLE()      __HAL_RCC_TIM1_CLK_DISABLE()


//TIM1 ����Ƚ�ͨ��
#define ADVANCE_TIM_CH1_GPIO_CLK()         __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADVANCE_TIM_CH1_PORT               GPIOA
#define ADVANCE_TIM_CH1_PIN                GPIO_PIN_8



//TIM1 ����Ƚ�ͨ���Ļ���ͨ��
#define ADVANCE_TIM_CH1N_GPIO_CLK()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define ADVANCE_TIM_CH1N_PORT              GPIOB
#define ADVANCE_TIM_CH1N_PIN               GPIO_PIN_13


//TIM1����Ƚ�ͨ����ɲ��ͨ��
#define ADVANCE_TIM_BKIN_GPIO_CLK()        __HAL_RCC_GPIOB_CLK_ENABLE();
#define ADVANCE_TIM_BKIN_PORT              GPIOB
#define ADVANCE_TIM_BKIN_PIN               GPIO_PIN_12







extern TIM_HandleTypeDef TIM_TimeBaseStructure;


void TIM__Config(void);


#endif   /* __BSP_ADVANCE_TIM_H__ */

