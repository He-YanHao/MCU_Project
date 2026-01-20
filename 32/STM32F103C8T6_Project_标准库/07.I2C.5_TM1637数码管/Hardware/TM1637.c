#include "stm32f10x.h"                  // Device header
#include "Delay.h"
unsigned char tab[] =
{
    0x3F,/*0*/
    0x06,/*1*/
    0x5B,/*2*/
    0x4F,/*3*/
    0x66,/*4*/
    0x6D,/*5*/
    0x7D,/*6*/
    0x07,/*7*/
    0x7F,/*8*/
    0x6F,/*9*/
    0x77,/*10 A*/
    0x7C,/*11 b*/
    0x58,/*12 c*/
    0x5E,/*13 d*/
    0x79,/*14 E*/
    0x71,/*15 F*/
    0x76,/*16 H*/
    0x38,/*17 L*/
    0x54,/*18 n*/
    0x73,/*19 P*/
    0x3E,/*20 U*/
    0x00,/*21 黑屏*/
};

// 最高位设置为1时显示 数码管上的":" 符号
unsigned char disp_num[] = {0x3F, 0x06 | 0x80, 0x5B, 0x4F, 0x66, 0x6D};			//存放6个数码管要显示的内容

//IO方向设置		   								0011输出模式   1000上下拉输入模式
#define TM1637_DIO_IN()      {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define TM1637_DIO_OUT()     {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

void MyI2C_Init(void)//GPIO初始化
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

void I2C2_W_SCL(uint8_t BitValue)//SCL写
{
    TM1637_DIO_OUT();
	Delay_us(5);//延时10us，防止时序频率超过要求
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);//根据BitValue，设置SCL引脚的电平
	Delay_us(5);//延时10us，防止时序频率超过要求
}

void I2C2_W_SDA(uint8_t BitValue)//SDA写
{
    TM1637_DIO_OUT();
    Delay_us(5);//延时10us，防止时序频率超过要求
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	Delay_us(5);//延时10us，防止时序频率超过要求
}

uint8_t I2C2_R_SDA(void)//SDA读
{
    TM1637_DIO_IN();
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);		//读取SDA电平
	Delay_us(10);												//延时10us，防止时序频率超过要求
	return BitValue;											//返回SDA电平
}

void MyI2C_Start(void)//I2C的开始
{
    TM1637_DIO_OUT();
	I2C2_W_SDA(1);//释放SDA，确保SDA为高电平
	I2C2_W_SCL(1);//释放SCL，确保SCL为高电平
	I2C2_W_SDA(0);//在SCL高电平期间，拉低SDA，产生起始信号
	I2C2_W_SCL(0);//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}

void MyI2C_Stop(void)//I2C的结束
{
    TM1637_DIO_OUT();
	I2C2_W_SDA(0);//拉低SDA，确保SDA为低电平
	I2C2_W_SCL(1);//释放SCL，使SCL呈现高电平
	I2C2_W_SDA(1);//在SCL高电平期间，释放SDA，产生终止信号
}

void MyI2C_SendByte(uint8_t Byte)//I2C发送一个字节
{
    TM1637_DIO_OUT();
	uint8_t i = 0;
    for( i = 0; i < 8; i++ )
    {
        if( Byte & 0x01 )//低位在前
        {
            I2C2_W_SDA(1);
        }
        else
        {
            I2C2_W_SDA(0);
        }
        Byte = Byte >> 1;
        I2C2_W_SCL(0);
    }
}

uint8_t MyI2C_ReceiveByte(void)//I2C接收一个字节
{
	uint8_t i = 0, Byte = 0x00;             //定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
	I2C2_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	for (i = 0; i < 8; i ++)				//循环8次，主机依次接收数据的每一位
	{
		I2C2_W_SCL(1);						//释放SCL，主机在SCL高电平期间读取SDA
		if (I2C2_R_SDA() == 1)//读取SDA数据，并存储到Byte变量 当SDA为1时，置变量指定位为1，当SDA为0时，不做处理，指定位为默认的初值0
		{
            Byte |= (0x80 >> i);
		}
		I2C2_W_SCL(0);						//拉低SCL，从机在SCL低电平期间写入SDA
	}
	return Byte;							//返回接收到的一个字节数据
}

void TM1637_ACK(void)//TM1637的应答
{
	I2C2_W_SCL(0);
    Delay_us(5);
    while(I2C2_R_SDA());
	I2C2_W_SCL(1);
    Delay_us(2);
	I2C2_W_SCL(0);
}

uint8_t ReadKey()//读按键值
{
    uint8_t rekey = 0, i = 0;
    MyI2C_Start();
    MyI2C_SendByte(0x42);
    TM1637_ACK();
    I2C2_W_SDA(1);
    for(i=0;i<8;i++) //从低位开始读 
    {
        I2C2_W_SCL(0);
        rekey = rekey >> 1;
        Delay_us(30);
        I2C2_W_SCL(1);
        if(I2C2_R_SDA())
        {
            rekey = rekey | 0x80;
        }
        else
        {
            rekey = rekey | 0x00;
        }
        Delay_us(30);
    }
    TM1637_ACK();
    MyI2C_Stop();
    return rekey;
}

void MyI2C_SendAck(uint8_t AckBit)//I2C发送应答位 范围：0~1，0表示应答，1表示非应答
{
	I2C2_W_SDA(AckBit);             //主机把应答位数据放到SDA线
	I2C2_W_SCL(1);                  //释放SCL，从机在SCL高电平期间，读取应答位
	I2C2_W_SCL(0);                  //拉低SCL，开始下一个时序模块
}

uint8_t MyI2C_ReceiveAck(void)//I2C接收应答位 范围：0~1，0表示应答，1表示非应答
{
	uint8_t AckBit;							//定义应答位变量
	I2C2_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	I2C2_W_SCL(1);							//释放SCL，主机机在SCL高电平期间读取SDA
	AckBit = I2C2_R_SDA();					//将应答位存储到变量里
	I2C2_W_SCL(0);							//拉低SCL，开始下一个时序模块
	return AckBit;							//返回定义应答位变量
}

void TM1637()
{
    uint8_t i = 0;
	MyI2C_Start();//I2C起始
	MyI2C_SendByte(0X40);//发送从机地址，读写位为0，表示即将写入
	TM1637_ACK();//
	MyI2C_Stop();//I2C终止
	MyI2C_Start();//I2C起始
	MyI2C_SendByte(0XC0);//发送从机地址，读写位为0，表示即将写入
	TM1637_ACK();//
	for(i = 0; i < 6; i++)//地址自加，不必每次都写地址 
     { 
        MyI2C_SendByte(0x11);//送数据 
        TM1637_ACK();
     }
	MyI2C_Stop();//I2C终止
	MyI2C_Start();//I2C起始
	MyI2C_SendByte( 0X88 | 0X07 );	//发送从机地址，读写位为0，表示即将写入
	TM1637_ACK();//
	MyI2C_Stop();//I2C终止
}
