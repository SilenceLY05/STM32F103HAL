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
     
  DMA_Handle.Instance=DEBUG_USART_DMA_STREAM;                            //������ѡ��
                             
  DMA_Handle.Init.Direction=DMA_MEMORY_TO_PERIPH;             //�洢��������HAL_DMA_Init(&DMA_Handle);
  DMA_Handle.Init.PeriphInc=DMA_PINC_DISABLE;                 //���������ģʽ/* Associate the DMA handle */
  DMA_Handle.Init.MemInc=DMA_MINC_ENABLE;                     //�洢������ģʽ__HAL_LINKDMA(&UartHandle, hdmatx, DMA_Handle); 
  DMA_Handle.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;    //�������ݳ���:8λ
  DMA_Handle.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;       //�洢�����ݳ���:8λ
  DMA_Handle.Init.Mode=DMA_NORMAL;                            //������ͨģʽ
  DMA_Handle.Init.Priority=DMA_PRIORITY_MEDIUM;               //�е����ȼ�
  
  HAL_DMA_Init(&DMA_Handle);
  /* Associate the DMA handle */
  __HAL_LINKDMA(&UartHandle, hdmatx, DMA_Handle); 
  
}




