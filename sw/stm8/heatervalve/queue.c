/**
 * @addtogroup QUEUE
 * @brief Queue with static length created for data input/output.
 *
 * @{
 * @file    queue.c

 * @brief   Queue with static length created for data input/output.
 * @author  Robert Mueller
 *///---------------------------------------------------------------------------/*
/*
 * Copyright (C) 2024
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
 * @returns true, pending data is waiting in queue, otherwise false.
 * @param[in]  q        Queue-handle.
 */
bool q_is_pending (queue_t *q)
{
    return (q->writepos != q->readpos);
}

/**
 * Put a message into the queue. The message is copied into the queue-array.
 *
 * @param[in]  q        Queue-handle.
 * @param[in]  byte     Byte to enqueue.
 *
 * @returns    true if byte was enqueued successful, otherwise false
 *             (e.g. if queue is full).
 */
bool q_put_byte (queue_t *q, uint8_t byte)
{
    if ( !q_get_free(q) ) return false;
    q->data[q->writepos++] = byte;
    q->writepos %= q->size;
    return true;
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

/** @} */
