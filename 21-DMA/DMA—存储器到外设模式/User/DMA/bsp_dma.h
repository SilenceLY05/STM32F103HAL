#ifndef __DMA_H__
#define __DMA_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx.h"
#include <stdio.h>
	


/* DMA memory to memory transfer handles -------------------------------------*/

extern 	DMA_HandleTypeDef DMA_Handle;


/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void USART_DMA_Config(void);

/* USER CODE BEGIN Prototypes */
#define DEBUG_USART_DMA_CLK_ENABLE()      		__HAL_RCC_DMA1_CLK_ENABLE();	
#define DEBUG_USART_DMA_STREAM            		DMA1_Channel4
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __DMA_H__ */

