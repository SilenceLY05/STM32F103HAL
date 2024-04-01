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




void Delay(__IO uint32_t nCount);

void SystemClock_Config(void);



//读取的ID存储位置
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	uint8_t cal_flag = 0;
	uint8_t k;
	
	long double double_buffer[7] = {0};
	int int_buffer[7] = {0};
	
  HAL_Init();

  
  SystemClock_Config();
	
	LED_GPIO_Config();
	LED_BLUE;
	
	DEBUG_USART_Config();
	
	printf("\r\n 这是一个FLASH读写小数和长整数实验 \r\n");
	printf("\r\n 使用指南者底板时 左上角排针位置 不要将PC0盖有跳帽 防止影响PC0做SPIFLASH片选脚 \r\n");
	
	SPI_FLASH_Init();
	
	DeviceID = SPI_FLASH_ReadDeviceID();
	Delay(200);
	
	FlashID = SPI_FLASH_ReadID();
	
	printf("\r\n FlashID is 0x%X,Manufacturer Device ID is 0x%X \r\n",FlashID,DeviceID);
	
	if(sFLASH_ID == FlashID)
	{
		printf("\r\n 检测到SPI FLASH W25Q64！ \r\n");
		
		
		SPI_FLASH_BufferWrite(&cal_flag,SPI_FLASH_PageSize*0,1);
		if(cal_flag == 0xCD)
		{
			printf("\r\n 检测到数据标志 \r\n");
			
			SPI_FLASH_BufferRead((void*)double_buffer,SPI_FLASH_PageSize*1,sizeof(double_buffer));
			
			SPI_FLASH_BufferRead((void*)int_buffer,SPI_FLASH_PageSize*2,sizeof(int_buffer));
			
			printf("\r\n 从芯片读取数据 \r\n");
			
			for(k=0;k<7;k++)
			{
				printf("\r\n 小数 rx = %LF \r\n",double_buffer[k]);
				printf("\r\n 整数 rx = %d \r\n",int_buffer[k]);
			}
		}
		else
		{
			printf("\r\n 没有检测到数据标志，FLASH没有存储数据，即将进行小数写入数据 \r\n");
			cal_flag = 0xCD;
			
			SPI_FLASH_SectorErase(0);
			
			SPI_FLASH_BufferWrite(&cal_flag,SPI_FLASH_PageSize*0,1);
			
			for(k=0;k<7;k++)
			{
				double_buffer[k] = k+0.1;
				int_buffer[k] = k*500+1;
			}
			
			SPI_FLASH_BufferWrite((void*)double_buffer,SPI_FLASH_PageSize*1,sizeof(double_buffer));
			
			SPI_FLASH_BufferWrite((void*)int_buffer,SPI_FLASH_PageSize*2,sizeof(int_buffer));
			
			
			printf("\r\n 向芯片写入数据 \r\n");
			
			for(k=0;k<7;k++)
			{
				printf("\r\n 小数 rx = %LF \r\n",double_buffer[k]);
				printf("\r\n 整数 rx = %d \r\n",int_buffer[k]);
			}
			printf("\r\n 请复位开发板，以读取数据进行监测 \r\n");
		}
		
	}
	else
	{
		LED_RED;
		
		printf("\r\n 获取不到W25Q128 \r\n");
	}

  
	
	SPI_FLASH_PowerDown();
  while (1)
  {
    
  }
  
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
