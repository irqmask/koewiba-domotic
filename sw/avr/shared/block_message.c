/*
 * @brief Functions for handling block-messages.
 * This module contains the functions which handle incoming block mesages.
 *
 * @{
 * @file    blocksend.c
 * @brief   Block-message handling.
 * @author  Robert Mueller
 * @author  Christian Verhalen
 */
/*
 * Copyright (C) 2023
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
#include "block_message.h"

#include "bus.h"
#include "cmddef_common.h"
#include "crc16.h"
#include "eeprom_spi.h"
#include "moddef_common.h"
#include "prjtypes.h"
#include "register.h"
#include "timer.h"

static timer_data_t     g_timer;    //!< transmission timer to detect timeouts
static block_data_t     g_bd;       //!< status data of current_transmission

static void send_ack_message (sBus_t* bus, uint16_t receiver, uint8_t parent_cmd)
{
    uint8_t msg[5];

    msg[0] = eCMD_ACK;
    msg[1] = parent_cmd;
    msg[2] = g_bd.last_offset >> 8;
    msg[3] = g_bd.last_offset & 0x00FF;
    bus_send_message(bus, receiver, 4, msg);
}

static void send_nak_message (sBus_t* bus, uint16_t receiver, uint8_t parent_cmd)
{
    uint8_t msg[5];

    msg[0] = eCMD_NAK;
    msg[1] = parent_cmd;
    msg[2] = g_bd.additional_info1;
    msg[3] = g_bd.additional_info2;
    bus_send_message(bus, receiver, 4, msg);
}

static void send_block_info_message (sBus_t* bus, uint16_t receiver, uint16_t crc_host, uint16_t crc_local, uint32_t length)
{
    uint8_t msg[10];

    msg[0] = eCMD_BLOCK_INFO;
    msg[1] = crc_host >> 8;
    msg[2] = crc_host & 0x00FF;
    msg[3] = crc_local >> 8;
    msg[4] = crc_local & 0x00FF;
    msg[5] = (uint8_t)((length >> 24) & 0x000000FF);
    msg[6] = (uint8_t)((length >> 16) & 0x000000FF);
    msg[7] = (uint8_t)((length >> 8) & 0x000000FF);
    msg[8] = (uint8_t)(length & 0x000000FF);

    bus_send_message(bus, receiver, 9, msg);
}

static bool process_eeprom_block_start (uint8_t msglen, uint8_t* msg)
{
    if (10 > msglen) {
        g_bd.additional_info1 = 2;
        return false;
    }

    g_bd.blocktype	    = msg[1];
    g_bd.startaddress   = ((uint32_t)msg[2]<<24) | ((uint32_t)msg[3]<<16) | ((uint32_t)msg[4]<<8) | (uint32_t)msg[5];
    g_bd.blocksize   	= ((uint32_t)msg[6]<<24) | ((uint32_t)msg[7]<<16) | ((uint32_t)msg[8]<<8) | (uint32_t)msg[9];

    // check if data block fits in external EEProm
    if (g_bd.startaddress >= EEPROM_SIZE ||
            (EEPROM_SIZE - g_bd.startaddress) < g_bd.blocksize ) {
        g_bd.additional_info1 = 3;
        return false;
    }
    return true;
}

static bool process_eeprom_block_data (uint8_t msglen, uint8_t* msg)
{
    uint16_t address16;
    uint8_t  len     = 0;

    // message has to contain at least command ID, and 2 bytes offset
    if (3 > msglen) {
        g_bd.additional_info1 = 2;
        g_bd.additional_info2 = msglen;
        return false;
    }

    // get current offset from message and
    // calculate current offset in external EEProm
    g_bd.last_offset = (msg[1]<<8) | msg[2];
    address16 = (uint16_t)(MOD_eExtEEPAddr_AppStart + g_bd.startaddress + g_bd.last_offset) & 0x0000FFFF;

    // length of data if message length without command ID and 2 bytes offset
    len = msglen-3;

    // copy data into external EEProm
    if (len != eep_write(address16, len, &msg[3])) {
        g_bd.additional_info1 = 3;
        g_bd.additional_info2 = len;
        return false;
    }

    // save highest address of written byte
    if (address16 + len - 1 > g_bd.received) {
        g_bd.received = address16 + len - 1;
    }
    return true;
}

static bool process_eeprom_block_end (uint8_t msglen, uint8_t* msg)
{
    uint16_t address_eeprom, size;
    uint8_t  byte;

    // message has to contain at least command ID and CRC calculated by host-computer
    if (3 > msglen) {
        g_bd.additional_info1 = 2;
        g_bd.additional_info2 = msglen;
        return false;
    }

    g_bd.crc_host = ((msg[1]<<8) | msg[2]);

    // calculate CRC local
    g_bd.crc_local = CRC_START_VALUE;
    for (address_eeprom = MOD_eExtEEPAddr_AppStart;
         address_eeprom<g_bd.blocksize + MOD_eExtEEPAddr_AppStart;
         address_eeprom++) {
        if (1 != eep_read(address_eeprom, 1, &byte)) {
            g_bd.additional_info1 = 3;
            g_bd.additional_info2 = 0;
            return false;
        }
        g_bd.crc_local = crc_16_next_byte(g_bd.crc_local, byte);
    }
    if (g_bd.crc_local != g_bd.crc_host) {
        g_bd.additional_info1 = 4;
        g_bd.additional_info2 = 0;
        return false;
    }
    // save length and crc in external EEPROM
    size = (uint16_t)g_bd.blocksize;
    if (MOD_LEN_APPSIZE != eep_write(MOD_eExtEEPAddr_AppSize, MOD_LEN_APPSIZE, (uint8_t*)&size)) {
        g_bd.additional_info1 = 5;
        return false;
    }
    if (MOD_LEN_APPCRC != eep_write(MOD_eExtEEPAddr_AppCrc, MOD_LEN_APPCRC, (uint8_t*)&g_bd.crc_host)) {
        g_bd.additional_info1 = 6;
        return false;
    }
    // set flag that controller is reprogrammed after next reset
    register_set_u8(MOD_eReg_BldFlag, (1<<eBldFlagAppProgram));

    return true;
}


bool block_message_start (sBus_t* bus, uint16_t sender, uint8_t msglen, uint8_t* msg)
{
    bool ret = false;

    do {
        g_bd.blocktype = msg[1];
        g_bd.sender = sender;
        g_bd.received = 0;
        switch (g_bd.blocktype) {
        case eSTORAGE_EEPROM_EXT:
            ret = process_eeprom_block_start(msglen, msg);
            break;
        default:
            break;
        }
        if (ret != true) break;

#if APP_HAS_BLOCK_HANDLER
        ret = app_block_start(g_bd.sender, g_bd.blocktype);
#endif
    } while (false);
    if (ret == true) {
        timer_start(&g_timer, TIMER_MS_2_TICKS(10000));
        send_ack_message(bus, sender, eCMD_BLOCK_START);
    } else {
        send_nak_message(bus, sender, eCMD_BLOCK_START);
    }
    return ret;
}

bool block_message_data (sBus_t* bus, uint16_t sender, uint8_t msglen, uint8_t* msg)
{
    bool ret = false;

    do {
        if (sender != g_bd.sender) {
            g_bd.additional_info1 = 1;
            g_bd.additional_info2 = 0;
            break;
        }
        switch (g_bd.blocktype) {
        case eSTORAGE_EEPROM_EXT:
            ret = process_eeprom_block_data(msglen, msg);
            break;
        default:
            break;
        }
    } while (false);
    if (ret == true) {
        timer_start(&g_timer, TIMER_MS_2_TICKS(10000));
        send_ack_message(bus, sender, eCMD_BLOCK_DATA);
    } else {
        send_nak_message(bus, sender, eCMD_BLOCK_DATA);
    }
    return ret;
}

bool block_message_end (sBus_t* bus, uint16_t sender, uint8_t msglen, uint8_t* msg)
{
    bool ret = true;

    do {
        if (sender != g_bd.sender) {
            g_bd.additional_info1 = 1;
            g_bd.additional_info2 = 0;
            ret = false;
            break;
        }
        switch (g_bd.blocktype) {
        case eSTORAGE_EEPROM_INT:
            break;
        case eSTORAGE_EEPROM_EXT:
            ret = process_eeprom_block_end(msglen, msg);
            break;
        default:
            break;
        }
        if (ret != true) break;
#if APP_HAS_BLOCK_HANDLER
        ret = app_block_end(g_bd.sender, g_bd.blocktype);
#endif
    } while (false);
    send_block_info_message(bus, sender,
            g_bd.crc_host,
            g_bd.crc_local,
            g_bd.received);
    if (ret != true) {
        send_nak_message (bus, sender, eCMD_BLOCK_END);
    }

    return ret;
}

void block_data_reset (void)
{
    timer_stop(&g_timer);
    g_bd.blocktype    = eSTORAGE_NONE;
    g_bd.sender       = 0;
    g_bd.startaddress = 0x00000000;
    g_bd.blocksize    = 0x00000000;
    g_bd.crc_local    = 0x0000;
    g_bd.crc_host     = 0x0000;
    g_bd.received     = 0x00000000;
}

/**
 * Check if timer is active and is elapsed.
 *
 * @param[in] psTimer
 * Pointer to timer structure.
 *
 * @returns true, if timer is active and time is over, otherwise false.
 */
bool block_timer_elapsed (void)
{
    return (timer_is_running(&g_timer) && timer_is_elapsed(&g_timer));
}
