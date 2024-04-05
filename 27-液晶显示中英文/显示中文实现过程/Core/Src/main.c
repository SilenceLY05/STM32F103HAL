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
#include "bsp_ili9341_lcd.h"


void LCD_Test(void);
static void Delay(__IO uint32_t nCount);
void SystemClock_Config(void);



int main(void)
{
  
  //HAL_Init();

  
  SystemClock_Config();
	LED_GPIO_Config();
	
	DEBUG_USART_Config();
	
	ILI9341_Init();
	
	printf("\r\n ****************** 液晶屏英文显示程序 ***************** \r\n");
	printf("\r\n 本程序不支持中文，显示中文程序再下一章 \r\n");
	
	
	//其中0、3、5、6 模式适合从左至右显示文字，
 //不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
 //其中 6 模式为大部分液晶例程的默认显示方向
	ILI9341_GramScan(6);
	
	myshow_ch(240,200);
	myshow_chs(280,240,"一");
	
	
  
  while (1)
  {
    LCD_Test();
  }
  
}

extern uint16_t lcdid;

void LCD_Test(void)
{
	/* 演示显示变量 */
	static uint8_t testCNT=0;
	char dispBuff[100];
	
	testCNT++;
	
	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);
	
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	
	/* 显示字符串示例 */
	ILI9341_DispStringLine_EN(LINE(0),"BH 3.2 inch LCD para");
	ILI9341_DispStringLine_EN(LINE(1),"Image resolution:240x320 px");
	
	if(lcdid == LCDID_ILI9341)
	{
		ILI9341_DispStringLine_EN(LINE(2),"ILI9341 LCD driver");
	}
	else if(lcdid == LCDID_ST7789V)
	{
		ILI9341_DispStringLine_EN(LINE(2),"ST7789V LCD driver");
		
	}
	ILI9341_DispStringLine_EN(LINE(3),"XPT2045 Touch Pad driver");
	
	/* 显示变量示例 */
	LCD_SetFont(&Font8x16);
	LCD_SetTextColor(GREEN);
	
	/* 使用标准库把变量转化成字符串 */
	sprintf(dispBuff,"Count : %d",testCNT);
	
	LCD_ClearLine(LINE(4));
	
	ILI9341_DispStringLine_EN(LINE(4),dispBuff);
	
	LCD_SetFont(&Font24x32);
	
	LCD_ClearLine(LINE(4));
	LCD_SetTextColor(BLUE);
	
	ILI9341_DispStringLine_EN(LINE(4),"Draw line:");
	
	LCD_SetTextColor(RED);
	ILI9341_DrawLine(50,170,210,230);
	ILI9341_DrawLine(50,200,210,240);
	
	LCD_SetTextColor(GREEN);
  ILI9341_DrawLine(100,170,200,230);  
  ILI9341_DrawLine(200,200,220,240);
	
	LCD_SetTextColor(BLUE);
  ILI9341_DrawLine(110,170,110,230);  
  ILI9341_DrawLine(130,200,220,240);
  
  Delay(0xFFFFFF);
  
  ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全黑 */
  
  
  /*画矩形*/

  LCD_ClearLine(LINE(4));	/* 清除单行文字 */
	LCD_SetTextColor(BLUE);

  ILI9341_DispStringLine_EN(LINE(4),"Draw Rect:");

	LCD_SetTextColor(RED);
  ILI9341_DrawRectangle(50,200,100,30,1);
	
	LCD_SetTextColor(GREEN);
  ILI9341_DrawRectangle(160,200,20,40,0);
	
	LCD_SetTextColor(BLUE);
  ILI9341_DrawRectangle(170,200,50,20,1);
  
  
  Delay(0xFFFFFF);
	
	ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全黑 */

  /* 画圆 */
  LCD_ClearLine(LINE(4));	/* 清除单行文字 */
	LCD_SetTextColor(BLUE);
	
  ILI9341_DispStringLine_EN(LINE(4),"Draw Cir:");

	LCD_SetTextColor(RED);
  ILI9341_DrawCircle(100,200,20,0);
	
	LCD_SetTextColor(GREEN);
  ILI9341_DrawCircle(100,200,10,1);
	
	LCD_SetTextColor(BLUE);
	ILI9341_DrawCircle(140,200,20,0);

  Delay(0xFFFFFF);
  
  ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全黑 */
	
	

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
