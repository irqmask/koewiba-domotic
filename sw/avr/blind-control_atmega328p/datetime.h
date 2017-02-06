/**
 * @addtogroup DATETIME
 * @brief Public interface of date and time module.
 *
 * This module contains functions to manage date and time.
 *
 * @{
 * @file    datetime.h
 * @brief   This module contains functions to manage date and time.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _DATETIME_H_
#define _DATETIME_H_

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef enum weekday {
	eMONDAY,
	eTUESDAY,
	eWEDNESDAY,
	eTHURSDAY,
	eFRIDAY,
	eSATURDAY,
	eSUNDAY
} weekday_t;

//! Time-out timer runtime-data.
typedef struct datetime {
    uint16_t 		year;
    uint8_t			month;
    uint8_t			day;
    weekday_t		day_of_week;

    uint8_t			hour;
    uint8_t         minute;
    uint8_t         second;
} datetime_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void dat_initialize        (void);

#endif /* _DATETIME_H_ */
/** @} */
