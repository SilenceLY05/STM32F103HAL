#include "bsp_mpu6050.h"
#include "bsp_usart.h"
#include "bsp_i2c_gpio.h"


#define MPU_ERROR        I2C_ERROR
#define MPU_INFO         I2C_INFO



/* д���ݵ�MPU6050�Ĵ��� reg_add:�Ĵ�����ַ reg_data:Ҫд������� */
void MPU6050_WriteReg(uint8_t reg_add,uint8_t reg_data)
{
	i2c_Start();
	i2c_SendByte(MPU6050_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	i2c_SendByte(reg_data);
	i2c_WaitAck();
	i2c_Stop();
	
}



/* ��MPU6050�Ĵ�����ȡ���� reg_add:�Ĵ�����ַ reg_data:�洢���ݵĻ�����, num:Ҫ��ȡ�������� */
void MPU6050_ReadReg(uint8_t reg_add,unsigned char *reg_data,uint8_t num)
{
	unsigned char i;
	
	i2c_Start();
	i2c_SendByte(MPU6050_ADDRESS);
	i2c_WaitAck();
	i2c_SendByte(reg_add);
	i2c_WaitAck();
	
	i2c_Start();
	i2c_SendByte(MPU6050_ADDRESS+1);
	i2c_WaitAck();
	
	for(i=0;i<(num-1);i++)
	{
		*reg_data = i2c_ReadByte(1);
		reg_data++;
	}
	*reg_data = i2c_ReadByte(0);
	i2c_Stop();
	
}



/* ��ʼ��MPU6050оƬ */
void MPU6050_Init(void)
{
	int i = 0,j = 0;
	//�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬��û����ʱ����ϵ�����ϵ����ݿ��ܻ����
	for(i=0;i<1000;i++)
	{
		for(j=0;j<1000;j++)
		{
			
		}
	}

	
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1,0x00);    //�������״̬
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV,0x07);    //�����ǲ�����
	MPU6050_WriteReg(MPU6050_RA_CONFIG,0x06);      
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG,0x01);  //���ü��ٶȴ�����������16Gģʽ
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG,0x18);   //�������Լ켰������Χ������ֵ0x18(���Լ죬2000deg/s)

}


/* ��ȡMPU6050��ID  ��������1���쳣����0 */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
	MPU6050_ReadReg(MPU6050_RA_WHO_AM_I,&Re,1);   //��������ַ
	
	if(Re != 0x68)
	{
		printf("MPU6050 dectected error!\r\n ��ⲻ��MPU6050ģ�飬����ģ���뿪����Ľ���");
		return 0;
	}
	else
	{
		printf("MPU6050 ID = %d\r\n",Re);
		return 1;
	}
}



/* ��ȡMPU6050�ļ��ٶ����� */
void MPU6050ReadAce(short *accData)
{
	uint8_t buff[6];
	MPU6050_ReadReg(MPU6050_ACC_OUT,buff,6);
	accData[0] = (buff[0]<<8) | buff[1];
	accData[1] = (buff[2]<<8) | buff[3];
	accData[2] = (buff[4]<<8) | buff[5];
}



/* ��ȡMPU6050�Ľ��ٶ����� */
void MPU6050ReadGyro(short *gyroData)
{
	uint8_t buff[6];
	MPU6050_ReadReg(MPU6050_GYRO_OUT,buff,6);
	gyroData[0] = (buff[0]<<8) | buff[1];
	gyroData[1] = (buff[2]<<8) | buff[3];
	gyroData[2] = (buff[4]<<8) | buff[5];
}


/* ��ȡMPU6050��ԭʼ�¶����� */
void MPU6050ReadTemp(short *tempData)
{
	uint8_t buff[2];
	MPU6050_ReadReg(MPU6050_RA_TEMP_OUT_H,buff,2);    //��ȡ�¶�ֵ
	*tempData = (buff[0]<<8) | buff[1];
}



/* ��ȡMPU6050���¶����ݣ�ת�������϶� */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	uint8_t buff[2];
	
	MPU6050_ReadReg(MPU6050_RA_TEMP_OUT_H,buff,2);
	temp3 = (buff[0]<<8) | buff[1];
	*Temperature = ((double)temp3/340.0) + 36.53;
}



