#include "bsp_GeneralTim_tim.h"

//��ʱ�����벶���û��Զ�������ṹ�嶨��
STRUCT_CAPTURE TIM_ICUserValueStructure = {0,0,0,0};

TIM_HandleTypeDef     TIM_TimeBase;


//�������ж����ȼ�����
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	/* ������ʱ������ʱ��ʹ�� */
	GENERAL_TIM_RCC_CLK_ENABLE();
	
	KEY1_RCC_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pin = KEY1_GPIO_PIN;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(KEY1_GPIO,&GPIO_InitStructure);
	
	/* �����ж����� */
	HAL_NVIC_SetPriority(GENERAL_TIM_IRQn,0,0);
	HAL_NVIC_EnableIRQ(GENERAL_TIM_IRQn);
}



/*
 * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
 * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
 * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         ����
 * TIM_CounterMode			 TIMx,x[6,7]û�У��������У�������ʱ����
 * TIM_Period            ����
 * TIM_ClockDivision     TIMx,x[6,7]û�У���������(������ʱ��)
 * TIM_RepetitionCounter TIMx,x[1,8]����(�߼���ʱ��)
 *-----------------------------------------------------------------------------
 */
void GENERAL_TIM_Mode_Config(void)
{
	TIM_MasterConfigTypeDef MasterConfig;
	TIM_ClockConfigTypeDef ClockSource;
	TIM_IC_InitTypeDef ICConfig;
	
	TIM_TimeBase.Instance = GENERAL_TIMx;
	// ʱ��Ԥ��Ƶ��
	TIM_TimeBase.Init.Prescaler = GENERAL_TIMx_PRESCALER;
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBase.Init.Period = GENERAL_TIMx_PERIOD;
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBase.Init.CounterMode = TIM_COUNTERMODE_UP;
	// ʱ�ӷ�Ƶ���� ��û�õ����ù�
	TIM_TimeBase.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	// ��ʼ����ʱ��
	HAL_TIM_Base_Init(&TIM_TimeBase);
	
	ClockSource.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&TIM_TimeBase,&ClockSource);
	
	
	MasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	MasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	
	HAL_TIMEx_MasterConfigSynchronization(&TIM_TimeBase,&MasterConfig);
	
	ICConfig.ICFilter = 0;
	ICConfig.ICPolarity = GENERAL_TIM_START_ICPolarity;
	ICConfig.ICPrescaler = TIM_ICPSC_DIV1;
	ICConfig.ICSelection = TIM_ICSELECTION_DIRECTTI;
	
	HAL_TIM_IC_ConfigChannel(& TIM_TimeBase, &ICConfig, GENERAL_TIM_CHANNELx);
}


