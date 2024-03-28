#include "bsp_exti.h"


void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef     GPIO_InitStructure;
	
	/* ��������GPIO�ڵ�ʱ�� */
	KEY1_INT_GPIO_CLK_ENABLE();
	KEY2_INT_GPIO_CLK_ENABLE();
	
	
	/* ѡ�񰴼�1������ */
	GPIO_InitStructure.Pin = KEY1_INT_GPIO_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(KEY1_INT_GPIO_PORT,&GPIO_InitStructure);
	/* ����EXTI�ж�Դ��KEY1���š������ж����ȼ� */
	HAL_NVIC_SetPriority(KEY1_INT_EXTI_IRQ,0,0);
	/* ʹ���ж� */
	HAL_NVIC_EnableIRQ(KEY1_INT_EXTI_IRQ);
	
	/* ѡ�񰴼�2������ */
	GPIO_InitStructure.Pin = KEY2_INT_GPIO_PIN;
	HAL_GPIO_Init(KEY2_INT_GPIO_PORT,&GPIO_InitStructure);
	/* ����EXTI�ж�Դ��KEY1���š������ж����ȼ� */
	HAL_NVIC_SetPriority(KEY2_INT_EXTI_IRQ,0,0);
	/* ʹ���ж� */
	HAL_NVIC_EnableIRQ(KEY2_INT_EXTI_IRQ);
	
}

