cmake_minimum_required (VERSION 2.6)
project (KoeWiBa)

#set (CMAKE_BINARY_DIR ../../../bin/os)
#set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory (libsystem)

add_subdirectory (kwbmonitor)
add_subdirectory (kwbrouter)
add_subdirectory (kwbscheduler)
add_subdirectory (kwbtest)
add_subdirectory (vbusd)
