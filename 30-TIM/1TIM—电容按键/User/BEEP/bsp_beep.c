#include "bsp_beep.h"

void BEEP_GPIO_Config(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	BEEP_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	
	GPIO_InitStructure.Pin = BEEP_GPIO_PIN;
	
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(BEEP_GPIO_PORT,&GPIO_InitStructure);
	
	/* ¹Ø±Õ·äÃùÆ÷ */
	HAL_GPIO_WritePin(BEEP_GPIO_PORT,BEEP_GPIO_PIN,GPIO_PIN_RESET);
}


