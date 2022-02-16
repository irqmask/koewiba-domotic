#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#define ACCESS_READ 0x01;
#define ACCESS_WRITE 0x02;

enum class RegType : uint8_t
{
    eREG_TYPE_INVALID = 0,
    eREG_TYPE_U8 = 1,
    eREG_TYPE_U16,
    eREG_TYPE_U32,
    eREG_TYPE_I8,
    eREG_TYPE_I16,
    eREG_TYPE_I32
};


enum RegAccess
{
    eREG_ACCESS_READ = 1,
    eREG_ACCESS_WRITE = 2
};


class BaseRegister
{
public:
    uint16_t index;
    RegType type;
    uint8_t accessMask;
    std::string name;
    int32_t value;
};


template <typename T>
class Register : public BaseRegister
{
public:
    T value;


};


class Application;

class BackupRestore
{
public:
    BackupRestore(Application &conn);

    bool backup(uint16_t moduleId, std::string regValueFile, std::string regLayoutFile);
    bool restore(uint16_t moduleId, std::string regValueFile, std::string regLayoutFile);

protected:

    uint16_t getRegIndexFromJson(const nlohmann::json &j);
    RegType getRegTypeFromJson(const nlohmann::json &j);
    uint8_t getRegAccessFromJson(const nlohmann::json &j);
    std::string getRegNameFromJson(const nlohmann::json &j);

    std::string regAccessToJson(uint8_t access);

    std::vector<BaseRegister> loadLayoutFile(std::string filename);

    std::vector<BaseRegister> loadValueFile(std::string filename);
    void saveValueFile(std::string filename, const std::vector<BaseRegister>& regs);

    std::vector<BaseRegister> registers;

    Application &app;
};

