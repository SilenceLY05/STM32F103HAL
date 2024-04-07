#include "bsp_adc.h"


DMA_HandleTypeDef hdma_adcx;
ADC_HandleTypeDef hadcx1;
ADC_HandleTypeDef hadcx2;


void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if(hadc->Instance == ADCx1)
	{
		/* ����ʱ��ʹ�� */
		ADCx1_RCC_CLK_ENABLE();
		
		/* ADת��ͨ������ʱ��ʹ�� */
		ADC_GPIO_CLK_ENABLE();
		
		/* DMAʱ��ʹ�� */
		DMAx1_RCC_CLK_ENABLE();
		
		/* ADת��ͨ�����ų�ʼ�� */
		GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStructure.Pin = ADC_GPIO_PIN;
		HAL_GPIO_Init(ADC_GPIO_PORT,&GPIO_InitStructure);
		
		/* DMA�����ʼ������ */
		hdma_adcx.Instance = ADC_DMAx_CHANNELn;
		hdma_adcx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adcx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
		hdma_adcx.Init.MemInc = DMA_MINC_DISABLE;
		hdma_adcx.Init.Mode = DMA_CIRCULAR;
		hdma_adcx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_adcx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adcx.Init.Priority = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_adcx);
		
		/* ����DMA */
		__HAL_LINKDMA(hadc,DMA_Handle,hdma_adcx);
		
		/* �����ж�ʹ�����ȼ� */
		HAL_NVIC_SetPriority(ADC_DMAx_CHANNELn_IRQn,0,0);
		HAL_NVIC_EnableIRQ(ADC_DMAx_CHANNELn_IRQn);
	}
	else if(hadc->Instance == ADCx2)
	{
		/* ����ʱ��ʹ�� */
		ADCx2_RCC_CLK_ENABLE();
		
		/* ADת��ͨ������ʱ��ʹ�� */
		ADC_GPIO_CLK_ENABLE();
		
		/* DMAʱ��ʹ�� */
		DMAx1_RCC_CLK_ENABLE();
		
		/* ADת��ͨ�����ų�ʼ�� */
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
	
	//���ò���ͨ��
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
	
	//���ò���ͨ��
	sConfig.Channel = ADC_CHANNEL;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
	HAL_ADC_ConfigChannel(&hadcx2,&sConfig);
}


