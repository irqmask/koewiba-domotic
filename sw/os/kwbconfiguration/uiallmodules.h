/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    UIAllModules.h
 * @brief   UI sub-menu for actions on all modules.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2022  christian <irqmask@web.de>
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

#include "uiconsole.h"

class UIAllModules : public UIConsole
{
public:

    /**
     * Constructor
     * @param[in]   app         Reference to belonging application
     */
    UIAllModules(Application &app);

protected:
    virtual void display() override;
    virtual void onMenuChoice() override;
};

