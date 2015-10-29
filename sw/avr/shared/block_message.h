/*
 * block_message.h
 *
 *  Created on: 03.04.2015
 *      Author: mue
 */

// avr supports blocks up to 0x0000FFFF bytes!

#ifndef _BLOCK_MESSAGE_H_
#define _BLOCK_MESSAGE_H_

#include "prjtypes.h"

#include "bus.h"

typedef struct block_data {
    uint8_t  blocktype;
    uint32_t startaddress;
    uint32_t blocksize;
    uint16_t sender;
    uint16_t crc;
    uint32_t received;
    uint8_t  additional_info1;
    uint8_t  additional_info2;
} block_data_t;


BOOL block_message_start(sBus_t* bus, uint16_t sender, uint8_t msglen, uint8_t* msg);
BOOL block_message_data (sBus_t* bus, uint16_t sender, uint8_t msglen, uint8_t* msg);
BOOL block_message_end  (sBus_t* bus, uint16_t sender, uint8_t msglen, uint8_t* msg);
void block_data_reset   (void);
BOOL block_timer_elapsed(void);
#endif /* _BLOCK_MESSAGE_H_ */
