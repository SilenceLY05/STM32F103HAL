#ifndef __BSP_I2C_GPIO_H__
#define __BSP_I2C_GPIO_H__

#include "stm32f1xx.h"
#include <inttypes.h>

#define I2C_WR     0       /* д����bit */
#define I2C_RD     1       /* ������bit */


/* ����I2C�������ӵ�GPIO�˿� */
#define I2Cx_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()


#define GPIO_PORT_I2C               GPIOB
#define I2C_SCL_PIN                 GPIO_PIN_6
#define I2C_SDA_PIN                 GPIO_PIN_7



/* ֱ�Ӳ����Ĵ�������IO */
#define digitalH(p,i)                      {p->BSRR=i;}        //����Ϊ�ߵ�ƽ
#define digitalL(p,i)                      {p->BSRR = (uint32_t)i<<16;}        //����Ϊ�͵�ƽ



/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if    0
	#define I2C_SCL_1()               digitalH(GPIO_PORT_I2C,I2C_SCL_PIN)
	#define I2C_SCL_0()               digitalL(GPIO_PORT_I2C,I2C_SCL_PIN)

	#define I2C_SDA_1()               digitalH(GPIO_PORT_I2C,I2C_SDA_PIN)
	#define I2C_SDA_0()               digitalL(GPIO_PORT_I2C,I2C_SDA_PIN)

	#define I2C_SDA_READ()            HAL_GPIO_ReadPin(GPIO_PORT_I2C,I2C_SDA_PIN)   //��SDA����״̬
#else     /* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д ע������д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define I2C_SCL_1()               GPIO_PORT_I2C->BSRR = I2C_SCL_PIN          /* SCL = 1 */
	#define I2C_SCL_0()               GPIO_PORT_I2C->BRR = I2C_SCL_PIN          /* SCL = 0 */

	#define I2C_SDA_1()               GPIO_PORT_I2C->BSRR = I2C_SDA_PIN          /* SCL = 1 */
	#define I2C_SDA_0()               GPIO_PORT_I2C->BRR = I2C_SDA_PIN          /* SCL = 0 */

	#define I2C_SDA_READ()            ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)    /* ��SDA����״̬ */
#endif



void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t __ucByte);
uint8_t i2c_ReadByte(uint8_t ack);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
void i2c_CfgGpio(void);
uint8_t i2c_CheckDevice(uint8_t Addr);




#endif /* __BSP_I2C_GPIO_H__ */

