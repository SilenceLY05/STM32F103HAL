#ifndef __BSP_BREATHLIGHT_H__
#define __BSP_BREATHLIGHT_H__


#include "stm32f1xx.h"



#define COLOR_TIMx                   TIM3
#define COLOR_TIM_CLK_ENABLE()       __HAL_RCC_TIM3_CLK_ENABLE()
#define COLOR_TIM_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()

#define COLOR_RED_TIM_LED_PORT       GPIOB
#define COLOR_RED_TIM_LED_PIN        GPIO_PIN_5
#define COLOR_RED_CCRx               CCR2

#define COLOR_GREEN_TIM_LED_PORT       GPIOB
#define COLOR_GREEN_TIM_LED_PIN        GPIO_PIN_0
#define COLOR_GREEN_CCRx               CCR2

#define COLOR_BLUE_TIM_LED_PORT       GPIOB
#define COLOR_BLUE_TIM_LED_PIN        GPIO_PIN_1
#define COLOR_BLUE_CCRx               CCR2


#define digitalHi(p,i)                     {p->BSRR=i;}        //设置为高电平
#define digitalLo(p,i)                     {p->BSRR=(uint32_t)i << 16;}    //设置为低电平
#define OPEN_RLED()                 digitalLo(COLOR_RED_TIM_LED_PORT,COLOR_RED_TIM_LED_PIN)
#define CLOSE_RLED()                digitalHi(COLOR_RED_TIM_LED_PORT,COLOR_RED_TIM_LED_PIN)

#define OPEN_GLED()                 digitalLo(COLOR_GREEN_TIM_LED_PORT,COLOR_GREEN_TIM_LED_PIN)
#define CLOSE_GLED()                digitalHi(COLOR_GREEN_TIM_LED_PORT,COLOR_GREEN_TIM_LED_PIN)

#define OPEN_BLED()                 digitalLo(COLOR_BLUE_TIM_LED_PORT,COLOR_BLUE_TIM_LED_PIN)
#define CLOSE_BLED()                digitalHi(COLOR_BLUE_TIM_LED_PORT,COLOR_BLUE_TIM_LED_PIN)



void TIMx_Mode_Config(void);
void SetRGBColor(uint32_t rgb);
void SetColorValue(uint8_t r,uint8_t g, uint8_t b);
void COLOR_TIMx_LED_Close(void);







void TIMx_Mode_Config(void);

#endif /* __BSP_BREATHLIGHT_H__ */


