#ifndef __BSP_WWDG_H__
#define __BSP_WWDG_H__


#include "stm32f1xx.h"


extern WWDG_HandleTypeDef     WWDG_Handle;


void WWDG_Config(uint8_t tr,uint8_t wr,uint32_t prv);
void WWDG_Feed(void);


#endif /* __BSP_WWDG_H__ */


