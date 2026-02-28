/*
 * 立创开发板软硬件资料与相关扩展板软硬件资料官网全部开源
 * 开发板官网：www.lckfb.com
 * 技术支持常驻论坛，任何技术问题欢迎随时交流学习
 * 立创论坛：https://oshwhub.com/forum
 * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
 * 不靠卖板赚钱，以培养中国工程师为己任
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
    //WIFI初始化
    WIFI_ESP01S_Init();
    //设置WIFI模块为AP模式
    WIFI_MODE_AP_Init();
    while(1)
    {
        //判断是否有设备连接
        Get_Device_connection_status();
        //如果当前设备连接了服务器（手机APP连接上热点）
        if( wifi_link_flag == 2 )
        {
            if( Get_WIFI_AP_Data(&ap_parameter) == 1 )//如果接收到数据
            {
                                //输出接收的数据
                printf("ID = %d\r\n", ap_parameter.device_id );//设备ID
                printf("data len = %d\r\n", ap_parameter.device_datalen );//发送过来的数据长度
                printf("data = %s\r\n", ap_parameter.device_data );//发送过来的数据
                                
                                
            }
        }
    }
	

}
