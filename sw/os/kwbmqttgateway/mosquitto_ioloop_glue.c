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

/**
 * Callback whenever the registered socket for this callback is able to be read.
 * @param[in] Ponter to structure carrying application handles.
 * @returns always 0 meaning ioloop shall continue calling this callback.
 */
static int on_read_handler(void* arg)
{
    int rc = 0;
    app_handles_t* h = (app_handles_t*)arg;

    rc = mosquitto_loop_read(h->mosq, 1);
    if (rc != 0) {
        log_error("mosquitto_loop_read returned error %d: %s", rc, mosquitto_strerror(rc));
        if (rc == MOSQ_ERR_NO_CONN || rc == MOSQ_ERR_CONN_LOST) {
            h->mqtt_disconnected = true;
        }
    }

    return 0;
}

/**
 * Callback whenever the registered socket for this callback is able to be written.
 * @param[in] Ponter to structure carrying application handles.
 * @returns always 0 meaning ioloop shall continue calling this callback.
 */
static int on_write_handler(void* arg)
{
    int rc;
    app_handles_t* h = (app_handles_t*)arg;

    rc = mosquitto_loop_write(h->mosq, 1);
    if (rc != 0) {
        log_error("mosquitto_loop_write returned error %d: %s", rc, mosquitto_strerror(rc));
        if (rc == MOSQ_ERR_NO_CONN || rc == MOSQ_ERR_CONN_LOST) {
            h->mqtt_disconnected = true;
        }
    }

    mosquitto_ioloop_suspend_write(h);
    return 0;
}

/**
 * Callback called in a cyclic interval for MQTT housekeeping.
 * @param[in] Ponter to structure carrying application handles.
 * @returns always 0 meaning ioloop shall continue calling this callback.
 */
static int on_misc_handler(void* arg)
{
    int rc = 0;
    app_handles_t* h = (app_handles_t*)arg;

    rc = mosquitto_loop_misc(h->mosq);
    if (rc != 0) {
        log_error("mosquitto_loop_misc returned error %d: %s", rc, mosquitto_strerror(rc));
        if (rc == MOSQ_ERR_NO_CONN || rc == MOSQ_ERR_CONN_LOST) {
            h->mqtt_disconnected = true;
        }
    }

    return 0;
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Connect MQTT to ioloop.
 * @param[in] structure carrying all network (kwb and mqtt) handles of the application.
 * @returns 0 if successful otherwise error-code.
 */
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
 * @param[in] structure carrying all network (kwb and mqtt) handles of the application.
 */
void mosquitto_ioloop_suspend_write(app_handles_t* h)
{
    sys_fd_t fd;

    fd = (sys_fd_t)mosquitto_socket(h->mosq);
    if (fd == INVALID_FD) return;

    if (mosquitto_want_write(h->mosq) == true) {
        ioloop_register_fd(h->ioloop, fd, eIOLOOP_EV_WRITE, on_write_handler, h->mosq);
    } else {
        ioloop_unregister_fd(h->ioloop, fd, eIOLOOP_EV_WRITE);
    }
}

/** @} */
