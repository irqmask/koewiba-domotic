# libsystem
cmake_minimum_required (VERSION 2.6)

include_directories (
    .
    ../include
    ../../include
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


# list of source files
# ---------------------------------------------------------
set (libsystem_SRCS
    sysconsole.c
    sysserial.c
    syssocket.c
    systime.c
)

# library
# ---------------------------------------------------------
add_library (libsystem ${libsystem_SRCS})
