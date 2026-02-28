

## 野火

### GPIO_Configuration

```c
/*
* 函数名：GPIO_Configuration
* 描述  ：初始化SDIO用到的引脚，开启时钟。
* 输入  ：无
* 输出  ：无
* 调用  ：内部调用
*/
static void GPIO_Configuration(void)
{
    /*!< 使能 SDIO AHB 时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO, ENABLE);
    /*!< 使能 DMA2 时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
}
```



### SD_DMA_RxConfig

```C
//SDIO_FIOF地址=SDIO地址+0x80至 sdio地址+0xfc
#define SDIO_FIFO_ADDRESS                ((uint32_t)0x40018080)

/*
* 函数名：SD_DMA_RxConfig
* 描述  ：为SDIO接收数据配置DMA2的通道4的请求
* 输入  ：BufferDST：用于装载数据的变量指针
*       : BufferSize： 缓冲区大小
* 输出  ：无
*/
void SD_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize)
{
    DMA_InitTypeDef DMA_InitStructure;

    DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 |
                DMA2_FLAG_HT4 | DMA2_FLAG_GL4);//清除DMA标志位

    /*!< 配置前先禁止DMA */
    DMA_Cmd(DMA2_Channel4, DISABLE);

    /*!< DMA2 传输配置 */
    //外设地址，fifo
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
    //目标地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BufferDST;
    //外设为源地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //除以4，把字转成字节单位
    DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
    //外设地址不自增
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //存储目标地址自增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //外设数据大小为字，32位
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    //外设数据大小为字，32位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    //不循环
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    //通道优先级高
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    //非 存储器至存储器模式
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA2_Channel4, &DMA_InitStructure);

    /*!< 使能DMA通道 */
    DMA_Cmd(DMA2_Channel4, ENABLE);
}
```



```c
/*
* 函数名：SD_DMA_RxConfig
* 描述  ：为SDIO发送数据配置DMA2的通道4的请求
* 输入  ：BufferDST：装载了数据的变量指针
    BufferSize：  缓冲区大小
* 输出  ：无
*/
void SD_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize)
{
    DMA_InitTypeDef DMA_InitStructure;

    DMA_ClearFlag(DMA2_FLAG_TC4 | DMA2_FLAG_TE4 |
                DMA2_FLAG_HT4 | DMA2_FLAG_GL4);

    /*!< 配置前先禁止DMA */
    DMA_Cmd(DMA2_Channel4, DISABLE);

    /*!< DMA2 传输配置 */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SDIO_FIFO_ADDRESS;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)BufferSRC;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//外设为写入目标
    DMA_InitStructure.DMA_BufferSize = BufferSize / 4;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA2_Channel4, &DMA_InitStructure);

    /*!< 使能DMA通道 */
    DMA_Cmd(DMA2_Channel4, ENABLE);
}
```



### NVIC_Configuration

```c
/*
* 函数名：NVIC_Configuration
* 描述  ：SDIO 优先级配置为最高优先级。
* 输入  ：无
* 输出  ：无
*/
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
```

### SD_Init

```c
/**
* 函数名：SD_Init
* 描述  ：初始化SD卡，使卡处于就绪状态(准备传输数据)
* 输入  ：无
* 输出  ：-SD_Error SD卡错误代码
*         成功时则为 SD_OK
* 调用  ：外部调用
*/
SD_Error SD_Init(void)
{
    /*重置SD_Error状态*/
    SD_Error errorstatus = SD_OK;

    NVIC_Configuration();

    /* SDIO 外设底层引脚初始化 */
    GPIO_Configuration();

    /*对SDIO的所有寄存器进行复位*/
    SDIO_DeInit();

    /*上电并进行卡识别流程，确认卡的操作电压  */
    errorstatus = SD_PowerON();

    /*如果上电，识别不成功，返回“响应超时”错误 */
    if (errorstatus != SD_OK) {
        /*!< CMD Response TimeOut (wait for CMDSENT flag) */
        return (errorstatus);
    }

    /*卡识别成功，进行卡初始化    */
    errorstatus = SD_InitializeCards();

    if (errorstatus != SD_OK) {  //失败返回
        /*!< CMD Response TimeOut (wait for CMDSENT flag) */
        return (errorstatus);
    }

    /* 配置SDIO外设
    * 上电识别，卡初始化都完成后，进入数据传输模式，提高读写速度
    */

    /* SDIOCLK = HCLK, SDIO_CK = HCLK/(2 + SDIO_TRANSFER_CLK_DIV) */
    SDIO_InitStructure.SDIO_ClockDiv = SDIO_TRANSFER_CLK_DIV;

    /*上升沿采集数据 */
    SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;

    /* Bypass模式使能的话，SDIO_CK不经过SDIO_ClockDiv分频 */
    SDIO_InitStructure.SDIO_ClockBypass = SDIO_ClockBypass_Disable;

    /* 若开启此功能，在总线空闲时关闭sd_clk时钟 */
    SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;

    /* 暂时配置成1bit模式 */
    SDIO_InitStructure.SDIO_BusWide = SDIO_BusWide_1b;

    /* 硬件流，若开启，在FIFO不能进行发送和接收数据时，数据传输暂停 */
    SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;

    SDIO_Init(&SDIO_InitStructure);

    if (errorstatus == SD_OK) {
        /* 用来读取csd/cid寄存器 */
        errorstatus = SD_GetCardInfo(&SDCardInfo);
    }

    if (errorstatus == SD_OK) {
        /* 通过cmd7  ,rca选择要操作的卡 */
    errorstatus = SD_SelectDeselect((uint32_t) (SDCardInfo.RCA << 16));
    }

    if (errorstatus == SD_OK) {
        /* 最后为了提高读写，开启4bits模式 */
        errorstatus = SD_EnableWideBusOperation(SDIO_BusWide_4b);
    }

    return (errorstatus);
}
```



### SD_PowerON

```c
*
* 函数名：SD_PowerON
* 描述  ：确保SD卡的工作电压和配置控制时钟
* 输入  ：无
* 输出  ：-SD_Error SD卡错误代码
*         成功时则为 SD_OK
* 调用  ：在 SD_Init() 调用
*/
SD_Error SD_PowerON(void)
{
    SD_Error errorstatus = SD_OK;
    uint32_t response = 0, count = 0, validvoltage = 0;
    uint32_t SDType = SD_STD_CAPACITY;
    /****************************************************************/

    /****************************************************************/


    /*检查是否接收到命令*/
    errorstatus = CmdResp7Error();

    /* 有响应则card遵循sd协议2.0版本 */
    if (errorstatus == SD_OK) {
        /* SD Card 2.0 ，先把它定义会sdsc类型的卡 */
        CardType = SDIO_STD_CAPACITY_SD_CARD_V2_0;

        /* 这个变量用作ACMD41的参数，用来询问是sdsc卡还是sdhc卡 */
        SDType = SD_HIGH_CAPACITY;
    } else {  /* 无响应，说明是1.x的或mmc的卡 */
        /* 发命令 CMD55 */
        SDIO_CmdInitStructure.SDIO_Argument = 0x00;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
        SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
        SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
        SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
        SDIO_SendCommand(&SDIO_CmdInitStructure);
        errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
    }

    /* CMD55
    * 发送cmd55，用于检测是sd卡还是mmc卡，或是不支持的卡
    * CMD 响应: R1
    */
    SDIO_CmdInitStructure.SDIO_Argument = 0x00;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    /* 是否响应，没响应的是mmc或不支持的卡 */
    errorstatus = CmdResp1Error(SD_CMD_APP_CMD);
    /*****************************************************************/
    /* 若 errorstatus 为 Command TimeOut, 说明是MMC 卡
    * 若 errorstatus 为 SD_OK ，说明是SD card: SD 卡 2.0 (电压范围不匹配)
    * 或 SD 卡 1.x
    */
    if (errorstatus == SD_OK) { //响应了cmd55，是sd卡，可能为1.x,可能为2.0
        /*下面开始循环地发送sdio支持的电压范围，循环一定次数*/

        /* SD CARD
        * 发送 ACMD41 SD_APP_OP_COND ，带参数 0x80100000
            */
        while ((!validvoltage) && (count < SD_MAX_VOLT_TRIAL)) {
            /* 在发送ACMD命令前都要先向卡发送CMD55
            * 发送 CMD55 APP_CMD ， RCA 为 0
            */
            SDIO_CmdInitStructure.SDIO_Argument = 0x00;
            SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_APP_CMD;
            SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
            SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
            SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
            SDIO_SendCommand(&SDIO_CmdInitStructure);

            errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

            if (errorstatus != SD_OK) {
                return (errorstatus);
            }

            /* ACMD41
        * 命令参数由支持的电压范围及HCS位组成，HCS位置一来区分卡是SDSC还是SDHC
            * 0:SDSC
            * 1:SDHC
            * 响应：R3,对应的是OCR寄存器
            */
            SDIO_CmdInitStructure.SDIO_Argument = SD_VOLTAGE_WINDOW_SD | SDType;
            SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_APP_OP_COND;
            SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
            SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
            SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
            SDIO_SendCommand(&SDIO_CmdInitStructure);

            errorstatus = CmdResp3Error();

            if (errorstatus != SD_OK) {
                return (errorstatus);
            }

            /* 若卡需求电压在SDIO的供电电压范围内，会自动上电并标志pwr_up位
            * 读取卡寄存器，卡状态
            */
            response = SDIO_GetResponse(SDIO_RESP1);

            /* 读取卡的ocr寄存器的pwr_up位，看是否已工作在正常电压 */
            validvoltage = (((response >> 31) == 1) ? 1 : 0);
            count++;        /* 计算循环次数 */
        }

        if (count >= SD_MAX_VOLT_TRIAL) {  /* 循环检测超过一定次数还没上电 */
        errorstatus = SD_INVALID_VOLTRANGE; /* SDIO不支持card的供电电压 */
            return (errorstatus);
        }

        /*检查卡返回信息中的HCS位*/
        /* 判断ocr中的ccs位 ，如果是sdsc卡则不执行下面的语句 */
        if (response &= SD_HIGH_CAPACITY) {
            CardType = SDIO_HIGH_CAPACITY_SD_CARD; /* 把卡类型从初始化的sdsc型改为sdhc型 */
        }

    }/* else MMC Card */

    return (errorstatus);
}
```



### SD_InitializeCards

```c
/*
* 函数名：SD_InitializeCards
* 描述  ：初始化所有的卡或者单个卡进入就绪状态
* 输入  ：无
* 输出  ：-SD_Error SD卡错误代码
*         成功时则为 SD_OK
*调用：在 SD_Init() 调用，在调用power_on（）上电卡识别完毕后，调用此函数进行卡初始化
*/
SD_Error SD_InitializeCards(void)
{
    SD_Error errorstatus = SD_OK;
    uint16_t rca = 0x01;

    if (SDIO_GetPowerState() == SDIO_PowerState_OFF) {
        errorstatus = SD_REQUEST_NOT_APPLICABLE;
        return (errorstatus);
    }

    /* 判断卡的类型 */
    if (SDIO_SECURE_DIGITAL_IO_CARD != CardType) {
        /* Send CMD2 ALL_SEND_CID
        * 响应：R2，对应CID寄存器
        */
        SDIO_CmdInitStructure.SDIO_Argument = 0x0;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_ALL_SEND_CID;
        SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Long;
        SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
        SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
        SDIO_SendCommand(&SDIO_CmdInitStructure);

        errorstatus = CmdResp2Error();

        if (SD_OK != errorstatus) {
            return (errorstatus);
        }

        /* 将返回的CID信息存储起来 */
        CID_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
        CID_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
        CID_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
        CID_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
    }
/********************************************************************/
    if (   (SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType)
        ||(SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType)
        ||(SDIO_SECURE_DIGITAL_IO_COMBO_CARD == CardType)
        ||(SDIO_HIGH_CAPACITY_SD_CARD == CardType) ) { /* 使用的是2.0的卡 */
        /* 发送 CMD3 SET_REL_ADDR ，带参数 0
        * 要求各个SD卡返回自身的RCA地址.
        * 响应：R6，对应RCA寄存器
        */
        SDIO_CmdInitStructure.SDIO_Argument = 0x00;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_REL_ADDR;
        SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
        SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
        SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
        SDIO_SendCommand(&SDIO_CmdInitStructure);

        /* 把接收到的卡相对地址存起来 */
        errorstatus = CmdResp6Error(SD_CMD_SET_REL_ADDR, &rca);

        if (SD_OK != errorstatus) {
            return (errorstatus);
        }
    }
    /*******************************************************************/
    if (SDIO_SECURE_DIGITAL_IO_CARD != CardType) {
        RCA = rca;

        /* Send CMD9 SEND_CSD with argument as card's RCA
        * 响应:R2  对应寄存器CSD(Card-Specific Data)
        */
        SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)(rca << 16);
        SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SEND_CSD;
        SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Long;
        SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
        SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
        SDIO_SendCommand(&SDIO_CmdInitStructure);

        errorstatus = CmdResp2Error();

        if (SD_OK != errorstatus) {
            return (errorstatus);
        }

        CSD_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
        CSD_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
        CSD_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
        CSD_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
    }
    /******************************************************************/
    /*全部卡初始化成功 */
    errorstatus = SD_OK;

    return (errorstatus);
}
```

### SD_Erase

```c
/**
* @brief  控制SD卡擦除指定的数据区域
* @param  startaddr: 擦除的开始地址
* @param  endaddr: 擦除的结束地址
* @retval SD_Error: SD返回的错误代码.
*/
SD_Error SD_Erase(uint32_t startaddr, uint32_t endaddr)
{
    SD_Error errorstatus = SD_OK;
    uint32_t delay = 0;
    __IO uint32_t maxdelay = 0;
    uint8_t cardstate = 0;

    /*!< 检查SD卡是否支持擦除操作 */
    if (((CSD_Tab[1] >> 20) & SD_CCCC_ERASE) == 0) {
        errorstatus = SD_REQUEST_NOT_APPLICABLE;
        return (errorstatus);
    }
    //延时，根据时钟分频设置来计算
    maxdelay = 120000 / ((SDIO->CLKCR & 0xFF) + 2);

    if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED) { //卡已上锁
        errorstatus = SD_LOCK_UNLOCK_FAILED;
        return (errorstatus);
    }

    if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) { //sdhc卡
        //在sdhc卡中，地址参数为块地址，每块512字节，而sdsc卡地址为字节地址
        //所以若是sdhc卡要对地址/512进行转换
        startaddr /= 512;
        endaddr /= 512;
    }

    /*!<  ERASE_GROUP_START (CMD32)设置擦除的起始地址，
    erase_group_end(CMD33)设置擦除的结束地址， */
    if ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) ||
        (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) ||
        (SDIO_HIGH_CAPACITY_SD_CARD == CardType)) {
        /*!< 发送命令 CMD32 SD_ERASE_GRP_START ，带参数startaddr  */
        SDIO_CmdInitStructure.SDIO_Argument = startaddr;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_ERASE_GRP_START;
        SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;  //R1
        SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
        SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
        SDIO_SendCommand(&SDIO_CmdInitStructure);

        errorstatus = CmdResp1Error(SD_CMD_SD_ERASE_GRP_START);
        if (errorstatus != SD_OK) {
            return (errorstatus);
        }

        /*!< 发送命令 CMD33 SD_ERASE_GRP_END ，带参数endaddr  */
        SDIO_CmdInitStructure.SDIO_Argument = endaddr;
        SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SD_ERASE_GRP_END;
        SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
        SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
        SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
        SDIO_SendCommand(&SDIO_CmdInitStructure);

        errorstatus = CmdResp1Error(SD_CMD_SD_ERASE_GRP_END);
        if (errorstatus != SD_OK) {
            return (errorstatus);
        }
    }

    /*!< 发送 CMD38 ERASE 命令，开始擦除*/
    SDIO_CmdInitStructure.SDIO_Argument = 0;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_ERASE;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_ERASE);

    if (errorstatus != SD_OK) {
        return (errorstatus);
    }

    for (delay = 0; delay < maxdelay; delay++) {
    }

    /*!< 等待SD卡的内部时序操作完成 */
    errorstatus = IsCardProgramming(&cardstate);

    while ((errorstatus == SD_OK)&&((SD_CARD_PROGRAMMING == cardstate) ||
                                (SD_CARD_RECEIVING == cardstate))) {
        errorstatus = IsCardProgramming(&cardstate);
    }

    return (errorstatus);
}
```



### SD_WriteBlock

```c
/**
* @brief  向sd卡写入一个BLOCK的数据（512字节）
* @note   本函数使用后需要调用如下两个函数来等待数据传输完成
*          - SD_WaitWriteOperation(): 确认DMA已把数据传输到SDIO接口
*          - SD_GetStatus(): 确认SD卡内部已经把数据写入完毕
* @param  writebuff: 指向要写入的数据
* @param  WriteAddr: 要把数据写入到sd卡的地址
* @param  BlockSize: 块大小，sdhc卡为512字节
* @retval SD_Error: 返回的sd错误代码
*/
SD_Error SD_WriteBlock(uint8_t *writebuff, uint32_t WriteAddr, uint16_t BlockSize)
{
    SD_Error errorstatus = SD_OK;

    TransferError = SD_OK;
    TransferEnd = 0;
    StopCondition = 0;

    SDIO->DCTRL = 0x0;

    if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
        BlockSize = 512;
        WriteAddr /= 512;
    }

    /*----以下这段是在st驱动库上添加的 , 没有这一段容易卡死在DMA检测中 ---*/
    /* 设置块BLOCK的大小，cmd16,
    * 若是sdsc卡，可以用来设置块大小，
    * 若是sdhc卡，块大小为512字节，不受cmd16影响
    */
    SDIO_CmdInitStructure.SDIO_Argument = (uint32_t) BlockSize;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);

    if (SD_OK != errorstatus) {
        return (errorstatus);
    }
    /********************************************************/

    /*!< 发送 CMD24 WRITE_SINGLE_BLOCK 写入 */
    SDIO_CmdInitStructure.SDIO_Argument = WriteAddr;    //写入地址
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_WRITE_SINGLE_BLOCK;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;   //r1
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_WRITE_SINGLE_BLOCK);

    if (errorstatus != SD_OK) {
        return (errorstatus);
    }

    //配置sdio的写数据寄存器
    SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
    SDIO_DataInitStructure.SDIO_DataLength = BlockSize;
    SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) 9 << 4;  //512字节
    SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;//写数据
    SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
    SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;   //开启数据通道状态机
    SDIO_DataConfig(&SDIO_DataInitStructure);

    SDIO_ITConfig(SDIO_IT_DATAEND, ENABLE);  //数据传输结束中断
    SD_DMA_TxConfig((uint32_t *)writebuff, BlockSize); //配置dma，跟rx类似
    SDIO_DMACmd(ENABLE);   // 使能sdio的dma请求

    return (errorstatus);
}
```



### SD_WaitWriteOperation

```c
/**
* @brief  本函数会一直等待到DMA传输结束
*         在 SDIO_WriteBlock() 和 SDIO_WriteMultiBlocks() 函数后必须被调用以确保DMA数据传输完成
* @param  None.
* @retval SD_Error: 返回的sd错误代码.
*/
SD_Error SD_WaitWriteOperation(void)
{
    SD_Error errorstatus = SD_OK;
    //等待dma是否传输结束
    while ((SD_DMAEndOfTransferStatus() == RESET) &&
        (TransferEnd == 0) && (TransferError == SD_OK)) {
    }

    if (TransferError != SD_OK) {
        return (TransferError);
    }

    /*!< 清除标志 */
    SDIO_ClearFlag(SDIO_STATIC_FLAGS);

    return (errorstatus);
}
```



### SD_ReadBlock

```c
/**
* @brief  从sd卡读取一个BLOCK的数据（512字节）
* @note   本函数使用后需要调用如下两个函数来等待数据传输完成
*          - SD_ReadWaitOperation(): 确认DMA已从SDIO传输到数据到内存
*          - SD_GetStatus(): 确认SD卡传输完成
* @param  writebuff: 指向要接收数据的缓冲区
* @param  WriteAddr: 要把数据写入到sd卡的地址
* @param  BlockSize: 块大小，sdhc卡为512字节
* @retval SD_Error: 返回的sd错误代码
*/
SD_Error SD_ReadBlock(uint8_t *readbuff, uint32_t ReadAddr, uint16_t BlockSize)
{
    SD_Error errorstatus = SD_OK;

    TransferError = SD_OK;
    TransferEnd = 0;   //传输结束标置位，在中断服务置1
    StopCondition = 0;

    SDIO->DCTRL = 0x0;


    if (CardType == SDIO_HIGH_CAPACITY_SD_CARD) {
        BlockSize = 512;
        ReadAddr /= 512;
    }
    /*******************add，没有这一段容易卡死在DMA检测中*************/
    /* Set Block Size for Card，cmd16,
    * 若是sdsc卡，可以用来设置块大小，
    * 若是sdhc卡，块大小为512字节，不受cmd16影响
    */
    SDIO_CmdInitStructure.SDIO_Argument = (uint32_t) BlockSize;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_SET_BLOCKLEN;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;   //r1
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);

    if (SD_OK != errorstatus) {
        return (errorstatus);
    }
    /*********************************************************/
    SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
    SDIO_DataInitStructure.SDIO_DataLength = BlockSize;
    SDIO_DataInitStructure.SDIO_DataBlockSize = (uint32_t) 9 << 4;
    SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
    SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
    SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
    SDIO_DataConfig(&SDIO_DataInitStructure);

    /*!< 发送 CMD17 READ_SINGLE_BLOCK 命令*/
    SDIO_CmdInitStructure.SDIO_Argument = (uint32_t)ReadAddr;
    SDIO_CmdInitStructure.SDIO_CmdIndex = SD_CMD_READ_SINGLE_BLOCK;
    SDIO_CmdInitStructure.SDIO_Response = SDIO_Response_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(&SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_READ_SINGLE_BLOCK);

    if (errorstatus != SD_OK) {
        return (errorstatus);
    }

    SDIO_ITConfig(SDIO_IT_DATAEND, ENABLE);
    SDIO_DMACmd(ENABLE);
    SD_DMA_RxConfig((uint32_t *)readbuff, BlockSize);

    return (errorstatus);
}
```

### SD_WaitReadOperation

```c
/**
* @brief  本函数会一直等待到DMA传输结束
*          SDIO_ReadMultiBlocks() 函数后
    必须被调用以确保DMA数据传输完成
* @param  None.
* @retval SD_Error: 返回的sd错误代码.
*/
SD_Error SD_WaitReadOperation(void)
{
    SD_Error errorstatus = SD_OK;
    //等待dma传输结束
    while ((SD_DMAEndOfTransferStatus() == RESET) &&
            (TransferEnd == 0) && (TransferError == SD_OK)) {
    }

    if (TransferError != SD_OK) {
        return (TransferError);
    }

    return (errorstatus);
}
```



### SDIO_IRQHandler

```c
/*
* 函数名：SDIO_IRQHandler
* 描述  ：在SDIO_ITConfig(）这个函数开启了sdio中断 ，
    数据传输结束时产生中断
* 输入  ：无
* 输出  ：无
*/
void SDIO_IRQHandler(void)
{
    /* SDIO中断相关的处理* /
    SD_ProcessIRQSrc();
}

```

### SD_ProcessIRQSrc

```c
/*
* 函数名：SD_ProcessIRQSrc
* 描述  ：数据传输结束中断
* 输入  ：无
* 输出  ：SD错误类型
*/
SD_Error SD_ProcessIRQSrc(void)
{
    if (StopCondition == 1) { //发送读取、多块读写命令时置1
        SDIO->ARG = 0x0;   //命令参数寄存器
        SDIO->CMD = 0x44C;    // 命令寄存器： 0100  01    001100
        //            [7:6]   [5:0]
        //        CPSMEN  WAITRESP CMDINDEX
        //    开启命令状态机 短响应   cmd12 STOP_ TRANSMISSION
        TransferError = CmdResp1Error(SD_CMD_STOP_TRANSMISSION);
    } else {
        TransferError = SD_OK;
    }
    SDIO_ClearITPendingBit(SDIO_IT_DATAEND); //清中断
    SDIO_ITConfig(SDIO_IT_DATAEND, DISABLE); //关闭sdio中断使能
    TransferEnd = 1;
    return (TransferError);
}
```



### SD_Test

```c
void SD_Test(void)
{
    LED_BLUE;
    /*---------------------------- SD Init -------------------------- */
    /* SD卡使用SDIO中断及DMA中断接收数据，中断服务程序位于bsp_sdio_sd.c文件尾

    if ((Status = SD_Init()) != SD_OK) {
        LED_RED;
printf("SD卡初始化失败，请确保SD卡已正确接入开发板，或换一张SD卡测试！\n");
    } else {
        printf("SD卡初始化成功！\n");
    }
    if (Status == SD_OK) {
        LED_BLUE;
        /*擦除测试*/
        SD_EraseTest();

        LED_BLUE;
        /*single block 读写测试*/
        SD_SingleBlockTest();

        LED_BLUE;
    }
}
```



### SD_EraseTest

```c
void SD_EraseTest(void)
{
    /*------------------- 块擦除 -------------------------------*/
    if (Status == SD_OK) {
        /* 擦除 NumberOfBlocks 个块每个块长度为512字节 */
        Status = SD_Erase(0x00, (BLOCK_SIZE * NUMBER_OF_BLOCKS));
    }

    if (Status == SD_OK) {
        Status = SD_ReadMultiBlocks(Buffer_MultiBlock_Rx, 0x00,
                                    BLOCK_SIZE, NUMBER_OF_BLOCKS);

        /* 等待传输完成 */
        Status = SD_WaitReadOperation();

        /* 检查传输是否正常*/
        while (SD_GetStatus() != SD_TRANSFER_OK);
    }

    /* 校验数据 */
    if (Status == SD_OK) {
        EraseStatus = eBuffercmp(Buffer_MultiBlock_Rx, MULTI_BUFFER_SIZE);
    }

    if (EraseStatus == PASSED) {
        LED_GREEN;
        printf("SD卡擦除测试成功！\n");
    } else {
        LED_BLUE;
        printf("SD卡擦除测试失败！\n");
        printf("温馨提示：部分SD卡不支持擦除测试，若SD卡能通过下面的single \
                读写测试，即表示SD卡能够正常使用。\n");
    }
}
```



### SD_SingleBlockTest

```c
void SD_SingleBlockTest(void)
{
    /*------------------- 块 读写 --------------------------*/
    /* 向数组填充要写入的数据*/
    Fill_Buffer(Buffer_Block_Tx, BLOCK_SIZE, 0x320F);

    if (Status == SD_OK) {
        /* 把512个字节写入到SD卡的0地址 */
        Status = SD_WriteBlock(Buffer_Block_Tx, 0x00, BLOCK_SIZE);
        /* 检查传输 */
        Status = SD_WaitWriteOperation();
        while (SD_GetStatus() != SD_TRANSFER_OK);
    }
    if (Status == SD_OK) {
        /* 从SD卡的0地址读取512个字节 */
        Status = SD_ReadBlock(Buffer_Block_Rx, 0x00, BLOCK_SIZE);
        /* 检查传输 */
        Status = SD_WaitReadOperation();
        while (SD_GetStatus() != SD_TRANSFER_OK);
    }
    /* 校验读出的数据是否与写入的数据一致 */
    if (Status == SD_OK) {
        TransferStatus1 = Buffercmp(Buffer_Block_Tx,
                                    Buffer_Block_Rx, BLOCK_SIZE);
    }
    if (TransferStatus1 == PASSED) {
        LED_GREEN;
        printf("Single block 测试成功！\n");
    } else {
    LED_RED;
    printf("Single block 测试失败，请确保SD卡正确接入开发板，或换一张SD卡测试！\n");
    }
}
```

### main

```c
int main(void)
{
    /* 初始化LED灯 */
    LED_GPIO_Config();
    LED_BLUE;
    /* 初始化独立按键 */
    Key_GPIO_Config();

    /*初始化USART1*/
    USART_Config();

    printf("\r\n欢迎使用野火  STM32 开发板。\r\n");

    printf("在开始进行SD卡基本测试前，请给开发板插入32G以内的SD卡\r\n");
    printf("本程序会对SD卡进行 非文件系统 方式读写，会删除SD卡的文件系统\r\n");
    printf("实验后可通过电脑格式化或使用SD卡文件系统的例程恢复SD卡文件系统\r\n");
    printf("\r\n 但sd卡内的原文件不可恢复，实验前务必备份SD卡内的原文件！！！\r\n");

    printf("\r\n 若已确认，请按开发板的KEY1按键，开始SD卡测试实验....\r\n");

    /* Infinite loop */
    while (1) {
        /*按下按键开始进行SD卡读写实验，会损坏SD卡原文件*/
        if (  Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON) {
            printf("\r\n开始进行SD卡读写实验\r\n");
            SD_Test();
        }
    }
}
```



## SDIO

### 初始化/复位

```c
// 将 SDIO 外设寄存器恢复到默认复位状态，相当于“清空并重置 SDIO”。
void SDIO_DeInit(void);
// 根据 SDIO_InitTypeDef 结构体配置 SDIO 外设（如时钟边沿、总线宽度、时钟分频等）。
void SDIO_Init(SDIO_InitTypeDef* SDIO_InitStruct);
// 将 SDIO_InitTypeDef 结构体填充为默认值，便于后续修改和初始化。
void SDIO_StructInit(SDIO_InitTypeDef* SDIO_InitStruct);
// 启动或关闭 SDIO 时钟输出。
void SDIO_ClockCmd(FunctionalState NewState);
```

### 电源管理

```c
// 设置 SDIO 的电源状态（开/关/低功耗等）。
void SDIO_SetPowerState(uint32_t SDIO_PowerState);
// 获取当前 SDIO 电源状态。
uint32_t SDIO_GetPowerState(void);
```

### 中断/DMA

```c
// 启用或禁用指定的 SDIO 中断源（如命令完成、中断响应、数据传输完成等）。
void SDIO_ITConfig(uint32_t SDIO_IT, FunctionalState NewState);
// 启用或禁用 SDIO 数据传输的 DMA 功能。
void SDIO_DMACmd(FunctionalState NewState);
```

### 命令操作

```c
// 根据 SDIO_CmdInitTypeDef 结构体发送 SD/SDIO 命令（如 CMD0, CMD8, CMD17 等）。
void SDIO_SendCommand(SDIO_CmdInitTypeDef *SDIO_CmdInitStruct);
// 将 SDIO_CmdInitTypeDef 结构体填充为默认值，方便配置命令参数。
void SDIO_CmdStructInit(SDIO_CmdInitTypeDef* SDIO_CmdInitStruct);
// 获取 SDIO 命令响应状态（比如 CMD 成功或错误）。
uint8_t SDIO_GetCommandResponse(void);
// 获取 SDIO 响应值（R1~R7），SDIO_RESP 指定要读的响应寄存器。
uint32_t SDIO_GetResponse(uint32_t SDIO_RESP);
```

### 数据传输

```c
// 配置 SDIO 数据传输参数（块大小、传输方向、总线宽度、超时等）。
void SDIO_DataConfig(SDIO_DataInitTypeDef* SDIO_DataInitStruct);
// 初始化 SDIO_DataInitTypeDef 结构体为默认值。
void SDIO_DataStructInit(SDIO_DataInitTypeDef* SDIO_DataInitStruct);
// 获取当前剩余要传输的数据字数。
uint32_t SDIO_GetDataCounter(void);
// 从 SDIO FIFO 读取 32 位数据。
uint32_t SDIO_ReadData(void);
// 向 SDIO FIFO 写入 32 位数据。
void SDIO_WriteData(uint32_t Data);
// 获取 FIFO 中当前数据字数（以 32bit 计）。
uint32_t SDIO_GetFIFOCount(void);
```

### 特殊功能

```c
// 启动或停止 SDIO 读等待功能（用于多功能 SDIO 卡）。
void SDIO_StartSDIOReadWait(FunctionalState NewState);
// 停止 SDIO 读等待功能（同上）。
void SDIO_StopSDIOReadWait(FunctionalState NewState);
// 设置读等待模式（CLK 或 DATA 模式）。
void SDIO_SetSDIOReadWaitMode(uint32_t SDIO_ReadWaitMode);
// 启动或停止 SDIO 卡操作功能（多功能卡命令处理）。
void SDIO_SetSDIOOperation(FunctionalState NewState);
// 向 SDIO 卡发送挂起命令，暂停当前操作。
void SDIO_SendSDIOSuspendCmd(FunctionalState NewState);
// 启用或禁用命令完成信号检测。
void SDIO_CommandCompletionCmd(FunctionalState NewState);
// 启用或禁用 CE-ATA 卡中断信号检测。
void SDIO_CEATAITCmd(FunctionalState NewState);
// 向 CE-ATA 卡发送特定命令。
void SDIO_SendCEATACmd(FunctionalState NewState);
```

### 状态/标志

```c
// 获取指定 SDIO 状态标志（如 CMD 完成、数据完成、FIFO 空/满等）。
FlagStatus SDIO_GetFlagStatus(uint32_t SDIO_FLAG);
// 清除指定状态标志。
void SDIO_ClearFlag(uint32_t SDIO_FLAG);
// 获取指定中断标志的状态（是否触发）。
ITStatus SDIO_GetITStatus(uint32_t SDIO_IT);
// 清除指定中断标志位，避免重复触发。
void SDIO_ClearITPendingBit(uint32_t SDIO_IT);
```

