#include "MyPID.h"

extern PID_NUM MyPID;

void Kp_Change(PID_NUM* PID, double Kp)
{
    PID->Kp = Kp;
}
void Ki_Change(PID_NUM* PID, double Ki)
{
    PID->Ki = Ki;
}
void Kd_Change(PID_NUM* PID, double Kd)
{
    PID->Kd = Kd;
}
void T_Change(PID_NUM* PID, uint32_t T)
{
    PID->T = T;
}
void Target_Change(PID_NUM* PID, double Target)
{
    PID->Target = Target;
}
void add_MAX_Change(PID_NUM* PID, double add_MAX)
{
    PID->add_MAX = add_MAX;
}
void add_MIN_Change(PID_NUM* PID, double add_MIN)
{
    PID->add_MIN = add_MIN;
}
void integral_Change(PID_NUM* PID, double integral)
{
    PID->integral = integral;
}
void prev_error_Change(PID_NUM* PID, double prev_error)
{
    PID->prev_error = prev_error;
}

void PID_Init(PID_NUM* PID)
{
    
}

uint32_t CountSensorCountT = 0;
extern volatile uint32_t CountSensorCount;//ȫ�ֱ��������ڼ���
double Speed(PID_NUM* PID)
{
    double num = (CountSensorCount-CountSensorCountT)/(MyPID.T/1000);
    CountSensorCountT = CountSensorCount;
    return num;
}

double PID_OUT(PID_NUM* PID)
{
    double error = Speed(PID);       //�������
    PID->prev_error = error; //�������
    PID->integral += error;  //�����ۼ����

    double OUT = (PID->Kp * error) + (PID->Ki * PID->integral) + (PID->Kd * (error - PID->prev_error));
    if(OUT > PID->add_MAX)
    {
        return PID->add_MAX;
    } else if(OUT < PID->add_MIN)
    {
        return PID->add_MIN;
    }
    return OUT;
}
