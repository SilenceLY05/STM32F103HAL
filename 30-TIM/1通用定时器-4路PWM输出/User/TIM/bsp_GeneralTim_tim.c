#include "bsp_GeneralTim_tim.h"


TIM_HandleTypeDef     TIM_TimeBase;


static void GENERAL_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef          GPIO_InitStructure;
	
	
	GENERAL_TIM_CH1_CLK_ENABLE();
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = GENERAL_TIM_CH1_GPIO_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GENERAL_TIM_CH1_GPIO,&GPIO_InitStructure);
	
	
	GENERAL_TIM_CH2_CLK_ENABLE();
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = GENERAL_TIM_CH2_GPIO_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GENERAL_TIM_CH2_GPIO,&GPIO_InitStructure);
	
	
	GENERAL_TIM_CH3_CLK_ENABLE();
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = GENERAL_TIM_CH3_GPIO_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GENERAL_TIM_CH3_GPIO,&GPIO_InitStructure);
	
	
	GENERAL_TIM_CH4_CLK_ENABLE();
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = GENERAL_TIM_CH4_GPIO_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GENERAL_TIM_CH4_GPIO,&GPIO_InitStructure);
}



///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */
void GENERAL_TIM_Mode_Config(void)
{
	GENERAL_TIM_RCC_CLK_ENABLE();
	
	TIM_TimeBase.Instance = GENERAL_TIM;
	// ʱ��Ԥ��Ƶ��
	TIM_TimeBase.Init.Prescaler = GENERAL_TIM_PRESCALER;
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBase.Init.Period = GENERAL_TIM_PERIOD;
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBase.Init.CounterMode = TIM_COUNTERMODE_UP;
	// ʱ�ӷ�Ƶ���� ��û�õ����ù�
	TIM_TimeBase.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_TimeBase.Init.RepetitionCounter = 0;
	// ��ʼ����ʱ��
	HAL_TIM_Base_Init(&TIM_TimeBase);
	
	
	/*--------------------����ȽϽṹ���ʼ��-------------------*/	
	// ռ�ձ�����
	uint16_t CCR1_Val = 5;
	uint16_t CCR2_Val = 4;
	uint16_t CCR3_Val = 3;
	uint16_t CCR4_Val = 2;
	
	
	TIM_OC_InitTypeDef   TIM_OCInitStructure;
	
	//ʱ�ӷ�Ƶ���� ��������ʱ��û��
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_RESET;
	
	//����ΪPWMģʽ1
	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
	
	//�������ͨ�����е�ƽ��������
	TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_SET;
	
	//�������ͨ����ƽ��������
	TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	
	//����Ƚ�ͨ��1
	TIM_OCInitStructure.Pulse = CCR1_Val;
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBase,&TIM_OCInitStructure,TIM_CHANNEL_1);	
	HAL_TIM_PWM_Start(&TIM_TimeBase,TIM_CHANNEL_1);
	
	//����Ƚ�ͨ��2
	TIM_OCInitStructure.Pulse = CCR2_Val;
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBase,&TIM_OCInitStructure,TIM_CHANNEL_2);	
	HAL_TIM_PWM_Start(&TIM_TimeBase,TIM_CHANNEL_2);
	
	//����Ƚ�ͨ��3
	TIM_OCInitStructure.Pulse = CCR3_Val;
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBase,&TIM_OCInitStructure,TIM_CHANNEL_3);	
	HAL_TIM_PWM_Start(&TIM_TimeBase,TIM_CHANNEL_3);
	
	//����Ƚ�ͨ��4
	TIM_OCInitStructure.Pulse = CCR4_Val;
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBase,&TIM_OCInitStructure,TIM_CHANNEL_4);	
	HAL_TIM_PWM_Start(&TIM_TimeBase,TIM_CHANNEL_4);
	
	
}


void GENERAL_TIM_Init(void)
{
	GENERAL_TIM_GPIO_Config();
	GENERAL_TIM_Mode_Config();
}


