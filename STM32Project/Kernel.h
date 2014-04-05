#ifndef __KERNEL_H
#define __KERNEL_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "common.h"   
#include "stm32f0xx_gpio.h"   
   
//extern __IO u32 ADC_Done;
void ConfigKernel();
void ExecKernel(u8 task_number);

#ifdef __cplusplus
}
#endif



#endif /* __KERNEL_H*/   