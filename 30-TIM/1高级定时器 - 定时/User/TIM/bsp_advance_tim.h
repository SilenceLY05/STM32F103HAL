#ifndef __BSP_ADVANCE_TIM_H__
#define __BSP_ADVANCE_TIM_H__

#include "stm32f1xx.h"


// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��72MHz/��BASIC_TIMx_PRESCALER+1��
#define ADVANCE_TIMx_PRESCALER               71

//�߼���ʱ���ظ������Ĵ���ֵ
#define ADVANCE_TIM_REPETITIONCOUNTER        9


// ���嶨ʱ�����ڣ�����ʱ����ʼ������BASIC_TIMx_PERIODֵ�Ǹ��¶�ʱ�������ɶ�Ӧ�¼����ж�
#define ADVANCE_TIMx_PERIOD                  1000




#define ADVANCE_TIMx                       TIM1
#define ADVANCE_TIM_RCC_CLK_ENABLE()       __HAL_RCC_TIM1_CLK_ENABLE()
#define ADVANCE_TIM_RCC_CLK_DISABLE()      __HAL_RCC_TIM1_CLK_DISABLE()
#define ADVANCE_TIM_IRQn                   TIM1_UP_IRQn
#define ADVANCE_TIM_IRQHandler             TIM1_UP_IRQHandler


extern TIM_HandleTypeDef TIM_TimeBaseStructure;


void TIM_Mode_Config(void);


#endif   /* __BSP_ADVANCE_TIM_H__ */

