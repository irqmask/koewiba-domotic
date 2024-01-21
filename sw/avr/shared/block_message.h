/*
 * block_message.h
 *
 *  Created on: 03.04.2015
 *      Author: mue
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
// avr supports blocks up to 0x0000FFFF bytes!

#ifndef _BLOCK_MESSAGE_H_
#define _BLOCK_MESSAGE_H_

#include "prjtypes.h"

#include "bus.h"

#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

typedef struct block_data {
    uint8_t  blocktype;
    uint32_t startaddress;
    uint32_t blocksize;
    uint16_t sender;
    uint16_t crc_host;
    uint16_t crc_local;
    uint16_t last_offset;
    uint32_t received;
    uint8_t  additional_info1;
    uint8_t  additional_info2;
} block_data_t;

#if APP_HAS_BLOCK_HANDLER
extern bool app_block_start(uint16_t sender, uint8_t blocktype);
extern bool app_block_end(uint16_t sender, uint8_t blocktype);
#endif


bool block_message_start(sBus_t* bus, uint16_t sender, uint8_t msglen, uint8_t* msg);
bool block_message_data (sBus_t* bus, uint16_t sender, uint8_t msglen, uint8_t* msg);
bool block_message_end  (sBus_t* bus, uint16_t sender, uint8_t msglen, uint8_t* msg);
void block_data_reset   (void);
bool block_timer_elapsed(void);

#endif /* _BLOCK_MESSAGE_H_ */
