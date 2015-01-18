/**
 * @addtogroup UART
 * @brief UART driver.
 *
 * @{
 * @file    UART.c
 * @brief   UART driver.
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

typedef struct uart {
    uint8_t                 auRecvQueue[UART_RECV_QUEUE_SIZE+QUEUE_HEADER_SIZE];
    uint8_t                 auSendQueue[UART_SEND_QUEUE_SIZE+QUEUE_HEADER_SIZE];
    uint8_t                 uStatus;
} sUart_t;

// --- Local variables ---------------------------------------------------------

#ifdef UART_WITH_IRQ

static sUart_t              sUart;

#endif // UART_WITH_IRQ

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

#ifdef UART_WITH_IRQ
// Start a new UART transmission.
static void vStartTransmission(void)
{
    uint16_t val = 0;


    // Start transmission
    val = QUE_uGet(sUart.auSendQueue);
    if (val >> 8) {
        REGISTER_UDR = val & 0x00FF;
        REGISTER_UCSRB |= (1<<REGBIT_UDRIE);
    }
}

#endif // UART_WITH_IRQ

// convert a 4bit value into a hex-character
static char cVal2Hex(uint8_t uValue)
{
    if (uValue <= 9) return '0' + uValue;
    if (uValue <= 15) return 'A'- 0x0A + uValue;
    return '-';
}

// --- Interrupts --------------------------------------------------------------

#ifdef UART_WITH_IRQ
// Interrupt when next byte is ready to be loaded into the UART
ISR(INTERRUPT_USART_UDRE)
{
    uint16_t val = 0;

    val = QUE_uGet(sUart.auSendQueue);
    if (val >> 8) {
        REGISTER_UDR = val & 0x00FF;
    } else {
        REGISTER_UCSRB &= ~(1<<REGBIT_UDRIE);
    }
}

// Interrupt when a byte has been received by the UART
ISR(INTERRUPT_USART_RXC)
{
    uint8_t val;

    val = REGISTER_UDR;
    if (!QUE_uPut(sUart.auRecvQueue, val)) {
        sUart.uStatus |= (1<<(eUartFlag_BufOverrun-8));
       // UART_vPutHexBlk (sUart.auRecvQueue[0]);
       // UART_vPutHexBlk (sUart.auRecvQueue[1]);
       // UART_vPutHexBlk (sUart.auRecvQueue[2]);
    }
    // check framing error
    if (REGISTER_UCSRA & (1<<REGBIT_FE)) {
        REGISTER_UCSRA &= ~(1<<REGBIT_FE);
        sUart.uStatus |= (1<<(eUartFlag_FramingError-8));
    }
}

#endif // UART_WITH_IRQ

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#ifdef UART_WITH_IRQ

/**
 * Initialize UART 8N1 with interrupts.
 *
 * @param[in] uBaud
 * Baudrate to configure.
 */
void            UART_vInit          (uint32_t       uBaud)
{
    // set baud-rate
    REGISTER_UBRRH = (uint8_t)(UART_UBRR_CALC( uBaud, F_CPU ) >> 8);
    REGISTER_UBRRL = (uint8_t)UART_UBRR_CALC( uBaud, F_CPU );

    // enable receiver, receiver interrupt and transmitter
    REGISTER_UCSRB = (1<<REGBIT_RXCIE) | (1<<REGBIT_RXEN) | (1<<REGBIT_TXEN);

    // set frame format: asynchronous 8N1
    REGISTER_UCSRC = (1 << UCSZ01) | (1 << UCSZ00);

    // Initialize queue and status variables
    QUE_vInit(sUart.auRecvQueue, sizeof(sUart.auRecvQueue));
    QUE_vInit(sUart.auSendQueue, sizeof(sUart.auSendQueue));
}

/**
 * Check if a UART transmission is ongoing.
 *
 * @returns TRUE, if UART is busy, otherwise FALSE.
 */
BOOL            UART_bIsBusy        (void)
{
    return REGISTER_UCSRB & (1<<REGBIT_UDRIE);
}

/**
 * Transmit a amount of data. Data will be enqueued, if there is not
 * enough space in the queue, the function blocks until everything is enqueued.
 *
 * @param[in] puSendBuf
 * Pointer to data to be sent.
 * @param[in] length
 * Length of data to be sent.
*/
void            UART_vTransmit      (uint8_t*               puSendBuf,
                                     uint8_t                length)
{
    BOOL enqueued;
    while (length--) {
        do {
            enqueued = QUE_uPut(sUart.auSendQueue, *puSendBuf);
        } while (enqueued == FALSE);
        
        REGISTER_UCSRB |= (1<<REGBIT_UDRIE);
        puSendBuf++;
    }
}

/**
 * Receive a byte.
 *
 * @retval Lower 8bit data, higher 8bit statusflags.
 * @see eUartFlags_t for detailed flags.
 */
uint16_t        UART_uReceive       (void)
{
    uint16_t    val;

    val = QUE_uGet(sUart.auRecvQueue);
    if (val>>8) {   // "GotByte" flag set?
        val &= 0x00FF;
        val |= sUart.uStatus << 8;
        sUart.uStatus &= ~sUart.uStatus;
    } else {
        val = (1<<eUartFlag_NoData);
    }
    return val;
}

uint8_t dumpindex = 0;
uint8_t queuedump[32][2];

void vDumpQueue(void)
{
    uint8_t currdi;
    
    currdi = dumpindex;
    do {
        UART_vPutCharBlk('\r');
        UART_vPutHexBlk(queuedump[dumpindex][0]);
        UART_vPutCharBlk(' ');
        UART_vPutHexBlk(queuedump[dumpindex][1]);
        
        dumpindex++; if (dumpindex >= 32) dumpindex = 0;
    } while (dumpindex != currdi);
    UART_vPutCharBlk('\r');
    UART_vPutCharBlk('\r');
}

/**
 * Puts a char into the queue. This function waits until there is enough space
 * in the queue.
 *
 * @param[in] cChar
 * Character to send.
 */
void            UART_vPutChar       (char                   cChar)
{
    BOOL enqueued;
    do {
        enqueued = QUE_uPut(sUart.auSendQueue, cChar);
        queuedump[dumpindex][0] = sUart.auSendQueue[1]; // writeindex
        queuedump[dumpindex++][1] = sUart.auSendQueue[2]; // readindex
        if (dumpindex >= 32) dumpindex = 0;
       // if (enqueued == FALSE) {
         //   vDumpQueue();
       // }
    } while (enqueued == FALSE);
    REGISTER_UCSRB |= (1<<REGBIT_UDRIE);
//    if (!UART_bIsBusy()) {
  //      vStartTransmission();
    //}
    #ifdef UART_AUTOCR
    if ( cChar == '\n' )
        UART_vPutChar('\r');
    #endif
}

/**
 * Puts a string into the queue. This function waits until there is enough space
 * in the queue. Contents in the queue will be send after the FIFO principle.
 *
 * @param[in] pcString
 * String to send. String has to be terminated with a null character.
 */
void            UART_vPutString     (const char*            pcString)
{
    while (*pcString != '\0') {
        UART_vPutChar(*pcString++);
    }
}

/**
 * Puts a hexadecimal representation of a byte as a string into the queue.
 * Contents in the queue will be send after the FIFO principle.
 *
 * @param[in] uValue
 * Byte-value to convert into hex and to be sent.
 */
void            UART_vPutHex        (uint8_t                uValue)
{
    UART_vPutChar(cVal2Hex(uValue >> 4));
    UART_vPutChar(cVal2Hex(uValue & 0x0F));
}

/**
 * Sends a hexadecimal dump of a memory region queued to the UART.
 * Contents in the queue will be send after the FIFO principle.
 *
 * @param[in] puArray
 * Byte-pointer to memory region.
 * @param[in] uSize
 * Size in bytes of the memory region.
 */
void            UART_vHexDump       (const uint8_t*         puArray,
                                     uint8_t                uSize)
{
    uint8_t ii;

    for (ii=0; ii<uSize; ii++) {
        UART_vPutHex(puArray[ii]);
        if (ii>1 && (ii+1)%16==0) {
            UART_vPutChar('\n');
        } else {
            UART_vPutChar(' ');
        }
    }
    if (ii%16) UART_vPutChar('\n');
}

#endif // UART_WITH_IRQ

#ifdef UART_WITH_BLOCKING

/**
 * Initialize UART 8N1 without interrupts.
 *
 * @param[in] uBaud
 * Baudrate to configure.
 */
void UART_vInitBlk ( uint32_t uBaud )
{
    // 9600 Baud 8N1
    REGISTER_UCSRB = (1 << TXEN0) | (1 << RXEN0);  // UART TX und RX einschalten
    REGISTER_UCSRC = (1 << UCSZ01) | (1 << UCSZ00); // Asynchron 8N1

    REGISTER_UBRRH = (uint8_t)(UART_UBRR_CALC( uBaud, F_CPU ) >> 8);
    REGISTER_UBRRL = (uint8_t)UART_UBRR_CALC( uBaud, F_CPU );
}

/**
 * Send a char over the UART. Before sending a char, this function waits until
 * the UART is ready to send the next char.
 *
 * @param[in] cChar
 * Character to send.
 */
void UART_vPutCharBlk ( char cChar )
{
    if ( cChar == '\n' )
        UART_vPutCharBlk('\r');

    loop_until_bit_is_set(REGISTER_UCSRA, UDRE0);
    REGISTER_UDR = cChar;
}

/**
 * If there is a received char in the UART, whis function will return it. If no
 * char has been received the null-character is received.
 *
 * @returns Received char, otherwise zero.
 */
char UART_uGetCharBlk ( void )
{
    // is char available?
    if ( !(REGISTER_UCSRA & (1 << RXC0)) ) {
        // return NUL character if nothing has been received
        return '\0';
    }
    // return received char
    return REGISTER_UDR;
}

/**
 * Sends a string over the UART. Before sending a char, this function waits until
 * the UART is ready to send the next char.
 *
 * @param[in] pcString
 * String to send. String has to be terminated with a null character.
 */
void UART_vPutStringBlk ( const char *pcChar )
{
    while ( *pcChar != '\0' ) {
        UART_vPutCharBlk(*pcChar++);
    }
}

/**
 * Sends a hexadecimal representation of a byte over the UART. Before each
 * character is sent, this function waits until the UART is ready to send
 * the next char.
 *
 * @param[in] uValue
 * Byte-value to convert into hex and to be sent.
 */
void UART_vPutHexBlk (uint8_t uValue)
{
    UART_vPutCharBlk(cVal2Hex(uValue >> 4));
    UART_vPutCharBlk(cVal2Hex(uValue & 0x0F));
}

/**
 * Sends a hexadecimal dump of a memory region queued over the UART.
 * Before each character is sent, this function waits until the UART is ready
 * to send the next char.
 *
 * @param[in] puArray
 * Byte-pointer to memory region.
 * @param[in] uSize
 * Size in bytes of the memory region.
 */
void UART_vHexDumpBlk ( const uint8_t *puArray, uint8_t uSize)
{
    uint8_t ii;

    for (ii=0; ii<uSize; ii++) {
        UART_vPutHexBlk(puArray[ii]);
        if (ii>1 && (ii+1)%16==0) {
            UART_vPutCharBlk('\n');
        } else {
            UART_vPutCharBlk(' ');
        }
    }
    if (ii%16) UART_vPutCharBlk('\n');
}

#endif // UART_WITH_BLOCKING

/** @} */
