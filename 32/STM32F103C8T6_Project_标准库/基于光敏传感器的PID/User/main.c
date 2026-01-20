#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

#define ON GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(0))
#define OFF GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)(1))

#define TARGET 3000 //目标值
#define T 10

void PID(void)
{
    int32_t ADValue = 0, Error = 0, ErrorLiShi[T] = {0}, ErrorLeiJia = 0;			//定义AD值变量
    uint32_t i = 0;

    double Actual = 0, PIDout = 0;//定义实际值
    double Kp = 0.5, KpOut = 0;//定义比例系数
    double Ki = 0.5, KiOut = 0;//定义积分系数
    double Kd = 0.5, KdOut = 0;//定义微分系数

    //目标值获取
    for(uint8_t i = 0; i < 100; i++)
    {
        Actual = Actual + AD_GetValue();//获取实际值
    }
    Actual = Actual/100;
    //PID计算
    Error = 3000 - Actual;//获取误差
    KpOut = Kp * (double)Error;//获取Kp比例修正
    if(i == T)//防止历史误差越界
    {i = 0;}
    ErrorLiShi[i] = Error;//错误累加到历史错误里
    for(uint32_t j = 0; j < T; j++)
    {
        ErrorLeiJia = ErrorLiShi[i] + ErrorLeiJia;
    }
    KiOut = Ki * (double)Error;//获取Ki积分修正
    if(i == 0)
    {
        KdOut = Kd * (ErrorLiShi[0] - ErrorLiShi[T - 1]);//获取Kd微分修正
    }
    else
    {
        KdOut = Kd * (ErrorLiShi[i] - ErrorLiShi[i - 1]);//获取Kd微分修正
    }
    i++;
    PIDout = Actual + KpOut + KiOut + KdOut;//获得PID修正

    //输出数值
    OLED_ShowSignedNum(1, 1, KpOut, 4);//
    OLED_ShowSignedNum(1, 6, KiOut, 4);//
    OLED_ShowSignedNum(1, 11, KdOut, 4);//
    OLED_ShowNum(2, 8, Actual, 4);//实际值
    OLED_ShowNum(3, 8, PIDout, 4);//修正过的值
    OLED_ShowSignedNum(4, 8, Error, 4);//误差
}

int main(void)
{
	/*模块初始化*/
	OLED_Init();			//OLED初始化
	AD_Init();				//AD初始化
    CS_Init();

	/*显示静态字符串*/
	OLED_ShowString(2, 1, "Actual:");//实际值
	OLED_ShowString(3, 1, "PIDout:");//修正过的值
	OLED_ShowString(4, 1, " Error:");//误差
	while (1)
	{
        PID();
        Delay_ms(500);
	}
}
