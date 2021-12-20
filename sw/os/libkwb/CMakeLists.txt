# libkwb
cmake_minimum_required (VERSION 3.0)

include_directories (
    .
    ../include
    ../libsystem
    ../../include
    ../../shared
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

# system dependent definitions
# ---------------------------------------------------------
if (WIN32)
    # TODO implement missing secure functions for linux,
    # change implementation to secure functions and remove
    # this warning suppression.
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif (WIN32)

# list of source files
# ---------------------------------------------------------
set (include_SRCS
    ../../include/prjconf.h
    ../../include/prjtypes.h
)

set (libkwb_SRCS
    connection.h
    connection_serial.h
    connection_socket.h
    exceptions.h
    ioloop.h
    log.h
    message.h
    vos.h

    connection.cpp
    connection_serial.cpp
    connection_socket.cpp
    exceptions.cpp
    ioloop.c
    log.c
    message.c
    vos.c
)

# source groups
# ---------------------------------------------------------
source_group(\\ FILES ${libkwb_SRCS})
source_group(include FILES ${include_SRCS})

# library
# ---------------------------------------------------------
add_library (libkwb ${include_SRCS} ${libkwb_SRCS})
