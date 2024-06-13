#include <stdio.h>
#include <string.h>
#include "stm32f4xx.h"

/*Function prototypes*/
void GPIO_Setup(void);
void VirtualCOMPort_Setup(void);

void USART_SendText(USART_TypeDef* USARTx, const char *sendText);

char userText[99]; 

int main(void)
{
  GPIO_Setup();
  VirtualCOMPort_Setup();

  sprintf(userText, "This is the message to be sent via Virtual COM PORT\r\n");

  while(1)
  {
    GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14);
    for(int i = 0; i < (SystemCoreClock >> 5); i++)
    {
      __NOP();
    }
    GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14);

    USART_SendText(USART3, userText);
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