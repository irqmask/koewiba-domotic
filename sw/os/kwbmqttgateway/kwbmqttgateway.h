/**
 * @addtogroup KWBMQTTGATEWAY
 *
 * @{
 * @file    kwbmqttgateway.h
 * @brief   Common datastructures and functions used in kwbmqttgateway.
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
#ifndef _KWBMQTTGATEWAY_H_
#define _KWBMQTTGATEWAY_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>
#include <stdio.h>

#include "mosquitto.h"

// include
#include "prjtypes.h"

// os/shared
#include "ioloop.h"
#include "message.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

//! Structure of global variables.
typedef struct _app_handles_t {
    ioloop_t           *ioloop;     //!< IO background loop /ref IOLOOP ioloop.
    struct mosquitto   *mosq;       //!< Global mosquitto handle.
    bool                mqtt_disconnected; //!< Flag if MQTT got disconnected
} app_handles_t;

// --- Local variables ---------------------------------------------------------

extern app_handles_t    g_handles;

// --- Module global variables -------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

extern int mosquitto_connect_to_ioloop(app_handles_t *h);

extern void mosquitto_ioloop_suspend_write(app_handles_t *h);

extern int msg2mqtt(const msg_t *message,
                    char *topic, size_t max_topic_len,
                    char *msgtext, size_t max_msgtext_len);

extern int mqtt2msg_subscribe(struct mosquitto *mosq);

extern int mqtt2msg(const char *topic, const char *msgtext, msg_t *message);

#endif // _KWBMQTTGATEWAY_H_
/** @} */
