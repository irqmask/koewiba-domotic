/**
 * @addtogroup APPLICATION
 *
 * @{
 * @date    2025-06-30
 * @file    blind_event.h
 * @brief   A module for managing timed alarms/events for blinds.
 *
 * @author  Robert Mueller
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2018 rfmkoeln <djrfm@gmx.net>
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


#ifndef BLIND_EVENT_H_
#define BLIND_EVENT_H_

int8_t blind_event_evaluate_next_alarm(void);

void blind_event_process_alarm(uint8_t event_idx);

uint8_t blind_event_get_next_alarm_minute(void);
uint8_t blind_event_get_next_alarm_hour(void);
uint8_t blind_event_get_next_alarm_data(void);

void test_blind_event_set_alarm(uint8_t idx);


#endif /* BLIND_EVENT_H_ */
