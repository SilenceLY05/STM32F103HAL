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
#include <string.h>



FATFS fs;                     //FatFs�ļ�ϵͳ����
FIL   fnew;                   //�ļ�����
FRESULT res_flash;            //�ļ��������
UINT fnum;                    //�ļ��ɹ���д����
char fpath[100];
char readbuffer[512];


void SystemClock_Config(void);



/* FatFs����ܲ��� */
static FRESULT miscellaneous(void)
{
	DIR dir;
	FATFS *pfs;
	DWORD fre_clust,fre_sect,tot_sect;
	
	printf("\n*************** �豸��Ϣ��ȡ ***************\r\n");
	/* ��ȡ�豸��Ϣ�Ϳմش�С */
	res_flash = f_getfree("1:",&fre_clust,&pfs);
	
	/* ����õ��ܵ����������Ϳ��������� */
	tot_sect = (pfs->n_fatent -2)*pfs->csize;
	fre_sect = fre_clust * pfs->csize;
	
	/* ��ӡ��Ϣ */
	printf("���豸�ܿռ䣺%10lu KB��\n�����ÿռ䣺  %10lu KB��\n", tot_sect *4, fre_sect *4);
	printf("\n******** �ļ���λ�͸�ʽ��д�빦�ܲ��� ********\r\n");
	res_flash = f_open(&fnew,"1:FatFs��д�����ļ�.txt",FA_OPEN_ALWAYS | FA_WRITE | FA_READ);
	
	if(res_flash == FR_OK)
	{
		/* �ļ���λ */
		res_flash = f_lseek(&fnew,f_size(&fnew));
		if(res_flash == FR_OK)
		{
			f_printf(&fnew,"\n ��ԭ���ļ������һ������ \n");
			f_printf(&fnew,"���豸�ܿռ䣺%10lu KB��\n�����ÿռ䣺  %10lu KB��\n", tot_sect *4, fre_sect *4);
			
			/* �ļ���λ����ʼλ�� */
			res_flash = f_lseek(&fnew,0);
			/* ��ȡ�ļ��������ݵ������� */
			res_flash = f_read(&fnew,readbuffer,f_size(&fnew),&fnum);
			
			if(res_flash == FR_OK)
			{
				printf("���ļ����ݣ�\n%s\n",readbuffer);
			}
		}
		f_close(&fnew);
		
		printf("\n********** Ŀ¼���������������ܲ��� **********\r\n");
		
		/* ���Դ�Ŀ¼ */
		res_flash = f_opendir(&dir,"1:TestDir");
		if(res_flash != FR_OK)
		{
			/* ��Ŀ¼ʧ�ܣ��ʹ���Ŀ¼ */
			res_flash = f_mkdir("1:TestDir");
		}
		else
		{
			/* ���Ŀ¼�Ѿ����ڣ��ر��� */
			res_flash = f_closedir(&dir);
			/* ɾ���ļ� */
      f_unlink("1:TestDir/testdir.txt");
		}
		
		if(res_flash == FR_OK)
		{
			/* ���������ƶ��ļ� */
			res_flash = f_rename("1:FatFs��д�����ļ�.txt","1:TestDir/testdir.txt");
		}
	}
	else
	{
		printf("!! ���ļ�ʧ�ܣ�%d\n",res_flash);
    printf("!! ������Ҫ�ٴ����С�FatFs��ֲ���д���ԡ�����\n");
	}
	return res_flash;
}


FILINFO fno;



/* �ļ���Ϣ��ȡ */
static FRESULT file_check(void)
{
	/* ��ȡ�ļ���Ϣ */
	res_flash = f_stat("1:TestDir/testdir.txt",&fno);
	
	if(res_flash == FR_OK)
	{
		printf("��testdir.txt���ļ���Ϣ��\n");
    printf("���ļ���С: %ld(�ֽ�)\n", fno.fsize);
    printf("��ʱ���: %u/%02u/%02u, %02u:%02u\n",
           (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,fno.ftime >> 11, fno.ftime >> 5 & 63);
    printf("������: %c%c%c%c%c\n\n",
           (fno.fattrib & AM_DIR) ? 'D' : '-',      // ��һ��Ŀ¼
           (fno.fattrib & AM_RDO) ? 'R' : '-',      // ֻ���ļ�
           (fno.fattrib & AM_HID) ? 'H' : '-',      // �����ļ�
           (fno.fattrib & AM_SYS) ? 'S' : '-',      // ϵͳ�ļ�
           (fno.fattrib & AM_ARC) ? 'A' : '-');     // �����ļ�
	}
	return res_flash;
}


/**
  * @brief  scan_files �ݹ�ɨ��FatFs�ڵ��ļ�
  * @param  path:��ʼɨ��·��
  * @retval result:�ļ�ϵͳ�ķ���ֵ
  */
static FRESULT scan_files (char* path) 
{ 
  FRESULT res; 		//�����ڵݹ���̱��޸ĵı���������ȫ�ֱ���	
  FILINFO fno; 
  DIR dir; 
  int i;            
  char *fn;        // �ļ���	
	
#if _USE_LFN 
  /* ���ļ���֧�� */
  /* ����������Ҫ2���ֽڱ���һ�����֡�*/
  static char lfn[_MAX_LFN*2 + 1]; 	
  fno.lfname = lfn; 
  fno.lfsize = sizeof(lfn); 
#endif 
  //��Ŀ¼
  res = f_opendir(&dir, path); 
  if (res == FR_OK) 
	{ 
    i = strlen(path); 
    for (;;) 
		{ 
      //��ȡĿ¼�µ����ݣ��ٶ����Զ�����һ���ļ�
      res = f_readdir(&dir, &fno); 								
      //Ϊ��ʱ��ʾ������Ŀ��ȡ��ϣ�����
      if (res != FR_OK || fno.fname[0] == 0) break; 	
#if _USE_LFN 
      fn = *fno.lfname ? fno.lfname : fno.fname; 
#else 
      fn = fno.fname; 
#endif 
      //���ʾ��ǰĿ¼������			
      if (*fn == '.') continue; 	
      //Ŀ¼���ݹ��ȡ      
      if (fno.fattrib & AM_DIR)         
			{ 			
        //�ϳ�����Ŀ¼��        
        sprintf(&path[i], "/%s", fn); 		
        //�ݹ����         
        res = scan_files(path);	
        path[i] = 0;         
        //��ʧ�ܣ�����ѭ��        
        if (res != FR_OK) 
					break; 
      } 
			else 
			{ 
				printf("%s/%s\r\n", path, fn);								//����ļ���	
        /* ������������ȡ�ض���ʽ���ļ�·�� */        
      }//else
    } //for
  } 
  return res; 
}



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
	
	if(res_flash != FR_OK)
	{
		printf("�����ⲿFlash�����ļ�ϵͳʧ�ܡ�(%d)\r\n",res_flash);
    printf("��������ԭ��SPI Flash��ʼ�����ɹ���\r\n");
		while(1);
	}
	else
	{
		printf("���ļ�ϵͳ���سɹ������Խ��в���\r\n");   
	}
	
	res_flash = miscellaneous();
	
	printf("\n*************** �ļ���Ϣ��ȡ���� **************\r\n");
	
	res_flash = file_check();
	
	printf("***************** �ļ�ɨ����� ****************\r\n");
	strcpy(fpath,"1:");
	scan_files(fpath);
	
	f_mount(NULL,"1:",1);
	
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
