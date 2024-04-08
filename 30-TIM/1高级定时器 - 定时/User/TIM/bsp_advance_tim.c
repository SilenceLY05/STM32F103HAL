#include "bsp_advance_tim.h"



TIM_HandleTypeDef     TIM_TimeBaseStructure;


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	/* ������ʱ������ʱ��ʹ�� */
	ADVANCE_TIM_RCC_CLK_ENABLE();
	
	/* �����ж����� */
	HAL_NVIC_SetPriority(ADVANCE_TIM_IRQn,1,0);
	HAL_NVIC_EnableIRQ(ADVANCE_TIM_IRQn);
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
void TIM_Mode_Config(void)
{
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_ClockConfigTypeDef sClockSourceConfig;
	
	TIM_TimeBaseStructure.Instance = ADVANCE_TIMx;
	// ʱ��Ԥ��Ƶ��
	TIM_TimeBaseStructure.Init.Prescaler = ADVANCE_TIMx_PRESCALER;
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.Init.Period = ADVANCE_TIMx_PERIOD;
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.Init.RepetitionCounter = ADVANCE_TIM_REPETITIONCOUNTER;
	// ʱ�ӷ�Ƶ���� ��û�õ����ù�
	TIM_TimeBaseStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	// ��ʼ����ʱ��
	HAL_TIM_Base_Init(&TIM_TimeBaseStructure);
	
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&TIM_TimeBaseStructure,&sClockSourceConfig);
	
	
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	
	HAL_TIMEx_MasterConfigSynchronization(&TIM_TimeBaseStructure,&sMasterConfig);
	
	HAL_TIM_Base_Start_IT(&TIM_TimeBaseStructure);
}


