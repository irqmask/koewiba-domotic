/*
 * digital_output.h
 *
 *  Created on: 12.04.2024
 *      Author: mue
 */

#ifndef DIGITAL_OUTPUT_H_
#define DIGITAL_OUTPUT_H_


#include "prjtypes.h"
#include "bus.h"

void digital_output_initialize (void);

void digital_output_set(uint8_t index, uint8_t value);

uint8_t digital_output_get_current_state(uint8_t chn);

void digital_output_set_threshold_off(uint8_t chn, uint8_t throff);

void digital_output_set_threshold_on(uint8_t chn, uint8_t thron);

void digital_output_background(sBus_t* bus);

#endif /* DIGITAL_OUTPUT_H_ */
