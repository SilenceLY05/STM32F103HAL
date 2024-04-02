/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "bsp_spi_flash.h"


/* Definitions of physical drive number for each drive */
#define ATA                 0     //Ԥ��SD��ʹ��
#define SPI_FLASH           1     //�ⲿSPI FLASH


/*-----------------------------------------------------------------------*/
/* ��ȡ�豸״̬                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* ������ */
)
{
	DSTATUS status = STA_NOINIT;
	
	switch(pdrv)
	{
		case ATA:
			break;
		
		case SPI_FLASH:
			/* SPI FLASH״̬��⣺��ȡSPI FLASH �豸ID */
			if(sFLASH_ID == SPI_FLASH_ReadID())
			{
				/* �豸ID��ȡ�����ȷ */
				status &= ~STA_NOINIT;
			}
			else
			{
				/* �豸ID��ȡ������� */
				status = STA_NOINIT;
			}
			break;
			
		default:
			status = STA_NOINIT;
	}
	return status;
}



/*-----------------------------------------------------------------------*/
/* �豸��ʼ��                                                   */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	uint16_t i;
	DSTATUS status = STA_NOINIT;
	switch(pdrv)
	{
		case ATA:
			break;
		
		case SPI_FLASH:
			SPI_FLASH_Init();
			i = 500;
			while(i--);
			SPI_FLASH_WakeUp();
			status = disk_status(SPI_FLASH);
		break;
		
		default:
			status = STA_NOINIT;
	}
	return status;
}



/*-----------------------------------------------------------------------*/
/* ����������ȡ�������ݵ�ָ���洢��                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* �豸������ */
	BYTE *buff,		/* ���ݻ����� */
	LBA_t sector,	/* �����׵�ַ */
	UINT count		/* �������� */
)
{
	DSTATUS status = RES_PARERR;
	
	switch(pdrv)
	{
		case ATA:
			break;
		
		case SPI_FLASH:
			/* ����ƫ��2MB���ⲿFLash�ļ�ϵͳ�ռ����SPI FLash����6MB�ռ� */
		sector += 512;
		SPI_FLASH_BufferRead(buff,sector<<12,count<<12);
		
		status = RES_OK;
		break;
		
		default:
			status = RES_PARERR;
	}
	return status;
}



/*-----------------------------------------------------------------------*/
/* д������������д��ָ�������ռ���                                      */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	uint32_t write_addr;
	DSTATUS status = RES_PARERR;
	
	if(!count)
	{
		return RES_PARERR;
	}
	
	switch(pdrv)
	{
		case ATA:
			break;
		
		case SPI_FLASH:
			sector += 512;
			write_addr = sector <<12;
			SPI_FLASH_SectorErase(write_addr);
			SPI_FLASH_BufferWrite((uint8_t *)buff,write_addr,count<<12);
			status = RES_OK;
		
			break;
		
		default:
			status = RES_PARERR;
	}
	return status;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DSTATUS status = RES_PARERR;
	
	switch(pdrv)
	{
		case ATA:
			break;
		
		case SPI_FLASH:
			switch(cmd)
			{
				case GET_SECTOR_COUNT:
					*(DWORD*)buff = 1536;
				break;
				
				case GET_SECTOR_SIZE:
					*(DWORD*)buff = 4096;
				break;
				
				case GET_BLOCK_SIZE:
					*(DWORD*)buff =1;
				break;
			}
			status = RES_OK;
			break;
			
		default:
			status = RES_PARERR;
	}
	return status;
}


DWORD get_fattime (void)
{
  /* ���������ʵ�ʵĻ�ȡʱ��ķ���... */
  
  
  /* ��Ӧ��Ҫ�޸ķ���ֵ */
  return ((2021-1980)<<25) | ((1)<<21) | ((1)<<16) | ((1)<<11) | ((1)<<5) | ((1)<<0);
}


