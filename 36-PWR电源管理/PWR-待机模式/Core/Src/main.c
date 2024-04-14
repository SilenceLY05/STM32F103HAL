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




/*
*�� ����ʵ�������
* 1.ʹ���ⲿ�ɵ���Դ�����ڳ�5V��������ӵ�ʵ��������� 5V<--->GND��������ӹ��磻
*2.��λʵ��壬��ѹ����ʱ�����ϵ�LED�ʵ�ӦΪ��ɫ
*3.���µ��ڿɵ���Դ�ĵ�ѹ����Լ������4V��ʱ��LED�ʵƻ�תΪ��ɫ��
*�������п���PVD��ص�ѹԼΪ2.8V,��5V��Դ����4V��ʱ������STM32��VDD��Դ(3.3V��Դ)�����2.8V������PVD�¼������ж��п�������ƣ�
*/

/*�� ������ע�����
*ʹ�ÿɵ���Դ��ʵ��幩�磬������Դ�߶��ε�(������������USB��)��
*����ֱ�ӽ����빩��û�е�·���������ڵ�ԴʱС�Ĳ�Ҫʹ�����ѹԶ����5V,��ѹ̫�߻��ջ�ʵ��壡��
*/


void SystemClock_Config(void);
static uint8_t KEY2_LongPress(void);


int main(void)
{

  //HAL_Init();

  
  SystemClock_Config();
	LED_GPIO_Config();
	LED_GREEN;
	
	DEBUG_USART_Config();
	

	Key_GPIO_Config();
	
	printf("\r\n ��ӭʹ��Ұ��  STM32 F103 �����塣\r\n");
	printf("\r\n Ұ��F103 ����ģʽ����\r\n");
	
	printf("\r\n ʵ��˵����\r\n");

	printf("\r\n 1.�������У��̵Ʊ�ʾ���θ�λ���ϵ�����Ÿ�λ����Ʊ�ʾ�����������״̬�����Ʊ�ʾ�����Ǵ������ѵĸ�λ\r\n");
	printf("\r\n 2.����KEY2�����󣬻�������ģʽ\r\n");
	printf("\r\n 3.�ڴ���ģʽ�£���KEY1�����ɻ��ѣ����Ѻ�ϵͳ����и�λ�������ͷ��ʼִ��\r\n");
	printf("\r\n 4.��ͨ�����WU��־λȷ����λ��Դ\r\n");
	
	printf("\r\n 5.�ڴ���״̬�£�DAP�������޷���STM32���س�����Ҫ���Ѻ��������");
	
	//��⸴λ��Դ
	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) == SET)
	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
		LED_BLUE;
		printf("\r\n �������Ѹ�λ \r\n");
	}
	else
	{
		LED_GREEN;
		printf("\r\n �Ǵ������Ѹ�λ \r\n");
	}
	
  while (1)
  {
		if(KEY2_LongPress())
		{
			printf("\r\n �����������ģʽ���������ģʽ��ɰ�KEY1�������ѣ����Ѻ����и�λ�������ͷ��ʼִ�� \r\n");
			LED_RED;
			HAL_Delay(1000);
			
			/* ���WU״̬λ */
			__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
			
			/* ʹ��WKUP���ŵĻ��ѹ��ܣ�ʹ��PA0 */
			HAL_PWR_EnableWakeUpPin(0x00000100U);
			
			/* ��ͣ�δ�ʱ�ӣ���ֹͨ���δ�ʱ���жϻ��� */
			HAL_SuspendTick();
			
			/* �������ģʽ */
			HAL_PWR_EnterSTANDBYMode();
			
		}

  }
  
}

static uint8_t KEY2_LongPress(void)
{
	uint8_t downCnt = 0;   //��¼���µĴ���
	uint8_t upCnt = 0;     //��¼�ɿ��Ĵ���
	
	
	while(1)
	{
		HAL_Delay(20);    //��ʱһ��ʱ���ټ��
		
		if(HAL_GPIO_ReadPin(KEY2_GPIO_PORT,KEY2_PIN) == SET)  //��⵽���°���
		{
			downCnt++;
			
			upCnt = 0;
			
			if(downCnt >= 100)
			{
				return 1;
			}
			else
			{
				upCnt ++;//��¼�ͷŴ���
				if(upCnt >5)
				{
					return 0;//����ʱ��̫�̣����ǰ�����������
				}
			}
		}
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
