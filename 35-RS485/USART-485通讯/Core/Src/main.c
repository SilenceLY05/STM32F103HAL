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
//#include "bsp_sram.h"
#include "bsp_led.h"
#include <stdlib.h>
#include "bsp_key.h"
#include "bsp_rs485.h"



static void Delay(__IO uint32_t nCount);


void SystemClock_Config(void);



int main(void)
{
  char *pbuf;
	uint16_t len;
  //HAL_Init();

  
  SystemClock_Config();
	LED_GPIO_Config();
	
	DEBUG_USART_Config();
	

	Key_GPIO_Config();
	
	__485_Config();
	
	printf("\r\n ��ӭʹ��Ұ��  STM32 F103 �����塣\r\n");
  printf("\r\n Ұ��F103 485ͨѶʵ������\r\n");
	
	printf("\r\n ʵ�鲽�裺\r\n");

	printf("\r\n 1.ʹ�õ������Ӻ�����485ͨѶ�豸\r\n");
	printf("\r\n 2.ʹ������ñ���Ӻ�:5v --- C/4-5V,485-D --- PD5,485-R ---PD6 \r\n");
	printf("\r\n 3.��ʹ������Ұ�𿪷������ʵ�飬�����������嶼���ر����򼴿ɡ�\r\n");
	printf("\r\n 4.׼���ú󣬰�������һ���������KEY1������ʹ��485���ⷢ��0-255������ \r\n");
	printf("\r\n 5.���������485���յ�256���ֽ����ݣ����������16������ʽ��ӡ������ \r\n");
	
	
  while (1)
  {
		if(Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON)
		{
			uint16_t i;
			LED_BLUE;
			
			__485_TX_EN();
			
			for(i=0;i<0xff;i++)
			{
				__485_SendByte(i);
			}
			
			/* �Ӷ�����ʱ����֤485����������� */
			Delay(0xFFF);
			__485_RX_EN();
			
			LED_GREEN;
			
			printf("\r\n �������ݳɹ��� \r\n");
		}
		else
		{
			LED_BLUE;
			
			pbuf = get_rebuff(&len);
			
			if(len >=256)
			{
				LED_GREEN;
				printf("\r\n ���յ�����Ϊ:%d������ \r\n",len);
				
				_485_DEBUG_ARRAY((uint8_t *)pbuf,len);
				clean_rebuff();
			}
		}

  }
  
}


static void Delay(__IO uint32_t nCount)
{
	for(;nCount != 0;nCount--);
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
