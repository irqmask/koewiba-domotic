/**
 * @addtogroup KWBMONITOR
 * @brief Analyze KWB messages on a byte level.
 *
 * @{
 * @file    kwbmonitor.h
 * @brief   Analyze KWB messages on a byte level.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _KWBMONITOR_H_
#define _KWBMONITOR_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>
#include "systime.h"

// --- Definitions -------------------------------------------------------------

//! (netto) length of the message
#define BUS_MAXMSGLEN          16

//! length of message including header and footer
#define BUS_MAXTOTALMSGLEN      (BUS_MAXMSGLEN + 7)

// --- Type definitions --------------------------------------------------------

typedef struct bus_history {
    sys_time_t      uTimeStart;
    sys_time_t      uTimeLastByte;
    sys_time_t      uTimeCurrByte;
    uint8_t         uCurrMsgBytes;
    uint8_t         uExpectedLength;
    uint16_t        uExpectedCRC;
    uint8_t         uCurrSender;
    uint8_t         auMessage[BUS_MAXTOTALMSGLEN+10];
} bus_history_t;


// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void monitor_init (bus_history_t* history);

void monitor_parse_message (uint8_t new_byte, bus_history_t* history);

void monitor_toggle_display_empty_message (void);

void monitor_toggle_display_token_message (void);

#endif /* _KWBMONITOR_H_ */
/** @} */
