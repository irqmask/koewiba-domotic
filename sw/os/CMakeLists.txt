cmake_minimum_required (VERSION 2.8)
project (KoeWiBa)

set (CMAKE_BINARY_DIR ../../bin)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory (libsystem)

add_subdirectory (kwbfirmware)
add_subdirectory (kwbmonitor)
add_subdirectory (kwbrouter)
add_subdirectory (kwbscheduler)
add_subdirectory (kwbtest)
if (WIN32)
add_subdirectory (unittests)
endif (WIN32)
add_subdirectory (vbusd)
