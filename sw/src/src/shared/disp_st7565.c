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

void            ST7565_vDispOn      (BOOL                   bOn)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    if (bOn) {
        SPI_uTransmitBlk(0b10101111);
    } else {
        SPI_uTransmitBlk(0b10101110);
    }        
    DISP_PORT_SS |= (1<<DISP_SS);
}

void            ST7565_vStartLine   (uint8_t                uStartLine)
{
    uStartLine &= 0b00111111;
    
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    SPI_uTransmitBlk(0b01000000 | uStartLine);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void            ST7565_vPageAddr    (uint8_t                uPageAddress)
{
    uPageAddress &= 0b00001111;
    
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    SPI_uTransmitBlk(0b10110000 | uPageAddress);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void            ST7565_vColumnAddr  (uint8_t                uColumnAddress)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    SPI_uTransmitBlk(0b00010000 | (uColumnAddress >> 4));
    DISP_PORT_SS |= (1<<DISP_SS);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    SPI_uTransmitBlk(uColumnAddress & 0x0F);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void            ST7565_vWriteData   (uint8_t                uData)
{
    DISP_PORT_A0 |= (1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    SPI_uTransmitBlk(uData);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void            ST7565_vADCSelect   (BOOL                   bReverse)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    if (bReverse) {
        SPI_uTransmitBlk(0b10100001);
    } else {
        SPI_uTransmitBlk(0b10100000);
    }
    DISP_PORT_SS |= (1<<DISP_SS);
}

void            ST7565_vDispReverse (BOOL                   bReverse)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    if (bReverse) {
        SPI_uTransmitBlk(0b10100111);
    } else {
        SPI_uTransmitBlk(0b10100110);
    }
    DISP_PORT_SS |= (1<<DISP_SS);
}

void            ST7565_vDispAllPixel(BOOL                   bAllPixOn)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    if (bAllPixOn) {
        SPI_uTransmitBlk(0b10100101);
    } else {
        SPI_uTransmitBlk(0b10100100);
    }
    DISP_PORT_SS |= (1<<DISP_SS);
}

void            ST7565_vLCDBias     (uint8_t                uBias)
{
    uBias &= 0b00000001;
    
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    SPI_uTransmitBlk(0b10100010 | uBias);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void            ST7565_vReset       (void)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    SPI_uTransmitBlk(0b11100010);
    DISP_PORT_SS |= (1<<DISP_SS);
}
    
void            ST7565_vCOMSelect   (BOOL                   bReverse)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    if (bReverse) {
        SPI_uTransmitBlk(0b11001000);
    } else {
        SPI_uTransmitBlk(0b11000000);
    }
    DISP_PORT_SS |= (1<<DISP_SS);
}

void            ST7565_vPowerCtrl   (BOOL                   bBooster,
                                     BOOL                   bVoltageRegulator,
                                     BOOL                   bVoltageFollower)
{
   uint8_t val;
   
   val = 0b00101000;
   if (bBooster) val |= 0b00000100;
   if (bVoltageRegulator) val |= 0b00000010;
   if (bVoltageFollower) val |= 0b00000001;
   
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    SPI_uTransmitBlk(val);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void            ST7565_vVoltageResistorRatio
                                    (uint8_t                uRatio)
{
    uRatio &= 0b00000111;
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    SPI_uTransmitBlk(0b00100000 | uRatio);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void            ST7565_vElVolumeRegSet
                                    (uint8_t                uLevel)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    // electronic volume mode set
    DISP_PORT_SS &= ~(1<<DISP_SS);
    SPI_uTransmitBlk(0b10000001);
    DISP_PORT_SS |= (1<<DISP_SS);
    // electronic volume register set
    uLevel &= 0b00111111;
    DISP_PORT_SS &= ~(1<<DISP_SS);
    SPI_uTransmitBlk(uLevel);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void            ST7565_vStaticIndicatorRegSet
                                    (BOOL                   bOn,
                                     uint8_t                uState)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    // static indicator
    DISP_PORT_SS &= ~(1<<DISP_SS);
    if (bOn) {
        SPI_uTransmitBlk(0b10101101);
    } else {
        SPI_uTransmitBlk(0b10101100);
    }
    DISP_PORT_SS |= (1<<DISP_SS);
    // static indicator state
    uState &= 0b00000011;
    DISP_PORT_SS &= ~(1<<DISP_SS);
    SPI_uTransmitBlk(uState);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void            ST7565_vBoosterRatioRegSet
                                    (uint8_t                uLevel)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    // booster ratio mode set
    DISP_PORT_SS &= ~(1<<DISP_SS);
    SPI_uTransmitBlk(0b11111000);
    DISP_PORT_SS |= (1<<DISP_SS);
    // booster ratio register set
    uLevel &= 0b00000011;
    DISP_PORT_SS &= ~(1<<DISP_SS);
    SPI_uTransmitBlk(uLevel);
    DISP_PORT_SS |= (1<<DISP_SS);
}

void            ST7565_vNOP         (void)
{
    DISP_PORT_A0 &= ~(1<<DISP_A0);
    DISP_PORT_SS &= ~(1<<DISP_SS);
    SPI_uTransmitBlk(0b11100011);
    DISP_PORT_SS |= (1<<DISP_SS);    
}

void            ST7565_vInit        (void)
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
    
    ST7565_vLCDBias(0);                 // bias 1/9   
    ST7565_vADCSelect(TRUE);            // reverse orientation
    ST7565_vCOMSelect(FALSE);           // 6 o'clock mode
    
    ST7565_vVoltageResistorRatio(3);    // set voltage regulator resistor ratio
    ST7565_vElVolumeRegSet(0x1F);       // set volume mode register

    ST7565_vPowerCtrl(TRUE, TRUE, TRUE);// power control mode: all features on

    ST7565_vDispReverse(FALSE);         // display mode positive
    ST7565_vStaticIndicatorRegSet(TRUE, 0); // switch indicator off, no blinking
    ST7565_vDispOn(TRUE);               // switch display on  
}
/** @} */
