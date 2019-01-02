/**
 * @addtogroup KWBMQTTGATEWAY
 *
 * @{
 * @file    mqtt2msg.c
 * @brief   Convert koewiba message to mqtt and vice versa.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
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
 */

// --- Include section ---------------------------------------------------------

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mosquitto.h"
#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
#include <safe_lib.h>
#endif

// include
#include "cmddef_common.h"
#include "prjtypes.h"

// libsystem
#include "sysstring.h"

// os/include
#include "error_codes.h"

// os/shared
#include "log.h"
#include "message.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef int (*mqtt_msg_converter_func)(char* topic, char* msgtext, msg_t* message);

typedef struct _mqtt_message_types {
    const char* msgtype;
    const char* topic;
    mqtt_msg_converter_func func;
} mqtt_msg_types_t;

// --- Local variables ---------------------------------------------------------

int mqtt2msg_state_8bit(char* topic, char* msgtext, msg_t* message);
int mqtt2msg_state_16bit(char* topic, char* msgtext, msg_t* message);
int mqtt2msg_state_32bit(char* topic, char* msgtext, msg_t* message);
int mqtt2msg_request_reg(char* topic, char* msgtext, msg_t* message);
int mqtt2msg_setreg_8bit(char* topic, char* msgtext, msg_t* message);
int mqtt2msg_setreg_16bit(char* topic, char* msgtext, msg_t* message);
int mqtt2msg_setreg_32bit(char* topic, char* msgtext, msg_t* message);

mqtt_msg_types_t g_mqtt_msg_types[] = {
    "state8bit", "kwb/+/+/state8bit/#", mqtt2msg_state_8bit,
    "state16bit", "kwb/+/+/state16bit/#", mqtt2msg_state_16bit,
    "state32bit", "kwb/+/+/state32bit/#", mqtt2msg_state_32bit,
    "requestreg", "kwb/+/+/requestreg", mqtt2msg_request_reg,
    "setreg8bit", "kwb/+/+/setreg8bit/#", mqtt2msg_setreg_8bit,
    "setreg16bit", "kwb/+/+/setreg16bit/#", mqtt2msg_setreg_16bit,
    "setreg32bit", "kwb/+/+/setreg32bit/#", mqtt2msg_setreg_32bit,
    NULL, NULL
};

// --- Module global variables -------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Local functions ---------------------------------------------------------

int mqtt_split_topic(char* topic,
                     uint16_t* sender, uint16_t* receiver,
                     char** msgtype,
                     char** remaining_topic)
{
    int retval = eERR_UNKNOWN_MESSAGE;
    char *slash_pos = topic;
    char *senderstr, *receiverstr;

    do {
        slash_pos = strstr(topic, "/");
        if (slash_pos == NULL) break;
        //! todo use safe string functions correctly! e.g. strlen(topic) = :-(
        //if (strstr_s(topic, strlen(topic), "/", 1, &slash_pos) != 0) break;
        *slash_pos = '\0'; slash_pos++; if (*slash_pos == '\0') break;
        senderstr = slash_pos;
        topic = slash_pos;
        //if (strstr_s(topic, strlen(topic), "/", 1, &slash_pos) != 0) break;
        slash_pos = strstr(topic, "/");
        if (slash_pos == NULL) break;
        *slash_pos = '\0'; slash_pos++; if (*slash_pos == '\0') break;
        receiverstr = slash_pos;
        topic = slash_pos;
        //if (strstr_s(topic, strlen(topic), "/", 1, &slash_pos) != 0) break;
        slash_pos = strstr(topic, "/");
        if (slash_pos == NULL) break;
        *slash_pos = '\0'; slash_pos++; if (*slash_pos == '\0') break;
        *msgtype = slash_pos;
        topic = slash_pos;
        //if (strstr_s(topic, strlen(topic), "/", 1, &slash_pos) == 0) {
        slash_pos = strstr(topic, "/");
        if (slash_pos == NULL) {
            *remaining_topic = NULL;
        } else {
            *slash_pos = '\0'; slash_pos++;
            *remaining_topic = slash_pos;
        }

        *sender = (uint16_t)strtoul(senderstr, NULL, 16);
        *receiver = (uint16_t)strtoul(receiverstr, NULL, 16);

        retval = eERR_NONE;
    } while (0);
    return retval;
}

int mqtt2msg_state_8bit(char* topic, char* msgtext, msg_t* message)
{
    int retval = eERR_NONE;
    uint32_t value;

    do {
        message->length = 3;
        message->data[0] = eCMD_STATE_8BIT;

        value = strtoul(topic, NULL, 16);
        if (value > UCHAR_MAX || errno != 0) {
            retval = eERR_UNKNOWN_MESSAGE;
            break;
        }
        message->data[1] = value;

        value = strtoul(msgtext, NULL, 10);
        if (value > UCHAR_MAX || errno != 0) {
            retval = eERR_UNKNOWN_MESSAGE;
            break;
        }
        message->data[2] = value;
    } while (0);
    return retval;
}

int mqtt2msg_state_16bit(char* topic, char* msgtext, msg_t* message)
{
    int retval = eERR_NONE;
    uint32_t value;

    do {
        message->length = 4;
        message->data[0] = eCMD_STATE_16BIT;

        value = strtoul(topic, NULL, 16);
        if (value > UCHAR_MAX || errno != 0) {
            retval = eERR_UNKNOWN_MESSAGE;
            break;
        }
        message->data[1] = value;

        value = strtoul(msgtext, NULL, 10);
        if (value > USHRT_MAX || errno != 0) {
            retval = eERR_UNKNOWN_MESSAGE;
            break;
        }
        message->data[2] = (value >> 8) & 0x000000FF;
        message->data[3] = value & 0x000000FF;
    } while (0);
    return retval;
}

int mqtt2msg_state_32bit(char* topic, char* msgtext, msg_t* message)
{
    int retval = eERR_NONE;
    uint32_t value;

    do {
        message->length = 6;
        message->data[0] = eCMD_STATE_16BIT;

        value = strtoul(topic, NULL, 16);
        if (value > UCHAR_MAX || errno != 0) {
            retval = eERR_UNKNOWN_MESSAGE;
            break;
        }
        message->data[1] = value;

        value = strtoul(msgtext, NULL, 10);
        if (value > UINT_MAX || errno != 0) {
            retval = eERR_UNKNOWN_MESSAGE;
            break;
        }
        message->data[2] = (value >> 24) & 0x000000FF;
        message->data[3] = (value >> 16) & 0x000000FF;
        message->data[4] = (value >> 8) & 0x000000FF;
        message->data[5] = value & 0x000000FF;
    } while (0);
    return retval;
}

int mqtt2msg_request_reg(char* topic, char* msgtext, msg_t* message)
{
    int retval = eERR_NONE;
    uint32_t value;

    do {
        message->length = 2;
        message->data[0] = eCMD_REQUEST_REG;

        value = strtoul(msgtext, NULL, 16);
        if (value > UCHAR_MAX || errno != 0) {
            retval = eERR_UNKNOWN_MESSAGE;
            break;
        }
        message->data[1] = value;
    } while (0);
    return retval;
}

int mqtt2msg_setreg_8bit(char* topic, char* msgtext, msg_t* message)
{
    int retval = eERR_NONE;
    uint32_t value;

    do {
        message->length = 3;
        message->data[0] = eCMD_SET_REG_8BIT;

        value = strtoul(topic, NULL, 16);
        if (value > UCHAR_MAX || errno != 0) {
            retval = eERR_UNKNOWN_MESSAGE;
            break;
        }
        message->data[1] = value;

        value = strtoul(msgtext, NULL, 10);
        if (value > UCHAR_MAX || errno != 0) {
            retval = eERR_UNKNOWN_MESSAGE;
            break;
        }
        message->data[2] = value;
    } while (0);
    return retval;
}

int mqtt2msg_setreg_16bit(char* topic, char* msgtext, msg_t* message)
{
    int retval = eERR_NONE;
    uint32_t value;

    do {
        message->length = 4;
        message->data[0] = eCMD_SET_REG_16BIT;

        value = strtoul(topic, NULL, 16);
        if (value > UCHAR_MAX || errno != 0) {
            retval = eERR_UNKNOWN_MESSAGE;
            break;
        }
        message->data[1] = value;

        value = strtoul(msgtext, NULL, 10);
        if (value > USHRT_MAX || errno != 0) {
            retval = eERR_UNKNOWN_MESSAGE;
            break;
        }
        message->data[2] = (value >> 8) & 0x000000FF;
        message->data[3] = value & 0x000000FF;
    } while (0);
    return retval;
}

int mqtt2msg_setreg_32bit(char* topic, char* msgtext, msg_t* message)
{
    int retval = eERR_NONE;
    uint32_t value;

    do {
        message->length = 6;
        message->data[0] = eCMD_SET_REG_32BIT;

        value = strtoul(topic, NULL, 16);
        if (value > UCHAR_MAX || errno != 0) {
            retval = eERR_UNKNOWN_MESSAGE;
            break;
        }
        message->data[1] = value;

        value = strtoul(msgtext, NULL, 10);
        if (value > UINT_MAX || errno != 0) {
            retval = eERR_UNKNOWN_MESSAGE;
            break;
        }
        message->data[2] = (value >> 24) & 0x000000FF;
        message->data[3] = (value >> 16) & 0x000000FF;
        message->data[4] = (value >> 8) & 0x000000FF;
        message->data[5] = value & 0x000000FF;
    } while (0);
    return retval;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int mqtt2msg_subscribe(struct mosquitto* mosq)
{
    int mrc;
    uint16_t mid = 0;
    mqtt_msg_types_t* msg_types = g_mqtt_msg_types;
    while (msg_types->func != NULL) {
        mrc = mosquitto_subscribe(mosq, NULL, msg_types->topic, 0);
        if (mrc != MOSQ_ERR_SUCCESS) {
            log_error("mosquitto_subscribe() of topic %s failed with errorcode %d",
                      msg_types->topic, mrc);
            break;
        }
        log_msg(LOG_STATUS, "mosquitto_subscribe() sucessfully subscribed %s", msg_types->topic);
        msg_types++;
    }
    if (mrc != MOSQ_ERR_SUCCESS) {
        return eERR_3RD_PARTY_FUNC_FAILED;
    }
    return eERR_NONE;
}

int mqtt2msg(char* topic, char* msgtext, msg_t* message)
{
    int retval = eERR_UNKNOWN_MESSAGE;

    uint16_t sender = 0, receiver = 0;
    char *msgtype = NULL, *remaining_topic = NULL;
    mqtt_msg_types_t* msg_types = g_mqtt_msg_types;

    do {
        retval = mqtt_split_topic(topic, &sender, &receiver, &msgtype, &remaining_topic);
        if (retval != eERR_NONE) {
            break;
        }

        while (msg_types->func != NULL) {
            if (sys_strcasecmp(msgtype, msg_types->msgtype) == 0) {
                message->sender = sender;
                message->receiver = receiver;
                retval = msg_types->func(remaining_topic, msgtext, message);
                break;
            }
            msg_types++;
        }
    } while (0);
    return retval;
}

/** @} */
