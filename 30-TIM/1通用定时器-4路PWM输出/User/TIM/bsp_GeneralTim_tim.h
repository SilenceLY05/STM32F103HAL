#ifndef __BSP_GENERALTIM_H__
#define __BSP_GENERALTIM_H__

#include "stm32f1xx.h"



/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������Ĭ��ʹ��TIM3
#define GENERAL_TIM                         TIM3
#define GENERAL_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM3_CLK_ENABLE()
#define GENERAL_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM3_CLK_DISABLE()
// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��72MHz/��BASIC_TIMx_PRESCALER+1��
#define GENERAL_TIM_PRESCALER               71
// ���嶨ʱ�����ڣ�����ʱ����ʼ������BASIC_TIMx_PERIODֵ�Ǹ��¶�ʱ�������ɶ�Ӧ�¼����ж�
#define GENERAL_TIM_PERIOD                  9


//TIM3����Ƚ�ͨ��1
#define GENERAL_TIM_CH1_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define GENERAL_TIM_CH1_GPIO_PIN            GPIO_PIN_6
#define GENERAL_TIM_CH1_GPIO                GPIOA


//TIM3����Ƚ�ͨ��2
#define GENERAL_TIM_CH2_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define GENERAL_TIM_CH2_GPIO_PIN            GPIO_PIN_7
#define GENERAL_TIM_CH2_GPIO                GPIOA


//TIM3����Ƚ�ͨ��3
#define GENERAL_TIM_CH3_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define GENERAL_TIM_CH3_GPIO_PIN            GPIO_PIN_0
#define GENERAL_TIM_CH3_GPIO                GPIOB


//TIM3����Ƚ�ͨ��4
#define GENERAL_TIM_CH4_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define GENERAL_TIM_CH4_GPIO_PIN            GPIO_PIN_1
#define GENERAL_TIM_CH4_GPIO                GPIOB












extern TIM_HandleTypeDef TIM_TimeBase;



void GENERAL_TIM_Init(void);


#endif   /* __BSP_GENERALTIM_H__ */

