#include "bsp_ad5689.h"
#include "bsp_usart.h"


SPI_HandleTypeDef   hspi_AD5689;


void AD5689_GPIO_Config(void)
{
	GPIO_InitTypeDef          GPIO_InitStructure;
	
	AD5689_SPIx_CLK_ENABLE();
	AD5689_GPIO_CLK_ENABLE();
	
	/* 使能SPI外设以及SPI引脚时钟 */
	GPIO_InitStructure.Pin = AD5689_SCK_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AD5689_SCK_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = AD5689_MISO_PIN | AD5689_MOSI_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AD5689_MISO_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = AD5689_SYNC_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(AD5689_SYNC_GPIO_PORT,&GPIO_InitStructure);
	
	hspi_AD5689.Instance = AD5689_SPIx;
	hspi_AD5689.Init.Mode = SPI_MODE_MASTER;
	hspi_AD5689.Init.Direction = SPI_DIRECTION_1LINE;
	hspi_AD5689.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi_AD5689.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi_AD5689.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi_AD5689.Init.NSS = SPI_NSS_SOFT;
	hspi_AD5689.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	hspi_AD5689.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi_AD5689.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi_AD5689.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi_AD5689.Init.CRCPolynomial = 7;
	HAL_SPI_Init(&hspi_AD5689);
	
	
}


/* 延时函数 */
static void delay(void)
{
	uint16_t i;
	
	for(i=0;i<50;i++)
	{
		__NOP();
	}
}



/* 设置AD5689寄存器值 command:指令 channel:通道 data:数据 */
void AD5689_SetRegisterValue(uint8_t command,uint16_t channel,uint16_t data)
{
	uint8_t reg[3] = {0};
	
	reg[0] = (command << 4) | channel;
	reg[1] = (data>>8)&0xFF;
	reg[2] = data&0xFF;
	
	AD5689_SYNC_L;
	delay();
	HAL_SPI_Transmit(&hspi_AD5689,reg,3,0xF);
	delay();
	AD5689_SYNC_H;
	
}



/* LDAC屏蔽寄存器设置 */
void AD5689_LDAC_MASK(uint16_t ldac1,uint16_t ldac2)
{
	uint8_t data = ldac1;
	data |= (ldac2<<3);
	AD5689_SetRegisterValue(CMD_LDAC_MASK,DAC_ALL,data);
}



/* 软件复位 */
void AD5689_Reset(void)
{
	AD5689_SetRegisterValue(CMD_RESET,DAC_ALL,0);
}



/* 掉电模式 channel1:通道1的掉电模式 channel2:通道2的掉电模式 */
void AD5689_PowerDown(uint16_t channel1,uint16_t channel2)
{
	uint8_t data = 0x3C;
	data |= ((channel2<<6) | channel1);
	AD5689_SetRegisterValue(CMD_POWER_DOWN,DAC_ALL,data);
}



/* 写入的更新DAC通道n(与LDAC无关) channel:通道 data:数值 */
void AD5689_WriteUpdata_DACREG(uint16_t channel,uint16_t data)
{
	AD5689_SetRegisterValue(CMD_WRITE_UPDATEn,channel,data);
}



/* 写入输入寄存器n(取决于LDAC) channel:通道 data:数值 */
void AD5689_Write_InputDACREG(uint16_t channel,uint16_t data)
{
	AD5689_SetRegisterValue(CMD_WRITEn,channel,data);
}




/* 以输入寄存器n的内容更新DAC寄存器n(取决于LDAC) channel:通道 data:数值 */
void AD5689_InputREG_toUpdata_DACREG(uint16_t channel,uint16_t data)
{
	AD5689_SetRegisterValue(CMD_WRITEn_UPDATEn,channel,data);
}



/* LDAC屏蔽寄存器设置 菊花链操作 */
void AD5689_DAISYCHAIN_OPERATION(uint16_t decn)
{
	AD5689_SetRegisterValue(CMD_DCEN_REG,DAC_ALL,decn);
}



/* AD5689初始化 */
void AD5689_Init(void)
{
	AD5689_GPIO_Config();
	AD5689_RESET_L;
	HAL_Delay(5);
	AD5689_RESET_H;
	HAL_Delay(5);
	AD5689_Reset();
	AD5689_LDAC_MASK(LDAC_Disable,LDAC_Disable);
}



