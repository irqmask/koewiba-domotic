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

    uint16_t getRegIndexFromJson(const nlohmann::json &j);
    RegType getRegTypeFromJson(const nlohmann::json &j);
    uint8_t getRegAccessFromJson(const nlohmann::json &j);
    std::string getRegNameFromJson(const nlohmann::json &j);
    template <typename T>
    T getRegValueFromJson(const nlohmann::json &j);

    std::string regAccessToJson(uint8_t access);

    std::vector<BaseRegister> loadLayoutFile(std::string filename);

    std::vector<BaseRegister> loadValueFile(std::string filename, uint16_t &moduleIdFile);
    void saveValueFile(std::string filename, uint16_t moduleId, const std::vector<BaseRegister>& regs);

    std::vector<BaseRegister> registers;

    Application &app;
};

