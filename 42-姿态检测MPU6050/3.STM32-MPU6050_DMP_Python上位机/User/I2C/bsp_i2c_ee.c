#include "bsp_i2c_ee.h"


I2C_HandleTypeDef      I2C_Handle;


void I2CMaster_Init(void)
{
	GPIO_InitTypeDef          GPIO_InitStructure;
	
	SENSORS_I2C_RCC_CLK_ENABLE();
	
	SENSORS_I2C_SCL_GPIO_CLK_ENABLE();
	SENSORS_I2C_SDA_GPIO_CLK_ENABLE();

  GPIO_InitStructure.Pin = SENSORS_I2C_SCL_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SENSORS_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = SENSORS_I2C_SDA_PIN;
	HAL_GPIO_Init(SENSORS_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
  
	
	if(HAL_I2C_GetState(&I2C_Handle) == HAL_I2C_STATE_RESET)
	{
		/* 强制复位I2C外设时钟 */
		SENSORS_I2C_FORCE_RESET();
		
		/* 释放I2C外设时钟复位 */
		SENSORS_I2C_RELEASE_RESET();
		
		/* I2C配置 */
		I2C_Handle.Instance = SENSORS_I2C;
		I2C_Handle.Init.ClockSpeed = 400000;
		I2C_Handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
		I2C_Handle.Init.OwnAddress1 = 0;
		I2C_Handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
		I2C_Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
		I2C_Handle.Init.OwnAddress2 = 0;
		I2C_Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
		I2C_Handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

		/* 初始化I2C */
		HAL_I2C_Init(&I2C_Handle);
	}
}



/* 通过重新初始化I2C来管理错误回调 */
static void I2Cx_Error(uint8_t Addr)
{
	/* 恢复I2C寄存器为默认值 */
	HAL_I2C_DeInit(&I2C_Handle);
	
	/* 重新初始化I2C外设 */
	I2CMaster_Init();
}



/* 写寄存器，提供给上层的接口，slave_addr:从机地址 reg_addr:寄存器地址 len:写入的长度 data_ptr:只想要写入的数据 */
int Sensors_I2C_WriteRegister(unsigned char slave_addr,unsigned char reg_addr,unsigned short len,unsigned char *data_ptr)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	status = HAL_I2C_Mem_Write(&I2C_Handle,slave_addr,reg_addr,I2C_MEMADD_SIZE_8BIT,data_ptr,len,I2Cx_FLAG_TIMEOUT);
	
	/* 检查通讯状态 */
	if(status != HAL_OK)
	{
		/* 总线出错处理 */
		I2Cx_Error(slave_addr);
	}
	while(HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY);
	
	/* 检查SENSORS是否就绪进行下一次读写操作 */
	while(HAL_I2C_IsDeviceReady(&I2C_Handle,slave_addr,I2Cx_FLAG_TIMEOUT,I2Cx_FLAG_TIMEOUT) == HAL_TIMEOUT);
	
	/* 等待传输结束 */
	while(HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY);
	
	return status;
}


/* 读寄存器，提供给上层的接口，slave_addr:从机地址 reg_addr:寄存器地址 len:要读取的长度 data_ptr:只想要存储数据的指针 */
int Sensors_I2C_ReadRegister(unsigned char slave_addr,unsigned char reg_addr,unsigned short len,unsigned char *data_ptr)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	status = HAL_I2C_Mem_Read(&I2C_Handle,slave_addr,reg_addr,I2C_MEMADD_SIZE_8BIT,data_ptr,len,I2Cx_FLAG_TIMEOUT);
	
	/* 检查通讯状态 */
	if(status != HAL_OK)
	{
		/* 总线出错处理 */
		I2Cx_Error(slave_addr);
	}
	while(HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY);
	
	/* 检查SENSORS是否就绪进行下一次读写操作 */
	while(HAL_I2C_IsDeviceReady(&I2C_Handle,slave_addr,I2Cx_FLAG_TIMEOUT,I2Cx_FLAG_TIMEOUT) == HAL_TIMEOUT);
	
	/* 等待传输结束 */
	while(HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY);
	
	return status;
	
}


int get_tick_count(unsigned long *count)
{
	count[0] = HAL_GetTick();
	
	return 0;
}




