#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "cfg_module.hpp"
#include "cfg_module_register.hpp"
#include "cfg_module_register_json.h"

class Application;

class BackupRestore
{
public:
    BackupRestore(Application &conn);

    void backup(uint16_t moduleId, const std::string &regValueFile, const std::string &regLayoutFile);
    void restore(uint16_t moduleId, const std::string &regValueFile);

    static std::string createValuesFilename(uint16_t moduleId);
    static std::string createLayoutFilename(uint16_t appId);

protected:
    std::vector<ModuleRegisterJson> loadLayoutFile(std::string filename);

    std::vector<ModuleRegisterJson> loadValueFile(std::string filename, uint16_t &moduleIdFile);
    void saveValueFile(std::string filename, uint16_t moduleId, const std::vector<ModuleRegisterJson> &regs);

    std::vector<ModuleRegisterJson> registers;

    Application &app;
};
