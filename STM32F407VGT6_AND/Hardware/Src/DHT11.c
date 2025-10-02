#include "DHT11.h"

uint8_t rec_data[4] = {0, 0, 0, 0};  // ��ʼ������

void DHT11_Init(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // �����ٶȣ�
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    DHT11_UP;
}

void DHT11_Mode_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // �����ٶȣ�
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void DHT11_Mode_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;  // �����ٶȣ�
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// ����ʱ���Ŀ�ʼ�źź���
uint8_t DHT11_Start(void)
{
    uint32_t timeout = 100000;
    
    DHT11_Mode_OUT();
    DHT11_LOW;
    Delay_ms(18);
    DHT11_UP;
    Delay_us(30);
    DHT11_Mode_IN();
    
    // �ȴ�DHT11��Ӧ��80us�͵�ƽ��
    timeout = 100000;
    while(DATA_GPIO_IN == 1 && timeout--) {
        if(timeout == 0) return 0;  // ��ʱ
    }
    
    // �ȴ�DHT11���ߣ�80us�ߵ�ƽ��
    timeout = 100000;
    while(DATA_GPIO_IN == 0 && timeout--) {
        if(timeout == 0) return 0;  // ��ʱ
    }
    
    return 1;  // ��ʼ�ɹ�
}

// ����ʱ�����ֽڶ�ȡ����
uint8_t DHT11_Rec_Byte(void)
{
    uint8_t i, data = 0;
    
    for(i = 0; i < 8; i++)
    {
        uint32_t timeout = 100000;
        
        // �ȴ��͵�ƽ������50us��
        while(DATA_GPIO_IN == 0 && timeout--) {
            if(timeout == 0) return 0;  // ��ʱ
        }
        
        Delay_us(40);  // ��ʱ40us�жϵ�ƽ
        
        data <<= 1;
        if(DATA_GPIO_IN == 1)
        {
            data |= 1;
            // �ȴ��ߵ�ƽ����
            timeout = 100000;
            while(DATA_GPIO_IN == 1 && timeout--) {
                if(timeout == 0) return 0;  // ��ʱ
            }
        }
    }
    return data;
}
#include <stdio.h>
uint8_t DHT11_Read_Data_Diagnostic(void)
{
    printf("\n=== DHT11 Communication Diagnostic ===\n");
    
    // 1. ���Ϳ�ʼ�ź�
    printf("1. Sending start signal...\n");
    DHT11_Mode_OUT();
    DHT11_LOW;
    Delay_ms(18);
    DHT11_UP;
    Delay_us(30);
    
    // 2. �л�����ģʽ
    printf("2. Switching to input mode...\n");
    DHT11_Mode_IN();
    
    // 3. �ȴ��͵�ƽ��Ӧ��Ӧ��80us����Ӧ��
    printf("3. Waiting for LOW response (80us)...\n");
    uint32_t timeout = 100000;  // Լ100ms��ʱ
    uint32_t start_time = timeout;
    
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 1) {
        timeout--;
        if(timeout == 0) {
            printf("ERROR: DHT11 did not respond with LOW within timeout!\n");
            printf("Check: 1) Pull-up resistor 2) Power supply 3) DHT11 module\n");
            DHT11_Mode_OUT();
            DHT11_UP;
            return 0;
        }
    }
    
    uint32_t low_duration = start_time - timeout;
    printf("SUCCESS: DHT11 responded with LOW after %lu cycles\n", low_duration);
    
    // 4. �ȴ��͵�ƽ������80us�����ߣ�
    printf("4. Waiting for HIGH after LOW response...\n");
    timeout = 100000;
    start_time = timeout;
    
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0) {
        timeout--;
        if(timeout == 0) {
            printf("ERROR: DHT11 stuck in LOW state!\n");
            DHT11_Mode_OUT();
            DHT11_UP;
            return 0;
        }
    }
    
    uint32_t high_duration = start_time - timeout;
    printf("SUCCESS: DHT11 pulled HIGH after %lu cycles\n", high_duration);
    
    // 5. �ȴ����ݿ�ʼ��80us�ߵ�ƽ��ʼ���ݴ��䣩
    printf("5. Waiting for data transmission start...\n");
    timeout = 100000;
    start_time = timeout;
    
    while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 1) {
        timeout--;
        if(timeout == 0) {
            printf("ERROR: Data transmission did not start!\n");
            DHT11_Mode_OUT();
            DHT11_UP;
            return 0;
        }
    }
    
    uint32_t data_start = start_time - timeout;
    printf("SUCCESS: Data transmission started after %lu cycles\n", data_start);
    
    // ��������˵������ͨ�����������Գ��Զ�ȡ����
    printf("6. Basic communication established, attempting to read data...\n");
    
    DHT11_Mode_OUT();
    DHT11_UP;
    return 1;
}