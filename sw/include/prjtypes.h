/**
 * @addtogroup PRJTYPES
 * @brief Define project specific types definitions.
 *
 * @{
 * @file    prjtypes.h
 * @brief   Define project specific types definitions..
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _PRJTYPES_H__
#define _PRJTYPES_H__

// --- Include section ---------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>

// --- Definitions -------------------------------------------------------------

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

//! Clock ticks per second.
#define CLOCK_TICKS_PER_SECOND 100

// --- Type definitions --------------------------------------------------------

typedef uint8_t BOOL;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _PRJTYPES_H__ */
/** @} */
