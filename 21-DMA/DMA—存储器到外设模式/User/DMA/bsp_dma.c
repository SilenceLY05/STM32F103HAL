#include "bsp_dma.h"
#include "bsp_led.h"
#include "bsp_usart.h"

DMA_HandleTypeDef DMA_Handle;




/**
  * Enable DMA controller clock
  * Configure DMA for memory to memory transfers
  *   hdma_memtomem_dma1_channel1
  */
void USART_DMA_Config(void)
{
  DEBUG_USART_DMA_CLK_ENABLE();  
     
  DMA_Handle.Instance=DEBUG_USART_DMA_STREAM;                            //数据流选择
                             
  DMA_Handle.Init.Direction=DMA_MEMORY_TO_PERIPH;             //存储器到外设HAL_DMA_Init(&DMA_Handle);
  DMA_Handle.Init.PeriphInc=DMA_PINC_DISABLE;                 //外设非增量模式/* Associate the DMA handle */
  DMA_Handle.Init.MemInc=DMA_MINC_ENABLE;                     //存储器增量模式__HAL_LINKDMA(&UartHandle, hdmatx, DMA_Handle); 
  DMA_Handle.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //外设数据长度:8位
  DMA_Handle.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //存储器数据长度:8位
  DMA_Handle.Init.Mode=DMA_NORMAL;                            //外设普通模式
  DMA_Handle.Init.Priority=DMA_PRIORITY_MEDIUM;               //中等优先级
  
  HAL_DMA_Init(&DMA_Handle);
  /* Associate the DMA handle */
  __HAL_LINKDMA(&UartHandle, hdmatx, DMA_Handle); 
  
}




