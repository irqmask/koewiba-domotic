# KWB Configuration
cmake_minimum_required (VERSION 3.0)

include_directories (
    ../../include
    ../../shared
    ../include
    ../libkwb
    ../libsystem
    ../shared
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

# system dependent definitions
# ---------------------------------------------------------
if (WIN32)
    # TODO implement missing secure functions for linux,
    # change implementation to secure functions and remove
    # this warning suppression.
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif (WIN32)

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/kwbconfiguration_version.h
     "const char* PGM_VERSION(\"${VERSION}\");\n")

# header and source files
# ---------------------------------------------------------
set (include_SRCS
    ../../include/cmddef_common.h
    ../../include/moddef_common.h
    ../../include/prjconf.h
    ../../include/prjtypes.h
)

set (shared_SRCS
    ../../shared/bus.h
    ../../shared/crc16.h

    ../../shared/crc16.c
)

set (os_include_SRCS
    ../include/error_codes.h
)

set (kwbconfiguration_SRCS
    Action.h
    ActionRequest.h
    ActionWithResponse.h
    ActionQueryModules.h
    ActionReadRegister.h
    ActionWriteRegister.h
    Application.h
    kwbconfiguration_version.h
    MsgBroker.h
    MsgEndpoint.h
    MsgEndpointSerial.h
    UIConsole.h

    Action.cpp
    ActionRequest.cpp
    ActionWithResponse.cpp
    ActionQueryModules.cpp
    ActionReadRegister.cpp
    ActionWriteRegister.cpp
    Application.cpp
    kwbconfiguration_main.cpp
    MsgBroker.cpp
    MsgEndpoint.cpp
    MsgEndpointSerial.cpp
    UIConsole.cpp
)

# source groups
# ---------------------------------------------------------
source_group(\\ FILES ${kwbconfiguration_SRCS})
source_group(include FILES ${include_SRCS})
source_group(shared FILES ${shared_SRCS})
source_group(os\\include FILES ${os_include_SRCS})
source_group(os\\shared FILES ${os_shared_SRCS})

# executable
# ---------------------------------------------------------
if (WIN32)
    add_executable ( kwbconfiguration ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${kwbconfiguration_SRCS} )
    install ( TARGETS kwbconfiguration
              RUNTIME DESTINATION . )
else (WIN32)
    add_executable ( kwbconfiguration ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${kwbconfiguration_SRCS} )
    install ( TARGETS kwbconfiguration
              RUNTIME DESTINATION bin )
endif (WIN32)

set_property(TARGET kwbconfiguration PROPERTY CXX_STANDARD 14)

# Link libraries
# ---------------------------------------------------------
target_link_libraries (kwbconfiguration libkwb)
target_link_libraries (kwbconfiguration libsystem)
if (WIN32)

else (WIN32)
    target_link_libraries (kwbconfiguration pthread)
    target_link_libraries (kwbconfiguration safec-1.0)
endif (WIN32)

add_dependencies (kwbconfiguration libsystem)
