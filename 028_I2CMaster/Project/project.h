#pragma once
#include "main.h"
#include <stddef.h>

/*Prototypes*/
void USART_SendText(USART_TypeDef* USARTx, volatile char* sendText);
void USART_SendNumber(USART_TypeDef* USARTx, uint32_t sendNumber);

#ifdef DAC_PA4_PA5
/*SPI prototypes*/
void SPI_Tx(uint8_t address, uint8_t data);
uint8_t SPI_Rx(uint8_t address);
#endif

void I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void I2C_Write(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_Read_Ack(I2C_TypeDef* I2Cx);
uint8_t I2C_Read_Nack(I2C_TypeDef* I2Cx);
void I2C_Stop(I2C_TypeDef* I2Cx);
