#include "stm32f4xx.h"
#include <stdio.h>

char buffer[10];

void USART_SendText(USART_TypeDef* USARTx, volatile char *s);
void USART_SendNumber(USART_TypeDef* USARTx, uint32_t x);
void RCC_Setup();
void GPIO_Setup();
void USART_Setup();
void ADC_Setup();
void DMA_Setup();
void SPI_Setup();
void SPI_Tx(uint8_t address, uint8_t data);
uint8_t SPI_Rx(uint8_t address);

uint16_t adcData[2];
float temp;
uint16_t g_nZAx;

int main(void)
{
  RCC_Setup();
  GPIO_Setup();
  USART_Setup();
  ADC_Setup();
  DMA_Setup();
  SPI_Setup();

  SPI_Tx(0x20, 0x47);

  while(1)
  { 
    g_nZAx = SPI_Rx(0x20);

    /*Data comes as 2's complement value*/
    if ((g_nZAx & 0x80) == 0x80)
    {
      g_nZAx = ~g_nZAx;
      g_nZAx += 1;
      g_nZAx &= 0x00FF;
      g_nZAx = g_nZAx * 2300 / (1 << 7);

      USART_SendText(USART2, "-");    
      USART_SendNumber(USART2, g_nZAx); 
    }
    else
    {
      g_nZAx = g_nZAx * 2300 / (1 << 7);
   
      USART_SendNumber(USART2, g_nZAx);       
    }
    USART_SendText(USART2, "\n");    

    for(int i = 0; i < (SystemCoreClock >> 9); i++)
    {
      __NOP();
    }
  }
  return 0;
}

void RCC_Setup()
{
  /*Initialize GPIOB for toggling LEDs*/
  /*USART_2 (USART_B_RX: PD6 D52 on CN9, USART_B_TX: PD5 D53 on CN9) & USART_6 (USART_A_TX: PG14 D1 on CN10, USART_A_RX: PG9 D0 on CN10)*/
  /* Initiate clock for GPIOB, GPIOD, and GPIOG */
  /*Initialize GPIOA for PA3/ADC123_IN3 clock*/
  /*Initialize DMA2 clock*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_DMA2, ENABLE);

  /*Initialize USART2 clock*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  /*Initialize USART6 clock*/
  /*Initialize ADC1 clock*/
  /*Initialize SPI1 clock*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6 | RCC_APB2Periph_ADC1 | RCC_APB2Periph_SPI1, ENABLE);
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
  
  /* Initialize SPI1_CS GPIOD*/
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_14;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Set SPI1_CS*/
  GPIO_SetBits(GPIOD, GPIO_Pin_14);

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
  
  /* Initialize SPI1 on GPIOA */
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;

  GPIO_Init(GPIOA, &GPIO_InitStruct);
    
  /*Connect GPIOA pins to AF*/
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
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
  ADC_InitStruct.ADC_ScanConvMode = ENABLE;
  ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStruct.ADC_NbrOfConversion = 2;

  ADC_Init(ADC1, &ADC_InitStruct);

  /*Configuring ADC1*/
  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_144Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 2, ADC_SampleTime_144Cycles);
  
  /*Enable temperature sensor channel*/
  ADC_TempSensorVrefintCmd(ENABLE);

  /*Enable ADC DMA request*/
  ADC_DMACmd(ADC1, ENABLE);

  /*Enable ADC1*/
  ADC_Cmd(ADC1, ENABLE);

  /*Enable ADC DMA request after last transfer (Single-ADC mode)*/
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

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

void DMA_Setup()
{
  DMA_InitTypeDef DMA_InitStruct;

  /* Initialize the DMA Struct members */
  DMA_InitStruct.DMA_Channel = 0;
  DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; /*Address needed; not the value*/
  DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)adcData; /*Address needed; not the value*/
  DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStruct.DMA_BufferSize = 2;
  DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

  /*Initialize DMA2*/
  DMA_Init(DMA2_Stream0, &DMA_InitStruct);

  /*Enable DMA2*/
  DMA_Cmd(DMA2_Stream0, ENABLE);
}

void SPI_Setup()
{
  SPI_InitTypeDef SPI_InitStruct;

  /* Initialize the SPI struct members */
  SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
  SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; /*Check the slave Datasheet*/
  SPI_InitStruct.SPI_CPOL = SPI_CPOL_High; /*Check the slave Datasheet*/
  SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge; /*Check the slave Datasheet*/
  SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
  SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB; /*Check the slave Datasheet*/
  SPI_InitStruct.SPI_CRCPolynomial = 7;

  /*Initialize SPI1*/
  SPI_Init(SPI1, &SPI_InitStruct);

  /*Enable SPI1*/
  SPI_Cmd(SPI1, ENABLE);
}

void SPI_Tx(uint8_t address, uint8_t data)
{
  /*Reset SPI1_CS*/
  GPIO_ResetBits(GPIOD, GPIO_Pin_14);

  while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
  SPI_I2S_SendData(SPI1, address);
  while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
  SPI_I2S_ReceiveData(SPI1);
  while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
  SPI_I2S_SendData(SPI1, data);
  while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
  SPI_I2S_ReceiveData(SPI1);
  
  /*Set SPI1_CS and wait*/
  GPIO_SetBits(GPIOD, GPIO_Pin_14);
}

uint8_t SPI_Rx(uint8_t address)
{
  /*Reset SPI1_CS*/
  GPIO_ResetBits(GPIOD, GPIO_Pin_14);

  address = 0x00 | address;
  while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
  SPI_I2S_SendData(SPI1, address);
  while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
  SPI_I2S_ReceiveData(SPI1);
  while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE));
  SPI_I2S_SendData(SPI1, 0x00); /*Check the slave Datasheet*/
  while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE));
    
  /*Set SPI1_CS and wait*/
  GPIO_SetBits(GPIOD, GPIO_Pin_14);

  return SPI_I2S_ReceiveData(SPI1);
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