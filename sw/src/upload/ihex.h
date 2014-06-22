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

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

typedef (void)(*ihex_write_byte_func)(uint32_t uAddress, uint8_t uByte);

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int32_t ihex_read_file (const char*             pcFilename,
                        uint32_t*               puStartAddress,
                        uint32_t*               puLastAddress,
                        ihex_write_byte_func    pWriteByte);

#endif // _IHEX_H_
/** @} */
