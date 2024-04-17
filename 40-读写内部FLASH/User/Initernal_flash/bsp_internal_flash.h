#ifndef __BSP_INTERNAL_FLASH_H__
#define __BSP_INTERNAL_FLASH_H__


#include "stm32f1xx.h"



#define WRITE_START_ADDR            ((uint32_t)0x08008000)
#define WRITE_END_ADDR              ((uint32_t)0x0800C000)


typedef enum
{
	FAILED = 0,
	PASSED = !FAILED
}TestStatus;


int InternalFLASH_Test(void);



#endif /* __BSP_INTERNAL_FLASH_H__ */

