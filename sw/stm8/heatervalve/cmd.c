/**
 * @addtogroup CMD
 *
 * @{
 * @file    cmd.c
 * @brief   A simple command interpreter.
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

#include "cmd.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "control_temp.h"
#include "debug.h"
#include "motor.h"
#include "lcd.h"
#include "remote_tempsense.h"
#include "uart.h"

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

uint8_t g_bufidx;
uint8_t g_readidx;
char g_cmdbuf[32];

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

uint8_t peek(uint16_t offset)
{
    uint8_t value;

    value = (*(volatile uint8_t *)offset);
    return value;
}

void poke(uint16_t offset, uint8_t value)
{
    (*(volatile uint8_t *)offset) = value;
}

static bool starts_with(const char* cmd)
{
    uint8_t idx = 0;

    while (g_cmdbuf[idx] != '\0' && cmd[idx] != '\0') {

        if (g_cmdbuf[idx] != cmd[idx]) {
            return false;
        }
        idx++;
    }
    if (cmd[idx] == '\0') {
        g_readidx = idx;
        return true;
    }
    return false;
}

static uint8_t get_nibble(bool *psuccess)
{
    uint8_t nibble = 0;
    if (g_cmdbuf[g_readidx] >= '0' && g_cmdbuf[g_readidx] <= '9') {
        nibble = g_cmdbuf[g_readidx++] - '0';
    }
    else if (g_cmdbuf[g_readidx] >= 'a' && g_cmdbuf[g_readidx] <= 'f') {
        nibble = 10 + g_cmdbuf[g_readidx++] - 'a';
    }
    else if (g_cmdbuf[g_readidx] >= 'A' && g_cmdbuf[g_readidx] <= 'F') {
        nibble = 10 + g_cmdbuf[g_readidx++] - 'A';
    }
    else {
        *psuccess = false;
    }
    return nibble;
}

static uint8_t get_val_u8(bool *psuccess)
{
    uint8_t value = 0;
    while (g_cmdbuf[g_readidx] == ' ' && g_cmdbuf[g_readidx] != '\0') {
        g_readidx++;
    }
    value = get_nibble(psuccess);
    value <<= 4;
    value |= get_nibble(psuccess);
    return value;
}

static uint16_t get_val_u16(bool *psuccess)
{
    uint16_t value = 0;
    while (g_cmdbuf[g_readidx] == ' ' && g_cmdbuf[g_readidx] != '\0') {
        g_readidx++;
    }
    value = get_nibble(psuccess);
    value <<= 4;
    value |= get_nibble(psuccess);
    value <<= 4;
    value |= get_nibble(psuccess);
    value <<= 4;
    value |= get_nibble(psuccess);
    return value;
}

static void interpret_command(void)
{
    bool success = true;

    if (starts_with("peek")) {
        uint16_t offset = get_val_u16(&success);
        uint8_t val8;
        if (success) {
            val8 = peek(offset);
            printf("PEEK 0 %04x\n", val8);
        }
    }
    else if (starts_with("poke")) {
        uint16_t offset = get_val_u16(&success);
        uint8_t val8 = get_val_u8(&success);
        if (success) {
            poke(offset, val8);
            puts("POKE 0");
        }
    }
    else if (starts_with("dv")) { // display value
        uint16_t val16 = get_val_u16(&success);
        uint8_t dp = get_val_u8(&success);
        if (success) {
            lcd_disp_value(val16, dp);
        }
    }
    else if (starts_with("mh")) { // motor home
        if (motor_start_homing() != true) {
            puts("ebusy");
        }
        else {
            puts("MH 0");
        }
    }
    else if (starts_with("ms")) { // motor stop
        motor_set(false, false);
        puts("MS 0");
    }
    else if (starts_with("mo")) { // motor move towards opening
        motor_set(true, false);
        puts("MO 0");
    }
    else if (starts_with("mc")) { // motor move towards closing
        motor_set(true, true);
        puts("MC 0");
    }
    else if (starts_with("mp")) { // motor position
        uint16_t pos = get_val_u16(&success);
        if (motor_start_move_pos(pos) != true) {
            puts("ebusy");
        }
        else {
            puts("MP 0");
        }
    }
    else if (starts_with("srt")) { // set remote temperatures <current> <desired>
        uint16_t current_temp = get_val_u16(&success);
        uint16_t desired_temp = get_val_u16(&success);
        if (success) {
            remts_set_current_temp(current_temp);
            remts_set_desired_temp(desired_temp);
            puts("SRT 0");
        }
    }
    else if (starts_with("skp")) { // set remote temperatures <current> <desired>
        uint16_t tmp = get_val_u16(&success);
        if (success) {
            ctrl_set_kp(tmp);
            puts("SKP 0");
        }
    }
    if (!success) {
        uart_write("eparam\n");
        uart_write("# ");
        uart_write(g_cmdbuf);
        uart_write("\n");
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Initialize command interpreter.
 */
void cmd_initialize(void)
{
    g_bufidx = 0;
    g_readidx = 0;

}


void cmd_process_char(char c)
{
    if (c == '\n') {
        g_cmdbuf[g_bufidx] = '\0';
        g_readidx = 0;
        g_bufidx = 0;
        interpret_command();
        return;
    }
    if (g_bufidx >= sizeof(g_cmdbuf)) {
        return;
    }
    g_cmdbuf[g_bufidx] = c;
    g_bufidx++;
}

/** @} */
