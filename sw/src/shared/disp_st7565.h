/**
 * @addtogroup DISPLAY
 * @addtogroup DISPDRV_ST7565
 * @brief Driver for graphical LCD chip ST7565.
 *
 * Contains functions to send the commands via SPI to the ST7565 chip. The chip
 * in SPI mode is write-only.
 * The blocking functions of the SPI driver are used to send commands to the chip.
 *
 * Supports pcbconfig.h for individual PCB settings of the LCD and SPI pinout.
 * Supports appconfig.h for individual settings depending on the application.
 *
 * @{
 * @file    disp_st7565.h
 * @brief   Driver for graphical LCD chip ST7565.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _DISP_ST7565_H_
#define _DISP_ST7565_H_

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
#ifndef DISP_ST7565_PCBCONFIG
 #define DISP_ST7565_PCBCONFIG  1
 //! Defines the DDR of a port which contains the slave select signal of the 
 //! display.
 #define DISP_DDR_SS    DDRB
 //! Defines the DDR which contains the A0 signal of the display.
 #define DISP_DDR_A0    DDRD
 //! Defines the DDR which contains the RES signal of the display.
 #define DISP_DDR_RES   DDRD
 //! Defines the PORT which contains the slave select signal of the display.
 #define DISP_PORT_SS   PORTB
 //! Defines the PORT which contains the A0 signal of the display.
 #define DISP_PORT_A0   PORTD
 //! Defines the PORT which contains the RES signal of the display. 
 #define DISP_PORT_RES  PORTD
 //! Defines the SS pin.
 #define DISP_SS           PB3
 //! Defines the A0 pin.
 #define DISP_A0           PD6
 //! Defines the reset pin
 #define DISP_RES          PD5
#endif // DISP_ST7565_PCBCONFIG
/** @} */

/**
 * @subsection DISP_ST7565_APPCONFIG
 * Configure size of the SPI driver. E.g. only blocking functions.
 * @{
 */
#ifndef DISP_ST7565_APPCONFIG
 #define DISP_ST7565_APPCONFIG 1
#endif // DISP_ST7565_APPCONFIG
/** @} */

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void            ST7565_vDispOn      (BOOL                   bOn);

void            ST7565_vStartLine   (uint8_t                uStartLine);

void            ST7565_vPageAddr    (uint8_t                uPageAddress);

void            ST7565_vColumnAddr  (uint8_t                uColumnAddress);

void            ST7565_vWriteData   (uint8_t                uData);

void            ST7565_vADCSelect   (BOOL                   bReverse);

void            ST7565_vDispReverse (BOOL                   bReverse);

void            ST7565_vDispAllPixel(BOOL                   bAllPixOn);

void            ST7565_vLCDBias     (uint8_t                uBias);

void            ST7565_vReset       (void);

void            ST7565_vCOMSelect   (BOOL                   bReverse);

void            ST7565_vPowerCtrl   (BOOL                   bBooster,
                                     BOOL                   bVoltageRegulator,
                                     BOOL                   bVoltageFollower);

void            ST7565_vVoltageResistorRatio
                                    (uint8_t                uRatio);

void            ST7565_vElVolumeRegSet
                                    (uint8_t                uLevel);

void            ST7565_vStaticIndicatorRegSet
                                    (BOOL                   bOn,
                                     uint8_t                uState);

void            ST7565_vBoosterRatioRegSet
                                    (uint8_t                uLevel);

void            ST7565_vNOP         (void);

void            ST7565_vInit        (void);

#endif // _DISP_ST7565_H_
/** 
 * @} 
 * @}
 */
