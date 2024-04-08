#include "bsp_advance_tim.h"


TIM_HandleTypeDef     TIM_TimeBaseStructure;

__IO uint16_t IC2Value = 0;
__IO uint16_t IC1Value = 0;
__IO float DutyCycle = 0;
__IO float Frequency = 0;


static void ADVANCE_TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
	
	GENERAL_TIM_RCC_CLK_ENABLE();
	ADVANCE_TIM_RCC_CLK_ENABLE();
	
	GENERAL_TIM_CH1_GPIO_CLK();
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = GENERAL_TIM_CH1_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GENERAL_TIM_CH1_PORT,&GPIO_InitStructure);
	
	ADVANCE_TIM_CH1_GPIO_CLK();
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = ADVANCE_TIM_CH1_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ADVANCE_TIM_CH1_PORT,&GPIO_InitStructure);
	
}

 /**
  * @brief  �߼����ƶ�ʱ�� TIMx,x[1,8]�ж����ȼ�����
  * @param  ��
  * @retval ��
  */
static void TIMx_NVIC_Configuration(void)
{
	//������ռ���ȼ��������ȼ�
	HAL_NVIC_SetPriority(ADVANCE_TIM_IRQn,0,3);
	// �����ж���Դ
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

static void ADVANCE_TIM_PWMOUTPUT_Config(void)
{
	ADVANCE_TIM_RCC_CLK_ENABLE();
	
	TIM_TimeBaseStructure.Instance = GENERAL_TIMx;
	
	GENERAL_TIM_RCC_CLK_ENABLE();
	//ʱ�ӷ�Ƶ���� ��������ʱ��û��
	TIM_TimeBaseStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	
	//����������ģʽ��������ʱ��ֻ�����ϼ�����û�м���ģʽ������
	TIM_TimeBaseStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
	
	//�Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.Init.Period = GENERAL_TIMx_PERIOD;
	
	//����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.Init.Prescaler = GENERAL_TIMx_PRESCALER;
	
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
	
	//�������ͨ����ƽ��������
	TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;;
	
	TIM_OCInitStructure.Pulse = GENERAL_TIMx_PULSE;
	
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBaseStructure,&TIM_OCInitStructure,TIM_CHANNEL_1);
	
	HAL_TIM_PWM_Start(&TIM_TimeBaseStructure,TIM_CHANNEL_1);
	
}




static void TIM_PWMINPUT_Config(void)
{
	TIM_IC_InitTypeDef    TIM_ICInitStructure;
	
	TIM_SlaveConfigTypeDef   TIM_SlaveConfigStructure;
	// ����TIMx_CLK,x[1,8] 
	ADVANCE_TIM_RCC_CLK_ENABLE();
	/* ���嶨ʱ���ľ����ȷ����ʱ���Ĵ����Ļ���ַ*/
	TIM_TimeBaseStructure.Instance = ADVANCE_TIMx;
	TIM_TimeBaseStructure.Init.Period = ADVANCE_TIMx_PERIOD;
	TIM_TimeBaseStructure.Init.Prescaler 	= ADVANCE_TIMx_PRESCALER;
	TIM_TimeBaseStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_TimeBaseStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_IC_Init(&TIM_TimeBaseStructure);
	
	/* IC1���������ش��� TI1FP1 */
	TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_RISING;
	TIM_ICInitStructure.ICSelection = TIM_ICSELECTION_DIRECTTI;
	TIM_ICInitStructure.ICFilter = 0x0;
	TIM_ICInitStructure.ICPrescaler = TIM_ICPSC_DIV1;
	HAL_TIM_IC_ConfigChannel(&TIM_TimeBaseStructure,&TIM_ICInitStructure,TIM_CHANNEL_1);
	
	
	/* IC2�����½��ش��� TI1FP2 */
	TIM_ICInitStructure.ICPolarity = TIM_ICPOLARITY_FALLING;
	TIM_ICInitStructure.ICSelection = TIM_ICSELECTION_INDIRECTTI;
	TIM_ICInitStructure.ICFilter = 0x0;
	TIM_ICInitStructure.ICPrescaler = TIM_ICPSC_DIV1;
	HAL_TIM_IC_ConfigChannel(&TIM_TimeBaseStructure,&TIM_ICInitStructure,TIM_CHANNEL_2);
	
	
	TIM_SlaveConfigStructure.SlaveMode = TIM_SLAVEMODE_RESET;
	TIM_SlaveConfigStructure.InputTrigger = TIM_TS_TI1FP1;
	HAL_TIM_SlaveConfigSynchronization(&TIM_TimeBaseStructure,&TIM_SlaveConfigStructure);
	
	
	/* ʹ�ܲ���/�Ƚ�2�ж����� */
	HAL_TIM_IC_Start_IT(&TIM_TimeBaseStructure,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&TIM_TimeBaseStructure,TIM_CHANNEL_2);
	
}


/**
  * @brief  ��ʼ���߼����ƶ�ʱ����ʱ��1ms����һ���ж�
  * @param  ��
  * @retval ��
  */
void TIMx_Configuration(void)
{
	ADVANCE_TIM_GPIO_Config();
	TIMx_NVIC_Configuration();
	ADVANCE_TIM_PWMOUTPUT_Config();
	TIM_PWMINPUT_Config();
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		/* ��ȡ���벶��ֵ */
		IC1Value = HAL_TIM_ReadCapturedValue(&TIM_TimeBaseStructure,TIM_CHANNEL_1);
		IC2Value = HAL_TIM_ReadCapturedValue(&TIM_TimeBaseStructure,TIM_CHANNEL_2);
		if(IC1Value != 0)
		{
			DutyCycle = (float)((IC2Value+1)*100) / (IC1Value+1);
			
			/* Ƶ�ʼ��� */
		Frequency = 7200000/72/(float)(IC1Value+1);
		
	  }
	  else
	  {
		DutyCycle = 0;
		Frequency = 0;
	  }
	}
}






