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
#include "bsp_key.h"
#include "bsp_sdio_sdcard.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"



char SDPath[4];                      /* SD���߼��豸·�� */  
FATFS fs;                            /* FatFs�ļ�ϵͳ���� */
FIL   file;                          /* �ļ����� */
FRESULT f_res;                       /* �ļ�������� */
UINT fnum;                           /* �ļ��ɹ���д���� */
BYTE ReadBuffer[1024] = {0};         /* �������� */
BYTE WriteBuffer[] = "��ӭʹ��Ұ��STM32 ������ �����Ǹ������ӣ��½��ļ�ϵͳ�����ļ�\r\n";





void printf_fatfs_error(FRESULT fresult);
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
	
	Key_GPIO_Config();
	
	DEBUG_USART_Config();
	
	
	printf("****** ����һ��SD�� �ļ�ϵͳʵ�� ******\r\n");
	
	
	/* ע��һ��FatFs�豸��SD�� */
	if(FATFS_LinkDriver(&SD_Driver,SDPath) == 0)
	{
		f_res = f_mount(&fs,(TCHAR const*)SDPath,1);
		printf_fatfs_error(f_res);
	
	/*----------------------- ��ʽ������ -----------------*/  
	/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
	if(f_res == FR_NO_FILESYSTEM)
	{
		printf("��SD����û���ļ�ϵͳ���������и�ʽ��....\r\n");
		f_res = f_mkfs((TCHAR const*)SDPath,0,0);
		
		if(f_res == FR_OK)
		{
			printf("��SD���ѳɹ���ʽ���ļ�ϵͳ\r\n");
			
			/* ��ʽ������ȡ������ */
			f_res = f_mount(NULL,(TCHAR const*)SDPath,1);
			f_res = f_mount(&fs,(TCHAR const*)SDPath,1);
		}
		else
		{
			LED_RED;
			printf("���� ��ʽ��ʧ�� ����\r\n");
			while(1);
		}
	}
	else if(f_res != FR_OK)
	{
		printf("����SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n",f_res);
		printf_fatfs_error(f_res);
		while(1);
	}
	else
	{
		printf("���ļ�ϵͳ���سɹ������Խ��ж�д���� \r\n");
	}
	
	/*----------------------- �ļ�ϵͳ���ԣ�д���� -------------------*/
	/* ���ļ���ÿ�ζ����½�����ʽ�򿪣�����Ϊ��д */
	printf("\r\n ****** ���������ļ�д�����... ****** \r\n");
	char tempfilepath[60];
	
	sprintf(tempfilepath,"%s%s",SDPath,"SD����д�����ļ�.txt");
	f_res = f_open(&file, tempfilepath,FA_WRITE|FA_CREATE_ALWAYS);
	
	if(f_res == FR_OK)
	{
		printf("����/����SD����д�����ļ�,txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
		
		/* ��ָ���洢������д�뵽�ļ��� */
		f_res = f_write(&file,WriteBuffer,sizeof(WriteBuffer),&fnum);
		if(f_res == FR_OK)
		{
			printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\r\n",fnum);
			printf("�����ļ�д������Ϊ\r\n%s\r\n",WriteBuffer);
		}
		else
		{
			printf("�����ļ�д������ʧ�ܣ�%d\r\n",f_res);
		}
		f_close(&file);
	}
	else
	{	
		LED_RED;
		printf("������/�����ļ�ʧ�ܡ�\r\n");
	}
	
	
	/*------------------- �ļ�ϵͳ���ԣ������� --------------------------*/
	printf("****** ���������ļ���ȡ����... ******\r\n");
	f_res = f_open(&file,tempfilepath,FA_OPEN_EXISTING | FA_READ);
	if(f_res == FR_OK)
	{
		LED_GREEN;
		printf("�����ļ��ɹ�\r\n");
		
		f_res = f_read(&file,ReadBuffer,sizeof(ReadBuffer),&fnum);
		
		if(f_res == FR_OK)
		{
			printf("���ļ���ȡ�ɹ��������ֽ�����:%d\r\n",fnum);
			printf("����ȡ�õ��ļ�����Ϊ:\r\n%s\r\n",ReadBuffer);
		}
		else
		{
			printf("�����ļ���ȡʧ��:%d\r\n",f_res);
		}
	}
	else
	{
		LED_RED;
		printf("�������ļ�ʧ�ܡ�\r\n");
	}
	/* ���ٶ�д���ر��ļ� */
	f_close(&file);	
  
	/* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
	f_mount(NULL,(TCHAR const*)SDPath,1);
  
  /* ������ɣ�ͣ�� */
	}
	
  /* ע��һ��FatFS�豸������FLASH */
  FATFS_UnLinkDriver(SDPath);
	
  while (1)
  {
    
  }
	
}




void printf_fatfs_error(FRESULT fresult)
{
	switch(fresult)
	{
		case FR_OK:                   
      printf("�������ɹ���\r\n");
    break;
    case FR_DISK_ERR:            
      printf("����Ӳ�����������������\r\n");
    break;
    case FR_INT_ERR:             
      printf("�������Դ���\r\n");
    break;
    case FR_NOT_READY:            
      printf("���������豸�޷�������\r\n");
    break;
    case FR_NO_FILE:             
      printf("�����޷��ҵ��ļ���\r\n");
    break;
    case FR_NO_PATH:              
      printf("�����޷��ҵ�·����\r\n");
    break;
    case FR_INVALID_NAME:         
      printf("������Ч��·������\r\n");
    break;
    case FR_DENIED:               
    case FR_EXIST:               
      printf("�����ܾ����ʡ�\r\n");
    break;
    case FR_INVALID_OBJECT:       
      printf("������Ч���ļ���·����\r\n");
    break;
    case FR_WRITE_PROTECTED:      
      printf("�����߼��豸д������\r\n");
    break;
    case FR_INVALID_DRIVE:        
      printf("������Ч���߼��豸��\r\n");
    break;
    case FR_NOT_ENABLED:          
      printf("������Ч�Ĺ�������\r\n");
    break;
    case FR_NO_FILESYSTEM:        
      printf("������Ч���ļ�ϵͳ��\r\n");
    break;
    case FR_MKFS_ABORTED:         
      printf("���������������⵼��f_mkfs��������ʧ�ܡ�\r\n");
    break;
    case FR_TIMEOUT:              
      printf("����������ʱ��\r\n");
    break;
    case FR_LOCKED:               
      printf("�����ļ���������\r\n");
    break;
    case FR_NOT_ENOUGH_CORE:      
      printf("�������ļ���֧�ֻ�ȡ�ѿռ�ʧ�ܡ�\r\n");
    break;
    case FR_TOO_MANY_OPEN_FILES:  
      printf("������̫���ļ���\r\n");
    break;
    case FR_INVALID_PARAMETER:    
      printf("����������Ч��\r\n");
    break;
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
