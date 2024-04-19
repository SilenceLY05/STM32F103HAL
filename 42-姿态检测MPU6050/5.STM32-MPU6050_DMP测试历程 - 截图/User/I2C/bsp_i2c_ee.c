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
		/* ǿ�Ƹ�λI2C����ʱ�� */
		SENSORS_I2C_FORCE_RESET();
		
		/* �ͷ�I2C����ʱ�Ӹ�λ */
		SENSORS_I2C_RELEASE_RESET();
		
		/* I2C���� */
		I2C_Handle.Instance = SENSORS_I2C;
		I2C_Handle.Init.ClockSpeed = 400000;
		I2C_Handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
		I2C_Handle.Init.OwnAddress1 = 0;
		I2C_Handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
		I2C_Handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
		I2C_Handle.Init.OwnAddress2 = 0;
		I2C_Handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
		I2C_Handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

		/* ��ʼ��I2C */
		HAL_I2C_Init(&I2C_Handle);
	}
}



/* ͨ�����³�ʼ��I2C���������ص� */
static void I2Cx_Error(uint8_t Addr)
{
	/* �ָ�I2C�Ĵ���ΪĬ��ֵ */
	HAL_I2C_DeInit(&I2C_Handle);
	
	/* ���³�ʼ��I2C���� */
	I2CMaster_Init();
}



/* д�Ĵ������ṩ���ϲ�Ľӿڣ�slave_addr:�ӻ���ַ reg_addr:�Ĵ�����ַ len:д��ĳ��� data_ptr:ֻ��Ҫд������� */
int Sensors_I2C_WriteRegister(unsigned char slave_addr,unsigned char reg_addr,unsigned short len,unsigned char *data_ptr)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	status = HAL_I2C_Mem_Write(&I2C_Handle,slave_addr,reg_addr,I2C_MEMADD_SIZE_8BIT,data_ptr,len,I2Cx_FLAG_TIMEOUT);
	
	/* ���ͨѶ״̬ */
	if(status != HAL_OK)
	{
		/* ���߳����� */
		I2Cx_Error(slave_addr);
	}
	while(HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY);
	
	/* ���SENSORS�Ƿ����������һ�ζ�д���� */
	while(HAL_I2C_IsDeviceReady(&I2C_Handle,slave_addr,I2Cx_FLAG_TIMEOUT,I2Cx_FLAG_TIMEOUT) == HAL_TIMEOUT);
	
	/* �ȴ�������� */
	while(HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY);
	
	return status;
}


/* ���Ĵ������ṩ���ϲ�Ľӿڣ�slave_addr:�ӻ���ַ reg_addr:�Ĵ�����ַ len:Ҫ��ȡ�ĳ��� data_ptr:ֻ��Ҫ�洢���ݵ�ָ�� */
int Sensors_I2C_ReadRegister(unsigned char slave_addr,unsigned char reg_addr,unsigned short len,unsigned char *data_ptr)
{
	HAL_StatusTypeDef status = HAL_OK;
	
	status = HAL_I2C_Mem_Read(&I2C_Handle,slave_addr,reg_addr,I2C_MEMADD_SIZE_8BIT,data_ptr,len,I2Cx_FLAG_TIMEOUT);
	
	/* ���ͨѶ״̬ */
	if(status != HAL_OK)
	{
		/* ���߳����� */
		I2Cx_Error(slave_addr);
	}
	while(HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY);
	
	/* ���SENSORS�Ƿ����������һ�ζ�д���� */
	while(HAL_I2C_IsDeviceReady(&I2C_Handle,slave_addr,I2Cx_FLAG_TIMEOUT,I2Cx_FLAG_TIMEOUT) == HAL_TIMEOUT);
	
	/* �ȴ�������� */
	while(HAL_I2C_GetState(&I2C_Handle) != HAL_I2C_STATE_READY);
	
	return status;
	
}


int get_tick_count(unsigned long *count)
{
	count[0] = HAL_GetTick();
	
	return 0;
}




