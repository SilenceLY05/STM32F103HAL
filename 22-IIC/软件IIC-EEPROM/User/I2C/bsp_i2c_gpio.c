#include "bsp_i2c_gpio.h"

static void i2c_Delay(void)
{
	uint8_t i;
	
	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	for(i=0;i<10;i++);
	
	
}


/* CPU����I2C���������ź� */
void i2c_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�����ر�ʾI2C���������ź� */
	EEPROM_I2C_SDA_1();
	EEPROM_I2C_SCL_1();
	
	i2c_Delay();
	EEPROM_I2C_SDA_0();
	i2c_Delay();
	EEPROM_I2C_SCL_0();
	i2c_Delay();
}



/* CPU����I2C����ֹͣ�ź� */
void i2c_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	EEPROM_I2C_SDA_0();
	EEPROM_I2C_SCL_1();
	i2c_Delay();
	EEPROM_I2C_SDA_1();
}



/* CPU��I2C�����豸����8Bit���� */
void i2c_SendByte(uint8_t __ucByte)
{
	uint8_t i;
	
	/* �ȷ����ֽڵĸ�λ7Bit */
	for(i=0;i<8;i++)
	{
		if(__ucByte & 0x80)
		{
			EEPROM_I2C_SDA_1();
		}
		else
		{
			EEPROM_I2C_SDA_0();
		}
		i2c_Delay();
		EEPROM_I2C_SCL_1();
		i2c_Delay();
		EEPROM_I2C_SCL_0();
		if(i==7)
		{
			EEPROM_I2C_SDA_1();   //�ͷ�����
		}
		__ucByte <<= 1;         //����һ��Bit
		i2c_Delay();
	}
}


/* CPU��I2C�����豸��ȡ8Bit���� */
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;
	
	value = 0;
	for(i=0;i<8;i++)
	{
		value <<= 1;
		EEPROM_I2C_SCL_1();
		i2c_Delay();
		if(EEPROM_I2C_SDA_READ())
		{
			value++;
		}
		EEPROM_I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}



/* CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź� */
uint8_t i2c_WaitAck(void)
{
	uint8_t re;
	
	EEPROM_I2C_SDA_1();  /* CPU�ͷ�SDA���� */
	i2c_Delay();
	EEPROM_I2C_SCL_1();  /* CPU����SDA=1����ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	if(EEPROM_I2C_SDA_READ())
	{
		re = 1;
		
	}
	else
	{
		re = 0;
	}
	EEPROM_I2C_SCL_0();
	i2c_Delay();
	return re;
	
}



/* CPU����һ��ACK�ź� */
void i2c_Ack(void)
{
	EEPROM_I2C_SDA_0();
	i2c_Delay();
	EEPROM_I2C_SCL_1();
	i2c_Delay();
	EEPROM_I2C_SCL_0();
	i2c_Delay();
	EEPROM_I2C_SDA_1();
}


/* CPU����һ��NACK�ź� */
void i2c_NAck(void)
{
	EEPROM_I2C_SDA_1();
	i2c_Delay();
	EEPROM_I2C_SCL_1();
	i2c_Delay();
	EEPROM_I2C_SCL_0();
	i2c_Delay();
}



/* ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ�� */
void i2c_CfgGpio(void)
{
	GPIO_InitTypeDef       GPIO_InitStructure;
	I2Cx_SCL_GPIO_CLK_ENABLE();
	I2Cx_SDA_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = EEPROM_I2C_SCL_PIN | EEPROM_I2C_SDA_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	
	HAL_GPIO_Init(EEPROM_GPIO_PORT_I2C,&GPIO_InitStructure);
	
	i2c_Stop();
}



