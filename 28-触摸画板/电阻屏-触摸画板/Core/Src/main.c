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
#include <string.h>
#include "bsp_ili9341_lcd.h"
#include "bsp_spi_flash.h"
#include "bsp_xpt2046_lcd.h"
#include "palette.h"





void SystemClock_Config(void);
void Printf_Charater(void)   ;


int main(void)
{
  
  //HAL_Init();

  
  SystemClock_Config();
	LED_GPIO_Config();
	
	DEBUG_USART_Config();
	
	ILI9341_Init();
	XPT2046_Init();
	
	//��FLASH���ȡУ����������FLASH�޲�������ʹ��ģʽ3����У��
	
	
	printf("\r\n ********** ����������� *********** \r\n"); 
	printf("\r\n ��������ʾ�����������Ķ������е�readme.txt�ļ�˵��������Ҫ���FLASH��ˢ��ģ����\r\n"); 
	
	Calibrate_or_Get_TouchParaWithFlash(3,1);
	//����0��3��5��6 ģʽ�ʺϴ���������ʾ���֣�
 //���Ƽ�ʹ������ģʽ��ʾ����	����ģʽ��ʾ���ֻ��о���Ч��			
 //���� 6 ģʽΪ�󲿷�Һ�����̵�Ĭ����ʾ����
	ILI9341_GramScan(3);
	
	Palette_Init(LCD_SCAN_MODE);

	
  
  while (1)
  {
		//������⺯��������������10ms����һ��
    XPT2046_TouchEvenHandler();
  }
  
}



/*"��"�ַ�����ģ16x16 */ 	  
unsigned char charater_matrix[] =    
{ /*"��",0*/
0x01,0x00,0x21,0x08,0x11,0x08,0x09,0x10,0x09,0x20,0x01,0x00,0x7F,0xF8,0x00,	0x08,
0x00,0x08,0x00,0x08,0x3F,0xF8,0x00,0x08,0x00,0x08,0x00,0x08,0x7F,0xF8,0x00,0x08,

};   
  
/**
  * @brief  ʹ�ô�������λ����ӡ��ģ
	*					��ʾ��ģ��ʾԭ��
  * @retval ��
  */
void Printf_Charater(void)   
{   
	int i,j;   
	unsigned char kk; 
  
	/*i�����м���*/
  for ( i=0;i<16;i++)   
	{   
		/*j����һ�ֽ������ݵ���λ����*/
		/*һ�����صĵ�һ���ֽ�*/
    for(j=0; j<8; j++)   
    {      
			/*һ������λһ������λ�ش���*/
      kk = charater_matrix[2*i] << j ;  //����Jλ          
			if( kk & 0x80)     
			{   
				printf("*"); //������λΪ1�����*�ţ���ʾ�ʼ�
			}   
			else  
			{   
				printf(" "); //������λΪ0������ո񣬱�ʾ�հ�  
			}   
    }   
		/*һ�����صĵڶ����ֽ�*/
    for(j=0; j<8; j++)   
		{              
      kk = charater_matrix[2*i+1] << j ;  //����Jλ   
       
			if( kk & 0x80)         
			{   
				printf("*"); //������λΪ1�����*�ţ���ʾ�ʼ�   
			}   
			else  
			{   
				printf(" "); //������λΪ0������ո񣬱�ʾ�հ� 
			}   
		}         
    printf("\n");    //�����һ�����أ����� 
	}   
	printf("\n\n"); 		//һ����������
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
