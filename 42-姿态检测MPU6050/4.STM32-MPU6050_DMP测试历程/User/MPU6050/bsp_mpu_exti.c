#include "bsp_mpu_exti.h"



void EXTI_MPU_Config(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	MPU_INT_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	
	GPIO_InitStructure.Pin = MPU_INT_GPIO_PIN;
	
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(MPU_INT_GPIO_PORT,&GPIO_InitStructure);
	
	HAL_NVIC_SetPriority(MPU_INT_EXTI_IRQ,0,0);
	
	HAL_NVIC_EnableIRQ(MPU_INT_EXTI_IRQ);
	
}

