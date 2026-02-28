#ifndef __ESP01S_H
#define __ESP01S_H

#include "stm32f4xx.h"
#include "string.h"

//是否开启串口0调试，查看WIFI回显数据
#define     DEBUG   1

/****************************   STA模式    ****************************/
//STA模式下，WIFI模块要连接的热点（需要自行修改为自己的参数）
#define  WIFISSID       "aaa"               //wifi热点名称
#define  WIFIPASS       "12345678"          //wifi热点密码

//连接的阿里云IP端口（不可修改）
#define  IP             "203.107.45.14"     //阿里云服务器IP或域名
//#define  IP           "a1PaQm83lGg.iot-as-mqtt.cn-shenzhen.aliyuncs.com"   //服务器IP或域名
#define  PORT           1883                            //连接端口号，MQTT默认1883

//阿里云三元组（需要自行修改为自己的参数）
#define DeviceName                 "TEST"                   //设备名称
#define ProductKey                 "a1PJRLOWo3p"            //产品密匙
#define DeviceSecret        "3e7a1a43694b65abdcbc4f5c724e9faf"        //设备密匙

//计算哈希算法用到加密（不可修改）
#define Client                 "clientId123deviceName"
#define productKey  "productKey"
#define Encryption  (Client DeviceName productKey ProductKey)
//AT指令的
#define AND "&"
#define ClientId "123|securemode=3\\,signmethod=hmacsha1|"                //客户端ID
#define UserName (DeviceName AND ProductKey)                                        //用户名

//订阅发布的主题（不可修改）
#define SYS "/sys/"
#define LINK "/"
#define TOP "/thing/event/property/"
#define POST "post"
#define ESET  "set"
#define PublishMessageTopPost         (SYS ProductKey LINK DeviceName TOP POST)
#define PublishMessageTopSet         (SYS ProductKey LINK DeviceName TOP ESET)

//上传数据结构体
typedef struct
{
        char keyname[50];   //键读取
        char value[20];     //读取到的值，类型字符串
}JSON_PUBLISH;

extern  JSON_PUBLISH  publish_mqtt_message[2];

/****************************   AP模式    ****************************/

//AP模式下，WIFI开启的热点
#define AP_WIFISSID "ESP-01S"
#define AP_WIFIPASS "12345678"

typedef struct
{
    uint8_t device_id;
    uint8_t device_datalen;
    uint8_t device_data[200];
}AP_PARAMETER;



/****************************   串口配置    ****************************/
#define RCC_WIFI_TX         RCC_AHB1Periph_GPIOA   // 串口TX的端口时钟
#define RCC_WIFI_RX         RCC_AHB1Periph_GPIOA   // 串口RX的端口时钟
#define RCC_WIFI_USART      RCC_APB1Periph_USART2  // 串口2的时钟

#define PORT_WIFI_TX        GPIOA               // 串口TX的端口
#define PORT_WIFI_RX        GPIOA               // 串口RX的端口

#define GPIO_WIFI_TX        GPIO_Pin_2                  // 串口TX的引脚
#define GPIO_WIFI_TX_SOURCE GPIO_PinSource2           // 串口TX的引脚
#define GPIO_WIFI_RX        GPIO_Pin_3                  // 串口RX的引脚
#define GPIO_WIFI_RX_SOURCE GPIO_PinSource3           // 串口RX的引脚

#define BSP_WIFI_AF         GPIO_AF_USART2           // 串口2的复用功能

#define WIFI_USART               USART2                      // 串口2
#define WIFI_USART_IRQ           USART2_IRQn                 // 串口2中断
#define WIFI_USART_IRQHandler    USART2_IRQHandler        // 串口2中断服务函数


#define     WIFI_RX_LEN_MAX     200       //串口接收最大长度

extern uint8_t wifi_link_flag;//设备连接状态
//配合    Get_Device_connection_status    函数使用
// *              0=没有设备连接
// *              1=有设备连接了WIFI
// *              0=有设备断开了WIFI
// *              2=有设备连接了服务器
// *              3=有设备断开了服务器


void WIFI_ESP01S_Init(void);            //WIFI模块初始化
uint8_t WIFI_MODE_AP_Init(void);        //AP模式初始化
uint8_t Get_Device_connection_status(void);//获取连接状态
void WIFI_MODE_STA_Aliyun_Init(void);   //连接阿里云初始化
uint8_t Get_WIFI_AP_Data(AP_PARAMETER *ap_parameter);
uint8_t Get_Aliyun_json_data(JSON_PUBLISH *data);
void Clear_Aliyun_json_data(JSON_PUBLISH *data);
void Publish_MQTT_message(JSON_PUBLISH *data,uint8_t data_num);

#endif
