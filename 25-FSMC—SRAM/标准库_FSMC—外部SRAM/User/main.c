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

//1.ʹ��ָ���SRAM���ж�д
//2.ʹ�þ��Ե�ַ�ķ�ʽ����SRAM,���ַ�ʽ���붨���ȫ�ֱ���


uint8_t testValue __attribute__ ((at (Bank1_SRAM3_ADDR+0x40)));

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{
	
	LED_GPIO_Config();

	
	Debug_USART_Config();
	printf("\r\n ����һ��SRAM����ʵ�� \r\n");
	FSMC_SRAM_Init();
	
	LED_BLUE;
	
	/* ��SRAM���ж�д���ԣ����SRAM�Ƿ����� */
	if(SRAM_Test() == 1)
	{
		//�������� �̵���
		LED_GREEN;
	}
	else
	{
		//����ʧ�� �����
		LED_RED;
	}
	
	{
		/* ָ�뷽ʽ����SRAM */
		uint32_t temp;
		printf("\r\n ָ�뷽ʽ����SRAM \r\n");
		
		/* ��SRAMд��8λ���� */
		*(uint8_t*)(Bank1_SRAM3_ADDR) = (uint8_t)0xAA;
		printf("\r\n ָ�����SRAM��д������0xAA \r\n");
		
		/* ��SRAM�ж�ȡ���� */
		temp = *(uint8_t*)(Bank1_SRAM3_ADDR);
		printf("��ȡ���ݣ�0x%X \r\n",temp);
		
		/* ��SRAMд��16λ���� */
		*(uint16_t*)(Bank1_SRAM3_ADDR+10) = (uint16_t)0xBBBB;
		printf("\r\n ָ�����SRAM��д������0xAA \r\n");
		
		/* ��SRAM�ж�ȡ���� */
		temp = *(uint16_t*)(Bank1_SRAM3_ADDR+10);
		printf("��ȡ���ݣ�0x%X \r\n",temp);
		
		/* ��SRAMд��32λ���� */
		*(uint32_t*)(Bank1_SRAM3_ADDR+20) = (uint32_t)0xCCCCCCCC;
		printf("\r\n ָ�����SRAM��д������0xAA \r\n");
		
		/* ��SRAM�ж�ȡ���� */
		temp = *(uint32_t*)(Bank1_SRAM3_ADDR+20);
		printf("��ȡ���ݣ�0x%X \r\n",temp);
	}
	
	
	/* ���Զ�λ��ʽ����SRAM�����ַ�ʽ���붨���ȫ�ֱ��� */
	{
		testValue = 0xDD;
		printf("\r\n ���Զ�λ����SRAM��д������0xDD,��������0x%X,������ַΪ%X \r\n",testValue,(uint32_t )&testValue);
	}
}


/*********************************************END OF FILE**********************/
