/**
 * @addtogroup BUS
 * @addtogroup BUS_PHY
 * @brief Physical layer of bus protocol.
 *
 * @{
 * @file    tx_queue.c
 * @date    27.04.2014
 * @brief   Queue with static length created for data output.
 * @todo    describe file purpose
 * @author  Robert Mueller
 *///---------------------------------------------------------------------------/*

#include "prjtypes.h"
#include "bus_intern.h"


void bus_q_initialize(queue_t *q)
{
    uint8_t     n;

    for (n=0; n<BUS_TX_QUEUE_SIZE; ++n) q->data[n] = 0;
    q->pwrite = 0;
    q->pread  = 0;
}

uint8_t bus_q_get_free (queue_t *q)
{
    return (BUS_TX_QUEUE_SIZE - 1 + q->pread - q->pwrite) % BUS_TX_QUEUE_SIZE;
}

uint8_t bus_q_get_pending (queue_t *q)
{
    return (BUS_TX_QUEUE_SIZE + q->pwrite - q->pread) % BUS_TX_QUEUE_SIZE;
}

/**
 * Put a message into the queue. The message is copied into the queue-array.
 *
 * @param[in]  queue    Queue-handle.
 * @param[in]  puMsg    Pointer to message.
 *
 * @returns    TRUE if message was copied successful, otherwise FALSE (e.g. if queue is full).
 */
void bus_q_put_byte (queue_t *q, uint8_t byte)
{
    q->data[q->pwrite++] = byte;
    if (BUS_TX_QUEUE_SIZE <= q->pwrite) q->pwrite = 0;
}

/**
 * Get Pointer of the first Byte of message to read and the number of bytes to read.
 * IMPORTANT: The message remains in the queue. If message data is not needed any more
 *            call 'tx_queue_free()' with message-length as parameter.
 *
 * @param[in]  queue    Queue-handle.
 * @param[out] puMsg    Pointer to message.
 *
 * @returns    len      Length in bytes of message to be sent.
 */
uint8_t bus_q_get_byte (queue_t *q)
{
    uint8_t byte;

    byte = q->data[q->pread];
    q->pread++;
    q->pread %= BUS_TX_QUEUE_SIZE;

    return byte;
}
