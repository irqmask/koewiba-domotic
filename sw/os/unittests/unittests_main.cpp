/**
 * @addtogroup UNITTESTS
 *
 * @{
 * @file    unittests_main.cpp
 * @brief   Test application for unittests of os code
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2020  christian <irqmask@gmx.de>
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

#include "prjconf.h"

#include <gtest/gtest.h>

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/**
 * Main entry point of unit tests.
 * @param[in]   argc    Number of arguments passed.
 * @param[in]   argv    List of program arguments.
 * @return 0, if tests passed successfully otherwise non-zero.
 */
int main(int argc, char *argv[])
{
    int rc;
    testing::InitGoogleTest(&argc, argv);
    rc = RUN_ALL_TESTS();
    return rc;
}

/** @} */
