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
*【 ！】实验操作：
* 1.使用外部可调电源，调节成5V输出，连接到实验板引出的 5V<--->GND排针给板子供电；
*2.复位实验板，电压正常时板子上的LED彩灯应为绿色
*3.向下调节可调电源的电压，大约当降至4V的时候，LED彩灯会转为红色。
*（程序中控制PVD监控电压约为2.8V,当5V电源降至4V的时候，连接STM32的VDD电源(3.3V电源)会低于2.8V，产生PVD事件，在中断中控制亮红灯）
*/

/*【 ！！】注意事项：
*使用可调电源给实验板供电，其它电源线都拔掉(包括下载器、USB线)。
*由于直接接排针供电没有电路保护，调节电源时小心不要使供电电压远高于5V,电压太高会烧坏实验板！！
*/


void SystemClock_Config(void);



int main(void)
{

  //HAL_Init();

  
  SystemClock_Config();
	LED_GPIO_Config();
	
	DEBUG_USART_Config();
	
	EXTI_Key_Config();
	
	
	printf("\r\n 1.本程序中，绿灯表示STM32正常运行，红灯表示睡眠状态，蓝灯表示刚从睡眠状态被唤醒\r\n");
	printf("\r\n 2.程序运行一段时间后自动进入睡眠状态，在睡眠状态下，可使用KEY1或KEY2唤醒\r\n");
	printf("\r\n 3.本实验执行这样一个循环：\r\n ------》亮绿灯(正常运行)->亮红灯(睡眠模式)->按KEY1或KEY2唤醒->亮蓝灯(刚被唤醒)-----》\r\n");
	printf("\r\n 4.在睡眠状态下，DAP下载器无法给STM32下载程序，\r\n 可按KEY1、KEY2唤醒后下载，\r\n 或按复位键使芯片处于复位状态，然后在电脑上点击下载按钮，再释放复位按键，即可下载\r\n");
	
	
	
	
  while (1)
  {
		printf("\r\n STM32正常运行，亮绿灯 \r\n");
		LED_GREEN;
		
		HAL_Delay(2000);
		
		
		/* 任务执行完毕，进入睡眠降低功耗 */
		printf("\r\n 进入睡眠模式，亮红灯，按KEY1或KEY2按键可唤醒 \r\n");
		
		//使用红灯指示进入睡眠
		LED_RED;
		
		//暂停滴答时钟，防止通过滴答时钟中断唤醒
		HAL_SuspendTick();
		
		//进入睡眠模式
		HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON,PWR_SLEEPENTRY_WFI);
		
		//等待中断唤醒  K1或K2按键中断	
		/***被唤醒，亮蓝灯指示***/
		LED_BLUE;
		//被唤醒后，回复滴答时钟
		HAL_ResumeTick();
		HAL_Delay(2000);
		
		printf("\r\n 已退出睡眠模式 \r\n");
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
