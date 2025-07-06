/**
 * @addtogroup APPLICATION
 *
 * @{
 * @date    2025-07-01
 * @file    blind_event.c
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

// --- Include section ---------------------------------------------------------

#include "datetime.h"
#include "prjtypes.h"

#include "alarmclock.h"
#include "appconfig.h"
#include "blind_event.h"
#include "blindctrl.h"
#include "register.h"

#ifdef HAS_PCBCONFIG_H
 #include "pcbconfig.h"
#endif
#ifdef HAS_APPCONFIG_H
 #include "appconfig.h"
#endif

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

extern bool app_register_get        (uint8_t                reg_no,
                                     eRegType_t*            reg_type,
                                     void*                  value);

// --- Global functions --------------------------------------------------------

/**
 * Get a 8bit content from register.
 *
 * @param[in]   reg_no  The index of the register.
 * @param[out]  p_data  The data pointer the register content is written into.
 * @return      retval  The validity of the read value.
 */
static bool get_data_8bit_data_from_register(uint8_t reg_no, uint8_t* p_data)
{
    eRegType_t regtype = eRegType_U8;
    return app_register_get(reg_no, &regtype, p_data);
}

/**
 * Determines the next “time event” that occurs after or simultaneous to the time
 * specified in the arguments. If there is more than one event with the same time,
 * the amount of events with the same time is count and the index of the last
 * of them is returned at the first time. At the second time the counter is
 * decreased and the second last index is returned.This continues until each
 * simultaneous event has been returned once.
 *
 * @param[in]   day       The day the search should start.
 * @param[in]   hour      The hour the search should start.
 * @param[in]   minute    The minute the search should start.
 * @param[out]  day       The day of the next event found in event list.
 * @param[out]  hour      The hour of the next event found in event list.
 * @param[out]  minute    The minute of the next event found in event list.
 * @param[out]  retval    The index of the next event.
 */
static int8_t blind_event_get_next_alarm(uint8_t* day, uint8_t* hour, uint8_t* minute)
{
    int8_t  retval = -1;
    uint8_t be_idx, be_day;                 //!< blind alarm index and day index
    uint8_t evt_days;                       //!< blind alarm weekday bitfield
    uint8_t evt_hour;                       //!< blind alarm hour
    uint8_t evt_minute;                     //!< blind alarm minute
    uint16_t c_minutes;                     //!< current amount of minutes since week begin.
    uint16_t e_minutes;                     //!< events amount of minutes from week begin.
    uint16_t diff;                          //!< difference between current day and the event day
    uint16_t diff_min = 0xFFFF;             //!< minimum difference of minutes
    uint8_t equal_time_loop_cnt = 0;        //!< events with current time already found in this loop run
    uint8_t	equal_evttime_cnt = 0;          //!< events with equal event time found in this loop run
    static uint8_t	equal_nextevent_cnt = 0;//!< number of next events with the same event time

    c_minutes = (*day)*24 + (*hour)*60 + *minute;
    for(be_idx=0; be_idx<APP_NOF_BLINDEVENTS; be_idx++){
        uint8_t reg_offset = be_idx*APP_NUM_REGS_PER_BLINDEVENT;
        get_data_8bit_data_from_register(APP_eReg_BlindEvent0_Weekday+reg_offset, &evt_days);
        get_data_8bit_data_from_register(APP_eReg_BlindEvent0_Hour   +reg_offset, &evt_hour);
        get_data_8bit_data_from_register(APP_eReg_BlindEvent0_Minute +reg_offset, &evt_minute);
        if(59 < evt_minute || 23 < evt_hour) continue;
        for(be_day=0; be_day<7; be_day++)
        {
            if( 0 == ((1<<be_day) & evt_days) ) continue;
            e_minutes = 24*be_day  + 60*evt_hour  + evt_minute;
            diff = (e_minutes<c_minutes)?(e_minutes+MINUTES_PER_WEEK-c_minutes):(e_minutes-c_minutes);
            if(0 < diff)
            {
                if(diff < diff_min)
                {
                    equal_evttime_cnt = 0;
                    diff_min = diff;
                    *day     = be_day; // 0=Sunday
                    *hour    = evt_hour;
                    *minute  = evt_minute;
                    retval = be_idx;
                }
                else if(diff == diff_min)
                {   // Event entry with the same time like the next event
                    equal_evttime_cnt++;
                    retval = be_idx;
                }
            }
            else // diff = 0
            { /* This is one of the event entries that should be processed by now but
                 dependent on the event entries still pending it will be skipped. */
                equal_time_loop_cnt++; // last entry of the same time is already processed
                if(equal_time_loop_cnt == equal_nextevent_cnt)
                {
                    *day     = be_day; // 0=Sunday
                    *hour    = evt_hour;
                    *minute  = evt_minute;
                    retval = be_idx;
                    equal_nextevent_cnt--;
                    equal_time_loop_cnt = 0;
                    return retval;
                }
            }
        }
    }
    if(0 == equal_nextevent_cnt)
    { /*the number of next events happening at the same time must not be set
        before all current events are processed.*/
        equal_nextevent_cnt = equal_evttime_cnt;
    }
    return retval;
}

/**
 * Sets the next “time event” in the alarm clock and adds the blind event index
 * as 'additional data'.
 *
 * @return      retval    The index of the next event.
 */
int8_t blind_event_evaluate_next_alarm(void)
{
    uint8_t be_day      = dt_get_day_of_week(); // 0=Sunday ... Saturday=6
    uint8_t be_hour     = dt_get_hour();   //!< current hour
    uint8_t be_minute   = dt_get_minute(); //!< current minute

    int8_t be_idx = blind_event_get_next_alarm(&be_day, &be_hour, &be_minute);
    if(0 <= be_idx)
    {
        alarm_set(0, be_hour, be_minute, (1<<be_day));
        alarm_set_data(0, be_idx);
    }
    return be_idx;
}


/**
 * Processes the time event with index given by the argument'event_idx' by
 * - checking the exclusion conditions
 * - reading the position value out of the event register.
 * - starting the movement of blinds that are configured in the event register.
 * - configuring the next blind event in the alarm clock.
 *
 * @param[in]   event_idx  The event index that should be processed.
 */
void blind_event_process_alarm(uint8_t event_idx)
{
    uint8_t b_idx, reg_offset;
    uint8_t blinds = 0x00;
    uint8_t evt_exclflags = 0x00;
    uint8_t curr_exclflags = 0x00;
    uint8_t b_pos_val;

    blind_event_evaluate_next_alarm();

    reg_offset = event_idx*APP_NUM_REGS_PER_BLINDEVENT;

    get_data_8bit_data_from_register(APP_eReg_MasterExclusionFlags, &curr_exclflags);
    get_data_8bit_data_from_register(reg_offset+APP_eReg_BlindEvent0_ExclusionFlags, &evt_exclflags);
    if(curr_exclflags & evt_exclflags) return;

    get_data_8bit_data_from_register(reg_offset+APP_eReg_BlindEvent0_Blinds, &blinds);
    get_data_8bit_data_from_register(reg_offset+APP_eReg_BlindEvent0_PositionValue, &b_pos_val);
    for(b_idx=0; b_idx<BLIND_COUNT; b_idx++)
    {
        if( ((1<<b_idx) & blinds) && (blind_is_moving(b_idx) == false) )
        {
            blind_move_to_position(b_idx, b_pos_val);
        }
    }
}

uint8_t blind_event_get_next_alarm_hour(void)
{
    return alarm_get_hour(0);
}
uint8_t blind_event_get_next_alarm_minute(void)
{
    return alarm_get_minute(0);
}
uint8_t blind_event_get_next_alarm_data(void)
{
    uint8_t p_data;
    alarm_get_data(0, &p_data);
    return p_data;
}

void test_blind_event_set_alarm(uint8_t idx)
{
	switch(idx)
	{
	case 0:
		alarm_set(0, 0, 1, 0b01111111);
		alarm_set_data(0, 0);
		break;
	case 1:
		alarm_set(0, 0, 2, 0b01111111);
		alarm_set_data(0, 1);
		break;
	case 2:
		alarm_set(0, 0, 2, 0b01111111);
		alarm_set_data(0, 2);
		break;
	case 3:
		alarm_set(0, 0, 3, 0b01111111);
		alarm_set_data(0, 3);
		break;
	case 4:
		alarm_set(0, 0, 5, 0b01111111);
		alarm_set_data(0, 4);
		break;
	case 5:
		alarm_set(0, 0, 6, 0b01111111);
		alarm_set_data(0, 5);
		break;
	case 6:
		alarm_set(0, 0, 7, 0b01111111);
		alarm_set_data(0, 6);
		break;
	case 7:
		alarm_set(0, 0, 8, 0b01111111);
		alarm_set_data(0, 7);
		break;
	case 8:
		alarm_set(0, 0, 9, 0b01111111);
		alarm_set_data(0, 8);
		break;
	case 9:
		alarm_set(0, 0, 10, 0b01111111);
		alarm_set_data(0, 9);
		break;
	default:
		alarm_set(0, 0, 2, 0b00000000);
		alarm_set_data(0, 0);
		break;
	}
}

/** @} */
