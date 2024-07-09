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

/* This function issues a start condition and
 * transmits the slave address + R/W bit
 *
 * Parameters:
 * 		I2Cx --> the I2C peripheral e.g. I2C
 * 		address --> the 7 bit slave address
 * 		direction --> the tranmission direction can be:
 * 						I2C_Direction_Tranmitter for Master transmitter mode
 * 						I2C_Direction_Receiver for Master receiver
 */
void I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction)
{
    /* wait until I2C is not busy anymore */
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY))
        ;

    /* Send I2C START condition */
    I2C_GenerateSTART(I2Cx, ENABLE);

    /* wait for I2C EV5 --> Slave has acknowledged start condition */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
        ;

    /* Send slave Address for write */
    I2C_Send7bitAddress(I2Cx, address, direction);

    /* wait for I2C EV6, check if
     * either Slave has acknowledged Master transmitter or
     * Master receiver mode, depending on the transmission
     * direction
     */
    if (direction == I2C_Direction_Transmitter)
    {
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
            ;
    }
    else if (direction == I2C_Direction_Receiver)
    {
        while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
            ;
    }
}

/* This function transmits one byte to the slave device
 * Parameters:
 *		I2Cx --> the I2C peripheral e.g. I2C
 *		data --> the data byte to be transmitted
 */
void I2C_Write(I2C_TypeDef* I2Cx, uint8_t data)
{
    I2C_SendData(I2Cx, data);
    /* wait for I2C EV8_2 --> byte has been transmitted */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        ;
}

/* This function reads one byte from the slave device
 * and acknowledges the byte (requests another byte)
 */
uint8_t I2C_Read_Ack(I2C_TypeDef* I2Cx)
{
    /* enable acknowledge of recieved data */
    I2C_AcknowledgeConfig(I2Cx, ENABLE);
    /* wait until one byte has been received */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
        ;
    /* read data from I2C data register and return data byte */
    uint8_t data = I2C_ReceiveData(I2Cx);
    return data;
}

/* This function reads one byte from the slave device
 * and doesn't acknowledge the recieved data
 */
uint8_t I2C_Read_Nack(I2C_TypeDef* I2Cx)
{
    /* disabe acknowledge of received data nack also generates stop condition after last byte
     * received, see reference manual for more info */
    I2C_AcknowledgeConfig(I2Cx, DISABLE);
    I2C_GenerateSTOP(I2Cx, ENABLE);

    /* wait until one byte has been received */
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED))
        ;
    /* read data from I2C data register and return data byte */
    uint8_t data = I2C_ReceiveData(I2Cx);
    return data;
}

/* This funtion issues a stop condition and therefore
 * releases the bus
 */
void I2C_Stop(I2C_TypeDef* I2Cx)
{
    /* Send I2C STOP Condition */
    I2C_GenerateSTOP(I2Cx, ENABLE);
}
