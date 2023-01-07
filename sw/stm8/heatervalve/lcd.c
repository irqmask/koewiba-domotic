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

#include "lcd.h"

#include <stm8l15x_lcd.h>

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

static uint8_t g_segmentA[4][2] = {
        { LCD_RAMRegister_3, 0x40 },
        { LCD_RAMRegister_4, 0x02 },
        { LCD_RAMRegister_4, 0x80 },
        { LCD_RAMRegister_5, 0x04 }
};

static uint8_t g_segmentB[4][2] = {
        { LCD_RAMRegister_0, 0x08 },
        { LCD_RAMRegister_0, 0x40 },
        { LCD_RAMRegister_1, 0x10 },
        { LCD_RAMRegister_1, 0x80 }
};

static uint8_t g_segmentC[4][2] = {
        { LCD_RAMRegister_10, 0x80 },
        { LCD_RAMRegister_11, 0x04 },
        { LCD_RAMRegister_12, 0x01 },
        { LCD_RAMRegister_12, 0x08 }
};

static uint8_t g_segmentD[4][2] = {
        { LCD_RAMRegister_10, 0x40 },
        { LCD_RAMRegister_11, 0x02 },
        { LCD_RAMRegister_11, 0x80 },
        { LCD_RAMRegister_12, 0x04 }
};

static uint8_t g_segmentE[4][2] = {
        { LCD_RAMRegister_10, 0x20 },
        { LCD_RAMRegister_11, 0x01 },
        { LCD_RAMRegister_11, 0x40 },
        { LCD_RAMRegister_12, 0x02 }
};

static uint8_t g_segmentF[4][2] = {
        { LCD_RAMRegister_3, 0x20 },
        { LCD_RAMRegister_4, 0x01 },
        { LCD_RAMRegister_4, 0x40 },
        { LCD_RAMRegister_5, 0x02 }
};

static uint8_t g_segmentG1[4][2] = {
        { LCD_RAMRegister_7, 0x02 },
        { LCD_RAMRegister_7, 0x10 },
        { LCD_RAMRegister_8, 0x04 },
        { LCD_RAMRegister_8, 0x20 }
};

static uint8_t g_segmentG2[4][2] = {
        { LCD_RAMRegister_3, 0x80 },
        { LCD_RAMRegister_4, 0x04 },
        { LCD_RAMRegister_5, 0x01 },
        { LCD_RAMRegister_5, 0x08 }
};

static uint8_t g_segmentT[4][2] = {
        { LCD_RAMRegister_7, 0x04 },
        { LCD_RAMRegister_7, 0x20 },
        { LCD_RAMRegister_8, 0x08 },
        { LCD_RAMRegister_8, 0x40 }
};

static uint8_t g_segmentX[4][2] = {
        { LCD_RAMRegister_7, 0x08 },
        { LCD_RAMRegister_7, 0x40 },
        { LCD_RAMRegister_8, 0x10 },
        { LCD_RAMRegister_8, 0x80 }
};

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

extern void delay_ms(uint32_t dwDelay);

/**
 * Initialize LCD interface.
 */
void lcd_initialize(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_LCD, ENABLE);

//    LCD_Init(LCD_Prescaler_2, LCD_Divider_18, LCD_Duty_1_4,
//             LCD_Bias_1_3, LCD_VoltageSource_Internal);
    LCD_Init(LCD_Prescaler_2, LCD_Divider_23, LCD_Duty_1_4,
             LCD_Bias_1_2, LCD_VoltageSource_Internal);

    LCD_PortMaskConfig(LCD_PortMaskRegister_0, 0xFF);
    LCD_PortMaskConfig(LCD_PortMaskRegister_1, 0xFF);
    LCD_PortMaskConfig(LCD_PortMaskRegister_2, 0x7F);// 0x03
    LCD_PortMaskConfig(LCD_PortMaskRegister_3, 0x00);

//    LCD_ContrastConfig(LCD_Contrast_Level_4);
//    LCD_DeadTimeConfig(LCD_DeadTime_0);
//    LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1);
    LCD_ContrastConfig(LCD_Contrast_Level_6);
    LCD_DeadTimeConfig(LCD_DeadTime_0);
    LCD_PulseOnDurationConfig(LCD_PulseOnDuration_1);

    lcd_blink(false);
    LCD_Cmd(ENABLE); /*!< Enable LCD peripheral */
    for (uint8_t idx = 0; idx < 14; idx++) {
        LCD->RAM[LCD_RAMRegister_0 + idx] = 0x00;
    }
}


void lcd_blink(bool on)
{
    if (on) {
        LCD_BlinkConfig(LCD_BlinkMode_AllSEG_AllCOM, LCD_BlinkFrequency_Div512);
    }
    else {
        LCD_BlinkConfig(LCD_BlinkMode_Off, LCD_BlinkFrequency_Div512); //LCD_BlinkMode_TypeDef LCD_BlinkMode, LCD_BlinkFrequency_TypeDef LCD_BlinkFrequency
    }
}

/*
void lcd_digit0(char value)
{
    switch (value)
    {
    case 0:
    case '0':
        //LCD->RAM[LCD_RAMRegister_0] &= ~(0);
        LCD->RAM[LCD_RAMRegister_3] &= ~(0x80);
        LCD->RAM[LCD_RAMRegister_7] &= ~(0x02 | 0x04 | 0x08);
        //LCD->RAM[LCD_RAMRegister_10] &= ~(0);
        LCD->RAM[LCD_RAMRegister_0] |= (0x08);
        LCD->RAM[LCD_RAMRegister_3] |= (0x20 | 0x40);
        //LCD->RAM[LCD_RAMRegister_7] |= (0);
        LCD->RAM[LCD_RAMRegister_10] |= (0x20 | 0x40 | 0x80);
        break;

    case 1:
    case '1':
        //LCD->RAM[LCD_RAMRegister_0] &= ~(0);
        LCD->RAM[LCD_RAMRegister_3] &= ~(0x20 | 0x40 | 0x80);
        LCD->RAM[LCD_RAMRegister_7] &= ~(0x02 | 0x04 | 0x08);
        LCD->RAM[LCD_RAMRegister_10] &= ~(0x20 | 0x40);
        LCD->RAM[LCD_RAMRegister_0] |= (0x08);
        //LCD->RAM[LCD_RAMRegister_3] |= (0);
        //LCD->RAM[LCD_RAMRegister_7] |= (0);
        LCD->RAM[LCD_RAMRegister_10] |= (0x80);
        break;

    case 2:
    case '2':
        //LCD->RAM[LCD_RAMRegister_0] &= ~(0);
        LCD->RAM[LCD_RAMRegister_3] &= ~(0x20);
        LCD->RAM[LCD_RAMRegister_7] &= ~(0x04 | 0x08);
        LCD->RAM[LCD_RAMRegister_10] &= ~(0x80);
        LCD->RAM[LCD_RAMRegister_0] |= (0x08);
        LCD->RAM[LCD_RAMRegister_3] |= (0x40 | 0x80);
        LCD->RAM[LCD_RAMRegister_7] |= (0x02);
        LCD->RAM[LCD_RAMRegister_10] |= (0x20 | 0x40);
        break;

    case 3:
    case '3':
        //LCD->RAM[LCD_RAMRegister_0] &= ~(0);
        LCD->RAM[LCD_RAMRegister_3] &= ~(0x20);
        LCD->RAM[LCD_RAMRegister_7] &= ~(0x04 | 0x08);
        LCD->RAM[LCD_RAMRegister_10] &= ~(0x20);
        LCD->RAM[LCD_RAMRegister_0] |= (0x08);
        LCD->RAM[LCD_RAMRegister_3] |= (0x40 | 0x80);
        LCD->RAM[LCD_RAMRegister_7] |= (0x02);
        LCD->RAM[LCD_RAMRegister_10] |= (0x40 | 0x80);
        break;

    case 4:
    case '4':
        //LCD->RAM[LCD_RAMRegister_0] &= ~(0);
        LCD->RAM[LCD_RAMRegister_3] &= ~(0x40);
        LCD->RAM[LCD_RAMRegister_7] &= ~(0x04 | 0x08);
        LCD->RAM[LCD_RAMRegister_10] &= ~(0x20 | 0x40);
        LCD->RAM[LCD_RAMRegister_0] |= (0x08);
        LCD->RAM[LCD_RAMRegister_3] |= (0x20 | 0x80);
        LCD->RAM[LCD_RAMRegister_7] |= (0x02);
        LCD->RAM[LCD_RAMRegister_10] |= (0x80);
        break;

    case 5:
    case '5':
        LCD->RAM[LCD_RAMRegister_0] &= ~(0x08);
        //LCD->RAM[LCD_RAMRegister_3] &= ~(0);
        LCD->RAM[LCD_RAMRegister_7] &= ~(0x04 | 0x08);
        LCD->RAM[LCD_RAMRegister_10] &= ~(0x20);
        //LCD->RAM[LCD_RAMRegister_0] |= (0);
        LCD->RAM[LCD_RAMRegister_3] |= (0x20 | 0x40 | 0x80);
        LCD->RAM[LCD_RAMRegister_7] |= (0x02);
        LCD->RAM[LCD_RAMRegister_10] |= (0x40 | 0x80);
        break;

    case 6:
    case '6':
        LCD->RAM[LCD_RAMRegister_0] &= ~(0x08);
        //LCD->RAM[LCD_RAMRegister_3] &= ~(0);
        LCD->RAM[LCD_RAMRegister_7] &= ~(0x04 | 0x08);
        //LCD->RAM[LCD_RAMRegister_10] &= ~(0);
        //LCD->RAM[LCD_RAMRegister_0] |= (0);
        LCD->RAM[LCD_RAMRegister_3] |= (0x20 | 0x40 | 0x80);
        LCD->RAM[LCD_RAMRegister_7] |= (0x02);
        LCD->RAM[LCD_RAMRegister_10] |= (0x20 | 0x40 | 0x80);
        break;

    case 7:
    case '7':
        //LCD->RAM[LCD_RAMRegister_0] &= ~(0);
        LCD->RAM[LCD_RAMRegister_3] &= ~(0x20 | 0x80);
        LCD->RAM[LCD_RAMRegister_7] &= ~(0x02| 0x04 | 0x08);
        LCD->RAM[LCD_RAMRegister_10] &= ~(0x20 | 0x40 );
        LCD->RAM[LCD_RAMRegister_0] |= (0x08);
        LCD->RAM[LCD_RAMRegister_3] |= (0x40);
        //LCD->RAM[LCD_RAMRegister_7] |= (0);
        LCD->RAM[LCD_RAMRegister_10] |= (0x80);
        break;

    case 8:
    case '8':
        //LCD->RAM[LCD_RAMRegister_0] &= ~(0);
        //LCD->RAM[LCD_RAMRegister_3] &= ~(0);
        LCD->RAM[LCD_RAMRegister_7] &= ~(0x04 | 0x08);
        //LCD->RAM[LCD_RAMRegister_10] &= ~(0);
        LCD->RAM[LCD_RAMRegister_0] |= (0x08);
        LCD->RAM[LCD_RAMRegister_3] |= (0x20 | 0x40 | 0x80);
        LCD->RAM[LCD_RAMRegister_7] |= (0x02);
        LCD->RAM[LCD_RAMRegister_10] |= (0x20 | 0x40 | 0x80);
        break;

    case 9:
    case '9':
        //LCD->RAM[LCD_RAMRegister_0] &= ~(0);
        //LCD->RAM[LCD_RAMRegister_3] &= ~(0);
        LCD->RAM[LCD_RAMRegister_7] &= ~(0x04 | 0x08);
        LCD->RAM[LCD_RAMRegister_10] &= ~(0x20);
        LCD->RAM[LCD_RAMRegister_0] |= (0x08);
        LCD->RAM[LCD_RAMRegister_3] |= (0x20 | 0x40 | 0x80);
        LCD->RAM[LCD_RAMRegister_7] |= (0x02);
        LCD->RAM[LCD_RAMRegister_10] |= (0x40 | 0x80);
        break;

    default:
        break;
    }
}
*/


void lcd_digit(uint8_t index, char value)
{
    switch (value)
    {
    case 0:
    case '0':
        LCD->RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD->RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD->RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD->RAM[g_segmentD[index][0]] |= g_segmentD[index][1];
        LCD->RAM[g_segmentE[index][0]] |= g_segmentE[index][1];
        LCD->RAM[g_segmentF[index][0]] |= g_segmentF[index][1];
        LCD->RAM[g_segmentG1[index][0]] &= ~g_segmentG1[index][1];
        LCD->RAM[g_segmentG2[index][0]] &= ~g_segmentG2[index][1];
        LCD->RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD->RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 1:
    case '1':
        LCD->RAM[g_segmentA[index][0]] &= ~g_segmentA[index][1];
        LCD->RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD->RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD->RAM[g_segmentD[index][0]] &= ~g_segmentD[index][1];
        LCD->RAM[g_segmentE[index][0]] &= ~g_segmentE[index][1];
        LCD->RAM[g_segmentF[index][0]] &= ~g_segmentF[index][1];
        LCD->RAM[g_segmentG1[index][0]] &= ~g_segmentG1[index][1];
        LCD->RAM[g_segmentG2[index][0]] &= ~g_segmentG2[index][1];
        LCD->RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD->RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 2:
    case '2':
        LCD->RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD->RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD->RAM[g_segmentC[index][0]] &= ~g_segmentC[index][1];
        LCD->RAM[g_segmentD[index][0]] |= g_segmentD[index][1];
        LCD->RAM[g_segmentE[index][0]] |= g_segmentE[index][1];
        LCD->RAM[g_segmentF[index][0]] &= ~g_segmentF[index][1];
        LCD->RAM[g_segmentG1[index][0]] |= g_segmentG1[index][1];
        LCD->RAM[g_segmentG2[index][0]] |= g_segmentG2[index][1];
        LCD->RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD->RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 3:
    case '3':
        LCD->RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD->RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD->RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD->RAM[g_segmentD[index][0]] |= g_segmentD[index][1];
        LCD->RAM[g_segmentE[index][0]] &= ~g_segmentE[index][1];
        LCD->RAM[g_segmentF[index][0]] &= ~g_segmentF[index][1];
        LCD->RAM[g_segmentG1[index][0]] |= g_segmentG1[index][1];
        LCD->RAM[g_segmentG2[index][0]] |= g_segmentG2[index][1];
        LCD->RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD->RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 4:
    case '4':
        LCD->RAM[g_segmentA[index][0]] &= ~g_segmentA[index][1];
        LCD->RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD->RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD->RAM[g_segmentD[index][0]] &= ~g_segmentD[index][1];
        LCD->RAM[g_segmentE[index][0]] &= ~g_segmentE[index][1];
        LCD->RAM[g_segmentF[index][0]] |= g_segmentF[index][1];
        LCD->RAM[g_segmentG1[index][0]] |= g_segmentG1[index][1];
        LCD->RAM[g_segmentG2[index][0]] |= g_segmentG2[index][1];
        LCD->RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD->RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 5:
    case '5':
        LCD->RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD->RAM[g_segmentB[index][0]] &= ~g_segmentB[index][1];
        LCD->RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD->RAM[g_segmentD[index][0]] |= g_segmentD[index][1];
        LCD->RAM[g_segmentE[index][0]] &= ~g_segmentE[index][1];
        LCD->RAM[g_segmentF[index][0]] |= g_segmentF[index][1];
        LCD->RAM[g_segmentG1[index][0]] |= g_segmentG1[index][1];
        LCD->RAM[g_segmentG2[index][0]] |= g_segmentG2[index][1];
        LCD->RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD->RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 6:
    case '6':
        LCD->RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD->RAM[g_segmentB[index][0]] &= ~g_segmentB[index][1];
        LCD->RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD->RAM[g_segmentD[index][0]] |= g_segmentD[index][1];
        LCD->RAM[g_segmentE[index][0]] |= g_segmentE[index][1];
        LCD->RAM[g_segmentF[index][0]] |= g_segmentF[index][1];
        LCD->RAM[g_segmentG1[index][0]] |= g_segmentG1[index][1];
        LCD->RAM[g_segmentG2[index][0]] |= g_segmentG2[index][1];
        LCD->RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD->RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 7:
    case '7':
        LCD->RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD->RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD->RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD->RAM[g_segmentD[index][0]] &= ~g_segmentD[index][1];
        LCD->RAM[g_segmentE[index][0]] &= ~g_segmentE[index][1];
        LCD->RAM[g_segmentF[index][0]] &= ~g_segmentF[index][1];
        LCD->RAM[g_segmentG1[index][0]] &= ~g_segmentG1[index][1];
        LCD->RAM[g_segmentG2[index][0]] &= ~g_segmentG2[index][1];
        LCD->RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD->RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 8:
    case '8':
        LCD->RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD->RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD->RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD->RAM[g_segmentD[index][0]] |= g_segmentD[index][1];
        LCD->RAM[g_segmentE[index][0]] |= g_segmentE[index][1];
        LCD->RAM[g_segmentF[index][0]] |= g_segmentF[index][1];
        LCD->RAM[g_segmentG1[index][0]] |= g_segmentG1[index][1];
        LCD->RAM[g_segmentG2[index][0]] |= g_segmentG2[index][1];
        LCD->RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD->RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    case 9:
    case '9':
        LCD->RAM[g_segmentA[index][0]] |= g_segmentA[index][1];
        LCD->RAM[g_segmentB[index][0]] |= g_segmentB[index][1];
        LCD->RAM[g_segmentC[index][0]] |= g_segmentC[index][1];
        LCD->RAM[g_segmentD[index][0]] |= g_segmentD[index][1];
        LCD->RAM[g_segmentE[index][0]] &= ~g_segmentE[index][1];
        LCD->RAM[g_segmentF[index][0]] |= g_segmentF[index][1];
        LCD->RAM[g_segmentG1[index][0]] |= g_segmentG1[index][1];
        LCD->RAM[g_segmentG2[index][0]] |= g_segmentG2[index][1];
        LCD->RAM[g_segmentT[index][0]] &= ~g_segmentT[index][1];
        LCD->RAM[g_segmentX[index][0]] &= ~g_segmentX[index][1];
        break;

    default:
        break;
    }
}

void lcd_test(void)
{
    /*
    LCD->RAM[LCD_RAMRegister_0] = 0xff;
    LCD->RAM[LCD_RAMRegister_1] = 0xff;
    delay_ms(500);
    //LCD->RAM[LCD_RAMRegister_1] |=  0x80;
    for (uint8_t idx = 0; idx < 22*8; idx++) {
        //LCD->RAM[LCD_RAMRegister_0 + idx / 8] |= (1 << (idx % 8));
        LCD->RAM[LCD_RAMRegister_0] = 0xff;
        delay_ms(500);
        LCD->RAM[LCD_RAMRegister_0] = 0x0;
        delay_ms(500);
    }*/
}

/** @} */
