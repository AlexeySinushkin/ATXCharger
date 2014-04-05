#ifndef __TMANAGER_H
#define __TMANAGER_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "common.h"
   
#define TASKS_NUMBER 4
typedef struct 
{
	u32 Delay;//задержка в мс
	u32 Execute_Time;//время следующего выполнения
	bool Unstop;
	bool Used;	//таймер используется
	void (*FuncPtr)(u8); //индекс
} Task_t;

//добавляет таймер и сразу же запускает его
//миллисекунды, по кругу, указатель на функцию
//возвращает номер таймера для добавления
u8 Add_Task(u32 Delay, bool Unstop, void (*FuncPtr)(u8));
//удаляет таймер.
void Remove_Task(u8 Task_Number);
void OnSysTick(void);
void Check_Tasks(void);
extern __IO u32 Tasks_Counter;
#ifdef __cplusplus
}
#endif



#endif /* __TMANAGER_H*/   