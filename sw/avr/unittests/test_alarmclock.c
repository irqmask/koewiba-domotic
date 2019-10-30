#include <stdio.h>
#include <CUnit/CUnit.h>

#include "alarmclock.h"
#include "datetime.h"

void testal_set_alarm(void)
{
    uint8_t hour = 0, 
            minute = 0,
            dow = 0;
            
    alarm_set(0, 11, 05, (1<<DT_MONDAY));
    alarm_set(1, 15, 33, (1<<DT_SUNDAY));
    CU_ASSERT_EQUAL(alarm_get_hour(0), 11);
    CU_ASSERT_EQUAL(alarm_get_minute(0), 05);
    CU_ASSERT_EQUAL(alarm_get_days_of_week_mask(0), (1<<DT_MONDAY));
    CU_ASSERT_EQUAL(alarm_get_hour(1), 15);
    CU_ASSERT_EQUAL(alarm_get_minute(1), 33);
    CU_ASSERT_EQUAL(alarm_get_days_of_week_mask(1), (1<<DT_SUNDAY));
    alarm_set_hour(2, 7);
    alarm_set_minute(2, 20);
    alarm_set_days_of_week_mask(2, (1<<DT_WEDNESDAY));
    alarm_get(2, &hour, &minute, &dow);
    CU_ASSERT_EQUAL(hour, 7);
    CU_ASSERT_EQUAL(minute, 20);
    CU_ASSERT_EQUAL(dow, (1<<DT_WEDNESDAY));  
}

void testal_alarm(void)
{
    int8_t alarm_index = -1;
    bool is_alarm = false;
    
    dt_initialize();
    alarm_initialize();
    
    dt_set(2018, 6, 4, 8, 59, 59, DT_MONDAY);
    alarm_set(0, 9, 0, (1<<DT_MONDAY)); // 9.00
    alarm_activate(0, true);
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_FALSE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, -1);
    dt_tick_second();
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_TRUE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, 0);
    alarm_index = -1;
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_TRUE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, 0);
    dt_set_minute(1); // 9:01
    alarm_index = -1;
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_FALSE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, -1);  
}

void testal_alarm_not_active(void)
{
    int8_t alarm_index = -1;
    bool is_alarm = false;
    
    dt_initialize();
    alarm_initialize();
    
    dt_set(2018, 6, 4, 8, 59, 59, DT_MONDAY);
    alarm_set(0, 9, 0, (1<<DT_MONDAY)); // 9.00
    alarm_activate(0, false);
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_FALSE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, -1);
    dt_tick_second();
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_FALSE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, -1);
}

void testal_alarm_per_day(void)
{
    int8_t alarm_index = -1;
    bool is_alarm = false;
    
    dt_initialize();
    alarm_initialize();

    alarm_set(0, 9, 0, ((1<<DT_MONDAY)|(1<<DT_TUESDAY)|(1<<DT_SUNDAY))); // 9.00
    alarm_activate(0, true);
    
    dt_set(2018, 6, 4, 8, 59, 59, DT_MONDAY);
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_FALSE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, -1);
    dt_tick_second();
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_TRUE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, 0);

    dt_set(2018, 6, 5, 8, 59, 59, DT_TUESDAY);
    alarm_index = -1;
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_FALSE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, -1);
    dt_tick_second();
    alarm_index = -1;
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_TRUE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, 0);
    
    dt_set(2018, 6, 6, 8, 59, 59, DT_WEDNESDAY);
    alarm_index = -1;
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_FALSE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, -1);
    dt_tick_second();
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_FALSE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, -1);
    
    dt_set(2018, 6, 7, 8, 59, 59, DT_THURSDAY);
    alarm_index = -1;
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_FALSE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, -1);
    dt_tick_second();
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_FALSE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, -1);
    
    dt_set(2018, 6, 8, 8, 59, 59, DT_FRIDAY);
    alarm_index = -1;
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_FALSE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, -1);
    dt_tick_second();
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_FALSE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, -1);
    
    dt_set(2018, 6, 9, 8, 59, 59, DT_SATURDAY);
    alarm_index = -1;
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_FALSE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, -1);
    dt_tick_second();
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_FALSE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, -1);
    
    dt_set(2018, 6, 10, 8, 59, 59, DT_SUNDAY);
    alarm_index = -1;
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_FALSE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, -1);
    dt_tick_second();
    is_alarm = alarm_check(&alarm_index);
    CU_ASSERT_TRUE(is_alarm);
    CU_ASSERT_EQUAL(alarm_index, 0);    
}

CU_TestInfo alarmclock_tests[] = {
    {"Set and get alarm times",                         testal_set_alarm            },
    {"Test alarm going off",                            testal_alarm                },
    {"Test alarm not active",                           testal_alarm_not_active     },
    {"Alarm only on Monday, Tuesday and Sunday :-(",    testal_alarm_per_day        },
    CU_TEST_INFO_NULL,
};
