#include "bsp_advance_tim.h"


TIM_HandleTypeDef     TIM_TimeBaseStructure;
TIM_OC_InitTypeDef    TIM_OCInitStructure;

__IO uint16_t ChannelPulse = 500;


static void ADVANCE_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	
	ADVANCE_TIM_CH1_GPIO_CLK();
	ADVANCE_TIM_CH1N_GPIO_CLK();
	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = ADVANCE_TIM_CH1_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ADVANCE_TIM_CH1_PORT,&GPIO_InitStructure);
	
	ADVANCE_TIM_CH1N_GPIO_CLK();
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = ADVANCE_TIM_CH1N_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ADVANCE_TIM_CH1N_PORT,&GPIO_InitStructure);
	
}




static void ADVANCE_TIM_Mode_Config(void)
{
	TIM_BreakDeadTimeConfigTypeDef    TIM_BDTRInitStructure;
	
	ADVANCE_TIM_RCC_CLK_ENABLE();
	
	TIM_TimeBaseStructure.Instance = ADVANCE_TIMx;
	
	//ʱ�ӷ�Ƶ���� ��������ʱ��û��
	TIM_TimeBaseStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	
	//����������ģʽ��������ʱ��ֻ�����ϼ�����û�м���ģʽ������
	TIM_TimeBaseStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
	
	//�Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.Init.Period = 1000-1;
	
	//����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.Init.Prescaler = 72-1;
	
	//�ظ���������ֵ��������ʱ��û��
	TIM_TimeBaseStructure.Init.RepetitionCounter = 0;
	
	// ��ʼ����ʱ��
	HAL_TIM_PWM_Init(&TIM_TimeBaseStructure);
	
	/* ------------------ ����ṹ���ʼ�� --------------- */
	TIM_OC_InitTypeDef        TIM_OCInitStructure;
	
	//ʱ�ӷ�Ƶ���� ��������ʱ��û��
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_RESET;
	
	//����ΪPWMģʽ1
	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
	
	//�������ͨ�����е�ƽ��������
	TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_SET;
	
	TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_SET;
	
	//�������ͨ����ƽ��������
	TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;;
	
	TIM_OCInitStructure.Pulse = ChannelPulse;
	
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	//��ʼ��ͨ��1���PWM 
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure,&TIM_OCInitStructure,TIM_CHANNEL_1);
	
	/* �Զ����ʹ�ܣ���·������ʱ����������� */
	TIM_BDTRInitStructure.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
	TIM_BDTRInitStructure.BreakPolarity = TIM_BREAKPOLARITY_LOW;
	TIM_BDTRInitStructure.BreakState = TIM_BREAK_ENABLE;
	TIM_BDTRInitStructure.DeadTime = 11;
	TIM_BDTRInitStructure.LockLevel = TIM_LOCKLEVEL_1;
	TIM_BDTRInitStructure.OffStateIDLEMode = TIM_OSSI_ENABLE;
	TIM_BDTRInitStructure.OffStateRunMode = TIM_OSSR_ENABLE;
	
	HAL_TIMEx_ConfigBreakDeadTime(&TIM_TimeBaseStructure,&TIM_BDTRInitStructure);
	
	/* ��ʱ��ͨ��1���PWM */
	HAL_TIM_PWM_Start(&TIM_TimeBaseStructure,TIM_CHANNEL_1);
	/* ��ʱ��ͨ��1�������PWM */
	HAL_TIMEx_PWMN_Start(&TIM_TimeBaseStructure,TIM_CHANNEL_1);
	
}




void TIM__Config(void)
{
	ADVANCE_TIM_GPIO_Config();
	
	ADVANCE_TIM_Mode_Config();
	
	
}





