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

// --- Include section ---------------------------------------------------------

#include <errno.h>
#include <stdint.h>
#include <stdio.h>

#include "ihex.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef enum {
    eADDRESSMODE_SEGMENT,
    eADDRESSMODE_LINEAR
} e_addressmode_t;

//! temporary data, which is used during file parsing
typedef struct ihex_data {
    // temporary line data
    uint8_t         line_data_length;
    uint16_t        line_offset;        //!< line load offset
    uint8_t         line_checksum;      //!< current checksum of a line

    // data during parsing the whole files
    e_addressmode_t mode;
    uint32_t        global_offset;

    // resulting data
    uint32_t        start_address;
    e_addressmode_t start_address_mode;
    uint32_t        first_address;
    uint32_t        last_address;
} ihex_data_t;

// temporary data, which is used by the simple memory-writer function
typedef struct meminfo {
    uint8_t* ptr;
    uint32_t size;
} meminfo_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// read mark at the beginning of the line, which is the colon ':'
// skip linefeed- or newline- characters which may occur during reading the file.
// -----------------------------------------------------------------------------
static int32_t read_mark(FILE*                  file)
{
    int32_t retval      = e_ihex_error_line_mark;
    char    markchar  = '\0';

    do {
        if (fread(&markchar, sizeof(markchar), 1, file) == 1) {
            if (markchar == ':') {
                retval = e_ihex_success;
                break;
            }
        } else {
            retval = e_ihex_eof;
            break;
        }
    } while ((markchar == '\n' || markchar == '\r') && (markchar != ':'));
    return retval;
}

// read a nibble. if the read character dos not represent a hexadecimal number,
// an error is returned,
// ----------------------------------------------------------------------------
static int32_t read_nibble(FILE*                  file,
                           uint8_t*               nibble)
{
    int32_t retval      = e_ihex_success;
    char    nibblechar  = '\0';

    if (fread(&nibblechar, sizeof(nibblechar), 1, file) == 1) {
        if (nibblechar >= '0' && nibblechar <= '9') {
            *nibble = nibblechar - '0';
        } else if (nibblechar >= 'A' && nibblechar <= 'F') {
            *nibble = nibblechar - 'A' + 10;
        } else if (nibblechar >= 'a' && nibblechar <= 'f') {
            *nibble = nibblechar - 'a' + 10;
        } else if (nibblechar == '\r' || nibblechar == '\n') {
            retval = e_ihex_error_unexpected_eol;
        } else {
            retval = e_ihex_error_unexpected_char;
        }
    } else {
        retval = e_ihex_error_unexpected_eof;
    }
    return retval;
}

// read a byte, which is represented by to characters
// -----------------------------------------------------------------------------
static int32_t read_byte(FILE*                  file,
                         uint8_t*               pbyte)
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
static int32_t read_data_rec(FILE*                  file,
                             ihex_data_t*           meta_data,
                             ihex_write_byte_func   write_byte_func,
                             void*                  arg
                            )
{
    int32_t     retval = e_ihex_success;
    uint32_t    address, ii = 0;
    uint8_t     byte;

    while (meta_data->line_data_length-- > 0) {
        // calculate address and store min/max
        if (meta_data->mode == eADDRESSMODE_LINEAR) {
            address = meta_data->global_offset + meta_data->line_offset + ii;
        } else {
            address = (meta_data->line_offset + ii) & 0x0000FFFF;
            address += meta_data->global_offset;
        }
        if (address < meta_data->first_address) meta_data->first_address = address;
        if (address > meta_data->last_address) meta_data->last_address = address;
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
static int32_t read_extsegaddr_rec(FILE*                  file,
                                   ihex_data_t*           meta_data)
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
static int32_t read_startsegaddr_rec(FILE*                  file,
                                     ihex_data_t*           meta_data)
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
static int32_t read_extlinearaddr_rec(FILE*                  file,
                                      ihex_data_t*           meta_data)
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
static int32_t read_startlinearaddr_rec(FILE*                  file,
                                        ihex_data_t*           meta_data)
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
static int32_t read_line(FILE*                  file,
                         ihex_data_t*           meta_data,
                         ihex_write_byte_func   write_byte_func,
                         void*                  arg)
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
            retval = read_data_rec(file, meta_data, write_byte_func, arg); break;
        case 1:
            break; // nothing more
        case 2:
            retval = read_extsegaddr_rec(file, meta_data); break;
        case 3:
            retval = read_startsegaddr_rec(file, meta_data); break;
        case 4:
            retval = read_extlinearaddr_rec(file, meta_data); break;
        case 5:
            retval = read_startlinearaddr_rec(file, meta_data); break;
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
void write_byte_to_mem(uint32_t address, uint8_t byte, void* arg)
{
    meminfo_t* mem = arg;

    if (mem != NULL) {
        if (mem->ptr && address < mem->size) {
            mem->ptr[address] = byte;
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Read an Intel Hex file. For each byte the write_byte_func() is caĺled.
 * 
 * @param[in]   filename        Path and filename of Intel HEX file.
 * @param[out]  pstart_address  Address in target memory of the program entry 
 *                              point.
 * @param[out]  pfirst_address  First (lowest) address in target memory which 
 *                              is filled by the IHEX file.
 * @param[out]  plast_address   Last (highest) address in target memory which is
 *                              filled by the IHEX file.
 * @param[in]   write_byte_func Callback function which is called for every byte
 *                              to by written @see ihex_write_byte_func for 
 *                              details.
 * @param[in]   arg             User defined argument which is passed to the 
 *                              write_byte_func.
 * 
 * @returns     0 if the IHEX file has been successfully read in, otherwise false.
 */
int32_t ihex_read_file (const char*             filename,
                        uint32_t*               pstart_address,
                        uint32_t*               pfirst_address,
                        uint32_t*               plast_address,
                        ihex_write_byte_func    write_byte_func,
                        void*                   arg)
{
    int32_t     retval;
    FILE*       handle;
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
        if (retval == e_ihex_eof) retval = e_ihex_success;
    } while (0);

    // on success return metadata
    if (retval == e_ihex_success) {
        if (pstart_address != NULL) *pstart_address = metadata.start_address;
        if (pfirst_address != NULL) *pfirst_address = metadata.first_address;
        if (plast_address != NULL) *plast_address = metadata.last_address;
    }

    // if file has been opened, close it
    if (handle != NULL) fclose(handle);

    return retval;
}


/**
 * Read an Intel Hex file. For each byte the write_byte_func() is caĺled.
 * 
 * @param[in]   filename        Path and filename of Intel HEX file.
 * @param[out]  pstart_address  Address in target memory of the program entry 
 *                              point.
 * @param[out]  pfirst_address  First (lowest) address in target memory which 
 *                              is filled by the IHEX file.
 * @param[out]  plast_address   Last (highest) address in target memory which is
 *                              filled by the IHEX file.
 * @param[in]   target_memory   Byte pointer to the target memory, into which 
 *                              the IHEX file content is written.
 * @param[in]   target_memory_size Size of target memory. If the size of the 
 *                              IHEX file contnt exceeds the target memoy size, 
 *                              the read process is stopped.
 * 
 * @returns     0 if the IHEX file has been successfully read in, otherwise false.
 */
int32_t ihex_read_file_mem (const char*             filename,
                            uint32_t*               pstart_address,
                            uint32_t*               pfirst_address,
                            uint32_t*               plast_address,
                            uint8_t*                target_memory,
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
