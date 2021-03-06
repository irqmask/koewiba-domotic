/**
 * @addtogroup IHEX
 * @brief Loader of intel hex files.
 *
 * @{
 * @file    ihex.c
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

// --- Include section ---------------------------------------------------------

#include <errno.h>
#include <stdint.h>
#include <stdio.h>

#include "ihex.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

/**
 * possible addressing modes in an ihex file.
 */
typedef enum {
    eADDRESSMODE_SEGMENT,
    eADDRESSMODE_LINEAR
} e_addressmode_t;

/**
 * temporary data, which is used during file parsing
 */
typedef struct ihex_data {
    // temporary line data
    //! length in char of the line
    uint8_t         line_data_length;
    //! line load offset
    uint16_t        line_offset;
    //! current checksum of a line
    uint8_t         line_checksum;

    // data during parsing the whole files
    //! addressing mode, segment or linear
    e_addressmode_t mode;
    //! offset set by extended segmend address record of an IHEX file.
    uint32_t        global_offset;

    // resulting data
    //! Start address of the program in the IHEX file
    uint32_t        start_address;
    //! Addressing mode of start address
    e_addressmode_t start_address_mode;
    //! First address of used target memory
    uint32_t        first_address;
    //! Last address of used target memory
    uint32_t        last_address;
} ihex_data_t;

/**
 * temporary data, which is used by the simple memory-writer function
 */
typedef struct meminfo {
    //! Pointer to memory to write into
    uint8_t *ptr;
    //! Size of memory block to write into
    uint32_t size;
} meminfo_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// read mark at the beginning of the line, which is the colon ':'
// skip linefeed- or newline- characters which may occur during reading the file.
// -----------------------------------------------------------------------------
static int32_t read_mark(FILE                  *file)
{
    int32_t retval      = e_ihex_error_line_mark;
    char    markchar  = '\0';

    do {
        if (fread(&markchar, sizeof(markchar), 1, file) == 1) {
            if (markchar == ':') {
                retval = e_ihex_success;
                break;
            }
        }
        else {
            retval = e_ihex_eof;
            break;
        }
    } while ((markchar == '\n' || markchar == '\r') && (markchar != ':'));
    return retval;
}

// read a nibble. if the read character dos not represent a hexadecimal number,
// an error is returned,
// ----------------------------------------------------------------------------
static int32_t read_nibble(FILE                  *file,
                           uint8_t               *nibble)
{
    int32_t retval      = e_ihex_success;
    char    nibblechar  = '\0';

    if (fread(&nibblechar, sizeof(nibblechar), 1, file) == 1) {
        if (nibblechar >= '0' && nibblechar <= '9') {
            *nibble = nibblechar - '0';
        }
        else if (nibblechar >= 'A' && nibblechar <= 'F') {
            *nibble = nibblechar - 'A' + 10;
        }
        else if (nibblechar >= 'a' && nibblechar <= 'f') {
            *nibble = nibblechar - 'a' + 10;
        }
        else if (nibblechar == '\r' || nibblechar == '\n') {
            retval = e_ihex_error_unexpected_eol;
        }
        else {
            retval = e_ihex_error_unexpected_char;
        }
    }
    else {
        retval = e_ihex_error_unexpected_eof;
    }
    return retval;
}

// read a byte, which is represented by to characters
// -----------------------------------------------------------------------------
static int32_t read_byte(FILE                  *file,
                         uint8_t               *pbyte)
{
    int32_t retval      = e_ihex_success;
    uint8_t nibble      = 0,
            byte        = 0;

    do {
        if ((retval = read_nibble(file, &nibble)) != e_ihex_success) {
            break;
        }
        byte = nibble << 4;
        if ((retval = read_nibble(file, &nibble)) != e_ihex_success) {
            break;
        }
        byte |= nibble & 0x0F;
        *pbyte = byte;
    } while (0);
    return retval;
}

// read record type 00: Data Record
// ----------------------------------------------------------------------------
static int32_t read_data_rec(FILE                  *file,
                             ihex_data_t           *meta_data,
                             ihex_write_byte_func   write_byte_func,
                             void                  *arg
                            )
{
    int32_t     retval = e_ihex_success;
    uint32_t    address, ii = 0;
    uint8_t     byte;

    while (meta_data->line_data_length-- > 0) {
        // calculate address and store min/max
        if (meta_data->mode == eADDRESSMODE_LINEAR) {
            address = meta_data->global_offset + meta_data->line_offset + ii;
        }
        else {
            address = (meta_data->line_offset + ii) & 0x0000FFFF;
            address += meta_data->global_offset;
        }
        if (address < meta_data->first_address) {
            meta_data->first_address = address;
        }
        if (address > meta_data->last_address) {
            meta_data->last_address = address;
        }
        // read byte
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += byte;
        // store byte, if wanted
        if (write_byte_func != NULL) {
            write_byte_func(address, byte, arg);
        }
        ii++;
    }
    return retval;
}

// read record type 02: Extended Segment Address Record
// ----------------------------------------------------------------------------
static int32_t read_extsegaddr_rec(FILE                  *file,
                                   ihex_data_t           *meta_data)
{
    int32_t     retval = e_ihex_success;
    uint16_t    usba;
    uint8_t     byte = 0;

    do {
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += byte;
        usba = byte << 8;
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += byte;
        usba |= byte;
        meta_data->global_offset = usba << 4;
        meta_data->mode = eADDRESSMODE_SEGMENT;
    } while (0);
    return retval;
}

// read record type 03: Start Segment Address Record
// ----------------------------------------------------------------------------
static int32_t read_startsegaddr_rec(FILE                  *file,
                                     ihex_data_t           *meta_data)
{
    int32_t retval = e_ihex_success;
    uint8_t byte = 0;

    do {
        meta_data->start_address_mode = eADDRESSMODE_SEGMENT;
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += byte;
        meta_data->start_address = byte << 24;
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += byte;
        meta_data->start_address |= byte << 16;
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += byte;
        meta_data->start_address |= byte << 8;
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += byte;
        meta_data->start_address |= byte;
    } while (0);
    return retval;
}

// read record type 04: Extended Linear Address Record
// ----------------------------------------------------------------------------
static int32_t read_extlinearaddr_rec(FILE                  *file,
                                      ihex_data_t           *meta_data)
{
    int32_t retval = e_ihex_success;
    uint8_t byte = 0;

    do {

        meta_data->mode = eADDRESSMODE_LINEAR;
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += byte;
        meta_data->global_offset = byte << 24;
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += byte;
        meta_data->global_offset |= byte << 16;
        meta_data->global_offset &= 0xFFFF0000;
    } while (0);
    return retval;
}

// read record type 05: Start Linear Address Record
// ----------------------------------------------------------------------------
static int32_t read_startlinearaddr_rec(FILE                  *file,
                                        ihex_data_t           *meta_data)
{
    int32_t retval = e_ihex_success;
    uint8_t byte = 0;

    do {
        meta_data->start_address_mode = eADDRESSMODE_LINEAR;
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += byte;
        meta_data->start_address = byte << 24;
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += byte;
        meta_data->start_address |= byte << 16;
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += byte;
        meta_data->start_address |= byte << 8;
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += byte;
        meta_data->start_address |= byte;
    } while (0);
    return retval;
}

// read an entire line of the IHEX file and interpret it's content
// ----------------------------------------------------------------------------
static int32_t read_line(FILE                  *file,
                         ihex_data_t           *meta_data,
                         ihex_write_byte_func   write_byte_func,
                         void                  *arg)
{
    int32_t     retval      = e_ihex_success;
    uint8_t     byte        = 0,
                rectype     = 0;
    char        lineendchar = '\0';

    do {
        meta_data->line_data_length = 0;
        meta_data->line_offset = 0;
        meta_data->line_checksum = 0;
        // read record mark, every line starts with
        if ((retval = read_mark(file)) != e_ihex_success) {
            break;
        }

        // read record length
        if ((retval = read_byte(file, &meta_data->line_data_length)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += meta_data->line_data_length;

        // read record offset
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_offset = byte << 8;
        meta_data->line_checksum += byte;
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_offset |= byte;
        meta_data->line_checksum += byte;

        // read record type
        if ((retval = read_byte(file, &rectype)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += rectype;

        switch (rectype) {
        case 0:
            retval = read_data_rec(file, meta_data, write_byte_func, arg);
            break;
        case 1:
            break; // nothing more
        case 2:
            retval = read_extsegaddr_rec(file, meta_data);
            break;
        case 3:
            retval = read_startsegaddr_rec(file, meta_data);
            break;
        case 4:
            retval = read_extlinearaddr_rec(file, meta_data);
            break;
        case 5:
            retval = read_startlinearaddr_rec(file, meta_data);
            break;
        default:
            retval = e_ihex_error_invalid_rectype;
            return retval;
        }

        // read line checksum
        if ((retval = read_byte(file, &byte)) != e_ihex_success) {
            break;
        }
        meta_data->line_checksum += byte;
        // sum of checksum of data and checksum byte ate the line ending has
        // to be zero.
        if (meta_data->line_checksum != 0) {
            retval = e_ihex_error_checksum;
            break;
        }

        // read line ending
        if (fread(&lineendchar, sizeof(lineendchar), 1, file) == 1) {
            if (lineendchar != '\n' && lineendchar != '\r') {
                retval = e_ihex_error_chars_at_eol;
                break;
            }
        }
    } while (0);

    return retval;
}

// helper function, which is used to read the IHEX file completely into memory.
//----------------------------------------------------------------------------
static void write_byte_to_mem(uint32_t address, uint8_t byte, void *arg)
{
    meminfo_t *mem = arg;

    if (mem != NULL) {
        if (mem->ptr && address < mem->size) {
            mem->ptr[address] = byte;
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int32_t ihex_read_file(const char             *filename,
                       uint32_t               *pstart_address,
                       uint32_t               *pfirst_address,
                       uint32_t               *plast_address,
                       ihex_write_byte_func   write_byte_func,
                       void                   *arg)
{
    int32_t     retval;
    FILE       *handle;
    uint32_t    line = 0;
    ihex_data_t metadata;

    do {
        metadata.mode = eADDRESSMODE_LINEAR;
        metadata.global_offset = 0;
        metadata.first_address = 0xFFFFFFFF;
        metadata.last_address = 0x00000000;
        metadata.start_address = 0x00000000;

        // try to open the file for reading
        handle = fopen(filename, "r");
        if (handle == NULL) {
            retval = e_ihex_error_file_open;
            break;
        }

        // read the file line by line
        do {
            retval = read_line(handle, &metadata, write_byte_func, arg);
            line++;
        } while (retval == e_ihex_success);

        // skip end of file error, because it is normal
        if (retval == e_ihex_eof) {
            retval = e_ihex_success;
        }
    } while (0);

    // on success return metadata
    if (retval == e_ihex_success) {
        if (pstart_address != NULL) {
            *pstart_address = metadata.start_address;
        }
        if (pfirst_address != NULL) {
            *pfirst_address = metadata.first_address;
        }
        if (plast_address != NULL) {
            *plast_address = metadata.last_address;
        }
    }

    // if file has been opened, close it
    if (handle != NULL) {
        fclose(handle);
    }

    return retval;
}

//----------------------------------------------------------------------------
int32_t ihex_read_file_mem(const char             *filename,
                           uint32_t               *pstart_address,
                           uint32_t               *pfirst_address,
                           uint32_t               *plast_address,
                           uint8_t                *target_memory,
                           uint32_t                target_memory_size)
{
    meminfo_t mem;

    mem.ptr = target_memory;
    mem.size = target_memory_size;

    return ihex_read_file(filename,
                          pstart_address,
                          pfirst_address,
                          plast_address,
                          write_byte_to_mem, &mem);
}
/** @} */
