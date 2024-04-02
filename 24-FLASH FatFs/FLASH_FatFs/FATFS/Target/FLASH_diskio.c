#include <string.h>
#include "ff_gen_drv.h"

#include "bsp_spi_flash.h"

static __IO DSTATUS Stat = STA_NOINIT;


#define SPI_FLASH_REBUILD            0           // 1:使能每次重新格式化串行Flash，0：禁用每次重新格式化串行Flash
#define SPI_FLASH_SECTOR_SIZE        4096        // 串行Flash扇区大小
#define SPI_FLASH_START_SECTOR       514         // 串行Flash文件系统FatFS偏移量
#define SPI_FLASH_SECTOR_COUNT       1534        // 串行Flash文件系统FatFS占用扇区个数    


DSTATUS SPIFLASH_initialize(BYTE);
DSTATUS SPIFLASH_status(BYTE);
DRESULT SPIFLASH_read(BYTE,BYTE*,DWORD,UINT);


#if _USE_WRITE == 1      //如果允许写操作
	DRESULT SPIFLASH_write(BYTE,const BYTE*,DWORD,UINT);
#endif

#if _USE_IOCTL == 1      // 如果输入输出操作控制
	DRESULT SPIFLASH_ioctl(BYTE,BYTE,void*);
#endif


/* 定义串行FLASH接口函数 */
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


/* 函数体 --------------------------------------------------------------------*/
/* FLASH初始化 */
DSTATUS SPIFLASH_initialize(BYTE lun)
{
	#if SPI_FLASH_REBUILD == 1
	static uint8_t startflag=1;
	#endif
	
	Stat = STA_NOINIT;
	/* 初始化SPI FLASH IO外设 */
	SPI_FLASH_Init();
	SPI_FLASH_WakeUp();
	/* 获取串行FLASH状态 */
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


/* FLASH 状态获取 */
DSTATUS SPIFLASH_status(BYTE lun)
{
	Stat = STA_NOINIT;
	
	if(SPI_FLASH_ReadID() == sFLASH_ID)
	{
		Stat &= ~STA_NOINIT;
	}
	return Stat;
}


/* 从串行FLASH读取数据到缓冲区 lun:不用buff:存放读取到的缓冲区指针sector:扇区地址count:扇区数目 */
DRESULT SPIFLASH_read(BYTE lun,BYTE *buff,DWORD sector,UINT count)
{
	sector += SPI_FLASH_START_SECTOR;
	SPI_FLASH_BufferRead(buff,sector*SPI_FLASH_SECTOR_SIZE,count*SPI_FLASH_SECTOR_SIZE);
	return RES_OK;
	
}



/* 将缓冲区数据写入到串行FLASH中 lun:不用buff:存放读取到的缓冲区指针sector:扇区地址count:扇区数目 */
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


/* 输入输出控制操作 lun:不用 cmd:控制命令 buff:存放读取到的数据缓冲区指针 */
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
		
		/* 获取串行FLASH总扇区数目(DWORD) */
  case GET_SECTOR_COUNT :
    *(DWORD * )buff = SPI_FLASH_SECTOR_COUNT;	
    res = RES_OK;
    break;
  
  /* 获取读写扇区大小(WORD) */
  case GET_SECTOR_SIZE :
    *(WORD * )buff = SPI_FLASH_SECTOR_SIZE;
    res = RES_OK;
    break;
  
  /* 获取擦除块大小(DWORD) */
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


	



