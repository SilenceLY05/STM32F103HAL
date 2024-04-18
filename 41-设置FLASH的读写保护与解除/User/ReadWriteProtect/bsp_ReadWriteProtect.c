#include "bsp_ReadWriteProtect.h"
#include "bsp_usart.h"
#include "bsp_led.h"

uint32_t Address = 0;
uint32_t PageError = 0;

__IO TestStatus MemoryProgramStatus = PASSED;


/* 用于擦除过程的变量 */
FLASH_EraseInitTypeDef EraseInitStructure;
/* 用于处理选项字节的变量 */
FLASH_OBProgramInitTypeDef         OptionBytesStruct;


/* FLASH_Test普通的写保护配置，运行本函数后会给FLASH_WRP_SECTORS进行写保护，再重复一次进行解除保护 */
void FLASH_Test(void)
{
	printf("初始化结束，开始进行测试\r\n");
	
	/* 初始化测试状态 */
	MemoryProgramStatus = PASSED;
	
	/* 解锁FLASH已启用闪存控制寄存器访问 */
	HAL_FLASH_Unlock();
	
	/* 解锁选项字节 */
	HAL_FLASH_OB_Unlock();
	
	/* 获取页面写保护状态 */
	HAL_FLASHEx_OBGetConfig(&OptionBytesStruct);
	
	#ifdef WRITE_PROTECTION_DISABLE
	/* 检查所需页面是否已经被写保护 */
	if((OptionBytesStruct.WRPPage & FLASH_PAGE_TO_BE_PROTECTED) != FLASH_PAGE_TO_BE_PROTECTED)
	{
		printf("\r\n 页面已被写保护，正在恢复写保护页面 \r\n");
		
		OptionBytesStruct.OptionType = OPTIONBYTE_WRP;
		OptionBytesStruct.WRPState = OB_WRPSTATE_DISABLE;
		OptionBytesStruct.WRPPage = FLASH_PAGE_TO_BE_PROTECTED;
		
		if(HAL_FLASHEx_OBProgram(&OptionBytesStruct) != HAL_OK)
		{
			while(1)
			{
				LED1_ON;
				printf("运行失败\r\n");
			}
		}
		
		/* 生成系统重置以加载新选项字节值 */
		HAL_FLASH_OB_Launch();
	}
	
	#elif   defined WRITE_PROTECTION_ENABLE
	/* 检查所需页面是否尚未写保护 */
	if(((~OptionBytesStruct.WRPPage) & FLASH_PAGE_TO_BE_PROTECTED) != FLASH_PAGE_TO_BE_PROTECTED)
	{
		printf("\r\n 页面未被写保护，正在启用写保护页面 \r\n");
		
		OptionBytesStruct.OptionType = OPTIONBYTE_WRP;
		OptionBytesStruct.WRPState = OB_WRPSTATE_ENABLE;
		OptionBytesStruct.WRPPage = FLASH_PAGE_TO_BE_PROTECTED;
		
		if(HAL_FLASHEx_OBProgram(&OptionBytesStruct) != HAL_OK)
		{
			while(1)
			{
				LED1_ON;
				printf("运行失败\r\n");
			}
		}
		
		/* 生成系统重置以加载新选项字节值 */
		HAL_FLASH_OB_Launch();
	}
	
	#endif /* WRITE_PROTECTION_DISABLE */
	
	/* 锁定选项字节 */
	HAL_FLASH_OB_Lock();
	
	/* 所选页面未被写保护 */
	if((OptionBytesStruct.WRPPage & FLASH_PAGE_TO_BE_PROTECTED) != 0x00)
	{
		printf("\r\n 页面未被写保护，正在擦除页面 \r\n");
		
		EraseInitStructure.TypeErase = FLASH_TYPEERASE_PAGES;
		EraseInitStructure.PageAddress = FLASH_USER_START_ADDR;
		EraseInitStructure.NbPages = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;
		
		if(HAL_FLASHEx_Erase(&EraseInitStructure,&PageError) != HAL_OK)
		{
			/* 
       页面擦除时发生错误。 用户可以在这里添加一些代码来处理这个错误。 PageError将包含有问题的页面，然后知道此页面上的代码错误，
       用户可以调用函数'HAL_FLASH_GetError（）'
      */
			while(1)
			{
				LED1_ON;
				printf("运行失败\r\n");
			}
		}
		
		/* 由FLASH_USER_END_ADDR 和 FLASH_USER_START_ADDR定义的地址处的DATA_32 FLASH字程序 */
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
					printf("运行失败\r\n");
				}
			}
		}
		
		
		/* 检查书面数据的正确性 */
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
		/* 所需页面受写保护 检查是否允许在此页面中写入 */
		Address = FLASH_USER_START_ADDR;
		
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Address,DATA_32) != HAL_OK)
		{
			/* 编程期间返回错误 检查WRPERR标志是否设置良好 */
			if(HAL_FLASH_GetError() == HAL_FLASH_ERROR_WRP)
			{
				MemoryProgramStatus = FAILED;
				
			}
			else
			{
				while(1)
				{
					LED1_ON;
					printf("运行失败\r\n");
				}
			}
		}
		else
		{
			while(1)
			{
				LED1_ON;
				printf("运行失败\r\n");
			}
		}
		
	}
	
	HAL_FLASH_Lock();
	
	
	/* 检测数据是否存在问题 */
	if(MemoryProgramStatus == PASSED)
	{
		LED2_ON;
		printf("\r\n 检查---->数据正常 \r\n");
	}
	else
	{
		while(1)
		{
			LED1_ON;
			printf("运行失败\r\n");
		}
	}
	
	printf("测试结束\r\n");
	
	while(1)
	{
		
	}
}

