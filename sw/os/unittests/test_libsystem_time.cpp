/**
 * @addtogroup UNITTESTS
 *
 * @{
 * @file    test_libsystem_time.c
 * @brief   This module contains the unittests for: libsystem/time.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2017  christian <irqmask@gmx.de>
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

#include "gtest/gtest.h"

#include "prjconf.h"

// include
#include "prjtypes.h"
// libsystem
#include "systime.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

TEST(libsystem, time_1)
{
    sys_time_t start, end, diff;

    start = sys_time_get_usecs();
    sys_sleep_ms(100);
    end = sys_time_get_usecs();

    diff = end - start;

    ASSERT_GT(diff, 99 * 1000);
    ASSERT_LT(diff, 110 * 1000);
}

/** @} */
