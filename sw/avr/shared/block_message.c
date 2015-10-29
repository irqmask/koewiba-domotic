/*
 * @brief Functions for handling block-messages.
 * This module contains the functions which handle incoming block mesages.
 *
 * @{
 * @file    blocksend.c
 * @brief   Block-message handling.
 * @author  robert mueller
 */

#include "bus.h"
#include "block_message.h"
#include "crc16.h"
#include "cmddef_common.h"
#include "eeprom_spi.h"
#include "prjtypes.h"
#include "clock.h"

static clock_timer_t	g_transmission_timer;
static block_data_t   	g_current_transmission;

static void send_nak_message (sBus_t* bus, uint16_t receiver, uint8_t parent_cmd)
{
	uint8_t msg[5];

	msg[0] = eCMD_NAK;
	msg[1] = parent_cmd;
	msg[2] = g_current_transmission.additional_info1;
	msg[3] = g_current_transmission.additional_info2;
	bus_send_message(bus, receiver, 4, msg);
}


static BOOL process_eeprom_block_start (block_data_t *bm, uint8_t msglen, uint8_t* msg)
{
    if (10 > msglen) {
    	g_current_transmission.additional_info1 = 2;
    	return FALSE;
    }

    bm->blocktype	= msg[1];
    bm->startaddress    = ((uint32_t)msg[2]<<24) | ((uint32_t)msg[3]<<16) | ((uint32_t)msg[4]<<8) | (uint32_t)msg[5];
    bm->blocksize   	= ((uint32_t)msg[6]<<24) | ((uint32_t)msg[7]<<16) | ((uint32_t)msg[8]<<8) | (uint32_t)msg[9];

	if (bm->startaddress >= EEPROM_SIZE ||
		(EEPROM_SIZE - bm->startaddress) < bm->blocksize ) {
		g_current_transmission.additional_info1 = 3;
		return FALSE;
	}
  	eep_check_statusregister(0);

    return TRUE;
}

static BOOL process_eeprom_block_data (block_data_t *bm, uint8_t msglen, uint8_t* msg)
{
    uint16_t offset, address16;
    uint8_t  len     = 0;

    if (3 > msglen) {
    	g_current_transmission.additional_info1 = 2;
    	g_current_transmission.additional_info2 = msglen;
    	return FALSE;
    }

    offset = (msg[1]<<8) | msg[2];
    address16 = (uint16_t)(bm->startaddress + offset) & 0x0000FFFF;
    len = msglen-3;

    if (len != eep_write(address16, len, &msg[3])) {
    	g_current_transmission.additional_info1 = 3;
    	g_current_transmission.additional_info2 = len;
       return FALSE;
    }
    return TRUE;
}

static BOOL process_eeprom_block_end (block_data_t *bm, uint8_t msglen, uint8_t* msg)
{
    uint16_t crc_eeprom, address_eeprom;
    uint8_t  byte;

    if (3 > msglen) {
    	g_current_transmission.additional_info1 = 2;
    	g_current_transmission.additional_info2 = msglen;
    	return FALSE;
    }

    bm->crc = ((msg[1]<<8) | msg[2]);

	crc_eeprom = crc_16_start();
	for (address_eeprom=0; address_eeprom<bm->blocksize; address_eeprom++) {
		if (1 != eep_read(address_eeprom, 1, &byte)) {
	    	g_current_transmission.additional_info1 = 3;
	    	g_current_transmission.additional_info2 = 0;
	    	return FALSE;
		}
		crc_eeprom = crc_16_next_byte(crc_eeprom, byte);
		if (crc_eeprom != bm->crc) {
	    	g_current_transmission.additional_info1 = 4;
	    	g_current_transmission.additional_info2 = 0;
			return FALSE;
		}
	}
	return TRUE;
}


BOOL block_message_start (sBus_t* bus, uint16_t sender, uint8_t msglen, uint8_t* msg)
{
	BOOL ret = TRUE;

	do {
		if (eSTORAGE_NONE != g_current_transmission.blocktype &&
			sender != 0 && sender != g_current_transmission.sender) {
			g_current_transmission.additional_info1 = 1;
			g_current_transmission.additional_info2 = 0;
			ret = FALSE;
			break;
		}
		g_current_transmission.sender = sender;
		switch (g_current_transmission.blocktype) {
		case eSTORAGE_EEPROM_INT:
			break;
		case eSTORAGE_EEPROM_EXT:
			ret = process_eeprom_block_start(&g_current_transmission, msglen, msg);
			break;
		default:
			break;
		}
	} while (FALSE);
	if (ret == TRUE) {
		clk_timer_start(&g_transmission_timer, CLOCK_MS_2_TICKS(10000));
	} else {
		send_nak_message (bus, sender, eCMD_BLOCK_START);
	}
    return ret;
}

BOOL block_message_data(sBus_t* bus, uint16_t sender, uint8_t msglen, uint8_t* msg)
{
	BOOL ret = TRUE;

	do {
		if (sender != g_current_transmission.sender) {
			g_current_transmission.additional_info1 = 1;
			g_current_transmission.additional_info2 = 0;
			ret = FALSE;
			break;
		}
		switch (g_current_transmission.blocktype) {
		case eSTORAGE_EEPROM_INT:
			break;
		case eSTORAGE_EEPROM_EXT:
			ret = process_eeprom_block_data(&g_current_transmission, msglen, msg);
			break;
		default:
			break;
		}
	} while (FALSE);
    if (ret == TRUE) {
    	clk_timer_start(&g_transmission_timer, CLOCK_MS_2_TICKS(10000));
    } else {
    	send_nak_message (bus, sender, eCMD_BLOCK_DATA);
    }
    return ret;
}

BOOL block_message_end(sBus_t* bus, uint16_t sender, uint8_t msglen, uint8_t* msg)
{
	BOOL ret = TRUE;

	do {
		if (sender != g_current_transmission.sender) {
			g_current_transmission.additional_info1 = 1;
			g_current_transmission.additional_info2 = 0;
			ret = FALSE;
			break;
		}
		switch (g_current_transmission.blocktype) {
		case eSTORAGE_EEPROM_INT:
			break;
		case eSTORAGE_EEPROM_EXT:
			ret = process_eeprom_block_end(&g_current_transmission, msglen, msg);
			break;
		default:
			break;
		}
	} while (FALSE);
    if (ret == TRUE) {
    	clk_timer_start(&g_transmission_timer, CLOCK_MS_2_TICKS(10000));
    } else {
    	send_nak_message (bus, sender, eCMD_BLOCK_END);
    }
    return ret;
}

void block_data_reset(void)
{
    clk_timer_stop(&g_transmission_timer);
    g_current_transmission.blocktype    = eSTORAGE_NONE;
    g_current_transmission.sender       = 0;
    g_current_transmission.startaddress  = 0x00000000;
    g_current_transmission.blocksize    = 0x00000000;
    g_current_transmission.crc          = 0x0000;
    g_current_transmission.received     = 0x00000000;
}

/**
 * Check if timer is active and is elapsed.
 *
 * @param[in] psTimer
 * Pointer to timer structure.
 *
 * @returns TRUE, if timer is active and time is over, otherwise false.
 */
BOOL block_timer_elapsed(void)
{
    return (clk_timer_is_running(&g_transmission_timer) && clk_timer_is_elapsed(&g_transmission_timer));
}
