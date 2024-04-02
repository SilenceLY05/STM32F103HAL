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

BYTE work[FF_MAX_SS];         //格式化设备工作区

FATFS fs;                     //FatFs文件系统对象
FIL   fnew;                   //文件对象
FRESULT res_flash;            //文件操作结果
UINT fnum;                    //文件成功读写数量
BYTE ReadBuffer[1024] = {0};
BYTE WriteBuffer[] = "使用发卡板进行新建文件系统测试\r\n";


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
	
	printf("****** 这是一个SPI FLASH 文件系统实验 ******\r\n");
	
	
	//在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
	//初始化函数调用流程如下
	//f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()
	res_flash = f_mount(&fs,"1:",1);
	
	/*----------------------- 格式化测试 -----------------*/  
	/* 如果没有文件系统就格式化创建创建文件系统 */
	if(res_flash == FR_NO_FILESYSTEM)
	{
		printf("》FLASH还没有文件系统，即将进行格式化....\r\n");
		res_flash = f_mkfs("1:",0,work,sizeof(work));
		
		if(res_flash == FR_OK)
		{
			printf("》FLASH已成功格式化文件系统\r\n");
			
			/* 格式化后，先取消挂载 */
			res_flash = f_mount(NULL,"1:",1);
			res_flash = f_mount(&fs,"1:",1);
		}
		else
		{
			LED_RED;
			printf("《《 格式化失败 》》\r\n");
			while(1);
		}
	}
	else if(res_flash != FR_OK)
	{
		printf("！！外部FLASH挂载文件系统失败。(%d)\r\n",res_flash);
		printf("请下载SPI-读写串行FLASH例程测试，如果正常，在该例程f_mount语句下if语句前临时多添加一句 res_flash = FR_NO_FILESYSTEM; 让重新直接执行格式化流程\r\n");
		while(1);
	}
	else
	{
		printf("》文件系统挂载成功，可以进行读写测试 \r\n");
	}
	
	/*----------------------- 文件系统测试：写测试 -------------------*/
	/* 打开文件，每次都以新建的形式打开，属性为可写 */
	printf("\r\n ****** 即将进行文件写入测试... ****** \r\n");
	res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt",FA_WRITE|FA_CREATE_ALWAYS);
	
	if(res_flash == FR_OK)
	{
		printf("》打开/创建FatFs读写测试文件,txt文件成功，向文件写入数据。\r\n");
		
		/* 将指定存储器内容写入到文件内 */
		res_flash = f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
		if(res_flash == FR_OK)
		{
			printf("》文件写入成功，写入字节数据：%d\r\n",fnum);
			printf("》向文件写入数据为\r\n%s\r\n",WriteBuffer);
		}
		else
		{
			printf("！！文件写入数据失败：%d\r\n",res_flash);
		}
		f_close(&fnew);
	}
	else
	{	
		LED_RED;
		printf("！！打开/创建文件失败。\r\n");
	}
	
	
	/*------------------- 文件系统测试：读测试 --------------------------*/
	printf("****** 即将进行文件读取测试... ******\r\n");
	res_flash = f_open(&fnew,"1:FatFs读写测试文件.txt",FA_OPEN_EXISTING | FA_READ);
	if(res_flash == FR_OK)
	{
		LED_GREEN;
		printf("》打开文件成功\r\n");
		
		res_flash = f_read(&fnew,ReadBuffer,sizeof(ReadBuffer),&fnum);
		
		if(res_flash == FR_OK)
		{
			printf("》文件读取成功，读到字节数据:%d\r\n",fnum);
			printf("》读取得的文件数据为:\r\n%s\r\n",ReadBuffer);
		}
		else
		{
			printf("！！文件读取失败:%d\r\n",res_flash);
		}
	}
	else
	{
		LED_RED;
		printf("！！打开文件失败。\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&fnew);	
  
	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,"1:",1);
  
  /* 操作完成，停机 */
	
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
