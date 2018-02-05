# KWB Router
cmake_minimum_required (VERSION 2.6)

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

# system dependent definitions
# ---------------------------------------------------------
if (WIN32)
    # TODO implement missing secure functions for linux,
    # change implementation to secure functions and remove
    # this warning suppression.
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif (WIN32)

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/kwbrouter_version.h
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

set (os_shared_SRCS
    ../shared/ioloop.h
    ../shared/log.h
    ../shared/message.h
    ../shared/message_serial.h
    ../shared/message_socket.h

    ../shared/ioloop.c
    ../shared/log.c
    ../shared/message.c
    ../shared/message_serial.c
    ../shared/message_socket.c
)

set (kwbrouter_SRCS
    router.h
    routeconnection.h
    rconnserial.h
    rconnsocketclient.h
    socketserver.h

    router.cpp
    routeconnection.cpp
    rconnserial.cpp
    rconnsocketclient.cpp
    socketserver.cpp

    kwbrouter_main.cpp
)

# source groups
# ---------------------------------------------------------
source_group(\\ FILES ${kwbrouter_SRCS})
source_group(include FILES ${include_SRCS})
source_group(shared FILES ${shared_SRCS})
source_group(os\\include FILES ${os_include_SRCS})
source_group(os\\shared FILES ${os_shared_SRCS})

# executable
# ---------------------------------------------------------
if (WIN32)
    add_executable ( kwbrouter
        ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${os_shared_SRCS} ${kwbrouter_SRCS} )
    install ( TARGETS kwbrouter
              RUNTIME DESTINATION . )
else (WIN32)
    add_executable ( kwbrouter
        ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${os_shared_SRCS} ${kwbrouter_SRCS} )
    install ( TARGETS kwbrouter
              RUNTIME DESTINATION bin )
endif (WIN32)

set_property(TARGET kwbrouter PROPERTY CXX_STANDARD 11)

# Link libraries
# ---------------------------------------------------------
target_link_libraries (kwbrouter libsystem)

if (WIN32)

else (WIN32)
    target_link_libraries (kwbrouter pthread)
    target_link_libraries (kwbrouter safec-1.0)
endif (WIN32)

add_dependencies (kwbrouter libsystem)
