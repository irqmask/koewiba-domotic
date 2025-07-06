cmake_minimum_required (VERSION 3.16)
project (KoeWiBa)

## folder settings ############################################################

set (CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)
include(cmake/gitversion.cmake)

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
