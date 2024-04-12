#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__


#include "stm32f1xx.h"
#include "bsp_usart.h"


extern CAN_HandleTypeDef CAN_Handle;
extern CAN_TxHeaderTypeDef  CAN_HandleTxMsg;
extern CAN_RxHeaderTypeDef  CAN_HandleRxMsg;


#define CANx                         CAN1
#define CAN_CLK_ENABLE()             __HAL_RCC_CAN1_CLK_ENABLE()
#define CAN_RX_IRQ                   CAN1_RX0_IRQn
#define CAN_RX_IRQHandler            CAN1_RX0_IRQHandler


#define CAN_RX_PIN                   GPIO_PIN_8
#define CAN_RX_GPIO_PORT             GPIOB
#define CAN_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()
#define CAN_TX_PIN                   GPIO_PIN_9
#define CAN_TX_GPIO_PORT             GPIOB
#define CAN_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOB_CLK_ENABLE()



/*debug*/

#define CAN_DEBUG_ON         1
#define CAN_DEBUG_ARRAY_ON   1
#define CAN_DEBUG_FUNC_ON    1



// Log define
#define CAN_INFO(fmt,arg...)           printf("<<-CAN-INFO->> "fmt"\n",##arg)
#define CAN_ERROR(fmt,arg...)          printf("<<-CAN-ERROR->> "fmt"\n",##arg)
#define CAN_DEBUG(fmt,arg...)          do{\
                                         if(CAN_DEBUG_ON)\
                                         printf("<<-CAN-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                       }while(0)

#define CAN_DEBUG_ARRAY(array, num)    do{\
                                         int32_t i;\
                                         uint8_t* a = array;\
                                         if(CAN_DEBUG_ARRAY_ON)\
                                         {\
                                            printf("<<-CAN-DEBUG-ARRAY->>\n");\
                                            for (i = 0; i < (num); i++)\
                                            {\
                                                printf("%02x   ", (a)[i]);\
                                                if ((i + 1 ) %10 == 0)\
                                                {\
                                                    printf("\n");\
                                                }\
                                            }\
                                            printf("\n");\
                                        }\
                                       }while(0)

#define CAN_DEBUG_FUNC()               do{\
                                         if(CAN_DEBUG_FUNC_ON)\
                                         printf("<<-CAN-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)


static void CAN_GPIO_Config(void);
static void CAN_NVIC_Config(void);
static void CAN_Mode_Config(void);
static void CAN_Filter_Config(void);
void CAN_Config(void);
void CAN_SetMsg(void);
void Init_RxMes(void);



#endif /* __BSP_CAN_H__ */


