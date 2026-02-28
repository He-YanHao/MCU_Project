#include "ff.h"
#include "diskio.h"

#include "BSP_SD_card.h"
#include <stdint.h>
#include <stdio.h>

/* 此处需要为每一种存储设备设置唯一对应物理驱动号 */
#define SD_CARD     0

// 获取驱动状态
DSTATUS disk_status (
	BYTE pdrv		/* 驱动器号 */
)
{
	DSTATUS stat = STA_NOINIT;
    uint32_t cardstate = 0;
	switch (pdrv) {
        case SD_CARD :
    //        // 是否插卡（GPIO/CD）
    //        if (!sd_hw_detect()) {
    //            stat |= STA_NODISK;
    //            return stat;
    //        }

            // 是否已初始化
            if (sd_cardstatus_get(&cardstate) == SD_OK)
            {
                if ((cardstate&0xFFF800FF) == 0)
                {
                    stat &= ~STA_NOINIT;
                }
            }

            // 写保护检测 判断卡有没有被锁定
            if (sd_cardstatus_get(&cardstate) == SD_OK) {
                if(cardstate & 0x02000000) {
                    stat |= STA_NOINIT;
                }
            }
        break;
        default:
            stat = STA_NODISK;
        break;
    }
    return stat;
}

// 初始化一个驱动器                                                      */
DSTATUS disk_initialize (
	BYTE pdrv				// 用于标识驱动器的物理驱动号
)
{
	DSTATUS stat = STA_NOINIT;
	switch (pdrv) {
	case SD_CARD :
		if (SD_OK == BSP_SD_card_init())
        {
            stat = disk_status(pdrv);
        }
		// 在这里翻译结果代码
        break;

    default:
        stat = STA_NOINIT;
	}

    return stat;
}

/* 读取扇区（或多个扇区）                                                */
DRESULT disk_read (
	BYTE pdrv,		/* 物理驱动号，用于标识存储设备 */
	BYTE *buff,		/* 数据缓冲区，用于存储读取的数据 */
	LBA_t sector,	/* 起始扇区（LBA） */
	UINT count		/* 要读取的扇区数 */
)
{
	DRESULT res = RES_PARERR;;
    sd_error_enum sd_statu = SD_ERROR;
	switch (pdrv) {
        case SD_CARD :
        // 在这里对函数的参数进行转换/处理
        // SD驱动使用的是字节地址 转化为块地址

        /* 单扇区：永远用 CMD17 */
        if (count == 1) {
            sd_statu = sd_block_read((uint32_t*)buff, sector * 512, 512);
        } else {
            sd_statu = sd_multiblocks_read((uint32_t*)buff, sector*512, 512, count);
        }

        // translate the reslut code here
        // 在这里对返回结果代码进行转换/处理
        switch (sd_statu) {
            case SD_OK:
                res = RES_OK;
                break;
            // 写保护
            case SD_WP_VIOLATION:
                res = RES_WRPRT;
                break;
            // 读/写错误
            default:
                res = RES_ERROR;
                break;
        }
        break;
    default:
        res = RES_PARERR;
	}
	return res;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

// 配置为0 表示不是只读模式
#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* 物理驱动号，用于标识存储设备 */
	const BYTE *buff,	/* 要写入的数据 */
	LBA_t sector,		/* 起始扇区（LBA） */
	UINT count			/* 要写入的扇区数 */
)
{
	DRESULT res = RES_PARERR;
    sd_error_enum sd_statu = SD_ERROR;

	switch (pdrv) {
        case SD_CARD :
        // 在这里对函数的参数进行转换/处理
        // SD驱动使用的是字节地址 转化为块地址
        if (count == 1) {
            sd_statu = sd_block_write((uint32_t*)buff, sector * 512, 512);
        } else {
            sd_statu = sd_multiblocks_write((uint32_t*)buff, sector*512, 512, count);
        }

        // 在这里对返回结果代码进行转换/处理
        switch (sd_statu) {
            case SD_OK:
                res = RES_OK;
                break;
            // 写保护
            case SD_WP_VIOLATION:
                res = RES_WRPRT;
                break;
            // 读/写错误
            default:
                res = RES_ERROR;
                break;
        }
        break;
    default:
        res = RES_PARERR;
	}
	return res;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/
DRESULT disk_ioctl (
	BYTE pdrv,		/* 物理驱动号（0..） */
	BYTE cmd,		/* 控制命令 */
	void *buff		/* 用于发送/接收控制数据的缓冲区 */
)
{
	DRESULT res = RES_PARERR;

	switch (pdrv) {
	case SD_CARD :
		// Process of the command for the RAM drive
		switch(cmd) {
            case GET_SECTOR_COUNT: // 获取存储介质的总扇区数
                // sd_card_capacity_get() 可以获取SD卡大小 单位kb 乘以2之后就是块(512字节大小)
                *(DWORD *)buff = sd_card_capacity_get()*2;
                res = RES_OK;
                break;
				
            case GET_SECTOR_SIZE: // 获取扇区大小
				*(WORD *)buff = 512;
                res = RES_OK;
				break;
				
			case GET_BLOCK_SIZE: // 获取擦除块大小
				*(DWORD *)buff = 1;
                res = RES_OK;
				break;
        default:
            break;
        }
        default:
            res = RES_PARERR;
	}
	return res;
}
