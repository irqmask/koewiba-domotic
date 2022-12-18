cmake_minimum_required (VERSION 3.0)
project (KoeWiBa)

## folder settings ############################################################

set (CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(ExternalProject)
include(cmake/gitversion.cmake)

## thirdparty projects ########################################################

#### googletest ###############################################################
ExternalProject_Add(googletest
                    GIT_REPOSITORY      https://github.com/google/googletest.git
                    GIT_TAG             release-1.11.0
                    SOURCE_DIR          3rd_party/googletest
                    INSTALL_COMMAND     "")
ExternalProject_Get_Property(googletest SOURCE_DIR)
set(googletest_SOURCE_DIR ${SOURCE_DIR})
set(googletest_INCLUDE_DIR ${googletest_SOURCE_DIR}/googletest/include/)
ExternalProject_Get_Property(googletest BINARY_DIR)
set(googletest_LIBRARY_DIR ${BINARY_DIR}/lib)
message("googletest source dir = ${googletest_SOURCE_DIR}")
message("googletest include dir = ${googletest_INCLUDE_DIR}")
message("googletest lib dir = ${googletest_LIBRARY_DIR}")

## sub-projects ###############################################################

add_subdirectory (libkwb)
add_subdirectory (libsystem)

add_subdirectory (kwbconfiguration)
add_subdirectory (kwbfirmware)
add_subdirectory (kwbmonitor)
add_subdirectory (kwbmqttgateway)
add_subdirectory (kwbrouter)
add_subdirectory (kwbtest)
add_subdirectory (unittests)
