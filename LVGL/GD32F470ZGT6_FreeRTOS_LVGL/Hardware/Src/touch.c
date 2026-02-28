#include "touch.h"
#include "stdio.h"
#include "string.h"
#include "lcd.h"

// GPIO 宏定义
#define SCL_RCU RCU_GPIOB
#define SCL_PORT GPIOB
#define SCL_PIN GPIO_PIN_6
#define SCL_ON gpio_bit_set(SCL_PORT, SCL_PIN)
#define SCL_OFF gpio_bit_reset(SCL_PORT, SCL_PIN)
#define SCL_TOGGLE gpio_bit_toggle(SCL_PORT, SCL_PIN)

#define SDA_RCU RCU_GPIOB
#define SDA_PORT GPIOB
#define SDA_PIN GPIO_PIN_7
#define SDA_ON gpio_bit_set(SDA_PORT, SDA_PIN)
#define SDA_OFF gpio_bit_reset(SDA_PORT, SDA_PIN)
#define SDA_TOGGLE gpio_bit_toggle(SDA_PORT, SDA_PIN)

// IO方向设置
#define CT_SDA_IN() gpio_mode_set(SCL_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, SDA_PIN);   //{GPIOF->MODER&=~(3<<(2*11));GPIOF->MODER|=0<<2*11;}	//PF11输入模式
#define CT_SDA_OUT() gpio_mode_set(SCL_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, SDA_PIN); //{GPIOF->MODER&=~(3<<(2*11));GPIOF->MODER|=1<<2*11;} 	//PF11输出模式
// IO操作函数
#define CT_IIC_SCL                                        // PBout(0) 	//SCL
#define CT_IIC_SDA(val) ((val) == 1 ? SDA_ON : SDA_OFF)   // PFout(11) //SDA
#define CT_READ_SDA gpio_input_bit_get(SDA_PORT, SDA_PIN) // PFin(11)  //输入SDA

#define RST_RCU RCU_GPIOD
#define RST_PORT GPIOD
#define RST_PIN GPIO_PIN_12
#define RST_ON gpio_bit_set(RST_PORT, RST_PIN);
#define RST_OFF gpio_bit_reset(RST_PORT, RST_PIN);
#define RST_TOGGLE gpio_bit_toggle(RST_PORT, RST_PIN);

#define INT_RCU RCU_GPIOD
#define INT_PORT GPIOD
#define INT_PIN GPIO_PIN_11
#define INT_ON gpio_bit_set(INT_PORT, INT_PIN);
#define INT_OFF gpio_bit_reset(INT_PORT, INT_PIN);
#define INT_TOGGLE gpio_bit_toggle(INT_PORT, INT_PIN);

// I2C 通讯相关 宏定义
// I2C读写命令
#define FT_CMD_WR 0X70 // 写命令
#define FT_CMD_RD 0X71 // 读命令
// FT5206 部分寄存器定义
#define FT_DEVIDE_MODE 0x00    // FT5206模式控制寄存器

#define FT_REG_NUM_FINGER 0x02 // 触摸状态寄存器
#define FT_TP1_REG 0X03        // 第一个触摸点数据地址
#define FT_TP2_REG 0X09        // 第二个触摸点数据地址
#define FT_TP3_REG 0X0F        // 第三个触摸点数据地址
#define FT_TP4_REG 0X15        // 第四个触摸点数据地址
#define FT_TP5_REG 0X1B        // 第五个触摸点数据地址

#define FT_ID_G_LIB_VERSION  0xA1 // 版本
#define FT_ID_G_MODE         0xA4 // FT5206中断模式控制寄存器
#define FT_ID_G_THGROUP      0x80 // 触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE 0x88 // 激活状态周期设置寄存器

#define TP_PRES_DOWN 0x80 // 触屏被按下
#define TP_CATH_PRES 0x40 // 有按键按下了

const uint16_t FT5206_TPX_TBL[5] = {FT_TP1_REG, FT_TP2_REG, FT_TP3_REG, FT_TP4_REG, FT_TP5_REG};
uint8_t g_gt_tnum = 5; // 默认支持的触摸屏点数(5点触摸)

void touch_delay_1us(uint16_t us)
{
    while (us--)
    {
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
    }
}

// 控制I2C速度的延时
void CT_Delay(void)
{
    touch_delay_1us(2);
}

// 电容触摸芯片IIC接口初始化
void CT_IIC_Init(void)
{
    /* enable the led clock */
    rcu_periph_clock_enable(SCL_RCU);
    /* configure led GPIO port */
    gpio_mode_set(SCL_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, SCL_PIN);
    gpio_output_options_set(SCL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SCL_PIN);

    rcu_periph_clock_enable(SDA_RCU);
    gpio_mode_set(SDA_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, SDA_PIN);
    gpio_output_options_set(SDA_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SDA_PIN);
}

// 产生IIC起始信号
void CT_IIC_Start(void)
{
    CT_SDA_OUT(); // sda线输出
    SDA_ON;
    SCL_ON; // SCL=1时，SDA由1到0跳变
    touch_delay_1us(30);
    SDA_OFF; // START:when CLK is high,DATA change form high to low
    CT_Delay();
    SCL_OFF; // 钳住I2C总线，准备发送或接收数据
}

// 产生IIC停止信号
void CT_IIC_Stop(void)
{
    CT_SDA_OUT(); // sda线输出
    SCL_ON;       // SCL=1时，SDA由0到1跳变
    touch_delay_1us(30);
    SDA_OFF; // STOP:when CLK is high DATA change form low to high
    CT_Delay();
    SDA_ON; // 发送I2C总线结束信号
}

// 等待应答信号到来
// 返回值：1，接收应答失败
//         0，接收应答成功
uint8_t CT_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    CT_SDA_IN(); // SDA设置为输入
    SDA_ON;
    SCL_ON;
    CT_Delay();
    while (CT_READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime > 250)
        {
            CT_IIC_Stop();
            return 1;
        }
        CT_Delay();
    }
    SCL_OFF; // 时钟输出0
    return 0;
}

// 产生ACK应答
void CT_IIC_Ack(void)
{
    SCL_OFF;
    CT_SDA_OUT();
    CT_Delay();
    SDA_OFF;
    CT_Delay();
    SCL_ON;
    CT_Delay();
    SCL_OFF;
}

// 不产生ACK应答
void CT_IIC_NAck(void)
{
    SCL_OFF;
    CT_SDA_OUT();
    CT_Delay();
    SDA_ON;
    CT_Delay();
    SCL_ON;
    CT_Delay();
    SCL_OFF;
}

// IIC发送一个字节
// 返回从机有无应答
// 1，有应答
// 0，无应答
void CT_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    CT_SDA_OUT();
    SCL_OFF; // 拉低时钟开始数据传输
    CT_Delay();
    for (t = 0; t < 8; t++)
    {
        // CT_IIC_SDA=(txd&0x80)>>7;
        // CT_IIC_SDA((txd&0x80)>>7);
        if ((txd & 0x80) >> 7)
        {
            gpio_bit_set(SDA_PORT, SDA_PIN);
        }
        else
        {
            gpio_bit_reset(SDA_PORT, SDA_PIN);
        }

        txd <<= 1;
        SCL_ON;
        CT_Delay();
        SCL_OFF;
        CT_Delay();
    }
}

// 读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t CT_IIC_Read_Byte(unsigned char ack)
{
    volatile uint8_t i, receive = 0;
    CT_SDA_IN(); // SDA设置为输入
    touch_delay_1us(30);
    for (i = 0; i < 8; i++)
    {
        SCL_OFF;
        CT_Delay();
        SCL_ON;
        receive <<= 1;

        // printf("%d ",CT_READ_SDA);
        if (CT_READ_SDA)
            receive++;
    }
    // printf("\r\n receive:%0x \r\n",receive);

    if (!ack)
        CT_IIC_NAck(); // 发送nACK
    else
        CT_IIC_Ack(); // 发送ACK
    return receive;
}

// 向FT5206写入一次数据
uint8_t FT5206_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    uint8_t ret = 0;
    CT_IIC_Start();
    CT_IIC_Send_Byte(FT_CMD_WR); // 发送写命令
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg & 0XFF); // 发送低8位地址
    CT_IIC_Wait_Ack();
    for (i = 0; i < len; i++)
    {
        CT_IIC_Send_Byte(buf[i]); // 发数据
        ret = CT_IIC_Wait_Ack();
        if (ret)
            break;
    }
    CT_IIC_Stop(); // 产生一个停止条件
    return ret;
}

// 从FT5206读出一次数据
void FT5206_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    CT_IIC_Start();
    CT_IIC_Send_Byte(FT_CMD_WR); // 发送写命令
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg & 0XFF); // 发送低8位地址
    CT_IIC_Wait_Ack();
    CT_IIC_Start();
    CT_IIC_Send_Byte(FT_CMD_RD); // 发送读命令
    CT_IIC_Wait_Ack();
    for (i = 0; i < len; i++)
    {
        buf[i] = CT_IIC_Read_Byte(i == (len - 1) ? 0 : 1); // 发数据
    }
    CT_IIC_Stop(); // 产生一个停止条件
}

// 初始化FT5206触摸屏
void FT5206_Init(void)
{
    // 临时存放
    uint8_t temp[2] = {0};

    // 初始化 GPIO
    /* enable the led clock */
    rcu_periph_clock_enable(INT_RCU);
    /* configure led GPIO port */
    gpio_mode_set(INT_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, INT_PIN);

    // 初始化电容屏的I2C总线
    CT_IIC_Init();

    // 初始化触摸芯片
    FT5206_WR_Reg(FT_DEVIDE_MODE, temp, 1);  // 进入正常操作模式
    FT5206_WR_Reg(FT_ID_G_MODE, temp, 1);    // 查询模式
    temp[0] = 22;                            // 触摸有效值，22，越小越灵敏
    FT5206_WR_Reg(FT_ID_G_THGROUP, temp, 1); // 设置触摸有效值
    temp[0] = 12;                            // 激活周期，不能小于12，最大14
    FT5206_WR_Reg(FT_ID_G_PERIODACTIVE, temp, 1);

//    // 读取版本号，参考值：0x3003
//    FT5206_RD_Reg(FT_ID_G_LIB_VERSION, &temp[0], 2);
//    printf("CTP ID:%x\r\n", ((uint16_t)temp[0] << 8) + temp[1]);
}

// 
touch_t g_touch_status = {0};

// 
uint8_t FT5206_Scan(void)
{
    // 临时存放触摸点的数量
    uint8_t td_status_reg = 0;
    // 临时存放触摸点的坐标
    uint8_t buf[4];

    // 读取寄存器的值 保存到 td_status_reg
    // FT_REG_NUM_FINGER寄存器代表现在触摸点的数量
    // 如：FT_REG_NUM_FINGER=0x3 表示现在有三个点被按下
    FT5206_RD_Reg(FT_REG_NUM_FINGER, &td_status_reg, 1);

    // 触摸点仅取低四位不为0 并且 小于5个触摸点 触摸状态正确
    if ((td_status_reg & 0XF) && ((td_status_reg & 0XF) <= g_gt_tnum))
    {
        // 保存当前的触摸点数量
        g_touch_status.tp_pressed_count = td_status_reg & 0XF;
        for (uint8_t i = 0; i < g_touch_status.tp_pressed_count; i++)
        {
            // 读取XY坐标值
            FT5206_RD_Reg(FT5206_TPX_TBL[i], buf, 4); 
            g_touch_status.y[i] = ((uint16_t)(buf[0] & 0X0F) << 8) + buf[1];
            g_touch_status.x[i] = ((uint16_t)(buf[2] & 0X0F) << 8) + buf[3];
        }
        // 表示触摸被按下了
        return 1;
    }
    return 0;
}

// 获得触摸状态结构体的指针
const touch_t *FT5206_GetState(void)
{
    return &g_touch_status;
}
