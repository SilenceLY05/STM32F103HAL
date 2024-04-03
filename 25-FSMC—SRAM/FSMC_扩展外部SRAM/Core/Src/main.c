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
#include "stm32f1xx.h"
#include "bsp_usart.h"
#include "bsp_sram.h"
#include "bsp_led.h"
#include <stdlib.h>



void SystemClock_Config(void);


/* 绝对定位方式访问SRAM，这种方式必须定义成全局变量 */
uint8_t testValue __attribute__((at(Bank1_SRAM3_ADDR)));



int main(void)
{
  
  HAL_Init();

  
  SystemClock_Config();
	LED_GPIO_Config();
	
	DEBUG_USART_Config();
	
	FSMC_SRAM_Init();
	
	printf("\r\n 野火外部 SRAM 测试 \r\n");
	LED_BLUE;
	
	/* 对SRAM进行读写测试，检测是否正常 */
	if(SRAM_Test() == 1)
	{
		LED_GREEN;
	}
	else
	{
		LED_RED;
	}
	
	/* 指针方式访问SRAM */
	{
		uint32_t temp;
		
		printf("\r\n 指针方式访问SRAM \r\n");
		
		/* 向SRAM写入8位数据 */
		*(uint8_t *)(Bank1_SRAM3_ADDR) = (uint8_t)0xAA;
		printf("\r\n 指针访问SRAM，写入数据0xAA \r\n");
		
		/* 从SRAM中读取数据 */
		temp = *(uint8_t*)(Bank1_SRAM3_ADDR);
		printf("\r\n 读取数据:0x%X \r\n",temp);
		
		*(uint16_t *)(Bank1_SRAM3_ADDR+10) = (uint16_t)0xBBBB;
		printf("\r\n 指针访问SRAM，写入数据0xBBBB \r\n");
		
		/* 从SRAM中读取数据 */
		temp = *(uint16_t*)(Bank1_SRAM3_ADDR+10);
		printf("\r\n 读取数据:0x%X \r\n",temp);
		
		
		*(uint32_t *)(Bank1_SRAM3_ADDR+20) = (uint32_t)0xCCCCCCCC;
		printf("\r\n 指针访问SRAM，写入数据0xCCCCCCCC \r\n");
		
		/* 从SRAM中读取数据 */
		temp = *(uint32_t*)(Bank1_SRAM3_ADDR+20);
		printf("\r\n 读取数据:0x%X \r\n",temp);
	}
	
	/* 绝对定位方式访问SRAM，这种方式必须定义成全局变量 */
	{
		testValue = 0xDD;
		printf("\r\n绝对定位访问SRAM，写入数据0xDD,读出数据0x%X,变量地址为%X\r\n",testValue,(uint32_t )&testValue);
	}

  
  while (1)
  {
    
  }
  
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
