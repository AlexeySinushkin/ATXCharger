
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


//����������� ������ 10��
void ExecLedMgr(u8 task_number)
{
  if (step==0){
     //���������� ������ ����� (0 - ������� ������)
   GPIOLed->BSRR=Symb_ALL;//�������
   GPIOLed->BRR=Symb0;
   GPIOLedAnode->BSRR=LedAnode_0; //��������
   //����� ����� ��������� �����    
   //vTaskDelay(10);
   step=1;
  }else if (step==1){
   GPIOLedAnode->BRR=LedAnode_0; //�����    
    //���������� ������
   GPIOLed->BSRR=Symb_ALL;//�������
   GPIOLed->BRR=Symb1;
   GPIOLedAnode->BSRR=LedAnode_1; //��������   
   //���� ����� �������� �����
   GPIOLed->BSRR=Led_dp;//�������
   if (UseDot){
     GPIOLed->BRR=Led_dp;
   }
   //����� ����� ��������� �����
   //vTaskDelay(10);
   step=2;
  }
  else if (step==2){   
   GPIOLedAnode->BRR=LedAnode_1; //�����   
    GPIOLed->BSRR=Led_dp;//�������  
   step=0;
  }
  
}

//���������� ����� � ������� 0.0
void DisplayLedDigitsFloat(float value)
{
  UseDot=1;
  if (value>10.0F || value<0.0F)
  {
   Symb1=Symb_UNDERLINE;
   Symb0=Symb_UNDERLINE;
   return;
  }    
  //����� �����
  Symb1=(u8)value;
  //������� �����
  if (Symb1==0)
  {
    Symb0=(u8)(value*10); 
  }else{
    Symb0=(u8)(value*10)-(u8)(Symb1*10);     
  }
  Symb1=Digits[Symb1]; //��������
  Symb0=Digits[Symb0]; //��������
}
//���������� ��� ����� � ������� 07
void DisplayLedDigits(u8 value)
{
  UseDot=0;
  if (value>100)
  {
   Symb1=Symb_UNDERLINE;
   Symb0=Symb_UNDERLINE;
   return;
  }
  //���������� ������� �����
  if (value<10)
  {
   Symb1=0; //������
   Symb1=Symb_EMPTY;
  }else{
   Symb1=(u8)(value/10); //������
   Symb1=Digits[Symb1]; //��������
  }
  
  Symb0=(u8)(value%10);  //������
  Symb0=Digits[Symb0]; //��������
}