/*
 * queue.h
 *
 *  Created on: 28.12.2014
 *      Author: mue
 */

#ifndef TX_QUEUE_H_
#define TX_QUEUE_H_

/**
 * @subsection BUS_APPCONFIG
 * Configure bus modules. E.g. activate scheduling capabilities
 * @{
 */
//! size in bytes of the message transmit queue.
#define TX_QUEUE_SIZE  128

typedef struct queue {
    uint8_t readpos;
    uint8_t writepos;
    uint8_t data[TX_QUEUE_SIZE];
} queue_t;

#endif /* TX_QUEUE_H_ */

void    q_initialize  (queue_t *q);
uint8_t q_get_free    (queue_t *q);
uint8_t q_get_pending (queue_t *q);
uint8_t q_read_byte   (queue_t *q, uint8_t pos);
uint8_t q_get_byte    (queue_t *q);
void    q_put_byte    (queue_t *q, uint8_t byte);
void    q_flush_bytes (queue_t *q, uint8_t quantity);

