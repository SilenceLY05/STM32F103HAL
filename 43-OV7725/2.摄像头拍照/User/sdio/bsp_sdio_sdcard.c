#include "bsp_sdio_sdcard.h"
#include "bsp_usart.h"



SD_HandleTypeDef uSdHandle;




__weak void BSP_SD_MspInit(void *Params)
{
	GPIO_InitTypeDef gpioinitstruct = {0};
	
	__HAL_RCC_SDIO_CLK_ENABLE();
	
	__DMAx_TxRx_CLK_ENABLE();
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__SD_DETECT_GPIO_CLK_ENABLE();
	
	
	gpioinitstruct.Mode = GPIO_MODE_AF_PP;
	gpioinitstruct.Pull = GPIO_PULLUP;
	gpioinitstruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	gpioinitstruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	
	HAL_GPIO_Init(GPIOC,&gpioinitstruct);
	
	
	gpioinitstruct.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOD,&gpioinitstruct);
}



uint8_t SD_DMAConfigRx(SD_HandleTypeDef* hsd)
{
	static DMA_HandleTypeDef hdma_rx;
	HAL_StatusTypeDef status = HAL_ERROR;
	
	
	if(hsd->hdmarx == NULL)
	{
		hdma_rx.Instance = SD_DMAx_Rx_INSTANCE;
		
		hdma_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_rx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
		hdma_rx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_rx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_rx.Init.Priority = DMA_PRIORITY_HIGH;
		
		
		__HAL_LINKDMA(hsd,hdmarx,hdma_rx);
		
		HAL_DMA_Abort(&hdma_rx);
		HAL_DMA_DeInit(&hdma_rx);
		
		status = HAL_DMA_Init(&hdma_rx);
		
		HAL_NVIC_SetPriority(SD_DMAx_Rx_IRQn,0xD,0);
		HAL_NVIC_EnableIRQ(SD_DMAx_Rx_IRQn);
	}
	else
	{
	status = HAL_OK;
	}
	
	return (status != HAL_OK ? MSD_ERRPR : MSD_OK);
}


uint8_t SD_DMAConfigTx(SD_HandleTypeDef* hsd)
{
	static DMA_HandleTypeDef hdma_tx;
	HAL_StatusTypeDef status;
	
	
	if(hsd->hdmarx == NULL)
	{
		hdma_tx.Instance = SD_DMAx_Tx_INSTANCE;
		
		hdma_tx.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_tx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
		hdma_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_tx.Init.Priority = DMA_PRIORITY_HIGH;
		
		
		__HAL_LINKDMA(hsd,hdmarx,hdma_tx);
		
		HAL_DMA_Abort(&hdma_tx);
		HAL_DMA_DeInit(&hdma_tx);
		
		status = HAL_DMA_Init(&hdma_tx);
		
		HAL_NVIC_SetPriority(SD_DMAx_Tx_IRQn,0xD,0);
		HAL_NVIC_EnableIRQ(SD_DMAx_Tx_IRQn);
	}
	else
	{
	status = HAL_OK;
	}
	
	return (status != HAL_OK ? MSD_ERRPR : MSD_OK);
}



uint8_t BSP_SD_Init(void)
{
	uint8_t state = MSD_OK;
	
	uSdHandle.Instance = SDIO;
	uSdHandle.Init.BusWide = SDIO_BUS_WIDE_1B;
	uSdHandle.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
	uSdHandle.Init.ClockDiv = SDIO_TRANSFER_CLK_DIV;
	uSdHandle.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
	uSdHandle.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
	uSdHandle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
	
	BSP_SD_MspInit(NULL);
	
	if(HAL_SD_Init(&uSdHandle)!= HAL_OK)
	{
		state = MSD_ERRPR;
	}
	
	if(state == MSD_OK)
	{
		if(HAL_SD_ConfigWideBusOperation(&uSdHandle,SDIO_BUS_WIDE_4B)!=HAL_OK)
		{
			state = MSD_ERRPR;
		}
		else
		{
			state = MSD_OK;
		}
	}
	return state;
}


/* ΪSD��������������ж�ģʽ */
uint8_t BSP_SD_ITConfig(void)
{
	return 0;
}


/* ���SD���Ƿ���ȷ�����ڴ��� */
uint8_t BSP_SD_IsDetected(void)
{
	__IO uint8_t status = SD_PRESENT;
	
	if(HAL_GPIO_ReadPin(SD_DETECT_GPIO_PORT,SD_DETECT_PIN) != GPIO_PIN_RESET)
	{
		status = SD_NOT_PRESENT;
	}
	return status;
}


__weak void BSP_SD_DetectCallback(void)
{
	
}



/* SD���IT���� */
void BSP_SD_DetectIT(void)
{
	BSP_SD_DetectCallback();
}



/* ����ѯģʽ��SD����ָ����ַ��ȡ��  pData:ָ�򻺳�����ָ�� ReadAddr:��ȡ���ݵĵ�ַ NumOfBlocks:Ҫ��ȡ��SD������� Timeout:��ȡ�ݲ����ĳ�ʱʱ�� */
uint8_t BSP_SD_ReadBlocks(uint32_t *pData,uint32_t ReadAddr,uint32_t NumOfBlocks,uint32_t Timeout)
{
	if(HAL_SD_ReadBlocks(&uSdHandle,(uint8_t*)pData,ReadAddr,NumOfBlocks,Timeout) != HAL_OK)
	{
		return MSD_ERRPR;
	}
	else
	{
		return MSD_OK;
	}
}



/* ����ѯģʽ��SD����ָ����ַд��  pData:ָ�򻺳�����ָ�� WriterAddr:д�����ݵĵ�ַ NumOfBlocks:Ҫд���SD������� Timeout:��ȡ�ݲ����ĳ�ʱʱ�� */
uint8_t BSP_SD_WriteBlocks(uint32_t *pData,uint32_t WriteAddr,uint32_t NumOfBlocks,uint32_t Timeout)
{
	if(HAL_SD_WriteBlocks(&uSdHandle,(uint8_t*)pData,WriteAddr,NumOfBlocks,Timeout) != HAL_OK)
	{
		return MSD_ERRPR;
	}
	else
	{
		return MSD_OK;
	}
}



/* ��DMAģʽ�£���SD���е�ָ����ַ��ȡ�� pData:ָ�򻺳�����ָ�� ReadAddr:��ȡ���ݵĵ�ַ NumOfBlocks:Ҫ��ȡ��SD������� */
uint8_t BSP_SD_ReadBlocks_DMA(uint32_t *pData,uint32_t ReadAddr,uint32_t NumOfBlocks)
{
	uint8_t state = MSD_OK;
	
	uSdHandle.hdmatx = NULL;
	
	state = ((SD_DMAConfigRx(&uSdHandle) == MSD_OK) ?MSD_OK : MSD_ERRPR);
	
	
	if(state == MSD_OK)
	{
		state = (HAL_SD_ReadBlocks_DMA(&uSdHandle,(uint8_t*)pData,ReadAddr,NumOfBlocks) == HAL_OK ) ? MSD_OK : MSD_ERRPR;
	}
	
	return state;
}



/* ��DMAģʽ�£���SD���е�ָ����ַд��� pData:ָ�򻺳�����ָ�� WriteAddr:д�����ݵĵ�ַ NumOfBlocks:Ҫд���SD������� */
uint8_t BSP_SD_WriteBlocks_DMA(uint32_t *pData,uint32_t WriteAddr,uint32_t NumOfBlocks)
{
	uint8_t state = MSD_OK;
	
	uSdHandle.hdmarx = NULL;
	
	state = ((SD_DMAConfigTx(&uSdHandle) == MSD_OK) ?MSD_OK : MSD_ERRPR);
	
	
	if(state == MSD_OK)
	{
		state = (HAL_SD_WriteBlocks_DMA(&uSdHandle,(uint8_t*)pData,WriteAddr,NumOfBlocks) == HAL_OK ) ? MSD_OK : MSD_ERRPR;
	}
	
	return state;
}




/* ����ָ��SD�����ڴ����� StartAddr:��ʼ�ֽڵ�ַ EndAddr:�����ֽڵ�ַ */
uint8_t BSP_SD_Erase(uint32_t StartAddr,uint32_t EndAddr)
{
	if(HAL_SD_Erase(&uSdHandle,StartAddr,EndAddr) != HAL_OK)
	{
		return MSD_ERRPR;
	}
	else
	{
		return MSD_OK;
	}
}



/* ��ȡ��ǰSD������״̬ SD_TRANSFER_OK:û�����ݴ������ڽ��� SD_TRANSFER_BUSY:���ݴ������ڽ����� */
uint8_t BSP_SD_GetCardState(void)
{
	return ((HAL_SD_GetCardState(&uSdHandle) == HAL_SD_CARD_TRANSFER) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}



/* ��ȡ�ض�SD����SD��Ϣ CardInfo:ָ��HAL_SD_CardInfo TypeDef�ṹ��ָ�� */
void BSP_SD_GetCardInfo(HAL_SD_CardInfoTypeDef *CardInfo)
{
	HAL_SD_GetCardInfo(&uSdHandle,CardInfo);
}


__weak void BSP_SD_AbortCallback(void)
{
	
}


__weak void BSP_SD_WriteCpltCallback(void)
{
	
}



__weak void BSP_SD_ReadCpltCallback(void)
{
	
}


void HAL_SD_AbortCallback(SD_HandleTypeDef *hsd)
{
	BSP_SD_AbortCallback();
}



void HAL_SD_TxCpltCallback(SD_HandleTypeDef *hsd)
{
	BSP_SD_WriteCpltCallback();
}


void HAL_SD_RxCpltCallback(SD_HandleTypeDef *hsd)
{
	BSP_SD_ReadCpltCallback();
}








