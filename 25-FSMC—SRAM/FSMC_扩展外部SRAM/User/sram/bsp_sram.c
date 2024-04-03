#include "bsp_sram.h"



static SRAM_HandleTypeDef SRAM_Handler;
static FSMC_NORSRAM_TimingTypeDef Timing;



void FSMC_SRAM_MspInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	__HAL_RCC_FSMC_CLK_ENABLE();
	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	
	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 
												 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 
												 | GPIO_PIN_14 | GPIO_PIN_15;
	
	HAL_GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 
												 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 
												 | GPIO_PIN_14 | GPIO_PIN_15;
	
	HAL_GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 
												 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 
												 | GPIO_PIN_14 | GPIO_PIN_15;
	
	HAL_GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 
												 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 
												 | GPIO_PIN_14 | GPIO_PIN_15;
	
	HAL_GPIO_Init(GPIOG,&GPIO_InitStructure);
	

}



void FSMC_SRAM_Init(void)
{
	SRAM_Handler.Instance = FSMC_NORSRAM_DEVICE;
	SRAM_Handler.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	
	Timing.AddressSetupTime = 0x00;
	Timing.AddressHoldTime = 0x00;
	Timing.DataSetupTime = 0x08;
	Timing.BusTurnAroundDuration = 0x00;
	Timing.CLKDivision = 0x00;
	Timing.DataLatency = 0x00;
	Timing.AccessMode = FSMC_ACCESS_MODE_A;
	
	SRAM_Handler.Init.NSBank = FSMC_NORSRAM_BANK3;
	SRAM_Handler.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	SRAM_Handler.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
	SRAM_Handler.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
	SRAM_Handler.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	SRAM_Handler.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	SRAM_Handler.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	SRAM_Handler.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	SRAM_Handler.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	SRAM_Handler.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	SRAM_Handler.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
	
	FSMC_SRAM_MspInit();
	HAL_SRAM_Init(&SRAM_Handler,&Timing,&Timing);
}



/* 以字为单位向sdram写入数据 pBuffer:指向数据的指针 uwWriteAddress:要写入的SRAM内部地址 uwBufferSize:要写入数据大小 */
void SRAM_WriteBuffer(uint32_t *pBuffer,uint32_t uwWriteAddress,uint32_t uwBufferSize)
{
	__IO uint32_t write_pointer = (uint32_t)uwWriteAddress;
	
	/* 循环写入数据 */
	for(;uwBufferSize != 0;uwBufferSize--)
	{
		*(uint32_t *)(Bank1_SRAM3_ADDR + write_pointer) = *pBuffer++;
		
		/* 地址自增 */
		write_pointer += 4;
	}
}




/* 从sdram读出数据 pBuffer:指向数据的指针 uwReadAddress:要读取数据的地址 uwBufferSize:要读取数据大小 */
void SRAM_ReadBuffer(uint32_t *pBuffer,uint32_t uwReadAddress,uint32_t uwBufferSize)
{
	__IO uint32_t write_pointer = (uint32_t)uwReadAddress;
	
	/* 循环写入数据 */
	for(;uwBufferSize != 0x00;uwBufferSize--)
	{
	  *pBuffer++ = *(uint32_t *)(Bank1_SRAM3_ADDR + write_pointer) ;
		
		/* 地址自增 */
		write_pointer += 4;
	}
}


/* 测试SRAM是否正常 */
uint8_t SRAM_Test(void)
{
	/* 写入数据计数器 */
	uint32_t counter = 0;
	
	/* 8位的数据 */
	uint8_t ubWritedata_8b = 0,ubReaddata_8b = 0;
	
	/* 16位数据 */
	uint16_t ubWritedata_16b = 0,ubReaddata_16b = 0;
	
	SRAM_INFO("正在检测SRAM，以8位，16位的方式读写sram....");
	
	
	/* 按8位格式读写数据，并校验 */
	
	/* 把SRAM数据全部置为0，IS62WV51216_SIZE是以8位为单位的 */
	for(counter = 0x00;counter < IS62WV51216_SIZE; counter++)
	{
		*(__IO uint8_t*)(Bank1_SRAM3_ADDR +counter) = (uint8_t)0x0;
	}
	
	/* 向整个SRAM写入数据 8位 */
	for(counter = 0;counter < IS62WV51216_SIZE; counter++)
	{
		*(__IO uint8_t*)(Bank1_SRAM3_ADDR +counter) = (uint8_t)(ubWritedata_8b +counter);
	}
	
	/* 读取SRAM数据并检测 */
	for(counter = 0;counter < IS62WV51216_SIZE ;counter++)
	{
		ubReaddata_8b = *(__IO uint8_t*)(Bank1_SRAM3_ADDR+counter);   //从改地址读出数据
		
		if(ubReaddata_8b != (uint8_t)(ubWritedata_8b +counter))
		{
			SRAM_ERROR("8位数据读写错误！");
			return 0;
		}
	}
	
	
	
	/* 按16位格式读写数据，并校验 */
	
	/* 把SRAM数据全部置为0，IS62WV51216_SIZE是以16位为单位的 */
	for(counter = 0x00;counter < IS62WV51216_SIZE/2; counter++)
	{
		*(__IO uint16_t*)(Bank1_SRAM3_ADDR +2*counter) = (uint16_t)0x00;
	}
	
	/* 向整个SRAM写入数据 8位 */
	for(counter = 0;counter < IS62WV51216_SIZE/2; counter++)
	{
		*(__IO uint16_t*)(Bank1_SRAM3_ADDR +2*counter) = (uint16_t)(ubWritedata_16b +counter);
	}
	
	/* 读取SRAM数据并检测 */
	for(counter = 0;counter < IS62WV51216_SIZE/2 ;counter++)
	{
		ubReaddata_16b = *(__IO uint16_t*)(Bank1_SRAM3_ADDR+2*counter);   //从改地址读出数据
		
		if(ubReaddata_16b != (uint16_t)(ubWritedata_16b +counter))
		{
			SRAM_ERROR("16位数据读写错误！");
			return 0;
		}
	}
	 SRAM_INFO("SRAM读写测试正常！"); 
  /*检测正常，return 1 */
  return 1;
}




