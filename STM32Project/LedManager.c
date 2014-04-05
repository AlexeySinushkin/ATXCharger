
#include "common.h"
#include "LedManager.h"

const u16 Digits[]={Symb_0,Symb_1,Symb_2,Symb_3,Symb_4,
                      Symb_5,Symb_6,Symb_7,Symb_8,Symb_9};

u16 Symb0;
u16 Symb1;
u8 UseDot=0;
u8 step;

void ConfigLedManager()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Pin = Symb_ALL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_1;
  GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
  GPIO_Init(GPIOLed, &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = (LedAnode_0)|(LedAnode_1);
  GPIO_Init(GPIOLedAnode, &GPIO_InitStructure);   
  
  Symb0=Symb_UNDERLINE;
  Symb1=Symb_UNDERLINE;
  
  step=0;
}


//выполняется каждые 10мс
void ExecLedMgr(u8 task_number)
{
  if (step==0){
     //показываем первую цифру (0 - крайняя справа)
   GPIOLed->BSRR=Symb_ALL;//очищаем
   GPIOLed->BRR=Symb0;
   GPIOLedAnode->BSRR=LedAnode_0; //зажигаем
   //пусть горит некоторое время    
   //vTaskDelay(10);
   step=1;
  }else if (step==1){
   GPIOLedAnode->BRR=LedAnode_0; //гасим    
    //показываем вторую
   GPIOLed->BSRR=Symb_ALL;//очищаем
   GPIOLed->BRR=Symb1;
   GPIOLedAnode->BSRR=LedAnode_1; //зажигаем   
   //если нужно показать точку
   GPIOLed->BSRR=Led_dp;//очищаем
   if (UseDot){
     GPIOLed->BRR=Led_dp;
   }
   //пусть горит некоторое время
   //vTaskDelay(10);
   step=2;
  }
  else if (step==2){   
   GPIOLedAnode->BRR=LedAnode_1; //гасим   
    GPIOLed->BSRR=Led_dp;//очищаем  
   step=0;
  }
  
}

//отображаем число в формате 0.0
void DisplayLedDigitsFloat(float value)
{
  UseDot=1;
  if (value>10.0F || value<0.0F)
  {
   Symb1=Symb_UNDERLINE;
   Symb0=Symb_UNDERLINE;
   return;
  }    
  //целая часть
  Symb1=(u8)value;
  //дробная часть
  if (Symb1==0)
  {
    Symb0=(u8)(value*10); 
  }else{
    Symb0=(u8)(value*10)-(u8)(Symb1*10);     
  }
  Symb1=Digits[Symb1]; //значения
  Symb0=Digits[Symb0]; //значения
}
//отображаем две цифры к примеру 07
void DisplayLedDigits(u8 value)
{
  UseDot=0;
  if (value>100)
  {
   Symb1=Symb_UNDERLINE;
   Symb0=Symb_UNDERLINE;
   return;
  }
  //определяем старшую часть
  if (value<10)
  {
   Symb1=0; //индекс
   Symb1=Symb_EMPTY;
  }else{
   Symb1=(u8)(value/10); //индекс
   Symb1=Digits[Symb1]; //значения
  }
  
  Symb0=(u8)(value%10);  //индекс
  Symb0=Digits[Symb0]; //значения
}