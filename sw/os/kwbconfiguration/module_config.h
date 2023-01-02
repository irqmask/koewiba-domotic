#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#define ACCESS_READ 0x01;
#define ACCESS_WRITE 0x02;

enum class RegType : uint8_t
{
    eINVALID = 0,
    eU8 = 1,
    eU16,
    eU32,
    eI8,
    eI16,
    eI32
};


enum RegAccess
{
    eREG_ACCESS_READ = 1,
    eREG_ACCESS_WRITE = 2
};

enum class ValueFormat
{
    eBINARY,
    eDECIMAL,
    eOCTAL,
    eHEXADECIMAL
};

class BaseRegister
{
public:
    uint16_t index;
    RegType type;
    uint8_t accessMask;
    std::string name;
    int32_t value;
    ValueFormat format = ValueFormat::eDECIMAL;
};


template <typename T>
class Register : public BaseRegister
{
public:
    T value;
};


class Module
{
public:
    uint16_t nodeId;    ///< Node ID of the module
    uint16_t appId;     ///< application ID as defined in moddef_common.h
    std::string name;   ///< name of the module
    std::string description; ///< description of the module (e.g. purpose, location,...)
};


class ModuleConfig
{
public:
    ModuleConfig();

    static std::string          createValuesFilename(uint16_t moduleId);
    static std::string          createLayoutFilename(uint16_t appId);

    std::vector<Module>         loadModulesFile(const std::string &filename);

    std::vector<BaseRegister>   loadLayoutFile(const std::string &filename);

    std::vector<BaseRegister>   loadValueFile(const std::string &filename, uint16_t &moduleIdFile);
    void                        saveValueFile(const std::string &filename, uint16_t moduleId, const std::vector<BaseRegister> &regs);

protected:
    uint16_t getRegIndexFromJson(const nlohmann::json &j);
    RegType getRegTypeFromJson(const nlohmann::json &j);
    uint8_t getRegAccessFromJson(const nlohmann::json &j);
    std::string getRegNameFromJson(const nlohmann::json &j);
    template <typename T>
    T getRegValueFromJson(const nlohmann::json &j);

    std::string regAccessToJson(uint8_t access);
};


class ModuleLayout : public ModuleConfig
{
    ModuleLayout(const std::string &layoutFilename);
};
