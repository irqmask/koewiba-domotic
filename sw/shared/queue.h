/*
 * queue.h
 *
 *  Created on: 28.12.2014
 *      Author: mue
 */

#ifndef QUEUE_H_
#define QUEUE_H_

/**
 * @subsection BUS_APPCONFIG
 * Configure bus modules. E.g. activate scheduling capabilities
 * @{
 */

typedef struct queue {
    uint16_t readpos;
    uint16_t writepos;
    uint16_t size;
    uint8_t* data;
} queue_t;


void    q_initialize  (queue_t *q, uint8_t* data, uint16_t size);
uint8_t q_get_free    (queue_t *q);
uint8_t q_get_pending (queue_t *q);
uint8_t q_read_byte   (queue_t *q, uint16_t pos);
uint8_t q_get_byte    (queue_t *q);
bool    q_put_byte    (queue_t *q, uint8_t byte);
void    q_flush_bytes (queue_t *q, uint16_t quantity);
void    q_flush_all   (queue_t *q);

#endif /* QUEUE_H_ */
