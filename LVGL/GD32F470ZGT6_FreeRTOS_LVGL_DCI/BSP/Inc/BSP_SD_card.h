#ifndef __BSP_SD_CARD_H__
#define __BSP_SD_CARD_H__

#include "gd32f4xx.h"

/* SD 存储卡标准命令索引 */
#define SD_CMD_GO_IDLE_STATE                  ((uint8_t) 0)  /* 将SD卡置于空闲状态 */
#define SD_CMD_ALL_SEND_CID                   ((uint8_t) 2)  /* 获取SD卡的唯一识别号CID */
#define SD_CMD_SEND_RELATIVE_ADDR             ((uint8_t) 3)  /* 请求卡分配一个相对地址RCA */
#define SD_CMD_SWITCH_FUNC                    ((uint8_t) 6)  /* 设置SD卡的数据总线宽度 */
#define SD_CMD_SELECT_DESELECT_CARD           ((uint8_t) 7)  /* 选中SD卡 */
#define SD_CMD_SEND_IF_COND                   ((uint8_t) 8)  /* 检查SD卡是否符合特定的电压要求 用于初始化时 检查卡是否支持某些特定的电压范围 */
#define SD_CMD_SEND_CSD                       ((uint8_t) 9)  /* 读取卡的CSD寄存器 CSD包含卡的特定信息 如最大块大小 卡类型等 */
#define SD_CMD_SEND_CID                       ((uint8_t)10)  /* 读取卡的CID寄存器 获取卡的唯一身份信息。 */
#define SD_CMD_STOP_TRANSMISSION              ((uint8_t)12)  /* 停止当前的数据传输 */
#define SD_CMD_SEND_STATUS                    ((uint8_t)13)  /* 请求卡的状态信息 */
#define SD_CMD_GO_INACTIVE_STATE              ((uint8_t)15)  /* 将卡置于非活动状态 */
#define SD_CMD_SET_BLOCKLEN                   ((uint8_t)16)  /* 设置块大小 */
#define SD_CMD_READ_SINGLE_BLOCK              ((uint8_t)17)  /* 读取一个块的数据 */
#define SD_CMD_READ_MULTIPLE_BLOCK            ((uint8_t)18)  /* 读取多个数据块 */
#define SD_CMD_WRITE_BLOCK                    ((uint8_t)24)  /* 向卡写入一个数据块 */
#define SD_CMD_WRITE_MULTIPLE_BLOCK           ((uint8_t)25)  /* 向卡写入多个数据块 */
#define SD_CMD_PROG_CSD                       ((uint8_t)27)  /* 编程CSD寄存器 用于更改卡的一些特性 */
#define SD_CMD_SET_WRITE_PROT                 ((uint8_t)28)  /* 设置写保护 */
#define SD_CMD_CLR_WRITE_PROT                 ((uint8_t)29)  /* 清除写保护 */
#define SD_CMD_SEND_WRITE_PROT                ((uint8_t)30)  /* 查询写保护状态 */
#define SD_CMD_ERASE_WR_BLK_START             ((uint8_t)32)  /* 设置擦除起始块 */
#define SD_CMD_ERASE_WR_BLK_END               ((uint8_t)33)  /* 设置擦除结束块 */
#define SD_CMD_ERASE                          ((uint8_t)38)  /* 执行擦除操作 */
#define SD_CMD_LOCK_UNLOCK                    ((uint8_t)42)  /* 锁定或解锁卡的保护区域 */
#define SD_CMD_APP_CMD                        ((uint8_t)55)  /* 指示后续的命令是应用命令ACMD 必须在应用命令之前发送 */

/* SD 存储卡应用特定命令索引 */
#define SD_APPCMD_SET_BUS_WIDTH               ((uint8_t) 6)  /* 设置SD卡的数据总线宽度 */
#define SD_APPCMD_SD_STATUS                   ((uint8_t)13)  /* 查询SD卡的状态 */
#define SD_APPCMD_SEND_NUM_WR_BLOCKS          ((uint8_t)22)  /* 查询卡的写入块数 */
#define SD_APPCMD_SET_WR_BLK_ERASE_COUNT      ((uint8_t)23)  /* 设置写入块擦除计数 */
#define SD_APPCMD_SD_SEND_OP_COND             ((uint8_t)41)  /* 主机发送容量支持信息和OCR寄存器内容请求卡的操作条件 */
#define SD_APPCMD_SET_CLR_CARD_DETECT         ((uint8_t)42)  /* 设置或清除卡检测功能 */
#define SD_APPCMD_SEND_SCR                    ((uint8_t)51)  /* 读取SCR寄存器 获取SD卡的特性 */

/* SD 存储卡标准命令索引 */
#define CMD0           ((uint8_t) 0)  /* 将SD卡置于空闲状态 */
#define CMD2           ((uint8_t) 2)  /* 获取SD卡的唯一识别号CID */
#define CMD3           ((uint8_t) 3)  /* 请求卡分配一个相对地址RCA */
#define CMD6           ((uint8_t) 6)  /* 切换卡的功能设置 用于切换卡的操作模式 */
#define CMD7           ((uint8_t) 7)  /* 选中SD卡 */
#define CMD8           ((uint8_t) 8)  /* 检查SD卡是否符合特定的电压要求 用于初始化时 检查卡是否支持某些特定的电压范围 */
#define CMD9           ((uint8_t) 9)  /* 读取卡的CSD寄存器 包含卡的特定信息 如最大块大小 卡类型等 */
#define CMD10          ((uint8_t)10)  /* 读取卡的CID寄存器 获取卡的唯一身份信息。 */
#define CMD12          ((uint8_t)12)  /* 停止当前的数据传输 */
#define CMD13          ((uint8_t)13)  /* 请求卡的状态信息 */
#define CMD15          ((uint8_t)15)  /* 将卡置于非活动状态 */
#define CMD16          ((uint8_t)16)  /* 设置块大小 */
#define CMD17          ((uint8_t)17)  /* 读取一个块的数据 */
#define CMD18          ((uint8_t)18)  /* 读取多个数据块 */
#define CMD24          ((uint8_t)24)  /* 向卡写入一个数据块 */
#define CMD25          ((uint8_t)25)  /* 向卡写入多个数据块 */
#define CMD27          ((uint8_t)27)  /* 编程CSD寄存器 用于更改卡的一些特性 */
#define CMD28          ((uint8_t)28)  /* 设置写保护 */
#define CMD29          ((uint8_t)29)  /* 清除写保护 */
#define CMD30          ((uint8_t)30)  /* 查询写保护状态 */
#define CMD32          ((uint8_t)32)  /* 设置擦除起始块 */
#define CMD33          ((uint8_t)33)  /* 设置擦除结束块 */
#define CMD38          ((uint8_t)38)  /* 执行擦除操作 */
#define CMD42          ((uint8_t)42)  /* 锁定或解锁卡的保护区域 */
#define CMD55          ((uint8_t)55)  /* 指示后续的命令是应用命令ACMD 必须在应用命令之前发送 */

/* SD 存储卡应用特定命令索引 */
#define ACMD6          ((uint8_t) 6)  /* 设置SD卡的数据总线宽度 */
#define ACMD13         ((uint8_t)13)  /* 查询SD卡的状态 */
#define ACMD22         ((uint8_t)22)  /* 查询卡的写入块数 */
#define ACMD23         ((uint8_t)23)  /* 设置写入块擦除计数 */
#define ACMD41         ((uint8_t)41)  /* 主机发送容量支持信息和OCR寄存器内容请求卡的操作条件 */
#define ACMD42         ((uint8_t)42)  /* 设置或清除卡检测功能 */
#define ACMD51         ((uint8_t)51)  /* 读取SCR寄存器 获取SD卡的特性 */

/* card command class */
#define SD_CCC_SWITCH                          BIT(10)       /* class 10 */
#define SD_CCC_IO_MODE                         BIT(9)        /* class 9 */
#define SD_CCC_APPLICATION_SPECIFIC            BIT(8)        /* class 8 */
#define SD_CCC_LOCK_CARD                       BIT(7)        /* class 7 */
#define SD_CCC_WRITE_PROTECTION                BIT(6)        /* class 6 */
#define SD_CCC_ERASE                           BIT(5)        /* class 5 */
#define SD_CCC_BLOCK_WRITE                     BIT(4)        /* class 4 */
#define SD_CCC_BLOCK_READ                      BIT(2)        /* class 2 */
#define SD_CCC_BASIC                           BIT(0)        /* class 0 */

/* SD card data transmission mode */
#define SD_DMA_MODE                           ((uint32_t)0x00000000) /* DMA mode */
#define SD_POLLING_MODE                       ((uint32_t)0x00000001) /* polling mode */

/* lock unlock status */
#define SD_LOCK                               ((uint8_t)0x05)        /* lock the SD card */
#define SD_UNLOCK                             ((uint8_t)0x02)        /* unlock the SD card */

/* 支持的存储卡类型 */
typedef enum
{
    SDIO_STD_CAPACITY_SD_CARD_V1_1 = 0,   /* 标准容量 SD 卡版本 1.1 */
    SDIO_STD_CAPACITY_SD_CARD_V2_0,       /* 标准容量 SD 卡版本 2.0 */
    SDIO_HIGH_CAPACITY_SD_CARD,           /* 高容量 SD 卡 */
    SDIO_SECURE_DIGITAL_IO_CARD,          /* 安全数字 IO 卡 */
    SDIO_SECURE_DIGITAL_IO_COMBO_CARD,    /* 安全数字 IO 组合卡 */
    SDIO_MULTIMEDIA_CARD,                 /* 多媒体卡 */
    SDIO_HIGH_CAPACITY_MULTIMEDIA_CARD,   /* 高容量多媒体卡 */
    SDIO_HIGH_SPEED_MULTIMEDIA_CARD       /* 高速多媒体卡 */
} sdio_card_type_enum;

/* 卡片识别（CID）寄存器 */
typedef struct
{
    __IO uint8_t mid;                     /* 制造商ID */
    __IO uint16_t oid;                    /* OEM/应用程序ID */
    __IO uint32_t pnm0;                   /* 产品名称 */
    __IO uint8_t pnm1;                    /* 产品名称 */
    __IO uint8_t prv;                     /* 产品版本 */
    __IO uint32_t psn;                    /* 产品序列号 */
    __IO uint16_t mdt;                    /* 制造日期 */
    __IO uint8_t cid_crc;                 /* CID CRC7 校验和 */
} sd_cid_struct;

/* CSD 寄存器（CSD 版本 1.0 和 2.0） */
typedef struct
{
    __IO uint8_t csd_struct;              /* CSD 结构体 */
    __IO uint8_t taac;                    /* 数据读取访问时间 */
    __IO uint8_t nsac;                    /* 数据读取访问时间（时钟周期数） */
    __IO uint8_t tran_speed;              /* 最大数据传输速率 */
    __IO uint16_t ccc;                    /* 卡命令类别 */
    __IO uint8_t read_bl_len;             /* 最大读取数据块长度 */
    __IO uint8_t read_bl_partial;         /* 是否允许部分块读取 */
    __IO uint8_t write_blk_misalign;      /* 是否允许写块不对齐 */
    __IO uint8_t read_blk_misalign;       /* 是否允许读块不对齐 */
    __IO uint8_t dsp_imp;                 /* 是否实现 DSR（Driver Stage Register） */
    __IO uint32_t c_size;                 /* 设备大小，CSD 版本 1.0 为 12 位，CSD 版本 2.0 为 22 位 */
    __IO uint8_t vdd_r_curr_min;          /* 在 VDD 最小电压下的最大读取电流，CSD 版本 1.0 */
    __IO uint8_t vdd_r_curr_max;          /* 在 VDD 最大电压下的最大读取电流，CSD 版本 1.0 */
    __IO uint8_t vdd_w_curr_min;          /* 在 VDD 最小电压下的最大写入电流，CSD 版本 1.0 */
    __IO uint8_t vdd_w_curr_max;          /* 在 VDD 最大电压下的最大写入电流，CSD 版本 1.0 */
    __IO uint8_t c_size_mult;             /* 设备大小乘数，CSD 版本 1.0 */
    __IO uint8_t erase_blk_en;            /* 启用单块擦除 */
    __IO uint8_t sector_size;             /* 擦除扇区大小 */
    __IO uint8_t wp_grp_size;             /* 写保护组大小 */
    __IO uint8_t wp_grp_enable;           /* 启用写保护组 */
    __IO uint8_t r2w_factor;              /* 写入速度因子 */
    __IO uint8_t write_bl_len;            /* 最大写入数据块长度 */
    __IO uint8_t write_bl_partial;        /* 是否允许部分块写入 */
    __IO uint8_t file_format_grp;         /* 文件格式组 */
    __IO uint8_t copy_flag;               /* 复制标志（OTP） */
    __IO uint8_t perm_write_protect;      /* 永久写保护 */
    __IO uint8_t tmp_write_protect;       /* 临时写保护 */
    __IO uint8_t file_format;             /* 文件格式 */
    __IO uint8_t csd_crc;                 /* CSD CRC 校验和 */
} sd_csd_struct;

/* 卡片信息 */
typedef struct
{
    sd_cid_struct card_cid;               /* CID 寄存器 */
    sd_csd_struct card_csd;               /* CSD 寄存器 */
    sdio_card_type_enum card_type;        /* 卡类型 */
    uint32_t card_capacity;               /* 卡容量 */
    uint32_t card_blocksize;              /* 卡块大小 */
    uint16_t card_rca;                    /* 卡相对地址 */
} sd_card_info_struct;

// SD卡错误状态枚举
typedef enum
{
    SD_OUT_OF_RANGE = 0,              /* 0*/ /* 命令的参数超出了范围 */
    SD_ADDRESS_ERROR,                 /* 1*/ /* 地址不对齐 地址与块长度不匹配 */
    SD_BLOCK_LEN_ERROR,               /* 2*/ /* 传输的块长度不允许 或者传输的字节数与块长度不匹配 */
    SD_ERASE_SEQ_ERROR,               /* 3*/ /* 擦除命令序列出错 */
    SD_ERASE_PARAM,                   /* 4*/ /* 擦除的写块选择无效 */
    SD_WP_VIOLATION,                  /* 5*/ /* 尝试对写保护块或永久写保护卡进行编程 */
    SD_LOCK_UNLOCK_FAILED,            /* 6*/ /* 在锁/解锁卡命令过程中发现密码或序列错误 */
    SD_COM_CRC_ERROR,                 /* 7*/ /* 上一个命令的CRC校验失败 */
    SD_ILLEGAL_COMMAND,               /* 8*/ /* 该命令不合法 卡的当前状态不允许执行此命令 */
    SD_CARD_ECC_FAILED,               /* 9*/ /* 卡内部的 ECC（错误更正码）应用失败 未能修正数据 */
    SD_CC_ERROR,                      /*10*/ /* 卡内部控制器错误 */
    SD_GENERAL_UNKNOWN_ERROR,         /*11*/ /* 操作过程中发生的一般或未知错误 */
    SD_CSD_OVERWRITE,                 /*12*/ /* CSD 寄存器的只读部分与卡内容不匹配，或试图反向拷贝或修改永久写保护位 */
    SD_WP_ERASE_SKIP,                 /*13*/ /* 只擦除了部分地址空间，或临时或永久写保护的卡被擦除 */
    SD_CARD_ECC_DISABLED,             /*14*/ /* 命令执行时没有使用内部 ECC */
    SD_ERASE_RESET,                   /*15*/ /* 在执行之前，擦除序列被重置，因为接收到了非擦除序列的命令 */
    SD_AKE_SEQ_ERROR,                 /*16*/ /* 在身份验证过程的序列中出错 */
    
    SD_CMD_CRC_ERROR,                 /*17*/ /* 命令响应接收到的 CRC 校验失败 */
    SD_DATA_CRC_ERROR,                /*18*/ /* 传输的单数据块或多数据块的 CRC 校验失败 */
    SD_CMD_RESP_TIMEOUT,              /*19*/ /* 命令响应超时 */
    SD_DATA_TIMEOUT,                  /*20*/ /* 数据超时 */
    SD_TX_UNDERRUN_ERROR,             /*21*/ /* 传输 FIFO 下溢错误 */
    SD_RX_OVERRUN_ERROR,              /*22*/ /* 接收 FIFO 溢出错误 */
    SD_START_BIT_ERROR,               /*23*/ /* 总线中的起始位错误 */
    
    SD_VOLTRANGE_INVALID,             /*24*/ /* 电压范围无效 */
    SD_PARAMETER_INVALID,             /*25*/ /* 参数无效 */
    SD_OPERATION_IMPROPER,            /*26*/ /* 操作不当 */
    SD_FUNCTION_UNSUPPORTED,          /*27*/ /* 不支持的功能 */
    SD_ERROR,                         /*28*/ /* 发生了一般错误 */
    SD_OK                             /*29*/ /* 没有错误 */
}sd_error_enum;

typedef enum {
    SD_NO_TRANSFER = 0,                     /* no data transfer is acting */
    SD_TRANSFER_IN_PROGRESS                 /* data transfer is in progress */
} sd_transfer_state_enum;

extern uint32_t sd_scr[2];                /* SD card SCR */

// 完整的初始化代码
sd_error_enum BSP_SD_card_init(void);
// 获取SD卡信息并通过USART打印出来
sd_error_enum card_info_get(void);
// 测试SD卡
sd_error_enum sd_test(void);

/* 初始化GD32的SDIO外设 */
sd_error_enum sd_init(void);
/* 获取SD卡片的CID和CSD信息 */
sd_error_enum sd_card_init(void);
/* 配置时钟和工作电压，并获取卡片类型 */
sd_error_enum sd_power_on(void);
/* 关闭SDIO的电源 */
sd_error_enum sd_power_off(void);

/* 配置总线模式 */
sd_error_enum sd_bus_mode_config(uint32_t busmode);
/* 配置传输模式 */
sd_error_enum sd_transfer_mode_config(uint32_t txmode);


/* 从卡片的指定地址读取一个块的数据到缓冲区 */
sd_error_enum sd_block_read(uint32_t *preadbuffer, uint32_t readaddr, uint16_t blocksize);
/* 从卡片的指定地址读取多个块的数据到缓冲区 */
sd_error_enum sd_multiblocks_read(uint32_t *preadbuffer, uint32_t readaddr, uint16_t blocksize, uint32_t blocksnumber);
/* 向卡片的指定地址写入一个块的数据 */
sd_error_enum sd_block_write(uint32_t *pwritebuffer, uint32_t writeaddr, uint16_t blocksize);
/* 向卡片的指定地址写入多个块的数据 */
sd_error_enum sd_multiblocks_write(uint32_t *pwritebuffer, uint32_t writeaddr, uint16_t blocksize, uint32_t blocksnumber);
/* 擦除卡片的连续区域 */
sd_error_enum sd_erase(uint32_t startaddr, uint32_t endaddr);
/* 处理所有已设置标志的中断 */
sd_error_enum sd_interrupts_process(void);


/* 选择或取消选择卡片 */
sd_error_enum sd_card_select_deselect(uint16_t cardrca);
// 使用 CMD13 获取选定卡的状态寄存器
sd_error_enum sd_cardstatus_get(uint32_t *pcardstatus);
/* 获取SD卡状态，SD卡状态的大小为一个512位的数据块 */
sd_error_enum sd_sdstatus_get(uint32_t *psdstatus);
/* 停止正在进行的数据传输 */
sd_error_enum sd_transfer_stop(void);
/* 锁定或解锁卡片 */
sd_error_enum sd_lock_unlock(uint8_t lockstate);

/* 获取数据传输状态 */
sd_transfer_state_enum sd_transfer_state_get(void);
/* 获取SD卡的容量（KB） */
uint32_t sd_card_capacity_get(void);
/* 基于接收到的CID和CSD获取SD卡的详细信息 */
sd_error_enum sd_card_information_get(sd_card_info_struct *pcardinfo);

#endif /* SDCARD_H */
