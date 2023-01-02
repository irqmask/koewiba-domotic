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
#include "module_config.h"
#include "msgbroker.h"

using namespace nlohmann;

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

// map TaskState values to JSON as strings
NLOHMANN_JSON_SERIALIZE_ENUM( ValueFormat, {
    {ValueFormat::eBINARY, "bin"},
    {ValueFormat::eOCTAL, "oct"},
    {ValueFormat::eDECIMAL, "dec"},
    {ValueFormat::eHEXADECIMAL, "hex"},
})

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
        jr["access"] = regAccessToString(r.accessMask);
        jr["format"] = r.format;
        jr["value"] = valueToString(r.value, r.format);
        jr["name"] = r.name;
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

    std::vector<BaseRegister> regs = loadValueFile(regValueFile, moduleIdFile);
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
uint16_t BackupRestore::getRegIndexFromJson(const json &j) const
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

//----------------------------------------------------------------------------
RegType BackupRestore::getRegTypeFromJson(const json &j) const
{
    if (!j.contains("type")) {
        throw InvalidParameter(LOC, "Missing parameter 'type'. expected entity 'type' beeing 'u8', 'u16', 'u32', 'i8', 'i16', 'i32' !");
    }
    RegType rt = j["type"].get<RegType>();
    return rt;
}

//----------------------------------------------------------------------------
uint8_t BackupRestore::getRegAccessFromJson(const nlohmann::json &j) const
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
std::string BackupRestore::getRegNameFromJson(const nlohmann::json &j) const
{
    if (!j.contains("name")) {
        throw InvalidParameter(LOC, "Missing parameter 'name'!");
    }
    std::string s = j["name"];

    return s;
}

//----------------------------------------------------------------------------
ValueFormat BackupRestore::getValueFormatFromJson(const nlohmann::json &j) const
{
    ValueFormat format = ValueFormat::eDECIMAL;
    if (j.contains("format")) {
        format = j["format"];
    }
    return format;
}

//----------------------------------------------------------------------------
template <typename T>
T BackupRestore::getRegValueFromJson(const nlohmann::json &j, ValueFormat format) const
{
    if (!j.contains("value")) {
        throw InvalidParameter(LOC, "Missing parameter 'value'!");
    }
    if (!j["value"].is_string()) {
        throw InvalidParameter(LOC, "Expected parameter 'value' beeing string!");
    }
    std::string valuestr = j["value"];
    T v;
    switch (format)
    {
    case ValueFormat::eBINARY:
        v = strtol(valuestr.c_str(), nullptr, 2);
        break;
    case ValueFormat::eOCTAL:
        v = strtol(valuestr.c_str(), nullptr, 8);
        break;
    case ValueFormat::eDECIMAL:
        v = strtol(valuestr.c_str(), nullptr, 10);
        break;
    case ValueFormat::eHEXADECIMAL:
        v = strtol(valuestr.c_str(), nullptr, 16);
        break;
    default:
        throw InvalidParameter(LOC, "unknown format");
    }

    return v;
}

//----------------------------------------------------------------------------
template<typename T>
std::string BackupRestore::valueToBinaryString(T value) const
{
    std::stringstream ss;
    while (value > 0) {
        ss << (value % 2);
        value /= 2;
    }
    std::string binary = ss.str();
    std::reverse(binary.begin(), binary.end());
    return binary;
}

//----------------------------------------------------------------------------
template<typename T>
std::string BackupRestore::valueToString(T value, ValueFormat format) const
{
    std::stringstream ss;
    switch (format)
    {
    case ValueFormat::eBINARY:
        ss << valueToBinaryString(value);
        break;
    case ValueFormat::eOCTAL:
        ss << std::oct << value;
        break;
    case ValueFormat::eDECIMAL:
        ss << value;
        break;
    case ValueFormat::eHEXADECIMAL:
        ss << std::hex << value;
        break;
    default:
        throw InvalidParameter(LOC, "unknown format");
    }
    return ss.str();
}

//----------------------------------------------------------------------------
std::string BackupRestore::regAccessToString(uint8_t access) const
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
        throw ResourceMissing(LOC, "Unable to read layout file %s! %s", filename.c_str(), e.what());
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
        br.format = getValueFormatFromJson(*it);
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
            br.format = getValueFormatFromJson(*it);
            switch (br.type) {
            case RegType::eU8:
                br.value = getRegValueFromJson<uint8_t>(*it, br.format);
                break;
            case RegType::eU16:
                br.value = getRegValueFromJson<uint16_t>(*it, br.format);
                break;
            case RegType::eU32:
                br.value = getRegValueFromJson<uint32_t>(*it, br.format);
                break;
            case RegType::eI8:
                br.value = getRegValueFromJson<int8_t>(*it, br.format);
                break;
            case RegType::eI16:
                br.value = getRegValueFromJson<int16_t>(*it, br.format);
                break;
            case RegType::eI32:
                br.value = getRegValueFromJson<int32_t>(*it, br.format);
                break;
            default:
                br.value = 0;
            }

            regs.push_back(br);
        }
    }

    return regs;
}
