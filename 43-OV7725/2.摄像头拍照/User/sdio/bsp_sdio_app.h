#ifndef __BSP_SDIO_APP_H__
#define __BSP_SDIO_APP_H__


#include "stm32f1xx.h"
#include "diskio.h"



void Sd_fs_init(void);
void Sd_fs_test(void);
int Sdfs_new(BYTE *new_file_name,BYTE *write_buffer,BYTE buffer_size);
int Sdfs_write(BYTE *write_file_name,BYTE *write_buffer,BYTE buffer_size);
int Sdfs_read(BYTE *read_file_name,BYTE *save_buffer);
//int GetGBKCode_from_sd(unsigned char* pBuffer,const unsigned char * c);
int GetGBKCode_from_sd(uint8_t * pBuffer,uint16_t c);


#endif /* __BSP_SDIO_APP_H__ */

