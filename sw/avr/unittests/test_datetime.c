#include <stdio.h>
#include <CUnit/CUnit.h>

#include "datetime.h"


void testdt_set_date_and_time(void)
{
    uint16_t year = 65535;
    uint8_t month = 255, day = 255, hour = 255, minute = 255, second = 255;
    dt_day_of_week_t dow = DT_SUNDAY + 1;

    dt_initialize();
    dt_set_year(2018);
    dt_set_month(05);
    dt_set_day(16);
    dt_set_day_of_week(DT_WEDNESDAY);
    dt_set_hour(8);
    dt_set_minute(50);
    dt_set_second(30);

    year = dt_get_year();
    CU_ASSERT_EQUAL(year, 2018);
    
    month = dt_get_month();
    CU_ASSERT_EQUAL(month, 05);
    
    day = dt_get_day();
    CU_ASSERT_EQUAL(day, 16);
    
    dow = dt_get_day_of_week();
    CU_ASSERT_EQUAL(dow, DT_WEDNESDAY);
    
    hour = dt_get_hour();
    CU_ASSERT_EQUAL(hour, 8);
    
    minute = dt_get_minute();
    CU_ASSERT_EQUAL(minute, 50);
    
    second = dt_get_second();
    CU_ASSERT_EQUAL(second, 30);  
}

void testdt_initialize(void)
{
    uint16_t year = 65535;
    uint8_t month = 255, day = 255, hour = 255, minute = 255, second = 255;
    dt_day_of_week_t dow = DT_SUNDAY + 1;
    
    dt_initialize();
    year = dt_get_year();
    CU_ASSERT_EQUAL(year, 0);
    
    month = dt_get_month();
    CU_ASSERT_EQUAL(month, 1);
    
    day = dt_get_day();
    CU_ASSERT_EQUAL(day, 1);
    
    dow = dt_get_day_of_week();
    CU_ASSERT_EQUAL(dow, DT_MONDAY);
    
    hour = dt_get_hour();
    CU_ASSERT_EQUAL(hour, 0);
    
    minute = dt_get_minute();
    CU_ASSERT_EQUAL(minute, 0);
    
    second = dt_get_second();
    CU_ASSERT_EQUAL(second, 0);
}

void testdt_second_overflow(void)
{
    dt_set_minute(1);
    dt_set_second(58);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_second(), 59);
    CU_ASSERT_EQUAL(dt_get_minute(), 1);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_second(), 0);
    CU_ASSERT_EQUAL(dt_get_minute(), 2);    
}

void testdt_minute_overflow(void)
{
    dt_set_hour(1);
    dt_set_minute(59);
    dt_set_second(58);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_minute(), 59);
    CU_ASSERT_EQUAL(dt_get_hour(), 1);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_minute(), 0);
    CU_ASSERT_EQUAL(dt_get_hour(), 2);       
}

void testdt_hour_overflow(void)
{
    dt_set_day(1);
    dt_set_day_of_week(DT_MONDAY);
    dt_set_hour(23);
    dt_set_minute(59);
    dt_set_second(58);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_hour(), 23);
    CU_ASSERT_EQUAL(dt_get_day(), 1);
    CU_ASSERT_EQUAL(dt_get_day_of_week(), DT_MONDAY);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_hour(), 0);
    CU_ASSERT_EQUAL(dt_get_day(), 2);       
    CU_ASSERT_EQUAL(dt_get_day_of_week(), DT_TUESDAY);
}

void testdt_day_of_week_overflow(void)
{
    dt_set_day(1);
    dt_set_day_of_week(DT_SUNDAY);
    dt_set_hour(23);
    dt_set_minute(59);
    dt_set_second(58);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_hour(), 23);
    CU_ASSERT_EQUAL(dt_get_day(), 1);
    CU_ASSERT_EQUAL(dt_get_day_of_week(), DT_SUNDAY);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_hour(), 0);
    CU_ASSERT_EQUAL(dt_get_day(), 2);       
    CU_ASSERT_EQUAL(dt_get_day_of_week(), DT_MONDAY);
}

void testdt_day_overflow(void)
{
    // test 28 day month
    dt_set_year(1999);
    dt_set_month(2);
    dt_set_day(28);
    dt_set_day_of_week(DT_SUNDAY);
    dt_set_hour(23);
    dt_set_minute(59);
    dt_set_second(58);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_day(), 28);
    CU_ASSERT_EQUAL(dt_get_month(), 2);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_day(), 1);       
    CU_ASSERT_EQUAL(dt_get_month(), 3);
    
    // test 29 day month
    dt_set_year(2000);
    dt_set_month(2);
    dt_set_day(28);
    dt_set_day_of_week(DT_MONDAY);
    dt_set_hour(23);
    dt_set_minute(59);
    dt_set_second(58);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_day(), 28);
    CU_ASSERT_EQUAL(dt_get_month(), 2);
    dt_tick_second(); // test advancing to Feb 29th
    CU_ASSERT_EQUAL(dt_get_day(), 29);       
    CU_ASSERT_EQUAL(dt_get_month(), 2);
    dt_set_hour(23);
    dt_set_minute(59);
    dt_set_second(59);
    dt_tick_second(); // test advancing to Mar 1st
    CU_ASSERT_EQUAL(dt_get_day(), 1);       
    CU_ASSERT_EQUAL(dt_get_month(), 3);
    
    // test 30 day month
    dt_set_year(2001);
    dt_set_month(4);
    dt_set_day(30);
    dt_set_day_of_week(DT_MONDAY);
    dt_set_hour(23);
    dt_set_minute(59);
    dt_set_second(58);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_day(), 30);
    CU_ASSERT_EQUAL(dt_get_month(), 4);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_day(), 1);       
    CU_ASSERT_EQUAL(dt_get_month(), 5);
    
    // test 31 day month
    dt_set_year(2002);
    dt_set_month(5);
    dt_set_day(31);
    dt_set_day_of_week(DT_TUESDAY);
    dt_set_hour(23);
    dt_set_minute(59);
    dt_set_second(58);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_day(), 31);
    CU_ASSERT_EQUAL(dt_get_month(), 5);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_day(), 1);       
    CU_ASSERT_EQUAL(dt_get_month(), 6); 
}

void testdt_month_overflow(void)
{
    dt_set_year(2002);
    dt_set_month(12);
    dt_set_day(31);
    dt_set_day_of_week(DT_TUESDAY);
    dt_set_hour(23);
    dt_set_minute(59);
    dt_set_second(58);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_day(), 31);
    CU_ASSERT_EQUAL(dt_get_month(), 12);
    CU_ASSERT_EQUAL(dt_get_year(), 2002);
    dt_tick_second();
    CU_ASSERT_EQUAL(dt_get_day(), 1);       
    CU_ASSERT_EQUAL(dt_get_month(), 1); 
    CU_ASSERT_EQUAL(dt_get_year(), 2003);
}

void testdt_set_bad_params(void)
{
    dt_set_year(2002);
    dt_set_month(12);
    dt_set_day(31);
    dt_set_day_of_week(DT_TUESDAY);
    dt_set_hour(23);
    dt_set_minute(59);
    dt_set_second(58);
    
    
    dt_set_month(0);
    CU_ASSERT_EQUAL(dt_get_month(), 12);
    dt_set_month(13);
    CU_ASSERT_EQUAL(dt_get_month(), 12);

    dt_set_day(0);
    CU_ASSERT_EQUAL(dt_get_day(), 31);
    dt_set_day(32);
    CU_ASSERT_EQUAL(dt_get_day(), 31);
    
    dt_set_hour(24);
    CU_ASSERT_EQUAL(dt_get_hour(), 23);
    
    dt_set_minute(60);
    CU_ASSERT_EQUAL(dt_get_minute(), 59);

    dt_set_second(60);
    CU_ASSERT_EQUAL(dt_get_second(), 58);    
}

void test_dt_set_and_get(void)
{
    uint16_t year = 0;
    uint8_t month = 0, day = 0, hour = 0, minute = 0, second = 0;
    dt_day_of_week_t dow = DT_SUNDAY;
    
    dt_set(2007, 03, 23, 13, 44, 11, DT_FRIDAY); 
    CU_ASSERT_EQUAL(dt_get_year(), 2007);
    CU_ASSERT_EQUAL(dt_get_month(), 03);
    CU_ASSERT_EQUAL(dt_get_day(), 23);
    CU_ASSERT_EQUAL(dt_get_hour(), 13);
    CU_ASSERT_EQUAL(dt_get_minute(), 44);
    CU_ASSERT_EQUAL(dt_get_second(), 11);       
    CU_ASSERT_EQUAL(dt_get_day_of_week(), DT_FRIDAY);
    
    dt_get(&year, &month, &day, &hour, &minute, &second, &dow);
    CU_ASSERT_EQUAL(year, 2007);
    CU_ASSERT_EQUAL(month, 03);
    CU_ASSERT_EQUAL(day, 23);
    CU_ASSERT_EQUAL(hour, 13);
    CU_ASSERT_EQUAL(minute, 44);
    CU_ASSERT_EQUAL(second, 11);       
    CU_ASSERT_EQUAL(dow, DT_FRIDAY);
}


CU_TestInfo datetime_tests[] = {
    {"Set and get date and time",                       testdt_set_date_and_time    },
    {"Initialize date and time again",                  testdt_initialize           },
    {"Second overflow from 59 to 00",                   testdt_second_overflow      },
    {"Minute overflow from 59 to 00",                   testdt_minute_overflow      },
    {"Hour overflow from 23 to 00",                     testdt_hour_overflow        },
    {"Day overflow from 28/29/30/31",                   testdt_day_overflow         },
    {"Month overflow from 59 to 00",                    testdt_month_overflow       },
    {"Test passing bad parameters to setter-functions", testdt_set_bad_params       },
    {"Test dt_set() and dt_get() functions",            test_dt_set_and_get         },
    CU_TEST_INFO_NULL,
};
