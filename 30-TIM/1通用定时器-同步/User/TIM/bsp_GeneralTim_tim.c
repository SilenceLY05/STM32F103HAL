#include "bsp_GeneralTim_tim.h"


static void TIMx_GPIO_Config(void)
{
	GPIO_InitTypeDef     GPIO_InitStructure;
	
	GENERAL_MASTER_CLK();
	GENERAL_SLAVE1_CLK();
	GENERAL_SLAVE2_CLK();
	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = GENERAL_MASTER_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GENERAL_MASTER_PORT,&GPIO_InitStructure);
	
	GENERAL_MASTER_AF();
	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = GENERAL_SLAVE1_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GENERAL_SLAVE1_PORT,&GPIO_InitStructure);
	
	GENERAL_SLAVE1_AF();
	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = GENERAL_SLAVE2_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GENERAL_SLAVE2_PORT,&GPIO_InitStructure);
	
}


/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)
 *-----------------------------------------------------------------------------
 */

static void TIM2_Mode_Config(void)
{
	TIM_OC_InitTypeDef    TIM_OCInitStructure;
	TIM_HandleTypeDef    TIM_TimeBase;
	TIM_MasterConfigTypeDef   sMasterConfig;
	
	GENERAL_TIM_MASTER_CLK();

	TIM_TimeBase.Instance = GENERAL_TIM_MASTER;
	TIM_TimeBase.Init.Period = 72;
	TIM_TimeBase.Init.Prescaler = 0;
	TIM_TimeBase.Init.ClockDivision = 0;
	TIM_TimeBase.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&TIM_TimeBase);
	
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;

	HAL_TIMEx_MasterConfigSynchronization(&TIM_TimeBase, &sMasterConfig);
	
	
	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
	TIM_OCInitStructure.Pulse = 36;
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBase, &TIM_OCInitStructure, TIM_CHANNEL_3);
	HAL_TIM_Base_Start_IT(&TIM_TimeBase);
}

static void TIM3_Mode_Config(void)
{
	TIM_OC_InitTypeDef    TIM_OCInitStructure;
	TIM_HandleTypeDef    TIM_TimeBase;
	TIM_SlaveConfigTypeDef   sSlaveConfig;
	
	GENERAL_TIM_SLAVE1_CLK();
	
	TIM_TimeBase.Instance = GENERAL_TIM_SLAVE1;
	TIM_TimeBase.Init.Period = 4;
	TIM_TimeBase.Init.Prescaler = 0;
	TIM_TimeBase.Init.ClockDivision = 0;
	TIM_TimeBase.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&TIM_TimeBase);
	
	sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
	sSlaveConfig.InputTrigger = TIM_TS_ITR0;
	HAL_TIM_SlaveConfigSynchronization(&TIM_TimeBase,&sSlaveConfig);
	
	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
	TIM_OCInitStructure.Pulse = 36;
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBase, &TIM_OCInitStructure, TIM_CHANNEL_3);
	
	HAL_TIM_Base_Start_IT(&TIM_TimeBase);
}


static void TIM4_Mode_Config(void)
{
	TIM_OC_InitTypeDef    TIM_OCInitStructure;
	TIM_HandleTypeDef    TIM_TimeBase;
	TIM_SlaveConfigTypeDef   sSlaveConfig;
	
	GENERAL_TIM_SLAVE2_CLK();
	
	TIM_TimeBase.Instance = GENERAL_TIM_SLAVE2;
	TIM_TimeBase.Init.Period = 4;
	TIM_TimeBase.Init.Prescaler = 0;
	TIM_TimeBase.Init.ClockDivision = 0;
	TIM_TimeBase.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&TIM_TimeBase);
	
	sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
	sSlaveConfig.InputTrigger = TIM_TS_ITR0;
	HAL_TIM_SlaveConfigSynchronization(&TIM_TimeBase,&sSlaveConfig);
	
	
	
	TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
	TIM_OCInitStructure.Pulse = 36;
	TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
	HAL_TIM_PWM_ConfigChannel(&TIM_TimeBase, &TIM_OCInitStructure, TIM_CHANNEL_3);
	
	HAL_TIM_Base_Start_IT(&TIM_TimeBase);
}


void TIM_Configuration(void)
{
	TIMx_GPIO_Config();
	TIM2_Mode_Config();
	TIM3_Mode_Config();
	TIM4_Mode_Config();
}


