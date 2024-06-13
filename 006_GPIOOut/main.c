#include "stm32f4xx.h"

int main(void)
{
  /* Initiate clock for GPIOB*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  /* Initiate GPIOB*/
  GPIO_InitTypeDef GPIO_InitStruct;
    
  GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  while(1)
  {
    GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14);
    for(int i = 0; i < (SystemCoreClock >> 8); i++)
    {
      __NOP();
    }
    GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_7 | GPIO_Pin_14);
    for(int i = 0; i < (SystemCoreClock >> 8); i++)
    {
      __NOP();
    }
  }
}
