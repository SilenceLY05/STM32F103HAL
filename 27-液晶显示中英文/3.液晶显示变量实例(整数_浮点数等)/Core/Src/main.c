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
#include "bsp_spi_flash.h"
#include <string.h>




void LCD_Test(void);
static void Delay(__IO uint32_t nCount);
void SystemClock_Config(void);
void Printf_Charater(void)   ;


int main(void)
{
  
  //HAL_Init();

  
  SystemClock_Config();
	LED_GPIO_Config();
	
	DEBUG_USART_Config();
	
	ILI9341_Init();
	
	printf("\r\n ********** 液晶显示变量示例（整数_浮点数等）*********** \r\n"); 
	printf("\r\n 若汉字显示不正常，请阅读工程中的readme.txt文件说明，根据要求给FLASH重刷字模数据\r\n"); 
	
	
	//其中0、3、5、6 模式适合从左至右显示文字，
 //不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
 //其中 6 模式为大部分液晶例程的默认显示方向
	ILI9341_GramScan(6);
	

	Printf_Charater();
	
  
  while (1)
  {
    LCD_Test();
  }
  
}



void LCD_Test(void)
{
	/* 演示显示变量 */
	static float testFloatCNT = 0;
	static uint8_t testIntCNT=0;
	char dispBuff[100];
	char *pStr = 0;
	
	testIntCNT++;
	testFloatCNT += 0.1;
	
	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);
	
	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
	
	/* 显示变量示例 */
	//使用c标准库把变量转化为字符串
	sprintf(dispBuff,"显示整型变量:%d",testIntCNT);
	LCD_ClearLine(LINE(5));
	
	/* 然后显示该字符串即可，其他变量也是这个处理 */
	ILI9341_DispStringLine_EN_CH(LINE(5),dispBuff);
	
	
	sprintf(dispBuff,"显示浮点型变量:%f",testFloatCNT);
	LCD_ClearLine(LINE(6));
	
	/* 然后显示该字符串即可，其他变量也是这个处理 */
	ILI9341_DispStringLine_EN_CH(LINE(6),dispBuff);
	
	
	sprintf(dispBuff,"显示浮点型(保留两位小数):%.2f",testFloatCNT);
	LCD_ClearLine(LINE(7));
	
	/* 然后显示该字符串即可，其他变量也是这个处理 */
	ILI9341_DispStringLine_EN_CH(LINE(7),dispBuff);
	
	
	/* 居中显示示例 */
	LCD_SetTextColor(GREEN);
	
	pStr = "插入2个英文空格示例";
	//使用%*c在字符串前插入指定个数的英文空格
	sprintf(dispBuff,"%*c%s",2,' ',pStr);
	
	LCD_ClearLine(LINE(9));
	ILI9341_DispStringLine_EN_CH(LINE(9),dispBuff);
	
	ILI9341_DispStringLine_EN_CH(LINE(11),"纯英文24x32居中示例：");
	LCD_SetFont(&Font24x32);
	pStr = "ABCDEFG";
	
	/* 居中时，要插入的空格个数 = （液晶宽度/单个字体宽度 - 字符串长度）/2 */
	sprintf(dispBuff ,"%*c%s",((LCD_X_LENGTH / (((sFONT*)LCD_GetFont())->Width))-strlen(pStr))/2,' ',pStr);
	
	LCD_ClearLine(LINE(6));
	ILI9341_DispStringLine_EN_CH(LINE(6),dispBuff);
	
	LCD_SetFont(&Font8x16);
	pStr = "中文居中显示：";
	
	//居中时，要插入的空格个数 = (液晶宽度/字体宽度 - 字符串长度)/2
	//strlen计算长度时，一个中文等于2个字节，即2个英文字符，而且插入的是英文空格
	//所以用(WIDTH_CH_CHAR/2)来计算字体宽度
	sprintf(dispBuff ,"%*c%s",(LCD_X_LENGTH / (WIDTH_CH_CHAR/2)-strlen(pStr))/2,' ',pStr);
	
	LCD_ClearLine(LINE(15));
	ILI9341_DispStringLine_EN_CH(LINE(15),dispBuff);
	
	pStr = "含英文居中示例ABCDEFG";
	//居中时，要插入的空格个数 = (液晶宽度/字体宽度 - 字符串长度)/2
	//strlen计算长度时，一个中文等于2个字节，即2个英文字符，而且插入的是英文空格
	//所以用(WIDTH_CH_CHAR/2)来计算字体宽度
	sprintf(dispBuff,"%*c%s",(LCD_X_LENGTH/(WIDTH_CH_CHAR/2)-strlen(pStr))/2,' ',pStr);
 
	LCD_ClearLine(LINE(16));												// 清除单行文字 
	ILI9341_DispStringLine_EN_CH(LINE(16),dispBuff);//显示格式化后的字符串
	
	Delay(0xffffff);
	
	
	
	

}


static void Delay(__IO uint32_t nCount)
{
	for(;nCount != 0;nCount--);
}

/*"当"字符的字模16x16 */ 	  
unsigned char charater_matrix[] =    
{ /*"当",0*/
0x01,0x00,0x21,0x08,0x11,0x08,0x09,0x10,0x09,0x20,0x01,0x00,0x7F,0xF8,0x00,	0x08,
0x00,0x08,0x00,0x08,0x3F,0xF8,0x00,0x08,0x00,0x08,0x00,0x08,0x7F,0xF8,0x00,0x08,

};   
  
/**
  * @brief  使用串口在上位机打印字模
	*					演示字模显示原理
  * @retval 无
  */
void Printf_Charater(void)   
{   
	int i,j;   
	unsigned char kk; 
  
	/*i用作行计数*/
  for ( i=0;i<16;i++)   
	{   
		/*j用作一字节内数据的移位计数*/
		/*一行像素的第一个字节*/
    for(j=0; j<8; j++)   
    {      
			/*一个数据位一个数据位地处理*/
      kk = charater_matrix[2*i] << j ;  //左移J位          
			if( kk & 0x80)     
			{   
				printf("*"); //如果最高位为1，输出*号，表示笔迹
			}   
			else  
			{   
				printf(" "); //如果最高位为0，输出空格，表示空白  
			}   
    }   
		/*一行像素的第二个字节*/
    for(j=0; j<8; j++)   
		{              
      kk = charater_matrix[2*i+1] << j ;  //左移J位   
       
			if( kk & 0x80)         
			{   
				printf("*"); //如果最高位为1，输出*号，表示笔迹   
			}   
			else  
			{   
				printf(" "); //如果最高位为0，输出空格，表示空白 
			}   
		}         
    printf("\n");    //输出完一行像素，换行 
	}   
	printf("\n\n"); 		//一个字输出完毕
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
