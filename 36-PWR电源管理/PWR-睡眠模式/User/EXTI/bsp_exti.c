#include "bsp_exti.h"


void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	KEY1_INT_GPIO_CLK_ENABLE();
	KEY2_INT_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pin = KEY1_INT_GPIO_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(KEY1_INT_GPIO_PORT,&GPIO_InitStructure);
	
	HAL_NVIC_SetPriority(KEY1_INT_EXTI_IRQ,0,0);
	HAL_NVIC_EnableIRQ(KEY1_INT_EXTI_IRQ);
	
	
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pin = KEY2_INT_GPIO_PIN;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(KEY2_INT_GPIO_PORT,&GPIO_InitStructure);
	
	HAL_NVIC_SetPriority(KEY2_INT_EXTI_IRQ,0,0);
	HAL_NVIC_EnableIRQ(KEY2_INT_EXTI_IRQ);
}


