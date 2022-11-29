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
#ifndef _ERROR_CODES_H_
#define _ERROR_CODES_H_

// --- Include section ---------------------------------------------------------

// --- Definitions -------------------------------------------------------------

//! First error number for generic errors
#define ERROR_GENERIC       100
//! First error number for errors of libsystem library
#define ERROR_LIBSYSTEM     200
//! First error number for user-defined errors.
#define ERROR_USER1         300

// --- Type definitions --------------------------------------------------------

//! Generic errors
typedef enum {
    eRUNNING = -1,
    eERR_NONE = 0,
    eERR_NOT_IMPLEMENTED = ERROR_GENERIC,
    /// A resource (memory, device etc.) is not available
    eERR_RESOURCE,
    /// A System error has occurred
    eERR_SYSTEM,
    /// A invalid parameter has been passed into a method / function
    eERR_BAD_PARAMETER,
    /// An operation or process failed
    eERR_PROCESS_FAILED,
    eERR_3RD_PARTY_FUNC_FAILED,
    eERR_UNSUPPORTED,
    /// Unable to establish connection
    eERR_CONNECTION,
    eERR_UNKNOWN
} gen_errors_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _ERROR_CODES_H_ */
/** @} */
