#include "bsp_i2c_ee.h"
#include "bsp_usart.h"
#include "bsp_i2c_gpio.h"


/* ���i2c�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ���� */
uint8_t ee_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;
	i2c_Start();
	
	/* �����豸��ַ+��д����bit */
	i2c_SendByte(_Address | EEPROM_I2C_WR);
	ucAck = i2c_WaitAck();       /* ����豸��ACKӦ�� */
	
	i2c_Stop();                  /* ����ֹͣ�ź� */
	i2c_NAck();                  /*��������Ƕ���ַ����Ҫ������Ӧ���ź�*/
	
	return ucAck;
}




/* �ȴ�EEPROM��׼��״̬����д�����ݺ󣬱�����ñ����� 
	 д�����ʱ��ʹ��I2C�����ݴ��䵽EEPROM��
	 EEPROM�����ڲ��ռ�д��������Ҫһ����ʱ�䣬
	 ��EEPROM�ڲ�д����ɺ���I2C���豸Ѱַ����Ӧ��
	 ���ñ������ɵȴ���EEPROM�ڲ�ʱ��д����� */
uint8_t ee_WaitStandby(void)
{
	uint32_t wait_count= 0;
	
	while(ee_CheckDevice(EEPROM_DEV_ADDR))
	{
		//����ⳬ���������˳�ѭ��
		if(wait_count++> 0xFFFF)
		{
			return 1;
		}
		
	}
	return 0;
}


/* �Ӵ���EEPROMָ����ַ����ʼ��ȡ�������� _usAddress:��ʼ��ַ _usSize:���ݳ��ȣ����ֽ�Ϊ��λ _pReadBuff:��Ŷ�ȡ�������ݵĻ�����ָ�� */
uint8_t ee_ReadBytes(uint8_t *_pReadBuff,uint16_t _usAddress,uint16_t _usSize)
{
	uint16_t i;
	
	/* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */
	
	/* ��1��������I2C���������ź� */
	i2c_Start();
	
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);       /* �˴���дָ�� */
	
	/* ��3�����ȴ�ACK */
	if(i2c_WaitAck() != 0)
	{
		goto cmd_fail;      /* EEPROM������Ӧ�� */
	}
	
	/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
	i2c_SendByte((uint8_t)_usAddress);
	
	/* ��5�����ȴ�ACK */
	if(i2c_WaitAck()!=0)
	{
		goto cmd_fail;      /* EEPROM������Ӧ�� */
	}
	
	/* ��6������������I2C���ߡ�ǰ��Ĵ����Ŀ����EEPROM���͵�ַ�����濪ʼ��ȡ���� */
	i2c_Start();
	
	
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_RD);      /* �˴��Ƕ�ָ�� */
	
	/* ��8��������ACK */
	if(i2c_WaitAck() != 0)      /* EEPROM������Ӧ�� */
	{
		goto cmd_fail;       
	} 
	
	/* ��9����ѭ����ȡ���� */
	for(i=0;i<_usSize;i++)
	{
		_pReadBuff[i] = i2c_ReadByte();     /* ��1���ֽ� */
		
		/* ÿ����1���ֽں���Ҫ����Ack�� ���һ���ֽڲ���ҪAck����Nack */
		if(i!= _usSize -1)
		{
			i2c_Ack();         /* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
		}
		else
		{
			i2c_NAck();        /* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		}
	}
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	
	return 1;
	
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	
	return 0;
}



/* ����EEPROMָ����ַд���������ݣ�����ҳд�����д��Ч�� _usAddress:��ʼ��ַ _usSize:д�����ݳ��� _pWriteBuff:��Ŷ��������ݵĻ�����ָ�� */
uint8_t ee_WriteByte(uint8_t *_pWriteBuff,uint16_t _usAddress,uint16_t _usSize)
{
	uint16_t i,m;
	uint16_t usAddr;
	
	/* 
		д����EEPROM�������������������ȡ�ܶ��ֽڣ�ÿ��д����ֻ����ͬһ��page��
		����24xx02��page size = 8
		�򵥵Ĵ�����Ϊ�����ֽ�д����ģʽ��ÿд1���ֽڣ������͵�ַ
		Ϊ���������д��Ч��: ����������page wirte������
	*/
	usAddr = _usAddress;
	for(i=0;i<_usSize;i++)
	{
		/* �����͵�1���ֽڻ���ҳ���׵�ַʱ����Ҫ���·��������źź͵�ַ */
		if((i == 0) || (usAddr & (EEPROM_PAGE_SIZE - 1)) == 0)
		{
			/*���ڣ�������ֹͣ�źţ������ڲ�д������*/
			i2c_Stop();
			
			/* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����,һ��С�� 10ms 	
			
			  CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������	
			  ԭ��ͬ ee_WaitStandby ���������ú��������ɺ�����ֹͣ�źţ��������ڴ˴�				
			*/
			for(m=0;m<1000;m++)
			{
				/* ��1��������I2C���������ź� */
				i2c_Start();
				
				/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
				i2c_SendByte(EEPROM_DEV_ADDR | EEPROM_I2C_WR);   /* �˴���дָ�� */
				
				/* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
				if(i2c_WaitAck() == 0)
				{
					break;
				}
			}
			if(m == 1000)
			{
				goto cmd_fail;         /* EEPROM����д��ʱ */
			}
			
			/* ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ������24C04���ϣ���ô�˴���Ҫ���������ַ */
			i2c_SendByte((uint8_t)usAddr);
			
			/* ��5�����ȴ�ACK */
			if(i2c_WaitAck() != 0)
			{
				goto cmd_fail;       /* EEPROM������Ӧ�� */
			}
		}
		
		/* ��6������ʼд������ */
		i2c_SendByte(_pWriteBuff[i]);
		
		/* ��7��������ACK */
		if(i2c_WaitAck() != 0)
		{
			goto cmd_fail;  /* EEPROM������Ӧ�� */
		}
		usAddr++;    /* ��ַ��1 */		
	}
	
	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
	i2c_Stop();
	
	//�ȴ����һ��EEPROM�ڲ�д�����
	if(ee_WaitStandby() == 1)   //����1��ʾ��ʱ
		goto cmd_fail;
	
	return 1;
	
cmd_fail:/* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 0;
}



void ee_Erase(void)
{
	uint16_t i;
	uint8_t buff[EEPROM_SIZE];
	
	/* ��仺���� */
	for(i=0;i<EEPROM_SIZE;i++)
	{
		buff[i] = 0xFF;
	}
	
	/* дEEPROM, ��ʼ��ַ = 0�����ݳ���Ϊ 256 */
	if(ee_WriteByte(buff,0,EEPROM_SIZE) == 0)
	{
		printf("\r\n ����eeprom���� \r\n");
		return;
	}
	else
	{
		printf("����eeprom�ɹ���\r\n");
	}
}


/* eeprom AT24C02 ��д���� */
uint8_t ee_Test(void)
{
	uint16_t i;
	uint8_t write_buff[EEPROM_SIZE];
	uint8_t read_buff[EEPROM_SIZE];
	
	if(ee_CheckDevice(EEPROM_DEV_ADDR) == 1)
	{
		/* û�м�⵽EEPROM */
		printf("\r\n û�м�⵽����EEPROM \r\n");
		
		return 0;
	}
	
	
	/* �����Ի����� */
	for(i=0;i<EEPROM_SIZE;i++)
	{
		write_buff[i] = i;
	}
	
	if(ee_WriteByte(write_buff,0,EEPROM_SIZE) == 0)
	{
		printf("\r\n дEEPROM���� \r\n");
		return 0;
	}
	else
	{
		printf("\r\n дEEPROM�ɹ��� \r\n");
	}
	
	if(ee_ReadBytes(read_buff,0,EEPROM_SIZE) == 0)
	{
		printf("\r\n ��EERPOM���� \r\n");
		return 0;
	}
	else
	{
		printf("\r\n ��EEPROM�ɹ����������£� \r\n");
	}
	
	for(i=0;i<EEPROM_SIZE;i++)
	{
		if(read_buff[i] != write_buff[i])
		{
			printf("\r\n 0x%02x \r\n",read_buff[i]);
			printf("\r\n ����EEPROM������д������ݲ�һ�� \r\n");
			return 0;
		}
		printf("\r\n %02x \r\n",read_buff[i]);
		
		if((i&15) == 15)
		{
			printf("\r\n");
		}
	}
	printf("\r\n eeprom��д���Գɹ��� \r\n");
	
	return 1;
}



