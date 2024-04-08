#ifndef __BSP_ADVANCE_TIM_H__
#define __BSP_ADVANCE_TIM_H__

#include "stm32f1xx.h"


/************�߼���ʱ��TIM�������壬ֻ��TIM1��TIM8************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������ʹ�ø߼����ƶ�ʱ��TIM1


#define GENERAL_TIMx                       TIM8
#define GENERAL_TIM_RCC_CLK_ENABLE()       __HAL_RCC_TIM8_CLK_ENABLE()


#define GENERAL_TIM_CH1_PORT               GPIOA
#define GENERAL_TIM_CH1_PIN                GPIO_PIN_6
#define GENERAL_TIM_CH1_GPIO_CLK()         __HAL_RCC_GPIOA_CLK_ENABLE()






// PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define GENERAL_TIMx_PERIOD                  9
#define GENERAL_TIMx_PRESCALER               71
#define GENERAL_TIMx_PULSE                   5




#define ADVANCE_TIMx                       TIM1
#define ADVANCE_TIM_RCC_CLK_ENABLE()       __HAL_RCC_TIM1_CLK_ENABLE()
#define ADVANCE_TIM_RCC_CLK_DISABLE()      __HAL_RCC_TIM1_CLK_DISABLE()


// PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define ADVANCE_TIMx_PERIOD                  999
#define ADVANCE_TIMx_PRESCALER               71
//#define ADVANCE_TIMx_PULSE                   




#define ADVANCE_TIM_IRQn                   TIM1_UP_IRQn
#define ADVANCE_TIM_IRQHandler             TIM1_UP_IRQHandler


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



/* ֱ�Ӳ����Ĵ�������IO */
#define digitalH(p,i)                      {p->BSRR=i;}        //����Ϊ�ߵ�ƽ
#define digitalL(p,i)                      {p->BSRR = (uint32_t)i<<16;}        //����Ϊ�͵�ƽ



#define ADVANCE_TIM_BKIN_1()               digitalH(ADVANCE_TIM_BKIN_PORT,ADVANCE_TIM_BKIN_PIN)
#define ADVANCE_TIM_BKIN_0()               digitalL(ADVANCE_TIM_BKIN_PORT,ADVANCE_TIM_BKIN_PIN)








extern TIM_HandleTypeDef TIM_TimeBaseStructure;


void TIMx_Configuration(void);
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);


#endif   /* __BSP_ADVANCE_TIM_H__ */

