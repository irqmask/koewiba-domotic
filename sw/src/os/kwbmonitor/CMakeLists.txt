# KWB Monitor
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

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/kwbmonitor_version.h
     "const char* PGM_VERSION(\"${VERSION}\");\n")

set (kwbmonitor_SRCS
     kwbmonitor_main.c
     kwbmonitor.c
     ../shared/ioloop.c
     ../shared/vos.c
     ../../shared/crc16.c
)

# executable
# ---------------------------------------------------------
if (WIN32)
    add_executable ( kwbmonitor WIN32
        ${kwbmonitor_SRCS} )
    install ( TARGETS kwbmonitor
              RUNTIME DESTINATION . )
else (WIN32)
    add_executable ( kwbmonitor
        ${kwbmonitor_SRCS} )
    install ( TARGETS kwbmonitor
              RUNTIME DESTINATION bin )
endif (WIN32)

# Link libraries
# ---------------------------------------------------------
target_link_libraries (kwbmonitor libsystem)
if (WIN32)

else (WIN32)
    target_link_libraries (kwbmonitor pthread)
    target_link_libraries (kwbmonitor safec-1.0)
endif (WIN32)

add_dependencies (kwbmonitor libsystem)
