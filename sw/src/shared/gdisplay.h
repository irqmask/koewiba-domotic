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
 #define GDISP_RAM_PAGES    5
 #define GDISP_WIDTH        100
 #define GDISP_HEIGHT       64
#endif // GDISPLAY_APPCONFIG
/** @} */

#define GDISP_vPutText8x8P(__str) GDISP_vPutText8x8p(PSTR(__str))

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

extern uint8_t GDISP_auFont1_8x8[];

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void            GDISP_vInit         (void);

void            GDISP_vBacklight    (uint8_t                uLevel);

void            GDISP_vGotoColPage  (uint8_t               uCol,
                                     uint8_t               uPage);
                                     
void            GDISP_vPutText8x8   (const char*           pcText);

void            GDISP_vPutText8x8p  (const char*           pcText);   
                                  
#endif // _GDISPLAY_H_
/** 
 * @} 
 * @}
 */
