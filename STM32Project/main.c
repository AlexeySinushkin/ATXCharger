#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"

#include "common.h"
#include "task_manager.h"
#include "Kernel.h"
#include "LedManager.h"

void RCC_Config()
{
    RCC_ClocksTypeDef RCC_Clocks;
    /* SysTick end of count event each 1ms */
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
  
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB |
                          RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
}


/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
	/* 1 bit for pre-emption priority, 3 bits for subpriority */
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
}


int main()
{
  RCC_Config();
  NVIC_Configuration();

  ConfigLedManager();
  ConfigKernel();
  
  Add_Task(200, TRUE, ExecKernel);
  Add_Task(7, TRUE, ExecLedMgr);
  for (;;){
    Check_Tasks();
  }
  
  //return 0;
}



void assert_failed(uint8_t* file, uint32_t line){
  while(1){
    
  }  
}