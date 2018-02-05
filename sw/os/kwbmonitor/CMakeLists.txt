# KWB Monitor
cmake_minimum_required (VERSION 2.8)

include_directories (
    ../../include
    ../../shared
    ../include
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

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/kwbmonitor_version.h
    "const char* PGM_VERSION(\"${VERSION}\");\n")

# system dependent definitions
# ---------------------------------------------------------
if (WIN32)
    # TODO implement missing secure functions for linux,
    # change implementation to secure functions and remove
    # this warning suppression.
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif (WIN32)

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

    ../../shared/crc16.c
)

set (os_include_SRCS
    ../include/error_codes.h
)

set (os_shared_SRCS
    ../shared/log.h
    ../shared/message.h
    ../shared/vos.h

    ../shared/vos.c
)

set (kwbmonitor_SRCS
    kwbmonitor.h

    kwbmonitor_main.c
    kwbmonitor.c
)

# source groups
# ---------------------------------------------------------
source_group(\\ FILES ${kwbmonitor_SRCS})
source_group(include FILES ${include_SRCS})
source_group(shared FILES ${shared_SRCS})
source_group(os\\include FILES ${os_include_SRCS})
source_group(os\\shared FILES ${os_shared_SRCS})

# executable
# ---------------------------------------------------------
if (WIN32)
    add_executable ( kwbmonitor ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${os_shared_SRCS} ${kwbmonitor_SRCS} )
    install ( TARGETS kwbmonitor
              RUNTIME DESTINATION . )
else (WIN32)
    add_executable ( kwbmonitor ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${os_shared_SRCS} ${kwbmonitor_SRCS} )
    install ( TARGETS kwbmonitor
              RUNTIME DESTINATION bin )
endif (WIN32)

set_property(TARGET kwbmonitor PROPERTY CXX_STANDARD 11)

# Link libraries
# ---------------------------------------------------------
target_link_libraries (kwbmonitor libsystem)
if (WIN32)

else (WIN32)
    target_link_libraries (kwbmonitor pthread)
    target_link_libraries (kwbmonitor safec-1.0)
endif (WIN32)

add_dependencies (kwbmonitor libsystem)
