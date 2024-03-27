#include "bsp_exti.h"


void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef     GPIO_InitStructure;
	
	/* 开启按键GPIO口的时钟 */
	KEY1_INT_GPIO_CLK_ENABLE();
	KEY2_INT_GPIO_CLK_ENABLE();
	
	
	/* 选择按键1的引脚 */
	GPIO_InitStructure.Pin = KEY1_INT_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(KEY1_INT_GPIO_PORT,&GPIO_InitStructure);
	/* 配置EXTI中断源到KEY1引脚、配置中断优先级 */
	HAL_NVIC_SetPriority(KEY1_INT_EXTI_IRQ,0,0);
	/* 使能中断 */
	HAL_NVIC_EnableIRQ(KEY1_INT_EXTI_IRQ);
	
	/* 选择按键2的引脚 */
	GPIO_InitStructure.Pin = KEY2_INT_GPIO_PIN;
	HAL_GPIO_Init(KEY2_INT_GPIO_PORT,&GPIO_InitStructure);
	/* 配置EXTI中断源到KEY1引脚、配置中断优先级 */
	HAL_NVIC_SetPriority(KEY2_INT_EXTI_IRQ,0,0);
	/* 使能中断 */
	HAL_NVIC_EnableIRQ(KEY2_INT_EXTI_IRQ);
	
}

