#include "bsp_breathlight.h"

TIM_HandleTypeDef    TIM_Handle;

/* LED 亮度登记PWM表，指数曲线，此表使用工程目录下的python脚本生成 */
uint16_t indexWave[] = {
1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 
	2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5,
	5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10,
	11, 12, 12, 13, 14, 15, 17, 18,
	19, 20, 22, 23, 25, 27, 29, 31, 
	33, 36, 38, 41, 44, 47, 51, 54, 
	58, 63, 67, 72, 77, 83, 89, 95, 
	102, 110, 117, 126, 135, 145, 156, 
	167, 179, 192, 206, 221, 237, 254,
	272, 292, 313, 336, 361, 387, 415,
	445, 477, 512, 512, 477, 445, 415, 
	387, 361, 336, 313, 292, 272, 254, 
	237, 221, 206, 192, 179, 167, 156, 
	145, 135, 126, 117, 110, 102, 95, 
	89, 83, 77, 72, 67, 63, 58, 54, 51, 
	47, 44, 41, 38, 36, 33, 31, 29, 27,
	25, 23, 22, 20, 19, 18, 17, 15, 14,
	13, 12, 12, 11, 10, 9, 9, 8, 8, 7, 7,
	6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 
	3, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1
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
	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = BRE_GREEN_TIM_LED_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(BRE_GREEN_TIM_LED_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = BRE_BLUE_TIM_LED_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(BRE_BLUE_TIM_LED_PORT,&GPIO_InitStructure);
	
}


void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	__HAL_RCC_AFIO_CLK_ENABLE();
	
	__HAL_AFIO_REMAP_TIM3_PARTIAL();
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
	
	HAL_TIM_PWM_ConfigChannel(&TIM_Handle,&sConfig,TIM_CHANNEL_2);
	
	HAL_TIM_PWM_ConfigChannel(&TIM_Handle,&sConfig,TIM_CHANNEL_4);
	
	HAL_TIM_MspPostInit(&TIM_Handle);
	
}

