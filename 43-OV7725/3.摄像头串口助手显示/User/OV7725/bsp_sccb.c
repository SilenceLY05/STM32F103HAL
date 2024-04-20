#include "bsp_sccb.h"

#define DEV_ADR     ADDR_OV7725


/* 配置SCCB的GPIO */
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


/* CPU发起SCCB总线启动信号 */
static int SCCB_Start(void)
{
	/* 当SCL高电平时，SDA出现一个跳沿表示I2C总线启动信号 */
	SDA_H;
	SCL_H;
	
	SCCB_Delay();
	if(!SDA_read)
		return DISABLE;    /* SDA为低电平则总线忙，退出 */
	SDA_L;
	SCCB_Delay();
	if(SDA_read)
		return DISABLE;
	SCL_L;
	SCCB_Delay();
	return ENABLE;
}



/* CPU发起SCCB总线停止信号 */
static int SCCB_Stop(void)
{
	SCL_L;
	SCCB_Delay();
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	SDA_L;
	SCCB_Delay();
	
	SCL_H;
	SCCB_Delay();
	SDA_H;
	SCCB_Delay();
}


/* CPU产生一个ACK信号 */
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


/* CPU产生一个NACK信号 */
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



/* CPU产生一个时钟，并读取器件的ACK应答信号 */
static int SCCB_WaitAck(void)
{
	SCL_L;
	SCCB_Delay();
	SDA_H;  /* CPU释放SDA总线 */
	SCCB_Delay();
	SCL_H;  /* CPU驱动SDA=1，此时器件会返回ACK应答 */
	SCCB_Delay();
	if(SDA_read)
	{
		SCL_L;
		return DISABLE;
		
	}
	
	SCL_L;
	return ENABLE;
	
}





/* CPU向I2C总线设备发送8Bit数据 */
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


/* CPU从I2C总线设备读取8Bit数据 */
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


/* 写一节数据 WriteAddress:待写入地址 SendByte:待写入数据 DeviceAddress:器件类型 */
int SCCB_WriteByte(uint16_t WriteAddress,uint8_t SendByte)
{
	if(!SCCB_Start())
	{
		return DISABLE;
	}
	
	SCCB_SendByte(DEV_ADR);                 /* 器件地址 */
	if(!SCCB_WaitAck())
	{
		SCCB_Stop();
		return DISABLE;
	}
	
	SCCB_SendByte((uint8_t)(WriteAddress&0x00FF));          /* 设置低起始地址 */
	SCCB_WaitAck();
	SCCB_SendByte(SendByte);
	SCCB_WaitAck();
	SCCB_Stop();
	return ENABLE;
}


/* 读取一串数据 pBuffer:存放读出数据 length:待读出长度 ReadAddress:待读出地址 DeviceAddress:期间类型 */
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











