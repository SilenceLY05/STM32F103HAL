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

//#include "bsp_sram.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include <stdlib.h>
#include "bsp_sram.h"








void Delay(__IO uint32_t nCount);

void SystemClock_Config(void);


//�ؼ��� __attribute__((at(��ַ)))   __attribute__((section("������")))
//���ñ������嵽��EXRAM�������ĺ�
#define __EXRAM __attribute__((section("EXRAM")))


//�����������ָ���Ĵ洢�ռ䡱
uint32_t testValue __EXRAM= 7;

//�����������ָ���Ĵ洢�ռ䡱
uint32_t testValue2 = 7;


//�������鵽��ָ���Ĵ洢�ռ䡱
uint8_t testGroup[3] __EXRAM = {1,2,3};
////�������鵽SRAM
uint8_t testGroup2[3] = {1,2,3};


int main(void)
{
	uint32_t inertestValue = 10;
	
	
	
	/* ϵͳʱ�ӳ�ʼ����72 MHz */
	SystemClock_Config();

	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();
	
	DEBUG_USART_Config();
	
	
	printf("\r\n SCT�ļ�Ӧ�á�������ʹ���ڲ�SRAM���Ѷѷ���ָ���ռ� ʵ�� \r\n");
	
	printf("\r\n ʹ�á�uint32_t inertestValue = 10;����䶨��ľֲ�����  \r\n");
	printf("��������ĵ�ַΪ��0x%x,����ֵΪ��%d\r\n",(uint32_t)&inertestValue,inertestValue);
	
	printf("\r\n ʹ�á�uint32_t testValue __EXRAM= 7;����䶨��ľֲ�����  \r\n");
	printf("��������ĵ�ַΪ��0x%x,����ֵΪ��%d\r\n",(uint32_t)&testValue,testValue);
	
	printf("\r\n ʹ�á�uint32_t testValue2 = 7;����䶨��ľֲ�����  \r\n");
	printf("��������ĵ�ַΪ��0x%x,����ֵΪ��%d\r\n",(uint32_t)&testValue2,testValue2);
	
	printf("\r\n ʹ�á�uint32_t testGroup[3] __EXRAM = {1,2,3};����䶨��ľֲ�����  \r\n");
	printf("��������ĵ�ַΪ��0x%x,����ֵΪ��%d,%d,%d\r\n",(uint32_t)&testGroup,testGroup[0],testGroup[1],testGroup[2]);
	
	printf("\r\n ʹ�á�uint32_t testGroup2[3] = {0,1,2};����䶨��ľֲ�����  \r\n");
	printf("��������ĵ�ַΪ��0x%x,����ֵΪ��%d,%d,%d\r\n",(uint32_t)&testGroup2,testGroup2[0],testGroup2[1],testGroup2[2]);
	
	
	
	uint32_t *pointer = (uint32_t *)malloc(sizeof(uint32_t)*3);
	
	if(pointer != NULL)
	{
		*(pointer) = 1;
		*(++pointer) = 2;
		*(++pointer) = 3;
		
		
		printf("\r\n ʹ�á�uint32_t * pointer = (uint32_t *)malloc(sizeof(uint32_t)*3);����̬����ı��� \r\n");
		printf("\r\n �����Ĳ���Ϊ�� \r\n*(pointer++) = 1;\r\n*(pointer++) = 2;\r\n*pointer = 3;\r\n");
		printf("��������������ĵ�ַΪ��0x%x���鿴����������\r\n",(uint32_t)pointer);
		printf("*(pointer--) = %d,\r\n",*(pointer--));
		printf("*(pointer--) = %d,\r\n",*(pointer--));
		printf("*(pointer) = %d,\r\n",*(pointer));
		
		free(pointer);
	}
	else
	{
		printf("\r\n ʹ��malloc��̬��������������� \r\n");
	}
	LED_BLUE;
	

	/* ����LED�� */
	while (1)
	{
			
	}
}


void Delay(__IO uint32_t nCount)
{
	for(;nCount != 0; nCount --);
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
