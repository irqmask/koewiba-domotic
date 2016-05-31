/**
 * @addtogroup UART
 * @brief UART driver.
 *
 * @{
 * @file    UART.c
 * @brief   UART driver.
 * Which uart is used is defined in pcbconfig.h. If the controller supports only
 * one uart, uart0 is used as default, otherwise the second uart is used as default.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------

// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>

#include "prjtypes.h"
#include "queue.h"
#include "ucontroller.h"
#include "uart.h"

// --- Definitions -------------------------------------------------------------

//! Calculation of UART baudrate
#define UART_UBRR_CALC(BAUD_,FREQ_) ((FREQ_)/((BAUD_)*16L)-1)

// --- Type definitions --------------------------------------------------------

#ifdef UART_WITH_IRQ
typedef struct uart {
    uint8_t                 auRecvQueue[UART_RECV_QUEUE_SIZE+QUEUE_HEADER_SIZE];
    uint8_t                 auSendQueue[UART_SEND_QUEUE_SIZE+QUEUE_HEADER_SIZE];
    uint8_t                 status;
    queue_t                 q;
} uart_t;
#endif // UART_WITH_IRQ

// --- Local variables ---------------------------------------------------------

#ifdef UART_WITH_IRQ

static uart_t               g_uart;

#endif // UART_WITH_IRQ

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

#ifdef UART_WITH_IRQ
// Start a new UART transmission.
static void start_transmission (void)
{
    uint16_t value = 0;


    // Start transmission
    value = q_get_byte(g_uart.q);
    if (value >> 8) {
        REGISTER_UDR0 = value & 0x00FF;
        REGISTER_UCSRB0 |= (1<<REGBIT_UDRIE);
    }
}

#endif // UART_WITH_IRQ

// convert a 4bit value into a hex-character
static char val2hex(uint8_t value)
{
    if (value <= 9) return '0' + value;
    if (value <= 15) return 'A'- 0x0A + value;
    return '-';
}

// --- Interrupts --------------------------------------------------------------

#ifdef UART_WITH_IRQ
// Interrupt when next byte is ready to be loaded into the UART
ISR(INTERRUPT_USART_UDRE)
{
    uint16_t value = 0;

    value = QUE_uGet(g_uart.auSendQueue);
    if (value >> 8) {
        REGISTER_UDR0 = value & 0x00FF;
    } else {
        REGISTER_UCSRB0 &= ~(1<<REGBIT_UDRIE);
    }
}

// Interrupt when a byte has been received by the UART
ISR(INTERRUPT_USART_RXC)
{
    uint8_t value;

    value = REGISTER_UDR0;
    if (!QUE_uPut(g_uart.auRecvQueue, value)) {
        g_uart.uStatus |= (1<<(eUartFlag_BufOverrun-8));
       // UART_vPutHexBlk (g_uart.auRecvQueue[0]);
       // UART_vPutHexBlk (g_uart.auRecvQueue[1]);
       // UART_vPutHexBlk (g_uart.auRecvQueue[2]);
    }
    // check framing error
    if (REGISTER_UCSRA0 & (1<<REGBIT_FE)) {
        REGISTER_UCSRA0 &= ~(1<<REGBIT_FE);
        g_uart.status |= (1<<(eUartFlag_FramingError-8));
    }
}

#endif // UART_WITH_IRQ

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#ifdef UART_WITH_IRQ

/**
 * Initialize UART 8N1 with interrupts.
 *
 * @param[in] baudrate
 * Baudrate to configure.
 */
void uart_init (uint32_t baudrate)
{
    // set baud-rate
    REGISTER_UBRRH0 = (uint8_t)(UART_UBRR_CALC( baudrate, F_CPU ) >> 8);
    REGISTER_UBRRL0 = (uint8_t)UART_UBRR_CALC( baudrate, F_CPU );

    // enable receiver, receiver interrupt and transmitter
    REGISTER_UCSRB0 = (1<<REGBIT_RXCIE) | (1<<REGBIT_RXEN) | (1<<REGBIT_TXEN);

    // set frame format: asynchronous 8N1
    REGISTER_UCSRC0 = (1 << UCSZ01) | (1 << UCSZ00);

    // Initialize queue and status variables
    QUE_vInit(g_uart.auRecvQueue, sizeof(g_uart.auRecvQueue));
    QUE_vInit(g_uart.auSendQueue, sizeof(g_uart.auSendQueue));
}

/**
 * Check if a UART transmission is ongoing.
 *
 * @returns true, if UART is busy, otherwise false.
 */
bool uart_is_busy (void)
{
    return REGISTER_UCSRB0 & (1<<REGBIT_UDRIE);
}

/**
 * Transmit a amount of data. Data will be enqueued, if there is not
 * enough space in the queue, the function blocks until everything is enqueued.
 *
 * @param[in] data
 * Pointer to data to be sent.
 * @param[in] length
 * Length of data to be sent.
*/
void uart_transmit (uint8_t*               data,
                    uint8_t                length)
{
    bool enqueued;
    while (length--) {
        do {
            enqueued = QUE_uPut(g_uart.auSendQueue, *data);
        } while (enqueued == false);

        REGISTER_UCSRB0 |= (1<<REGBIT_UDRIE);
        data++;
    }
}

/**
 * Puts a char into the queue. This function waits until there is enough space
 * in the queue.
 *
 * @param[in] single_char
 * Character to send.
 */
void uart_put_char (char single_char)
{
    bool enqueued;
    do {
        enqueued = QUE_uPut(g_uart.auSendQueue, single_char);
        queuedump[dumpindex][0] = g_uart.auSendQueue[1]; // writeindex
        queuedump[dumpindex++][1] = g_uart.auSendQueue[2]; // readindex
        if (dumpindex >= 32) dumpindex = 0;
    } while (enqueued == false);
    REGISTER_UCSRB0 |= (1<<REGBIT_UDRIE);
    if (!uart_is_busy()) {
        start_transmission();
    }
    #ifdef UART_AUTOCR
    if ( single_char == '\n' )
        uart_put_char('\r');
    #endif
}

/**
 * Puts a string into the queue. This function waits until there is enough space
 * in the queue. Contents in the queue will be send after the FIFO principle.
 *
 * @param[in] string
 * String to send. String has to be terminated with a null character.
 */
void uart_put_string (const char* string)
{
    while (*string != '\0') {
        uart_put_char(*string++);
    }
}

/**
 * Puts a hexadecimal representation of a byte as a string into the queue.
 * Contents in the queue will be send after the FIFO principle.
 *
 * @param[in] value
 * Byte-value to convert into hex and to be sent.
 */
void uart_put_hex8 (uint8_t value)
{
    uart_put_char(val2hex(value >> 4));
    uart_put_char(val2hex(value & 0x0F));
}

/**
 * Sends a hexadecimal dump of a memory region queued to the UART.
 * Contents in the queue will be send after the FIFO principle.
 *
 * @param[in] data
 * Byte-pointer to memory region.
 * @param[in] length
 * Size in bytes of the memory region.
 */
void uart_hex_dump (const uint8_t*  data,
                    uint8_t         length)
{
    uint8_t ii;

    for (ii=0; ii<length; ii++) {
        uart_put_hex8(data[ii]);
        if (ii>1 && (ii+1)%16==0) {
            uart_put_char('\n');
        } else {
            uart_put_char(' ');
        }
    }
    if (ii%16) uart_put_char('\n');
}

/**
 * Receive a byte.
 *
 * @retval Lower 8bit data, higher 8bit statusflags.
 * @see eUartFlags_t for detailed flags.
 */
uint16_t uart_get_char (void)
{
    uint16_t    value;

    val = QUE_uGet(g_uart.auRecvQueue);
    if (val>>8) {   // "GotByte" flag set?
        val &= 0x00FF;
        val |= g_uart.status << 8;
        g_uart.status &= ~g_uart.status;
    } else {
        val = (1<<eUartFlag_NoData);
    }
    return value;
}

#endif // UART_WITH_IRQ

#ifdef UART_WITH_BLOCKING

/**
 * Initialize UART 0 8N1 without interrupts.
 *
 * @param[in] baudrate
 * Baudrate to configure.
 */
void uart_init_blk0 ( uint32_t baudrate )
{
    REGISTER_UCSRB0 = (1 << TXEN0) | (1 << RXEN0);  // UART TX und RX einschalten
    REGISTER_UCSRC0 = (1 << UCSZ01) | (1 << UCSZ00); // Asynchron 8N1

    REGISTER_UBRRH0 = (uint8_t)(UART_UBRR_CALC( baudrate, F_CPU ) >> 8);
    REGISTER_UBRRL0 = (uint8_t)UART_UBRR_CALC( baudrate, F_CPU );
}

/**
 * Send a char over the UART 0. Before sending a char, this function waits until
 * the UART is ready to send the next char.
 *
 * @param[in] single_char
 * Character to send.
 */
void uart_put_char_blk0 ( char single_char )
{
    if ( single_char == '\n' )
        uart_put_char_blk('\r');

    loop_until_bit_is_set(REGISTER_UCSRA0, UDRE0);
    REGISTER_UDR0 = single_char;
}

/**
 * Sends a string over the UART. Before sending a char, this function waits until
 * the UART is ready to send the next char.
 *
 * @param[in] string
 * String to send. String has to be terminated with a null character.
 */
void uart_put_string_blk0 ( const char *string )
{
    while ( *string != '\0' ) {
        uart_put_char_blk0(*string++);
    }
}

/**
 * Sends a hexadecimal representation of a byte over the UART. Before each
 * character is sent, this function waits until the UART is ready to send
 * the next char.
 *
 * @param[in] value
 * Byte-value to convert into hex and to be sent.
 */
void uart_put_hex8_blk0 (uint8_t value)
{
    uart_put_char_blk0(val2hex(value >> 4));
    uart_put_char_blk0(val2hex(value & 0x0F));
}

/**
 * Sends a hexadecimal dump of a memory region queued over the UART.
 * Before each character is sent, this function waits until the UART is ready
 * to send the next char.
 *
 * @param[in] data
 * Byte-pointer to memory region.
 * @param[in] length
 * Size in bytes of the memory region.
 */
void uart_hex_dump_blk0 ( const uint8_t *data, uint8_t length)
{
    uint8_t ii;

    for (ii=0; ii<length; ii++) {
        uart_put_hex8_blk0(data[ii]);
        if (ii>1 && (ii+1)%16==0) {
            uart_put_char_blk0('\n');
        } else {
            uart_put_char_blk0(' ');
        }
    }
    if (ii%16) uart_put_char_blk0('\n');
}

/**
 * If there is a received char in the UART, whis function will return it. If no
 * char has been received the null-character is received.
 *
 * @returns Received char, otherwise zero.
 */
char uart_get_char_blk0 ( void )
{
    // is char available?
    if ( !(REGISTER_UCSRA0 & (1 << RXC0)) ) {
        // return NUL character if nothing has been received
        return '\0';
    }
    // return received char
    return REGISTER_UDR0;
}

#ifdef HAS_UART1
/**
 * Initialize UART 1 8N1 without interrupts.
 *
 * @param[in] baudrate
 * Baudrate to configure.
 */
void uart_init_blk1 ( uint32_t baudrate )
{
    REGISTER_UCSRB1 = (1 << TXEN1) | (1 << RXEN1);  // UART TX und RX einschalten
    REGISTER_UCSRC1 = (1 << UCSZ11) | (1 << UCSZ10); // Asynchron 8N1

    REGISTER_UBRRH1 = (uint8_t)(UART_UBRR_CALC( baudrate, F_CPU ) >> 8);
    REGISTER_UBRRL1 = (uint8_t)UART_UBRR_CALC( baudrate, F_CPU );
}

/**
 * Send a char over the UART 0. Before sending a char, this function waits until
 * the UART is ready to send the next char.
 *
 * @param[in] single_char
 * Character to send.
 */
void uart_put_char_blk1 ( char single_char )
{
    if ( single_char == '\n' )
        uart_put_char_blk1('\r');

    loop_until_bit_is_set(REGISTER_UCSRA1, UDRE1);
    REGISTER_UDR1 = single_char;
}

/**
 * Sends a string over the UART. Before sending a char, this function waits until
 * the UART is ready to send the next char.
 *
 * @param[in] string
 * String to send. String has to be terminated with a null character.
 */
void uart_put_string_blk1 ( const char *string )
{
    while ( *string != '\0' ) {
        uart_put_char_blk1(*string++);
    }
}

/**
 * Sends a hexadecimal representation of a byte over the UART. Before each
 * character is sent, this function waits until the UART is ready to send
 * the next char.
 *
 * @param[in] value
 * Byte-value to convert into hex and to be sent.
 */
void uart_put_hex8_blk1 (uint8_t value)
{
    uart_put_char_blk1(val2hex(value >> 4));
    uart_put_char_blk1(val2hex(value & 0x0F));
}

/**
 * Sends a hexadecimal dump of a memory region queued over the UART.
 * Before each character is sent, this function waits until the UART is ready
 * to send the next char.
 *
 * @param[in] data
 * Byte-pointer to memory region.
 * @param[in] length
 * Size in bytes of the memory region.
 */
void uart_hex_dump_blk1 ( const uint8_t *data, uint8_t length)
{
    uint8_t ii;

    for (ii=0; ii<length; ii++) {
        uart_put_hex8_blk1(data[ii]);
        if (ii>1 && (ii+1)%16==0) {
            uart_put_char_blk1('\n');
        } else {
            uart_put_char_blk1(' ');
        }
    }
    if (ii%16) uart_put_char_blk1('\n');
}

/**
 * If there is a received char in the UART, whis function will return it. If no
 * char has been received the null-character is received.
 *
 * @returns Received char, otherwise zero.
 */
char uart_get_char_blk1 ( void )
{
    // is char available?
    if ( !(REGISTER_UCSRA1 & (1 << RXC1)) ) {
        // return NUL character if nothing has been received
        return '\0';
    }
    // return received char
    return REGISTER_UDR1;
}

#endif // HAS_UART1
#endif // UART_WITH_BLOCKING

/** @} */
