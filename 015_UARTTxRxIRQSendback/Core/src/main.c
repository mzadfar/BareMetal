#include <string.h>
#include "stm32f4xx.h"

volatile int32_t timeDelay;

/*Function prototypes*/
void SysTick_Setup(uint32_t ticksValue);
void delayUS(uint32_t usTime);

void GPIO_Setup(void);
void VirtualCOMPort_Setup(void);
void USART_Setup(void);

void USART_SendText(USART_TypeDef* USARTx, const char *sendText);
void USART_SendNumber(USART_TypeDef* USARTx, uint32_t sendNumber);

uint32_t n = 0;
char buffer[10];
int i = 0;

int main(void)
{
  /*Using virtual come port the clocks set as
  SYSCLK_Frequency  HCLK_Frequency  PCLK1_Frequency  PCLK2_Frequency
  53760000          53760000        13440000         26880000
  SysTick interrupt period is 13.44 cycles for 1us*/
  SysTick_Setup(14);
  GPIO_Setup();
  VirtualCOMPort_Setup();
  USART_Setup();

  while(1)
  {
    USART_SendNumber(USART3, n++);       
    USART_SendText(USART3, "\tHI\n");    
    delayUS(500000);
  }
}

void SysTick_Setup(uint32_t ticksValue)
{
  /*Disable SysTick*/
  SysTick->CTRL = 0;

  /*Set reload register*/
  SysTick->LOAD = ticksValue - 1;

  /*Set interrupt priority of SysTick to least urgency (i.e., largest priority value)*/
  NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) -1);

  /*Reset the SysTick counter value*/
  SysTick->VAL = 0;

  /*Select processor clock: 1 =processor clock; 0 = external clock*/
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Pos;

  /*Enable SysTick interrrupt, 1 = Enabl, 0 = Disable*/
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Pos;

  /*Enable SysTick*/
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Pos;
}

void SysTick_Handler(void)
{
  if(timeDelay > 0)
  {
    timeDelay--;
  }
}

void delayUS(uint32_t usTime)
{
  /*Time delay length*/
  timeDelay = usTime;
  
  /*Busy wait*/
  while(timeDelay != 0);
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
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9;
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
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  

  /*Initialize USART3*/
  USART_Init(USART3, &USART_InitStruct);

  /*Enable USART3*/
  USART_Cmd(USART3, ENABLE);

  /*Enable interrupt for UART3*/
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

  /*Enable interrupt to UART3*/
  NVIC_EnableIRQ(USART3_IRQn);
}

void USART3_IRQHandler(void)
{
  if (USART_GetITStatus(USART3, USART_IT_RXNE))
  {
    if (i == 9)
    {
      i = 0;
      USART_SendText(USART3, buffer);
      USART_SendText(USART3, "\n");
      GPIO_ToggleBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14);
    }
    buffer[i] = (char) USART_ReceiveData(USART3);
    i++;
  }
}

void USART_Setup(void)
{
  /*USART_2 (USART_B_RX: PD6 D52 on CN9, USART_B_TX: PD5 D53 on CN9)*/
  /*Configure USART2*/
  USART_InitTypeDef USART_InitStruct;

  /*Reset every member element of the structure*/
  memset(&USART_InitStruct, 0, sizeof(USART_InitStruct));

  /*Connect GPIOD pins to AF to USART2*/
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
  
  /*Initialize USART2 clock*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  /*Configure USART2*/
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
}

void USART2_IRQHandler(void)
{
  if (USART_GetITStatus(USART2, USART_IT_RXNE))
  {
    if (i == 9)
    {
      i = 0;
      USART_SendText(USART2, buffer);
      USART_SendText(USART2, "\n");
      GPIO_ToggleBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14);
    }
    buffer[i] = (char) USART_ReceiveData(USART2);
    i++;
  }
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