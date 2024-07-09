/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include <stdio.h>
#include <string.h>

void RCC_Setup(void);
void GPIO_Setup(void);
void VirtualCOMPort_Setup(void);
void USART_Setup(void);
void ADC_Setup(void);
void DMA_Setup(void);
#ifdef DAC_PA4_PA5
void SPI_Setup(void); // Need to disable SPI since PA4 and PA5 for DAC are on
                      // SPI2 and SPI1, respectively
#endif
void TIMER_Setup(void);
void DAC_Setup(void);
void RTC_Setup(void);
void I2C_Setup(void);

/*Delay prototypes*/
void delayMS(uint32_t ms);
void delayUS(uint32_t us);

#endif /* __MAIN_H */
