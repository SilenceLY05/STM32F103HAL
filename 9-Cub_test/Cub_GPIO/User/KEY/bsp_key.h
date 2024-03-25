#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__


#include "stm32f1xx.h"
#include "main.h"



#define KEY_ON        1
#define KEY_OFF       0
//Òý½Å¶¨Òå
/*******************************************************/
#define KEY1_PIN                  GPIO_PIN_0                 
#define KEY1_GPIO_PORT            GPIOA                      
#define KEY1_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()

#define KEY2_PIN                  GPIO_PIN_13                 
#define KEY2_GPIO_PORT            GPIOC                      
#define KEY2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOC_CLK_ENABLE()
/*******************************************************/



void KEY_GPIO_Init(void);
uint8_t KEY_Scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_PIN);


#endif  /* __BSP_KEY_H__ */


