
#include "common.h"
#include "LedManager.h"
#include "stm32f0xx_adc.h"  
#include "stm32f0xx_dma.h" 
#include "stm32f0xx_misc.h"
#include "Kernel.h"

__IO u16 ADCValue;

void UpCurrent();
void DownCurrent();
void DownZeroCurrent();


typedef enum {
  StateStart=0,
  StateWaitADC=1
}KernelStates;
KernelStates State;

#define Step 1
#define ADC1_DR_Address                0x40012440
//При нормально режиме (НЕинверсии) 
//чем выше значение Channel2CCR
//тем выше напряжение на выходе DAC
#define INVERSEN

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
#define TimerStart  2
uint16_t TimerPeriod = 255-Step;
uint16_t Channel2CCR = TimerStart+Step+1;
float Current;
float Voltage;
__IO uint16_t RegularConvData_Tab[2];

void ConfigKernel()
{

  
    //настраиваем ADC порт Current
    GPIO_InitTypeDef  GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
    //Voltage
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    DMA_InitTypeDef     DMA_InitStructure;
    /* DMA1 Channel1 Config */
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)RegularConvData_Tab;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 2;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    /* DMA1 Channel1 enable */
    DMA_Cmd(DMA1_Channel1, ENABLE);
    
    /* ADC DMA request in circular mode */
    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
    
    /* Enable ADC_DMA */
    ADC_DMACmd(ADC1, ENABLE);  
  
    
    
    
    /* ADC1 configuration ---------------*/    
    ADC_InitTypeDef ADC_InitStructure;
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;// DISABLE; // вкл/выкл непрерывное преобразование 
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;// выравнивание
    ADC_InitStructure.ADC_Resolution=ADC_Resolution_12b;
    ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_T15_TRGO;//чтобы ошибка не срабатывала
    ADC_InitStructure.ADC_ExternalTrigConvEdge=ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_ScanDirection=ADC_ScanDirection_Upward;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    /* ADC1 regular channel configuration */ 
    ADC_ChannelConfig(ADC1, ADC_Channel_6, ADC_SampleTime_55_5Cycles);//current
    ADC_ChannelConfig(ADC1, ADC_Channel_9, ADC_SampleTime_55_5Cycles);//voltage
    
    /* Check the end of ADC1 reset calibration register */
    ADC_GetCalibrationFactor(ADC1);
 
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
      
        /* DMA1 Channel1 enable */
    DMA_Cmd(DMA1_Channel1, ENABLE);
    
    /* ADC DMA request in circular mode */
    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
    
    /* Enable ADC_DMA */
    ADC_DMACmd(ADC1, ENABLE);  
  
    
    /* Wait the ADCEN falg */
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));     
    
    
    //настраиваем TIM2_CH2
  
  /* GPIOA Configuration: Channel 2 PA1*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //прикрепляем АФ к таймеру 2
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_2);


  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  //TIM_TimeBaseStructure.TIM_Prescaler =3;
  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);


  TIM_OCStructInit(&TIM_OCInitStructure);
  /* Channel 2 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_Pulse = Channel2CCR;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  //TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //Not use N port
  
#ifdef INVERSEN  
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  //TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; //Not use N port
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  //TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset; //Not use N port
#else
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  //TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //Not use N port
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  //TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set; //Not use N port 
#endif
  
  
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);

    // Как я понял - автоматическая перезарядка таймера.
  TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
  TIM_ARRPreloadConfig(TIM2, ENABLE);

  TIM_CCxCmd(TIM2, TIM_Channel_2, TIM_CCx_Enable);
    
  /* TIM2 counter enable */
  TIM_Cmd(TIM2, ENABLE);

  

    
    ADC_StartOfConversion(ADC1);
    State=StateStart;    
}

u8 displayMode=0;
u8 displayCount=0;
u8 startWait=0;
void ExecKernel(u8 task_number)
{
  if (startWait<100){
    DownZeroCurrent();
    startWait+=5;
    DisplayLedDigits(startWait);
    return;
  }
  
    FlagStatus adcDone=DMA_GetFlagStatus(DMA1_FLAG_TC1);//transfer complete
    if (adcDone == SET){
      u16 adcCurrent=RegularConvData_Tab[0];
      u16 adcVoltage=RegularConvData_Tab[1];

      //пропорция
      //4096 - 3.3V
      //adc - ?V
      adcCurrent =  adcCurrent*3300/4096;
      adcVoltage =  adcVoltage*3300/4096;

      //I = U/R
      //сопротивление шунта 2.5Е-03
      Current = (float)adcCurrent/2.5F;
      //делим на коэффициент усиления ОУ
      Current/=100;

      //Делитель 10кОм/1кОм
      //ток на одном килооме
      //float cur=volts/1000;
      //Voltage=(volts/1000)*11000;        
      Voltage=adcVoltage*11;
      //из милливольт получаем вольты
      Voltage/=1000;
      if (Current>9.3F ||
          Voltage>15.0F)
      {
          DownZeroCurrent();
          /* Clear DMA TC flag */
      }else{
        if (Voltage<14.2F){
            if (Current<5.0F){
              UpCurrent();
            }else if (Current>6.5F){
              DownCurrent();
            }
        }else if (Voltage>14.5F){
          DownCurrent();        
        }
      }
      /* Clear DMA TC flag */
      DMA_ClearFlag(DMA1_FLAG_TC1);
    }//adcDone == SET 


  
  displayCount++;
  if (displayCount>4)
  {
    displayCount=0;
    if (displayMode==0)
    {
      displayMode=1;
    }else if (displayMode==1)
    {
      displayMode=2;  
    }else if (displayMode==2)
    {
      displayMode=0;  
    }
    
  }
  
  displayMode=0;
  
  if (displayMode==0)
  {
    DisplayLedDigitsFloat(Current);
  }else if (displayMode==1){
    DisplayLedDigits((u8)Voltage);
  }else if (displayMode==2){
    //float decVolt=Voltage-(u8)Voltage;
    //DisplayLedDigits((u8)(decVolt*10));
    DisplayLedDigits((u8)Channel2CCR);
  }

  
}



void doUpdate(){
  TIM_OCInitStructure.TIM_Pulse = Channel2CCR; 
  TIM_OC2Init(TIM2, &TIM_OCInitStructure);
}

void UpCurrent(){
  if (Channel2CCR<TimerPeriod-(Step+1)){
    Channel2CCR+=Step;
  }
  doUpdate();
}

void DownCurrent(){
  if (Channel2CCR>TimerStart+Step+1){
    Channel2CCR-=Step;
  }
  doUpdate();  
}

void DownZeroCurrent(){
    Channel2CCR=TimerStart+Step+1;
    doUpdate();
}


