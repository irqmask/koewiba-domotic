# KWB Router
cmake_minimum_required (VERSION 2.6)

include_directories (
    ../../include
    ../../shared
    ../include
    ../libsystem
    ../shared
    ../3rd_party/jansson-2.6/src
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

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/kwbrouter_version.h
     "const char* PGM_VERSION(\"${VERSION}\");\n")

set (kwbrouter_SRCS
    kwbrouter_main.c
    ../shared/bus_phy_os.c
    ../shared/clock_os.c
    ../shared/ioloop.c
    ../shared/message_bus.c
    ../shared/message_socket.c
    ../../shared/bus_transport.c
    ../../shared/crc16.c
)

# executable
# ---------------------------------------------------------
if (WIN32)
    add_executable ( kwbrouter WIN32
        ${kwbrouter_SRCS} )
    install ( TARGETS kwbrouter
              RUNTIME DESTINATION . )
else (WIN32)
    add_executable ( kwbrouter
        ${kwbrouter_SRCS} )
    install ( TARGETS kwbrouter
              RUNTIME DESTINATION bin )
endif (WIN32)

# Link libraries
# ---------------------------------------------------------
target_link_libraries (kwbrouter libsystem jansson)
if (WIN32)

else (WIN32)
    target_link_libraries (kwbrouter pthread)
    target_link_libraries (kwbrouter safec-1.0)
endif (WIN32)

add_dependencies (kwbrouter libsystem)
