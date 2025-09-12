/*
 * ������������Ӳ�������������չ����Ӳ�����Ϲ���ȫ����Դ
 * �����������www.lckfb.com
 * ����֧�ֳ�פ��̳���κμ������⻶ӭ��ʱ����ѧϰ
 * ������̳��https://oshwhub.com/forum
 * ��עbilibili�˺ţ������������塿���������ǵ����¶�̬��
 * ��������׬Ǯ���������й�����ʦΪ����
 * 
 Change Logs:
 * Date           Author       Notes
 * 2024-03-21     LCKFB-LP    first version
 */
#ifndef _BSP_ESP01S_H_
#define _BSP_ESP01S_H_
 
#include "stm32f4xx.h" 
#include "string.h"  


//�Ƿ�������0���ԣ��鿴WIFI��������
#define     DEBUG   1

/****************************   STAģʽ    ****************************/
//STAģʽ�£�WIFIģ��Ҫ���ӵ��ȵ㣨��Ҫ�����޸�Ϊ�Լ��Ĳ�����
#define  WIFISSID       "aaa"               //wifi�ȵ�����
#define  WIFIPASS       "12345678"          //wifi�ȵ�����

//���ӵİ�����IP�˿ڣ������޸ģ�
#define  IP             "203.107.45.14"     //�����Ʒ�����IP������
//#define  IP           "a1PaQm83lGg.iot-as-mqtt.cn-shenzhen.aliyuncs.com"   //������IP������
#define  PORT           1883                            //���Ӷ˿ںţ�MQTTĬ��1883

//��������Ԫ�飨��Ҫ�����޸�Ϊ�Լ��Ĳ�����
#define DeviceName                 "TEST"                   //�豸����
#define ProductKey                 "a1PJRLOWo3p"            //��Ʒ�ܳ�
#define DeviceSecret        "3e7a1a43694b65abdcbc4f5c724e9faf"        //�豸�ܳ�
                    
//�����ϣ�㷨�õ����ܣ������޸ģ�
#define Client                 "clientId123deviceName"                                        
#define productKey  "productKey"
#define Encryption  (Client DeviceName productKey ProductKey)
//ATָ���
#define AND "&"
#define ClientId "123|securemode=3\\,signmethod=hmacsha1|"                //�ͻ���ID
#define UserName (DeviceName AND ProductKey)                                        //�û���
                     
//���ķ��������⣨�����޸ģ�
#define SYS "/sys/"
#define LINK "/"
#define TOP "/thing/event/property/"
#define POST "post"
#define ESET  "set"
#define PublishMessageTopPost         (SYS ProductKey LINK DeviceName TOP POST)
#define PublishMessageTopSet         (SYS ProductKey LINK DeviceName TOP ESET)

//�ϴ����ݽṹ��
typedef struct
{
        char keyname[50];   //����ȡ
        char value[20];     //��ȡ����ֵ�������ַ���
}JSON_PUBLISH;

extern  JSON_PUBLISH  publish_mqtt_message[2];

/****************************   APģʽ    ****************************/

//APģʽ�£�WIFI�������ȵ�
#define AP_WIFISSID "ESP-01S"
#define AP_WIFIPASS "12345678"

typedef struct
{
    uint8_t device_id;
    uint8_t device_datalen;
    uint8_t device_data[200];
}AP_PARAMETER;



/****************************   ��������    ****************************/
#define RCC_WIFI_TX         RCC_AHB1Periph_GPIOA   // ����TX�Ķ˿�ʱ��
#define RCC_WIFI_RX         RCC_AHB1Periph_GPIOA   // ����RX�Ķ˿�ʱ��
#define RCC_WIFI_USART      RCC_APB1Periph_USART2  // ����2��ʱ��

#define PORT_WIFI_TX        GPIOA               // ����TX�Ķ˿�
#define PORT_WIFI_RX        GPIOA               // ����RX�Ķ˿�

#define GPIO_WIFI_TX        GPIO_Pin_2  		// ����TX������
#define GPIO_WIFI_TX_SOURCE GPIO_PinSource2   	// ����TX������
#define GPIO_WIFI_RX        GPIO_Pin_3  		// ����RX������
#define GPIO_WIFI_RX_SOURCE GPIO_PinSource3   	// ����RX������

#define BSP_WIFI_AF         GPIO_AF_USART2   	// ����2�ĸ��ù���

#define WIFI_USART               USART2                      // ����2
#define WIFI_USART_IRQ           USART2_IRQn                 // ����2�ж�
#define WIFI_USART_IRQHandler    USART2_IRQHandler        // ����2�жϷ�����


#define     WIFI_RX_LEN_MAX     200       //���ڽ�����󳤶�

extern uint8_t wifi_link_flag;//�豸����״̬
//���    Get_Device_connection_status    ����ʹ��
// *              0=û���豸���� 
// *              1=���豸������WIFI   
// *              0=���豸�Ͽ���WIFI     
// *              2=���豸�����˷�����
// *              3=���豸�Ͽ��˷�����


void WIFI_ESP01S_Init(void);            //WIFIģ���ʼ��
uint8_t WIFI_MODE_AP_Init(void);        //APģʽ��ʼ��
uint8_t Get_Device_connection_status(void);//��ȡ����״̬
void WIFI_MODE_STA_Aliyun_Init(void);   //���Ӱ����Ƴ�ʼ��
uint8_t Get_WIFI_AP_Data(AP_PARAMETER *ap_parameter);
uint8_t Get_Aliyun_json_data(JSON_PUBLISH *data);
void Clear_Aliyun_json_data(JSON_PUBLISH *data);
void Publish_MQTT_message(JSON_PUBLISH *data,uint8_t data_num);
#endif

