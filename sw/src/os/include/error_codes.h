/**
 * @addtogroup ERROR_CODES
 * @brief Definition of general error codes and error ranges.
 *
 * This module contains the definition of global error codes. Libraries and
 * modules define it's own error codes, bus this header defines the starting
 * number per library.
 *
 * @{
 * @file    error_codes.h
 * @brief   Definition of general error codes and error ranges.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _ERROR_CODES_H_
#define _ERROR_CODES_H_

// --- Include section ---------------------------------------------------------

// --- Definitions -------------------------------------------------------------

#define ERROR_GENERIC       100
#define ERROR_LIBSYSTEM     200
#define ERROR_LIBKWB        300

// --- Type definitions --------------------------------------------------------

typedef enum {
    eERR_NONE = 0,
    eERR_NOT_IMPLEMENTED = ERROR_GENERIC,
} gen_errors_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _ERROR_CODES_H_ */
/** @} */
