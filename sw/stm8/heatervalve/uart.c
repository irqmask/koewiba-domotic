/**
 * @addtogroup STM8_UART
 *
 * @{
 * @file    uart.c
 * @brief   A module to interface STM8 UART.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2022  christian <irqmask@web.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// --- Include section ---------------------------------------------------------

#include "uart.h"

//#include "STM8L052C6.h"
#include <stm8l15x.h>

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

#define MP1 (1 << 2)

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void enable_uart_tx(void)
{
    PA_DDR |= MP1;
    USART1_CR2 |= USART_CR2_TEN;    // enable UART
}


static void disable_uart_tx(void)
{
    USART1_CR2 &= ~USART_CR2_TEN;   // disable UART to not disturb ATtiny's SPI
    PA_DDR &= ~MP1;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize UART interface.
 */
void uart_initialize(void)
{
    /*
    // TX: PA2, RX: PA3
    SYSCFG_RMPCR1 |= 0x10;
    PA_CR1 |= MP1;
    disable_uart_tx();

    // enable transmit and receive, no interrrupts
    USART1_CR2 = USART_CR2_REN; // not USART_CR2_TEN

    // 1 stop bit
    USART1_CR3 &= ~(USART_CR3_STOP1 | USART_CR3_STOP2);
    // 57600 baud
    USART1_BRR1 = 0x11;
    USART1_BRR2 = 0x6;*/
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, ENABLE);
    GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_In_FL_No_IT);
    SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortA, ENABLE);

    GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_2, ENABLE);
    GPIO_ExternalPullUpConfig(GPIOA, GPIO_Pin_3, ENABLE);

    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
    USART_Init(USART1,
               9600, // 57600
               USART_WordLength_8b,
               USART_StopBits_1,
               USART_Parity_No,
               (USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));

    USART_Cmd(USART1, ENABLE);
}


uint8_t uart_write(const char *str)
{
    uint8_t i = 0;
    enable_uart_tx();
    while (str[i] != '\0') {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART_SendData8(USART1, str[i]);
        //while (!(USART1_SR & USART_SR_TXE));
        //USART1_DR = str[i];
        i++;
    }
    disable_uart_tx();
    return (i); // Bytes sent
}

bool uart_rx_pending(void)
{
    //return USART1_SR & USART_SR_RXNE;
    return USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET;
}

uint8_t uart_rx_data(void)
{
    //return USART1_DR;
    return USART_ReceiveData8(USART1);
}

void putchar(unsigned char data)
{
    enable_uart_tx();
    USART1_DR = data;
    while (!(USART1_SR & USART_SR_TC))
        ;
    disable_uart_tx();
}

void dec2bcd(uint16_t val, char* bcdbuf)
{
    uint16_t temp;

    temp = val / 10000;
    bcdbuf[0] = temp + '0';
    val = val % 10000;

    temp = val / 1000;
    bcdbuf[1] = temp + '0';
    val = val % 1000;

    temp = val / 100;
    bcdbuf[2] = temp + '0';
    val = val % 100;

    temp = val / 10;
    bcdbuf[3] = temp + '0';
    val = val % 10;

    temp = val;
    bcdbuf[4] = temp + '0';

    bcdbuf[5] = '\0';
}

/** @} */
