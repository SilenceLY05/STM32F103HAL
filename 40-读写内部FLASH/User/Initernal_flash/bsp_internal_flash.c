#include "bsp_internal_flash.h"
#include "bsp_usart.h"


/* internal FLASH���ڲ�FLASH���ж�д���� */
int InternalFLASH_Test(void)
{
	uint32_t Address = 0x00;            //��¼д��ĵ�ַ
	uint32_t DATA_32 = 0x3210ABCD;      //��¼д�������
	uint32_t NbrOfPage = 0x00;          //��¼д�����ҳ
	__IO uint32_t Data32 = 0;
	
	uint32_t SECTORError = 0;
	
	TestStatus MemoryProgramStatus = PASSED;      //��¼�������Խ��
	
	static FLASH_EraseInitTypeDef EraseInitStruct;
	
	/* ����	*/
	HAL_FLASH_Unlock();
	
	/* ����Ҫ��������ҳ */
	NbrOfPage = (WRITE_END_ADDR - WRITE_START_ADDR) / FLASH_PAGE_SIZE;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.NbPages = NbrOfPage;
	EraseInitStruct.PageAddress = WRITE_START_ADDR;
	
	if(HAL_FLASHEx_Erase(&EraseInitStruct,&SECTORError) != HAL_OK)
	{
		/* �����������أ�ʵ��Ӧ���пɼ��봦�� */
		return -1;
	}
	
	/* ���ڲ�FLASHд������ */
	Address = WRITE_START_ADDR;
	
	while(Address < WRITE_END_ADDR)
	{
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Address,DATA_32) == HAL_OK)
		{
			Address = Address + 4;
		}
		else
		{
			/* д��������أ�ʵ��Ӧ���пɼ��봦�� */
			return -1;
		}
	}
	
	HAL_FLASH_Lock();
	
	/* ���д��������Ƿ���ȷ */
	Address = WRITE_START_ADDR;
	
	while((Address < WRITE_END_ADDR) && (MemoryProgramStatus != FAILED))
	{
		if((*(__IO uint32_t *)Address) != DATA_32)
		{
			MemoryProgramStatus = FAILED;
		}
		Address += 4;
	}
	return MemoryProgramStatus;
}



