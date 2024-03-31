#include "bsp_i2c_gpio.h"

static void i2c_Delay(void)
{
	uint8_t i;
	
	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for(i=0;i<10;i++);
	
	
}


/* CPU发起I2C总线启动信号 */
void i2c_Start(void)
{
	/* 当SCL高电平时，SDA出现一个跳沿表示I2C总线启动信号 */
	EEPROM_I2C_SDA_1();
	EEPROM_I2C_SCL_1();
	
	i2c_Delay();
	EEPROM_I2C_SDA_0();
	i2c_Delay();
	EEPROM_I2C_SCL_0();
	i2c_Delay();
}



/* CPU发起I2C总线停止信号 */
void i2c_Stop(void)
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	EEPROM_I2C_SDA_0();
	EEPROM_I2C_SCL_1();
	i2c_Delay();
	EEPROM_I2C_SDA_1();
}



/* CPU向I2C总线设备发送8Bit数据 */
void i2c_SendByte(uint8_t __ucByte)
{
	uint8_t i;
	
	/* 先发送字节的高位7Bit */
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
			EEPROM_I2C_SDA_1();   //释放总线
		}
		__ucByte <<= 1;         //左移一个Bit
		i2c_Delay();
	}
}


/* CPU从I2C总线设备读取8Bit数据 */
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



/* CPU产生一个时钟，并读取器件的ACK应答信号 */
uint8_t i2c_WaitAck(void)
{
	uint8_t re;
	
	EEPROM_I2C_SDA_1();  /* CPU释放SDA总线 */
	i2c_Delay();
	EEPROM_I2C_SCL_1();  /* CPU驱动SDA=1，此时器件会返回ACK应答 */
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



/* CPU产生一个ACK信号 */
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


/* CPU产生一个NACK信号 */
void i2c_NAck(void)
{
	EEPROM_I2C_SDA_1();
	i2c_Delay();
	EEPROM_I2C_SCL_1();
	i2c_Delay();
	EEPROM_I2C_SCL_0();
	i2c_Delay();
}



/* 配置I2C总线的GPIO，采用模拟IO的方式实现 */
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



