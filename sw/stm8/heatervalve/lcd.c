/**
 * @addtogroup STM8_LCD
 *
 * @{
 * @file    lcd.c
 * @brief   A module to interface STM8 LCD display.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2024  christian <irqmask@web.de>
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

#include "lcd.h"

// include
#include <stm8l052c6.h>

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static uint8_t g_segmentA[4][2] = {
        { LCD_RAM_IDX03, 0x40 },
        { LCD_RAM_IDX04, 0x02 },
        { LCD_RAM_IDX04, 0x80 },
        { LCD_RAM_IDX05, 0x04 }
};

static uint8_t g_segmentB[4][2] = {
        { LCD_RAM_IDX00, 0x08 },
        { LCD_RAM_IDX00, 0x40 },
        { LCD_RAM_IDX01, 0x10 },
        { LCD_RAM_IDX01, 0x80 }
};

static uint8_t g_segmentC[4][2] = {
        { LCD_RAM_IDX0A, 0x80 },
        { LCD_RAM_IDX0B, 0x04 },
        { LCD_RAM_IDX0C, 0x01 },
        { LCD_RAM_IDX0C, 0x08 }
};

static uint8_t g_segmentD[4][2] = {
        { LCD_RAM_IDX0A, 0x40 },
        { LCD_RAM_IDX0B, 0x02 },
        { LCD_RAM_IDX0B, 0x80 },
        { LCD_RAM_IDX0C, 0x04 }
};

static uint8_t g_segmentE[4][2] = {
        { LCD_RAM_IDX0A, 0x20 },
        { LCD_RAM_IDX0B, 0x01 },
        { LCD_RAM_IDX0B, 0x40 },
        { LCD_RAM_IDX0C, 0x02 }
};

static uint8_t g_segmentF[4][2] = {
        { LCD_RAM_IDX03, 0x20 },
        { LCD_RAM_IDX04, 0x01 },
        { LCD_RAM_IDX04, 0x40 },
        { LCD_RAM_IDX05, 0x02 }
};

static uint8_t g_segmentG1[4][2] = {
        { LCD_RAM_IDX07, 0x02 },
        { LCD_RAM_IDX07, 0x10 },
        { LCD_RAM_IDX08, 0x04 },
        { LCD_RAM_IDX08, 0x20 }
};

static uint8_t g_segmentG2[4][2] = {
        { LCD_RAM_IDX03, 0x80 },
        { LCD_RAM_IDX04, 0x04 },
        { LCD_RAM_IDX05, 0x01 },
        { LCD_RAM_IDX05, 0x08 }
};

static uint8_t g_segmentT[4][2] = {
        { LCD_RAM_IDX07, 0x04 },
        { LCD_RAM_IDX07, 0x20 },
        { LCD_RAM_IDX08, 0x08 },
        { LCD_RAM_IDX08, 0x40 }
};

static uint8_t g_segmentX[4][2] = {
        { LCD_RAM_IDX07, 0x08 },
        { LCD_RAM_IDX07, 0x40 },
        { LCD_RAM_IDX08, 0x10 },
        { LCD_RAM_IDX08, 0x80 }
};

static uint8_t g_segment_dp[4][2] = {
        { LCD_RAM_IDX00, 0x10 },
        { LCD_RAM_IDX01, 0x01 },
        { LCD_RAM_IDX01, 0x08 },
        { LCD_RAM_IDX01, 0x40 }
};

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize LCD interface.
 */
void lcd_initialize(void)
{
    // enable LCD peripheral clock
    CLK_PCKENR2 |=  CLK_PCKENR2_LCD;

    // - set prescaler to 2
    LCD_FRQ &= ~LCD_FRQ_PS_MASK;
    LCD_FRQ |= LCD_FRQ_PS_2;

    // - set divider to / 23
    LCD_FRQ &= ~LCD_FRQ_DIV_MASK;
    LCD_FRQ |= LCD_FRQ_DIV_23;

    // - set duty cycle to 1/4
    LCD_CR1 &= ~LCD_CR1_DUTY_MASK;
    LCD_CR4 &= ~LCD_CR4_DUTY8;
    LCD_CR1 |= LCD_CR1_DUTY_1_4;

    // - set BIAS to 1/3
    LCD_CR1 &= ~LCD_CR1_B2_MASK;
    LCD_CR4 &= ~LCD_CR4_BIAS_1_4;
    LCD_CR1 |= LCD_CR1_BIAS_1_3;

    // - set LCD's voltage source to internal source
    LCD_CR2 &= ~LCD_CR2_VSEL_MASK;
    LCD_CR2 |= LCD_CR2_VSEL_INT;

    // - configure LCD port pins
    LCD_PM0 = 0xFF;
    LCD_PM1 = 0xFF;
    LCD_PM2 = 0x00; //0x7F;// 0x03
    LCD_PM3 = 0x00;

    // - set contrast
    LCD_CR2 &= ~LCD_CR2_CTRST_MASK;
    LCD_CR2 |= LCD_CR2_CTRST_LVL_3;

    // - set dead time
    LCD_CR3 &= ~LCD_CR3_DEAD_MASK;
    LCD_CR3 |= LCD_CR3_DEAD_3;

    // - set pulse-on duration
    LCD_CR2 &= ~LCD_CR2_PON_MASK;
    LCD_CR2 |= LCD_CR2_PON_DUR_1;

    lcd_blink(false);

    // - enable LCD peripheral and clear LCD RAM
    LCD_CR3 |= LCD_CR3_LCDEN;
    for (uint8_t idx = 0; idx < 14; idx++) {
        LCD_RAM[LCD_RAM_IDX00 + idx] = 0x00;
    }
}


void lcd_blink(bool on)
{
    LCD_CR1 &= ~LCD_CR1_BLINK_MASK;
    LCD_CR1 &= ~LCD_CR1_BLINKF_MASK;
    if (on) {
        LCD_CR1 |= LCD_CR1_BLINKMODE_ALLSEG_ALLCOM;
        LCD_CR1 |= LCD_CR1_BLINKFREQ_DIV_512;
    }
    else {
        LCD_CR1 |= LCD_CR1_BLINKMODE_OFF;
        LCD_CR1 |= LCD_CR1_BLINKFREQ_DIV_512;
    }
}


void lcd_digit(uint8_t index, char value)
{
    switch (value)
    {
    case 0:
    case '0':
        LCD_RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD_RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD_RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD_RAM[g_segmentD[index][0]] |= g_segmentD[index][1];
        LCD_RAM[g_segmentE[index][0]] |= g_segmentE[index][1];
        LCD_RAM[g_segmentF[index][0]] |= g_segmentF[index][1];
        LCD_RAM[g_segmentG1[index][0]] &= ~g_segmentG1[index][1];
        LCD_RAM[g_segmentG2[index][0]] &= ~g_segmentG2[index][1];
        LCD_RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD_RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 1:
    case '1':
        LCD_RAM[g_segmentA[index][0]] &= ~g_segmentA[index][1];
        LCD_RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD_RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD_RAM[g_segmentD[index][0]] &= ~g_segmentD[index][1];
        LCD_RAM[g_segmentE[index][0]] &= ~g_segmentE[index][1];
        LCD_RAM[g_segmentF[index][0]] &= ~g_segmentF[index][1];
        LCD_RAM[g_segmentG1[index][0]] &= ~g_segmentG1[index][1];
        LCD_RAM[g_segmentG2[index][0]] &= ~g_segmentG2[index][1];
        LCD_RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD_RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 2:
    case '2':
        LCD_RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD_RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD_RAM[g_segmentC[index][0]] &= ~g_segmentC[index][1];
        LCD_RAM[g_segmentD[index][0]] |= g_segmentD[index][1];
        LCD_RAM[g_segmentE[index][0]] |= g_segmentE[index][1];
        LCD_RAM[g_segmentF[index][0]] &= ~g_segmentF[index][1];
        LCD_RAM[g_segmentG1[index][0]] |= g_segmentG1[index][1];
        LCD_RAM[g_segmentG2[index][0]] |= g_segmentG2[index][1];
        LCD_RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD_RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 3:
    case '3':
        LCD_RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD_RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD_RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD_RAM[g_segmentD[index][0]] |= g_segmentD[index][1];
        LCD_RAM[g_segmentE[index][0]] &= ~g_segmentE[index][1];
        LCD_RAM[g_segmentF[index][0]] &= ~g_segmentF[index][1];
        LCD_RAM[g_segmentG1[index][0]] |= g_segmentG1[index][1];
        LCD_RAM[g_segmentG2[index][0]] |= g_segmentG2[index][1];
        LCD_RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD_RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 4:
    case '4':
        LCD_RAM[g_segmentA[index][0]] &= ~g_segmentA[index][1];
        LCD_RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD_RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD_RAM[g_segmentD[index][0]] &= ~g_segmentD[index][1];
        LCD_RAM[g_segmentE[index][0]] &= ~g_segmentE[index][1];
        LCD_RAM[g_segmentF[index][0]] |= g_segmentF[index][1];
        LCD_RAM[g_segmentG1[index][0]] |= g_segmentG1[index][1];
        LCD_RAM[g_segmentG2[index][0]] |= g_segmentG2[index][1];
        LCD_RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD_RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 5:
    case '5':
        LCD_RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD_RAM[g_segmentB[index][0]] &= ~g_segmentB[index][1];
        LCD_RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD_RAM[g_segmentD[index][0]] |= g_segmentD[index][1];
        LCD_RAM[g_segmentE[index][0]] &= ~g_segmentE[index][1];
        LCD_RAM[g_segmentF[index][0]] |= g_segmentF[index][1];
        LCD_RAM[g_segmentG1[index][0]] |= g_segmentG1[index][1];
        LCD_RAM[g_segmentG2[index][0]] |= g_segmentG2[index][1];
        LCD_RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD_RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 6:
    case '6':
        LCD_RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD_RAM[g_segmentB[index][0]] &= ~g_segmentB[index][1];
        LCD_RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD_RAM[g_segmentD[index][0]] |= g_segmentD[index][1];
        LCD_RAM[g_segmentE[index][0]] |= g_segmentE[index][1];
        LCD_RAM[g_segmentF[index][0]] |= g_segmentF[index][1];
        LCD_RAM[g_segmentG1[index][0]] |= g_segmentG1[index][1];
        LCD_RAM[g_segmentG2[index][0]] |= g_segmentG2[index][1];
        LCD_RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD_RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 7:
    case '7':
        LCD_RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD_RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD_RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD_RAM[g_segmentD[index][0]] &= ~g_segmentD[index][1];
        LCD_RAM[g_segmentE[index][0]] &= ~g_segmentE[index][1];
        LCD_RAM[g_segmentF[index][0]] &= ~g_segmentF[index][1];
        LCD_RAM[g_segmentG1[index][0]] &= ~g_segmentG1[index][1];
        LCD_RAM[g_segmentG2[index][0]] &= ~g_segmentG2[index][1];
        LCD_RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD_RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 8:
    case '8':
        LCD_RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD_RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD_RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD_RAM[g_segmentD[index][0]] |= g_segmentD[index][1];
        LCD_RAM[g_segmentE[index][0]] |= g_segmentE[index][1];
        LCD_RAM[g_segmentF[index][0]] |= g_segmentF[index][1];
        LCD_RAM[g_segmentG1[index][0]] |= g_segmentG1[index][1];
        LCD_RAM[g_segmentG2[index][0]] |= g_segmentG2[index][1];
        LCD_RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD_RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 9:
    case '9':
        LCD_RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD_RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD_RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD_RAM[g_segmentD[index][0]] |= g_segmentD[index][1];
        LCD_RAM[g_segmentE[index][0]] &= ~g_segmentE[index][1];
        LCD_RAM[g_segmentF[index][0]] |= g_segmentF[index][1];
        LCD_RAM[g_segmentG1[index][0]] |= g_segmentG1[index][1];
        LCD_RAM[g_segmentG2[index][0]] |= g_segmentG2[index][1];
        LCD_RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD_RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case ' ':
        LCD_RAM[g_segmentA[index][0]] &= ~g_segmentA[index][1];
        LCD_RAM[g_segmentB[index][0]] &= ~g_segmentB[index][1];
        LCD_RAM[g_segmentC[index][0]] &= ~g_segmentC[index][1];
        LCD_RAM[g_segmentD[index][0]] &= ~g_segmentD[index][1];
        LCD_RAM[g_segmentE[index][0]] &= ~g_segmentE[index][1];
        LCD_RAM[g_segmentF[index][0]] &= ~g_segmentF[index][1];
        LCD_RAM[g_segmentG1[index][0]] &= ~g_segmentG1[index][1];
        LCD_RAM[g_segmentG2[index][0]] &= ~g_segmentG2[index][1];
        LCD_RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD_RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case '°':
        LCD_RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD_RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD_RAM[g_segmentC[index][0]] &= ~g_segmentC[index][1];
        LCD_RAM[g_segmentD[index][0]] &= ~g_segmentD[index][1];
        LCD_RAM[g_segmentE[index][0]] &= ~g_segmentE[index][1];
        LCD_RAM[g_segmentF[index][0]] |= g_segmentF[index][1];
        LCD_RAM[g_segmentG1[index][0]] |= g_segmentG1[index][1];
        LCD_RAM[g_segmentG2[index][0]] |= g_segmentG2[index][1];
        LCD_RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD_RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 'C':
        LCD_RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD_RAM[g_segmentB[index][0]] &= ~g_segmentB[index][1];
        LCD_RAM[g_segmentC[index][0]] &= ~g_segmentC[index][1];
        LCD_RAM[g_segmentD[index][0]] |= g_segmentD[index][1];
        LCD_RAM[g_segmentE[index][0]] |= g_segmentE[index][1];
        LCD_RAM[g_segmentF[index][0]] |= g_segmentF[index][1];
        LCD_RAM[g_segmentG1[index][0]] &= ~g_segmentG1[index][1];
        LCD_RAM[g_segmentG2[index][0]] &= ~g_segmentG2[index][1];
        LCD_RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD_RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    default:
        break;
    }
}

void lcd_disp_value(uint16_t value, uint8_t decpoint)
{
    uint8_t digit, dv;
    for (digit=4; digit>0; digit--) {
        if (value == 0) {
            if (digit == 4) {
                lcd_digit(digit - 1, 0);
            }
            else {
                lcd_digit(digit - 1, ' ');
            }
        } else {
            uint8_t dv = value % 10;
            value = value / 10;
            lcd_digit(digit - 1, dv);
        }
    }
    // insert decimal point
    if (decpoint < 4) {
        decpoint = 3 - decpoint;
        LCD_RAM[g_segment_dp[0][0]] &= ~g_segment_dp[0][1];
        LCD_RAM[g_segment_dp[1][0]] &= ~g_segment_dp[1][1];
        LCD_RAM[g_segment_dp[2][0]] &= ~g_segment_dp[2][1];
        LCD_RAM[g_segment_dp[3][0]] &= ~g_segment_dp[3][1];
        LCD_RAM[g_segment_dp[decpoint][0]] |= g_segment_dp[decpoint][1];
    }
}

void lcd_disp_symbol(char symbol, bool on)
{
    // FIXME implement displaying various symbols
    (void)on;
    switch (symbol) {
    case 'D':
        if (on) {

        }
        else {

        }
        break;
    default:
        break;
    }
}

/** @} */
