#include "bsp_mpu6050.h"
#include "bsp_usart.h"
#include "bsp_i2c_gpio.h"


#define MPU_ERROR        I2C_ERROR
#define MPU_INFO         I2C_INFO



/* 写数据到MPU6050寄存器 reg_add:寄存器地址 reg_data:要写入的数据 */
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



/* 从MPU6050寄存器读取数据 reg_add:寄存器地址 reg_data:存储数据的缓冲区, num:要读取的数据量 */
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



/* 初始化MPU6050芯片 */
void MPU6050_Init(void)
{
	int i = 0,j = 0;
	//在初始化之前要延时一段时间，若没有延时，则断电后再上电数据可能会出错
	for(i=0;i<1000;i++)
	{
		for(j=0;j<1000;j++)
		{
			
		}
	}

	
	MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1,0x00);    //解除休眠状态
	MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV,0x07);    //陀螺仪采样率
	MPU6050_WriteReg(MPU6050_RA_CONFIG,0x06);      
	MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG,0x01);  //配置加速度传感器工作在16G模式
	MPU6050_WriteReg(MPU6050_RA_GYRO_CONFIG,0x18);   //陀螺仪自检及测量范围，典型值0x18(不自检，2000deg/s)

}


/* 读取MPU6050的ID  正常返回1，异常返回0 */
uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
	MPU6050_ReadReg(MPU6050_RA_WHO_AM_I,&Re,1);   //读器件地址
	
	if(Re != 0x68)
	{
		printf("MPU6050 dectected error!\r\n 检测不倒MPU6050模块，请检查模块与开发板的接线");
		return 0;
	}
	else
	{
		printf("MPU6050 ID = %d\r\n",Re);
		return 1;
	}
}



/* 读取MPU6050的加速度数据 */
void MPU6050ReadAce(short *accData)
{
	uint8_t buff[6];
	MPU6050_ReadReg(MPU6050_ACC_OUT,buff,6);
	accData[0] = (buff[0]<<8) | buff[1];
	accData[1] = (buff[2]<<8) | buff[3];
	accData[2] = (buff[4]<<8) | buff[5];
}



/* 读取MPU6050的角速度数据 */
void MPU6050ReadGyro(short *gyroData)
{
	uint8_t buff[6];
	MPU6050_ReadReg(MPU6050_GYRO_OUT,buff,6);
	gyroData[0] = (buff[0]<<8) | buff[1];
	gyroData[1] = (buff[2]<<8) | buff[3];
	gyroData[2] = (buff[4]<<8) | buff[5];
}


/* 读取MPU6050的原始温度数据 */
void MPU6050ReadTemp(short *tempData)
{
	uint8_t buff[2];
	MPU6050_ReadReg(MPU6050_RA_TEMP_OUT_H,buff,2);    //读取温度值
	*tempData = (buff[0]<<8) | buff[1];
}



/* 读取MPU6050的温度数据，转化成摄氏度 */
void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	uint8_t buff[2];
	
	MPU6050_ReadReg(MPU6050_RA_TEMP_OUT_H,buff,2);
	temp3 = (buff[0]<<8) | buff[1];
	*Temperature = ((double)temp3/340.0) + 36.53;
}



