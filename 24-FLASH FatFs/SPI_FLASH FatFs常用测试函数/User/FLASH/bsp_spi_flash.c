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



/* ͨ��SPI����һ���ֽڵ����� byte:Ҫ���͵����� ���ؽ��ܵ������� */
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	
	/* �ȴ����ͻ�����λ�� TXE�¼� */
	while(__HAL_SPI_GET_FLAG(&SPI_Handle,SPI_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
	}
	
	/* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
	WRITE_REG(SPI_Handle.Instance->DR,byte);
	
	SPITimeout = SPIT_FLAG_TIMEOUT;
	
	/* �ȴ����ջ�����λ�� RXNE�¼� */
	while(__HAL_SPI_GET_FLAG(&SPI_Handle,SPI_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
	}
	
	/* ��ȡ���ݼĴ�������ȡ���ջ��������� */
	return READ_REG(SPI_Handle.Instance->DR);
}



/* ʹ��SPI��ȡһ���ֽڵ����� */
uint8_t SPI_FLASH_ReadByte(void)
{
	return SPI_FLASH_SendByte(Dummy_Byte);
}



/* ͨ��SPI�����������ֽڵ����� HalfWord:Ҫ���͵����� ���ؽ��ܵ������� */
uint8_t SPI_FLASH_HalfWord(uint16_t HalfWord)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	
	/* �ȴ����ͻ�����λ�� TXE�¼� */
	while(__HAL_SPI_GET_FLAG(&SPI_Handle,SPI_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
	}
	
	/* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
	WRITE_REG(SPI_Handle.Instance->DR,HalfWord);
	
	SPITimeout = SPIT_FLAG_TIMEOUT;
	
	/* �ȴ����ջ�����λ�� RXNE�¼� */
	while(__HAL_SPI_GET_FLAG(&SPI_Handle,SPI_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
	}
	
	/* ��ȡ���ݼĴ�������ȡ���ջ��������� */
	return READ_REG(SPI_Handle.Instance->DR);
}



/* ��FLASH����дʹ������ */
void SPI_FLASH_WriteEnable(void)
{
	/* ͨѶ��ʼ CS�� */
	SPI_FLASH_CS_LOW();
	
	/* ����дʹ������ */
	SPI_FLASH_SendByte(W25X_WriteEnable);
	
	/* ͨѶ���� CS�� */
	SPI_FLASH_CS_HIGH();
}


/* �ȴ�WIP��־����0�����ȴ���FLASH�ڲ�����д����� */
void SPI_FLASH_WaitForWriteEnd(void)
{
	uint8_t FLASH_Status = 0;
	
	/* ѡ��CS�� */
	SPI_FLASH_CS_LOW();
	
	/* ���Ͷ�ȡ�Ĵ���״̬���� */
	SPI_FLASH_SendByte(W25X_ReadStatusReg);
	
	SPITimeout = SPIT_FLAG_TIMEOUT;
	
	/* ���FLASHæµ��ȴ� */
	do
	{
		FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);
		
		if((SPITimeout --) ==0)
		{
			SPI_TIMEOUT_UserCallback(4);
			return;
		}
	}while((FLASH_Status & WIP_Flag) == SET);     //����д���־
	
	SPI_FLASH_CS_HIGH();
}


/* ����ģʽ */
void SPI_FLASH_PowerDown(void)
{
	/* ͨѶ��ʼ CS�� */
	SPI_FLASH_CS_LOW();
	
	/* ���͵������� */
	SPI_FLASH_SendByte(W25X_PowerDown);
	
	/* ͨѶ���� CS�� */
	SPI_FLASH_CS_HIGH();
}


/* ����ģʽ */
void SPI_FLASH_WakeUp(void)
{
	/* ͨѶ��ʼ CS�� */
	SPI_FLASH_CS_LOW();
	
	/* ���͵������� */
	SPI_FLASH_SendByte(W25X_ReleasePowerDown);
	
	/* ͨѶ���� CS�� */
	SPI_FLASH_CS_HIGH();
}




/* ����FLASH���� SectorAddr:Ҫ������������ַ */
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
	/* ����FLASHдʹ������ */
	SPI_FLASH_WriteEnable();
	SPI_FLASH_WaitForWriteEnd();
	
	/* �������� */
	SPI_FLASH_CS_LOW();
	
	/* ���Ͳ���ָ�� */
	SPI_FLASH_SendByte(W25X_SectorErase);
	/* ���Ͳ���������ַ */
	SPI_FLASH_SendByte((SectorAddr & 0xFF0000)>>16);
	SPI_FLASH_SendByte((SectorAddr & 0xFF00)>>8);
	SPI_FLASH_SendByte(SectorAddr & 0xFF);
	
	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_WaitForWriteEnd();
}



/* ����FLASH��������Ƭ���� */
void SPI_FLASH_BulkErase(void)
{
	SPI_FLASH_WriteEnable();
	
	SPI_FLASH_CS_LOW();
	
	SPI_FLASH_SendByte(W25X_ChipErase);
	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_WaitForWriteEnd();
}




/* ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ������� pBuffer:Ҫд�����ݵ�ָ�� WriteAddr:д���ַ NumByteToWrite:д�����ݳ��ȣ�����С�ڵ���SPI_FLASH_PerWritePageSize */
void SPI_FLASH_PageWrite(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	/* ����FLASHдʹ�� */
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
	
	/* д������ */
	while(NumByteToWrite--)
	{
		/* ���͵�ǰҪд����ֽ����� */
		SPI_FLASH_SendByte(*pBuffer);
		/* ָ����һ�ֽ����� */
		pBuffer++;
	}
	SPI_FLASH_CS_HIGH();
	
	SPI_FLASH_WaitForWriteEnd();
}



/* ��FLASHд������ pBuffer:Ҫд�������ָ�� WriteAddr:Ҫд��ĵ�ַ NumByteToWrite:д�����ݳ��� */
void SPI_FLASH_BufferWrite(uint8_t *pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	uint8_t Addr=0,count=0,temp=0,NumOfSingle=0,NumOfPage=0;
	
	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
  Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
  count = SPI_FLASH_PageSize - Addr;	
	/*�����Ҫд��������ҳ*/
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	
	/* Addr=0 ��WriteAddr�պð�ҳ���� */
	if(Addr == 0)
	{
		/* NumByteToWrite < SPI_FLASH_PageSize */
		if(NumOfPage ==0)
		{
			SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumByteToWrite);
		}
		else /* NumByteToWrite > SPI_FLASH_PageSize */
		{
			/* �Ȱ�����ҳд�� */
			while(NumOfPage --)
			{
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,SPI_FLASH_PageSize);
				pBuffer += SPI_FLASH_PageSize;
				WriteAddr += SPI_FLASH_PageSize;
			}
			
			/* ���ж���Ĳ���һҳ������ д�� */
			SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumOfSingle);
		}
	}
	
	/* ����ַ��SPI_FLASH_PageSize������ */
	else
	{
		if(NumOfPage == 0)
		{
			/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��д����*/
			if(NumOfSingle>count)
			{
				temp = NumOfSingle - count;
				
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,count);
				WriteAddr += count;
				pBuffer += count;
				
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,temp);
			}
			else   /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
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
			
			/* ����ҳ��д�� */
			while(NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,SPI_FLASH_PageSize);
				pBuffer += SPI_FLASH_PageSize;
				WriteAddr += SPI_FLASH_PageSize;
			}
			
			/* ����ж���Ĳ���һҳ������ */
			if(NumOfSingle != 0)
			{
				SPI_FLASH_PageWrite(pBuffer,WriteAddr,NumOfSingle);
			}
		}
	}
	
}



/* ��ȡFLASH���� pBuffer:�洢�������ݵ�ָ�� ReadAddr:��ȡ��ַ NumByteToRead:��ȡ���ݳ��� */
void SPI_FLASH_BufferRead(uint8_t *pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)
{
	SPI_FLASH_CS_LOW();
	
	SPI_FLASH_SendByte(W25X_ReadData);
	
	SPI_FLASH_SendByte((ReadAddr&0xFF0000)>>16);
	SPI_FLASH_SendByte((ReadAddr&0xFF00)>>8);
	SPI_FLASH_SendByte(ReadAddr&0xFF);
	
	/* д������ */
	while(NumByteToRead--)
	{
		/* ���͵�ǰҪд����ֽ����� */
		*pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
		/* ָ����һ�ֽ����� */
		pBuffer++;
	}
	SPI_FLASH_CS_HIGH();

}


/* ��ȡFLASH ID */
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



/* ��ȡFLASH Device ID */
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



/* ��FLASH���Ͷ�ȡ���ݵ����� */
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
	/* �ȴ���ʱ��Ĵ������������Ϣ */
	FLASH_ERROR("SPI �ȴ���ʱ! errorCode = %d",errorCode);
	return 0;
}


