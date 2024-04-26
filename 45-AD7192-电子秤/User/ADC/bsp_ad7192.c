#include "bsp_ad7192.h"
#include "bsp_usart.h"
#include <string.h>



uint32_t AD7192Registers[8] = {0,0,0,0,0,0,0,0};        //Ҫ����ҪдAD7192Registers[8]
//AD7192Registers����Ϊʲô������޷���32λ�����أ���Ϊʲô����8��Ԫ���أ�
//��Ϊ���ݼĴ�����λ�����(���ݼĴ���)���Դﵽ32λ����32λΪ׼������ͨѶ�Ĵ������ɲ����ļĴ���Ϊ8�������Զ���8��Ԫ�أ�һ���Ĵ�����ӦAD7192Registers�����һ��Ԫ�أ��������š�
//8���Ĵ�����ReadFromAD7192ViaSPI()
uint32_t AD7192Data = 0;
volatile unsigned char Read_flag = 0;

SPI_HandleTypeDef    hspi_AD7192;



/* AD7192��ʼ�� */
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
	
	
	/* SPI�������� */
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
	
	
	/* Ƭѡʹ�� */
	AD7192_CS_ENABLE();
	
}




/* AD7192��д���� WriteBuffer:��Ҫд�����ݵĻ����� ReadBuffer:�������ݵĻ����� NumberOfByte:���ݴ�С(�ֽ�) */
void AD7192_ReadData(unsigned char *WriteBuffer,unsigned char *ReadBuffer,unsigned char NumberOfByte)
{
	HAL_SPI_TransmitReceive(&hspi_AD7192,WriteBuffer,ReadBuffer,NumberOfByte,1000);
}




/* ͨ��SPI��AD7192ִ��д����ƼĴ����Ĳ������� RegisterStarAddress:д��Ĵ�������ʼ��ַ NumberOfRegisterToWrite:Ҫд��Ĵ����ĸ��� DataBuffer:д���ֵ OffsetInBuffer:û���õ� */
unsigned char WriteToAD7192ViaSPI(const unsigned char RegisterStarAddress,const unsigned char NumberOfRegisterToWrite,uint32_t *DataBuffer,unsigned char OffsetInBuffer)
{
	//�βΰ���Ҫд��Ĵ�������ʼ��ַRegisterStartAddress(ȡֵ��Χ�Ǵ�0x00����0x07)��д��Ĵ����ĸ�����ָ��Ҫд��AD7192�Ĵ����������ָ��(DataBuffer����Ҫд����ֵ�������м����)��
	//const unsigned char OffsetInBuffer��Ȼ���嵫û���õ�����Ҫ����ͨ��SPI��AD7192�����������Գ�
	/*�ֱ���һ���������д���棬�ٰ�д���渳ֵ��Ȼ��ͨ��
		����void ADuC7026SpiOperation(unsigned char* WriteBuffer, unsigned char *ReadBuffer, unsigned char NumberOfByte)��ִ�У���д�������ݴ�����AD7192*/
		
	unsigned char WriteBuffer[4];//����unsigned char WriteBuf[4]���飬�����е�Ԫ��Ϊ�޷����ַ��ͣ�ռһ���ֽ�
	unsigned char ReadBuffer[4]; //����unsigned char ReadBuf[4]���飬�����е�Ԫ��Ϊ�޷����ַ��ͣ�ռһ���ֽ�
	unsigned char i;
	
	
	for(i = 0;i<NumberOfRegisterToWrite;i++)         //DataBufferͨ��ָ��AD7192Registers
	{
		WriteBuffer[0] = WEN | RW_W | ((RegisterStarAddress + i) << 3) | CREAD_DIS;     //��һ���⣬����д��ͨ�żĴ���;8λ����;��һ�������Ƕ�ָ���Ĵ���ִ��д������
		WriteBuffer[1] = DataBuffer[RegisterStarAddress+i]>>16;                         //����16λ��ʾʲô��˼��DataBuffer��ָ���޷��ų����͵�����ָ�룬ÿ������Ԫ��ռ4���ֽ�(32λ)����ߵ�8λ��Ч��������ǽ�16-23��8λ������������WriteBuf[1]��
		WriteBuffer[2] = DataBuffer[RegisterStarAddress + i]>>8;                        //����8λ��ʾʲô��˼��ͬ�������ǽ�8-15��8λ������������WriteBuf[2]��
		WriteBuffer[3] = DataBuffer[RegisterStarAddress +i];                            //ͬ�������ǽ�0-7��8λ������������WriteBuf[3]��
		
		AD7192_ReadData(WriteBuffer,ReadBuffer,4);                                      //�����˰ɣ������ͨ������STM32F103SpiOperation(WriteBuf, ReadBuf, 4)�����д����
	}
	
	return 0;
}


/* ͨ��SPI��AD7192ִ��һ�ζ��������� RegisterStartAddress ��Ҫ���Ĵ�������ʼ��ַ(ȡֵ��Χ�Ǵ�0x00����0x07) NumberOfRegistersToWrite �� Ҫ���Ĵ����ĸ��� DataBuffer �� Ҫ�����ֵ OffsetInBuffer �� ������ƫ�� */
unsigned char ReadFromAD7192ViaSPI(const unsigned char RegisterStarAddress,const unsigned char NumberOfRegistersToRead,uint32_t *DataBuffer,const unsigned char OffsetInBuffer)
{
	//�βΰ���Ҫ���Ĵ�������ʼ��ַRegisterStartAddress(ȡֵ��Χ�Ǵ�0x00����0x07)��Ҫ��ȡ�Ĵ����ĸ�����ָ�򽫶�ȡAD7192�Ĵ������ݴ���������ָ��(DataBuffer����Ҫ�����ֵ�������м����)��
	//һ��ָ��AD7192Registers[8]��
	//const unsigned char OffsetInBuffer��������˼�ǻ�����ƫ�ƣ���ָAD7192Registers[8]�����ڲ�ƫ�ƣ�ע��������Ŷ��֮ǰ����˵��AD7192Registers[8]֮���Զ���8��Ԫ�أ�
	//һ���Ĵ�����ӦAD7192Registers[8]�����һ��Ԫ�أ��������š�
	
	unsigned char WriteBuffer[4] = {0,0,0,0};       //������4��Ԫ�ص�д�������飬ÿ������Ԫ��ռ1���ֽڡ�
	unsigned char ReadBuffer[4] = {0,0,0,0};        //������4��Ԫ�صĶ��������飬ÿ������Ԫ��ռ1���ֽڡ�
	unsigned char i;
	
	
	for(i=0;i<NumberOfRegistersToRead;i++)
	{
		WriteBuffer[0] = WEN | RW_R | ((RegisterStarAddress + i) << 3) | CREAD_DIS;  //д��ͨ�żĴ���;8λ����;��һ�������Ƕ�ָ���Ĵ���ִ�ж�������CREAD_DIS��ʾ��ʹ����������
		
		//ȷ����һ�����мĴ�������������ôд������Ȼ��Чඡ�
		AD7192_ReadData(WriteBuffer,ReadBuffer,1);           //����ͨ��д��ͨ�żĴ�����ѡ����һ��Ҫ��ȡ�ļĴ���,Ȼ���ٽ�WriteBuf���
		
		WriteBuffer[0] = NOP;
		WriteBuffer[1] = NOP;
		WriteBuffer[2] = NOP;
		WriteBuffer[3] = NOP;
		
		switch(RegisterStarAddress + i)
		{
			case REG_ID:                    //ID�Ĵ���(0x04��8λ�Ĵ���)
			case REG_COM_STA:               //״̬�Ĵ���(0x00��8λ�Ĵ���)
			case REG_GPOCON:                //ͨ������������ƼĴ���(0x05��8λ�Ĵ���)
				AD7192_ReadData(WriteBuffer,ReadBuffer,1);    //��3������Ƕ�ȡһ���ֽ�
				DataBuffer[OffsetInBuffer + i] = ReadBuffer[0];
			break;
			
			case REG_MODE:                  //ģʽ�Ĵ���(0x01��24λ)
			case REG_CONF:                  //���üĴ���(0x02��24λ)
			case REG_OFFSET:                //ʧ���Ĵ���(0x06��24λ)
			case REG_FS:                    //�����̼Ĵ���(0x07��24λ)
				AD7192_ReadData(WriteBuffer,ReadBuffer,3);         //��4������Ƕ�ȡ3���ֽ�
				DataBuffer[OffsetInBuffer +i] = ReadBuffer[0];
				DataBuffer[OffsetInBuffer +i] = (DataBuffer[OffsetInBuffer +i]<<8)+ReadBuffer[1];
				DataBuffer[OffsetInBuffer +i] = (DataBuffer[OffsetInBuffer +i]<<8)+ReadBuffer[2];
			break;
			
			
			case REG_DATA:              //���ݼĴ���(0x03��24λ��32λ) 
				if(AD7192Registers[REG_MODE]&DAT_STA_EN)       //��ͨ��ʹ�ܣ���״̬�Ĵ��������ݸ��ӵ����ݼĴ���24λ�������ϣ�������32λ����
				{
					AD7192_ReadData(WriteBuffer,ReadBuffer,4);    //���Դ�����Ƕ�4���ֽ� 
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


/* AD7192ģʽ�����üĴ������� mode:ģʽ�Ĵ�����ַ cfg:���üĴ�����ַ */
void AD7192_ModeCfgReg(uint32_t mode,uint32_t cfg)
{
	/* �������� */
	AD7192Registers[REG_CONF] = cfg;
	AD7192Registers[REG_MODE] = mode;
	
	/* д������ */
	WriteToAD7192ViaSPI(REG_MODE,2,AD7192Registers,REG_MODE);      //����дģʽ�Ĵ��������üĴ���
}




/* AD7192�����λ */
void AD7192SoftWareReset(void)
{
	
	//AD7192�����λ�����������λ��ԭ��ͨ��ִ��һ��ռ������40������ʱ�����ڵ�д��������ʽDIN��ȥ�ߵ�ƽ״̬��
	
	 /*�ֱ���һ���������д���棬�ٰ�д���渳ֵ��Ȼ��ͨ��
	����void ADuC7026SpiOperation(unsigned char* WriteBuffer, unsigned char *ReadBuffer, unsigned char NumberOfByte)��ִ�У���д�������ݴ�����AD7192*/
	
	unsigned char WriteBuffer[1];   //д����
	unsigned char ReadBuffer[1];    //������
	unsigned char i;
	
	WriteBuffer[0] = 0xFF;
	
	for(i=0;i<10;i++)    //����STM32F103SpiOperation()��ʵ�����ݴ��䣬��AD7192��ͨ�żĴ���д40��1��0xFF��8��1��ѭ��5��һ����40��1��
	{
		AD7192_ReadData(WriteBuffer,ReadBuffer,1);
	}
	AD7192_SCLK_H;
}




/* AD7192�ڲ����ƽУ׼ */
void AD7192InternalZeroScaleCalibration(void)
{
	AD7192Registers[REG_MODE] = 0;
	
	ReadFromAD7192ViaSPI(REG_MODE,1,AD7192Registers,REG_MODE);     // ��ģʽ�Ĵ���
	
	AD7192Registers[REG_MODE] &= ~0xE00000;                        // ���ģʽλ
	AD7192Registers[REG_MODE] |= MODE_INZCL;                       // �����ڲ����ƽУ׼
	
	WriteToAD7192ViaSPI(REG_MODE,1,AD7192Registers,REG_MODE);      // д��ģʽ�Ĵ���
	
	do
	{
		ReadFromAD7192ViaSPI(REG_COM_STA,1,AD7192Registers,REG_COM_STA);  // ��״̬�Ĵ���
	}while((AD7192Registers[REG_COM_STA]&RDY_H) != 0);             // �ȴ�У׼���
}



/* AD7192�ڲ�����ƽУ׼ */
void AD7192InternalFullScaleCalibration(void)
{
	AD7192Registers[REG_MODE] = 0;
	
	ReadFromAD7192ViaSPI(REG_MODE,1,AD7192Registers,REG_MODE);     // ��ģʽ�Ĵ���
	
	AD7192Registers[REG_MODE] &= ~0xE00000;                        // ���ģʽλ
	AD7192Registers[REG_MODE] |= MODE_INFCL;                       // �����ڲ�����ƽУ׼
	
	WriteToAD7192ViaSPI(REG_MODE,1,AD7192Registers,REG_MODE);      // д��ģʽ�Ĵ���
	
	do
	{
		ReadFromAD7192ViaSPI(REG_COM_STA,1,AD7192Registers,REG_COM_STA);  // ��״̬�Ĵ���
	}while((AD7192Registers[REG_COM_STA]&RDY_H) != 0);             // �ȴ�У׼���
}



/* AD7192ϵͳ���ƽУ׼ */
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




/* AD7192ϵͳ����ƽУ׼ */
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



/* AD7192��ȡת�����ݺ��� DataBuffer:ת�������ֵ */
uint32_t AD7192ReadConvertingData(void)
{
	unsigned char WriteBuffer[4];
	unsigned char ReadBuffer[4];
	unsigned long int DataBuffer;
	
	//AD7192_CS_ENABLE() Ƭѡ�ź�ʹ�ܣ�ѡͨAD7192
	WriteBuffer[0] = WEN |RW_R |(REG_DATA << 3) | CREAD_DIS;
	
	AD7192_ReadData(WriteBuffer,ReadBuffer,1);   //����д��ͨ�żĴ�����ȷ����һ�������Ƕ�ȡ���ݼĴ���
	
	WriteBuffer[0] = NOP;
	WriteBuffer[1] = NOP;
	WriteBuffer[2] = NOP;
	WriteBuffer[3] = NOP;
	
	while(HAL_GPIO_ReadPin(AD7192_MISO_PORT,AD7192_MISO_PIN) == 0){}
	while(HAL_GPIO_ReadPin(AD7192_MISO_PORT,AD7192_MISO_PIN) == 1){}    //waiting the 1st RDY failling edge;��DOUT��Ϊ�͵�ƽ��
		
	
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



/* Ad7192�������������� */
void AD7192StartContinuousRead(void)
{
	unsigned char WriteBuffer[1];
	unsigned char ReadBuffer[1];
	
	WriteBuffer[0] = WEN | RW_R | (REG_DATA << 3) | CREAD_EN;
	AD7192_ReadData(WriteBuffer,ReadBuffer,1);
}





/* AD7192���������� */
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
	while(HAL_GPIO_ReadPin(AD7192_MISO_PORT,AD7192_MISO_PIN) == 1){}    //waiting the 1st RDY failling edge;��DOUT��Ϊ�͵�ƽ��
		
	
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



/* AD7192�˳�������ȡ���� */
void AD7192ExitContinuousRead(void)
{
	unsigned char WriteBuffer[1];
	unsigned char ReadBuffer[1];
	
	while(HAL_GPIO_ReadPin(AD7192_MISO_PORT,AD7192_MISO_PIN) == 0){}
	while(HAL_GPIO_ReadPin(AD7192_MISO_PORT,AD7192_MISO_PIN) == 1){}    //waiting the 1st RDY failling edge;��DOUT��Ϊ�͵�ƽ��
		
	
	//AD7192_CS_ENABLE() Ƭѡ�ź�ʹ�ܣ�ѡͨAD7192
	WriteBuffer[0] = WEN |RW_R |(REG_DATA << 3) | CREAD_DIS;
	
	AD7192_ReadData(WriteBuffer,ReadBuffer,1);   //����д��ͨ�żĴ�����ȷ����һ�������Ƕ�ȡ���ݼĴ���
	
}




/* AD7192��ȡ�¶� */
uint32_t AD7192ReadTemperature(void)
{
	uint32_t reg_data = 0;
	
	ReadFromAD7192ViaSPI(REG_MODE,2,AD7192Registers,REG_MODE);       // ��ģʽ��״̬�Ĵ����Ĵ���
	
	AD7192Registers[REG_MODE] &= ~0xE00000;                          // ���ģʽλ
	AD7192Registers[REG_MODE] |= MODE_SING;                          // ����Ϊ����ת��ģʽ
	
	AD7192Registers[REG_CONF] = TEMP;                                // �����¶�ת��ͨ��
	
	WriteToAD7192ViaSPI(REG_MODE,2,AD7192Registers,REG_MODE);        // д��ģʽ�Ĵ���
	
	reg_data = AD7192ReadConvertingData();
	
	return reg_data;
}




/* ��ȡ���¶�ֵ���㺯�� TemperatureCode:������ */
float RealTemperature(uint32_t TemperatureCode)
{
	float temp = 0.0;
	temp = (TemperatureCode - 0x800000) / 2815.0 - 273;
	return temp;
}



/* AD7192��������ת�� Channels:Ҫ������ͨ�� */
void AD7192StartSingleConvertion(uint32_t Channels)
{
	ReadFromAD7192ViaSPI(REG_MODE,2,AD7192Registers,REG_MODE);     // ��ģʽ��״̬�Ĵ����Ĵ���
	
	AD7192Registers[REG_MODE] &= ~0xE00000;                        // ���ģʽλ
	AD7192Registers[REG_MODE] |= MODE_SING;                        // ����Ϊ����ת��ģʽ

	AD7192Registers[REG_CONF] &= ~0xFF00;                          // ���ͨ��ѡ��λ
	AD7192Registers[REG_CONF] |= Channels;                         // ����Ҫת����ͨ��
	
	WriteToAD7192ViaSPI(REG_MODE,2,AD7192Registers,REG_MODE);      // д��ģʽ�Ĵ���
}




/* AD7192��������ת�� Channels:Ҫ������ͨ�� */
void AD7192StartContinuousConvertion(uint32_t Channels)
{
	AD7192Registers[REG_MODE] = 0;
	ReadFromAD7192ViaSPI(REG_MODE,2,AD7192Registers,REG_MODE);     // ��ģʽ��״̬�Ĵ����Ĵ���
	
	AD7192Registers[REG_MODE] &= ~0xE00000;                        // ���ģʽλ
	AD7192Registers[REG_MODE] |= MODE_CONT;                        // ����Ϊ����ת��ģʽ

	AD7192Registers[REG_CONF] &= ~0xFF00;                          // ���ͨ��ѡ��λ
	AD7192Registers[REG_CONF] |= Channels;                         // ����Ҫת����ͨ��
	
	WriteToAD7192ViaSPI(REG_MODE,2,AD7192Registers,REG_MODE);      // д��ģʽ�Ĵ���
}











