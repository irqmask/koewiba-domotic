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
#include "queue.h"


void q_initialize(queue_t *q)
{
    uint8_t     n;

    for (n=0; n<sizeof(q->data); ++n) q->data[n] = 0;
    q->writepos = 0;
    q->readpos  = 0;
}

uint8_t q_get_free (queue_t *q)
{
    return (sizeof(q->data) - 1 + q->readpos - q->writepos) % sizeof(q->data);
}

uint8_t q_get_pending (queue_t *q)
{
    return (sizeof(q->data) + q->writepos - q->readpos) % sizeof(q->data);
}

/**
 * Put a message into the queue. The message is copied into the queue-array.
 *
 * @param[in]  queue    Queue-handle.
 * @param[in]  puMsg    Pointer to message.
 *
 * @returns    TRUE if message was copied successful, otherwise FALSE (e.g. if queue is full).
 */
BOOL q_put_byte (queue_t *q, uint8_t byte)
{
    if ( !q_get_free(q) ) return FALSE;
    q->data[q->writepos] = byte;
    if (sizeof(q->data) <= ++q->writepos) q->writepos = 0;
    return TRUE;
}

/**
 *
 */
uint8_t q_get_byte (queue_t *q)
{
    uint8_t byte;

    byte = q->data[q->readpos];
    q->readpos++;
    q->readpos %= sizeof(q->data);

    return byte;
}


/**
 * Reads the byte on position <pos> relative to the current read-position.
 *
 * @returns uint8_t Byte on position <pos> + readpos
 */
uint8_t q_read_byte (queue_t *q, uint8_t offset)
{
    return q->data[(q->readpos+offset) % sizeof(q->data)];
}


/**
 * Increments the read-position by <quantitiy>.
 * If the write-pointer would be overrun, flushing ends at this position.
 *
 * @returns void
 */
void q_flush_bytes (queue_t *q, uint8_t quantity)
{
    if(q_get_pending(q) < quantity) quantity = q_get_pending(q);
    q->readpos = (q->readpos + quantity) % sizeof(q->data);
}
