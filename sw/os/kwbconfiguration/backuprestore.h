#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "module_config.h"


class Application;

class BackupRestore
{
public:
    BackupRestore(Application &conn);

    void backup(uint16_t moduleId, const std::string & regValueFile, const std::string & regLayoutFile);
    void restore(uint16_t moduleId, const std::string & regValueFile);

protected:

    uint16_t getRegIndexFromJson(const nlohmann::json &j) const;
    RegType getRegTypeFromJson(const nlohmann::json &j) const;
    uint8_t getRegAccessFromJson(const nlohmann::json &j) const;
    std::string getRegNameFromJson(const nlohmann::json &j) const;
    ValueFormat getValueFormatFromJson(const nlohmann::json &j) const;
    template <typename T>
    T getRegValueFromJson(const nlohmann::json &j, ValueFormat format) const;
    template <typename T>
    std::string valueToBinaryString(T value) const;
    template <typename T>
    std::string valueToString(T value, ValueFormat format) const;
    std::string regAccessToString(uint8_t access) const;

    std::vector<BaseRegister> loadLayoutFile(std::string filename);

    std::vector<BaseRegister> loadValueFile(std::string filename, uint16_t &moduleIdFile);
    void saveValueFile(std::string filename, uint16_t moduleId, const std::vector<BaseRegister>& regs);

    std::vector<BaseRegister> registers;

    Application &app;
};

