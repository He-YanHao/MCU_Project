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

#include "bsp_ESP01S.h"
#include "stdio.h"
#include "board.h"
#include "hmacsha1.h"
#include "string.h"
#include <stdlib.h>

unsigned char WIFI_RX_BUFF[WIFI_RX_LEN_MAX];     
unsigned char WIFI_RX_FLAG = 0;
unsigned char WIFI_RX_LEN = 0;

uint8_t wifi_link_flag = 0;//�豸����״̬
//���    Get_Device_connection_status    ����ʹ��
// *              0=û���豸���� 
// *              1=���豸������WIFI   
// *              0=���豸�Ͽ���WIFI     
// *              2=���豸�����˷�����
// *              3=���豸�Ͽ��˷�����



/************************************************************
 * �������ƣ�WIFI_USART_Init
 * ����˵��������WIFI�ĳ�ʼ��
 * ��    �Σ�bund=���ڲ�����
 * �� �� ֵ����
 * ��    ע����
*************************************************************/
void WIFI_USART_Init(unsigned int bund)
{
	GPIO_InitTypeDef GPIO_InitStructure;        

	RCC_AHB1PeriphClockCmd(RCC_WIFI_TX,ENABLE);        // TX�˿�ʱ��
	RCC_AHB1PeriphClockCmd(RCC_WIFI_RX,ENABLE);        // RX�˿�ʱ��

	GPIO_PinAFConfig(PORT_WIFI_TX,GPIO_WIFI_TX_SOURCE, BSP_WIFI_AF);//IO��������������Ҫ���ø���ģʽ
	GPIO_PinAFConfig(PORT_WIFI_RX,GPIO_WIFI_RX_SOURCE, BSP_WIFI_AF);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin           = GPIO_WIFI_TX;//TX����
	GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_AF;//IO��������������Ҫ���ø���ģʽ
	GPIO_InitStructure.GPIO_Speed         = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType         = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd          = GPIO_PuPd_UP;
	GPIO_Init(PORT_WIFI_TX,&GPIO_InitStructure);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin           = GPIO_WIFI_RX;//RX����
	GPIO_InitStructure.GPIO_Mode          = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed         = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType         = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd          = GPIO_PuPd_UP;
	GPIO_Init(PORT_WIFI_RX,&GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;//�������ô��ڵĽṹ�����

	RCC_APB1PeriphClockCmd(RCC_WIFI_USART, ENABLE);//��������2��ʱ��

	USART_DeInit(WIFI_USART);//�����˼�ǽ���˴��ڵ���������

	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate              = bund;//���ò�����
	USART_InitStructure.USART_WordLength            = USART_WordLength_8b;//�ֽڳ���Ϊ8bit
	USART_InitStructure.USART_StopBits              = USART_StopBits_1;//1��ֹͣλ
	USART_InitStructure.USART_Parity                = USART_Parity_No ;//û��У��λ
	USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;//����������Ϊ�շ�ģʽ
	USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None; //���ṩ���� 
	USART_Init(WIFI_USART,&USART_InitStructure);//����ز�����ʼ��������2
	
	USART_ITConfig(WIFI_USART, USART_IT_RXNE, ENABLE);//��ʼ���ý����ж�
	USART_ITConfig(WIFI_USART, USART_IT_IDLE, ENABLE);//��ʼ���ý����ж�
	
	USART_ClearFlag(WIFI_USART,USART_FLAG_RXNE);//��ʼ����ʱ���������λ
	USART_ClearFlag(WIFI_USART,USART_IT_IDLE);//��ʼ����ʱ���������λ

	USART_Cmd(WIFI_USART, ENABLE);//��������2
	
	NVIC_InitTypeDef NVIC_InitStructure;//�жϿ��ƽṹ���������

	NVIC_InitStructure.NVIC_IRQChannel                    = WIFI_USART_IRQ;//�ж�ͨ��ָ��ΪUSART2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority  = 1;//�����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority         = 1;//�����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd                 = ENABLE;//ȷ��ʹ��
	NVIC_Init(&NVIC_InitStructure);//��ʼ�����ô��ж�ͨ��
}

/******************************************************************
 * �� �� �� �ƣ�WIFI_USART_Send_Bit
 * �� �� ˵ ������WIFIģ�鷢�͵����ַ�
 * �� �� �� �Σ�ch=�ַ�
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void WIFI_USART_Send_Bit(unsigned char ch)
{
    USART_SendData(WIFI_USART, (uint8_t)ch);
        
	while( RESET == USART_GetFlagStatus(WIFI_USART, USART_FLAG_TXE) ){} // �ȴ��������ݻ�������־��λ

}  


/******************************************************************
 * �� �� �� �ƣ�WIFI_USART_send_String
 * �� �� ˵ ������WIFIģ�鷢���ַ���
 * �� �� �� �Σ�str=���͵��ַ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void WIFI_USART_send_String(unsigned char *str)
{
        while( str && *str ) // ��ַΪ�ջ���ֵΪ������
        {        
                WIFI_USART_Send_Bit(*str++);
        }        
}
//������ڽ��յ�����
/******************************************************************
 * �� �� �� �ƣ�Clear_WIFI_RX_BUFF
 * �� �� ˵ �������WIFI������������
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void Clear_WIFI_RX_BUFF(void)
{
        unsigned char i = WIFI_RX_LEN_MAX-1;
        while(i)        
        {
                WIFI_RX_BUFF[i--] = 0;
        }
        WIFI_RX_LEN = 0;
        WIFI_RX_FLAG = 0;
}


/******************************************************************
 * �� �� �� �ƣ�WIFI_Send_Cmd
 * �� �� ˵ ������WIFIģ�鷢��ָ����鿴WIFIģ���Ƿ񷵻���Ҫ������
 * �� �� �� �Σ�cmd=���͵�ATָ��        ack=��Ҫ��Ӧ��                waitms=�ȴ�Ӧ���ʱ��                cnt=�ȴ�Ӧ����ٴ�
 * �� �� �� �أ�1=�õ�����Ҫ��Ӧ��                0=û�еõ���Ҫ��Ӧ��
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
char WIFI_Send_Cmd(char *cmd,char *ack,unsigned int waitms,unsigned char cnt)
{        
        WIFI_USART_send_String((unsigned char*)cmd);//1.����ATָ��
        while(cnt--)
        {
        //ʱ����
                delay_1ms(waitms);
                //�����жϽ�������Ӧ��
                if( WIFI_RX_FLAG == 1 )
                {
                        WIFI_RX_FLAG = 0;
                        WIFI_RX_LEN = 0;
            //�����Ƿ�����Ҫ������
                        if( strstr((char*)WIFI_RX_BUFF, ack) != NULL )
                        {
                                return 1;
                        }
            //������յ�����
                        memset( WIFI_RX_BUFF, 0, sizeof(WIFI_RX_BUFF) );
                }
        }
        WIFI_RX_FLAG = 0;
        WIFI_RX_LEN = 0;
        return 0;
}

/******************************************************************
 * �� �� �� �ƣ�WIFI_ESP01S_Init
 * �� �� ˵ ����WIFIģ��ESP01S��ʼ��
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע��ESP01S��Ĭ�ϲ�������115200
******************************************************************/
void WIFI_ESP01S_Init(void)
{
        WIFI_USART_Init(115200);//Ĭ�ϲ�����Ϊ115200
}

/******************************************************************
 * �� �� �� �ƣ�WIFI_MODE_AP_Init
 * �� �� ˵ ��������APģʽ����ģ�鿪���ȵ����ֻ�����
 * �� �� �� �Σ���
 * �� �� �� �أ�1=���óɹ�   ����=ʧ��
 * ��       �ߣ�LC
 * ��       ע���ֻ�ͨ��WIFIģ��Ĭ�ϵ�IP��ַ192.168.4.1�����õĶ˿ںţ���������
******************************************************************/
uint8_t WIFI_MODE_AP_Init(void)
{
    uint8_t ret = 0;
        char send_buff[200];
    
    ret = WIFI_Send_Cmd("AT\r\n", "OK", 10, 3);//����ָ�AT\r\n  �ɹ�����OK  ʧ�ܷ���ERROR
        if( ret != 1 ) return ret;
    
    ret = WIFI_Send_Cmd("AT+CWMODE=2\r\n","OK",30,3);   //����WIFI APģʽ
    if( ret != 1 ) return ret;
    
    sprintf(send_buff, "AT+CWSAP=\"%s\",\"%s\",11,4\r\n", AP_WIFISSID,AP_WIFIPASS );
        ret = WIFI_Send_Cmd(send_buff,"OK",30,3);  //����wifi�˺�������
        if( ret != 1 ) return ret;
    
    ret = WIFI_Send_Cmd("AT+RST\r\n","ready",800,3); //���¸�λ
        if( ret != 1 ) return ret;
    ret = WIFI_Send_Cmd("AT+CIPMUX=1\r\n","OK",50,3); //�����������
        if( ret != 1 ) return ret;
    ret = WIFI_Send_Cmd("AT+CIPSERVER=1,5000\r\n","OK",50,3); //�������������ö˿ں�Ϊ5000
    if( ret != 1 ) return ret;
    return ret;
}





/******************************************************************
 * �� �� �� �ƣ�Get_Device_connection_status
 * �� �� ˵ ������ȡ�豸����״̬(APģʽ)
 * �� �� �� �Σ���
 * �� �� �� �أ�0=û���豸���� 
 *              1=���豸������WIFI   
 *              2=���豸�Ͽ���WIFI     
 *              3=���豸�����˷�����
 *              4=���豸�Ͽ��˷�����
 * ��       �ߣ�LC
 * ��       ע���ֻ�Ҫ����WIFIģ��Ĳ�����������WIFI�����ӷ�����
//�����豸����APģʽ�µ��ȵ�ʱ��WIFIģ�������ӵ��豸����IP��ַ
//����ֻ�����Ƿ��з����ַ����֪���Ƿ����豸���ӡ�
//�豸����ʱWIFI���أ�
//  +STA_CONNECTED:"f0:6c:5d:d6:f6:18"
//  +DIST_STA_IP:"f0:6c:5d:d6:f6:18","192.168.4.2"
//�豸�Ͽ�����ʱ���أ�
//  +STA_DISCONNECTED:"f0:6c:5d:d6:f6:18"
******************************************************************/
uint8_t Get_Device_connection_status(void)
{
    //�����жϽ���WIFIӦ��
    if( WIFI_RX_FLAG == 1 )
    {
        WIFI_RX_FLAG = 0;
        WIFI_RX_LEN = 0;
        //���豸�������ȵ�
        if( strstr((char*)WIFI_RX_BUFF, "+STA_CONNECTED") != NULL )
        {
            //������յ�����
            wifi_link_flag = 1;
            memset( WIFI_RX_BUFF, 0, sizeof(WIFI_RX_BUFF) );
#if        DEBUG
 printf("The device is connected to a hotspot.\r\n");
 #endif            
            return 1;
        }
        //���豸�Ͽ����ȵ�
        if( strstr((char*)WIFI_RX_BUFF, "+STA_DISCONNECTED") != NULL ) 
        {
            //������յ�����
            wifi_link_flag = 0;
            memset( WIFI_RX_BUFF, 0, sizeof(WIFI_RX_BUFF) );
#if        DEBUG
 printf("The device is disconnected from the hotspot.\r\n");
 #endif 
            return 2;
        }
        //���豸�����˷�����
        if( strstr((char*)WIFI_RX_BUFF, ",CONNECT") != NULL ) 
        {
            //������յ�����
            wifi_link_flag = 2;
            memset( WIFI_RX_BUFF, 0, sizeof(WIFI_RX_BUFF) );
            return 3;
#if        DEBUG
 printf("The device is connected to the server.\r\n");
 #endif
        }        
        //���豸�Ͽ��˷�����
        if( strstr((char*)WIFI_RX_BUFF, ",CLOSED") != NULL ) 
        {
            //������յ�����
            wifi_link_flag = 3;
            memset( WIFI_RX_BUFF, 0, sizeof(WIFI_RX_BUFF) );
            return 4;
#if        DEBUG
 printf("The device is disconnected from the server.\r\n");
 #endif
        }         
    }  
    return 0;    
}


/**********************************************************
 * �� �� �� �ƣ�Get_WIFI_AP_Data
 * �� �� �� �ܣ������豸���͹���������
 * �� �� �� ����ap_parameterҪ�����ݱ���ĵ�ַ
 * �� �� �� �أ�1�����豸���͹�������        0��û���豸���͹�������
 * ��       �ߣ�LC
 * ��       ע��device_id���5��  //+IPD,1,4:abcd
**********************************************************/    
uint8_t Get_WIFI_AP_Data(AP_PARAMETER *ap_parameter)
{
    char buff[50];
    char *test;
    
    char i=0;

    //���յ��豸������������
    if( strstr((char*)WIFI_RX_BUFF,"+IPD,") != NULL )
    {
        test = strstr((char*)WIFI_RX_BUFF,"+IPD,");
        
        //��¼�豸ID��
        strncpy(buff,test+5,1);  
        buff[1] ='\0';
        ap_parameter->device_id = atoi(buff);
        printf("device_id = %s\r\n",buff);
                
        //��¼���͹��������ݳ���
        strncpy(buff,test+7,strcspn(test+7,":") );  
        buff[ strcspn(test+7,":") ] ='\0';
                printf("device_data = %s\r\n",buff);
        ap_parameter->device_datalen = atoi(buff);
        printf("device_datalen = %s\r\n",buff);
        //��¼���͹���������
        memset(buff,0,sizeof(buff));  
        while(test[i++]!=':');
        strncpy(buff, test+i,strcspn(test+i,"\r") );
                printf("device_data = %s\r\n",buff);
        strcpy((char*)ap_parameter->device_data, buff);
        
        //������ڽӽ�����
        Clear_WIFI_RX_BUFF();
        return 1;
    }
    return 0;
}
/******************************************************************
 * �� �� �� �ƣ�WIFI_Send_To_Client
 * �� �� ˵ ����APģʽ�£�WIFI�����������ͻ��ˣ�����APģʽ���ȵ���豸��
 * �� �� �� �Σ�id=��ڼ����ͻ��˷�������   data=Ҫ���͵����ݣ��ַ�����ʽ��
 * �� �� �� �أ�0=����ʧ��   1=���ͳɹ�
 * ��       �ߣ�LC
 * ��       ע��ʹ�øú���������ȷ��WIFIģ�鴦��APģʽ
******************************************************************/
uint8_t WIFI_Send_To_Client(uint8_t id,char * data)
{
        uint8_t send_buf[20]={0};
        sprintf((char*)send_buf,"AT+CIPSEND=%d,%d\r\n",id,strlen(data));
        if(WIFI_Send_Cmd((char*)send_buf,">",20,3))
        {
                WIFI_USART_send_String((unsigned char *)data);
                return 1;
        }
        return 0;        
}

/******************************************************************
 * �� �� �� �ƣ�mstrcat
 * �� �� ˵ �����ַ�������
 * �� �� �� �Σ�s1��Ŀ���ַ����� s2��Դ�ַ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����ϣʹ��
******************************************************************/
static void mstrcat(char *s1, const char *s2)
{
        if(*s1 != NULL)
                while(*++s1);
        while((*s1++ = *s2++));
}

/******************************************************************
 * �� �� �� �ƣ�CalculateSha1
 * �� �� ˵ ��������sha1�ܳ�
 * �� �� �� �Σ�password���ܳ״�Ż�����
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
static void CalculateSha1(unsigned char *password)         
{           
        unsigned char temp[3] = {0};
        unsigned char digest[30]={0};
        unsigned char cnt = 0;
        hmac_sha1((unsigned char *)DeviceSecret,32,(unsigned char *)Encryption,46,digest);
        memset(temp, 0, sizeof(temp));
        for(cnt=0;cnt<20;cnt++)
        {
                sprintf((char *)temp,"%02X",digest[cnt]);
                mstrcat((char *)password,(char *)temp);
        }
}

/******************************************************************
 * �� �� �� �ƣ�WIFI_MODE_STA_Aliyun_Init
 * �� �� ˵ ��������WIFIģ�����Ӱ�����������ƽ̨
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void WIFI_MODE_STA_Aliyun_Init(void)
{
        char AT_CMD[250]={0};
        uint8_t PassWord[50] = {0};  //��ŵ��ǹ�ϣ�������Կ
        
        RST:
    //����ָ��AT
        WIFI_Send_Cmd("AT\r\n","OK",100,3);

        //����WIFI STA
        WIFI_Send_Cmd("AT+CWMODE=1\r\n","OK",300,3);
    
        //����ʱ��  NTSP������  ���ڵ����ͻ�����������ϵͳ��ʱ�䣬�ﵽͬ��ʱ���Ŀ��        
        WIFI_Send_Cmd("AT+CIPSNTPCFG=1,8,\"ntp1.alliyun.com\"\r\n","OK",100,3);
    
        //����wifi �˺�&����
        sprintf(AT_CMD,"AT+CWJAP=\"%s\",\"%s\"\r\n",WIFISSID,WIFIPASS);
    
        if( WIFI_Send_Cmd(AT_CMD,"OK",3000,3) == 0 )
        {
                printf("WIFI���ƻ������д�,��λ����\r\n");
        //wifi���Ӳ��ϣ�����
                WIFI_Send_Cmd("AT+RST\r\n","ready",1000,1);  
                goto RST;
        }
        //��0���飬����
        memset(AT_CMD,0,sizeof(AT_CMD));  
    
    //�����ϣ     
        CalculateSha1(PassWord);        

    #if        DEBUG
//    sprintf(PassWord,"%s","AF7596ADFFD4C57C5FD43F1CA1A20DE961634360");
        printf("haxi=%s\r\n",PassWord);
        printf("UserName=%s\r\n",UserName);
    #endif
    
        sprintf(AT_CMD,"AT+MQTTUSERCFG=0,1,\"NULL\",\"%s\",\"%s\",0,0,\"\"\r\n", UserName, PassWord);
        WIFI_Send_Cmd(AT_CMD,"OK",2000,3);
        
        //�������ӿͻ���ID
        memset(AT_CMD,0,sizeof(AT_CMD));  //��0���飬����
        sprintf(AT_CMD,"AT+MQTTCLIENTID=0,\"%s\"\r\n",ClientId);
        WIFI_Send_Cmd(AT_CMD,"OK",1000,3);
        
        //���ӵ�MQTT����������ƽ̨��
        memset(AT_CMD,0,sizeof(AT_CMD));
        sprintf(AT_CMD,"AT+MQTTCONN=0,\"%s\",1883,1\r\n",IP);
        if(WIFI_Send_Cmd(AT_CMD,"OK",2000,3)==0)
        {
                printf("����aliyuʧ��,��λSTM32����\r\n");
        //wifi���Ӳ��ϣ�����  1000��ʱ1S    2���Ӵ���
                WIFI_Send_Cmd("AT+RST\r\n","ready",1000,2);  
                __set_FAULTMASK(1); //STM32���������λ
                NVIC_SystemReset();
        }
        
        //��������
        memset(AT_CMD,0,sizeof(AT_CMD));
        sprintf(AT_CMD, "AT+MQTTSUB=0,\"%s\",1\r\n", PublishMessageTopSet);
        WIFI_Send_Cmd(AT_CMD,"OK",1000,3);
        printf("����aliyu�ɹ�\r\n");
        Clear_WIFI_RX_BUFF();//������ڽ��ջ���
    //�ϵ���ϴ��������ֻ�
//        Publish_MQTT_message(publish_mqtt_message,5,); //��������             
}    




/*���LED����
+MQTTSUBRECV:0,"/sys/a1PJRLOWo3p/TEST/thing/service/property/set",100,
{"method":"thing.service.property.set","id":"367399823","params":{"LED_Switch":1},"version":"1.0.0"}
*/

/*����������
+MQTTSUBRECV:0,"/sys/a1PJRLOWo3p/TEST/thing/service/property/set",101,
{"method":"thing.service.property.set","id":"812539841","params":{"brightness":75},"version":"1.0.0"}
*/
/******************************************************************
 * �� �� �� �ƣ�Get_Aliyun_json_data
 * �� �� ˵ ������ȡ������JSON��ʽ�����ݣ������ֻ����͹��������ݣ�
 * �� �� �� �Σ�data=���ݵı����ַ
 * �� �� �� �أ�1=���յ�JSON���ݲ�����   0=û�н��յ�����
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
uint8_t Get_Aliyun_json_data(JSON_PUBLISH *data)
{
        char *buff=0;
    //�����жϽ���WIFIӦ��
    if( WIFI_RX_FLAG == SET )
    {
                printf("\r\n--\r\n");
        WIFI_RX_FLAG = 0;
        WIFI_RX_LEN = 0;
        //���豸�������ȵ�
        if( strstr((char*)WIFI_RX_BUFF, "params\":") != NULL )
        {
                        //��ȡ��������
                        buff = strstr((char*)WIFI_RX_BUFF, "params\":");
                        buff += strlen("params\":{\"");
                        strcpy(data->keyname,strtok(buff,"\""));
                        printf("data->keyname = %s\r\n",data->keyname);
                        //��ȡ����ֵ
                        buff = strstr((char*)WIFI_RX_BUFF, "params\":" );
                        buff += strlen("params\":{\"")+strlen(data->keyname)+2;
                        strcpy(data->value, strtok(buff,"}"));
            return 1;
        }
        }
    return 0;
}


/******************************************************************
 * �� �� �� �ƣ�Clear_Aliyun_json_data
 * �� �� ˵ �������JSON���չ�������
 * �� �� �� �Σ�data=Ҫ���������
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void Clear_Aliyun_json_data(JSON_PUBLISH *data)
{
        uint16_t i = 0;
        while( data->keyname[i] != 0 )
        {
                data->keyname[i++] = '\0';
        }
        i= 0;
        while( data->value[i] != 0 )
        {
                data->keyname[i++] = '\0';
        }
}

//���Զ��幦��ʱ���ж��ٸ���ʶ��������Ӷ��ٸ�
//�Ҿ�ֻ��������ʶ����
//LED_Switch=LED���� ��Χ0-1
//brightness=����    ��Χ0-100
JSON_PUBLISH publish_mqtt_message[2]=        
{
        { "\\\"LED_Switch\\\":","0" },
        { "\\\"brightness\\\":","0" }
};            

/******************************************************************
 * �� �� �� �ƣ�Publish_MQTT_message
 * �� �� ˵ ������������ ���Ϸ��������(�豸�����ݷ������ֻ�)
 * �� �� �� �Σ�data=publish_mqtt_message�� data_num=�ϴ������ݸ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void Publish_MQTT_message(JSON_PUBLISH *data,uint8_t data_num)  
{
        char AT_CMD[384]={0};
        char params[256]={0},i,*sp;
        
        sp=params;
        
        sprintf(data[0].value,"%d",0);   //�Ѵ�������ֵ��ֵ��json�ṹ���value
        sprintf(data[1].value,"%d",90);
        
        //          4
        for(i=0;i<data_num;i++)
        {        // 3
                if(i<(data_num-1))
                {   
                        sprintf(sp,"%s%s%s",data[i].keyname,data[i].value,"\\,");
                        while(*sp!=0) {sp++;} //��ֹ����
                }
                else
                        sprintf(sp,"%s%s",data[i].keyname,data[i].value);
        }
        sprintf(AT_CMD,"AT+MQTTPUB=0,\"%s\",\"{\\\"params\\\":{%s}}\",1,0\r\n",PublishMessageTopPost,params);
        //����ָ���ȴ�2000ms���ж��Ƿ��ͳɹ��������ͳɹ����ط�3-1��
    //���������CPU������,����ֱ��ʹ��    WIFI_USART_send_String(AT_CMD); ���ͣ������޷��ж��Ƿ��ͳɹ���
    WIFI_Send_Cmd(AT_CMD,"OK",2000,3);                
}


/******************************************************************
 * �� �� �� �ƣ�WIFI_USART_IRQHandler
 * �� �� ˵ ��������WIFI�Ĵ����жϷ�����
 * �� �� �� �Σ���
 * �� �� �� �أ���
 * ��       �ߣ�LC
 * ��       ע����
******************************************************************/
void WIFI_USART_IRQHandler(void)
{
        if(USART_GetITStatus(WIFI_USART, USART_IT_RXNE) == SET) // ���ջ�������Ϊ��
        {
        //��������
                WIFI_RX_BUFF[ WIFI_RX_LEN ] = USART_ReceiveData(WIFI_USART);
        
#if DEBUG
        //���ԣ��鿴���յ���ʲô����
        printf("%c", WIFI_RX_BUFF[ WIFI_RX_LEN ]);
#endif
                //���ճ�������
        WIFI_RX_LEN = ( WIFI_RX_LEN + 1 ) % WIFI_RX_LEN_MAX;
        }
        if(USART_GetITStatus(WIFI_USART, USART_IT_IDLE) == SET) // ��⵽�����ж�
        {
                volatile uint32_t temp;
                temp = WIFI_USART->SR; // ��ȡ״̬�Ĵ��������IDLE��־
                temp = WIFI_USART->DR; // ��ȡ���ݼĴ��������IDLE��־
			
                WIFI_RX_BUFF[WIFI_RX_LEN] = '\0'; //�ַ�����β�� '\0'
                WIFI_RX_FLAG = SET;            // �������
        }
}

