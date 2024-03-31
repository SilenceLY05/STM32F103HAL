#ifndef __BSP_I2C_GPIO_H__
#define __BSP_I2C_GPIO_H__

#include "stm32f1xx.h"
#include <inttypes.h>

#define EEPROM_I2C_WR     0       /* д����bit */
#define EEPROM_I2C_RD     1       /* ������bit */


/* ����I2C�������ӵ�GPIO�˿� */
#define I2Cx_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()


#define EEPROM_GPIO_PORT_I2C               GPIOB
#define EEPROM_I2C_SCL_PIN                 GPIO_PIN_6
#define EEPROM_I2C_SDA_PIN                 GPIO_PIN_7



/* ֱ�Ӳ����Ĵ�������IO */
#define digitalH(p,i)                      {p->BSRR=i;}        //����Ϊ�ߵ�ƽ
#define digitalL(p,i)                      {p->BSRR = (uint32_t)i<<16;}        //����Ϊ�͵�ƽ



/* �����дSCL��SDA�ĺ꣬�����Ӵ���Ŀ���ֲ�ԺͿ��Ķ��� */
#if    0
	#define EEPROM_I2C_SCL_1()               digitalH(EEPROM_GPIO_PORT_I2C,EEPROM_I2C_SCL_PIN)
	#define EEPROM_I2C_SCL_0()               digitalL(EEPROM_GPIO_PORT_I2C,EEPROM_I2C_SCL_PIN)

	#define EEPROM_I2C_SDA_1()               digitalH(EEPROM_GPIO_PORT_I2C,EEPROM_I2C_SDA_PIN)
	#define EEPROM_I2C_SDA_0()               digitalL(EEPROM_GPIO_PORT_I2C,EEPROM_I2C_SDA_PIN)

	#define EEPROM_I2C_SDA_READ()            HAL_GPIO_ReadPin(EEPROM_GPIO_PORT_I2C,EEPROM_I2C_SDA_PIN)   //��SDA����״̬
#else     /* �����֧ѡ��ֱ�ӼĴ�������ʵ��IO��д ע������д������IAR��߼����Ż�ʱ���ᱻ�����������Ż� */
	#define EEPROM_I2C_SCL_1()               EEPROM_GPIO_PORT_I2C->BSRR = EEPROM_I2C_SCL_PIN          /* SCL = 1 */
	#define EEPROM_I2C_SCL_0()               EEPROM_GPIO_PORT_I2C->BRR = EEPROM_I2C_SCL_PIN          /* SCL = 0 */

	#define EEPROM_I2C_SDA_1()               EEPROM_GPIO_PORT_I2C->BSRR = EEPROM_I2C_SDA_PIN          /* SCL = 1 */
	#define EEPROM_I2C_SDA_0()               EEPROM_GPIO_PORT_I2C->BRR = EEPROM_I2C_SDA_PIN          /* SCL = 0 */

	#define EEPROM_I2C_SDA_READ()            ((EEPROM_GPIO_PORT_I2C->IDR & EEPROM_I2C_SDA_PIN) != 0)    /* ��SDA����״̬ */
#endif



void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t __ucByte);
uint8_t i2c_ReadByte(void);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
void i2c_CfgGpio(void);




#endif /* __BSP_I2C_GPIO_H__ */

