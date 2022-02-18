#include "backuprestore.h"

#include <fstream>
#include <iomanip>
#include <iostream>

#include <nlohmann/json.hpp>

#include "action_query_modules.h"
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
    std::vector<BaseRegister> regs = loadLayoutFile(regLayoutFile);

    // prepare values file
    json modules_array = json::array();
    json module = json::object();
    module["nodeid"] = moduleId;

    auto jregs = json::array();

    // read register values from module
    for (auto &r : regs) {
        if (!app.readRegister(r.index, r.value)) {
            log_msg(LOG_ERROR, "Unable to read register %d", r.index);
            read_success = false;
        }

        log_msg(LOG_INFO, "Reg index %d value %d", r.index, r.value);
    }

    // save values to json
    for (auto &r : regs) {
        log_msg(LOG_INFO, "Reg index %d value %d", r.index, r.value);
        auto jr = json::object();
        jr["index"] = r.index;
        jr["type"] = r.type;
        jr["access"] = regAccessToJson(r.accessMask);
        jr["value"] = r.value;
        jr["name"] = r.name;
        jregs.emplace_back(jr);
    }

    module["regs"] = jregs;

    modules_array.emplace_back(module);
    std::cout << modules_array << std::endl;

    std::ofstream o(regValueFile);
    o << std::setw(4) << modules_array << std::endl;

    if (!read_success) {
        throw OperationFailed(LOC, "Error occurred reading register values! Not all registers have been read correctly from the module!");
    }
}

//----------------------------------------------------------------------------
void BackupRestore::restore(uint16_t moduleId,
                            const std::string & regValueFile)
{
    bool write_success = true;
    uint16_t moduleIdFile = 0;
    (void)moduleId;

    std::vector<BaseRegister> regs = loadValueFile(regValueFile, moduleIdFile);
    if (moduleId != moduleIdFile) {
        log_msg(LOG_ERROR, "Mismatch of moduleId. Module id in file 0x%04X is not equal to selected module id 0x%04X");
        throw OperationFailed(LOC, "Mismatch of moduleId. Module id in file 0x%04X is not equal to selected module id 0x%04X");
    }

    // read register values from module
    for (auto &r : regs) {
        if ((r.accessMask & eREG_ACCESS_WRITE) == 0) {
            continue;
        }
        /*if (!app.writeRegister(r.index, r.value)) {
            log_msg(LOG_ERROR, "Unable to write register %d, value %d", r.index, r.value);
            write_success = false;
        }*/

        log_msg(LOG_INFO, "Reg index %d value %d", r.index, r.value);
    }
    if (!write_success) {
        throw OperationFailed(LOC, "Error occurred writing register values! Not all registers have been written correctly to the module!");
    }
}

//----------------------------------------------------------------------------
uint16_t BackupRestore::getRegIndexFromJson(const json &j)
{

    if (!j.contains("index")) {
        throw InvalidParameter(LOC, "Missing parameter 'index'. expected entity 'index' beeing unsigned 16bit integer!");
    }
    if (!j["index"].is_number_unsigned()) {
        std::string kv = j;
        throw InvalidParameter(LOC, "Invalid format of parameter 'index'. expected entity 'index' beeing unsigned 16bit integer but is %s!", kv.c_str());
    }

    uint16_t val = j.value<uint16_t>("index", 0);

    return val;
}

NLOHMANN_JSON_SERIALIZE_ENUM( RegType, {
    {RegType::eINVALID, nullptr},
    {RegType::eU8, "u8"},
    {RegType::eU8, "U8"},
    {RegType::eU16, "u16"},
    {RegType::eU16, "U16"},
    {RegType::eU32, "u32"},
    {RegType::eU32, "U32"},
    {RegType::eI8, "i8"},
    {RegType::eI8, "I8"},
    {RegType::eI16, "i16"},
    {RegType::eI16, "I16"},
    {RegType::eI32, "i32"},
    {RegType::eI32, "I32"},
})

//----------------------------------------------------------------------------
RegType BackupRestore::getRegTypeFromJson(const json &j)
{
    if (!j.contains("type")) {
        throw InvalidParameter(LOC, "Missing parameter 'type'. expected entity 'type' beeing 'u8', 'u16', 'u32', 'i8', 'i16', 'i32' !");
    }
    RegType rt = j["type"].get<RegType>();
    return rt;
}

//----------------------------------------------------------------------------
uint8_t BackupRestore::getRegAccessFromJson(const nlohmann::json &j)
{
    if (!j.contains("access")) {
        throw InvalidParameter(LOC, "Missing parameter 'access'. expected entity 'access' beeing '', 'r', 'rw' !");
    }
    std::string s = j["access"];
    if (s == "r") {
        return RegAccess::eREG_ACCESS_READ;
    }
    else if (s == "rw") {
        return RegAccess::eREG_ACCESS_READ | RegAccess::eREG_ACCESS_WRITE;
    }
    else {
        return 0;
    }
}

//----------------------------------------------------------------------------
std::string BackupRestore::getRegNameFromJson(const nlohmann::json &j)
{
    if (!j.contains("name")) {
        throw InvalidParameter(LOC, "Missing parameter 'name'!");
    }
    std::string s = j["name"];

    return s;
}

//----------------------------------------------------------------------------
template <typename T>
T BackupRestore::getRegValueFromJson(const nlohmann::json &j)
{
    if (!j.contains("value")) {
        throw InvalidParameter(LOC, "Missing parameter 'value'!");
    }
    if (!j["value"].is_number()) {
        throw InvalidParameter(LOC, "Expected parameter 'value' beeing value!");
    }
    T v = j.value<T>("value", 0);

    return v;
}

//----------------------------------------------------------------------------
std::string BackupRestore::regAccessToJson(uint8_t access)
{
    switch (access) {
    case eREG_ACCESS_READ:
        return "r";
    case (eREG_ACCESS_READ | eREG_ACCESS_WRITE):
        return "rw";
    default:
        return "";
    }
}

//----------------------------------------------------------------------------
std::vector<BaseRegister> BackupRestore::loadLayoutFile(std::string filename)
{
    json layout;
    try {
        std::ifstream ifs(filename, std::ios::in);
        ifs >> layout;
    }
    catch (std::exception & e) {
        throw ResourceMissing(LOC, "Unable to read layout file! %s", e.what());
    }

    json reg_array = layout["regs"];
    if (!reg_array.is_array()) {
        throw InvalidParameter(LOC, "layout file %s format wrong. expected entity 'regs' beeing an array!", filename.c_str());
    }

    std::vector<BaseRegister> regs;
    for (json::iterator it = reg_array.begin(); it != reg_array.end(); ++it) {
        std::cout << *it << '\n';

        BaseRegister br;
        br.index = getRegIndexFromJson(*it);
        br.type = getRegTypeFromJson(*it);
        br.accessMask = getRegAccessFromJson(*it);
        br.name = getRegNameFromJson(*it);
        regs.push_back(br);
    }
    return regs;
}

//----------------------------------------------------------------------------
std::vector<BaseRegister> BackupRestore::loadValueFile(std::string filename,
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
    std::vector<BaseRegister> regs;

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

            BaseRegister br;
            br.index = getRegIndexFromJson(*it);
            br.type = getRegTypeFromJson(*it);
            br.accessMask = getRegAccessFromJson(*it);
            br.name = getRegNameFromJson(*it);
            switch (br.type) {
            case RegType::eU8:
                br.value = getRegValueFromJson<uint8_t>(*it);
                break;
            case RegType::eU16:
                br.value = getRegValueFromJson<uint16_t>(*it);
                break;
            case RegType::eU32:
                br.value = getRegValueFromJson<uint32_t>(*it);
                break;
            case RegType::eI8:
                br.value = getRegValueFromJson<int8_t>(*it);
                break;
            case RegType::eI16:
                br.value = getRegValueFromJson<int16_t>(*it);
                break;
            case RegType::eI32:
                br.value = getRegValueFromJson<int32_t>(*it);
                break;
            default:
                br.value = 0;
            }

            regs.push_back(br);
        }
    }

    return regs;
}

