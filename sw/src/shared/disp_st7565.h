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

void            st7565_display_on   (BOOL                   on);

void            st7565_start_line   (uint8_t                start_line);

void            st7565_page_addr    (uint8_t                page_address);

void            st7565_column_addr  (uint8_t                column_address);

void            st7565_write_data   (uint8_t                data);

void            st7565_adc_select   (BOOL                   reverse);

void            st7565_disp_reverse (BOOL                   reverse);

void            st7565_disp_all_pixel(BOOL                  all_pixel_on);

void            st7565_lcd_bias     (uint8_t                bias);

void            st7565_reset        (void);

void            st7565_com_select   (BOOL                   reverse);

void            st7565_power_ctrl   (BOOL                   booster,
                                     BOOL                   voltage_regulator,
                                     BOOL                   voltage_follower);

void            st7565_voltage_resistor_ratio
                                    (uint8_t                ratio);

void            st7565_el_volume_reg_set
                                    (uint8_t                level);

void            st7565_static_indicator_reg_set
                                    (BOOL                   on,
                                     uint8_t                state);

void            st7565_booster_ratio_reg_set
                                    (uint8_t                level);

void            st7565_nop          (void);

void            st7565_initialize   (void);

#endif // _DISP_ST7565_H_
/** 
 * @} 
 * @}
 */
