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
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_spi_flash.h"
#include "stm32f1xx.h"
#include "ff.h"

BYTE work[FF_MAX_SS];         //��ʽ���豸������

FATFS fs;                     //FatFs�ļ�ϵͳ����
FIL   fnew;                   //�ļ�����
FRESULT res_flash;            //�ļ��������
UINT fnum;                    //�ļ��ɹ���д����
BYTE ReadBuffer[1024] = {0};
BYTE WriteBuffer[] = "ʹ�÷���������½��ļ�ϵͳ����\r\n";


void SystemClock_Config(void);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
 
  HAL_Init();

  
  SystemClock_Config();
	
	LED_GPIO_Config();
	LED_BLUE;
	
	DEBUG_USART_Config();
	
	printf("****** ����һ��SPI FLASH �ļ�ϵͳʵ�� ******\r\n");
	
	
	//���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
	//��ʼ������������������
	//f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
	res_flash = f_mount(&fs,"1:",1);
	
	/*----------------------- ��ʽ������ -----------------*/  
	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("��FLASH��û���ļ�ϵͳ���������и�ʽ��....\r\n");
		res_flash = f_mkfs("1:",0,work,sizeof(work));
		
		if(res_flash == FR_OK)
		{
			printf("��FLASH�ѳɹ���ʽ���ļ�ϵͳ\r\n");
			
			/* ��ʽ������ȡ������ */
			res_flash = f_mount(NULL,"1:",1);
			res_flash = f_mount(&fs,"1:",1);
		}
		else
		{
			LED_RED;
			printf("���� ��ʽ��ʧ�� ����\r\n");
			while(1);
		}
	}
	else if(res_flash != FR_OK)
	{
		printf("�����ⲿFLASH�����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
		printf("������SPI-��д����FLASH���̲��ԣ�����������ڸ�����f_mount�����if���ǰ��ʱ�����һ�� res_flash = FR_NO_FILESYSTEM; ������ֱ��ִ�и�ʽ������\r\n");
		while(1);
	}
	else
	{
		printf("���ļ�ϵͳ���سɹ������Խ��ж�д���� \r\n");
	}
	
	/*----------------------- �ļ�ϵͳ���ԣ�д���� -------------------*/
	/* ���ļ���ÿ�ζ����½�����ʽ�򿪣�����Ϊ��д */
	printf("\r\n ****** ���������ļ�д�����... ****** \r\n");
	res_flash = f_open(&fnew, "1:FatFs��д�����ļ�.txt",FA_WRITE|FA_CREATE_ALWAYS);
	
	if(res_flash == FR_OK)
	{
		printf("����/����FatFs��д�����ļ�,txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
		
		/* ��ָ���洢������д�뵽�ļ��� */
		res_flash = f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
		if(res_flash == FR_OK)
		{
			printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\r\n",fnum);
			printf("�����ļ�д������Ϊ\r\n%s\r\n",WriteBuffer);
		}
		else
		{
			printf("�����ļ�д������ʧ�ܣ�%d\r\n",res_flash);
		}
		f_close(&fnew);
	}
	else
	{	
		LED_RED;
		printf("������/�����ļ�ʧ�ܡ�\r\n");
	}
	
	
	/*------------------- �ļ�ϵͳ���ԣ������� --------------------------*/
	printf("****** ���������ļ���ȡ����... ******\r\n");
	res_flash = f_open(&fnew,"1:FatFs��д�����ļ�.txt",FA_OPEN_EXISTING | FA_READ);
	if(res_flash == FR_OK)
	{
		LED_GREEN;
		printf("�����ļ��ɹ�\r\n");
		
		res_flash = f_read(&fnew,ReadBuffer,sizeof(ReadBuffer),&fnum);
		
		if(res_flash == FR_OK)
		{
			printf("���ļ���ȡ�ɹ��������ֽ�����:%d\r\n",fnum);
			printf("����ȡ�õ��ļ�����Ϊ:\r\n%s\r\n",ReadBuffer);
		}
		else
		{
			printf("�����ļ���ȡʧ��:%d\r\n",res_flash);
		}
	}
	else
	{
		LED_RED;
		printf("�������ļ�ʧ�ܡ�\r\n");
	}
	/* ���ٶ�д���ر��ļ� */
	f_close(&fnew);	
  
	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
	f_mount(NULL,"1:",1);
  
  /* ������ɣ�ͣ�� */
	
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
