#ifndef __BSP_IWDG_H__
#define __BSP_IWDG_H__


#include "stm32f1xx.h"


extern IWDG_HandleTypeDef     IWDG_Handle;


void IWDG_Config(uint8_t prv,uint16_t rlv);


#endif /* __BSP_IWDG_H__ */


