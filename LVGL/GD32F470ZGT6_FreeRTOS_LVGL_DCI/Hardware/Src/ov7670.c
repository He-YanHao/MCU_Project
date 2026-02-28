#include "ov7670.h"
#include "gd32f4xx.h"                   // Device header
#include "Delay.h"
#include "stdio.h"
#include "lcd_st7789.h"

// 拉高
#define	I2C_SDA_SET   GPIO_BOP(GPIOD)=GPIO_PIN_4
#define	I2C_SCL_SET   GPIO_BOP(GPIOD)=GPIO_PIN_5
#define	OV_PWDN_SET   GPIO_BOP(GPIOG)=GPIO_PIN_7
#define	OV_RET_SET    GPIO_BOP(GPIOG)=GPIO_PIN_6
// 拉低
#define	I2C_SDA_CLR   GPIO_BC(GPIOD)=GPIO_PIN_4
#define	I2C_SCL_CLR   GPIO_BC(GPIOD)=GPIO_PIN_5
#define	OV_PWDN_CLR   GPIO_BC(GPIOG)=GPIO_PIN_7
#define	OV_RET_CLR    GPIO_BC(GPIOG)=GPIO_PIN_6
// 读
#define	I2C_SDA_GET   gpio_input_bit_get(GPIOD,GPIO_PIN_4)		
// 
#define	SDA_OUT       gpio_mode_set(GPIOD,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,GPIO_PIN_4)
#define	SDA_IN        gpio_mode_set(GPIOD,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,GPIO_PIN_4)

extern __attribute__((section(".sdram.lcd_fb"),aligned(2)))
uint16_t lcd_fb[LCD_WIDTH * LCD_HEIGHT];
// 临时缓冲存 8bit 数据
uint8_t dma_temp_buffer[LCD_WIDTH*LCD_HEIGHT*2]; // 每像素2字节

static void write_reg(uint8_t reg, uint8_t data);
static uint8_t read_reg(uint8_t reg);
static void ov7670_gpio_config(void);
static void OV_XLK_Init(void);
static void ov7670_register_config(void);
static void ov7670_DMA_DCI_config(void);

void OV7670_Init(void)
{
    ov7670_gpio_config();
    OV_XLK_Init();
    Delay_ms(20);
    OV_PWDN_CLR; // 正常模式 非节能模式
    OV_RET_CLR;  // 拉低 复位 清除配置
    Delay_ms(10);
    OV_RET_SET;  // 拉高 正常工作
    Delay_ms(100);
    ov7670_register_config();
    ov7670_DMA_DCI_config();
}

void OV7670_read_pid_ver(void)
{
    uint8_t pid = read_reg(0x0A);
    uint8_t ver = read_reg(0x0B);
    printf("PID=0x%02X VER=0x%02X\r\n", pid, ver);
}

static void ov7670_gpio_config(void)
{
    // 时钟初始化
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOG);
    
    // I2C接口 初始化
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_4 | GPIO_PIN_5);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_5);

    // PWDN RET 接口初始化
    gpio_mode_set(GPIOG, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7);
    
    // PWM时钟 接口初始化
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX,GPIO_PIN_8);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_8);

    // DCI接口 初始化
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4 | GPIO_PIN_6);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_4 | GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_3);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11);
    gpio_af_set(GPIOA, GPIO_AF_13, GPIO_PIN_4 | GPIO_PIN_6);
    gpio_af_set(GPIOC, GPIO_AF_13, GPIO_PIN_6 | GPIO_PIN_7);
    gpio_af_set(GPIOD, GPIO_AF_13, GPIO_PIN_3);
    gpio_af_set(GPIOE, GPIO_AF_13, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
    gpio_af_set(GPIOG, GPIO_AF_13, GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11);
}

static void OV_XLK_Init(void)
{
    rcu_periph_clock_enable(RCU_TIMER0);//使能时钟
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);//配置TIMER时钟源
    timer_deinit(TIMER0);//缺省值初始化
    timer_parameter_struct timer_initpara;//结构体
    timer_struct_para_init(&timer_initpara);//默认值

    timer_initpara.prescaler = 0;//预分频值（0~65535）
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;//对齐模式
    timer_initpara.counterdirection = TIMER_COUNTER_UP;//计数方向
    timer_initpara.clockdivision = TIMER_CKDIV_DIV2;//时钟分频因子
    timer_initpara.period = 20 - 1;//周期（0~65535） 自动重载值
    timer_initpara.repetitioncounter = 1;//重复计数器值（0~255）
    timer_init(TIMER0,&timer_initpara);

    timer_oc_parameter_struct timer_ocintpara;
    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;//通道输出极性
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;//通道输出状态
    timer_ocintpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;//互补通道输出极性
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;//互补通道输出状态
    timer_ocintpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;//空闲状态下通道输出
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;//空闲状态下互补通道输出
    //初始化0通道
    timer_channel_output_config(TIMER0,TIMER_CH_0,&timer_ocintpara);

    //配置通道TIMER0_CH0
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, 10);                //输出比较寄存器
    timer_channel_output_mode_config(TIMER0,TIMER_CH_0,TIMER_OC_MODE_TOGGLE);         //配置外设TIMERx通道输出比较模式
    timer_channel_output_shadow_config(TIMER0,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);  //配置TIMERx通道输出比较影子寄存器功能

    timer_primary_output_config(TIMER0, ENABLE);//所有的通道输出使能
    timer_auto_reload_shadow_enable(TIMER0);//TIMERx自动重载影子使能
    timer_enable(TIMER0);//使能定时器
}

static void ov7670_register_config(void)
{
    // 1. 软件复位
    write_reg(0x12, 0x80);   // COM7[7]=1 复位所有寄存器
    Delay_ms(50);            // 等待复位完成

    // 2. 时钟配置
    //   假设XCLK输入为12MHz，目标PCLK约6-12MHz
    write_reg(0x11, 0x01);   // CLKRC[5:0] = 1 => 分频系数 = 1+1=2, PCLK = XCLK/2 = 6MHz (若XCLK=12MHz)
    write_reg(0x6B, 0x4A);   // PLL倍频系数 (DSPLL[7:0] = 0x4A => 4倍频)
    //   注意：PLL使能需与COM11配合，此处保持默认

    // 3. 图像格式与尺寸
    write_reg(0x12, 0x14);   // COM7: RGB565 (bit2=1), QVGA (bit4=0, bit3=1? 实际上0x14=0001 0100: bit4=0 QVGA, bit2=1 RGB)
    //   参考手册：COM7[3:2] 00=YUV, 01=RGB, 10=Raw RGB, bit4=1 VGA, 0 QVGA
    //   0x14 = 0001 0100 => bit4=0 QVGA, bit2=1 RGB

    write_reg(0x40, 0xD0);   // COM15: 输出范围 00-FF, RGB565 (bit5=1, bit4=0 => RGB565)
    write_reg(0x3A, 0x04);   // TSLB[2]: 输出顺序 RGB (根据硬件接线可能需要调整字节序，此处设0x04使字节顺序正确)

    // 4. 关闭可能干扰的测试模式/特殊模式
    write_reg(0x0C, 0x00);   // COM3: 默认 (bit3=0 禁止缩放)
    write_reg(0x3E, 0x00);   // COM14: 禁止缩放和PCLK分频
//    write_reg(0x70, 0x3A);   // SCALING_XSC: 水平缩放系数 (默认)
    write_reg(0x70, 0xBA);   // SCALING_XSC: 水平缩放系数 (默认)
//    write_reg(0x71, 0x35);   // SCALING_YSC: 垂直缩放系数 (默认)
    write_reg(0x71, 0xB5);   // SCALING_YSC: 垂直缩放系数 (默认)
    write_reg(0x72, 0x11);   // SCALING_DCWCTR: 缩放控制 (默认)
    write_reg(0x73, 0xF0);   // SCALING_PCLK_DIV: PCLK分频 (默认)
    write_reg(0xA2, 0x02);   // SCALING_PCLK_DELAY: PCLK延时 (默认)

    // 5. 设置窗口 (QVGA 320x240)
    //   以下值根据OV7670数据手册和常见驱动得来，确保有效图像区域正确
    write_reg(0x17, 0x16);   // HSTART[7:0] 水平起始高8位
    write_reg(0x18, 0x04);   // HSTOP[7:0]  水平停止高8位
    write_reg(0x32, 0x80);   // HREF[9:2]   图像宽度控制 (配合HSTART/HSTOP)
    write_reg(0x19, 0x02);   // VSTART[7:0] 垂直起始
    write_reg(0x1A, 0x7A);   // VSTOP[7:0]  垂直停止
    write_reg(0x03, 0x0A);   // VREF[9:1]   图像高度控制

    // 6. 启用缩放至QQVGA (160x120)
    write_reg(0x0C, 0x04);   // COM3: bit2=1 使能缩放
    write_reg(0x3E, 0x19);   // COM14: bit4=1 使能缩放, bit3=0 使用DCW, bit0=1 手动PCLK分频 (0x19=0001 1001)
    write_reg(0x72, 0x22);   // SCALING_DCWCTR: 缩放系数 (0x22 => 水平/垂直各除以2, 320/2=160, 240/2=120)
    write_reg(0x73, 0xF2);   // SCALING_PCLK_DIV: PCLK分频 (0xF2 可能使能分频, 具体见手册)
    write_reg(0xA2, 0x02);   // SCALING_PCLK_DELAY: 调整PCLK延时, 提高稳定性

    // 7. 基本图像处理设置 (色度、亮度、自动增益等)
    write_reg(0x13, 0xE5);   // COM8: 使能AGC, AWB, AEC (默认)
    write_reg(0x00, 0x00);   // GAIN: AGC增益 (默认)
    write_reg(0x01, 0x80);   // BLUE: 蓝色增益 (默认)
    write_reg(0x02, 0x80);   // RED:  红色增益 (默认)
    write_reg(0x14, 0x2A);   // COM9: 最大AGC增益 4x (默认)
    write_reg(0x4F, 0x80);   // 矩阵系数 (默认)
    write_reg(0x50, 0x80);   // 矩阵系数
    write_reg(0x51, 0x00);   // 矩阵系数
    write_reg(0x52, 0x22);   // 矩阵系数
    write_reg(0x53, 0x5E);   // 矩阵系数
    write_reg(0x54, 0x80);   // 矩阵系数
    write_reg(0x56, 0x40);   // 矩阵系数

    // 8. 其他重要设置
    write_reg(0x15, 0x00);   // COM10: PCLK输出方式, HREF/HSYNC极性等 (默认)
    //   注意: COM10[2] = 0 => HSYNC高有效; COM10[1] = 0 => VSYNC高有效
    //   请根据实际硬件和DCI配置调整此寄存器

    write_reg(0x3D, 0xC0);   // COM13: 使能GAMMA, 自动白平衡等
    write_reg(0x3F, 0x5F);   // EDGE: 边缘增强系数 (默认)
    write_reg(0x42, 0x14);   // 色度增益
    write_reg(0x43, 0x00);   // 色度中心
    write_reg(0x44, 0x80);   // 色度UV平均值
    write_reg(0x45, 0x20);   // 色度增益控制
    write_reg(0x46, 0x22);   // 色度增益控制
    write_reg(0x47, 0x28);   // 亮度增益
    write_reg(0x48, 0x00);   // 亮度中心
    write_reg(0x49, 0x04);   // 亮度平均值
    write_reg(0x4A, 0x81);   // 亮度增益控制
    write_reg(0x4B, 0x84);   // 亮度增益控制

    // 9. 最后等待稳定
    Delay_ms(100);
}

static void ov7670_DMA_DCI_config(void)
{
    rcu_periph_clock_enable(RCU_DCI);
    rcu_periph_clock_enable(RCU_DMA1);
    // 配置DMA
    dma_channel_disable(DMA1, DMA_CH1);
    dma_deinit(DMA1, DMA_CH1);//DMA0复位
    dma_single_data_parameter_struct dma_init_struct;
    dma_single_data_para_struct_init(&dma_init_struct);
    // 外设地址
    dma_init_struct.periph_addr       = (uint32_t)&DCI_DATA;       // DCI 数据寄存器
    dma_init_struct.periph_inc        = DMA_PERIPH_INCREASE_FIX;   // 外设地址固定
    // 内存地址
    dma_init_struct.memory0_addr      = (uint32_t)lcd_fb;          // SDRAM framebuffer
    dma_init_struct.memory0_addr      = (uint32_t)dma_temp_buffer;          // SDRAM framebuffer
    dma_init_struct.memory_inc        = DMA_MEMORY_INCREASE_ENABLE;// 内存地址递增
    // 方向
    dma_init_struct.direction          = DMA_PERIPH_TO_MEMORY;     // 外设到内存
    dma_init_struct.circular_mode      = DMA_CIRCULAR_MODE_ENABLE; // 循环模式 DMA_CIRCULAR_MODE_DISABLE
    dma_init_struct.circular_mode      = DMA_CIRCULAR_MODE_DISABLE; // 循环模式 
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;   // 8bit
//    dma_init_struct.number             = LCD_WIDTH * LCD_HEIGHT * 2;   // 传输长度
    dma_init_struct.number             = 160 * 120 * 2;   // 传输长度
    dma_init_struct.priority           = DMA_PRIORITY_HIGH;        // 通道优先级
    dma_single_data_mode_init(DMA1, DMA_CH1, &dma_init_struct);    // 初始化 DMA
    dma_periph_width_config(DMA1, DMA_CH1, DMA_PERIPH_WIDTH_8BIT);
    dma_memory_width_config(DMA1, DMA_CH1, DMA_MEMORY_WIDTH_8BIT);
    dma_flow_controller_config(DMA1, DMA_CH1, DMA_FLOW_CONTROLLER_DMA);
    dma_channel_subperipheral_select(DMA1, DMA_CH1, DMA_SUBPERI1);
    dma_interrupt_disable(DMA1, DMA_CH1, DMA_INT_SDE|DMA_INT_TAE|DMA_INT_HTF|DMA_INT_FTF|DMA_INT_FEE);
    dma_interrupt_enable(DMA1, DMA_CH1, DMA_INT_HTF); // 使能 DMA 完成中断
    nvic_irq_enable(DMA1_Channel1_IRQn, 2, 0);
    dma_channel_enable(DMA1, DMA_CH1); // 使能 DMA

    // DCI 初始化
    dci_deinit();
    dci_parameter_struct dci_struct;//结构体
    dci_struct.capture_mode    = DCI_CAPTURE_MODE_CONTINUOUS;   // 连续帧
    dci_struct.clock_polarity  = DCI_CK_POLARITY_RISING;        // 上升沿
    // HS 低   VS 高   有图像 较快
    dci_struct.vsync_polarity  = DCI_VSYNC_POLARITY_HIGH;       // 高有效
    dci_struct.hsync_polarity  = DCI_HSYNC_POLARITY_LOW;        // 低有效
    dci_struct.frame_rate      = DCI_FRAME_RATE_ALL;            // 不丢帧
    dci_struct.interface_format = DCI_INTERFACE_FORMAT_8BITS;   // 8bit 数据线
    dci_init(&dci_struct);
    dci_jpeg_disable();
    dci_interrupt_disable(DCI_INT_EF | DCI_INT_OVR | DCI_INT_ESE | DCI_INT_VSYNC | DCI_INT_EL);
    dci_enable();
    dci_capture_enable();
}

volatile uint8_t frame_ready = 0;   // 一帧完成标记
void DMA1_Channel1_IRQHandler(void)
{
    // 检查全传输完成标志
    if (dma_interrupt_flag_get(DMA1, DMA_CH1, DMA_INT_FTF)) {
        dma_interrupt_flag_clear(DMA1, DMA_CH1, DMA_INT_FTF);
        // 8bit 数据 → 拼装 16bit 像素
        for(uint32_t i=0;i<LCD_WIDTH*LCD_HEIGHT;i++) {
            uint8_t high = dma_temp_buffer[i*2];
            uint8_t low  = dma_temp_buffer[i*2+1];
            lcd_fb[i] = (high << 8) | low;
        }
        frame_ready = 2;   // 标记一帧完成
    }
//    // 检查半传输完成标志（即使未使能，也可能意外置位，清除无害）
//    if (dma_interrupt_flag_get(DMA1, DMA_CH1, DMA_INT_HTF)) {
//        dma_interrupt_flag_clear(DMA1, DMA_CH1, DMA_INT_HTF);
//        // 可加入调试信息或空操作
//        frame_ready = 3;   // 标记一帧完成
//    }
//    // 检查传输访问错误标志
//    if (dma_interrupt_flag_get(DMA1, DMA_CH1, DMA_INT_TAE)) {
//        dma_interrupt_flag_clear(DMA1, DMA_CH1, DMA_INT_TAE);
//        // 处理错误，如停止传输、重启等
//        frame_ready = 4;   // 标记一帧完成
//    }
//    // 检查单数据模式异常标志
//    if (dma_interrupt_flag_get(DMA1, DMA_CH1, DMA_INT_SDE)) {
//        dma_interrupt_flag_clear(DMA1, DMA_CH1, DMA_INT_SDE);
//        frame_ready = 5;   // 标记一帧完成
//        // 处理异常
//    }
//    // 检查 FIFO 异常标志
//    if (dma_interrupt_flag_get(DMA1, DMA_CH1, DMA_INT_FEE)) {
//        dma_interrupt_flag_clear(DMA1, DMA_CH1, DMA_INT_FEE);
//        frame_ready = 6;   // 标记一帧完成
//        // 处理 FIFO 错误
//    }
    
    // 可选：清除全局中断标志（如果库支持）
    // dma_interrupt_flag_clear(DMA1, DMA_CH1, DMA_INT_G); 
}

void DCI_IRQHandler(void)
{
    if(dci_interrupt_flag_get(DCI_INT_FLAG_EF)) { 
        dci_interrupt_flag_clear(DCI_INT_FLAG_EF);
//        frame_ready = 1;
    }
}

static void OV_I2C_Start(void)
{
    I2C_SDA_SET;
    I2C_SCL_SET;
    Delay_us(5);
    I2C_SDA_CLR;
    Delay_us(5);
    I2C_SCL_CLR;
}

static void OV_I2C_Stop(void)
{
    I2C_SDA_CLR;
    Delay_us(5);
    I2C_SCL_SET;
    Delay_us(5);
    I2C_SDA_SET;
    Delay_us(5);
}

static uint8_t OV_I2C_Wait_Ack(void)
{
    I2C_SDA_SET;
    SDA_IN;
    Delay_us(5);
    I2C_SCL_SET;
    Delay_us(5);
    if (I2C_SDA_GET) {
        SDA_OUT;
        I2C_SCL_CLR;
        OV_I2C_Stop();
        return 1;
    }
    SDA_OUT;
    I2C_SCL_CLR;
    return 0;
}

static void OV_I2C_Ack(void)
{
    I2C_SCL_CLR;
    Delay_us(5);
    I2C_SDA_CLR;
    Delay_us(5);
    I2C_SCL_SET;
    Delay_us(5);
    I2C_SCL_CLR;
    I2C_SDA_SET;
    Delay_us(5);
}

static void OV_I2C_NAck(void)
{
    I2C_SCL_CLR;
    Delay_us(5);
    I2C_SDA_SET;
    Delay_us(5);
    I2C_SCL_SET;
    Delay_us(5);
    I2C_SCL_CLR;
    Delay_us(5);
}

static void OV_I2C_Send_Byte(uint8_t Byte)
{
	for (uint8_t i = 0; i < 8; i++)//循环8次，主机依次发送数据的每一位
	{
        if (Byte & 0x80) {
            I2C_SDA_SET;
        } else {
            I2C_SDA_CLR;
        }
        Delay_us(5);
        I2C_SCL_SET;
        Delay_us(5);
        I2C_SCL_CLR;
        Delay_us(5);
        Byte <<= 1;
	}
    I2C_SDA_SET;
}

static uint8_t OV_I2C_Read_Byte(uint8_t ack)
{
	uint8_t Byte = 0x00;
	I2C_SDA_SET;
    SDA_IN;
    Delay_us(5);
	for (uint8_t i = 0; i < 8; i ++)
	{
        Byte <<= 1;    
        I2C_SCL_SET;
        Delay_us(5);
		if (I2C_SDA_GET == 1) {
            Byte |= 0x01;
		}
		I2C_SCL_CLR;
        Delay_us(5);
	}
    SDA_OUT;
    if (ack) {
        OV_I2C_Ack();
    } else {
        OV_I2C_NAck();
    }
	return Byte;
}

static void write_reg(uint8_t reg, uint8_t data)
{
    OV_I2C_Start();

    OV_I2C_Send_Byte(0x42);          // OV7670 写地址 (0x21<<1)
    if (OV_I2C_Wait_Ack()) goto stop;

    OV_I2C_Send_Byte(reg);           // 寄存器地址
    if (OV_I2C_Wait_Ack()) goto stop;

    OV_I2C_Send_Byte(data);          // 数据
    OV_I2C_Wait_Ack();

stop:
    OV_I2C_Stop();
}

static uint8_t read_reg(uint8_t reg)
{
    uint8_t data = 0;

    // 第一阶段：写寄存器地址
    OV_I2C_Start();

    OV_I2C_Send_Byte(0x42);          // 写地址
    if (OV_I2C_Wait_Ack()) goto stop1;

    OV_I2C_Send_Byte(reg);
    OV_I2C_Wait_Ack();

stop1:
    OV_I2C_Stop();

    // 短延时（避免某些模块读失败）
    for (volatile int i = 0; i < 50; i++);

    // 第二阶段：读取数据
    OV_I2C_Start();

    OV_I2C_Send_Byte(0x43);          // 读地址
    if (OV_I2C_Wait_Ack()) goto stop2;

    data = OV_I2C_Read_Byte(0);      // 最后一个字节发送 NACK

stop2:
    OV_I2C_Stop();

    return data;
}
