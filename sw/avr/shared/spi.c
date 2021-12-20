/**
 * @addtogroup SPI
 * @brief Serial peripheral interface driver.
 *
 * @{
 * @file    spi.c
 * @brief   Serial peripheral interface driver.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2021  christian <irqmask@web.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
// --- Include section ---------------------------------------------------------

#include <avr/io.h>
#include <avr/interrupt.h>

#include "prjtypes.h"
#include "ucontroller.h"
#include "spi.h"

// --- Definitions -------------------------------------------------------------

// Size of the queue header: 1 byte length, 1 byte UserArg, callback before,
// callback after
#define SIZE_QUEUE_HEADER   (2 + 3 * sizeof(void*))
// overall size of the queue
#define SIZE_QUEUE          (SPI_QUEUE_SIZE + (SIZE_QUEUE_HEADER*SPI_NUM_SLAVES))
// Simple check makro if there is unread data in the queue.
#define IS_DATA_IN_QUEUE    (uWriteIdx != uReadIdx)

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

#ifdef SPI_WITH_IRQ 

static uint8_t              auQueue[SIZE_QUEUE];
static uint8_t              uWriteIdx = 0, uReadIdx = 0;
static uint8_t              uCurrMsgLength          = 0;
static uint8_t              uCurrUserArg            = 0;
static uint8_t*             puCurrReceiveBuf        = NULL;
static SPI_StartSendFunc_t  fpBeforeTransmission    = NULL;
static SPI_EndSendFunc_t    fpAfterTransmission     = NULL;
static bool                 bIsBusy                 = false;

#endif

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

#ifdef SPI_WITH_IRQ

// Puts a byte on the queue but only the virtual write index is increased, the
// real one has to be set, if a message is complete enqueued and ready to be
// sent.
static bool bEnqueue(uint8_t* puWriteIdx, uint8_t uVal)
{
    uint8_t tempidx;

    tempidx = *puWriteIdx + 1;
    if (tempidx >= SIZE_QUEUE) {
        tempidx = 0;
    }
    if (tempidx != uReadIdx) {
        auQueue[*puWriteIdx] = uVal;
        *puWriteIdx = tempidx;
        return true;
    }
    return false;
}

// Takes a byte from the queue
static bool bDequeue(uint8_t* puVal)
{
    if (IS_DATA_IN_QUEUE) {
        *puVal = auQueue[uReadIdx++];
        if (uReadIdx >= SIZE_QUEUE) {
            uReadIdx = 0;
        }
        return true;
    }
    return false;
}

// Returns the left space in queue
static uint8_t uQueueSpace(void)
{
    uint8_t spaceleft;
    if (uWriteIdx >= uReadIdx) {
        spaceleft = SIZE_QUEUE - (uWriteIdx - uReadIdx);
    } else {
        spaceleft = uReadIdx - uWriteIdx;
    }
    // maximum space in queue is queuesize - 1
    if (spaceleft > 0) spaceleft--;
    return spaceleft;
}

// Start a new SPI transmission. Can be called from interrupt or normal context.
static void vStartTransmission(void)
{
    uint8_t high = 0, low = 0;

    // from now on SPI is busy. no other transmission can be started.
    bIsBusy = 1;

    bDequeue(&uCurrMsgLength);
    bDequeue(&uCurrUserArg);
    // dequeue receive buffer pointer
    bDequeue(&high);
    bDequeue(&low);
    puCurrReceiveBuf = (uint8_t*)((high << 8) | low);
    // dequeue function pointers
    bDequeue(&high);
    bDequeue(&low);
    fpBeforeTransmission = (SPI_StartSendFunc_t)((high << 8) | low);
    bDequeue(&high);
    bDequeue(&low);
    fpAfterTransmission = (SPI_EndSendFunc_t)((high << 8) | low);

    // call function before transmission
    if (fpBeforeTransmission != NULL) {
        fpBeforeTransmission(uCurrUserArg);
    }

    // start transmission of first byte
    bDequeue(&high);
    SPDR = high;
    uCurrMsgLength--;
}

#endif

// --- Interrupts --------------------------------------------------------------

#ifdef SPI_WITH_IRQ

ISR(INTERRUPT_SPI)
{
    uint8_t val;
    if (uCurrMsgLength > 0) {
        // store received byte
        if (puCurrReceiveBuf != NULL) *puCurrReceiveBuf++ = SPDR;

        // send next byte
        bDequeue(&val);
        SPDR = val;
        uCurrMsgLength--;
    } else {
        // call function after transmission
        if (fpAfterTransmission != NULL) {
            fpAfterTransmission(uCurrUserArg);
        }
        // start next transmission
        if (IS_DATA_IN_QUEUE) {
            vStartTransmission();
        } else {
            bIsBusy = false;
        }
    }
}

#endif // SPI_WITH_IRQ

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

#ifdef SPI_WITH_IRQ

/**
 * Initialize SPI as master with interrupts.
 */
void            SPI_vMasterInit     (void)
{
    // Set MOSI and SCK output, all others input
    SPI_DDR_MISO &= ~(1 << SPI_MISO);
    SPI_DDR_MOSI |= (1 << SPI_MOSI);
    SPI_DDR_SCK |= (1 << SPI_SCK);
    SPI_DDR_SS |= (1 << SPI_SS);    // set DDR for slave select as output to
                                    // guarantee SPI master mode
    SPI_PORT_SS |= (1 << SPI_SS);   // set slave select to 1 (slave disabled)
    // double clock rate
    SPSR = (1 << SPI2X);
    // Enable SPI, interrupt, master, set clock rate
    SPCR = (1 << SPE) | (1<<SPIE) | (1 << MSTR) | (1 << SPR0);

    // clear interrupt flags by reading registers
    uWriteIdx = SPDR;
    uReadIdx = SPSR;

    // Initialize queue and status variables
    uWriteIdx               = 0;
    uReadIdx                = 0;
    uCurrMsgLength          = 0;
    uCurrUserArg            = 0;
    puCurrReceiveBuf        = NULL;
    fpBeforeTransmission    = NULL;
    fpAfterTransmission     = NULL;
    bIsBusy                 = false;
}

/**
 * Check how much space is left in the SPI queue.
 *
 * @returns Queue space left in bytes.
 */
uint8_t         SPI_uQueueSpace     (void)
{
    uint8_t spaceleft;

    spaceleft = uQueueSpace();
    if (spaceleft <= SIZE_QUEUE_HEADER) {
        return 0;
    } else {
        return spaceleft - SIZE_QUEUE_HEADER;
    }
}

/**
 * Check if a SPI transmission is ongoing.
 *
 * @returns true, if SPI is busy, otherwise false.
 */
bool            SPI_bIsBusy         (void)
{
    return bIsBusy;
}

/**
 * Transmit a amount of data and receive the same amount of data via SPI.
 *
 * @param[in] puSendBuf
 * Pointer to data to be sent.
 * @param[out] puReceiveBuf
 * Optional pointer to receive buffer otherwise NULL. Attention, this buffer is
 * filled from an interrupt context!
 * @param[in] length
 * Length of data to be sent and received.
 * @param[in] fpBeforeTransmission
 * Optional pointer to function which is called before SPI transmission,
 * otherwise NULL.
 * @param[in] fpAfterTransmission
 * Optional pointer to function which is called after SPI transmission,
 * otherwise NULL.
 * @param[in] uUserArg
 * User defined argument which is passed to the fpBeforeTransmission() and
 * fpAfterTransmission() functions.
 *
 * @returns Number of bytes which have been enqueued to the send-queue.
 */
uint8_t         SPI_uTransmit       (uint8_t*               puSendBuf,
                                     uint8_t*               puReceiveBuf,
                                     uint8_t                length,
                                     SPI_StartSendFunc_t    fpBeforeTransmission,
                                     SPI_EndSendFunc_t      fpAfterTransmission,
                                     uint8_t                uUserArg)
{
    uint8_t enqueued = 0;
    uint8_t temp_wr_idx;

    // check length
    if (length > SPI_QUEUE_SIZE) return 0;

    // wait until there is enough space in the transmit queue
    while (SPI_uQueueSpace() < length);

    // hold queue until full message is transmitted
    temp_wr_idx = uWriteIdx;

    // enqueue header
    bEnqueue(&temp_wr_idx, length);
    bEnqueue(&temp_wr_idx, uUserArg);
    bEnqueue(&temp_wr_idx, (uint16_t)puReceiveBuf >> 8);
    bEnqueue(&temp_wr_idx, (uint16_t)puReceiveBuf & 0xFF);
    bEnqueue(&temp_wr_idx, (uint16_t)fpBeforeTransmission >> 8);
    bEnqueue(&temp_wr_idx, (uint16_t)fpBeforeTransmission & 0xFF);
    bEnqueue(&temp_wr_idx, (uint16_t)fpAfterTransmission >> 8);
    bEnqueue(&temp_wr_idx, (uint16_t)fpAfterTransmission & 0xFF);
    // enqueue data
    for (enqueued=0; enqueued<length; enqueued++) {
        bEnqueue(&temp_wr_idx, puSendBuf[enqueued]);
    }

    // afte complete message has been enqueued, set write pointer
    uWriteIdx = temp_wr_idx;

    // If no transmission is already busy, start a new one, otherwise the
    // transmission is started automatically after the current running finishes.
    if (!bIsBusy) {
        vStartTransmission();
    }

    return enqueued;
}

#endif // SPI_WITH_IRQ

/** @} */
