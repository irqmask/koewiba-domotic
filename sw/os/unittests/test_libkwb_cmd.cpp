/**
 * @addtogroup UNITTESTS
 *
 * @{
 * @file    test_libkwb_connection.cpp
 * @brief   This module contains the unittests for: libkwb/connection.
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

#include "gtest/gtest.h"

#include "prjconf.h"

// include
#include "prjtypes.h"
// libkwb
#include "connection.h"
#include "log.h"
#include "message.h"

/// Unittests for the connection class
class CommandHandlerTest : public ::testing::Test
{
protected:
    /// SetUp is called before each test is started
    void SetUp() override
    {
        log_set_mask(0xFFFFFFFF);
        log_add_prefix("[          ] ");
    }

    void TearDown() override {}

public:

};


/// @test Test if connection object can be instantiated.
TEST_F(CommandHandlerTest, instantiate)
{


}

/** @} */
