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

static void SYSCLKConfig_STOP(void);
void SystemClock_Config(void);



int main(void)
{
	uint32_t SYSCLK_Frequency = 0;
	uint32_t HCLK_Frequency = 0;
	uint32_t PCLK1_Frequency = 0;
	uint32_t PCLK2_Frequency = 0;
	uint32_t SYSCLK_Source = 0;
	
  //HAL_Init();

  
  SystemClock_Config();
	LED_GPIO_Config();
	
	DEBUG_USART_Config();
	
	EXTI_Key_Config();
	
	
	printf("\r\n 欢迎使用野火  STM32 F103 开发板。\r\n");
    printf("\r\n 野火F103 停止模式例程\r\n");
	
	printf("\r\n 实验说明：\r\n");

	printf("\r\n 1.本程序中，绿灯表示STM32正常运行，红灯表示睡眠状态，蓝灯表示刚从停止状态被唤醒\r\n");
	printf("\r\n 2.程序运行一段时间后自动进入停止状态，在停止状态下，可使用KEY1或KEY2唤醒\r\n");
	printf("\r\n 3.本实验执行这样一个循环：\r\n ------》亮绿灯(正常运行)->亮红灯(停止模式)->按KEY1或KEY2唤醒->亮蓝灯(刚被唤醒)-----》\r\n");
	printf("\r\n 4.在停止状态下，DAP下载器无法给STM32下载程序，\r\n 可按KEY1、KEY2唤醒后下载，\r\n 或按复位键使芯片处于复位状态，然后在电脑上点击下载按钮，再释放复位按键，即可下载\r\n");

	
	
	
  while (1)
  {
		/* 执行任务 */
		printf("\r\n STM32正常运行，亮绿灯 \r\n");
		LED_GREEN;
		
		HAL_Delay(2000);
		
		
		/* 任务执行完毕，进入睡眠降低功耗 */
		printf("\r\n 进入停止模式，亮红灯，按KEY1或KEY2按键可唤醒 \r\n");
		
		//使用红灯指示进入睡眠
		LED_RED;
		
		//暂停滴答时钟，防止通过滴答时钟中断唤醒
		HAL_SuspendTick();
		
		//进入睡眠模式
		HAL_PWR_EnterSTOPMode(PWR_MAINREGULATOR_ON,PWR_STOPENTRY_WFI);
		
		//等待中断唤醒  K1或K2按键中断	
		/***被唤醒，亮蓝灯指示***/
		LED_BLUE;
		
		//根据时钟寄存器的值更新SystemCoreClock变量
		SystemCoreClockUpdate();
		
		//获取唤醒后的时钟状态
	  SYSCLK_Frequency = HAL_RCC_GetSysClockFreq();
	  HCLK_Frequency = HAL_RCC_GetHCLKFreq();
	  PCLK1_Frequency = HAL_RCC_GetPCLK1Freq();
		PCLK2_Frequency = HAL_RCC_GetPCLK2Freq();
		SYSCLK_Source = __HAL_RCC_GET_SYSCLK_SOURCE();
		
		//这里由于串口直接使用HSI时钟，不会影响串口波特率
		printf("\r\n 刚唤醒的时钟状态： \r\n");
		printf("SYSCLK频率:%d,\r\n HCLK频率:%d,\r\n PCLK1频率:%d,\r\n PCLK2频率:%d,\r\n 时钟源:%d(0表示HSI，8表示PLLCLK)\r\n",
		SYSCLK_Frequency,HCLK_Frequency,PCLK1_Frequency,PCLK2_Frequency,SYSCLK_Source);
		
		/* 从停止模式唤醒后配置系统时钟：启用HSE、PLL */
		/* 选择PLL作为系数时钟源(HSE和PLL在停止模式下被禁用) */
		SYSCLKConfig_STOP();
		
		//被唤醒后，回复滴答时钟
		HAL_ResumeTick();
		
		//获取重新配置后的时钟状态
		SYSCLK_Frequency = HAL_RCC_GetSysClockFreq();
	  HCLK_Frequency = HAL_RCC_GetHCLKFreq();
	  PCLK1_Frequency = HAL_RCC_GetPCLK1Freq();
		PCLK2_Frequency = HAL_RCC_GetPCLK2Freq();
		SYSCLK_Source = __HAL_RCC_GET_SYSCLK_SOURCE();
		
		//这里由于串口直接使用HSI时钟，不会影响串口波特率
		printf("\r\n 重新配置后的时钟状态： \r\n");
		printf("SYSCLK频率:%d,\r\n HCLK频率:%d,\r\n PCLK1频率:%d,\r\n PCLK2频率:%d,\r\n 时钟源:%d(0表示HSI，8表示PLLCLK)\r\n",
		SYSCLK_Frequency,HCLK_Frequency,PCLK1_Frequency,PCLK2_Frequency,SYSCLK_Source);
		
		HAL_Delay(2000);
		
		printf("\r\n 已退出停止模式 \r\n");
  }
  
}


static void SYSCLKConfig_STOP(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	uint32_t pFlatency = 0;

	/* 启动电源控制时钟 */
	__HAL_RCC_PWR_CLK_ENABLE();
	
	/* 根据内部RCC寄存器获取振荡器配置 */
	HAL_RCC_GetOscConfig(&RCC_OscInitStruct);
 
	/* 从停止模式唤醒后重新配置系统时钟：启用HSE和PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* 根据内部RCC寄存器获取时钟配置 */
	HAL_RCC_GetClockConfig(&RCC_ClkInitStruct,&pFlatency);
	
	/* 选择PLL作为系统时钟源，并配置HCLK、PCLK1、PCLK2时钟分频系数 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
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
