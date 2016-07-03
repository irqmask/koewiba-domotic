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
#include "prjconf.h"

#if !defined (PRJCONF_WINDOWS) 
#include <stdbool.h>
#include <stdint.h>
#else
#include <windows.h>
#endif

// --- Type definitions --------------------------------------------------------

#if defined (PRJCONF_WINDOWS) 
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned long long int uint64_t;
typedef signed long long int int64_t;
#endif

// --- Definitions -------------------------------------------------------------

#ifndef bool
typedef unsigned char bool;
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true (!false)
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#define CLOCK_TICKS_PER_SECOND 100

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _PRJTYPES_H__ */
/** @} */
