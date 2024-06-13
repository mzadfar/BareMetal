#include "stm32f4xx.h"
#include <stdio.h>

char buffer[10];

void USART_SendText(USART_TypeDef* USARTx, volatile char *s);
void USART_SendNumber(USART_TypeDef* USARTx, uint32_t x);
void RCC_Setup();
void GPIO_Setup();
void USART_Setup();
void ADC_Setup();

float adcData;

int main(void)
{
  RCC_Setup();
  GPIO_Setup();
  USART_Setup();
  ADC_Setup();

  while(1)
  { 
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    adcData = (float)ADC_GetConversionValue(ADC1);
    adcData = adcData * 3.3 / (1 << 12);
    sprintf(buffer, "%f", adcData);
    USART_SendText(USART2, buffer);
    USART_SendText(USART2, "\n");
  }
}

void RCC_Setup()
{
  /*Initialize GPIOB for toggling LEDs*/
  /*USART_2 (USART_B_RX: PD6 D52 on CN9, USART_B_TX: PD5 D53 on CN9) & USART_6 (USART_A_TX: PG14 D1 on CN10, USART_A_RX: PG9 D0 on CN10)*/
  /* Initiate clock for GPIOB, GPIOD, and GPIOG */
  /*Initialize GPIOA for PA3/ADC123_IN3 clock*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG , ENABLE);

  /*Initialize USART2 clock*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  /*Initialize USART6 clock*/
  /*Initialize ADC1 clock*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6 | RCC_APB2Periph_ADC1, ENABLE);
}

void GPIO_Setup()
{
  /* Initialize GPIOB*/
  GPIO_InitTypeDef GPIO_InitStruct;
    
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* Initialize GPIOD*/
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Connect GPIOD pins to AF*/
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

  /* Initialize GPIOG */
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_9 | GPIO_Pin_14;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  
  GPIO_Init(GPIOG, &GPIO_InitStruct);
  
  /*Connect GPIOG pins to AF*/
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource9, GPIO_AF_USART6);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_USART6);

  /* Initialize GPIOA */
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_3;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void USART_Setup()
{
  /*Configure USART2*/
  USART_InitTypeDef USART_InitStruct;

  USART_InitStruct.USART_BaudRate = 9600;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_Parity = USART_Parity_No ;
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  

  /*Initialize USART2*/
  USART_Init(USART2, &USART_InitStruct);

  /*Enable USART2*/
  USART_Cmd(USART2, ENABLE);

  /*Enable interrupt for UART2*/
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  /*Enable interrupt to UART2*/
  NVIC_EnableIRQ(USART2_IRQn);


  /*Configure USART6*/
  USART_InitStruct.USART_BaudRate = 9600;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_Parity = USART_Parity_No ;
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  

  /*Initialize USART6*/
  USART_Init(USART6, &USART_InitStruct);

  /*Enable USART6*/
  USART_Cmd(USART6, ENABLE);
}

void ADC_Setup()
{
  /*ADC Common Init structure definition*/
  ADC_CommonInitTypeDef ADC_CommonInitStruct;

  /*ADC Init structure definition*/
  ADC_InitTypeDef ADC_InitStruct;

  /* Initialize the ADC_Mode, ADC_Prescaler, ADC_DMAAccessMode, ADC_TwoSamplingDelay member */
  ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;

  ADC_CommonInit(&ADC_CommonInitStruct);

  /* Initialize the ADC_Mode, ADC_ScanConvMode, ADC_ContinuousConvMode,
  ADC_ExternalTrigConvEdge, ADC_ExternalTrigConv, ADC_DataAlign,
  ADC_NbrOfConversion member */
  ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;
  ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStruct.ADC_NbrOfConversion = 1;

  ADC_Init(ADC1, &ADC_InitStruct);

  /*Configuring ADC1*/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_144Cycles);
  
  /*Enable ADC1*/
  ADC_Cmd(ADC1, ENABLE);

  /*Start Conversion*/
  ADC_SoftwareStartConv(ADC1);
}

void USART2_IRQHandler(void)
{
  if (USART_GetITStatus(USART2, USART_IT_RXNE))
  {
    if (USART_ReceiveData(USART2) == 'K')
    {
      GPIO_ToggleBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14);
      
      USART_SendText(USART2, "LED Toggled\n");
    }
  }
}

void USART_SendText(USART_TypeDef* USARTx, volatile char *s)
{
  while(*s)
  {
    /*Wait until data register is empty*/
    while (!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));
    USART_SendData(USARTx, *s); 
    *s++;
  }
}

void USART_SendNumber(USART_TypeDef* USARTx, uint32_t x)
{
  /*A temp array to build results of conversion*/
  char value[10];
  /*Loop index*/
  int i = 0;

  do
  {
    /*Convert integer to character*/
    value[i++] = (char)(x % 10) + '0';
    x /= 10;
  } while (x);
  
  /*Send data*/
  while(i)
  {
    //USART_SendNumber8b(USARTx, value[--i]);
    /*Wait until data register is empty*/
    while (!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));
    USART_SendData(USARTx, value[--i]); 
  }
}