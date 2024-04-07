#ifndef __BSP_ADC_H__
#define __BSP_ADC_H__


#include "stm32f1xx.h"


extern DMA_HandleTypeDef hdma_adcx;
extern ADC_HandleTypeDef hadcx;


#define ADCx_RCC_CLK_ENABLE()             __HAL_RCC_ADC1_CLK_ENABLE()
#define ADCx_RCC_CLK_DISABLE()            __HAL_RCC_ADC1_CLK_DISABLE()
#define DMAx_RCC_CLK_ENABLE()             __HAL_RCC_DMA1_CLK_ENABLE()


#define ADCx                              ADC1
#define ADC_DMAx_CHANNELn                 DMA1_Channel1
#define ADC_CHANNEL                       ADC_CHANNEL_TEMPSENSOR







void ADCx_Init(void);

#endif /* __BSP_ADC_H__ */

