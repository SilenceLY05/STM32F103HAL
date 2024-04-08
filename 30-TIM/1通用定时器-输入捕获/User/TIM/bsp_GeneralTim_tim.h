#ifndef __BSP_GENERALTIM_H__
#define __BSP_GENERALTIM_H__

#include "stm32f1xx.h"


// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��72MHz/��BASIC_TIMx_PRESCALER+1��
#define GENERAL_TIMx_PRESCALER               71


// ���嶨ʱ�����ڣ�����ʱ����ʼ������BASIC_TIMx_PERIODֵ�Ǹ��¶�ʱ�������ɶ�Ӧ�¼����ж�
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



//��������ʼ����
#define GENERAL_TIM_START_ICPolarity       TIM_INPUTCHANNELPOLARITY_RISING
//�����Ľ�������
#define GENERAL_TIM_END_ICPolarity         TIM_INPUTCHANNELPOLARITY_FALLING


//��ʱ�����벶���û��Զ�������ṹ������
typedef struct
{
	uint8_t ucFinishing;        //���������־λ
	uint8_t ucStartFlag;        //����ʼ��־λ
	uint16_t usCtr;             //����Ĵ�����ֵ
	uint16_t usPeriod;          //�Զ���װ�ؼĴ������±�־
}STRUCT_CAPTURE;



extern TIM_HandleTypeDef TIM_TimeBase;
extern STRUCT_CAPTURE TIM_ICUserValueStructure;


void GENERAL_TIM_Mode_Config(void);


#endif   /* __BSP_GENERALTIM_H__ */

