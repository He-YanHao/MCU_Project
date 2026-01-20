#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void I2C_W_SCL(uint8_t BitValue)//I2C写SCL引脚电平
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_10,(BitAction)BitValue);//根据BitValue，设置SCL引脚的电平
	Delay_us(10);												//延时10us，防止时序频率超过要求
}

void I2C_W_SDA(uint8_t BitValue)								//I2C写SDA引脚电平
{
	GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);		//根据BitValue，设置SDA引脚的电平，BitValue要实现非0即1的特性
	Delay_us(10);												//延时10us，防止时序频率超过要求
}

uint8_t I2C_R_SDA(void)										//I2C读SDA引脚电平
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);		//读取SDA电平
	Delay_us(10);												//延时10us，防止时序频率超过要求
	return BitValue;											//返回SDA电平
}

void MyI2C_Init(void)										//GPIO口初始化
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//开启GPIOB的时钟
	GPIO_InitTypeDef GPIO_InitStructure;					//GPIO初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//将PB10和PB11引脚初始化为开漏输出
	/*设置默认电平*/
	GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);			//设置PB10和PB11引脚初始化后默认为高电平（释放总线状态）
}

void MyI2C_Start(void)						//I2C开始
{
	I2C_W_SDA(1);							//释放SDA，确保SDA为高电平
	I2C_W_SCL(1);							//释放SCL，确保SCL为高电平
	I2C_W_SDA(0);							//在SCL高电平期间，拉低SDA，产生起始信号
	I2C_W_SCL(0);							//起始后把SCL也拉低，即为了占用总线，也为了方便总线时序的拼接
}

void MyI2C_Stop(void)						//I2C终止
{
	I2C_W_SDA(0);							//拉低SDA，确保SDA为低电平
	I2C_W_SCL(1);							//释放SCL，使SCL呈现高电平
	I2C_W_SDA(1);							//在SCL高电平期间，释放SDA，产生终止信号
}

void MyI2C_SendByte(uint8_t Byte)			//I2C发送一个字节，范围：0x00~0xFF。
{
	uint8_t i;
	for (i = 0; i < 8; i ++)				//循环8次，主机依次发送数据的每一位
	{
		I2C_W_SDA(Byte & (0x80 >> i));	    //使用掩码的方式取出Byte的指定一位数据并写入到SDA线
		I2C_W_SCL(1);						//释放SCL，从机在SCL高电平期间读取SDA
		I2C_W_SCL(0);						//拉低SCL，主机开始发送下一位数据
	}
}

uint8_t MyI2C_ReceiveByte(void)//I2C接收一个字节
{
	uint8_t i, Byte = 0x00;//定义接收的数据，并赋初值0x00，此处必须赋初值0x00，后面会用到
	I2C_W_SDA(1);//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	for (i = 0; i < 8; i ++)//循环8次，主机依次接收数据的每一位
	{
		I2C_W_SCL(1);//释放SCL，主机机在SCL高电平期间读取SDA
		if (I2C_R_SDA() == 1){Byte |= (0x80 >> i);}//读取SDA数据，并存储到Byte变量。当SDA为1时，置变量指定位为1，当SDA为0时，不做处理，指定位为默认的初值0
		I2C_W_SCL(0);//拉低SCL，从机在SCL低电平期间写入SDA
	}
	return Byte;//返回接收到的一个字节数据
}

void MyI2C_SendAck(uint8_t AckBit)//I2C发送应答位AckBit，0表示应答，1表示非应答。
{
	I2C_W_SDA(AckBit);//主机把应答位数据放到SDA线
	I2C_W_SCL(1);//释放SCL，从机在SCL高电平期间，读取应答位
	I2C_W_SCL(0);//拉低SCL，开始下一个时序模块
}

uint8_t MyI2C_ReceiveAck(void)//I2C接收应答位，0表示应答，1表示非应答
{
	uint8_t AckBit;//定义应答位变量
	I2C_W_SDA(1);//接收前，主机先确保释放SDA，避免干扰从机的数据发送
	I2C_W_SCL(1);//释放SCL，主机机在SCL高电平期间读取SDA
	AckBit = I2C_R_SDA();//将应答位存储到变量里
	I2C_W_SCL(0);//拉低SCL，开始下一个时序模块
	return AckBit;//返回定义应答位变量
}

#define AT24C256_ADDRESS		0xA0

/**
  * @brief  AT24C02写入一个字节
  * @param  WordAddress 要写入字节的地址
  * @param  Data 要写入的数据
  * @retval 无
  */
void AT24C256_WriteByte(uint16_t WordAddress,uint8_t Data)
{
	MyI2C_Start();
	MyI2C_SendByte(AT24C256_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(WordAddress>>8);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(WordAddress>>0);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(Data);
	MyI2C_ReceiveAck();
	MyI2C_Stop();
	Delay_ms(30);
}

/**
  * @brief  AT24C02读取一个字节
  * @param  WordAddress 要读出字节的地址
  * @retval 读出的数据
  */
uint8_t AT24C256_ReadByte(uint16_t WordAddress)
{
	uint8_t Data;
	MyI2C_Start();
	MyI2C_SendByte(AT24C256_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(WordAddress>>8);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(WordAddress>>0);
	MyI2C_ReceiveAck();
	MyI2C_Start();
	MyI2C_SendByte(AT24C256_ADDRESS|0x01);
	MyI2C_ReceiveAck();
	Data=MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	return Data;
}
