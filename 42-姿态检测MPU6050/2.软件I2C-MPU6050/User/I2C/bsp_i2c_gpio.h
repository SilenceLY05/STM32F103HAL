#ifndef __BSP_I2C_GPIO_H__
#define __BSP_I2C_GPIO_H__

#include "stm32f1xx.h"
#include <inttypes.h>

#define I2C_WR     0       /* 写控制bit */
#define I2C_RD     1       /* 读控制bit */


/* 定义I2C总线连接的GPIO端口 */
#define I2Cx_SDA_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOB_CLK_ENABLE()


#define GPIO_PORT_I2C               GPIOB
#define I2C_SCL_PIN                 GPIO_PIN_6
#define I2C_SDA_PIN                 GPIO_PIN_7



/* 直接操作寄存器控制IO */
#define digitalH(p,i)                      {p->BSRR=i;}        //设置为高电平
#define digitalL(p,i)                      {p->BSRR = (uint32_t)i<<16;}        //设置为低电平



/* 定义读写SCL和SDA的宏，以增加代码的可移植性和可阅读性 */
#if    0
	#define I2C_SCL_1()               digitalH(GPIO_PORT_I2C,I2C_SCL_PIN)
	#define I2C_SCL_0()               digitalL(GPIO_PORT_I2C,I2C_SCL_PIN)

	#define I2C_SDA_1()               digitalH(GPIO_PORT_I2C,I2C_SDA_PIN)
	#define I2C_SDA_0()               digitalL(GPIO_PORT_I2C,I2C_SDA_PIN)

	#define I2C_SDA_READ()            HAL_GPIO_ReadPin(GPIO_PORT_I2C,I2C_SDA_PIN)   //读SDA口线状态
#else     /* 这个分支选择直接寄存器操作实现IO读写 注：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
	#define I2C_SCL_1()               GPIO_PORT_I2C->BSRR = I2C_SCL_PIN          /* SCL = 1 */
	#define I2C_SCL_0()               GPIO_PORT_I2C->BRR = I2C_SCL_PIN          /* SCL = 0 */

	#define I2C_SDA_1()               GPIO_PORT_I2C->BSRR = I2C_SDA_PIN          /* SCL = 1 */
	#define I2C_SDA_0()               GPIO_PORT_I2C->BRR = I2C_SDA_PIN          /* SCL = 0 */

	#define I2C_SDA_READ()            ((GPIO_PORT_I2C->IDR & I2C_SDA_PIN) != 0)    /* 读SDA口线状态 */
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

