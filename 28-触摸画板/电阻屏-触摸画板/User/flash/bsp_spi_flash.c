#include "bsp_spi_flash.h"

SPI_HandleTypeDef SPI_Handle;


static __IO uint32_t SPITimeout = SPIT_LONG_TIMEOUT;

static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);


/* SPI1 init function */
void SPI_FLASH_Init(void)
{

  SPI_Handle.Instance = SPIx;
  SPI_Handle.Init.Mode = SPI_MODE_MASTER;
  SPI_Handle.Init.Direction = SPI_DIRECTION_2LINES;
  SPI_Handle.Init.DataSize = SPI_DATASIZE_8BIT;
  SPI_Handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
  SPI_Handle.Init.CLKPhase = SPI_PHASE_2EDGE;
  SPI_Handle.Init.NSS = SPI_NSS_SOFT;
  SPI_Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  SPI_Handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
  SPI_Handle.Init.TIMode = SPI_TIMODE_DISABLE;
  SPI_Handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  SPI_Handle.Init.CRCPolynomial = 7;
	
	SPI_Handle.Init.Mode = SPI_MODE_MASTER;
	
	HAL_SPI_Init(&SPI_Handle);
	
	__HAL_SPI_ENABLE(&SPI_Handle);


}

void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{

  GPIO_InitTypeDef GPIO_InitStructure;
	
	SPIx_CLK_ENABLE();
	SPIx_SCK_GPIO_CLK_ENABLE();
	SPIx_MISO_GPIO_CLK_ENABLE();
	SPIx_MOSI_GPIO_CLK_ENABLE();
	SPIx_CS_GPIO_CLK_ENABLE();
    
  GPIO_InitStructure.Pin = SPIx_SCK_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pin = SPIx_MISO_PIN;  
  HAL_GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);
  
  /* SPI MOSI GPIO pin configuration  */
  GPIO_InitStructure.Pin = SPIx_MOSI_PIN; 
  HAL_GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);   

  GPIO_InitStructure.Pin = FLASH_CS_PIN ;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStructure); 

}



/* 通过SPI发送一个字节的数据 byte:要发送的数据 返回接受到的数据 */
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	
	/* 等待发送缓冲区位空 TXE事件 */
	while(__HAL_SPI_GET_FLAG(&SPI_Handle,SPI_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
	}
	
	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	WRITE_REG(SPI_Handle.Instance->DR,byte);
	
	SPITimeout = SPIT_FLAG_TIMEOUT;
	
	/* 等待接收缓冲区位空 RXNE事件 */
	while(__HAL_SPI_GET_FLAG(&SPI_Handle,SPI_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
	}
	
	/* 读取数据寄存器，获取接收缓冲区数据 */
	return READ_REG(SPI_Handle.Instance->DR);
}



/* 使用SPI读取一个字节的数据 */
uint8_t SPI_FLASH_ReadByte(void)
{
	return SPI_FLASH_SendByte(Dummy_Byte);
}



/* 通过SPI发送两个个字节的数据 HalfWord:要发送的数据 返回接受到的数据 */
uint8_t SPI_FLASH_HalfWord(uint16_t HalfWord)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	
	/* 等待发送缓冲区位空 TXE事件 */
	while(__HAL_SPI_GET_FLAG(&SPI_Handle,SPI_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
	}
	
	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	WRITE_REG(SPI_Handle.Instance->DR,HalfWord);
	
	SPITimeout = SPIT_FLAG_TIMEOUT;
	
	/* 等待接收缓冲区位空 RXNE事件 */
	while(__HAL_SPI_GET_FLAG(&SPI_Handle,SPI_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
	}
	
	/* 读取数据寄存器，获取接收缓冲区数据 */
	return READ_REG(SPI_Handle.Instance->DR);
}



/* 向FLASH发送写使能命令 */
void SPI_FLASH_WriteEnable(void)
{
	/* 通讯开始 CS低 */
	SPI_FLASH_CS_LOW();
	
	/* 发送写使能命令 */
	SPI_FLASH_SendByte(W25X_WriteEnable);
	
	/* 通讯结束 CS高 */
	SPI_FLASH_CS_HIGH();
}


/* 等待WIP标志被置0，即等待到FLASH内部数据写入完毕 */
void SPI_FLASH_WaitForWriteEnd(void)
{
	uint8_t FLASH_Status = 0;
	
	/* 选择CS低 */
	SPI_FLASH_CS_LOW();
	
	/* 发送读取寄存器状态命令 */
	SPI_FLASH_SendByte(W25X_ReadStatusReg);
	
	SPITimeout = SPIT_FLAG_TIMEOUT;
	
	/* 如果FLASH忙碌则等待 */
	do
	{
		FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
		
		if((SPITimeout --) ==0)
		{
			SPI_TIMEOUT_UserCallback(4);
			return;
		}
	}while((FLASH_Status & WIP_Flag) == SET);     //正在写入标志
	
	SPI_FLASH_CS_HIGH();
}


/* 掉电模式 */
void SPI_FLASH_PowerDown(void)
{
	/* 通讯开始 CS低 */
	SPI_FLASH_CS_LOW();
	
	/* 发送掉电命令 */
	SPI_FLASH_SendByte(W25X_PowerDown);
	
	/* 通讯结束 CS高 */
	SPI_FLASH_CS_HIGH();
}


/* 唤醒模式 */
void SPI_FLASH_WakeUp(void)
{
	/* 通讯开始 CS低 */
	SPI_FLASH_CS_LOW();
	
	/* 发送掉电命令 */
	SPI_FLASH_SendByte(W25X_ReleasePowerDown);
	
	/* 通讯结束 CS高 */
	SPI_FLASH_CS_HIGH();
}




/* 擦除FLASH扇区 SectorAddr:要擦除的扇区地址 */
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
	/* 发送FLASH写使能命令 */
	SPI_FLASH_WriteEnable();
	SPI_FLASH_WaitForWriteEnd();
	
	/* 擦除扇区 */
	SPI_FLASH_CS_LOW();
	
	/* 发送擦除指令 */
	SPI_FLASH_SendByte(W25X_SectorErase);
	/* 发送擦除扇区地址 */
	SPI_FLASH_SendByte((SectorAddr & 0xFF0000)>>16);
	SPI_FLASH_SendByte((SectorAddr & 0xFF00)>>8);
	SPI_FLASH_SendByte(SectorAddr & 0xFF);
	
	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_WaitForWriteEnd();
}



/* 擦除FLASH扇区，整片擦除 */
void SPI_FLASH_BulkErase(void)
{
	SPI_FLASH_WriteEnable();
	
	SPI_FLASH_CS_LOW();
	
	SPI_FLASH_SendByte(W25X_ChipErase);
	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_WaitForWriteEnd();
}




/* 对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区 pBuffer:要写入数据的指针 WriteAddr:写入地址 NumByteToWrite:写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize */
void SPI_FLASH_PageWrite(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	/* 发送FLASH写使能 */
	SPI_FLASH_WriteEnable();
	
	SPI_FLASH_CS_LOW();
	
	SPI_FLASH_SendByte(W25X_PageProgram);
	
	SPI_FLASH_SendByte((WriteAddr&0xFF0000)>>16);
	SPI_FLASH_SendByte((WriteAddr&0xFF00)>>8);
	SPI_FLASH_SendByte(WriteAddr&0xFF);
	
	if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
	{
		NumByteToWrite = SPI_FLASH_PerWritePageSize;
		FLASH_ERROR("SPI_FLASH_PageWrite too large");
	}
	
	/* 写入数据 */
	while(NumByteToWrite--)
	{
		/* 发送当前要写入的字节数据 */
		SPI_FLASH_SendByte(*pBuffer);
		/* 指向下一字节数据 */
		pBuffer++;
	}
	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_WaitForWriteEnd();
}



/* 对FLASH写入数据 pBuffer:要写入的数据指针 WriteAddr:要写入的地址 NumByteToWrite:写入数据长度 */
void SPI_FLASH_BufferWrite(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	uint8_t Addr=0,count=0,temp=0,NumOfSingle=0,NumOfPage=0;
	
	/*mod运算求余，若writeAddr是SPI_FLASH_PageSize整数倍，运算结果Addr值为0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*差count个数据值，刚好可以对齐到页地址*/
  count = SPI_FLASH_PageSize - Addr;	
	/*计算出要写多少整数页*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod运算求余，计算出剩余不满一页的字节数*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	
	/* Addr=0 则WriteAddr刚好按页对齐 */
	if(Addr == 0)
	{
		/* NumByteToWrite < SPI_FLASH_PageSize */
		if(NumOfPage ==0)
		{
			SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumByteToWrite);
		}
		else /* NumByteToWrite > SPI_FLASH_PageSize */
		{
			/* 先把整数页写了 */
			while(NumOfPage --)
			{
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,SPI_FLASH_PageSize);
				pBuffer += SPI_FLASH_PageSize;
				WriteAddr += SPI_FLASH_PageSize;
			}
			
			/* 若有多余的不满一页的数据 写完 */
			SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumOfSingle);
		}
	}
	
	/* 若地址与SPI_FLASH_PageSize不对齐 */
	else
	{
		if(NumOfPage == 0)
		{
			/*当前页剩余的count个位置比NumOfSingle小，写不完*/
			if(NumOfSingle>count)
			{
				temp = NumOfSingle - count;
				
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,count);
				WriteAddr += count;
				pBuffer += count;
				
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,temp);
			}
			else   /*当前页剩余的count个位置能写完NumOfSingle个数据*/
			{
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumByteToWrite);
			}
		}
		else
		{
			NumByteToWrite -= count;
			
			NumOfPage = NumByteToWrite /SPI_FLASH_PageSize;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			
			SPI_FLASH_PageWrite(pBuffer,WriteAddr,count);
			
			pBuffer += count;
			WriteAddr += count;
			
			/* 把整页都写了 */
			while(NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,SPI_FLASH_PageSize);
				pBuffer += SPI_FLASH_PageSize;
				WriteAddr += SPI_FLASH_PageSize;
			}
			
			/* 如果有多余的不满一页的数据 */
			if(NumOfSingle != 0)
			{
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumOfSingle);
			}
		}
	}
	
}



/* 读取FLASH数据 pBuffer:存储读出数据的指针 ReadAddr:读取地址 NumByteToRead:读取数据长度 */
void SPI_FLASH_BufferRead(uint8_t *pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
	SPI_FLASH_CS_LOW();
	
	SPI_FLASH_SendByte(W25X_ReadData);
	
	SPI_FLASH_SendByte((ReadAddr&0xFF0000)>>16);
	SPI_FLASH_SendByte((ReadAddr&0xFF00)>>8);
	SPI_FLASH_SendByte(ReadAddr&0xFF);
	
	/* 写入数据 */
	while(NumByteToRead--)
	{
		/* 发送当前要写入的字节数据 */
		*pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
		/* 指向下一字节数据 */
		pBuffer++;
	}
	SPI_FLASH_CS_HIGH();

}


/* 读取FLASH ID */
uint32_t SPI_FLASH_ReadID(void)
{
	uint32_t temp = 0,temp1 = 0,temp2 =0,temp3 = 0;
	
	SPI_FLASH_CS_LOW();
	
	SPI_FLASH_SendByte(W25X_JedecDeviceID);
	
	temp1 = SPI_FLASH_SendByte(Dummy_Byte);
	temp2 = SPI_FLASH_SendByte(Dummy_Byte);
	temp3 = SPI_FLASH_SendByte(Dummy_Byte);
	
	SPI_FLASH_CS_HIGH()
	
	temp = (temp1<<16 | temp2 <<8 |temp3);
	
	return temp;

}



/* 读取FLASH Device ID */
uint32_t SPI_FLASH_ReadDeviceID(void)
{
	uint32_t temp = 0;
	
	SPI_FLASH_CS_LOW();
	
	SPI_FLASH_SendByte(W25X_DeviceID);
	
	SPI_FLASH_SendByte(Dummy_Byte);
	SPI_FLASH_SendByte(Dummy_Byte);
	SPI_FLASH_SendByte(Dummy_Byte);
	
	temp = SPI_FLASH_SendByte(Dummy_Byte);
	
	SPI_FLASH_CS_HIGH()
	
	return temp;

}



/* 向FLASH发送读取数据的序列 */
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
	SPI_FLASH_CS_LOW();
	
	SPI_FLASH_SendByte(W25X_ReadData);
	
	SPI_FLASH_SendByte((ReadAddr & 0xFF0000)>>16);
	SPI_FLASH_SendByte((ReadAddr & 0xFF00)>>8);
	SPI_FLASH_SendByte(ReadAddr & 0xFF);
}





static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
	/* 等待超时后的处理，输出错误信息 */
	FLASH_ERROR("SPI 等待超时! errorCode = %d",errorCode);
	return 0;
}


