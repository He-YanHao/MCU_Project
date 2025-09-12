#include "MyI2C.h"

void My_I2C_GPIO_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_OD;//��ʼ��Ϊ��©���
    GPIO_InitStruct.GPIO_Pin= GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);
    GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);
}

void Write_SCL(uint8_t bit)//дʱ��
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)bit);
    Delay_us(DELAY_NUM);    //�ʵ���ʱ
}

void Write_SDA(uint8_t bit)//д����
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)bit);
    Delay_us(DELAY_NUM);    //�ʵ���ʱ
}

uint8_t Read_SDA(void)//������
{
    uint8_t bit = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    return bit;
}

void I2C_Start(void)//I2C��ʼ
{
    Write_SDA(1);            //��֤ʱ�Ӻ������߳�ʼΪ��
    Write_SCL(1);            //��֤ʱ�Ӻ������߳�ʼΪ��
    Write_SDA(0);            //��������λ ��ʼ���ӻ�
    Write_SCL(0);            //����ʱ��λ ֪ͨ�ӻ���������
}

void I2C_Stop(void)//I2C����
{
    Write_SDA(0);            //��������λ 
    Write_SCL(1);            //�ͷ�ʱ��λ
    Write_SDA(1);            //�ͷ�����λ 
}

void MyI2C_SendAck(uint8_t AckBit)//I2C����Ӧ��λAckBit��0��ʾӦ��1��ʾ��Ӧ��
{
	Write_SDA(AckBit);//������Ӧ��λ���ݷŵ�SDA��
	Write_SCL(1);//�ͷ�SCL���ӻ���SCL�ߵ�ƽ�ڼ䣬��ȡӦ��λ
	Write_SCL(0);//����SCL����ʼ��һ��ʱ��ģ��
}

uint8_t MyI2C_ReceiveAck(void)//I2C����Ӧ��λ��0��ʾӦ��1��ʾ��Ӧ��
{
	uint8_t AckBit;      //����Ӧ��λ����
	Write_SDA(1);        //����ǰ��������ȷ���ͷ�SDA��������Ŵӻ������ݷ���
	Write_SCL(1);        //�ͷ�SCL����������SCL�ߵ�ƽ�ڼ��ȡSDA
	AckBit = Read_SDA();//��Ӧ��λ�洢��������
	Write_SCL(0);        //����SCL����ʼ��һ��ʱ��ģ��
	return AckBit;       //���ض���Ӧ��λ����
}

void I2C_SendByte(uint8_t Byte)//�������ݣ������ֽ� ����ACK
{
    for(int8_t i = 7;i >= 0; i--) //ѭ��8��
    {
        Write_SDA(Byte&(0x01<<i));
        Write_SCL(1);
        Write_SCL(0);
    }
    Write_SCL(1);         //���ݴ������
    Write_SCL(0);         //����ʱ�� �ͷ�I2C
    //return Read_SDA();    //��ȡӦ��λ
}

uint8_t I2C_SendBytePack(uint8_t ADDR, uint8_t *Data,uint8_t Size)
{
    I2C_Start();//��ʼ���ӻ� ֪ͨ�ӻ���ʼ����
    I2C_SendByte(ADDR);
    
    for(uint8_t i = 0; i < Size; i++)
    {
        I2C_SendByte(Data[i]);   
    }
    I2C_Stop();//������
    return 0;
}

