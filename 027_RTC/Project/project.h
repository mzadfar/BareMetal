#pragma once
#include "main.h"
#include <stddef.h>

/*USART prototypes*/
void USART_SendText(USART_TypeDef* USARTx, volatile char* sendText);
void USART_SendNumber(USART_TypeDef* USARTx, uint32_t sendNumber);
#ifdef DAC_PA4_PA5
/*SPI prototypes*/
void SPI_Tx(uint8_t address, uint8_t data);
uint8_t SPI_Rx(uint8_t address);
#endif