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
#include "board.h"
#include "bsp_uart.h"
#include <stdio.h>
#include "bsp_esp01s.h"

int main(void)
{
	AP_PARAMETER ap_parameter={0};
	uint8_t test_ret = 0;
	
	board_init();
	
	uart1_init(115200U);

    printf("start\r\n");
    //WIFI��ʼ��
    WIFI_ESP01S_Init();
    //����WIFIģ��ΪAPģʽ
    WIFI_MODE_AP_Init();
    while(1)
    {
        //�ж��Ƿ����豸����
        Get_Device_connection_status();
        //�����ǰ�豸�����˷��������ֻ�APP�������ȵ㣩
        if( wifi_link_flag == 2 )
        {
            if( Get_WIFI_AP_Data(&ap_parameter) == 1 )//������յ�����
            {
                                //������յ�����
                printf("ID = %d\r\n", ap_parameter.device_id );//�豸ID
                printf("data len = %d\r\n", ap_parameter.device_datalen );//���͹��������ݳ���
                printf("data = %s\r\n", ap_parameter.device_data );//���͹���������
                                
                                
            }
        }
    }
	

}
