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
    e_addressmode_segment,
    e_addressmode_linear
} e_addressmode_t;

// temporary data, which is used during file parsing
typedef struct ihex_data {
    // temporary line data
    uint8_t         uLineDataLength;
    uint16_t        uLineOffset;    // line load offset
    uint8_t         uLineChkSum;    // current checksum of a line

    // data during parsing the whole files
    e_addressmode_t eMode;
    uint32_t        uGlobalOffset;

    // resulting data
    uint32_t        uStartAddress;
    e_addressmode_t eStartAddressMode;
    uint32_t        uFirstAddress;
    uint32_t        uLastAddress;
} ihex_data_t;

// temporary data, which is used by the simple memory-writer function
typedef struct meminfo {
    uint8_t* puPtr;
    uint32_t uSize;
} meminfo_t;

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// read mark at the beginning of the line, which is the colon ':'
// skip linefeed- or newline- characters which may occur during reading the file.
// ----------------------------------------------------------------------------
static int32_t read_mark(FILE*                  psFile)
{
    int32_t retval      = e_ihex_error_line_mark;
    char    markchar  = '\0';

    do {
        if (fread(&markchar, sizeof(markchar), 1, psFile) == 1) {
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
static int32_t read_nibble(FILE*                  psFile,
                           uint8_t*               puNibble)
{
    int32_t retval      = e_ihex_success;
    char    nibblechar  = '\0';

    if (fread(&nibblechar, sizeof(nibblechar), 1, psFile) == 1) {
        if (nibblechar >= '0' && nibblechar <= '9') {
            *puNibble = nibblechar - '0';
        } else if (nibblechar >= 'A' && nibblechar <= 'F') {
            *puNibble = nibblechar - 'A' + 10;
        } else if (nibblechar >= 'a' && nibblechar <= 'f') {
            *puNibble = nibblechar - 'a' + 10;
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
// ----------------------------------------------------------------------------
static int32_t read_byte(FILE*                  psFile,
                         uint8_t*               puByte)
{
    int32_t retval      = e_ihex_success;
    uint8_t nibble      = 0,
            byte        = 0;

    do {
        if ((retval = read_nibble(psFile, &nibble)) != e_ihex_success) {
            break;
        }
        byte = nibble << 4;
        if ((retval = read_nibble(psFile, &nibble)) != e_ihex_success) {
            break;
        }
        byte |= nibble & 0x0F;
        *puByte = byte;
    } while (0);
    return retval;
}

// read record type 00: Data Record
// ----------------------------------------------------------------------------
static int32_t read_data_rec(FILE*                  psFile,
                             ihex_data_t*           psMetaData,
                             ihex_write_byte_func   pWriteByte,
                             void*                  pvArg
                            )
{
    int32_t     retval = e_ihex_success;
    uint32_t    address, ii = 0;
    uint8_t     byte;

    while (psMetaData->uLineDataLength-- > 0) {
        // calculate address and store min/max
        if (psMetaData->eMode == e_addressmode_linear) {
            address = psMetaData->uGlobalOffset + psMetaData->uLineOffset + ii;
        } else {
            address = (psMetaData->uLineOffset + ii) & 0x0000FFFF;
            address += psMetaData->uGlobalOffset;
        }
        if (address < psMetaData->uFirstAddress) psMetaData->uFirstAddress = address;
        if (address > psMetaData->uLastAddress) psMetaData->uLastAddress = address;
        // read byte
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += byte;
        // store byte, if wanted
        if (pWriteByte != NULL) {
            pWriteByte(address, byte, pvArg);
        }
        ii++;
    }
    return retval;
}

// read record type 02: Extended Segment Address Record
// ----------------------------------------------------------------------------
static int32_t read_extsegaddr_rec(FILE*                  psFile,
                                   ihex_data_t*           psMetaData)
{
    int32_t     retval = e_ihex_success;
    uint16_t    usba;
    uint8_t     byte = 0;

    do {
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += byte;
        usba = byte << 8;
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += byte;
        usba |= byte;
        psMetaData->uGlobalOffset = usba << 4;
        psMetaData->eMode = e_addressmode_segment;
    } while (0);
    return retval;
}

// read record type 03: Start Segment Address Record
// ----------------------------------------------------------------------------
static int32_t read_startsegaddr_rec(FILE*                  psFile,
                                     ihex_data_t*           psMetaData)
{
    int32_t retval = e_ihex_success;
    uint8_t byte = 0;

    do {
        psMetaData->eStartAddressMode = e_addressmode_segment;
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += byte;
        psMetaData->uStartAddress = byte << 24;
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += byte;
        psMetaData->uStartAddress |= byte << 16;
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += byte;
        psMetaData->uStartAddress |= byte << 8;
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += byte;
        psMetaData->uStartAddress |= byte;
    } while (0);
    return retval;
}

// read record type 04: Extended Linear Address Record
// ----------------------------------------------------------------------------
static int32_t read_extlinearaddr_rec(FILE*                  psFile,
                                      ihex_data_t*           psMetaData)
{
    int32_t retval = e_ihex_success;
    uint8_t byte = 0;

    do {

        psMetaData->eMode = e_addressmode_linear;
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += byte;
        psMetaData->uGlobalOffset = byte << 24;
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += byte;
        psMetaData->uGlobalOffset |= byte << 16;
        psMetaData->uGlobalOffset &= 0xFFFF0000;
    } while (0);
    return retval;
}

// read record type 05: Start Linear Address Record
// ----------------------------------------------------------------------------
static int32_t read_startlinearaddr_rec(FILE*                  psFile,
                                        ihex_data_t*           psMetaData)
{
    int32_t retval = e_ihex_success;
    uint8_t byte = 0;

    do {
        psMetaData->eStartAddressMode = e_addressmode_linear;
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += byte;
        psMetaData->uStartAddress = byte << 24;
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += byte;
        psMetaData->uStartAddress |= byte << 16;
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += byte;
        psMetaData->uStartAddress |= byte << 8;
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += byte;
        psMetaData->uStartAddress |= byte;
    } while (0);
    return retval;
}

// read an entire line of the IHEX file and interpret it's content
// ----------------------------------------------------------------------------
static int32_t read_line(FILE*                  psFile,
                         ihex_data_t*           psMetaData,
                         ihex_write_byte_func   pWriteByte,
                         void*                  pvArg)
{
    int32_t     retval      = e_ihex_success;
    uint8_t     byte        = 0,
                rectype     = 0;
    char        lineendchar = '\0';

    do {
        psMetaData->uLineDataLength = 0;
        psMetaData->uLineOffset = 0;
        psMetaData->uLineChkSum = 0;
        // read record mark, every line starts with
        if ((retval = read_mark(psFile)) != e_ihex_success) {
            break;
        }

        // read record length
        if ((retval = read_byte(psFile, &psMetaData->uLineDataLength)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += psMetaData->uLineDataLength;

        // read record offset
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineOffset = byte << 8;
        psMetaData->uLineChkSum += byte;
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineOffset |= byte;
        psMetaData->uLineChkSum += byte;

        // read record type
        if ((retval = read_byte(psFile, &rectype)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += rectype;

        switch (rectype) {
        case 0:
            retval = read_data_rec(psFile, psMetaData, pWriteByte, pvArg); break;
        case 1:
            break; // nothing more
        case 2:
            retval = read_extsegaddr_rec(psFile, psMetaData); break;
        case 3:
            retval = read_startsegaddr_rec(psFile, psMetaData); break;
        case 4:
            retval = read_extlinearaddr_rec(psFile, psMetaData); break;
        case 5:
            retval = read_startlinearaddr_rec(psFile, psMetaData); break;
        default:
            retval = e_ihex_error_invalid_rectype;
            return retval;
        }

        // read line checksum
        if ((retval = read_byte(psFile, &byte)) != e_ihex_success) {
            break;
        }
        psMetaData->uLineChkSum += byte;
        // sum of checksum of data and checksum byte ate the line ending has
        // to be zero.
        if (psMetaData->uLineChkSum != 0) {
            retval = e_ihex_error_checksum;
            break;
        }

        // read line ending
        if (fread(&lineendchar, sizeof(lineendchar), 1, psFile) == 1) {
            if (lineendchar != '\n' && lineendchar != '\r') {
                retval = e_ihex_error_chars_at_eol;
                break;
            }
        }
    } while (0);

    return retval;
}

void write_byte_to_mem(uint32_t uAddress, uint8_t uByte, void* pvArg)
{
    meminfo_t* mem = pvArg;

    if (mem != NULL) {
        if (mem->puPtr && uAddress < mem->uSize) {
            mem->puPtr[uAddress] = uByte;
        }
    }
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int32_t ihex_read_file (const char*             pcFilename,
                        uint32_t*               puStartAddress,
                        uint32_t*               puFirstAddress,
                        uint32_t*               puLastAddress,
                        ihex_write_byte_func    pWriteByte,
                        void*                   pvArg)
{
    int32_t     retval;
    FILE*       handle;
    uint32_t    line = 0;
    ihex_data_t metadata;

    do {
        metadata.eMode = e_addressmode_linear;
        metadata.uGlobalOffset = 0;
        metadata.uFirstAddress = 0xFFFFFFFF;
        metadata.uLastAddress = 0x00000000;
        metadata.uStartAddress = 0x00000000;

        // try to open the file for reading
        handle = fopen(pcFilename, "r");
        if (handle == NULL) {
            retval = e_ihex_error_file_open;
            break;
        }

        // read the file line by line
        do {
            retval = read_line(handle, &metadata, pWriteByte, pvArg);
            line++;
        } while (retval == e_ihex_success);

        // skip end of file error, because it is normal
        if (retval == e_ihex_eof) retval = e_ihex_success;
    } while (0);

    // on success return metadata
    if (retval == e_ihex_success) {
        if (puStartAddress != NULL) *puStartAddress = metadata.uStartAddress;
        if (puFirstAddress != NULL) *puFirstAddress = metadata.uFirstAddress;
        if (puLastAddress != NULL) *puLastAddress = metadata.uLastAddress;
    }

    // if file has been opened, close it
    if (handle != NULL) fclose(handle);

    return retval;
}


int32_t ihex_read_file_mem (const char*             pcFilename,
                            uint32_t*               puStartAddress,
                            uint32_t*               puFirstAddress,
                            uint32_t*               puLastAddress,
                            uint8_t*                puTargetMemory,
                            uint32_t                uTargetMemorySize)
{
    meminfo_t mem;

    mem.puPtr = puTargetMemory;
    mem.uSize = uTargetMemorySize;

    return ihex_read_file(pcFilename,
                          puStartAddress,
                          puFirstAddress,
                          puLastAddress,
                          write_byte_to_mem, &mem);
}
/** @} */
