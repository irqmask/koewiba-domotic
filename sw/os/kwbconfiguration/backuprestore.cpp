#include "backuprestore.h"

#include <fstream>
#include <iomanip>
#include <iostream>

#include <nlohmann/json.hpp>

#include "exceptions.h"

using namespace nlohmann;


//----------------------------------------------------------------------------
BackupRestore::BackupRestore()
{
}

//----------------------------------------------------------------------------
bool BackupRestore::backup(uint16_t moduleId, std::string regValueFile, std::string regLayoutFile)
{
    // load register layout
    std::vector<BaseRegister> regs = loadLayoutFile(regLayoutFile);

    // prepare values file
    json modules_array = json::array();
    json module = json::object();
    module["nodeid"] = moduleId;

    auto jregs = json::array();

    // read register values from module
    for (auto r : regs) {
        auto jr = json::object();
        jr["index"] = r.index;
        jr["type"] = r.type;
        jr["access"] = regAccessToJson(r.accessMask);
        jr["value"] = 0;
        jr["name"] = r.name;
        jregs.emplace_back(jr);
    }

    module["regs"] = jregs;

    modules_array.emplace_back(module);
    std::cout << modules_array << std::endl;

    std::ofstream o(regValueFile);
    o << std::setw(4) << modules_array << std::endl;
    return false;
}

//----------------------------------------------------------------------------
bool BackupRestore::restore(uint16_t moduleId, std::string regValueFile, std::string regLayoutFile)
{
    (void)moduleId;
    (void)regValueFile;
    (void)regLayoutFile;
    return false;
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
    {RegType::eREG_TYPE_INVALID, nullptr},
    {RegType::eREG_TYPE_U8, "u8"},
    {RegType::eREG_TYPE_U8, "U8"},
    {RegType::eREG_TYPE_U16, "u16"},
    {RegType::eREG_TYPE_U16, "U16"},
    {RegType::eREG_TYPE_U32, "u32"},
    {RegType::eREG_TYPE_U32, "U32"},
    {RegType::eREG_TYPE_I8, "i8"},
    {RegType::eREG_TYPE_I8, "I8"},
    {RegType::eREG_TYPE_I16, "i16"},
    {RegType::eREG_TYPE_I16, "I16"},
    {RegType::eREG_TYPE_I32, "i32"},
    {RegType::eREG_TYPE_I32, "I32"},
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

    json reg_array = layout["register"];
    if (!reg_array.is_array()) {
        throw InvalidParameter(LOC, "layout file %s format wrong. expected entity 'register' beeing an array!", filename.c_str());
    }

    std::vector<BaseRegister> registers;
    for (json::iterator it = reg_array.begin(); it != reg_array.end(); ++it) {
        std::cout << *it << '\n';

        BaseRegister br;
        br.index = getRegIndexFromJson(*it);
        br.type = getRegTypeFromJson(*it);
        br.accessMask = getRegAccessFromJson(*it);
        br.name = getRegNameFromJson(*it);
        registers.push_back(br);
    }
    return registers;
}

