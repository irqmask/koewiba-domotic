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

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "cmd.h"
#include "motor.h"
#include "STM8L052C6.h"

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
    uint16_t offset;
    uint8_t value;
    if (starts_with("peek")) {
        offset = get_val_u16(&success);
        if (success) {
            value = peek(offset);
            printf("PEEK 0 %04x\n", value);
        }
    }
    else if (starts_with("poke")) {
        offset = get_val_u16(&success);
        value = get_val_u8(&success);
        if (success) {
            poke(offset, value);
            puts("POKE 0");
        }
    }
    else if (starts_with("mh")) {
        if (motor_start_homing() != true) {
            puts("ebusy");
        }
        else {
            puts("MH 0");
        }
    }
    else if (starts_with("ms")) {
        motor_set(false, false);
        puts("MS 0");
    }
    else if (starts_with("mo")) {
        motor_set(true, false);
        puts("MO 0");
    }
    else if (starts_with("mc")) {
        motor_set(true, true);
        puts("MC 0");
    }
    else if (starts_with("mp")) {
        uint16_t pos = get_val_u16(&success);
        if (motor_start_move_pos(pos) != true) {
            puts("ebusy");
        }
        else {
            puts("MP 0");
        }
    }
    else {
        printf("eunknown\n");
    }
    if (!success) {
        printf("eparam\n");
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
