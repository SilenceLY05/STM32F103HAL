#include "bsp_adc.h"


DMA_HandleTypeDef hdma_adcx;
ADC_HandleTypeDef hadcx1;
ADC_HandleTypeDef hadcx2;


void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(hadc->Instance == ADCx1)
	{
		/* 外设时钟使能 */
		ADCx1_RCC_CLK_ENABLE();
		
		/* AD转换通道引脚时钟使能 */
		ADC_GPIO_CLK_ENABLE();
		
		/* DMA时钟使能 */
		DMAx1_RCC_CLK_ENABLE();
		
		/* AD转换通道引脚初始化 */
		GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStructure.Pin = ADC_GPIO_PIN;
		HAL_GPIO_Init(ADC_GPIO_PORT,&GPIO_InitStructure);
		
		/* DMA外设初始化配置 */
		hdma_adcx.Instance = ADC_DMAx_CHANNELn;
		hdma_adcx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adcx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
		hdma_adcx.Init.MemInc = DMA_MINC_DISABLE;
		hdma_adcx.Init.Mode = DMA_CIRCULAR;
		hdma_adcx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_adcx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adcx.Init.Priority = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_adcx);
		
		/* 连接DMA */
		__HAL_LINKDMA(hadc,DMA_Handle,hdma_adcx);
		
		/* 外设中断使能优先级 */
		HAL_NVIC_SetPriority(ADC_DMAx_CHANNELn_IRQn,0,0);
		HAL_NVIC_EnableIRQ(ADC_DMAx_CHANNELn_IRQn);
	}
	else if(hadc->Instance == ADCx2)
	{
		/* 外设时钟使能 */
		ADCx2_RCC_CLK_ENABLE();
		
		/* AD转换通道引脚时钟使能 */
		ADC_GPIO_CLK_ENABLE();
		
		/* DMA时钟使能 */
		DMAx1_RCC_CLK_ENABLE();
		
		/* AD转换通道引脚初始化 */
		GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStructure.Pin = ADC_GPIO_PIN;
		HAL_GPIO_Init(ADC_GPIO_PORT,&GPIO_InitStructure);
	}
}


void ADCx_Init(void)
{
	ADC_MultiModeTypeDef multimode;
	ADC_ChannelConfTypeDef sConfig;
	
	hadcx1.Instance = ADCx1;
	hadcx1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadcx1.Init.ContinuousConvMode = ENABLE;
	hadcx1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadcx1.Init.DiscontinuousConvMode = DISABLE;
	hadcx1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadcx1.Init.NbrOfConversion = 1;
	HAL_ADC_Init(&hadcx1);
	
	multimode.Mode = ADC_DUALMODE_INTERLSLOW;
	HAL_ADCEx_MultiModeConfigChannel(&hadcx1,&multimode);
	
	//配置采样通道
	sConfig.Channel = ADC_CHANNEL;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
	HAL_ADC_ConfigChannel(&hadcx1,&sConfig);
	
	
	hadcx1.Instance = ADCx2;
	hadcx1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadcx1.Init.ContinuousConvMode = ENABLE;
	hadcx1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadcx1.Init.DiscontinuousConvMode = DISABLE;
	hadcx1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadcx1.Init.NbrOfConversion = 1;
	HAL_ADC_Init(&hadcx2);
	
	multimode.Mode = ADC_DUALMODE_INTERLSLOW;
	HAL_ADCEx_MultiModeConfigChannel(&hadcx2,&multimode);
	
	//配置采样通道
	sConfig.Channel = ADC_CHANNEL;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
	HAL_ADC_ConfigChannel(&hadcx2,&sConfig);
}


