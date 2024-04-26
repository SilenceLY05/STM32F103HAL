#include "bsp_ad7192.h"
#include "bsp_usart.h"
#include <string.h>



uint32_t AD7192Registers[8] = {0,0,0,0,0,0,0,0};        //要读或要写AD7192Registers[8]
//AD7192Registers数组为什么定义成无符号32位整型呢？又为什么定义8个元素呢？
//因为数据寄存器的位数最多(数据寄存器)可以达到32位，以32位为准。除了通讯寄存器，可操作的寄存器为8个，所以定义8个元素，一个寄存器对应AD7192Registers数组的一个元素，互不干扰。
//8个寄存器见ReadFromAD7192ViaSPI()
uint32_t AD7192Data = 0;
volatile unsigned char Read_flag = 0;

SPI_HandleTypeDef    hspi_AD7192;



/* AD7192初始化 */
void AD7192_Init(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	
	AD7192_GPIO_CLK_ENABLE();
	AD7192_SPIx_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = AD7192_CS_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AD7192_CS_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = AD7192_MISO_PIN | AD7192_MOSI_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(AD7192_MISO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = AD7192_SCK_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(AD7192_SCK_PORT,&GPIO_InitStructure);
	
	
	/* SPI外设配置 */
	hspi_AD7192.Instance = AD7192_SPIx;
  hspi_AD7192.Init.Mode = SPI_MODE_MASTER;
  hspi_AD7192.Init.Direction = SPI_DIRECTION_2LINES;
  hspi_AD7192.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi_AD7192.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi_AD7192.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi_AD7192.Init.NSS = SPI_NSS_SOFT;
  hspi_AD7192.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi_AD7192.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi_AD7192.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi_AD7192.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi_AD7192.Init.CRCPolynomial = 7;
	HAL_SPI_Init(&hspi_AD7192);
	
	
	/* 片选使能 */
	AD7192_CS_ENABLE();
	
}




/* AD7192读写函数 WriteBuffer:需要写入数据的缓冲区 ReadBuffer:读出数据的缓冲区 NumberOfByte:数据大小(字节) */
void AD7192_ReadData(unsigned char *WriteBuffer,unsigned char *ReadBuffer,unsigned char NumberOfByte)
{
	HAL_SPI_TransmitReceive(&hspi_AD7192,WriteBuffer,ReadBuffer,NumberOfByte,1000);
}




/* 通过SPI向AD7192执行写入控制寄存器的操作函数 RegisterStarAddress:写入寄存器的起始地址 NumberOfRegisterToWrite:要写入寄存器的个数 DataBuffer:写入的值 OffsetInBuffer:没有用到 */
unsigned char WriteToAD7192ViaSPI(const unsigned char RegisterStarAddress,const unsigned char NumberOfRegisterToWrite,uint32_t *DataBuffer,unsigned char OffsetInBuffer)
{
	//形参包括要写入寄存器的起始地址RegisterStartAddress(取值范围是从0x00——0x07)，写入寄存器的个数，指向要写入AD7192寄存器的数组的指针(DataBuffer才是要写出的值其他是中间变量)，
	//const unsigned char OffsetInBuffer虽然定义但没有用到，主要是与通过SPI向AD7192读操作函数对称
	/*分别定义一个读缓存和写缓存，再把写缓存赋值，然后通过
		调用void ADuC7026SpiOperation(unsigned char* WriteBuffer, unsigned char *ReadBuffer, unsigned char NumberOfByte)来执行，将写缓存数据传送至AD7192*/
		
	unsigned char WriteBuffer[4];//定义unsigned char WriteBuf[4]数组，数组中的元素为无符号字符型，占一个字节
	unsigned char ReadBuffer[4]; //定义unsigned char ReadBuf[4]数组，数组中的元素为无符号字符型，占一个字节
	unsigned char i;
	
	
	for(i = 0;i<NumberOfRegisterToWrite;i++)         //DataBuffer通常指向AD7192Registers
	{
		WriteBuffer[0] = WEN | RW_W | ((RegisterStarAddress + i) << 3) | CREAD_DIS;     //无一例外，首先写入通信寄存器;8位数据;下一个操作是对指定寄存器执行写操作。
		WriteBuffer[1] = DataBuffer[RegisterStarAddress+i]>>16;                         //右移16位表示什么意思？DataBuffer是指向无符号长整型的数组指针，每个数组元素占4个字节(32位)，最高的8位无效，该语句是将16-23这8位二进制数赋给WriteBuf[1]。
		WriteBuffer[2] = DataBuffer[RegisterStarAddress + i]>>8;                        //右移8位表示什么意思？同理该语句是将8-15这8位二进制数赋给WriteBuf[2]。
		WriteBuffer[3] = DataBuffer[RegisterStarAddress +i];                            //同理该语句是将0-7这8位二进制数赋给WriteBuf[3]。
		
		AD7192_ReadData(WriteBuffer,ReadBuffer,4);                                      //看到了吧，最后还是通过调用STM32F103SpiOperation(WriteBuf, ReadBuf, 4)来完成写操作
	}
	
	return 0;
}


/* 通过SPI对AD7192执行一次读操作函数 RegisterStartAddress ：要读寄存器的起始地址(取值范围是从0x00——0x07) NumberOfRegistersToWrite ： 要读寄存器的个数 DataBuffer ： 要读入的值 OffsetInBuffer ： 缓存内偏移 */
unsigned char ReadFromAD7192ViaSPI(const unsigned char RegisterStarAddress,const unsigned char NumberOfRegistersToRead,uint32_t *DataBuffer,const unsigned char OffsetInBuffer)
{
	//形参包括要读寄存器的起始地址RegisterStartAddress(取值范围是从0x00——0x07)，要读取寄存器的个数，指向将读取AD7192寄存器数据存入的数组的指针(DataBuffer才是要读入的值其他是中间变量)，
	//一般指向AD7192Registers[8]，
	//const unsigned char OffsetInBuffer，字面意思是缓存内偏移，是指AD7192Registers[8]数组内部偏移，注意是数组哦，之前我们说过AD7192Registers[8]之所以定义8个元素，
	//一个寄存器对应AD7192Registers[8]数组的一个元素，互不干扰。
	
	unsigned char WriteBuffer[4] = {0,0,0,0};       //定义有4个元素的写缓存数组，每个数组元素占1个字节。
	unsigned char ReadBuffer[4] = {0,0,0,0};        //定义有4个元素的读缓存数组，每个数组元素占1个字节。
	unsigned char i;
	
	
	for(i=0;i<NumberOfRegistersToRead;i++)
	{
		WriteBuffer[0] = WEN | RW_R | ((RegisterStarAddress + i) << 3) | CREAD_DIS;  //写入通信寄存器;8位数据;下一个操作是对指定寄存器执行读操作。CREAD_DIS表示不使能连续读。
		
		//确定下一步进行寄存器读操作，那么写操作自然无效喽。
		AD7192_ReadData(WriteBuffer,ReadBuffer,1);           //首先通过写入通信寄存器来选定下一步要读取的寄存器,然后再将WriteBuf清空
		
		WriteBuffer[0] = NOP;
		WriteBuffer[1] = NOP;
		WriteBuffer[2] = NOP;
		WriteBuffer[3] = NOP;
		
		switch(RegisterStarAddress + i)
		{
			case REG_ID:                    //ID寄存器(0x04，8位寄存器)
			case REG_COM_STA:               //状态寄存器(0x00，8位寄存器)
			case REG_GPOCON:                //通用数字输出控制寄存器(0x05，8位寄存器)
				AD7192_ReadData(WriteBuffer,ReadBuffer,1);    //此3种情况是读取一个字节
				DataBuffer[OffsetInBuffer + i] = ReadBuffer[0];
			break;
			
			case REG_MODE:                  //模式寄存器(0x01，24位)
			case REG_CONF:                  //配置寄存器(0x02，24位)
			case REG_OFFSET:                //失调寄存器(0x06，24位)
			case REG_FS:                    //满量程寄存器(0x07，24位)
				AD7192_ReadData(WriteBuffer,ReadBuffer,3);         //此4种情况是读取3个字节
				DataBuffer[OffsetInBuffer +i] = ReadBuffer[0];
				DataBuffer[OffsetInBuffer +i] = (DataBuffer[OffsetInBuffer +i]<<8)+ReadBuffer[1];
				DataBuffer[OffsetInBuffer +i] = (DataBuffer[OffsetInBuffer +i]<<8)+ReadBuffer[2];
			break;
			
			
			case REG_DATA:              //数据寄存器(0x03，24位或32位) 
				if(AD7192Registers[REG_MODE]&DAT_STA_EN)       //多通道使能，将状态寄存器的内容附加到数据寄存器24位的数据上，所以是32位数据
				{
					AD7192_ReadData(WriteBuffer,ReadBuffer,4);    //所以此情况是读4个字节 
					DataBuffer[OffsetInBuffer +i] = ReadBuffer[0];
					DataBuffer[OffsetInBuffer +i] = (DataBuffer[OffsetInBuffer +i]<<8)+ReadBuffer[1];
					DataBuffer[OffsetInBuffer +i] = (DataBuffer[OffsetInBuffer +i]<<8)+ReadBuffer[2];
					DataBuffer[OffsetInBuffer +i] = (DataBuffer[OffsetInBuffer +i]<<8)+ReadBuffer[3];
					break;
				}
				else
				{
					AD7192_ReadData(WriteBuffer,ReadBuffer,3);         
					DataBuffer[OffsetInBuffer +i] = ReadBuffer[0];
					DataBuffer[OffsetInBuffer +i] = (DataBuffer[OffsetInBuffer +i]<<8)+ReadBuffer[1];
					DataBuffer[OffsetInBuffer +i] = (DataBuffer[OffsetInBuffer +i]<<8)+ReadBuffer[2];
					break;
				}
				
			default:
				break;
		}
	}
	return 0;
}


/* AD7192模式和配置寄存器配置 mode:模式寄存器地址 cfg:配置寄存器地址 */
void AD7192_ModeCfgReg(uint32_t mode,uint32_t cfg)
{
	/* 更新配置 */
	AD7192Registers[REG_CONF] = cfg;
	AD7192Registers[REG_MODE] = mode;
	
	/* 写入配置 */
	WriteToAD7192ViaSPI(REG_MODE,2,AD7192Registers,REG_MODE);      //连续写模式寄存器和配置寄存器
}




/* AD7192软件复位 */
void AD7192SoftWareReset(void)
{
	
	//AD7192软件复位函数；软件复位的原理，通过执行一个占用至少40个串行时钟周期的写操作，并式DIN处去高电平状态。
	
	 /*分别定义一个读缓存和写缓存，再把写缓存赋值，然后通过
	调用void ADuC7026SpiOperation(unsigned char* WriteBuffer, unsigned char *ReadBuffer, unsigned char NumberOfByte)来执行，将写缓存数据传送至AD7192*/
	
	unsigned char WriteBuffer[1];   //写缓存
	unsigned char ReadBuffer[1];    //读缓存
	unsigned char i;
	
	WriteBuffer[0] = 0xFF;
	
	for(i=0;i<10;i++)    //调用STM32F103SpiOperation()来实现数据传输，向AD7192的通信寄存器写40个1；0xFF是8个1，循环5次一共是40个1。
	{
		AD7192_ReadData(WriteBuffer,ReadBuffer,1);
	}
	AD7192_SCLK_H;
}




/* AD7192内部零电平校准 */
void AD7192InternalZeroScaleCalibration(void)
{
	AD7192Registers[REG_MODE] = 0;
	
	ReadFromAD7192ViaSPI(REG_MODE,1,AD7192Registers,REG_MODE);     // 读模式寄存器
	
	AD7192Registers[REG_MODE] &= ~0xE00000;                        // 清除模式位
	AD7192Registers[REG_MODE] |= MODE_INZCL;                       // 设置内部零电平校准
	
	WriteToAD7192ViaSPI(REG_MODE,1,AD7192Registers,REG_MODE);      // 写入模式寄存器
	
	do
	{
		ReadFromAD7192ViaSPI(REG_COM_STA,1,AD7192Registers,REG_COM_STA);  // 读状态寄存器
	}while((AD7192Registers[REG_COM_STA]&RDY_H) != 0);             // 等待校准完成
}



/* AD7192内部满电平校准 */
void AD7192InternalFullScaleCalibration(void)
{
	AD7192Registers[REG_MODE] = 0;
	
	ReadFromAD7192ViaSPI(REG_MODE,1,AD7192Registers,REG_MODE);     // 读模式寄存器
	
	AD7192Registers[REG_MODE] &= ~0xE00000;                        // 清除模式位
	AD7192Registers[REG_MODE] |= MODE_INFCL;                       // 设置内部满电平校准
	
	WriteToAD7192ViaSPI(REG_MODE,1,AD7192Registers,REG_MODE);      // 写入模式寄存器
	
	do
	{
		ReadFromAD7192ViaSPI(REG_COM_STA,1,AD7192Registers,REG_COM_STA);  // 读状态寄存器
	}while((AD7192Registers[REG_COM_STA]&RDY_H) != 0);             // 等待校准完成
}



/* AD7192系统零电平校准 */
void AD7192ExternalZeroScaleCalibration(void)
{
	AD7192Registers[REG_MODE] = 0;
	AD7192Registers[REG_CONF] = 0;
	AD7192Registers[REG_CONF] = CHOP_DIS | REF_IN1 |AIN1_AIN2 |BURN_DIS | REFDET_DIS |BUF_DIS | UB_BI | GAIN_1;
	WriteToAD7192ViaSPI(REG_CONF,1,AD7192Registers,REG_CONF);
	
	AD7192Registers[REG_MODE] = 0;
	AD7192Registers[REG_MODE] = MODE_SYSZCL | DAT_STA_EN | EXT_XTAL | SINC_4 | ENPAR_EN | CLK_DIV_DIS | SINGLECYCLE_DIS |REJ60_DIS | 0x080;
	WriteToAD7192ViaSPI(REG_MODE,1,AD7192Registers,REG_MODE);
}




/* AD7192系统满电平校准 */
void AD7192ExternalFUllScaleCalibration(void)
{
	AD7192Registers[REG_MODE] = 0;
	AD7192Registers[REG_CONF] = 0;
	AD7192Registers[REG_CONF] = CHOP_DIS | REF_IN1 |AIN1_AIN2 |BURN_DIS | REFDET_DIS |BUF_DIS | UB_BI | GAIN_1;
	WriteToAD7192ViaSPI(REG_CONF,1,AD7192Registers,REG_CONF);
	
	AD7192Registers[REG_MODE] = 0;
	AD7192Registers[REG_MODE] = MODE_SYSFCL | DAT_STA_EN | EXT_XTAL | SINC_4 | ENPAR_EN | CLK_DIV_2 | SINGLECYCLE_DIS |REJ60_DIS | 0x080;
	WriteToAD7192ViaSPI(REG_MODE,1,AD7192Registers,REG_MODE);
}



/* AD7192读取转换数据函数 DataBuffer:转换结果的值 */
uint32_t AD7192ReadConvertingData(void)
{
	unsigned char WriteBuffer[4];
	unsigned char ReadBuffer[4];
	unsigned long int DataBuffer;
	
	//AD7192_CS_ENABLE() 片选信号使能，选通AD7192
	WriteBuffer[0] = WEN |RW_R |(REG_DATA << 3) | CREAD_DIS;
	
	AD7192_ReadData(WriteBuffer,ReadBuffer,1);   //首先写入通信寄存器，确认下一步操作是读取数据寄存器
	
	WriteBuffer[0] = NOP;
	WriteBuffer[1] = NOP;
	WriteBuffer[2] = NOP;
	WriteBuffer[3] = NOP;
	
	while(HAL_GPIO_ReadPin(AD7192_MISO_PORT,AD7192_MISO_PIN) == 0){}
	while(HAL_GPIO_ReadPin(AD7192_MISO_PORT,AD7192_MISO_PIN) == 1){}    //waiting the 1st RDY failling edge;即DOUT变为低电平。
		
	
	if((AD7192Registers[REG_MODE] &DAT_STA_EN) == DAT_STA_EN)
	{
		AD7192_ReadData(WriteBuffer,ReadBuffer,4);
		
		DataBuffer = ReadBuffer[0];
		DataBuffer = (DataBuffer<<8)+ReadBuffer[1];
		DataBuffer = (DataBuffer<<8)+ReadBuffer[2];
		DataBuffer = (DataBuffer<<8)+ReadBuffer[3];
	}
	else
	{
		AD7192_ReadData(WriteBuffer,ReadBuffer,3);
		
		DataBuffer = ReadBuffer[0];
		DataBuffer = (DataBuffer<<8)+ReadBuffer[1];
		DataBuffer = (DataBuffer<<8)+ReadBuffer[2];
	}
	return DataBuffer;
}



/* Ad7192启动连续读函数 */
void AD7192StartContinuousRead(void)
{
	unsigned char WriteBuffer[1];
	unsigned char ReadBuffer[1];
	
	WriteBuffer[0] = WEN | RW_R | (REG_DATA << 3) | CREAD_EN;
	AD7192_ReadData(WriteBuffer,ReadBuffer,1);
}





/* AD7192连续读函数 */
uint32_t AD7192ContinuousRead(void)
{
	unsigned char WriteBuffer[4];
	unsigned char ReadBuffer[4];
	unsigned long int DataBuffer;

	
	WriteBuffer[0] = NOP;
	WriteBuffer[1] = NOP;
	WriteBuffer[2] = NOP;
	WriteBuffer[3] = NOP;
	
	while(HAL_GPIO_ReadPin(AD7192_MISO_PORT,AD7192_MISO_PIN) == 0){}
	while(HAL_GPIO_ReadPin(AD7192_MISO_PORT,AD7192_MISO_PIN) == 1){}    //waiting the 1st RDY failling edge;即DOUT变为低电平。
		
	
	if((AD7192Registers[REG_MODE] &DAT_STA_EN) == DAT_STA_EN)
	{
		AD7192_ReadData(WriteBuffer,ReadBuffer,4);
		
		DataBuffer = ReadBuffer[0];
		DataBuffer = (DataBuffer<<8)+ReadBuffer[1];
		DataBuffer = (DataBuffer<<8)+ReadBuffer[2];
		DataBuffer = (DataBuffer<<8)+ReadBuffer[3];
	}
	else
	{
		AD7192_ReadData(WriteBuffer,ReadBuffer,3);
		
		DataBuffer = ReadBuffer[0];
		DataBuffer = (DataBuffer<<8)+ReadBuffer[1];
		DataBuffer = (DataBuffer<<8)+ReadBuffer[2];
	}
	return DataBuffer;
}



/* AD7192退出连续读取函数 */
void AD7192ExitContinuousRead(void)
{
	unsigned char WriteBuffer[1];
	unsigned char ReadBuffer[1];
	
	while(HAL_GPIO_ReadPin(AD7192_MISO_PORT,AD7192_MISO_PIN) == 0){}
	while(HAL_GPIO_ReadPin(AD7192_MISO_PORT,AD7192_MISO_PIN) == 1){}    //waiting the 1st RDY failling edge;即DOUT变为低电平。
		
	
	//AD7192_CS_ENABLE() 片选信号使能，选通AD7192
	WriteBuffer[0] = WEN |RW_R |(REG_DATA << 3) | CREAD_DIS;
	
	AD7192_ReadData(WriteBuffer,ReadBuffer,1);   //首先写入通信寄存器，确认下一步操作是读取数据寄存器
	
}




/* AD7192读取温度 */
uint32_t AD7192ReadTemperature(void)
{
	uint32_t reg_data = 0;
	
	ReadFromAD7192ViaSPI(REG_MODE,2,AD7192Registers,REG_MODE);       // 读模式和状态寄存器寄存器
	
	AD7192Registers[REG_MODE] &= ~0xE00000;                          // 清除模式位
	AD7192Registers[REG_MODE] |= MODE_SING;                          // 设置为单次转换模式
	
	AD7192Registers[REG_CONF] = TEMP;                                // 设置温度转换通道
	
	WriteToAD7192ViaSPI(REG_MODE,2,AD7192Registers,REG_MODE);        // 写入模式寄存器
	
	reg_data = AD7192ReadConvertingData();
	
	return reg_data;
}




/* 读取的温度值计算函数 TemperatureCode:计算结果 */
float RealTemperature(uint32_t TemperatureCode)
{
	float temp = 0.0;
	temp = (TemperatureCode - 0x800000) / 2815.0 - 273;
	return temp;
}



/* AD7192启动单次转换 Channels:要启动的通道 */
void AD7192StartSingleConvertion(uint32_t Channels)
{
	ReadFromAD7192ViaSPI(REG_MODE,2,AD7192Registers,REG_MODE);     // 读模式和状态寄存器寄存器
	
	AD7192Registers[REG_MODE] &= ~0xE00000;                        // 清除模式位
	AD7192Registers[REG_MODE] |= MODE_SING;                        // 设置为单次转换模式

	AD7192Registers[REG_CONF] &= ~0xFF00;                          // 清除通道选择位
	AD7192Registers[REG_CONF] |= Channels;                         // 设置要转换的通道
	
	WriteToAD7192ViaSPI(REG_MODE,2,AD7192Registers,REG_MODE);      // 写入模式寄存器
}




/* AD7192启动连续转换 Channels:要启动的通道 */
void AD7192StartContinuousConvertion(uint32_t Channels)
{
	AD7192Registers[REG_MODE] = 0;
	ReadFromAD7192ViaSPI(REG_MODE,2,AD7192Registers,REG_MODE);     // 读模式和状态寄存器寄存器
	
	AD7192Registers[REG_MODE] &= ~0xE00000;                        // 清除模式位
	AD7192Registers[REG_MODE] |= MODE_CONT;                        // 设置为单次转换模式

	AD7192Registers[REG_CONF] &= ~0xFF00;                          // 清除通道选择位
	AD7192Registers[REG_CONF] |= Channels;                         // 设置要转换的通道
	
	WriteToAD7192ViaSPI(REG_MODE,2,AD7192Registers,REG_MODE);      // 写入模式寄存器
}











