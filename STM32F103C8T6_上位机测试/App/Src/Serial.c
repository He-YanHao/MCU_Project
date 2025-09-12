#include "Serial.h"


/**
  * ��    �������ڳ�ʼ��
  * ��    ������
  * �� �� ֵ����
  */
void Serial_Init(void)
{
	/*����ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	//����USART1��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//����GPIOA��ʱ��
	
	/*GPIO��ʼ��*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��PA9���ų�ʼ��Ϊ�����������
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��PA10���ų�ʼ��Ϊ��������
	
	/*USART��ʼ��*/
	USART_InitTypeDef USART_InitStructure;					//����ṹ�����
	USART_InitStructure.USART_BaudRate = 9600;				//������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ�������ƣ�����Ҫ
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	//ģʽ������ģʽ�ͽ���ģʽ��ѡ��
	USART_InitStructure.USART_Parity = USART_Parity_No;		//��żУ�飬����Ҫ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//ֹͣλ��ѡ��1λ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ���ѡ��8λ
	USART_Init(USART1, &USART_InitStructure);				//���ṹ���������USART_Init������USART1
	
	/*�ж��������*/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//�������ڽ������ݵ��ж�
	
	/*NVIC�жϷ���*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//����NVICΪ����2
	
	/*NVIC����*/
	NVIC_InitTypeDef NVIC_InitStructure;					//����ṹ�����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		//ѡ������NVIC��USART1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//ָ��NVIC��·ʹ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//ָ��NVIC��·����ռ���ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//ָ��NVIC��·����Ӧ���ȼ�Ϊ1
	NVIC_Init(&NVIC_InitStructure);							//���ṹ���������NVIC_Init������NVIC����
	
	/*USARTʹ��*/
	USART_Cmd(USART1, ENABLE);								//ʹ��USART1�����ڿ�ʼ����
}

/**
  * ��    �������ڷ���һ���ֽ�
  * ��    ����Byte Ҫ���͵�һ���ֽ�
  * �� �� ֵ����
  */
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);		//���ֽ�����д�����ݼĴ�����д���USART�Զ�����ʱ����
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);	//�ȴ��������
	/*�´�д�����ݼĴ������Զ����������ɱ�־λ���ʴ�ѭ�������������־λ*/
}

/**
  * ��    �������ڷ���һ������
  * ��    ����Array Ҫ����������׵�ַ
  * ��    ����Length Ҫ��������ĳ���
  * �� �� ֵ����
  */
void Serial_SendArray(uint16_t Length, uint8_t *Array)
{
	uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		Serial_SendByte(Array[i]);		//���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

/**
  * ��    �������ڷ���һ���ַ���
  * ��    ����String Ҫ�����ַ������׵�ַ
  * �� �� ֵ����
  */
void Serial_SendString(char *String)
{
	uint8_t i;
	for (i = 0; String[i] != '\0'; i ++)//�����ַ����飨�ַ������������ַ���������־λ��ֹͣ
	{
		Serial_SendByte(String[i]);		//���ε���Serial_SendByte����ÿ���ֽ�����
	}
}

//֡ͷΪ0xAA
//У��Ϊ����λ���
void Serial_SendPack(uint16_t Length, uint8_t *Array)
{
    uint8_t Checksum = 0;
	uint16_t i;
	for (i = 0; i < Length; i ++)		//��������
	{
		Checksum += Array[i];		//���ε���Serial_SendByte����ÿ���ֽ�����
	}
    Serial_SendByte(0xAA);
    Serial_SendByte(Length);
    Serial_SendArray(Length, Array);
    Serial_SendByte(Checksum);
}

/**
  * ��    ����USART1�жϺ���
  * ��    ������
  * �� �� ֵ����
  * ע������˺���Ϊ�жϺ�����������ã��жϴ������Զ�ִ��
  *           ������ΪԤ����ָ�����ƣ����Դ������ļ�����
  *           ��ȷ����������ȷ���������κβ��죬�����жϺ��������ܽ���
  */
uint8_t RxDataPack[100] = {};
volatile uint8_t Serial_RxFlag = 0;//��־λ
uint8_t  RxState  = 0; //����״̬
uint8_t  BitState = 0; //����λ��
uint8_t  Checksum = 0; //����λ��
uint16_t Length   = 0; //�������ݰ�����
uint8_t  RxData   = 0; //��ʱ�������
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
    {
//        if(USART_ReceiveData(USART1) != 0)
//        {
//            OLED_ShowHexNum(2, 1, USART_ReceiveData(USART1), 4);
//        }
        RxData = USART_ReceiveData(USART1);//��ȡ���ݼĴ���������ڽ��յ����ݱ���
        /*��ǰ״̬Ϊ0���������ݰ���ͷ*/
		if (RxState == 0)      //RxState=0��˼��û��ʼ��������ͷ
		{
			if (RxData == 0xAA)//��������ȷʵ�ǰ�ͷ
			{
				RxState = 1;   //��ʼ������һλ
			}
		}
		/*��ǰ״̬Ϊ1���������ݰ�����*/
		else if (RxState == 1)//RxState=1��˼�ǽ���������ͷ����ʼ�������ݳ��ȡ�
		{
            RxDataPack[BitState] = RxData;   //RxDataPack[0]�����ݳ���
            BitState++;
            RxState = 2;
        }
		else if (RxState == 2)//RxState=2��˼�ǽ��������ݳ��ȣ���ʼ��������λ��
        {
            RxDataPack[BitState] = RxData; //RxDataPack[1]��ʼ�����ݱ���
            Checksum += RxData;
            BitState++;
            if(BitState-1 == RxDataPack[0])//��������λ=���������ݳ��� ��ʼ����У��λ
            {
                RxState = 3;
            }
        }
        else if (RxState == 3)//��ʼ����У��λ
        {
            if(Checksum == RxData)
            {
                //OK
                //Serial_SendPack(Length, RxDataPack);
            }
            Serial_RxFlag = 1;
            RxState = 0;//����״̬
            BitState = 0;//����λ��
            Checksum = 0;//����λ��
            Length = 0;//�������ݰ�����
        }
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);//�����־λ
    }
}
