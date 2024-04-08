#include "bsp_breathlight.h"

TIM_HandleTypeDef    TIM_Handle;

/* LED 亮度登记PWM表，指数曲线，此表使用工程目录下的python脚本生成 */
uint16_t indexWave[] = {
1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 4,
4, 5, 5, 6, 7, 8, 9, 10, 11, 13,
15, 17, 19, 22, 25, 28, 32, 36,
41, 47, 53, 61, 69, 79, 89, 102,
116, 131, 149, 170, 193, 219, 250,
284, 323, 367, 417, 474, 539, 613,
697, 792, 901, 1024, 1024, 901, 792,
697, 613, 539, 474, 417, 367, 323,
284, 250, 219, 193, 170, 149, 131, 
116, 102, 89, 79, 69, 61, 53, 47, 41,
36, 32, 28, 25, 22, 19, 17, 15, 13, 
11, 10, 9, 8, 7, 6, 5, 5, 4, 4, 3, 3,
2, 2, 2, 2, 1, 1, 1, 1
};



/* 计算PWM表有多少个元素 */
uint16_t POINT_NUM = sizeof(indexWave) / sizeof(indexWave[0]);


void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	BRE_TIM_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = BRE_RED_TIM_LED_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(BRE_RED_TIM_LED_PORT,&GPIO_InitStructure);
	
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	BRE_TIM_CLK_ENABLE();
	
	HAL_NVIC_SetPriority(BRE_TIM_IRQn,0,0);
	HAL_NVIC_EnableIRQ(BRE_TIM_IRQn);
}


void TIMx_Mode_Config(void)
{
	TIM_ClockConfigTypeDef  sClockSourceConfig;
	TIM_MasterConfigTypeDef  sMasterConfig;
	TIM_OC_InitTypeDef   sConfig;

	
	TIM_Handle.Instance = BRE_TIMx;
	TIM_Handle.Init.Prescaler = BRE_TIM_PRESCALER;
	TIM_Handle.Init.Period = BRE_TIM_PERIOD;
	TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	HAL_TIM_Base_Init(&TIM_Handle);
	
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&TIM_Handle,&sClockSourceConfig);
	
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&TIM_Handle,&sMasterConfig);
	
	
	sConfig.OCMode = TIM_OCMODE_PWM1;
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfig.OCFastMode = TIM_OCFAST_DISABLE;
	sConfig.Pulse = indexWave[0];
	HAL_TIM_PWM_ConfigChannel(&TIM_Handle,&sConfig,TIM_CHANNEL_3);
	
	HAL_TIM_MspPostInit(&TIM_Handle);
	
}

