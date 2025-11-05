/**
 * @addtogroup KWBCONFIGURATION
 *
 * @{
 * @file    uisinglemodule.cpp
 * @brief   UI for console.
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

#include "uisinglemodule.h"

#include "backuprestore.h"
#include "exceptions.h"

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

UISingleModule::UISingleModule(Application &app) : UIConsole(app)
{
}

//----------------------------------------------------------------------------
void UISingleModule::display()
{
    std::cout << std::endl;
    std::cout << "Single module Menu" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << " (2) read version" << std::endl;
    //std::cout << " (3) update firmware" << std::endl;
    std::cout << " (4) read register" << std::endl;
    std::cout << " (5) write register" << std::endl;
    std::cout << " (6) backup all registers" << std::endl;
    std::cout << " (7) restore all registers" << std::endl;
    std::cout << " (t) set time (according to system time)" << std::endl;
    std::cout << " (z) read time" << std::endl;
    std::cout << " (x) leave sub-menu" << std::endl;
}

//----------------------------------------------------------------------------
void UISingleModule::onMenuChoice()
{
    switch (this->last_choice) {
    case '2':
        readVersion();
        break;
    case '4':
        readRegister();
        break;
    case '5':
        writeRegister();
        break;
    case '6':
        backupModule();
        break;
    case '7':
        restoreModule();
        break;
    case 't':
        setTime();
        break;
    case 'z':
        getTime();
        break;
    case 'x':
        leave_menu = true;
        break;
    default:
        UIConsole::onMenuChoice();
        break;
    }
}

//----------------------------------------------------------------------------
void UISingleModule::readVersion()
{
    uint16_t selected_module_id = app.getSelectedModule();
    int32_t major = 0, minor = 0, bugfix = 0, dummy;
    uint32_t hash = 0;
    std::string versionString;

    if (selected_module_id == 0) {
        std::cout << "no module selected!" << std::endl;
        return;
    }
    std::cout << "Read version of module ID: 0x" << std::hex << selected_module_id << std::endl;
    try {
        app.readRegister(selected_module_id, MOD_eReg_AppVersionMajor, major);
        app.readRegister(selected_module_id, MOD_eReg_AppVersionMinor, minor);
        app.readRegister(selected_module_id, MOD_eReg_AppVersionBugfix, bugfix);
    }
    catch (Exception &e) {
        std::cout << "reading version failed!\n" << e.what() << std::endl;
    }
    try {
        app.readRegister(selected_module_id, MOD_eReg_AppVersionHash, dummy);
        hash = dummy;
        std::cout << "Version: " << major << "." << minor << "." << bugfix << " #" << std::hex << hash << std::endl;
    }
    catch (Exception &e) {
        std::cout << "Version: " << major << "." << minor << "." << bugfix << std::endl;
    }
}

//----------------------------------------------------------------------------
void UISingleModule::readRegister()
{
    uint16_t selected_module_id = app.getSelectedModule();
    uint8_t register_id = 0;
    int value = 0;

    if (selected_module_id == 0) {
        std::cout << "no module selected!" << std::endl;
        return;
    }
    std::cout << "Read register of module ID: 0x" << std::hex << selected_module_id << std::endl;
    register_id = queryU8("Please select register: ");
    std::cout << "Module: 0x" << std::hex << selected_module_id << " Register: 0x" << std::hex << +register_id << " ";
    try {
        app.readRegister(selected_module_id, register_id, value);
        std::cout << "Value: " << std::dec << value << " / 0x" << std::hex << value << " successfully read." << std::endl;
    }
    catch (Exception &e) {
        std::cout << "reading failed!" << std::endl;
    }
}

//----------------------------------------------------------------------------
void UISingleModule::writeRegister()
{
    uint16_t selected_module_id = app.getSelectedModule();
    uint8_t register_id = 0;
    int value = 0, read_back_value = 0;

    if (selected_module_id == 0) {
        std::cout << "no module selected!" << std::endl;
        return;
    }
    std::cout << "Write register of module ID: 0x" << std::hex << selected_module_id << std::endl;
    register_id = queryU8("Please select register: ");
    value = queryU32("Please enter value: ");
    std::cout << "Module: 0x" << std::hex << selected_module_id << " Register: 0x" << std::hex << +register_id << " ";
    try {
        app.writeRegister(selected_module_id, register_id, value);
        std::cout << "Value: " << std::dec << value << " / 0x" << std::hex << value << " written." << std::endl;
    }
    catch (Exception &e) {
        std::cout << "writing failed!\n" << e.what() << std::endl;
    }
    std::cout << "Module: 0x" << std::hex << selected_module_id << " Register: 0x" << std::hex << +register_id << " ";
    try {
        app.verifyRegister(selected_module_id, register_id, value, read_back_value);
        std::cout << "Value: " << std::dec << value << " / 0x" << std::hex << value << " successfully verified." << std::endl;
    }
    catch (Exception &e) {
        std::cout << "verification failed: written " << value << " read " << read_back_value << "!" << std::endl;
    }
}

//----------------------------------------------------------------------------
void UISingleModule::backupModule()
{
    uint16_t selected_module_id = app.getSelectedModule();

    try {
        BackupRestore br(app);

        int32_t temp = 0;
        uint16_t appid = 0;
        app.readRegister(selected_module_id, MOD_eReg_AppID, temp);
        appid = static_cast<uint16_t>(temp);
        br.backup(selected_module_id,
                  BackupRestore::createValuesFilename(selected_module_id),
                  BackupRestore::createLayoutFilename(appid));
        fprintf(stdout, "Backup of module ID: 0x%04X registers SUCCESS\n", selected_module_id);
    }
    catch (std::exception &e) {
        fprintf(stdout, "Backup of module ID: 0x%04X registers FAILED\n%s\n", selected_module_id, e.what());
    }
}

//----------------------------------------------------------------------------
void UISingleModule::restoreModule()
{
    uint16_t selected_module_id = app.getSelectedModule();

    try {
        BackupRestore br(app);

        br.restore(selected_module_id, BackupRestore::createValuesFilename(selected_module_id));
        fprintf(stdout, "Restore of module ID: 0x%04X registers SUCCESS\n", selected_module_id);
    }
    catch (std::exception &e) {
        fprintf(stdout, "Restore of module ID: 0x%04X registers FAILED\n%s\n", selected_module_id, e.what());
    }
}

//----------------------------------------------------------------------------
void UISingleModule::setTime()
{
    uint16_t selected_module_id = app.getSelectedModule();
    int year = 0, month = 0, day = 0, dow = 0, hour = 0, minute = 0, second = 0;

    if (selected_module_id == 0) {
        std::cout << "no module selected!" << std::endl;
        return;
    }

    std::time_t t = std::time(0);

    tm *now = std::localtime(&t);
    year = now->tm_year + 1900;
    month = now->tm_mon + 1;
    day = now->tm_mday;
    dow = now->tm_wday;
    hour = now->tm_hour;
    minute = now->tm_min;
    second = now->tm_sec;

    try {
        app.writeRegister(selected_module_id, 223, year);
        app.writeRegister(selected_module_id, 224, month);
        app.writeRegister(selected_module_id, 225, day);
        app.writeRegister(selected_module_id, 226, dow);
        app.writeRegister(selected_module_id, 227, hour);
        app.writeRegister(selected_module_id, 228, minute);
        app.writeRegister(selected_module_id, 229, second);

        fprintf(stdout, "Set time of module ID: 0x%04X %04d-%02d-%02d dow %d %02d:%02d:%02d\n", selected_module_id, year, month,
                day, dow, hour, minute, second);
    }
    catch (Exception &e) {
        fprintf(stdout, "Get time of module ID: 0x%04X FAILED\n", selected_module_id);
    }
}

//----------------------------------------------------------------------------
void UISingleModule::getTime()
{
    uint16_t selected_module_id = app.getSelectedModule();
    int year = 0, month = 0, day = 0, dow = 0, hour = 0, minute = 0, second = 0;

    if (selected_module_id == 0) {
        std::cout << "no module selected!" << std::endl;
        return;
    }
    try {
        app.readRegister(selected_module_id, 223, year);
        app.readRegister(selected_module_id, 224, month);
        app.readRegister(selected_module_id, 225, day);
        app.readRegister(selected_module_id, 226, dow);
        app.readRegister(selected_module_id, 227, hour);
        app.readRegister(selected_module_id, 228, minute);
        app.readRegister(selected_module_id, 229, second);
        fprintf(stdout, "Get time of module ID: 0x%04X %04d-%02d-%02d dow %d %02d:%02d:%02d\n", selected_module_id, year, month,
                day, dow, hour, minute, second);
    }
    catch (Exception &e) {
        fprintf(stdout, "Get time of module ID: 0x%04X FAILED\n", selected_module_id);
    }
}

/** @} */
