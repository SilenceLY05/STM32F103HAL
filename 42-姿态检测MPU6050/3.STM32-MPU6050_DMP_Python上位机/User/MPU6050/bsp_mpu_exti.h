#ifndef __BSP_MPU_EXTI_H__
#define __BSP_MPU_EXTI_H__


#include "stm32f1xx.h"


#define MPU_INT_GPIO_PORT                GPIOA
#define MPU_INT_GPIO_PIN                 GPIO_PIN_11
#define MPU_INT_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define MPU_INT_EXTI_IRQ                 EXTI15_10_IRQn
#define MPU_IRQHandler                   EXTI15_10_IRQHandler



void EXTI_MPU_Config(void);

#endif /* __BSP_MPU_EXTI_H__ */

