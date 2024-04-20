/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "ff_gen_drv.h"



/* Ϊÿ���豸����һ�������� */
//#define ATA			    0     // Ԥ��SD��ʹ��
//#define SPI_FLASH		1     // �ⲿSPI Flash

extern Disk_drvTypeDef disk;

/*-----------------------------------------------------------------------*/
/* ��ȡ�豸״̬                                                          */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (
	BYTE pdrv		/* ������ */
)
{

	DSTATUS stat;
	
	stat = disk.drv[pdrv]->disk_status(disk.lun[pdrv]);
  return stat;
}

/*-----------------------------------------------------------------------*/
/* �豸��ʼ��                                                            */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (
	BYTE pdrv				/* ������ */
)
{

	DSTATUS stat = RES_OK;	
	
	if(disk.is_initialized[pdrv] == 0)
	{
		disk.is_initialized[pdrv] =1;
		stat = disk.drv[pdrv]->disk_initialize(disk.lun[pdrv]);
	}
	
	return stat;
}


/*-----------------------------------------------------------------------*/
/* ����������ȡ�������ݵ�ָ���洢��                                              */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (
	BYTE pdrv,		/* �豸������(0..) */
	BYTE *buff,		/* ���ݻ����� */
	DWORD sector,	/* �����׵�ַ */
	UINT count		/* ��������(1..128) */
)
{
	DRESULT res;
	
	res = disk.drv[pdrv]->disk_read(disk.lun[pdrv],buff,sector,count);
	
	return res;
}

/*-----------------------------------------------------------------------*/
/* д������������д��ָ�������ռ���                                      */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			  /* �豸������(0..) */
	const BYTE *buff,	/* ��д�����ݵĻ����� */
	DWORD sector,		  /* �����׵�ַ */
	UINT count			  /* ��������(1..128) */
)
{

	DRESULT res;
	
	res = disk.drv[pdrv]->disk_write(disk.lun[pdrv],buff,sector,count);
	
	return res;
}
#endif


/*-----------------------------------------------------------------------*/
/* ��������                                                              */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* ������ */
	BYTE cmd,		  /* ����ָ�� */
	void *buff		/* д����߶�ȡ���ݵ�ַָ�� */
)
{
	DRESULT res;

  res = disk.drv[pdrv]->disk_ioctl(disk.lun[pdrv], cmd, buff);
  return res;
}
#endif

__weak DWORD get_fattime(void) {
	/* ���ص�ǰʱ��� */
	return	 0;
}

