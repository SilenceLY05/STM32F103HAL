#ifndef __BSP_I2C_EE_H__
#define __BSP_I2C_EE_H__


#include "stm32f1xx.h"
#include "bsp_usart.h"



extern I2C_HandleTypeDef      I2C_Handle;


/* AT24C01/02每页有8个字节 */
#define EEPROM_PAGESIZE            8
/* AT24C04/08A/16A每页有16个字节 */
//#define EEPROM_PAGESIZE            16

/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2C_OWN_ADDRESS            0x00

#define SENSORS_I2C                       I2C1
#define SENSORS_I2C_RCC_CLK_ENABLE()      __HAL_RCC_I2C1_CLK_ENABLE()
#define SENSORS_I2C_FORCE_RESET()         __HAL_RCC_I2C1_FORCE_RESET()
#define SENSORS_I2C_RELEASE_RESET()       __HAL_RCC_I2C1_RELEASE_RESET()


#define SENSORS_I2C_SCL_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SENSORS_I2C_SCL_PIN               GPIO_PIN_6
#define SENSORS_I2C_SCL_GPIO_PORT         GPIOB

#define SENSORS_I2C_SDA_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define SENSORS_I2C_SDA_PIN               GPIO_PIN_7
#define SENSORS_I2C_SDA_GPIO_PORT         GPIOB


#define SENSORS_I2C_AF                    GPIO_AF4_I2C1


/* 等待超时时间 */
#define Delay                      HAL_Delay
#define I2Cx_FLAG_TIMEOUT          ((uint32_t)0x1000)
#define I2Cx_LONG_TIMEOUT          ((uint32_t)(300*I2CT_FLAG_TIMEOUT))




/* 信息输出 */
#define I2C_DEBUG_ON            1
#define I2C_DEBUG_FUNC_ON       0

#define I2C_INFO(fmt,arg...)           printf("<<-I2C-INFO->> "fmt"\n",##arg)
#define I2C_ERROR(fmt,arg...)          printf("<<-I2C-ERROR->> "fmt"\n",##arg)
#define I2C_DEBUG(fmt,arg...)          do{\
                                          if(I2C_DEBUG_ON)\
                                          printf("<<-I2C-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)







void I2CMaster_Init(void);
static void I2Cx_Error(uint8_t Addr);
int Sensors_I2C_WriteRegister(unsigned char slave_addr,unsigned char reg_addr,unsigned short len,unsigned char *data_ptr);
int Sensors_I2C_ReadRegister(unsigned char slave_addr,unsigned char reg_addr,unsigned short len,unsigned char *data_ptr);
int get_tick_count(unsigned long *count);																				




#endif /* __BSP_I2C_EE_H__ */


