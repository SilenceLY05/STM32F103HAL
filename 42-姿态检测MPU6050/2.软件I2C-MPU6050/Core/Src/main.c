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
#include "bsp_i2c_gpio.h"
#include "bsp_led.h"
#include "bsp_mpu6050.h"

//�����Ƿ�ʹ��LCD������ʾ������Ҫ�Ļ��������ע�͵�����
#define USE_LCD_DISPLAY


#ifdef USE_LCD_DISPLAY
	#include "bsp_ili9341_lcd.h"
#endif 



/* ��������� */
uint32_t Task_Delay[NumOfTask] = {0};

void SystemClock_Config(void);



int main(void)
{
	short Acel[3];
	short Gyro[3];
	float Temp;

  HAL_Init();

  SystemClock_Config();
	LED_GPIO_Config();
	
#ifdef USE_LCD_DISPLAY

	char cStr[70];
	
	ILI9341_Init();
	
	ILI9341_GramScan(6);
	
	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);
	
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	
	ILI9341_DispString_EN(20,20,"This is a MPU6050 demo");
	
#endif 
	
	DEBUG_USART_Config();
	
	i2c_CfgGpio();
	
	MPU6050_Init();
	
	
	printf("\r\n ��ӭʹ��Ұ�𿪷��� \r\n");
	printf("\r\n ����һ�� MPU6050�������� \r\n");
	
	
	if(MPU6050ReadID() == 1)
	{
		while(1)
		{
			if(Task_Delay[0] == TASK_ENABLE)
			{
				LED2_TOGGLE;
				Task_Delay[0] = 1000;
			}
			
			if(Task_Delay[1] == 0)
			{
				MPU6050ReadAce(Acel);
				printf("\r\n ���ٶȣ�%8d%8d%8d \r\n",Acel[0],Acel[1],Acel[2]);
				#ifdef USE_LCD_DISPLAY
				ILI9341_DispStringLine_EN(LINE(7),"Acceleration");
				sprintf(cStr,"%8d%8d%8d ",Acel[0],Acel[1],Acel[2]);
				ILI9341_DispStringLine_EN(LINE(8),cStr);
				#endif
				
				MPU6050ReadAce(Gyro);
				printf("\r\n �����ǣ�%8d%8d%8d \r\n",Gyro[0],Gyro[1],Gyro[2]);
				#ifdef USE_LCD_DISPLAY
				ILI9341_DispStringLine_EN(LINE(11),"Gyro");
				sprintf(cStr,"%8d%8d%8d ",Gyro[0],Gyro[1],Gyro[2]);
				ILI9341_DispStringLine_EN(LINE(12),cStr);
				#endif
				
				MPU6050_ReturnTemp(&Temp);
				printf("\r\n �¶�:%8.2f \r\n",Temp);
				#ifdef USE_LCD_DISPLAY
				sprintf(cStr,"Temperature:%8.2f ",Temp);
				ILI9341_DispStringLine_EN(LINE(15),cStr);
				#endif
				
				Task_Delay[1] = 500;        //����һ�����ݣ��ɸ����Լ���������߲����ٶȣ���100ms����һ��
			}
		}
	}
	else
	{
		printf("\r\n û�м�⵽MPU6050�������� \r\n");
		LED_RED;
		#ifdef USE_LCD_DISPLAY			
      /*����������ɫ������ı�����ɫ*/
      LCD_SetColors(BLUE,BLACK);	
      ILI9341_DispStringLine_EN(LINE(4),"No MPU6050 detected! ");			
      ILI9341_DispStringLine_EN(LINE(5),"Please check the hardware connection! ");		
    #endif
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
