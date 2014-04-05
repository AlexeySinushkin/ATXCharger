#include "task_manager.h"
#include "common.h"




__IO Task_t Tasks[TASKS_NUMBER];
__IO u32 Tasks_Counter=0;
__IO bool clocked=FALSE;

/**
  * @brief  Add 1msec
  * @param  None
  * @retval None
  */
void OnSysTick(void)
{
	Tasks_Counter++;
	clocked=TRUE;
	if (Tasks_Counter>0xA4CB8000)//32 дня
	{
	  //log(0xEF);
	  //понижаем счетчик
		u32 min_time=0xA4CB8000;
		for (u8 i=0;i<TASKS_NUMBER;i++)
		{
			__IO Task_t* t = &Tasks[i];
			if (t->Used==TRUE)
			{
				if (t->Execute_Time<min_time)
				  min_time=t->Execute_Time;
			}
		}
		for (u8 i=0;i<TASKS_NUMBER;i++)
		{
			__IO Task_t* t = &Tasks[i];
			if (t->Used==TRUE)
			{
				t->Execute_Time-=min_time;
			}
		}		
		Tasks_Counter-=min_time;
	}
}

void Init_Task(void)
{
  
}

u8 Add_Task(u32 Delay, bool Unstop, void (*FuncPtr)(u8))
{
	for (u8 i=0;i<TASKS_NUMBER;i++)
	{
		__IO Task_t* t = &Tasks[i];
		if (t->Used==FALSE)
		{
			t->Delay=Delay;
			t->Execute_Time=Tasks_Counter+Delay;
			t->Unstop=Unstop;
			t->FuncPtr=FuncPtr;
			t->Used=TRUE;
			return i;
		}		
	}
	
	//log(0xEE);
	return 100;
}
void Remove_Task(u8 Task_Number)
{
  	__IO Task_t* t =  &Tasks[Task_Number];
	t->Used=FALSE;
}
void Check_Tasks(void)
{
  if (clocked==TRUE){
  	clocked=FALSE;
	for (u8 i=0;i<TASKS_NUMBER;i++)
	{
		__IO Task_t* t = &Tasks[i];
		if (t->Used==TRUE)
		{
		  u32 et=t->Execute_Time;
			if (Tasks_Counter>=et)
			{
				if (t->Unstop==TRUE)	//если бесконечный таймер
				{	//меняем следующее время срабатывания
				  	u32 d=t->Delay;
					t->Execute_Time=Tasks_Counter+d;							
				}
				else
				{
					t->Used=FALSE;
				}
				t->FuncPtr(i);
			}
		}			
	}
  }
}