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

// include
#include "prjtypes.h"

// shared
#include "timer.h"

// os/shared
#include "log.h"

#include "kwbmqttgateway.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Local functions ---------------------------------------------------------

static int on_read_handler(void* arg)
{
    app_handles_t* h = (app_handles_t*)arg;

    mosquitto_loop_read(h->mosq, 1);
    return 0;
}

static int on_write_handler(void* arg)
{
    app_handles_t* h = (app_handles_t*)arg;

    mosquitto_loop_write(h->mosq, 1);
    mosquitto_ioloop_suspend_write(h);
    return 0;
}

static int on_misc_handler(void* arg)
{
    app_handles_t* h = (app_handles_t*)arg;

    mosquitto_loop_misc(h->mosq);
    return 0;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int mosquitto_connect_to_ioloop(app_handles_t* h)
{
    int retval = eERR_NONE;
    sys_fd_t fd = INVALID_FD;

    do {
        fd = (sys_fd_t)mosquitto_socket(h->mosq);
        if (fd == INVALID_FD) {
            retval = eERR_RESOURCE;
            break;
        }
        ioloop_register_fd(h->ioloop, fd, eIOLOOP_EV_READ, on_read_handler, h);
        ioloop_register_fd(h->ioloop, fd, eIOLOOP_EV_WRITE, on_write_handler, h);
        ioloop_register_timer(h->ioloop, TIMER_MS_2_TICKS(100), 1, on_misc_handler, h);
    } while (0);
    return retval;
}

/** 
 * Check if a MQTTmessage has to be written. If not suspend write callbacks.
 */
void mosquitto_ioloop_suspend_write(app_handles_t* h)
{
    sys_fd_t fd;

    fd = (sys_fd_t)mosquitto_socket(h->mosq);
    if (fd == INVALID_FD) return;

    if (mosquitto_want_write(h->mosq)) {
        ioloop_register_fd(h->ioloop, fd, eIOLOOP_EV_WRITE, on_write_handler, h->mosq);
    } else {
        ioloop_unregister_fd(h->ioloop, fd, eIOLOOP_EV_WRITE);
    }
}

/** @} */
