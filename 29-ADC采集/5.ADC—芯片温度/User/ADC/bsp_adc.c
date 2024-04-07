#include "bsp_adc.h"


DMA_HandleTypeDef hdma_adcx;
ADC_HandleTypeDef hadcx;



void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	ADCx_RCC_CLK_ENABLE();
	
	DMAx_RCC_CLK_ENABLE();
	
	hdma_adcx.Instance = ADC_DMAx_CHANNELn;
	
	hdma_adcx.Init.Direction = DMA_PERIPH_TO_MEMORY;      //存储器到外设
	hdma_adcx.Init.PeriphInc = DMA_PINC_DISABLE;          //外设非增量模式
	hdma_adcx.Init.MemInc = DMA_MINC_ENABLE;             //存储器非增量模式
	hdma_adcx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;    //外设数据长度16位
	hdma_adcx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;       //存储器数据长度16位
	hdma_adcx.Init.Mode = DMA_CIRCULAR;                   //外设普通模式
	hdma_adcx.Init.Priority = DMA_PRIORITY_HIGH;        //中等优先级
	
	//初始化DMA流，流相当于一个大的管道，管道里面有很多通道
	HAL_DMA_Init(&hdma_adcx);
	
	__HAL_LINKDMA(hadc,DMA_Handle,hdma_adcx);
	
	
}


void ADCx_Init(void)
{
	ADC_ChannelConfTypeDef sConfig;

	hadcx.Instance = ADCx;
	hadcx.Init.DataAlign = ADC_DATAALIGN_RIGHT;             //右对齐
	hadcx.Init.ScanConvMode = ADC_SCAN_DISABLE;                      //非扫描模式
	hadcx.Init.ContinuousConvMode = ENABLE;                 //连续转换
	hadcx.Init.NbrOfConversion = 1;                         //1个转换在规则序列中，也就是只转换规则序列1
	hadcx.Init.DiscontinuousConvMode = DISABLE;             //禁止不连续采样模式
	hadcx.Init.ExternalTrigConv = ADC_SOFTWARE_START;       //软件触发
	HAL_ADC_Init(&hadcx);
	
	
	
	sConfig.Channel = ADC_CHANNEL;
	sConfig.Rank = 1;
	//采样时间间隔
	sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
	
	//配置ADC通道转换顺序为1，第一个转换，采样时间为3个时钟周期
	HAL_ADC_ConfigChannel(&hadcx,&sConfig);
	
	
}

