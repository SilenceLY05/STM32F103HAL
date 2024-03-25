#include "bsp_key.h"


void KEY_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;



    /*开启按键GPIO口的时钟*/

    KEY1_GPIO_CLK_ENABLE();

    KEY2_GPIO_CLK_ENABLE();

    /*选择按键的引脚*/	

    GPIO_InitStructure.Pin = KEY1_PIN; 



    /*设置引脚为输入模式*/

    GPIO_InitStructure.Mode = GPIO_MODE_INPUT; 


    /*设置引脚不上拉也不下拉*/

    GPIO_InitStructure.Pull = GPIO_NOPULL;

    /*使用上面的结构体初始化按键*/

    HAL_GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);

    /*选择按键的引脚*/

    GPIO_InitStructure.Pin = KEY2_PIN; 


    /*使用上面的结构体初始化按键*/

    HAL_GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);

}


uint8_t KEY_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_PIN)
{
	/* 判断按键是否按下 */
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_PIN) == GPIO_PIN_SET)
	{
		/* 按键按下 */
		while(HAL_GPIO_ReadPin(GPIOx,GPIO_PIN) == GPIO_PIN_SET);
		return KEY_ON;
	}
	else
	{
		/* 按键未被按下 */
		return KEY_OFF;
	}
}

