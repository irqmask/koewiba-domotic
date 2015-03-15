#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "crc16.h"
#include "kwbmonitor.h"
#include "system.h"
#include "systime.h"

bool                g_display_empty_msg = true;
bool                g_display_token_msg = true;

static void current_time_difference (bus_history_t* history, int32_t* diff_ms, int32_t* remaining_us)
{
    int32_t diff_us;

    diff_us = (int32_t)(history->uTimeCurrByte - history->uTimeStart);
    *diff_ms = diff_us / 1000;
    *remaining_us = diff_us - (*diff_ms * 1000);
}

void monitor_init (bus_history_t* history)
{
    history->uTimeStart = sys_time_get_usecs();
    history->uTimeCurrByte = 0;
    history->uTimeLastByte = 0;
    history->uCurrMsgBytes = 0;
}

void monitor_parse_message (uint8_t new_byte, bus_history_t* history)
{
    char        buffer[256], part[256];
    uint16_t    calcedcrc = 0;
    uint8_t     ii, crclen = 0;
    int32_t     diff_ms = 0, remaining_us = 0;
    enum {
        eMsgNothing,
        eMsgError,
        eMsgToken,
        eMsgEmpty,
        eMsgComplete
    } msgstatus;

    msgstatus = eMsgNothing;

    history->uTimeLastByte = history->uTimeCurrByte;
    history->uTimeCurrByte = sys_time_get_usecs();
    if (history->uCurrMsgBytes == 0) {
        current_time_difference(history, &diff_ms, &remaining_us);
        snprintf(buffer, sizeof(buffer)-1, "%9d.%03d | ", diff_ms, remaining_us);
    }
    snprintf(part, sizeof(part-1), "%02X ", new_byte);
    strcat_s(buffer, sizeof(buffer)-1, part);

    // compute status of message
    if (history->uCurrMsgBytes == 0) {
        // check for sync byte
        if (new_byte != 0x9a) {
            msgstatus = eMsgError;
        }
        history->auMessage[history->uCurrMsgBytes] = new_byte;

    } else if (history->uCurrMsgBytes == 1) {
        // check if token or message
        if (new_byte & 0x80) {
            msgstatus = eMsgToken;
        }
        history->uCurrSender = new_byte & 0x7F;
        history->auMessage[history->uCurrMsgBytes] = new_byte;

    } else if (history->uCurrMsgBytes == 2) {
        // check length
        if (new_byte == 0) {
            msgstatus = eMsgEmpty;
        } else if (new_byte > BUS_MAXTOTALMSGLEN) {
            msgstatus = eMsgError;
        } else {
            history->uExpectedLength = new_byte;
        }
        history->auMessage[history->uCurrMsgBytes] = new_byte;

    } else if (history->uCurrMsgBytes == 3) {
        // check address receiver
        if (new_byte & 0x80) {
            msgstatus = eMsgError;
        } else
            history->auMessage[history->uCurrMsgBytes] = new_byte;

    } else if (history->uCurrMsgBytes == 4) {
        // extended address
        history->auMessage[history->uCurrMsgBytes] = new_byte;

    } else if (history->uCurrMsgBytes < 3 + history->uExpectedLength - 2) {
        // message data
        history->auMessage[history->uCurrMsgBytes] = new_byte;

    } else if (history->uCurrMsgBytes == 3 + history->uExpectedLength - 2) {
        // CRC high byte
        history->uExpectedCRC = new_byte << 8;

    } else if (history->uCurrMsgBytes == 3 + history->uExpectedLength - 1) {
        // CRC high byte
        history->uExpectedCRC |= (new_byte & 0xFF);
        crclen = 3 + history->uExpectedLength - 2;
        calcedcrc = crc_calc16(&history->auMessage[0], crclen);
        msgstatus = eMsgComplete;
    }
    history->uCurrMsgBytes++;
    if (history->uCurrMsgBytes >= BUS_MAXTOTALMSGLEN+9) {
        msgstatus = eMsgError;
    }

    // fill line
    if (msgstatus != eMsgNothing) {
        for (ii=0; ii<(BUS_MAXTOTALMSGLEN-history->uCurrMsgBytes); ii++)
            strcat_s(buffer, sizeof(buffer)-1, "   ");
        history->uCurrMsgBytes = 0;
    }

    // print status at end of line
    switch (msgstatus) {
        case eMsgNothing:
            break;
        case eMsgError:
            strcat_s(buffer, sizeof(buffer)-1, "| ERR stray bytes\r\n");
            printf("%s", buffer);
            break;
        case eMsgToken:
            strcat_s(buffer, sizeof(buffer)-1, "| TOKEN\r\n");
            if (g_display_token_msg) printf("%s", buffer);
            break;
        case eMsgEmpty:
            strcat_s(buffer, sizeof(buffer)-1, "| EMPTY MESSAGE\r\n");
            if (g_display_empty_msg) printf("%s", buffer);
            break;
        case eMsgComplete:
            strcat_s(buffer, sizeof(buffer)-1, "| MESSAGE");
            if (history->uExpectedCRC != calcedcrc) {
                snprintf(part, sizeof(part)-1, "  BAD CRC %x %x %d\r\n", history->uExpectedCRC, calcedcrc, crclen);
            } else {
                snprintf(part, sizeof(part)-1, " GOOD CRC %x %x %d\r\n", history->uExpectedCRC, calcedcrc, crclen);
            }
            strcat_s(buffer, sizeof(buffer)-1, part);
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