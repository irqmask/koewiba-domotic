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

