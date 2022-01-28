#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#define ACCESS_READ 0x01;
#define ACCESS_WRITE 0x02;

enum class REG_TYPE
{
    eREGU8,
    eREGU16,
    eREGU32
};


class BaseRegister
{
public:
    uint16_t index;
    REG_TYPE type;
    uint8_t accessMask;
};


template <typename T>
class Register : public BaseRegister
{
public:
    T value;


};


class BackupRestore
{
public:
    BackupRestore();

    bool backup(uint16_t moduleId, std::string regValueFile, std::string regLayoutFile);
    bool restore(uint16_t moduleId, std::string regValueFile, std::string regLayoutFile);

protected:
    nlohmann::json loadLayoutFile(std::string filename);

    std::vector<BaseRegister> registers;
};

