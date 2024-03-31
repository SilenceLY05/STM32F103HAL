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
#include "bsp_i2c_gpio.h"


uint8_t cal_flag = 0;
uint8_t k;

/* 存储小数和整数的数组，各7个 */
long double double_buffer[7] = {0};
int int_buffer[7] = {0};

#define DOUBLE_ADDR        10
#define LONGINT_ADDR       70

void SystemClock_Config(void);


int main(void)
{
  
  HAL_Init();

  
  SystemClock_Config();
	LED_GPIO_Config();
	
	DEBUG_USART_Config();
	
	printf("\r\n 这是一个EEPROM 读写小数和长整数实 \r\n");
	

	/* I2C 外设初(AT24C02)始化 */
	i2c_CfgGpio();
	
	/* 读取数据标志位 */
	ee_ReadBytes(&cal_flag,0,1);
	
	if(cal_flag != 0xCD)   /*若标志等于0xcd，表示之前已有写入数据*/
	{
		printf("\r\n 没有检测到数据标志，FLASH没有存储数据，即将进行小数写入实验 \r\n");
		
		cal_flag = 0xCD;
		
		/* 写入标志到0地址 */
		ee_WriteByte(&cal_flag,0,1);
		
		/* 生成要写入的数据 */
		for(k=0;k<7;k++)
		{
			double_buffer[k] = k+0.1;
			int_buffer[k] = k*500+1;
		}
		
		/*写入小数数据到地址10*/
		ee_WriteByte((void*)double_buffer,DOUBLE_ADDR,sizeof(double_buffer));
		ee_WriteByte((void*)int_buffer,LONGINT_ADDR,sizeof(int_buffer));
		
		printf("向芯片写入数据：");
		
		for(k=0;k<7;k++)
		{
			printf("小数tx = %LF\r\n",double_buffer[k]);
			printf("整数tx = %d\r\n",int_buffer[k]);
		}
		
		printf("\r\n 请复位开发板，已读取数据进行检验 \r\n");
	}
	else
	{
		printf("\r\n 检测到数据标志 \r\n");
		
		ee_ReadBytes((void*)double_buffer,DOUBLE_ADDR,sizeof(double_buffer));
		ee_ReadBytes((void*)int_buffer,LONGINT_ADDR,sizeof(int_buffer));
		
		printf("\r\n 从芯片读取到数据： \r\n");
		for(k=0;k<7;k++)
		{
			printf("小数tx = %LF\r\n",double_buffer[k]);
			printf("整数tx = %d\r\n",int_buffer[k]);
		}
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
