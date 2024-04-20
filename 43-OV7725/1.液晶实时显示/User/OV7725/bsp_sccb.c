#include "bsp_sccb.h"

#define DEV_ADR     ADDR_OV7725


/* ����SCCB��GPIO */
void SCCB_GPIO_Config(void)
{
	GPIO_InitTypeDef       GPIO_InitStructure;
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = GPIO_PIN_7;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	
	GPIO_InitStructure.Pin = GPIO_PIN_6;
	
	HAL_GPIO_Init(GPIOC,&GPIO_InitStructure);
	

}


static void SCCB_Delay(void)
{
	uint16_t i = 400;
	

	while(i)
	{
		i--;
	}
	
}


/* CPU����SCCB���������ź� */
static int SCCB_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�����ر�ʾI2C���������ź� */
	SDA_H;
	SCL_H;
	
	SCCB_Delay();
	if(!SDA_read)
		return DISABLE;    /* SDAΪ�͵�ƽ������æ���˳� */
	SDA_L;
	SCCB_Delay();
	if(SDA_read)
		return DISABLE;
	SCL_L;
	SCCB_Delay();
	return ENABLE;
}



/* CPU����SCCB����ֹͣ�ź� */
static int SCCB_Stop(void)
{
	SCL_L;
	SCCB_Delay();
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	SDA_L;
	SCCB_Delay();
	
	SCL_H;
	SCCB_Delay();
	SDA_H;
	SCCB_Delay();
}


/* CPU����һ��ACK�ź� */
static void SCCB_Ack(void)
{
	SCL_L;
	SCCB_Delay();
	SDA_L;
	SCCB_Delay();
	SCL_H;
	SCCB_Delay();
	SCL_L;
	SCCB_Delay();

}


/* CPU����һ��NACK�ź� */
static void SCCB_NAck(void)
{
	SCL_L;
	SCCB_Delay();
	SDA_H;
	SCCB_Delay();
	SCL_H;
	SCCB_Delay();
	SCL_L;
	SCCB_Delay();
}



/* CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź� */
static int SCCB_WaitAck(void)
{
	SCL_L;
	SCCB_Delay();
	SDA_H;  /* CPU�ͷ�SDA���� */
	SCCB_Delay();
	SCL_H;  /* CPU����SDA=1����ʱ�����᷵��ACKӦ�� */
	SCCB_Delay();
	if(SDA_read)
	{
		SCL_L;
		return DISABLE;
		
	}
	
	SCL_L;
	return ENABLE;
	
}





/* CPU��I2C�����豸����8Bit���� */
static void SCCB_SendByte(uint8_t SendByte)
{
	uint8_t i=8;
	
	while(i--)
	{
		SCL_L;
		SCCB_Delay();
		if(SendByte&0x80)
		{
			SDA_H;
		}
		else
		{
			SDA_L;
			SendByte<<=1;
			SCL_H;
			SCCB_Delay();
		}
	}
	SCL_L;
}


/* CPU��I2C�����豸��ȡ8Bit���� */
static int SCCB_ReceiveByte(void)
{
	uint8_t i=8;
	uint8_t ReceiveByte=0;
	
	SDA_H;
	while(i--)
	{
		ReceiveByte <<= 1;
		SCL_L;
		SCCB_Delay();
		SCL_H;
		SCCB_Delay();
		if(SDA_read)
		{
			ReceiveByte|=0x01;
		}
		
	}
	SCL_L;
	return ReceiveByte;
}


/* дһ������ WriteAddress:��д���ַ SendByte:��д������ DeviceAddress:�������� */
int SCCB_WriteByte(uint16_t WriteAddress,uint8_t SendByte)
{
	if(!SCCB_Start())
	{
		return DISABLE;
	}
	
	SCCB_SendByte(DEV_ADR);                 /* ������ַ */
	if(!SCCB_WaitAck())
	{
		SCCB_Stop();
		return DISABLE;
	}
	
	SCCB_SendByte((uint8_t)(WriteAddress&0x00FF));          /* ���õ���ʼ��ַ */
	SCCB_WaitAck();
	SCCB_SendByte(SendByte);
	SCCB_WaitAck();
	SCCB_Stop();
	return ENABLE;
}


/* ��ȡһ������ pBuffer:��Ŷ������� length:���������� ReadAddress:��������ַ DeviceAddress:�ڼ����� */
int SCCB_ReadByte(uint8_t *pBuffer,uint16_t length,uint8_t ReadAddress)
{
	if(!SCCB_Start())
	{
		return DISABLE;
	}
	SCCB_SendByte(DEV_ADR);
	if(!SCCB_WaitAck())
	{
		SCCB_Stop();
		return DISABLE;
	}
	SCCB_SendByte(ReadAddress);
	SCCB_WaitAck();
	SCCB_Stop();
	
	if(!SCCB_Start())
	{
		return DISABLE;
	}
	SCCB_SendByte(DEV_ADR+1);
	if(!SCCB_WaitAck())
	{
		SCCB_Stop();
		return DISABLE;
	}
	while(length)
	{
		*pBuffer= SCCB_ReceiveByte();
		if(length == 1)
		{
			SCCB_NAck();
		}
		else
		{
			SCCB_Ack();
		}
		pBuffer++;
		length--;
	}
	SCCB_Stop();
	return ENABLE;
}











