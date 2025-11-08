# KWB Test
cmake_minimum_required (VERSION 3.16)

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

set (kwbtest_SRCS
    kwbtest_main.cpp
)

# source groups
# ---------------------------------------------------------
source_group(\\ FILES ${kwbscheduler_SRCS})
source_group(include FILES ${include_SRCS})
source_group(shared FILES ${shared_SRCS})
source_group(os\\include FILES ${os_include_SRCS})
source_group(os\\shared FILES ${os_shared_SRCS})

# executable
# ---------------------------------------------------------
if (WIN32)
    add_executable ( kwbtest ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${kwbtest_SRCS} )
    # install ( TARGETS kwbtest RUNTIME DESTINATION . )
else (WIN32)
    add_executable ( kwbtest ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${kwbtest_SRCS} )
    # install ( TARGETS kwbtest RUNTIME DESTINATION bin )
endif (WIN32)

set_property(TARGET kwbtest PROPERTY CXX_STANDARD 11)

# Link libraries
# ---------------------------------------------------------
target_link_libraries (kwbtest libkwb)
target_link_libraries (kwbtest libsystem)
if (WIN32)

else (WIN32)
    target_link_libraries (kwbtest pthread)
    target_link_libraries (kwbtest safec-1.0)
endif (WIN32)

add_dependencies (kwbtest libsystem)

