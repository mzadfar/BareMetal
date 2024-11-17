#include "stm32f4xx.h"

int main(void)
{
  /* Initiate clock for GPIOB and GPIOC */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
  
  /* Initiate GPIOB */
  GPIO_InitTypeDef GPIO_InitStruct;
    
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* Initiate GPIOC */
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_13;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
  
  GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  while(1)
  {
    if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
    {
      GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14);
    }
    else
    {
      GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14);
    }
    /*
    GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14);
    for(int i = 0; i < (SystemCoreClock >> 5); i++)
    {
      __NOP();
    }
    GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14);
    for(int i = 0; i < (SystemCoreClock >> 5); i++)
    {
      __NOP();
    }
    */
  }
}
