#include "bsp_dma.h"
#include "bsp_led.h"

DMA_HandleTypeDef DMA_Handle;


/* 定义aSRC_Const_Buffer数组作为DMA传输数据源
  const关键字将aSRC_Const_Buffer数组变量定义为常量类型 */
uint32_t aSRC_Const_Buffer[BUFFER_SIZE]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};
/* 定义DMA传输目标存储器 */
uint32_t aDST_Buffer[BUFFER_SIZE];

static void Delay(__IO uint32_t nTime);

/**
  * Enable DMA controller clock
  * Configure DMA for memory to memory transfers
  *   hdma_memtomem_dma1_channel1
  */
void DMA_Config(void)
{
	HAL_StatusTypeDef DMA_Status = HAL_ERROR;
	


  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Configure DMA request hdma_memtomem_dma1_channel1 on DMA1_Channel1 */
  DMA_Handle.Instance = DMA1_Channel6;
  DMA_Handle.Init.Direction = DMA_MEMORY_TO_MEMORY;
  DMA_Handle.Init.PeriphInc = DMA_PINC_ENABLE;
  DMA_Handle.Init.MemInc = DMA_MINC_ENABLE;
  DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  DMA_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  DMA_Handle.Init.Mode = DMA_NORMAL;
  DMA_Handle.Init.Priority = DMA_PRIORITY_MEDIUM;
  
	/* 完成DMA数据流参数配置 */
	HAL_DMA_Init(&DMA_Handle);

  DMA_Status = HAL_DMA_Start(&DMA_Handle,(uint32_t )aSRC_Const_Buffer,(uint32_t)aDST_Buffer,BUFFER_SIZE);
	
	
	/* 判断状态 */
	if(DMA_Status != HAL_OK)
	{
		while(1)
		{
			LED_RED;
			Delay(0xFFFFFF);
			LED_RGBOFF;
			Delay(0xFFFFFF);
		}
	}

}


static void Delay(__IO uint32_t nTime)
{
	for(;nTime != 0;nTime--);
}

