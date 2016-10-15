/**
 * @addtogroup SYSERROR
 *
 * @{
 * @file    syserror.c
 * @brief   Get error information of errors thrown by the operating system.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <stdio.h>
#include "syserror.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#if defined (PRJCONF_WINDOWS)
void sys_err_get_as_string (char* buffer, size_t buffersize)
{
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, buffersize-1, NULL);
    buffer[buffersize - 1] = '\0';
}
#else
void sys_err_get_as_string (char* buffer, size_t buffersize)
{
    snprintf(buffer, buffersize-1, "sys_err_get_as_string() not implemented!";
    buffer[buffersize - 1] = '\0';
}
#endif

void sys_err_print_last (void)
{
    char buffer[256];
    sys_err_get_as_string(buffer, sizeof(buffer));
    fprintf(stderr, "SYSTEM ERROR: %s\n", buffer);
}
/** @} */
