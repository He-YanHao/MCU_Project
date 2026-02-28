#include "BSP_SDRAM.h"
#include "gd32f4xx.h"
#include "systick.h"

// SDRAM 在进行读取或写入操作时，数据是以突发（Burst）模式传输的.
// 突发长度决定了每次读取或写入多少个连续的数据单元。
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0003)

/* 突发类型 burst type */
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)

/* CAS 延迟 CAS latency */
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)

/* 写模式 write mode */
// 程序化写突发模式。在该模式下，数据写入按照指定的顺序进行。
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
// 单次写入模式。在此模式下，每个突发操作只写入一个数据单元。
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

// 标准操作模式。
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)

// 超时
#define SDRAM_TIMEOUT                            ((uint32_t)0x0000FFFF)

/**
* @brief 初始化sdram
 * 
 * @param sdram_device 选择 4 对应 SDRAM_DEVICE0_ADDR  或者 非4 对应 SDRAM_DEVICE1_ADDR
 * @return ErrStatus 
 */
ErrStatus sdram_init(void)
{
    /* enable EXMC clock*/
    // 开启相关GPIO的时钟
    rcu_periph_clock_enable(RCU_EXMC);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_GPIOG);
    rcu_periph_clock_enable(RCU_GPIOH);

    /* common GPIO configuration */
    /* SDNWE(PC0),SDNE0(PC2),SDCKE0(PC3) pin configuration */ 
    gpio_af_set(GPIOC, GPIO_AF_12, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3);

    /* D2(PD0),D3(PD1),D13(PD8),D14(PD9),D15(PD10),D0(PD14),D1(PD15) pin configuration */
    gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_8 | GPIO_PIN_9 |
                                   GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_8 | GPIO_PIN_9 |
                                                         GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_8 | GPIO_PIN_9 |
                                                                     GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15);

    /* NBL0(PE0),NBL1(PE1),D4(PE7),D5(PE8),D6(PE9),D7(PE10),D8(PE11),D9(PE12),D10(PE13),D11(PE14),D12(PE15) pin configuration */
    gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_7  | GPIO_PIN_8 |
                                   GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |
                                   GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_7  | GPIO_PIN_8 |
                                                         GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |
                                                         GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_7  | GPIO_PIN_8 |
                                                                     GPIO_PIN_9  | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |
                                                                     GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

    /* A0(PF0),A1(PF1),A2(PF2),A3(PF3),A4(PF4),A5(PF5),NRAS(PF11),A6(PF12),A7(PF13),A8(PF14),A9(PF15) pin configuration */
    gpio_af_set(GPIOF, GPIO_AF_12, GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3  |
                                   GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_11 | GPIO_PIN_12 |
                                   GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3  |
                                                         GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_11 | GPIO_PIN_12 |
                                                         GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_2  | GPIO_PIN_3  |
                                                                     GPIO_PIN_4  | GPIO_PIN_5  | GPIO_PIN_11 | GPIO_PIN_12 |
                                                                     GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

    /* A10(PG0),A11(PG1),A12(PG2),A14(PG4),A15(PG5),SDCLK(PG8),NCAS(PG15) pin configuration */
    gpio_af_set(GPIOG, GPIO_AF_12, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | 
                                   GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15);
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | 
                                                         GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | 
                                                                     GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15);


    /* EXMC SDRAM 设备初始化序列 --------------------------------*/
    /* 步骤 1：配置 SDRAM 定时寄存器 --------------------------------*/
    // 用于配置初始化的结构体
    exmc_sdram_timing_parameter_struct  sdram_timing_init_struct;
    /* LMRD: 2 clock cycles */
    sdram_timing_init_struct.load_mode_register_delay = 2;
    /* XSRD: min = 75ns */
    sdram_timing_init_struct.exit_selfrefresh_delay = 9;  
    /* RASD: min=44ns , max=120k (ns) */
    sdram_timing_init_struct.row_address_select_delay = 5;  
    /* ARFD: min=66ns */
    sdram_timing_init_struct.auto_refresh_delay = 8;  
    /* WRD:  min=1 Clock cycles +7.5ns */
    sdram_timing_init_struct.write_recovery_delay = 3;
    /* RPD:  min=20ns */
    sdram_timing_init_struct.row_precharge_delay = 3; 
    /* RCD:  min=20ns */
    sdram_timing_init_struct.row_to_column_delay = 3; 


    // 用于配置初始化的结构体
    exmc_sdram_parameter_struct        sdram_init_struct;
    /* step 2 : configure SDRAM control registers ---------------------------------*/
    /* 步骤 2：配置 SDRAM 控制寄存器 ---------------------------------*/
    sdram_init_struct.sdram_device = EXMC_SDRAM_DEVICE0;
    sdram_init_struct.column_address_width = EXMC_SDRAM_COW_ADDRESS_9;
    sdram_init_struct.row_address_width = EXMC_SDRAM_ROW_ADDRESS_13;
    sdram_init_struct.data_width = EXMC_SDRAM_DATABUS_WIDTH_16B;
    sdram_init_struct.internal_bank_number = EXMC_SDRAM_4_INTER_BANK;
    sdram_init_struct.cas_latency = EXMC_CAS_LATENCY_3_SDCLK;  
    sdram_init_struct.write_protection = DISABLE;
    sdram_init_struct.sdclock_config = EXMC_SDCLK_PERIODS_2_HCLK;  
    sdram_init_struct.burst_read_switch = ENABLE;
    sdram_init_struct.pipeline_read_delay = EXMC_PIPELINE_DELAY_2_HCLK;
    sdram_init_struct.timing  = &sdram_timing_init_struct;
    /* EXMC SDRAM bank initialization */
    exmc_sdram_init(&sdram_init_struct);


    // 用于配置初始化的结构体
    exmc_sdram_command_parameter_struct     sdram_command_init_struct;
    /* step 3 : configure CKE high command---------------------------------------*/
    /* 步骤 3：配置 CKE 高命令 ---------------------------------------*/
    sdram_command_init_struct.command = EXMC_SDRAM_CLOCK_ENABLE;
    sdram_command_init_struct.bank_select = EXMC_SDRAM_DEVICE0_SELECT;
    sdram_command_init_struct.auto_refresh_number = EXMC_SDRAM_AUTO_REFLESH_2_SDCLK;
    sdram_command_init_struct.mode_register_content = 0;
    
    /* wait until the SDRAM controller is ready */
    /* 等待直到 SDRAM 控制器准备好 */
    uint32_t timeout = SDRAM_TIMEOUT; // SDRAM_TIMEOUT = ((uint32_t)0x0000FFFF)
    while((exmc_flag_get(EXMC_SDRAM_DEVICE0, EXMC_SDRAM_FLAG_NREADY) != RESET) && (timeout > 0))
    {
        timeout--;
    }
    if(0 == timeout)
    {
        return ERROR;
    }

    /* send the command */
    /* 发送命令 */
    exmc_sdram_command_config(&sdram_command_init_struct);

    /* 延时等待 */
    for(uint32_t i = 0; i < 0xfffff; i++) {
        __NOP();
    }

    /* step 5 : configure precharge all command----------------------------------*/
    // sdram_command_init_struct.command = EXMC_SDRAM_AUTO_REFRESH;
    sdram_command_init_struct.command = EXMC_SDRAM_PRECHARGE_ALL;
    sdram_command_init_struct.bank_select = EXMC_SDRAM_DEVICE0_SELECT;
    sdram_command_init_struct.auto_refresh_number = EXMC_SDRAM_AUTO_REFLESH_9_SDCLK;
    sdram_command_init_struct.mode_register_content = 0;
    /* wait until the SDRAM controller is ready */
    timeout = SDRAM_TIMEOUT; 
    while((exmc_flag_get(EXMC_SDRAM_DEVICE0, EXMC_SDRAM_FLAG_NREADY) != RESET) && (timeout > 0))
    {
        timeout--;
    }
    if(0 == timeout)
    {
        return ERROR;
    }
    /* send the command */
    exmc_sdram_command_config(&sdram_command_init_struct);

    /* step 6 : configure Auto-Refresh command-----------------------------------*/
    sdram_command_init_struct.command = EXMC_SDRAM_AUTO_REFRESH;
    sdram_command_init_struct.bank_select = EXMC_SDRAM_DEVICE0_SELECT;
    sdram_command_init_struct.auto_refresh_number = EXMC_SDRAM_AUTO_REFLESH_9_SDCLK;
    sdram_command_init_struct.mode_register_content = 0;
    /* wait until the SDRAM controller is ready */ 
    timeout = SDRAM_TIMEOUT; 
    while((exmc_flag_get(EXMC_SDRAM_DEVICE0, EXMC_SDRAM_FLAG_NREADY) != RESET) && (timeout > 0))
    {
        timeout--;
    }
    if(0 == timeout)
    {
        return ERROR;
    }    
    /* send the command */
    exmc_sdram_command_config(&sdram_command_init_struct);

    /* step 7 : configure load mode register command-----------------------------*/
    /* program mode register */
    uint32_t command_content = 0;
    command_content = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
                                SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                                SDRAM_MODEREG_CAS_LATENCY_3           |
                                SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                                SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

    sdram_command_init_struct.command = EXMC_SDRAM_LOAD_MODE_REGISTER;
    sdram_command_init_struct.bank_select = EXMC_SDRAM_DEVICE0_SELECT;
    sdram_command_init_struct.auto_refresh_number = EXMC_SDRAM_AUTO_REFLESH_2_SDCLK;
    sdram_command_init_struct.mode_register_content = command_content;

    /* wait until the SDRAM controller is ready */ 
    timeout = SDRAM_TIMEOUT; 
    while((exmc_flag_get(EXMC_SDRAM_DEVICE0, EXMC_SDRAM_FLAG_NREADY) != RESET) && (timeout > 0)){
        timeout--;
    }
    if(0 == timeout){
        return ERROR;
    }    
    /* send the command */
    exmc_sdram_command_config(&sdram_command_init_struct);

    /* step 8 : set the auto-refresh rate counter--------------------------------*/
    /* 64ms, 8192-cycle refresh, 64ms/8192=7.81us */
    /* SDCLK_Freq = SYS_Freq/2 */
    /* (7.81 us * SDCLK_Freq) - 20 */
    exmc_sdram_refresh_count_set(761); 

    /* wait until the SDRAM controller is ready */ 
    timeout = SDRAM_TIMEOUT; 
    while((exmc_flag_get(EXMC_SDRAM_DEVICE0, EXMC_SDRAM_FLAG_NREADY) != RESET) && (timeout > 0))
    {
        timeout--;
    }
    if(0 == timeout)
    {
        return ERROR;
    }
    return SUCCESS;    
}

/*!
    \brief      将字节缓冲区（数据为 8 位）写入 EXMC SDRAM 内存
    \param[in]  pbuffer: 指向缓冲区的指针
    \param[in]  writeaddr: 从该地址开始写入数据的 SDRAM 内部地址
    \param[in]  numbytetowrite: 要写入的字节数
    \param[out] none
    \retval     none
*/
void sdram_writebuffer_8(uint8_t* pbuffer, uint32_t writeaddr, uint32_t numbytetowrite)
{
    if (writeaddr + numbytetowrite > 0xC2000000 || writeaddr < SDRAM_DEVICE0_ADDR) {
        return;
    }

    /* 当还有数据要写入时 */
    for(; numbytetowrite != 0; numbytetowrite--)
    {
        /* 将数据传输到内存 */
        // 将 pbuffer 指向的数据 按照 (uint8_t*) 存入 (SDRAM_DEVICE0_ADDR + writeaddr) 位置
        *(uint8_t *)writeaddr = *pbuffer++;
        /* 增加地址 */
        writeaddr += 1;
    }
}

/*!
    \brief      从 EXMC SDRAM 内存中读取一块 8 位数据
    \param[in]  pbuffer: 指向缓冲区的指针
    \param[in]  readaddr: 从该地址开始读取数据的 SDRAM 内部地址
    \param[in]  numbytetoread: 要读取的字节数
    \param[out] none
    \retval     none
*/
void sdram_readbuffer_8(uint8_t* pbuffer, uint32_t readaddr, uint32_t numbytetoread)
{
    if (readaddr + numbytetoread > 0xC2000000 || readaddr < SDRAM_DEVICE0_ADDR) {
        return;
    }

    /* while there is data to read */
    for(; numbytetoread != 0; numbytetoread--)
    {
        /* read a byte from the memory */
        *pbuffer++ = *(uint8_t*)readaddr;
        /* increment the address */
        readaddr += 1;
    }
}

/*!
    \brief      将字节缓冲区（数据为 16 位）写入 EXMC SDRAM 内存
    \param[in]  pbuffer: 指向缓冲区的指针
    \param[in]  writeaddr: 从该地址开始写入数据的 SDRAM 内部地址
    \param[in]  numbytetowrite: 要写入的字节数
    \param[out] none
    \retval     none
*/
void sdram_writebuffer_16(uint16_t* pbuffer, uint32_t writeaddr, uint32_t numtowrite)
{
    if (writeaddr + 2 * numtowrite > 0xC2000000 || writeaddr < SDRAM_DEVICE0_ADDR) {
        return;
    }

    __IO uint32_t write_addr_prt = writeaddr;

    /* While there is data to write */
    for(; numtowrite != 0; numtowrite--) {
        /* Transfer data to the memory */
        *(uint16_t *) write_addr_prt = *pbuffer++;
        /* Increment the address */  
        write_addr_prt += 2;
    }
}

/*!
    \brief      从 EXMC SDRAM 内存中读取一块 16 位数据
    \param[in]  pbuffer: 指向缓冲区的指针
    \param[in]  readaddr: 从该地址开始读取数据的 SDRAM 内部地址
    \param[in]  numbytetoread: 要读取的字节数
    \param[out] none
    \retval     none
*/
void sdram_readbuffer_16(uint16_t* pbuffer, uint32_t readaddr, uint32_t numtowrite)
{
    if (readaddr + 2 * numtowrite > 0xC2000000 || readaddr < SDRAM_DEVICE0_ADDR) {
        return;
    }

    __IO uint32_t write_addr_prt = readaddr;

    /* while there is data to read */
    for(; numtowrite != 0; numtowrite--){
        /* read a byte from the memory */
        *pbuffer++ = *(uint16_t*)write_addr_prt;
        /* increment the address */
        write_addr_prt += 2;
    }
}


//void sdram_test(void)
//{
//    uint16_t Write[128] = {0};
//    uint16_t Read [128] = {0};
//    for(uint8_t i = 0; i < 128; i++) {
//        Write[i] = i;
//    }
//    uint32_t sdram_addr = SDRAM_DEVICE0_ADDR;
//    do {
//        printf("sdram test addr is %x\r\n",sdram_addr);
//        sdram_writebuffer_16(Write, sdram_addr, 128);
//        memset(Read, 0, sizeof(Read));
//        sdram_readbuffer_16(Read, sdram_addr, 128);
//        sdram_addr += 0x100000;
//        Delay_ms(1000);
//    } while(memcmp(Write, Read, sizeof(Read)) == 0);
//    printf("sdram addr max is %x\r\n",sdram_addr);
//}
