#ifndef _DISKIO_DEFINED
#define _DISKIO_DEFINED

#ifdef __cplusplus
extern "C" {
#endif
#include "ff.h"			/* Basic definitions and declarations of API */

/* Status of Disk Functions */
typedef BYTE	DSTATUS;

/* 磁盘函数的返回结果 */
typedef enum {
    RES_OK = 0,     /* 0：成功 */
    RES_ERROR,      /* 1：读/写错误 */
    RES_WRPRT,      /* 2：写保护 */
    RES_NOTRDY,     /* 3：设备未就绪 */
    RES_PARERR      /* 4：参数无效 */
} DRESULT;

/* Prototypes for disk control functions */
DSTATUS disk_initialize (BYTE pdrv);
DSTATUS disk_status (BYTE pdrv);
DRESULT disk_read (BYTE pdrv, BYTE* buff, LBA_t sector, UINT count);
DRESULT disk_write (BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count);
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);

/* 磁盘控制函数的原型声明 */
#define STA_NOINIT   0x01    /* 驱动器尚未初始化 */
#define STA_NODISK   0x02    /* 驱动器中没有存储介质 */
#define STA_PROTECT  0x04    /* 存储介质被写保护 */

/* disk_ioctl 函数使用的命令码 */
/* 通用命令（FatFs 会使用） */
#define CTRL_SYNC            0   /* 完成所有挂起的写操作（当 FF_FS_READONLY == 0 时需要） */
#define GET_SECTOR_COUNT     1   /* 获取存储介质的总扇区数（当 FF_USE_MKFS == 1 时需要） */
#define GET_SECTOR_SIZE      2   /* 获取扇区大小（当 FF_MAX_SS != FF_MIN_SS 时需要） */
#define GET_BLOCK_SIZE       3   /* 获取擦除块大小（当 FF_USE_MKFS == 1 时需要） */
#define CTRL_TRIM            4   /* 通知设备某些扇区的数据已不再使用（当 FF_USE_TRIM == 1 时需要） */

/* 通用命令（FatFs 不会使用） */
#define CTRL_POWER           5   /* 获取 / 设置电源状态 */
#define CTRL_LOCK            6   /* 锁定 / 解锁介质的移除 */
#define CTRL_EJECT           7   /* 弹出介质 */
#define CTRL_FORMAT          8   /* 对介质进行物理格式化 */

/* MMC / SD 卡专用 ioctl 命令（FatFs 不会使用） */
#define MMC_GET_TYPE         10  /* 获取卡类型 */
#define MMC_GET_CSD          11  /* 获取 CSD 寄存器内容 */
#define MMC_GET_CID          12  /* 获取 CID 寄存器内容 */
#define MMC_GET_OCR          13  /* 获取 OCR 寄存器内容 */
#define MMC_GET_SDSTAT       14  /* 获取 SD 状态信息 */
#define ISDIO_READ           55  /* 从 SD iSDIO 寄存器读取数据 */
#define ISDIO_WRITE          56  /* 向 SD iSDIO 寄存器写入数据 */
#define ISDIO_MRITE          57  /* 向 SD iSDIO 寄存器进行掩码写入 */

/* ATA / CF 设备专用 ioctl 命令（FatFs 不会使用） */
#define ATA_GET_REV          20  /* 获取固件版本号 */
#define ATA_GET_MODEL        21  /* 获取设备型号名称 */
#define ATA_GET_SN           22  /* 获取设备序列号 */

#ifdef __cplusplus
}
#endif

#endif
