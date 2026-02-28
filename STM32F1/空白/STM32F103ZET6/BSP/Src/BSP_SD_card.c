#include "BSP_SD_card.h"
#include "BSP_Delay.h"                  // Device header
#include <stdio.h>

uint32_t cid[4] = {0};
uint32_t csd[4] = {0};
uint16_t RCA    = {0};

// 命令最大超时
#define SD_CMD_MAX_WAIT_TIME  0xF
#define SDIO_STATIC_FLAGS (SDIO_FLAG_CCRCFAIL | SDIO_FLAG_DCRCFAIL| \
                           SDIO_FLAG_CTIMEOUT | SDIO_FLAG_DTIMEOUT| \
                           SDIO_FLAG_TXUNDERR | SDIO_FLAG_RXOVERR | \
                           SDIO_FLAG_CMDREND  | SDIO_FLAG_CMDSENT | \
                           SDIO_FLAG_DBCKEND)


#define  CMD0  ((uint8_t) 0) // 复位SD卡
#define  CMD2  ((uint8_t) 2) // 
#define  CMD3  ((uint8_t) 3) // 
#define  CMD7  ((uint8_t) 7) // 
#define  CMD8  ((uint8_t) 8) // 检查SD卡支持电压 并判断SD卡标准
#define  CMD9  ((uint8_t) 9) // 
#define  CMD16 ((uint8_t)16) // 
#define  CMD17 ((uint8_t)17) // 
#define  CMD24 ((uint8_t)24) // 
#define  CMD55 ((uint8_t)55) // 

#define ACMD6  ((uint8_t) 6) // 
#define ACMD41 ((uint8_t)41) // 
#define ACMD51 ((uint8_t)51) // 读取 SCR 寄存器

static SD_ERROR BSP_SD_card_GPIO_init(void);
static SD_ERROR BSP_SD_card_SDIO_init(void);
static SD_ERROR SDIO_Send_CMD(uint32_t CMD, uint32_t Argument, uint32_t Response);
static SD_ERROR SDIO_Check_R1_Response(void);
static SD_ERROR SDIO_Check_R1b_Response(void);
static SD_ERROR SDIO_Check_R2_Response(void);
static SD_ERROR SDIO_Check_R3_Response(void);
static SD_ERROR SDIO_Check_R6_Response(void);
static SD_ERROR SDIO_Check_R7_Response(uint8_t CheckPattern);
static SD_ERROR SDIO_Config_HighSpeed_4Bus(void);

#define SDIO_TRANSFER_CLK_DIV            ((uint8_t)0x01) 

/**
  * @brief  初始化SD卡
  * @retval SD_ERROR: 操作结果
  */
SD_ERROR BSP_SD_card_init(void)
{
    SD_ERROR ret;
    uint32_t response;
    // SDIO外设底层引脚初始化
    BSP_SD_card_GPIO_init();
    // SDIO外设初始化
    BSP_SD_card_SDIO_init();

    // 发送CMD0复位SD卡
    SDIO_Send_CMD(CMD0, 0, SDIO_Response_No);
    ret = SDIO_Check_R1_Response();
    if (SD_OK != ret) {
        printf("BSP_SD_card_init SDIO_Set_CMD0 ret %d:\r\n", ret);
        return ret;
    }

    // 发送CMD8获取SD卡标准
    SDIO_Send_CMD(CMD8, 0x1AA, SDIO_Response_Short);
    ret = SDIO_Check_R7_Response(0xAA);
    if (SD_OK != ret) {
        printf("BSP_SD_card_init SDIO_Set_CMD8 ret %d:\r\n", ret);
        return ret;
    }

    int retry = SD_CMD_MAX_WAIT_TIME;
    do {
        // 发送 CMD55 开始应用命令
        SDIO_Send_CMD(CMD55, 0, SDIO_Response_Short);
        ret = SDIO_Check_R1_Response();
        if (SD_OK != ret) {
            printf("BSP_SD_card_init SDIO_Set_CMD55 ret %d:\r\n", ret);
            return ret;
        }
        // 发送 ACMD41 开始检查
        ret = SDIO_Send_CMD(ACMD41, 0xC0100000, SDIO_Response_Short);
        ret = SDIO_Check_R3_Response();
        if (SD_OK != ret) {
            printf("BSP_SD_card_init SDIO_Set_ACMD41 ret %d:\r\n", ret);
            return ret;
        }
        response = SDIO_GetResponse(SDIO_RESP1);
        retry--;
    } while((response & 0x80000000) == 0 && retry);

    // 获取 CID
    SDIO_Send_CMD(CMD2, 0, SDIO_Response_Long);
    ret = SDIO_Check_R2_Response();
    if (SD_OK != ret) {
        printf("BSP_SD_card_init SDIO_Set_CMD2 ret %d:\r\n", ret);
        return ret;
    }
    cid[0] = SDIO_GetResponse(SDIO_RESP1);
    cid[1] = SDIO_GetResponse(SDIO_RESP2);
    cid[2] = SDIO_GetResponse(SDIO_RESP3);
    cid[3] = SDIO_GetResponse(SDIO_RESP4);

    // 获取RCA（相对卡地址）
    SDIO_Send_CMD(CMD3, 0, SDIO_Response_Long);
    ret = SDIO_Check_R6_Response();
    if (SD_OK != ret) {
        printf("BSP_SD_card_init SDIO_Set_CMD3 ret %d:\r\n", ret);
        return ret;
    }
    response = SDIO_GetResponse(SDIO_RESP1);
    RCA = response >> 16;

    // CSD
    SDIO_Send_CMD(CMD9, RCA << 16, SDIO_Response_Long);
    ret = SDIO_Check_R2_Response();
    if (SD_OK != ret) {
        printf("BSP_SD_card_init SDIO_Set_CMD2 ret %d:\r\n", ret);
        return ret;
    }
    csd[0] = SDIO_GetResponse(SDIO_RESP1);
    csd[1] = SDIO_GetResponse(SDIO_RESP2);
    csd[2] = SDIO_GetResponse(SDIO_RESP3);
    csd[3] = SDIO_GetResponse(SDIO_RESP4);

    // 选择卡
    SDIO_Send_CMD(CMD7, RCA << 16, SDIO_Response_Short);
    ret = SDIO_Check_R1b_Response();
    if (ret != SD_OK) {
        printf("BSP_SD_card_init SDIO_Set_CMD7 ret %d:\r\n", ret);
        return ret;
    }

    // 设置块大小
    ret = SDIO_Send_CMD(CMD16, 512, SDIO_Response_Short);
    ret = SDIO_Check_R1_Response();
    if (ret != SD_OK) {
        printf("BSP_SD_card_init SDIO_Set_CMD16 ret %d:\r\n", ret);
        return ret;
    }

//    // 发送 CMD55 开始应用命令
//    SDIO_Send_CMD(CMD55, RCA << 16, SDIO_Response_Short);
//    ret = SDIO_Check_R1_Response();
//    if (SD_OK != ret) {
//        printf("BSP_SD_card_init SDIO_Set_CMD55 ret %d:\r\n", ret);
//        return ret;
//    }
//    // 发送 ACMD6 开始切换四总线模式
//    ret = SDIO_Send_CMD(ACMD6, 0, SDIO_Response_Short);
//    ret = SDIO_Check_R1_Response();
//    if (SD_OK != ret) {
//        printf("BSP_SD_card_init SDIO_Set_ACMD6 ret %d:\r\n", ret);
//        return ret;
//    }

//    ret = SDIO_Config_HighSpeed_4Bus();
//    if (ret != SD_OK) {
//        printf("SDIO_Config_HighSpeed_4Bus NO\r\n");
//        return ret;
//    }

    printf("BSP_SD_card_init OK\r\n");
    return SD_OK;
}

/**
  * @brief  写入SD卡上指定块
  * @param  pData: 存放写入数据的地址
  * @param  blockAddr: 要写入的数据块位置
  * @retval SD_ERROR: 操作结果
  */
SD_ERROR BSP_SD_Write_Block(uint32_t *pData, uint32_t blockAddr)
{
    SD_ERROR ret;

    // CMD24
    SDIO_Send_CMD(CMD24, blockAddr, SDIO_Response_Short);
    ret = SDIO_Check_R1_Response();
    if (ret != SD_OK) {
        printf("BSP_SD_card_init SDIO_Set_CMD24 ret %d:\r\n", ret);
        return ret;
    }

    // 配置数据传输结构体
    SDIO_DataInitTypeDef SDIO_DataInitStruct;
    SDIO_DataStructInit(&SDIO_DataInitStruct);
    // 设置超时周期（例如 0xFFFF）
    SDIO_DataInitStruct.SDIO_DataTimeOut = 0xFFFF;
    // 设置要传输的数据长度（例如 512 字节）
    SDIO_DataInitStruct.SDIO_DataLength = 512;
    // 设置数据块大小（例如 512 字节）
    SDIO_DataInitStruct.SDIO_DataBlockSize = SDIO_DataBlockSize_512b;
    // 设置传输方向为写入卡
    SDIO_DataInitStruct.SDIO_TransferDir = SDIO_TransferDir_ToCard;
    // 设置传输模式为块模式
    SDIO_DataInitStruct.SDIO_TransferMode = SDIO_TransferMode_Block;
    // 启用数据路径状态机
    SDIO_DataInitStruct.SDIO_DPSM = SDIO_DPSM_Enable;
    // 配置 SDIO 数据传输
    SDIO_DataConfig(&SDIO_DataInitStruct);
    
    // 写数据
    while(!(SDIO->STA & (SDIO_FLAG_DCRCFAIL|SDIO_FLAG_DTIMEOUT|SDIO_FLAG_TXUNDERR|SDIO_FLAG_STBITERR|SDIO_FLAG_DBCKEND))) {
        // 发送FIFO半空
        if (SDIO_GetFlagStatus(SDIO_FLAG_TXFIFOHE) != RESET) {
            //写入数据
            SDIO_WriteData(*pData);
            // 移动指针
            pData++;
        }
    }
    // 正在发送数据 
    // 在发送FIFO中的数据可用 
    while(SDIO->STA & (SDIO_FLAG_TXACT | SDIO_FLAG_TXDAVL));

    // 数 据 结 束
    if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET) {
        SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
        return SD_CMD_CRC_FAIL;
    } else
    // 数据超时
    if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET) {
        SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
        return SD_CMD_RSP_TIMEOUT;
    } else
    // 发送FIFO下溢错误
    if (SDIO_GetFlagStatus(SDIO_FLAG_TXUNDERR) != RESET) {
        SDIO_ClearFlag(SDIO_FLAG_TXUNDERR);
        return SD_FLAG_TXUNDERR;
    } else
    // 在宽总线模式，没有在所有数据信号上检测到起始位
    if (SDIO_GetFlagStatus(SD_GENERAL_ERROR) != RESET) {
        SDIO_ClearFlag(SD_GENERAL_ERROR);
        return SD_FLAG_TXUNDERR;
    } else
    // 已发送/接收数据块(CRC检测成功)
    if (SDIO_GetFlagStatus(SDIO_FLAG_DBCKEND) != RESET) {
        SDIO_ClearFlag(SDIO_FLAG_DBCKEND);
        return SD_OK;
    } else {
        printf("SDIO->FIFOCNT:%d\r\n",SDIO->FIFOCNT);
        printf("STA:%x\r\n",SDIO->STA);
        printf("BSP_SD_Write_Block ERROR\r\n");
        return SD_GENERAL_ERROR;
    }
}

/**
  * @brief  读取SD卡上指定块
  * @param  pData: 存放读取到数据的地址
  * @param  blockAddr: 要读取的数据块位置
  * @retval SD_ERROR: 操作结果
  */
SD_ERROR BSP_SD_Read_Block(uint32_t *pData, uint32_t blockAddr) {
    SD_ERROR ret;

    // 发送 CMD17: 读取单个数据块命令
    SDIO_Send_CMD(CMD17, blockAddr, SDIO_Response_Short);
    ret = SDIO_Check_R1_Response();
    if (ret != SD_OK) {
        printf("BSP_SD_Read_Block CMD17 failed, ret %d\n", ret);
        return ret;
    }

    // 配置数据传输结构体
    SDIO_DataInitTypeDef SDIO_DataInitStruct;
    SDIO_DataStructInit(&SDIO_DataInitStruct);
    // 设置超时周期
    SDIO_DataInitStruct.SDIO_DataTimeOut = 0xFFFF;
    SDIO_DataInitStruct.SDIO_DataLength  = 512;  // 512 字节数据
    SDIO_DataInitStruct.SDIO_DataBlockSize = SDIO_DataBlockSize_512b;  // 使用正确的常量
    SDIO_DataInitStruct.SDIO_TransferDir   = SDIO_TransferDir_ToSDIO;  // 从卡读取
    SDIO_DataInitStruct.SDIO_TransferMode  = SDIO_TransferMode_Block;  // 块模式
    SDIO_DataInitStruct.SDIO_DPSM = SDIO_DPSM_Enable;  // 启用数据路径状态机
    // 配置数据传输
    SDIO_DataConfig(&SDIO_DataInitStruct);

    // 读数据
    while (!(SDIO->STA &(SDIO_FLAG_RXOVERR | SDIO_FLAG_DCRCFAIL | SDIO_FLAG_DTIMEOUT | SDIO_FLAG_DBCKEND | SDIO_FLAG_STBITERR))) {
        // 半满读取
        if (SDIO_GetFlagStatus(SDIO_FLAG_RXDAVL) != RESET) {
            //读取数据
            *pData = SDIO_ReadData();
            // 移动指针
            pData++;
        }
    }

    // 已发送/接收数据块(CRC检测失败)
    if (SDIO_GetFlagStatus(SDIO_FLAG_DCRCFAIL) != RESET) {
        SDIO_ClearFlag(SDIO_FLAG_DCRCFAIL);
        return SD_CMD_CRC_FAIL;
    } else
    // 数据超时
    if (SDIO_GetFlagStatus(SDIO_FLAG_DTIMEOUT) != RESET) {
        SDIO_ClearFlag(SDIO_FLAG_DTIMEOUT);
        return SD_CMD_RSP_TIMEOUT;
    } else
    // 正在接收数据
    if (SDIO_GetFlagStatus(SDIO_FLAG_RXOVERR) != RESET) {
        SDIO_ClearFlag(SDIO_FLAG_RXOVERR);
        return SD_GENERAL_ERROR;
    } else
    // 在宽总线模式，没有在所有数据信号上检测到起始位
    if (SDIO_GetFlagStatus(SDIO_FLAG_STBITERR) != RESET) {
        SDIO_ClearFlag(SDIO_FLAG_STBITERR);
        return SD_GENERAL_ERROR;
    } else
    // 已发送/接收数据块(CRC检测成功)
    if (SDIO_GetFlagStatus(SDIO_FLAG_DBCKEND) != RESET) {
        SDIO_ClearFlag(SDIO_FLAG_DBCKEND);
        return SD_OK;
    } else {
        printf("SDIO->FIFOCNT:%d\r\n",SDIO->FIFOCNT);
        printf("STA:%x\r\n",SDIO->STA);
        printf("BSP_SD_Read_Block ERROR\r\n");
        return SD_GENERAL_ERROR;
    }
}

// SDIO_D0  -> PC8
// SDIO_D1  -> PC9
// SDIO_D2  -> PC10
// SDIO_D3  -> PC11
// SDIO_SCK -> PC12
// SDIO_CMD -> PD2
// 初始化SD卡所需GPIO
static SD_ERROR BSP_SD_card_GPIO_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO , ENABLE);

    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_InitStructure);    
    return SD_OK;
}

// 初始化SD卡所需SDIO外设
static SD_ERROR BSP_SD_card_SDIO_init(void)
{
    // 打开 SDIO 时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);

    // 对 SDIO 进行复位
    SDIO_DeInit();
    // 配置SDIO的外设
    SDIO_InitTypeDef SDIO_InitStructure;
    // 初始化时需要配置为单总线模式
    SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;
    // 不使用bypass模式，直接用HCLK进行分频得到SDIO_CK
    SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;
    // SDIO 时钟分频器
    SDIO_InitStructure.SDIO_ClockDiv = 0xFF;
    // 指定 SDIO 在哪个时钟边沿采样数据
    SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
    // 空闲时不关闭时钟电源
    SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
    // 禁止能硬件流控制
    SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
    SDIO_Init(&SDIO_InitStructure);

    /* 开启SDIO外设的电源 */
    SDIO_SetPowerState(SDIO_PowerState_ON);

    /* 使能 SDIO 时钟 */
    SDIO_ClockCmd(ENABLE);
    return SD_OK;
}

/**
  * @brief  发送命令 (CMD)
  * @param  CMD: 应用命令索引
  * @param  Argument: 命令参数
  * @param  Response: 响应类型
  * @retval SD_ERROR: 操作结果
  */
static SD_ERROR SDIO_Send_CMD(uint32_t CMD, uint32_t Argument, uint32_t Response)
{
    // 清除命令相关的所有标志
    SDIO_ClearFlag(SDIO_STATIC_FLAGS);

    SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
    // 要发送的命令
    SDIO_CmdInitStructure.SDIO_CmdIndex = CMD;
    // 要发送的命令携带的参数
    SDIO_CmdInitStructure.SDIO_Argument = Argument;
    // 响应类型
    SDIO_CmdInitStructure.SDIO_Response = Response;
    // 对普通 SD 存储卡 始终用 SDIO_Wait_No
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    // 始终是 SDIO_CPSM_Enable
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);
    return SD_OK;
}

static SD_ERROR SDIO_Check_R1_Response(void)
{
    // 等待命令完成
    uint32_t timeout = SD_CMD_MAX_WAIT_TIME;
    while (timeout--) {
        // 保存状态
        uint32_t sta = SDIO->STA;
        for (volatile int i = 0; i < 100; i++);
        // 无响应命令命令已发送视为成功 有响应命令命令收到CRC正确的响应视为成功
        if (SDIO->STA & (SDIO_FLAG_CMDSENT | SDIO_FLAG_CMDREND)) {
            // 清除相关标志
            SDIO_ClearFlag(SDIO_FLAG_CMDSENT | SDIO_FLAG_CMDREND);
            return SD_OK;
        }
        // 响应CRC校验失败返回失败
        if (SDIO->STA & SDIO_FLAG_CCRCFAIL) {
            // 清除相关标志
            SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
            return SD_CMD_CRC_FAIL;
        }
        // 短延时，避免占用过多CPU
        for (volatile int i = 0; i < 100; i++);
    }
    printf("General timeout\r\n");
    return SD_CMD_RSP_TIMEOUT;
}

static SD_ERROR SDIO_Check_R1b_Response(void)
{
    // 等待命令完成
    uint32_t timeout = SD_CMD_MAX_WAIT_TIME;
    while (timeout--) {
        // 保存状态
        for (volatile int i = 0; i < 100; i++);
        uint32_t sta = SDIO->STA;
        // 响应CRC校验成功返回成功
        if (SDIO->STA & SDIO_FLAG_CMDREND) {
            // 清除相关标志
            SDIO_ClearFlag(SDIO_FLAG_CMDREND);
            return SD_OK;
        }
        // 短延时，避免占用过多CPU
        for (volatile int i = 0; i < 100; i++);
    }
    return SD_CMD_RSP_TIMEOUT;
}

static SD_ERROR SDIO_Check_R2_Response(void)
{
    // 等待命令完成
    uint32_t timeout = SD_CMD_MAX_WAIT_TIME;
    while (timeout--) {
        // 保存状态
        for (volatile int i = 0; i < 100; i++);
        uint32_t sta = SDIO->STA;
        for (volatile int i = 0; i < 100; i++);
        // 响应CRC校验成功返回成功
        if (sta & SDIO_FLAG_CMDREND) {
            // 清除相关标志
            SDIO_ClearFlag(SDIO_FLAG_CMDREND);
            return SD_OK;
        }
        // 短延时，避免占用过多CPU
        for (volatile int i = 0; i < 100; i++);
    }
    return SD_CMD_RSP_TIMEOUT;
}

// R3 无有效 CRC 所以会返回CRC错误
static SD_ERROR SDIO_Check_R3_Response(void)
{
    // 等待命令完成
    uint32_t timeout = SD_CMD_MAX_WAIT_TIME;
    while (timeout--) {
        // 保存状态
        uint32_t sta = SDIO->STA;
        for (volatile int i = 0; i < 100; i++);
        // 响应CRC校验失败返回成功
        if (sta & SDIO_FLAG_CCRCFAIL) {
            // 清除相关标志
            SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
            return SD_OK;
        }
        // 短延时，避免占用过多CPU
        for (volatile int i = 0; i < 100; i++);
    }
    return SD_CMD_RSP_TIMEOUT;
}

static SD_ERROR SDIO_Check_R6_Response(void)
{
    // 等待命令完成
    uint32_t timeout = SD_CMD_MAX_WAIT_TIME;
    while (timeout--) {
        // 保存状态
        for (volatile int i = 0; i < 100; i++);
        uint32_t sta = SDIO->STA;
        for (volatile int i = 0; i < 100; i++);
        // 响应CRC校验失败返回成功
        if (sta & SDIO_FLAG_CCRCFAIL) {
            // 清除相关标志
            SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
            return SD_OK;
        }
        // 短延时，避免占用过多CPU
        for (volatile int i = 0; i < 100; i++);
    }
    return SD_CMD_RSP_TIMEOUT;
}

static SD_ERROR SDIO_Check_R7_Response(uint8_t CheckPattern)
{
    // 等待命令完成
    uint32_t timeout = SD_CMD_MAX_WAIT_TIME;
    while (timeout--) {
        for (volatile int i = 0; i < 100; i++);
        // 保存状态
        uint32_t sta = SDIO->STA;
        // 响应CRC校验成功视为成功
        if (sta & SDIO_FLAG_CMDREND) {
            // 清除相关标志
            SDIO_ClearFlag(SDIO_FLAG_CMDREND);
            break;
        }
        // 响应CRC校验失败返回失败
        if (sta & SDIO_FLAG_CCRCFAIL) {
            // 清除相关标志
            SDIO_ClearFlag(SDIO_FLAG_CCRCFAIL);
            return SD_CMD_CRC_FAIL;
        }
        // 短延时，避免占用过多CPU
        for (volatile int i = 0; i < 100; i++);
    }
    if (timeout == 0) {
        return SD_CMD_RSP_TIMEOUT;
    }
    uint8_t ret = SDIO_GetResponse(SDIO_RESP1)&0xFF;
    if (CheckPattern != ret) {
        printf("OK is %x\r\n",CheckPattern);
        printf("NO is %x\r\n",ret);
        return SD_CMD_RSP_ERROR;
    } else {
        return SD_OK;
    }
}

// 切换到高速4总线模式
static SD_ERROR SDIO_Config_HighSpeed_4Bus(void)
{
    SDIO_InitTypeDef SDIO_InitStructure;
    SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;  // 时钟边沿
    SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;  // 不启用 bypass 模式
    SDIO_InitStructure.SDIO_ClockDiv = 0x01;  // 设置为较高的时钟频率，具体根据系统时钟配置来设置
    SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
    SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
    SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_4b;  // 切换到四总线模式
    SDIO_Init(&SDIO_InitStructure);  // 配置并初始化 SDIO 外设
    return SD_OK;
}
