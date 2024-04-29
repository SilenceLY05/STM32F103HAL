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
#include "bsp_ad5689.h"
#include "bsp_led.h"
#include "bsp_key.h"





void SystemClock_Config(void);


int main(void)
{
  float temp1,temp2;
	uint16_t vol = 0x7FFF;
  HAL_Init();

  
  SystemClock_Config();
	LED_GPIO_Config();
	
	DEBUG_USART_Config();
	printf("\r\n AD5689电压输出-双通道 \r\n");
	
	printf("按下KEY1增大输出电压，按下KEY2减小输出电压\r\n");
	
	Key_GPIO_Config();
	
	AD5689_Init();
	
	AD5689_WriteUpdata_DACREG(DAC_A,vol);
	AD5689_WriteUpdata_DACREG(DAC_B,0xFF - vol);
	
  while (1)
  {
    if(Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON)
		{
			vol += 0x200;
			AD5689_WriteUpdata_DACREG(DAC_A,vol);
			AD5689_WriteUpdata_DACREG(DAC_B,0xFFFF - vol);
			
			temp1 = (float)vol*1000/0xFFFF;
			temp2 = (float)(0xFFFF-vol)*1000/0xFFFF;
			printf("Voltage Channel1 = %0.2fV\r\n",temp1/100);
			printf("Voltage Channel2 = %0.2fV\r\n",temp2/100);
		}
		if(Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON)
		{
			vol -= 0x200;
			AD5689_WriteUpdata_DACREG(DAC_A,vol);
			AD5689_WriteUpdata_DACREG(DAC_B,0xFFFF - vol);
			
			temp1 = (float)vol*1000/0xFFFF;
			temp2 = (float)(0xFFFF - vol)*1000/0xFFFF;
			printf("Voltage Channel1 = %0.2fV\r\n",temp1/100);
			printf("Voltage Channel2 = %0.2fV\r\n",temp2/100);
		}
		HAL_Delay(20);
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
