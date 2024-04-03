/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����led
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103-ָ���� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_usart.h"
#include "bsp_led.h"
#include "bsp_sram.h"
#include "bsp_malloc.h"



/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{
	uint8_t *p=0;
	uint8_t sramx = 0;  //Ĭ��Ϊ�ڲ�sram

	Debug_USART_Config();

	FSMC_SRAM_Init();
	
	my_mem_init(SRAMIN);  //��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);  //��ʼ���ⲿ�ڴ��
	
	printf("\r\n ����һ���ڴ�������� \r\n");
	printf("\r\n �����ڴ� \r\n");
	
	p = mymalloc (sramx,1024*2);  //����2K�ֽ�
	
	sprintf((char*)p,"Memory Malloc"); // ��������ռ�洢����
	
	printf("SRAMIN USED:%d%%\r\n",my_mem_perused(SRAMIN));  //��ʾ�ڲ��ڴ�ʹ����
	printf("SRAMEX USED:%d%%\r\n",my_mem_perused(SRAMEX));  //��ʾ�ⲿ�ڴ�ʹ����
	
	myfree(sramx,p); // �ͷ��ڴ�
	p = 0; //ָ��յ�ַ
	
	printf("\r\n �ͷ��ڴ� \r\n");
	printf("SRAMIN USED:%d%%\r\n",my_mem_perused(SRAMIN));  //��ʾ�ڲ��ڴ�ʹ����
	printf("SRAMEX USED:%d%%\r\n",my_mem_perused(SRAMEX));  //��ʾ�ⲿ�ڴ�ʹ����
	
	while(1);
	
}


/*********************************************END OF FILE**********************/
