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

// --- Include section ---------------------------------------------------------

#include "prjtypes.h"
#include "queue.h"

/**
 * Initialize the queue.
 * @param[in]  q        Queue-handle.
 */
void q_initialize(queue_t *q, uint8_t *data, uint16_t size)
{
    uint8_t     n;

    q->data = data;
    q->size = size;
    for (n=0; n<size; n++) q->data[n] = 0;
    q->writepos = 0;
    q->readpos  = 0;
}

/**
 * Returns the number of free bytes in queue.
 *
 * @param[in]  q        Queue-handle.
 * @returns    Number of free space in the queue.
 */
uint8_t q_get_free (queue_t *q)
{
    return (q->size - 1 + q->readpos - q->writepos) % q->size;
}

/**
 * Returns the number of pending bytes in the queue.
 *
 * @param[in]  q        Queue-handle.
 * @returns    Number of pending bytes to be read.
 */
uint8_t q_get_pending (queue_t *q)
{
    return (q->size + q->writepos - q->readpos) % q->size;
}

/**
 * Put a message into the queue. The message is copied into the queue-array.
 *
 * @param[in]  q        Queue-handle.
 * @param[in]  byte     Byte to enqueue.
 *
 * @returns    TRUE if byte was enqueued successful, otherwise FALSE
 *             (e.g. if queue is full).
 */
BOOL q_put_byte (queue_t *q, uint8_t byte)
{
    if ( !q_get_free(q) ) return FALSE;
    q->data[q->writepos++] = byte;
    q->writepos %= q->size;
    return TRUE;
}

/**
 * Take and remove a byte from the queue.
 * This function does not check, if the queue contains bytes.
 *
 * @note: Always check with q_get_pending if the queue contains sufficient bytes
 * to read.
 *
 * @param[in]   q       Queue-handle.
 * @returns     byte, or 0 if queue is empty.
 */
uint8_t q_get_byte (queue_t *q)
{
    uint8_t byte;

    byte = q->data[q->readpos++];
    q->readpos %= q->size;

    return byte;
}

/**
 * Reads the byte on position <pos> relative to the current read-position.
 * The byte is not taken from the queue.
 *
 * @param[in]  q        Queue-handle.
 * @param[in]  offset   Offset from current read position.
 *
 * @returns uint8_t Byte on position <pos> + readpos
 */
uint8_t q_read_byte (queue_t *q, uint16_t offset)
{
    return q->data[(q->readpos+offset) % q->size];
}

/**
 * Increments the read-position by <quantitiy>.
 * If the write-pointer would be overrun, flushing ends at this position.
 *
 * @param[in]  q        Queue-handle.
 * @param[in]  quantity Number of bytes to be removed from queue.
 */
void q_flush_bytes (queue_t *q, uint16_t quantity)
{
    if (q_get_pending(q) < quantity) quantity = q_get_pending(q);
    q->readpos = (q->readpos + quantity) % q->size;
}

/**
 * Flush all content of the queue.
 *
 * @param[in]  q        Queue-handle.
 */
void q_flush_all (queue_t *q)
{
    q->readpos = q->writepos;
}
