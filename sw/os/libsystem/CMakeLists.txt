# libsystem
cmake_minimum_required (VERSION 2.6)

include_directories (
    .
    ../include
    ../../include
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


# list of source files
# ---------------------------------------------------------
set (include_SRCS
    ../../include/prjconf.h
    ../../include/prjtypes.h
)

set (libsystem_SRCS
    system.h
    sysconsole.h
    sysgetopt.h
    sysserial.h
    syssocket.h
    systhread.h
    systime.h

    sysconsole.c
    sysgetopt.c
    sysserial.c
    syssocket.c
    systime.c
)

# source groups
# ---------------------------------------------------------
source_group(\\ FILES ${libsystem_SRCS})
source_group(include FILES ${include_SRCS})

# library
# ---------------------------------------------------------
add_library (libsystem ${include_SRCS} ${libsystem_SRCS})

set_property(TARGET libsystem PROPERTY CXX_STANDARD 11)
