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

#include <stdio.h>
#include <stdint.h>

#include "ihex.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

int32_t ihex_read_file (const char*             pcFilename,
                        uint32_t*               puStartAddress,
                        uint32_t*               puLastAddress,
                        ihex_write_byte_func    pWriteByte)
{
    int handle;

    do {
        handle = fopen(pcFilename);
    } while (0);

    return 0;
}


/** @} */
