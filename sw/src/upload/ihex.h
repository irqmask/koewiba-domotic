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
#ifndef _IHEX_H_
#define _IHEX_H_

// --- Include section ---------------------------------------------------------

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef void (*ihex_write_byte_func)(uint32_t uAddress, uint8_t uByte, void* pvArg);

typedef enum {
    e_ihex_eof = -1,
    e_ihex_success = 0,
    e_ihex_error_file_open = 100,
    e_ihex_error_unexpected_char,
    e_ihex_error_unexpected_eol,
    e_ihex_error_unexpected_eof,
    e_ihex_error_line_mark,
    e_ihex_error_chars_at_eol,
    e_ihex_error_invalid_rectype,
    e_ihex_error_checksum,
} eIHEX_Error;



// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int32_t ihex_read_file (const char*             pcFilename,
                        uint32_t*               puStartAddress,
                        uint32_t*               puFirstAddress,
                        uint32_t*               puLastAddress,
                        ihex_write_byte_func    pWriteByte,
                        void*                   pvArg);

int32_t ihex_read_file_mem (const char*             pcFilename,
                            uint32_t*               puStartAddress,
                            uint32_t*               puFirstAddress,
                            uint32_t*               puLastAddress,
                            uint8_t*                puTargetMemory,
                            uint32_t                uTargetMemorySize);

#ifdef __cplusplus
}
#endif
#endif // _IHEX_H_
/** @} */
