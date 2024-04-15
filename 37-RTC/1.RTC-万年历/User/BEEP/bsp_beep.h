#ifndef __BSP_BEEP_H__
#define __BSP_BEEP_H__

#include "stm32f1xx.h"

/* ������������ӵ�GPIO�˿� */
#define BEEP_GPIO_PORT           GPIOA
#define BEEP_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define BEEP_GPIO_PIN            GPIO_PIN_8



/* �ߵ�ƽʱ���������� */
#define ON  1
#define OFF 0


#define BEEP(a)   HAL_GPIO_WritePin(BEEP_GPIO_PORT,BEEP_GPIO_PIN,a)


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define digitalHi(p,i)               {p->BSRR = i;}
#define digitalLo(p,i)               {p->BSRR = (uint32_t)i <<16;}
#define digitalToggle(p,i)           {p->ODR ^=i; }


/* �������IO�ĺ� */
#define BEEP_TOGGLE                  digitalToggle(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_ON                      digitalHi(BEEP_GPIO_PORT,BEEP_GPIO_PIN)
#define BEEP_OFF                     digitalLo(BEEP_GPIO_PORT,BEEP_GPIO_PIN)


void BEEP_GPIO_Config(void);


#endif /* __BSP_BEEP_H__ */


