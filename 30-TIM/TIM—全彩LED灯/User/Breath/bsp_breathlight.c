#include "bsp_breathlight.h"

TIM_HandleTypeDef    TIM_Handle;



void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	
	COLOR_TIM_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = COLOR_RED_TIM_LED_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(COLOR_RED_TIM_LED_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = COLOR_GREEN_TIM_LED_PIN;
	HAL_GPIO_Init(COLOR_GREEN_TIM_LED_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = COLOR_BLUE_TIM_LED_PIN;
	HAL_GPIO_Init(COLOR_BLUE_TIM_LED_PORT,&GPIO_InitStructure);
	
}


void TIMx_Mode_Config(void)
{
	TIM_ClockConfigTypeDef  sClockSourceConfig;
	TIM_MasterConfigTypeDef  sMasterConfig;
	TIM_OC_InitTypeDef   sConfig;

	
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_AFIO_REMAP_TIM3_PARTIAL();
	COLOR_TIM_CLK_ENABLE();
	
	TIM_Handle.Instance = COLOR_TIMx;
	TIM_Handle.Init.Prescaler = 4000;
	TIM_Handle.Init.Period = 255;
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
	sConfig.Pulse = 0;
	HAL_TIM_PWM_ConfigChannel(&TIM_Handle,&sConfig,TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(&TIM_Handle,&sConfig,TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&TIM_Handle,&sConfig,TIM_CHANNEL_4);
	
	HAL_TIM_MspPostInit(&TIM_Handle);
	
}


/* 设置RGB LED的颜色 rgb:要设置LED显示的颜色格式RGB888 */
void SetRGBColor(uint32_t rgb)
{
	//根据颜色值修改定时器的比较寄存器值
	COLOR_TIMx->COLOR_RED_CCRx = (uint8_t)(rgb>>16);
	COLOR_TIMx->COLOR_GREEN_CCRx = (uint8_t)(rgb>>8);
	COLOR_TIMx->COLOR_BLUE_CCRx = (uint8_t)rgb;
} 

/* 设置RGB LED的颜色 r\g\b:要设置LED显示的颜色值 */
void SetColorValue(uint8_t r,uint8_t g, uint8_t b)
{
	//根据颜色值修改定时器的比较寄存器值
	COLOR_TIMx->COLOR_RED_CCRx = r;
	COLOR_TIMx->COLOR_GREEN_CCRx = g;
	COLOR_TIMx->COLOR_BLUE_CCRx = b;
}

/* 停止PWM输出 */
void COLOR_TIMx_LED_Close(void)
{
	SetColorValue(0,0,0);
	
	__HAL_RCC_TIM3_CLK_DISABLE();
	HAL_TIM_Base_Stop(&TIM_Handle);
	CLOSE_RLED();
	CLOSE_GLED();
	CLOSE_BLED();
	
}



