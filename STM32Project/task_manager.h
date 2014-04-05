#ifndef __TMANAGER_H
#define __TMANAGER_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "common.h"
   
#define TASKS_NUMBER 4
typedef struct 
{
	u32 Delay;//�������� � ��
	u32 Execute_Time;//����� ���������� ����������
	bool Unstop;
	bool Used;	//������ ������������
	void (*FuncPtr)(u8); //������
} Task_t;

//��������� ������ � ����� �� ��������� ���
//������������, �� �����, ��������� �� �������
//���������� ����� ������� ��� ����������
u8 Add_Task(u32 Delay, bool Unstop, void (*FuncPtr)(u8));
//������� ������.
void Remove_Task(u8 Task_Number);
void OnSysTick(void);
void Check_Tasks(void);
extern __IO u32 Tasks_Counter;
#ifdef __cplusplus
}
#endif



#endif /* __TMANAGER_H*/   