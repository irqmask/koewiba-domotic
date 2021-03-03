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
#define INPUT_PORT_PIN0     PINC
#define INPUT_DDR_PIN0      DDRC
#define INPUT_PIN0          PC0
#define INPUT_PORT_PIN1     PINC
#define INPUT_DDR_PIN1      DDRC
#define INPUT_PIN1          PC1
#define INPUT_PORT_PIN2     PINC
#define INPUT_DDR_PIN2      DDRC
#define INPUT_PIN2          PC2
#define INPUT_PORT_PIN3     PINC
#define INPUT_DDR_PIN3      DDRC
#define INPUT_PIN3          PC3
#define INPUT_PORT_PIN4     PINC
#define INPUT_DDR_PIN4      DDRC
#define INPUT_PIN4          PC4
#define INPUT_PORT_PIN5     PINC
#define INPUT_DDR_PIN5      DDRC
#define INPUT_PIN5          PC5
#define INPUT_PORT_PIN6     PINC
#define INPUT_DDR_PIN6      DDRC
#define INPUT_PIN6          PC6
#define INPUT_PORT_PIN7     PINC
#define INPUT_DDR_PIN7      DDRC
#define INPUT_PIN7          PC7

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
    INPUT_DDR_PIN0 &= ~(1<<INPUT_PIN0);
    g_debounce[0] = 0;
#endif
#if INPUT_NUM_PINS > 1
    INPUT_DDR_PIN1 &= ~(1<<INPUT_PIN1);
    g_debounce[1] = 0;
#endif
#if INPUT_NUM_PINS > 2
    INPUT_DDR_PIN2 &= ~(1<<INPUT_PIN2);
    g_debounce[2] = 0;
#endif
#if INPUT_NUM_PINS > 3
    INPUT_DDR_PIN3 &= ~(1<<INPUT_PIN3);
    g_debounce[3] = 0;
#endif
#if INPUT_NUM_PINS > 4
    INPUT_DDR_PIN4 &= ~(1<<INPUT_PIN4);
    g_debounce[4] = 0;
#endif
#if INPUT_NUM_PINS > 5
    INPUT_DDR_PIN5 &= ~(1<<INPUT_PIN5);
    g_debounce[5] = 0;
#endif
#if INPUT_NUM_PINS > 6
    INPUT_DDR_PIN6 &= ~(1<<INPUT_PIN6);
    g_debounce[6] = 0;
#endif
#if INPUT_NUM_PINS > 7
    INPUT_DDR_PIN7 &= ~(1<<INPUT_PIN7);
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
    if (INPUT_PORT_PIN0 & (1<<INPUT_PIN0)) g_debounce[0] |= 1;
    if (g_debounce[0] == 0xFF) g_new_state |= (1 << 0);
    if (g_debounce[0] == 0x00) g_new_state &= ~(1 << 0);
#endif
#if INPUT_NUM_PINS > 1
    g_debounce[1] <<= 1;
    if (INPUT_PORT_PIN1 & (1<<INPUT_PIN1)) g_debounce[1] |= 1;
    if (g_debounce[1] == 0xFF) g_new_state |= (1 << 1);
    if (g_debounce[1] == 0x00) g_new_state &= ~(1 << 1);
#endif
#if INPUT_NUM_PINS > 2
    g_debounce[2] <<= 1;
    if (INPUT_PORT_PIN2 & (1<<INPUT_PIN2)) g_debounce[2] |= 1;
    if (g_debounce[2] == 0xFF) g_new_state |= (1 << 2);
    if (g_debounce[2] == 0x00) g_new_state &= ~(1 << 2);
#endif
#if INPUT_NUM_PINS > 3
    g_debounce[3] <<= 1;
    if (INPUT_PORT_PIN3 & (1<<INPUT_PIN3)) g_debounce[3] |= 1;
    if (g_debounce[3] == 0xFF) g_new_state |= (1 << 3);
    if (g_debounce[3] == 0x00) g_new_state &= ~(1 << 3);
#endif
#if INPUT_NUM_PINS > 4
    g_debounce[4] <<= 1;
    g_debounce[4] |= INPUT_PORT_PIN4 & (1<<INPUT_PIN4);
    if (INPUT_PORT_PIN4 & (1<<INPUT_PIN4)) g_debounce[4] |= 1;
    if (g_debounce[4] == 0xFF) g_new_state |= (1 << 4);
    if (g_debounce[4] == 0x00) g_new_state &= ~(1 << 4);
#endif
#if INPUT_NUM_PINS > 5
    g_debounce[5] <<= 1;
    g_debounce[5] |= INPUT_PORT_PIN5 & (1<<INPUT_PIN5);
    if (INPUT_PORT_PIN5 & (1<<INPUT_PIN5)) g_debounce[5] |= 1;
    if (g_debounce[5] == 0xFF) g_new_state |= (1 << 5);
    if (g_debounce[5] == 0x00) g_new_state &= ~(1 << 5);
#endif
#if INPUT_NUM_PINS > 6
    g_debounce[6] <<= 1;
    g_debounce[6] |= INPUT_PORT_PIN6 & (1<<INPUT_PIN6);
    if (INPUT_PORT_PIN6 & (1<<INPUT_PIN6)) g_debounce[6] |= 1;
    if (g_debounce[6] == 0xFF) g_new_state |= (1 << 6);
    if (g_debounce[6] == 0x00) g_new_state &= ~(1 << 6);

#endif
#if INPUT_NUM_PINS > 7
    g_debounce[7] <<= 1;
    if (INPUT_PORT_PIN7 & (1<<INPUT_PIN7)) g_debounce[7] |= 1;
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
