#ifndef __BSP_SD_CARD_H__
#define __BSP_SD_CARD_H__

#include "stm32f10x.h"                  // Device header

typedef enum {
    SD_OK,                 // 正确
    SD_GENERAL_ERROR,      // 通用错误
    SD_CMD_RSP_TIMEOUT,    // 命令响应超时
    SD_CMD_RSP_ERROR,      // 响应内容错误
    SD_CMD_CRC_FAIL,       // CRC错误
    SD_FLAG_TXUNDERR,      // 发送FIFO下溢错误
} SD_ERROR;

SD_ERROR BSP_SD_card_init(void);
SD_ERROR BSP_SD_Write_Block(uint32_t *pData, uint32_t blockAddr);
SD_ERROR BSP_SD_Read_Block (uint32_t *pData, uint32_t blockAddr);



#endif
