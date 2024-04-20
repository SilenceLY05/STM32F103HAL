#ifndef __BSP_SCCB_H__
#define __BSP_SCCB_H__

#include "stm32f1xx.h"
#include <inttypes.h>





/* 直接操作寄存器控制IO */
#define digitalH(p,i)                      {p->BSRR=i;}        //设置为高电平
#define digitalL(p,i)                      {p->BSRR = (uint32_t)i<<16;}        //设置为低电平


#define SCL_H                               digitalH(GPIOC,GPIO_PIN_6)
#define SCL_L                               digitalL(GPIOC,GPIO_PIN_6)


#define SDA_H                               digitalH(GPIOC,GPIO_PIN_7)
#define SDA_L                               digitalL(GPIOC,GPIO_PIN_7)

#define SCL_read                            HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)
#define SDA_read                            HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)


#define ADDR_OV7725                         0x42


void SCCB_GPIO_Config(void);
int SCCB_WriteByte(uint16_t WriteAddress,uint8_t SendByte);
int SCCB_ReadByte(uint8_t *pBuffer,uint16_t length,uint8_t ReadAddress);

#endif /* __BSP_SCCB_H__ */


