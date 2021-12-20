/**
 * @addtogroup IHEX
 * @brief Loader of intel hex files.
 *
 * @{
 * @file    ihex.h
 * @brief   Loader of intel hex files.
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
#ifndef _IHEX_H_
#define _IHEX_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

/**
 * Write byte callback function type. A function with this interface is called
 * every time a byte has to be written into target memory.
 * @param[in]   address     Address in target memory.
 * @param[in]   byte        Byte to be written.
 * @param[in]   arg         User defined argument.
 */
typedef void (*ihex_write_byte_func)(uint32_t address, uint8_t byte, void *arg);

/**
 * Enumeration of errors which may occur during parsing of the IHEX file.
 */
typedef enum {
    e_ihex_eof = -1,
    e_ihex_success = 0,
    e_ihex_error_file_open = 200,
    e_ihex_error_unexpected_char,
    e_ihex_error_unexpected_eol,
    e_ihex_error_unexpected_eof,
    e_ihex_error_line_mark,
    e_ihex_error_chars_at_eol,
    e_ihex_error_invalid_rectype,
    e_ihex_error_checksum,
} ihex_error_t;



// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Read an Intel Hex file. For each byte the write_byte_func() is caĺled.
 *
 * @param[in]   filename        Path and filename of Intel HEX file.
 * @param[out]  start_address  Address in target memory of the program entry
 *                              point.
 * @param[out]  first_address  First (lowest) address in target memory which
 *                              is filled by the IHEX file.
 * @param[out]  last_address   Last (highest) address in target memory which is
 *                              filled by the IHEX file.
 * @param[in]   write_byte_func Callback function which is called for every byte
 *                              to by written @see ihex_write_byte_func for
 *                              details.
 * @param[in]   arg             User defined argument which is passed to the
 *                              write_byte_func.
 *
 * @returns     0 if the IHEX file has been successfully read in, otherwise false.
 */
int32_t ihex_read_file(const char             *filename,
                       uint32_t               *start_address,
                       uint32_t               *first_address,
                       uint32_t               *last_address,
                       ihex_write_byte_func    write_byte_func,
                       void                   *arg);

/**
 * Read an Intel Hex file. For each byte the write_byte_func() is caĺled.
 *
 * @param[in]   filename        Path and filename of Intel HEX file.
 * @param[out]  start_address  Address in target memory of the program entry
 *                              point.
 * @param[out]  first_address  First (lowest) address in target memory which
 *                              is filled by the IHEX file.
 * @param[out]  last_address   Last (highest) address in target memory which is
 *                              filled by the IHEX file.
 * @param[in]   target_memory   Byte pointer to the target memory, into which
 *                              the IHEX file content is written.
 * @param[in]   target_memory_size Size of target memory. If the size of the
 *                              IHEX file contnt exceeds the target memoy size,
 *                              the read process is stopped.
 *
 * @returns     0 if the IHEX file has been successfully read in, otherwise false.
 */
int32_t ihex_read_file_mem(const char         *filename,
                           uint32_t           *start_address,
                           uint32_t           *first_address,
                           uint32_t           *last_address,
                           uint8_t            *target_memory,
                           uint32_t            target_memory_size);

#ifdef __cplusplus
}
#endif
#endif // _IHEX_H_
/** @} */
