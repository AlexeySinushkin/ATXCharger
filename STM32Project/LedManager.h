#ifndef __LEDMGR_H
#define __LEDMGR_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "common.h"
#include "stm32f0xx_gpio.h"
   
#define Led_a GPIO_Pin_9
#define Led_b GPIO_Pin_4
#define Led_c GPIO_Pin_3
#define Led_d GPIO_Pin_0
#define Led_e GPIO_Pin_2
#define Led_f GPIO_Pin_10  
#define Led_g GPIO_Pin_5
#define Led_dp GPIO_Pin_7
#define GPIOLed GPIOA  


#define Symb_0  (Led_a)|(Led_b)|(Led_c)|(Led_d)|(Led_e)|(Led_f)
#define Symb_1  (Led_b)|(Led_c)
#define Symb_2  (Led_a)|(Led_b)|(Led_g)|(Led_e)|(Led_d)
#define Symb_3  (Led_a)|(Led_b)|(Led_g)|(Led_c)|(Led_d)
#define Symb_4  (Led_b)|(Led_c)|(Led_f)|(Led_g)   
#define Symb_5  (Led_a)|(Led_c)|(Led_d)|(Led_f)|(Led_g)
#define Symb_6  (Led_a)|(Led_c)|(Led_d)|(Led_e)|(Led_f)|(Led_g)
#define Symb_7  (Led_a)|(Led_b)|(Led_c)|(Led_f)
#define Symb_8  (Led_a)|(Led_b)|(Led_c)|(Led_d)|(Led_e)|(Led_f)|(Led_g)
#define Symb_9  (Led_a)|(Led_b)|(Led_c)|(Led_d)|(Led_f)|(Led_g)   
#define Symb_ALL (Symb_8)|(Led_dp)
#define Symb_E (Led_a)|(Led_d)|(Led_e)|(Led_f)|(Led_g)
#define Symb_UNDERLINE (Led_d)
#define Symb_EMPTY 0
   
#define GPIOLedAnode GPIOF 
#define LedAnode_1 GPIO_Pin_1
#define LedAnode_0 GPIO_Pin_0

void ConfigLedManager();
void ExecLedMgr(u8 task_number);   
void DisplayLedDigits(u8 value);
void DisplayLedDigitsFloat(float value);   

#ifdef __cplusplus
}
#endif



#endif /* __LEDMGR_H*/   