# unittests
cmake_minimum_required (VERSION 3.0)

cmake_policy(SET CMP0015 NEW)

include_directories (
    ../../include
    ../../shared
    ../include
    ../libkwb
    ../libsystem
    ../shared
    ${googletest_INCLUDE_DIR}
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
    ${googletest_LIBRARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

# system dependant link directories
# ---------------------------------------------------------
if (WIN32)
endif (WIN32)

# system dependent definitions
# ---------------------------------------------------------
if (WIN32)
    # TODO implement missing secure functions for linux,
    # change implementation to secure functions and remove
    # this warning suppression.
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
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

set (unittests_SRCS
    test_json.cpp
    test_libsystem_threads.cpp
    test_libsystem_time.cpp
    
    test_libkwb_connection.cpp
    test_libkwb_connection_serial.cpp
    test_libkwb_connection_socket.cpp

    test_configuration_register.cpp
    test_configuration_module.cpp
    
    unittests_main.cpp
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
target_link_libraries (unittests libkwb)
target_link_libraries (unittests libsystem)
target_link_libraries (unittests gtest)

if (WIN32)
else (WIN32)
    target_link_libraries (unittests pthread)
    target_link_libraries (unittests safec-1.0)
endif (WIN32)

add_dependencies (unittests libsystem)

