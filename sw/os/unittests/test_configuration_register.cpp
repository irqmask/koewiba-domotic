/**
 * @addtogroup UNITTESTS
 *
 * @{
 * @file    test_configuration_register.cpp
 * @brief   This module contains the unittests for the ModuleRegisterJson class.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2024  christian <irqmask@gmx.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// --- Include section ---------------------------------------------------------

#include <string>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "prjconf.h"

// include
#include "prjtypes.h"
// libsystem
#include "systime.h"
// kwbconfiguration
#include "../kwbconfiguration/cfg_module_register_json.h"
#include "../kwbconfiguration/cfg_module_register_json.cpp"

using namespace nlohmann;

// --- Definitions -------------------------------------------------------------

// --- Type definitions --------------------------------------------------------

// --- Local variables ---------------------------------------------------------

// --- Global variables --------------------------------------------------------

// --- Module global variables -------------------------------------------------

// --- Local functions ---------------------------------------------------------

template <typename Enumeration>
auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

// --- Module global functions -------------------------------------------------

// --- Global functions --------------------------------------------------------

/// @test Test valid values for index parameter
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_IndexOK)
{
    ModuleRegisterJson r;
    EXPECT_NO_THROW(r.fromJson("{\"index\":0,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.index, 0);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.index, 1);
    EXPECT_NO_THROW(r.fromJson("{\"index\":65535,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.index, 65535);
}

/// @test Test to parse invalid values for 'index' parameter
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_IndexNOK)
{
    ModuleRegisterJson r;
    EXPECT_THROW(r.fromJson("{\"type\":\"U8\",\"name\":\"abc\",\"access\":\"rw\"}"), InvalidParameter);
    EXPECT_THROW(r.fromJson("{\"index\":-1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"rw\"}"), InvalidParameter);
    EXPECT_THROW(r.fromJson("{\"index\":65536,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"rw\"}"), InvalidParameter);
    EXPECT_THROW(r.fromJson("{\"index\":\"\",\"type\":\"U8\",\"name\":\"abc\",\"access\":\"rw\"}"), InvalidParameter);
    EXPECT_THROW(r.fromJson("{\"index\":\"a\",\"type\":\"U8\",\"name\":\"abc\",\"access\":\"rw\"}"), InvalidParameter);
    EXPECT_THROW(r.fromJson("{\"index\":\"z\",\"type\":\"U8\",\"name\":\"abc\",\"access\":\"rw\"}"), InvalidParameter);
}

/// @test Test to parse different register types
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_TypeOK)
{
    ModuleRegisterJson r;
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.type, RegType::eU8);

    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"u8\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.type, RegType::eU8);

    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U16\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.type, RegType::eU16);

    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"u16\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.type, RegType::eU16);

    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U32\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.type, RegType::eU32);

    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"u32\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.type, RegType::eU32);

    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I8\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.type, RegType::eI8);

    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"i8\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.type, RegType::eI8);

    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I16\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.type, RegType::eI16);

    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"i16\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.type, RegType::eI16);

    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I32\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.type, RegType::eI32);

    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"i32\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.type, RegType::eI32);
}

/// @test Test to parse invalid values for 'type' parameter
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_TypeNOK)
{
    ModuleRegisterJson r;
    r.type = RegType::eI32;
    EXPECT_THROW(r.fromJson("{\"index\":1,\"type\":\"Z8\",\"name\":\"abc\",\"access\":\"rw\"}"), InvalidParameter);
    EXPECT_EQ(r.type, RegType::eI32) << "value changed, but shouldn't";
    EXPECT_THROW(r.fromJson("{\"index\":1,\"name\":\"abc\",\"access\":\"rw\"}"), InvalidParameter);
}

/// @test Test valid values for 'name' parameter
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_NameOK)
{
    ModuleRegisterJson r;
    EXPECT_NO_THROW(r.fromJson("{\"index\":0,\"type\":\"U8\",\"name\":\"a\",\"access\":\"rw\"}"));
    EXPECT_STREQ(r.name.c_str(), "a");
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"TestText\",\"access\":\"rw\"}"));
    EXPECT_STREQ(r.name.c_str(), "TestText");
}

/// @test Test to parse invalid values for 'name' parameter
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_NameNOK)
{
    ModuleRegisterJson r;
    EXPECT_THROW(r.fromJson("{\"index\":0,\"type\":\"U8\",\"name\":\"\",\"access\":\"rw\"}"), InvalidParameter);
    EXPECT_THROW(r.fromJson("{\"index\":0,\"type\":\"U8\",\"name\":12,\"access\":\"rw\"}"), InvalidParameter);
    EXPECT_THROW(r.fromJson("{\"index\":0,\"type\":\"U8\",\"access\":\"rw\"}"), InvalidParameter);
}

/// @test Test valid values for 'access' parameter
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_AccessOK)
{
    ModuleRegisterJson r;
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\"}"));
    EXPECT_EQ(r.accessMask, eREG_ACCESS_READ);
    EXPECT_NO_THROW(r.fromJson("{\"index\":65535,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"rw\"}"));
    EXPECT_EQ(r.accessMask, eREG_ACCESS_READ | eREG_ACCESS_WRITE);
}

/// @test Test to parse invalid values for 'access' parameter
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_AccessNOK)
{
    ModuleRegisterJson r;
    EXPECT_THROW(r.fromJson("{\"index\":2,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"\"}"), InvalidParameter);
    EXPECT_THROW(r.fromJson("{\"index\":2,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"x\"}"), InvalidParameter);
    EXPECT_THROW(r.fromJson("{\"index\":2,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"w\"}"), InvalidParameter);
    EXPECT_THROW(r.fromJson("{\"index\":2,\"type\":\"U8\",\"name\":\"abc\",\"access\":\" r\"}"), InvalidParameter);
}

/// @test Test valid values for 'format' parameter
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_FormatOK)
{
    ModuleRegisterJson r;
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"bin\"}"));
    EXPECT_EQ(r.format, ValueFormat::eBINARY);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"oct\"}"));
    EXPECT_EQ(r.format, ValueFormat::eOCTAL);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"dec\"}"));
    EXPECT_EQ(r.format, ValueFormat::eDECIMAL);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"hex\"}"));
    EXPECT_EQ(r.format, ValueFormat::eHEXADECIMAL);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\"}"));
    EXPECT_EQ(r.format, ValueFormat::eDECIMAL);
}

/// @test Test to parse invalid values for 'format' parameter
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_FormatNOK)
{
    ModuleRegisterJson r;
    EXPECT_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"DEC\"}"), InvalidParameter);
    EXPECT_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"brr\"}"), InvalidParameter);
    EXPECT_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":5}"), InvalidParameter);
}

/// @test Test to parse invalid JSON formats for whole ModuleRegister
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_NOK)
{
    ModuleRegisterJson r;
    ASSERT_THROW(r.fromJson("{}"), InvalidParameter);
    ASSERT_THROW(r.fromJson("{\"unwanted_property\":45}"), InvalidParameter);
}

/// @test Test valid values for 'value' parameter
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_ValueOK)
{
    ModuleRegisterJson r;
    r.value = 33;
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"bin\"}"));
    EXPECT_EQ(r.value, 0);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"bin\", \"value\":\"0\"}"));
    EXPECT_EQ(r.value, 0);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"bin\", \"value\":\"10\"}"));
    EXPECT_EQ(r.value, 2);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"oct\", \"value\":\"10\"}"));
    EXPECT_EQ(r.value, 8);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"dec\", \"value\":\"10\"}"));
    EXPECT_EQ(r.value, 10);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"hex\", \"value\":\"10\"}"));
    EXPECT_EQ(r.value, 16);
}

/// @test Test valid binary numbers
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_ValueBinaryOK)
{
    ModuleRegisterJson r;
    r.value = 33;
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"bin\", \"value\":\"11111111\"}"));
    EXPECT_EQ(r.value, 0xFF);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U16\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"bin\", \"value\":\"1111111111111111\"}"));
    EXPECT_EQ(r.value, 0xFFFF);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U32\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"bin\", \"value\":\"11111111111111111111111111111111\"}"));
    EXPECT_EQ(r.value, 0xFFFFFFFF);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"bin\", \"value\":\"11111111\"}"));
    EXPECT_EQ(r.value, -1);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I16\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"bin\", \"value\":\"1111111111111111\"}"));
    EXPECT_EQ(r.value, -1);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I32\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"bin\", \"value\":\"11111111111111111111111111111111\"}"));
    EXPECT_EQ(r.value, -1);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"bin\", \"value\":\"10000000\"}"));
    EXPECT_EQ(r.value, INT8_MIN);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I16\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"bin\", \"value\":\"1000000000000000\"}"));
    EXPECT_EQ(r.value, INT16_MIN);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I32\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"bin\", \"value\":\"10000000000000000000000000000000\"}"));
    EXPECT_EQ(r.value, INT32_MIN);
}

/// @test Test valid octal numbers
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_ValueOctalOK)
{
    ModuleRegisterJson r;
    r.value = 33;
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"oct\", \"value\":\"377\"}"));
    EXPECT_EQ(r.value, 0xFF);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U16\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"oct\", \"value\":\"177777\"}"));
    EXPECT_EQ(r.value, 0xFFFF);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U32\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"oct\", \"value\":\"37777777777\"}"));
    EXPECT_EQ(r.value, 0xFFFFFFFF);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"oct\", \"value\":\"377\"}"));
    EXPECT_EQ(r.value, -1);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I16\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"oct\", \"value\":\"177777\"}"));
    EXPECT_EQ(r.value, -1);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I32\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"oct\", \"value\":\"37777777777\"}"));
    EXPECT_EQ(r.value, -1);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"oct\", \"value\":\"200\"}"));
    EXPECT_EQ(r.value, INT8_MIN);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I16\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"oct\", \"value\":\"100000\"}"));
    EXPECT_EQ(r.value, INT16_MIN);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I32\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"oct\", \"value\":\"20000000000\"}"));
    EXPECT_EQ(r.value, INT32_MIN);
}

/// @test Test valid decimal numbers
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_ValueDecimalOK)
{
    ModuleRegisterJson r;
    r.value = 33;
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"dec\", \"value\":\"255\"}"));
    EXPECT_EQ(r.value, 0xFF);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U16\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"dec\", \"value\":\"65535\"}"));
    EXPECT_EQ(r.value, 0xFFFF);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U32\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"dec\", \"value\":\"4294967295\"}"));
    EXPECT_EQ(r.value, 0xFFFFFFFF);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"dec\", \"value\":\"-1\"}"));
    EXPECT_EQ(r.value, -1);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I16\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"dec\", \"value\":\"-1\"}"));
    EXPECT_EQ(r.value, -1);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I32\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"dec\", \"value\":\"-1\"}"));
    EXPECT_EQ(r.value, -1);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"dec\", \"value\":\"-128\"}"));
    EXPECT_EQ(r.value, INT8_MIN);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I16\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"dec\", \"value\":\"-32768\"}"));
    EXPECT_EQ(r.value, INT16_MIN);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I32\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"dec\", \"value\":\"-2147483648\"}"));
    EXPECT_EQ(r.value, INT32_MIN);
}

/// @test Test valid hexadecimal numbers
TEST(kwbconfiguration_ModuleRegister, ModuleRegister_ValueHexadecimalOK)
{
    ModuleRegisterJson r;
    r.value = 33;
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"hex\", \"value\":\"FF\"}"));
    EXPECT_EQ(r.value, 0xFF);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U16\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"hex\", \"value\":\"FFFF\"}"));
    EXPECT_EQ(r.value, 0xFFFF);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"U32\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"hex\", \"value\":\"FFFFFFFF\"}"));
    EXPECT_EQ(r.value, 0xFFFFFFFF);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"hex\", \"value\":\"FF\"}"));
    EXPECT_EQ(r.value, -1);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I16\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"hex\", \"value\":\"FFFF\"}"));
    EXPECT_EQ(r.value, -1);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I32\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"hex\", \"value\":\"FFFFFFFF\"}"));
    EXPECT_EQ(r.value, -1);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I8\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"hex\", \"value\":\"80\"}"));
    EXPECT_EQ(r.value, INT8_MIN);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I16\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"hex\", \"value\":\"8000\"}"));
    EXPECT_EQ(r.value, INT16_MIN);
    EXPECT_NO_THROW(r.fromJson("{\"index\":1,\"type\":\"I32\",\"name\":\"abc\",\"access\":\"r\",\"format\":\"hex\", \"value\":\"80000000\"}"));
    EXPECT_EQ(r.value, INT32_MIN);
}


TEST(kwbconfiguration_ModuleRegister, ModuleRegister_toJsonU8OK)
{
    {
        ModuleRegisterJson r(123, RegType::eU8, eREG_ACCESS_READ, "testu8", ValueFormat::eBINARY, 0xC4);
        EXPECT_STREQ(r.toJsonStr().c_str(), "{\"access\":\"r\",\"format\":\"bin\",\"index\":123,\"name\":\"testu8\",\"type\":\"u8\",\"value\":\"11000100\"}");
    }
    {
        ModuleRegisterJson r(123, RegType::eU8, eREG_ACCESS_READ, "testu8", ValueFormat::eOCTAL, 0xC4);
        EXPECT_STREQ(r.toJsonStr().c_str(), "{\"access\":\"r\",\"format\":\"oct\",\"index\":123,\"name\":\"testu8\",\"type\":\"u8\",\"value\":\"304\"}");
    }
    {
        ModuleRegisterJson r(123, RegType::eU8, eREG_ACCESS_READ, "testu8", ValueFormat::eDECIMAL, 0xC4);
        EXPECT_STREQ(r.toJsonStr().c_str(), "{\"access\":\"r\",\"format\":\"dec\",\"index\":123,\"name\":\"testu8\",\"type\":\"u8\",\"value\":\"196\"}");
    }
    {
        ModuleRegisterJson r(123, RegType::eU8, eREG_ACCESS_READ, "testu8", ValueFormat::eHEXADECIMAL, 0xC4);
        EXPECT_STREQ(r.toJsonStr().c_str(), "{\"access\":\"r\",\"format\":\"hex\",\"index\":123,\"name\":\"testu8\",\"type\":\"u8\",\"value\":\"c4\"}");
    }
}


TEST(kwbconfiguration_ModuleRegister, ModuleRegister_toJsonU16OK)
{
    {
        ModuleRegisterJson r(1, RegType::eU16, eREG_ACCESS_READ | eREG_ACCESS_WRITE, "testu16", ValueFormat::eBINARY, 547);
        EXPECT_STREQ(r.toJsonStr().c_str(), "{\"access\":\"rw\",\"format\":\"bin\",\"index\":1,\"name\":\"testu16\",\"type\":\"u16\",\"value\":\"1000100011\"}");
    }
    {
        ModuleRegisterJson r(1, RegType::eU16, eREG_ACCESS_READ | eREG_ACCESS_WRITE, "testu16", ValueFormat::eOCTAL, 547);
        EXPECT_STREQ(r.toJsonStr().c_str(), "{\"access\":\"rw\",\"format\":\"oct\",\"index\":1,\"name\":\"testu16\",\"type\":\"u16\",\"value\":\"1043\"}");
    }
    {
        ModuleRegisterJson r(1, RegType::eU16, eREG_ACCESS_READ | eREG_ACCESS_WRITE, "testu16", ValueFormat::eDECIMAL, 547);
        EXPECT_STREQ(r.toJsonStr().c_str(), "{\"access\":\"rw\",\"format\":\"dec\",\"index\":1,\"name\":\"testu16\",\"type\":\"u16\",\"value\":\"547\"}");
    }
    {
        ModuleRegisterJson r(1, RegType::eU16, eREG_ACCESS_READ | eREG_ACCESS_WRITE, "testu16", ValueFormat::eHEXADECIMAL, 547);
        EXPECT_STREQ(r.toJsonStr().c_str(), "{\"access\":\"rw\",\"format\":\"hex\",\"index\":1,\"name\":\"testu16\",\"type\":\"u16\",\"value\":\"223\"}");
    }
}


TEST(kwbconfiguration_ModuleRegister, ModuleRegister_toJsonU32OK)
{
    {
        ModuleRegisterJson r(240, RegType::eU32, eREG_ACCESS_READ, "testu32", ValueFormat::eBINARY, 252115721);
        EXPECT_STREQ(r.toJsonStr().c_str(), "{\"access\":\"r\",\"format\":\"bin\",\"index\":240,\"name\":\"testu32\",\"type\":\"u32\",\"value\":\"1111000001101111101100001001\"}");
    }
    {
        ModuleRegisterJson r(240, RegType::eU32, eREG_ACCESS_READ, "testu32", ValueFormat::eHEXADECIMAL, 252115721);
        EXPECT_STREQ(r.toJsonStr().c_str(), "{\"access\":\"r\",\"format\":\"hex\",\"index\":240,\"name\":\"testu32\",\"type\":\"u32\",\"value\":\"f06fb09\"}");
    }
}


TEST(kwbconfiguration_ModuleRegister, ModuleRegister_toJsonNOK)
{
    std::string s;

    {
        ModuleRegisterJson r(1, RegType::eINVALID, eREG_ACCESS_READ | eREG_ACCESS_WRITE, "def", ValueFormat::eDECIMAL, 547);
        EXPECT_THROW(s = r.toJsonStr(), LogicError);
    }

    {
        ModuleRegisterJson r(1, RegType::eU16, eREG_ACCESS_READ | eREG_ACCESS_WRITE, "def", ValueFormat::eUNSPECIFIED, 547);
        EXPECT_THROW(s = r.toJsonStr(), LogicError);
    }
}

/** @} */
