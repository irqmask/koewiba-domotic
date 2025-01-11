#include "backuprestore.h"

#include <fstream>
#include <iomanip>
#include <iostream>

#include <nlohmann/json.hpp>

#include "application.h"
#include "connection.h"
#include "exceptions.h"
#include "log.h"
#include "msgbroker.h"

using namespace nlohmann;

//----------------------------------------------------------------------------
BackupRestore::BackupRestore(Application &app)
    : app(app)
{
}

//----------------------------------------------------------------------------
void BackupRestore::backup(uint16_t moduleId, const std::string & regValueFile, const std::string & regLayoutFile)
{
    bool read_success = true;
    // load register layout
    std::vector<ModuleRegisterJson> regs = loadLayoutFile(regLayoutFile);

    // prepare values file
    json modules_array = json::array();
    json module = json::object();
    module["nodeid"] = moduleId;

    auto jregs = json::array();

    // read register values from module
    for (auto &r : regs) {
        int32_t val = 0;
        if (!app.readRegister(r.index, val)) {
            log_msg(LOG_ERROR, "Unable to read register %d", r.index);
            read_success = false;
        }
        r.value = val;
        log_msg(LOG_INFO, "Read register index %d -> value %d", r.index, r.value);
    }

    // save values to json
    for (auto &r : regs) {
        json jr = r.toJson();
        log_msg(LOG_INFO, "Save register to file: index %d value %d", r.index, r.value);
        jregs.emplace_back(jr);
    }

    module["regs"] = jregs;

    modules_array.emplace_back(module);
    std::cout << modules_array << std::endl;

    std::ofstream o(regValueFile);
    o << std::setw(4) << modules_array << std::endl;
    o.close();
    if (!read_success) {
        throw OperationFailed(LOC, "Error occurred reading register values! Not all registers have been read correctly from the module!");
    }
    log_msg(LOG_INFO, "Backup of registers of module 0x%04X successfully written to file %s", moduleId, regValueFile.c_str());
}

//----------------------------------------------------------------------------
void BackupRestore::restore(uint16_t moduleId,
                            const std::string & regValueFile)
{
    bool write_success = true;
    uint16_t moduleIdFile = 0;
    (void)moduleId;

    std::vector<ModuleRegisterJson> regs = loadValueFile(regValueFile, moduleIdFile);
    if (moduleId != moduleIdFile) {
        log_msg(LOG_ERROR, "Mismatch of moduleId. Module id in file 0x%04X is not equal to selected module id 0x%04X");
        throw OperationFailed(LOC, "Mismatch of moduleId. Module id in file 0x%04X is not equal to selected module id 0x%04X");
    }

    // read register values from module
    for (auto &r : regs) {
        log_msg(LOG_INFO, "Reg index %d value %d", r.index, r.value);
        if ((r.accessMask & eREG_ACCESS_WRITE) == 0) {
            continue;
        }
        if (!app.writeRegister(r.index, r.value)) {
            log_msg(LOG_ERROR, "Unable to write register %d, value %d", r.index, r.value);
            write_success = false;
            continue;
        }
        int32_t read_value = 0;
        if (!app.readRegister(r.index, read_value)) {
            log_msg(LOG_ERROR, "Unable to verify register %d", r.index);
            write_success = false;
            continue;
        }
        if (read_value != r.value) {
            log_msg(LOG_ERROR, "register %d value mismatch. written %d, read %d", r.index, r.value, read_value);
            write_success = false;
            continue;
        }
    }
    if (!write_success) {
        throw OperationFailed(LOC, "Error occurred writing register values! Not all registers have been written correctly to the module!");
    }
}

//----------------------------------------------------------------------------
std::string BackupRestore::createValuesFilename(uint16_t moduleId)
{
    std::stringstream filename;
    filename << "backup" << std::hex << std::setfill('0') << std::setw(4) << moduleId << ".json";
    return filename.str();
}

//----------------------------------------------------------------------------
std::string BackupRestore::createLayoutFilename(uint16_t appId)
{
    std::stringstream filename;
    filename << "layout" << std::hex << std::setfill('0') << std::setw(4) << appId << ".json";
    return filename.str();
}

//----------------------------------------------------------------------------
std::vector<ModuleRegisterJson> BackupRestore::loadLayoutFile(std::string filename)
{
    json layout;
    try {
        std::ifstream ifs(filename, std::ios::in);
        ifs >> layout;
    }
    catch (std::exception & e) {
        throw ResourceMissing(LOC, "Unable to read layout file %s! %s", filename.c_str(), e.what());
    }

    json reg_array = layout["regs"];
    if (!reg_array.is_array()) {
        throw InvalidParameter(LOC, "layout file %s format wrong. expected entity 'regs' beeing an array!", filename.c_str());
    }

    std::vector<ModuleRegisterJson> regs;
    for (json::iterator it = reg_array.begin(); it != reg_array.end(); ++it) {
        std::cout << *it << '\n';

        ModuleRegisterJson br;
        br.fromJson(*it);
        regs.push_back(br);
    }
    return regs;
}

//----------------------------------------------------------------------------
std::vector<ModuleRegisterJson> BackupRestore::loadValueFile(std::string filename,
                                                             uint16_t &moduleIdFile)
{
    json all_modules_all_regs;
    try {
        std::ifstream ifs(filename, std::ios::in);
        ifs >> all_modules_all_regs;
    }
    catch (std::exception & e) {
        throw ResourceMissing(LOC, "Unable to read values file! %s\n%s", filename.c_str(), e.what());
    }
    std::vector<ModuleRegisterJson> regs;

    if (!all_modules_all_regs.is_array()) {
        throw InvalidParameter(LOC, "value file %s format wrong. expected outer entity beeing an array!", filename.c_str());
    }
    for (auto & jmod : all_modules_all_regs) {
        if (!jmod.contains("nodeid")) {
            throw InvalidParameter(LOC, "expected module to have parameter nodeid!");
        }
        if (!jmod["nodeid"].is_number_unsigned()) {
            throw InvalidParameter(LOC, "expected nodeid beeing unsigned integer!");
        }

        moduleIdFile = jmod.value<uint16_t>("nodeid", 0);
        if (!jmod.contains("regs")) {
            throw InvalidParameter(LOC, "expected module to have array 'regs'!");
        }
        if (!jmod["regs"].is_array()) {
            throw InvalidParameter(LOC, "expected module to have array 'regs'!");
        }

        auto &jregs = jmod["regs"];
        for (json::iterator it = jregs.begin(); it != jregs.end(); ++it) {
            std::cout << *it << '\n';

            ModuleRegisterJson br;
            br.fromJson(*it);
            regs.push_back(br);
        }
    }

    return regs;
}
