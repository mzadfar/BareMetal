#include "project.h"

void USART_SendText(USART_TypeDef* USARTx, volatile char* sendText)
{
    while (*sendText)
    {
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) != SET)
            ;
        USART_SendData(USARTx, *sendText);
        *sendText++;
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
    while (i)
    {
        // USART_SendNumber8b(USARTx, value[--i]);
        /*Wait until data register is empty*/
        while (!USART_GetFlagStatus(USARTx, USART_FLAG_TXE))
            ;
        USART_SendData(USARTx, value[--i]);
    }
}

#ifdef DAC_PA4_PA5
void SPI_Tx(uint8_t address, uint8_t data)
{
    /*Reset SPI1_CS*/
    GPIO_ResetBits(GPIOD, GPIO_Pin_14);

    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
        ;
    SPI_I2S_SendData(SPI1, address);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))
        ;
    SPI_I2S_ReceiveData(SPI1);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
        ;
    SPI_I2S_SendData(SPI1, data);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))
        ;
    SPI_I2S_ReceiveData(SPI1);

    /*Set SPI1_CS and wait*/
    GPIO_SetBits(GPIOD, GPIO_Pin_14);
}

uint8_t SPI_Rx(uint8_t address)
{
    /*Reset SPI1_CS*/
    GPIO_ResetBits(GPIOD, GPIO_Pin_14);

    address = 0x00 | address;
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
        ;
    SPI_I2S_SendData(SPI1, address);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))
        ;
    SPI_I2S_ReceiveData(SPI1);
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
        ;
    SPI_I2S_SendData(SPI1, 0x00); /*Check the slave Datasheet*/
    while (!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE))
        ;

    /*Set SPI1_CS and wait*/
    GPIO_SetBits(GPIOD, GPIO_Pin_14);

    return SPI_I2S_ReceiveData(SPI1);
}
#endif