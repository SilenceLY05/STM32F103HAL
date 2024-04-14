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
#include "bsp_exti.h"




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



int main(void)
{

  //HAL_Init();

  
  SystemClock_Config();
	LED_GPIO_Config();
	
	DEBUG_USART_Config();
	
	EXTI_Key_Config();
	
	
	printf("\r\n 1.�������У��̵Ʊ�ʾSTM32�������У���Ʊ�ʾ˯��״̬�����Ʊ�ʾ�մ�˯��״̬������\r\n");
	printf("\r\n 2.��������һ��ʱ����Զ�����˯��״̬����˯��״̬�£���ʹ��KEY1��KEY2����\r\n");
	printf("\r\n 3.��ʵ��ִ������һ��ѭ����\r\n ------�����̵�(��������)->�����(˯��ģʽ)->��KEY1��KEY2����->������(�ձ�����)-----��\r\n");
	printf("\r\n 4.��˯��״̬�£�DAP�������޷���STM32���س���\r\n �ɰ�KEY1��KEY2���Ѻ����أ�\r\n �򰴸�λ��ʹоƬ���ڸ�λ״̬��Ȼ���ڵ����ϵ�����ذ�ť�����ͷŸ�λ��������������\r\n");
	
	
	
	
  while (1)
  {
		printf("\r\n STM32�������У����̵� \r\n");
		LED_GREEN;
		
		HAL_Delay(2000);
		
		
		/* ����ִ����ϣ�����˯�߽��͹��� */
		printf("\r\n ����˯��ģʽ������ƣ���KEY1��KEY2�����ɻ��� \r\n");
		
		//ʹ�ú��ָʾ����˯��
		LED_RED;
		
		//��ͣ�δ�ʱ�ӣ���ֹͨ���δ�ʱ���жϻ���
		HAL_SuspendTick();
		
		//����˯��ģʽ
		HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON,PWR_SLEEPENTRY_WFI);
		
		//�ȴ��жϻ���  K1��K2�����ж�	
		/***�����ѣ�������ָʾ***/
		LED_BLUE;
		//�����Ѻ󣬻ظ��δ�ʱ��
		HAL_ResumeTick();
		HAL_Delay(2000);
		
		printf("\r\n ���˳�˯��ģʽ \r\n");
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
