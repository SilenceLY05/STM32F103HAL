#include "bsp_led.h"
#include "bsp_sdio_sdcard.h"
#include "bsp_usart.h"
#include "sdio_test.h"

#define BLOCK_START_ADDR   0
#define NUM_OF_BLOCKS      5
#define BUFFER_WORDS_SIZE  ((BLOCKSIZE * NUM_OF_BLOCKS) >> 2)



uint32_t aTxBuffer[BUFFER_WORDS_SIZE];
uint32_t aRxBuffer[BUFFER_WORDS_SIZE];



static void Fill_Buffer(uint32_t*pBuffer,uint32_t uwbufferlength,uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t *pBuffer1,uint32_t *pBuffer2,uint16_t BufferLength);



/* 用用户预定义的数据填充缓冲区 pBuffer:指向要天填充的缓冲区指针 uwbufferlength:要填充的缓冲区大小 uwOffset:第一个填充缓冲区的值 */
static void Fill_Buffer(uint32_t*pBuffer,uint32_t uwbufferlength,uint32_t uwOffset)
{
	uint32_t tmpIndex = 0;
	
	for(tmpIndex = 0; tmpIndex < uwbufferlength;tmpIndex++)
	{
		pBuffer[tmpIndex ]= tmpIndex + uwOffset;
	}
}



/* 比较两个缓冲区 pBuffer1 pBuffer2 BufferLength缓冲区的长度 */
static uint8_t Buffercmp(uint32_t *pBuffer1,uint32_t *pBuffer2,uint16_t BufferLength)
{
	while(BufferLength --)
	{
		if(*pBuffer1 != *pBuffer2)
		{
			return 1;
		}
		pBuffer1++;
		pBuffer2++;
	}
	return 0;
}


void SD_Test(void)
{
	uint8_t SD_state = MSD_OK;
	
	SD_state = BSP_SD_Init();
	
	if(SD_state != MSD_OK)
	{
		printf("\r\n SD Initialization:FAIL \r\n");
	}
	else
	{
		printf("\r\n SD Initialization:OK \r\n");
		
		SD_state = BSP_SD_Erase(BLOCK_START_ADDR,NUM_OF_BLOCKS);
		
		while((BSP_SD_GetCardState()!= SD_TRANSFER_OK));
		
		if(SD_state != MSD_OK)
		{
			printf("\r\n SD ERASE: FAIL \r\n");
		}
		else
		{
			printf("\r\n SD ERASE: OK \r\n");
			
			Fill_Buffer(aTxBuffer,BUFFER_WORDS_SIZE,0x22FF);
			
			SD_state = BSP_SD_WriteBlocks(aTxBuffer,BLOCK_START_ADDR,NUM_OF_BLOCKS,10);
			
			while((BSP_SD_GetCardState() != SD_TRANSFER_OK));
			
			if(SD_state != MSD_OK)
			{
				printf("\r\n SD WRITE :FAILED \r\n");
			}
			else
			{
				printf("\r\n SD WRITE :OK \r\n");
				
				SD_state = BSP_SD_ReadBlocks(aRxBuffer,BLOCK_START_ADDR,NUM_OF_BLOCKS,10);
				
				while((BSP_SD_GetCardState()!= SD_TRANSFER_OK));
				
				if(SD_state != MSD_OK)
				{
					printf("\r\n SD READ :FAILED \r\n");
				}
				else
				{
					printf("\r\n SD READ :OK \r\n");
					
					if(Buffercmp(aTxBuffer,aRxBuffer,BUFFER_WORDS_SIZE)>0)
					{
						printf("\r\n SD COMPARE : FAILED \r\n");
					}
					else
					{
						printf("\r\n SD COMPARE : OK \r\n");
					}
				}
			}
		}
	}
}










