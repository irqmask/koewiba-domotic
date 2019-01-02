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
typedef void (*ihex_write_byte_func)(uint32_t address, uint8_t byte, void* arg);

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

int32_t ihex_read_file (const char*             filename,
                        uint32_t*               start_address,
                        uint32_t*               first_address,
                        uint32_t*               last_address,
                        ihex_write_byte_func    write_byte_func,
                        void*                   arg);

int32_t ihex_read_file_mem (const char*         filename,
                            uint32_t*           start_address,
                            uint32_t*           first_address,
                            uint32_t*           last_address,
                            uint8_t*            target_memory,
                            uint32_t            target_memory_size);

#ifdef __cplusplus
}
#endif
#endif // _IHEX_H_
/** @} */
