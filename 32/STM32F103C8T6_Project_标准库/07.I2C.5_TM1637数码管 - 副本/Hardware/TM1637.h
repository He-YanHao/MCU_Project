#ifndef __TM1637_H
#define __TM1637_H

void TM1637_Init( void );
void TM1637_Start( void );
void TM1637_Ack( void );
void TM1637_Stop( void );
void TM1637_WriteByte( unsigned char oneByte );
//输入数据在CLK的低电平变化，在CLK的高电平被传输。
//每传输一个字节，芯片内部在第八个时钟下降沿产生一个ACK
// 写一个字节
void TM1637_Display_INC( void );
//写显示寄存器  地址自增
void TM1637_Display_NoINC( unsigned char add, unsigned char value );
//写显示寄存器  地址不自增
// add 数码管的地址 0--5
// value 要显示的内容
void TM1637_SetBrightness( unsigned char level );
// level : 设置亮度等级  0---7
unsigned char TM1637_ScanKey( void );
//读按键  读按键时，时钟频率应小于 250K，先读低位，后读高位。
unsigned char TM1637_KeyProcess( void );
//按键处理函数，按键数据低位在前高位在后


/* 定义IIC连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define TM1637_CLK_GPIO_PORT    	GPIOB		                /* GPIO端口 */
#define TM1637_CLK_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define TM1637_CLK_GPIO_PIN			  GPIO_Pin_6
 
#define TM1637_DIO_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define TM1637_DIO_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define TM1637_DIO_GPIO_PIN		    GPIO_Pin_7

//使用 位带 操作
#define TM1637_CLK           PBout(6)
#define TM1637_DIO           PBout(7)
#define TM1637_READ_DIO      PBin(7)
 
//IO方向设置		   								0011输出模式   1000上下拉输入模式
#define TM1637_DIO_IN()      {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define TM1637_DIO_OUT()     {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

#endif
