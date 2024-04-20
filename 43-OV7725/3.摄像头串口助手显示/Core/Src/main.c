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
#include "bsp_key.h"
#include "bsp_ov7725.h"
#include <stdlib.h>
#include "bsp_systick.h"
#include "protocol.h"
#include "wildfire_image_assistant.h"


extern uint8_t Ov7725_vsync;
unsigned int Task_Delay[NumOfTask];
extern OV7725_MODE_PARAM cam_mode;


void SystemClock_Config(void);



int main(void)
{
  
  //HAL_Init();

	uint8_t retry = 0;
	protocol_init();

  
  SystemClock_Config();
	LED_GPIO_Config();
	
	DEBUG_USART_Config();
	
	Key_GPIO_Config();
	Systick_Init();
	
	/* ov7725 gpio ��ʼ�� */
	OV7725_GPIO_Config();
	
	
	
//	ILI9341_Init();
//	ILI9341_GramScan(3);
//	LCD_SetFont(&Font8x16);
//	LCD_SetColors(RED,BLACK);
//	
//	ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);   /* ��������ʾȫ�� */
//	
//	/********��ʾ�ַ���ʾ��*******/
//	ILI9341_DispStringLine_EN(LINE(0),"BH OV7725 Test Demo");
//	
//	
//	
//	printf("\r\n ** OV7725����ͷʵʱҺ����ʾ����** \r\n"); 
//	
//	
//	LED_BLUE;
	
  /* ov7725 �Ĵ���Ĭ�����ó�ʼ�� */
  while (OV7725_Init()!=SUCCESS)
  {
		retry++;
		if(retry>5)
		{
//			printf("\r\n û�м�⵽OV7725����ͷ \r\n");
//			ILI9341_DispStringLine_EN(LINE(2),"No OV7725 module detexted!");
			
			LED_RED;
			while(1);
		}
  }
	/*��������ͷ����������ģʽ*/
	OV7725_Special_Effect(cam_mode.effect);
	/*����ģʽ*/
	OV7725_Light_Mode(cam_mode.light_mode);
	/*���Ͷ�*/
	OV7725_Color_Saturation(cam_mode.saturation);
	/*���ն�*/
	OV7725_Brightness(cam_mode.brightness);
	/*�Աȶ�*/
	OV7725_Contrast(cam_mode.contrast);
	/*����Ч��*/
	OV7725_Special_Effect(cam_mode.effect);
	
	/*����ͼ�������ģʽ��С*/
	OV7725_Window_Set(cam_mode.cam_sx,cam_mode.cam_sy,cam_mode.cam_width,cam_mode.cam_height,cam_mode.QVGA_VGA);
	
//	/* ����Һ��ɨ��ģʽ */
//	ILI9341_GramScan(cam_mode.lcd_scan);
//	ILI9341_DispStringLine_EN(LINE(2),"OV7725 initialize success!");
	
	Ov7725_vsync = 0;
	
	while(1)
	{
		write_reb_wincc(0,cam_mode.cam_width,cam_mode.cam_height);
		receiving_process();
//		/*���յ���ͼ�������ʾ*/
//		if(Ov7725_vsync == 2)
//		{
//			frame_count++;
//			FIFO_PREPARE;       /*FIFO׼��*/				
//			ImageDisp(cam_mode.lcd_sx,cam_mode.lcd_sy,cam_mode.cam_width,cam_mode.cam_height);   	/*�ɼ�����ʾ*/
//			
//			Ov7725_vsync = 0;
//			
//			LED1_TOGGLE;
//		}
//		
//		/* ��ⰴ�� */
//		if(Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON)
//		{
//			LED2_TOGGLE;
//		}
//		
//		if(Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON)
//		{
//			LED3_TOGGLE;
//			
//			/*��̬��������ͷ��ģʽ��
//			����Ҫ�������ʹ�ô��ڡ��û���������ѡ���ȷ�ʽ�޸���Щ������
//			�ﵽ��������ʱ��������ͷģʽ��Ŀ��*/
//			cam_mode.QVGA_VGA = 0,
//			cam_mode.cam_sx = 0,
//			cam_mode.cam_sy = 0,
//			
//			cam_mode.cam_width = 320,
//			cam_mode.cam_height = 240,
//			
//			cam_mode.lcd_sx = 0,
//			cam_mode.lcd_sy = 0,
//			cam_mode.lcd_scan = 3,
//			//LCDɨ��ģʽ�����������ÿ���1��3��5��7ģʽ

//		  //���¿ɸ����Լ�����Ҫ������������Χ���ṹ�����Ͷ���
//			cam_mode.light_mode = 0,      //�Զ�����ģʽ
//			cam_mode.saturation = 0,
//			cam_mode.brightness = 0,
//			cam_mode.contrast = 0,
//			cam_mode.effect = 1,          //�ڰ�ģʽ
//			
//			
//			/*��������ͷ����������ģʽ*/
//			OV7725_Special_Effect(cam_mode.effect);
//			/*����ģʽ*/
//			OV7725_Light_Mode(cam_mode.light_mode);
//			/*���Ͷ�*/
//			OV7725_Color_Saturation(cam_mode.saturation);
//			/*���ն�*/
//			OV7725_Brightness(cam_mode.brightness);
//			/*�Աȶ�*/
//			OV7725_Contrast(cam_mode.contrast);
//			/*����Ч��*/
//			OV7725_Special_Effect(cam_mode.effect);
//			
//			/*����ͼ�������ģʽ��С*/
//			OV7725_Window_Set(cam_mode.cam_sx,cam_mode.cam_sy,cam_mode.cam_width,cam_mode.cam_height,cam_mode.QVGA_VGA);
//			
//			/* ����Һ��ɨ��ģʽ */
//			ILI9341_GramScan(cam_mode.lcd_scan);
//		}
//		
//		/* ÿ��һ��ʱ�����һ��Ƶ�� */
//		if(Task_Delay[0] == 0)
//		{
//			printf("\r\n frame_ate = %.2f fps \r\n",frame_count /10);
//			frame_count = 0;
//			Task_Delay[0] = 10000;
//		}
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
