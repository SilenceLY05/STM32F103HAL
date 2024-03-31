#include "bsp_i2c_ee.h"
#include "bsp_usart.h"

I2C_HandleTypeDef      I2C_Handle;


void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	GPIO_InitTypeDef          GPIO_InitStructure;
	
	I2Cx_CLK_ENABLE();
	
	I2Cx_SDA_GPIO_CLK_ENABLE();
	I2Cx_SCL_GPIO_CLK_ENABLE();

  GPIO_InitStructure.Pin = I2Cx_SCL_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = I2Cx_SDA_PIN;
	HAL_GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStructure);
  /* Force the I2C peripheral clock reset */ 
  I2Cx_FORCE_RESET();
	/* Release the I2C peripheral clock reset */  
	I2Cx_RELEASE_RESET();
}


void I2C_Mode_Config(void)
{
  I2C_Handle.Instance = I2Cx;
  I2C_Handle.Init.ClockSpeed = 400000;
  I2C_Handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
  I2C_Handle.Init.OwnAddress1 = I2C_OWM_ADDRESS7;
  I2C_Handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  I2C_Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2C_Handle.Init.OwnAddress2 = 0;
  I2C_Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2C_Handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  /* USER CODE BEGIN I2C1_Init 2 */
	HAL_I2C_Init(&I2C_Handle);
  /* USER CODE END I2C1_Init 2 */

}

/* I2C外设EEPROM初始化 */
void I2C_EE_Init(void)
{
	I2C_Mode_Config();
}


/* 写一个字节到I2C EEPROM缓冲区 pBuffer:缓冲区指针 WriteAddr:写地址 */
uint32_t I2C_EE_ByteWrite(uint8_t *pBuffer,uint8_t WriteAddr)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	/* 写一定量的数据到内存地址 返回HAL状态 */
	status = HAL_I2C_Mem_Write(&I2C_Handle,EEPROM_ADDRESS,(uint16_t)WriteAddr,I2C_MEMADD_SIZE_8BIT,pBuffer,1,100);
	
	/* 检查通讯状态 */
	if(status != HAL_OK)
	{
		/* Execute user timeout callback */
		//I2Cx_Error(Addr);
	}
	
	while(HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY)
	{
		
	}
	
	/* 检查EEPROM是否准备好新操作 */
	while(HAL_I2C_IsDeviceReady(&I2C_Handle,EEPROM_ADDRESS,EEPROM_MAX_TRIALS,I2Cx_TIMEOUT_MAX) == HAL_TIMEOUT);
	
	/* 等待传输结束 */
	while(HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY)
	{
		
	}
	return status;
}


/* 在EEPROM的一个写循环中可以写入多个字节，但一次写入的字节数不能超过EEPROM页的大小，AT24C02每页有8个字节
	 pBuffer:缓冲区指针 WriteAddr:写地址 NumByteToWrite:写的字节数*/
uint32_t I2C_EE_PageWrite(uint8_t *pBuffer,uint8_t WriteAddr,uint8_t NumByteToWrite)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	/* 写一定量的数据到内存地址 返回HAL状态 */
	status = HAL_I2C_Mem_Write(&I2C_Handle,EEPROM_ADDRESS,WriteAddr,I2C_MEMADD_SIZE_8BIT,(uint8_t*)pBuffer,NumByteToWrite,100);

	while(HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY)
	{
		
	}
	
	/* 检查EEPROM是否准备好新操作 */
	while(HAL_I2C_IsDeviceReady(&I2C_Handle,EEPROM_ADDRESS,EEPROM_MAX_TRIALS,I2Cx_TIMEOUT_MAX) == HAL_TIMEOUT);
	
	/* 等待传输结束 */
	while(HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY)
	{
		
	}
	return status;
}


/* 从EEPROM里面读取一块数据 pBuffer:存放从EEPROM读取的数据的缓冲区指针 WriteAddr:接收数据的EEPROM地址 NumByteToRead:要从EEPROM中读取的字节数 */
uint32_t I2C_EE_BufferRead(uint8_t *pBuffer,uint8_t ReadAddr,uint16_t NumByteToRead)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	status = HAL_I2C_Mem_Read(&I2C_Handle,EEPROM_ADDRESS,ReadAddr,I2C_MEMADD_SIZE_8BIT,(uint8_t *)pBuffer,NumByteToRead,100);
	
	return status;
}



/* 将缓冲区中的数据写到I2C EEPROM中 pBuffer:缓冲区指针 WriteAddr:写地址 NumByteToWrite:写的字节数 */
void I2C_EE_BufferWrite(uint8_t *pBuffer,uint8_t WriteAddr,uint16_t NumByteToWrite)
{
	uint8_t NumOfPage = 0,NumOfSingle = 0,Addr = 0,count = 0;
	
	Addr = WriteAddr % EEPROM_PAGESIZE;   //页地址开始位置
	count = EEPROM_PAGESIZE - Addr;       //这页还剩几个数据位
	NumOfPage = NumByteToWrite / EEPROM_PAGESIZE;    //写入数据能写几页
	NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;  //写入数据除了整页还剩几个数据
	
	/* 如果写地址和页对齐 */
	if(Addr == 0)
	{
		/* 如果写入数据量小于页写量 */
		if(NumOfPage == 0)
		{
			I2C_EE_PageWrite(pBuffer,WriteAddr,NumOfSingle);
		}
		else
		{
			while(NumOfPage--)
			{
				I2C_EE_PageWrite(pBuffer,WriteAddr,EEPROM_PAGESIZE);
				WriteAddr += EEPROM_PAGESIZE;
				pBuffer += EEPROM_PAGESIZE;
			}
			if(NumOfSingle != 0)
			{
				I2C_EE_PageWrite(pBuffer,WriteAddr,NumOfSingle);
			}
		}
	}
	else   /* 如果没有页对齐 */
	{
		if(NumOfPage == 0)
		{
			I2C_EE_PageWrite(pBuffer,WriteAddr,NumOfSingle);
		}
		else
		{
			NumByteToWrite -= count;
			NumOfPage = NumByteToWrite /EEPROM_PAGESIZE;
			NumOfSingle = NumByteToWrite % EEPROM_PAGESIZE;
			
			if(count !=0)
			{
				I2C_EE_PageWrite(pBuffer,WriteAddr,count);
				WriteAddr += count;
				pBuffer += count;
			}
			while(NumOfPage --)
			{
				I2C_EE_PageWrite(pBuffer,WriteAddr,EEPROM_PAGESIZE);
				WriteAddr += EEPROM_PAGESIZE;
				pBuffer += EEPROM_PAGESIZE;
			}
			if(NumOfSingle != 0)
      {
        I2C_EE_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
      }
		}
	}
}




