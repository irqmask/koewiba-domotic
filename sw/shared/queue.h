/**
 * @addtogroup QUEUE
 * @brief Queue with static length created for data input/output.
 *
 * @{
 * @file    queue.h

 * @brief   Queue with static length created for data input/output.
 * @author  Robert Mueller

 */
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
#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef struct queue {
    uint16_t readpos;
    uint16_t writepos;
    uint16_t size;
    uint8_t* data;
} queue_t;


void    q_initialize  (queue_t *q, uint8_t* data, uint16_t size);
uint8_t q_get_free    (queue_t *q);
uint8_t q_get_pending (queue_t *q);
bool    q_is_pending  (queue_t *q);
uint8_t q_read_byte   (queue_t *q, uint16_t pos);
uint8_t q_get_byte    (queue_t *q);
bool    q_put_byte    (queue_t *q, uint8_t byte);
void    q_flush_bytes (queue_t *q, uint16_t quantity);
void    q_flush_all   (queue_t *q);

#endif /* _QUEUE_H_ */

/** @} */
