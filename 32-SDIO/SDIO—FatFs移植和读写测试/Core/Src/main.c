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



char SDPath[4];                      /* SD卡逻辑设备路径 */  
FATFS fs;                            /* FatFs文件系统对象 */
FIL   file;                          /* 文件对象 */
FRESULT f_res;                       /* 文件操作结果 */
UINT fnum;                           /* 文件成功读写数量 */
BYTE ReadBuffer[1024] = {0};         /* 读缓冲区 */
BYTE WriteBuffer[] = "欢迎使用野火STM32 开发板 今天是个好日子，新建文件系统测试文件\r\n";





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
	
	
	printf("****** 这是一个SD卡 文件系统实验 ******\r\n");
	
	
	/* 注册一个FatFs设备：SD卡 */
	if(FATFS_LinkDriver(&SD_Driver,SDPath) == 0)
	{
		f_res = f_mount(&fs,(TCHAR const*)SDPath,1);
		printf_fatfs_error(f_res);
	
	/*----------------------- 格式化测试 -----------------*/  
	/* 如果没有文件系统就格式化创建创建文件系统 */
	if(f_res == FR_NO_FILESYSTEM)
	{
		printf("》SD卡还没有文件系统，即将进行格式化....\r\n");
		f_res = f_mkfs((TCHAR const*)SDPath,0,0);
		
		if(f_res == FR_OK)
		{
			printf("》SD卡已成功格式化文件系统\r\n");
			
			/* 格式化后，先取消挂载 */
			f_res = f_mount(NULL,(TCHAR const*)SDPath,1);
			f_res = f_mount(&fs,(TCHAR const*)SDPath,1);
		}
		else
		{
			LED_RED;
			printf("《《 格式化失败 》》\r\n");
			while(1);
		}
	}
	else if(f_res != FR_OK)
	{
		printf("！！SD卡挂载文件系统失败。(%d)\r\n",f_res);
		printf_fatfs_error(f_res);
		while(1);
	}
	else
	{
		printf("》文件系统挂载成功，可以进行读写测试 \r\n");
	}
	
	/*----------------------- 文件系统测试：写测试 -------------------*/
	/* 打开文件，每次都以新建的形式打开，属性为可写 */
	printf("\r\n ****** 即将进行文件写入测试... ****** \r\n");
	char tempfilepath[60];
	
	sprintf(tempfilepath,"%s%s",SDPath,"SD卡读写测试文件.txt");
	f_res = f_open(&file, tempfilepath,FA_WRITE|FA_CREATE_ALWAYS);
	
	if(f_res == FR_OK)
	{
		printf("》打开/创建SD卡读写测试文件,txt文件成功，向文件写入数据。\r\n");
		
		/* 将指定存储器内容写入到文件内 */
		f_res = f_write(&file,WriteBuffer,sizeof(WriteBuffer),&fnum);
		if(f_res == FR_OK)
		{
			printf("》文件写入成功，写入字节数据：%d\r\n",fnum);
			printf("》向文件写入数据为\r\n%s\r\n",WriteBuffer);
		}
		else
		{
			printf("！！文件写入数据失败：%d\r\n",f_res);
		}
		f_close(&file);
	}
	else
	{	
		LED_RED;
		printf("！！打开/创建文件失败。\r\n");
	}
	
	
	/*------------------- 文件系统测试：读测试 --------------------------*/
	printf("****** 即将进行文件读取测试... ******\r\n");
	f_res = f_open(&file,tempfilepath,FA_OPEN_EXISTING | FA_READ);
	if(f_res == FR_OK)
	{
		LED_GREEN;
		printf("》打开文件成功\r\n");
		
		f_res = f_read(&file,ReadBuffer,sizeof(ReadBuffer),&fnum);
		
		if(f_res == FR_OK)
		{
			printf("》文件读取成功，读到字节数据:%d\r\n",fnum);
			printf("》读取得的文件数据为:\r\n%s\r\n",ReadBuffer);
		}
		else
		{
			printf("！！文件读取失败:%d\r\n",f_res);
		}
	}
	else
	{
		LED_RED;
		printf("！！打开文件失败。\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&file);	
  
	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,(TCHAR const*)SDPath,1);
  
  /* 操作完成，停机 */
	}
	
  /* 注销一个FatFS设备：串行FLASH */
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
      printf("》操作成功。\r\n");
    break;
    case FR_DISK_ERR:            
      printf("！！硬件输入输出驱动出错。\r\n");
    break;
    case FR_INT_ERR:             
      printf("！！断言错误。\r\n");
    break;
    case FR_NOT_READY:            
      printf("！！物理设备无法工作。\r\n");
    break;
    case FR_NO_FILE:             
      printf("！！无法找到文件。\r\n");
    break;
    case FR_NO_PATH:              
      printf("！！无法找到路径。\r\n");
    break;
    case FR_INVALID_NAME:         
      printf("！！无效的路径名。\r\n");
    break;
    case FR_DENIED:               
    case FR_EXIST:               
      printf("！！拒绝访问。\r\n");
    break;
    case FR_INVALID_OBJECT:       
      printf("！！无效的文件或路径。\r\n");
    break;
    case FR_WRITE_PROTECTED:      
      printf("！！逻辑设备写保护。\r\n");
    break;
    case FR_INVALID_DRIVE:        
      printf("！！无效的逻辑设备。\r\n");
    break;
    case FR_NOT_ENABLED:          
      printf("！！无效的工作区。\r\n");
    break;
    case FR_NO_FILESYSTEM:        
      printf("！！无效的文件系统。\r\n");
    break;
    case FR_MKFS_ABORTED:         
      printf("！！因函数参数问题导致f_mkfs函数操作失败。\r\n");
    break;
    case FR_TIMEOUT:              
      printf("！！操作超时。\r\n");
    break;
    case FR_LOCKED:               
      printf("！！文件被保护。\r\n");
    break;
    case FR_NOT_ENOUGH_CORE:      
      printf("！！长文件名支持获取堆空间失败。\r\n");
    break;
    case FR_TOO_MANY_OPEN_FILES:  
      printf("！！打开太多文件。\r\n");
    break;
    case FR_INVALID_PARAMETER:    
      printf("！！参数无效。\r\n");
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
