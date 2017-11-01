/*
 * kwbkouter - A router for koewiba-domotic messages.
 * Copyright (C) 2017  christian <irqmask@gmx.de>
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
 *
 */
/**
 * @addtogroup KWBMQTTGATEWAY
 *
 * @{
 * @file    msg2mqtt.c
 * @brief   Convert koewiba message to mqtt and vice versa.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <stdio.h>
#include <string.h>

// include
#include "cmddef_common.h"
#include "prjtypes.h"

// os/include
#include "error_codes.h"

// os/shared
#include "log.h"
#include "message.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void msg2mqtt_regstate_8bit(msg_t* message, char* topic, size_t max_topic_len, char* msgtext, size_t max_msgtext_len)
{
    snprintf(topic, max_topic_len-1, "kwb/%04X/%04X/state8bit/%02X",
             message->sender, message->receiver, message->data[1]);
    topic[max_topic_len-1] = '\0';
    snprintf(msgtext, max_msgtext_len-1, "%d",
             message->data[2]);
    msgtext[max_msgtext_len-1] = '\0';
}

void msg2mqtt_regstate_16bit(msg_t* message, char* topic, size_t max_topic_len, char* msgtext, size_t max_msgtext_len)
{
    uint16_t value;

    snprintf(topic, max_topic_len-1, "kwb/%04X/%04X/state16bit/%02X",
             message->sender, message->receiver, message->data[1]);
    topic[max_topic_len-1] = '\0';
    value = message->data[2];
    value <<= 8;
    value |= message->data[3];
    snprintf(msgtext, max_msgtext_len-1, "%d", value);
    msgtext[max_msgtext_len-1] = '\0';
}

void msg2mqtt_regstate_32bit(msg_t* message, char* topic, size_t max_topic_len, char* msgtext, size_t max_msgtext_len)
{
    uint32_t value;

    snprintf(topic, max_topic_len-1, "kwb/%04X/%04X/state32bit/%02X",
             message->sender, message->receiver, message->data[1]);
    topic[max_topic_len-1] = '\0';
    value = message->data[2];
    value <<= 8;
    value |= message->data[3];
    value <<= 8;
    value |= message->data[4];
    value <<= 8;
    value |= message->data[5];
    snprintf(msgtext, max_msgtext_len-1, "%d", value);
    msgtext[max_msgtext_len-1] = '\0';
}

void msg2mqtt_request_reg(msg_t* message, char* topic, size_t max_topic_len, char* msgtext, size_t max_msgtext_len)
{
    snprintf(topic, max_topic_len-1, "kwb/%04X/%04X/requestreg",
             message->sender, message->receiver);
    topic[max_topic_len-1] = '\0';
    snprintf(msgtext, max_msgtext_len-1, "%02X",
             message->data[1]);
    msgtext[max_msgtext_len-1] = '\0';
}

void msg2mqtt_set_reg_8bit(msg_t* message, char* topic, size_t max_topic_len, char* msgtext, size_t max_msgtext_len)
{
    snprintf(topic, max_topic_len-1, "kwb/%04X/%04X/setreg8bit/%02X",
             message->sender, message->receiver, message->data[1]);
    topic[max_topic_len-1] = '\0';
    snprintf(msgtext, max_msgtext_len-1, "%d",
             message->data[2]);
    msgtext[max_msgtext_len-1] = '\0';
}

void msg2mqtt_set_reg_16bit(msg_t* message, char* topic, size_t max_topic_len, char* msgtext, size_t max_msgtext_len)
{
    uint16_t value;

    snprintf(topic, max_topic_len-1, "kwb/%04X/%04X/setreg16bit/%02X",
             message->sender, message->receiver, message->data[1]);
    topic[max_topic_len-1] = '\0';
    value = message->data[2];
    value <<= 8;
    value |= message->data[3];
    snprintf(msgtext, max_msgtext_len-1, "%d", value);
    msgtext[max_msgtext_len-1] = '\0';
}

void msg2mqtt_set_reg_32bit(msg_t* message, char* topic, size_t max_topic_len, char* msgtext, size_t max_msgtext_len)
{
    uint32_t value;

    snprintf(topic, max_topic_len-1, "kwb/%04X/%04X/setreg32bit/%02X",
             message->sender, message->receiver, message->data[1]);
    topic[max_topic_len-1] = '\0';
    value = message->data[2];
    value <<= 8;
    value |= message->data[3];
    value <<= 8;
    value |= message->data[4];
    value <<= 8;
    value |= message->data[5];
    snprintf(msgtext, max_msgtext_len-1, "%d", value);
    msgtext[max_msgtext_len-1] = '\0';
}

void msg2mqtt_sleep(msg_t* message, char* topic, size_t max_topic_len, char* msgtext, size_t max_msgtext_len)
{
    snprintf(topic, max_topic_len-1, "kwb/%04X/%04X",
             message->sender, message->receiver);
    topic[max_topic_len-1] = '\0';
    snprintf(msgtext, max_msgtext_len-1, "SLEEP");
    msgtext[max_msgtext_len-1] = '\0';
}

void msg2mqtt_reset(msg_t* message, char* topic, size_t max_topic_len, char* msgtext, size_t max_msgtext_len)
{
    snprintf(topic, max_topic_len-1, "kwb/%04X/%04X",
             message->sender, message->receiver);
    topic[max_topic_len-1] = '\0';
    snprintf(msgtext, max_msgtext_len-1, "RESET");
    msgtext[max_msgtext_len-1] = '\0';
}

int msg2mqtt(msg_t* message, char* topic, size_t max_topic_len, char* msgtext, size_t max_msgtext_len)
{
    int retval = eERR_NONE;

    switch (message->data[0]) {
    case eCMD_STATE_TYPELESS:
    case eCMD_STATE_BITFIELDS:
    case eCMD_STATE_DATE_TIME:
    case eCMD_BLOCK_START:
    case eCMD_BLOCK_DATA:
    case eCMD_BLOCK_END:
    case eCMD_BLOCK_INFO:
    case eCMD_BLOCK_RESET:
    case eCMD_ACK:
    case eCMD_NAK:
        retval = eERR_UNSUPPORTED_MESSAGE;
        log_warning("msg2mqtt() Message %d not supported for MQTT", message->data[0]);
        break;

    case eCMD_STATE_8BIT:
        msg2mqtt_regstate_8bit(message, topic, max_topic_len, msgtext, max_msgtext_len);
        break;
    case eCMD_STATE_16BIT:
        msg2mqtt_regstate_16bit(message, topic, max_topic_len, msgtext, max_msgtext_len);
        break;
    case eCMD_STATE_32BIT:
        msg2mqtt_regstate_32bit(message, topic, max_topic_len, msgtext, max_msgtext_len);
        break;

    case eCMD_REQUEST_REG:
        msg2mqtt_request_reg(message, topic, max_topic_len, msgtext, max_msgtext_len);
        break;
    case eCMD_SET_REG_8BIT:
        msg2mqtt_set_reg_8bit(message, topic, max_topic_len, msgtext, max_msgtext_len);
        break;
    case eCMD_SET_REG_16BIT:
        msg2mqtt_set_reg_16bit(message, topic, max_topic_len, msgtext, max_msgtext_len);
        break;
    case eCMD_SET_REG_32BIT:
        msg2mqtt_set_reg_32bit(message, topic, max_topic_len, msgtext, max_msgtext_len);
        break;

    case eCMD_SLEEP:
        msg2mqtt_sleep(message, topic, max_topic_len, msgtext, max_msgtext_len);
        break;
    case eCMD_RESET:
        msg2mqtt_reset(message, topic, max_topic_len, msgtext, max_msgtext_len);
        break;

    default:
        retval = eERR_UNKNOWN_MESSAGE;
        log_warning("msg2mqtt() Unkown message %d", message->data[0]);
        break;
    }
    return retval;
}

/** @} */
