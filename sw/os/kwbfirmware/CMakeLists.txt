# KWB Firmware
cmake_minimum_required (VERSION 2.6)

include_directories (
    ../../include
    ../../shared
    ../include
    ../libsystem
    ../shared
    ${CMAKE_CURRENT_BINARY_DIR}
)

# system dependant include directories
# ---------------------------------------------------------
if (WIN32)

else (WIN32)
    include_directories (
        ../3rd_party/libsafec/include
    )
endif (WIN32)

link_directories (
    ${CMAKE_CURRENT_BINARY_DIR}
)

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/kwbfirmware_version.h
     "const char* PGM_VERSION(\"${VERSION}\");\n")

set (kwbfirmware_SRCS
    firmwareupdate.c
    ihex.c
    kwbfirmware_main.c
    ../shared/ioloop.c
    ../shared/log.c
    ../shared/message.c
    ../shared/message_serial.c
    ../../shared/crc16.c
)

# executable
# ---------------------------------------------------------
if (WIN32)
    add_executable ( kwbfirmware
        ${kwbfirmware_SRCS} )
    install ( TARGETS kwbfirmware
              RUNTIME DESTINATION . )
else (WIN32)
    add_executable ( kwbfirmware
        ${kwbfirmware_SRCS} )
    install ( TARGETS kwbfirmware
              RUNTIME DESTINATION bin )
endif (WIN32)

# Link libraries
# ---------------------------------------------------------
target_link_libraries (kwbfirmware libsystem)
if (WIN32)

else (WIN32)
    target_link_libraries (kwbfirmware pthread)
    target_link_libraries (kwbfirmware safec-1.0)
endif (WIN32)

add_dependencies (kwbfirmware libsystem)
