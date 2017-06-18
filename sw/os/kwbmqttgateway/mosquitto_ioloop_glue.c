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
 * @file    mosquitto_ioloop_glue.c
 * @brief   Gateway to convert koewiba message to mqtt and vice versa.
 * kwbmqttgateway routes commands comming from a kwbrouter to an mqtt broker.
 * Messages from another MQTT client are aubscribed for and transmitted to a
 * kwbrouter for distribution.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include "mosquitto.h"

// include
#include "prjtypes.h"

// shared
#include "clock.h"

// os/shared
#include "ioloop.h"
#include "log.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Local functions ---------------------------------------------------------

static int on_read_handler(void* arg)
{
    struct mosquitto* mosq = (struct mosquitto*)arg;

    mosquitto_loop_read(mosq, 10);
    return 0;
}

static int on_write_handler(void* arg)
{
    struct mosquitto* mosq = (struct mosquitto*)arg;

    mosquitto_loop_write(mosq, 10);
    return 0;
}

static int on_misc_handler(void* arg)
{
    struct mosquitto* mosq = (struct mosquitto*)arg;

    mosquitto_loop_misc(mosq);
    return 0;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int mosquitto_connect_to_ioloop(struct  mosquitto* mosq, ioloop_t* ioloop)
{
    int fd, retval = eERR_NONE;

    do {
        fd = mosquitto_socket(mosq);
        if (fd == INVALID_FD) {
            retval = eERR_INVALID_FD;
            break;
        }
        ioloop_register_fd(ioloop, fd, eIOLOOP_EV_READ, on_read_handler, mosq);
        ioloop_register_fd(ioloop, fd, eIOLOOP_EV_WRITE, on_write_handler, mosq);
        ioloop_register_timer(ioloop, CLOCK_MS_2_TICKS(100), 1, on_misc_handler, mosq);
    } while (0);
    return retval;
}

/** @} */
