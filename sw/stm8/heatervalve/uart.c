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

// include
#include "stm8l052c6.h"

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void enable_uart_tx(void)
{
    PA_DDR |= PIN_2;
    USART1_CR2 |= USART_CR2_TEN;    // enable UART
}

static void disable_uart_tx(void)
{
    USART1_CR2 &= ~USART_CR2_TEN;   // disable UART to not disturb ATtiny's SPI
    PA_DDR &= ~PIN_2;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize UART interface.
 */
void uart_initialize(void)
{
    // remap UART1 PINs TX: PA2, RX: PA3
    SYSCFG_RMPCR1 &= ~SYSCFG_RMPCR1_USART1TXRXMASK;
    SYSCFG_RMPCR1 |= SYSCFG_RMPCR1_USART1TXRXPORTA;

    // enable peripheral clock
    CLK_PCKENR1 |= CLK_PCKENR1_USART1;

    // PA2: TX
    PA_CR1 |= PIN_2;
    PA_DDR &= ~PIN_2;
    // PA3: RX
    PA_DDR &= ~PIN_3;

    // enable USART1 (resetting disable bit)
    USART1_CR1 &= ~USART_CR1_UARTD;
    // enable receive, no interrupts
    USART1_CR2 = USART_CR2_REN; // not USART_CR2_TEN
    USART1_CR3 &= ~(USART_CR3_STOP1 | USART_CR3_STOP2);

    //disable_uart_tx();

    // calculate baudrate
    // fHSI = 16MHz, ckdiv = 8 -> fClk = 2MHz
    // M = fClk / fBaud

    //         msb              lsb
    // BRR1 = M(15..12) | M(03..00)
    // BRR2 = M(11..08) | M(07..04)

    // 9600 baud
    // M = fClk / fBaud = 2000000 / 9600 = 208,33 = 0xD0
    USART1_BRR1 = 0x0D;
    USART1_BRR2 = 0x00;

    // 57600 baud
    // M = fClk / fBaud = 2000000 / 57600 = 34,72 = 35 = 0x23
    //USART1_BRR1 = 0x02;
    //USART1_BRR2 = 0x03;

    enable_uart_tx();
}

uint8_t uart_write(const char *str)
{
    uint8_t i = 0;
    //enable_uart_tx();
    while (str[i] != '\0') {
        while (!(USART1_SR & USART_SR_TXE));
        USART1_DR = str[i];
        i++;
    }
    //disable_uart_tx();
    return (i); // Bytes sent
}

bool uart_rx_pending(void)
{
    return USART1_SR & USART_SR_RXNE;
}

uint8_t uart_rx_data(void)
{
    return USART1_DR;
}

int putchar (int data)
{
    //enable_uart_tx();
    USART1_DR = data;
    while (!(USART1_SR & USART_SR_TC));
    //disable_uart_tx();
    return 1;
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
