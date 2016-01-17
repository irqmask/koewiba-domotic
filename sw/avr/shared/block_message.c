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

#include "bus.h"
#include "block_message.h"
#include "clock.h"
#include "cmddef_common.h"
#include "crc16.h"
#include "eeprom_spi.h"
#include "prjtypes.h"

#include "led_debug.h"

static clock_timer_t	g_timer; 	//!< transmission timer to detect timeouts
static block_data_t   	g_bd; 		//!< status data of current_transmission

static void send_ack_message (sBus_t* bus, uint16_t receiver,
		                      uint8_t parent_cmd)
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

static BOOL process_eeprom_block_start (uint8_t msglen, uint8_t* msg)
{
    if (10 > msglen) {
    	g_bd.additional_info1 = 2;
    	return FALSE;
    }

    g_bd.blocktype	    = msg[1];
    g_bd.startaddress   = ((uint32_t)msg[2]<<24) | ((uint32_t)msg[3]<<16) | ((uint32_t)msg[4]<<8) | (uint32_t)msg[5];
    g_bd.blocksize   	= ((uint32_t)msg[6]<<24) | ((uint32_t)msg[7]<<16) | ((uint32_t)msg[8]<<8) | (uint32_t)msg[9];

	if (g_bd.startaddress >= EEPROM_SIZE ||
		(EEPROM_SIZE - g_bd.startaddress) < g_bd.blocksize ) {
		g_bd.additional_info1 = 3;
		return FALSE;
	}
    return TRUE;
}

static BOOL process_eeprom_block_data (uint8_t msglen, uint8_t* msg)
{
    uint16_t address16;
    uint8_t  len     = 0;

    if (3 > msglen) {
    	g_bd.additional_info1 = 2;
    	g_bd.additional_info2 = msglen;
    	return FALSE;
    }

    g_bd.last_offset = (msg[1]<<8) | msg[2];
    address16 = (uint16_t)(MOD_eExtEEPAddr_AppStart + g_bd.startaddress + g_bd.last_offset) & 0x0000FFFF;
    len = msglen-3;
    LED_STATUS_TOGGLE;
    if (len != eep_write(address16, len, &msg[3])) {
        g_bd.additional_info1 = 3;
        g_bd.additional_info2 = len;
        return FALSE;
    }
    if (address16 + len - 1 > g_bd.received) {
        g_bd.received = address16 + len - 1;
    }
    return TRUE;
}

static BOOL process_eeprom_block_end (uint8_t msglen, uint8_t* msg)
{
    uint16_t address_eeprom;
    uint8_t  byte;

    if (3 > msglen) {
    	g_bd.additional_info1 = 2;
    	g_bd.additional_info2 = msglen;
    	return FALSE;
    }

    g_bd.crc_host = ((msg[1]<<8) | msg[2]);

    g_bd.crc_local = crc_16_start();
	for (address_eeprom = MOD_eExtEEPAddr_AppStart;
         address_eeprom<g_bd.blocksize + MOD_eExtEEPAddr_AppStart;
         address_eeprom++) {
		if (1 != eep_read(address_eeprom, 1, &byte)) {
	    	g_bd.additional_info1 = 3;
	    	g_bd.additional_info2 = 0;
	    	return FALSE;
		}
		g_bd.crc_local = crc_16_next_byte(g_bd.crc_local, byte);
	}
	if (g_bd.crc_local != g_bd.crc_host) {
    	g_bd.additional_info1 = 4;
    	g_bd.additional_info2 = 0;
		return FALSE;
	}
	// save length and crc in external EEPROM
	if (MOD_LEN_APPSIZE != eep_write(MOD_eExtEEPAddr_AppSize, MOD_LEN_APPSIZE, (uint8_t*)&g_bd.blocksize)) {
		g_bd.additional_info1 = 5;
		return FALSE;
	}
	if (MOD_LEN_APPCRC != eep_write(MOD_eExtEEPAddr_AppCrc, MOD_LEN_APPCRC, (uint8_t*)&g_bd.crc_host)) {
		g_bd.additional_info1 = 6;
		return FALSE;
	}
	return TRUE;
}


BOOL block_message_start (sBus_t* bus, uint16_t sender, uint8_t msglen, uint8_t* msg)
{
	BOOL ret = TRUE;

	do {
		g_bd.blocktype = msg[1];
		g_bd.sender = sender;
		g_bd.received = 0;
		switch (g_bd.blocktype) {
		case eSTORAGE_EEPROM_INT:
			break;
		case eSTORAGE_EEPROM_EXT:
			ret = process_eeprom_block_start(msglen, msg);
			break;
		default:
			break;
		}
	} while (FALSE);
	if (ret == TRUE) {
		clk_timer_start(&g_timer, CLOCK_MS_2_TICKS(10000));
	} else {
		send_nak_message(bus, sender, eCMD_BLOCK_START);
	}
    return ret;
}

BOOL block_message_data (sBus_t* bus, uint16_t sender, uint8_t msglen, uint8_t* msg)
{
	BOOL ret = TRUE;

	do {
		if (sender != g_bd.sender) {
			g_bd.additional_info1 = 1;
			g_bd.additional_info2 = 0;
			ret = FALSE;
			break;
		}
		switch (g_bd.blocktype) {
		case eSTORAGE_EEPROM_INT:
			break;
		case eSTORAGE_EEPROM_EXT:
			ret = process_eeprom_block_data(msglen, msg);
			break;
		default:
			break;
		}
	} while (FALSE);
    if (ret == TRUE) {
    	clk_timer_start(&g_timer, CLOCK_MS_2_TICKS(10000));
    	send_ack_message(bus, sender, eCMD_BLOCK_DATA);
    } else {
    	send_nak_message(bus, sender, eCMD_BLOCK_DATA);
    }
    return ret;
}

BOOL block_message_end (sBus_t* bus, uint16_t sender, uint8_t msglen, uint8_t* msg)
{
	BOOL ret = TRUE;

	do {
		if (sender != g_bd.sender) {
			g_bd.additional_info1 = 1;
			g_bd.additional_info2 = 0;
			ret = FALSE;
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
	} while (FALSE);
    if (ret != TRUE) {
    	send_nak_message (bus, sender, eCMD_BLOCK_END);
    }
	send_block_info_message(bus, sender,
							g_bd.crc_host,
							g_bd.crc_local,
							g_bd.received);
    return ret;
}

void block_data_reset (void)
{
    clk_timer_stop(&g_timer);
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
 * @returns TRUE, if timer is active and time is over, otherwise false.
 */
BOOL block_timer_elapsed (void)
{
    return (clk_timer_is_running(&g_timer) && clk_timer_is_elapsed(&g_timer));
}
