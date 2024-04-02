#include <string.h>
#include "ff_gen_drv.h"

#include "bsp_spi_flash.h"

static __IO DSTATUS Stat = STA_NOINIT;


#define SPI_FLASH_REBUILD            0           // 1:ʹ��ÿ�����¸�ʽ������Flash��0������ÿ�����¸�ʽ������Flash
#define SPI_FLASH_SECTOR_SIZE        4096        // ����Flash������С
#define SPI_FLASH_START_SECTOR       514         // ����Flash�ļ�ϵͳFatFSƫ����
#define SPI_FLASH_SECTOR_COUNT       1534        // ����Flash�ļ�ϵͳFatFSռ����������    


DSTATUS SPIFLASH_initialize(BYTE);
DSTATUS SPIFLASH_status(BYTE);
DRESULT SPIFLASH_read(BYTE,BYTE*,DWORD,UINT);


#if _USE_WRITE == 1      //�������д����
	DRESULT SPIFLASH_write(BYTE,const BYTE*,DWORD,UINT);
#endif

#if _USE_IOCTL == 1      // ������������������
	DRESULT SPIFLASH_ioctl(BYTE,BYTE,void*);
#endif


/* ���崮��FLASH�ӿں��� */
const Diskio_drvTypeDef SPIFLASH_Driver = 
{
	SPIFLASH_initialize,SPIFLASH_status,SPIFLASH_read,
	#if _USE_WRITE == 1
	SPIFLASH_write,
	#endif
	
	#if _USE_IOCTL == 1
	SPIFLASH_ioctl,
	#endif
	
};


/* ������ --------------------------------------------------------------------*/
/* FLASH��ʼ�� */
DSTATUS SPIFLASH_initialize(BYTE lun)
{
	#if SPI_FLASH_REBUILD == 1
	static uint8_t startflag=1;
	#endif
	
	Stat = STA_NOINIT;
	/* ��ʼ��SPI FLASH IO���� */
	SPI_FLASH_Init();
	SPI_FLASH_WakeUp();
	/* ��ȡ����FLASH״̬ */
	if(SPI_FLASH_ReadID() == sFLASH_ID)
	{
		#if SPI_FLASH_REBUILD == 1
		if(startflag)
		{
			SPI_FLASH_SectorErase(SPI_FLASH_START_SECTOR*SPI_FLASH_SECTOR_SIZE);
			startflag =0;
		}
		#endif
		Stat &= ~STA_NOINIT;
	}
	return Stat;
}


/* FLASH ״̬��ȡ */
DSTATUS SPIFLASH_status(BYTE lun)
{
	Stat = STA_NOINIT;
	
	if(SPI_FLASH_ReadID() == sFLASH_ID)
	{
		Stat &= ~STA_NOINIT;
	}
	return Stat;
}


/* �Ӵ���FLASH��ȡ���ݵ������� lun:����buff:��Ŷ�ȡ���Ļ�����ָ��sector:������ַcount:������Ŀ */
DRESULT SPIFLASH_read(BYTE lun,BYTE *buff,DWORD sector,UINT count)
{
	sector += SPI_FLASH_START_SECTOR;
	SPI_FLASH_BufferRead(buff,sector*SPI_FLASH_SECTOR_SIZE,count*SPI_FLASH_SECTOR_SIZE);
	return RES_OK;
	
}



/* ������������д�뵽����FLASH�� lun:����buff:��Ŷ�ȡ���Ļ�����ָ��sector:������ַcount:������Ŀ */
#if _USE_WRITE == 1
DRESULT SPIFLASH_write(BYTE lun,const BYTE *buff,DWORD sector,UINT count)
{
	uint32_t write_addr;
	sector += SPI_FLASH_START_SECTOR;
	write_addr = sector * SPI_FLASH_SECTOR_SIZE;
	SPI_FLASH_SectorErase(write_addr);
	SPI_FLASH_BufferWrite((uint8_t *)buff,write_addr,count*SPI_FLASH_SECTOR_SIZE);
	return RES_OK;
}
#endif 


/* ����������Ʋ��� lun:���� cmd:�������� buff:��Ŷ�ȡ�������ݻ�����ָ�� */
#if _USE_IOCTL == 1
DRESULT SPIFLASH_ioctl(BYTE lun,BYTE cmd,void *buff)
{
	DRESULT res = RES_ERROR;
	
	if(Stat & STA_NOINIT)
		return RES_NOTRDY;
	
	switch(cmd)
	{
		case CTRL_SYNC:
			res = RES_OK;
		break;
		
		/* ��ȡ����FLASH��������Ŀ(DWORD) */
  case GET_SECTOR_COUNT :
    *(DWORD * )buff = SPI_FLASH_SECTOR_COUNT;	
    res = RES_OK;
    break;
  
  /* ��ȡ��д������С(WORD) */
  case GET_SECTOR_SIZE :
    *(WORD * )buff = SPI_FLASH_SECTOR_SIZE;
    res = RES_OK;
    break;
  
  /* ��ȡ�������С(DWORD) */
  case GET_BLOCK_SIZE :
    *(DWORD * )buff = 1;
    res = RES_OK;
    break;
  
  default:
    res = RES_PARERR;
	}
	return res;
}

#endif


	



