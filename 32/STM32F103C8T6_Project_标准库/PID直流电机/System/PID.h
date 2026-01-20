#ifndef __PID_H
#define __PID_H

void modify_Kp(double KpInput);
void modify_Ki(double KiInput);
void modify_Kd(double KdInput);
void modify_Target(double TargetInput);
void PID_Init(void);

#endif
