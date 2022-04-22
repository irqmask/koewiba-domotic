/**
 * @addtogroup DISPLAY
 * @addtogroup DISPDRV_SH1106
 * @brief Driver for graphical LCD chip SH1106.
 *
 * @{
 * @file    disp_sh1106.c
 * @brief   Driver for graphical LCD chip SH1106.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2019  christian <irqmask@web.de>
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "prjtypes.h"
#include "spi.h"
#include "ucontroller.h"

#include "disp_sh1106.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

static void write_command(uint8_t command)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(command);
    DISP_PORT_SS |= (1<<DISP_SS);
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void sh1106_column_addr (uint8_t column_address)
{
    write_command(0b00010000 | (column_address >> 4));
    write_command(column_address & 0x0F);
}

void sh1106_page_addr (uint8_t page_address)
{
    page_address &= 0b00001111;
    write_command(0b10110000 | page_address);
}

void sh1106_start_line (uint8_t start_line)
{
    start_line &= 0b00011111;

    write_command(0b01000000 | start_line);
}

void sh1106_contrast (uint8_t contrast)
{
    write_command(0b10000001);
    write_command(contrast);
}

void sh1106_adc_select (uint8_t reverse)
{
    if (reverse) {
        write_command(0b10100001);
    } else {
        write_command(0b10100000);
    }
}

void sh1106_display_on (uint8_t on)
{
    if (on) {
        write_command(0b10101111);
    } else {
        write_command(0b10101110);
    }        
}

void sh1106_disp_reverse (uint8_t reverse)
{
    if (reverse) {
        write_command(0b10100111);
    } else {
        write_command(0b10100110);
    }
}

void sh1106_com_scan_direction (uint8_t dir)
{
    if (dir) {
        write_command(0b11001000);
    } else {
        write_command(0b11000000);
    }
}

void sh1106_display_offset (uint8_t offset)
{
    offset &= 0b00111111;
    write_command(0b11010011);
    write_command(offset);
}

void sh1106_nop (void)
{
    write_command(0b11100011);
}

void sh1106_initialize (void)
{
    // set port data directions and initial levels
    DISP_DDR_RES |= (1<<DISP_RES);
    DISP_DDR_A0 |= (1<<DISP_A0);
    DISP_DDR_SS |= (1<<DISP_SS);
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS |= (1<<DISP_SS);

    // reset the display
    DISP_PORT_RES &= ~(1<<DISP_RES);
    _delay_us(10);
    DISP_PORT_RES |= (1<<DISP_RES);
    _delay_us(25);

    sh1106_display_on(0);
    sh1106_disp_reverse(0);
    write_command(0xD5);        // set display clock div
    write_command(0x80);
    write_command(0xA8);        // set multiplex
    write_command(0x3F);
    sh1106_display_offset(0);
    sh1106_start_line(0);   // set start line 0
    write_command(0x8D);        // charge pump
    write_command(0x14);
    write_command(0x20);        // memory mode
    write_command(0x00);
    write_command(0xA1);        // segremap
    write_command(0xC8);
    write_command(0xDA);        // set comp ins
    write_command(0x12);
    sh1106_contrast(0xCF);
    write_command(0xD9);        // set precharge
    write_command(0xF1);
    write_command(0xDB);        // set VCOM detect
    write_command(0x40);
    write_command(0xA4);        // set display all on resume
    sh1106_display_on(1);
}

void sh1106_write_data(uint8_t data)
{
    DISP_PORT_A0 |= (1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(data);
    DISP_PORT_SS |= (1<<DISP_SS);
}
/** @} */
