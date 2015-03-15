/**
 * @addtogroup SPI
 * @brief Serial peripheral interface driver.
 *
 * Contains functions to send and receive data over the SPI interface. Two
 * versions exist: A blocking version which halts the application until the data
 * is transmitted and a non-blocking version, which does the transmission in
 * background. In this case optional start and end-callbacks are called at the
 * beginning and at the end of the transmission.
 *
 * Only use the non-blocking option if the CPU frequency is at least times 10
 * faster than the SPI frequency. Otherwise the control overhead takes more CPU
 * time than the transmission itself.
 *
 * Supports pcbconfig.h for individual PCB settings of the SPI pinout.
 * Supports appconfig.h for individual settings depending on the application.
 *
 * @{
 * @file    spi.h
 * @brief   Serial peripheral interface driver.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
#ifndef _SPI_H_
#define _SPI_H_

// --- Include section ---------------------------------------------------------

#include <avr/io.h>

#include "prjtypes.h"
#include "ucontroller.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

/**
 * @subsection SPI_PCBCONFIG
 * Configure SPI Pinout.
 * @{
 */
#ifndef SPI0_PCBCONFIG
 #define SPI0_PCBCONFIG  1
 //! Defines the DDR of a port which contains the MOSI signal.
 #define SPI0_DDR_MOSI   DDRB
 //! Defines the DDR of a port which contains the MISO signal.
 #define SPI0_DDR_MISO   DDRB
 //! Defines the DDR of a port which contains the SCK signal.
 #define SPI0_DDR_SCK    DDRB
 //! Defines the DDR of a port which contains the standard slave select signal.
 #define SPI0_DDR_SS     DDRB
 //! Defines the PORT which contains the standard slave select signal.
 #define SPI0_PORT_SS    PORTB
 //! Defines the MOSI pin.
 #define SPI0_MOSI       PB5
 //! Defines the MISO pin.
 #define SPI0_MISO       PB6
 //! Defines the SCK pin.
 #define SPI0_SCK        PB7
 //! Defines the SS (slave select) pin.
 #define SPI0_SS         PB4
 //! data register of spi0 
 #define SPI0_DATA_REG   REG_SPDR0

 #if defined (__AVR_ATtiny1634__)
  #define SPI0_TRANSMITION_COMPLETE    (UCSR0A & (1<<UDRE0))
  #define SPI0_RECEPTION_COMPLETE      (UCSR0A & (1<<RXC0 ))
 #else // defined (__AVR_ATtiny1634__)
  #define SPI0_TRANSMITION_COMPLETE    ((REG_SPSR0) & (1 << REGBIT_SPIF0))
  #define SPI0_RECEPTION_COMPLETE      ((REG_SPSR0) & (1 << REGBIT_SPIF0))
 #endif // defined (__AVR_ATtiny1634__)
#endif // SPI0_PCBCONFIG

#if defined (__AVR_ATtiny1634__)
 #ifndef SPI1_PCBCONFIG
  #define SPI1_PCBCONFIG  1
  //! Defines the DDR of a port which contains the MOSI signal.
  #define SPI1_DDR_MOSI   DDRB
  //! Defines the DDR of a port which contains the MISO signal.
  #define SPI1_DDR_MISO   DDRB
  //! Defines the DDR of a port which contains the SCK signal.
  #define SPI1_DDR_SCK    DDRB
  //! Defines the DDR of a port which contains the standard slave select signal.
  #define SPI1_DDR_SS     DDRB
  //! Defines the PORT which contains the standard slave select signal.
  #define SPI1_PORT_SS    PORTB
  //! Defines the MOSI pin.
  #define SPI1_MOSI       PB5
  //! Defines the MISO pin.
  #define SPI1_MISO       PB6
  //! Defines the SCK pin.
  #define SPI1_SCK        PB7
  //! Defines the SS (slave select) pin.
  #define SPI1_SS         PB4
  //! data register of spi1 
  #define SPI1_DATA_REG   REG_SPDR1
  #define SPI1_TRANSMITION_COMPLETE    (UCSR1A & (1<<UDRE1))
  #define SPI1_RECEPTION_COMPLETE      (UCSR1A & (1<<RXC1 ))
 #endif // SPI0_PCBCONFIG
#endif // defined (__AVR_ATtiny1634__)

/** @} */

/**
 * @subsection SPI_APPCONFIG
 * Configure size of the SPI driver. E.g. only blocking functions.
 * @{
 */
#ifndef SPI_APPCONFIG
 #define SPI_APPCONFIG 1
 #define SPI_WITH_BLOCKING  1
 #undef  SPI_WITH_IRQ
 #define SPI_QUEUE_SIZE     32  //!< Size of the SPI queue. Per defined slave an
                                //!< overhead of 2 bytes is additionally used.
 #define SPI_NUM_SLAVES     2   //!< number of slaves connected to SPI. From 0
                                //!< up to 8 slaves can be configured.
#endif // SPI_APPCONFIG
/** @} */

// --- Type definitions --------------------------------------------------------

/**
 * Function pointer type for the routine which is called before the physical SPI
 * transmission is started.
 * It can be used e.g. to set the slave select line.
 * @param[in] arg
 * User specific byte which has been passed to the SPI_uTransmit() function.
 */
typedef void (*spi_start_send_func_t)(uint8_t arg);
/**
 * Function pointer type for the routine which is called after the physical SPI
 * transmission has ended.
 * It can be used e.g. to reset the slave select line.
 * @note This function is called from an interrupt context!
 * @param[in] arg
 * User specific byte which has been passed to the SPI_uTransmit() function.
 */
typedef void (*spi_end_send_func_t)(uint8_t arg);

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#ifdef SPI_WITH_BLOCKING

/**
 * Initialize the SPI as master.
 */
#if defined (__AVR_ATmega8__)    || \
    defined (__AVR_ATmega88__)   || \
    defined (__AVR_ATmega88A__)  || \
    defined (__AVR_ATmega88P__)  || \
    defined (__AVR_ATmega324P__) || \
    defined (__AVR_ATmega324A__)    

inline void     spi0_master_init_blk  (void)
{
    // Set MOSI and SCK output, all others input
    SPI0_DDR_MISO &= ~(1 << SPI0_MISO);
    SPI0_DDR_MOSI |=  (1 << SPI0_MOSI);
    SPI0_DDR_SCK  |=  (1 << SPI0_SCK);
    SPI0_DDR_SS   |=  (1 << SPI0_SS);    // set DDR for slave select as output to guarantee SPI master mode
    SPI0_PORT_SS  |=  (1 << SPI0_SS);    // set slave select to 1 (slave disabled)
    REG_SPSR0     =  (1 << REGBIT_SPI2X0);
    // Enable SPI, Master, set clock rate fck/2
    REG_SPCR0     =  (1 << REGBIT_SPE0) | (1 << REGBIT_MSTR0);
}
#endif

#if defined (__AVR_ATtiny1634__)

inline void     spi0_master_init_blk  (void)
{
    UBRR0 = 0;
    /* Setting the XCKn port pin as output, enables master mode. */
    SPI0_SCK_DDR |= (1<<SPI0_SCK_PIN);
    /* Set MSPI mode of operation and SPI data mode 0. */
    UCSR0C = (1<<UMSEL01)|(1<<UMSEL00)|(0<<UCPHA0)|(0<<UCPOL0);
    /* Enable receiver and transmitter. */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set baud rate. */
    /* IMPORTANT: The Baud Rate must be set after the transmitter is enabled
    */
    UBRR0 = 0;
}

inline void     spi1_master_init_blk  (void)
{
    UBRR1 = 0;
    /* Setting the XCKn port pin as output, enables master mode. */
    SPI1_SCK_DDR |= (1<<SPI1_SCK_PIN);
    /* Set MSPI mode of operation and SPI data mode 0. */
    UCSR1C = (1<<UMSEL11)|(1<<UMSEL11)|(0<<UCPHA1)|(0<<UCPOL1);
    /* Enable receiver and transmitter. */
    UCSR1B = (1<<RXEN1)|(1<<TXEN1);
    /* Set baud rate. */
    /* IMPORTANT: The Baud Rate must be set after the transmitter is enabled
    */
    UBRR1 = 0;
}
#endif // defined (__AVR_ATtiny1634__)

/**
 * Simple blocking SPI transmission. The function returns when the byte has been
 * sent.
 *
 * @param[in] data
 * Byte to be sent over SPI0.
 * @returns Byte received through MISO line during transmission.
 */
inline uint8_t  spi0_transmit_blk    (uint8_t                data )
{
    // Start transmission
    SPI0_DATA_REG = data;
    // Wait for transmission complete
    while ( !(SPI0_TRANSMITION_COMPLETE) );
    return SPI0_DATA_REG; // SPDR contains received byte during transmission.
}

#if defined (__AVR_ATtiny1634__)
/**
 * Simple blocking SPI transmission. The function returns when the byte has been
 * sent.
 *
 * @param[in] data
 * Byte to be sent over SPI1.
 * @returns Byte received through MISO line during transmission.
 */
inline uint8_t  spi1_transmit_blk    (uint8_t                data )
{
    // Start transmission
    while ( !(SPI1_TRANSMITION_COMPLETE) );
    SPI1_DATA_REG = data;
    // Wait for transmission complete
    while ( !(SPI1_RECEPTION_COMPLETE) );
    return SPI1_DATA_REG; // SPDR contains received byte during transmission.
}
#endif // defined (__AVR_ATtiny1634__)

#endif // SPI_WITH_BLOCKING


#ifdef SPI_WITH_IRQ
void            spi_master_init     (void);

uint8_t         spi_transmit        (uint8_t*               send_buf,
                                     uint8_t*               receive_buf,
                                     uint8_t                length,
                                     spi_start_send_func_t  before_transmission_func,
                                     spi_end_send_func_t    after_transmission_func,
                                     uint8_t                user_arg);

uint8_t         spi_queue_space     (void);

BOOL            spi_is_busy         (void);

#endif // SPI_WITH_IRQ

#endif // _SPI_H_
/** @} */
