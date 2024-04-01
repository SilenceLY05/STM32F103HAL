/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_spi_flash.h"
#include "stm32f1xx.h"


typedef enum {FAILED=0,PASSED=!FAILED} TestStatus;

void Delay(__IO uint32_t nCount);
TestStatus Buffercmp(uint8_t *pBuffer1,uint8_t *pBuffer2,uint16_t BufferLength);
void SystemClock_Config(void);

/* 获取缓冲区长度 */
#define TxBufferSize1    (countof(TxBuffer1) - 1)
#define RxBufferSize1    (countof(RxBuffer1) - 1)
#define countof(a)       (sizeof(a) / sizeof(*(a)))
	
#define BufferSize       (countof(Tx_Buffer) - 1)

#define FLASH_WriteAddress        0x00000
#define FLASH_ReadAddress         FLASH_WriteAddress
#define FLASH_SectorToErase       FLASH_WriteAddress


/* 发送缓冲区初始化 */
uint8_t Tx_Buffer[] = "感谢您选用野火stm32开发板\r\nhttp://firestm32.taobao.com";
uint8_t Rx_Buffer[BufferSize];

//读取的ID存储位置
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
 
  HAL_Init();

  
  SystemClock_Config();
	
	LED_GPIO_Config();
	LED_BLUE;
	
	DEBUG_USART_Config();
	
	printf("\r\n 这是一个16M串行FLASH实验 \r\n");
	
	SPI_FLASH_Init();
	
	DeviceID = SPI_FLASH_ReadDeviceID();
	Delay(200);
	
	FlashID = SPI_FLASH_ReadID();
	
	printf("\r\n FlashID is 0x%X,Manufacturer Device ID is 0x%X \r\n",FlashID,DeviceID);
	
	if(sFLASH_ID == FlashID)
	{
		printf("\r\n 检测到SPI FLASH W25Q64！ \r\n");
		
		SPI_FLASH_SectorErase(FLASH_SectorToErase);
		
		SPI_FLASH_BufferWrite(Tx_Buffer,FLASH_WriteAddress,BufferSize);
		
		printf("\r\n 写入的数据为：%s \r\n",Tx_Buffer);
		
		SPI_FLASH_BufferRead(Rx_Buffer,FLASH_ReadAddress,BufferSize);
		printf("\r\n 读出的数据为: %s \r\n",Rx_Buffer);
		
		TransferStatus1 = Buffercmp(Tx_Buffer,Rx_Buffer,BufferSize);
		
		if(PASSED == TransferStatus1)
		{
			LED_GREEN;
			printf("\r\n16M串行flash(W25Q64)测试成功!\n\r");
		}
		else
		{
			LED_RED;
			printf("\r\n16M串行flash(W25Q64)测试失败!\n\r");
		}
	}

  else
	{    
		LED_RED;
		printf("\r\n获取不到 W25Q64 ID!\n\r");
	}
	
	SPI_FLASH_PowerDown();
  while (1)
  {
    
  }
  
}


/* 比较两个缓冲区中的数据是否相等 pBuffer1:src缓冲区指针 pBuffer2:dst缓冲区指针 BufferLength:缓冲区长度*/
TestStatus Buffercmp(uint8_t *pBuffer1,uint8_t *pBuffer2,uint16_t BufferLength)
{
	while(BufferLength--)
	{
		if(*pBuffer1 != *pBuffer2)
		{
			return FAILED;
		}
		pBuffer1++;
		pBuffer2++;
	}
	return PASSED;
}


void Delay(__IO uint32_t nCount)
{
	for(;nCount!=0;nCount--);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}



/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
