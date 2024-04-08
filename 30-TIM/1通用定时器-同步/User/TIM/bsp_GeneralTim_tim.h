#ifndef __BSP_GENERALTIM_H__
#define __BSP_GENERALTIM_H__

#include "stm32f1xx.h"


/* 定时器 */
#define GENERAL_TIM_MASTER            TIM2
#define GENERAL_TIM_MASTER_CLK()      __HAL_RCC_TIM2_CLK_ENABLE()

#define GENERAL_TIM_SLAVE1            TIM3
#define GENERAL_TIM_SLAVE1_CLK()      __HAL_RCC_TIM3_CLK_ENABLE()

#define GENERAL_TIM_SLAVE2            TIM4
#define GENERAL_TIM_SLAVE2_CLK()      __HAL_RCC_TIM4_CLK_ENABLE()


/* TIM主模式PWM输出引脚 */
#define GENERAL_MASTER_CLK()          __HAL_RCC_GPIOB_CLK_ENABLE()
#define GENERAL_MASTER_PORT           GPIOB
#define GENERAL_MASTER_PIN            GPIO_PIN_10
#define GENERAL_MASTER_AF()           __HAL_AFIO_REMAP_TIM2_ENABLE();

/* TIM从模式PWM输出引脚 */
#define GENERAL_SLAVE1_CLK()          __HAL_RCC_GPIOC_CLK_ENABLE()
#define GENERAL_SLAVE1_PORT           GPIOC
#define GENERAL_SLAVE1_PIN            GPIO_PIN_6
#define GENERAL_SLAVE1_AF()           __HAL_AFIO_REMAP_TIM3_ENABLE();

/* TIM从模式PWM输出引脚 */
#define GENERAL_SLAVE2_CLK()          __HAL_RCC_GPIOB_CLK_ENABLE()
#define GENERAL_SLAVE2_PORT           GPIOB
#define GENERAL_SLAVE2_PIN            GPIO_PIN_6





void TIM_Configuration(void);



#endif   /* __BSP_GENERALTIM_H__ */

