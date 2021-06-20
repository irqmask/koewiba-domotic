# KWB MQTT Gateway
cmake_minimum_required (VERSION 3.0)

include_directories (
    ../../include
    ../../shared
    ../include
    ../libsystem
    ../libkwb
    ../shared
    ../3rd_party/mosquitto
    ${CMAKE_CURRENT_BINARY_DIR}
)

# system dependent include directories
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

# system dependent link directories
# ---------------------------------------------------------
if (WIN32)
    link_directories (
        ../3rd_party/mosquitto
        ../../3rd_party/mosquitto
    )
endif (WIN32)

# system dependent definitions
# ---------------------------------------------------------
if (WIN32)
    # TODO implement missing secure functions for linux,
    # change implementation to secure functions and remove
    # this warning suppression.
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif (WIN32)

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/kwbmqttgateway_version.h
     "const char* PGM_VERSION(\"${VERSION}\");\n")

# header and source files
# ---------------------------------------------------------
set (include_SRCS
    ../../include/cmddef_common.h
    ../../include/moddef_common.h
    ../../include/prjconf.h
    ../../include/prjtypes.h
)

set (os_include_SRCS
    ../include/error_codes.h
)

set (shared_SRCS
    ../../shared/crc16.h

    ../../shared/crc16.c
)


set (kwbmqttgateway_SRCS
    kwbmqttgateway.h

    kwbmqttgateway_main.cpp
    msg2mqtt.cpp
    mosquitto_ioloop_glue.cpp
    mqtt2msg.cpp
)

# source groups
# ---------------------------------------------------------
source_group(\\ FILES ${kwbmqttgateway_SRCS})
source_group(include FILES ${include_SRCS})
source_group(shared FILES ${shared_SRCS})
source_group(os\\include FILES ${os_include_SRCS})
source_group(os\\shared FILES ${os_shared_SRCS})

# executable
# ---------------------------------------------------------
if (WIN32)
    add_executable ( kwbmqttgateway
        ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${kwbmqttgateway_SRCS} )
    install ( TARGETS kwbmqttgateway
              RUNTIME DESTINATION . )
else (WIN32)
    add_executable ( kwbmqttgateway
        ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${kwbmqttgateway_SRCS} )
    install ( TARGETS kwbmqttgateway
              RUNTIME DESTINATION bin )
endif (WIN32)

set_property(TARGET kwbmqttgateway PROPERTY CXX_STANDARD 11)

# Link libraries
# ---------------------------------------------------------
target_link_libraries (kwbmqttgateway libkwb)
target_link_libraries (kwbmqttgateway libsystem)
target_link_libraries (kwbmqttgateway mosquitto)

if (WIN32)
else (WIN32)
    target_link_libraries (kwbmqttgateway pthread)
    target_link_libraries (kwbmqttgateway safec-1.0)
endif (WIN32)

add_dependencies (kwbmqttgateway libsystem)
