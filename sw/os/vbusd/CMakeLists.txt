# vbusd
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

file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/vbusd_version.h
     "const char* PGM_VERSION(\"${VERSION}\");\n")

# header and source files
# ---------------------------------------------------------
set (include_SRCS
    ../../include/prjconf.h
    ../../include/prjtypes.h
)

set (shared_SRCS
)

set (os_include_SRCS
    ../include/error_codes.h
)

set (os_shared_SRCS
    ../shared/ioloop.h
    ../shared/log.h

    ../shared/ioloop.c
    ../shared/log.c
)

set (vbusd_SRCS
    vbusd_main.c
    ../shared/ioloop.c
)

# source groups
# ---------------------------------------------------------
source_group(\\ FILES ${vbusd_SRCS})
source_group(include FILES ${include_SRCS})
source_group(shared FILES ${shared_SRCS})
source_group(os\\include FILES ${os_include_SRCS})
source_group(os\\shared FILES ${os_shared_SRCS})

# executable
# ---------------------------------------------------------
if (WIN32)
    add_executable ( vbusd ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${os_shared_SRCS} ${vbusd_SRCS} )
    install ( TARGETS vbusd
              RUNTIME DESTINATION . )
else (WIN32)
    add_executable ( vbusd ${include_SRCS} ${shared_SRCS} ${os_include_SRCS} ${os_shared_SRCS} ${vbusd_SRCS} )
    install ( TARGETS vbusd
              RUNTIME DESTINATION bin )
endif (WIN32)

# Link libraries
# ---------------------------------------------------------
target_link_libraries (vbusd libsystem)
if (WIN32)

else (WIN32)
    target_link_libraries (vbusd safec-1.0)
endif (WIN32)

add_dependencies (vbusd libsystem)
