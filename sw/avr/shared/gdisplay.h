/**
 * @addtogroup DISPLAY
 * @addtogroup GDISPLAY
 * @brief Graphical LCD routines.
 *
 * Contains functions to display content on a graphic display.
 * Supports ST7565 driver chip.
 *
 * Supports pcbconfig.h for individual PCB settings of the LCD pinout.
 * Supports appconfig.h for individual settings depending on the application.
 *
 * @{
 * @file    gdisplay.h
 * @brief   Graphical LCD routines.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _GDISPLAY_H_
#define _GDISPLAY_H_

// --- Include section ---------------------------------------------------------

#include <avr/io.h>

#include "prjtypes.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

/**
 * @subsection DISP_ST7565_PCBCONFIG
 * Configure LCD Pinout.
 * @{
 */
#ifndef GDISPLAY_PCBCONFIG
 #define GDISPLAY_PCBCONFIG  1
 //! Defines the DDR of a port which contains the backlight LED signal.
 #define DISP_DDR_LED   DDRD
 //! Defines the PORT which contains the backlight LED signal.
 #define DISP_PORT_LED  PORTD
 //! Defines the LED pin.
 #define DISP_LED       PD7
#endif // GDISPLAY_PCBCONFIG
/** @} */

/**
 * @subsection GDISPLAY_APPCONFIG
 * Application specific settings of the graphical display.
 * @{
 */
#ifndef GDISPLAY_APPCONFIG
 #define GDISPLAY_APPCONFIG 1
 #define GDISP_RAM_PAGES    4
 #define GDISP_WIDTH        132
 #define GDISP_HEIGHT       32
#endif // GDISPLAY_APPCONFIG
/** @} */

#define gdisp_put_text_8x8P(__str) gdisp_put_text_8x8p(PSTR(__str))

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern const uint8_t gdisp_font1_x8[];
extern const uint8_t gdisp_font1_x16[];
extern const uint8_t gdisp_font1_x24[];
extern const uint8_t gdisp_symbols_x16[];
extern const uint8_t gdisp_symbols_x24[];

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void            gdisp_initialize    (void);

void            gdisp_backlight     (uint8_t                level);

void            gdisp_choose_font   (const uint8_t*         font);

void            gdisp_goto_col_line (uint8_t                col,
                                     uint8_t                page);

void            gdisp_put_spacer    (uint8_t                height);

void            gdisp_put_char      (const char             single_char);

void            gdisp_put_text      (const char*            text);

void            gdisp_put_textp     (const char*            text);

#endif // _GDISPLAY_H_
/**
 * @}
 * @}
 */
