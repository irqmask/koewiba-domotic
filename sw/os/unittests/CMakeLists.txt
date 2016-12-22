# unittests
cmake_minimum_required (VERSION 2.8.1)

cmake_policy(SET CMP0015 NEW)

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
    include_directories (
        ../3rd_party
    )
else (WIN32)
    include_directories (
        ../3rd_party/libsafec/include
    )
endif (WIN32)

link_directories (
    ${CMAKE_CURRENT_BINARY_DIR}
)

# system dependant include directories
# ---------------------------------------------------------
if (WIN32)
    link_directories (
        ../3rd_party/CUnit
    )
endif (WIN32)

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/kwbtest_version.h
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
)

set (os_include_SRCS
    ../include/error_codes.h
)

set (os_shared_SRCS
    ../shared/log.h
    ../shared/message.h

    ../shared/ioloop.c
    ../shared/log.c
    ../shared/message_socket.c
)

set (unittests_SRCS
    tests.c
    test_libsystem_threads.c
    test_libsystem_time.c
    unittests_main.c
)

# source groups
# ---------------------------------------------------------
source_group(\\ FILES ${unittests_SRCS})
source_group(include FILES ${include_SRCS})
source_group(shared FILES ${shared_SRCS})
source_group(os\\include FILES ${os_include_SRCS})
source_group(os\\shared FILES ${os_shared_SRCS})

# executable
# ---------------------------------------------------------
if (WIN32)
    add_executable ( unittests ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${os_shared_SRCS} ${unittests_SRCS} )
    install ( TARGETS unittests
              RUNTIME DESTINATION . )
else (WIN32)
    add_executable ( unittests ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${os_shared_SRCS} ${unittests_SRCS} )
    install ( TARGETS unittests
              RUNTIME DESTINATION bin )
endif (WIN32)

set_property(TARGET unittests PROPERTY CXX_STANDARD 11)

# Link libraries
# ---------------------------------------------------------
target_link_libraries (unittests libsystem)
if (WIN32)
    target_link_libraries (unittests cunit)
else (WIN32)
    target_link_libraries (unittests pthread)
    target_link_libraries (unittests safec-1.0)
endif (WIN32)

add_dependencies (unittests libsystem)

