#ifndef __WILDFIRE_IMAGE_ASSISTANT_H__
#define __WILDFIRE_IMAGE_ASSISTANT_H__

#include "stm32f1xx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bsp_usart.h"


typedef __packed struct
{
	uint32_t head;      //��ͷ
	uint8_t addr;       //�豸��ַ
	uint32_t len;       //������
	uint8_t cmd;        //����
}packet_head_t;



/* �������ݽӿ� */
#define CAM_ASS_SEND_DATA(data,len)          debug_send_data(data,len)


void ack_read_wincc(uint16_t number,uint8_t val);
void read_reg(uint16_t number,uint8_t addr);
int8_t receiving_process(void);
void set_wincc_format(uint8_t addr,uint8_t type,uint16_t width,uint8_t height);
int write_reb_wincc(uint8_t addr,uint16_t width,uint16_t height);




#endif /* __WILDFIRE_IMAGE_ASSISTANT_H__ */


