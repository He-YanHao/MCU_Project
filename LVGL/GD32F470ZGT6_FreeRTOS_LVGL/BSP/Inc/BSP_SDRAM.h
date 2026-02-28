#ifndef EXMC_SDRAM_H
#define EXMC_SDRAM_H

#include "gd32f4xx.h"

#define SDRAM_DEVICE0_ADDR                         ((uint32_t)0xC0000000)
#define SDRAM_DEVICE1_ADDR                         ((uint32_t)0xD0000000)

// 初始化外部同步动态随机存取存储器（SDRAM）外设。
ErrStatus sdram_init(void);

// 将一个 8 位字节缓冲区的数据写入到指定的 SDRAM 存储器中。
void sdram_writebuffer_8(uint8_t* pbuffer, uint32_t writeaddr, uint32_t numbytetowrite);
// 从指定的 SDRAM 存储器中读取 8 位字节数据。
void sdram_readbuffer_8(uint8_t* pbuffer, uint32_t readaddr, uint32_t numbytetoread);

// 将一个 16 位半字缓冲区的数据写入到指定的 SDRAM 存储器中。
void sdram_writebuffer_16(uint16_t* pbuffer, uint32_t writeaddr, uint32_t numtowrite);
// 从指定的 SDRAM 存储器中读取 16 位半字数据。
void sdram_readbuffer_16(uint16_t* pbuffer, uint32_t readaddr, uint32_t numtowrite);

#endif /* EXMC_SDRAM_H */
