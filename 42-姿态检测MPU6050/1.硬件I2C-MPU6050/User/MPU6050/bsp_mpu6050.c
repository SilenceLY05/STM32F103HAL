#include "bsp_mpu6050.h"
#include "bsp_usart.h"
#include "bsp_i2c_ee.h"



#define MPU_ERROR        I2C_ERROR
#define MPU_INFO         I2C_INFO



/* д���ݵ�MPU6050�Ĵ��� reg_add:�Ĵ�����ַ reg_data:Ҫд������� */
void MPU6050_WriteReg(uint8_t reg_add,uint8_t reg_data)
{
	Sensors_I2C_WriteRegister(MPU6050_ADDRESS,reg_add,1,&reg_data);
}



/* ��MPU6050�Ĵ�����ȡ���� reg_add:�Ĵ�����ַ reg_data:�洢���ݵĻ�����, num:Ҫ��ȡ�������� */
void MPU6050_ReadReg(uint8_t reg_add,unsigned char *reg_data,uint8_t num)
{
	Sensors_I2C_ReadRegister(MPU6050_ADDRESS,reg_add,num,reg_data);
}



/* ��ʼ��MPU6050оƬ */
void MPU6050_Init(void)
{
	//�ڳ�ʼ��֮ǰҪ��ʱһ��ʱ�䣬Ҫû����ʱ����ϵ�����ϵ����ݿ��ܻ����
	Delay(100);
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1,0x00);    //�������״̬
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV,0x07);    //�����ǲ�����
	MPU6050_WriteReg(MPU6050_RA_CONFIG,0x06);      
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG,0x01);  //���ü��ٶȴ�����������16Gģʽ
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG,0x18);   //�������Լ켰������Χ������ֵ0x18(���Լ죬2000deg/s)
	Delay(200);
}


/* ��ȡMPU6050��ID  ��������1���쳣����0 */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
	MPU6050_ReadReg(MPU6050_RA_WHO_AM_I,&Re,1);   //��������ַ
	
	if(Re != 0x68)
	{
		MPU_ERROR("MPU6050 dectected error!\r\n ��ⲻ��MPU6050ģ�飬����ģ���뿪����Ľ���");
		return 0;
	}
	else
	{
		MPU_INFO("MPU6050 ID = %d\r\n",Re);
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
void MPU6050ReadGryo(short *gyroData)
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



