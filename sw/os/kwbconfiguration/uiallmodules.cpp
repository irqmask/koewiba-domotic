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

// os/libkwb
#include "exceptions.h"
#include "log.h"

#include "action_read_module_info.h"
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
            fprintf(stdout, "  0x%04x 0x%04x %16s %s\n", module.nodeId, module.appId, module.name.c_str(), module.description.c_str());
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

    std::cout << "Modules from file " << filename << std::endl;
    std::cout << "  ModId  AppId  Name              AppId  BoardId.BoardRev Version   ControllerId" << std::endl;
    for (auto &module : modules) {
        try {
            ActionReadModuleInfo armi(app.getMsgEndpoint(), app.getMsgBroker(), module.nodeId);
            armi.start();
            armi.waitFinished();
            auto ctrlId = armi.getControllerId();
            fprintf(stdout, "  0x%04x 0x%04x %16s 0x%04x %2d.%02d.%03d-g%08x  0x%04x.%d %02x:%02x:%02x:%02x\n",
                    module.nodeId, module.appId, module.name.c_str(),
                    armi.getAppId(), armi.getMajorVersion(), armi.getMinorVersion(), armi.getBugfixVersion(), armi.getVersionHash(), armi.getBoardId(), armi.getBoardRev(),
                    ctrlId[0], ctrlId[1], ctrlId[2], ctrlId[3]);

        }
        catch (Exception &e) {
            log_error("%s", e.what());
        }
    }
}
