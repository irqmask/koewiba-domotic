/**
 * @addtogroup UNITTESTS
 *
 * @{
 * @file    test_json.cpp
 * @brief   Playground for json serializing and deserializing.
 *
 * @author  Christian Verhalen
 *///---------------------------------------------------------------------------
/*
 * Copyright (C) 2022  christian <irqmask@gmx.de>
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

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include "exceptions.h"

using namespace nlohmann;


/// Unittests for the serial connection class
class JsonRegsTest : public ::testing::Test
{
protected:
    /// SetUp is called before each test is started
    void SetUp() override
    {
    }

    /// TearDown is called after each test is finished
    void TearDown() override
    {
    }
};


/// Test to decode
TEST_F(JsonRegsTest, GetType)
{
    std::string jsonstr = "{\"access\":\"rw\",\"index\":\"28\",\"name\":\"TimeUp1_Minute\",\"type\":\"U8\"}";
    
    json j = json::parse(jsonstr);

    ASSERT_TRUE(j.contains("access"));
    ASSERT_FALSE(j.contains("bogus"));
    std::string s;
    try {
        s = j["access"];
    }
    catch (json::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    ASSERT_STREQ("rw", s.c_str());

    try {
        s = j["bogus"];
    }
    catch (json::exception &e) {
        std::cerr << e.what() << std::endl;
    }

}

// example enum type declaration
enum class TestType
{
    eREGInvalid = -1,
    eREGU8 = 0,
    eREGU16,
    eREGU32
};

// map RegType values to JSON as strings
NLOHMANN_JSON_SERIALIZE_ENUM( TestType, {
    {TestType::eREGInvalid, nullptr},
    {TestType::eREGU8, "regu8"},
    {TestType::eREGU8, "REGU8"},
    {TestType::eREGU16, "regu16"},
    {TestType::eREGU16, "REGU16"},
    {TestType::eREGU32, "regu32"},
    {TestType::eREGU32, "REGU32"},
})

/// Test to decode enums
TEST_F(JsonRegsTest, GetEnum)
{
    json j = json::parse("{\"type\":\"U8\"}");
    TestType rt = j["type"].get<TestType>();
    ASSERT_EQ(TestType::eREGInvalid, rt);

    j = json::parse("{\"bogus\":\"U8\"}");
    rt = j["type"].get<TestType>();
    ASSERT_EQ(TestType::eREGInvalid, rt);


    j = json::parse("{\"type\":\"REGU8\"}");
    rt = j["type"].get<TestType>();
    ASSERT_EQ(TestType::eREGU8, rt);

    j = json::parse("{\"type\":\"regu8\"}");
    rt = j["type"].get<TestType>();
    ASSERT_EQ(TestType::eREGU8, rt);
}
