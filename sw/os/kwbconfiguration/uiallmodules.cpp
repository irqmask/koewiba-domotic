/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    uiallmodules.cpp
 * @brief   UI sub-menu for actions on all modules.
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2025  christian <irqmask@web.de>
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

#include "uiallmodules.h"

#if defined (PRJCONF_UNIX) || \
    defined (PRJCONF_POSIX) || \
    defined (PRJCONF_LINUX)
extern "C" {
#include <safe_lib.h>
}
#endif

// os/libkwb
#include "exceptions.h"
#include "log.h"

#include "cfg_module_file.h"
#include "cfg_module_json.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

constexpr char UIAllModules::DEF_MODULE_FILENAME[];

UIAllModules::UIAllModules(Application &app) : UIConsole(app)
{
}

//----------------------------------------------------------------------------
void UIAllModules::display()
{
    std::cout << std::endl;
    std::cout << "All modules Menu" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << " (1) list all (from file)" << std::endl;
    std::cout << " (2) read all versions" << std::endl;
    //std::cout << " (4) backup all registers" << std::endl;
    //std::cout << " (5) restore all registers" << std::endl;
    std::cout << " (x) leave sub-menu" << std::endl;
}

//----------------------------------------------------------------------------
void UIAllModules::onMenuChoice()
{
    try {
        switch (this->last_choice) {
        case '1':
            listAllFromFile();
            break;
        case '2':
            readAllVersions();
            break;
        case 'x':
            leave_menu = true;
            break;
        default:
            UIConsole::onMenuChoice();
            break;
        }
    }
    catch (Exception &e) {
        std::cerr << "Error occured during executing " << this->last_choice << "!" << std::endl;
        std::cerr << e.what() << std::endl;
    }
}

//----------------------------------------------------------------------------
void UIAllModules::listAllFromFile()
{
    std::string filename = "modules.json";
    try {
        ModuleFile mf(filename);
        std::vector<ModuleJson> modules = mf.getModules();

        std::cout << "Modules from file " << filename << std::endl;
        std::cout << "  ModId  AppId  Name              Description" << std::endl;
        for (auto &module : modules) {
            fprintf(stdout, "  0x%04x 0x%04x %16s %s\n", module.nodeId, module.appId, module.name.c_str(),
                    module.description.c_str());
        }
    }
    catch (Exception &e) {
        throw OperationFailed(LOC, "Cannot load modules file! %s", e.what());
    }
}

//----------------------------------------------------------------------------
void UIAllModules::readAllVersions()
{
    std::string filename = DEF_MODULE_FILENAME;

    std::cout << "Read versions of modules from file " << filename << std::endl;
    ModuleFile mf(filename);
    std::vector<ModuleJson> modules = mf.getModules();

    std::stringstream output;
    output << "Modules from file " << filename << std::endl;
    output << "  ModId  AppId  Name             AppId   Version     BoardId.BoardRev ControllerId" << std::endl;
    for (auto &module : modules) {
        try {
            struct ModuleInfo mi;
            app.readModuleInfo(module.nodeId, mi);
            char line[256];
            snprintf(line, sizeof(line) - 1, "  0x%04x 0x%04x %16s 0x%04x %2d.%02d.%03d-g%08x  0x%04x.%d %02x:%02x:%02x:%02x\n",
                     module.nodeId, module.appId, module.name.c_str(),
                     mi.appId, mi.majorVersion, mi.minorVersion, mi.bugfixVersion, mi.versionHash, mi.boardId, mi.boardRev,
                     mi.controllerId[0], mi.controllerId[1], mi.controllerId[2], mi.controllerId[3]);
            output << line;
        }
        catch (Exception &e) {
            log_error("%s", e.what());
        }
    }
    std::cout << output.str();
}
