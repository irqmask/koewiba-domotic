/**
 * @addtogroup PRJCONF
 * @brief Define project environment configuration.
 *
 * Is this a windows, linux, AVR microcontroller etc environment?
 * @{
 * @file    prjconf.h
 * @brief   Define project environment configuration.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

#ifndef _PRJCONF_H__
#define _PRJCONF_H__

// --- Include section ---------------------------------------------------------

// --- Definitions -------------------------------------------------------------

#if defined (_WIN32) || defined (WIN32) // note the underscore: without it, it's not msdn official!
    // Windows (x64 and x86)
    #define PRJCONF_WINDOWS
#elif __unix__ // all unixes
    // Unix
    #define PRJCONF_UNIX
#elif __posix__
    // POSIX
    #define PRJCONF_POSIX
#elif __linux__
    // linux
    #define PRJCONF_LINUX
#elif __APPLE__
    // Mac OS, not sure if this is covered by __posix__ and/or __unix__ though...
    #define PRJCONF_APPLE
#elif defined (__AVR_ATmega8__)    || \
      defined (__AVR_ATmega88__)   || \
      defined (__AVR_ATmega88A__)  || \
      defined (__AVR_ATmega88P__)  || \
      defined (__AVR_ATmega328__)  || \
      defined (__AVR_ATmega328P__) || \
      defined (__AVR_ATmega324P__) || \
      defined (__AVR_ATmega324A__) || \
      defined (__AVR_ATtiny1634__)
    #define PRJCONF_UC_AVR
#else
    #define PRJCONF_OTHER
#endif

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#endif /* _PRJCONF_H__ */
/** @} */
