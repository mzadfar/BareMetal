#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"

volatile int32_t timeDelay;

/*Function prototypes*/
void GPIO_Setup(void);
void VirtualCOMPort_Setup(void);

void USART_SendText(USART_TypeDef* USARTx, const char *sendText);
void USART_SendNumber(USART_TypeDef* USARTx, uint32_t sendNumber);

uint32_t n = 0;

int main(void)
{
  /*Reset the RCC clock configuration to the default reset state.
  Only HSI  is turned on, and uses as system clock source as HSI= 16MHz*/
  RCC_DeInit();

  /*Update SytemCoreClock variable to 16MHz*/
  SystemCoreClockUpdate();
  
  /*Initialization of peripherals*/
  GPIO_Setup();
  VirtualCOMPort_Setup();

  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
 
  while(1)
  { 
    GPIO_ToggleBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14);
   
    USART_SendNumber(USART3, n++);       
    USART_SendText(USART3, "\tHI\n");    
  
    USART_SendNumber(USART3, RCC_Clocks.SYSCLK_Frequency);       
    USART_SendText(USART3, "\t"); 
    USART_SendNumber(USART3, RCC_Clocks.HCLK_Frequency);       
    USART_SendText(USART3, "\t"); 
    USART_SendNumber(USART3, RCC_Clocks.PCLK1_Frequency);       
    USART_SendText(USART3, "\t"); 
    USART_SendNumber(USART3, RCC_Clocks.PCLK2_Frequency);       
    USART_SendText(USART3, "\n"); 
    
    for(int i = 0; i < (SystemCoreClock >> 5); i++)
    {
      __NOP();
    }
  }
}

void GPIO_Setup(void)
{
  /* Initiate GPIOB */
  /* Initiate clock for GPIOB as LEDs and GPIOD as virtual COM port*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOD, ENABLE);
  
  /* Initiate GPIOB */
  GPIO_InitTypeDef GPIO_InitStruct;
     
  /*Reset every member element of the structure*/
  memset(&GPIO_InitStruct, 0, sizeof(GPIO_InitStruct));
    
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Initiate GPIOD */
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
  
  GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void VirtualCOMPort_Setup(void)
{
  /*USART_3 (USART_C_TX: PD8, USART_C_RX: PD9) has virtual COM port capability*/

  /*Configure USART2*/
  USART_InitTypeDef USART_InitStruct;

  /*Reset every member element of the structure*/
  memset(&USART_InitStruct, 0, sizeof(USART_InitStruct));

  /*Connect GPIOD pins to AF for USART3*/
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
  
  /*Initialize USART3 clock*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

  /*Configure USART3*/
  USART_InitStruct.USART_BaudRate = 9600;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_Parity = USART_Parity_No ;
  USART_InitStruct.USART_Mode = USART_Mode_Tx;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  

  /*Initialize USART3*/
  USART_Init(USART3, &USART_InitStruct);

  /*Enable USART3*/
  USART_Cmd(USART3, ENABLE);
}

void USART_SendText(USART_TypeDef* USARTx, const char *sendText)
{
  for (uint32_t i = 0; i < strlen(sendText); i++)
  {
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) != SET);
    USART_SendData(USARTx, sendText[i]);
  }
}

void USART_SendNumber(USART_TypeDef* USARTx, uint32_t sendNumber)
{
  /*A temp array to build results of conversion*/
  char value[10];
  /*Loop index*/
  int i = 0;

  do
  {
    /*Convert integer to character*/
    value[i++] = (char)(sendNumber % 10) + '0';
    sendNumber /= 10;
  } while (sendNumber);
  
  /*Send data*/
  while(i)
  {
    //USART_SendNumber8b(USARTx, value[--i]);
    /*Wait until data register is empty*/
    while (!USART_GetFlagStatus(USARTx, USART_FLAG_TXE));
    USART_SendData(USARTx, value[--i]); 
  }
}