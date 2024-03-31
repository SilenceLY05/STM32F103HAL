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
#include "bsp_i2c_ee.h"
#include "bsp_led.h"

#define DATA_SIZE               256
#define EEPROM_FirstPage        0x00
uint8_t I2C_Buff_Write[DATA_SIZE];
uint8_t I2C_Buff_Read[DATA_SIZE];

void SystemClock_Config(void);
uint8_t I2C_Test(void);


int main(void)
{
  
  HAL_Init();

  
  SystemClock_Config();
	LED_GPIO_Config();
	
	DEBUG_USART_Config();
	
	printf("\r\n 欢迎使用野火STM32F103开发板 \r\n");
	printf("\r\n 这是一个I2C外设AT24C02读写测试例程 \r\n");
	
	I2C_EE_Init();
	
	if(I2C_Test() == 1)
	{
		LED_GREEN;
	}
	else
	{
		LED_RED;
	}

  
  while (1)
  {
    
  }
  
}


/* I2C(AT24C02)读写测试 */
uint8_t I2C_Test(void)
{
	uint16_t i;
	
	EEPROM_INFO("写入的数据");
	
	for(i=0;i<DATA_SIZE;i++)
	{
		I2C_Buff_Write[i] = i;
		printf("0x%02X",I2C_Buff_Write[i]);
		if(i%16 == 15)
			printf("\n\r");
	}
	
	//将I2C_BUFF_WRITE中顺序递增的数据写入EEPROM中
	I2C_EE_BufferWrite(I2C_Buff_Write,EEPROM_FirstPage,DATA_SIZE);
	
	EEPROM_INFO("读出的数据");
	
	//将EEPORM读出的数据顺序保持到I2C_BUFF_READ中
	I2C_EE_BufferRead(I2C_Buff_Read,EEPROM_FirstPage,DATA_SIZE);
	
	//将I2C_BUFF_READ中的数据通过串口打印
	for(i=0;i<DATA_SIZE;i++)
	{
		if(I2C_Buff_Read[i] != I2C_Buff_Write[i])
		{
			printf("0x%02X ", I2C_Buff_Read[i]);
			EEPROM_ERROR("错误:I2C EEPROM写入与读出的数据不一致");
			return 0;
		}
		printf("0x%02X ", I2C_Buff_Read[i]);
		if(i%16 == 15)    
		printf("\n\r");
	}
	EEPROM_INFO("I2C(AT24C02)读写测试成功");
	return 1;
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
