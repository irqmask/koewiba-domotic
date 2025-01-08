/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    cfg_module.hpp
 * @brief   Configuration: Holds properties of the configuration of a module.
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2024  christian <irqmask@web.de>
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
#pragma once

// --- Include section ---------------------------------------------------------

#include "prjconf.h"

#include <string>

// include
#include "prjtypes.h"
#include "moddef_common.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Class definition --------------------------------------------------------

class Module
{
public:
    Module()
        : nodeId(0)
        , appId(0)
        , name("")
        , description("")
    {
    }

    uint16_t nodeId;    ///< Node ID of the module
    uint16_t appId;     ///< application ID as defined in moddef_common.h
    std::string name;   ///< name of the module
    std::string description; ///< description of the module (e.g. purpose, location,...)
};

/** @} */
