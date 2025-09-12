#include "MyI2C.h"

void My_I2C_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;//初始化为开漏输出
    GPIO_InitStruct.GPIO_Pin= GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
    GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

void Write_SCL(uint8_t bit)//写时钟
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)bit);
    Delay_us(DELAY_NUM);    //适当延时
}

void Write_SDA(uint8_t bit)//写数据
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)bit);
    Delay_us(DELAY_NUM);    //适当延时
}

uint8_t Read_SDA(void)//读数据
{
    uint8_t bit = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    return bit;
}

void I2C_Start(void)//I2C开始
{
    Write_SDA(1);            //保证时钟和数据线初始为高
    Write_SCL(1);            //保证时钟和数据线初始为高
    Write_SDA(0);            //拉低数据位 初始化从机
    Write_SCL(0);            //拉低时钟位 通知从机发送数据
}

void I2C_Stop(void)//I2C结束
{
    Write_SDA(0);            //拉低数据位 
    Write_SCL(1);            //释放时钟位
    Write_SDA(1);            //释放数据位 
}

void MyI2C_SendAck(uint8_t AckBit)//I2C发送应答位AckBit，0表示应答，1表示非应答。
{
	Write_SDA(AckBit);//主机把应答位数据放到SDA线
	Write_SCL(1);//释放SCL，从机在SCL高电平期间，读取应答位
	Write_SCL(0);//拉低SCL，开始下一个时序模块
}

uint8_t MyI2C_ReceiveAck(void)//I2C接收应答位，0表示应答，1表示非应答
{
	uint8_t AckBit;      //定义应答位变量
	Write_SDA(1);        //接收前，主机先确保释放SDA，避免干扰从机的数据发送
	Write_SCL(1);        //释放SCL，主机机在SCL高电平期间读取SDA
	AckBit = Read_SDA();//将应答位存储到变量里
	Write_SCL(0);        //拉低SCL，开始下一个时序模块
	return AckBit;       //返回定义应答位变量
}

void I2C_SendByte(uint8_t Byte)//发送数据：发送字节 返回ACK
{
    for(int8_t i = 7;i >= 0; i--) //循环8次
    {
        Write_SDA(Byte&(0x01<<i));
        Write_SCL(1);
        Write_SCL(0);
    }
    Write_SCL(1);         //数据传输完毕
    Write_SCL(0);         //拉高时钟 释放I2C
    //return Read_SDA();    //读取应答位
}

uint8_t I2C_SendBytePack(uint8_t ADDR, uint8_t *Data,uint8_t Size)
{
    I2C_Start();//初始化从机 通知从机开始发送
    I2C_SendByte(ADDR);
    
    for(uint8_t i = 0; i < Size; i++)
    {
        I2C_SendByte(Data[i]);   
    }
    I2C_Stop();//不发了
    return 0;
}

