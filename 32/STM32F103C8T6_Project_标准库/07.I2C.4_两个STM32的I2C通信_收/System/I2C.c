#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void I2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);//根据BitValue，设置SCL引脚的电平
	Delay_us(10);												//延时10us，防止时序频率超过要求
}

void I2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	Delay_us(10);												//延时10us，防止时序频率超过要求
}

uint8_t I2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);		//读取SDA电平
	Delay_us(10);												//延时10us，防止时序频率超过要求
	return BitValue;											//返回SDA电平
}

void My_I2C_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);		//开启I2C2的时钟

	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);

	/*I2C初始化*/
	I2C_InitTypeDef I2C_InitStructure;//定义结构体变量
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;//模式，选择为I2C模式	包含I2C模式和SMBUS模式两种，其中SMBUS为I2C的一个子集
	I2C_InitStructure.I2C_ClockSpeed = 50000;//时钟速度，选择为50KHz
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;//时钟占空比，选择Tlow/Thigh = 2	该配置有两个选择，分别为低电平时间比高电平时间为2：1 ( I2C_DutyCycle_2)和16：9 (I2C_DutyCycle_16_9)。一般不严格要求。
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;//应答，包含 使能I2C_Ack_Enable 和 禁止应答I2C_Ack_Disable，一般为了兼容性选择使能I2C_Ack_Enable。
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//应答地址，包含7位或10位，此处选择7位，从机模式下才有效，
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;//自身地址，从机模式下才有效
	I2C_Init(I2C2, &I2C_InitStructure);//将结构体变量交给I2C_Init，配置I2C2
	/*I2C使能*/
	I2C_Cmd(I2C2, ENABLE);									//使能I2C2，开始运行

}

void My_I2C_Start(void)//I2C的开始
{
	I2C_W_SDA(1);//释放SDA，确保SDA为高电平
	I2C_W_SCL(1);//释放SCL，确保SCL为高电平
	I2C_W_SDA(0);//在SCL高电平期间，拉低SDA，产生起始信号
	I2C_W_SCL(0);//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}

void My_I2C_Stop(void)//I2C的结束
{
	I2C_W_SDA(0);//拉低SDA，确保SDA为低电平
	I2C_W_SCL(1);//释放SCL，使SCL呈现高电平
	I2C_W_SDA(1);//在SCL高电平期间，释放SDA，产生终止信号
}

void My_I2C_SendByte(uint8_t Byte)//I2C发送一个字节
{
	uint8_t i;
	for (i = 0; i < 8; i++)//循环8次，主机依次发送数据的每一位
	{
 	I2C_W_SDA(Byte & (0x80 >> i));
 	I2C_W_SCL(1);//释放SCL，从机在SCL高电平期间读取SDA
 	I2C_W_SCL(0);//拉低SCL，主机开始发送下一位数据
	}
	//My_I2C2_W_SCL(1);//额外的一个时钟，不处理应答信号
	//My_I2C2_W_SCL(0);
}

uint8_t My_I2C_ReceiveByte(void)//I2C接收一个字节
{
	uint8_t i, Byte = 0x00;					//定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
	I2C_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	for (i = 0; i < 8; i ++)				//循环8次，主机依次接收数据的每一位
	{
		I2C_W_SCL(1);						//释放SCL，主机在SCL高电平期间读取SDA
		if (I2C_R_SDA() == 1)//读取SDA数据，并存储到Byte变量 当SDA为1时，置变量指定位为1，当SDA为0时，不做处理，指定位为默认的初值0
		{
		Byte |= (0x80 >> i);
		}
		I2C_W_SCL(0);						//拉低SCL，从机在SCL低电平期间写入SDA
	}
	return Byte;							//返回接收到的一个字节数据
}

void My_I2C_SendAck(uint8_t AckBit)//I2C发送应答位 范围：0~1，0表示应答，1表示非应答
{
	I2C_W_SDA(AckBit);				//主机把应答位数据放到SDA线
	I2C_W_SCL(1);							//释放SCL，从机在SCL高电平期间，读取应答位
	I2C_W_SCL(0);							//拉低SCL，开始下一个时序模块
}

uint8_t My_I2C_ReceiveAck(void)//I2C接收应答位 范围：0~1，0表示应答，1表示非应答
{
	uint8_t AckBit;							//定义应答位变量
	I2C_W_SDA(1);							//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	I2C_W_SCL(1);							//释放SCL，主机机在SCL高电平期间读取SDA
	AckBit = I2C_R_SDA();					//将应答位存储到变量里
	I2C_W_SCL(0);							//拉低SCL，开始下一个时序模块
	return AckBit;							//返回定义应答位变量
}
