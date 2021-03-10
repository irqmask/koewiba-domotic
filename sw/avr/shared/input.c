/**
 * @addtogroup SINPUT
 * @brief This module contains functions to read, debounce and evaluate input 
 * signals.
 * 
 * @{
 * @file    input.c
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
  
// --- Include section ---------------------------------------------------------
 
#include <avr/io.h>
 
#include "input.h"
 
// --- Definitions -------------------------------------------------------------
 
#ifdef HAS_PCBCONFIG_H
#include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
#include "appconfig.h"
#endif
 
#ifndef INPUT_PCBCONFIG
#define INPUT_PCBCONFIG     1

#define INPUT_NUM_PINS      6
#define INPUT_PORT_0        PORTC
#define INPUT_PIN_0         PINC
#define INPUT_DDR_0         DDRC
#define INPUT_0             PC0
#define INPUT_PORT_1        PORTC
#define INPUT_PIN_1         PINC
#define INPUT_DDR_1         DDRC
#define INPUT_1             PC1
#define INPUT_PORT_2        PORTC
#define INPUT_PIN_2         PINC
#define INPUT_DDR_2         DDRC
#define INPUT_2             PC2
#define INPUT_PORT_3        PORTC
#define INPUT_PIN_3         PINC
#define INPUT_DDR_3         DDRC
#define INPUT_3             PC3
#define INPUT_PORT_4        PORTC
#define INPUT_PIN_4         PINC
#define INPUT_DDR_4         DDRC
#define INPUT_4             PC4
#define INPUT_PORT_5        PORTC
#define INPUT_PIN_5         PINC
#define INPUT_DDR_5         DDRC
#define INPUT_5             PC5
#define INPUT_PORT_6        PORTC
#define INPUT_PIN_6         PINC
#define INPUT_DDR_6         DDRC
#define INPUT_6             PC6
#define INPUT_PORT_7        PORTC
#define INPUT_PIN_7         PINC
#define INPUT_DDR_7         DDRC
#define INPUT_7             PC7

#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static uint8_t g_debounce[INPUT_NUM_PINS];
static uint8_t g_last_state;
static uint8_t g_new_state;

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void input_initialize(void)
{
#if INPUT_NUM_PINS > 0
    INPUT_DDR_0 &= ~(1<<INPUT_0);
    INPUT_PORT_0 |= (1<<INPUT_0);
    g_debounce[0] = 0;
#endif
#if INPUT_NUM_PINS > 1
    INPUT_DDR_1 &= ~(1<<INPUT_1);
    INPUT_PORT_1 |= (1<<INPUT_1);
    g_debounce[1] = 0;
#endif
#if INPUT_NUM_PINS > 2
    INPUT_DDR_2 &= ~(1<<INPUT_2);
    INPUT_PORT_2 |= (1<<INPUT_2);
    g_debounce[2] = 0;
#endif
#if INPUT_NUM_PINS > 3
    INPUT_DDR_3 &= ~(1<<INPUT_3);
    INPUT_PORT_3 |= (1<<INPUT_3);
    g_debounce[3] = 0;
#endif
#if INPUT_NUM_PINS > 4
    INPUT_DDR_4 &= ~(1<<INPUT_4);
    INPUT_PORT_4 |= (1<<INPUT_4);
    g_debounce[4] = 0;
#endif
#if INPUT_NUM_PINS > 5
    INPUT_DDR_5 &= ~(1<<INPUT_5);
    INPUT_PORT_5 |= (1<<INPUT_5);
    g_debounce[5] = 0;
#endif
#if INPUT_NUM_PINS > 6
    INPUT_DDR_6 &= ~(1<<INPUT_6);
    INPUT_PORT_6 |= (1<<INPUT_6);
    g_debounce[6] = 0;
#endif
#if INPUT_NUM_PINS > 7
    INPUT_DDR_7 &= ~(1<<INPUT_7);
    INPUT_PORT_7 |= (1<<INPUT_7);
    g_debounce[7] = 0;
#endif
    g_last_state = 0;
    g_new_state = 0;
}


void input_background(void) 
{
    g_last_state = g_new_state;
#if INPUT_NUM_PINS > 0
    g_debounce[0] <<= 1;
    if (INPUT_PIN_0 & (1<<INPUT_0)) g_debounce[0] |= 1;
    if (g_debounce[0] == 0xFF) g_new_state |= (1 << 0);
    if (g_debounce[0] == 0x00) g_new_state &= ~(1 << 0);
#endif
#if INPUT_NUM_PINS > 1
    g_debounce[1] <<= 1;
    if (INPUT_PIN_1 & (1<<INPUT_1)) g_debounce[1] |= 1;
    if (g_debounce[1] == 0xFF) g_new_state |= (1 << 1);
    if (g_debounce[1] == 0x00) g_new_state &= ~(1 << 1);
#endif
#if INPUT_NUM_PINS > 2
    g_debounce[2] <<= 1;
    if (INPUT_PIN_2 & (1<<INPUT_2)) g_debounce[2] |= 1;
    if (g_debounce[2] == 0xFF) g_new_state |= (1 << 2);
    if (g_debounce[2] == 0x00) g_new_state &= ~(1 << 2);
#endif
#if INPUT_NUM_PINS > 3
    g_debounce[3] <<= 1;
    if (INPUT_PIN_3 & (1<<INPUT_3)) g_debounce[3] |= 1;
    if (g_debounce[3] == 0xFF) g_new_state |= (1 << 3);
    if (g_debounce[3] == 0x00) g_new_state &= ~(1 << 3);
#endif
#if INPUT_NUM_PINS > 4
    g_debounce[4] <<= 1;
    if (INPUT_PIN_4 & (1<<INPUT_4)) g_debounce[4] |= 1;
    if (g_debounce[4] == 0xFF) g_new_state |= (1 << 4);
    if (g_debounce[4] == 0x00) g_new_state &= ~(1 << 4);
#endif
#if INPUT_NUM_PINS > 5
    g_debounce[5] <<= 1;
    if (INPUT_PIN_5 & (1<<INPUT_5)) g_debounce[5] |= 1;
    if (g_debounce[5] == 0xFF) g_new_state |= (1 << 5);
    if (g_debounce[5] == 0x00) g_new_state &= ~(1 << 5);
#endif
#if INPUT_NUM_PINS > 6
    g_debounce[6] <<= 1;
    if (INPUT_PIN_6 & (1<<INPUT_6)) g_debounce[6] |= 1;
    if (g_debounce[6] == 0xFF) g_new_state |= (1 << 6);
    if (g_debounce[6] == 0x00) g_new_state &= ~(1 << 6);

#endif
#if INPUT_NUM_PINS > 7
    g_debounce[7] <<= 1;
    if (INPUT_PIN_7 & (1<<INPUT_7)) g_debounce[7] |= 1;
    if (g_debounce[7] == 0xFF) g_new_state |= (1 << 7);
    if (g_debounce[7] == 0x00) g_new_state &= ~(1 << 7);
#endif
}


uint8_t input_state(void)
{
    return g_new_state;
}


uint8_t input_went_high(void)
{
    return (~g_last_state & g_new_state);
}


uint8_t input_went_low(void)
{
    return (g_last_state & ~g_new_state);
}


uint8_t input_toggled(void)
{
    return (g_last_state ^ g_new_state);
}

/** @} */
