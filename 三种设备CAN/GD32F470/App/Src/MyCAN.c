#include "MyCAN.h"

void MyCAN_Init(void)
{
    can_deinit(CAN1);
    can_parameter_struct can_parameter;
    can_parameter.working_mode = 0;//����ģʽ 
    can_parameter.resync_jump_width = 0;//��ͬ��������� 
    can_parameter.time_segment_1 = 0;//λ��1 
    can_parameter.time_segment_2 = 0;//λ��2 
    can_parameter.time_triggered = 0;//ʱ�䴥��ͨ��ģʽ 
    can_parameter.auto_bus_off_recovery = 0;//�Զ����߻ָ�
    can_parameter.auto_wake_up = 0;//�Զ�����
    can_parameter.auto_retrans = 0;//�Զ��ش� 
    can_parameter.rec_fifo_overwrite = 0;//����FIFO��ʱ���� 
    can_parameter.trans_fifo_order = 0;//����FIFO˳�� 
    can_parameter.prescaler = 0;//�����ʷ�Ƶϵ��
    
    can_init(CAN1, &can_parameter);
    
    //can_struct_para_init(CAN_INIT_STRUCT, );
    
}
