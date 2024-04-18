#include "bsp_ReadWriteProtect.h"
#include "bsp_usart.h"
#include "bsp_led.h"

uint32_t Address = 0;
uint32_t PageError = 0;

__IO TestStatus MemoryProgramStatus = PASSED;


/* ���ڲ������̵ı��� */
FLASH_EraseInitTypeDef EraseInitStructure;
/* ���ڴ���ѡ���ֽڵı��� */
FLASH_OBProgramInitTypeDef         OptionBytesStruct;


/* FLASH_Test��ͨ��д�������ã����б���������FLASH_WRP_SECTORS����д���������ظ�һ�ν��н������ */
void FLASH_Test(void)
{
	printf("��ʼ����������ʼ���в���\r\n");
	
	/* ��ʼ������״̬ */
	MemoryProgramStatus = PASSED;
	
	/* ����FLASH������������ƼĴ������� */
	HAL_FLASH_Unlock();
	
	/* ����ѡ���ֽ� */
	HAL_FLASH_OB_Unlock();
	
	/* ��ȡҳ��д����״̬ */
	HAL_FLASHEx_OBGetConfig(&OptionBytesStruct);
	
	#ifdef WRITE_PROTECTION_DISABLE
	/* �������ҳ���Ƿ��Ѿ���д���� */
	if((OptionBytesStruct.WRPPage & FLASH_PAGE_TO_BE_PROTECTED) != FLASH_PAGE_TO_BE_PROTECTED)
	{
		printf("\r\n ҳ���ѱ�д���������ڻָ�д����ҳ�� \r\n");
		
		OptionBytesStruct.OptionType = OPTIONBYTE_WRP;
		OptionBytesStruct.WRPState = OB_WRPSTATE_DISABLE;
		OptionBytesStruct.WRPPage = FLASH_PAGE_TO_BE_PROTECTED;
		
		if(HAL_FLASHEx_OBProgram(&OptionBytesStruct) != HAL_OK)
		{
			while(1)
			{
				LED1_ON;
				printf("����ʧ��\r\n");
			}
		}
		
		/* ����ϵͳ�����Լ�����ѡ���ֽ�ֵ */
		HAL_FLASH_OB_Launch();
	}
	
	#elif   defined WRITE_PROTECTION_ENABLE
	/* �������ҳ���Ƿ���δд���� */
	if(((~OptionBytesStruct.WRPPage) & FLASH_PAGE_TO_BE_PROTECTED) != FLASH_PAGE_TO_BE_PROTECTED)
	{
		printf("\r\n ҳ��δ��д��������������д����ҳ�� \r\n");
		
		OptionBytesStruct.OptionType = OPTIONBYTE_WRP;
		OptionBytesStruct.WRPState = OB_WRPSTATE_ENABLE;
		OptionBytesStruct.WRPPage = FLASH_PAGE_TO_BE_PROTECTED;
		
		if(HAL_FLASHEx_OBProgram(&OptionBytesStruct) != HAL_OK)
		{
			while(1)
			{
				LED1_ON;
				printf("����ʧ��\r\n");
			}
		}
		
		/* ����ϵͳ�����Լ�����ѡ���ֽ�ֵ */
		HAL_FLASH_OB_Launch();
	}
	
	#endif /* WRITE_PROTECTION_DISABLE */
	
	/* ����ѡ���ֽ� */
	HAL_FLASH_OB_Lock();
	
	/* ��ѡҳ��δ��д���� */
	if((OptionBytesStruct.WRPPage & FLASH_PAGE_TO_BE_PROTECTED) != 0x00)
	{
		printf("\r\n ҳ��δ��д���������ڲ���ҳ�� \r\n");
		
		EraseInitStructure.TypeErase = FLASH_TYPEERASE_PAGES;
		EraseInitStructure.PageAddress = FLASH_USER_START_ADDR;
		EraseInitStructure.NbPages = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;
		
		if(HAL_FLASHEx_Erase(&EraseInitStructure,&PageError) != HAL_OK)
		{
			/* 
       ҳ�����ʱ�������� �û��������������һЩ����������������� PageError�������������ҳ�棬Ȼ��֪����ҳ���ϵĴ������
       �û����Ե��ú���'HAL_FLASH_GetError����'
      */
			while(1)
			{
				LED1_ON;
				printf("����ʧ��\r\n");
			}
		}
		
		/* ��FLASH_USER_END_ADDR �� FLASH_USER_START_ADDR����ĵ�ַ����DATA_32 FLASH�ֳ��� */
		Address = FLASH_USER_START_ADDR;
		
		while(Address < FLASH_USER_END_ADDR)
		{
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Address,DATA_32) == HAL_OK)
			{
				Address = Address +4;
			}
			else
			{
				while(1)
				{
					LED1_ON;
					printf("����ʧ��\r\n");
				}
			}
		}
		
		
		/* ����������ݵ���ȷ�� */
		Address = FLASH_USER_START_ADDR;
		
		while(Address < FLASH_USER_END_ADDR)
		{
			if((*(__IO uint32_t*)Address) != DATA_32)
			{
				MemoryProgramStatus = FAILED;
			}
			Address += 4;
		}
	}
	else
	{
		/* ����ҳ����д���� ����Ƿ������ڴ�ҳ����д�� */
		Address = FLASH_USER_START_ADDR;
		
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Address,DATA_32) != HAL_OK)
		{
			/* ����ڼ䷵�ش��� ���WRPERR��־�Ƿ��������� */
			if(HAL_FLASH_GetError() == HAL_FLASH_ERROR_WRP)
			{
				MemoryProgramStatus = FAILED;
				
			}
			else
			{
				while(1)
				{
					LED1_ON;
					printf("����ʧ��\r\n");
				}
			}
		}
		else
		{
			while(1)
			{
				LED1_ON;
				printf("����ʧ��\r\n");
			}
		}
		
	}
	
	HAL_FLASH_Lock();
	
	
	/* ��������Ƿ�������� */
	if(MemoryProgramStatus == PASSED)
	{
		LED2_ON;
		printf("\r\n ���---->�������� \r\n");
	}
	else
	{
		while(1)
		{
			LED1_ON;
			printf("����ʧ��\r\n");
		}
	}
	
	printf("���Խ���\r\n");
	
	while(1)
	{
		
	}
}

