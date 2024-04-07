#include "bsp_adc.h"


DMA_HandleTypeDef hdma_adcx;
ADC_HandleTypeDef hadcx;



void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	ADCx_RCC_CLK_ENABLE();
	
	DMAx_RCC_CLK_ENABLE();
	
	hdma_adcx.Instance = ADC_DMAx_CHANNELn;
	
	hdma_adcx.Init.Direction = DMA_PERIPH_TO_MEMORY;      //�洢��������
	hdma_adcx.Init.PeriphInc = DMA_PINC_DISABLE;          //���������ģʽ
	hdma_adcx.Init.MemInc = DMA_MINC_ENABLE;             //�洢��������ģʽ
	hdma_adcx.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;    //�������ݳ���16λ
	hdma_adcx.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;       //�洢�����ݳ���16λ
	hdma_adcx.Init.Mode = DMA_CIRCULAR;                   //������ͨģʽ
	hdma_adcx.Init.Priority = DMA_PRIORITY_HIGH;        //�е����ȼ�
	
	//��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
	HAL_DMA_Init(&hdma_adcx);
	
	__HAL_LINKDMA(hadc,DMA_Handle,hdma_adcx);
	
	
}


void ADCx_Init(void)
{
	ADC_ChannelConfTypeDef sConfig;

	hadcx.Instance = ADCx;
	hadcx.Init.DataAlign = ADC_DATAALIGN_RIGHT;             //�Ҷ���
	hadcx.Init.ScanConvMode = ADC_SCAN_DISABLE;                      //��ɨ��ģʽ
	hadcx.Init.ContinuousConvMode = ENABLE;                 //����ת��
	hadcx.Init.NbrOfConversion = 1;                         //1��ת���ڹ��������У�Ҳ����ֻת����������1
	hadcx.Init.DiscontinuousConvMode = DISABLE;             //��ֹ����������ģʽ
	hadcx.Init.ExternalTrigConv = ADC_SOFTWARE_START;       //�������
	HAL_ADC_Init(&hadcx);
	
	
	
	sConfig.Channel = ADC_CHANNEL;
	sConfig.Rank = 1;
	//����ʱ����
	sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
	
	//����ADCͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ3��ʱ������
	HAL_ADC_ConfigChannel(&hadcx,&sConfig);
	
	
}

