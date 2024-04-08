#ifndef __BSP_BREATHLIGHT_H__
#define __BSP_BREATHLIGHT_H__


#include "stm32f1xx.h"

extern TIM_HandleTypeDef  TIM_Handle;
/* PWM表中的点数 */
extern uint16_t POINT_NUM;

/* 控制输出波形的频率 */
extern uint16_t indexWave[];


// 定时器预分频
#define BRE_TIM_PRESCALER               9

// 定时器周期
#define BRE_TIM_PERIOD                  511


/* 电压幅值等级 */
#define AMPLITUDE_CLASS                 256



///* 定义使用什么颜色的灯 */
//#define LIGHT_COLOR           RED_LIGHT


///* 定时器通道 */
//#if    LIGHT_COLOR == RED_LIGHT

#define BRE_TIMx                   TIM3
#define BRE_TIM_CLK_ENABLE()       __HAL_RCC_TIM3_CLK_ENABLE()
#define BRE_TIM_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()

#define BRE_RED_TIM_LED_PORT       GPIOB
#define BRE_RED_TIM_LED_PIN        GPIO_PIN_0

#define BRE_TIM_IRQn               TIM3_IRQn
#define BRE_TIM_IRQHandler         TIM3_IRQHandler


#define BRE_GREEN_TIM_CHANNEL        TIM_CHANNEL_3
#define BRE_GREEN_TIM_LED_PORT       GPIOB
#define BRE_GREEN_TIM_LED_PIN        GPIO_PIN_0


#define BRE_BLUE_TIM_CHANNEL        TIM_CHANNEL_4
#define BRE_BLUE_TIM_LED_PORT       GPIOB
#define BRE_BLUE_TIM_LED_PIN        GPIO_PIN_1






void TIMx_Mode_Config(void);

#endif /* __BSP_BREATHLIGHT_H__ */


