/**
 * @addtogroup DISPLAY
 * @addtogroup DISPDRV_ST7565
 * @brief Driver for graphical LCD chip ST7565.
 *
 * @{
 * @file    disp_st7565.c
 * @brief   Driver for graphical LCD chip ST7565.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "prjtypes.h"
#include "spi.h"
#include "ucontroller.h"

#include "disp_st7565.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

void st7565_display_on (BOOL on)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    if (on) {
        spi_transmit_blk(0b10101111);
    } else {
        spi_transmit_blk(0b10101110);
    }        
    DISP_PORT_SS |= (1<<DISP_SS);
}

void st7565_start_line (uint8_t start_line)
{
    start_line &= 0b00111111;
    
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(0b01000000 | start_line);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void st7565_page_addr (uint8_t page_address)
{
    page_address &= 0b00001111;
    
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(0b10110000 | page_address);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void st7565_column_addr (uint8_t column_address)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(0b00010000 | (column_address >> 4));
    DISP_PORT_SS |= (1<<DISP_SS);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(column_address & 0x0F);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void st7565_write_data (uint8_t data)
{
    DISP_PORT_A0 |= (1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(data);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void st7565_adc_select (BOOL reverse)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    if (reverse) {
        spi_transmit_blk(0b10100001);
    } else {
        spi_transmit_blk(0b10100000);
    }
    DISP_PORT_SS |= (1<<DISP_SS);
}

void st7565_disp_reverse (BOOL reverse)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    if (reverse) {
        spi_transmit_blk(0b10100111);
    } else {
        spi_transmit_blk(0b10100110);
    }
    DISP_PORT_SS |= (1<<DISP_SS);
}

void st7565_disp_all_pixel (BOOL all_pixel_on)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    if (all_pixel_on) {
        spi_transmit_blk(0b10100101);
    } else {
        spi_transmit_blk(0b10100100);
    }
    DISP_PORT_SS |= (1<<DISP_SS);
}

void st7565_lcd_bias (uint8_t bias)
{
    bias &= 0b00000001;
    
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(0b10100010 | bias);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void st7565_reset (void)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(0b11100010);
    DISP_PORT_SS |= (1<<DISP_SS);
}
    
void st7565_com_select (BOOL reverse)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    if (reverse) {
        spi_transmit_blk(0b11001000);
    } else {
        spi_transmit_blk(0b11000000);
    }
    DISP_PORT_SS |= (1<<DISP_SS);
}

void st7565_power_ctrl (BOOL booster,
                        BOOL voltage_regulator,
                        BOOL voltage_follower)
{
   uint8_t val;
   
   val = 0b00101000;
   if (booster) val |= 0b00000100;
   if (voltage_regulator) val |= 0b00000010;
   if (voltage_follower) val |= 0b00000001;
   
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(val);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void st7565_voltage_resistor_ratio (uint8_t ratio)
{
    ratio &= 0b00000111;
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(0b00100000 | ratio);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void st7565_el_volume_reg_set (uint8_t level)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    // electronic volume mode set
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(0b10000001);
    DISP_PORT_SS |= (1<<DISP_SS);
    // electronic volume register set
    level &= 0b00111111;
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(level);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void st7565_static_indicator_reg_set (BOOL      on,
                                      uint8_t   state)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    // static indicator
    DISP_PORT_SS &= ~(1<<DISP_SS);
    if (on) {
        spi_transmit_blk(0b10101101);
    } else {
        spi_transmit_blk(0b10101100);
    }
    DISP_PORT_SS |= (1<<DISP_SS);
    // static indicator state
    state &= 0b00000011;
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(state);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void st7565_booster_ratio_reg_set (uint8_t level)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    // booster ratio mode set
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(0b11111000);
    DISP_PORT_SS |= (1<<DISP_SS);
    // booster ratio register set
    level &= 0b00000011;
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(level);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void st7565_nop (void)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    spi_transmit_blk(0b11100011);
    DISP_PORT_SS |= (1<<DISP_SS);    
}

void st7565_initialize (void)
{
    // set port data directions and initial levels
    DISP_DDR_A0 |= (1<<DISP_A0);
    DISP_DDR_SS |= (1<<DISP_SS);
    DISP_DDR_RES |= (1<<DISP_RES);
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS |= (1<<DISP_SS);
    
    // Reset the display controller
    DISP_PORT_RES &= ~(1<<DISP_RES);
    _delay_us(250);
    DISP_PORT_RES |= (1<<DISP_RES);
    _delay_us(1000);
    
    st7565_lcd_bias(0);                 // bias 1/9   
    st7565_adc_select(TRUE);            // reverse orientation
    st7565_com_select(FALSE);           // 6 o'clock mode
    
    st7565_voltage_resistor_ratio(3);   // set voltage regulator resistor ratio
    st7565_el_volume_reg_set(0x1F);     // set volume mode register

    st7565_power_ctrl(TRUE, TRUE, TRUE);// power control mode: all features on

    st7565_disp_reverse(FALSE);         // display mode positive
    st7565_static_indicator_reg_set(TRUE, 0); // switch indicator off, no blinking
    st7565_display_on(TRUE);            // switch display on  
}
/** @} */
