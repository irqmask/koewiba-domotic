#include <stdio.h>
#include <string.h>

#include "crc16.h"
#include "kwbmonitor.h"
#include "prjtypes.h"
#include "system.h"
#include "sysconsole.h"
#include "systime.h"

bool g_display_empty_msg = true;
bool g_display_token_msg = true;

static void current_time_difference (bus_history_t* history, uint32_t* diff_ms, uint32_t* remaining_us)
{
    sys_time_t diff_us;

    diff_us = (history->time_curr_byte - history->time_start);
    *diff_ms = diff_us / 1000;
    *remaining_us = diff_us - (*diff_ms * 1000);
}

void monitor_init (bus_history_t* history)
{
    history->time_start = sys_time_get_usecs();
    history->time_curr_byte = 0;
    history->time_last_byte = 0;
    history->current_msg_bytes = 0;
    history->last_message_status = eMSG_NOTHING;
}

void monitor_parse_message (uint8_t new_byte, bus_history_t* history)
{
    char        buffer[1024], part[256];
    uint16_t    calcedcrc = 0;
    uint8_t     ii, crclen = 0;
    uint32_t    diff_ms = 0, remaining_us = 0;

    msg_status_t msgstatus = eMSG_NOTHING;

    history->time_last_byte = history->time_curr_byte;
    history->time_curr_byte = sys_time_get_usecs();
    if (history->current_msg_bytes == 0) {
        current_time_difference(history, &diff_ms, &remaining_us);
        snprintf(buffer, sizeof(buffer)-1, "%9d.%03d | ", diff_ms, remaining_us);
    }
    snprintf(part, sizeof(part-1), "%02X ", new_byte);
    strcat_s(buffer, sizeof(buffer)-1, part);

    // compute status of message
    if (history->current_msg_bytes == 0) {
        // check for ACK byte
        if (history->last_message_status == eMSG_COMPLETE && new_byte == 0x06) {
            msgstatus = eMSG_ACK;
        }
        // check for sync byte
        else if (new_byte != 0x9a) {
            msgstatus = eMSG_ERROR;
        }
        history->message[history->current_msg_bytes] = new_byte;

    } else if (history->current_msg_bytes == 1) {
        // check if token or message
        if (new_byte & 0x80) {
            msgstatus = eMSG_TOKEN;
        }
        history->current_sender = new_byte & 0x7F;
        history->message[history->current_msg_bytes] = new_byte;

    } else if (history->current_msg_bytes == 2) {
        // check length
        if (new_byte == 0) {
            msgstatus = eMSG_EMPTY;
        } else if (new_byte > BUS_MAXTOTALMSGLEN) {
            msgstatus = eMSG_ERROR;
        } else {
            history->expected_length = new_byte;
        }
        history->message[history->current_msg_bytes] = new_byte;

    } else if (history->current_msg_bytes == 3) {
        // check address receiver
        if (new_byte & 0x80) {
            msgstatus = eMSG_ERROR;
        } else
            history->message[history->current_msg_bytes] = new_byte;

    } else if (history->current_msg_bytes == 4) {
        // extended address
        history->message[history->current_msg_bytes] = new_byte;

    } else if (history->current_msg_bytes < 3 + history->expected_length - 2) {
        // message data
        history->message[history->current_msg_bytes] = new_byte;

    } else if (history->current_msg_bytes == 3 + history->expected_length - 2) {
        // CRC high byte
        history->expected_CRC = new_byte << 8;

    } else if (history->current_msg_bytes == 3 + history->expected_length - 1) {
        // CRC high byte
        history->expected_CRC |= (new_byte & 0xFF);
        crclen = 3 + history->expected_length - 2;
        calcedcrc = crc_calc16(&history->message[0], crclen);
        msgstatus = eMSG_COMPLETE;
    }
    history->current_msg_bytes++;
    if (history->current_msg_bytes >= BUS_MAXTOTALMSGLEN+9) {
        msgstatus = eMSG_ERROR;
    }

    // fill line
    if (msgstatus != eMSG_NOTHING) {
        for (ii=0; ii<(MAX_DISPLAYLEN-history->current_msg_bytes); ii++)
            strcat_s(buffer, sizeof(buffer)-1, "   ");
        history->current_msg_bytes = 0;
        history->last_message_status = msgstatus;
    }

    // print status at end of line
    switch (msgstatus) {
        case eMSG_NOTHING:
            break;
        case eMSG_ERROR:
            strcat_s(buffer, sizeof(buffer)-1, "| ERR stray bytes\r\n");
            printf("%s", buffer);
            break;

        case eMSG_TOKEN:
            strcat_s(buffer, sizeof(buffer)-1, "| TOKEN\r\n");
            if (g_display_token_msg) printf("%s", buffer);
            break;

        case eMSG_EMPTY:
            strcat_s(buffer, sizeof(buffer)-1, "| EMPTY MESSAGE\r\n");
            if (g_display_empty_msg) printf("%s", buffer);
            break;

        case eMSG_COMPLETE:
            strcat_s(buffer, sizeof(buffer)-1, "| MESSAGE");
            if (history->expected_CRC != calcedcrc) {
                snprintf(part, sizeof(part)-1, "  BAD CRC %x %x %d\r\n", history->expected_CRC, calcedcrc, crclen);
            } else {
                snprintf(part, sizeof(part)-1, " GOOD CRC %x %x %d\r\n", history->expected_CRC, calcedcrc, crclen);
            }
            strcat_s(buffer, sizeof(buffer)-1, part);
            printf("%s", buffer);
            break;

        case eMSG_ACK:
            strcat_s(buffer, sizeof(buffer)-1, "| ACK\r\n");
            printf("%s", buffer);
            break;

        default:
            break;
    }
}

void monitor_toggle_display_empty_message (void)
{
    if (g_display_empty_msg) {
        g_display_empty_msg = false;
    } else {
        g_display_empty_msg = true;
    }
}

void monitor_toggle_display_token_message (void)
{
    if (g_display_token_msg) {
        g_display_token_msg = false;
    } else {
        g_display_token_msg = true;
    }
}
